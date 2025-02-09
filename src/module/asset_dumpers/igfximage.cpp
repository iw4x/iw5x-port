#include <std_include.hpp>

#include "igfximage.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"
#include "utils/hook.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module/exporter.hpp>

namespace asset_dumpers
{

	void igfximage::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.image);
		out.image = reinterpret_cast<iw4::native::GfxImage*>(header.image);

		out.image->delayLoadPixels = false; // 🤔
	}

	void igfximage::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_IMAGE, header.data);
		assert(result);
	}

	int igfximage::store_texture_hk()
	{
		game::native::GfxImageLoadDef** loadDef = *reinterpret_cast<game::native::GfxImageLoadDef***>(0x13E2940);
		game::native::GfxImage* image = *reinterpret_cast<game::native::GfxImage**>(0X13E2410);

		size_t size = 16 + (*loadDef)->resourceSize;
		void* data = utils::memory::allocate(size);
		std::memcpy(data, *loadDef, size);

		image->texture.loadDef = reinterpret_cast<game::native::GfxImageLoadDef*>(data);

		return 0;
	}

	void igfximage::release_texture_hk(game::native::XAssetHeader header)
	{
		if (header.image && header.image->texture.loadDef)
		{
			utils::memory::free(header.image->texture.loadDef);
		}
	}

	igfximage::igfximage()
	{
		command::add("dumpGfxImage", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					std::vector<game::native::XAssetHeader> headers{};

					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_IMAGE, [](game::native::XAssetHeader header, void* data) {
						auto headers = reinterpret_cast<std::vector<game::native::XAssetHeader>*>(data);
						
						headers->push_back(header);

					}, &headers, false);

					for (auto header : headers)
					{
						convert_and_write(header, true);
					}
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_IMAGE, name, false);

					if (header.data)
					{
						convert_and_write(header, true);
						console::info("successfullly dumped image %s!\n", name);
					}
					else
					{
						console::info("could not dump image %s from the database (cannot find it)\n", name);
					}
				}
			});

		utils::hook(0x66A700, igfximage::store_texture_hk, HOOK_JUMP).install()->quick();
		utils::hook(0x66A1C0, igfximage::release_texture_hk, HOOK_JUMP).install()->quick();
	}

}