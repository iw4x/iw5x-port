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
		iw4::native::GfxLightGrid convert(game::native::GfxWorld* native_world, const game::native::GfxLightGrid& native_lightgrid);
		iw4::native::GfxWorldDpvsStatic convert(game::native::GfxWorld* world, const game::native::GfxWorldDpvsStatic& dpvs_static);
		iw4::native::GfxWorldDraw convert(game::native::GfxWorld* world, const game::native::GfxWorldDraw& dpvs_static);
		void write(const iw4::native::GfxWorldDraw* dpvs_static, utils::stream* buffer);
		void write(const iw4::native::GfxWorld* world, const iw4::native::GfxWorldDpvsStatic* dpvs_static, utils::stream* buffer);
		void write(const iw4::native::GfxLightGrid* asset, utils::stream* buffer);
		void write(const iw4::native::GfxWorld* world, const game::native::GfxWorldDpvsPlanes* asset, utils::stream* buffer);
	};
}