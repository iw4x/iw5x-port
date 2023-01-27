#include <std_include.hpp>

#include "iscriptfile.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <xsk/gsc/types.hpp>
#include <xsk/gsc/interfaces/compiler.hpp>
#include <xsk/gsc/interfaces/assembler.hpp>
#include <xsk/gsc/interfaces/decompiler.hpp>
#include <xsk/gsc/interfaces/disassembler.hpp>
#include <xsk/utils/compression.hpp>
#include <xsk/resolver.hpp>
#include <interface.hpp>
#include "../gsc/script_loading.hpp"
#include <module/console.hpp>
#include "utils/hook.hpp"

namespace asset_dumpers
{

	void iscriptfile::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.scriptfile);

		auto iw4_rawfile = local_allocator.allocate<game::native::RawFile>();

		auto script = header.scriptfile;

		if (script->compressedLen > 0 && script->bytecodeLen > 0 && script->len > 0)
		{
			std::string script_name = get_script_name(script);
			std::string script_data = get_decompiled_script(script);

			/////////////////////
			/// quick GSC Fixups
			///
			if (script_name.ends_with("_fog.gsc"))
			{
				script_data = get_converted_fog(script_data);
			}
			else if (script_name.ends_with("_fx.gsc"))
			{
				if (script_name.starts_with("maps/createfx/"))
				{
					dump_create_fx_sounds(script_data);
					script_data = convert_to_strict_createfx(script_data);
				}
				else
				{
					dump_map_fx(script_data);
				}
			}
			else if (script_name.ends_with("_precache.gsc"))
			{
				dump_map_precache(script_data);
			}

			dump_ambient_play(script_data);
			script_data = add_fog_init(script_data);
			script_data = remove_compatibility_calls(script_data);
			///
			////////////////////////////


			iw4_rawfile->buffer = local_allocator.allocate_array<char>(script_data.size());
			memcpy(iw4_rawfile->buffer, script_data.data(), script_data.size());
			iw4_rawfile->len = script_data.size();
			iw4_rawfile->compressedLen = 0;
			iw4_rawfile->name = local_allocator.duplicate_string(script_name);
		}

		out.rawfile = exporter::dump(game::native::ASSET_TYPE_RAWFILE, {iw4_rawfile}).rawfile;
	}

	std::string iscriptfile::get_converted_fog(const std::string& original)
	{
		// Fog works completely differently in iw5 but the settings are very similar

		auto regex_str_template = "(?:{} *= *((?:[0-9]|\\.)*))";
		const std::string members[] =
		{
			"startdist", "halfwaydist", "red", "green" ,"blue", "maxopacity", "transitiontime"
		};

		std::stringstream regex_str{};

		auto i = 0;
		for (const auto& member : members)
		{
			regex_str << std::format(regex_str_template, member);
			i++;

			if (i != ARRAYSIZE(members))
			{
				regex_str << ";\n.*"; // Glue
			}
		}

		std::regex matcher(regex_str.str());

		std::stringstream final_string{};
		std::smatch m;

		final_string << "// _createart generated.  modify at your own risk. Changing values should be fine.\nmain()\n{\n\n";

		if (std::regex_search(original, m, matcher,  std::regex_constants::match_default))
		{
			if (m.size() == ARRAYSIZE(members) + 1)
			{

				final_string << "    setDevDvar( \"scr_fog_disable\", \"0\" );\n"; // For good measure

				auto i = 0;
				final_string << "    setExpFog(";
				for (auto match : m)
				{
					i++;

					if (i == 1)
					{
						continue;
					}

					final_string << match.str();

					if (i <= ARRAYSIZE(members))
					{
						final_string << ", ";
					}
				}

				final_string << " ); // Added by iw5xport\n";
			}
		}

		final_string << "}\n"; // and i believe we're done ?

		return final_string.str();
	}

	void iscriptfile::write(const iw4::native::XAssetHeader& header)
	{
		// We don't! iRawfile.cpp handles that
	}

	std::string iscriptfile::add_fog_init(const std::string& script)
	{
		const auto map_name = exporter::get_map_name();
		auto script_copy = script;

		// Add fog init
		auto load_offset = script_copy.find("maps\\mp\\_load::main\(\);\n");
		if (load_offset != std::string::npos)
		{
			script_copy = script_copy.insert(
				load_offset,
				std::format("maps\\createart\\{}_fog::main();// Added by iw5xport (iw4 can't make that call automatically)\n    ", map_name)
			);
		}

		return script_copy;
	}

	std::string iscriptfile::remove_compatibility_calls(const std::string& script)
	{
		// General removals (things that don't exist in IW4)
		static std::vector<std::pair<std::regex, std::string>> replacements
		{
			// Remove audio reverb setting
			{std::regex("maps\\\\mp\\\\_audio::"), "// Commented out by iw5xport\n    // $&" },

			// sunlight createart call
			{std::regex("setsunlight"), "// Commented out by iw5xport\n    // $&"},

			// faceoff maps have these calls to set up spawns
			{std::regex("maps\\\\mp\\\\gametypes\\\\faceoff"), "// Commented out by iw5xport\n    // $&"}
		};

		std::string script_cpy = script;

		for (auto replacement : replacements)
		{
			script_cpy = std::regex_replace(script_cpy, replacement.first, replacement.second);
		}

		return script_cpy;
	}

	void iscriptfile::dump_ambient_play(const std::string& script)
	{
		static std::regex ambientplay("ambientplay\\( \"(ambient_mp_seatown)\" \\);");
		std::smatch matches;

		if (std::regex_search(script.cbegin(), script.cend(), matches, ambientplay))
		{
			if (matches.size() > 1)
			{
				const auto& match = matches[1];
				auto sound_name = match.str();

				auto sound = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_SOUND, sound_name.data(), 0);

				if (sound.data)
				{
					exporter::dump(game::native::ASSET_TYPE_SOUND, sound);
					exporter::add_to_source(game::native::ASSET_TYPE_SOUND, sound_name);
				}
			}
		}
	}

	void iscriptfile::dump_map_fx(const std::string& script)
	{
		static std::regex fx_catcher("\\] = loadfx\\( *\"(.+)\" *\\);");
		std::smatch m;
		std::string::const_iterator search_start(script.cbegin());
		while (std::regex_search(search_start, script.cend(), m, fx_catcher))
		{
			bool skip = true;
			for (auto match : m)
			{
				if (skip)
				{
					skip = false;
					continue;
				}

				auto fx_name = match.str();
				auto fx = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_FX, fx_name.data(), 0);
				search_start = m.suffix().first;

				if (fx.data)
				{
					exporter::dump(game::native::ASSET_TYPE_FX, fx);
					exporter::add_to_source(game::native::ASSET_TYPE_FX, fx_name);
				}
			}
		}
	}

	void iscriptfile::dump_map_precache(const std::string& script)
	{
		static std::regex anim_script_catcher(" *(.*)::main\\(\\);");
		std::smatch anim_script_matches;

		std::string::const_iterator search_start(script.cbegin());
		while (std::regex_search(search_start, script.cend(), anim_script_matches, anim_script_catcher))
		{
			if (anim_script_matches.size() > 1)
			{
				const auto& match = anim_script_matches[1];
				auto script_name = std::format("{}", match.str());
				std::replace(script_name.begin(), script_name.end(), '\\', '/');
				auto token = xsk::gsc::iw5::resolver::token_id(script_name);
				auto obfuscated_name = token == 0 ? script_name : std::to_string(token);

				auto script = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_SCRIPTFILE, obfuscated_name.data(), 0);
				search_start = anim_script_matches.suffix().first;

				assert(script.data);
				if (script.data)
				{
					std::string raw_script = get_decompiled_script(script.scriptfile);
					std::string rawfile_name = get_script_name(script.scriptfile);

					raw_script = dump_rename_map_animtrees(raw_script);
					dump_map_animated_model_anim(raw_script);

					auto raw_file = local_allocator.allocate<game::native::RawFile>();
					raw_file->buffer = local_allocator.duplicate_string(raw_script);
					raw_file->len = raw_script.size();
					raw_file->compressedLen = 0;
					raw_file->name = local_allocator.duplicate_string(rawfile_name);

					exporter::dump(game::native::ASSET_TYPE_RAWFILE, { raw_file });
				}
			}
		}
	}

	std::string iscriptfile::dump_rename_map_animtrees(const std::string& script)
	{
		static std::regex animtree_catcher("#using_animtree\\(\"(.*)\"\\);");
		std::smatch m;
		
		std::stringstream result_builder{};

		std::string::const_iterator search_start(script.cbegin());
		while (std::regex_search(search_start, script.cend(), m, animtree_catcher))
		{
			if (m.size() > 1)
			{
				const auto& match = m[1];
				auto animtree_name = std::format("animtrees/{}.atr", match.str());
				auto animtree = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_RAWFILE, animtree_name.data(), 0);
				search_start = m.suffix().first;

				if (animtree.rawfile)
				{
					auto rawfile = exporter::dump(game::native::ASSET_TYPE_RAWFILE, animtree);
					auto new_name = std::string(rawfile.rawfile->name);
					exporter::add_to_source(game::native::ASSET_TYPE_RAWFILE, new_name);

					std::string short_name = new_name.substr(ARRAYSIZE("animtrees")); // Remove path
					short_name = short_name.substr(0, short_name.size() - 4); // Remove extension

					std::string file_so_far = m.prefix();
					std::string using_call = m[0].str();
					result_builder << file_so_far << std::format(
						"#using_animtree(\"{}\");", 
						short_name
					);
				}
				else
				{
					assert(false);
				}
			}
		}

		result_builder << m.suffix();

		std::string final_output = result_builder.str();

		// this is not necessary
#if false
		// We're not done!
		// IW5 anim scripts reference animtree elements with a percentage (%)
		// The problem is that we're in MP and we don't have access to SP anims
		// But they're still getting compiled, except we will miss the animtree
		// So if the map looks like it's gonna be MP/SP modular, we replace every animtree query we find
		static std::regex is_mp_sp_modular("utility::string_starts_with\\(.*, *\"mp_\" *\\)");
		if (std::regex_match(final_output, is_mp_sp_modular))
		{
			final_output = std::regex_replace(final_output, std::regex("%(.*);"), "\"$1\"");
		}
#endif

		return final_output;
	}

	void iscriptfile::dump_map_animated_model_anim(const std::string& script)
	{
		// We use % because it's used for MP and SP (on destructibles for instance it's for both)
		// but we also have to use "" for mp_specific anims
		static std::regex anim_catchers[] = {
			std::regex("%(.*);"),				// SP & MP sometimes
			std::regex("\"\\] *= \"*(.*)\";")		// MP only
		};
		
		for (size_t i = 0; i < ARRAYSIZE(anim_catchers); i++)
		{
			const auto anim_catcher = anim_catchers[i];
			std::smatch m;
			std::string::const_iterator search_start(script.cbegin());
			while (std::regex_search(search_start, script.cend(), m, anim_catcher))
			{
				if (m.size() > 1)
				{
					const auto& match = m[1];
					auto anim_name = match.str();
					auto anim = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_XANIMPARTS, anim_name.data(), 0);
					search_start = m.suffix().first;

					if (anim.data)
					{
						exporter::dump(game::native::ASSET_TYPE_XANIMPARTS, anim);
						exporter::add_to_source(game::native::ASSET_TYPE_XANIMPARTS, anim_name);
					}
				}
			}
		}
	}

	std::string iscriptfile::convert_to_strict_createfx(const std::string& script)
	{
		std::stringstream new_data;

		// Obligatory IW4 header
		new_data << "//_createfx generated. Do not touch!!\n#include common_scripts\\utility;\n#include common_scripts\\_createfx;\n\n";

		std::vector< std::string> to_remove{
			"common_scripts\\utility::" ,
			"common_scripts\\_createfx::"
		};

		std::unordered_map< std::string, std::string> to_replace{
			{"var_0", "ent"},
			{"createoneshoteffect", "createOneshotEffect"},
			{"createloopsound", "createLoopSound"},
			{"\"origin\"", " \"origin\" "},
			{"\"angles\"", " \"angles\" "},
			{"\"soundalias\"", " \"soundalias\" "},
			{"\"fxid\"", " \"fxid\" "},
			{"\"delay\"", " \"delay\" "},
			{"    ent = ", "\n    ent = "},
		};

		std::istringstream f(script);
		std::string line;
		auto i = 0;
		while (std::getline(f, line)) 
		{
			// Skip the two Xensik comments - i wish i could keep them, but iw4 cfx does not allow comments except the canonical one
			if (i > 2)
			{
				for (const auto& str : to_remove)
				{
					std::string::size_type position = line.find(str);

					if (position != std::string::npos)
					{
						line.erase(position, str.length());
					}
				}

				for (const auto& kv : to_replace)
				{
					line = std::regex_replace(line, std::regex(kv.first), kv.second);
				}

				new_data << line << '\n';
			}
			i++;

		}

		auto result = new_data.str();
		return result;
	}

	std::string iscriptfile::get_decompiled_script(const game::native::ScriptFile* script)
	{
		std::string script_name = get_script_name(script);

		auto data_compressed = std::vector<unsigned char>();
		data_compressed.assign(script->buffer, script->buffer + script->compressedLen);

		auto data_decompressed = xsk::utils::zlib::decompress(data_compressed, script->len);
		auto decompressed_length = data_decompressed.size();
		assert(decompressed_length == script->len);

		std::string bytecode(reinterpret_cast<char*>(script->bytecode), script->bytecodeLen);

		std::vector<uint8_t> script_xsk(script->bytecodeLen);
		memcpy(&script_xsk.front(), script->bytecode, script->bytecodeLen);

		std::vector<uint8_t> stack_xsk(decompressed_length);
		memcpy(&stack_xsk.front(), data_decompressed.data(), decompressed_length);

		auto dis = gsc::disassembler();
		dis->disassemble(script_name, script_xsk, stack_xsk);

		auto out_compiled = dis->output();

		auto dec = gsc::decompiler();
		dec->decompile(script_name, out_compiled);

		auto final_output_vector = dec->output();
		std::string final_output = std::string(reinterpret_cast<char*>(final_output_vector.data()), final_output_vector.size());

		return final_output;
	}

	std::string iscriptfile::get_script_name(const game::native::ScriptFile* script)
	{
		std::string script_name = script->name;

		// This name has been mangled!
		if (script_name[0] <= '9')
		{
			script_name = xsk::gsc::iw5::resolver::token_name(std::stoi(script_name));
		}

		return std::format("{}.gsc", script_name);
	}

	void iscriptfile::dump_create_fx_sounds(const std::string& script)
	{
		static std::regex sound_catcher("(?:\\.v\\[\"soundalias\"\\] *= *\"(.+)\")");
		std::smatch m;

		std::string::const_iterator search_start(script.cbegin());
		while (std::regex_search(search_start, script.cend(), m, sound_catcher))
		{
			bool skip = true;
			for (auto match : m)
			{
				if (skip)
				{
					skip = false;
					continue;
				}

				auto sound_name = match.str();
				auto sound = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_SOUND, sound_name.data(), 0);
				search_start = m.suffix().first;

				if (sound.data)
				{
					exporter::dump(game::native::ASSET_TYPE_SOUND, sound);
					exporter::add_to_source(game::native::ASSET_TYPE_SOUND, sound_name);
				}
			}
		}
	}

	iscriptfile::iscriptfile()
	{
		command::add("dumpScript", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];
				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, name);

				if (entry)
				{
					auto rawfile = dump(entry->asset.header).rawfile;
					exporter::dump(game::native::ASSET_TYPE_RAWFILE, { rawfile });
					exporter::add_to_source(game::native::ASSET_TYPE_RAWFILE, rawfile->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n", params.get(1));
				}
				
			});
	}
}