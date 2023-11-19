#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

namespace asset_dumpers
{
	struct iweapon : asset_dumper
	{
		iweapon();

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		iw4::native::WeaponCompleteDef* convert(const game::native::WeaponCompleteDef* asset);
		iw4::native::WeaponDef* convert(const game::native::WeaponDef* asset);

		iw4::native::XModel* append(const iw4::native::XModel* a, const iw4::native::XModel* b);

		// private:
		//static Game::IW4::TracerDef* GenerateTracerDef(Game::IW3::WeaponDef* weapon);
		//static void DifferentiateWeapon(Game::IW4::WeaponCompleteDef* weapon);
		//static void DifferentiateModel(Game::IW4::XModel* model);
	};
}
