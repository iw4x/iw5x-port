#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>
#include <utils/io.hpp>

#include "game/scripting/context.hpp"
#include "scheduler.hpp"

class scripting final : public module
{
public:
	void post_load() override
	{
		start_hook_.initialize(SELECT_VALUE(0x50C575, 0x50D4F2, 0x48A026), &start_execution_stub, HOOK_CALL) //
		           ->install() //
		           ->quick();

		stop_hook_.initialize(SELECT_VALUE(0x528B04, 0x569E46, 0x4F03FA), &stop_execution_stub, HOOK_CALL) //
		          ->install() //
		          ->quick();

		utils::hook(SELECT_VALUE(0x4F9706, 0x5772A0, 0x4FAB88), &frame_stub, HOOK_CALL).install()->quick();
		utils::hook(SELECT_VALUE(0x4FFA48, 0x5774AB, 0x4FEFD7), &frame_stub, HOOK_CALL).install()->quick();
		// Only relevant one?

		utils::hook(SELECT_VALUE(0x6109F3, 0x56B637, 0x4EDFF7), &vm_notify_stub, HOOK_CALL).install()->quick();
		utils::hook(SELECT_VALUE(0x6128BE, 0x56D541, 0x4EFAF9), &vm_notify_stub, HOOK_CALL).install()->quick();

		if (game::is_sp())
		{
			utils::hook(0x50C57E, &frame_stub, HOOK_CALL).install()->quick();
			utils::hook(0x6523A3, &frame_stub, HOOK_CALL).install()->quick();
			utils::hook(0x5145D2, &frame_stub, HOOK_CALL).install()->quick();

			utils::hook(0x610970, &vm_notify_stub, HOOK_JUMP).install()->quick();
		}
	}

	void pre_destroy() override
	{
		this->scripts_.clear();
	}

private:
	std::vector<std::unique_ptr<game::scripting::context>> scripts_;

	void load_scripts()
	{
		const auto script_dir = "open-iw5/scripts/"s;

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

	void run_frame()
	{
		for (const auto& script : this->scripts_)
		{
			script->get_scheduler()->run_frame();
		}
	}

	void dispatch(game::scripting::event* event)
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
		module_loader::get<scripting>()->start_execution();
		static_cast<void(*)()>(start_hook_.get_original())();
	}

	static void stop_execution_stub()
	{
		module_loader::get<scripting>()->stop_execution();
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

			//printf("%X: %s\n", e.entity_id, e.name.data());

			for (auto* value = stack; value->type != game::native::SCRIPT_END; --value)
			{
				e.arguments.emplace_back(*value);
			}

			module_loader::get<scripting>()->dispatch(&e);
		}
		catch (std::exception& e)
		{
			propagate_error(e);
		}

		game::native::VM_Notify(notify_id, type, stack);
	}

	static int frame_stub(const int a1, const int a2)
	{
		module_loader::get<scripting>()->run_frame();
		return game::native::G_RunFrame(a1, a2);
	}
};

utils::hook scripting::start_hook_;
utils::hook scripting::stop_hook_;

REGISTER_MODULE(scripting)
