#pragma once
#include "loader/module_loader.hpp"
#include "game/game.hpp"

class player_movement final : public module
{
public:
	void post_load() override;

private:
	static const game::native::dvar_t* player_sustainAmmo;
	static const game::native::dvar_t* pm_bounces;
	static const game::native::dvar_t* pm_playerEjection;
	static const game::native::dvar_t* pm_playerCollision;

	static void pm_weapon_use_ammo(game::native::playerState_s* ps, const game::native::Weapon weapon,
		bool isAlternate, int amount, game::native::PlayerHandIndex hand);

	static const game::native::dvar_t* dvar_register_player_sustain_ammo(const char* dvar_name,
		bool value, unsigned __int16 flags, const char* description);

	static void pm_step_slide_move_stub_mp();
	static void pm_step_slide_move_stub_sp();

	static int stuck_in_client_stub(game::native::gentity_s* self);
	static void cm_transformed_capsule_trace_stub(game::native::trace_t* results, const float* start,
		const float* end, const game::native::Bounds* bounds, const game::native::Bounds* capsule,
		int contents, const float* origin, const float* angles);

	static void patch_mp();
	static void patch_sp();
};
