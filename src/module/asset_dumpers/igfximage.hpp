#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct igfximage : asset_dumper
	{
		igfximage();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		static int store_texture_hk();
		static void release_texture_hk(game::native::XAssetHeader header);
	};
}