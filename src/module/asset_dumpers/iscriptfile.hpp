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
		std::string get_script_name(const game::native::ScriptFile* script);

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		std::string get_converted_fog(const std::string& original);
		void write(const iw4::native::XAssetHeader& header) override;
		std::string add_fog_init(const std::string& script);
		std::string remove_compatibility_calls(const std::string& script);
		void dump_ambient_play(const std::string& script);
		void dump_map_fx(const std::string& script);
		void dump_map_precache(const std::string& script);
		std::string dump_rename_map_animtrees(const std::string& script);
		void dump_map_animated_model_anim(const std::string& script);
		void dump_create_fx_sounds(const std::string& script);
		std::string convert_to_strict_createfx(const std::string& script);
		std::string get_decompiled_script(const game::native::ScriptFile* script);
	};
}