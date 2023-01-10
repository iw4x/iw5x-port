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
		static const std::unordered_map<iw4::native::MaterialTechniqueType, game::native::MaterialTechniqueType> techniques_from_iw5_to_iw4;

	protected:
		void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) override;
		void write(const iw4::native::XAssetHeader& header) override;

	private:
		static const std::unordered_map <std::uint16_t, std::uint16_t> iw5_code_const_map;
		static const std::unordered_map <std::uint32_t, std::uint32_t> iw5_sampler_table;
		static const std::unordered_map <std::uint16_t, std::uint16_t> iw5_argument_type_map;

		iw4::native::MaterialTechnique* convert(const game::native::MaterialTechnique* native_technique);
		void write(const iw4::native::MaterialTechnique* iw4_technique);

		iw4::native::MaterialVertexDeclaration* dump(const game::native::MaterialVertexDeclaration* decl);
		game::native::MaterialVertexShader* dump(const game::native::MaterialVertexShader* vs);
		game::native::MaterialPixelShader* dump(const game::native::MaterialPixelShader* ps);

		iw4::native::MaterialUpdateFrequency get_update_frequency(const game::native::MaterialShaderArgument& iw4_argument);
	};
}