#pragma once

#include <game\structs.hpp>

namespace iw4::native
{
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

	enum XAnimPartType
	{
		PART_TYPE_NO_QUAT = 0x0,
		PART_TYPE_HALF_QUAT = 0x1,
		PART_TYPE_FULL_QUAT = 0x2,
		PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
		PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
		PART_TYPE_SMALL_TRANS = 0x5,
		PART_TYPE_TRANS = 0x6,
		PART_TYPE_TRANS_NO_SIZE = 0x7,
		PART_TYPE_NO_TRANS = 0x8,
		PART_TYPE_ALL = 0x9,
	};

	union XAnimIndices
	{
		char* _1;
		unsigned short* _2;
		void* data;
	};

	struct XAnimNotifyInfo
	{
		unsigned __int16 name;
		float time;
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned __int16(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		char _1[1];
		unsigned __int16 _2[1];
	};

	struct XAnimPartTransFrames
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		float frame0[3];
	};

	struct XAnimPartTrans
	{
		unsigned __int16 size;
		char smallTrans;
		XAnimPartTransData u;
	};

	struct XAnimDeltaPartQuatDataFrames2
	{
		__int16* frames;
		char indices[1];
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		__int16 frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData2 u;
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16* frames;
		char indices[1];
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		__int16 frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat2* quat2;
		XAnimDeltaPartQuat* quat;
	};

	struct XAnimParts
	{
		const char* name; // 0
		unsigned short dataByteCount; // 4
		unsigned short dataShortCount; // 6
		unsigned short dataIntCount; // 8
		unsigned short randomDataByteCount; // 10 - 0xA
		unsigned short randomDataIntCount;// 12 - 0xC
		unsigned short framecount; // 14 - 0xE
		char bLoop; // 16
		char boneCount[10]; // 17
		char notetrackCount; // 27
		char pad1; // 28
		char bDelta; // 29
		char assetType; // 30
		char pad2; // 31
		int randomDataShortCount; // 32 - 0x20
		int indexcount; // 36 - 0x24
		float framerate; // 40 - 0x28
		float frequency; // 44 - 0x2C
		short* tagnames; // 48 - 0x30
		char* dataByte;// 52 - 0x34
		short* dataShort; // 56 - 0x38
		int* dataInt; // 60 - 0x3C
		short* randomDataShort; // 64 - 0x40
		char* randomDataByte; // 68 - 0x44
		int* randomDataInt; // 72 - 0x48
		XAnimIndices indices; // 76 - 0x4C
		XAnimNotifyInfo* notetracks; // 80 - 0x50
		XAnimDeltaPart* delta; // 84 - 0x54
								// 88 - 0x58
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
		short(*quats)[4];
		float(*trans)[3];
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
		game::native::cbrushedge_t* brushEdges;
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
		TS_WATER_MAP = 0xB,
		TS_DISPLACEMENT_MAP = 0xC
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
		int knotCount;
		float knots[8][2];
	};

	const struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		game::native::SoundFile* soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		SoundAliasFlags flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
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

	union XAssetHeader
	{
		void* data;
		PhysPreset* physPreset;
		game::native::PhysCollmap* physCollmap;
		XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		Material* material;
		game::native::MaterialPixelShader* pixelShader;
		game::native::MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		//SndCurve* sndCurve;
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
		//WeaponCompleteDef* weapon;
		//SndDriverGlobals* sndDriverGlobals;
		FxEffectDef* fx;
		//FxImpactTable* impactFx;
		game::native::RawFile* rawfile;
		//StringTable* stringTable;
		//LeaderboardDef* leaderboardDef;
		//StructuredDataDefSet* structuredDataDefSet;
		//TracerDef* tracerDef;
		//VehicleDef* vehDef;
		//AddonMapEnts* addonMapEnts;
	};
}