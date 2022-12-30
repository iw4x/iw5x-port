#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include "script_error.hpp"
#include "script_loading.hpp"

#include "module/log_file.hpp"
#include "module/scripting.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

using namespace utils::string;

namespace gsc
{
	namespace
	{
		std::array<const char*, game::native::VAR_TOTAL_COUNT> var_typename =
		{
			"undefined",
			"object",
			"string",
			"localized string",
			"vector",
			"float",
			"int",
			"codepos",
			"precodepos",
			"function",
			"builtin function",
			"builtin method",
			"stack",
			"animation",
			"pre animation",
			"thread",
			"thread",
			"thread",
			"thread",
			"struct",
			"removed entity",
			"entity",
			"array",
			"removed thread",
			"<free>",
			"thread list",
			"endon list",
		};

		utils::hook::detour scr_emit_function_hook;

		unsigned int current_filename = 0;

		std::string unknown_function_error;

		char gsc_error_msg[1024];

		void scr_emit_function_stub(unsigned int filename, unsigned int thread_name, char* code_pos)
		{
			current_filename = filename;
			scr_emit_function_hook.invoke<void>(filename, thread_name, code_pos);
		}

		std::string get_filename_name()
		{
			const auto filename_str = game::native::SL_ConvertToString(current_filename);
			const auto id = std::strtol(filename_str, nullptr, 10);
			if (!id)
			{
				return filename_str;
			}

			return scripting::get_token(id);
		}

		void get_unknown_function_error(const char* code_pos)
		{
			const auto function = find_function(code_pos);
			if (function.has_value())
			{
				const auto& pos = function.value();
				unknown_function_error = std::format(
					"while processing function '{}' in script '{}':\nunknown script '{}'", pos.first, pos.second, scripting::current_file
				);
			}
			else
			{
				unknown_function_error = std::format("unknown script '{}'.gsc", scripting::current_file);
			}
		}

		void get_unknown_function_error(unsigned int thread_name)
		{
			const auto filename = get_filename_name();
			const auto name = scripting::get_token(thread_name);

			unknown_function_error = std::format(
				"while processing script '{}':\nunknown function '{}::{}'", scripting::current_file, filename, name
			);
		}

		void compile_error_stub(const char* code_pos, [[maybe_unused]] const char* msg)
		{
			get_unknown_function_error(code_pos);
			game::native::Com_Error(game::native::ERR_DROP, "script link error\n%s", unknown_function_error.data());
		}

		unsigned int find_variable_stub(unsigned int parent_id, unsigned int thread_name)
		{
			const auto res = utils::hook::invoke<unsigned int>(SELECT_VALUE(0x4C4E70, 0x5651F0), parent_id, thread_name);
			if (!res)
			{
				get_unknown_function_error(thread_name);
				game::native::Com_Error(game::native::ERR_DROP, "script link error\n%s", unknown_function_error.data());
			}

			return res;
		}

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				return {xsk::gsc::iw5::resolver::opcode_name(opcode)};
			}
			catch (...)
			{
				return {};
			}
		}

		void builtin_call_error(const std::string& error)
		{
			const auto pos = game::native::scr_function_stack->pos;
			const auto function_id = *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::size_t>(pos - 2));

			if (function_id > (scr_func_max_id - 1))
			{
				log_file::info("in call to builtin method \"%s\"%s\n", xsk::gsc::iw5::resolver::method_name(function_id).data(), error.data());
			}
			else
			{
				log_file::info("in call to builtin function \"%s\"%s\n", xsk::gsc::iw5::resolver::function_name(function_id).data(), error.data());
			}
		}

		void vm_error_stub(int mark_pos)
		{
			log_file::info("******* script runtime error ********\n");

			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(SELECT_VALUE(0x1BF6928, 0x20B8E28));
			const auto error = (*gsc_error_msg) ? std::format(": {}\n", gsc_error_msg) : std::string();

			if ((opcode_id >= 0x84 && opcode_id <= 0x8A) || (opcode_id >= 0x8B && opcode_id <= 0x91))
			{
				builtin_call_error(error);
			}
			else
			{
				const auto opcode = get_opcode_name(opcode_id);
				if (opcode.has_value())
				{
					log_file::info("while processing instruction %s%s\n", opcode.value().data(), error.data());
				}
				else
				{
					log_file::info("while processing instruction 0x%X%s\n", opcode_id, error.data());
				}
			}

			ZeroMemory(gsc_error_msg, sizeof(gsc_error_msg));

			log_file::info("************************************\n");

			game::native::LargeLocalResetToMark(mark_pos);
		}

		void scr_fx_param_error([[maybe_unused]] int param_index, const char* error_string, int fx_id)
		{
			assert(error_string);

			char fx_name[1024]{};

			if (fx_id)
			{
				const auto index = SELECT_VALUE(game::native::sp::CS_EFFECT_NAMES, game::native::mp::CS_EFFECT_NAMES);
				game::native::SV_GetConfigstring(fx_id + index, fx_name, sizeof(fx_name));
			}
			else
			{
				strncpy_s(fx_name, "not successfully loaded", _TRUNCATE);
			}

			scr_error(va("%s (effect = %s)\n", error_string, fx_name));
		}

		void gscr_cast_int()
		{
			switch (scr_get_type(0))
			{
			case game::native::VAR_STRING:
				game::native::Scr_AddInt(std::atoi(game::native::Scr_GetString(0)));
				break;
			case game::native::VAR_FLOAT:
				game::native::Scr_AddInt(static_cast<int>(scr_get_float(0)));
				break;
			case game::native::VAR_INTEGER:
				game::native::Scr_AddInt(scr_get_int(0));
				break;
			default:
				scr_error(va("cannot cast %s to int", scr_get_type_name(0)));
				break;
			}
		}

		void gscr_cast_float()
		{
			switch (scr_get_type(0))
			{
			case game::native::VAR_STRING:
				game::native::Scr_AddFloat(static_cast<float>(std::atof(game::native::Scr_GetString(0))));
				break;
			case game::native::VAR_FLOAT:
				game::native::Scr_AddFloat(scr_get_float(0));
				break;
			case game::native::VAR_INTEGER:
				game::native::Scr_AddFloat(static_cast<float>(scr_get_int(0)));
				break;
			default:
				scr_error(va("cannot cast %s to float", scr_get_type_name(0)));
				break;
			}
		}

		void assert_cmd()
		{
			if (!scr_get_int(0))
			{
				scr_error("Assert fail");
			}
		}

		void assert_ex_cmd()
		{
			if (!scr_get_int(0))
			{
				scr_error(va("Assert fail: %s", game::native::Scr_GetString(1)));
			}
		}

		void assert_msg_cmd()
		{
			scr_error(va("Assert fail: %s", game::native::Scr_GetString(0)));
		}
	}

	std::optional<std::pair<std::string, std::string>> find_function(const char* pos)
	{
		for (const auto& file : scripting::script_function_table_sort)
		{
			for (auto i = file.second.begin(); i != file.second.end() && std::next(i) != file.second.end(); ++i)
			{
				const auto next = std::next(i);
				if (pos >= i->second && pos < next->second)
				{
					return {std::make_pair(i->first, file.first)};
				}
			}
		}

		return {};
	}

	unsigned int scr_get_object(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (value->type == game::native::VAR_POINTER)
			{
				return value->u.pointerValue;
			}

			scr_error(va("Type %s is not an object", var_typename[value->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	unsigned int scr_get_const_string(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (game::native::Scr_CastString(value))
			{
				assert(value->type == game::native::VAR_STRING);
				return value->u.stringValue;
			}

			game::native::Scr_ErrorInternal();
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	unsigned int scr_get_const_istring(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (value->type == game::native::VAR_ISTRING)
			{
				return value->u.stringValue;
			}

			scr_error(va("Type %s is not a localized string", var_typename[value->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	void scr_validate_localized_string_ref([[maybe_unused]] int parm_index, const char* token, int token_len)
	{
		assert(token);
		assert(token_len >= 0);

		if (token_len < 2)
		{
			return;
		}

		for (auto char_iter = 0; char_iter < token_len; ++char_iter)
		{
			if (!std::isalnum(static_cast<unsigned char>(token[char_iter])) && token[char_iter] != '_')
			{
				scr_error(va("Illegal localized string reference: %s must contain only alpha-numeric characters and underscores", token));
			}
		}
	}

	void scr_get_vector(unsigned int index, float* vector_value)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (value->type == game::native::VAR_VECTOR)
			{
				std::memcpy(vector_value, value->u.vectorValue, sizeof(game::native::vec3_t));
				return;
			}

			scr_error(va("Type %s is not a vector", var_typename[value->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
	}

	int scr_get_int(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (value->type == game::native::VAR_INTEGER)
			{
				return value->u.intValue;
			}

			scr_error(va("Type %s is not an int", var_typename[value->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	float scr_get_float(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			auto* value = game::native::scr_VmPub->top - index;
			if (value->type == game::native::VAR_FLOAT)
			{
				return value->u.floatValue;
			}

			if (value->type == game::native::VAR_INTEGER)
			{
				return static_cast<float>(value->u.intValue);
			}

			scr_error(va("Type %s is not a float", var_typename[value->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0.0f;
	}

	int scr_get_pointer_type(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			if ((game::native::scr_VmPub->top - index)->type == game::native::VAR_POINTER)
			{
				return static_cast<int>(game::native::GetObjectType((game::native::scr_VmPub->top - index)->u.pointerValue));
			}

			scr_error(va("Type %s is not an object", var_typename[(game::native::scr_VmPub->top - index)->type]));
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	int scr_get_type(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			return (game::native::scr_VmPub->top - index)->type;
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return 0;
	}

	void scr_error(const char* error)
	{
		strncpy_s(gsc_error_msg, error, _TRUNCATE);
		game::native::Scr_ErrorInternal();
	}

	const char* scr_get_type_name(unsigned int index)
	{
		if (index < game::native::scr_VmPub->outparamcount)
		{
			return var_typename[(game::native::scr_VmPub->top - index)->type];
		}

		scr_error(va("Parameter %u does not exist", index + 1));
		return nullptr;
	}

	class error final : public module
	{
	public:
		void post_start() override
		{
			ZeroMemory(gsc_error_msg, sizeof(gsc_error_msg));
		}

		void post_load() override
		{
			scr_emit_function_hook.create(SELECT_VALUE(0x40DCB0, 0x561400), &scr_emit_function_stub);

			utils::hook(SELECT_VALUE(0x60DABA, 0x5615FA), &compile_error_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x60DAD1, 0x561611), &compile_error_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x40DCFA, 0x56144A), &find_variable_stub, HOOK_CALL).install()->quick();

			utils::hook(SELECT_VALUE(0x612BFC, 0x56D87D), &vm_error_stub, HOOK_CALL).install()->quick(); // LargeLocalResetToMark

			// Restore basic error messages for commonly used scr functions
			utils::hook(SELECT_VALUE(0x52F730, 0x56A630), &scr_get_object, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x40FDE0, 0x56A200), &scr_get_const_string, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x4FD700, 0x56A420), &scr_get_const_istring, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x536FAC, 0x523FAE), &scr_validate_localized_string_ref, HOOK_CALL).install()->quick(); // Scr_ConstructMessageString
			utils::hook(SELECT_VALUE(0x452E90, 0x56A4D0), &scr_get_vector, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x51B520, 0x56A010), &scr_get_int, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x4D8B50, 0x56A190), &scr_get_float, HOOK_JUMP).install()->quick();

			utils::hook(SELECT_VALUE(0x4C02D0, 0x51F230), &scr_fx_param_error, HOOK_JUMP).install()->quick();

			utils::hook(SELECT_VALUE(0x4D6510, 0x56A980), &scr_get_pointer_type, HOOK_JUMP).install()->quick();
			utils::hook(SELECT_VALUE(0x4958D0, 0x56A8C0), &scr_get_type, HOOK_JUMP).install()->quick();

			utils::hook::set<game::native::BuiltinFunction>(SELECT_VALUE(0x92BB58, 0x8AC040), gscr_cast_int);
			utils::hook::set<game::native::BuiltinFunction>(SELECT_VALUE(0x92BB64, 0x8AC04C), gscr_cast_float);

			utils::hook::set<game::native::BuiltinFunction>(SELECT_VALUE(0x92B93C, 0x8ABE24), assert_cmd);
			utils::hook::set<game::native::BuiltinFunction>(SELECT_VALUE(0x92B948, 0x8ABE30), assert_ex_cmd);
			utils::hook::set<game::native::BuiltinFunction>(SELECT_VALUE(0x92B954, 0x8ABE3C), assert_msg_cmd);
		}

		void pre_destroy() override
		{
			scr_emit_function_hook.clear();
		}
	};
}

REGISTER_MODULE(gsc::error)
