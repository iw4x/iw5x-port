#include <std_include.hpp>
#include "player_movement.hpp"

#include "utils/hook.hpp"

const game::native::dvar_t* player_movement::player_sustainAmmo;
const game::native::dvar_t* player_movement::pm_bounces;

void player_movement::pm_weapon_use_ammo(game::native::playerState_s* ps, const game::native::Weapon weapon,
	bool is_alternate, int amount, game::native::PlayerHandIndex hand)
{
	if (!player_movement::player_sustainAmmo->current.enabled)
	{
		game::native::PM_WeaponUseAmmo(ps, weapon, is_alternate, amount, hand);
	}
}

__declspec(naked) void player_movement::pm_step_slide_move_stub_mp()
{
	__asm
	{
		// Check value of pm_bounces
		push eax
		mov eax, player_movement::pm_bounces
		cmp byte ptr [eax + 0xC], 1
		pop eax

		// Bounce if enabled is true
		je bounce

		// Code hook skipped
		cmp dword ptr [esp + 0x24], 0
		jnz dontBounce

	bounce:
		push 0x424D58
		retn

	dontBounce:
		push 0x424D6C
		retn
	}
}

__declspec(naked) void player_movement::pm_step_slide_move_stub_sp()
{
	__asm
	{
		// Check value of pm_bounces
		push eax
		mov eax, player_movement::pm_bounces
		cmp byte ptr [eax + 0xC], 1
		pop eax

		// Bounce if enabled is true
		je bounce

		// Code hook skipped
		cmp dword ptr [esp + 0x24], 0
		jnz dontBounce

	bounce:
		push 0x43D91F
		retn

	dontBounce:
		push 0x43D933
		retn
	}
}

const game::native::dvar_t* player_movement::dvar_register_player_sustain_ammo(const char* dvar_name,
		bool value, unsigned __int16 /*flags*/, const char* description)
{
	player_movement::player_sustainAmmo = game::native::Dvar_RegisterBool(dvar_name,
		value, game::native::DVAR_CODINFO, description);

	return player_movement::player_sustainAmmo;
}

void player_movement::patch_mp()
{
	utils::hook(0x418D9C, &player_movement::dvar_register_player_sustain_ammo, HOOK_CALL).install()->quick();

	utils::hook(0x42B5DA, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x42B2BD, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x42AE95, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();

	utils::hook(0x424D51, &player_movement::pm_step_slide_move_stub_mp, HOOK_JUMP).install()->quick();
}

void player_movement::patch_sp()
{
	player_movement::player_sustainAmmo = game::native::Dvar_RegisterBool("player_sustainAmmo",
		false, game::native::DVAR_CODINFO, "Firing weapon will not decrease clip ammo");

	utils::hook(0x648C3A, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x64891D, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x6484E2, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();

	utils::hook(0x43D918, &player_movement::pm_step_slide_move_stub_sp, HOOK_JUMP).install()->quick();
}

void player_movement::post_load()
{
	if (game::is_dedi())
	{
		return;
	}

	player_movement::pm_bounces = game::native::Dvar_RegisterBool("pm_bounces", false,
		game::native::dvar_flags::DVAR_CODINFO, "CoD4 Bounces");

	if (game::is_mp()) this->patch_mp();
	else if (game::is_sp()) this->patch_sp();
}

REGISTER_MODULE(player_movement);
