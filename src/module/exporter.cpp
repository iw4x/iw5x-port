#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <module/command.hpp>

#include <module/asset_dumpers/igfximage.hpp>
#include <module/asset_dumpers/imaterial.hpp>
#include <module/asset_dumpers/itechniqueset.hpp>

#include "exporter.hpp"
#include <module/scheduler.hpp>
#include <module/log_file.hpp>
#include <module/console.hpp>
#include "asset_dumper.hpp"

asset_dumper* exporter::asset_dumpers[game::native::ASSET_TYPE_COUNT]{};

DEFINE_OG_FUNCTION(Com_EventLoop, 0x555880);

typedef void (*Cbuf_Execute_t)(int localClient, int controllerIndex); 
Cbuf_Execute_t Cbuf_Execute = (Cbuf_Execute_t)0x546590;

DEFINE_OG_FUNCTION(R_RegisterDvars, 0X6678F0);
DEFINE_OG_FUNCTION(Com_Frame, 0x556870);

DEFINE_OG_FUNCTION(SL_Init, 0x5643A0);
DEFINE_OG_FUNCTION(Swap_Init, 0x5C2800);
DEFINE_OG_FUNCTION(Cbuf_Init, 0x545640);
DEFINE_OG_FUNCTION(Cmd_Init, 0x546710);
DEFINE_OG_FUNCTION(Com_InitDvars, 0x5540F0);
DEFINE_OG_FUNCTION(Com_InitHunkMemory, 0x5B6990);
DEFINE_OG_FUNCTION(Scr_InitProgramHunkUser, 0x561AC0);
DEFINE_OG_FUNCTION(LargeLocalInit, 0x5B75B0);
DEFINE_OG_FUNCTION(Con_OneTimeInit, 0x487120);
DEFINE_OG_FUNCTION(Sys_InitCmdEvents, 0x55F730);
DEFINE_OG_FUNCTION(PMem_Init, 0x5C13D0);
DEFINE_OG_FUNCTION(Com_GetInit, 0x556420);

DEFINE_OG_FUNCTION(j_R_InitWorkerThrea, 0x690FF0);
DEFINE_OG_FUNCTION(CL_InitKeyCommands, 0x48C560);
DEFINE_OG_FUNCTION(FS_InitFilesystem, 0x5B20A0);
DEFINE_OG_FUNCTION(Con_InitChannels, 0X49AB60);
DEFINE_OG_FUNCTION(LiveStorage_Init, 0x550520);

DEFINE_OG_FUNCTION(DB_InitThread, 0x4CD7C0);
DEFINE_OG_FUNCTION(Com_InitPreXAssets, 0x5545D0);
DEFINE_OG_FUNCTION(Live_Init, 0x5CB170);
DEFINE_OG_FUNCTION(Content_Init, 0x5BC580);
DEFINE_OG_FUNCTION(Sys_Init, 0x5CC8A0);
DEFINE_OG_FUNCTION(Scr_InitVariables, 0x5650E0);
DEFINE_OG_FUNCTION(Scr_Init, 0x568F40);

DEFINE_OG_FUNCTION(CL_InitOnceForAllClients, 0x487B00);


void exporter::event_loop()
{
	while (true)
	{
		Com_Frame();
		Com_EventLoop();
		Cbuf_Execute(0, 0);

		Sleep(1u);
	}
}

void exporter::perform_common_initialization()
{
	SL_Init();
	Swap_Init();
	Cbuf_Init();
	Cmd_Init();
	Com_InitDvars();
	Com_InitHunkMemory();
	Scr_InitProgramHunkUser();
	LargeLocalInit();
	//bdCore::init(1); // ??
	Con_OneTimeInit();
	Sys_InitCmdEvents();
	PMem_Init();

	//mjpeg_initonce();
	j_R_InitWorkerThrea();
	CL_InitKeyCommands();
	FS_InitFilesystem();
	Con_InitChannels();
	//LiveStorage_Init();

	DB_InitThread();
	Com_InitPreXAssets();

	Live_Init();
	Content_Init();

	Sys_Init();
	Scr_InitVariables();
	Scr_Init();

	CL_InitOnceForAllClients();

	// Manual "renderer" init
	R_RegisterDvars();
	auto renderer = game::native::Dvar_FindVar("r_loadForRenderer");
	memset(&renderer->modified, 0, sizeof game::native::dvar_t - 9);

	//

	auto com_frameTime = (int*)(0x1CF0B88);
	*com_frameTime = 1;

	auto com_fullyInitialized = (bool*)(0x1CF0BB8);
	*com_fullyInitialized = 1;


	load_common_zones();
}

void exporter::dump_map(const command::params& params)
{
	if (params.size() < 2) return;
	std::string mapName = params[1];

	command::execute(std::format("loadzone {}", mapName), true);
}

void exporter::add_commands()
{
command::add("test", []()
	{
		game::native::Conbuf_AppendText("hello!");
	});

command::add("dumpmap", dump_map);

command::add("loadzone", [](const command::params& params)
	{
		if (params.size() < 2) return;
		std::string zone = params[1];

		game::native::XZoneInfo info;
		info.name = zone.data();
		info.allocFlags = 0;
		info.freeFlags = 0;

		game::native::DB_LoadXAssets(&info, 1, 0);
	});
}

void exporter::load_common_zones()
{
	const std::string common_zones[] = {
		"code_post_gfx_mp",
		"localized_code_post_gfx_mp",
		"ui_mp",
		"localized_ui_mp",
		"common_mp",
		"localized_common_mp",
		"patch_mp"
	};

	console::info("loading common zones...\n");

	for (auto& zone : common_zones)
	{
		game::native::XZoneInfo info;
		info.name = zone.data();
		info.allocFlags = 1;
		info.freeFlags = 0;

		game::native::DB_LoadXAssets(&info, 1, 0);
	}


	console::info("done!\n");
}

void exporter::initialize_exporters()
{
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_IMAGE] = new asset_dumpers::igfximage();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_MATERIAL] = new asset_dumpers::imaterial();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET] = new asset_dumpers::itechniqueset();
}

bool exporter::exporter_exists(game::native::XAssetType assetType)
{
	return asset_dumpers[assetType];
}

iw4::native::XAssetHeader exporter::dump(game::native::XAssetType type, game::native::XAssetHeader header)
{
	if (exporter_exists(type))
	{
		return asset_dumpers[type]->dump(header);
	}
	else
	{
		console::warn("Cannot dump type %s, no asset dumper found\n", game::native::g_assetNames[type]);
		return iw4::native::XAssetHeader{};
	}
}

void DB_AddXAsset_Hk(game::native::XAssetType type, game::native::XAssetHeader* header)
{
	if (reinterpret_cast<void*>(-1) == header)
	{
		// ???
		return;
	}

	auto name = game::native::g_assetNames[type];
	auto asset = game::native::XAsset{ type, *header };
	auto assetName = game::native::DB_GetXAssetName(&asset);

	///
	if (type == game::native::XAssetType::ASSET_TYPE_GFXWORLD)
	{
		console::info("loading %s %s\n", name, assetName);
	}

	if (type == game::native::XAssetType::ASSET_TYPE_MATERIAL)
	{
		for (auto i = 0; i < header->material->textureCount; i++)
		{
			if (header->material->textureTable[i].semantic == 0xB)
			{
				printf("");
			}
		}
	}
	///
}

void __declspec(naked) DB_AddXAsset_stub()
{
	__asm
	{
		pushad

			push ecx
			push[esp + 0x20 + 8]

			call DB_AddXAsset_Hk

			add esp,8

		popad

		// original code
		sub     esp, 10h
		mov     eax, [esp + 18h]

		// go back
		push 0x4CAC57
		retn
	}
}

void Sys_Error_Hk(LPCSTR str)
{
	std::string err = str;

	printf("");
}


void exporter::post_load()
{
	// OnFindAsset
	utils::hook(0x4CAC50, DB_AddXAsset_stub, HOOK_JUMP).install()->quick();

	// Hide splash window
	utils::hook::nop(0x5CCE28, 5);

	//// Return on Com_StartHunkUsers
	utils::hook::set(0x556380, 0xC3);

	//// Return on CL_InitRenderer
	utils::hook::set(0x48EEA0, 0xC3);

	// Return on com_frame_try_block
	utils::hook::set(0x556470, 0xC3);

	// catch sys_error
	utils::hook(0x5CC43E, Sys_Error_Hk, HOOK_CALL).install()->quick();

	utils::hook(0x5CCED8, event_loop, HOOK_CALL).install()->quick();
	utils::hook(0x5CCE4E, &perform_common_initialization, HOOK_CALL).install()->quick();

	initialize_exporters();
	add_commands();
}

REGISTER_MODULE(exporter)
