#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <module/command.hpp>

#include "asset_dumper.hpp"

#include "asset_dumpers\igfximage.hpp"

asset_dumper::asset_dumper()
{
	//dumpers[game::native::XAssetType::ASSET_TYPE_IMAGE] = asset_dumpers::igfximage::dump;
}

void asset_dumper::post_load()
{
	new asset_dumpers::igfximage();
}

REGISTER_MODULE(asset_dumper)
