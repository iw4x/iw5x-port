#pragma once

#include "game/structs.iw4.hpp"
#include "game/game.hpp"

#include "loader\module.hpp"
#include "../asset_dumper.hpp"

#include <unordered_map>

namespace asset_dumpers
{
	struct itechniqueset : asset_dumper
	{
		itechniqueset();

	public:
		static const std::string techset_suffix;
		const static std::unordered_map<iw4::native::MaterialTechniqueType, game::native::MaterialTechniqueType> techniques_from_iw5_to_iw4;

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;
	};
}