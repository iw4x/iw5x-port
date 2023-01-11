#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct iloadedsound : asset_dumper
	{
		iloadedsound();

	public:
		static void duplicate_sound_data(game::native::LoadedSound* loadedSound);

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		static std::unordered_map<std::string, char*> available_sound_data;
	};
}