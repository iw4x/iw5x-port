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

		std::string name(header.rawfile->name);
		if (name.ends_with(".vision"))
		{
			// IW4 is a very vibrant game, so let's saturate vision a little bit
			saturate_vision(header.rawfile);
		}

		if (name.ends_with(".atr"))
		{
			out.rawfile = local_allocator.allocate<game::native::RawFile>();
			memcpy_s(out.rawfile, sizeof game::native::RawFile, header.rawfile, sizeof game::native::RawFile);

			// We rename animtrees because they're canonically part of common_mp and will cause problems in iw4
			out.rawfile->name = local_allocator.duplicate_string(
				std::format("{}_5x.atr", name.substr(0, name.size() - 4))
			);
		}
		else
		{
			out.rawfile = header.rawfile;
		}
	}

	void irawfile::saturate_vision(game::native::RawFile* rawfile)
	{
		if (rawfile->len > 0)
		{
			std::vector<unsigned char> data_decompressed;
			std::string data;
			if (rawfile->compressedLen > 0)
			{
				auto data_compressed = std::vector<unsigned char>();
				data_compressed.assign(rawfile->buffer, rawfile->buffer + rawfile->compressedLen);
				data_decompressed = xsk::utils::zlib::decompress(data_compressed, rawfile->len);
				auto decompressed_length = data_decompressed.size();
				assert(decompressed_length == rawfile->len);
				data = std::string(reinterpret_cast<const char*>(data_decompressed.data()), decompressed_length);
			}
			else
			{
				data = rawfile->buffer;
			}

			static std::regex saturation_catcher("(r_filmDesaturation +)\"([0-9]+(?:\\.[0-9]*)?)\"");

			std::smatch m;

			if (std::regex_search(data, m, saturation_catcher))
			{
				auto value = m.str(2);
				auto f_value = std::stof(value);

				// I hope that isn't a bit too much
				f_value -= 0.25f;

				value = std::to_string(f_value);

				data = std::regex_replace(data, saturation_catcher, "$1\""s + value + "\"");

				rawfile->buffer = local_allocator.duplicate_string(data);
				rawfile->compressedLen = 0;
				rawfile->len = data.size();
			}
		}
	}

	void irawfile::write(const iw4::native::XAssetHeader& header)
	{
		assert(header.rawfile);
		assert(header.rawfile->buffer);

		if (std::string(header.rawfile->name).contains("chicken"))
		{
			printf("");
		}

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