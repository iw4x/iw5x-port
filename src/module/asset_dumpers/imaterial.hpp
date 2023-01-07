#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader/module.hpp"

#include "rapidjson/document.h"

#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct imaterial : asset_dumper
	{
		imaterial();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;
		
		rapidjson::Value statebits_to_json_array(game::native::GfxStateBits* stateBits, unsigned char count, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
	};
}