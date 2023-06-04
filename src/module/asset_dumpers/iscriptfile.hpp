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
		std::string get_script_name(const game::native::ScriptFile* script) const;
		std::string get_obfuscated_string(const std::string& name) const;
		void dump_rename_common_scripts();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		std::string get_converted_fog(const std::string& original);
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		const static std::string includes_to_rename[];
		static std::string common_scripts_to_dump[];
		static std::string regex_ready_includes_to_rename[];

		void dump_character_related_script_recursively(const std::string& obfuscated_name);
		std::string add_fog_init(const std::string& script);
		std::string general_compatibility_fixes(const std::string& script);
		std::string rename_map_animtrees(const std::string& script);
		std::string convert_to_strict_createfx(const std::string& script);
		std::string rename_common_imports(const std::string& script);
		std::string get_decompiled_script(const game::native::ScriptFile* script);
	};
}