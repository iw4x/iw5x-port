#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

#include "command.hpp"


static utils::memory::allocator allocator;

std::unordered_map<std::string, std::function<void(const command::params&)>> command::handlers;
std::unordered_map<std::string, std::function<void(game::native::gentity_s*, const command::params_sv&)>> command::handlers_sv;
std::unordered_map<std::string, std::function<void(game::native::sp::gentity_s*, const command::params_sv&)>> command::handlers_sp_sv;

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
	game::native::Cmd_AddCommand(name, callback, allocator.allocate<game::native::cmd_function_t>());
}

void command::add(const char* name, const std::function<void(const params&)>& callback)
{
	const auto command = utils::string::to_lower(name);

	if (!handlers.contains(command))
	{
		add_raw(name, main_handler);
	}

	handlers[command] = callback;
}

void command::add(const char* name, const std::function<void()>& callback)
{
	add(name, [callback]([[maybe_unused]] const params& params)
	{
		callback();
	});
}

void command::add_sv(const char* name, std::function<void(game::native::gentity_s*, const params_sv&)> callback)
{
	// Since the game console is not usable there is no point in calling add_raw
	const auto command = utils::string::to_lower(name);

	if (!handlers_sv.contains(command))
	{
		handlers_sv[command] = callback;
	}
}

void command::add_sp_sv(const char* name, std::function<void(game::native::sp::gentity_s*, const params_sv&)> callback)
{
	const auto command = utils::string::to_lower(name);

	if (!handlers_sp_sv.contains(command))
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
	const params params;

	const auto command = utils::string::to_lower(params[0]);

	if (const auto itr = handlers.find(command); itr != handlers.end())
	{
		itr->second(params);
	}
}

void command::client_command_stub(int client_num)
{
	const auto entity = &game::native::g_entities[client_num];

	if (!entity->client)
	{
		return;
	}

	const params_sv params;

	const auto command = utils::string::to_lower(params[0]);

	if (const auto itr = handlers_sv.find(command); itr != handlers_sv.end())
	{
		itr->second(entity, params);
		return;
	}

	game::native::ClientCommand(client_num);
}

void command::client_command_sp(int client_num, const char* s)
{
	auto* entity = &game::native::sp::g_entities[client_num];

	assert(entity->client); // On sp it should only be an assertion

	const params_sv params;

	const auto command = utils::string::to_lower(params[0]);

	if (const auto itr = handlers_sp_sv.find(command); itr != handlers_sp_sv.end())
	{
		itr->second(entity, params);
	}
}

__declspec(naked) void command::client_command_sp_stub()
{
	__asm
	{
		pushad

		push [esp + 0x20 + 0x8]
		push [esp + 0x20 + 0x8]
		call client_command_sp
		add esp, 0x8

		popad

		// Code our hook skipped
		mov eax, [esp + 0x8]
		sub esp, 0x400

		push 0x44BB5A // ClientCommand
		retn
	}
}

// Between ufo/noclip functions and their mp counterpart is that I reversed the 'CG' type
void command::add_sp_commands()
{
	add("noclip", []
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

	add("ufo", []
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

	add("god", []
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

	add("demigod", []
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

	add("notarget", []
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

	add("setviewpos", [](const params& params)
	{
		if (!game::native::sp::IsServerRunning())
			return;

		auto* ent = &game::native::sp::g_entities[0];
		if (ent->health < 1)
			return;

		assert(ent->s.eType == game::native::ET_PLAYER);
		assert(ent->client);

		game::native::vec3_t origin, angles{0.f, 0.f, 0.f};

		if (params.size() < 4 || params.size() > 6)
		{
			printf("setviewpos x y z [yaw] [pitch]\n");
			return;
		}

		for (auto i = 0; i < 3; i++)
		{
			origin[i] = std::strtof(params.get(i + 1), nullptr);
		}

		if (params.size() > 4)
		{
			angles[1] = std::strtof(params.get(4), nullptr); // Yaw
		}

		if (params.size() == 6)
		{
			angles[0] = std::strtof(params.get(5), nullptr); // Pitch
		}

		game::native::sp::TeleportPlayer(ent, origin, angles);
	});
}

void command::post_load()
{
	add("quit", game::native::Com_Quit_f);

	if (game::is_mp())
	{
		utils::hook(0x57192A, &client_command_stub, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommand
	}
	else
	{
		utils::hook(0x44BB50, &client_command_sp_stub, HOOK_JUMP).install()->quick();
		utils::hook::nop(0x44BB55, 5); // Nop skipped instructions
		add_sp_commands();
	}
}

REGISTER_MODULE(command);
