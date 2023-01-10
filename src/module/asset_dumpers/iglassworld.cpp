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
		auto asset = header.gameWorldMp;


		constexpr auto prefix = "maps/mp/";
		constexpr auto suffix = ".d3dbsp";

		std::string basename(header.gameWorldMp->name);
		basename = basename.substr(strlen(prefix), basename.size() - strlen(suffix) - strlen(prefix));

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();
		utils::memory::allocator str_duplicator;

		output.AddMember("version", IW4X_GAMEWORLD_VERSION, allocator);

		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

		rapidjson::Value glassData(rapidjson::kObjectType);

		rapidjson::Value pieces(rapidjson::kArrayType);
		for (size_t i = 0; i < asset->g_glassData->pieceCount; i++)
		{
			auto iwPiece = asset->g_glassData->glassPieces[i];

			rapidjson::Value piece(rapidjson::kObjectType);

			piece.AddMember("damageTaken", iwPiece.damageTaken, allocator);
			piece.AddMember("collapseTime", iwPiece.collapseTime, allocator);
			piece.AddMember("lastStateChangeTime", iwPiece.lastStateChangeTime, allocator);
			piece.AddMember("impactDir", iwPiece.impactDir, allocator);

			rapidjson::Value impactPos(rapidjson::kArrayType);
			impactPos.PushBack(iwPiece.impactPos[0], allocator);
			impactPos.PushBack(iwPiece.impactPos[1], allocator);

			piece.AddMember("impactPos", impactPos, allocator);

			pieces.PushBack(piece, allocator);
		}

		glassData.AddMember("glassPieces", pieces, allocator);
		glassData.AddMember("damageToWeaken", asset->g_glassData->damageToWeaken, allocator);
		glassData.AddMember("damageToDestroy", asset->g_glassData->damageToDestroy, allocator);

		rapidjson::Value glassNames(rapidjson::kArrayType);

		for (size_t i = 0; i < asset->g_glassData->glassNameCount; i++)
		{
			auto iwName = asset->g_glassData->glassNames[i];

			rapidjson::Value name(rapidjson::kObjectType);

			name.AddMember("nameStr", RAPIDJSON_STR(iwName.nameStr), allocator);
			name.AddMember("name", iwName.name, allocator);


			rapidjson::Value pieceIndices(rapidjson::kArrayType);

			for (size_t j = 0; j < iwName.pieceCount; j++)
			{
				pieceIndices.PushBack(iwName.pieceIndices[j], allocator);
			}

			name.AddMember("piecesIndices", pieceIndices, allocator);

			glassNames.PushBack(name, allocator);
		}

		glassData.AddMember("glassNames", glassNames, allocator);

		output.AddMember("glassData", glassData, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		utils::io::write_file(std::format("{}/gameworld/{}.iw4x.json", get_export_path(), basename), dump);
	}

	iglassworld::iglassworld()
	{
		command::add("dumpGlassWorld", [&](const command::params& params)
			{
				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_GLASSWORLD, [](game::native::XAssetHeader header, void* data) {
					auto dumper = reinterpret_cast<iglassworld*>(data);

					dumper->dump(header);

					}, this, false);
			});
	}
}