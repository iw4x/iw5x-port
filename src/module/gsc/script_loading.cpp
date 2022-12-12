#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include "script_loading.hpp"

#include "module/log_file.hpp"
#include "module/scripting.hpp"

#include <utils/hook.hpp>
#include <utils/memory.hpp>

#include <xsk/gsc/types.hpp>
#include <xsk/gsc/interfaces/compiler.hpp>
#include <xsk/gsc/interfaces/assembler.hpp>
#include <xsk/utils/compression.hpp>
#include <xsk/resolver.hpp>
#include <interface.hpp>

namespace gsc
{
	namespace
	{
		auto compiler = ::gsc::compiler();
		auto assembler = ::gsc::assembler();

		utils::memory::allocator script_file_allocator;

		std::unordered_map<std::string, game::native::ScriptFile*> loaded_scripts;

		void clear()
		{
			loaded_scripts.clear();
			script_file_allocator.clear();
		}

		bool read_script_file(const std::string& name, std::string* data)
		{
			char* buffer{};
			const auto file_len = game::native::FS_ReadFile(name.data(), &buffer);
			if (file_len > 0 && buffer)
			{
				data->append(buffer, file_len);
				game::native::Hunk_FreeTempMemory(buffer);
				return true;
			}

			return false;
		}

		game::native::ScriptFile* load_custom_script(const char* file_name, const std::string& real_name)
		{
			if (const auto itr = loaded_scripts.find(real_name); itr != loaded_scripts.end())
			{
				return itr->second;
			}

			std::string source_buffer;
			if (!read_script_file(real_name + ".gsc", &source_buffer))
			{
				return nullptr;
			}

			std::vector<std::uint8_t> data;
			data.assign(source_buffer.begin(), source_buffer.end());

			try
			{
				compiler->compile(real_name, data);
			}
			catch (const std::exception& ex)
			{
				log_file::info("*********** script compile error *************\n");
				log_file::info("failed to compile '%s':\n%s", real_name.data(), ex.what());
				log_file::info("**********************************************\n");
				return nullptr;
			}

			auto assembly = compiler->output();

			try
			{
				assembler->assemble(real_name, assembly);
			}
			catch (const std::exception& ex)
			{
				log_file::info("*********** script compile error *************\n");
				log_file::info("failed to assemble '%s':\n%s", real_name.data(), ex.what());
				log_file::info("**********************************************\n");
				return nullptr;
			}

			const auto script_file_ptr = static_cast<game::native::ScriptFile*>(script_file_allocator.allocate(sizeof(game::native::ScriptFile)));
			script_file_ptr->name = file_name;

			const auto stack = assembler->output_stack();
			script_file_ptr->len = static_cast<int>(stack.size());

			const auto script = assembler->output_script();
			script_file_ptr->bytecodeLen = static_cast<int>(script.size());

			const auto compressed = xsk::utils::zlib::compress(stack);
			const auto stack_size = compressed.size();
			const auto byte_code_size = script.size() + 1;

			script_file_ptr->buffer = static_cast<char*>(game::native::Hunk_AllocateTempMemoryHighInternal(stack_size));
			std::memcpy(const_cast<char*>(script_file_ptr->buffer), compressed.data(), compressed.size());

			script_file_ptr->bytecode = static_cast<std::uint8_t*>(game::native::PMem_AllocFromSource_NoDebug(byte_code_size, 4, 0, game::native::PMEM_SOURCE_SCRIPT));
			std::memcpy(script_file_ptr->bytecode, script.data(), script.size());

			script_file_ptr->compressedLen = static_cast<int>(compressed.size());

			loaded_scripts[real_name] = script_file_ptr;

			return script_file_ptr;
		}

		std::string get_script_file_name(const std::string& name)
		{
			const auto id = xsk::gsc::iw5::resolver::token_id(name);
			if (!id)
			{
				return name;
			}

			return std::to_string(id);
		}

		int db_is_x_asset_default(game::native::XAssetType type, const char* name)
		{
			if (loaded_scripts.contains(name))
			{
				return 0;
			}

			return game::native::DB_IsXAssetDefault(type, name);
		}
	}

	game::native::ScriptFile* find_script(game::native::XAssetType type, const char* name, int allow_create_default)
	{
		std::string real_name = name;
		const auto id = static_cast<std::uint16_t>(std::strtol(name, nullptr, 10));
		if (id)
		{
			real_name = xsk::gsc::iw5::resolver::token_name(id);
		}

		auto* script = load_custom_script(name, real_name);
		if (script)
		{
			return script;
		}

		return game::native::DB_FindXAssetHeader(type, name, allow_create_default).scriptfile;
	}

	class script_loading final : public module
	{
	public:
		void post_load() override
		{
			// ProcessScript
			utils::hook(SELECT_VALUE(0x44685E, 0x56B13E), find_script, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x446868, 0x56B148), db_is_x_asset_default, HOOK_CALL).install()->quick();

			scripting::on_shutdown([](int free_scripts) -> void
			{
				if (free_scripts)
				{
					xsk::gsc::iw5::resolver::cleanup();
					clear();
				}
			});
		}
	};
}

REGISTER_MODULE(gsc::script_loading)
