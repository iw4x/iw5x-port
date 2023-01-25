#include <std_include.hpp>

#include "ixanimparts.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <xsk/utils/compression.hpp>
#include <module/console.hpp>

#define IW4X_ANIM_VERSION 2

namespace asset_dumpers
{

	void ixanimparts::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.parts);

		// No conversion
		out.parts = header.parts;
	}

	void ixanimparts::write(const iw4::native::XAssetHeader& header)
	{
		assert(header.parts);
		utils::stream buffer;
		buffer.saveArray("IW4xAnim", 8);
		buffer.saveObject(IW4X_ANIM_VERSION);

		auto parts = header.parts;

		buffer.saveArray(parts, 1);

		if (parts->name)
		{
			buffer.saveString(parts->name);
		}

		for (int i = 0; i < parts->boneCount[game::native::XAnimPartType::PART_TYPE_ALL]; ++i)
		{
			buffer.saveString(game::native::SL_ConvertToString(parts->names[i]));
		}

		if (parts->notify)
		{
			buffer.saveArray(parts->notify, parts->notifyCount);

			for (int i = 0; i < parts->notifyCount; ++i)
			{
				buffer.saveString(game::native::SL_ConvertToString(parts->notify[i].name));
			}
		}

		if (parts->dataByte)
		{
			buffer.saveArray(parts->dataByte, parts->dataByteCount);
		}

		if (parts->dataShort)
		{
			buffer.saveArray(parts->dataShort, parts->dataShortCount);
		}

		if (parts->dataInt)
		{
			buffer.saveArray(parts->dataInt, parts->dataIntCount);
		}

		if (parts->randomDataByte)
		{
			buffer.saveArray(parts->randomDataByte, parts->randomDataByteCount);
		}

		if (parts->randomDataShort)
		{
			buffer.saveArray(parts->randomDataShort, parts->randomDataShortCount);
		}

		if (parts->randomDataInt)
		{
			buffer.saveArray(parts->randomDataInt, parts->randomDataIntCount);
		}

		if (parts->indices.data)
		{
			if (parts->numframes < 256)
			{
				buffer.saveArray(parts->indices._1, parts->indexCount); 
			}
			else
			{
				buffer.saveArray(parts->indices._2, parts->indexCount);
			}
		}

		if (parts->deltaPart)
		{
			auto delta = parts->deltaPart;
			static_assert(sizeof game::native::XAnimDeltaPart == 12);

			buffer.saveObject(*delta);
			if (delta->trans)
			{
				buffer.saveObject(*delta->trans);
				if (delta->trans->size)
				{
					buffer.saveObject(delta->trans->u.frames);

					if (parts->numframes > 0xFF)
					{
						buffer.saveArray(delta->trans->u.frames.indices._2, delta->trans->size + 1);
					}
					else
					{
						buffer.saveArray(delta->trans->u.frames.indices._1, delta->trans->size + 1);
					}

					if (delta->trans->u.frames.frames._1)
					{
						if (delta->trans->smallTrans)
						{
							buffer.save(delta->trans->u.frames.frames._1, 3, delta->trans->size + 1);
						}
						else
						{
							buffer.save(delta->trans->u.frames.frames._1, 6, delta->trans->size + 1);
						}
					}
				}
				else {
					buffer.saveObject(delta->trans->u.frame0);
				}
			}
		}

		utils::io::write_file(std::format("{}/xanim/{}.iw4xAnim", get_export_path(), parts->name), buffer.toBuffer());

	}

	ixanimparts::ixanimparts()
	{
		command::add("dumpXAnim", [this](const command::params& params)
			{
				if (params.size() < 2) return;

				auto entry = game::native::DB_FindXAssetEntry(game::native::XAssetType::ASSET_TYPE_XANIMPARTS, params[1]);

				if (entry)
				{
					dump(entry->asset.header);
					exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_XANIMPARTS, entry->asset.header.rawfile->name);
				}
				else
				{
					console::warn("i cannot find %s! :(\n ", params.get(1));
				}
			});
	}
}