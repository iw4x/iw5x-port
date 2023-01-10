#pragma once
#include <module/command.hpp>


class exporter final : public module
{

#define DEFINE_OG_FUNCTION(name, addr)\
typedef void (*##name##_t)();\
##name##_t name = (##name##_t)addr

public:
	void post_load() override;
	static iw4::native::XAssetHeader dump(game::native::XAssetType type, game::native::XAssetHeader header);

private:
	static void DB_AddXAsset_Hk(game::native::XAssetType type, game::native::XAssetHeader* header);
	static void DB_AddXAsset_stub();

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
	static bool capture;
	static bool ready;
};

