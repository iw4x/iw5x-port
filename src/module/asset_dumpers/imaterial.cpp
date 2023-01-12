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

#define IW4X_MAT_VERSION 1

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
			exporter::dump(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, game::native::XAssetHeader{ nativeTechset }).techniqueSet;

		iw4_material->textureTable = utils::memory::allocate_array<iw4::native::MaterialTextureDef>(iw4_material->textureCount);
		for (auto i = 0; i < iw4_material->textureCount; i++)
		{
			auto tex = &iw4_material->textureTable[i];
			std::memcpy(tex, &native_material->textureTable[i], sizeof(iw4_material->textureTable[i]));
			
			if (tex->semantic > iw4::native::TS_WATER_MAP)
			{
				// DISPLACEMENT_MAP is not in iw4
				tex->semantic = iw4::native::TS_UNUSED_2;
			}
		}

		iw4_material->constantTable = native_material->constantTable;
		iw4_material->stateBitsTable = native_material->stateBitsTable;
	}

	void imaterial::write(const iw4::native::XAssetHeader& header)
	{
		auto asset = header.material;

		utils::memory::allocator str_duplicator;
		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		output.AddMember("version", IW4X_MAT_VERSION, allocator);

		output.AddMember("name", RAPIDJSON_STR(asset->info.name), allocator);


		const auto gameFlagsStr = std::format("{:08b}", asset->info.gameFlags);
		output.AddMember("gameFlags", RAPIDJSON_STR(gameFlagsStr.c_str()), allocator);

		const auto stateFlags = std::format("{:08b}", static_cast<char>(asset->stateFlags));
		output.AddMember("stateFlags", RAPIDJSON_STR(stateFlags.c_str()), allocator);

#define SAME_NAME_JSON_MEMBER(x) output.AddMember(#x, asset->info.x, allocator)

		SAME_NAME_JSON_MEMBER(sortKey);

		std::string techsetName;
		if (asset->techniqueSet)
		{
			output.AddMember("techniqueSet", RAPIDJSON_STR(asset->techniqueSet->name), allocator);
		}


		SAME_NAME_JSON_MEMBER(textureAtlasRowCount);
		SAME_NAME_JSON_MEMBER(textureAtlasColumnCount);

		const auto surfaceTypeBits = std::format("{:032b}", asset->info.surfaceTypeBits);
		output.AddMember("surfaceTypeBits", RAPIDJSON_STR(surfaceTypeBits.c_str()), allocator);

		rapidjson::Value textureTable(rapidjson::kArrayType);

		if (asset->textureTable)
		{
			for (char i = 0; i < asset->textureCount; ++i)
			{
				auto textureDef = &asset->textureTable[i];
				rapidjson::Value textureJson(rapidjson::kObjectType);

				textureJson.AddMember("nameStart", textureDef->nameStart, allocator);
				textureJson.AddMember("nameEnd", textureDef->nameEnd, allocator);
				textureJson.AddMember("nameHash", textureDef->nameHash, allocator);
				textureJson.AddMember("samplerState", textureDef->samplerState, allocator); // $6961E030A9677F7C86FC6FF9B5901495
				textureJson.AddMember("semantic", textureDef->semantic, allocator);

				if (textureDef->semantic == iw4::native::TS_WATER_MAP)
				{
					auto water = textureDef->u.water;

					if (water)
					{
						rapidjson::Value waterJson(rapidjson::kObjectType);

						if (water->image)
						{
							waterJson.AddMember("image", RAPIDJSON_STR(water->image->name), allocator);
							exporter::dump(game::native::XAssetType::ASSET_TYPE_IMAGE, { water->image });
						}

						constexpr unsigned long BUFF_SIZE = 1024;

						// Save_water_t
						if (water->H0)
						{
							auto ptr = reinterpret_cast<uint8_t*>(water->H0);
							auto buffer = std::vector<uint8_t>(ptr, ptr + water->M * water->N * sizeof(game::native::complex_s));
							
							unsigned long buffLength = BUFF_SIZE;
							char b64[BUFF_SIZE];
							base64_encode(&buffer.front(), buffer.size(), b64, &buffLength);
							assert(buffLength < BUFF_SIZE);

							waterJson.AddMember("H0", RAPIDJSON_STR(str_duplicator.duplicate_string(b64)), allocator);
						}

						if (water->wTerm)
						{
							auto ptr = reinterpret_cast<uint8_t*>(water->wTerm);
							auto buffer = std::vector<uint8_t>(ptr, ptr + water->M * water->N * sizeof(float));

							unsigned long buffLength = BUFF_SIZE;
							char b64[BUFF_SIZE];
							base64_encode(&buffer.front(), buffer.size(), b64, &buffLength);
							assert(buffLength < BUFF_SIZE);

							waterJson.AddMember("wTerm", RAPIDJSON_STR(str_duplicator.duplicate_string(b64)), allocator);
						}

#define SAME_NAME_WATER_MEMBER(x) waterJson.AddMember(#x, water->x, allocator)

						SAME_NAME_WATER_MEMBER(M);
						SAME_NAME_WATER_MEMBER(N);
						SAME_NAME_WATER_MEMBER(Lx);
						SAME_NAME_WATER_MEMBER(Lz);
						SAME_NAME_WATER_MEMBER(gravity);
						SAME_NAME_WATER_MEMBER(windvel);
						waterJson.AddMember("winddir", utils::json::make_json_array(water->winddir, 2, allocator), allocator);

						SAME_NAME_WATER_MEMBER(amplitude);
						waterJson.AddMember("codeConstant", utils::json::make_json_array(water->codeConstant, 4, allocator), allocator);


						textureJson.AddMember("water", waterJson, allocator);
					}
				}
				else if (textureDef->u.image)
				{
					textureJson.AddMember("image", RAPIDJSON_STR(textureDef->u.image->name), allocator);
					exporter::dump(game::native::XAssetType::ASSET_TYPE_IMAGE, { textureDef->u.image });
				}

				textureTable.PushBack(textureJson, allocator);
			}
		}

		output.AddMember("textureTable", textureTable, allocator);
		rapidjson::Value gfxDrawSurface(rapidjson::kObjectType);

#define SAME_NAME_GFXDRAWSURF_MEMBER(x) gfxDrawSurface.AddMember(#x, asset->info.drawSurf.fields.##x##, allocator)

		SAME_NAME_GFXDRAWSURF_MEMBER(objectId);
		SAME_NAME_GFXDRAWSURF_MEMBER(reflectionProbeIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(hasGfxEntIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(customIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(materialSortedIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(prepass);
		SAME_NAME_GFXDRAWSURF_MEMBER(useHeroLighting);
		SAME_NAME_GFXDRAWSURF_MEMBER(sceneLightIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(surfType);
		SAME_NAME_GFXDRAWSURF_MEMBER(primarySortKey);
		SAME_NAME_GFXDRAWSURF_MEMBER(unused);

		output.AddMember("gfxDrawSurface", gfxDrawSurface, allocator);

		output.AddMember("hashIndex", 0, allocator);

		rapidjson::Value stateBitsEntry(rapidjson::kArrayType);
		for (size_t i = 0; i < 48; i++)
		{
			stateBitsEntry.PushBack(asset->stateBitsEntry[i], allocator);
		}

		output.AddMember("stateBitsEntry", stateBitsEntry, allocator);
		output.AddMember("cameraRegion", asset->cameraRegion, allocator);

		if (asset->constantTable)
		{
			rapidjson::Value constantTable(rapidjson::kArrayType);

			for (char i = 0; i < asset->constantCount; ++i)
			{
				game::native::MaterialConstantDef constantDef;
				std::memcpy(&constantDef, &asset->constantTable[i], sizeof game::native::MaterialConstantDef);

				rapidjson::Value constantDefJson(rapidjson::kObjectType);

				constantDefJson.AddMember("nameHash", constantDef.nameHash, allocator);
				constantDefJson.AddMember("literal", utils::json::make_json_array(constantDef.literal, 4, allocator), allocator);


				std::string constantDefName = constantDef.name;
				constantDefName = constantDefName.substr(0, 12);

				constantDefJson.AddMember("name", RAPIDJSON_STR(str_duplicator.duplicate_string(constantDefName.c_str())), allocator);


				constantTable.PushBack(constantDefJson, allocator);
			}

			output.AddMember("constantTable", constantTable, allocator);
		}

		if (asset->stateBitsTable)
		{
			output.AddMember("stateBitsTable", statebits_to_json_array(asset->stateBitsTable, asset->stateBitsCount, allocator), allocator);
		}

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		utils::io::write_file(std::format("{}/materials/{}.iw4x.json", get_export_path(), asset->info.name), buff.GetString());

	}


	rapidjson::Value imaterial::statebits_to_json_array(game::native::GfxStateBits* stateBits, unsigned char count, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
	{
		rapidjson::Value arr(rapidjson::kArrayType);

		for (auto index = 0u; index < count; index++)
		{
			const auto& entry = stateBits[index];

			const auto srcBlendRgb = (entry.flags.loadbit0 & game::native::GFXS0_SRCBLEND_RGB_MASK) >> game::native::GFXS0_SRCBLEND_RGB_SHIFT;
			const auto dstBlendRgb = (entry.flags.loadbit0 & game::native::GFXS0_DSTBLEND_RGB_MASK) >> game::native::GFXS0_DSTBLEND_RGB_SHIFT;
			const auto blendOpRgb = (entry.flags.loadbit0 & game::native::GFXS0_BLENDOP_RGB_MASK) >> game::native::GFXS0_BLENDOP_RGB_SHIFT;
			const auto srcBlendAlpha = (entry.flags.loadbit0 & game::native::GFXS0_SRCBLEND_ALPHA_MASK) >> game::native::GFXS0_SRCBLEND_ALPHA_SHIFT;
			const auto dstBlendAlpha = (entry.flags.loadbit0 & game::native::GFXS0_DSTBLEND_ALPHA_MASK) >> game::native::GFXS0_DSTBLEND_ALPHA_SHIFT;
			const auto blendOpAlpha = (entry.flags.loadbit0 & game::native::GFXS0_BLENDOP_ALPHA_MASK) >> game::native::GFXS0_BLENDOP_ALPHA_SHIFT;
			const auto depthTest = (entry.flags.loadbit1 & game::native::GFXS1_DEPTHTEST_DISABLE) ? -1 : (entry.flags.loadbit1 & game::native::GFXS1_DEPTHTEST_MASK) >> game::native::GFXS1_DEPTHTEST_SHIFT;
			const auto polygonOffset = (entry.flags.loadbit1 & game::native::GFXS1_POLYGON_OFFSET_MASK) >> game::native::GFXS1_POLYGON_OFFSET_SHIFT;

			const auto* alphaTest = "disable";
			if ((entry.flags.loadbit0 & game::native::GFXS0_ATEST_MASK) == game::native::GFXS0_ATEST_GE_128)
			{
				alphaTest = ">=128";
			}
			else if ((entry.flags.loadbit0 & game::native::GFXS0_ATEST_MASK) == game::native::GFXS0_ATEST_GT_0)
			{
				alphaTest = ">0";
			}
			else if ((entry.flags.loadbit0 & game::native::GFXS0_ATEST_MASK) == game::native::GFXS0_ATEST_LT_128)
			{
				alphaTest = "<128";
			}
			else
			{
				assert(entry.flags.loadbit0 & game::native::GFXS0_ATEST_DISABLE);
			}

			const auto* cullFace = "none";
			if ((entry.flags.loadbit0 & game::native::GFXS0_CULL_MASK) == game::native::GFXS0_CULL_BACK)
			{
				cullFace = "back";
			}
			else if ((entry.flags.loadbit0 & game::native::GFXS0_CULL_MASK) == game::native::GFXS0_CULL_FRONT)
			{
				cullFace = "front";
			}
			else
			{
				assert((entry.flags.loadbit0 & game::native::GFXS0_CULL_MASK) == game::native::GFXS0_CULL_NONE);
			}

			rapidjson::Value stateBitEntry(rapidjson::kObjectType);

			const auto colorWriteRgb = entry.flags.loadbit0 & game::native::GFXS0_COLORWRITE_RGB ? true : false;
			const auto colorWriteAlpha = entry.flags.loadbit0 & game::native::GFXS0_COLORWRITE_ALPHA ? true : false;
			const auto polymodeLine = entry.flags.loadbit0 & game::native::GFXS0_POLYMODE_LINE ? true : false;
			const auto gammaWrite = entry.flags.loadbit0 & game::native::GFXS0_GAMMAWRITE ? true : false;
			const auto depthWrite = (entry.flags.loadbit1 & game::native::GFXS1_DEPTHWRITE) ? true : false;
			const auto stencilFrontEnabled = (entry.flags.loadbit1 & game::native::GFXS1_STENCIL_FRONT_ENABLE) ? true : false;
			const auto stencilBackEnabled = (entry.flags.loadbit1 & game::native::GFXS1_STENCIL_BACK_ENABLE) ? true : false;
			const auto stencilFrontPass = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_FRONT_PASS_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilFrontFail = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_FRONT_FAIL_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilFrontZFail = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_FRONT_ZFAIL_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilFrontFunc = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_FRONT_FUNC_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilBackPass = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_BACK_PASS_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilBackFail = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_BACK_FAIL_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilBackZFail = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_BACK_ZFAIL_SHIFT) & game::native::GFXS_STENCILOP_MASK;
			const auto stencilBackFunc = (entry.flags.loadbit1 >> game::native::GFXS1_STENCIL_BACK_FUNC_SHIFT) & game::native::GFXS_STENCILOP_MASK;

#define ADD_TO_JSON(x) stateBitEntry.AddMember(#x, x, allocator)
#define ADD_TO_JSON_STR(x) stateBitEntry.AddMember(#x, RAPIDJSON_STR(x), allocator)

			ADD_TO_JSON_STR(alphaTest);
			ADD_TO_JSON(blendOpAlpha);
			ADD_TO_JSON(blendOpRgb);
			ADD_TO_JSON(colorWriteAlpha);
			ADD_TO_JSON(colorWriteRgb);
			ADD_TO_JSON_STR(cullFace);
			ADD_TO_JSON(depthTest);
			ADD_TO_JSON(depthWrite);
			ADD_TO_JSON(dstBlendAlpha);
			ADD_TO_JSON(dstBlendRgb);
			ADD_TO_JSON(gammaWrite);
			ADD_TO_JSON(polygonOffset);
			ADD_TO_JSON(polymodeLine);
			ADD_TO_JSON(srcBlendRgb);
			ADD_TO_JSON(srcBlendAlpha);
			ADD_TO_JSON(stencilBackEnabled);
			ADD_TO_JSON(stencilBackFail);
			ADD_TO_JSON(stencilBackFunc);
			ADD_TO_JSON(stencilBackPass);
			ADD_TO_JSON(stencilBackZFail);
			ADD_TO_JSON(stencilFrontEnabled);
			ADD_TO_JSON(stencilFrontFail);
			ADD_TO_JSON(stencilFrontFunc);
			ADD_TO_JSON(stencilFrontPass);
			ADD_TO_JSON(stencilFrontZFail);

#if DEBUG
			stateBitEntry.AddMember("check0", entry.flags.loadbit0, allocator);
			stateBitEntry.AddMember("check1", entry.flags.loadbit1, allocator);
#endif

			arr.PushBack(stateBitEntry, allocator);
		}

		return arr;
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

						asset_dmper->dump(header, true);
						m.unlock();

						}, this, false);
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_MATERIAL, name, false);

					if (header.data)
					{
						dump(header, true);
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