#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include "log_file.hpp"

#include <utils/hook.hpp>

class fov final : public module
{
public:
	void post_load() override
	{
		// Set dvar flag
		utils::hook::set<BYTE>(SELECT_VALUE(0x4302C5, 0x455155), game::native::DVAR_ARCHIVE | (game::is_mp() ? game::native::DVAR_SAVED : 0));

		if (game::is_mp())
		{
			// Set dvar limit
			static const auto cg_fov_limit = 120.0f;
			utils::hook::set(0x455148, &cg_fov_limit);

			// Prevent value change via internal scripts
			utils::hook(0x4698BA, &set_server_command_dvar_stub, HOOK_CALL).install()->quick();
		}
	}

private:
	static void set_server_command_dvar_stub(const char* dvar_name, const char* value)
	{
		const auto* dvar = game::native::Dvar_FindVar(dvar_name);
		if (dvar != nullptr && ((dvar->flags & game::native::DVAR_ARCHIVE) != 0))
		{
			log_file::info("Not allowing server to override archive dvar '%s'\n", dvar_name);
			return;
		}

		game::native::Dvar_SetFromStringByName(dvar_name, value);
	}
};

REGISTER_MODULE(fov)
