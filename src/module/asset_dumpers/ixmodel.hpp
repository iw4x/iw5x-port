#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "utils/stream.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct ixmodel : asset_dumper
	{
		ixmodel();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

		
	private:
		iw4::native::XSurface convert(const game::native::XSurface* source);

		void write(const game::native::XSurfaceCollisionTree* entry, utils::stream* buffer);
		void write(const iw4::native::XSurface* surf, utils::stream* buffer);
		void write(const iw4::native::XModelSurfs* asset, utils::stream* buffer);
	};
}