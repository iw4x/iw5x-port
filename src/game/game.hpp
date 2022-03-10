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

		typedef void (*Com_Error_t)(int code, const char* fmt, ...);
		extern Com_Error_t Com_Error;

		typedef void (*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zoneCount, int sync);
		extern DB_LoadXAssets_t DB_LoadXAssets;

		typedef void (*Dvar_SetFromStringByName_t)(const char* dvarName, const char* string);
		extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

		typedef int (*G_RunFrame_t)(int, int);
		extern G_RunFrame_t G_RunFrame;

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

		typedef void (*Scr_Notify_t)(gentity_s* ent, unsigned __int16 stringValue, unsigned int paramcount);
		extern Scr_Notify_t Scr_Notify;

		typedef void (*Sys_ShowConsole_t)();
		extern Sys_ShowConsole_t Sys_ShowConsole;

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

		extern decltype(longjmp)* _longjmp;

		extern CmdArgs* sv_cmd_args;
		extern CmdArgs* cmd_args;

		extern short* scrVarGlob;
		extern char** scrMemTreePub;
		extern char* scrMemTreeGlob;

		extern scrVmPub_t* scr_VmPub;

		extern scr_call_t* scr_instanceFunctions;
		extern scr_call_t* scr_globalFunctions;

		extern unsigned int* levelEntityId;

		extern int* g_script_error_level;
		extern jmp_buf* g_script_error;

		extern scr_classStruct_t* g_classMap;

		extern int* svs_clientCount;

		namespace mp
		{
			extern client_t* svs_clients;
		}

		namespace dedi
		{
			extern client_t* svs_clients;
		}

		void AddRefToValue(VariableValue* value);

		void Conbuf_AppendText(const char* message);

		unsigned int FindVariable(unsigned int parentId, unsigned int name);

		VariableValue GetEntityFieldValue(unsigned int classnum, int entnum, int offset);

		void* MT_Alloc(int numBytes, int type);

		dvar_t* Dvar_FindVar(const char* dvarName);

		const float* Scr_AllocVector(const float* v);
		void Scr_ClearOutParams();
		scr_entref_t Scr_GetEntityIdRef(unsigned int id);
		scr_call_t Scr_GetFunc(unsigned int index);
		void Scr_NotifyId(unsigned int id, unsigned int stringValue, unsigned int paramcount);
		int Scr_SetObjectField(unsigned int classnum, int entnum, int offset);
		void Scr_AddString(const char* value);

		const char* SL_ConvertToString(unsigned int stringValue);
		unsigned int SL_GetString(const char* str, unsigned int user);

		void SV_SendClientGameState(mp::client_t* client);
		int SV_IsTestClient(int clientNum);
		void SV_DropClient(mp::client_t* drop, const char* reason, bool tellThem);
		void SV_DropAllBots();

		int GetProtocolVersion();

		void NetAdr_SetType(netadr_s* addr, netadrtype_t type);
	}

	bool is_mp();
	bool is_sp();
	bool is_dedi();

	void initialize(launcher::mode mode);
}
