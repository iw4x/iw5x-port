#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/game.hpp"

#include <utils/hook.hpp>

static void notify_on_say(game::native::gentity_s* ent, int mode, const char* message)
{
	game::native::Scr_AddString(message + 1); // First character has nothing to do with actual message
	game::native::Scr_AddInt(mode);
	game::native::Scr_AddEntityNum(ent->s.number, 0);

	game::native::Scr_NotifyLevel(game::native::SL_GetString("say", 0), 3);
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
	void post_load() override
	{
		if (game::is_mp())
		{
			utils::hook(0x502BAF, &g_say_stub, HOOK_CALL).install()->quick(); // I_strncpyz
		}
	}
};

REGISTER_MODULE(chat)
