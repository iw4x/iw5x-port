#include <std_include.hpp>

#include "itechniqueset.hpp"

namespace asset_dumpers
{
    const std::string itechniqueset::techset_suffix = ".5x";

	const std::unordered_map<iw4::native::MaterialTechniqueType, game::native::MaterialTechniqueType> itechniqueset::techniques_from_iw5_to_iw4 
	{
        { iw4::native::TECHNIQUE_DEPTH_PREPASS, game::native::TECHNIQUE_DEPTH_PREPASS },
        { iw4::native::TECHNIQUE_BUILD_FLOAT_Z, game::native::TECHNIQUE_BUILD_FLOAT_Z },
        { iw4::native::TECHNIQUE_BUILD_SHADOWMAP_DEPTH, game::native::TECHNIQUE_BUILD_SHADOWMAP_DEPTH },
        { iw4::native::TECHNIQUE_BUILD_SHADOWMAP_COLOR, game::native::TECHNIQUE_BUILD_SHADOWMAP_COLOR },
        { iw4::native::TECHNIQUE_UNLIT, game::native::TECHNIQUE_UNLIT },
        { iw4::native::TECHNIQUE_EMISSIVE, game::native::TECHNIQUE_EMISSIVE },
        { iw4::native::TECHNIQUE_EMISSIVE_DFOG, game::native::TECHNIQUE_EMISSIVE_DFOG },
        { iw4::native::TECHNIQUE_EMISSIVE_SHADOW, game::native::TECHNIQUE_EMISSIVE_SHADOW },
        { iw4::native::TECHNIQUE_EMISSIVE_SHADOW_DFOG, game::native::TECHNIQUE_EMISSIVE_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT, game::native::TECHNIQUE_LIT },
        { iw4::native::TECHNIQUE_LIT_DFOG, game::native::TECHNIQUE_LIT_DFOG },
        { iw4::native::TECHNIQUE_LIT_SUN, game::native::TECHNIQUE_LIT_SUN },
        { iw4::native::TECHNIQUE_LIT_SUN_DFOG, game::native::TECHNIQUE_LIT_SUN_DFOG },
        { iw4::native::TECHNIQUE_LIT_SUN_SHADOW, game::native::TECHNIQUE_LIT_SUN_SHADOW },
        { iw4::native::TECHNIQUE_LIT_SUN_SHADOW_DFOG, game::native::TECHNIQUE_LIT_SUN_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT_SPOT, game::native::TECHNIQUE_LIT_SPOT },
        { iw4::native::TECHNIQUE_LIT_SPOT_DFOG, game::native::TECHNIQUE_LIT_SPOT_DFOG },
        { iw4::native::TECHNIQUE_LIT_SPOT_SHADOW, game::native::TECHNIQUE_LIT_SPOT_SHADOW },
        { iw4::native::TECHNIQUE_LIT_SPOT_SHADOW_DFOG, game::native::TECHNIQUE_LIT_SPOT_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT_OMNI, game::native::TECHNIQUE_LIT_OMNI },
        { iw4::native::TECHNIQUE_LIT_OMNI_DFOG, game::native::TECHNIQUE_LIT_OMNI_DFOG },
        { iw4::native::TECHNIQUE_LIT_OMNI_SHADOW, game::native::TECHNIQUE_LIT_OMNI_SHADOW },
        { iw4::native::TECHNIQUE_LIT_OMNI_SHADOW_DFOG, game::native::TECHNIQUE_LIT_OMNI_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED, game::native::TECHNIQUE_LIT_INSTANCED },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SUN, game::native::TECHNIQUE_LIT_INSTANCED_SUN },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SUN_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_SUN_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW, game::native::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SPOT, game::native::TECHNIQUE_LIT_INSTANCED_SPOT },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SPOT_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_SPOT_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW, game::native::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_OMNI, game::native::TECHNIQUE_LIT_INSTANCED_OMNI },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_OMNI_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_OMNI_DFOG },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW, game::native::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW },
        { iw4::native::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG, game::native::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG },
        { iw4::native::TECHNIQUE_LIGHT_SPOT, game::native::TECHNIQUE_LIGHT_SPOT },
        { iw4::native::TECHNIQUE_LIGHT_OMNI, game::native::TECHNIQUE_LIGHT_OMNI },
        { iw4::native::TECHNIQUE_LIGHT_SPOT_SHADOW, game::native::TECHNIQUE_LIGHT_SPOT_SHADOW },
        { iw4::native::TECHNIQUE_FAKELIGHT_NORMAL, game::native::TECHNIQUE_FAKELIGHT_NORMAL },
        { iw4::native::TECHNIQUE_FAKELIGHT_VIEW,game::native::TECHNIQUE_FAKELIGHT_VIEW },
        { iw4::native::TECHNIQUE_SUNLIGHT_PREVIEW,game::native::TECHNIQUE_SUNLIGHT_PREVIEW },
        { iw4::native::TECHNIQUE_CASE_TEXTURE, game::native::TECHNIQUE_CASE_TEXTURE },
        { iw4::native::TECHNIQUE_WIREFRAME_SOLID, game::native::TECHNIQUE_WIREFRAME_SOLID },
        { iw4::native::TECHNIQUE_WIREFRAME_SHADED, game::native::TECHNIQUE_WIREFRAME_SHADED },
        { iw4::native::TECHNIQUE_DEBUG_BUMPMAP, game::native::TECHNIQUE_DEBUG_BUMPMAP },
        { iw4::native::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED, game::native::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED },
	};

	itechniqueset::itechniqueset()
	{
	}

	void itechniqueset::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
	}

	void itechniqueset::write(const iw4::native::XAssetHeader& header)
	{
	}
}