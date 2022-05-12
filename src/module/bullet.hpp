#pragma once

class bullet final : public module
{
public:
	void post_load() override;

private:
	static const game::native::dvar_t* bg_bulletRange;
	static const game::native::dvar_t* bg_surfacePenetration;

	static DWORD bullet_fire_addr;
	static void bullet_fire_stub();

	static utils::hook::detour bg_get_surface_penetration_depth_hook;
	static float bg_get_surface_penetration_depth_stub(const game::native::Weapon weapon, bool isAlternate, int surfaceType);
};
