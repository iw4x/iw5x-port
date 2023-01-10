#include <std_include.hpp>

#include "icomworld.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#define IW4X_COMMAP_VERSION 0

namespace asset_dumpers
{

	void icomworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.comWorld);

		auto iw4_comworld = local_allocator.allocate<iw4::native::ComWorld>();
		const auto native_comworld = header.comWorld;

		static_assert(sizeof iw4::native::ComWorld == sizeof game::native::ComWorld);
		memcpy(iw4_comworld, native_comworld, sizeof iw4::native::ComWorld);

		iw4_comworld->primaryLights = local_allocator.allocate_array<iw4::native::ComPrimaryLight>(iw4_comworld->primaryLightCount);

		for (size_t i = 0; i < iw4_comworld->primaryLightCount; i++)
		{
			auto iw4_light = &iw4_comworld->primaryLights[i];
			auto native_light = &native_comworld->primaryLights[i];

			iw4_light->type = native_light->type;
			iw4_light->canUseShadowMap = native_light->canUseShadowMap;
			iw4_light->exponent = native_light->exponent;
			iw4_light->unused = native_light->unused;

			memcpy(iw4_light->color, native_light->color, sizeof iw4_light->color);
			memcpy(iw4_light->dir, native_light->dir, sizeof iw4_light->dir);
			memcpy(iw4_light->origin, native_light->origin, sizeof iw4_light->origin);

			iw4_light->radius = native_light->radius;
			iw4_light->cosHalfFovOuter = native_light->cosHalfFovOuter;
			iw4_light->cosHalfFovInner = native_light->cosHalfFovInner;
			iw4_light->cosHalfFovExpanded = native_light->cosHalfFovExpanded;
			iw4_light->rotationLimit = native_light->rotationLimit;
			iw4_light->translationLimit = native_light->translationLimit;
			iw4_light->defName = native_light->defName;

			if (iw4_light->defName)
			{
				exporter::dump(game::native::XAssetType::ASSET_TYPE_LIGHT_DEF, { game::native::DB_FindXAssetHeader(game::native::ASSET_TYPE_LIGHT_DEF, iw4_light->defName, 0).lightDef });
			}
		}

		out.comWorld = iw4_comworld;
	}

	void icomworld::write(const iw4::native::XAssetHeader& header)
	{
		auto asset = header.comWorld;

		utils::stream buffer;
		buffer.saveArray("IW4xComW", 8);
		buffer.saveObject(IW4X_COMMAP_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->primaryLights)
		{
			AssertSize(iw4::native::ComPrimaryLight, 68);

			buffer.saveArray(asset->primaryLights, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				iw4::native::ComPrimaryLight* light = &asset->primaryLights[i];

				if (light->defName)
				{
					buffer.saveString(light->defName);
				}
			}
		}

		assert(asset->name);

		if (asset->name)
		{
			std::string basename(asset->name);
			
			constexpr auto prefix = "maps/mp/";
			constexpr auto suffix = ".d3dbsp";

			basename = basename.substr(strlen(prefix), basename.size() - strlen(suffix) - strlen(prefix));

			utils::io::write_file(std::format("{}/comworld/{}.iw4xComWorld", get_export_path(), basename), buffer.toBuffer());
		}
	}

	icomworld::icomworld()
	{
		command::add("dumpComWorld", [&](const command::params& params)
			{
				game::native::XAssetHeader com_header;
				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_COMWORLD, [](game::native::XAssetHeader header, void* data) {
					memcpy(data, &header, sizeof header);

					}, &com_header, false);

				if (com_header.data)
				{
					dump(com_header);
				}
			});
	}
}