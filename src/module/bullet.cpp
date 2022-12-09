#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include <utils/hook.hpp>

#include "bullet.hpp"

const game::native::dvar_t* bullet::bg_bulletRange;
const game::native::dvar_t* bullet::bg_surfacePenetration;

DWORD bullet::bullet_fire_addr;

utils::hook::detour bullet::bg_get_surface_penetration_depth_hook;

__declspec(naked) void bullet::bullet_fire_stub()
{
	__asm
	{
		push eax
		mov eax, bg_bulletRange
		fld dword ptr [eax + 0xC] //dvar_t.current
		pop eax

		jmp bullet_fire_addr
	}
}

float bullet::bg_get_surface_penetration_depth_stub(const game::native::Weapon weapon, bool is_alternate, int surface_type)
{
	const auto value = bg_surfacePenetration->current.value;
	if (value > 0.0f)
	{
		return value;
	}

	return bg_get_surface_penetration_depth_hook.invoke<float>(weapon, is_alternate, surface_type);
}

void bullet::post_load()
{
	bg_bulletRange = game::native::Dvar_RegisterFloat("bg_bulletRange", 8192.0f, 0.0f,
		std::numeric_limits<float>::max(), game::native::DVAR_CODINFO,
		"Max range used when calculating the bullet end position");

	bg_surfacePenetration = game::native::Dvar_RegisterFloat("bg_surfacePenetration", 0.0f,
		0.0f, std::numeric_limits<float>::max(), game::native::DVAR_CODINFO,
		"Set to a value greater than 0 to override the surface penetration depth");

	bullet_fire_addr = SELECT_VALUE(0x5B6442, 0x4F6C5C);
	utils::hook(SELECT_VALUE(0x5B643C, 0x4F6C56), &bullet_fire_stub, HOOK_JUMP).install()->quick();

	bg_get_surface_penetration_depth_hook.create(SELECT_VALUE(0x43BDE0, 0x42F4D0), &bg_get_surface_penetration_depth_stub);
}

REGISTER_MODULE(bullet)
