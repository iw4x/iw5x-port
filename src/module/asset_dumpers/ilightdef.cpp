﻿#include <std_include.hpp>

#include "ilightdef.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace asset_dumpers
{

	void ilightdef::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.lightDef);

		auto iw4_lightdef = local_allocator.allocate<iw4::native::GfxLightDef>();
		const auto native_lightdef = header.lightDef;

		iw4_lightdef->name = native_lightdef->name;
		iw4_lightdef->lmapLookupStart = native_lightdef->lmapLookupStart;

		static_assert(sizeof iw4_lightdef->attenuation == sizeof native_lightdef->attenuation);
		memcpy(&iw4_lightdef->attenuation, &native_lightdef->attenuation, sizeof iw4_lightdef->attenuation);

		if (iw4_lightdef->attenuation.image)
		{
			iw4_lightdef->attenuation.image = 
				exporter::convert(game::native::XAssetType::ASSET_TYPE_IMAGE, { native_lightdef->attenuation.image }).image;
		}

		out.lightDef = iw4_lightdef;
	}

	void ilightdef::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_LIGHT_DEF, header.data);
		assert(result);
	}

	ilightdef::ilightdef()
	{
	}
}