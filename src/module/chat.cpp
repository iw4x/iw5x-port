#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/game.hpp"

#include <utils/hook.hpp>

#include "log_file.hpp"

static void notify_on_say(game::native::gentity_s* ent, int mode, const char* message)
{
	const auto ent_num = ent->s.number;

	game::native::Scr_AddString(message + 1); // First character has nothing to do with actual message
	game::native::Scr_AddInt(mode);
	game::native::Scr_AddEntityNum(ent_num, 0);

	game::native::Scr_NotifyLevel(game::native::SL_GetString("say", 0), 3);

	const auto* guid = game::native::mp::SV_GetGuid(ent_num);
	const auto* name = game::native::mp::svs_clients[ent_num].name;

	if (mode == 0)
	{
		log_file::g_log_printf("say;%s;%d;%s;%s\n", guid, ent_num, name, message + 1);
	}
	else
	{
		log_file::g_log_printf("sayteam;%s;%d;%s;%s\n", guid, ent_num, name, message + 1);
	}
}

static __declspec(naked) void g_say_stub()
{
	__asm
	{
		pushad

		push [esp + 0x20 + 0x108] // message
		push [esp + 0x20 + 0x108] // mode
		push ebp // ent
		call notify_on_say
		add esp, 0xC

		popad

		push 0x5C2940 // I_strncpyz
		retn
	}
}

class chat final : public module
{
public:
	static_assert(offsetof(game::native::mp::client_t, name) == 0x2142C);

	void post_load() override
	{
		if (game::is_mp())
		{
			utils::hook(0x502BAF, &g_say_stub, HOOK_CALL).install()->quick(); // I_strncpyz
		}
	}
};

REGISTER_MODULE(chat)
