#include <std_include.hpp>

#include "igfxworld.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module\exporter.hpp>

namespace asset_dumpers
{
	void igfxworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.gfxWorld);

		auto iw4_world = local_allocator.allocate<iw4::native::GfxWorld>();
		auto native_world = header.gfxWorld;

#define SET_WORLD_MEMBER(member_name) iw4_world->##member_name## = (native_world->##member_name##)

		SET_WORLD_MEMBER(name);
		SET_WORLD_MEMBER(baseName);
		SET_WORLD_MEMBER(planeCount);
		SET_WORLD_MEMBER(nodeCount);
		SET_WORLD_MEMBER(surfaceCount);
		SET_WORLD_MEMBER(skyCount);

		iw4_world->skies = local_allocator.allocate_array<iw4::native::GfxSky>(native_world->skyCount);
		static_assert(sizeof iw4::native::GfxSky == sizeof game::native::GfxSky);
		memcpy(iw4_world->skies, native_world->skies, native_world->skyCount * sizeof iw4::native::GfxSky);

		for (auto i = 0; i < native_world->skyCount; i++)
		{
			auto sky = &iw4_world->skies[i];
			sky->skyImage = exporter::convert(game::native::XAssetType::ASSET_TYPE_IMAGE, { native_world->skies[i].skyImage }).image;
		}

		SET_WORLD_MEMBER(lastSunPrimaryLightIndex);
		SET_WORLD_MEMBER(primaryLightCount);
		SET_WORLD_MEMBER(sortKeyLitDecal);
		SET_WORLD_MEMBER(sortKeyEffectDecal);
		SET_WORLD_MEMBER(sortKeyEffectAuto);
		SET_WORLD_MEMBER(sortKeyDistortion);
		SET_WORLD_MEMBER(dpvsPlanes);
		SET_WORLD_MEMBER(aabbTreeCounts);
		SET_WORLD_MEMBER(aabbTrees);

		iw4_world->cells = local_allocator.allocate_array<iw4::native::GfxCell>(native_world->dpvsPlanes.cellCount);
		for (auto i = 0; i < native_world->dpvsPlanes.cellCount; i++)
		{
			// This is okay because the two parameters we're missing are the last two
			// and for the rest, struct is the same size. so i guess we can do this?
			memcpy(&iw4_world->cells[i], &native_world->cells[i], sizeof iw4::native::GfxCell);
		}

		iw4_world->draw = convert(native_world, native_world->draw);
		iw4_world->lightGrid = convert(native_world, native_world->lightGrid);

		SET_WORLD_MEMBER(modelCount);
		SET_WORLD_MEMBER(models);
		SET_WORLD_MEMBER(bounds);
		SET_WORLD_MEMBER(checksum);
		SET_WORLD_MEMBER(materialMemoryCount);

		iw4_world->materialMemory = local_allocator.allocate_array<iw4::native::MaterialMemory>(iw4_world->materialMemoryCount);
		for (auto i = 0; i < iw4_world->materialMemoryCount; i++)
		{
			iw4_world->materialMemory[i].memory = native_world->materialMemory[i].memory;
			iw4_world->materialMemory[i].material = exporter::convert(game::native::ASSET_TYPE_MATERIAL, {native_world->materialMemory[i].material}).material;
		}

		// sun
		static_assert(sizeof iw4_world->sun == sizeof native_world->sun);
		memcpy(&iw4_world->sun, &native_world->sun, sizeof iw4_world->sun);

		if (native_world->sun.flareMaterial)
		{
			iw4_world->sun.flareMaterial = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { native_world->sun.flareMaterial }).material;
		}

		if (native_world->sun.spriteMaterial)
		{
			iw4_world->sun.spriteMaterial = exporter::convert(game::native::ASSET_TYPE_MATERIAL, { native_world->sun.spriteMaterial }).material;
		}

		static_assert(sizeof iw4_world->outdoorLookupMatrix == sizeof native_world->outdoorLookupMatrix);
		memcpy(iw4_world->outdoorLookupMatrix, native_world->outdoorLookupMatrix, sizeof iw4_world->outdoorLookupMatrix);

		if (native_world->outdoorImage)
		{
			iw4_world->outdoorImage = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_world->outdoorImage }).image;
		}

		SET_WORLD_MEMBER(cellCasterBits);
		SET_WORLD_MEMBER(cellHasSunLitSurfsBits);
		SET_WORLD_MEMBER(sceneDynModel);
		SET_WORLD_MEMBER(sceneDynBrush);
		SET_WORLD_MEMBER(primaryLightEntityShadowVis);

		static_assert(sizeof iw4_world->primaryLightDynEntShadowVis == sizeof native_world->primaryLightDynEntShadowVis);
		memcpy(iw4_world->primaryLightDynEntShadowVis, native_world->primaryLightDynEntShadowVis, sizeof iw4_world->primaryLightDynEntShadowVis);

		SET_WORLD_MEMBER(nonSunPrimaryLightForModelDynEnt);
		SET_WORLD_MEMBER(shadowGeom);
		SET_WORLD_MEMBER(lightRegion);
		SET_WORLD_MEMBER(primaryLightEntityShadowVis);
		SET_WORLD_MEMBER(primaryLightEntityShadowVis);

		iw4_world->dpvs = convert(native_world, native_world->dpvs);

		SET_WORLD_MEMBER(dpvsDyn);
		SET_WORLD_MEMBER(mapVtxChecksum);
		SET_WORLD_MEMBER(heroOnlyLightCount);

		if (iw4_world->heroOnlyLightCount)
		{
			iw4_world->heroOnlyLights = local_allocator.allocate_array<iw4::native::GfxHeroOnlyLight>(iw4_world->heroOnlyLightCount);
			for (size_t i = 0; i < iw4_world->heroOnlyLightCount; i++)
			{
				auto iw4_light = &iw4_world->heroOnlyLights[i];
				auto native_light = &native_world->heroOnlyLights[i];

				iw4_light->type = native_light->type;
				memcpy(iw4_light->unused, native_light->unused, sizeof native_light->unused);
				memcpy(iw4_light->color, native_light->color, sizeof native_light->color);
				memcpy(iw4_light->dir, native_light->dir, sizeof native_light->dir);
				memcpy(iw4_light->origin, native_light->origin, sizeof native_light->origin);
				iw4_light->radius = native_light->radius;
				iw4_light->cosHalfFovOuter = native_light->cosHalfFovOuter;
				iw4_light->cosHalfFovInner = native_light->cosHalfFovInner;
				iw4_light->exponent = native_light->exponent;
			}
		}

		SET_WORLD_MEMBER(fogTypesAllowed);

		iw4_world->checksum = 0xC0D80000;

		iw4_world->name = exporter::fix_map_name(iw4_world->name, local_allocator);
		iw4_world->baseName = exporter::fix_map_name(iw4_world->baseName, local_allocator);

		out.gfxWorld = iw4_world;
	}

	void igfxworld::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_GFXWORLD, header.data);
		assert(result);
	}

	iw4::native::GfxLightGrid igfxworld::convert(game::native::GfxWorld* native_world, const game::native::GfxLightGrid& native_lightgrid)
	{
		iw4::native::GfxLightGrid iw4_lightgrid{};

#define SET_LIGHTGRID_MEMBER(member_name) iw4_lightgrid.##member_name## = (native_lightgrid.##member_name##)

		SET_LIGHTGRID_MEMBER(hasLightRegions);
		SET_LIGHTGRID_MEMBER(lastSunPrimaryLightIndex);

		memcpy(iw4_lightgrid.mins, native_lightgrid.mins, sizeof native_lightgrid.mins);
		memcpy(iw4_lightgrid.maxs, native_lightgrid.maxs, sizeof native_lightgrid.maxs);

		SET_LIGHTGRID_MEMBER(rowAxis);
		SET_LIGHTGRID_MEMBER(colAxis);
		SET_LIGHTGRID_MEMBER(rowDataStart);
		SET_LIGHTGRID_MEMBER(rawRowDataSize);
		SET_LIGHTGRID_MEMBER(rawRowData);
		SET_LIGHTGRID_MEMBER(entryCount);
		SET_LIGHTGRID_MEMBER(entries);
		SET_LIGHTGRID_MEMBER(colorCount);
		SET_LIGHTGRID_MEMBER(colors);

#undef SET_LIGHTGRID_MEMBER

		return iw4_lightgrid;
	}

	iw4::native::GfxWorldDpvsStatic igfxworld::convert(game::native::GfxWorld* native_world, const game::native::GfxWorldDpvsStatic& native_dpvs_static)
	{
		iw4::native::GfxWorldDpvsStatic iw4_dpvs_static{};

		static_assert(sizeof game::native::GfxWorldDpvsStatic == sizeof iw4::native::GfxWorldDpvsStatic);
		std::memcpy(&iw4_dpvs_static, &native_dpvs_static, sizeof iw4::native::GfxWorldDpvsStatic);

		if (native_dpvs_static.surfacesBounds)
		{
			iw4_dpvs_static.surfacesBounds = local_allocator.allocate_array<game::native::GfxSurfaceBounds>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static.surfacesBounds, native_dpvs_static.surfacesBounds, sizeof game::native::GfxSurfaceBounds * native_world->surfaceCount);
		}

		if (native_dpvs_static.smodelInsts)
		{
			iw4_dpvs_static.smodelInsts = local_allocator.allocate_array<game::native::GfxStaticModelInst>(native_dpvs_static.smodelCount);
			std::memcpy(iw4_dpvs_static.smodelInsts, native_dpvs_static.smodelInsts, sizeof game::native::GfxStaticModelInst * native_dpvs_static.smodelCount);
		}

		if (native_dpvs_static.surfaces)
		{
			iw4_dpvs_static.surfaces = local_allocator.allocate_array<iw4::native::GfxSurface>(native_world->surfaceCount);
			for (unsigned int i = 0; i < native_world->surfaceCount; ++i)
			{
				game::native::GfxSurface* native_surface = &native_dpvs_static.surfaces[i];
				iw4::native::GfxSurface* iw4_surface = &iw4_dpvs_static.surfaces[i];

				static_assert(sizeof game::native::GfxSurface == sizeof iw4::native::GfxSurface);
				std::memcpy(iw4_surface, native_surface, sizeof  iw4::native::GfxSurface);

				if (iw4_surface->material)
				{
					iw4_surface->material = exporter::convert(game::native::XAssetType::ASSET_TYPE_MATERIAL, { iw4_surface->material }).material;
				}
			}
		}

		if (native_dpvs_static.smodelDrawInsts)
		{
			// data is already good but we need to dump every model
			for (unsigned int i = 0; i < iw4_dpvs_static.smodelCount; i++)
			{
				auto inst = &iw4_dpvs_static.smodelDrawInsts[i];
				inst->model = exporter::convert(game::native::XAssetType::ASSET_TYPE_XMODEL, { inst->model }).model;
			}
		}

		if (native_dpvs_static.surfaceMaterials)
		{
			iw4_dpvs_static.surfaceMaterials = local_allocator.allocate_array<game::native::GfxDrawSurf>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static.surfaceMaterials, native_dpvs_static.surfaceMaterials, sizeof game::native::GfxDrawSurf * native_world->surfaceCount);
		}

		if (native_dpvs_static.surfaceCastsSunShadow)
		{
			iw4_dpvs_static.surfaceCastsSunShadow = local_allocator.allocate_array<unsigned int>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static.surfaceCastsSunShadow, native_dpvs_static.surfaceCastsSunShadow, 4 * native_world->surfaceCount);
		}


		return iw4_dpvs_static;
	}

	iw4::native::GfxWorldDraw igfxworld::convert(game::native::GfxWorld* world, const game::native::GfxWorldDraw& native_draw)
	{
		iw4::native::GfxWorldDraw iw4_draw{};

#define SET_DRAW_MEMBER(member_name) iw4_draw.##member_name## = (native_draw.##member_name##)

		SET_DRAW_MEMBER(reflectionProbeCount);

		iw4_draw.reflectionProbes = local_allocator.allocate_array<iw4::native::GfxImage*>(iw4_draw.reflectionProbeCount);
		iw4_draw.reflectionProbeOrigins = local_allocator.allocate_array<game::native::GfxReflectionProbe>(iw4_draw.reflectionProbeCount);
		iw4_draw.reflectionProbeTextures = local_allocator.allocate_array<game::native::GfxTexture>(iw4_draw.reflectionProbeCount);

		for (size_t i = 0; i < iw4_draw.reflectionProbeCount; i++)
		{
			assert(native_draw.reflectionProbes[i]);
			
			iw4_draw.reflectionProbes[i] = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_draw.reflectionProbes[i] }).image;
			iw4_draw.reflectionProbeOrigins[i] = native_draw.reflectionProbeOrigins[i];
			iw4_draw.reflectionProbeTextures[i] = native_draw.reflectionProbeTextures[i];
		}

		SET_DRAW_MEMBER(lightmapCount);
		iw4_draw.lightmaps = local_allocator.allocate_array<iw4::native::GfxLightmapArray>(iw4_draw.lightmapCount);
		for (auto i = 0; i < iw4_draw.lightmapCount; i++)
		{
			if (native_draw.lightmaps[i].primary)
			{
				iw4_draw.lightmaps[i].primary = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmaps[i].primary }).image;
			}

			if (native_draw.lightmaps[i].secondary)
			{
				iw4_draw.lightmaps[i].secondary = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmaps[i].secondary }).image;
			}
		}

		SET_DRAW_MEMBER(lightmapPrimaryTextures);
		SET_DRAW_MEMBER(lightmapSecondaryTextures);

		if (native_draw.lightmapOverridePrimary)
		{
			iw4_draw.lightmapOverridePrimary = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmapOverridePrimary }).image;
		}

		if (native_draw.lightmapOverrideSecondary)
		{
			iw4_draw.lightmapOverrideSecondary = exporter::convert(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmapOverrideSecondary }).image;
		}

		SET_DRAW_MEMBER(vertexCount);
		SET_DRAW_MEMBER(vd);
		SET_DRAW_MEMBER(vertexLayerDataSize);
		SET_DRAW_MEMBER(vld);
		SET_DRAW_MEMBER(indexCount);
		SET_DRAW_MEMBER(indices);

#undef SET_DRAW_MEMBER

		return iw4_draw;
	}

	igfxworld::igfxworld()
	{
		command::add("dumpGfxWorld", [&](const command::params& params)
			{
				game::native::XAssetHeader out{};

				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_GFXWORLD, [](game::native::XAssetHeader header, void* data) {
					auto out = reinterpret_cast<game::native::XAssetHeader*>(data);
					*out = header;

					}, &out, false);

				if (out.data)
				{
					const auto converted = convert_and_write(out);
					exporter::add_to_source(game::native::ASSET_TYPE_GFXWORLD, converted.gfxWorld->name);
				}
			});
	}
}