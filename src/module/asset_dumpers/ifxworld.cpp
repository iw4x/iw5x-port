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
		auto asset = header.fxWorld;

		constexpr auto prefix = "maps/mp/";
		constexpr auto suffix = ".d3dbsp";

		std::string basename(header.fxWorld->name);
		basename = basename.substr(strlen(prefix), basename.size() - strlen(suffix) - strlen(prefix));


		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();
		utils::memory::allocator str_duplicator;

		output.AddMember("version", IW4X_FXWORLD_VERSION, allocator);
		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);


		rapidjson::Value glass_system(rapidjson::kObjectType);

#define SAME_NAME_JSON_MEMBER(obj, json, value) json.AddMember(#value, obj.value, allocator)

		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, time);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, prevTime);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, defCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, pieceLimit);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, pieceWordCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, initPieceCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, cellCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, activePieceCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, firstFreePiece);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, geoDataLimit);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, geoDataCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, initGeoDataCount);

		rapidjson::Value defs(rapidjson::kArrayType);
		for (auto i = 0; i < asset->glassSys.defCount; i++)
		{
			auto def = &asset->glassSys.defs[i];

			rapidjson::Value def_json(rapidjson::kObjectType);

			def_json.AddMember("halfThickness", def->halfThickness, allocator);

			rapidjson::Value texvecs_array(rapidjson::kArrayType);
			texvecs_array.PushBack(def->texVecs[0][0], allocator);
			texvecs_array.PushBack(def->texVecs[0][1], allocator);
			texvecs_array.PushBack(def->texVecs[1][0], allocator);
			texvecs_array.PushBack(def->texVecs[1][1], allocator);

			def_json.AddMember("texVecs", texvecs_array, allocator);

			def_json.AddMember("color", def->color.packed, allocator);
			def_json.AddMember("material", RAPIDJSON_STR(def->material->info.name), allocator);
			def_json.AddMember("materialShattered", RAPIDJSON_STR(def->materialShattered->info.name), allocator);
			def_json.AddMember("physPreset", RAPIDJSON_STR(def->physPreset->name), allocator);

			defs.PushBack(def_json, allocator);
		}

		glass_system.AddMember("defs", defs, allocator);

		rapidjson::Value init_pieces(rapidjson::kArrayType);
		for (auto i = 0; i < asset->glassSys.initPieceCount; i++)
		{
			auto init_piece_state = &asset->glassSys.initPieceStates[i];

			rapidjson::Value init_piece_json(rapidjson::kObjectType);

			rapidjson::Value fx_spatial_frame(rapidjson::kObjectType);

			rapidjson::Value fx_quat(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(init_piece_state->frame.quat); j++)
			{
				fx_quat.PushBack(init_piece_state->frame.quat[j], allocator);
			}

			rapidjson::Value fx_origin(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(init_piece_state->frame.origin); j++)
			{
				fx_origin.PushBack(init_piece_state->frame.origin[j], allocator);
			}

			fx_spatial_frame.AddMember("quat", fx_quat, allocator);
			fx_spatial_frame.AddMember("origin", fx_origin, allocator);

			init_piece_json.AddMember("frame", fx_spatial_frame, allocator);
			init_piece_json.AddMember("radius", init_piece_state->radius, allocator);

			rapidjson::Value tex_coord_origin(rapidjson::kArrayType);
			tex_coord_origin.PushBack(init_piece_state->texCoordOrigin[0],allocator);
			tex_coord_origin.PushBack(init_piece_state->texCoordOrigin[1],allocator);
			init_piece_json.AddMember("texCoordOrigin", tex_coord_origin, allocator);

			init_piece_json.AddMember("supportMask", init_piece_state->supportMask, allocator);
			init_piece_json.AddMember("areaX2", init_piece_state->areaX2, allocator);
			init_piece_json.AddMember("defIndex", init_piece_state->defIndex, allocator);
			init_piece_json.AddMember("vertCount", init_piece_state->vertCount, allocator);
			init_piece_json.AddMember("fanDataCount", init_piece_state->fanDataCount, allocator);
		
			init_pieces.PushBack(init_piece_json, allocator);
		}

		glass_system.AddMember("initPieceStates", init_pieces, allocator);


		rapidjson::Value init_geo_json(rapidjson::kArrayType);
		for (auto i = 0; i < asset->glassSys.initGeoDataCount; i++)
		{
			auto init_geo = &asset->glassSys.initGeoData[i];

			rapidjson::Value data_array(rapidjson::kArrayType);
			data_array.PushBack(init_geo->anonymous[0], allocator);
			data_array.PushBack(init_geo->anonymous[1], allocator);

			init_geo_json.PushBack(data_array, allocator);
		}

		glass_system.AddMember("initGeoData", init_geo_json, allocator);

		// And that's it! Everything else in this thing is RUNTIME!
		// Crazy huh ?

		output.AddMember("glassSys", glass_system, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		utils::io::write_file(std::format("{}/fxworld/{}.iw4x.json", get_export_path(), basename), dump);
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