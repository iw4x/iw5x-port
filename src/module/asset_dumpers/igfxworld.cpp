#include <std_include.hpp>

#include "igfxworld.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"
#include <module\exporter.hpp>

namespace asset_dumpers
{

	void igfxworld::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.gfxWorld);

		auto iw4_world = local_allocator.allocate<iw4::native::GfxWorld>();
		auto native_world = header.gfxWorld;

		iw4_world->dpvs = *convert(native_world, &native_world->dpvs);

		out.gfxWorld = iw4_world;
	}

	void igfxworld::write(const iw4::native::XAssetHeader& header)
	{
		auto gfxWorld = header.gfxWorld;
		std::string name = gfxWorld->name;
	}

	iw4::native::GfxWorldDpvsStatic* igfxworld::convert(game::native::GfxWorld* native_world, game::native::GfxWorldDpvsStatic* native_dpvs_static)
	{
		auto iw4_dpvs_static = local_allocator.allocate<iw4::native::GfxWorldDpvsStatic>();
		
		static_assert(sizeof game::native::GfxWorldDpvsStatic == sizeof iw4::native::GfxWorldDpvsStatic);
		std::memcpy(iw4_dpvs_static, native_dpvs_static, sizeof iw4::native::GfxWorldDpvsStatic);

		if (native_dpvs_static->surfacesBounds)
		{
			iw4_dpvs_static->surfacesBounds = local_allocator.allocate_array<game::native::GfxSurfaceBounds>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static->surfacesBounds, native_dpvs_static->surfacesBounds, sizeof game::native::GfxSurfaceBounds * native_world->surfaceCount);
		}

		if (native_dpvs_static->smodelInsts)
		{
			iw4_dpvs_static->smodelInsts = local_allocator.allocate_array<game::native::GfxStaticModelInst>(native_dpvs_static->smodelCount);
			std::memcpy(iw4_dpvs_static->smodelInsts, native_dpvs_static->smodelInsts, sizeof game::native::GfxStaticModelInst * native_dpvs_static->smodelCount);
		}

		if (native_dpvs_static->surfaces)
		{
			iw4_dpvs_static->surfaces = local_allocator.allocate_array<iw4::native::GfxSurface>(native_world->surfaceCount);
			for (unsigned int i = 0; i < native_world->surfaceCount; ++i)
			{
				game::native::GfxSurface* native_surface = &native_dpvs_static->surfaces[i];
				iw4::native::GfxSurface* iw4_surface = &iw4_dpvs_static->surfaces[i];

				static_assert(sizeof game::native::GfxSurface == sizeof iw4::native::GfxSurface);
				std::memcpy(iw4_surface, native_surface, sizeof  iw4::native::GfxSurface);

				if (iw4_surface->material)
				{
					iw4_surface->material = exporter::dump(game::native::XAssetType::ASSET_TYPE_MATERIAL, { iw4_surface->material }).material;
				}
			}
		}

		if (native_dpvs_static->smodelDrawInsts)
		{
			//
			// TODO
			//
		}

		if (native_dpvs_static->surfaceMaterials)
		{
			iw4_dpvs_static->surfaceMaterials = local_allocator.allocate_array<game::native::GfxDrawSurf>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static->surfaceMaterials, native_dpvs_static->surfaceMaterials, sizeof game::native::GfxDrawSurf * native_world->surfaceCount);
		}

		if (native_dpvs_static->surfaceCastsSunShadow)
		{
			iw4_dpvs_static->surfaceCastsSunShadow = local_allocator.allocate_array<unsigned int>(native_world->surfaceCount);
			std::memcpy(iw4_dpvs_static->surfaceCastsSunShadow, native_dpvs_static->surfaceCastsSunShadow, 4 * native_world->surfaceCount);
		}


		return iw4_dpvs_static;
	}

	void igfxworld::write(const iw4::native::GfxWorldDpvsStatic* dpvs_static, utils::stream* buffer)
	{

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

	//	Utils::Hook(0x616E80, igfximage::StoreTexture, HOOK_JUMP).install()->quick();
	//	Utils::Hook(0x488C00, igfximage::ReleaseTexture, HOOK_JUMP).install()->quick();
	}

	//igfximage::~IGfxImage()
	//{

	//}

}