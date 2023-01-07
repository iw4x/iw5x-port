#include <std_include.hpp>

#include "ixmodel.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module\exporter.hpp>

#define IW4X_MODEL_VERSION 8

namespace asset_dumpers
{
	iw4::native::XSurface ixmodel::convert(const game::native::XSurface* source)
	{
		iw4::native::XSurface target{};

		target.tileMode = source->tileMode;
		target.deformed = false;
		target.vertCount = source->vertCount;
		target.triCount = source->triCount;
		target.zoneHandle = source->zoneHandle;
		target.baseTriIndex = source->baseTriIndex;
		target.baseVertIndex = source->baseVertIndex;
		target.triIndices = source->triIndices;
		target.vertInfo = source->vertInfo;

		target.verts0 = local_allocator.allocate_array<iw4::native::GfxPackedVertex>(target.vertCount);
		static_assert(sizeof target.verts0 == sizeof source->verts0);
		memcpy(target.verts0, source->verts0.verts0, target.vertCount); // 🤞

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

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_model->##member_name## = (native_model->##member_name##)

		SET_MEMBER_NO_CONVERSION(name);
		SET_MEMBER_NO_CONVERSION(numBones);
		SET_MEMBER_NO_CONVERSION(numRootBones);
		SET_MEMBER_NO_CONVERSION(numsurfs);

		iw4_model->lodRampType = 0x0; // XMODEL_LOD_RAMP_RIGID

		SET_MEMBER_NO_CONVERSION(scale);

		static_assert(sizeof iw4_model->noScalePartBits == sizeof native_model->noScalePartBits);
		memcpy(iw4_model->noScalePartBits, native_model->noScalePartBits, sizeof native_model->noScalePartBits);

		SET_MEMBER_NO_CONVERSION(boneNames);
		SET_MEMBER_NO_CONVERSION(parentList);
		SET_MEMBER_NO_CONVERSION(quats);
		SET_MEMBER_NO_CONVERSION(trans);
		SET_MEMBER_NO_CONVERSION(partClassification);
		SET_MEMBER_NO_CONVERSION(baseMat);

		const auto num_bones = iw4_model->numBones;
		const auto num_root_bones = iw4_model->numRootBones;
		const auto num_quats = (num_bones - num_root_bones) * 4;
		const auto num_trans = (num_bones - num_root_bones) * 3;
		const auto num_surfs = native_model->numsurfs;

		iw4_model->materialHandles = local_allocator.allocate_array<iw4::native::Material*>(num_surfs);
		for (auto i = 0; i < num_surfs; i++)
		{
			iw4_model->materialHandles[i] = exporter::dump(game::native::XAssetType::ASSET_TYPE_MATERIAL, { native_model->materialHandles[i] }).material;
		}

		static_assert(sizeof iw4::native::XModelLodInfo == sizeof game::native::XModelLodInfo);
		memcpy(iw4_model->lodInfo, native_model->lodInfo, sizeof game::native::XModelLodInfo);

		for (auto i = 0; i < native_model->numLods; i++)
		{
			auto iw4_lod = &iw4_model->lodInfo[i];
			auto native_lod = &native_model->lodInfo[i];

			iw4_lod->modelSurfs = local_allocator.allocate<iw4::native::XModelSurfs>();
			iw4_lod->modelSurfs->name = native_lod->modelSurfs->name;

			iw4_lod->modelSurfs->surfaces = local_allocator.allocate_array<iw4::native::XSurface>(native_lod->modelSurfs->numsurfs);
			*iw4_lod->modelSurfs->surfaces = convert(native_lod->modelSurfs->surfs);
			iw4_lod->modelSurfs->numSurfaces = native_lod->modelSurfs->numsurfs;

			// "weapon_m320_gl" does not look okay... not okay at all
			//assert(iw4_lod->modelSurfs->numSurfaces == iw4_lod->numsurfs);

			static_assert(sizeof iw4_lod->modelSurfs->partBits == sizeof native_lod->modelSurfs->partBits);
			memcpy(iw4_lod->modelSurfs->partBits, native_lod->modelSurfs->partBits, sizeof native_lod->modelSurfs->partBits);

			static_assert(sizeof iw4_lod->partBits == sizeof native_lod->partBits);
			memcpy(iw4_lod->partBits, native_lod->partBits, sizeof native_lod->partBits);

			iw4_lod->surfs = local_allocator.allocate_array<iw4::native::XSurface>(iw4_lod->numsurfs);

			for (unsigned short j = 0; j < iw4_lod->numsurfs; ++j)
			{
				auto target = &iw4_lod->surfs[j];
				*target = convert(&native_lod->surfs[j]);
				
				if (i != native_model->collLod)
				{
					for (size_t k = 0; k < target->vertListCount; k++)
					{
						target->vertList[k].collisionTree = nullptr; // Only collod is used
					}
				}
			}

			// Should we keep these? TODO try
			iw4_lod->smcBaseIndexPlusOne = 0;
			iw4_lod->smcSubIndexMask = 0;
			iw4_lod->smcBucket = 0;
		}

		SET_MEMBER_NO_CONVERSION(maxLoadedLod);
		SET_MEMBER_NO_CONVERSION(numLods);
		SET_MEMBER_NO_CONVERSION(collLod);
		SET_MEMBER_NO_CONVERSION(flags);
		SET_MEMBER_NO_CONVERSION(collSurfs);
		SET_MEMBER_NO_CONVERSION(numCollSurfs);
		SET_MEMBER_NO_CONVERSION(contents);
		SET_MEMBER_NO_CONVERSION(boneInfo);
		SET_MEMBER_NO_CONVERSION(radius);
		SET_MEMBER_NO_CONVERSION(bounds);
		SET_MEMBER_NO_CONVERSION(memUsage);
		
		if (native_model->quantization != 0.F)
		{
			// 🤔
			console::warn("Model %s has quantization info and might not be backported correctly! (%f)\n", native_model->name, native_model->quantization);
		}

		iw4_model->bad = false; // good boy!

		if (native_model->physPreset)
		{
			iw4_model->physPreset = exporter::dump(game::native::XAssetType::ASSET_TYPE_PHYSPRESET, { native_model->physPreset }).physPreset;
		}

		SET_MEMBER_NO_CONVERSION(physCollmap);

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
			buffer.saveArrayIfNotExisting(asset->quats, (asset->numBones - asset->numRootBones) * 4);
		}

		if (asset->trans)
		{
			buffer.saveArrayIfNotExisting(asset->trans, (asset->numBones - asset->numRootBones) * 3);
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
						buffer.saveObject(*brush);
						{
							if (brush->brush.sides)
							{
								buffer.saveArray(brush->brush.sides, brush->brush.numsides);

								// Save_cbrushside_tArray
								for (unsigned short j = 0; j < brush->brush.numsides; ++j)
								{
									game::native::cbrushside_t* side = &brush->brush.sides[j];

									// TODO: Add pointer support
									if (side->plane)
									{
										buffer.saveObject(*side->plane);
									}
								}
							}

							if (brush->brush.baseAdjacentSide)
							{
								buffer.saveArray(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
							}
						}

						// TODO: Add pointer support
						if (brush->planes)
						{
							buffer.saveArray(brush->planes, brush->brush.numsides);
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
					dump(header, true);
					console::info("successfullly dumped xmodel %s!\n", name);
				}
				else
				{
					console::info("could not dump material %s from the database (cannot find it)\n", name);
				}
			}
		});
	}
}