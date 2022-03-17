#pragma once
#include "loader/module_loader.hpp"

class player_movement final : public module
{
public:
	void post_load() override;

private:
	static const game::native::dvar_t* player_sustainAmmo;
	static const game::native::dvar_t* jump_slowdownEnable;
	static const game::native::dvar_t* jump_ladderPushVel;
	static const game::native::dvar_t* jump_height;
	static const game::native::dvar_t* pm_bounces;
	static const game::native::dvar_t* pm_playerEjection;
	static const game::native::dvar_t* pm_playerCollision;
	static const game::native::dvar_t* pm_rocketJump;
	static const game::native::dvar_t* pm_elevators;

	static DWORD bounce_addr;
	static DWORD dont_bounce_addr;
	static DWORD push_off_ladder_addr;

	static void pm_weapon_use_ammo(game::native::playerState_s* ps, const game::native::Weapon weapon,
		bool is_alternate, int amount, game::native::PlayerHandIndex hand);

	static const game::native::dvar_t* dvar_register_jump_ladder_push_vel(const char* dvar_name,
		float value, float min, float max, unsigned __int16 flags, const char* description);
	static const game::native::dvar_t* dvar_register_jump_height(const char* dvar_name,
		float value, float min, float max, unsigned __int16 flags, const char* description);
	static const game::native::dvar_t* dvar_register_jump_slowdown_enable(const char* dvar_name,
		bool value, unsigned __int16 flags, const char* description);
	static const game::native::dvar_t* dvar_register_player_sustain_ammo(const char* dvar_name,
		bool value, unsigned __int16 flags, const char* description);

	static void pm_step_slide_move_stub();

	static int stuck_in_client_stub(game::native::gentity_s* self);
	static void cm_transformed_capsule_trace_stub(game::native::trace_t* results, const float* start,
		const float* end, const game::native::Bounds* bounds, const game::native::Bounds* capsule,
		int contents, const float* origin, const float* angles);

	static game::native::gentity_s* weapon_rocket_launcher_fire_stub(game::native::gentity_s* ent,
		const game::native::Weapon weapon, float spread, game::native::weaponParms* wp,
		const float* gun_vel, game::native::missileFireParms* fire_parms, game::native::missileFireParms* magic_bullet);

	static void pm_player_trace_stub(game::native::pmove_t* pm, game::native::trace_t* results,
		const float* start, const float* end, const game::native::Bounds* bounds,
		int pass_entity_num, int content_mask);

	static void pm_trace_stub(const game::native::pmove_t* pm, game::native::trace_t* results,
		const float* start, const float* end, const game::native::Bounds* bounds, int pass_entity_num, int content_mask);

	static void jump_push_off_ladder_stub();
	// On SP this is already unlocked
	static void jump_start_stub();

	static void jump_apply_slowdown_stub(game::native::playerState_s* ps);

	static void patch_mp();
	static void patch_sp();
};
