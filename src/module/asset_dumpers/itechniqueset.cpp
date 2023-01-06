#include <std_include.hpp>

#include "itechniqueset.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <utils/memory.hpp>
#include <module/exporter.hpp>
#include <module/console.hpp>
#include <utils/io.hpp>
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

	//

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

		{ game::native::CONST_SRC_CODE_EYEOFFSET, iw4::native::CONST_SRC_NONE }, // Wrong
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_R, iw4::native::CONST_SRC_NONE }, // Wrong
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_G, iw4::native::CONST_SRC_NONE }, // Wrong
		{ game::native::CONST_SRC_CODE_COLOR_SATURATION_B, iw4::native::CONST_SRC_NONE }, // Wrong

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

		{ game::native::CONST_SRC_CODE_THERMAL_COLOR_OFFSET, iw4::native::CONST_SRC_NONE }, // Wrong
		{ game::native::CONST_SRC_CODE_PLAYLIST_POPULATION_PARAMS, iw4::native::CONST_SRC_NONE }, // Wrong but probably not important (lobby?)

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
		{ game::native::CONST_SRC_CODE_CODE_MESH_ARG_LAST, iw4::native::CONST_SRC_CODE_CODE_MESH_ARG_LAST },
		{ game::native::CONST_SRC_CODE_COUNT_FLOAT4, iw4::native::CONST_SRC_CODE_COUNT_FLOAT4 },
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
		{ game::native::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY, iw4::native::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY },
		{ game::native::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY, iw4::native::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY },
		{ game::native::TEXTURE_SRC_CODE_SHADOWMAP_SUN, iw4::native::TEXTURE_SRC_CODE_SHADOWMAP_SUN },
		{ game::native::TEXTURE_SRC_CODE_SHADOWMAP_SPOT, iw4::native::TEXTURE_SRC_CODE_SHADOWMAP_SPOT },
		{ game::native::TEXTURE_SRC_CODE_FEEDBACK, iw4::native::TEXTURE_SRC_CODE_FEEDBACK },
		{ game::native::TEXTURE_SRC_CODE_RESOLVED_POST_SUN, iw4::native::TEXTURE_SRC_CODE_RESOLVED_POST_SUN },
		{ game::native::TEXTURE_SRC_CODE_RESOLVED_SCENE, iw4::native::TEXTURE_SRC_CODE_RESOLVED_SCENE },
		{ game::native::TEXTURE_SRC_CODE_POST_EFFECT_0, iw4::native::TEXTURE_SRC_CODE_POST_EFFECT_0 },
		{ game::native::TEXTURE_SRC_CODE_POST_EFFECT_1, iw4::native::TEXTURE_SRC_CODE_POST_EFFECT_1 },
		{ game::native::TEXTURE_SRC_CODE_LIGHT_ATTENUATION, iw4::native::TEXTURE_SRC_CODE_LIGHT_ATTENUATION },

		{ game::native::TEXTURE_SRC_CODE_LIGHT_CUCOLORIS, iw4::native::TEXTURE_SRC_CODE_WHITE }, // WRONG !

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

		{ game::native::TEXTURE_SRC_CODE_PIP_SCENE, iw4::native::TEXTURE_SRC_CODE_BLACK }, // WRONG !
		{ game::native::TEXTURE_SRC_CODE_COLOR_MANIPULATION, iw4::native::TEXTURE_SRC_CODE_BLACK }, // WRONG !
		{ game::native::TEXTURE_SRC_CODE_STREAMING_LOADING, iw4::native::TEXTURE_SRC_CODE_BLACK }, // WRONG !

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


		auto name = std::format("{}{}", native_techset->name, techset_suffix);
		iw4_techset->name = local_allocator.duplicate_string(name);


		iw4_techset->worldVertFormat = native_techset->worldVertFormat; // enum still didn't change

		// ~useless~
		iw4_techset->hasBeenUploaded = (bool)native_techset->unused[0];
		iw4_techset->unused[0] = native_techset->unused[1]; // ?
		// 

		// No remapping for now
		//static std::regex zFeatherRegx = std::regex("_zfeather");
		//static std::regex smRegx = std::regex("_sm");

		//if (name.contains("_zfeather"))
		//{
		//    techset->remappedTechniqueSet = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, std::regex_replace(techset->name, zFeatherRegx, "").data()).techniqueSet;
		//    iw4Techset->remappedTechniqueSet = Dump(techset->remappedTechniqueSet);
		//}
		//else if (name.contains("_sm"))
		//{
		//    techset->remappedTechniqueSet = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_TECHNIQUE_SET, std::regex_replace(techset->name, smRegx, "_hsm").data()).techniqueSet;
		//    iw4Techset->remappedTechniqueSet = Dump(techset->remappedTechniqueSet);
		//}

		// copy techniques to correct spots
		for (size_t i = 0; i < iw4::native::TECHNIQUE_COUNT; i++)
		{
			iw4::native::MaterialTechniqueType technique = static_cast<iw4::native::MaterialTechniqueType>(i);
			if (techniques_from_iw5_to_iw4.contains(technique))
			{
				auto iw5_technique = native_techset->techniques[techniques_from_iw5_to_iw4.at(technique)];
				iw4_techset->techniques[technique] = dump(iw5_technique);
			}
			else
			{
				iw4_techset->techniques[technique] = nullptr;
			}
		}
	}



	void itechniqueset::write(const iw4::native::XAssetHeader& header)
	{
		auto techset = header.techniqueSet;

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		output.AddMember("version", IW4X_TECHSET_VERSION, allocator);

		if (techset->name)
		{
			output.AddMember("name", RAPIDJSON_STR(techset->name), allocator);
		}

		if (techset->remappedTechniqueSet)
		{
			output.AddMember("remappedTechniqueSet", RAPIDJSON_STR(techset->remappedTechniqueSet->name), allocator);
		}

		output.AddMember("hasBeenUploaded", techset->hasBeenUploaded, allocator);
		output.AddMember("worldVertFormat", techset->worldVertFormat, allocator);

		// This could be an array but it's practical to have the enum index in front
		// Otherwise on a ~48 keys long array the mapping is not immediatly obvious
		rapidjson::Value techniqueMap(rapidjson::kObjectType);

		for (size_t i = 0; i < iw4::native::TECHNIQUE_COUNT; i++)
		{
			rapidjson::Value value = rapidjson::Value(rapidjson::kNullType);

			if (techset->techniques[i])
			{
				value = rapidjson::Value(
					techset->techniques[i]->name,
					allocator
				);
			}

			techniqueMap.AddMember(
				rapidjson::Value(std::to_string(i).c_str(), allocator),
				value,
				allocator);
		}

		output.AddMember("techniques", techniqueMap, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		utils::io::write_file(std::format("{}/techsets/{}.iw4x.json", get_export_path(), techset->name), buff.GetString());
	}

	iw4::native::MaterialTechnique* itechniqueset::dump(const game::native::MaterialTechnique* native_technique)
	{
		if (!native_technique) return nullptr;

		//
		if (native_technique->name == "wireframe_solid_dtex"s)
		{
			printf("");
		}
		//

		utils::memory::allocator str_duplicator;
		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		auto iw4_technique = local_allocator.allocate<iw4::native::MaterialTechnique>();
		iw4_technique->name = local_allocator.duplicate_string(std::format("{}{}", native_technique->name, techset_suffix));
		iw4_technique->flags = native_technique->flags;

		output.AddMember("version", IW4X_TECHSET_VERSION, allocator);
		output.AddMember("name", rapidjson::Value(iw4_technique->name, allocator), allocator);

		// We complete these later
		rapidjson::Value pass_array(rapidjson::kArrayType);

		for (int i = 0; i < native_technique->passCount; i++)
		{
			auto native_pass = &native_technique->passArray[i];
			auto iw4_pass = local_allocator.allocate<iw4::native::MaterialPass>();
			//iw4_technique->passArray[i] = iw4_pass;

			rapidjson::Value json_pass(rapidjson::kObjectType);


			if (native_pass->vertexDecl)
			{
				iw4_pass->vertexDecl = dump(native_pass->vertexDecl);
				json_pass.AddMember("vertexDeclaration", rapidjson::Value(iw4_pass->vertexDecl->name, allocator), allocator);
			}

			if (native_pass->vertexShader)
			{
				iw4_pass->vertexShader = dump(native_pass->vertexShader);
				json_pass.AddMember("vertexShader", rapidjson::Value(iw4_pass->vertexShader->name, allocator), allocator);
			}

			if (native_pass->pixelShader)
			{
				iw4_pass->pixelShader = dump(native_pass->pixelShader);
				json_pass.AddMember("pixelShader", rapidjson::Value(iw4_pass->pixelShader->name, allocator), allocator);
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

			json_pass.AddMember("perPrimArgCount", native_pass->perPrimArgCount, allocator);
			json_pass.AddMember("perObjArgCount", per_obj_arg_count, allocator);
			json_pass.AddMember("stableArgCount", native_pass->stableArgCount, allocator);
			json_pass.AddMember("customSamplerFlags", native_pass->customSamplerFlags, allocator);

			rapidjson::Value arguments_array(rapidjson::kArrayType);

			for (int k = 0; k < native_pass->perPrimArgCount + native_pass->perObjArgCount + native_pass->stableArgCount; ++k)
			{
				if (skipped_args.contains(k))
				{
					continue;
				}

				const game::native::MaterialShaderArgument* native_arg = &native_pass->args[k];
				auto iw4_arg = local_allocator.allocate<iw4::native::MaterialShaderArgument>();

				iw4_arg->type = iw5_argument_type_map.at(native_arg->type);

				rapidjson::Value arg_json(rapidjson::kObjectType);

				arg_json.AddMember("type", iw4_arg->type, allocator);
				arg_json.AddMember("dest", iw4_arg->dest, allocator); // Does not need conversion

				if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_LITERAL_VERTEX_CONST ||
					native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_LITERAL_PIXEL_CONST)
				{
					rapidjson::Value literalsArray(rapidjson::kArrayType);

					// always four
					for (size_t j = 0; j < 4; j++)
					{
						auto cons = native_arg->u.literalConst[j];
						literalsArray.PushBack(*cons, allocator);
					}

					arg_json.AddMember("literals", literalsArray, allocator);
				}
				else if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_VERTEX_CONST
					|| native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_CONST)
				{
					auto newIndex = iw5_code_const_map.find(native_arg->u.codeConst.index);
					if (newIndex == iw5_code_const_map.end())
					{
						console::info("Unable to map code constant %d for technique '%s'! Not exporting technique\n", native_arg->u.codeConst.index, native_technique->name);
						return nullptr;
					}

					unsigned short val = (unsigned short)newIndex->second;

					rapidjson::Value code_const(rapidjson::kObjectType);

					code_const.AddMember("index", val, allocator);
					code_const.AddMember("firstRow", native_arg->u.codeConst.firstRow, allocator);
					code_const.AddMember("rowCount", native_arg->u.codeConst.rowCount, allocator);

					arg_json.AddMember("codeConst", code_const, allocator);
				}
				else if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_MATERIAL_PIXEL_SAMPLER
					|| native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_MATERIAL_VERTEX_CONST
					|| native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_MATERIAL_PIXEL_CONST)
				{
					arg_json.AddMember("nameHash", native_arg->u.nameHash, allocator);
				}
				else if (native_arg->type == game::native::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_SAMPLER)
				{
					iw4::native::MaterialTextureSource code_sampler{};

					if (!iw5_sampler_table.contains(static_cast<game::native::MaterialTextureSource>(native_arg->u.codeSampler)))
					{
						console::warn("Unmapped sampler %i! This normally never happens, or should be very unusual! Mapping to black.\n");
						code_sampler = iw4::native::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK;
					}
					else
					{
						code_sampler = static_cast<iw4::native::MaterialTextureSource>(iw5_sampler_table.at(native_arg->u.codeSampler));
					}

					arg_json.AddMember("codeSampler", code_sampler, allocator);
				}
				else
				{
					console::error("??? This should have been caught earlier!\n");
				}


				arguments_array.PushBack(arg_json, allocator);
			}

			json_pass.AddMember("arguments", arguments_array, allocator);

			pass_array.PushBack(json_pass, allocator);
		}

		const auto flags = std::format("{:016b}", iw4_technique->flags); // no conversion?
		output.AddMember("flags", RAPIDJSON_STR(flags.c_str()), allocator);

		output.AddMember("passArray", pass_array, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		utils::io::write_file(std::format("{}/techniques/{}.iw4x.json", get_export_path(), iw4_technique->name), buff.GetString());

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

		utils::stream buffer;
		buffer.saveArray("IW4xDECL", 8);
		buffer.saveObject(static_cast<char>(IW4X_TECHSET_VERSION));

		buffer.saveObject(*iw4_decl);

		if (iw4_decl->name)
		{
			buffer.saveString(iw4_decl->name);
		}

		utils::io::write_file(std::format("{}/decl/{}.iw4xDECL", get_export_path(), iw4_decl->name), buffer.toBuffer());

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

		utils::stream buffer;
		buffer.saveArray(vs->prog.loadDef.program, vs->prog.loadDef.programSize);

		utils::io::write_file(std::format("{}/vs/{}.cso", get_export_path(), name.data()), buffer.toBuffer());

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

		utils::stream buffer;
		buffer.saveArray(ps->prog.loadDef.program, ps->prog.loadDef.programSize);

		utils::io::write_file(std::format("{}/ps/{}.cso", get_export_path(), name.data()), buffer.toBuffer());

		return ps_copy;
	}

	itechniqueset::itechniqueset()
	{
		command::add("dumpTechset", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					static std::mutex m{};
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
						asset_dumper::dump(header, true);
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