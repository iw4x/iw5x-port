#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <module/command.hpp>

#include <module/asset_dumpers/igfximage.hpp>
#include <module/asset_dumpers/imaterial.hpp>
#include <module/asset_dumpers/itechniqueset.hpp>
#include <module/asset_dumpers/ixmodel.hpp>
#include <module/asset_dumpers/iphyspreset.hpp>
#include <module/asset_dumpers/igfxworld.hpp>
#include <module/asset_dumpers/iglassworld.hpp>
#include <module/asset_dumpers/icomworld.hpp>
#include <module/asset_dumpers/ilightdef.hpp>
#include <module/asset_dumpers/iscriptfile.hpp>
#include <module/asset_dumpers/irawfile.hpp>
#include <module/asset_dumpers/imapents.hpp>
#include <module/asset_dumpers/iclipmap.hpp>
#include <module/asset_dumpers/isndalias.hpp>
#include <module/asset_dumpers/iloadedsound.hpp>
#include <module/asset_dumpers/ifxworld.hpp>
#include <module/asset_dumpers/ifx.hpp>

#include "exporter.hpp"
#include <module/scheduler.hpp>
#include <module/log_file.hpp>
#include <module/console.hpp>
#include "asset_dumper.hpp"
#include <utils/io.hpp>
#include <utils/flags.hpp>

const game::native::dvar_t* exporter::export_path_dvar;

asset_dumper* exporter::asset_dumpers[game::native::ASSET_TYPE_COUNT]{};
std::vector<std::string> exporter::captured_scripts{};
std::vector<std::string> exporter::captured_rawfiles{};
std::vector<std::string> exporter::captured_snd{};
std::vector<std::string> exporter::captured_models{};
std::vector<std::string> exporter::captured_fx{};
bool exporter::capture = false;
bool exporter::ready = false;
std::string exporter::map_name{};
std::vector<std::string> exporter::prepared_source{};


typedef bool (*DB_Update_t)();
DB_Update_t DB_Update = (DB_Update_t)0x4CDA40;

typedef void (*Cbuf_Execute_t)(int localClient, int controllerIndex);
Cbuf_Execute_t Cbuf_Execute = (Cbuf_Execute_t)0x546590;

DEFINE_OG_FUNCTION(Com_EventLoop, 0x555880);

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
DEFINE_OG_FUNCTION(Com_AddStartupCommands, 0x5557F0);

void exporter::event_loop()
{
	while (true)
	{
		Com_Frame();
		Com_EventLoop();

		if (ready)
		{
			Cbuf_Execute(0, 0);
		}

		Sleep(1u);
	}
}

void __declspec(naked) Com_ParseCommandLine_Stub(const char* commandLineArgs)
{
	static const auto Com_ParseCommandLine = 0x553B60;

	__asm
	{
		pushad
		mov eax, commandLineArgs
		call Com_ParseCommandLine
		popad
		ret
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


	auto com_consoleLines = (const char**)0x01CEF598;
	auto cmdLine = GetCommandLineA();
	Com_ParseCommandLine_Stub(cmdLine);


	export_path_dvar = game::native::Dvar_RegisterString("export_path", "iw5xport_out/default", game::native::DVAR_NONE, "export path for iw5xport");

	initialize_exporters();
	add_commands();


	DB_Update();
	Com_AddStartupCommands();

	Com_EventLoop();
	Cbuf_Execute(0, 0);
}

void exporter::dump_map(const command::params& params)
{
	if (params.size() < 2) return;
	std::string map_name = params[1];
	exporter::map_name = map_name;

	std::stringstream source{};

	console::info("dumping %s...\n", map_name.c_str());

	command::execute(std::format("loadzone {}_load", map_name), true);

	capture = true;
	command::execute(std::format("loadzone {}", map_name), true);

	while (!DB_Update())
	{
		Sleep(1u);
	}

	prepared_source.clear();
	capture = false;
	source << "\n# Source generated by IW5Xport\n# Louve@XLabs 2023\n\n";

	console::info("dumping fxworld %s...\n", map_name.c_str());
	command::execute("dumpFxWorld", true); // This adds it to the zone source

	prepared_source.emplace_back("\n");
	console::info("dumping comworld %s...\n", map_name.c_str());
	command::execute("dumpComWorld", true); // This adds it to the zone source

	prepared_source.emplace_back("\n");
	console::info("dumping glassworld %s...\n", map_name.c_str());
	command::execute("dumpGlassWorld", true); // This adds it to the zone source

	prepared_source.emplace_back("\n");
	console::info("dumping clipmap %s...\n", map_name.c_str());
	command::execute("dumpClipMap", true); // This adds it to the zone source

	prepared_source.emplace_back("\n");
	console::info("dumping gfxworld %s...\n", map_name.c_str());
	command::execute("dumpGfxWorld", true); // This adds it to the zone source

	for (const auto& script : exporter::captured_scripts)
	{

		if (
			!script.starts_with("maps/mp/gametypes"s) // No general gameplay scripts
		)
		{
			command::execute(std::format("dumpScript {}", script), true); // This adds it to the zone source
		}
	}

	for (const auto& rawfiles : exporter::captured_rawfiles)
	{
		if (rawfiles == std::format("{}_load", map_name) || rawfiles == map_name)
		{
			// Idk what this is, it gets captured but not dumped. Looks like an empty asset
			// IW branding?
			continue;
		}

		if (rawfiles.ends_with(".gsc"))
		{
			// => scriptfiles
			continue;
		}

		command::execute(std::format("dumpRawFile {}", rawfiles), true); // This adds it to the zone source
	}

	captured_rawfiles.clear();
	captured_scripts.clear();

	prepared_source.emplace_back("\n");
	command::execute(std::format("dumpScript maps/mp/{}", map_name), true);
	command::execute(std::format("dumpScript maps/mp/{}_precache", map_name), true);
	command::execute(std::format("dumpScript maps/createart/{}_art", map_name), true);

	prepared_source.emplace_back("\n");
	command::execute(std::format("dumpScript maps/mp/{}_fx", map_name), true);
	command::execute(std::format("dumpScript maps/createfx/{}_fx", map_name), true);
	command::execute(std::format("dumpScript maps/createart/{}_fog", map_name), true);

	prepared_source.emplace_back("\n");
	console::info("Exporting Vision...\n");
	command::execute(std::format("dumpRawFile vision/{}.vision", map_name.data()), true);

	console::info("Exporting Sun...\n");
	command::execute(std::format("dumpRawFile sun/{}.sun", map_name.data()), true);

	prepared_source.emplace_back("\n");
	console::info("Exporting Compass...\n");
	command::execute(std::format("dumpMaterial compass_map_{}", map_name.data()), true);

	console::info("Exporting Loadscreen...\n");
	command::execute(std::format("dumpGfxImage loadscreen_{}", map_name.data()), true);

	console::info("Additional fluff...\n");
	prepared_source.emplace_back("\n# Additional fluff (mostly destructibles)\n");

	for (const auto& fx : exporter::captured_fx)
	{
		command::execute(std::format("dumpFX {}", fx), true); // This adds it to the zone source
	}
	for (const auto& model : exporter::captured_models)
	{
		command::execute(std::format("dumpXModel {}", model), true); // This adds it to the zone source
	}
	for (const auto& snd : exporter::captured_snd)
	{
		command::execute(std::format("dumpSound {}", snd), true); // This adds it to the zone source
	}

	captured_models.clear();
	captured_snd.clear();
	captured_fx.clear();

	console::info("Writing source...\n");
	for (auto s : exporter::prepared_source)
	{
		source << s << "\n";
	}
		
	source << "\n\n\n";
	utils::io::write_file(std::format("{}/{}.csv", export_path_dvar->current.string, map_name), source.str(), false);

	console::info("done!\n");

	// Clear memory of everybody
	initialize_exporters();
	prepared_source.clear();
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
			console::info("successfully loaded %s!\n", info.name);
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
	for (auto dumper : asset_dumpers)
	{
		if (dumper)
		{
			delete dumper;
		}
	}

	asset_dumpers[game::native::XAssetType::ASSET_TYPE_IMAGE] = new asset_dumpers::igfximage();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_MATERIAL] = new asset_dumpers::imaterial();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET] = new asset_dumpers::itechniqueset();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_PHYSPRESET] = new asset_dumpers::iphyspreset();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_XMODEL] = new asset_dumpers::ixmodel();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_GFXWORLD] = new asset_dumpers::igfxworld();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_GLASSWORLD] = new asset_dumpers::iglassworld();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_COMWORLD] = new asset_dumpers::icomworld();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_LIGHT_DEF] = new asset_dumpers::ilightdef();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_SCRIPTFILE] = new asset_dumpers::iscriptfile();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_RAWFILE] = new asset_dumpers::irawfile();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_MAP_ENTS] = new asset_dumpers::imapents();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_CLIPMAP] = new asset_dumpers::iclipmap();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_SOUND] = new asset_dumpers::isndalias();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_LOADED_SOUND] = new asset_dumpers::iloadedsound();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_FXWORLD] = new asset_dumpers::ifxworld();
	asset_dumpers[game::native::XAssetType::ASSET_TYPE_FX] = new asset_dumpers::ifx();
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

void exporter::add_to_source(game::native::XAssetType type, const std::string asset)
{

	auto name = game::native::g_assetNames[type];

	if (type == game::native::ASSET_TYPE_GLASSWORLD)
	{
		name = "game_map_mp"; // Special case for iw4
	}

	auto line = std::format("{},{}", name, asset);

	if (std::find(prepared_source.begin(), prepared_source.end(), line) == prepared_source.end())
	{
		// No dupes in mah source!
		prepared_source.emplace_back(line);
	}
}

void exporter::DB_AddXAsset_Hk(game::native::XAssetType type, game::native::XAssetHeader* header)
{
	if (reinterpret_cast<void*>(-1) == header)
	{
		// ???
		return;
	}

	auto name = game::native::g_assetNames[type];
	auto asset = game::native::XAsset{ type, *header };
	std::string asset_name = game::native::DB_GetXAssetName(&asset);

	if (capture)
	{
		switch (type)
		{

		case game::native::XAssetType::ASSET_TYPE_SCRIPTFILE:
			if (asset_name[0] == ',') asset_name = asset_name.substr(1);
			captured_scripts.push_back(asset_name);
			break;

		case game::native::XAssetType::ASSET_TYPE_RAWFILE:
			if (asset_name[0] == ',') asset_name = asset_name.substr(1);
			captured_rawfiles.push_back(asset_name);
			break;

		case game::native::XAssetType::ASSET_TYPE_XMODEL:
			if (asset_name.contains("body")) break; // Multiplayer team bodies (maybe ultimately we could keep them)
			if (asset_name.contains("viewhands")) break; // Multiplayer viewhands
			
			if (asset_name[0] == ',') asset_name = asset_name.substr(1);

			// Destructibles
			captured_models.push_back(asset_name);
			break;

		case game::native::XAssetType::ASSET_TYPE_SOUND:
			// Destructibles
			if (asset_name[0] == ',') asset_name = asset_name.substr(1);

			if (asset_name.starts_with("weap_")) break; // Weapons

			captured_snd.push_back(asset_name);
			break;

		case game::native::XAssetType::ASSET_TYPE_FX:
			// Destructibles
			if (asset_name[0] == ',') asset_name = asset_name.substr(1);
			captured_fx.push_back(asset_name);
			break;
		}
	}
}

void __declspec(naked) exporter::DB_AddXAsset_stub()
{
	__asm
	{
		pushad

		push ecx
		push[esp + 0x20 + 8]

		call exporter::DB_AddXAsset_Hk

		add esp, 8

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

int exporter::SND_SetDataHook(game::native::MssSound*, char*)
{
	game::native::LoadedSound*** loadedSoundPtr = reinterpret_cast<game::native::LoadedSound***>(0x013E2748);
	auto loadedSound = *(*(loadedSoundPtr));

	// We do not dump rightaway, we'll do so when we need to because of soundaliases
	asset_dumpers::iloadedsound::duplicate_sound_data(loadedSound);
	return 0;
}


void exporter::post_load()
{
	if (utils::flags::has_flag("exporter"))
	{
		// Keep sounds around
		utils::hook(0x4B94EC, exporter::SND_SetDataHook, HOOK_CALL).install()->quick();

		// OnFindAsset
		utils::hook(0x4CAC50, exporter::DB_AddXAsset_stub, HOOK_JUMP).install()->quick();

		// Hide splash window
		utils::hook::nop(0x5CCE28, 5);

		//// Return on Com_StartHunkUsers
		utils::hook::set<unsigned char>(0x556380, 0xC3);

		//// Return on CL_InitRenderer
		utils::hook::set<unsigned char>(0x48EEA0, 0xC3);

		// Return on com_frame_try_block
		utils::hook::set<unsigned char>(0x556470, 0xC3);

		// Kill command whitelist
		utils::hook::set<unsigned char>(0X00553BA0, 0xC3);

		// catch sys_error
		utils::hook(0x5CC43E, Sys_Error_Hk, HOOK_CALL).install()->quick();

		utils::hook(0x5CCED8, event_loop, HOOK_CALL).install()->quick();
		utils::hook(0x5CCE4E, &perform_common_initialization, HOOK_CALL).install()->quick();

		scheduler::once([]() {


			console::info("ready!\n");
			ready = true;
			}, scheduler::pipeline::main);
	}




}

REGISTER_MODULE(exporter)
