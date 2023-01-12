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
#include <module/console.hpp>

namespace asset_dumpers
{

	void iscriptfile::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.scriptfile);

		auto iw4_rawfile = local_allocator.allocate<game::native::RawFile>();

		auto script = header.scriptfile;

		if (script->compressedLen > 0 && script->bytecodeLen > 0 && script->len > 0)
		{
			const std::string script_name = script->name;

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

			/////////////////////
			/// quick GSC Fixups
			///
			// Fog works completely differently in iw5 but the settings are very similar
			if (script_name.ends_with("_fog"))
			{
				final_output = get_converted_fog(final_output);
			}
			else if (script_name.ends_with("_fx"))
			{
				if (script_name.starts_with("maps/createfx/"))
				{
					dump_create_fx_sounds(final_output);
				}
				else
				{
					dump_map_fx(final_output);
				}
			}
			else if (script_name.ends_with("_precache"))
			{
				// TEMP - I didn't port animated model scripts yet
				final_output = "main(){\n// Nuked by iw5xport...for now\n}\n";
			}

			// General removals (things that don't exist in IW4)
			auto map_name = exporter::get_map_name();
			std::unordered_map<std::string, std::string> replacements
			{
				// Remove audio reverb setting
				{"maps\\mp\\_audio::", "// Commented out by iw5xport\n// maps\\mp\\_audio::"}
			};

			for (auto replacement : replacements)
			{
				int index;
				if ((index = final_output.find(replacement.first)) != std::string::npos) 
				{
					final_output = final_output.replace(index, replacement.second.length(), replacement.second);
				}
			}

			// Add fog init
			auto load_offset = final_output.find("maps\\mp\\_load::main\(\);\n");
			if (load_offset != std::string::npos)
			{
				final_output = final_output.insert(
					load_offset, 
					std::format("maps\\createart\\{}_fog::main();// Added by iw5xport (iw4 can't make that call automatically)\n    ", map_name)
				);
			}
			
			///
			////////////////////////////


			iw4_rawfile->buffer = local_allocator.allocate_array<char>(final_output.size());
			memcpy(iw4_rawfile->buffer, final_output.data(), final_output.size());
			iw4_rawfile->len = final_output.size();
			iw4_rawfile->compressedLen = 0;
			iw4_rawfile->name = local_allocator.duplicate_string(std::format("{}.gsc", script_name));
		}

		out.rawfile = exporter::dump(game::native::ASSET_TYPE_RAWFILE, {iw4_rawfile}).rawfile;
	}

	std::string iscriptfile::get_converted_fog(const std::string& original)
	{
		auto regex_str_template = "(?:{} *= *((?:[0-9]|\\.)*))";
		const std::string members[] =
		{
			"startdist", "halfwaydist", "red", "green" ,"blue", "maxopacity", "transitiontime", "sunfogenabled"
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

	void iscriptfile::dump_map_fx(const std::string& script)
	{
		std::regex fx_catcher("\\] = loadfx\\( *\"(.+)\" *\\);");
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

	void iscriptfile::dump_create_fx_sounds(const std::string& script)
	{
		std::regex sound_catcher("(?:\\.v\\[\"soundalias\"\\] *= *\"(.+)\")");
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