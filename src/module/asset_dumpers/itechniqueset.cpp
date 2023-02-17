#include <std_include.hpp>

#include "itechniqueset.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <utils/memory.hpp>
#include <module/exporter.hpp>
#include <module/console.hpp>
#include <unordered_set>
#include <utils\stream.hpp>

#define IW4X_TECHSET_VERSION 1

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

	std::unordered_map<std::uint16_t, std::array<float, 4>> literal_const_map =
	{
		// Offset is straight up added to output 1 
		//    add o1.xyz, r1, c10 CONST_SRC_CODE_EYEOFFSET
		// in vertices so making it zero effectively voids it
		{ game::native::CONST_SRC_CODE_EYEOFFSET, {0.f, 0.f, 0.f, 0.f}},

		// W (4th member) must be zero because otherwise it destroys color with a dot product 4 (DP4)
		// For saturations
		/*
			dp4_pp r2.x, r2, c6 CONST_SRC_CODE_COLOR_SATURATION_R
			dp4_pp r2.y, r2, c7 CONST_SRC_CODE_COLOR_SATURATION_G
			dp4_pp r2.z, r2, c8 CONST_SRC_CODE_COLOR_SATURATION_B
		*/
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_R, {1.f, 0.f, 0.f, 0.f}},
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_G, {0.f, 1.f, 0.f, 0.f}},
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_B, {0.f, 0.f, 1.f, 0.f}}
	};

	// IW5 on the left - IW4 on the right
	const std::unordered_map <std::uint16_t, std::uint16_t> itechniqueset::iw5_code_const_map =
	{
		{ game::native::CONST_SRC_CODE_LIGHT_POSITION, iw4::native::CONST_SRC_CODE_LIGHT_POSITION },
		{ game::native::CONST_SRC_CODE_LIGHT_DIFFUSE, iw4::native::CONST_SRC_CODE_LIGHT_DIFFUSE },
		{ game::native::CONST_SRC_CODE_LIGHT_SPECULAR, iw4::native::CONST_SRC_CODE_LIGHT_SPECULAR },
		{ game::native::CONST_SRC_CODE_LIGHT_SPOTDIR, iw4::native::CONST_SRC_CODE_LIGHT_SPOTDIR },
		{ game::native::CONST_SRC_CODE_LIGHT_SPOTFACTORS, iw4::native::CONST_SRC_CODE_LIGHT_SPOTFACTORS },
		{ game::native::CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT, iw4::native::CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_COLOR, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_COLOR },
		{ game::native::CONST_SRC_CODE_GAMETIME, iw4::native::CONST_SRC_CODE_GAMETIME },

		{ game::native::CONST_SRC_CODE_PIXEL_COST_FRACS, iw4::native::CONST_SRC_CODE_PIXEL_COST_FRACS },
		{ game::native::CONST_SRC_CODE_PIXEL_COST_DECODE, iw4::native::CONST_SRC_CODE_PIXEL_COST_DECODE },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_0, iw4::native::CONST_SRC_CODE_FILTER_TAP_0 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_1, iw4::native::CONST_SRC_CODE_FILTER_TAP_1 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_2, iw4::native::CONST_SRC_CODE_FILTER_TAP_2 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_3, iw4::native::CONST_SRC_CODE_FILTER_TAP_3 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_4, iw4::native::CONST_SRC_CODE_FILTER_TAP_4 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_5, iw4::native::CONST_SRC_CODE_FILTER_TAP_5 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_6, iw4::native::CONST_SRC_CODE_FILTER_TAP_6 },
		{ game::native::CONST_SRC_CODE_FILTER_TAP_7, iw4::native::CONST_SRC_CODE_FILTER_TAP_7 },
		{ game::native::CONST_SRC_CODE_COLOR_MATRIX_R, iw4::native::CONST_SRC_CODE_COLOR_MATRIX_R },
		{ game::native::CONST_SRC_CODE_COLOR_MATRIX_G, iw4::native::CONST_SRC_CODE_COLOR_MATRIX_G },
		{ game::native::CONST_SRC_CODE_COLOR_MATRIX_B, iw4::native::CONST_SRC_CODE_COLOR_MATRIX_B },
		{ game::native::CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET, iw4::native::CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET },
		{ game::native::CONST_SRC_CODE_RENDER_TARGET_SIZE, iw4::native::CONST_SRC_CODE_RENDER_TARGET_SIZE },

		{ game::native::CONST_SRC_CODE_RENDER_SOURCE_SIZE, iw4::native::CONST_SRC_CODE_RENDER_TARGET_SIZE }, // Wrong

		{ game::native::CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR, iw4::native::CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR },
		{ game::native::CONST_SRC_CODE_DOF_EQUATION_SCENE, iw4::native::CONST_SRC_CODE_DOF_EQUATION_SCENE },
		{ game::native::CONST_SRC_CODE_DOF_LERP_SCALE, iw4::native::CONST_SRC_CODE_DOF_LERP_SCALE },
		{ game::native::CONST_SRC_CODE_DOF_LERP_BIAS, iw4::native::CONST_SRC_CODE_DOF_LERP_BIAS },
		{ game::native::CONST_SRC_CODE_DOF_ROW_DELTA, iw4::native::CONST_SRC_CODE_DOF_ROW_DELTA },
		{ game::native::CONST_SRC_CODE_MOTION_MATRIX_X, iw4::native::CONST_SRC_CODE_MOTION_MATRIX_X },
		{ game::native::CONST_SRC_CODE_MOTION_MATRIX_Y, iw4::native::CONST_SRC_CODE_MOTION_MATRIX_Y },
		{ game::native::CONST_SRC_CODE_MOTION_MATRIX_W, iw4::native::CONST_SRC_CODE_MOTION_MATRIX_W },
		{ game::native::CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION, iw4::native::CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION },
		{ game::native::CONST_SRC_CODE_SHADOWMAP_SCALE, iw4::native::CONST_SRC_CODE_SHADOWMAP_SCALE },
		{ game::native::CONST_SRC_CODE_ZNEAR, iw4::native::CONST_SRC_CODE_ZNEAR },
		{ game::native::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE, iw4::native::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE },
		{ game::native::CONST_SRC_CODE_DEBUG_BUMPMAP, iw4::native::CONST_SRC_CODE_DEBUG_BUMPMAP },
		{ game::native::CONST_SRC_CODE_MATERIAL_COLOR, iw4::native::CONST_SRC_CODE_MATERIAL_COLOR },
		{ game::native::CONST_SRC_CODE_FOG, iw4::native::CONST_SRC_CODE_FOG },
		{ game::native::CONST_SRC_CODE_FOG_COLOR_LINEAR, iw4::native::CONST_SRC_CODE_FOG_COLOR_LINEAR },
		{ game::native::CONST_SRC_CODE_FOG_COLOR_GAMMA, iw4::native::CONST_SRC_CODE_FOG_COLOR_GAMMA },
		{ game::native::CONST_SRC_CODE_FOG_SUN_CONSTS, iw4::native::CONST_SRC_CODE_FOG_SUN_CONSTS },
		{ game::native::CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR, iw4::native::CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR },
		{ game::native::CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA, iw4::native::CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA },
		{ game::native::CONST_SRC_CODE_FOG_SUN_DIR, iw4::native::CONST_SRC_CODE_FOG_SUN_DIR },
		{ game::native::CONST_SRC_CODE_GLOW_SETUP, iw4::native::CONST_SRC_CODE_GLOW_SETUP },
		{ game::native::CONST_SRC_CODE_GLOW_APPLY, iw4::native::CONST_SRC_CODE_GLOW_APPLY },
		{ game::native::CONST_SRC_CODE_COLOR_BIAS, iw4::native::CONST_SRC_CODE_COLOR_BIAS },
		{ game::native::CONST_SRC_CODE_COLOR_TINT_BASE, iw4::native::CONST_SRC_CODE_COLOR_TINT_BASE },
		{ game::native::CONST_SRC_CODE_COLOR_TINT_DELTA, iw4::native::CONST_SRC_CODE_COLOR_TINT_DELTA },
		{ game::native::CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA, iw4::native::CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA },
		{ game::native::CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS, iw4::native::CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS },
		{ game::native::CONST_SRC_CODE_ENVMAP_PARMS, iw4::native::CONST_SRC_CODE_ENVMAP_PARMS },
		{ game::native::CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST, iw4::native::CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST },
		{ game::native::CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST, iw4::native::CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST },
		{ game::native::CONST_SRC_CODE_COMPOSITE_FX_DISTORTION, iw4::native::CONST_SRC_CODE_COMPOSITE_FX_DISTORTION },
		{ game::native::CONST_SRC_CODE_POSTFX_FADE_EFFECT, iw4::native::CONST_SRC_CODE_POSTFX_FADE_EFFECT },
		{ game::native::CONST_SRC_CODE_VIEWPORT_DIMENSIONS, iw4::native::CONST_SRC_CODE_VIEWPORT_DIMENSIONS },
		{ game::native::CONST_SRC_CODE_FRAMEBUFFER_READ, iw4::native::CONST_SRC_CODE_FRAMEBUFFER_READ },

		//{ game::native::CONST_SRC_CODE_THERMAL_COLOR_OFFSET, iw4::native::CONST_SRC_CODE_LIGHT_POSITION }, // Wrong
		//{ game::native::CONST_SRC_CODE_PLAYLIST_POPULATION_PARAMS, iw4::native::CONST_SRC_CODE_LIGHT_POSITION }, // Wrong but probably not important (lobby?)

		{ game::native::CONST_SRC_CODE_BASE_LIGHTING_COORDS, iw4::native::CONST_SRC_CODE_BASE_LIGHTING_COORDS },
		{ game::native::CONST_SRC_CODE_LIGHT_PROBE_AMBIENT, iw4::native::CONST_SRC_CODE_LIGHT_PROBE_AMBIENT },
		{ game::native::CONST_SRC_CODE_NEARPLANE_ORG, iw4::native::CONST_SRC_CODE_NEARPLANE_ORG },
		{ game::native::CONST_SRC_CODE_NEARPLANE_DX, iw4::native::CONST_SRC_CODE_NEARPLANE_DX },
		{ game::native::CONST_SRC_CODE_NEARPLANE_DY, iw4::native::CONST_SRC_CODE_NEARPLANE_DY },
		{ game::native::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE, iw4::native::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE },
		{ game::native::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET, iw4::native::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1 },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2 },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0 },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1 },
		{ game::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2, iw4::native::CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2 },
		{ game::native::CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0, iw4::native::CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0 },
		{ game::native::CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1, iw4::native::CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1 },
		{ game::native::CONST_SRC_CODE_DEPTH_FROM_CLIP, iw4::native::CONST_SRC_CODE_DEPTH_FROM_CLIP },
		{ game::native::CONST_SRC_CODE_CODE_MESH_ARG_0, iw4::native::CONST_SRC_CODE_CODE_MESH_ARG_0 },
		{ game::native::CONST_SRC_CODE_CODE_MESH_ARG_1, iw4::native::CONST_SRC_CODE_CODE_MESH_ARG_1 },
		{ game::native::CONST_SRC_CODE_VIEW_MATRIX, iw4::native::CONST_SRC_CODE_VIEW_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_VIEW_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_VIEW_MATRIX },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX, iw4::native::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX },
		{ game::native::CONST_SRC_CODE_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
		{ game::native::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ game::native::CONST_SRC_CODE_WORLD_MATRIX0, iw4::native::CONST_SRC_CODE_WORLD_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX0, iw4::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0, iw4::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ game::native::CONST_SRC_CODE_WORLD_MATRIX1, iw4::native::CONST_SRC_CODE_WORLD_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX1 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX1, iw4::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1, iw4::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 },
		{ game::native::CONST_SRC_CODE_WORLD_MATRIX2, iw4::native::CONST_SRC_CODE_WORLD_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_MATRIX2 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX2, iw4::native::CONST_SRC_CODE_WORLD_VIEW_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 },
		{ game::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2, iw4::native::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 },
		{ game::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2, iw4::native::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 },
		{ game::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2, iw4::native::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 },
	};

	const std::unordered_map<std::uint32_t, std::uint32_t> itechniqueset::iw5_sampler_table =
	{
		{ game::native::TEXTURE_SRC_CODE_BLACK, iw4::native::TEXTURE_SRC_CODE_BLACK },
		{ game::native::TEXTURE_SRC_CODE_WHITE, iw4::native::TEXTURE_SRC_CODE_WHITE },
		{ game::native::TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP, iw4::native::TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP },
		{ game::native::TEXTURE_SRC_CODE_MODEL_LIGHTING, iw4::native::TEXTURE_SRC_CODE_MODEL_LIGHTING },
		//{ game::native::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY, iw4::native::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY },
		//{ game::native::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY, iw4::native::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY },
		{ game::native::TEXTURE_SRC_CODE_SHADOWMAP_SUN, iw4::native::TEXTURE_SRC_CODE_SHADOWMAP_SUN },
		{ game::native::TEXTURE_SRC_CODE_SHADOWMAP_SPOT, iw4::native::TEXTURE_SRC_CODE_SHADOWMAP_SPOT },
		{ game::native::TEXTURE_SRC_CODE_FEEDBACK, iw4::native::TEXTURE_SRC_CODE_FEEDBACK },
		{ game::native::TEXTURE_SRC_CODE_RESOLVED_POST_SUN, iw4::native::TEXTURE_SRC_CODE_RESOLVED_POST_SUN },
		{ game::native::TEXTURE_SRC_CODE_RESOLVED_SCENE, iw4::native::TEXTURE_SRC_CODE_RESOLVED_SCENE },
		{ game::native::TEXTURE_SRC_CODE_POST_EFFECT_0, iw4::native::TEXTURE_SRC_CODE_POST_EFFECT_0 },
		{ game::native::TEXTURE_SRC_CODE_POST_EFFECT_1, iw4::native::TEXTURE_SRC_CODE_POST_EFFECT_1 },
		{ game::native::TEXTURE_SRC_CODE_LIGHT_ATTENUATION, iw4::native::TEXTURE_SRC_CODE_LIGHT_ATTENUATION },

		//{ game::native::TEXTURE_SRC_CODE_LIGHT_CUCOLORIS, iw4::native::TEXTURE_SRC_CODE_WHITE }, // WRONG !

		{ game::native::TEXTURE_SRC_CODE_OUTDOOR, iw4::native::TEXTURE_SRC_CODE_OUTDOOR },
		{ game::native::TEXTURE_SRC_CODE_FLOATZ, iw4::native::TEXTURE_SRC_CODE_FLOATZ },
		{ game::native::TEXTURE_SRC_CODE_PROCESSED_FLOATZ, iw4::native::TEXTURE_SRC_CODE_PROCESSED_FLOATZ },
		{ game::native::TEXTURE_SRC_CODE_RAW_FLOATZ, iw4::native::TEXTURE_SRC_CODE_RAW_FLOATZ },
		{ game::native::TEXTURE_SRC_CODE_HALF_PARTICLES, iw4::native::TEXTURE_SRC_CODE_HALF_PARTICLES },
		{ game::native::TEXTURE_SRC_CODE_HALF_PARTICLES_Z, iw4::native::TEXTURE_SRC_CODE_HALF_PARTICLES_Z },
		{ game::native::TEXTURE_SRC_CODE_CASE_TEXTURE, iw4::native::TEXTURE_SRC_CODE_CASE_TEXTURE },
		{ game::native::TEXTURE_SRC_CODE_CINEMATIC_Y, iw4::native::TEXTURE_SRC_CODE_CINEMATIC_Y },
		{ game::native::TEXTURE_SRC_CODE_CINEMATIC_CR, iw4::native::TEXTURE_SRC_CODE_CINEMATIC_CR },
		{ game::native::TEXTURE_SRC_CODE_CINEMATIC_CB, iw4::native::TEXTURE_SRC_CODE_CINEMATIC_CB },
		{ game::native::TEXTURE_SRC_CODE_CINEMATIC_A, iw4::native::TEXTURE_SRC_CODE_CINEMATIC_A },
		{ game::native::TEXTURE_SRC_CODE_REFLECTION_PROBE, iw4::native::TEXTURE_SRC_CODE_REFLECTION_PROBE },

		{ game::native::TEXTURE_SRC_CODE_COLOR_MANIPULATION, iw4::native::TEXTURE_SRC_CODE_COUNT } // It's wrong, but it's supposed to be random data anyway

		//{ game::native::TEXTURE_SRC_CODE_PIP_SCENE, iw4::native::TEXTURE_SRC_CODE_BLACK }, // WRONG !
		//{ game::native::TEXTURE_SRC_CODE_STREAMING_LOADING, iw4::native::TEXTURE_SRC_CODE_BLACK }, // WRONG !

	};

	const std::unordered_map<std::uint16_t, std::uint16_t> itechniqueset::iw5_argument_type_map = {
		{ game::native::MTL_ARG_MATERIAL_VERTEX_CONST, iw4::native::MTL_ARG_MATERIAL_VERTEX_CONST },
		{ game::native::MTL_ARG_LITERAL_VERTEX_CONST, iw4::native::MTL_ARG_LITERAL_VERTEX_CONST },
		{ game::native::MTL_ARG_MATERIAL_PIXEL_SAMPLER, iw4::native::MTL_ARG_MATERIAL_PIXEL_SAMPLER },
		{ game::native::MTL_ARG_CODE_VERTEX_CONST, iw4::native::MTL_ARG_CODE_VERTEX_CONST },
		{ game::native::MTL_ARG_CODE_PIXEL_SAMPLER, iw4::native::MTL_ARG_CODE_PIXEL_SAMPLER },
		{ game::native::MTL_ARG_CODE_PIXEL_CONST, iw4::native::MTL_ARG_CODE_PIXEL_CONST },
		{ game::native::MTL_ARG_MATERIAL_PIXEL_CONST, iw4::native::MTL_ARG_MATERIAL_PIXEL_CONST },
		{ game::native::MTL_ARG_LITERAL_PIXEL_CONST, iw4::native::MTL_ARG_LITERAL_PIXEL_CONST }
	};

	void itechniqueset::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		const auto native_techset = header.techniqueSet;

		out.techniqueSet = utils::memory::allocate<iw4::native::MaterialTechniqueSet>();
		auto iw4_techset = out.techniqueSet;

#if DEBUG
		std::string tech_name = native_techset->name;
		assert(!tech_name.ends_with(techset_suffix));
#endif

		auto name = std::format("{}{}", native_techset->name, techset_suffix);
		iw4_techset->name = local_allocator.duplicate_string(name);


		iw4_techset->worldVertFormat = native_techset->worldVertFormat; // enum still didn't change

		// ~useless~
		iw4_techset->hasBeenUploaded = (bool)native_techset->unused[0];
		iw4_techset->unused[0] = native_techset->unused[1]; // ?
		// 

		// No remapping for now
		static std::regex zFeatherRegx = std::regex("_zfeather");
		static std::regex smRegx = std::regex("_sm");

		if (name.contains("_zfeather"))
		{
			native_techset->remappedTechniqueSet =
				game::native::DB_FindXAssetHeader(
					game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET,
					std::regex_replace(native_techset->name, zFeatherRegx, "").data(),
					0
				).techniqueSet;

			iw4_techset->remappedTechniqueSet = exporter::dump(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, { native_techset->remappedTechniqueSet }).techniqueSet;
		}
		else if (name.contains("_sm"))
		{
			auto hsm_version = std::regex_replace(native_techset->name, smRegx, "_hsm");
			native_techset->remappedTechniqueSet = game::native::DB_FindXAssetHeader(
				game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET,
				hsm_version.data(),
				0
			).techniqueSet;

			if (native_techset->remappedTechniqueSet)
			{
				iw4_techset->remappedTechniqueSet = exporter::dump(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, { native_techset->remappedTechniqueSet }).techniqueSet;
			}
		}

		// copy techniques to correct spots
		for (size_t i = 0; i < iw4::native::TECHNIQUE_COUNT; i++)
		{
			iw4::native::MaterialTechniqueType technique = static_cast<iw4::native::MaterialTechniqueType>(i);
			if (techniques_from_iw5_to_iw4.contains(technique))
			{
				auto iw5_technique = native_techset->techniques[techniques_from_iw5_to_iw4.at(technique)];
				iw4_techset->techniques[technique] = convert(iw5_technique);
			}
			else
			{
				iw4_techset->techniques[technique] = nullptr;
			}
		}
	}



	void itechniqueset::write(const iw4::native::XAssetHeader& header)
	{
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_TECHNIQUE_SET, header.data);
		assert(result);
	}

	iw4::native::MaterialTechnique* itechniqueset::convert(const game::native::MaterialTechnique* native_technique)
	{
		if (!native_technique) return nullptr;


		utils::memory::allocator str_duplicator;

		auto iw4_technique = reinterpret_cast<iw4::native::MaterialTechnique*>(
			local_allocator.allocate(
				sizeof iw4::native::MaterialTechnique
				- sizeof iw4::native::MaterialPass
				+ sizeof iw4::native::MaterialPass * native_technique->passCount
			)
			);

		iw4_technique->name = local_allocator.duplicate_string(std::format("{}{}", native_technique->name, techset_suffix));
		iw4_technique->flags = native_technique->flags;

		iw4_technique->passCount = native_technique->passCount;

		for (int i = 0; i < iw4_technique->passCount; i++)
		{
			auto native_pass = &native_technique->passArray[i];
			auto iw4_pass = &iw4_technique->passArray[i];

			rapidjson::Value json_pass(rapidjson::kObjectType);


			if (native_pass->vertexDecl)
			{
				iw4_pass->vertexDecl = dump(native_pass->vertexDecl);
			}

			if (native_pass->vertexShader)
			{
				iw4_pass->vertexShader = dump(native_pass->vertexShader);
			}

			if (native_pass->pixelShader)
			{
				iw4_pass->pixelShader = dump(native_pass->pixelShader);
			}

			// Prepass to detect incompatible arguments
			std::unordered_set<int> skipped_args{};
			auto per_obj_arg_count = native_pass->perObjArgCount;
			for (int k = 0; k < native_pass->perPrimArgCount + native_pass->perObjArgCount + native_pass->stableArgCount; ++k)
			{
				const game::native::MaterialShaderArgument* native_arg = &native_pass->args[k];
				if (!iw5_argument_type_map.contains(native_arg->type))
				{
					console::info("Unable to map argument type %d for technique '%s'!\n", native_arg->type, native_technique->name);
					skipped_args.emplace(k);
					per_obj_arg_count--;
					assert(per_obj_arg_count >= 0);
				}
			}

			iw4_pass->perObjArgCount = per_obj_arg_count;
			iw4_pass->perPrimArgCount = native_pass->perPrimArgCount;
			iw4_pass->stableArgCount = native_pass->stableArgCount;
			iw4_pass->customSamplerFlags = native_pass->customSamplerFlags;

			auto total_args = iw4_pass->perPrimArgCount + iw4_pass->perObjArgCount + iw4_pass->stableArgCount;
			iw4_pass->args = local_allocator.allocate_array<game::native::MaterialShaderArgument>(total_args);

			std::vector<game::native::MaterialShaderArgument> arguments_to_sort{};

			for (int k = 0; k < total_args; ++k)
			{
				if (skipped_args.contains(k))
				{
					continue;
				}

				const game::native::MaterialShaderArgument* native_arg = &native_pass->args[k];
				auto iw4_arg = &iw4_pass->args[k];

				iw4_arg->type = iw5_argument_type_map.at(native_arg->type);
				iw4_arg->dest = native_arg->dest;
				iw4_arg->u = native_arg->u;

				if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_CONST
					|| native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_VERTEX_CONST)
				{

					// Handling special cases
					if (literal_const_map.contains(native_arg->u.codeConst.index))
					{
						iw4_arg->type =
							static_cast<unsigned short>(
								native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_CONST ?
								iw4::native::MTL_ARG_LITERAL_PIXEL_CONST :
								iw4::native::MTL_ARG_LITERAL_VERTEX_CONST
								);

						iw4_arg->u.literalConst = reinterpret_cast<float(*)[4]>(literal_const_map.at(native_arg->u.codeConst.index).data());
					}
					else
					{
						if (native_arg->u.codeConst.index == game::native::CONST_SRC_CODE_UNK1)
						{
							__debugbreak();
						}

						// Automap
						if (!iw5_code_const_map.contains(native_arg->u.codeConst.index))
						{
							auto native_const = static_cast<game::native::MaterialArgumentCodeConst>(native_arg->u.codeConst.index);
							console::info("Unable to map code constant %d for technique '%s'! Not exporting technique\n", native_arg->u.codeConst.index, native_technique->name);
							return nullptr;
						}

						auto iw4_index = iw5_code_const_map.at(native_arg->u.codeConst.index);
						iw4_arg->u.codeConst.index = iw4_index;


						assert(iw4_arg->u.codeConst.index < iw4::native::CONST_SRC_TOTAL_COUNT);

					}
				}
				else if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_SAMPLER)
				{
					iw4::native::MaterialTextureSource code_sampler{};

					// check your samplers manually cause i think they don't match
					if (!iw5_sampler_table.contains(static_cast<game::native::MaterialTextureSource>(native_arg->u.codeSampler)))
					{
						console::warn("Unmapped sampler IW5:%i! This normally never happens, or should be very unusual! Mapping to black.\n", native_arg->u.codeSampler);
						code_sampler = iw4::native::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK;
					}
					else
					{
						code_sampler = static_cast<iw4::native::MaterialTextureSource>(iw5_sampler_table.at(native_arg->u.codeSampler));

						if (code_sampler == iw4::native::TEXTURE_SRC_CODE_OUTDOOR)
						{
							iw4_technique->flags |= 0x40; // 0x40 is necessary for outdoor sampling. I'm not exactly sure why - there is no similar example in iw3
						}
					}

					iw4_arg->u.codeSampler = code_sampler;
				}

				arguments_to_sort.push_back(*iw4_arg);
			}

			// This fails if we put vertex arguments first for some reason
			bool is_ocean = std::string(native_technique->name).contains("ocean");

			std::sort(arguments_to_sort.begin(), arguments_to_sort.end(), [this, is_ocean](const game::native::MaterialShaderArgument& arg1, const game::native::MaterialShaderArgument& arg2)
				{
					auto a1_freq = get_update_frequency(arg1);
					auto a2_freq = get_update_frequency(arg2);

					if (a1_freq != a2_freq && !is_ocean)
					{
						return a1_freq < a2_freq;
					}
					else if (arg1.type != arg2.type)
					{
						return arg1.type < arg2.type;
					}
					else if (arg1.type == iw4::native::MTL_ARG_MATERIAL_VERTEX_CONST ||
						arg1.type == iw4::native::MTL_ARG_MATERIAL_PIXEL_CONST ||
						arg1.type == iw4::native::MTL_ARG_MATERIAL_PIXEL_SAMPLER
						)
					{
						return arg1.u.codeSampler < arg2.u.codeSampler;
					}
					else
					{
						return arg1.dest < arg2.dest;
					}
				});

			std::copy(arguments_to_sort.begin(), arguments_to_sort.end(), iw4_pass->args);
		}

		return iw4_technique;
	}

	iw4::native::MaterialVertexDeclaration* itechniqueset::dump(const game::native::MaterialVertexDeclaration* native_decl)
	{
		if (!native_decl) return nullptr;

		iw4::native::MaterialVertexDeclaration* iw4_decl = local_allocator.allocate<iw4::native::MaterialVertexDeclaration>();

		iw4_decl->name = local_allocator.duplicate_string(native_decl->name);
		iw4_decl->hasOptionalSource = native_decl->hasOptionalSource;
		iw4_decl->streamCount = native_decl->streamCount;

		// There's a problem here: we have to make 21 decl fit into 16 slots... Not sure which one to discard
		memcpy(&iw4_decl->routing, &native_decl->routing, sizeof(iw4::native::MaterialVertexStreamRouting));

		for (int i = 0; i < iw4_decl->streamCount; i++)
		{
			if (iw4_decl->routing.data[i].dest >= game::native::MaterialStreamRoutingDestination::STREAM_DST_COUNT
				|| iw4_decl->routing.data[i].source >= game::native::MaterialStreamRoutingSource::STREAM_SRC_COUNT)
			{
				assert(false);
			}
		}

		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_VERTEXDECL, iw4_decl);

		return iw4_decl;
	}

	game::native::MaterialVertexShader* itechniqueset::dump(const game::native::MaterialVertexShader* vs)
	{
		if (!vs) return nullptr;

		auto vs_copy = local_allocator.allocate<game::native::MaterialVertexShader>();
		memcpy_s(vs_copy, sizeof(game::native::MaterialVertexShader), vs, sizeof(game::native::MaterialVertexShader));

		std::string name{};
		if (vs->name)
		{
			name = std::format("{}{}", vs->name, itechniqueset::techset_suffix);
			vs_copy->name = local_allocator.duplicate_string(name);
		}


		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_VERTEXSHADER, vs_copy);
		assert(result);

		return vs_copy;
	}

	game::native::MaterialPixelShader* itechniqueset::dump(const game::native::MaterialPixelShader* ps)
	{
		if (!ps) return nullptr;

		auto ps_copy = local_allocator.allocate<game::native::MaterialPixelShader>();
		memcpy_s(ps_copy, sizeof(game::native::MaterialPixelShader), ps, sizeof(game::native::MaterialPixelShader));

		std::string name{};
		if (ps->name)
		{
			name = std::format("{}{}", ps->name, itechniqueset::techset_suffix);
			ps_copy->name = local_allocator.duplicate_string(name);
		}

		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_PIXELSHADER, ps_copy);
		assert(result);

		return ps_copy;
	}

	iw4::native::MaterialUpdateFrequency itechniqueset::get_update_frequency(const game::native::MaterialShaderArgument& iw4_argument)
	{
		assert(iw4_argument.type < iw4::native::MTL_ARG_COUNT);

		switch (iw4_argument.type)
		{
		case iw4::native::MTL_ARG_CODE_VERTEX_CONST:
		case iw4::native::MTL_ARG_CODE_PIXEL_CONST:
			if (iw4_argument.u.codeConst.index >= ARRAYSIZE(iw4::native::codeConstUpdateFrequency))
			{
				assert(false);
				return iw4::native::MTL_UPDATE_RARELY;
			}
			return iw4::native::codeConstUpdateFrequency[iw4_argument.u.codeConst.index];

		case iw4::native::MTL_ARG_CODE_PIXEL_SAMPLER:
			if (iw4_argument.u.codeSampler >= ARRAYSIZE(iw4::native::codeSamplerUpdateFrequency))
			{
				assert(false);
				return iw4::native::MTL_UPDATE_RARELY;
			}
			return iw4::native::codeSamplerUpdateFrequency[iw4_argument.u.codeSampler];

		default:
			return iw4::native::MTL_UPDATE_RARELY;
		}
	}

	itechniqueset::itechniqueset()
	{
		command::add("dumpTechset", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, [](game::native::XAssetHeader header, void* data) {
						auto asset_dmper = reinterpret_cast<asset_dumper*>(data);

						asset_dmper->dump(header, true);
						}, this, false);
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, name, false);

					if (header.data)
					{
						asset_dumper::dump(header);
						console::info("successfullly dumped techset %s!\n", name);
					}
					else
					{
						console::info("could not dump techset %s from the database (cannot find it)\n", name);
					}
				}
			});
	}
}