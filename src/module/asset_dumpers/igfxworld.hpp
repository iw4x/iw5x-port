#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "utils/stream.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct igfxworld : asset_dumper
	{
		igfxworld();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;
		
	private:
		iw4::native::GfxWorldDpvsStatic* convert(game::native::GfxWorld* world, game::native::GfxWorldDpvsStatic* dpvs_static);
		void write(const iw4::native::GfxWorldDpvsStatic* dpvs_static, utils::stream* buffer);
	};
}