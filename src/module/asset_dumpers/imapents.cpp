#include <std_include.hpp>

#include "imapents.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <xsk/utils/compression.hpp>
#include <module/console.hpp>

namespace asset_dumpers
{
	// I don't have time to figure out why SCR_OPAQUE_STRING don't match what I get. Do you ?
	static const std::unordered_map<int, std::string> map_ents_table
	{
		{ 1668, "classname"},
		{ 1669, "origin" },
		{ 1670, "model" },
		{ 1671, "spawnflags" },
		{ 1672, "target" },
		{ 1673, "targetname" },
		{ 1676, "dmg" },
		{ 1677, "angles" },
		{ 1679, "script_linkname" },
		{ 1705, "intensity" },
		{ 1774, "script_noteworthy" },
		{ 1775, "speed" },
		{ 1776, "lookahead" },
		{ 1782, "radius" },
		{ 1783, "height" },
		{ 1788, "script_speed" },
		{ 1987, "ambient" },
		{ 1989, "sunlight" },
		{ 1990, "suncolor" },
		{ 1991, "sundirection" },
		{ 2009, "script_exploder" },
		{ 2328, "script_linkto" },
		{ 2369, "destructible_type" },
		{ 2810, "sunradiosity" },
		{ 2811, "skycolor" },
		{ 2812, "skylight" },
		{ 2813, "_color" },
		{ 2814, "ltorigin" },
		{ 2815, "gndlt" },
		{ 2816, "sound_csv_include" },
		{ 2817, "csv_include" },
		{ 2818, "precache_script" },
		{ 2820, "maxbounces" },
		{ 2821, "radiosityscale" },
		{ 2823, "def" },
		{ 2827, "__smorigin" },
		{ 2828, "__smangles" },
		{ 2829, "__smname" },
		{ 2830, "__smid" },
		{ 3717, "script_destruct_collision" },
		{ 4630, "script_bombmode_original" },
		{ 7876, "script_accel" },
		{ 10338, "script_targetoffset_z" },
		{ 10396, "script_airspeed" },
		{ 11848, "script_gameobjectname" },
		{ 11996, "script_label" },
	};

	void imapents::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.mapEnts);

		auto iw4_ents = local_allocator.allocate<iw4::native::MapEnts>();

		static_assert(sizeof(iw4::native::MapEnts) < sizeof (game::native::MapEnts));
		memcpy(iw4_ents, header.mapEnts, sizeof(iw4::native::MapEnts));

		// We need to patch the string cause we don't really... use anything besides that
		// I think the rest is runtime data
		std::string iw5_ents_string = iw4_ents->entityString;

		std::stringstream iw4_ents_string;

		bool in_key = false;
		std::stringstream key_buffer{};
		for (size_t i = 0; i < iw5_ents_string.size(); i++)
		{
			if (iw5_ents_string[i] == ' ')
			{
				if (in_key)
				{
					auto i_key = std::stoi(key_buffer.str());
					auto decoded_key = map_ents_table.contains(i_key) ? map_ents_table.at(i_key) : std::format("unk_{}", i);
					iw4_ents_string << "    " << decoded_key << ' ';
					key_buffer.str("");
					key_buffer.clear();
					in_key = false;
					continue;
				}
			}
			else if (iw5_ents_string[i] == '\n')
			{
				in_key = true;
			}
			else if (in_key && iw5_ents_string[i] != '{' && iw5_ents_string[i] != '}')
			{
				key_buffer << iw5_ents_string[i];
				continue;
			}

			iw4_ents_string << iw5_ents_string[i];
		}

		auto str = iw4_ents_string.str();
		iw4_ents->entityString = local_allocator.duplicate_string(str);
		iw4_ents->numEntityChars = str.size();

		out.mapEnts = iw4_ents;
	}

	void imapents::write(const iw4::native::XAssetHeader& header)
	{
		std::string basename(header.mapEnts->name);
		basename.erase(0, 8);
		basename.erase(basename.end() - 7, basename.end());

		utils::io::write_file(std::format("{}/mapents/{}.ents", get_export_path(), basename), header.mapEnts->entityString);
	}

	imapents::imapents()
	{
	}
}