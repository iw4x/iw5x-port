#include <std_include.hpp>

#include "imapents.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <module/console.hpp>

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

namespace asset_dumpers
{
	void imapents::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.mapEnts);

		auto iw4_ents = local_allocator.allocate<iw4::native::MapEnts>();

		static_assert(sizeof(iw4::native::MapEnts) < sizeof (game::native::MapEnts));
		memcpy(iw4_ents, header.mapEnts, sizeof(iw4::native::MapEnts));

		// We need to patch the string cause we don't really... use anything besides that
		// I think the rest is runtime data
		std::string iw5_ents_string = iw4_ents->entityString;

		std::stringstream iw4_ents_string;

		bool in_key = false;
		std::stringstream key_buffer{};
		for (size_t i = 0; i < iw5_ents_string.size(); i++)
		{
			if (iw5_ents_string[i] == ' ')
			{
				if (in_key)
				{
					auto i_key = static_cast<unsigned short>(std::stoi(key_buffer.str()));
					auto decoded_key = xsk::gsc::iw5::resolver::token_name(i_key);
					iw4_ents_string << "    " << decoded_key << ' ';
					key_buffer.str("");
					key_buffer.clear();
					in_key = false;
					continue;
				}
			}
			else if (iw5_ents_string[i] == '\n')
			{
				in_key = true;
			}
			else if (in_key && iw5_ents_string[i] != '{' && iw5_ents_string[i] != '}')
			{
				key_buffer << iw5_ents_string[i];
				continue;
			}

			iw4_ents_string << iw5_ents_string[i];
		}

		auto str = iw4_ents_string.str();
		iw4_ents->entityString = local_allocator.duplicate_string(str);
		iw4_ents->numEntityChars = str.size();

		iw4_ents->name = exporter::fix_map_name(iw4_ents->name, local_allocator);

		out.mapEnts = iw4_ents;
	}

	void imapents::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_MAP_ENTS, header.data);
		assert(result);
	}


	imapents::imapents()
	{
	}
}