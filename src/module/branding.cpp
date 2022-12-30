#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include <utils/hook.hpp>

#include "game/game.hpp"

static char* com_get_build_version_stub()
{
	static char buf[128];

	const auto version_number = SELECT_VALUE(0x1CD, 0x5EC0E);

	sprintf_s(buf, _TRUNCATE, "%d %s", version_number, __DATE__);

	return buf;
}

class branding final : public module
{
public:
	void post_load() override
	{
		add_branding();

		// gamedate dvar
		utils::hook::set<const char*>(SELECT_VALUE(0x5C223B, 0x50B0F4), __DATE__);
	}

	static void add_branding()
	{
		utils::hook(SELECT_VALUE(0x50BBD0, 0x53B4B0), com_get_build_version_stub, HOOK_JUMP).install()->quick();
	}
};

REGISTER_MODULE(branding)
