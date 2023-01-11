#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct isndalias : asset_dumper
	{
		isndalias();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		static const std::unordered_map<unsigned int, unsigned int> channel_mapping;
	};
}