#include <std_include.hpp>
#include "game/game.hpp"
#include "asset_dumper.hpp"

#include "api.hpp"

asset_dumper::asset_dumper()
{
	export_path_dvar = game::native::Dvar_FindVar("export_path");
}
