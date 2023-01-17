#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct iclipmap : asset_dumper
	{
		iclipmap();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;
		void write_binary(const iw4::native::XAssetHeader& header);
		void write_plaintext(const iw4::native::XAssetHeader& header);
	};
}