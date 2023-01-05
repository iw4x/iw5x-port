#pragma once

class asset_dumper final : public module
{

public:
	inline static const std::string export_path = "out";
	asset_dumper();

	void post_load() override;

private:
	const std::function<void(game::native::XAssetType, game::native::XAssetHeader)> dumpers[game::native::ASSET_TYPE_COUNT]{};
};

