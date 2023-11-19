#include <std_include.hpp>

#include "itracerdef.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include <module/console.hpp>

namespace asset_dumpers
{
	void itracerdef::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.tracerDef);

		// No conversion
		out.tracerDef = local_allocator.allocate<iw4::native::TracerDef>();

		static_assert(sizeof(game::native::TracerDef) == sizeof(iw4::native::TracerDef));
		std::memcpy(out.tracerDef, header.tracerDef, sizeof(game::native::TracerDef));

		out.tracerDef->material = exporter::convert(game::native::XAssetType::ASSET_TYPE_MATERIAL, { header.tracerDef->material }).material;
	}

	void itracerdef::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_TRACER, header.data);
		assert(result);
	}

	itracerdef::itracerdef()
	{
		command::add("dumpTracer", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_TRACER, params[1]);

				if (entry)
				{
					convert_and_write(entry->asset.header);
					exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_TRACER, entry->asset.header.tracerDef->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n ", params.get(1));
				}
			});
	}
}