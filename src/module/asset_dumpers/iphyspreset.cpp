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

namespace asset_dumpers
{

#define IW4X_PHYSPRESET_VERSION 1

	void iphyspreset::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.physPreset);

		auto iw4_physpreset = local_allocator.allocate<iw4::native::PhysPreset>();
		auto native_physpreset = header.physPreset;

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_physpreset->##member_name## = (native_physpreset->##member_name##)

		SET_MEMBER_NO_CONVERSION(name);
		SET_MEMBER_NO_CONVERSION(type);
		SET_MEMBER_NO_CONVERSION(mass);
		SET_MEMBER_NO_CONVERSION(bounce);
		SET_MEMBER_NO_CONVERSION(friction);
		SET_MEMBER_NO_CONVERSION(bulletForceScale);
		SET_MEMBER_NO_CONVERSION(explosiveForceScale);
		SET_MEMBER_NO_CONVERSION(sndAliasPrefix);
		SET_MEMBER_NO_CONVERSION(piecesSpreadFraction);
		SET_MEMBER_NO_CONVERSION(piecesUpwardVelocity);
		SET_MEMBER_NO_CONVERSION(tempDefaultToCylinder);
		SET_MEMBER_NO_CONVERSION(perSurfaceSndAlias);

		out.physPreset = iw4_physpreset;
	}

	void iphyspreset::write(const iw4::native::XAssetHeader& header)
	{
		// actually... don't!

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		auto asset = header.physPreset;

		output.AddMember("version", IW4X_PHYSPRESET_VERSION, allocator);

		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

		output.AddMember("type", asset->type, allocator);
		output.AddMember("mass", asset->mass, allocator);
		output.AddMember("bounce", asset->bounce, allocator);
		output.AddMember("friction", asset->friction, allocator);
		output.AddMember("bulletForceScale", asset->bulletForceScale, allocator);
		output.AddMember("explosiveForceScale", asset->explosiveForceScale, allocator);
		output.AddMember("sndAliasPrefix", RAPIDJSON_STR(asset->sndAliasPrefix), allocator);
		output.AddMember("piecesSpreadFraction", asset->piecesSpreadFraction, allocator);
		output.AddMember("piecesUpwardVelocity", asset->piecesUpwardVelocity, allocator);
		output.AddMember("tempDefaultToCylinder", asset->tempDefaultToCylinder, allocator);
		output.AddMember("perSurfaceSndAlias", asset->perSurfaceSndAlias, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		utils::io::write_file(std::format("{}/{}/{}.iw4x.json", get_export_path(), game::native::g_assetNames[game::native::XAssetType::ASSET_TYPE_PHYSPRESET], asset->name), dump);
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