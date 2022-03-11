#include <std_include.hpp>
#include "player_movement.hpp"

#include "utils/hook.hpp"

const game::native::dvar_t* player_movement::player_sustainAmmo;

void player_movement::post_load()
{
	if (game::is_mp()) this->patch_mp();
	else if (game::is_sp()) this->patch_sp();
}

void player_movement::pm_weapon_use_ammo(game::native::playerState_s* ps, const game::native::Weapon weapon,
	bool is_alternate, int amount, game::native::PlayerHandIndex hand)
{
	if (!player_movement::player_sustainAmmo->current.enabled)
	{
		game::native::PM_WeaponUseAmmo(ps, weapon, is_alternate, amount, hand);
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
}

void player_movement::patch_sp()
{
	player_movement::player_sustainAmmo = game::native::Dvar_RegisterBool("player_sustainAmmo",
		false, game::native::DVAR_CODINFO, "Firing weapon will not decrease clip ammo");

	utils::hook(0x648C3A, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x64891D, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x6484E2, &player_movement::pm_weapon_use_ammo, HOOK_CALL).install()->quick();
}

REGISTER_MODULE(player_movement);
