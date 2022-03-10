#include <std_include.hpp>
#include "command.hpp"

#include "utils/string.hpp"
#include "utils/hook.hpp"

utils::memory::allocator command::allocator_;
std::unordered_map<std::string, std::function<void(const command::params&)>> command::handlers;
std::unordered_map<std::string, std::function<void(game::native::gentity_s*, command::params_sv&)>> command::handlers_sv;

command::params::params()
	: nesting_(game::native::cmd_args->nesting)
{
	assert(game::native::cmd_args->nesting < game::native::CMD_MAX_NESTING);
}

int command::params::size() const
{
	return game::native::cmd_args->argc[this->nesting_];
}

const char* command::params::get(const int index) const
{
	if (index >= this->size())
	{
		return "";
	}

	return game::native::cmd_args->argv[this->nesting_][index];
}

std::string command::params::join(const int index) const
{
	std::string result;

	for (auto i = index; i < this->size(); i++)
	{
		if (i > index) result.append(" ");
		result.append(this->get(i));
	}

	return result;
}

command::params_sv::params_sv()
	: nesting_(game::native::sv_cmd_args->nesting)
{
	assert(game::native::sv_cmd_args->nesting < game::native::CMD_MAX_NESTING);
}

int command::params_sv::size() const
{
	return game::native::sv_cmd_args->argc[this->nesting_];
}

const char* command::params_sv::get(const int index) const
{
	if (index >= this->size())
	{
		return "";
	}

	return game::native::sv_cmd_args->argv[this->nesting_][index];
}

std::string command::params_sv::join(const int index) const
{
	std::string result;

	for (auto i = index; i < this->size(); i++)
	{
		if (i > index) result.append(" ");
		result.append(this->get(i));
	}

	return result;
}

void command::add_raw(const char* name, void (*callback)())
{
	game::native::Cmd_AddCommand(name, callback, command::allocator_.allocate<game::native::cmd_function_t>());
}

void command::add(const char* name, const std::function<void(const command::params&)>& callback)
{
	const auto command = utils::string::to_lower(name);

	if (handlers.find(command) == handlers.end())
	{
		add_raw(name, main_handler);
	}

	handlers[command] = callback;
}

void command::add(const char* name, const std::function<void()>& callback)
{
	add(name, [callback](const command::params&)
	{
		callback();
	});
}

void command::add_sv(const char* name, std::function<void(game::native::gentity_s*, const command::params_sv&)> callback)
{
	// Since the console is not usable there is no point in calling add_raw
	const auto command = utils::string::to_lower(name);

	if (handlers_sv.find(command) == handlers_sv.end())
	{
		handlers_sv[command] = callback;
	}
}

void command::main_handler()
{
	params params;

	const auto command = utils::string::to_lower(params[0]);
	const auto got = handlers.find(command);

	if (got != handlers.end())
	{
		got->second(params);
	}
}

void command::client_command_stub(int client_num)
{
	const auto entity = &game::native::g_entities[client_num];

	if (entity->client == nullptr)
	{
		return;
	}

	params_sv params;

	const auto command = utils::string::to_lower(params[0]);
	const auto got = handlers_sv.find(command);

	if (got != handlers_sv.end())
	{
		got->second(entity, params);
	}

	game::native::ClientCommand(client_num);
}

__declspec(naked) void command::client_command_dedi_stub()
{
	__asm
	{
		pushad

		push edi
		call client_command_stub
		add esp, 4h

		popad
		retn
	}
}

void command::post_load()
{
	if (game::is_mp())
	{
		utils::hook(0x57192A, &command::client_command_stub, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommand
	}
	else if (game::is_dedi())
	{
		utils::hook(0x4F96B5, &command::client_command_dedi_stub, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommand
	}	
}

void command::pre_destroy()
{
	for (const auto& [key, val] : command::handlers)
	{
		game::native::Cmd_RemoveCommand(key.data());
		handlers.erase(key);
	}

	command::allocator_.clear();
}

REGISTER_MODULE(command);
