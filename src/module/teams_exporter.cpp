#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/structs.hpp"
#include "game/game.hpp"

#include "utils/io.hpp"

#include "module/console.hpp"

#include <module/asset_dumpers/iscriptfile.hpp>

#include "xsk/gsc/types.hpp"
#include "xsk/resolver.hpp"

#include "rapidjson/document.h"
#include <rapidjson/prettywriter.h>

#include "teams_exporter.hpp"


constexpr auto FACTION_REF_COL = 0;
constexpr auto  FACTION_NAME_COL = 1;
constexpr auto  FACTION_SHORT_NAME_COL = 2;
constexpr auto  FACTION_ELIMINATED_COL = 3;
constexpr auto  FACTION_FORFEITED_COL = 4;
constexpr auto  FACTION_ICON_COL = 5;
constexpr auto  FACTION_HUD_ICON_COL = 6;
constexpr auto  FACTION_VOICE_PREFIX_COL = 7;
constexpr auto  FACTION_SPAWN_MUSIC_COL = 8;
constexpr auto  FACTION_WIN_MUSIC_COL = 9;
constexpr auto  FACTION_FLAG_MODEL_COL = 10;
constexpr auto  FACTION_FLAG_CARRY_MODEL_COL = 11;
constexpr auto  FACTION_FLAG_ICON_COL = 12;
constexpr auto  FACTION_FLAG_FX_COL = 13;
constexpr auto  FACTION_COLOR_R_COL = 14;
constexpr auto  FACTION_COLOR_G_COL = 15;
constexpr auto  FACTION_COLOR_B_COL = 16;
constexpr auto  FACTION_HEAD_ICON_COL = 17;
constexpr auto  FACTION_CRATE_MODEL_COL = 18;

void teams_exporter::export_team(const std::string& team)
{
	const auto string_table =
		game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_STRINGTABLE, "mp/factionTable.csv", false).stringTable;

	const auto export_path = exporter::get_export_path();
	utils::io::create_directory(export_path);

	if (string_table)
	{
		const auto table_row_num = game::native::StringTable_LookupRowNumForValue(string_table, FACTION_REF_COL, team.data());

		if (table_row_num < 0)
		{
			console::warn("Could not find team {} in faction table\n", team.data());
			return;
		}

		// Localization
		const auto fname = std::format("{}/{}.str", export_path, team);
		std::ofstream output_localization(fname, std::ios_base::binary | std::ios_base::trunc);
		if (output_localization.is_open())
		{
			for (int i = FACTION_NAME_COL; i <= FACTION_FORFEITED_COL; i++)
			{
				const auto key = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, i);
				const auto value = game::native::UI_SafeLocalizeTextMessage(key);
				output_localization << "REFERENCE			" << key << "\n" << "LANG_ENGLISH		" << value << "\n\n";
			}
		}

		// Materials/icons
		for (int i = FACTION_ICON_COL; i <= FACTION_HUD_ICON_COL; i++)
		{
			const auto mat = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, i);
			command::execute(std::format("dumpMaterial {}", mat), true);
		}

		// Sound/voices
		char* prefix = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, FACTION_VOICE_PREFIX_COL);
		std::string lowercase_prefix = prefix;
		for (auto& c : lowercase_prefix) c = static_cast<char>(tolower(c));

		const std::string lowercase_new_prefix = std::format("iw5_{}", lowercase_prefix);
		const std::string new_prefix = std::format("IW5_{}", prefix);

		std::vector<std::string> sounds_to_dump{};

		struct {
			char* prefix;
			std::vector<std::string>* sounds_to_dump;
		} userdata{ prefix , &sounds_to_dump };

		game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_SOUND, [](game::native::XAssetHeader h, void* passed_userdata)
			{
				if (h.data)
				{
					std::string asset_name = h.sound->aliasName;
					for (auto& c : asset_name) c = static_cast<char>(toupper(c));

					const auto payload = reinterpret_cast<decltype(userdata)*>(passed_userdata);

					std::string str_prefix = payload->prefix;
					for (auto& c : str_prefix) c = static_cast<char>(toupper(c));

					if (asset_name.starts_with(str_prefix))
					{
						payload->sounds_to_dump->push_back(h.sound->aliasName);
					}
				}

			}, &userdata, false);

		for (const auto& sound_name : sounds_to_dump)
		{
			command::execute(std::format("dumpSound {}", sound_name), true);
		}

		// Spawn sounds/musics
		// Don't use factiontable.csv for that, these sounds are NOT USED since MW2 !
		// You need to do that instead
		command::execute(std::format("dumpSound {}spawn_music", lowercase_prefix), true);
		command::execute(std::format("dumpSound {}defeat_music", lowercase_prefix), true);
		command::execute(std::format("dumpSound {}victory_music", lowercase_prefix), true);
		command::execute(std::format("dumpSound {}losing_music", lowercase_prefix), true);
		command::execute(std::format("dumpSound {}winning_music", lowercase_prefix), true);

		// Ingame models
		const int models[] = {
			FACTION_FLAG_MODEL_COL, FACTION_FLAG_CARRY_MODEL_COL, FACTION_CRATE_MODEL_COL
		};

		for (int index = 0; index < ARRAYSIZE(models); index++)
		{
			const auto val = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, models[index]);
			command::execute(std::format("dumpXModel {}", val), true);
		}

		// Img
		const auto img = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, FACTION_HEAD_ICON_COL);
		command::execute(std::format("dumpMaterial {}", img), true);

		// flag fx for CDD
		const auto fx = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, FACTION_FLAG_FX_COL);
		command::execute(std::format("dumpFX {}", fx), true);

		// Generate faction CSV
		const auto csv_name = std::format("{}/factionTable.csv", export_path);
		std::ofstream output_csv(csv_name, std::ios_base::binary | std::ios_base::trunc);
		if (output_csv.is_open())
		{
			for (int i = FACTION_REF_COL; i <= FACTION_CRATE_MODEL_COL; i++)
			{
				const auto value = game::native::StringTable_GetColumnValueForRow(string_table, table_row_num, i);

				switch (i)
				{

					// The following two are completely unused since MW2 actually, so... not very important but anyway
					// Changed in mw3
				case FACTION_SPAWN_MUSIC_COL:
					output_csv << lowercase_new_prefix << "spawn_music";
					break;

					// Changed in mw3
				case FACTION_WIN_MUSIC_COL:
					output_csv << lowercase_new_prefix << "victory_music";
					break;

				case FACTION_VOICE_PREFIX_COL:
					output_csv << new_prefix;
					break;

				default:
					output_csv << value;
					break;
				}

				if (i != FACTION_CRATE_MODEL_COL)
				{
					output_csv << ",";
				}
			}
		}

		parse_dump_bodytypes(team);
		decide_fallback(lowercase_prefix);

		auto source_contents = exporter::get_source();
		change_soundaliases_prefix(source_contents, prefix, new_prefix);
		change_soundaliases_prefix(source_contents, lowercase_prefix, lowercase_new_prefix);
		utils::io::write_file(std::format("{}/{}.csv", export_path, team), source_contents, false);

		console::info("successfully dumped team %s!\n", team.data());
	}
}

void teams_exporter::change_soundaliases_prefix(std::string& csv_contents, const std::string& old_prefix, const std::string& new_prefix)
{
	// Nightmare time
	const auto& soundaliases_path = std::format("{}/sounds", exporter::get_export_path());

	for (const auto& entry : std::filesystem::directory_iterator(soundaliases_path))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}

		const auto& full_path = entry.path();
		const auto& file_name = entry.path().filename().string();


		if (file_name.starts_with(old_prefix))
		{
			const auto& contents = utils::io::read_file(full_path.string());
			rapidjson::Document document{};
			document.Parse(contents.data(), contents.size());

			const std::string& current_name = document["aliasName"].GetString();
			const auto& new_name = std::format("{}{}", new_prefix, current_name.substr(old_prefix.size()));

			document["aliasName"] = RAPIDJSON_STR(new_name.data());


			rapidjson::StringBuffer buff;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
			document.Accept(writer);

			const auto& dump = buff.GetString();
			const auto& corrected_path = std::format("{}/{}.json", full_path.parent_path().string(), new_name);

			utils::io::write_file(corrected_path, dump);
			std::filesystem::remove(full_path);
		}
	}

	const std::regex prefixed_sound_catcher(std::format("(sound,){}(.*)", old_prefix));
	const auto replace_pattern = std::format("{}{}{}", "$1", new_prefix, "$2");

	csv_contents = std::regex_replace(csv_contents, prefixed_sound_catcher,replace_pattern);
}

void teams_exporter::decide_fallback(const std::string& vocal_prefix)
{
	static const std::string mw2_voice_prefixes[] = {
		"uk_", "ns_", "pg_", "ru_", "us_", "ab_"
	};

	std::string fallback = mw2_voice_prefixes[0];

	for (const std::string& mw2_prefix : mw2_voice_prefixes)
	{
		if (vocal_prefix == mw2_prefix)
		{
			fallback = mw2_prefix;
			break;
		}
	}

	const auto export_path = exporter::get_export_path();
	utils::io::create_directory(export_path);
	utils::io::write_file(std::format("{}/iw4_voice_fallback.txt", export_path), fallback);
}

void teams_exporter::parse_dump_bodytypes(const std::string& team_name)
{
	auto id = xsk::gsc::iw5::resolver::token_id("maps/mp/gametypes/_teams");
	std::string teams_script_name;

	if (id)
	{
		teams_script_name = std::to_string(id);
	}
	else {
		return;
	}

	const auto iw5_header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, teams_script_name.data(), 0);

	const auto export_path = exporter::get_export_path();
	utils::io::create_directory(export_path);

	if (iw5_header.data)
	{
		const auto iw4_rf = exporter::convert(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, iw5_header);
		const auto file = std::string(iw4_rf.rawfile->buffer, iw4_rf.rawfile->len);

		std::string gsc_block{};

		{
			const std::regex whole_block_matcher(std::format("(case \"{}\":\n(?:.*;\n)+? *break;)", team_name));
			std::smatch m;
			if (std::regex_search(file, m, whole_block_matcher))
			{
				if (m.size() > 0)
				{
					gsc_block = m.str();
				}
			}
		}
		

		// Generate faction GSC
		if (gsc_block.size())
		{
			{
				gsc_block = std::regex_replace(gsc_block, std::regex("var_0"), "team");

				const auto gsc_name = std::format("{}/_teams.gsc", export_path, team_name);
				utils::io::write_file(gsc_name, gsc_block);
			}

			std::vector<std::string> scripts_to_dump{};

			{
				std::string::const_iterator search_start(gsc_block.cbegin());
				const std::regex bodytype_matcher(std::format("(mptype\\\\mptype_{}_[a-z]+)::[a-z]+\\(\\);", team_name));
				std::smatch m;
				while (std::regex_search(search_start, gsc_block.cend(), m, bodytype_matcher))
				{
					if (m.size() > 1)
					{
						auto script_name = m[1].str();

						std::replace(script_name.begin(), script_name.end(), '\\', '/');

						scripts_to_dump.push_back(script_name);
					}

					search_start = m.suffix().first;
				}
			}

			for (const auto& script_name : scripts_to_dump)
			{
				auto script_id = xsk::gsc::iw5::resolver::token_id(script_name);
				std::string script_obfuscated_name;
				if (script_id)
				{
					script_obfuscated_name = std::to_string(script_id);
					command::execute(std::format("dumpScript {}", script_obfuscated_name), true);
				}
				else {
					continue;
				}
			}
		}
	}
}

REGISTER_MODULE(teams_exporter)
