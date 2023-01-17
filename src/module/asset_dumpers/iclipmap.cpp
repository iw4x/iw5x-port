#include <std_include.hpp>

#include "iclipmap.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"
#include <module/console.hpp>

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>


namespace asset_dumpers
{

#define IW4X_CLIPMAP_VERSION 3

	static_assert(sizeof iw4::native::clipMap_t == 256);
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
		iw4_clipmap->mapEnts->stageCount = native_clipmap->stageCount;
		iw4_clipmap->mapEnts->stages = native_clipmap->stages;

		out.clipMap = iw4_clipmap;
	}

	void iclipmap::write(const iw4::native::XAssetHeader& header)
	{
		write_binary(header);
		write_plaintext(header);
	}

	void iclipmap::write_plaintext(const iw4::native::XAssetHeader& header)
	{
		assert(header.clipMap);

		const auto clip_map = header.clipMap;

		if (!clip_map) return;

		std::unordered_map<game::native::cplane_s*, int> planes{}; // +++
		std::unordered_map<game::native::cbrushside_t*, int> brush_sides{}; // +
		std::unordered_map<unsigned char*, int> brush_edges{}; // +
		std::unordered_map<unsigned short*, int> leaf_brushes{}; // ++
		std::unordered_map<game::native::CollisionBorder*, int> borders{}; // +

		utils::memory::allocator str_duplicator;
		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		auto float_array_to_json = [&allocator](const float* array, int size)
		{
			rapidjson::Value arr(rapidjson::kArrayType);

			for (size_t i = 0; i < size; i++)
			{
				arr.PushBack(rapidjson::Value(array[i]), allocator);
			}

			return arr;
		};

		auto ushort_to_array = [&allocator](const unsigned short* array, int size)
		{
			rapidjson::Value arr(rapidjson::kArrayType);

			for (size_t i = 0; i < size; i++)
			{
				arr.PushBack(rapidjson::Value(array[i]), allocator);
			}

			return arr;
		};

		auto uchar_to_array = [&allocator](const unsigned char* array, int size)
		{
			rapidjson::Value arr(rapidjson::kArrayType);

			for (size_t i = 0; i < size; i++)
			{
				arr.PushBack(rapidjson::Value(static_cast<uint8_t>(array[i])), allocator);
			}

			return arr;
		};

		auto bounds_to_json = [&allocator, &float_array_to_json](const game::native::Bounds& bounds)
		{
			rapidjson::Value bounds_json(rapidjson::kObjectType);

			bounds_json.AddMember("midPoint", float_array_to_json(bounds.midPoint, 3), allocator);
			bounds_json.AddMember("halfSize", float_array_to_json(bounds.halfSize, 3), allocator);

			return bounds_json;
		};

		auto placement_to_json = [&allocator, &float_array_to_json](const game::native::GfxPlacement& placement)
		{
			rapidjson::Value placement_json(rapidjson::kObjectType);

			placement_json.AddMember("quat", float_array_to_json(placement.quat, 4), allocator);
			placement_json.AddMember("origin", float_array_to_json(placement.origin, 3), allocator);

			return placement_json;
		};

		auto leaf_to_json = [&allocator, &bounds_to_json](const game::native::cLeaf_t& leaf)
		{
			rapidjson::Value json_leave(rapidjson::kObjectType);

			json_leave.AddMember("firstCollAabbIndex", leaf.firstCollAabbIndex, allocator);
			json_leave.AddMember("collAabbCount", leaf.collAabbCount, allocator);
			json_leave.AddMember("brushContents", leaf.brushContents, allocator);
			json_leave.AddMember("terrainContents", leaf.terrainContents, allocator);
			json_leave.AddMember("leafBrushNode", leaf.leafBrushNode, allocator);
			json_leave.AddMember("bounds", bounds_to_json(leaf.bounds), allocator);

			return json_leave;
		};

		output.AddMember("version", IW4X_CLIPMAP_VERSION, allocator);
		output.AddMember("name", RAPIDJSON_STR(clip_map->name), allocator);
		output.AddMember("isInUse", clip_map->isInUse, allocator);

		// Planes
		rapidjson::Value json_planes(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->planeCount; i++)
		{
			rapidjson::Value json_plane(rapidjson::kObjectType);
			auto plane = &clip_map->planes[i];

			json_plane.AddMember(
				"normal",
				float_array_to_json(plane->normal, ARRAYSIZE(plane->normal)), 
				allocator
			);

			json_plane.AddMember("dist", plane->dist, allocator);
			json_plane.AddMember("type", plane->type, allocator);

			json_planes.PushBack(json_plane, allocator);

			planes[plane] = i;
		}

		output.AddMember("planes", json_planes, allocator);

		// Static models
		rapidjson::Value json_static_models(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numStaticModels; i++)
		{
			rapidjson::Value json_static_model(rapidjson::kObjectType);
			auto static_model = &clip_map->staticModelList[i];

			json_static_model.AddMember(
				"xmodel", RAPIDJSON_STR(static_model->xmodel->name), allocator
			);
			json_static_model.AddMember(
				"origin",
				float_array_to_json(static_model->origin, ARRAYSIZE(static_model->origin)),
				allocator
			);

			rapidjson::Value inv_scaled_axis(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(static_model->invScaledAxis); j++)
			{
				inv_scaled_axis.PushBack(
					float_array_to_json(static_model->invScaledAxis[j], ARRAYSIZE(static_model->invScaledAxis[j])),
					allocator
				);
			}

			json_static_model.AddMember(
				"absBounds",
				bounds_to_json(static_model->absBounds),
				allocator
			);

			json_static_models.PushBack(json_static_model, allocator);
		}

		output.AddMember("staticModelList", json_static_models, allocator);

		// Materials
		rapidjson::Value json_materials(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numMaterials; i++)
		{
			rapidjson::Value json_material(rapidjson::kObjectType);
			auto material = &clip_map->materials[i];

			json_material.AddMember(
				"name",
				RAPIDJSON_STR(material->name),
				allocator
			);

			json_material.AddMember("surfaceFlags", material->surfaceFlags, allocator);
			json_material.AddMember("contents", material->contents, allocator);

			json_materials.PushBack(json_material, allocator);
		}

		output.AddMember("materials", json_materials, allocator);

		// Brushsides
		rapidjson::Value json_brush_sides(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numBrushSides; i++)
		{
			rapidjson::Value json_brush_side(rapidjson::kObjectType);
			auto brush_side = &clip_map->brushsides[i];

			auto index = planes[brush_side->plane];
			auto plane_index = std::format("#{}", index);
			json_brush_side.AddMember(
				"plane",
				RAPIDJSON_STR(str_duplicator.duplicate_string(plane_index)),
				allocator
			);

			json_brush_side.AddMember("materialNum", brush_side->materialNum, allocator);
			json_brush_side.AddMember("firstAdjacentSideOffset", brush_side->firstAdjacentSideOffset, allocator);
			json_brush_side.AddMember("edgeCount", brush_side->edgeCount, allocator);

			json_brush_sides.PushBack(json_brush_side, allocator);

			brush_sides[brush_side] = i;
		}

		output.AddMember("brushsides", json_brush_sides, allocator);

		// Brush edges
		rapidjson::Value json_brush_edges(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numBrushEdges; i++)
		{
			json_brush_edges.PushBack(clip_map->brushEdges[i], allocator);
			brush_edges[&clip_map->brushEdges[i]] = i;
		}

		output.AddMember("brushEdges", json_brush_edges, allocator);

		// Brush nodes
		rapidjson::Value json_nodes(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numNodes; i++)
		{
			auto node = &clip_map->nodes[i];

			rapidjson::Value json_node(rapidjson::kObjectType);

			auto index = planes[node->plane];
			auto plane_index = std::format("#{}", index);
			json_node.AddMember("plane", RAPIDJSON_STR(plane_index.c_str()), allocator);

			rapidjson::Value children(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(node->children); j++)
			{
				children.PushBack(node->children[j], allocator);
			}

			json_node.AddMember("children", children, allocator);

			json_nodes.PushBack(json_node, allocator);
		}

		output.AddMember("nodes", json_nodes, allocator);

		// Leaves
		rapidjson::Value json_leaves(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numLeafs; i++)
		{
			auto leaf = &clip_map->leafs[i];

			json_leaves.PushBack(leaf_to_json(*leaf), allocator);
		}

		output.AddMember("leafs", json_leaves, allocator);

		// Brush leafs
		rapidjson::Value json_brush_leafs(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numLeafBrushes; i++)
		{
			json_brush_leafs.PushBack(clip_map->leafbrushes[i], allocator);
		}

		output.AddMember("leafbrushes", json_brush_leafs, allocator);


		// leafsurfaces (unused)
		rapidjson::Value json_leaf_surfaces(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numLeafSurfaces; i++)
		{
			json_leaf_surfaces.PushBack(clip_map->leafsurfaces[i], allocator);
		}

		output.AddMember("leafsurfaces", json_leaf_surfaces, allocator);

		// vertices
		rapidjson::Value json_vertices(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->vertCount; i++)
		{
			json_vertices.PushBack(float_array_to_json(clip_map->verts[i], 3), allocator);
		}

		output.AddMember("verts", json_vertices, allocator);

		// tris
		rapidjson::Value json_tris(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->triCount * 3; i++)
		{
			json_tris.PushBack(ushort_to_array(&clip_map->triIndices[i], 3), allocator);
		}

		output.AddMember("tris", json_tris, allocator);

		rapidjson::Value json_tris_walkable(rapidjson::kArrayType);
		auto walkable_count = 4 * ((3 * clip_map->triCount + 31) >> 5);
		for (size_t i = 0; i < walkable_count * 3; i++)
		{
			json_tris_walkable.PushBack(static_cast<uint8_t>(clip_map->triEdgeIsWalkable[i]), allocator);
		}

		output.AddMember("triEdgeIsWalkable", json_tris_walkable, allocator);

		// Collision borders
		rapidjson::Value json_collision_borders(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->borderCount; i++)
		{
			auto collision_border = &clip_map->borders[i];

			rapidjson::Value json_collision_border(rapidjson::kObjectType);

			json_collision_border.AddMember("distEq", float_array_to_json(collision_border->distEq, 3), allocator);
			json_collision_border.AddMember("zBase", collision_border->zBase, allocator);
			json_collision_border.AddMember("zSlope", collision_border->zSlope, allocator);
			json_collision_border.AddMember("start", collision_border->start, allocator);
			json_collision_border.AddMember("length", collision_border->length, allocator);
			
			json_collision_borders.PushBack(json_collision_border, allocator);

			borders[collision_border] = i;
		}

		output.AddMember("borders", json_collision_borders, allocator);

		// Collision partitions
		rapidjson::Value json_collision_partitions(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->partitionCount; i++)
		{
			auto collision_partition = &clip_map->partitions[i];

			rapidjson::Value json_collision_partition(rapidjson::kObjectType);

			json_collision_partition.AddMember("triCount", collision_partition->triCount, allocator);
			json_collision_partition.AddMember("firstVertSegment", collision_partition->firstVertSegment, allocator);
			json_collision_partition.AddMember("firstTri", collision_partition->firstTri, allocator);

			rapidjson::Value partition_borders(rapidjson::kArrayType);
			for (size_t j = 0; j < collision_partition->borderCount; j++)
			{
				auto index_str = str_duplicator.duplicate_string(std::format("#{}", borders[collision_partition->borders]));
				partition_borders.PushBack(RAPIDJSON_STR(index_str), allocator);
			}

			json_collision_partition.AddMember("borders", partition_borders, allocator);

			json_collision_partitions.PushBack(json_collision_partition, allocator);
		}

		output.AddMember("partitions", json_collision_partitions, allocator);

		// Trees
		rapidjson::Value json_aabbtrees(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->aabbTreeCount; i++)
		{
			auto aabbtree = &clip_map->aabbTrees[i];

			rapidjson::Value json_aabbtree(rapidjson::kObjectType);

			json_aabbtree.AddMember("midPoint", float_array_to_json(aabbtree->midPoint, 3), allocator);
			json_aabbtree.AddMember("halfSize", float_array_to_json(aabbtree->halfSize, 3), allocator);
			json_aabbtree.AddMember("materialIndex", aabbtree->materialIndex, allocator);
			json_aabbtree.AddMember("u", aabbtree->u, allocator);

			json_aabbtrees.PushBack(json_aabbtree, allocator);
		}

		output.AddMember("aabbTrees", json_aabbtrees, allocator);

		// Cmodels
		rapidjson::Value json_cmodels(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numSubModels; i++)
		{
			auto cmodel = &clip_map->cmodels[i];

			rapidjson::Value json_cmodel(rapidjson::kObjectType);

			json_cmodel.AddMember("bounds", bounds_to_json(cmodel->bounds), allocator);
			json_cmodel.AddMember("radius", cmodel->radius, allocator);

			json_cmodel.AddMember("leaf", leaf_to_json(cmodel->leaf), allocator);
			json_cmodel.AddMember("bounds", bounds_to_json(cmodel->bounds), allocator);

			json_cmodels.PushBack(json_cmodel, allocator);
		}

		output.AddMember("cmodels", json_cmodels, allocator);

		// Brushes
		rapidjson::Value json_brushes(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numBrushes; i++)
		{
			auto brush = &clip_map->brushes[i];

			rapidjson::Value json_brush(rapidjson::kObjectType);

			json_brush.AddMember("glassPieceIndex", brush->glassPieceIndex, allocator);

			// Sides
			rapidjson::Value sides(rapidjson::kArrayType);
			for (size_t j = 0; j < brush->numsides; j++)
			{
				auto index_str = str_duplicator.duplicate_string(std::format("#{}", brush_sides[&brush->sides[j]]));
				sides.PushBack(RAPIDJSON_STR(index_str), allocator);
			}

			json_brush.AddMember("sides", sides, allocator);

			if (brush->baseAdjacentSide)
			{
				auto index_str = str_duplicator.duplicate_string(std::format("#{}", brush_edges[brush->baseAdjacentSide]));
				json_brush.AddMember("baseAdjacentSide", RAPIDJSON_STR(index_str), allocator);
			}
			else
			{
				json_brush.AddMember("baseAdjacentSide", rapidjson::Value(rapidjson::kNullType), allocator);
			}

			rapidjson::Value axial_material_num(rapidjson::kArrayType);
			for (size_t x = 0; x < 2; x++)
			{
				axial_material_num.PushBack(ushort_to_array(brush->axialMaterialNum[x], 3), allocator);
			}
			json_brush.AddMember("axialMaterialNum", axial_material_num, allocator);

			rapidjson::Value first_adjacent_side_offsets(rapidjson::kArrayType);
			for (size_t x = 0; x < 2; x++)
			{
				first_adjacent_side_offsets.PushBack(uchar_to_array(brush->firstAdjacentSideOffsets[x], 3), allocator);
			}
			json_brush.AddMember("firstAdjacentSideOffsets", first_adjacent_side_offsets, allocator);
			
			rapidjson::Value edge_count(rapidjson::kArrayType);
			for (size_t x = 0; x < 2; x++)
			{
				edge_count.PushBack(uchar_to_array(brush->edgeCount[x], 3), allocator);
			}
			json_brush.AddMember("edgeCount", edge_count, allocator);

			json_brushes.PushBack(json_brush, allocator);
		}

		output.AddMember("brushes", json_brushes, allocator);

		// Brushbounds
		rapidjson::Value json_brush_bounds(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numBrushes; i++)
		{
			json_brush_bounds.PushBack(bounds_to_json(clip_map->brushBounds[i]), allocator);
		}

		output.AddMember("brushBounds", json_brush_bounds, allocator);

		// Brush contents
		rapidjson::Value json_brush_contents(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->numBrushes; i++)
		{
			json_brush_contents.PushBack(clip_map->brushContents[i], allocator);
		}

		output.AddMember("brushContents", json_brush_contents, allocator);

		// ENTITIES
		if (clip_map->mapEnts)
		{
			static_assert(sizeof game::native::TriggerSlab == 20);
			static_assert(sizeof game::native::TriggerModel == 8);
			static_assert(sizeof game::native::TriggerHull == 32);

			rapidjson::Value json_map_ents(rapidjson::kObjectType);
			const auto ents = clip_map->mapEnts;

			json_map_ents.AddMember("name", RAPIDJSON_STR(ents->name), allocator);

			rapidjson::Value json_trigger(rapidjson::kObjectType);

			rapidjson::Value json_trigger_models(rapidjson::kArrayType);
			for (size_t i = 0; i < ents->trigger.count; i++)
			{
				rapidjson::Value json_trigger_model(rapidjson::kObjectType);
				json_trigger_model.AddMember("contents", ents->trigger.models[i].contents, allocator);
				json_trigger_model.AddMember("hullCount", ents->trigger.models[i].hullCount, allocator);
				json_trigger_model.AddMember("firstHull", ents->trigger.models[i].firstHull, allocator);
				
				json_trigger_models.PushBack(json_trigger_model, allocator);
			}

			json_trigger.AddMember("models", json_trigger_models, allocator);

			rapidjson::Value json_trigger_hulls(rapidjson::kArrayType);
			for (size_t i = 0; i < ents->trigger.hullCount; i++)
			{
				rapidjson::Value json_trigger_hull(rapidjson::kObjectType);
				json_trigger_hull.AddMember("bounds", bounds_to_json(ents->trigger.hulls[i].bounds), allocator);
				json_trigger_hull.AddMember("contents", ents->trigger.hulls[i].contents, allocator);
				json_trigger_hull.AddMember("hullCount", ents->trigger.hulls[i].slabCount, allocator);
				json_trigger_hull.AddMember("firstHull", ents->trigger.hulls[i].firstSlab, allocator);

				json_trigger_hulls.PushBack(json_trigger_hull, allocator);
			}

			json_trigger.AddMember("hulls", json_trigger_hulls, allocator);

			rapidjson::Value json_trigger_slabs(rapidjson::kArrayType);
			for (size_t i = 0; i < ents->trigger.slabCount; i++)
			{
				rapidjson::Value json_trigger_slab(rapidjson::kObjectType);
				json_trigger_slab.AddMember("bounds", float_array_to_json(ents->trigger.slabs[i].dir, 3), allocator);
				json_trigger_slab.AddMember("midPoint", ents->trigger.slabs[i].midPoint, allocator);
				json_trigger_slab.AddMember("halfSize", ents->trigger.slabs[i].halfSize, allocator);

				json_trigger_slabs.PushBack(json_trigger_slab, allocator);
			}

			json_trigger.AddMember("slabs", json_trigger_slabs, allocator);
			json_map_ents.AddMember("trigger", json_trigger, allocator);

			rapidjson::Value json_stages(rapidjson::kArrayType);
			for (size_t i = 0; i < ents->stageCount; i++)
			{
				auto stage = &ents->stages[i];
				rapidjson::Value json_stage(rapidjson::kObjectType);
				json_stage.AddMember("name", RAPIDJSON_STR(stage->name), allocator);
				json_stage.AddMember("origin", float_array_to_json(stage->origin, 3), allocator);
				json_stage.AddMember("triggerIndex", stage->triggerIndex, allocator);
				json_stage.AddMember("sunPrimaryLightIndex", stage->sunPrimaryLightIndex, allocator);

				json_stages.PushBack(json_stage, allocator);
			}

			json_map_ents.AddMember("stages", json_stages, allocator);

			output.AddMember("mapEnts", json_map_ents, allocator);
		}
		else
		{
			output.AddMember("mapEnts", rapidjson::Value(rapidjson::kNullType), allocator);
		}

		//Smodel nodes
		rapidjson::Value json_smodelnodes(rapidjson::kArrayType);
		for (size_t i = 0; i < clip_map->smodelNodeCount; i++)
		{
			auto smodelNode = &clip_map->smodelNodes[i];

			rapidjson::Value json_smodelnode(rapidjson::kObjectType);

			json_smodelnode.AddMember("bounds", bounds_to_json(smodelNode->bounds), allocator);
			json_smodelnode.AddMember("firstChild", smodelNode->firstChild, allocator);
			json_smodelnode.AddMember("childCount", smodelNode->childCount, allocator);

			json_smodelnodes.PushBack(json_smodelnode, allocator);
		}

		output.AddMember("smodelNodes", json_smodelnodes, allocator);

		// Dynent
		rapidjson::Value json_dyn_entities_def(rapidjson::kArrayType);
		for (size_t i = 0; i < ARRAYSIZE(clip_map->dynEntCount); i++)
		{
			auto def_list = clip_map->dynEntDefList[i];
			if (def_list)
			{
				rapidjson::Value json_dyn_entity_def_list(rapidjson::kArrayType);

				for (size_t j = 0; j < clip_map->dynEntCount[i]; j++)
				{
					rapidjson::Value json_dyn_entity_def_pack(rapidjson::kObjectType);
					rapidjson::Value json_dyn_entity_def(rapidjson::kObjectType);
					auto def = &def_list[j];

					json_dyn_entity_def.AddMember("type", def->type, allocator);
					json_dyn_entity_def.AddMember("pose", placement_to_json(def->pose), allocator);
					json_dyn_entity_def.AddMember("xModel", def->xModel ? RAPIDJSON_STR(def->xModel->name) : rapidjson::Value(rapidjson::kNullType), allocator);
					json_dyn_entity_def.AddMember("brushModel", def->brushModel, allocator); 
					json_dyn_entity_def.AddMember("physicsBrushModel", def->physicsBrushModel, allocator);
					json_dyn_entity_def.AddMember("destroyFx", def->destroyFx ? RAPIDJSON_STR(def->destroyFx->name) : rapidjson::Value(rapidjson::kNullType), allocator);
					json_dyn_entity_def.AddMember("physPreset", def->physPreset ? RAPIDJSON_STR(def->physPreset->name) : rapidjson::Value(rapidjson::kNullType), allocator);
					json_dyn_entity_def.AddMember("health", def->health, allocator);

					rapidjson::Value json_mass(rapidjson::kObjectType);
					json_mass.AddMember("centerOfMass", float_array_to_json(def->mass.centerOfMass, 3), allocator);
					json_mass.AddMember("momentsOfInertia", float_array_to_json(def->mass.momentsOfInertia, 3), allocator);
					json_mass.AddMember("productsOfInertia", float_array_to_json(def->mass.productsOfInertia, 3), allocator);
					json_dyn_entity_def.AddMember("mass", json_mass, allocator);

					json_dyn_entity_def.AddMember("contents", def->contents, allocator);

					json_dyn_entity_def_pack.AddMember("dynEntityDef", json_dyn_entity_def, allocator);

					rapidjson::Value json_dyn_entity_pose(rapidjson::kObjectType);
					auto pose = &clip_map->dynEntPoseList[i][j];
					json_dyn_entity_pose.AddMember("pose", placement_to_json(pose->pose), allocator);
					json_dyn_entity_pose.AddMember("radius", pose->radius, allocator);
					json_dyn_entity_def_pack.AddMember("dynEntPose", json_dyn_entity_pose, allocator);

					rapidjson::Value json_dyn_entity_client(rapidjson::kObjectType);
					auto client = &clip_map->dynEntClientList[i][j];
					json_dyn_entity_client.AddMember("physObjId", client->physObjId, allocator);
					json_dyn_entity_client.AddMember("flags", client->flags, allocator);
					json_dyn_entity_client.AddMember("lightingHandle", client->lightingHandle, allocator);
					json_dyn_entity_client.AddMember("health", client->health, allocator);
					json_dyn_entity_def_pack.AddMember("dynEntClient", json_dyn_entity_client, allocator);

					rapidjson::Value json_dyn_entity_coll(rapidjson::kObjectType);
					auto coll = &clip_map->dynEntCollList[i][j];
					json_dyn_entity_coll.AddMember("sector", coll->sector, allocator);
					json_dyn_entity_coll.AddMember("nextEntInSector", coll->nextEntInSector, allocator);
					json_dyn_entity_coll.AddMember("linkMins", float_array_to_json(coll->linkMins, 2), allocator);
					json_dyn_entity_coll.AddMember("linkMaxs", float_array_to_json(coll->linkMaxs, 2), allocator);
					json_dyn_entity_def_pack.AddMember("dynEntColl", json_dyn_entity_coll, allocator);

					json_dyn_entity_def_list.PushBack(json_dyn_entity_def_pack, allocator);
				}

				json_dyn_entities_def.PushBack(json_dyn_entity_def_list, allocator);
			}
			else
			{

				json_dyn_entities_def.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
			}
		}
		output.AddMember("dynEntitiesDef", json_dyn_entities_def, allocator);

		// Checksum
		output.AddMember("checksum", clip_map->checksum, allocator);

		// Write to disk
		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
		output.Accept(writer);

		utils::io::write_file(std::format("{}/clipmap/{}.iw4x.json", get_export_path(), clip_map->name), buff.GetString());

	}

	void iclipmap::write_binary(const iw4::native::XAssetHeader& header)
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
					buffer.saveArrayIfNotExisting(clipMap->leafbrushNodes[i].data.leaf.brushes, clipMap->leafbrushNodes[i].leafBrushCount);
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