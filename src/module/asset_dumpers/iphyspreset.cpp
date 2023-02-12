#include <std_include.hpp>

#include "iphyspreset.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "module/exporter.hpp"

namespace asset_dumpers
{

#define IW4X_PHYSPRESET_VERSION 1

	void iphyspreset::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.physPreset);

		auto iw4_physpreset = local_allocator.allocate<iw4::native::PhysPreset>();
		auto native_physpreset = header.physPreset;

#define SET_LIGHTGRID_MEMBER(member_name) iw4_physpreset->##member_name## = (native_physpreset->##member_name##)

		SET_LIGHTGRID_MEMBER(name);
		SET_LIGHTGRID_MEMBER(type);
		SET_LIGHTGRID_MEMBER(mass);
		SET_LIGHTGRID_MEMBER(bounce);
		SET_LIGHTGRID_MEMBER(friction);
		SET_LIGHTGRID_MEMBER(bulletForceScale);
		SET_LIGHTGRID_MEMBER(explosiveForceScale);
		SET_LIGHTGRID_MEMBER(sndAliasPrefix);
		SET_LIGHTGRID_MEMBER(piecesSpreadFraction);
		SET_LIGHTGRID_MEMBER(piecesUpwardVelocity);
		SET_LIGHTGRID_MEMBER(tempDefaultToCylinder);
		SET_LIGHTGRID_MEMBER(perSurfaceSndAlias);

		out.physPreset = iw4_physpreset;
	}

	void iphyspreset::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_PHYSPRESET, header.data);
		assert(result);
	}

	iphyspreset::iphyspreset()
	{
		command::add("dumpPhysPreset", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					std::vector<game::native::XAssetHeader> headers{};

					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_PHYSPRESET, [](game::native::XAssetHeader header, void* data) {
						auto headers = reinterpret_cast<std::vector<game::native::XAssetHeader>*>(data);
						
						headers->push_back(header);

					}, &headers, false);

					for (auto header : headers)
					{
						dump(header, true);
					}
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_PHYSPRESET, name, false);

					if (header.data)
					{
						dump(header, true);
						console::info("successfullly dumped image %s!\n", name);
					}
					else
					{
						console::info("could not dump image %s from the database (cannot find it)\n", name);
					}
				}
			});
	}
}