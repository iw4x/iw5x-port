#pragma once

namespace gsc
{
	extern std::uint16_t scr_func_max_id;

	game::native::ScriptFile* find_script(game::native::XAssetType type, const char* name, int allow_create_default);
}
