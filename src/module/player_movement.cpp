#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>

#include "game/game.hpp"
#include "player_movement.hpp"

const game::native::dvar_t* player_movement::player_sustainAmmo;
const game::native::dvar_t* player_movement::player_lastStandCrawlSpeedScale;
const game::native::dvar_t* player_movement::player_duckedSpeedScale;
const game::native::dvar_t* player_movement::player_proneSpeedScale;
const game::native::dvar_t* player_movement::jump_slowdownEnable;
const game::native::dvar_t* player_movement::jump_ladderPushVel;
const game::native::dvar_t* player_movement::jump_enableFallDamage;
const game::native::dvar_t* player_movement::jump_height;
const game::native::dvar_t* player_movement::jump_stepSize;
const game::native::dvar_t* player_movement::jump_spreadAdd;
const game::native::dvar_t* player_movement::pm_bounces;
const game::native::dvar_t* player_movement::pm_bouncesAllAngles;
const game::native::dvar_t* player_movement::pm_playerEjection;
const game::native::dvar_t* player_movement::pm_playerCollision;
const game::native::dvar_t* player_movement::pm_rocketJump;
const game::native::dvar_t* player_movement::pm_elevators;
const game::native::dvar_t* player_movement::bg_gravity;
const game::native::dvar_t* player_movement::g_speed;

DWORD player_movement::bounce_addr;
DWORD player_movement::dont_bounce_addr;
DWORD player_movement::push_off_ladder_addr;
DWORD player_movement::jump_start_addr;
DWORD player_movement::jump_get_step_height_addr;

void player_movement::pm_weapon_use_ammo(game::native::playerState_s* ps, const game::native::Weapon weapon,
	bool is_alternate, int amount, game::native::PlayerHandIndex hand)
{
	if (!player_sustainAmmo->current.enabled)
	{
		game::native::PM_WeaponUseAmmo(ps, weapon, is_alternate, amount, hand);
	}
}

__declspec(naked) void player_movement::pm_step_slide_move_stub()
{
	__asm
	{
		push eax
		mov eax, pm_bounces
		cmp byte ptr [eax + 0xC], 1
		pop eax

		// Bounce if enabled is true
		je bounce

		// Code hook skipped
		cmp dword ptr [esp + 0x24], 0
		jnz dont_bounce

	bounce:
		jmp bounce_addr

	dont_bounce:
		jmp dont_bounce_addr
	}
}

int player_movement::stuck_in_client_stub(game::native::gentity_s* self)
{
	if (pm_playerEjection->current.enabled)
	{
		return utils::hook::invoke<int>(0x4F8930, self);
	}

	return 0;
}

void player_movement::cm_transformed_capsule_trace_stub(game::native::trace_t* results, const float* start,
	const float* end, const game::native::Bounds* bounds, const game::native::Bounds* capsule, int contents,
	const float* origin, const float* angles)
{
	if (pm_playerCollision->current.enabled)
	{
		game::native::CM_TransformedCapsuleTrace(results, start, end,
			bounds, capsule, contents, origin, angles);
	}
}

game::native::gentity_s* player_movement::weapon_rocket_launcher_fire_mp_stub(game::native::gentity_s* ent,
	const game::native::Weapon weapon, float spread, game::native::weaponParms* wp, const float* gun_vel,
	game::native::missileFireParms* fire_parms, game::native::missileFireParms* magic_bullet)
{
	auto* result = utils::hook::invoke<game::native::gentity_s*>(0x5305D0, ent, weapon, spread, wp,
		gun_vel, fire_parms, magic_bullet);

	if (ent->client != nullptr && pm_rocketJump->current.enabled)
	{
		ent->client->ps.velocity[0] += (0.0f - wp->forward[0]) * 64.0f;
		ent->client->ps.velocity[1] += (0.0f - wp->forward[1]) * 64.0f;
		ent->client->ps.velocity[2] += (0.0f - wp->forward[2]) * 64.0f;
	}

	return result;
}

game::native::sp::gentity_s* player_movement::weapon_rocket_launcher_fire_sp_stub(game::native::sp::gentity_s* ent,
	const game::native::Weapon weapon, float spread, game::native::weaponParms* wp, const float* gun_vel,
	game::native::missileFireParms* fire_parms, game::native::missileFireParms* magic_bullet)
{
	auto* result = utils::hook::invoke<game::native::sp::gentity_s*>(0x48C920, ent, weapon, spread, wp,
		gun_vel, fire_parms, magic_bullet);

	if (ent->client != nullptr && pm_rocketJump->current.enabled)
	{
		ent->client->ps.velocity[0] += (0.0f - wp->forward[0]) * 64.0f;
		ent->client->ps.velocity[1] += (0.0f - wp->forward[1]) * 64.0f;
		ent->client->ps.velocity[2] += (0.0f - wp->forward[2]) * 64.0f;
	}

	return result;
}

void player_movement::pm_player_trace_stub(game::native::pmove_t* pm, game::native::trace_t* results,
	const float* start, const float* end, const game::native::Bounds* bounds, int pass_entity_num, int content_mask)
{
	game::native::PM_playerTrace(pm, results, start, end, bounds, pass_entity_num, content_mask);

	if (pm_elevators->current.enabled)
	{
		results->startsolid = false;
	}
}

void player_movement::pm_trace_stub(const game::native::pmove_t* pm, game::native::trace_t* results,
	const float* start, const float* end, const game::native::Bounds* bounds, int pass_entity_num, int content_mask)
{
	game::native::PM_trace(pm, results, start, end, bounds, pass_entity_num, content_mask);

	if (pm_elevators->current.enabled)
	{
		results->allsolid = false;
	}
}

__declspec(naked) void player_movement::jump_push_off_ladder_stub()
{
	__asm
	{
		push eax
		mov eax, jump_ladderPushVel
		fld dword ptr [eax + 0xC]
		pop eax

		jmp push_off_ladder_addr
	}
}

__declspec(naked) void player_movement::jump_check_stub()
{
	__asm
	{
		push eax
		mov eax, jump_height
		fld dword ptr [eax + 0xC]
		pop eax

		push 0x41696F
		retn
	}
}

void player_movement::jump_apply_slowdown_stub(game::native::playerState_s* ps)
{
	assert(ps->pm_flags & game::native::PMF_JUMPING);

	auto scale = 1.0f;

	if (ps->pm_time <= game::native::JUMP_LAND_SLOWDOWN_TIME)
	{
		if (ps->pm_time == 0)
		{
			const auto height = ps->jumpOriginZ + 18.0f;

			if (height <= ps->origin[2])
			{
				scale = 0.5f;
				ps->pm_time = 1200;
			}
			else
			{
				scale = 0.65f;
				ps->pm_time = game::native::JUMP_LAND_SLOWDOWN_TIME;
			}
		}
	}
	else
	{
		game::native::Jump_ClearState(ps);
		scale = 0.65f;
	}

	if ((ps->pm_flags & game::native::PMF_DIVING) == 0
		&& jump_slowdownEnable->current.enabled)
	{
		game::native::VectorScale(ps->velocity, scale, ps->velocity);
	}
}

float player_movement::jump_get_land_factor(game::native::playerState_s* ps)
{
	assert(ps->pm_flags & game::native::PMF_JUMPING);
	assert(ps->pm_time <= game::native::JUMP_LAND_SLOWDOWN_TIME);

	if (!jump_slowdownEnable->current.enabled
		|| (ps->pm_flags & game::native::PMF_DIVING) != 0)
	{
		return 1.0f;
	}

	if (ps->pm_time < 1700)
	{
		return (static_cast<float>(ps->pm_time) * 1.5f * 0.000588f) + 1.0f;
	}

	return 2.5f;
}

__declspec(naked) void player_movement::jump_get_land_factor_stub()
{
	__asm
	{
		pushad

		push eax // ps
		call jump_get_land_factor
		add esp, 4

		popad
		ret
	}
}

__declspec(naked) void player_movement::pm_crash_land_stub_mp()
{
	static DWORD func = 0x41E6B0;

	__asm
	{
		push eax
		mov eax, jump_enableFallDamage
		cmp byte ptr [eax + 0xC], 0
		pop eax

		// If disabled just return
		jz skip_crash

		// Arguments are in the registers
		call func

	skip_crash:
		ret
	}
}

__declspec(naked) void player_movement::pm_crash_land_stub_sp()
{
	static DWORD func = 0x6405A0;

	__asm
	{
		push eax
		mov eax, jump_enableFallDamage
		cmp byte ptr [eax + 0xC], 0
		pop eax

		// If disabled just return
		jz skip_crash

		// ps is in the esi register
		push [esp + 0x4] // pml
		call func
		add esp, 4

	skip_crash:
		ret
	}
}

__declspec(naked) void player_movement::jump_get_step_height_stub()
{
	__asm
	{
		push eax
		mov eax, jump_stepSize
		fld dword ptr [eax + 0xC]
		pop eax

		jmp jump_get_step_height_addr
	}
}

__declspec(naked) void player_movement::jump_start_stub()
{
	__asm
	{
		push eax
		mov eax, jump_spreadAdd
		fadd dword ptr [eax + 0xC]
		pop eax

		jmp jump_start_addr
	}
}

void player_movement::pm_project_velocity_stub(const float* vel_in, const float* normal, float* vel_out)
{
	const auto length_squared_2d = vel_in[0] * vel_in[0] + vel_in[1] * vel_in[1];

	if (std::fabsf(normal[2]) < 0.001f || length_squared_2d == 0.0f)
	{
		std::memcpy(vel_out, vel_in, sizeof(std::float_t[3]));
		return;
	}

	auto new_z = vel_in[0] * normal[0] + vel_in[1] * normal[1];
	new_z = -new_z / normal[2];

	const auto length_scale = std::sqrtf((vel_in[2] * vel_in[2] + length_squared_2d) /
		(new_z * new_z + length_squared_2d));

	if (pm_bouncesAllAngles->current.enabled == true
		|| (length_scale < 1.f || new_z < 0.f || vel_in[2] > 0.f))
	{
		vel_out[0] = vel_in[0] * length_scale;
		vel_out[1] = vel_in[1] * length_scale;
		vel_out[2] = new_z * length_scale;
	}
}

__declspec(naked) void player_movement::bg_gravity_stub()
{
	__asm
	{
		push eax

		mov eax, bg_gravity
		// Quick and widely available extension since 1999
		cvttss2si eax, dword ptr [eax + 0xC]
		mov dword ptr [ebp + 0x58], eax // ps.gravity

		pop eax

		push 0x4F9BBA
		retn
	}
}

__declspec(naked) void player_movement::g_speed_stub()
{
	__asm
	{
		push eax

		mov eax, g_speed
		mov eax, dword ptr [eax + 0xC]
		mov dword ptr [ebp + 0x5C], eax // ps.speed

		pop eax

		push 0x4F93E4
		retn
	}
}

__declspec(naked) void player_movement::pm_cmd_scale_crawl_speed_stub()
{
	__asm
	{
		push eax
		mov eax, player_lastStandCrawlSpeedScale
		fld dword ptr [eax + 0xC]
		pop eax

		// Game's code
		pop ecx
		ret
	}
}

__declspec(naked) void player_movement::pm_cmd_scale_ducked_speed_stub()
{
	__asm
	{
		push eax
		mov eax, player_duckedSpeedScale
		fld dword ptr [eax + 0xC]
		pop eax

		// Game's code
		pop ecx
		ret
	}
}

__declspec(naked) void player_movement::pm_cmd_scale_prone_speed_stub()
{
	__asm
	{
		push eax
		mov eax, player_proneSpeedScale
		fld dword ptr [eax + 0xC]
		pop eax

		// Game's code
		pop ecx
		ret
	}
}

const game::native::dvar_t* player_movement::dvar_register_player_sustain_ammo(const char* dvar_name,
		bool value, unsigned __int16 /*flags*/, const char* description)
{
	player_sustainAmmo = game::native::Dvar_RegisterBool(dvar_name,
		value, game::native::DVAR_CODINFO, description);

	return player_sustainAmmo;
}

const game::native::dvar_t* player_movement::dvar_register_jump_ladder_push_vel(const char* dvar_name,
	float value, float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	jump_ladderPushVel = game::native::Dvar_RegisterFloat(dvar_name,
		value, min, max, game::native::DVAR_CODINFO, description);

	return jump_ladderPushVel;
}

const game::native::dvar_t* player_movement::dvar_register_jump_step_size(const char* dvar_name,
	float value, float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	jump_stepSize = game::native::Dvar_RegisterFloat(dvar_name,
		value, min, max, game::native::DVAR_CODINFO, description);

	return jump_stepSize;
}

const game::native::dvar_t* player_movement::dvar_register_jump_spread_add(const char* dvar_name,
	float value, float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	jump_spreadAdd = game::native::Dvar_RegisterFloat(dvar_name,
		value, min, max, game::native::DVAR_CODINFO, description);

	return jump_spreadAdd;
}

const game::native::dvar_t* player_movement::dvar_register_jump_slowdown_enable(const char* dvar_name,
	bool value, unsigned __int16 /*flags*/, const char* description)
{
	jump_slowdownEnable = game::native::Dvar_RegisterBool(dvar_name,
		value, game::native::DVAR_CODINFO, description);

	return jump_slowdownEnable;
}

const game::native::dvar_t* player_movement::dvar_register_jump_height(const char* dvar_name,
	float value, float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	jump_height = game::native::Dvar_RegisterFloat(dvar_name,
		value, min, max, game::native::DVAR_CODINFO, description);

	return jump_height;
}

void player_movement::patch_mp()
{
	pm_playerEjection = game::native::Dvar_RegisterBool("pm_playerEjection",
		true, game::native::DVAR_CODINFO, "Push intersecting players away from each other");
	// Name is correct, SP registers this dvar in BG_RegisterDvars but still names it just "g_gravity"
	bg_gravity = game::native::Dvar_RegisterFloat("g_gravity", 800.0f,
		1.0f, std::numeric_limits<float>::max(), game::native::DVAR_CODINFO, "Gravity in inches per second per second");
	g_speed = game::native::Dvar_RegisterInt("g_speed", 190,
		std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), game::native::DVAR_CODINFO, "Player speed");

	// Un-Cheat the dvars
	utils::hook(0x418D9C, dvar_register_player_sustain_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x4160A7, dvar_register_jump_ladder_push_vel, HOOK_CALL).install()->quick();
	utils::hook(0x41602B, dvar_register_jump_height, HOOK_CALL).install()->quick();
	utils::hook(0x416074, dvar_register_jump_slowdown_enable, HOOK_CALL).install()->quick();
	utils::hook(0x41605E, dvar_register_jump_step_size, HOOK_CALL).install()->quick();
	utils::hook(0x4160DA, dvar_register_jump_spread_add, HOOK_CALL).install()->quick();

	utils::hook(0x42B5DA, pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x42B2BD, pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x42AE95, pm_weapon_use_ammo, HOOK_CALL).install()->quick();

	utils::hook(0x424D51, pm_step_slide_move_stub, HOOK_JUMP).install()->quick();

	utils::hook(0x4F9EFB, stuck_in_client_stub, HOOK_CALL).install()->quick(); // ClientEndFrame
	utils::hook(0x57CF45, cm_transformed_capsule_trace_stub, HOOK_CALL).install()->quick(); // SV_ClipMoveToEntity
	utils::hook(0x482C1B, cm_transformed_capsule_trace_stub, HOOK_CALL).install()->quick(); // CG_ClipMoveToEntity

	utils::hook(0x530CCB, weapon_rocket_launcher_fire_mp_stub, HOOK_CALL).install()->quick(); // FireWeapon

	utils::hook(0x422861, pm_player_trace_stub, HOOK_CALL).install()->quick(); // PM_JitterPoint
	utils::hook(0x4228B5, pm_player_trace_stub, HOOK_CALL).install()->quick(); // PM_JitterPoint

	utils::hook(0x41F995, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck
	utils::hook(0x41F8D8, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck
	utils::hook(0x41F941, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck

	utils::hook(0x416866, jump_push_off_ladder_stub, HOOK_JUMP).install()->quick(); // Jump_Check
	utils::hook::nop(0x41686B, 1); // Nop skipped opcode

	// Modify third argument of Jump_Start with the value of jump_height dvar
	utils::hook(0x416969, jump_check_stub, HOOK_JUMP).install()->quick(); // Jump_Check
	utils::hook::nop(0x41696E, 1); // Nop skipped opcode

	utils::hook(0x4225CA, jump_apply_slowdown_stub, HOOK_CALL).install()->quick(); // PM_WalkMove
	utils::hook(0x41669B, jump_get_land_factor_stub, HOOK_CALL).install()->quick(); // Jump_Start

	utils::hook(0x422BE0, pm_crash_land_stub_mp, HOOK_CALL).install()->quick(); // PM_GroundTrace

	utils::hook(0x41613F, jump_get_step_height_stub, HOOK_JUMP).install()->quick(); // PM_StepSlideMove
	utils::hook::nop(0x416144, 1); // Nop skipped opcode

	// Modify the hardcoded value of the spread with the value of jump_spreadAdd
	utils::hook(0x4166F0, jump_start_stub, HOOK_JUMP).install()->quick();
	utils::hook::nop(0x4166F5, 1); // Nop skipped opcode

	utils::hook(0x424E0A, pm_project_velocity_stub, HOOK_CALL).install()->quick(); // PM_StepSlideMove

	utils::hook(0x4F9BB3, bg_gravity_stub, HOOK_JUMP).install()->quick(); // ClientEndFrame
	utils::hook::nop(0x4F9BB8, 2); // Nop skipped opcode

	utils::hook(0x4F93D7, g_speed_stub, HOOK_JUMP).install()->quick(); // ClientThink_real
	utils::hook::nop(0x4F93DC, 2); // Nop skipped opcode

	utils::hook(0x4220E5, pm_cmd_scale_crawl_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
	utils::hook(0x422104, pm_cmd_scale_ducked_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
	utils::hook(0x42210E, pm_cmd_scale_prone_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
}

void player_movement::patch_sp()
{
	player_sustainAmmo = game::native::Dvar_RegisterBool("player_sustainAmmo",
		false, game::native::DVAR_CODINFO, "Firing weapon will not decrease clip ammo");
	jump_ladderPushVel = game::native::Dvar_RegisterFloat("jump_ladderPushVel",
		128.0f, 0.0f, 1024.0f, game::native::DVAR_CODINFO, "The velocity of a jump off of a ladder");
	jump_stepSize = game::native::Dvar_RegisterFloat("jump_stepSize",
		18.0f, 0.0f, 64.0f, game::native::DVAR_CODINFO, "The maximum step up to the top of a jump arc");
	jump_spreadAdd = game::native::Dvar_RegisterFloat("jump_spreadAdd",
		64.0f, 0.0f, 512.0f, game::native::DVAR_CODINFO, "The amount of spread scale to add as a side effect of jumping");

	utils::hook(0x648C3A, pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x64891D, pm_weapon_use_ammo, HOOK_CALL).install()->quick();
	utils::hook(0x6484E2, pm_weapon_use_ammo, HOOK_CALL).install()->quick();

	utils::hook(0x43D918, pm_step_slide_move_stub, HOOK_JUMP).install()->quick();

	utils::hook(0x41F9A6, cm_transformed_capsule_trace_stub, HOOK_CALL).install()->quick(); // SV_ClipMoveToEntity
	utils::hook(0x57B14F, cm_transformed_capsule_trace_stub, HOOK_CALL).install()->quick(); // CG_ClipMoveToEntity

	utils::hook(0x45E606, weapon_rocket_launcher_fire_sp_stub, HOOK_CALL).install()->quick(); // FireWeapon

	utils::hook(0x643F84, pm_player_trace_stub, HOOK_CALL).install()->quick(); // PM_JitterPoint
	utils::hook(0x643FDB, pm_player_trace_stub, HOOK_CALL).install()->quick(); // PM_JitterPoint

	utils::hook(0x64181A, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck
	utils::hook(0x641701, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck
	utils::hook(0x6417A9, pm_trace_stub, HOOK_CALL).install()->quick(); // PM_CheckDuck

	utils::hook(0x63EA46, jump_push_off_ladder_stub, HOOK_JUMP).install()->quick(); // Jump_Check
	utils::hook::nop(0x63EA4B, 1); // Nop skipped opcode

	utils::hook(0x6442DF, pm_crash_land_stub_sp, HOOK_CALL).install()->quick(); // PM_GroundTrace

	utils::hook(0x48C1DC, jump_get_step_height_stub, HOOK_JUMP).install()->quick(); // PM_StepSlideMove
	utils::hook::nop(0x48C1E1, 1); // Nop skipped opcode

	// Modify the hardcoded value of the spread with the value of jump_spreadAdd
	utils::hook(0x63E90A, jump_start_stub, HOOK_JUMP).install()->quick();
	utils::hook::nop(0x63E90F, 1); // Nop skipped opcode

	utils::hook(0x43D9D1, pm_project_velocity_stub, HOOK_CALL).install()->quick(); // PM_StepSlideMove

	utils::hook(0x64384F, pm_cmd_scale_crawl_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
	utils::hook(0x643859, pm_cmd_scale_ducked_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
	utils::hook(0x643863, pm_cmd_scale_prone_speed_stub, HOOK_JUMP).install()->quick(); // PM_CmdScaleForStance
}

void player_movement::register_common_dvars()
{
	// pm dvars
	pm_bounces = game::native::Dvar_RegisterBool("pm_bounces", false,
		game::native::DVAR_CODINFO, "CoD4 Bounces");
	pm_bouncesAllAngles = game::native::Dvar_RegisterBool("pm_bouncesAllAngles", false,
		game::native::DVAR_CODINFO, "Force bounces from all angles");
	pm_playerCollision = game::native::Dvar_RegisterBool("pm_playerCollision",
		true, game::native::DVAR_CODINFO, "Push intersecting players away from each other");
	pm_elevators = game::native::Dvar_RegisterBool("pm_elevators",
		false, game::native::DVAR_CODINFO, "CoD4 Elevators");
	pm_rocketJump = game::native::Dvar_RegisterBool("pm_rocketJump",
		false, game::native::DVAR_CODINFO, "CoD4 rocket jumps");

	// Jump dvars
	jump_enableFallDamage = game::native::Dvar_RegisterBool("jump_enableFallDamage",
		true, game::native::DVAR_CODINFO, "Enable fall damage");

	// Player dvars
	player_lastStandCrawlSpeedScale = game::native::Dvar_RegisterFloat("player_lastStandCrawlSpeedScale", 0.2f,
		0.0f, 5.0f, game::native::DVAR_CODINFO, "The scale applied to the player speed when crawling in last stand");
	player_duckedSpeedScale = game::native::Dvar_RegisterFloat("player_duckedSpeedScale", 0.65f,
		0.0f, 5.0f, game::native::DVAR_CODINFO, "The scale applied to the player speed when ducking");
	player_proneSpeedScale = game::native::Dvar_RegisterFloat("player_proneSpeedScale", 0.15f,
		0.0f, 5.0f, game::native::DVAR_CODINFO, "The scale applied to the player speed when ducking");
}

void player_movement::post_load()
{
	// Un-cheat missileMacross. It seems it retained its functionality
	utils::hook::set<BYTE>(SELECT_VALUE(0x44DFED, 0x50DDDD, 0x48C16F), 0x0);

	if (game::is_dedi())
	{
		return;
	}

	bounce_addr = SELECT_VALUE(0x43D91F, 0x424D58, 0x0);
	dont_bounce_addr = SELECT_VALUE(0x43D933, 0x424D6C, 0x0);
	push_off_ladder_addr = SELECT_VALUE(0x63EA4C, 0x41686C, 0x0);
	jump_start_addr = SELECT_VALUE(0x63E910, 0x4166F6, 0x0);
	jump_get_step_height_addr = SELECT_VALUE(0x48C1E2, 0x416145, 0x0);

	this->register_common_dvars();

	if (game::is_mp()) this->patch_mp();
	else if (game::is_sp()) this->patch_sp();
}

REGISTER_MODULE(player_movement);
