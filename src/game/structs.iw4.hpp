#pragma once

#include <game\structs.hpp>

namespace iw4::native
{

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET = 0x0,
		ASSET_TYPE_PHYSCOLLMAP = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL_SURFS = 0x3,
		ASSET_TYPE_XMODEL = 0x4,
		ASSET_TYPE_MATERIAL = 0x5,
		ASSET_TYPE_PIXELSHADER = 0x6,
		ASSET_TYPE_VERTEXSHADER = 0x7,
		ASSET_TYPE_VERTEXDECL = 0x8,
		ASSET_TYPE_TECHNIQUE_SET = 0x9,
		ASSET_TYPE_IMAGE = 0xA,
		ASSET_TYPE_SOUND = 0xB,
		ASSET_TYPE_SOUND_CURVE = 0xC,
		ASSET_TYPE_LOADED_SOUND = 0xD,
		ASSET_TYPE_CLIPMAP_SP = 0xE,
		ASSET_TYPE_CLIPMAP_MP = 0xF,
		ASSET_TYPE_COMWORLD = 0x10,
		ASSET_TYPE_GAMEWORLD_SP = 0x11,
		ASSET_TYPE_GAMEWORLD_MP = 0x12,
		ASSET_TYPE_MAP_ENTS = 0x13,
		ASSET_TYPE_FXWORLD = 0x14,
		ASSET_TYPE_GFXWORLD = 0x15,
		ASSET_TYPE_LIGHT_DEF = 0x16,
		ASSET_TYPE_UI_MAP = 0x17,
		ASSET_TYPE_FONT = 0x18,
		ASSET_TYPE_MENULIST = 0x19,
		ASSET_TYPE_MENU = 0x1A,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
		ASSET_TYPE_WEAPON = 0x1C,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
		ASSET_TYPE_FX = 0x1E,
		ASSET_TYPE_IMPACT_FX = 0x1F,
		ASSET_TYPE_AITYPE = 0x20,
		ASSET_TYPE_MPTYPE = 0x21,
		ASSET_TYPE_CHARACTER = 0x22,
		ASSET_TYPE_XMODELALIAS = 0x23,
		ASSET_TYPE_RAWFILE = 0x24,
		ASSET_TYPE_STRINGTABLE = 0x25,
		ASSET_TYPE_LEADERBOARD = 0x26,
		ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
		ASSET_TYPE_TRACER = 0x28,
		ASSET_TYPE_VEHICLE = 0x29,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
		ASSET_TYPE_COUNT = 0x2B,
		ASSET_TYPE_STRING = 0x2B,
		ASSET_TYPE_ASSETLIST = 0x2C,
	};

	enum StaticModelFlag : char
	{
		STATIC_MODEL_FLAG_SUB_INDEX_MASK = 0x7,
		STATIC_MODEL_FLAG_NO_CAST_SHADOW = 0x10,
		STATIC_MODEL_FLAG_GROUND_LIGHTING = 0x20,
	};

	union SoundAliasFlags
	{
#pragma warning(push)
#pragma warning(disable: 4201)
		struct
		{
			unsigned int looping : 1;
			unsigned int isMaster : 1;
			unsigned int isSlave : 1;
			unsigned int fullDryLevel : 1;
			unsigned int noWetLevel : 1;
			unsigned int unknown : 1;
			unsigned int unk_is3D : 1;
			unsigned int type : 2;
			unsigned int channel : 6;
		}; 
#pragma warning(pop)
		unsigned int intValue;
	};

	static_assert(sizeof(SoundAliasFlags) == sizeof(unsigned int));

	enum SndChannel
	{
		SND_CHANNEL_PHYSICS,
		SND_CHANNEL_AMBDIST1,
		SND_CHANNEL_AMBDIST2,
		SND_CHANNEL_AUTO,
		SND_CHANNEL_AUTO2,
		SND_CHANNEL_AUTODOG,
		SND_CHANNEL_BULLETIMPACT,
		SND_CHANNEL_BULLETWHIZBY,
		SND_CHANNEL_EXPLOSIVEIMPACT,
		SND_CHANNEL_ELEMENT,
		SND_CHANNEL_AUTO2D,
		SND_CHANNEL_VEHICLE,
		SND_CHANNEL_VEHICLELIMITED,
		SND_CHANNEL_MENU,
		SND_CHANNEL_BODY,
		SND_CHANNEL_BODY2D,
		SND_CHANNEL_RELOAD,
		SND_CHANNEL_RELOAD2D,
		SND_CHANNEL_ITEM,
		SND_CHANNEL_EFFECTS1,
		SND_CHANNEL_EFFECTS2,
		SND_CHANNEL_WEAPON,
		SND_CHANNEL_WEAPON2D,
		SND_CHANNEL_NONSHOCK,
		SND_CHANNEL_VOICE,
		SND_CHANNEL_LOCAL,
		SND_CHANNEL_LOCAL2,
		SND_CHANNEL_LOCAL3,
		SND_CHANNEL_AMBIENT,
		SND_CHANNEL_HURT,
		SND_CHANNEL_PLAYER1,
		SND_CHANNEL_PLAYER2,
		SND_CHANNEL_MUSIC,
		SND_CHANNEL_MUSICNOPAUSE,
		SND_CHANNEL_MISSION,
		SND_CHANNEL_ANNOUNCER,
		SND_CHANNEL_SHELLSHOCK,

		SND_CHANNEL_COUNT
	};

	enum FogTypes
	{
		FOG_NORMAL = 0x1,
		FOG_DFOG = 0x2,
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_PINGPONG_0 = 0x6,
		R_RENDERTARGET_PINGPONG_1 = 0x7,
		R_RENDERTARGET_POST_EFFECT_0 = 0x8,
		R_RENDERTARGET_POST_EFFECT_1 = 0x9,
		R_RENDERTARGET_SHADOWMAP_LARGE = 0xA,
		R_RENDERTARGET_SHADOWMAP_SMALL = 0xB,
		R_RENDERTARGET_COUNT = 0xC,
		R_RENDERTARGET_NONE = 0xD,
	};

	enum snd_alias_type_t : char
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_VOICED = 0x3,
		SAT_COUNT,
	};

	struct ComPrimaryLight
	{
		unsigned char type;
		unsigned char canUseShadowMap;
		unsigned char exponent;
		unsigned char unused;
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		const char* defName;
	};

	struct ComWorld
	{
		const char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		int flags;
		char format;
		short dimensions[3];
		int fileSizeForPicmip[4];
	};

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	struct TriggerHull
	{
		game::native::Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct GfxSurface
	{
		game::native::srfTriangles_t tris;
		struct Material* material;
		game::native::GfxSurfaceLightingAndFlags laf;
	};

	struct GfxCell
	{
		game::native::Bounds bounds;
		int portalCount;
		game::native::GfxPortal* portals;
		unsigned char reflectionProbeCount;
		unsigned char* reflectionProbes;
	};

#pragma pack(push, 4)
	struct GfxStaticModelDrawInst
	{
		game::native::GfxPackedPlacement placement;
		struct XModel* model;
		unsigned __int16 cullDist;
		unsigned __int16 lightingHandle;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
		unsigned char firstMtlSkinIndex;
		game::native::GfxColor groundLighting;
		unsigned __int16 cacheId[4];
	};


	struct SunLightParseParams
	{
		char name[64];
		float ambientScale;
		float ambientColor[3];
		float diffuseFraction;
		float sunLight;
		float sunColor[3];
		float diffuseColor[3];
		bool diffuseColorHasBeenSet;
		float angles[3];
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImage
	{
		game::native::GfxTexture texture;
		unsigned char mapType;
		game::native::GfxImageCategory semantic;
		unsigned char category;
		bool useSrgbReads;
		Picmip picmip;
		bool noPicmip;
		char track;
		CardMemory cardMemory;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		bool delayLoadPixels;
		const char* name;
	};

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxLightImage
	{
		GfxImage* image;
		unsigned char samplerState;
	};

	struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage** reflectionProbes;
		game::native::GfxReflectionProbe* reflectionProbeOrigins;
		game::native::GfxTexture* reflectionProbeTextures;
		int lightmapCount;
		GfxLightmapArray* lightmaps;
		game::native::GfxTexture* lightmapPrimaryTextures;
		game::native::GfxTexture* lightmapSecondaryTextures;
		GfxImage* lightmapOverridePrimary;
		GfxImage* lightmapOverrideSecondary;
		unsigned int vertexCount;
		game::native::GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		game::native::GfxWorldVertexLayerData vld;
		unsigned int indexCount;
		unsigned __int16* indices;
	};


	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		int skySamplerState;
	};


	struct GfxHeroOnlyLight
	{
		char type;
		char unused[3];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litOpaqueSurfsBegin;
		unsigned int litOpaqueSurfsEnd;
		unsigned int litTransSurfsBegin;
		unsigned int litTransSurfsEnd;
		unsigned int shadowCasterSurfsBegin;
		unsigned int shadowCasterSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		char* smodelVisData[3];
		char* surfaceVisData[3];
		unsigned __int16* sortedSurfIndex;
		game::native::GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		game::native::GfxSurfaceBounds* surfacesBounds;
		GfxStaticModelDrawInst* smodelDrawInsts;
		game::native::GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct GfxLightGrid
	{
		bool hasLightRegions;
		unsigned int lastSunPrimaryLightIndex;
		unsigned short mins[3];
		unsigned short maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned short* rowDataStart;
		unsigned int rawRowDataSize;
		unsigned char* rawRowData;
		unsigned int entryCount;
		game::native::GfxLightGridEntry* entries;
		unsigned int colorCount;
		game::native::GfxLightGridColors* colors;
	};

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct sunflare_t
	{
		bool hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		int planeCount;
		int nodeCount;
		unsigned int surfaceCount;
		int skyCount;
		GfxSky* skies;
		unsigned int lastSunPrimaryLightIndex;
		unsigned int primaryLightCount;
		unsigned int sortKeyLitDecal;
		unsigned int sortKeyEffectDecal;
		unsigned int sortKeyEffectAuto;
		unsigned int sortKeyDistortion;
		game::native::GfxWorldDpvsPlanes dpvsPlanes;
		game::native::GfxCellTreeCount* aabbTreeCounts;
		game::native::GfxCellTree* aabbTrees;
		GfxCell* cells;
		GfxWorldDraw draw;
		GfxLightGrid lightGrid;
		int modelCount;
		game::native::GfxBrushModel* models;
		game::native::Bounds bounds;
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory* materialMemory;
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		unsigned int* cellCasterBits;
		unsigned int* cellHasSunLitSurfsBits;
		game::native::GfxSceneDynModel* sceneDynModel;
		game::native::GfxSceneDynBrush* sceneDynBrush;
		unsigned int* primaryLightEntityShadowVis;
		unsigned int* primaryLightDynEntShadowVis[2];
		unsigned char* nonSunPrimaryLightForModelDynEnt;
		game::native::GfxShadowGeometry* shadowGeom;
		game::native::GfxLightRegion* lightRegion;
		GfxWorldDpvsStatic dpvs;
		game::native::GfxWorldDpvsDynamic dpvsDyn;
		unsigned int mapVtxChecksum;
		unsigned int heroOnlyLightCount;
		GfxHeroOnlyLight* heroOnlyLights;
		char fogTypesAllowed;
	};

#pragma pack(pop)

	enum file_image_flags_t
	{
		IMG_FLAG_NOPICMIP = 0x1,
		IMG_FLAG_NOMIPMAPS = 0x2,
		IMG_FLAG_STREAMING = 0x4,
		IMG_FLAG_LEGACY_NORMALS = 0x8,
		IMG_FLAG_CLAMP_U = 0x10,
		IMG_FLAG_CLAMP_V = 0x20,
		IMG_FLAG_ALPHA_WEIGHTED_COLORS = 0x40,
		IMG_FLAG_DXTC_APPROX_WEIGHTS = 0x80,
		IMG_FLAG_GAMMA_NONE = 0x0,
		IMG_FLAG_GAMMA_SRGB = 0x100,
		IMG_FLAG_GAMMA_PWL = 0x200,
		IMG_FLAG_GAMMA_2 = 0x300,
		IMG_FLAG_MAPTYPE_2D = 0x0,
		IMG_FLAG_MAPTYPE_CUBE = 0x10000,
		IMG_FLAG_MAPTYPE_3D = 0x20000,
		IMG_FLAG_MAPTYPE_1D = 0x30000,
		IMG_FLAG_NORMALMAP = 0x40000,
		IMG_FLAG_INTENSITY_TO_ALPHA = 0x80000,
		IMG_FLAG_DYNAMIC = 0x1000000,
		IMG_FLAG_RENDER_TARGET = 0x2000000,
		IMG_FLAG_SYSTEMMEM = 0x4000000,
	};
	
	union PackedTexCoords
	{
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		game::native::GfxColor color;
		PackedTexCoords texCoord;
		game::native::PackedUnitVec normal;
		game::native::PackedUnitVec tangent;
	};

	struct XSurface
	{
		unsigned char tileMode;
		bool deformed;
		unsigned __int16 vertCount;
		unsigned __int16 triCount;
		char zoneHandle;
		unsigned __int16 baseTriIndex;
		unsigned __int16 baseVertIndex;
		unsigned __int16* triIndices;
		game::native::XSurfaceVertexInfo vertInfo;
		GfxPackedVertex* verts0;
		unsigned int vertListCount;
		game::native::XRigidVertList* vertList;
		int partBits[6];
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfaces;
		int numSurfaces;
		int partBits[6];
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[6];
		XSurface* surfs;
		char lod;
		char smcBaseIndexPlusOne;
		char smcSubIndexMask;
		char smcBucket;
	};

	struct PhysPreset
	{
		const char* name;
		int type;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		bool tempDefaultToCylinder;
		bool perSurfaceSndAlias;
	};

	struct XModel
	{
		const char* name;
		unsigned char numBones;
		unsigned char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		float scale;
		unsigned int noScalePartBits[6];
		unsigned __int16* boneNames;
		unsigned char* parentList;
		short *quats;
		float *trans;
		unsigned char* partClassification;
		game::native::DObjAnimMat* baseMat;
		Material** materialHandles;
		XModelLodInfo lodInfo[4];
		char maxLoadedLod;
		char numLods;
		char collLod;
		unsigned char flags;
		game::native::XModelCollSurf_s* collSurfs;
		int numCollSurfs;
		int contents;
		game::native::XBoneInfo* boneInfo;
		float radius;
		game::native::Bounds bounds;
		int memUsage;
		bool bad;
		PhysPreset* physPreset;
		game::native::PhysCollmap* physCollmap;
	};

	static_assert(sizeof iw4::native::XModel == 304);

	struct cStaticModel_t
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dmaterial_t
	{
		char* material;
		int surfaceFlags;
		int contentFlags;
	};

	struct cLeafBrushNodeLeaf_t
	{
		unsigned short* brushes;
	};

	struct cLeafBrushNode_t
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeLeaf_t data;
		char pad[8];
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		game::native::Stage* stages;
		char stageCount;
	};

	enum DynEntityType
	{
		DYNENT_TYPE_INVALID = 0x0,
		DYNENT_TYPE_CLUTTER = 0x1,
		DYNENT_TYPE_DESTRUCT = 0x2,
		DYNENT_TYPE_COUNT = 0x3,
	};

	struct DynEntityDef
	{
		DynEntityType type;
		game::native::GfxPlacement pose;
		XModel* xModel;
		unsigned short brushModel;
		unsigned short physicsBrushModel;
		struct FxEffectDef* destroyFx;
		PhysPreset* physPreset;
		int health;
		game::native::PhysMass mass;
		int contents;
	};

	struct DynEntityClient
	{
		int physObjId;
		unsigned short flags;
		unsigned short lightingHandle;
		int health;
	};

	struct cStaticModel_s
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		game::native::Bounds absBounds;
	};

	struct cmodel_t
	{
		game::native::Bounds bounds;
		float radius;
		game::native::cLeaf_t leaf;
	};

	struct clipMap_t
	{
		const char* name;
		int isInUse;
		int planeCount;
		game::native::cplane_s* planes;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unsigned int numMaterials;
		game::native::ClipMaterial* materials;
		unsigned int numBrushSides;
		game::native::cbrushside_t* brushsides;
		unsigned int numBrushEdges;
		unsigned char* brushEdges;
		unsigned int numNodes;
		game::native::cNode_t* nodes;
		unsigned int numLeafs;
		game::native::cLeaf_t* leafs;
		unsigned int leafbrushNodesCount;
		game::native::cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16* leafbrushes;
			unsigned int numLeafSurfaces;
			unsigned int* leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		int triCount;
		unsigned __int16* triIndices;
		unsigned char* triEdgeIsWalkable;
		int borderCount;
		game::native::CollisionBorder* borders;
		int partitionCount;
		game::native::CollisionPartition* partitions;
		int aabbTreeCount;
		game::native::CollisionAabbTree* aabbTrees;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned __int16 numBrushes;
		game::native::cbrush_t* brushes;
		game::native::Bounds* brushBounds;
		int* brushContents;
		MapEnts* mapEnts;
		unsigned __int16 smodelNodeCount;
		game::native::SModelAabbNode* smodelNodes;
		unsigned __int16 dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		game::native::DynEntityPose* dynEntPoseList[2];
		DynEntityClient* dynEntClientList[2];
		game::native::DynEntityColl* dynEntCollList[2];
		unsigned int checksum;
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_DFOG = 0x6,
		TECHNIQUE_EMISSIVE_SHADOW = 0x7,
		TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
		TECHNIQUE_LIT_BEGIN = 0x9,
		TECHNIQUE_LIT = 0x9,
		TECHNIQUE_LIT_DFOG = 0xA,
		TECHNIQUE_LIT_SUN = 0xB,
		TECHNIQUE_LIT_SUN_DFOG = 0xC,
		TECHNIQUE_LIT_SUN_SHADOW = 0xD,
		TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
		TECHNIQUE_LIT_SPOT = 0xF,
		TECHNIQUE_LIT_SPOT_DFOG = 0x10,
		TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
		TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
		TECHNIQUE_LIT_OMNI = 0x13,
		TECHNIQUE_LIT_OMNI_DFOG = 0x14,
		TECHNIQUE_LIT_OMNI_SHADOW = 0x15,
		TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x16,
		TECHNIQUE_LIT_INSTANCED = 0x17,
		TECHNIQUE_LIT_INSTANCED_DFOG = 0x18,
		TECHNIQUE_LIT_INSTANCED_SUN = 0x19,
		TECHNIQUE_LIT_INSTANCED_SUN_DFOG = 0x1A,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x1B,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG = 0x1C,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x1D,
		TECHNIQUE_LIT_INSTANCED_SPOT_DFOG = 0x1E,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x1F,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG = 0x20,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x21,
		TECHNIQUE_LIT_INSTANCED_OMNI_DFOG = 0x22,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x23,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG = 0x24,
		TECHNIQUE_LIT_END = 0x25,
		TECHNIQUE_LIGHT_SPOT = 0x25,
		TECHNIQUE_LIGHT_OMNI = 0x26,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x27,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x28,
		TECHNIQUE_FAKELIGHT_VIEW = 0x29,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x2A,
		TECHNIQUE_CASE_TEXTURE = 0x2B,
		TECHNIQUE_WIREFRAME_SOLID = 0x2C,
		TECHNIQUE_WIREFRAME_SHADED = 0x2D,
		TECHNIQUE_DEBUG_BUMPMAP = 0x2E,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x2F,
		TECHNIQUE_COUNT = 0x30,
		TECHNIQUE_TOTAL_COUNT = 0x31,
		TECHNIQUE_NONE = 0x32,
	};

#define NAMEOF(s) #s
	enum MaterialTextureSource : unsigned int
	{
		TEXTURE_SRC_CODE_BLACK = 0x0,
		TEXTURE_SRC_CODE_WHITE = 0x1,
		TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP = 0x2,
		TEXTURE_SRC_CODE_MODEL_LIGHTING = 0x3,
		TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY = 0x4,
		TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY = 0x5,
		TEXTURE_SRC_CODE_SHADOWMAP_SUN = 0x6,
		TEXTURE_SRC_CODE_SHADOWMAP_SPOT = 0x7,
		TEXTURE_SRC_CODE_FEEDBACK = 0x8,
		TEXTURE_SRC_CODE_RESOLVED_POST_SUN = 0x9,
		TEXTURE_SRC_CODE_RESOLVED_SCENE = 0xA,
		TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xB,
		TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xC,
		TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0xD,
		TEXTURE_SRC_CODE_OUTDOOR = 0xE,
		TEXTURE_SRC_CODE_FLOATZ = 0xF,
		TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x10,
		TEXTURE_SRC_CODE_RAW_FLOATZ = 0x11,
		TEXTURE_SRC_CODE_HALF_PARTICLES = 0x12,
		TEXTURE_SRC_CODE_HALF_PARTICLES_Z = 0x13,
		TEXTURE_SRC_CODE_CASE_TEXTURE = 0x14,
		TEXTURE_SRC_CODE_CINEMATIC_Y = 0x15,
		TEXTURE_SRC_CODE_CINEMATIC_CR = 0x16,
		TEXTURE_SRC_CODE_CINEMATIC_CB = 0x17,
		TEXTURE_SRC_CODE_CINEMATIC_A = 0x18,
		TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x19,
		TEXTURE_SRC_CODE_ALTERNATE_SCENE = 0x1A,
		TEXTURE_SRC_CODE_COUNT = 0x1B,
	};

	static const char* Debug_MaterialTextureSourceNames[]
	{
		NAMEOF(TEXTURE_SRC_CODE_BLACK),
		NAMEOF(TEXTURE_SRC_CODE_WHITE),
		NAMEOF(TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP),
		NAMEOF(TEXTURE_SRC_CODE_MODEL_LIGHTING),
		NAMEOF(TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY),
		NAMEOF(TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY),
		NAMEOF(TEXTURE_SRC_CODE_SHADOWMAP_SUN),
		NAMEOF(TEXTURE_SRC_CODE_SHADOWMAP_SPOT),
		NAMEOF(TEXTURE_SRC_CODE_FEEDBACK),
		NAMEOF(TEXTURE_SRC_CODE_RESOLVED_POST_SUN),
		NAMEOF(TEXTURE_SRC_CODE_RESOLVED_SCENE),
		NAMEOF(TEXTURE_SRC_CODE_POST_EFFECT_0),
		NAMEOF(TEXTURE_SRC_CODE_POST_EFFECT_1),
		NAMEOF(TEXTURE_SRC_CODE_LIGHT_ATTENUATION),
		NAMEOF(TEXTURE_SRC_CODE_OUTDOOR),
		NAMEOF(TEXTURE_SRC_CODE_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_PROCESSED_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_RAW_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_HALF_PARTICLES),
		NAMEOF(TEXTURE_SRC_CODE_HALF_PARTICLES_Z),
		NAMEOF(TEXTURE_SRC_CODE_CASE_TEXTURE),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_Y),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_CR),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_CB),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_A),
		NAMEOF(TEXTURE_SRC_CODE_REFLECTION_PROBE),
		NAMEOF(TEXTURE_SRC_CODE_ALTERNATE_SCENE)
	};

	static_assert(ARRAYSIZE(Debug_MaterialTextureSourceNames) == MaterialTextureSource::TEXTURE_SRC_CODE_COUNT);

	enum StateFlags : unsigned char {
		STATE_FLAG_CULL_BACK = 0x1,
		STATE_FLAG_AMBIENT = 0x2,
		STATE_FLAG_DECAL = 0x4,
		STATE_FLAG_WRITES_DEPTH = 0x8,
		STATE_FLAG_USES_DEPTH_BUFFER = 0x10,
		STATE_FLAG_USES_STENCIL_BUFFER = 0x20,
		STATE_FLAG_CULL_BACK_SHADOW = 0x40,
	};
	enum MaterialStateSet
	{
		MTL_STATE_SET_ALPHA_TEST = 0x0,
		MTL_STATE_SET_BLEND_FUNC_RGB = 0x1,
		MTL_STATE_SET_BLEND_FUNC_ALPHA = 0x2,
		MTL_STATE_SET_CULL_FACE = 0x3,
		MTL_STATE_SET_DEPTH_TEST = 0x4,
		MTL_STATE_SET_DEPTH_WRITE = 0x5,
		MTL_STATE_SET_COLOR_WRITE = 0x6,
		MTL_STATE_SET_GAMMA_WRITE = 0x7,
		MTL_STATE_SET_POLYGON_OFFSET = 0x8,
		MTL_STATE_SET_STENCIL = 0x9,
		MTL_STATE_SET_WIREFRAME = 0xA,
		MTL_STATE_SET_COUNT = 0xB,
	};

	struct MaterialGameFlagsFields
	{
		unsigned char unk1 : 1; // 0x1
		unsigned char addShadowToPrimaryLight : 1; // 0x2
		unsigned char isFoliageRequiresGroundLighting : 1; // 0x4
		unsigned char unk4 : 1; // 0x8
		unsigned char unk5 : 1; // 0x10
		unsigned char unk6 : 1; // 0x20
		unsigned char unk7 : 1; // 0x40
		unsigned char unkCastShadowMaybe : 1; // 0x80
	};

	union MaterialGameFlags
	{
		MaterialGameFlagsFields fields;
		unsigned char packed;
	};

	enum GfxCameraRegionType
	{
		CAMERA_REGION_LIT_OPAQUE = 0x0,
		CAMERA_REGION_LIT_TRANS = 0x1,
		CAMERA_REGION_EMISSIVE = 0x2,
		CAMERA_REGION_DEPTH_HACK = 0x3,
		CAMERA_REGION_COUNT = 0x4,
		CAMERA_REGION_NONE = 0x4,
	};

	enum TextureSemantic
	{
		TS_2D = 0x0,
		TS_FUNCTION = 0x1,
		TS_COLOR_MAP = 0x2,
		TS_DETAIL_MAP = 0x3,
		TS_UNUSED_2 = 0x4,
		TS_NORMAL_MAP = 0x5,
		TS_UNUSED_3 = 0x6,
		TS_UNUSED_4 = 0x7,
		TS_SPECULAR_MAP = 0x8,
		TS_UNUSED_5 = 0x9,
		TS_UNUSED_6 = 0xA,
		TS_WATER_MAP = 0xB
	};

	union MaterialTextureDefInfo
	{
		GfxImage* image;
		game::native::water_t* water;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned char samplerState;
		unsigned char semantic;
		MaterialTextureDefInfo u;
	};

	enum MaterialShaderArgumentType
	{
		MTL_ARG_MATERIAL_VERTEX_CONST = 0x0, // stable
		MTL_ARG_LITERAL_VERTEX_CONST = 0x1, // stable
		MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x2, // stable

		MTL_ARG_CODE_PRIM_BEGIN = 0x3,

		MTL_ARG_CODE_VERTEX_CONST = 0x3, // stable object prim
		MTL_ARG_CODE_PIXEL_SAMPLER = 0x4, // stable object
		MTL_ARG_CODE_PIXEL_CONST = 0x5, // stable

		MTL_ARG_CODE_PRIM_END = 0x6,

		MTL_ARG_MATERIAL_PIXEL_CONST = 0x6, // stable
		MTL_ARG_LITERAL_PIXEL_CONST = 0x7, // stable
		MTL_ARG_COUNT = 0x8,
	};

	static const char* Debug_MaterialShaderArgumentTypeName[]
	{
		NAMEOF(MTL_ARG_MATERIAL_VERTEX_CONST),
		NAMEOF(MTL_ARG_LITERAL_VERTEX_CONST),
		NAMEOF(MTL_ARG_MATERIAL_PIXEL_SAMPLER),

		NAMEOF(MTL_ARG_CODE_VERTEX_CONST = 0x3),
		NAMEOF(MTL_ARG_CODE_PIXEL_SAMPLER = 0x4),
		NAMEOF(MTL_ARG_CODE_PIXEL_CONST = 0x5),

		NAMEOF(MTL_ARG_MATERIAL_PIXEL_CONST = 0x6),
		NAMEOF(MTL_ARG_LITERAL_PIXEL_CONST = 0x7)
	};

	static_assert(ARRAYSIZE(Debug_MaterialShaderArgumentTypeName) == MaterialShaderArgumentType::MTL_ARG_COUNT);

	enum MaterialUpdateFrequency
	{
		MTL_UPDATE_PER_PRIM = 0x0,
		MTL_UPDATE_PER_OBJECT = 0x1,
		MTL_UPDATE_RARELY = 0x2,
		MTL_UPDATE_CUSTOM = 0x3,
	};

	static const MaterialUpdateFrequency codeSamplerUpdateFrequency[]
	{
		MTL_UPDATE_RARELY, // BLACK 
		MTL_UPDATE_RARELY, // WHITE 
		MTL_UPDATE_RARELY, // IDENTITY_NORMAL_MAP 
		MTL_UPDATE_RARELY, // MODEL_LIGHTING 
		MTL_UPDATE_CUSTOM, // LIGHTMAP_PRIMARY 
		MTL_UPDATE_CUSTOM, // LIGHTMAP_SECONDARY 
		MTL_UPDATE_RARELY, // SHADOWMAP_SUN 
		MTL_UPDATE_RARELY, // SHADOWMAP_SPOT 
		MTL_UPDATE_PER_OBJECT, // FEEDBACK 
		MTL_UPDATE_RARELY, // RESOLVED_POST_SUN 
		MTL_UPDATE_RARELY, // RESOLVED_SCENE 
		MTL_UPDATE_RARELY, // POST_EFFECT_0 
		MTL_UPDATE_RARELY, // POST_EFFECT_1 
		MTL_UPDATE_PER_OBJECT, // LIGHT_ATTENUATION 
		MTL_UPDATE_RARELY, // OUTDOOR 
		MTL_UPDATE_RARELY, // FLOATZ 
		MTL_UPDATE_RARELY, // PROCESSED_FLOATZ 
		MTL_UPDATE_RARELY, // RAW_FLOATZ 
		MTL_UPDATE_RARELY, // HALF_PARTICLES 
		MTL_UPDATE_RARELY, // HALF_PARTICLES_Z 
		MTL_UPDATE_PER_OBJECT, // CASE_TEXTURE 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_Y 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_CR 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_CB 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_A 
		MTL_UPDATE_CUSTOM, // REFLECTION_PROBE 
		MTL_UPDATE_RARELY, // ALTERNATE_SCENE 
	};

	static_assert(ARRAYSIZE(codeSamplerUpdateFrequency) == TEXTURE_SRC_CODE_COUNT);

	struct Material
	{
		game::native::MaterialInfo info;
		unsigned char stateBitsEntry[48];
		unsigned char textureCount;
		unsigned char constantCount;
		unsigned char stateBitsCount;
		unsigned char stateFlags;
		unsigned char cameraRegion; // GfxCameraRegionType
		struct MaterialTechniqueSet* techniqueSet;
		struct MaterialTextureDef* textureTable;
		game::native::MaterialConstantDef* constantTable;
		game::native::GfxStateBits* stateBitsTable;
	};


	struct MaterialVertexStreamRouting
	{
		game::native::MaterialStreamRouting data[13];
		void* decl[16];
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	};

	enum ShaderCodeConstants : unsigned short
	{
		CONST_SRC_CODE_LIGHT_POSITION = 0x0,
		CONST_SRC_CODE_LIGHT_DIFFUSE = 0x1,
		CONST_SRC_CODE_LIGHT_SPECULAR = 0x2,
		CONST_SRC_CODE_LIGHT_SPOTDIR = 0x3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS = 0x4,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT = 0x5,
		CONST_SRC_CODE_PARTICLE_CLOUD_COLOR = 0x6,
		CONST_SRC_CODE_GAMETIME = 0x7,
		CONST_SRC_CODE_PIXEL_COST_FRACS = 0x8,
		CONST_SRC_CODE_PIXEL_COST_DECODE = 0x9,
		CONST_SRC_CODE_FILTER_TAP_0 = 0xA,
		CONST_SRC_CODE_FILTER_TAP_1 = 0xB,
		CONST_SRC_CODE_FILTER_TAP_2 = 0xC,
		CONST_SRC_CODE_FILTER_TAP_3 = 0xD,
		CONST_SRC_CODE_FILTER_TAP_4 = 0xE,
		CONST_SRC_CODE_FILTER_TAP_5 = 0xF,
		CONST_SRC_CODE_FILTER_TAP_6 = 0x10,
		CONST_SRC_CODE_FILTER_TAP_7 = 0x11,
		CONST_SRC_CODE_COLOR_MATRIX_R = 0x12,
		CONST_SRC_CODE_COLOR_MATRIX_G = 0x13,
		CONST_SRC_CODE_COLOR_MATRIX_B = 0x14,
		CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET = 0x15,
		CONST_SRC_CODE_RENDER_TARGET_SIZE = 0x16,
		CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR = 0x17,
		CONST_SRC_CODE_DOF_EQUATION_SCENE = 0x18,
		CONST_SRC_CODE_DOF_LERP_SCALE = 0x19,
		CONST_SRC_CODE_DOF_LERP_BIAS = 0x1A,
		CONST_SRC_CODE_DOF_ROW_DELTA = 0x1B,
		CONST_SRC_CODE_MOTION_MATRIX_X = 0x1C,
		CONST_SRC_CODE_MOTION_MATRIX_Y = 0x1D,
		CONST_SRC_CODE_MOTION_MATRIX_W = 0x1E,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x1F,
		CONST_SRC_CODE_SHADOWMAP_SCALE = 0x20,
		CONST_SRC_CODE_ZNEAR = 0x21,
		CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x22,
		CONST_SRC_CODE_DEBUG_BUMPMAP = 0x23,
		CONST_SRC_CODE_MATERIAL_COLOR = 0x24,
		CONST_SRC_CODE_FOG = 0x25,
		CONST_SRC_CODE_FOG_COLOR_LINEAR = 0x26,
		CONST_SRC_CODE_FOG_COLOR_GAMMA = 0x27,
		CONST_SRC_CODE_FOG_SUN_CONSTS = 0x28,
		CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR = 0x29,
		CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA = 0x2A,
		CONST_SRC_CODE_FOG_SUN_DIR = 0x2B,
		CONST_SRC_CODE_GLOW_SETUP = 0x2C,
		CONST_SRC_CODE_GLOW_APPLY = 0x2D,
		CONST_SRC_CODE_COLOR_BIAS = 0x2E,
		CONST_SRC_CODE_COLOR_TINT_BASE = 0x2F,
		CONST_SRC_CODE_COLOR_TINT_DELTA = 0x30,
		CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA = 0x31,
		CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x32,
		CONST_SRC_CODE_ENVMAP_PARMS = 0x33,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST = 0x34,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x35,
		CONST_SRC_CODE_COMPOSITE_FX_DISTORTION = 0x36,
		CONST_SRC_CODE_POSTFX_FADE_EFFECT = 0x37,
		CONST_SRC_CODE_VIEWPORT_DIMENSIONS = 0x38,
		CONST_SRC_CODE_FRAMEBUFFER_READ = 0x39,
		CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x3A,
		CONST_SRC_CODE_LIGHT_PROBE_AMBIENT = 0x3B,
		CONST_SRC_CODE_NEARPLANE_ORG = 0x3C,
		CONST_SRC_CODE_NEARPLANE_DX = 0x3D,
		CONST_SRC_CODE_NEARPLANE_DY = 0x3E,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x3F,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x40,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 = 0x41,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1 = 0x42,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2 = 0x43,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0 = 0x44,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1 = 0x45,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2 = 0x46,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0 = 0x47,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1 = 0x48,
		CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x49,
		CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x4A,
		CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x4B,
		CONST_SRC_CODE_VIEW_MATRIX = 0x4C,
		CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x4D,
		CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x4E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x4F,
		CONST_SRC_CODE_PROJECTION_MATRIX = 0x50,
		CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x51,
		CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x52,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x53,
		CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x54,
		CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x55,
		CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x56,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x57,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x58,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x59,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5B,
		CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5C,
		CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5F,
		CONST_SRC_CODE_WORLD_MATRIX0 = 0x60,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 = 0x61,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 = 0x62,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 = 0x63,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX0 = 0x64,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 = 0x65,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x66,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x67,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x68,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x69,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6B,
		CONST_SRC_CODE_WORLD_MATRIX1 = 0x6C,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX1 = 0x6D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1 = 0x6E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1 = 0x6F,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX1 = 0x70,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1 = 0x71,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x72,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x73,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x74,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x75,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x76,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x77,
		CONST_SRC_CODE_WORLD_MATRIX2 = 0x78,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX2 = 0x79,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2 = 0x7A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2 = 0x7B,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX2 = 0x7C,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2 = 0x7D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7F,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x80,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x81,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x82,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x83,
		CONST_SRC_TOTAL_COUNT = 0x84,
		CONST_SRC_NONE = 0x85,
	};


	static const MaterialUpdateFrequency codeConstUpdateFrequency[]
	{
		MTL_UPDATE_RARELY, // LIGHT_POSITION 
		MTL_UPDATE_RARELY, // LIGHT_DIFFUSE 
		MTL_UPDATE_RARELY, // LIGHT_SPECULAR 
		MTL_UPDATE_RARELY, // LIGHT_SPOTDIR 
		MTL_UPDATE_RARELY, // LIGHT_SPOTFACTORS 
		MTL_UPDATE_RARELY, // LIGHT_FALLOFF_PLACEMENT 
		MTL_UPDATE_RARELY, // PARTICLE_CLOUD_COLOR 
		MTL_UPDATE_RARELY, // GAMETIME 
		MTL_UPDATE_RARELY, // PIXEL_COST_FRACS 
		MTL_UPDATE_RARELY, // PIXEL_COST_DECODE 
		MTL_UPDATE_RARELY, // FILTER_TAP_0 
		MTL_UPDATE_RARELY, // FILTER_TAP_1 
		MTL_UPDATE_RARELY, // FILTER_TAP_2 
		MTL_UPDATE_RARELY, // FILTER_TAP_3 
		MTL_UPDATE_RARELY, // FILTER_TAP_4 
		MTL_UPDATE_RARELY, // FILTER_TAP_5 
		MTL_UPDATE_RARELY, // FILTER_TAP_6 
		MTL_UPDATE_RARELY, // FILTER_TAP_7 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_R 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_G 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_B 
		MTL_UPDATE_RARELY, // SHADOWMAP_POLYGON_OFFSET 
		MTL_UPDATE_RARELY, // RENDER_TARGET_SIZE 
		MTL_UPDATE_RARELY, // DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR 
		MTL_UPDATE_RARELY, // DOF_EQUATION_SCENE 
		MTL_UPDATE_RARELY, // DOF_LERP_SCALE 
		MTL_UPDATE_RARELY, // DOF_LERP_BIAS 
		MTL_UPDATE_RARELY, // DOF_ROW_DELTA 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_X 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_Y 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_W 
		MTL_UPDATE_RARELY, // SHADOWMAP_SWITCH_PARTITION 
		MTL_UPDATE_RARELY, // SHADOWMAP_SCALE 
		MTL_UPDATE_RARELY, // ZNEAR 
		MTL_UPDATE_RARELY, // LIGHTING_LOOKUP_SCALE 
		MTL_UPDATE_RARELY, // DEBUG_BUMPMAP 
		MTL_UPDATE_RARELY, // MATERIAL_COLOR 
		MTL_UPDATE_RARELY, // FOG 
		MTL_UPDATE_RARELY, // FOG_COLOR_LINEAR 
		MTL_UPDATE_RARELY, // FOG_COLOR_GAMMA 
		MTL_UPDATE_RARELY, // FOG_SUN_CONSTS 
		MTL_UPDATE_RARELY, // FOG_SUN_COLOR_LINEAR 
		MTL_UPDATE_RARELY, // FOG_SUN_COLOR_GAMMA 
		MTL_UPDATE_RARELY, // FOG_SUN_DIR 
		MTL_UPDATE_RARELY, // GLOW_SETUP 
		MTL_UPDATE_RARELY, // GLOW_APPLY 
		MTL_UPDATE_RARELY, // COLOR_BIAS 
		MTL_UPDATE_RARELY, // COLOR_TINT_BASE 
		MTL_UPDATE_RARELY, // COLOR_TINT_DELTA 
		MTL_UPDATE_RARELY, // COLOR_TINT_QUADRATIC_DELTA 
		MTL_UPDATE_RARELY, // OUTDOOR_FEATHER_PARMS 
		MTL_UPDATE_RARELY, // ENVMAP_PARMS 
		MTL_UPDATE_RARELY, // SUN_SHADOWMAP_PIXEL_ADJUST 
		MTL_UPDATE_RARELY, // SPOT_SHADOWMAP_PIXEL_ADJUST 
		MTL_UPDATE_RARELY, // COMPOSITE_FX_DISTORTION 
		MTL_UPDATE_RARELY, // POSTFX_FADE_EFFECT 
		MTL_UPDATE_RARELY, // VIEWPORT_DIMENSIONS 
		MTL_UPDATE_RARELY, // FRAMEBUFFER_READ 
		MTL_UPDATE_PER_PRIM, // BASE_LIGHTING_COORDS 
		MTL_UPDATE_PER_PRIM, // LIGHT_PROBE_AMBIENT 
		MTL_UPDATE_RARELY, // NEARPLANE_ORG 
		MTL_UPDATE_RARELY, // NEARPLANE_DX 
		MTL_UPDATE_RARELY, // NEARPLANE_DY 
		MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_SCALE 
		MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_OFFSET 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX1 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX2 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR1 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR2 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM1 
		MTL_UPDATE_PER_OBJECT, // DEPTH_FROM_CLIP 
		MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_0 
		MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_1 
		MTL_UPDATE_PER_OBJECT, // VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
	};

	static_assert(ARRAYSIZE(codeConstUpdateFrequency) == CONST_SRC_TOTAL_COUNT);
	static const char* Debug_ShaderCodeConstantsNames[]
	{
		NAMEOF(CONST_SRC_CODE_LIGHT_POSITION),
		NAMEOF(CONST_SRC_CODE_LIGHT_DIFFUSE),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPECULAR),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPOTDIR),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPOTFACTORS),
		NAMEOF(CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_COLOR),
		NAMEOF(CONST_SRC_CODE_GAMETIME),
		NAMEOF(CONST_SRC_CODE_PIXEL_COST_FRACS),
		NAMEOF(CONST_SRC_CODE_PIXEL_COST_DECODE),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_0),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_1),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_2),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_3),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_4),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_5),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_6),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_7),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_R),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_G),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_B),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET),
		NAMEOF(CONST_SRC_CODE_RENDER_TARGET_SIZE),
		NAMEOF(CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR),
		NAMEOF(CONST_SRC_CODE_DOF_EQUATION_SCENE),
		NAMEOF(CONST_SRC_CODE_DOF_LERP_SCALE),
		NAMEOF(CONST_SRC_CODE_DOF_LERP_BIAS),
		NAMEOF(CONST_SRC_CODE_DOF_ROW_DELTA),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_X),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_Y),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_W),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_SCALE),
		NAMEOF(CONST_SRC_CODE_ZNEAR),
		NAMEOF(CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE),
		NAMEOF(CONST_SRC_CODE_DEBUG_BUMPMAP),
		NAMEOF(CONST_SRC_CODE_MATERIAL_COLOR),
		NAMEOF(CONST_SRC_CODE_FOG),
		NAMEOF(CONST_SRC_CODE_FOG_COLOR_LINEAR),
		NAMEOF(CONST_SRC_CODE_FOG_COLOR_GAMMA),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_CONSTS),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_DIR),
		NAMEOF(CONST_SRC_CODE_GLOW_SETUP),
		NAMEOF(CONST_SRC_CODE_GLOW_APPLY),
		NAMEOF(CONST_SRC_CODE_COLOR_BIAS),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_BASE),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_DELTA),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA),
		NAMEOF(CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS),
		NAMEOF(CONST_SRC_CODE_ENVMAP_PARMS),
		NAMEOF(CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST),
		NAMEOF(CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST),
		NAMEOF(CONST_SRC_CODE_COMPOSITE_FX_DISTORTION),
		NAMEOF(CONST_SRC_CODE_POSTFX_FADE_EFFECT),
		NAMEOF(CONST_SRC_CODE_VIEWPORT_DIMENSIONS),
		NAMEOF(CONST_SRC_CODE_FRAMEBUFFER_READ),
		NAMEOF(CONST_SRC_CODE_BASE_LIGHTING_COORDS),
		NAMEOF(CONST_SRC_CODE_LIGHT_PROBE_AMBIENT),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_ORG),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_DX),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_DY),
		NAMEOF(CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE),
		NAMEOF(CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2),
		NAMEOF(CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1),
		NAMEOF(CONST_SRC_CODE_DEPTH_FROM_CLIP),
		NAMEOF(CONST_SRC_CODE_CODE_MESH_ARG_0),
		NAMEOF(CONST_SRC_CODE_CODE_MESH_ARG_1),
		NAMEOF(CONST_SRC_CODE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2)
	};

	static_assert(ARRAYSIZE(Debug_ShaderCodeConstantsNames) == ShaderCodeConstants::CONST_SRC_TOTAL_COUNT);

	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl;
		game::native::MaterialVertexShader* vertexShader;
		game::native::MaterialPixelShader* pixelShader;
		unsigned char perPrimArgCount;
		unsigned char perObjArgCount;
		unsigned char stableArgCount;
		unsigned char customSamplerFlags;
		game::native::MaterialShaderArgument* args;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned short flags;
		unsigned short passCount;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		char worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		MaterialTechnique* techniques[48];
	};

	union FxEffectDefRef
	{
		struct FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[2];
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	union FxElemExtendedDefPtr
	{
		game::native::FxTrailDef* trailDef;
		game::native::FxSparkFountainDef* sparkFountainDef;
		char* unknownDef;
	};

	struct FxElemDef
	{
		int flags;
		game::native::FxSpawnDef spawn;
		game::native::FxFloatRange spawnRange;
		game::native::FxFloatRange fadeInRange;
		game::native::FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		game::native::FxIntRange spawnDelayMsec;
		game::native::FxIntRange lifeSpanMsec;
		game::native::FxFloatRange spawnOrigin[3];
		game::native::FxFloatRange spawnOffsetRadius;
		game::native::FxFloatRange spawnOffsetHeight;
		game::native::FxFloatRange spawnAngles[3];
		game::native::FxFloatRange angularVelocity[3];
		game::native::FxFloatRange initialRotation;
		game::native::FxFloatRange gravity;
		game::native::FxFloatRange reflectionFactor;
		game::native::FxElemAtlas atlas;
		unsigned char elemType;
		unsigned char visualCount;
		unsigned char velIntervalCount;
		unsigned char visStateIntervalCount;
		game::native::FxElemVelStateSample* velSamples;
		game::native::FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		game::native::Bounds collBounds;
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		game::native::FxFloatRange emitDist;
		game::native::FxFloatRange emitDistVariance;
		FxElemExtendedDefPtr extended;
		unsigned char sortOrder;
		unsigned char lightingFrac;
		unsigned char useItemClip;
		unsigned char fadeInfo;
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		FxElemDef* elemDefs;
	};

	struct FxGlassDef
	{
		float halfThickness;
		float texVecs[2][2];
		game::native::GfxColor color;
		Material* material;
		Material* materialShattered;
		PhysPreset* physPreset;
	};

	struct FxGlassSystem
	{
		int time;
		int prevTime;
		unsigned int defCount;
		unsigned int pieceLimit;
		unsigned int pieceWordCount;
		unsigned int initPieceCount;
		unsigned int cellCount;
		unsigned int activePieceCount;
		unsigned int firstFreePiece;
		unsigned int geoDataLimit;
		unsigned int geoDataCount;
		unsigned int initGeoDataCount;
		FxGlassDef* defs;
		game::native::FxGlassPiecePlace* piecePlaces;
		game::native::FxGlassPieceState* pieceStates;
		game::native::FxGlassPieceDynamics* pieceDynamics;
		game::native::FxGlassGeometryData* geoData;
		unsigned int* isInUse;
		unsigned int* cellBits;
		unsigned char* visData;
		float(*linkOrg)[3];
		float* halfThickness;
		unsigned __int16* lightingHandles;
		game::native::FxGlassInitPieceState* initPieceStates;
		game::native::FxGlassGeometryData* initGeoData;
		bool needToCompactData;
		char initCount;
		float effectChanceAccum;
		int lastPieceDeletionTime;
	};

	struct FxWorld
	{
		const char* name;
		FxGlassSystem glassSys;
	};



	struct _AILSOUNDINFO
	{
		int format;
		const void* data_ptr;
		unsigned int data_len;
		unsigned int rate;
		int bits;
		int channels;
		unsigned int samples;
		unsigned int block_size;
		const void* initial_ptr;
	};

	struct SndCurve
	{
		const char* filename;
		unsigned __int16 knotCount;
		float knots[16][2];
	};

	const struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		const char* mixerGroup;
		game::native::SoundFile* soundFile;
		int32_t sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		float velocityMin;
		SoundAliasFlags flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int32_t startDelay;
		SndCurve* volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		game::native::SpeakerMap* speakerMap;
	};

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		unsigned int count;
	};

	struct TracerDef
	{
		const char* name;
		Material* material;
		unsigned int drawInterval;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		float colors[5][4];
	};

	enum weapType_t
	{
		WEAPTYPE_BULLET = 0x0,
		WEAPTYPE_GRENADE = 0x1,
		WEAPTYPE_PROJECTILE = 0x2,
		WEAPTYPE_RIOTSHIELD = 0x3,
		WEAPTYPE_NUM = 0x4,
	};

	struct __declspec(align(4)) WeaponDef
	{
		const char* szOverlayName;
		XModel** gunXModel;
		XModel* handXModel;
		const char** szXAnimsRightHanded;
		const char** szXAnimsLeftHanded;
		const char* szModeName;
		unsigned __int16* notetrackSoundMapKeys;
		unsigned __int16* notetrackSoundMapValues;
		unsigned __int16* notetrackRumbleMapKeys;
		unsigned __int16* notetrackRumbleMapValues;
		int playerAnimType;
		weapType_t weapType;
		game::native::weapClass_t weapClass;
		game::native::PenetrateType penetrateType;
		game::native::weapInventoryType_t inventoryType;
		game::native::weapFireType_t fireType;
		game::native::OffhandClass offhandClass;
		game::native::weapStance_t stance;
		FxEffectDef* viewFlashEffect;
		FxEffectDef* worldFlashEffect;
		snd_alias_list_t* pickupSound;
		snd_alias_list_t* pickupSoundPlayer;
		snd_alias_list_t* ammoPickupSound;
		snd_alias_list_t* ammoPickupSoundPlayer;
		snd_alias_list_t* projectileSound;
		snd_alias_list_t* pullbackSound;
		snd_alias_list_t* pullbackSoundPlayer;
		snd_alias_list_t* fireSound;
		snd_alias_list_t* fireSoundPlayer;
		snd_alias_list_t* fireSoundPlayerAkimbo;
		snd_alias_list_t* fireLoopSound;
		snd_alias_list_t* fireLoopSoundPlayer;
		snd_alias_list_t* fireStopSound;
		snd_alias_list_t* fireStopSoundPlayer;
		snd_alias_list_t* fireLastSound;
		snd_alias_list_t* fireLastSoundPlayer;
		snd_alias_list_t* emptyFireSound;
		snd_alias_list_t* emptyFireSoundPlayer;
		snd_alias_list_t* meleeSwipeSound;
		snd_alias_list_t* meleeSwipeSoundPlayer;
		snd_alias_list_t* meleeHitSound;
		snd_alias_list_t* meleeMissSound;
		snd_alias_list_t* rechamberSound;
		snd_alias_list_t* rechamberSoundPlayer;
		snd_alias_list_t* reloadSound;
		snd_alias_list_t* reloadSoundPlayer;
		snd_alias_list_t* reloadEmptySound;
		snd_alias_list_t* reloadEmptySoundPlayer;
		snd_alias_list_t* reloadStartSound;
		snd_alias_list_t* reloadStartSoundPlayer;
		snd_alias_list_t* reloadEndSound;
		snd_alias_list_t* reloadEndSoundPlayer;
		snd_alias_list_t* detonateSound;
		snd_alias_list_t* detonateSoundPlayer;
		snd_alias_list_t* nightVisionWearSound;
		snd_alias_list_t* nightVisionWearSoundPlayer;
		snd_alias_list_t* nightVisionRemoveSound;
		snd_alias_list_t* nightVisionRemoveSoundPlayer;
		snd_alias_list_t* altSwitchSound;
		snd_alias_list_t* altSwitchSoundPlayer;
		snd_alias_list_t* raiseSound;
		snd_alias_list_t* raiseSoundPlayer;
		snd_alias_list_t* firstRaiseSound;
		snd_alias_list_t* firstRaiseSoundPlayer;
		snd_alias_list_t* putawaySound;
		snd_alias_list_t* putawaySoundPlayer;
		snd_alias_list_t* scanSound;
		snd_alias_list_t** bounceSound;
		FxEffectDef* viewShellEjectEffect;
		FxEffectDef* worldShellEjectEffect;
		FxEffectDef* viewLastShotEjectEffect;
		FxEffectDef* worldLastShotEjectEffect;
		Material* reticleCenter;
		Material* reticleSide;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		game::native::activeReticleType_t activeReticleType;
		float vStandMove[3];
		float vStandRot[3];
		float strafeMove[3];
		float strafeRot[3];
		float vDuckedOfs[3];
		float vDuckedMove[3];
		float vDuckedRot[3];
		float vProneOfs[3];
		float vProneMove[3];
		float vProneRot[3];
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		XModel** worldModel;
		XModel* worldClipModel;
		XModel* rocketModel;
		XModel* knifeModel;
		XModel* worldKnifeModel;
		Material* hudIcon;
		game::native::weaponIconRatioType_t hudIconRatio;
		Material* pickupIcon;
		game::native::weaponIconRatioType_t pickupIconRatio;
		Material* ammoCounterIcon;
		game::native::weaponIconRatioType_t ammoCounterIconRatio;
		game::native::ammoCounterClipType_t ammoCounterClip;
		int iStartAmmo;
		const char* szAmmoName;
		int iAmmoIndex;
		const char* szClipName;
		int iClipIndex;
		int iMaxAmmo;
		int shotCount;
		const char* szSharedAmmoCapName;
		int iSharedAmmoCapIndex;
		int iSharedAmmoCap;
		int damage;
		int playerDamage;
		int iMeleeDamage;
		int iDamageType;
		int iFireDelay;
		int iMeleeDelay;
		int meleeChargeDelay;
		int iDetonateDelay;
		int iRechamberTime;
		int rechamberTimeOneHanded;
		int iRechamberBoltTime;
		int iHoldFireTime;
		int iDetonateTime;
		int iMeleeTime;
		int meleeChargeTime;
		int iReloadTime;
		int reloadShowRocketTime;
		int iReloadEmptyTime;
		int iReloadAddTime;
		int iReloadStartTime;
		int iReloadStartAddTime;
		int iReloadEndTime;
		int iDropTime;
		int iRaiseTime;
		int iAltDropTime;
		int quickDropTime;
		int quickRaiseTime;
		int iBreachRaiseTime;
		int iEmptyRaiseTime;
		int iEmptyDropTime;
		int sprintInTime;
		int sprintLoopTime;
		int sprintOutTime;
		int stunnedTimeBegin;
		int stunnedTimeLoop;
		int stunnedTimeEnd;
		int nightVisionWearTime;
		int nightVisionWearTimeFadeOutEnd;
		int nightVisionWearTimePowerUp;
		int nightVisionRemoveTime;
		int nightVisionRemoveTimePowerDown;
		int nightVisionRemoveTimeFadeInStart;
		int fuseTime;
		int aiFuseTime;
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimPadding;
		float enemyCrosshairRange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		float fAdsZoomInFrac;
		float fAdsZoomOutFrac;
		Material* overlayMaterial;
		Material* overlayMaterialLowRes;
		Material* overlayMaterialEMP;
		Material* overlayMaterialEMPLowRes;
		game::native::weapOverlayReticle_t overlayReticle;
		game::native::WeapOverlayInteface_t overlayInterface;
		float overlayWidth;
		float overlayHeight;
		float overlayWidthSplitscreen;
		float overlayHeightSplitscreen;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float fHipReticleSidePos;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSwayPitchScale;
		float adsSwayYawScale;
		float adsSwayHorizScale;
		float adsSwayVertScale;
		float adsViewErrorMin;
		float adsViewErrorMax;
		game::native::PhysCollmap* physCollmap;
		float dualWieldViewModelOffset;
		game::native::weaponIconRatioType_t killIconRatio;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		int ammoDropStockMin;
		int ammoDropClipPercentMin;
		int ammoDropClipPercentMax;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		float bulletExplDmgMult;
		float bulletExplRadiusMult;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedForward;
		int iProjectileActivateDist;
		float projLifetime;
		float timeToAccelerate;
		float projectileCurvature;
		XModel* projectileModel;
		game::native::weapProjExposion_t projExplosion;
		FxEffectDef* projExplosionEffect;
		FxEffectDef* projDudEffect;
		snd_alias_list_t* projExplosionSound;
		snd_alias_list_t* projDudSound;
		game::native::WeapStickinessType stickiness;
		float lowAmmoWarningThreshold;
		float ricochetChance;
		float* parallelBounce;
		float* perpendicularBounce;
		FxEffectDef* projTrailEffect;
		FxEffectDef* projBeaconEffect;
		float vProjectileColor[3];
		game::native::guidedMissileType_t guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		FxEffectDef* projIgnitionEffect;
		snd_alias_list_t* projIgnitionSound;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float fightDist;
		float maxDist;
		const char* accuracyGraphName[2];
		float(*originalAccuracyGraphKnots[2])[2];
		unsigned __int16 originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char* szUseHintString;
		const char* dropHintString;
		int iUseHintStringIndex;
		int dropHintStringIndex;
		float horizViewJitter;
		float vertViewJitter;
		float scanSpeed;
		float scanAccel;
		int scanPauseTime;
		const char* szScript;
		float fOOPosAnimLength[2];
		int minDamage;
		int minPlayerDamage;
		float fMaxDamageRange;
		float fMinDamageRange;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float* locationDamageMultipliers;
		const char* fireRumble;
		const char* meleeImpactRumble;
		TracerDef* tracerType;
		float turretScopeZoomRate;
		float turretScopeZoomMin;
		float turretScopeZoomMax;
		float turretOverheatUpRate;
		float turretOverheatDownRate;
		float turretOverheatPenalty;
		snd_alias_list_t* turretOverheatSound;
		FxEffectDef* turretOverheatEffect;
		const char* turretBarrelSpinRumble;
		float turretBarrelSpinSpeed;
		float turretBarrelSpinUpTime;
		float turretBarrelSpinDownTime;
		snd_alias_list_t* turretBarrelSpinMaxSnd;
		snd_alias_list_t* turretBarrelSpinUpSnd[4];
		snd_alias_list_t* turretBarrelSpinDownSnd[4];
		snd_alias_list_t* missileConeSoundAlias;
		snd_alias_list_t* missileConeSoundAliasAtBase;
		float missileConeSoundRadiusAtTop;
		float missileConeSoundRadiusAtBase;
		float missileConeSoundHeight;
		float missileConeSoundOriginOffset;
		float missileConeSoundVolumescaleAtCore;
		float missileConeSoundVolumescaleAtEdge;
		float missileConeSoundVolumescaleCoreSize;
		float missileConeSoundPitchAtTop;
		float missileConeSoundPitchAtBottom;
		float missileConeSoundPitchTopSize;
		float missileConeSoundPitchBottomSize;
		float missileConeSoundCrossfadeTopSize;
		float missileConeSoundCrossfadeBottomSize;
		bool sharedAmmo;
		bool lockonSupported;
		bool requireLockonToFire;
		bool bigExplosion;
		bool noAdsWhenMagEmpty;
		bool avoidDropCleanup;
		bool inheritsPerks;
		bool crosshairColorChange;
		bool bRifleBullet;
		bool armorPiercing;
		bool bBoltAction;
		bool aimDownSight;
		bool bRechamberWhileAds;
		bool bBulletExplosiveDamage;
		bool bCookOffHold;
		bool bClipOnly;
		bool noAmmoPickup;
		bool adsFireOnly;
		bool cancelAutoHolsterWhenEmpty;
		bool disableSwitchToWhenEmpty;
		bool suppressAmmoReserveDisplay;
		bool laserSightDuringNightvision;
		bool markableViewmodel;
		bool noDualWield;
		bool flipKillIcon;
		bool bNoPartialReload;
		bool bSegmentedReload;
		bool blocksProne;
		bool silenced;
		bool isRollingGrenade;
		bool projExplosionEffectForceNormalUp;
		bool bProjImpactExplode;
		bool stickToPlayers;
		bool hasDetonator;
		bool disableFiring;
		bool timedDetonation;
		bool rotate;
		bool holdButtonToThrow;
		bool freezeMovementWhenFiring;
		bool thermalScope;
		bool altModeSameWeapon;
		bool turretBarrelSpinEnabled;
		bool missileConeSoundEnabled;
		bool missileConeSoundPitchshiftEnabled;
		bool missileConeSoundCrossfadeEnabled;
		bool offhandHoldIsCancelable;
	};

	/* 1281 */
	struct WeaponCompleteDef
	{
		const char* szInternalName;
		WeaponDef* weapDef;
		const char* szDisplayName;
		unsigned __int16* hideTags;
		const char** szXAnims;
		float fAdsZoomFov;
		int iAdsTransInTime;
		int iAdsTransOutTime;
		int iClipSize;
		game::native::ImpactType impactType;
		int iFireTime;
		game::native::weaponIconRatioType_t dpadIconRatio;
		float penetrateMultiplier;
		float fAdsViewKickCenterSpeed;
		float fHipViewKickCenterSpeed;
		const char* szAltWeaponName;
		unsigned int altWeaponIndex;
		int iAltRaiseTime;
		Material* killIcon;
		Material* dpadIcon;
		int fireAnimLength;
		int iFirstRaiseTime;
		int ammoDropStockMax;
		float adsDofStart;
		float adsDofEnd;
		unsigned __int16 accuracyGraphKnotCount[2];
		float(*accuracyGraphKnots[2])[2];
		bool motionTracker;
		bool enhanced;
		bool dpadIconShowsAmmo;
	};


	union XAssetHeader
	{
		void* data;
		PhysPreset* physPreset;
		game::native::PhysCollmap* physCollmap;
		game::native::XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		Material* material;
		game::native::MaterialPixelShader* pixelShader;
		game::native::MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		SndCurve* sndCurve;
		game::native::LoadedSound* loadSnd;
		clipMap_t* clipMap;
		ComWorld* comWorld;
		//GameWorldSp* gameWorldSp;
		game::native::GlassWorld* gameWorldMp;
		MapEnts* mapEnts;
		FxWorld* fxWorld;
		GfxWorld* gfxWorld;
		GfxLightDef* lightDef;
		//Font_s* font;
		//MenuList* menuList;
		//menuDef_t* menu;
		//LocalizeEntry* localize;
		WeaponCompleteDef* weapon;
		//SndDriverGlobals* sndDriverGlobals;
		FxEffectDef* fx;
		//FxImpactTable* impactFx;
		game::native::RawFile* rawfile;
		//StringTable* stringTable;
		//LeaderboardDef* leaderboardDef;
		//StructuredDataDefSet* structuredDataDefSet;
		TracerDef* tracerDef;
		//VehicleDef* vehDef;
		//AddonMapEnts* addonMapEnts;
	};
}