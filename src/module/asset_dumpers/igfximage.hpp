#pragma once

#include "game/structs.hpp"
#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"

namespace asset_dumpers
{
	struct igfximage
	{
		igfximage();
		static void dump(game::native::XAssetType, game::native::XAssetHeader header);

	private:
		static void dump(game::native::GfxImage* image);
	};
}