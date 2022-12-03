#pragma once

#include "structs.hpp"
#include "launcher/launcher.hpp"

#define SELECT_VALUE(sp, mp, dedi) (game::is_sp() ? (sp) : (game::is_mp() ? (mp) : (dedi)))

#define SERVER_CD_KEY "Open-IW5-CD-Key"

namespace game
{
	namespace native
	{
		typedef void (*Cmd_AddCommand_t)(const char* cmdName, void (*function)(), cmd_function_t* allocedCmd);
		extern Cmd_AddCommand_t Cmd_AddCommand;

		typedef void (*Cmd_RemoveCommand_t)(const char* cmdName);
		extern Cmd_RemoveCommand_t Cmd_RemoveCommand;

		typedef void (*Com_Error_t)(errorParm_t code, const char* fmt, ...);
		extern Com_Error_t Com_Error;

		typedef bool (*Com_Filter_t)(const char* filter, const char* name, int casesensitive);
		extern Com_Filter_t Com_Filter;

		typedef void (*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zoneCount, int sync);
		extern DB_LoadXAssets_t DB_LoadXAssets;

		typedef XAssetHeader (*DB_FindXAssetHeader_t)(XAssetType type, const char* name, int allowCreateDefault);
		extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

		typedef const dvar_t* (*Dvar_RegisterBool_t)(const char* dvarName, bool value,
			unsigned __int16 flags, const char* description);
		extern Dvar_RegisterBool_t Dvar_RegisterBool;

		typedef const dvar_t* (*Dvar_RegisterString_t)(const char* dvarName, const char* value,
			unsigned __int16 flags, const char* description);
		extern Dvar_RegisterString_t Dvar_RegisterString;

		typedef void (*Dvar_SetIntByName_t)(const char* dvarName, int value);
		extern Dvar_SetIntByName_t Dvar_SetIntByName;

		typedef void (*Dvar_SetFromStringByName_t)(const char* dvarName, const char* string);
		extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

		typedef void (*Dvar_SetString_t)(const dvar_t* dvar, const char* value);
		extern Dvar_SetString_t Dvar_SetString;

		typedef void (*Dvar_ForEach_t)(void (*callback)(const dvar_t*, void*), void* userData);
		extern Dvar_ForEach_t Dvar_ForEach;

		typedef const char* (*Dvar_DisplayableValue_t)(const dvar_t* dvar);
		extern Dvar_DisplayableValue_t Dvar_DisplayableValue;

		typedef const char* (*Dvar_DisplayableLatchedValue_t)(const dvar_t* dvar);
		extern Dvar_DisplayableLatchedValue_t Dvar_DisplayableLatchedValue;

		typedef int (*G_RunFrame_t)(int, int);
		extern G_RunFrame_t G_RunFrame;

		typedef Weapon (*G_GetWeaponForName_t)(const char* name);
		extern G_GetWeaponForName_t G_GetWeaponForName;

		typedef void (*MSG_ReadData_t)(msg_t* msg, void* data, int len);
		extern MSG_ReadData_t MSG_ReadData;

		typedef void* (*MT_AllocIndex_t)(int numBytes, int type);
		extern MT_AllocIndex_t MT_AllocIndex;

		typedef void (*RemoveRefToValue_t)(scriptType_e type, VariableUnion u);
		extern RemoveRefToValue_t RemoveRefToValue;

		typedef unsigned int (*SL_GetStringOfSize_t)(const char* str, unsigned int user, unsigned int len, int type);
		extern SL_GetStringOfSize_t SL_GetStringOfSize;

		typedef void (*Scr_AddEntityNum_t)(int entnum, unsigned int classnum);
		extern Scr_AddEntityNum_t Scr_AddEntityNum;

		typedef void (*Scr_Notify_t)(gentity_s* ent, scr_string_t, unsigned int paramcount);
		extern Scr_Notify_t Scr_Notify;

		typedef void (*Scr_NotifyLevel_t)(unsigned int stringValue, unsigned int paramcount);
		extern Scr_NotifyLevel_t Scr_NotifyLevel;

		typedef unsigned int (*Scr_GetNumParam_t)();
		extern Scr_GetNumParam_t Scr_GetNumParam;

		typedef const char* (*Scr_GetString_t)(unsigned int index);
		extern Scr_GetString_t Scr_GetString;

		typedef void (*Sys_ShowConsole_t)();
		extern Sys_ShowConsole_t Sys_ShowConsole;

		typedef void (*Sys_Error_t)(const char* error, ...);
		extern Sys_Error_t Sys_Error;

		typedef int (*Sys_Milliseconds_t)();
		extern Sys_Milliseconds_t Sys_Milliseconds;

		typedef void (*VM_Notify_t)(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue* top);
		extern VM_Notify_t VM_Notify;

		typedef unsigned int (*BG_NetDataChecksum_t)();
		extern BG_NetDataChecksum_t BG_NetDataChecksum;

		typedef int (*LiveStorage_GetPersistentDataDefVersion_t)();
		extern LiveStorage_GetPersistentDataDefVersion_t LiveStorage_GetPersistentDataDefVersion;

		typedef unsigned int (*LiveStorage_GetPersistentDataDefFormatChecksum_t)();
		extern LiveStorage_GetPersistentDataDefFormatChecksum_t LiveStorage_GetPersistentDataDefFormatChecksum;

		typedef void (*SV_DirectConnect_t)(netadr_s from);
		extern SV_DirectConnect_t SV_DirectConnect;

		typedef void (*SV_ClientEnterWorld_t)(mp::client_t* client, usercmd_s* cmd);
		extern SV_ClientEnterWorld_t SV_ClientEnterWorld;

		typedef void (*SV_Cmd_TokenizeString_t)(const char* text_in);
		extern SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString;

		typedef void (*SV_Cmd_EndTokenizedString_t)();
		extern SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString;

		typedef void (*XUIDToString_t)(const unsigned __int64* xuid, char* str);
		extern XUIDToString_t XUIDToString;

		typedef char* (*SEH_LocalizeTextMessage_t)(const char* pszInputBuffer, const char* pszMessageType, msgLocErrType_t errType);
		extern SEH_LocalizeTextMessage_t SEH_LocalizeTextMessage;

		typedef void (*CM_TransformedCapsuleTrace_t)(trace_t* results, const float* start, const float* end,
			const Bounds* bounds, const Bounds* capsule, int contents,
			const float* origin, const float* angles);
		extern CM_TransformedCapsuleTrace_t CM_TransformedCapsuleTrace;

		typedef void (*PM_WeaponUseAmmo_t)(playerState_s* ps, const Weapon weapon, bool isAlternate, int amount, PlayerHandIndex hand);
		extern PM_WeaponUseAmmo_t PM_WeaponUseAmmo;

		typedef void (*PM_playerTrace_t)(pmove_t* pm, trace_t* results, const float* start, const float* end,
			const Bounds* bounds, int passEntityNum, int contentMask);
		extern PM_playerTrace_t PM_playerTrace;

		typedef void (*PM_trace_t)(const pmove_t* pm, trace_t* results, const float* start, const float* end,
			const Bounds* bounds, int passEntityNum, int contentMask);
		extern PM_trace_t PM_trace;

		typedef void (*Jump_ClearState_t)(playerState_s* ps);
		extern Jump_ClearState_t Jump_ClearState;

		typedef void (*Vec3Normalize_t)(float* v);
		extern Vec3Normalize_t Vec3Normalize;

		typedef void (*Cmd_ExecuteSingleCommand_t)(LocalClientNum_t localClientNum, int controllerIndex, const char* text);
		extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

		typedef void (*Com_Quit_f_t)();
		extern Com_Quit_f_t Com_Quit_f;

		typedef void (*FS_Printf_t)(int h, const char* fmt, ...);
		extern FS_Printf_t FS_Printf;

		typedef void (*player_die_t)(gentity_s* self, const gentity_s* inflictor, gentity_s* attacker, int damage, int meansOfDeath, const Weapon* iWeapon, bool isAlternate, const float* vDir, const hitLocation_t hitLoc, int psTimeOffset);
		extern player_die_t player_die;

		extern decltype(longjmp)* _longjmp;

		constexpr auto CMD_MAX_NESTING = 8;
		extern CmdArgs* sv_cmd_args;
		extern CmdArgs* cmd_args;

		extern short* scrVarGlob;
		extern char** scrMemTreePub;
		extern char* scrMemTreeGlob;

		extern scrVarPub_t* scr_VarPub;
		extern scrVmPub_t* scr_VmPub;

		extern unsigned int* levelEntityId;

		extern int* g_script_error_level;
		extern jmp_buf* g_script_error;

		extern scr_classStruct_t* g_classMap;

		extern int* svs_clientCount;

		constexpr auto MAX_GENTITIES = 2048u;
		constexpr auto ENTITYNUM_NONE = MAX_GENTITIES - 1u;
		extern gentity_s* g_entities;

		extern level_locals_t* level;

		extern DeferredQueue* deferredQueue;

		extern float* com_codeTimeScale;

		extern RTL_CRITICAL_SECTION* s_criticalSection;

		extern int* logfile;

		extern searchpath_s** fs_searchpaths;
		extern char* fs_gamedir;
		extern fileHandleData_t* fsh;
		extern int* com_fileAccessed;

		extern unsigned int(*threadId)[THREAD_CONTEXT_COUNT];

		extern int* initialized_0;
		extern int* sys_timeBase;
		extern unsigned __int64* sys_counterBase;

		extern int* dvarCount;
		extern dvar_t** sortedDvars;

		// Global Definitions & Functions
		constexpr auto JUMP_LAND_SLOWDOWN_TIME = 1800;

		constexpr auto MAX_QPATH = 64;
		constexpr auto MAX_OSPATH = 256;

		// From Quake III, to match game's assembly
		template <typename T, typename R>
		constexpr auto VectorScale(T v, R s, T out) { out[0] = v[0] * s; out[1] = v[1] * s; out[2] = v[2] * s; }

		int Vec4Compare(const float* a, const float* b);

		namespace mp
		{
			typedef char* (*SV_GetGuid_t)(int clientNum);
			extern SV_GetGuid_t SV_GetGuid;

			typedef void (*SV_GameSendServerCommand_t)(int clientNum, svscmd_type type, const char* text);
			extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

			extern client_t* svs_clients;
		}

		namespace dedi
		{
			extern client_t* svs_clients;
		}

		namespace sp
		{
			typedef bool (*IsServerRunning_t)();
			extern IsServerRunning_t IsServerRunning;

			typedef void (*SV_GameSendServerCommand_t)(int clientNum, const char* text);
			extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

			typedef void (*TeleportPlayer_t)(gentity_s* player, float* origin, float* angles);
			extern TeleportPlayer_t TeleportPlayer;

			extern gentity_s* g_entities;

			extern gclient_s* g_clients;
		}

		void AddRefToValue(VariableValue* value);

		void Conbuf_AppendText(const char* message);

		unsigned int FindVariable(unsigned int parentId, unsigned int name);

		VariableValue GetEntityFieldValue(unsigned int classnum, int entnum, int offset);

		void* MT_Alloc(int numBytes, int type);

		dvar_t* Dvar_FindVar(const char* dvarName);
		const dvar_t* Dvar_RegisterFloat(const char* dvarName, float value, float min, float max, unsigned __int16 flags, const char* description);
		const dvar_t* Dvar_RegisterInt(const char* dvarName, int value, int min, int max, unsigned __int16 flags, const char* description);

		const float* Scr_AllocVector(const float* v);
		void Scr_ClearOutParams();
		scr_entref_t Scr_GetEntityIdRef(unsigned int id);
		void Scr_NotifyId(unsigned int id, unsigned int stringValue, unsigned int paramcount);
		int Scr_SetObjectField(unsigned int classnum, int entnum, int offset);
		void Scr_AddString(const char* value);
		void Scr_AddInt(int value);

		const char* SL_ConvertToString(unsigned int stringValue);
		unsigned int SL_GetString(const char* str, unsigned int user);
		unsigned int SL_GetCanonicalString(const char* str);

		void SV_SendClientGameState(mp::client_t* client);
		int SV_IsTestClient(int clientNum);
		void SV_DropClient(mp::client_t* drop, const char* reason, bool tellThem);
		void SV_DropAllBots();

		void ClientCommand(int clientNum);

		int GetProtocolVersion();

		void NetAdr_SetType(netadr_s* addr, netadrtype_t type);

		void Cbuf_AddText(LocalClientNum_t localClientNum, const char* text);

		void TeleportPlayer(gentity_s* player, float* origin, float* angles);

		void CG_GameMessage(LocalClientNum_t localClientNum, const char* msg, int flags = 0);

		void Sys_EnterCriticalSection(CriticalSection critSect);
		void Sys_LeaveCriticalSection(CriticalSection critSect);
		bool Sys_TryEnterCriticalSection(CriticalSection critSect);
		bool Sys_IsMainThread();
		bool Sys_IsDatabaseThread();
		bool Sys_IsStreamThread();
		bool Sys_IsRenderThread();
		bool Sys_IsServerThread();

		void FS_FCloseFile(int h);
		bool FS_Initialized();
		int FS_HandleForFile(FsThread thread);
		int FS_FOpenFileReadForThread(const char* filename, int* file, FsThread thread);
		int FS_CreatePath(char* OSPath);
		void FS_CheckFileSystemStarted();
	}

	bool is_mp();
	bool is_sp();
	bool is_dedi();

	void initialize(launcher::mode mode);
}
