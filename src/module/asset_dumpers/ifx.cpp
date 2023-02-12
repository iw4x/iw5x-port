#include <std_include.hpp>

#include "ifx.hpp"
#include "itechniqueset.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"
#include "module/console.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "api.hpp"

#define IW4X_FX_VERSION 2

namespace asset_dumpers
{
	void ifx::write(const iw4::native::FxElemVisuals* visuals, char elemType, utils::stream* buffer)
	{
		switch (elemType)
		{
		case game::native::FX_ELEM_TYPE_MODEL:
			if (visuals->model)
			{
				buffer->saveString(visuals->model->name);
			}

			break;

		case game::native::FX_ELEM_TYPE_OMNI_LIGHT:
		case game::native::FX_ELEM_TYPE_SPOT_LIGHT:
			break;

		case game::native::FX_ELEM_TYPE_SOUND:
			if (visuals->soundName)
			{
				buffer->saveString(visuals->soundName);
			}

			break;

		case game::native::FX_ELEM_TYPE_RUNNER:
			if (visuals->effectDef.handle)
			{
				buffer->saveString(visuals->effectDef.handle->name);
			}

			break;

		default:
			if (visuals->material)
			{
				buffer->saveString(visuals->material->info.name);
			}

			break;
		}
	}

	void ifx::convert(const game::native::FxElemVisuals* visuals, iw4::native::FxElemVisuals* into, char elemType)
	{
		switch (elemType)
		{
		case game::native::FX_ELEM_TYPE_MODEL:
		{
			assert(visuals->model);

			if (visuals->model)
			{
				into->model =
					exporter::dump(game::native::XAssetType::ASSET_TYPE_XMODEL, { visuals->model })
					.model;
			}

			break;
		}

		case game::native::FX_ELEM_TYPE_OMNI_LIGHT:
		case game::native::FX_ELEM_TYPE_SPOT_LIGHT:
			// Unused in COD6
			into->anonymous = (void*)0xDEADC0DE;
			break;

		case game::native::FX_ELEM_TYPE_SOUND:
		{

			if (visuals->soundName)
			{
				auto sound = game::native::DB_FindXAssetHeader(
					game::native::XAssetType::ASSET_TYPE_SOUND, 
					visuals->soundName, 
					1 // Create default asset if missing - ON DLC MAPS THIS HAPPENS!
				);

				if (sound.data)
				{
					into->soundName = visuals->soundName;
					exporter::dump(game::native::XAssetType::ASSET_TYPE_SOUND, { sound });
				}
			}

			break;
		}

		case game::native::FX_ELEM_TYPE_RUNNER:
		{
			assert(visuals->effectDef.handle);

			if (visuals->effectDef.handle)
			{
				into->effectDef.handle
					= exporter::dump(game::native::XAssetType::ASSET_TYPE_FX, { visuals->effectDef.handle })
					.fx;
			}

			break;
		}

		default:
		{
			if (visuals->material)
			{
				// We reallocate it because we're copying it over to the new one and modifying it soo...
				auto backup = visuals->material->info.name;

				auto suffixed_name = std::string(backup) + itechniqueset::techset_suffix;

				// We have to rename it because it gets otherwise shadowed by iw4!
				visuals->material->info.name = local_allocator.duplicate_string(suffixed_name);

				into->material =
					exporter::dump(game::native::XAssetType::ASSET_TYPE_MATERIAL, { visuals->material })
					.material;

				assert(into->material);

				//// Restore original name
				visuals->material->info.name = backup;
			}

			break;
		}
		}
	}

	void ifx::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.fx);

		auto iw4_fx = local_allocator.allocate<iw4::native::FxEffectDef>();
		const auto native_fx = header.fx;

		iw4_fx->name = native_fx->name;
		iw4_fx->flags = native_fx->flags;
		iw4_fx->totalSize = native_fx->totalSize;
		iw4_fx->msecLoopingLife = native_fx->msecLoopingLife;
		iw4_fx->elemDefCountLooping = native_fx->elemDefCountLooping;
		iw4_fx->elemDefCountOneShot = native_fx->elemDefCountOneShot;
		iw4_fx->elemDefCountEmission = native_fx->elemDefCountEmission;

		auto fx_count = iw4_fx->elemDefCountLooping + iw4_fx->elemDefCountOneShot + iw4_fx->elemDefCountEmission;
		iw4_fx->elemDefs = local_allocator.allocate_array<iw4::native::FxElemDef>(fx_count);

		for (auto i = 0; i < fx_count; i++)
		{
			// They're identical, IW5 just has a seed info that iw4 doesn't have
			static_assert(sizeof(game::native::FxElemDef) > sizeof(iw4::native::FxElemDef));
			memcpy(&iw4_fx->elemDefs[i], &native_fx->elemDefs[i], sizeof iw4::native::FxElemDef);

			auto iw4_def = &iw4_fx->elemDefs[i];
			auto native_def = &native_fx->elemDefs[i];

			iw4_def->visuals = iw4::native::FxElemDefVisuals{};

			// fxElemType is IDENTICAL
			if (native_def->elemType == game::native::FX_ELEM_TYPE_DECAL)
			{
				if (native_def->visuals.markArray)
				{
					iw4_def->visuals.markArray = local_allocator.allocate_array<iw4::native::FxElemMarkVisuals>(native_def->visualCount);
					for (char j = 0; j < native_def->visualCount; ++j)
					{
						constexpr auto arr_count = ARRAYSIZE(native_def->visuals.markArray[j].materials);
						static_assert(arr_count == 2);
						for (auto k = 0; k < arr_count; k++)
						{
							auto materials = iw4_def->visuals.markArray[j].materials;

							if (native_def->visuals.markArray[j].materials[k])
							{
								materials[k] =
									exporter::dump(
										game::native::XAssetType::ASSET_TYPE_MATERIAL,
										{ native_def->visuals.markArray[j].materials[k] }
								).material;
							}
						}
					}
				}
			}
			else if (native_def->visualCount > 1)
			{
				if (native_def->visuals.array)
				{
					iw4_def->visuals.array = local_allocator.allocate_array<iw4::native::FxElemVisuals>(native_def->visualCount);

					for (char j = 0; j < native_def->visualCount; ++j)
					{
						convert(&native_def->visuals.array[j], &iw4_def->visuals.array[j], native_def->elemType);
					}
				}
			}
			else if (native_def->visualCount == 1)
			{
				iw4_def->visuals.instance.anonymous = nullptr; // Making sure it's dead
				convert(&native_def->visuals.instance, &iw4_def->visuals.instance, native_def->elemType);
				assert(iw4_def->visuals.instance.anonymous);
			}

			if (native_def->effectOnImpact.handle)
			{
				iw4_def->effectOnImpact.handle = exporter::dump(game::native::ASSET_TYPE_FX, { native_def->effectOnImpact.handle }).fx;
			}

			if (native_def->effectOnDeath.handle)
			{
				iw4_def->effectOnDeath.handle = exporter::dump(game::native::ASSET_TYPE_FX, { native_def->effectOnDeath.handle }).fx;
			}

			if (native_def->effectEmitted.handle)
			{
				iw4_def->effectEmitted.handle = exporter::dump(game::native::ASSET_TYPE_FX, { native_def->effectEmitted.handle }).fx;
			}

			if (native_def->elemType == game::native::FX_ELEM_TYPE_TRAIL)
			{
				// good
			}
			else if (native_def->elemType == game::native::FX_ELEM_TYPE_SPARKFOUNTAIN)
			{
				// good
			}
			else
			{
				// Unsupported
				iw4_def->extended.unknownDef = nullptr;

				// Only case where this is valid
				assert((native_def->extended.unknownDef != nullptr) == (native_def->elemType == game::native::FX_ELEM_TYPE_SPOT_LIGHT));
			}
		}

		//
		if (iw4_fx->name == "props/car_glass_med"s)
		{
			printf("");
		}
		//

		out.fx = iw4_fx;
	}

	void ifx::write(const iw4::native::XAssetHeader& header)
	{
		AssertSize(iw4::native::FxEffectDef, 32);

		utils::stream buffer;
		buffer.saveArray("IW4xFx  ", 8); // No idea what to fill in
		buffer.saveObject(IW4X_FX_VERSION);

		auto asset = header.fx;
		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->elemDefs)
		{
			buffer.saveArray(asset->elemDefs, asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

			for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
			{
				iw4::native::FxElemDef* elem_def = &asset->elemDefs[i];

				if (elem_def->velSamples)
				{
					AssertSize(game::native::FxElemVelStateSample, 96);
					buffer.saveArray(elem_def->velSamples, elem_def->velIntervalCount + 1);
				}

				if (elem_def->visSamples)
				{
					AssertSize(game::native::FxElemVisStateSample, 48);
					buffer.saveArray(elem_def->visSamples, elem_def->visStateIntervalCount + 1);
				}

				// Save_FxElemDefVisuals
				{
					if (elem_def->elemType == game::native::FX_ELEM_TYPE_DECAL)
					{
						if (elem_def->visuals.markArray)
						{
							buffer.saveArray(elem_def->visuals.markArray, elem_def->visualCount);

							for (char j = 0; j < elem_def->visualCount; ++j)
							{
								if (elem_def->visuals.markArray[j].materials[0])
								{
									buffer.saveString(elem_def->visuals.markArray[j].materials[0]->info.name);
								}

								if (elem_def->visuals.markArray[j].materials[1])
								{
									buffer.saveString(elem_def->visuals.markArray[j].materials[1]->info.name);
								}
							}
						}
					}
					else if (elem_def->visualCount > 1)
					{
						if (elem_def->visuals.array)
						{
							buffer.saveArray(elem_def->visuals.array, elem_def->visualCount);

							for (char j = 0; j < elem_def->visualCount; ++j)
							{
								write(&elem_def->visuals.array[j], elem_def->elemType, &buffer);
							}
						}
					}
					else if (elem_def->visualCount == 1)
					{
						write(&elem_def->visuals.instance, elem_def->elemType, &buffer);
					}
				}

				if (elem_def->effectOnImpact.handle)
				{
					buffer.saveString(elem_def->effectOnImpact.name);
				}

				if (elem_def->effectOnDeath.handle)
				{
					buffer.saveString(elem_def->effectOnDeath.name);
				}

				if (elem_def->effectEmitted.handle)
				{
					buffer.saveString(elem_def->effectEmitted.name);
				}

				// Save_FxElemExtendedDefPtr
				{
					if (elem_def->elemType == game::native::FX_ELEM_TYPE_TRAIL)
					{
						// Save_FxTrailDef
						{
							if (elem_def->extended.trailDef)
							{
								AssertSize(game::native::FxTrailDef, 36);

								game::native::FxTrailDef* trailDef = elem_def->extended.trailDef;
								buffer.saveObject(*trailDef);

								if (trailDef->verts)
								{
									AssertSize(game::native::FxTrailVertex, 20);

									buffer.saveArray(trailDef->verts, trailDef->vertCount);
								}

								if (trailDef->inds)
								{
									buffer.saveArray(trailDef->inds, trailDef->indCount);
								}
							}
						}
					}
					else if (elem_def->elemType == game::native::FX_ELEM_TYPE_SPARKFOUNTAIN)
					{
						if (elem_def->extended.sparkFountainDef)
						{
							buffer.saveObject(*elem_def->extended.sparkFountainDef);
						}
					}
				}
			}
		}

		utils::io::write_file(std::format("{}/fx/{}.iw4xFx", get_export_path(), asset->name), buffer.toBuffer());
	}

	ifx::ifx()
	{
		command::add("dumpFX", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					std::vector<game::native::XAssetHeader> headers{};

					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_FX, [](game::native::XAssetHeader header, void* data) {
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
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_FX, name, false);

					if (header.data)
					{
						if (!is_already_dumped(header))
						{
							exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_FX, name);
						}

						dump(header);
					}
					else
					{
						console::info("could not dump fx %s from the database (cannot find it)\n", name);
					}
				}
			});

	}
}