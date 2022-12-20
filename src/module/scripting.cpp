#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include <utils/hook.hpp>

#include "game/scripting/functions.hpp"

#include "gsc/script_loading.hpp"

namespace scripting
{
	std::unordered_map<std::string, std::unordered_map<std::string, const char*>> script_function_table;
	std::unordered_map<std::string, std::vector<std::pair<std::string, const char*>>> script_function_table_sort;
	std::unordered_map<const char*, std::pair<std::string, std::string>> script_function_table_rev;

	std::string current_file;

	namespace
	{
		std::uint32_t current_file_id = 0;
		std::string current_script_file;

		std::vector<std::function<void(int)>> shutdown_callbacks;

		std::string get_token(unsigned int id)
		{
			return find_token(id);
		}

		void add_function_sort(unsigned int id, const char* pos)
		{
			std::string filename = current_file;
			if (current_file_id)
			{
				filename = get_token(current_file_id);
			}

			if (!script_function_table_sort.contains(filename))
			{
				auto* script = gsc::find_script(game::native::ASSET_TYPE_SCRIPTFILE, current_script_file.data(), false);
				if (script)
				{
					const auto* end = &script->bytecode[script->bytecodeLen];
					script_function_table_sort[filename].emplace_back("__end__", reinterpret_cast<const char*>(end));
				}
			}

			const auto name = scripting::get_token(id);
			auto& itr = script_function_table_sort[filename];
			itr.insert(itr.end() - 1, { name, pos });
		}

		void add_function(const std::string& file, unsigned int id, const char* pos)
		{
			const auto name = get_token(id);
			script_function_table[file][name] = pos;
			script_function_table_rev[pos] = { file, name };
		}

		void scr_set_thread_position(unsigned int thread_name, const char* code_pos)
		{
			add_function_sort(thread_name, code_pos);

			if (current_file_id)
			{
				const auto name = get_token(current_file_id);
				add_function(name, thread_name, code_pos);
			}
			else
			{
				add_function(current_file, thread_name, code_pos);
			}

			utils::hook::invoke<void>(SELECT_VALUE(0x4845F0, 0x5616D0), thread_name, code_pos);
		}

		void process_script(const char* filename)
		{
			current_script_file = filename;

			const auto file_id = std::strtol(filename, nullptr, 10);
			if (file_id)
			{
				current_file_id = file_id;
			}
			else
			{
				current_file_id = 0;
				current_file = filename;
			}

			utils::hook::invoke<void>(SELECT_VALUE(0x446850, 0x56B130), filename);
		}

		void g_shutdown_game_stub(int free_scripts)
		{
			utils::hook::invoke<void>(SELECT_VALUE(0x528A90, 0x50C100), free_scripts);

			if (free_scripts)
			{
				script_function_table_sort.clear();
				script_function_table.clear();
				script_function_table_rev.clear();
			}

			for (const auto& callback : shutdown_callbacks)
			{
				callback(free_scripts);
			}
		}
	}

	std::string get_token(unsigned int id)
	{
		return find_token(id);
	}

	void on_shutdown(const std::function<void(int)>& callback)
	{
		shutdown_callbacks.push_back(callback);
	}

	class scripting_class final : public module
	{
	public:
		void post_load() override
		{
			utils::hook(SELECT_VALUE(0x44690A, 0x56B1EA), &scr_set_thread_position, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x4232A8, 0x561748), &process_script, HOOK_CALL).install()->quick();

			utils::hook(SELECT_VALUE(0x651E1B, 0x573C82), &g_shutdown_game_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x651ECC, 0x573D3A), &g_shutdown_game_stub, HOOK_CALL).install()->quick();
		}
	};
}

REGISTER_MODULE(scripting::scripting_class)
