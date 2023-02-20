#include <std_include.hpp>

#include "ixanimparts.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <xsk/utils/compression.hpp>
#include <module/console.hpp>

#define IW4X_ANIM_VERSION 2

namespace asset_dumpers
{

	void ixanimparts::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.parts);

		// No conversion
		out.parts = header.parts;
	}

	void ixanimparts::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_XANIMPARTS, header.data);
		assert(result);
	}

	ixanimparts::ixanimparts()
	{
		command::add("dumpXAnim", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_XANIMPARTS, params[1]);

				if (entry)
				{
					convert_and_write(entry->asset.header);
					exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_XANIMPARTS, entry->asset.header.rawfile->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n ", params.get(1));
				}
			});
	}
}