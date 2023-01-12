#include <std_include.hpp>

#include "irawfile.hpp"
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

	void irawfile::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.rawfile);

		// No conversion
		out.rawfile = header.rawfile;
	}

	void irawfile::write(const iw4::native::XAssetHeader& header)
	{
		assert(header.rawfile);
		assert(header.rawfile->buffer);

		if (header.rawfile->len > 0)
		{

			auto data = header.rawfile->buffer;
			std::vector<unsigned char> data_decompressed;
			if (header.rawfile->compressedLen > 0)
			{
				auto data_compressed = std::vector<unsigned char>();
				data_compressed.assign(header.rawfile->buffer, header.rawfile->buffer + header.rawfile->compressedLen);
				data_decompressed = xsk::utils::zlib::decompress(data_compressed, header.rawfile->len);
				auto decompressed_length = data_decompressed.size();
				assert(decompressed_length == header.rawfile->len);
				data = reinterpret_cast<char*>(data_decompressed.data());
			}

			utils::io::write_file(std::format("{}/{}", get_export_path(), header.rawfile->name), std::string(data, header.rawfile->len));
		}
	}

	irawfile::irawfile()
	{
		command::add("dumpRawFile", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_RAWFILE, params[1]);

				if (entry)
				{
					dump(entry->asset.header);
					exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_RAWFILE, entry->asset.header.rawfile->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n ", params.get(1));
				}
			});
	}
}