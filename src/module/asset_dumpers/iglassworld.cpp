#include <std_include.hpp>

#include "iglassworld.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <module/exporter.hpp>

#define IW4X_GAMEWORLD_VERSION 1

namespace asset_dumpers
{

	void iglassworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.glassWorld);

		// No conversion necessary

		out.gameWorldMp = header.glassWorld;
	}

	void iglassworld::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_GAMEWORLD_MP, header.data);
		assert(result);
	}

	iglassworld::iglassworld()
	{
		command::add("dumpGlassWorld", [&](const command::params& params)
			{
				game::native::XAssetHeader out{};

				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_GLASSWORLD, [](game::native::XAssetHeader header, void* data) {
					auto out = reinterpret_cast<game::native::XAssetHeader*>(data);
					*out = header;

					}, &out, false);

				if (out.data)
				{
					dump(out);
					exporter::add_to_source(game::native::ASSET_TYPE_GLASSWORLD, out.glassWorld->name);
				}
			});
	}
}