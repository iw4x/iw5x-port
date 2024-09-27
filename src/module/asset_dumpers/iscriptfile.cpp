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
	const std::string iscriptfile::includes_to_rename[] = {
		"maps\\mp\\_load",
		"common_scripts\\_destructible",
		"common_scripts\\_destructible_types",
	};

	std::string iscriptfile::common_scripts_to_dump[ARRAYSIZE(includes_to_rename)];
	std::string iscriptfile::regex_ready_includes_to_rename[ARRAYSIZE(includes_to_rename)];

	void iscriptfile::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.scriptfile);

		auto iw4_rawfile = local_allocator.allocate<game::native::RawFile>();

		auto script = header.scriptfile;

		if (script->compressedLen > 0 && script->bytecodeLen > 0 && script->len > 0)
		{
			std::string script_name = get_script_name(script);
			std::string script_data = get_decompiled_script(script);

			const bool is_mp = exporter::get_map_name().starts_with("mp_");

			/////////////////////
			/// quick GSC Fixups
			///
			if (is_mp)
			{
				if (script_name.ends_with("_fog.gsc"))
				{
					script_data = get_converted_fog(script_data);
				}
				if (script_name.ends_with("_fx.gsc") && script_name.starts_with("maps/createfx/"))
				{
					script_data = convert_to_strict_createfx(script_data);
				}

				script_data = add_fog_init(script_data);
				script_data = general_compatibility_fixes(script_data);
				script_data = rename_map_animtrees(script_data);
			}
			
			script_data = rename_common_imports(script_data);
			///
			////////////////////////////

			// Character dumping
			if (script_name.starts_with("mptype/") || script_name.starts_with("xmodelalias/") || script_name.starts_with("character/"))
			{
				dump_character_related_script_recursively(script_data);
			}

			iw4_rawfile->buffer = local_allocator.allocate_array<char>(script_data.size());
			memcpy(iw4_rawfile->buffer, script_data.data(), script_data.size());
			iw4_rawfile->len = script_data.size();
			iw4_rawfile->compressedLen = 0;
			iw4_rawfile->name = local_allocator.duplicate_string(script_name);

			iw4_rawfile->name = exporter::fix_map_name(iw4_rawfile->name, local_allocator);
		}

		out.rawfile = exporter::convert(game::native::ASSET_TYPE_RAWFILE, {iw4_rawfile}).rawfile;
	}

	std::string iscriptfile::get_converted_fog(const std::string& original)
	{
		// Fog works completely differently in iw5 but the settings are very similar

		constexpr auto regex_str_template = "(?:{} *= *((?:[0-9]|\\.)*))";
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

				auto j = 0;
				final_string << "    setExpFog(";
				for (auto match : m)
				{
					j++;

					if (j == 1)
					{
						continue;
					}

					final_string << match.str();

					if (j <= ARRAYSIZE(members))
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
		const auto map_name = exporter::get_output_map_name();
		auto script_copy = script;

		// Add fog init
		auto load_offset = script_copy.find("maps\\mp\\_load::main();\n");
		if (load_offset != std::string::npos)
		{
			script_copy = script_copy.insert(
				load_offset,
				std::format("maps\\createart\\{}_fog::main();// Added by iw5xport (iw4 can't make that call automatically)\n    ", map_name)
			);
		}

		return script_copy;
	}

	std::string iscriptfile::general_compatibility_fixes(const std::string& script)
	{
		// General removals (things that don't exist in IW4)
		static std::vector<std::pair<std::regex, std::string>> replacements
		{
			// Remove audio reverb setting
			{std::regex("maps\\\\mp\\\\_audio::"), "// Commented out by iw5xport\n    // $&" },

			// sunlight createart call
			{std::regex("setsunlight"), "// Commented out by iw5xport\n    // $&"},

			// faceoff maps have these calls to set up spawns
			{std::regex("maps\\\\mp\\\\gametypes\\\\faceoff"), "// Commented out by iw5xport\n    // $&"},

			// These don't exist in mw2, we ignore them
			{std::regex("precachefxontag"), "    iw5xport_purge = 0;\n    // $&"},
			{std::regex("precachetag"), "    iw5xport_purge = 0;\n    // $&"},
			{std::regex("precachesound"), "    iw5xport_purge = 0;\n    // $&"},
			{std::regex("precacheitem\\( *\"bomb_site_mp\" *\\);"), "    iw5xport_purge = 0;\n    // $&"},

		};

		std::string script_cpy = script;

		console::info("- replacing iw5 functions with iw4 equivalents...\n");
		for (auto replacement : replacements)
		{
			script_cpy = std::regex_replace(script_cpy, replacement.first, replacement.second);
		}

		console::info("- downgrading syntax (arrays)...\n");
		// Downgrade syntax - we can't use on the fly arrays anymore
		{
			std::regex inline_array_catcher = std::regex("( *)(.*) \\[ ([^,]+) \\](.*;)");
			constexpr auto var_name_format = "iw5xport_$3_arr";
			const auto replacement = std::format(
				"$1{} = [];\n$1{}[{}.size] = $3;\n$1$2{}$4",
				var_name_format, var_name_format, var_name_format, var_name_format
			);

			script_cpy = std::regex_replace(
				script_cpy,
				inline_array_catcher,
				replacement
			);
		}

		console::info("- downgrading syntax (ternaries)...\n");
		// Downgrade syntax - ternaries
		{
			std::regex ternary_catcher = std::regex("( *)(.*)common_scripts\\\\utility::ter_op\\( (.*), (.*), (.*) \\)(.*;)");
			constexpr auto var_name_format = "iw5xport_ternary";
			const auto replacement = std::format(
				"\n$1{} = $5;\n$1if ($3)\n$1{{\n$1$1{} = $4;\n$1}}\n\n$1$2{} $6",
				var_name_format, var_name_format, var_name_format
			);

			script_cpy = std::regex_replace(
				script_cpy,
				ternary_catcher,
				replacement
			);
		}

		console::info("- downgrading syntax (multi array declarations)...\n");
		// Multiple array declarations, more work
		{
			std::regex inline_multi_array_finder = std::regex("(.*)\\[ ((?:.)+, (?:.)+) \\](.*;)");
			std::smatch m;
			std::string::const_iterator search_start(script_cpy.cbegin());
			std::stringstream final_output{};
			bool found_anything = false;
			while (std::regex_search(search_start, script_cpy.cend(), m, inline_multi_array_finder))
			{
				found_anything = true;
				if (m.size() > 3)
				{
					final_output << m.prefix();

					auto match = m[2];
					auto var_names = match.str();

					auto vars = utils::string::split(',', var_names);

					std::stringstream final_code_block{};
					std::string array_name = std::format("iw5xport_{}_arr", m.position());

					// Init array
					final_code_block << "// Unrolled by iw5xport because of iw4 syntax limitation\n" << array_name << " = [];\n";

					// Add all elements
					for (const auto& var : vars)
					{
						final_code_block << std::format("{}[{}.size] = {};\n", array_name, array_name, var);
					}

					search_start = m.suffix().first;

					final_output << final_code_block.str() << m[1] << array_name << m[3];
				}
			}

			if (found_anything)
			{
				final_output << m.suffix();
				script_cpy = final_output.str();
			}
		}

		return script_cpy;
	}

	std::string iscriptfile::rename_map_animtrees(const std::string& script)
	{
		console::info("- renaming animtrees...\n");

		static std::regex animtree_catcher("#using_animtree\\(\"(.*)\"\\);");
		std::smatch m;
		
		std::stringstream result_builder{};
		bool found_anything = false;

		std::string::const_iterator search_start(script.cbegin());
		while (std::regex_search(search_start, script.cend(), m, animtree_catcher))
		{
			found_anything = true;
			if (m.size() > 1)
			{
				const auto& match = m[1];
				auto animtree_name = std::format("animtrees/{}.atr", match.str());
				auto animtree = game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_RAWFILE, animtree_name.data(), 0);
				search_start = m.suffix().first;

				if (animtree.rawfile)
				{
					auto rawfile = exporter::convert(game::native::ASSET_TYPE_RAWFILE, animtree);
					auto new_name = std::string(rawfile.rawfile->name);
					exporter::add_rename_asset(animtree_name, new_name);

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
					//assert(false);
				}
			}
		}

		if (!found_anything)
		{
			// No changes
			return script;
		}

		result_builder << m.suffix();

		std::string final_output = result_builder.str();

		return final_output;
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

		// Replace inline entity initialization with delay
		static std::regex inline_delayed_one_shot_fx("ent = createOneshotEffect\\( \"((?:[0-z]|_)*)\" \\);\n *ent set_origin_and_angles\\( \\((.*)\\), \\((.*)\\) \\);\n( *ent\\.v\\[ \"delay\" \\] = .*;)");
		constexpr auto multiline_delayed_one_shot_fx("ent = createOneshotEffect( \"$1\" );\n	ent.v[ \"origin\" ] = ($2);\n	ent.v[ \"angles\" ] = ($3);\n	ent.v[ \"fxid\" ] = \"$1\";\n$4");
		result = std::regex_replace(result, inline_delayed_one_shot_fx, multiline_delayed_one_shot_fx);


		// and without delay
		static std::regex inline_one_shot_fx("ent = createOneshotEffect\\( \"((?:[0-z]|_)*)\" \\);\n *ent set_origin_and_angles\\( \\((.*)\\), \\((.*)\\) \\);");
		constexpr auto multiline_one_shot_fx("ent = createOneshotEffect( \"$1\" );\n	ent.v[ \"origin\" ] = ($2);\n	ent.v[ \"angles\" ] = ($3);\n	ent.v[ \"fxid\" ] = \"$1\";\n	ent.v[ \"delay\" ] = -1;");
		result = std::regex_replace(result, inline_one_shot_fx, multiline_one_shot_fx);

		return result;
	}

	std::string iscriptfile::rename_common_imports(const std::string& script)
	{
		std::string final_output = script;

		// Rename imports
		for (size_t i = 0; i < ARRAYSIZE(includes_to_rename); i++)
		{
			final_output = std::regex_replace(final_output, std::regex(regex_ready_includes_to_rename[i]), std::format("{}::", "$1_5x"));
		}

		// Rename map
		if (exporter::map_must_be_renamed())
		{
			const auto find = std::regex(exporter::get_map_name());
			final_output = std::regex_replace(final_output, find, exporter::get_output_map_name());
		}
		
		return final_output;
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

	std::string iscriptfile::get_script_name(const game::native::ScriptFile* script) const
	{
		std::string script_name = script->name;

		// This name has been mangled!
		if (script_name[0] <= '9')
		{
			script_name = xsk::gsc::iw5::resolver::token_name(static_cast<unsigned short>(std::stoi(script_name)));
		}

		return std::format("{}.gsc", script_name);
	}

	std::string iscriptfile::get_obfuscated_string(const std::string& name) const
	{
		auto id = xsk::gsc::iw5::resolver::token_id(name);
		if (id)
		{
			return std::to_string(id);
		}

		return name;
	}

	void iscriptfile::dump_character_related_script_recursively(const std::string& contents)
	{
		// Models (precache)
		static std::regex model_catcher("precachemodel\\( ?\"(.*)\" ?\\);");
		{
			std::smatch matches;
			std::string::const_iterator search_start(contents.cbegin());

			while (std::regex_search(search_start, contents.cend(), matches, model_catcher))
			{
				if (matches.size() > 1)
				{
					const auto& match = matches[1];
					auto model_name = match.str();
					command::execute(std::format("dumpXModel {}", model_name), true);
				}

				search_start = matches.suffix().first;
			}
		}

		// Models (heads)
		static std::regex head_catcher("var_0\\[[0-9]+\\] ?= ?\"(.*)\";");
		{
			std::smatch matches;
			std::string::const_iterator search_start(contents.cbegin());

			while (std::regex_search(search_start, contents.cend(), matches, head_catcher))
			{
				if (matches.size() > 1)
				{
					const auto& match = matches[1];
					auto model_name = match.str();

					command::execute(std::format("dumpXModel {}", model_name), true);
				}

				search_start = matches.suffix().first;
			}
		}

		// Sub scripts
		static std::regex subscript_catcher("((?:character|xmodelalias)\\\\(?:.+))::");
		{
			std::smatch matches;
			std::string::const_iterator search_start(contents.cbegin());

			while (std::regex_search(search_start, contents.cend(), matches, subscript_catcher))
			{
				if (matches.size() > 1)
				{
					const auto& match = matches[1];
					auto script_name = match.str();
					std::replace(script_name.begin(), script_name.end(), '\\', '/');

					const auto obfuscated_name = get_obfuscated_string(script_name);
					command::execute(std::format("dumpScript {}", obfuscated_name), true);
				}

				search_start = matches.suffix().first;
			}
		}
	}

	void iscriptfile::dump_rename_common_scripts()
	{
		// We have to dump and rename:
		// - maps/mp/_load.gsc
		// - common_scripts/_destructible.gsc
		// - comon_script/_destructible_types.gsc
		for (size_t i = 0; i < ARRAYSIZE(common_scripts_to_dump); i++)
		{
			const auto& file = includes_to_rename[i];

			const auto token = xsk::gsc::iw5::resolver::token_id(common_scripts_to_dump[i]);
			const auto obfuscated_name = token == 0 ? file : std::to_string(token);

			const auto script = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, obfuscated_name.data(), 0);

			if (script.data)
			{
				console::info("working on %s...\n", file.data());

				std::string raw_script = get_decompiled_script(script.scriptfile);
				std::string rawfile_name = std::format("{}_5x.gsc", file);

				if (file.ends_with("_load"))
				{
					// Specific fix for _load - we have to remove this function
					static std::regex remove_destructible_killcam("(setupdestructiblekillcaments\\(\\)\n\\{)[\\S\\s]*(\\})");

					raw_script = std::regex_replace(raw_script, remove_destructible_killcam, "$1\n    // iw5xport: Nothing to see here!\n$2");
				}

				if (file.ends_with("_destructible_types"))
				{
					// Too many calls, which breaks the precache limit of iw4
					// The limit is the same in iw5 but the code XSK generates makes too many explicit far calls
					// There's a limit of 1024 but this script alone does something like 4078 lol
					raw_script = std::format(
						"#include common_scripts\\_destructible_5x;\n{}", 
						std::regex_replace(raw_script, std::regex("common_scripts\\\\_destructible::"), "")
					);
				}

				// They can contain animtrees
				raw_script = rename_map_animtrees(raw_script);
				raw_script = general_compatibility_fixes(raw_script);
				for (size_t j = 0; j < ARRAYSIZE(includes_to_rename); j++)
				{
					if (i != j)
					{
						raw_script = std::regex_replace(
							raw_script,
							std::regex(regex_ready_includes_to_rename[j]),
							std::format("{}::", "$1_5x")
						);
					}
				}

				auto raw_file = local_allocator.allocate<game::native::RawFile>();
				raw_file->buffer = local_allocator.duplicate_string(raw_script);
				raw_file->len = raw_script.size();
				raw_file->compressedLen = 0;
				raw_file->name = local_allocator.duplicate_string(rawfile_name);

				exporter::convert_and_write(game::native::XAssetType::ASSET_TYPE_RAWFILE, { raw_file });
			}

		}
	}

	iscriptfile::iscriptfile()
	{
		// prepare regex
		for (size_t i = 0; i < ARRAYSIZE(includes_to_rename); i++)
		{
			const auto& file = includes_to_rename[i];

			// Ridiculous
			regex_ready_includes_to_rename[i] = std::format("({})::", std::regex_replace(file, std::regex("\\\\"), "\\$&"));
			common_scripts_to_dump[i] = std::regex_replace(file, std::regex("\\\\"), "/");
		}

		command::add("dumpScript", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];
				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, name);

				if (entry)
				{
					auto rawfile = convert_and_write(entry->asset.header).rawfile;
					exporter::convert_and_write(game::native::ASSET_TYPE_RAWFILE, { rawfile });
					exporter::add_to_source(game::native::ASSET_TYPE_RAWFILE, rawfile->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n", params.get(1));
				}
				
			});
	}
}