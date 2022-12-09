#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>
#include <utils/io.hpp>

#include "game/scripting/context.hpp"
#include "game/scripting/functions.hpp"

#include "scheduler.hpp"
#include "scripting.hpp"

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
	}

	std::string get_token(unsigned int id)
	{
		return find_token(id);
	}

	class scripting_class final : public module
	{
	public:
		void post_load() override
		{
			start_hook_.initialize(SELECT_VALUE(0x50C575, 0x50D4F2), &start_execution_stub, HOOK_CALL) //
				->install() //
				->quick();

			stop_hook_.initialize(SELECT_VALUE(0x528B04, 0x569E46), &stop_execution_stub, HOOK_CALL) //
				->install() //
				->quick();

			utils::hook(SELECT_VALUE(0x4F9706, 0x5772A0), &frame_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x4FFA48, 0x5774AB), &frame_stub, HOOK_CALL).install()->quick();
			// Only relevant one?

			utils::hook(SELECT_VALUE(0x6109F3, 0x56B637), &vm_notify_stub, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x6128BE, 0x56D541), &vm_notify_stub, HOOK_CALL).install()->quick();

			if (game::is_sp())
			{
				utils::hook(0x50C57E, &frame_stub, HOOK_CALL).install()->quick();
				utils::hook(0x6523A3, &frame_stub, HOOK_CALL).install()->quick();
				utils::hook(0x5145D2, &frame_stub, HOOK_CALL).install()->quick();

				utils::hook(0x610970, &vm_notify_stub, HOOK_JUMP).install()->quick();
			}

			utils::hook(SELECT_VALUE(0x44690A, 0x56B1EA), &scr_set_thread_position, HOOK_CALL).install()->quick();
			utils::hook(SELECT_VALUE(0x4232A8, 0x561748), &process_script, HOOK_CALL).install()->quick();
		}

		void pre_destroy() override
		{
			this->scripts_.clear();
		}

	private:
		std::vector<std::unique_ptr<game::scripting::context>> scripts_;

		void load_scripts()
		{
			const auto script_dir = "userraw/scripts/"s;

			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				if (script.substr(script.find_last_of('.') + 1) == "chai")
				{
					try
					{
						auto context = std::make_unique<game::scripting::context>();
						context->get_chai()->eval_file(script);
						this->scripts_.push_back(std::move(context));
					}
					catch (chaiscript::exception::eval_error& e)
					{
						throw std::runtime_error(e.pretty_print());
					}
				}
			}
		}

		void start_execution()
		{
			try
			{
				this->load_scripts();
			}
			catch (std::exception& e)
			{
				propagate_error(e);
			}
		}

		void stop_execution()
		{
			this->scripts_.clear();
		}

		void run_frame() const
		{
			for (const auto& script : this->scripts_)
			{
				script->get_scheduler()->run_frame();
			}
		}

		void dispatch(game::scripting::event* event) const
		{
			for (const auto& script : this->scripts_)
			{
				script->get_event_handler()->dispatch(event);
			}
		}

		static utils::hook start_hook_;
		static utils::hook stop_hook_;

		static void propagate_error(const std::exception& e)
		{
			printf("\n******* Script execution error *******\n");
			printf("%s\n", e.what());
			printf("**************************************\n\n");

			scheduler::once([]
			{
				game::native::Com_Error(game::native::errorParm_t::ERR_SCRIPT, "Script execution error\n(see console for actual details)\n");
			}, scheduler::pipeline::main);
		}

		static void start_execution_stub()
		{
			module_loader::get<scripting_class>()->start_execution();
			static_cast<void(*)()>(start_hook_.get_original())();
		}

		static void stop_execution_stub()
		{
			module_loader::get<scripting_class>()->stop_execution();
			static_cast<void(*)()>(stop_hook_.get_original())();
		}

		static void vm_notify_stub(const unsigned int notify_id, const unsigned short type,
			game::native::VariableValue* stack)
		{
			try
			{
				game::scripting::event e;
				e.name = game::native::SL_ConvertToString(type);
				e.entity_id = notify_id;

				if (e.name == "touch") return; // Skip that for now

				for (auto* value = stack; value->type != game::native::SCRIPT_END; --value)
				{
					e.arguments.emplace_back(*value);
				}

				module_loader::get<scripting_class>()->dispatch(&e);
			}
			catch (std::exception& e)
			{
				propagate_error(e);
			}

			game::native::VM_Notify(notify_id, type, stack);
		}

		static int frame_stub(const int a1, const int a2)
		{
			module_loader::get<scripting_class>()->run_frame();
			return game::native::G_RunFrame(a1, a2);
		}
	};

	utils::hook scripting_class::start_hook_;
	utils::hook scripting_class::stop_hook_;
}



REGISTER_MODULE(scripting::scripting_class)
