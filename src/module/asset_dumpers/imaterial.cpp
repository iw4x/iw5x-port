#include <std_include.hpp>

#include "imaterial.hpp"
#include "../asset_dumper.hpp"

#include "itechniqueset.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"
#include "utils/json.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <module\exporter.hpp>

namespace asset_dumpers
{

	void imaterial::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.material);

		const auto native_material = header.material;

		out.material = utils::memory::allocate<iw4::native::Material>();
		iw4::native::Material* iw4_material = out.material;

		iw4_material->info = native_material->info;

		std::memset(iw4_material->stateBitsEntry, 0xFF, sizeof(iw4_material->stateBitsEntry));


		// copy techniques to correct spots
		for (size_t i = 0; i < iw4::native::TECHNIQUE_COUNT; i++)
		{
			[[maybe_unused]] const auto count = itechniqueset::techniques_from_iw5_to_iw4.size();
			assert(count >= iw4::native::TECHNIQUE_COUNT);

			auto destinationTechnique = static_cast<iw4::native::MaterialTechniqueType>(i);
			auto originTechnique = itechniqueset::techniques_from_iw5_to_iw4.at(destinationTechnique);


			iw4_material->stateBitsEntry[destinationTechnique] = native_material->stateBitsEntry[originTechnique];
		}

		iw4_material->textureCount = native_material->textureCount;
		iw4_material->constantCount = native_material->constantCount;
		iw4_material->stateBitsCount = native_material->stateBitsCount;
		iw4_material->stateFlags = native_material->stateFlags;

		iw4_material->cameraRegion = std::clamp(
			native_material->cameraRegion, 
			static_cast<unsigned char>(iw4::native::CAMERA_REGION_LIT_OPAQUE), 
			static_cast<unsigned char>(iw4::native::CAMERA_REGION_COUNT) // CAMERA_REGION_LIGHT_MAP_OPAQUE => CAMERA_REGION_NONE
		);

		auto nativeTechset = native_material->techniqueSet;
		iw4_material->techniqueSet = 
			exporter::convert(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, game::native::XAssetHeader{ nativeTechset }).techniqueSet;

		iw4_material->textureTable = utils::memory::allocate_array<iw4::native::MaterialTextureDef>(iw4_material->textureCount);
		unsigned int iw4_texture_index = 0;
		for (auto i = 0; i < native_material->textureCount; i++)
		{
			auto iw4_tex = &iw4_material->textureTable[iw4_texture_index];
			auto native_tex = &native_material->textureTable[i];
			std::memcpy(iw4_tex, native_tex, sizeof(iw4_material->textureTable[i]));
			
			if (iw4_tex->semantic > iw4::native::TS_WATER_MAP)
			{
				// DISPLACEMENT_MAP is not in iw4, so we skip this image
				iw4_material->textureCount--;
				console::warn("killing image %i (%s) of material %s! The texture semantic %i is not supported.\n", i, iw4_tex->u.image->name, iw4_material->info.name, iw4_tex->semantic);
				continue;
			}

			iw4_texture_index++;

			if (iw4_material->info.name == "$levelbriefing"s && iw4_tex->u.image->name == "default"s)
			{
				// No idea why this image is ""wrong"", i suppose the game doesn't use it
				// bad news, iw4 needs it. So let's change the name
				iw4_tex->u.image->name = local_allocator.duplicate_string(std::format("loadscreen_{}", exporter::get_map_name()));
			}

			if (iw4_tex->u.image == nullptr)
			{
				// This happens sometimes! No idea why, it sounds like a big mistake to me
				// Maybe because of dedicated server files?
				// This will crash the game in iw4 so we gotta be very careful about it
				auto other_image = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_IMAGE, "$default", 1);
				iw4_tex->u.image = exporter::convert(game::native::XAssetType::ASSET_TYPE_IMAGE, other_image).image;
				console::warn("missing image %i of material %s! This will not crash, but it will look weird...\n", i, iw4_material->info.name);
			}

			assert(iw4_tex->u.image);
		}

		iw4_material->constantTable = native_material->constantTable;
		iw4_material->stateBitsTable = native_material->stateBitsTable;
	}

	void imaterial::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_MATERIAL, header.data);
		assert(result);
	}

	imaterial::imaterial()
	{
		command::add("dumpMaterial", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					static std::mutex m{};
					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_MATERIAL, [](game::native::XAssetHeader header, void* data) {
						m.lock();
						auto asset_dmper = reinterpret_cast<asset_dumper*>(data);

						asset_dmper->convert_and_write(header, true);
						m.unlock();

						}, this, false);
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_MATERIAL, name, false);

					if (header.data)
					{
						convert_and_write(header);
						exporter::add_to_source(game::native::ASSET_TYPE_MATERIAL, name);
						console::info("successfullly dumped material %s!\n", name);
					}
					else
					{
						console::info("could not dump material %s from the database (cannot find it)\n", name);
					}
				}
			});
	}


}