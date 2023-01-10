#include <std_include.hpp>

#include "igfxworld.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module\exporter.hpp>

#define IW4X_GFXMAP_VERSION 1

namespace asset_dumpers
{

	void igfxworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.gfxWorld);

		auto iw4_world = local_allocator.allocate<iw4::native::GfxWorld>();
		auto native_world = header.gfxWorld;

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_world->##member_name## = (native_world->##member_name##)

		SET_MEMBER_NO_CONVERSION(name);
		SET_MEMBER_NO_CONVERSION(baseName);
		SET_MEMBER_NO_CONVERSION(planeCount);
		SET_MEMBER_NO_CONVERSION(nodeCount);
		SET_MEMBER_NO_CONVERSION(surfaceCount);
		SET_MEMBER_NO_CONVERSION(skyCount);

		iw4_world->skies = local_allocator.allocate_array<iw4::native::GfxSky>(native_world->skyCount);
		static_assert(sizeof iw4::native::GfxSky == sizeof game::native::GfxSky);
		memcpy(iw4_world->skies, native_world->skies, native_world->skyCount);

		for (auto i = 0; i < native_world->skyCount; i++)
		{
			auto sky = &iw4_world->skies[i];
			sky->skyImage = exporter::dump(game::native::XAssetType::ASSET_TYPE_IMAGE, { native_world->skies[i].skyImage }).image;
		}

		SET_MEMBER_NO_CONVERSION(lastSunPrimaryLightIndex);
		SET_MEMBER_NO_CONVERSION(primaryLightCount);
		SET_MEMBER_NO_CONVERSION(sortKeyLitDecal);
		SET_MEMBER_NO_CONVERSION(sortKeyEffectDecal);
		SET_MEMBER_NO_CONVERSION(sortKeyEffectAuto);
		SET_MEMBER_NO_CONVERSION(sortKeyDistortion);
		SET_MEMBER_NO_CONVERSION(dpvsPlanes);
		SET_MEMBER_NO_CONVERSION(aabbTreeCounts);
		SET_MEMBER_NO_CONVERSION(aabbTrees);

		iw4_world->cells = local_allocator.allocate_array<iw4::native::GfxCell>(native_world->dpvsPlanes.cellCount);
		for (auto i = 0; i < native_world->dpvsPlanes.cellCount; i++)
		{
			// This is okay because the two parameters we're missing are the last two
			// and for the rest, struct is the same size. so i guess we can do this?
			memcpy(&iw4_world->cells[i], &native_world->cells[i], sizeof iw4::native::GfxCell);
		}

		iw4_world->draw = convert(native_world, native_world->draw);
		iw4_world->lightGrid = convert(native_world, native_world->lightGrid);

		SET_MEMBER_NO_CONVERSION(modelCount);
		SET_MEMBER_NO_CONVERSION(models);
		SET_MEMBER_NO_CONVERSION(bounds);
		SET_MEMBER_NO_CONVERSION(checksum);
		SET_MEMBER_NO_CONVERSION(materialMemoryCount);

		iw4_world->materialMemory = local_allocator.allocate_array<iw4::native::MaterialMemory>(iw4_world->materialMemoryCount);
		for (auto i = 0; i < iw4_world->materialMemoryCount; i++)
		{
			iw4_world->materialMemory[i].memory = native_world->materialMemory[i].memory;
			iw4_world->materialMemory[i].material = exporter::dump(game::native::ASSET_TYPE_MATERIAL, {native_world->materialMemory[i].material}).material;
		}

		// sun
		static_assert(sizeof iw4_world->sun == sizeof native_world->sun);
		memcpy(&iw4_world->sun, &native_world->sun, sizeof iw4_world->sun);

		if (native_world->sun.flareMaterial)
		{
			iw4_world->sun.flareMaterial = exporter::dump(game::native::ASSET_TYPE_MATERIAL, { native_world->sun.flareMaterial }).material;
		}

		if (native_world->sun.spriteMaterial)
		{
			iw4_world->sun.spriteMaterial = exporter::dump(game::native::ASSET_TYPE_MATERIAL, { native_world->sun.spriteMaterial }).material;
		}

		static_assert(sizeof iw4_world->outdoorLookupMatrix == sizeof native_world->outdoorLookupMatrix);
		memcpy(iw4_world->outdoorLookupMatrix, native_world->outdoorLookupMatrix, sizeof iw4_world->outdoorLookupMatrix);

		if (iw4_world->outdoorImage)
		{
			iw4_world->outdoorImage = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_world->outdoorImage }).image;
		}

		SET_MEMBER_NO_CONVERSION(cellCasterBits);
		SET_MEMBER_NO_CONVERSION(cellHasSunLitSurfsBits);
		SET_MEMBER_NO_CONVERSION(sceneDynModel);
		SET_MEMBER_NO_CONVERSION(sceneDynBrush);
		SET_MEMBER_NO_CONVERSION(primaryLightEntityShadowVis);

		static_assert(sizeof iw4_world->primaryLightDynEntShadowVis == sizeof native_world->primaryLightDynEntShadowVis);
		memcpy(iw4_world->primaryLightDynEntShadowVis, native_world->primaryLightDynEntShadowVis, sizeof iw4_world->primaryLightDynEntShadowVis);

		SET_MEMBER_NO_CONVERSION(nonSunPrimaryLightForModelDynEnt);
		SET_MEMBER_NO_CONVERSION(shadowGeom);
		SET_MEMBER_NO_CONVERSION(lightRegion);
		SET_MEMBER_NO_CONVERSION(primaryLightEntityShadowVis);
		SET_MEMBER_NO_CONVERSION(primaryLightEntityShadowVis);

		iw4_world->dpvs = convert(native_world, native_world->dpvs);

		SET_MEMBER_NO_CONVERSION(dpvsDyn);
		SET_MEMBER_NO_CONVERSION(mapVtxChecksum);
		SET_MEMBER_NO_CONVERSION(heroOnlyLightCount);

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

		SET_MEMBER_NO_CONVERSION(fogTypesAllowed);

		iw4_world->checksum = 0xC0D80000;

		out.gfxWorld = iw4_world;
	}

	void igfxworld::write(const iw4::native::XAssetHeader& header)
	{
		const auto asset = header.gfxWorld;

		utils::stream buffer;
		buffer.saveArray("IW4xGfxW", 8);
		buffer.saveObject(IW4X_GFXMAP_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->baseName)
		{
			buffer.saveString(asset->baseName);
		}

		if (asset->skies)
		{
			buffer.saveArray(asset->skies, asset->skyCount);

			for (int i = 0; i < asset->skyCount; ++i)
			{
				iw4::native::GfxSky* sky = &asset->skies[i];

				if (sky->skyStartSurfs)
				{
					buffer.saveArray(sky->skyStartSurfs, sky->skySurfCount);
				}

				if (sky->skyImage)
				{
					buffer.saveString(sky->skyImage->name);
				}
			}
		}

		igfxworld::write(asset, &asset->dpvsPlanes, &buffer);

		int cellCount = asset->dpvsPlanes.cellCount;

		if (asset->aabbTreeCounts)
		{
			buffer.saveArray(asset->aabbTreeCounts, cellCount);
		}

		if (asset->aabbTrees)
		{
			buffer.saveArray(asset->aabbTrees, cellCount);

			for (int i = 0; i < cellCount; ++i)
			{
				game::native::GfxCellTree* cellTree = &asset->aabbTrees[i];

				if (cellTree->aabbTree)
				{
					AssertSize(game::native::GfxAabbTree, 44);
					buffer.saveArray(cellTree->aabbTree, asset->aabbTreeCounts[i].aabbTreeCount);

					for (int j = 0; j < asset->aabbTreeCounts[i].aabbTreeCount; ++j)
					{
						game::native::GfxAabbTree* aabbTree = &cellTree->aabbTree[j];

						if (aabbTree->smodelIndexes)
						{
							for (unsigned short k = 0; k < aabbTree->smodelIndexCount; ++k)
							{
								buffer.saveObject(aabbTree->smodelIndexes[k]);
							}
						}
					}
				}
			}
		}

		if (asset->cells)
		{
			AssertSize(iw4::native::GfxCell, 40);
			buffer.saveArray(asset->cells, cellCount);

			for (int i = 0; i < cellCount; ++i)
			{
				iw4::native::GfxCell* cell = &asset->cells[i];

				if (cell->portals)
				{
					AssertSize(game::native::GfxPortal, 60);
					buffer.saveArray(cell->portals, cell->portalCount);

					for (int j = 0; j < cell->portalCount; ++j)
					{
						game::native::GfxPortal* portal = &cell->portals[j];

						if (portal->vertices)
						{
							buffer.saveArray(portal->vertices, portal->vertexCount);
						}
					}
				}

				if (cell->reflectionProbes)
				{
					// no align for char
					buffer.saveArray(cell->reflectionProbes, cell->reflectionProbeCount);
				}
			}
		}

		igfxworld::write(&asset->draw, &buffer);
		igfxworld::write(&asset->lightGrid, &buffer);

		if (asset->models)
		{
			AssertSize(game::native::GfxBrushModel, 60);

			buffer.saveArray(asset->models, asset->modelCount);
		}

		if (asset->materialMemory)
		{
			AssertSize(iw4::native::MaterialMemory, 8);
			buffer.saveArray(asset->materialMemory, asset->materialMemoryCount);

			for (int i = 0; i < asset->materialMemoryCount; ++i)
			{
				iw4::native::MaterialMemory* materialMemory = &asset->materialMemory[i];

				if (materialMemory->material)
				{
					buffer.saveString(materialMemory->material->info.name);
				}
			}
		}

		if (asset->sun.spriteMaterial)
		{
			buffer.saveString(asset->sun.spriteMaterial->info.name);
		}

		if (asset->sun.flareMaterial)
		{
			buffer.saveString(asset->sun.flareMaterial->info.name);
		}

		if (asset->outdoorImage)
		{
			buffer.saveString(asset->outdoorImage->name);
		}

		if (asset->shadowGeom)
		{
			AssertSize(game::native::GfxShadowGeometry, 12);
			buffer.saveArray(asset->shadowGeom, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				game::native::GfxShadowGeometry* shadowGeometry = &asset->shadowGeom[i];

				if (shadowGeometry->sortedSurfIndex)
				{
					buffer.saveArray(shadowGeometry->sortedSurfIndex, shadowGeometry->surfaceCount);
				}

				if (shadowGeometry->smodelIndex)
				{
					buffer.saveArray(shadowGeometry->smodelIndex, shadowGeometry->smodelCount);
				}
			}
		}

		if (asset->lightRegion)
		{
			AssertSize(game::native::GfxLightRegion, 8);
			buffer.saveArray(asset->lightRegion, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				game::native::GfxLightRegion* lightRegion = &asset->lightRegion[i];

				if (lightRegion->hulls)
				{
					AssertSize(game::native::GfxLightRegionHull, 80);
					buffer.saveArray(lightRegion->hulls, lightRegion->hullCount);

					for (unsigned int j = 0; j < lightRegion->hullCount; ++j)
					{
						game::native::GfxLightRegionHull* lightRegionHull = &lightRegion->hulls[j];

						if (lightRegionHull->axis)
						{
							AssertSize(game::native::GfxLightRegionAxis, 20);
							buffer.saveArray(lightRegionHull->axis, lightRegionHull->axisCount);
						}
					}
				}
			}
		}

		igfxworld::write(asset, &asset->dpvs, &buffer);

		// Obsolete, IW3 has no support for that
		if (asset->heroOnlyLights)
		{
			AssertSize(iw4::native::GfxHeroOnlyLight, 56);
			buffer.saveArray(asset->heroOnlyLights, asset->heroOnlyLightCount);
		}

		utils::io::write_file(std::format("{}/gfxworld/{}.iw4xGfxWorld", get_export_path(), asset->baseName), buffer.toBuffer());
	}

	iw4::native::GfxLightGrid igfxworld::convert(game::native::GfxWorld* native_world, const game::native::GfxLightGrid& native_lightgrid)
	{
		iw4::native::GfxLightGrid iw4_lightgrid{};

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_lightgrid.##member_name## = (native_lightgrid.##member_name##)

		SET_MEMBER_NO_CONVERSION(hasLightRegions);
		SET_MEMBER_NO_CONVERSION(lastSunPrimaryLightIndex);

		memcpy(iw4_lightgrid.mins, native_lightgrid.mins, sizeof native_lightgrid.mins);
		memcpy(iw4_lightgrid.maxs, native_lightgrid.maxs, sizeof native_lightgrid.maxs);

		SET_MEMBER_NO_CONVERSION(rowAxis);
		SET_MEMBER_NO_CONVERSION(colAxis);
		SET_MEMBER_NO_CONVERSION(rowDataStart);
		SET_MEMBER_NO_CONVERSION(rawRowDataSize);
		SET_MEMBER_NO_CONVERSION(rawRowData);
		SET_MEMBER_NO_CONVERSION(entryCount);
		SET_MEMBER_NO_CONVERSION(entries);
		SET_MEMBER_NO_CONVERSION(colorCount);
		SET_MEMBER_NO_CONVERSION(colors);

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
					iw4_surface->material = exporter::dump(game::native::XAssetType::ASSET_TYPE_MATERIAL, { iw4_surface->material }).material;
				}
			}
		}

		if (native_dpvs_static.smodelDrawInsts)
		{
			// data is already good but we need to dump every model
			for (unsigned int i = 0; i < iw4_dpvs_static.smodelCount; i++)
			{
				auto inst = &iw4_dpvs_static.smodelDrawInsts[i];
				inst->model = exporter::dump(game::native::XAssetType::ASSET_TYPE_XMODEL, { inst->model }).model;
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

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_draw.##member_name## = (native_draw.##member_name##)

		SET_MEMBER_NO_CONVERSION(reflectionProbeCount);

		iw4_draw.reflectionProbes = local_allocator.allocate_array<iw4::native::GfxImage*>(iw4_draw.reflectionProbeCount);
		iw4_draw.reflectionProbeOrigins = local_allocator.allocate_array<game::native::GfxReflectionProbe>(iw4_draw.reflectionProbeCount);
		iw4_draw.reflectionProbeTextures = local_allocator.allocate_array<game::native::GfxTexture>(iw4_draw.reflectionProbeCount);

		for (size_t i = 0; i < iw4_draw.reflectionProbeCount; i++)
		{
			assert(native_draw.reflectionProbes[i]);
			
			iw4_draw.reflectionProbes[i] = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_draw.reflectionProbes[i] }).image;
			iw4_draw.reflectionProbeOrigins[i] = native_draw.reflectionProbeOrigins[i];
			iw4_draw.reflectionProbeTextures[i] = native_draw.reflectionProbeTextures[i];
		}

		SET_MEMBER_NO_CONVERSION(lightmapCount);
		iw4_draw.lightmaps = local_allocator.allocate_array<iw4::native::GfxLightmapArray>(iw4_draw.lightmapCount);
		for (auto i = 0; i < iw4_draw.lightmapCount; i++)
		{
			if (native_draw.lightmaps[i].primary)
			{
				iw4_draw.lightmaps[i].primary = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmaps[i].primary }).image;
			}

			if (native_draw.lightmaps[i].secondary)
			{
				iw4_draw.lightmaps[i].secondary = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmaps[i].secondary }).image;
			}
		}

		SET_MEMBER_NO_CONVERSION(lightmapPrimaryTextures);
		SET_MEMBER_NO_CONVERSION(lightmapSecondaryTextures);

		if (native_draw.lightmapOverridePrimary)
		{
			iw4_draw.lightmapOverridePrimary = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmapOverridePrimary }).image;
		}

		if (native_draw.lightmapOverrideSecondary)
		{
			iw4_draw.lightmapOverrideSecondary = exporter::dump(game::native::ASSET_TYPE_IMAGE, { native_draw.lightmapOverrideSecondary }).image;
		}

		SET_MEMBER_NO_CONVERSION(vertexCount);
		SET_MEMBER_NO_CONVERSION(vd);
		SET_MEMBER_NO_CONVERSION(vertexLayerDataSize);
		SET_MEMBER_NO_CONVERSION(vld);
		SET_MEMBER_NO_CONVERSION(indexCount);
		SET_MEMBER_NO_CONVERSION(indices);

		return iw4_draw;
	}

	void igfxworld::write(const iw4::native::GfxWorld* world, const iw4::native::GfxWorldDpvsStatic* asset, utils::stream* buffer)
	{
		AssertSize(iw4::native::GfxWorldDpvsStatic, 108);

		if (asset->sortedSurfIndex)
		{
			buffer->saveArray(asset->sortedSurfIndex, asset->staticSurfaceCount + asset->staticSurfaceCountNoDecal);
		}

		if (asset->smodelInsts)
		{
			AssertSize(game::native::GfxStaticModelInst, 36);
			buffer->saveArray(asset->smodelInsts, asset->smodelCount);
		}

		if (asset->surfaces)
		{
			AssertSize(iw4::native::GfxSurface, 24);
			buffer->saveArray(asset->surfaces, world->surfaceCount);

			for (unsigned int i = 0; i < world->surfaceCount; ++i)
			{
				iw4::native::GfxSurface* surface = &asset->surfaces[i];

				if (surface->material)
				{
					buffer->saveString(world->dpvs.surfaces[i].material->info.name); // Redundant, but too lazy to implement pointer support
				}
			}
		}

		if (asset->surfacesBounds)
		{
			AssertSize(game::native::GfxSurfaceBounds, 24);
			buffer->saveArray(asset->surfacesBounds, world->surfaceCount);
		}

		if (asset->smodelDrawInsts)
		{
			AssertSize(iw4::native::GfxStaticModelDrawInst, 76);

			buffer->saveArray(asset->smodelDrawInsts, asset->smodelCount);

			for (unsigned int i = 0; i < asset->smodelCount; ++i)
			{
				iw4::native::GfxStaticModelDrawInst* model = &asset->smodelDrawInsts[i];

				if (model->model)
				{
					buffer->saveString(model->model->name);
				}
			}
		}
	}

	void igfxworld::write(const iw4::native::GfxLightGrid* asset, utils::stream* buffer)
	{
		AssertSize(iw4::native::GfxLightGrid, 56);

		if (asset->rowDataStart)
		{
			buffer->saveArray(asset->rowDataStart, (asset->maxs[asset->rowAxis] - asset->mins[asset->rowAxis]) + 1);
		}

		if (asset->rawRowData)
		{
			// no align for char
			buffer->saveArray(asset->rawRowData, asset->rawRowDataSize);
		}

		if (asset->entries)
		{
			AssertSize(game::native::GfxLightGridEntry, 4);
			buffer->saveArray(asset->entries, asset->entryCount);
		}

		if (asset->colors)
		{
			AssertSize(game::native::GfxLightGridColors, 168);
			buffer->saveArray(asset->colors, asset->colorCount);
		}
	}

	void igfxworld::write(const iw4::native::GfxWorldDraw* asset, utils::stream* buffer)
	{
		AssertSize(iw4::native::GfxWorldDraw, 72);

		if (asset->reflectionProbes)
		{
			buffer->saveArray(asset->reflectionProbes, asset->reflectionProbeCount);

			for (unsigned int i = 0; i < asset->reflectionProbeCount; ++i)
			{
				if (asset->reflectionProbes[i])
				{
					buffer->saveString(asset->reflectionProbes[i]->name);
				}
			}
		}

		if (asset->reflectionProbeOrigins)
		{
			AssertSize(game::native::GfxReflectionProbe, 12);
			buffer->saveArray(asset->reflectionProbeOrigins, asset->reflectionProbeCount);
		}

		if (asset->lightmaps)
		{
			AssertSize(game::native::GfxLightmapArray, 8);
			buffer->saveArray(asset->lightmaps, asset->lightmapCount);

			for (int i = 0; i < asset->lightmapCount; ++i)
			{
				iw4::native::GfxLightmapArray* lightmapArray = &asset->lightmaps[i];

				if (lightmapArray->primary)
				{
					buffer->saveString(lightmapArray->primary->name);
				}

				if (lightmapArray->secondary)
				{
					buffer->saveString(lightmapArray->secondary->name);
				}
			}
		}

		if (asset->lightmapOverridePrimary)
		{
			buffer->saveString(asset->lightmapOverridePrimary->name);
		}

		if (asset->lightmapOverrideSecondary)
		{
			buffer->saveString(asset->lightmapOverrideSecondary->name);
		}

		// saveGfxWorldVertexData
		{
			if (asset->vd.vertices)
			{
				buffer->saveArray(asset->vd.vertices, asset->vertexCount);
			}
		}

		// saveGfxWorldVertexLayerData
		{
			if (asset->vld.data)
			{
				// no align for char
				buffer->saveArray(asset->vld.data, asset->vertexLayerDataSize);
			}
		}

		if (asset->indices)
		{
			buffer->saveArray(asset->indices, asset->indexCount);
		}

	}

	void igfxworld::write(const iw4::native::GfxWorld* world, const game::native::GfxWorldDpvsPlanes* asset, utils::stream* buffer)
	{
		AssertSize(game::native::GfxWorldDpvsPlanes, 16);

		if (asset->planes)
		{
			for (int i = 0; i < world->planeCount; ++i)
			{
				buffer->saveObject(asset->planes[i]);
			}
		}

		if (asset->nodes)
		{
			buffer->saveArray(asset->nodes, world->nodeCount);
		}
	}


	igfxworld::igfxworld()
	{
		command::add("dumpGfxWorld", [&](const command::params& params)
			{
				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_GFXWORLD, [](game::native::XAssetHeader header, void* data) {
					auto dumper = reinterpret_cast<igfxworld*>(data);

					dumper->dump(header);

					}, this, false);
			});
	}
}