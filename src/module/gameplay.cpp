#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>

#include "game/game.hpp"

static const game::native::dvar_t** com_fixedtime = nullptr;

static const game::native::dvar_t* com_timescale = nullptr;
static const game::native::dvar_t* dev_timescale = nullptr;

static const game::native::dvar_t* dvar_register_com_timescale(const char* dvar_name, float value,
	float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	// Remove DVAR_ROM & DVAR_CHEAT
	com_timescale = game::native::Dvar_RegisterFloat(dvar_name, value,
		min, max, game::native::DVAR_CODINFO | game::native::DVAR_SAVED, description);

	return com_timescale;
}

static const game::native::dvar_t* dvar_register_dev_timescale(const char* dvar_name, float value,
	float min, float max, unsigned __int16 /*flags*/, const char* description)
{
	// Remove DVAR_CHEAT
	dev_timescale = game::native::Dvar_RegisterFloat(dvar_name, value,
		min, max, game::native::DVAR_CODINFO, description);

	return dev_timescale;
}

static float com_get_timescale_for_snd()
{
	const auto fixed_time = (*com_fixedtime)->current.integer;
	if (fixed_time != 0)
	{
		return static_cast<float>(fixed_time);
	}

	return dev_timescale->current.value * com_timescale->current.value;
}

static float com_get_timescale_for_sv()
{
	assert(com_timescale != nullptr);

	if (dev_timescale->current.value == 1.0f && com_timescale->current.value == 1.0f
		&& *game::native::com_codeTimeScale == 1.0f)
	{
		return 1.0f;
	}

	return dev_timescale->current.value * com_timescale->current.value * (*game::native::com_codeTimeScale);
}

class gameplay final : public module
{
public:
	void post_load() override
	{
		// com_timescale && timescale are already implemented on SP
		if (game::is_mp())
		{
			com_fixedtime = reinterpret_cast<const game::native::dvar_t**>(0x1CEF568);
			utils::hook(0x55419F, &dvar_register_com_timescale, HOOK_CALL).install()->quick();
			utils::hook(0x5541CF, &dvar_register_dev_timescale, HOOK_CALL).install()->quick();

			utils::hook(0x6C66CB, &com_get_timescale_for_snd, HOOK_CALL).install()->quick();
			utils::hook(0x6A71E0, &com_get_timescale_for_sv, HOOK_JUMP).install()->quick();
		}
	}
};

REGISTER_MODULE(gameplay)
