#pragma once
#include <module/command.hpp>

namespace iw4of
{
	class api;
}

class exporter final : public module
{

#define DEFINE_OG_FUNCTION(name, addr)\
typedef void (*##name##_t)();\
##name##_t name = (##name##_t)addr

public:
	void post_load() override;
	static iw4::native::XAssetHeader dump(game::native::XAssetType type, game::native::XAssetHeader header);
	static std::string get_map_name() { return map_name; }
	static void add_to_source(game::native::XAssetType type, const std::string asset);

private:
	static void DB_AddXAsset_Hk(game::native::XAssetType type, game::native::XAssetHeader* header);
	static void DB_AddXAsset_stub();
	static int SND_SetDataHook(game::native::MssSound*, char*);

	static void load_common_zones();
	static void initialize_exporters();
	static bool exporter_exists(game::native::XAssetType assetType);
	static void event_loop();
	static void perform_common_initialization();
	static void dump_map(const command::params& params);
	static void add_commands();

	static class asset_dumper* asset_dumpers[game::native::ASSET_TYPE_COUNT];
	static const game::native::dvar_t* export_path_dvar;

	static std::vector<std::string> captured_scripts;
	static std::vector<std::string> captured_rawfiles;
	static std::vector<std::string> captured_snd;
	static std::vector<std::string> captured_models;
	static std::vector<std::string> captured_fx;

	static std::string map_name;
	static std::vector<std::string> prepared_source;
	static bool capture;
	static bool ready;

	static iw4of::api* iw4of_api;
};

