#include <std_include.hpp>

#include "iclipmap.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"
#include <module\console.hpp>


namespace asset_dumpers
{

#define IW4X_CLIPMAP_VERSION 3

	void iclipmap::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.clipMap);

		auto iw4_clipmap = local_allocator.allocate<iw4::native::clipMap_t>();
		const auto native_clipmap = header.clipMap;

		iw4_clipmap->name = native_clipmap->name;
		iw4_clipmap->isInUse = native_clipmap->isInUse;

		iw4_clipmap->planeCount = native_clipmap->info.planeCount;
		iw4_clipmap->planes = native_clipmap->info.planes;
		iw4_clipmap->numStaticModels = native_clipmap->numStaticModels;

		iw4_clipmap->staticModelList = local_allocator.allocate_array<iw4::native::cStaticModel_s>(iw4_clipmap->numStaticModels);
		static_assert(sizeof iw4::native::cStaticModel_s == sizeof game::native::cStaticModel_s);
		memcpy(iw4_clipmap->staticModelList, native_clipmap->staticModelList, iw4_clipmap->numStaticModels * sizeof iw4::native::cStaticModel_s);
		for (size_t i = 0; i < iw4_clipmap->numStaticModels; i++)
		{
			auto iw4_csmodel = &iw4_clipmap->staticModelList[i];
			auto native_csmodel = &native_clipmap->staticModelList[i];

			iw4_csmodel->xmodel = exporter::dump(game::native::ASSET_TYPE_XMODEL, { native_csmodel->xmodel }).model;
		}

		iw4_clipmap->numMaterials = native_clipmap->info.numMaterials;
		iw4_clipmap->materials = native_clipmap->info.materials;
		iw4_clipmap->numBrushSides = native_clipmap->info.numBrushSides;
		iw4_clipmap->brushsides = native_clipmap->info.brushsides;
		iw4_clipmap->numBrushEdges = native_clipmap->info.numBrushEdges;
		iw4_clipmap->brushEdges = native_clipmap->info.brushEdges;
		iw4_clipmap->numNodes = native_clipmap->numNodes;
		iw4_clipmap->nodes = native_clipmap->nodes;
		iw4_clipmap->numLeafs = native_clipmap->numLeafs;
		iw4_clipmap->leafs = native_clipmap->leafs;
		iw4_clipmap->leafbrushNodesCount = native_clipmap->info.leafbrushNodesCount;
		iw4_clipmap->leafbrushNodes = native_clipmap->info.leafbrushNodes;
		iw4_clipmap->leafbrushes = native_clipmap->info.leafbrushes;

		// Leaf surfaces don't exist in iw4... leaving at 0 for now ?
		// I'm not sure what it corresponds to, maybe cleaf_t colllaabbtree concatenated?
		//iw4_clipmap->numLeafSurfaces = native_clipmap->numLeafSurfaces;
		//iw4_clipmap->leafsurfaces = native_clipmap->leafsurfaces;

		iw4_clipmap->vertCount = native_clipmap->vertCount;
		iw4_clipmap->verts = native_clipmap->verts;
		iw4_clipmap->triCount = native_clipmap->triCount;
		iw4_clipmap->triIndices = native_clipmap->triIndices;
		iw4_clipmap->triEdgeIsWalkable = native_clipmap->triEdgeIsWalkable;
		iw4_clipmap->borderCount = native_clipmap->borderCount;
		iw4_clipmap->borders = native_clipmap->borders;
		iw4_clipmap->partitionCount = native_clipmap->partitionCount;
		iw4_clipmap->partitions = native_clipmap->partitions;
		iw4_clipmap->aabbTreeCount = native_clipmap->aabbTreeCount;
		iw4_clipmap->aabbTrees = native_clipmap->aabbTrees;
		iw4_clipmap->numSubModels = native_clipmap->numSubModels;
		
		iw4_clipmap->cmodels = local_allocator.allocate_array<iw4::native::cmodel_t>(iw4_clipmap->numSubModels);
		for (size_t i = 0; i < iw4_clipmap->numSubModels; i++)
		{
			auto iw4_model = &iw4_clipmap->cmodels[i];
			auto native_model = &native_clipmap->cmodels[i];

			iw4_model->bounds = native_model->bounds;
			iw4_model->radius = native_model->radius;
			iw4_model->leaf = native_model->leaf;
		}


		iw4_clipmap->numBrushes = native_clipmap->info.numBrushes;
		iw4_clipmap->brushes = native_clipmap->info.brushes;
		iw4_clipmap->brushBounds = native_clipmap->info.brushBounds;
		iw4_clipmap->brushContents = native_clipmap->info.brushContents;

		auto clip = native_clipmap;
		
		iw4_clipmap->mapEnts = exporter::dump(game::native::ASSET_TYPE_MAP_ENTS, { native_clipmap->mapEnts }).mapEnts;

		iw4_clipmap->smodelNodeCount = native_clipmap->smodelNodeCount;
		iw4_clipmap->smodelNodes = native_clipmap->smodelNodes;


		memcpy(iw4_clipmap->dynEntCount, native_clipmap->dynEntCount, sizeof iw4_clipmap->dynEntCount);
		
		for (auto i = 0; i < ARRAYSIZE(iw4_clipmap->dynEntDefList); i++)
		{
			auto count = iw4_clipmap->dynEntCount[i];
			iw4_clipmap->dynEntDefList[i] = local_allocator.allocate_array<iw4::native::DynEntityDef>(count);
			for (auto dyn_index = 0; dyn_index < count; dyn_index++)
			{
				auto iw4_def = &iw4_clipmap->dynEntDefList[i][dyn_index];
				auto native_def = &native_clipmap->dynEntDefList[i][dyn_index];

				iw4_def->pose = native_def->pose;

				if (native_def->xModel)
				{
					iw4_def->xModel = exporter::dump(game::native::ASSET_TYPE_XMODEL, { native_def->xModel }).model;
				}

				iw4_def->brushModel = native_def->brushModel;
				iw4_def->physicsBrushModel = native_def->physicsBrushModel;

				if (native_def->destroyFx)
				{
					iw4_def->destroyFx = exporter::dump(game::native::ASSET_TYPE_FX, { native_def->destroyFx }).fx;
				}

				if (native_def->physPreset)
				{
					iw4_def->physPreset = exporter::dump(game::native::ASSET_TYPE_PHYSPRESET, { native_def->physPreset }).physPreset;
				}

				iw4_def->health = native_def->health;
				iw4_def->mass = native_def->mass;
				iw4_def->contents = native_def->contents;

				if (native_def->type == game::native::DynEntityType::DYNENT_TYPE_HINGE)
				{
					iw4_def->type = iw4::native::DynEntityType::DYNENT_TYPE_INVALID; // unsupported
					console::warn("Found non-supported hinge dynamic entity that we cannot convert (entity %i-%i)\n", i, dyn_index);
					assert(false);
				}
				else
				{
					iw4_def->type = static_cast<iw4::native::DynEntityType>(native_def->type);
					assert(iw4_def->type > 0);
					assert(iw4_def->type < iw4::native::DynEntityType::DYNENT_TYPE_COUNT);
				}
			}

		}
		
		// Useless
		memcpy(iw4_clipmap->dynEntPoseList, native_clipmap->dynEntPoseList, sizeof iw4_clipmap->dynEntPoseList);

		// Useless too!
		for (auto i = 0; i < ARRAYSIZE(iw4_clipmap->dynEntDefList); i++)
		{
			iw4_clipmap->dynEntClientList[i] = local_allocator.allocate_array<iw4::native::DynEntityClient>();
			// and that's it? we leave null apparently ? cause this is runtime data?
		}

		// Useless
		memcpy(iw4_clipmap->dynEntCollList, native_clipmap->dynEntCollList, sizeof iw4_clipmap->dynEntCollList);
		iw4_clipmap->checksum = 0xC0D80000;

		iw4_clipmap->mapEnts = exporter::dump(game::native::ASSET_TYPE_MAP_ENTS, { native_clipmap->mapEnts }).mapEnts;

		out.clipMap = iw4_clipmap;
	}

	void iclipmap::write(const iw4::native::XAssetHeader& header)
	{
		assert(header.clipMap);

		const auto clipMap = header.clipMap;

		if (!clipMap) return;

		std::unordered_map<void*, int> indexMap;

		auto addToMap = [&indexMap](void* ptr, int index)
		{
			if (ptr == nullptr)
			{
				printf("");
			}

			if (indexMap.count(ptr))
			{
				console::error("Error: trying to remap pointer to different index!\n");
				return;
			}

			indexMap[ptr] = index;
		};

		auto getFromMap = [&indexMap](void* ptr)
		{
			if (indexMap.count(ptr))
			{
				return indexMap[ptr];
			}

			console::error("Error: missing expected pointer in map\n");

			if (IsDebuggerPresent())
			{
				__debugbreak();
			}

			return -1;
		};



		utils::memory::allocator allocator;

		utils::stream buffer;
		buffer.saveArray("IW4xClip", 8);
		buffer.saveObject(IW4X_CLIPMAP_VERSION);
		buffer.saveString(clipMap->name);

		unsigned int numStaticModels = clipMap->numStaticModels;

		buffer.saveObject(clipMap->planeCount);
		buffer.saveObject(numStaticModels);
		buffer.saveObject(clipMap->numMaterials);
		buffer.saveObject(clipMap->numBrushSides);
		buffer.saveObject(clipMap->numBrushEdges);
		buffer.saveObject(clipMap->numNodes);
		buffer.saveObject(clipMap->numLeafs);
		buffer.saveObject(clipMap->leafbrushNodesCount);
		buffer.saveObject(clipMap->numLeafBrushes);
		buffer.saveObject(clipMap->numLeafSurfaces);
		buffer.saveObject(clipMap->vertCount);
		buffer.saveObject(clipMap->triCount);
		buffer.saveObject(clipMap->borderCount);
		buffer.saveObject(clipMap->partitionCount);
		buffer.saveObject(clipMap->aabbTreeCount);
		buffer.saveObject(clipMap->numSubModels);
		buffer.saveObject(clipMap->numBrushes);
		buffer.saveObject(clipMap->dynEntCount[0]);
		buffer.saveObject(clipMap->dynEntCount[1]);

		if (clipMap->planes)
		{
			buffer.saveObject<void*>(clipMap->planes);

			for (int i = 0; i < clipMap->planeCount; ++i)
			{
				addToMap(&clipMap->planes[i], i);
				buffer.saveObject(clipMap->planes[i]);
			}
		}

		if (clipMap->staticModelList)
		{
			for (unsigned short i = 0; i < clipMap->numStaticModels; ++i)
			{
				if (clipMap->staticModelList[i].xmodel)
				{
					buffer.saveString(clipMap->staticModelList[i].xmodel->name);
				}
				else
				{
					buffer.saveString("NONE");
				}

				buffer.saveArray(clipMap->staticModelList[i].origin, 3);
				buffer.saveArray(clipMap->staticModelList[i].invScaledAxis, 3); // fuck how this gets counted by c++
				buffer.saveObject(clipMap->staticModelList[i].absBounds);
			}
		}

		if (clipMap->materials)
		{
			for (unsigned int i = 0; i < clipMap->numMaterials; i++)
			{
				// Save manually because i think our struct is wrong (or is it right, and iw3x is wrong?)
				char name[64]{};
				memcpy(name, clipMap->materials[i].name, strnlen(clipMap->materials[i].name, ARRAYSIZE(name)));

				buffer.save(name, sizeof(name));
				buffer.saveObject(clipMap->materials[i].surfaceFlags);
				buffer.saveObject(clipMap->materials[i].contents);
			}
		}

		if (clipMap->brushsides)
		{
			for (unsigned int i = 0; i < clipMap->numBrushSides; ++i)
			{
				addToMap(&clipMap->brushsides[i], i);
				buffer.saveObject(getFromMap(clipMap->brushsides[i].plane));

				// Casts for legacy reasons
				buffer.saveObject(static_cast<int>(clipMap->brushsides[i].materialNum));
				buffer.saveObject(static_cast<short>(clipMap->brushsides[i].firstAdjacentSideOffset));

				buffer.saveObject(clipMap->brushsides[i].edgeCount);
			}
		}

		if (clipMap->brushEdges)
		{
			for (unsigned int i = 0; i < clipMap->numBrushEdges; ++i)
			{
				addToMap(&clipMap->brushEdges[i], i);
			}

			buffer.saveArray(clipMap->brushEdges, clipMap->numBrushEdges);
		}

		if (clipMap->nodes)
		{
			for (unsigned int i = 0; i < clipMap->numNodes; ++i)
			{
				buffer.saveObject(getFromMap(clipMap->nodes[i].plane));
				buffer.saveObject(clipMap->nodes[i].children[0]);
				buffer.saveObject(clipMap->nodes[i].children[1]);
			}
		}

		if (clipMap->leafs)
		{
			for (unsigned int i = 0; i < clipMap->numLeafs; ++i)
			{
				buffer.saveObject(clipMap->leafs[i]);
			}
		}

		if (clipMap->leafbrushNodes)
		{
			for (unsigned int i = 0; i < clipMap->leafbrushNodesCount; ++i)
			{
				buffer.saveObject(clipMap->leafbrushNodes[i]);
				if (clipMap->leafbrushNodes[i].leafBrushCount > 0)
				{
					buffer.saveArray(clipMap->leafbrushNodes[i].data.leaf.brushes, clipMap->leafbrushNodes[i].leafBrushCount);
				}
			}
		}

		if (clipMap->leafbrushes)
		{
			buffer.saveArray(clipMap->leafbrushes, clipMap->numLeafBrushes);
		}

		if (clipMap->leafsurfaces)
		{
			buffer.saveArray(clipMap->leafsurfaces, clipMap->numLeafSurfaces);
		}

		if (clipMap->verts)
		{
			buffer.save(clipMap->verts, 12, clipMap->vertCount);
		}

		if (clipMap->triIndices)
		{
			buffer.save(clipMap->triIndices, 6, clipMap->triCount);
		}

		if (clipMap->triEdgeIsWalkable)
		{
			// TODO: verify if correct?
			buffer.saveArray(clipMap->triEdgeIsWalkable, 4 * ((3 * clipMap->triCount + 31) >> 5));
		}

		if (clipMap->borders)
		{
			for (int i = 0; i < clipMap->borderCount; ++i)
			{
				addToMap(&clipMap->borders[i], i);
				buffer.saveObject(clipMap->borders[i]);
			}
		}

		if (clipMap->partitions)
		{
			int borderTotal = 0;
			for (int i = 0; i < clipMap->partitionCount; ++i)
			{
				borderTotal += clipMap->partitions[i].borderCount;
				buffer.saveObject(clipMap->partitions[i].triCount);
				buffer.saveObject(clipMap->partitions[i].borderCount);
				buffer.saveObject(clipMap->partitions[i].firstTri);
				if (clipMap->partitions[i].borderCount > 0)
				{
					buffer.saveObject(getFromMap(clipMap->partitions[i].borders));
				}
			}
			if (borderTotal != clipMap->borderCount)
			{
				console::warn("ClipMap: didn't use all borders\n");
			}
		}

		if (clipMap->aabbTrees)
		{
			for (int i = 0; i < clipMap->aabbTreeCount; ++i)
			{
				buffer.saveObject(clipMap->aabbTrees[i]);
			}
		}

		if (clipMap->cmodels)
		{
			for (unsigned int i = 0; i < clipMap->numSubModels; ++i)
			{
				buffer.saveObject(clipMap->cmodels[i]);
			}
		}

		if (clipMap->brushes)
		{
			unsigned int brushSideTotal = 0;
			for (unsigned short i = 0; i < clipMap->numBrushes; ++i)
			{
				// Cast for legacy reasons
				buffer.saveObject(static_cast<unsigned int>(clipMap->brushes[i].numsides));

				// NEW
				buffer.saveObject(clipMap->brushes[i].glassPieceIndex);

				brushSideTotal += clipMap->brushes[i].numsides;
				if (clipMap->brushes[i].numsides > 0)
				{
					auto side = getFromMap(clipMap->brushes[i].sides);
					buffer.saveObject(side);
				}

				if (clipMap->brushes[i].baseAdjacentSide)
				{
					auto adjacent = getFromMap(clipMap->brushes[i].baseAdjacentSide);
					buffer.saveObject(adjacent);
				}
				else
				{
					buffer.saveObject(-1); // No data	
				}

				for (int x = 0; x < 2; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						buffer.saveObject(clipMap->brushes[i].axialMaterialNum[x][y]);
					}
				}

				for (int x = 0; x < 2; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						// Cast for legacy reasons
						buffer.saveObject((short)clipMap->brushes[i].firstAdjacentSideOffsets[x][y]);
					}
				}

				for (int x = 0; x < 2; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						buffer.saveObject(clipMap->brushes[i].edgeCount[x][y]);
					}
				}
			}

			if (brushSideTotal != clipMap->numBrushSides)
			{
				console::error("didn't use all brush sides\n");
			}

			// brushbounds
			for (unsigned short i = 0; i < clipMap->numBrushes; ++i)
			{
				buffer.saveObject(clipMap->brushBounds[i]);
			}

			// brushcontents
			for (int i = 0; i < clipMap->numBrushes; ++i)
			{
				buffer.saveObject(clipMap->brushContents[i]);
			}
		}

		for (int n = 0; n < 2; ++n)
		{
			if (clipMap->dynEntDefList[n])
			{
				for (int i = 0; i < clipMap->dynEntCount[n]; ++i)
				{
					buffer.saveObject(clipMap->dynEntDefList[n][i].type);
					buffer.saveObject(clipMap->dynEntDefList[n][i].pose);
					if (clipMap->dynEntDefList[n][i].xModel)
					{
						buffer.saveString(clipMap->dynEntDefList[n][i].xModel->name);
					}
					else
					{
						buffer.saveString("NONE");
					}

					buffer.saveObject(clipMap->dynEntDefList[n][i].brushModel);
					buffer.saveObject(clipMap->dynEntDefList[n][i].physicsBrushModel);

					if (clipMap->dynEntDefList[n][i].destroyFx)
					{
						buffer.saveString(clipMap->dynEntDefList[n][i].destroyFx->name);
					}
					else
					{
						buffer.saveString("NONE");
					}

					if (clipMap->dynEntDefList[n][i].physPreset)
					{
						buffer.saveString(clipMap->dynEntDefList[n][i].physPreset->name);
					}
					else
					{
						buffer.saveString("NONE");
					}

					buffer.saveObject(clipMap->dynEntDefList[n][i].health);
					buffer.saveObject(clipMap->dynEntDefList[n][i].mass);
					buffer.saveObject(clipMap->dynEntDefList[n][i].contents);
				}
			}
		}

		game::native::SModelAabbNode* nodes = clipMap->smodelNodes;

		buffer.saveObject(clipMap->smodelNodeCount);
		buffer.saveArray<game::native::SModelAabbNode>(nodes, clipMap->smodelNodeCount);

		// V3
		// We save mapents
		if (clipMap->mapEnts)
		{
			const auto trigger = &clipMap->mapEnts->trigger;
			buffer.save(trigger->count);
			for (size_t i = 0; i < trigger->count; i++)
			{
				buffer.saveObject(trigger->models[i]);
				static_assert(sizeof game::native::TriggerModel == 8);
			}

			buffer.save(trigger->hullCount);
			for (size_t i = 0; i < trigger->hullCount; i++)
			{
				buffer.saveObject(trigger->hulls[i]);
				static_assert(sizeof game::native::TriggerHull == 32);
			}

			buffer.save(trigger->slabCount);
			for (size_t i = 0; i < trigger->slabCount; i++)
			{
				buffer.saveObject(trigger->slabs[i]);
				static_assert(sizeof game::native::TriggerSlab == 20);
			}
		}

		buffer.saveObject(clipMap->checksum);

		std::string basename(clipMap->name);

		basename.erase(0, 8);
		basename.erase(basename.end() - 7, basename.end());

		utils::io::write_file(std::format("{}/clipmap/{}.iw4xClipMap", get_export_path(), basename), buffer.toBuffer());
	}

	iclipmap::iclipmap()
	{
		command::add("dumpClipMap", [&](const command::params& params)
			{

				game::native::XAssetHeader out{};

				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_CLIPMAP, [](game::native::XAssetHeader header, void* data) {
					auto out = reinterpret_cast<game::native::XAssetHeader*>(data);
					*out = header;

					}, &out, false);

				if (out.data)
				{
					dump(out);
					exporter::add_to_source(game::native::ASSET_TYPE_CLIPMAP, out.clipMap->name);
				}
			});
	}
}