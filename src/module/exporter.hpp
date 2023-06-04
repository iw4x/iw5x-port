#pragma once
#include <module/command.hpp>
#include <game/structs.iw4.hpp>

#include "api.hpp"

namespace asset_dumpers
{
	struct iscriptfile;
}

class exporter final : public module
{

#define DEFINE_OG_FUNCTION(name, addr)\
typedef void (*##name##_t)();\
##name##_t name = (##name##_t)addr

public:
	void post_load() override;
	static iw4::native::XAssetHeader convert_and_write(game::native::XAssetType type, game::native::XAssetHeader header);
	static iw4::native::XAssetHeader convert(game::native::XAssetType type, game::native::XAssetHeader header);
	static std::string get_map_name() { return map_name; }
	static void add_to_source(game::native::XAssetType type, const std::string asset);
	static const iw4of::api* get_api() { return iw4of_api; };
	static void add_rename_asset(const std::string& old_name, const std::string& new_name);
	static std::string get_export_path() { return export_path_dvar->current.string; }
	static std::string get_source();

private:
	static void DB_AddXAsset_Hk(game::native::XAssetType type, game::native::XAssetHeader* header);
	static void DB_AddXAsset_stub();
	static int SND_SetDataHook(game::native::MssSound*, char*);

	static void* find_asset_for_api(int iw4_type, const std::string& name);

	static void load_common_zones();
	static void initialize_exporters();
	static bool exporter_exists(game::native::XAssetType assetType);
	static void event_loop();
	static void perform_common_initialization();
	static void dump_teams(const command::params& params);
	static void dump_map(const command::params& params);
	static void add_commands();

	static class asset_dumper* asset_dumpers[game::native::ASSET_TYPE_COUNT];
	static const game::native::dvar_t* export_path_dvar;
	static const game::native::dvar_t* export_teams_dvar;

	static std::string common_sounds[];

	static std::vector<std::string> captured_scripts;
	static std::vector<std::string> captured_rawfiles;
	static std::vector<std::string> captured_snd;
	static std::vector<std::string> captured_models;
	static std::vector<std::string> captured_fx;

	static std::string map_name;
	static std::vector<std::string> prepared_source;
	static std::unordered_map<std::string, std::string> rawfile_rename_map;
	static std::unordered_map<iw4::native::XAssetType, game::native::XAssetType> iw4_to_iw5_type_table;
	static bool capture;
	static bool ready;

	static iw4of::api* iw4of_api;
};

