#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include <utils/hook.hpp>

#include "game/game.hpp"

static char* com_get_build_version_stub()
{
	static char buf[128];

	const auto version_number = SELECT_VALUE(0x1CD, 0x5EC0E, 0x5EC0E);

	_snprintf_s(buf, _TRUNCATE, "%d %s", version_number, __DATE__);

	return buf;
}

static int com_get_build_version_dedi_stub(char* buf, const char* fmt, int version_number, const char* /*date*/)
{
	return _snprintf_s(buf, 0x80, _TRUNCATE, fmt, version_number, __DATE__);
}

class branding final : public module
{
public:
	void post_load() override
	{
		if (game::is_dedi()) this->patch_dedi();
		else this->add_branding();

		// gamedate dvar
		utils::hook::set<const char*>(SELECT_VALUE(0x5C223B, 0x50B0F4, 0x48844F), __DATE__);
	}

	static void patch_dedi()
	{
		utils::hook(0x4DAB99, com_get_build_version_dedi_stub, HOOK_CALL).install()->quick();
	}

	static void add_branding()
	{
		utils::hook(SELECT_VALUE(0x50BBD0, 0x53B4B0, 0x0), com_get_build_version_stub, HOOK_JUMP).install()->quick();
	}
};

REGISTER_MODULE(branding)
