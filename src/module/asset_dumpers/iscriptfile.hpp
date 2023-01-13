#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct iscriptfile : asset_dumper
	{
		iscriptfile();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		std::string get_converted_fog(const std::string& original);
		void write(const iw4::native::XAssetHeader& header) override;
		void dump_map_fx(const std::string& script);
		void patch_reference(std::string& data, std::string _old, std::string _new);
		void dump_create_fx_sounds(const std::string& script);
		std::string convert_to_strict_createfx(const std::string& script);
	};
}