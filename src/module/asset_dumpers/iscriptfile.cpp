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
			auto script_name = script->name;

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

			auto final_output = dec->output();


			iw4_rawfile->buffer = local_allocator.allocate_array<char>(final_output.size());
			memcpy(iw4_rawfile->buffer, final_output.data(), final_output.size());

			iw4_rawfile->len = final_output.size();
			iw4_rawfile->name = local_allocator.duplicate_string(std::format("{}.gsc", script_name));
		}

		out.rawfile = exporter::dump(game::native::ASSET_TYPE_RAWFILE, {iw4_rawfile}).rawfile;
	}

	void iscriptfile::write(const iw4::native::XAssetHeader& header)
	{
		// We don't! iRawfile.cpp handles that
	}

	iscriptfile::iscriptfile()
	{
		command::add("dumpScript", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_MATERIAL, [](game::native::XAssetHeader header, void* data) {
						
						auto asset_dmper = reinterpret_cast<asset_dumper*>(data);

						asset_dmper->dump(header, true);

						}, this, false);
				}
				else
				{
					auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_SCRIPTFILE, name);

					if (entry)
					{
						dump(entry->asset.header);
					}
					else
					{
						console::warn("i cannot find %s!\n :(", params.get(1));
					}
				}
			});
	}
}