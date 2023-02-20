#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader/module.hpp"
#include "utils/stream.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct ifx : asset_dumper
	{
		ifx();

	protected:
		void convert(const game::native::FxElemVisuals* visuals, iw4::native::FxElemVisuals* into, char elemType);
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;
	};
}