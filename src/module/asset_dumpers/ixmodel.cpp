#include <std_include.hpp>

#include "ixmodel.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module\exporter.hpp>

#define IW4X_MODEL_VERSION 9

namespace asset_dumpers
{
	iw4::native::XSurface ixmodel::convert(const game::native::XSurface* source)
	{
		iw4::native::XSurface target{};

		target.tileMode = source->tileMode;
		target.deformed = (source->flags & 0x40) == 0x40;
		target.vertCount = source->vertCount;
		target.triCount = source->triCount;
		target.zoneHandle = source->zoneHandle;
		target.baseTriIndex = source->baseTriIndex;
		target.baseVertIndex = source->baseVertIndex;
		target.triIndices = source->triIndices;
		target.vertInfo = source->vertInfo;

		/// NOT sure about this
		target.verts0 = reinterpret_cast<iw4::native::GfxPackedVertex*>(source->verts0.verts0);
		static_assert(sizeof target.verts0 == sizeof source->verts0);

		target.vertListCount = source->vertListCount;
		target.vertList = source->vertList;

		static_assert(sizeof target.partBits == sizeof source->partBits);
		memcpy(target.partBits, source->partBits, sizeof source->partBits);

		return target;
	}

	void ixmodel::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.model);

		auto iw4_model = local_allocator.allocate<iw4::native::XModel>();
		auto native_model = header.model;

#define SET_LIGHTGRID_MEMBER(member_name) iw4_model->##member_name## = (native_model->##member_name##)

		SET_LIGHTGRID_MEMBER(name);
		SET_LIGHTGRID_MEMBER(numBones);
		SET_LIGHTGRID_MEMBER(numRootBones);
		SET_LIGHTGRID_MEMBER(numsurfs);

		iw4_model->lodRampType = 0x0; // XMODEL_LOD_RAMP_RIGID

		SET_LIGHTGRID_MEMBER(scale);

		static_assert(sizeof iw4_model->noScalePartBits == sizeof native_model->noScalePartBits);
		memcpy(iw4_model->noScalePartBits, native_model->noScalePartBits, sizeof native_model->noScalePartBits);

		SET_LIGHTGRID_MEMBER(boneNames);
		SET_LIGHTGRID_MEMBER(parentList);
		SET_LIGHTGRID_MEMBER(quats);
		SET_LIGHTGRID_MEMBER(trans);
		SET_LIGHTGRID_MEMBER(partClassification);
		SET_LIGHTGRID_MEMBER(baseMat);

		const auto num_surfs = native_model->numsurfs;

		iw4_model->materialHandles = local_allocator.allocate_array<iw4::native::Material*>(num_surfs);
		for (auto i = 0; i < num_surfs; i++)
		{
			iw4_model->materialHandles[i] = exporter::dump(game::native::XAssetType::ASSET_TYPE_MATERIAL, { native_model->materialHandles[i] }).material;
		}

		static_assert(sizeof iw4_model->lodInfo == sizeof native_model->lodInfo);
		memcpy(iw4_model->lodInfo, native_model->lodInfo, sizeof iw4_model->lodInfo);

		for (auto i = 0; i < native_model->numLods; i++)
		{
			auto iw4_lod = &iw4_model->lodInfo[i];
			auto native_lod = &native_model->lodInfo[i];

			iw4_lod->modelSurfs = local_allocator.allocate<iw4::native::XModelSurfs>();
			iw4_lod->modelSurfs->name = native_lod->modelSurfs->name;

			iw4_lod->modelSurfs->numSurfaces = native_lod->modelSurfs->numsurfs;

			assert(iw4_lod->modelSurfs->numSurfaces == iw4_lod->numsurfs);

			static_assert(sizeof iw4_lod->modelSurfs->partBits == sizeof native_lod->modelSurfs->partBits);
			memcpy(iw4_lod->modelSurfs->partBits, native_lod->modelSurfs->partBits, sizeof native_lod->modelSurfs->partBits);

			static_assert(sizeof iw4_lod->partBits == sizeof native_lod->partBits);
			memcpy(iw4_lod->partBits, native_lod->partBits, sizeof native_lod->partBits);

			iw4_lod->surfs = local_allocator.allocate_array<iw4::native::XSurface>(iw4_lod->numsurfs);
			iw4_lod->modelSurfs->surfaces = iw4_lod->surfs;

			for (unsigned short j = 0; j < iw4_lod->numsurfs; ++j)
			{
				auto target = &iw4_lod->surfs[j];
				*target = convert(&native_lod->surfs[j]);
				
			}

			// Should we keep these? TODO try
			iw4_lod->smcBaseIndexPlusOne = 0;
			iw4_lod->smcSubIndexMask = 0;
			iw4_lod->smcBucket = 0;
		}

		SET_LIGHTGRID_MEMBER(maxLoadedLod);
		SET_LIGHTGRID_MEMBER(numLods);
		SET_LIGHTGRID_MEMBER(collLod);
		SET_LIGHTGRID_MEMBER(flags);
		SET_LIGHTGRID_MEMBER(collSurfs);
		SET_LIGHTGRID_MEMBER(numCollSurfs);
		SET_LIGHTGRID_MEMBER(contents);
		SET_LIGHTGRID_MEMBER(boneInfo);
		SET_LIGHTGRID_MEMBER(radius);
		SET_LIGHTGRID_MEMBER(bounds);
		SET_LIGHTGRID_MEMBER(memUsage);
		
		if (native_model->quantization != 1.F)
		{
			// 🤔
			//console::warn("Model %s has quantization info and might not be backported correctly! (%f)\n", native_model->name, native_model->quantization);
		}

		iw4_model->bad = false; // good boy!

		if (native_model->physPreset)
		{
			iw4_model->physPreset = exporter::dump(game::native::XAssetType::ASSET_TYPE_PHYSPRESET, { native_model->physPreset }).physPreset;
		}

		SET_LIGHTGRID_MEMBER(physCollmap);

		out.model = iw4_model;
	}

	void ixmodel::write(const iw4::native::XAssetHeader& header)
	{
		auto asset = header.model;
		assert(asset);

		utils::stream buffer{};

		buffer.saveArray("IW4xModl", 8);
		buffer.saveObject(IW4X_MODEL_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->boneNames)
		{
			for (char i = 0; i < asset->numBones; ++i)
			{
				buffer.saveString(game::native::SL_ConvertToString(asset->boneNames[i]));
			}
		}

		if (asset->parentList)
		{
			buffer.saveArrayIfNotExisting(asset->parentList, asset->numBones - asset->numRootBones);
		}

		if (asset->quats)
		{
			buffer.saveArrayIfNotExisting(asset->quats, (asset->numBones - asset->numRootBones));
		}

		if (asset->trans)
		{
			buffer.saveArrayIfNotExisting(asset->trans, (asset->numBones - asset->numRootBones));
		}

		if (asset->partClassification)
		{
			buffer.saveArrayIfNotExisting(asset->partClassification, asset->numBones);
		}

		if (asset->baseMat)
		{
			buffer.saveArrayIfNotExisting(asset->baseMat, asset->numBones);
		}

		if (asset->materialHandles)
		{
			buffer.saveArray(asset->materialHandles, asset->numsurfs);

			for (unsigned char i = 0; i < asset->numsurfs; ++i)
			{
				if (asset->materialHandles[i])
				{
					buffer.saveString(asset->materialHandles[i]->info.name);
				}
			}
		}

		// Save_XModelLodInfoArray
		{
			for (int i = 0; i < 4; ++i)
			{
				if (asset->lodInfo[i].modelSurfs)
				{
					ixmodel::write(asset->lodInfo[i].modelSurfs, &buffer);
				}
			}
		}

		// Save_XModelCollSurfArray
		if (asset->collSurfs)
		{
			buffer.saveArray(asset->collSurfs, asset->numCollSurfs);

			for (int i = 0; i < asset->numCollSurfs; ++i)
			{
				game::native::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

				if (collSurf->collTris)
				{
					buffer.saveArray(collSurf->collTris, collSurf->numCollTris);
				}
			}
		}

		if (asset->boneInfo)
		{
			buffer.saveArray(asset->boneInfo, asset->numBones);
		}

		if (asset->physPreset)
		{
			buffer.saveObject(*asset->physPreset);

			if (asset->physPreset->name)
			{
				buffer.saveString(asset->physPreset->name);
			}

			if (asset->physPreset->sndAliasPrefix)
			{
				buffer.saveString(asset->physPreset->sndAliasPrefix);
			}
		}

		if (asset->physCollmap)
		{
			auto collmap = asset->physCollmap;
			buffer.saveObject(*collmap);

			if (collmap->name)
			{
				buffer.saveString(collmap->name);
			}

			if (collmap->geoms)
			{
				buffer.saveArray(collmap->geoms, collmap->count);

				for (unsigned int i = 0; i < collmap->count; ++i)
				{
					auto geom = &collmap->geoms[i];

					if (geom->brushWrapper)
					{
						game::native::BrushWrapper* brush = geom->brushWrapper;
						buffer.saveArrayIfNotExisting(brush, 1);
						{
							if (brush->brush.sides)
							{
								buffer.saveArrayIfNotExisting(brush->brush.sides, brush->brush.numsides);

								// Save_cbrushside_tArray
								for (unsigned short j = 0; j < brush->brush.numsides; ++j)
								{
									game::native::cbrushside_t* side = &brush->brush.sides[j];

									// TODO: Add pointer support
									if (side->plane)
									{
										buffer.saveArrayIfNotExisting(side->plane, 1);
									}
								}
							}

							if (brush->brush.baseAdjacentSide)
							{
								buffer.saveArrayIfNotExisting(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
							}
						}

						// TODO: Add pointer support
						if (brush->planes)
						{
							buffer.saveArrayIfNotExisting(brush->planes, brush->brush.numsides);
						}
					}
				}
			}
		}

		utils::io::write_file(std::format("{}/xmodel/{}.iw4xModel", get_export_path(), asset->name), buffer.toBuffer());
	}


	void ixmodel::write(const game::native::XSurfaceCollisionTree* entry, utils::stream* buffer)
	{
		buffer->saveObject(*entry);

		if (entry->nodes)
		{
			buffer->saveArrayIfNotExisting(entry->nodes, entry->nodeCount);
		}

		if (entry->leafs)
		{
			buffer->saveArrayIfNotExisting(entry->leafs, entry->leafCount);
		}
	}

	void ixmodel::write(const iw4::native::XSurface* surf, utils::stream* buffer)
	{
		if (surf->vertInfo.vertsBlend)
		{
			buffer->saveArrayIfNotExisting(surf->vertInfo.vertsBlend, surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
		}

		// Access vertex block
		if (surf->verts0)
		{
			buffer->saveArrayIfNotExisting(surf->verts0, surf->vertCount);
		}

		// Save_XRigidVertListArray
		if (surf->vertList)
		{
			buffer->saveArrayIfNotExisting(surf->vertList, surf->vertListCount);

			for (unsigned int i = 0; i < surf->vertListCount; ++i)
			{
				game::native::XRigidVertList* rigidVertList = &surf->vertList[i];

				if (rigidVertList->collisionTree)
				{
					ixmodel::write(rigidVertList->collisionTree, buffer);
				}
			}
		}

		// Access index block
		if (surf->triIndices)
		{
			buffer->saveArrayIfNotExisting(surf->triIndices, surf->triCount * 3);
		}
	}

	void ixmodel::write(const iw4::native::XModelSurfs* asset, utils::stream* buffer)
	{
		buffer->saveObject(*asset);

		if (asset->name)
		{
			buffer->saveString(asset->name);
		}

		if (asset->surfaces)
		{
			buffer->saveArrayIfNotExisting(asset->surfaces, asset->numSurfaces);

			for (int i = 0; i < asset->numSurfaces; ++i)
			{
				ixmodel::write(&asset->surfaces[i], buffer);
			}
		}
	}

	ixmodel::ixmodel()
	{
		command::add("dumpXModel", [&](const command::params& params)
		{
			if (params.size() < 2) return;

			auto name = params[1];

			if (name == "*"s)
			{
				static std::mutex m{};
				game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_XMODEL, [](game::native::XAssetHeader header, void* data) {
					m.lock();
					auto asset_dmper = reinterpret_cast<asset_dumper*>(data);

					asset_dmper->dump(header, true);
					m.unlock();

					}, this, false);
			}
			else
			{
				auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_XMODEL, name, false);

				if (header.data)
				{
					if (!is_already_dumped(header))
					{
						exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_XMODEL, name);
					}

					dump(header);
				}
				else
				{
					console::info("could not dump material %s from the database (cannot find it)\n", name);
				}
			}
		});
	}
}