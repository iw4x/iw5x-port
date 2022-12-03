#pragma once

namespace gsc
{
	game::native::ScriptFile* find_script(game::native::XAssetType type, const char* name, int allow_create_default);
}
