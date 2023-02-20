#include <std_include.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "api.hpp"

#include "iclipmap.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"
#include <module/console.hpp>

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"
namespace asset_dumpers
{
	static_assert(sizeof game::native::ClipMaterial == 12);
	static_assert(sizeof game::native::cbrushside_t == 8);
	static_assert(sizeof game::native::cNode_t == 8);
	static_assert(sizeof game::native::cLeaf_t == 40);
	static_assert(sizeof game::native::cLeafBrushNode_s == 20);
	static_assert(sizeof game::native::CollisionBorder == 28);
	static_assert(sizeof game::native::CollisionPartition == 12);
	static_assert(sizeof game::native::CollisionAabbTree == 32);
	static_assert(sizeof iw4::native::cmodel_t == 68);
	static_assert(sizeof game::native::SModelAabbNode == 28);


	void iclipmap::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.clipMap);

		//static_assert(sizeof iw4::native::clipMap_t == 256);

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

			iw4_csmodel->xmodel = exporter::convert(game::native::ASSET_TYPE_XMODEL, { native_csmodel->xmodel }).model;
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

		iw4_clipmap->numLeafBrushes = native_clipmap->info.numLeafBrushes;
		iw4_clipmap->leafbrushes = native_clipmap->info.leafbrushes;

		// Leaf surfaces don't exist in iw5... leaving at 0 for now ?
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

		iw4_clipmap->smodelNodeCount = native_clipmap->smodelNodeCount;
		iw4_clipmap->smodelNodes = native_clipmap->smodelNodes;


		memcpy(iw4_clipmap->dynEntCount, native_clipmap->dynEntCount, sizeof iw4_clipmap->dynEntCount);
		
		for (auto i = 0; i < ARRAYSIZE(iw4_clipmap->dynEntDefList); i++)
		{
			auto count = iw4_clipmap->dynEntCount[i];

			if (count > 0)
			{
				iw4_clipmap->dynEntDefList[i] = local_allocator.allocate_array<iw4::native::DynEntityDef>(count);
				for (auto dyn_index = 0; dyn_index < count; dyn_index++)
				{
					auto iw4_def = &iw4_clipmap->dynEntDefList[i][dyn_index];
					auto native_def = &native_clipmap->dynEntDefList[i][dyn_index];

					iw4_def->pose = native_def->pose;

					if (native_def->xModel)
					{
						iw4_def->xModel = exporter::convert(game::native::ASSET_TYPE_XMODEL, { native_def->xModel }).model;
					}

					iw4_def->brushModel = native_def->brushModel;
					iw4_def->physicsBrushModel = native_def->physicsBrushModel;

					if (native_def->destroyFx)
					{
						iw4_def->destroyFx = exporter::convert(game::native::ASSET_TYPE_FX, { native_def->destroyFx }).fx;
					}

					if (native_def->physPreset)
					{
						iw4_def->physPreset = exporter::convert(game::native::ASSET_TYPE_PHYSPRESET, { native_def->physPreset }).physPreset;
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
		}
		
		// Useless
		memcpy(iw4_clipmap->dynEntPoseList, native_clipmap->dynEntPoseList, sizeof iw4_clipmap->dynEntPoseList);

		// Useless too!
		for (auto i = 0; i < ARRAYSIZE(iw4_clipmap->dynEntDefList); i++)
		{
			if (iw4_clipmap->dynEntCount[i])
			{
				iw4_clipmap->dynEntClientList[i] = local_allocator.allocate_array<iw4::native::DynEntityClient>(iw4_clipmap->dynEntCount[i]);
				iw4_clipmap->dynEntCollList[i] = local_allocator.allocate_array<game::native::DynEntityColl>(iw4_clipmap->dynEntCount[i]);
				iw4_clipmap->dynEntPoseList[i] = local_allocator.allocate_array<game::native::DynEntityPose>(iw4_clipmap->dynEntCount[i]);
			}
		}

		// Useless
		memcpy(iw4_clipmap->dynEntCollList, native_clipmap->dynEntCollList, sizeof iw4_clipmap->dynEntCollList);
		iw4_clipmap->checksum = 0xC0D80000;

		iw4_clipmap->mapEnts = exporter::convert(game::native::ASSET_TYPE_MAP_ENTS, { native_clipmap->mapEnts }).mapEnts;
		iw4_clipmap->mapEnts->stageCount = native_clipmap->stageCount;
		iw4_clipmap->mapEnts->stages = native_clipmap->stages;

		out.clipMap = iw4_clipmap;
	}

	void iclipmap::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_CLIPMAP_MP, header.data);
		assert(result);
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
					convert_and_write(out);
					exporter::add_to_source(game::native::ASSET_TYPE_CLIPMAP, out.clipMap->name);
				}
			});
	}
}