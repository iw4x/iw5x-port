#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct irawfile : asset_dumper
	{
		irawfile();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void saturate_vision(game::native::RawFile* rawfile);
		void write(const iw4::native::XAssetHeader& header) override;
	};
}