#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/string.hpp>
#include <utils/hook.hpp>

#include "game/game.hpp"

#include "command.hpp"

utils::memory::allocator command::allocator_;
std::unordered_map<std::string, std::function<void(const command::params&)>> command::handlers;
std::unordered_map<std::string, std::function<void(game::native::gentity_s*, command::params_sv&)>> command::handlers_sv;
std::unordered_map<std::string, std::function<void(game::native::sp::gentity_s*, command::params_sv&)>> command::handlers_sp_sv;

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
	// Since the game console is not usable there is no point in calling add_raw
	const auto command = utils::string::to_lower(name);

	if (handlers_sv.find(command) == handlers_sv.end())
	{
		handlers_sv[command] = callback;
	}
}

void command::add_sp_sv(const char* name, std::function<void(game::native::sp::gentity_s*, const params_sv&)> callback)
{
	const auto command = utils::string::to_lower(name);

	if (handlers_sp_sv.find(command) == handlers_sp_sv.end())
	{
		handlers_sp_sv[command] = callback;
	}
}

void command::execute(std::string command, bool sync)
{
	command += "\n";

	if (sync)
	{
		game::native::Cmd_ExecuteSingleCommand(game::native::LOCAL_CLIENT_0, 0, command.data());
	}
	else
	{
		game::native::Cbuf_AddText(game::native::LOCAL_CLIENT_0, command.data());
	}
}

void command::main_handler()
{
	params params;

	const auto command = utils::string::to_lower(params[0]);
	const auto got = command::handlers.find(command);

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
	const auto got = command::handlers_sv.find(command);

	if (got != handlers_sv.end())
	{
		got->second(entity, params);
		return;
	}

	game::native::ClientCommand(client_num);
}

void command::client_command_sp(int client_num, const char* s)
{
	auto* entity = &game::native::sp::g_entities[client_num];

	assert(entity->client != nullptr); // On sp it should only be an assertion

	params_sv params;

	const auto command = utils::string::to_lower(params[0]);
	const auto got = command::handlers_sp_sv.find(command);

	if (got != handlers_sp_sv.end())
	{
		got->second(entity, params);
	}
}

__declspec(naked) void command::client_command_sp_stub()
{
	__asm
	{
		pushad

		push [esp + 0x20 + 0x8]
		push [esp + 0x20 + 0x8]
		call command::client_command_sp
		add esp, 0x8

		popad

		// Code our hook skipped
		mov eax, [esp + 0x8]
		sub esp, 0x400

		push 0x44BB5A // ClientCommand
		retn
	}
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

// Between ufo/noclip functions and their mp counterpart is that I reversed the 'CG' type
void command::add_sp_commands()
{
	add("noclip", []()
	{
		if (!game::native::sp::IsServerRunning())
			return;

		const auto* ent = &game::native::sp::g_entities[0];

		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);

		ent->client->flags ^= 1;

		const auto* msg = (ent->client->flags & 1) ? "GAME_NOCLIPON" : "GAME_NOCLIPOFF";
		printf("%s\n", game::native::SEH_LocalizeTextMessage(msg, "noclip print", game::native::LOCMSG_SAFE));
	});

	add("ufo", []()
	{
		if (!game::native::sp::IsServerRunning())
			return;

		const auto* ent = &game::native::sp::g_entities[0];

		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);

		ent->client->flags ^= 2;

		const auto* msg = (ent->client->flags & 2) ? "GAME_UFOON" : "GAME_UFOOFF";
		printf("%s\n", game::native::SEH_LocalizeTextMessage(msg, "ufo print", game::native::LOCMSG_SAFE));
	});

	add("god", []()
	{
		if (!game::native::sp::IsServerRunning())
			return;

		auto* ent = &game::native::sp::g_entities[0];

		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);

		ent->flags ^= game::native::FL_GODMODE;

		const auto* msg = (ent->flags & game::native::FL_GODMODE) ? "GAME_GODMODE_ON" : "GAME_GODMODE_OFF";
		printf("%s\n", game::native::SEH_LocalizeTextMessage(msg, "god print", game::native::LOCMSG_SAFE));
	});

	add("demigod", []()
	{
		if (!game::native::sp::IsServerRunning())
			return;

		auto* ent = &game::native::sp::g_entities[0];

		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);

		ent->flags ^= game::native::FL_DEMI_GODMODE;

		const auto* msg = (ent->flags & game::native::FL_DEMI_GODMODE) ? "GAME_DEMI_GODMODE_ON" : "GAME_DEMI_GODMODE_OFF";
		printf("%s\n", game::native::SEH_LocalizeTextMessage(msg, "demigod print", game::native::LOCMSG_SAFE));
	});

	add("notarget", []()
	{
		if (!game::native::sp::IsServerRunning())
			return;

		auto* ent = &game::native::sp::g_entities[0];

		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);

		ent->flags ^= game::native::FL_NOTARGET;

		const auto* msg = (ent->flags & game::native::FL_NOTARGET) ? "GAME_NOTARGETON" : "GAME_NOTARGETOFF";
		printf("%s\n", game::native::SEH_LocalizeTextMessage(msg, "notarget print", game::native::LOCMSG_SAFE));
	});
}

void command::post_load()
{
	if (game::is_dedi())
	{
		utils::hook(0x4F96B5, &command::client_command_dedi_stub, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommand
		return;
	}

	add("quit", game::native::Com_Quit_f);

	if (game::is_mp())
	{
		utils::hook(0x57192A, &command::client_command_stub, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommand
	}
	else
	{
		utils::hook(0x44BB50, &command::client_command_sp_stub, HOOK_JUMP).install()->quick();
		utils::hook::nop(0x44BB55, 5); // Nop skipped instructions
		add_sp_commands();
	}
}

REGISTER_MODULE(command);
