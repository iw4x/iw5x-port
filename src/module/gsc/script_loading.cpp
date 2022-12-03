#include <std_include.hpp>
#include "game/game.hpp"

#include "script_loading.hpp"

namespace gsc
{
	game::native::ScriptFile* find_script(game::native::XAssetType type, const char* name, int allow_create_default)
	{
		return game::native::DB_FindXAssetHeader(type, name, allow_create_default).scriptfile;
	}
}
