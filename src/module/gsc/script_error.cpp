#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include "script_error.hpp"

#include "module/scripting.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

using namespace utils::string;

namespace gsc
{
	namespace
	{
		utils::hook::detour scr_emit_function_hook;

		unsigned int current_filename = 0;

		std::string unknown_function_error;

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
				unknown_function_error = std::format("unknown script '{}'", scripting::current_file);
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

	class error final : public module
	{
	public:
		void post_load() override
		{
			scr_emit_function_hook.create(SELECT_VALUE(0x40DCB0, 0x561400), &scr_emit_function_stub);

			utils::hook(SELECT_VALUE(0x60DABA, 0x5615FA), &compile_error_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x60DAD1, 0x561611), &compile_error_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x40DCFA, 0x56144A), &find_variable_stub, HOOK_CALL).install()->quick();
		}

		void pre_destroy() override
		{
			scr_emit_function_hook.clear();
		}
	};
}

REGISTER_MODULE(gsc::error)
