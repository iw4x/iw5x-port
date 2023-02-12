#include <std_include.hpp>

#include "ifxworld.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include "module/exporter.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#define IW4X_FXWORLD_VERSION 1

namespace asset_dumpers
{

	void ifxworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.fxWorld);

		auto iw4_fxworld = local_allocator.allocate<iw4::native::FxWorld>();
		auto native_fxworld = header.fxWorld;

		iw4_fxworld->name = native_fxworld->name;

#define SET_GLASS_SYS_MEMBER(member_name) iw4_fxworld->glassSys.##member_name## = (native_fxworld->glassSys.##member_name##)

		SET_GLASS_SYS_MEMBER(time);
		SET_GLASS_SYS_MEMBER(prevTime);
		SET_GLASS_SYS_MEMBER(defCount);
		SET_GLASS_SYS_MEMBER(pieceLimit);
		SET_GLASS_SYS_MEMBER(pieceWordCount);
		SET_GLASS_SYS_MEMBER(initPieceCount);
		SET_GLASS_SYS_MEMBER(cellCount);
		SET_GLASS_SYS_MEMBER(activePieceCount);
		SET_GLASS_SYS_MEMBER(firstFreePiece);
		SET_GLASS_SYS_MEMBER(geoDataLimit);
		SET_GLASS_SYS_MEMBER(geoDataCount);
		SET_GLASS_SYS_MEMBER(initGeoDataCount);

		auto iw4_sys = &iw4_fxworld->glassSys;
		auto native_sys = &native_fxworld->glassSys;

		iw4_sys->defs = local_allocator.allocate_array<iw4::native::FxGlassDef>(iw4_sys->defCount);
		for (size_t i = 0; i < iw4_sys->defCount; i++)
		{
			auto iw4_def = &iw4_sys->defs[i];
			auto native_def = &native_sys->defs[i];

			static_assert(sizeof iw4::native::FxGlassDef == sizeof game::native::FxGlassDef);
			memcpy(iw4_def, native_def, sizeof game::native::FxGlassDef);

			iw4_def->material = exporter::dump(game::native::ASSET_TYPE_MATERIAL, { native_def->material }).material;
			iw4_def->materialShattered = exporter::dump(game::native::ASSET_TYPE_MATERIAL, { native_def->materialShattered }).material;
			iw4_def->physPreset = exporter::dump(game::native::ASSET_TYPE_PHYSPRESET, { native_def->physPreset }).physPreset;
		}

		SET_GLASS_SYS_MEMBER(piecePlaces);
		SET_GLASS_SYS_MEMBER(pieceDynamics);
		SET_GLASS_SYS_MEMBER(pieceStates);
		SET_GLASS_SYS_MEMBER(geoData);

		SET_GLASS_SYS_MEMBER(isInUse);
		SET_GLASS_SYS_MEMBER(cellBits);
		SET_GLASS_SYS_MEMBER(visData);
		SET_GLASS_SYS_MEMBER(linkOrg);
		SET_GLASS_SYS_MEMBER(halfThickness);
		SET_GLASS_SYS_MEMBER(lightingHandles);

		SET_GLASS_SYS_MEMBER(initPieceStates);
		SET_GLASS_SYS_MEMBER(initGeoData);

		SET_GLASS_SYS_MEMBER(needToCompactData);
		SET_GLASS_SYS_MEMBER(initCount);
		SET_GLASS_SYS_MEMBER(effectChanceAccum);
		SET_GLASS_SYS_MEMBER(lastPieceDeletionTime);

		out.fxWorld = iw4_fxworld;
	}

	void ifxworld::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_FXWORLD, header.data);
		assert(result);
	}

	ifxworld::ifxworld()
	{
		command::add("dumpFXWorld", [&](const command::params& params)
			{
				game::native::XAssetHeader out{};

				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_FXWORLD, [](game::native::XAssetHeader header, void* data) {
					auto out = reinterpret_cast<game::native::XAssetHeader*>(data);
					*out = header;

					}, &out, false);

				if (out.data)
				{
					dump(out);
					exporter::add_to_source(game::native::ASSET_TYPE_FXWORLD, out.fxWorld->name);
				}
			});
	}
}