#include <std_include.hpp>
#include "game.hpp"

namespace game
{
	namespace native
	{
		Cmd_AddCommand_t Cmd_AddCommand;
		Cmd_RemoveCommand_t Cmd_RemoveCommand;

		Com_Error_t Com_Error;

		DB_LoadXAssets_t DB_LoadXAssets;

		Dvar_RegisterBool_t Dvar_RegisterBool;
		Dvar_RegisterString_t Dvar_RegisterString;

		Dvar_SetIntByName_t Dvar_SetIntByName;
		Dvar_SetFromStringByName_t Dvar_SetFromStringByName;
		Dvar_SetString_t Dvar_SetString;

		Dvar_DisplayableValue_t Dvar_DisplayableValue;

		G_RunFrame_t G_RunFrame;
		G_GetWeaponForName_t G_GetWeaponForName;

		MSG_ReadData_t MSG_ReadData;

		MT_AllocIndex_t MT_AllocIndex;

		RemoveRefToValue_t RemoveRefToValue;

		SL_GetStringOfSize_t SL_GetStringOfSize;

		Scr_AddEntityNum_t Scr_AddEntityNum;
		Scr_Notify_t Scr_Notify;
		Scr_NotifyLevel_t Scr_NotifyLevel;
		Scr_GetNumParam_t Scr_GetNumParam;
		Scr_GetString_t Scr_GetString;

		Sys_ShowConsole_t Sys_ShowConsole;
		Sys_Error_t Sys_Error;
		Sys_Milliseconds_t Sys_Milliseconds;

		VM_Notify_t VM_Notify;

		BG_NetDataChecksum_t BG_NetDataChecksum;

		LiveStorage_GetPersistentDataDefVersion_t LiveStorage_GetPersistentDataDefVersion;
		LiveStorage_GetPersistentDataDefFormatChecksum_t LiveStorage_GetPersistentDataDefFormatChecksum;

		SV_DirectConnect_t SV_DirectConnect;
		SV_ClientEnterWorld_t SV_ClientEnterWorld;
		SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString;
		SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString;

		XUIDToString_t XUIDToString;

		SEH_LocalizeTextMessage_t SEH_LocalizeTextMessage;

		CM_TransformedCapsuleTrace_t CM_TransformedCapsuleTrace;

		PM_WeaponUseAmmo_t PM_WeaponUseAmmo;
		PM_playerTrace_t PM_playerTrace;
		PM_trace_t PM_trace;

		Jump_ClearState_t Jump_ClearState;

		Vec3Normalize_t Vec3Normalize;

		Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

		Com_Quit_f_t Com_Quit_f;

		FS_Printf_t FS_Printf;

		player_die_t player_die;

		decltype(longjmp)* _longjmp;

		CmdArgs* sv_cmd_args;
		CmdArgs* cmd_args;

		short* scrVarGlob;
		char** scrMemTreePub;
		char* scrMemTreeGlob;

		scrVarPub_t* scr_VarPub;
		scrVmPub_t* scr_VmPub;

		scr_call_t* scr_instanceFunctions;
		scr_call_t* scr_globalFunctions;

		unsigned int* levelEntityId;

		int* g_script_error_level;
		jmp_buf* g_script_error;

		scr_classStruct_t* g_classMap;

		int* svs_clientCount;

		gentity_s* g_entities;

		level_locals_t* level;

		DeferredQueue* deferredQueue;

		float* com_codeTimeScale;

		RTL_CRITICAL_SECTION* s_criticalSection;

		int* logfile;

		searchpath_s** fs_searchpaths;
		char* fs_gamedir;
		fileHandleData_t* fsh;
		int* com_fileAccessed;

		// DS does not have MJPEG thread
		unsigned int (*threadId)[THREAD_CONTEXT_COUNT];

		int* initialized_0;
		int* sys_timeBase;
		unsigned __int64* sys_counterBase;

		int* dvarCount;
		dvar_t** sortedDvars;

		namespace mp
		{
			SV_GetGuid_t SV_GetGuid;
			SV_GameSendServerCommand_t SV_GameSendServerCommand;

			client_t* svs_clients;
		}

		namespace dedi
		{
			client_t* svs_clients;
		}

		namespace sp
		{
			IsServerRunning_t IsServerRunning;
			SV_GameSendServerCommand_t SV_GameSendServerCommand;

			TeleportPlayer_t TeleportPlayer;

			gentity_s* g_entities;

			gclient_s* g_clients;
		}

		void AddRefToValue(VariableValue* value)
		{
			if (value->type == SCRIPT_OBJECT)
			{
				++scrVarGlob[4 * value->u.entityId];
			}
			else if ((value->type & ~1) == SCRIPT_STRING)
			{
				static const auto size = is_sp() ? 16 : 12;
				const auto ref_count = reinterpret_cast<unsigned volatile *>(*scrMemTreePub + size * value
				                                                                                     ->u.stringValue);
				InterlockedIncrement(ref_count);
			}
			else if (value->type == SCRIPT_VECTOR)
			{
				if (!*PBYTE(value->u.vectorValue - 1))
				{
					++*PWORD(value->u.vectorValue - 4);
				}
			}
		}

		__declspec(naked) unsigned int conbuf_append_text_dedicated(const char* message)
		{
			static DWORD func = 0x53C790;

			__asm
			{
				mov ecx, message
				call func
				retn
			}
		}

		void Conbuf_AppendText(const char* message)
		{
			if (is_dedi())
			{
				conbuf_append_text_dedicated(message);
			}
			else
			{
				reinterpret_cast<void(*)(const char*)>(SELECT_VALUE(0x4C84E0, 0x5CF610, 0))(message);
			}
		}

		__declspec(naked) unsigned int find_variable_dedicated(unsigned int parentId, unsigned int name)
		{
			static DWORD func = 0x4E7ED0;

			__asm
			{
				mov eax, name
				mov ecx, parentId
				call func
				retn
			}
		}

		unsigned int FindVariable(const unsigned int parentId, const unsigned int name)
		{
			if (is_dedi())
			{
				return find_variable_dedicated(parentId, name);
			}

			return reinterpret_cast<unsigned int(*)(unsigned int, unsigned int)> //
				(SELECT_VALUE(0x4C4E70, 0x5651F0, 0x0))(parentId, name);
		}

		__declspec(naked) VariableValue get_entity_field_value_dedicated(unsigned int classnum, int entnum, int _offset)
		{
			static DWORD func = 0x4F1400;

			__asm
			{
				push _offset
				push entnum
				mov ecx, classnum
				call func
				add esp, 8h
				retn
			}
		}

		VariableValue GetEntityFieldValue(const unsigned int classnum, const int entnum, const int offset)
		{
			if (is_dedi())
			{
				return get_entity_field_value_dedicated(classnum, entnum, offset);
			}

			return reinterpret_cast<VariableValue(*)(unsigned int, int, int)> //
				(SELECT_VALUE(0x530E30, 0x56AF20, 0x0))(classnum, entnum, offset);
		}

		void* MT_Alloc(const int numBytes, const int type)
		{
			return scrMemTreeGlob + 12 * size_t(MT_AllocIndex(numBytes, type));
		}

		__declspec(naked) dvar_t* dvar_find_malleable_var(const char* dvar_name)
		{
			static DWORD func = 0x531320;

			__asm
			{
				mov edi, dvar_name
				call func
				retn
			}
		}

		dvar_t* Dvar_FindVar(const char* dvarName)
		{
			if (is_dedi())
			{
				return dvar_find_malleable_var(dvarName);
			}

			return reinterpret_cast<dvar_t*(*)(const char*)>
				(SELECT_VALUE(0x539550, 0x5BDCC0, 0x0))(dvarName);
		}

		__declspec(naked) const dvar_t* dvar_register_variant_dedicated(const char* dvar_name, unsigned char type,
			unsigned __int16 flags, DvarValue value, DvarLimits domain, const char* description)
		{
			static DWORD func = 0x531F70;

			__asm
			{
				push eax
				pushad

				mov edi, [esp + 0x24 + 0x28] // description
				mov eax, [esp + 0x24 + 0x4] // dvarName

				push [esp + 0x24 + 0x24] // domain
				push [esp + 0x24 + 0x24] // domain

				push [esp + 0x24 + 0x24] // value
				push [esp + 0x24 + 0x24] // value
				push [esp + 0x24 + 0x24] // value
				push [esp + 0x24 + 0x24] // value

				push [esp + 0x24 + 0x24] // flags
				push [esp + 0x24 + 0x24] // type

				call func
				add esp, 0x20

				mov [esp + 0x20], eax // result
				popad
				pop eax

				retn
			}
		}

		const dvar_t* Dvar_RegisterFloat(const char* dvarName, float value,
			float min, float max, unsigned __int16 flags, const char* description)
		{
			if (!is_dedi())
			{
				return reinterpret_cast<const dvar_t*(*)(const char*, float, float, float, unsigned __int16, const char*)>
					(SELECT_VALUE(0x4F9CC0, 0x5BEA80, 0x0))(dvarName, value, min, max, flags, description);
			}

			DvarLimits domain;
			DvarValue dvar_value;

			domain.value.min = min;
			domain.value.max = max;

			dvar_value.value = value;

			return dvar_register_variant_dedicated(dvarName, DVAR_TYPE_FLOAT,
				flags, dvar_value, domain, description);
		}

		const dvar_t* Dvar_RegisterInt(const char* dvarName, int value,
			int min, int max, unsigned __int16 flags, const char* description)
		{
			if (!is_dedi())
			{
				return reinterpret_cast<const dvar_t*(*)(const char*, int, int, int, unsigned __int16, const char*)>
					(SELECT_VALUE(0x48CD40, 0x5BEA40, 0x0))(dvarName, value, min, max, flags, description);
			}

			DvarLimits domain;
			DvarValue dvar_value;

			domain.integer.min = min;
			domain.integer.max = max;

			dvar_value.integer = value;

			return dvar_register_variant_dedicated(dvarName, DVAR_TYPE_INT,
				flags, dvar_value, domain, description);
		}

		void IncInParam()
		{
			Scr_ClearOutParams();

			if (scr_VmPub->top == scr_VmPub->maxStack)
			{
				Sys_Error("Internal script stack overflow");
			}

			scr_VmPub->top++;
			scr_VmPub->inparamcount++;
		}

		const float* Scr_AllocVector(const float* v)
		{
			const auto mem = static_cast<DWORD*>(MT_Alloc(16, 2));
			*mem = 0;

			const auto array = reinterpret_cast<float*>(mem + 1);
			array[0] = v[0];
			array[1] = v[1];
			array[2] = v[2];

			return array;
		}

		void Scr_ClearOutParams()
		{
			const auto num_params = scr_VmPub->outparamcount;
			for (unsigned int i = num_params; i > 0; --i)
			{
				const auto value = scr_VmPub->top[i - 1];
				RemoveRefToValue(value.type, value.u);
			}

			scr_VmPub->top -= num_params;
		}

		scr_entref_t Scr_GetEntityIdRef(const unsigned int id)
		{
			static auto class_array = reinterpret_cast<DWORD*>(SELECT_VALUE(0x19AFC84, 0x1E72184, 0x1D3C804));
			static auto ent_array = reinterpret_cast<WORD*>(SELECT_VALUE(0x19AFC82, 0x1E72182, 0x1D3C802));

			scr_entref_t result{};
			result.raw.classnum = static_cast<unsigned short>(class_array[2 * id]) >> 8;
			result.raw.entnum = ent_array[4 * id];

			return result;
		}

		scr_call_t Scr_GetFunc(const unsigned int index)
		{
			if (index > 0x1C7)
			{
				return scr_instanceFunctions[index];
			}

			return scr_globalFunctions[index];
		}

		__declspec(naked) void scr_notify_id_multiplayer(unsigned int id, unsigned int string_value,
		                                                 unsigned int paramcount)
		{
			static DWORD func = 0x56B5E0;

			__asm
			{
				mov eax, paramcount
				push string_value
				push id
				call func
				add esp, 8h
				retn
			}
		}

		__declspec(naked) void scr_notify_id_singleplayer(unsigned int id, unsigned int string_value,
		                                                  unsigned int paramcount)
		{
			static DWORD func = 0x610980;

			__asm
			{
				mov eax, paramcount
				push string_value
				push id
				call func
				add esp, 8h
				retn
			}
		}

		void Scr_NotifyId(unsigned int id, unsigned int stringValue, unsigned int paramcount)
		{
			if (is_mp())
			{
				scr_notify_id_multiplayer(id, stringValue, paramcount);
			}
			else if (is_sp())
			{
				scr_notify_id_singleplayer(id, stringValue, paramcount);
			}
			else
			{
				reinterpret_cast<void(*)(unsigned int, unsigned int, unsigned int)>(0x4EFAA0)
					(id, stringValue, paramcount);
			}
		}

		__declspec(naked) int scr_set_object_field_dedicated(unsigned int classnum, int entnum, int _offset)
		{
			static DWORD func = 0x4B15C0;

			__asm
			{
				mov ecx, _offset
				mov eax, entnum
				push classnum
				call func
				add esp, 4h
				retn
			}
		}

		int Scr_SetObjectField(const unsigned int classnum, const int entnum, const int offset)
		{
			if (is_dedi())
			{
				return scr_set_object_field_dedicated(classnum, entnum, offset);
			}
			
			return reinterpret_cast<int(*)(unsigned int, int, int)> //
				(SELECT_VALUE(0x42CAD0, 0x52BCC0, 0x0))(classnum, entnum, offset);
		}

		__declspec(naked) void scr_add_string_dedicated(const char* value)
		{
			static DWORD func = 0x4F1010;

			__asm
			{
				mov edi, value
				call func
				retn
			}
		}

		void Scr_AddString(const char* value)
		{
			if (is_dedi())
			{
				scr_add_string_dedicated(value);
			}
			else
			{
				reinterpret_cast<void(*)(const char*)>
					(SELECT_VALUE(0x4A5600, 0x56AC00, 0x0))(value);
			}
		}

		void Scr_AddInt(int value)
		{
			IncInParam();

			scr_VmPub->top->type = SCRIPT_INTEGER;
			scr_VmPub->top->u.intValue = value;
		}

		const char* SL_ConvertToString(const unsigned int stringValue)
		{
			if (!stringValue) return nullptr;

			static const auto size = is_sp() ? 16 : 12;
			return *scrMemTreePub + size * stringValue + 4;
		}

		unsigned int SL_GetString(const char* str, const unsigned int user)
		{
			return SL_GetStringOfSize(str, user, std::strlen(str) + 1, 7);
		}

		__declspec(naked) void sv_send_client_game_state_mp(mp::client_t* /*client*/)
		{
			static DWORD func = 0x570FC0;

			__asm
			{
				pushad

				mov esi, [esp + 0x20 + 0x4]
				call func

				popad
				retn
			}
		}

		void SV_SendClientGameState(mp::client_t* client)
		{
			if (is_mp())
			{
				sv_send_client_game_state_mp(client);
			}
		}

		int SV_IsTestClient(int clientNum)
		{
			assert(clientNum < *svs_clientCount);

			if (is_dedi())
			{
				return dedi::svs_clients[clientNum].bIsTestClient;
			}

			if (is_mp())
			{
				return mp::svs_clients[clientNum].bIsTestClient;
			}

			return 0;
		}

		void SV_DropClient(mp::client_t* drop, const char* reason, bool tellThem)
		{
			if (is_mp())
			{
				reinterpret_cast<void(*)(mp::client_t*, const char*, bool)>
					(0x570980)(drop, reason, tellThem);
			}
		}

		void sv_drop_all_bots_mp()
		{
			for (auto i = 0; i < *svs_clientCount; i++)
			{
				if (mp::svs_clients[i].header.state != CS_FREE
					&& mp::svs_clients[i].header.netchan.remoteAddress.type == NA_BOT)
				{
					SV_DropClient(&mp::svs_clients[i], "EXE_TIMEDOUT", true);
				}
			}
		}

		void SV_DropAllBots()
		{
			if (is_mp())
			{
				sv_drop_all_bots_mp();
			}
		}

		__declspec(naked) void client_command_dedicated(int client_num)
		{
			static DWORD func = 0x47EA40;

			__asm
			{
				mov edi, client_num
				call func
				retn
			}
		}

		void ClientCommand(int clientNum)
		{
			if (is_dedi())
			{
				client_command_dedicated(clientNum);
			}
			else if (is_mp())
			{
				reinterpret_cast<void(*)(int)>(0x502CB0)(clientNum);
			}
		}

		int GetProtocolVersion()
		{
			return 0x507C;
		}

		void NetAdr_SetType(netadr_s* addr, netadrtype_t type)
		{
			addr->type = type;
		}

		__declspec(naked) void cbuf_add_text_dedicated(LocalClientNum_t local_client_num, const char* text)
		{
			static DWORD func = 0x4CB5D0;

			__asm
			{
				mov eax, text
				push local_client_num
				call func
				add esp, 4h
				retn
			}
		}

		void Cbuf_AddText(LocalClientNum_t localClientNum, const char* text)
		{
			if (is_dedi())
			{
				cbuf_add_text_dedicated(localClientNum, text);
			}
			else
			{
				reinterpret_cast<void(*)(LocalClientNum_t, const char*)>
					(SELECT_VALUE(0x457C90, 0x545680, 0x0))(localClientNum, text);
			}
		}

		void teleport_player_dedicated(gentity_s* player, float* origin, float* angles)
		{
			static DWORD func = 0x48B840;

			__asm
			{
				pushad

				mov eax, player
				mov ecx, origin
				push angles
				call func
				add esp, 4h

				popad
			}
		}

		void TeleportPlayer(gentity_s* player, float* origin, float* angles)
		{
			if (is_dedi())
			{
				teleport_player_dedicated(player, origin, angles);
			}
			else if (is_mp())
			{
				reinterpret_cast<void(*)(gentity_s*, float*, float*)>
					(0x50D840)(player, origin, angles);
			}
		}

		// SP takes one extra argument, all possible values are undocumented
		void CG_GameMessage(LocalClientNum_t localClientNum, const char* msg, int flags)
		{
			if (is_mp())
			{
				reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(0x456DC0)(localClientNum, msg);
			}
			else if (is_sp())
			{
				reinterpret_cast<void(*)(LocalClientNum_t, const char*, int)>(0x4228A0)(localClientNum, msg, flags);
			}
		}

		void Sys_EnterCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) <
				static_cast<unsigned int>(CRITSECT_COUNT));

			EnterCriticalSection(&s_criticalSection[critSect]);
		}

		void Sys_LeaveCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) <
				static_cast<unsigned int>(CRITSECT_COUNT));

			LeaveCriticalSection(&s_criticalSection[critSect]);
		}

		bool Sys_TryEnterCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) <
				static_cast<unsigned int>(CRITSECT_COUNT));

			return TryEnterCriticalSection(&s_criticalSection[critSect]) != FALSE;
		}

		bool Sys_IsMainThread()
		{
			const auto id = GetCurrentThreadId();
			assert(id);
			return id == *threadId[THREAD_CONTEXT_MAIN];
		}

		bool Sys_IsDatabaseThread()
		{
			const auto id = GetCurrentThreadId();
			assert(id);
			return id == *threadId[THREAD_CONTEXT_DATABASE];
		}

		bool Sys_IsStreamThread()
		{
			const auto id = GetCurrentThreadId();
			assert(id);
			return id == *threadId[THREAD_CONTEXT_STREAM];
		}

		bool Sys_IsRenderThread()
		{
			const auto id = GetCurrentThreadId();
			assert(id);
			return id == *threadId[THREAD_CONTEXT_BACKEND];
		}

		bool Sys_IsServerThread()
		{
			const auto id = GetCurrentThreadId();
			assert(id);
			return id == *threadId[THREAD_CONTEXT_SERVER];
		}

		void fs_fclose_file_dedicated(int h)
		{
			static DWORD func = 0x524910;

			__asm
			{
				pushad
				mov eax, h
				call func
				popad
			}
		}

		void FS_FCloseFile(int h)
		{
			if (is_dedi())
			{
				fs_fclose_file_dedicated(h);
			}
			else
			{
				reinterpret_cast<void(*)(int)>
					(SELECT_VALUE(0x415160, 0x5AF170, 0x0))(h);
			}
		}

		bool FS_Initialized()
		{
			return (*fs_searchpaths != nullptr);
		}

		int fs_handle_for_file_dedicated(FsThread thread)
		{
			static DWORD func = 0x5245F0;
			auto result = 0;

			__asm
			{
				pushad
				mov edi, thread
				call func
				mov result, eax
				popad
			}

			return result;
		}

		int FS_HandleForFile(FsThread thread)
		{
			if (is_dedi())
			{
				return fs_handle_for_file_dedicated(thread);
			}

			return reinterpret_cast<int(*)(FsThread)>
				(SELECT_VALUE(0x46B1C0, 0x5AEE50, 0x0))(thread);
		}

		int fs_fopen_file_read_for_thread_singleplayer(const char* filename, int* file, FsThread thread)
		{
			static DWORD func = 0x627F20;
			auto result = 0;

			__asm
			{
				pushad

				mov eax, file
				mov edx, filename
				push thread
				call func
				add esp, 0x4
				mov result, eax

				popad
			}

			return result;
		}

		int fs_fopen_file_read_for_thread_multiplayer(const char* filename, int* file, FsThread thread)
		{
			static DWORD func = 0x5B1990;
			auto result = 0;

			__asm
			{
				pushad

				mov edx, filename
				push thread
				push file
				call func
				add esp, 0x8
				mov result, eax

				popad
			}

			return result;
		}

		int fs_fopen_file_read_for_thread_dedicated(const char* filename, int* file, FsThread thread)
		{
			static DWORD func = 0x524E30;
			auto result = 0;

			__asm
			{
				pushad

				mov edx, filename
				push thread
				push file
				call func
				add esp, 0x8
				mov result, eax

				popad
			}

			return result;
		}

		int FS_FOpenFileReadForThread(const char* filename, int* file, FsThread thread)
		{
			if (is_sp())
			{
				return fs_fopen_file_read_for_thread_singleplayer(filename, file, thread);
			}

			if (is_mp())
			{
				return fs_fopen_file_read_for_thread_multiplayer(filename, file, thread);
			}

			return fs_fopen_file_read_for_thread_dedicated(filename, file, thread);
		}

		int fs_create_path_directory(char* OSPath)
		{
			static DWORD func = 0x5247D0;
			auto result = 0;

			__asm
			{
				pushad
				mov edi, OSPath
				call func
				mov result, eax
				popad
			}

			return result;
		}

		int FS_CreatePath(char* OSPath)
		{
			if (is_dedi())
			{
				return fs_create_path_directory(OSPath);
			}

			return reinterpret_cast<int(*)(char*)>
				(SELECT_VALUE(0x4F5AB0, 0x5AF060, 0x0))(OSPath);
		}

		void FS_CheckFileSystemStarted()
		{
			assert(*fs_searchpaths);
		}
	}

	launcher::mode mode = launcher::mode::none;

	launcher::mode get_mode()
	{
		if (mode == launcher::mode::none)
		{
			throw std::runtime_error("Launcher mode not valid. Something must be wrong.");
		}

		return mode;
	}

	bool is_mp()
	{
		return get_mode() == launcher::mode::multiplayer;
	}

	bool is_sp()
	{
		return get_mode() == launcher::mode::singleplayer;
	}

	bool is_dedi()
	{
		return get_mode() == launcher::mode::server;
	}

	void initialize(const launcher::mode _mode)
	{
		mode = _mode;

		native::Cmd_AddCommand = native::Cmd_AddCommand_t(SELECT_VALUE(0x558820, 0x545DF0, 0));
		native::Cmd_RemoveCommand = native::Cmd_RemoveCommand_t(SELECT_VALUE(0x443A30, 0x545E20, 0x4CC060));

		native::Com_Error = native::Com_Error_t(SELECT_VALUE(0x425540, 0x555450, 0x4D93F0));

		native::DB_LoadXAssets = native::DB_LoadXAssets_t(SELECT_VALUE(0x48A8E0, 0x4CD020, 0x44F770));

		native::Dvar_RegisterBool = native::Dvar_RegisterBool_t(SELECT_VALUE(0x4914D0, 0x5BE9F0, 0x0));
		native::Dvar_RegisterString = native::Dvar_RegisterString_t(SELECT_VALUE(0x5197F0, 0x5BEC90, 0x0));

		native::Dvar_SetIntByName = native::Dvar_SetIntByName_t(SELECT_VALUE(0x5396B0, 0x5BF560, 0x0));
		native::Dvar_SetFromStringByName = native::Dvar_SetFromStringByName_t(
			SELECT_VALUE(0x4DD090, 0x5BF740, 0x518DF0));
		native::Dvar_SetString = native::Dvar_SetString_t(SELECT_VALUE(0x540570, 0x5BF3E0, 0x0));

		native::Dvar_DisplayableValue = native::Dvar_DisplayableValue_t(SELECT_VALUE(0x4AB1D0, 0x5BD260, 0x0));

		native::G_RunFrame = native::G_RunFrame_t(SELECT_VALUE(0x52EAA0, 0x50CB70, 0x48AD60));
		native::G_GetWeaponForName = native::G_GetWeaponForName_t(SELECT_VALUE(0x495E40, 0x531070, 0x0));

		native::MSG_ReadData = native::MSG_ReadData_t(SELECT_VALUE(0, 0x5592A0, 0));

		native::MT_AllocIndex = native::MT_AllocIndex_t(SELECT_VALUE(0x4B9610, 0x562080, 0x4E6C30));

		native::RemoveRefToValue = native::RemoveRefToValue_t(SELECT_VALUE(0x477EA0, 0x565730, 0x4E8A40));

		native::SL_GetStringOfSize = native::SL_GetStringOfSize_t(SELECT_VALUE(0x4E13F0, 0x564650, 0x4E7490));

		native::Scr_AddEntityNum = native::Scr_AddEntityNum_t(SELECT_VALUE(0x0, 0x56ABC0, 0x4EA2F0));

		native::Scr_Notify = native::Scr_Notify_t(SELECT_VALUE(0x4895B0, 0x52B190, 0x0));
		native::Scr_NotifyLevel = native::Scr_NotifyLevel_t(SELECT_VALUE(0x445E10, 0x56B6B0, 0x0));
		native::Scr_GetNumParam = native::Scr_GetNumParam_t(SELECT_VALUE(0x4C6FE0, 0x56AA10, 0x0));
		native::Scr_GetString = native::Scr_GetString_t(SELECT_VALUE(0x497530, 0x56A3D0, 0x0));

		native::Sys_ShowConsole = native::Sys_ShowConsole_t(SELECT_VALUE(0x470AF0, 0x5CF590, 0));
		native::Sys_Error = native::Sys_Error_t(SELECT_VALUE(0x490D90, 0x5CC3B0, 0x539590));
		native::Sys_Milliseconds = native::Sys_Milliseconds_t(SELECT_VALUE(0x4A1610, 0x5CE740, 0x53B900));

		native::VM_Notify = native::VM_Notify_t(SELECT_VALUE(0x610200, 0x569720, 0x4EF450));

		native::BG_NetDataChecksum = native::BG_NetDataChecksum_t(SELECT_VALUE(0x0, 0x41BB20, 0x0));

		native::LiveStorage_GetPersistentDataDefVersion = native::LiveStorage_GetPersistentDataDefVersion_t(
			SELECT_VALUE(0x0, 0x548D60, 0x4D0390));

		native::LiveStorage_GetPersistentDataDefFormatChecksum = native::LiveStorage_GetPersistentDataDefFormatChecksum_t(
			SELECT_VALUE(0x0, 0x548D80, 0x4D03D0));

		native::SV_DirectConnect = native::SV_DirectConnect_t(SELECT_VALUE(0x0, 0x572750, 0x4F74C0));
		native::SV_ClientEnterWorld = native::SV_ClientEnterWorld_t(SELECT_VALUE(0x0, 0x571100, 0x0));
		native::SV_Cmd_TokenizeString = native::SV_Cmd_TokenizeString_t(SELECT_VALUE(0x0, 0x545D40, 0x0));
		native::SV_Cmd_EndTokenizedString = native::SV_Cmd_EndTokenizedString_t(SELECT_VALUE(0x0, 0x545D70, 0x0));
		native::mp::SV_GameSendServerCommand = native::mp::SV_GameSendServerCommand_t(0x573220);
		native::mp::SV_GetGuid = native::mp::SV_GetGuid_t(0x573990);

		native::sp::IsServerRunning = native::sp::IsServerRunning_t(0x45D310);
		native::sp::SV_GameSendServerCommand = native::sp::SV_GameSendServerCommand_t(0x402130);

		native::sp::TeleportPlayer = native::sp::TeleportPlayer_t(0x4CCEE0);

		native::XUIDToString = native::XUIDToString_t(SELECT_VALUE(0x4FAA30, 0x55CC20, 0x0));

		native::SEH_LocalizeTextMessage = native::SEH_LocalizeTextMessage_t(
			SELECT_VALUE(0x41EA20, 0x57E240, 0x0));

		native::CM_TransformedCapsuleTrace = native::CM_TransformedCapsuleTrace_t(
			SELECT_VALUE(0x4F9B80, 0x541340, 0x0));

		native::PM_WeaponUseAmmo = native::PM_WeaponUseAmmo_t(SELECT_VALUE(0x463F80, 0x42E930, 0x0));
		native::PM_playerTrace = native::PM_playerTrace_t(SELECT_VALUE(0x4CE600, 0x421F00, 0x0));
		native::PM_trace = native::PM_trace_t(SELECT_VALUE(0x544BF0, 0x41CEB0, 0x0));

		native::Jump_ClearState = native::Jump_ClearState_t(SELECT_VALUE(0x514CE0, 0x4160F0, 0x0));

		native::Vec3Normalize = native::Vec3Normalize_t(SELECT_VALUE(0x536A20, 0x415EE0, 0x0));

		native::Cmd_ExecuteSingleCommand = native::Cmd_ExecuteSingleCommand_t(
			SELECT_VALUE(0x4D6960, 0x5462B0, 0x4CC360));

		native::Com_Quit_f = native::Com_Quit_f_t(SELECT_VALUE(0x4F48B0, 0x5556B0, 0x4D95B0));

		native::FS_Printf = native::FS_Printf_t(SELECT_VALUE(0x421E90, 0x5AF7C0, 0x5255A0));

		native::player_die = native::player_die_t(SELECT_VALUE(0x0, 0x503460, 0x47F4D0));

		native::_longjmp = reinterpret_cast<decltype(longjmp)*>(SELECT_VALUE(0x73AC20, 0x7363BC, 0x655558));

		native::sv_cmd_args = reinterpret_cast<native::CmdArgs*>(SELECT_VALUE(0x1757218, 0x1CAA998, 0x1B5E7D8));
		native::cmd_args = reinterpret_cast<native::CmdArgs*>(SELECT_VALUE(0x1750750, 0x1C978D0, 0x1B455F8));

		native::scrVarGlob = reinterpret_cast<short*>(SELECT_VALUE(0x19AFC80, 0x1E72180, 0x1D3C800));
		native::scrMemTreePub = reinterpret_cast<char**>(SELECT_VALUE(0x196FB00, 0x1E32000, 0x1C152A4));
		native::scrMemTreeGlob = reinterpret_cast<char*>(SELECT_VALUE(0x186DA00, 0x1D6FF00, 0x1C16600));

		native::scr_VarPub = reinterpret_cast<native::scrVarPub_t*>(SELECT_VALUE(0x0, 0x208E188, 0x1CD8720));
		native::scr_VmPub = reinterpret_cast<native::scrVmPub_t*>(SELECT_VALUE(0x1BF2580, 0x20B4A80, 0x1F5B080));

		native::scr_instanceFunctions = reinterpret_cast<native::scr_call_t*>(SELECT_VALUE(0x184CDB0, 0x1D4F258,
		                                                                                   0x1BF59C8));
		native::scr_globalFunctions = reinterpret_cast<native::scr_call_t*>(SELECT_VALUE(0x186C68C, 0x1D6EB34,
		                                                                                 0x1C152A4
		));

		native::g_script_error_level = reinterpret_cast<int*>(SELECT_VALUE(0x1BEFCFC, 0x20B21FC, 0x1F5B058));
		native::g_script_error = reinterpret_cast<jmp_buf*>(SELECT_VALUE(0x1BF1D18, 0x20B4218, 0x1F5A818));

		native::g_classMap = reinterpret_cast<native::scr_classStruct_t*>(SELECT_VALUE(0x92D140, 0x8B4300, 0x7C0408));

		native::svs_clientCount = reinterpret_cast<int*>(SELECT_VALUE(0x0, 0x4B5CF8C, 0x4A12E8C));

		native::levelEntityId = reinterpret_cast<unsigned int*>(SELECT_VALUE(0x1BCBCA4, 0x208E1A4, 0x1CD873C));

		native::mp::svs_clients = reinterpret_cast<native::mp::client_t*>(0x4B5CF90);
		native::dedi::svs_clients = reinterpret_cast<native::dedi::client_t*>(0x4A12E90);

		native::g_entities = reinterpret_cast<native::gentity_s*>(SELECT_VALUE(0, 0x1A66E28, 0x191B900));
		native::sp::g_entities = reinterpret_cast<native::sp::gentity_s*>(0x1197AD8);

		native::sp::g_clients = reinterpret_cast<native::sp::gclient_s*>(0x1381D48);

		native::level = reinterpret_cast<native::level_locals_t*>(SELECT_VALUE(0x0, 0x1C6D4D8, 0x1B21A20));

		native::deferredQueue = reinterpret_cast<native::DeferredQueue*>(SELECT_VALUE(0x0, 0x1D55438, 0x0));

		native::com_codeTimeScale = reinterpret_cast<float*>(SELECT_VALUE(0x1769F1C, 0x1CEF554, 0x1B9CEC0));

		native::s_criticalSection = reinterpret_cast<RTL_CRITICAL_SECTION*>(SELECT_VALUE(0x1CD5638, 0x5A91048, 0x593FF98));

		native::logfile = reinterpret_cast<int*>(SELECT_VALUE(0x176B534, 0x1CF0B78, 0x1B9E4C8));

		native::fs_searchpaths = reinterpret_cast<native::searchpath_s**>(SELECT_VALUE(0x1C2FE78, 0x59BA858, 0x62F4F60));
		native::fs_gamedir = reinterpret_cast<char*>(SELECT_VALUE(0x1C2B220, 0x59A98F8, 0x585A4D8));
		native::fsh = reinterpret_cast<native::fileHandleData_t*>(SELECT_VALUE(0x1C2B540, 0x59B5F20, 0x5866AF8));
		native::com_fileAccessed = reinterpret_cast<int*>(SELECT_VALUE(0x1C2B328, 0x59A9A04, 0x585A5E0));

		native::threadId = reinterpret_cast<unsigned(*)[native::THREAD_CONTEXT_COUNT]>(SELECT_VALUE(0x18576C8, 0x1D6E448, 0x1C14BDC));

		native::initialized_0 = reinterpret_cast<int*>(SELECT_VALUE(0x1CE1CA0, 0x5AA3058, 0x62F4F9C));
		native::sys_timeBase = reinterpret_cast<int*>(SELECT_VALUE(0x1CE1C98, 0x5AA3050, 0x5950CE4));
		native::sys_counterBase = reinterpret_cast<unsigned __int64*>(SELECT_VALUE(0x1CE1C90, 0x5AA3048, 0x5950CE8));

		native::dvarCount = reinterpret_cast<int*>(SELECT_VALUE(0x1C42398, 0x59CCDD8, 0x5879644));
		native::sortedDvars = reinterpret_cast<native::dvar_t**>(SELECT_VALUE(0x1C423C0, 0x59CCE00, 0x587C5D8));
	}
}
