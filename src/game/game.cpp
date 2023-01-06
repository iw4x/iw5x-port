#include <std_include.hpp>
#include "game.hpp"
#include "dvars.hpp"

namespace game
{
	namespace native
	{
		Cmd_AddCommand_t Cmd_AddCommand;
		Cmd_RemoveCommand_t Cmd_RemoveCommand;

		Cbuf_AddText_t Cbuf_AddText;

		Com_Error_t Com_Error;
		Com_Filter_t Com_Filter;

		DB_GetXAssetName_t DB_GetXAssetName;
		DB_LoadXAssets_t DB_LoadXAssets;
		DB_FindXAssetHeader_t DB_FindXAssetHeader;
		DB_IsXAssetDefault_t DB_IsXAssetDefault;
		DB_EnumXAssets_t DB_EnumXAssets;

		Dvar_RegisterBool_t Dvar_RegisterBool;
		Dvar_RegisterString_t Dvar_RegisterString;

		Dvar_SetIntByName_t Dvar_SetIntByName;
		Dvar_SetFromStringByName_t Dvar_SetFromStringByName;
		Dvar_SetString_t Dvar_SetString;

		Dvar_ForEach_t Dvar_ForEach;

		Dvar_DisplayableValue_t Dvar_DisplayableValue;
		Dvar_DisplayableLatchedValue_t Dvar_DisplayableLatchedValue;

		G_RunFrame_t G_RunFrame;
		G_GetWeaponForName_t G_GetWeaponForName;

		MSG_ReadData_t MSG_ReadData;

		MT_AllocIndex_t MT_AllocIndex;

		RemoveRefToValue_t RemoveRefToValue;

		SL_GetStringOfSize_t SL_GetStringOfSize;

		Scr_AddEntityNum_t Scr_AddEntityNum;
		Scr_AddString_t Scr_AddString;
		Scr_AddInt_t Scr_AddInt;
		Scr_AddFloat_t Scr_AddFloat;
		Scr_Notify_t Scr_Notify;
		Scr_NotifyLevel_t Scr_NotifyLevel;
		Scr_GetNumParam_t Scr_GetNumParam;
		Scr_GetString_t Scr_GetString;
		Scr_CastString_t Scr_CastString;
		Scr_ErrorInternal_t Scr_ErrorInternal;

		GetObjectType_t GetObjectType;

		Sys_ShowConsole_t Sys_ShowConsole;
		Sys_Error_t Sys_Error;
		Sys_Milliseconds_t Sys_Milliseconds;
		Sys_Sleep_t Sys_Sleep;

		PMem_AllocFromSource_NoDebug_t PMem_AllocFromSource_NoDebug;

		Hunk_AllocateTempMemoryHighInternal_t Hunk_AllocateTempMemoryHighInternal;
		Hunk_FreeTempMemory_t Hunk_FreeTempMemory;

		VM_Notify_t VM_Notify;

		BG_NetDataChecksum_t BG_NetDataChecksum;

		LiveStorage_GetPersistentDataDefVersion_t LiveStorage_GetPersistentDataDefVersion;
		LiveStorage_GetPersistentDataDefFormatChecksum_t LiveStorage_GetPersistentDataDefFormatChecksum;

		SV_DirectConnect_t SV_DirectConnect;
		SV_ClientEnterWorld_t SV_ClientEnterWorld;
		SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString;
		SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString;
		SV_SpawnServer_t SV_SpawnServer;
		SV_GetConfigstring_t SV_GetConfigstring;

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
		FS_ReadFile_t FS_ReadFile;
		FS_FreeFile_t FS_FreeFile;
		FS_Read_t FS_Read;
		FS_FCloseHandle_t FS_FCloseHandle;

		player_die_t player_die;

		LargeLocalResetToMark_t LargeLocalResetToMark;

		decltype(longjmp)* _longjmp;

		CmdArgs* sv_cmd_args;
		CmdArgs* cmd_args;

		short* scrVarGlob;
		char** scrMemTreePub;
		char* scrMemTreeGlob;

		function_stack_t* scr_function_stack;
		scrVarPub_t* scr_VarPub;
		scrVmPub_t* scr_VmPub;

		unsigned int* levelEntityId;

		int* g_script_error_level;
		jmp_buf* g_script_error;

		scr_classStruct_t* g_classMap;

		int* svs_clientCount;

		gentity_s* g_entities;

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

		FastCriticalSection* db_hashCritSect;

		const char** g_assetNames;

		int Vec4Compare(const float* a, const float* b)
		{
			return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
		}

		namespace mp
		{
			SV_GetGuid_t SV_GetGuid;
			SV_GameSendServerCommand_t SV_GameSendServerCommand;

			client_t* svs_clients;

			level_locals_t* level;
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
			if (value->type == VAR_POINTER)
			{
				++scrVarGlob[4 * value->u.entityId];
			}
			else if ((value->type & ~1) == VAR_STRING)
			{
				static const auto size = is_sp() ? 16 : 12;
				const auto ref_count = reinterpret_cast<unsigned volatile *>(*scrMemTreePub + size * value->u.stringValue);
				InterlockedIncrement(ref_count);
			}
			else if (value->type == VAR_VECTOR)
			{
				if (!*PBYTE(value->u.vectorValue - 1))
				{
					++*PWORD(value->u.vectorValue - 4);
				}
			}
		}

		void Conbuf_AppendText(const char* message)
		{
			reinterpret_cast<void(*)(const char*)>(SELECT_VALUE(0x4C84E0, 0x5CF610))(message);
		}

		unsigned int FindVariable(const unsigned int parentId, const unsigned int name)
		{
			return reinterpret_cast<unsigned int(*)(unsigned int, unsigned int)>(SELECT_VALUE(0x4C4E70, 0x5651F0))(parentId, name);
		}

		VariableValue GetEntityFieldValue(const unsigned int classnum, const int entnum, const int offset)
		{
			return reinterpret_cast<VariableValue(*)(unsigned int, int, int)>(SELECT_VALUE(0x530E30, 0x56AF20))(classnum, entnum, offset);
		}

		void* MT_Alloc(const int numBytes, const int type)
		{
			return scrMemTreeGlob + 12 * size_t(MT_AllocIndex(numBytes, type));
		}

		dvar_t* Dvar_FindVar(const char* dvarName)
		{
			return reinterpret_cast<dvar_t*(*)(const char*)>(SELECT_VALUE(0x539550, 0x5BDCC0))(dvarName);
		}

		const dvar_t* Dvar_RegisterFloat(const char* dvarName, float value,
			float min, float max, unsigned __int16 flags, const char* description)
		{
			return reinterpret_cast<const dvar_t*(*)(const char*, float, float, float, unsigned __int16, const char*)> //
				(SELECT_VALUE(0x4F9CC0, 0x5BEA80))(dvarName, value, min, max, flags, description);
		}

		const dvar_t* Dvar_RegisterInt(const char* dvarName, int value,
			int min, int max, unsigned __int16 flags, const char* description)
		{
			return reinterpret_cast<const dvar_t*(*)(const char*, int, int, int, unsigned __int16, const char*)> //
				(SELECT_VALUE(0x48CD40, 0x5BEA40))(dvarName, value, min, max, flags, description);
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
			static auto class_array = reinterpret_cast<DWORD*>(SELECT_VALUE(0x19AFC84, 0x1E72184));
			static auto ent_array = reinterpret_cast<WORD*>(SELECT_VALUE(0x19AFC82, 0x1E72182));

			scr_entref_t result;
			result.classnum = static_cast<unsigned short>(class_array[2 * id]) >> 8;
			result.entnum = ent_array[4 * id];

			return result;
		}

		__declspec(naked) void scr_notify_id_multiplayer(unsigned int id, unsigned int string_value, unsigned int paramcount)
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

		__declspec(naked) void scr_notify_id_singleplayer(unsigned int id, unsigned int string_value, unsigned int paramcount)
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
			else
			{
				scr_notify_id_singleplayer(id, stringValue, paramcount);
			}
		}

		int Scr_SetObjectField(const unsigned int classnum, const int entnum, const int offset)
		{
			return reinterpret_cast<int(*)(unsigned int, int, int)> //
				(SELECT_VALUE(0x42CAD0, 0x52BCC0))(classnum, entnum, offset);
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

		unsigned int sl_get_canonical_string(const char* string)
		{
			static DWORD func = SELECT_VALUE(0x610750, 0x56B040);
			unsigned int result{};

			__asm
			{
				pushad
				mov edi, string
				call func
				mov result, eax
				popad
			}

			return result;
		}

		unsigned int SL_GetCanonicalString(const char* str)
		{
			return sl_get_canonical_string(str);
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
			assert(clientNum < MAX_CLIENTS);

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
				reinterpret_cast<void(*)(mp::client_t*, const char*, bool)>(0x570980)(drop, reason, tellThem);
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

		void ClientCommand(int clientNum)
		{
			if (is_mp())
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

		void TeleportPlayer(gentity_s* player, float* origin, float* angles)
		{
			if (is_mp())
			{
				reinterpret_cast<void(*)(gentity_s*, float*, float*)>(0x50D840)(player, origin, angles);
			}
		}

		// SP takes one extra argument, all possible values are undocumented
		void CG_GameMessage(LocalClientNum_t localClientNum, const char* msg, int flags)
		{
			if (is_mp())
			{
				reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(0x456DC0)(localClientNum, msg);
			}
			else
			{
				reinterpret_cast<void(*)(LocalClientNum_t, const char*, int)>(0x4228A0)(localClientNum, msg, flags);
			}
		}

		void Sys_EnterCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) < //
				static_cast<unsigned int>(CRITSECT_COUNT));

			EnterCriticalSection(&s_criticalSection[critSect]);
		}

		void Sys_LeaveCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) < // 
				static_cast<unsigned int>(CRITSECT_COUNT));

			LeaveCriticalSection(&s_criticalSection[critSect]);
		}

		bool Sys_TryEnterCriticalSection(CriticalSection critSect)
		{
			assert(static_cast<unsigned int>(critSect) < //
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

		void Sys_LockRead(FastCriticalSection* critSect)
		{
			InterlockedIncrement(&critSect->readCount);
			while (critSect->writeCount) Sys_Sleep(0);
		}

		void Sys_UnlockRead(FastCriticalSection* critSect)
		{
			assert(critSect->readCount > 0);
			InterlockedDecrement(&critSect->readCount);
		}

		void FS_FCloseFile(int h)
		{
			reinterpret_cast<void(*)(int)>(SELECT_VALUE(0x415160, 0x5AF170))(h);
		}

		bool FS_Initialized()
		{
			return (*fs_searchpaths != nullptr);
		}

		int FS_HandleForFile(FsThread thread)
		{
			return reinterpret_cast<int(*)(FsThread)>(SELECT_VALUE(0x46B1C0, 0x5AEE50))(thread);
		}

		int fs_fopen_file_read_for_thread_singleplayer(const char* filename, int* file, FsThread thread)
		{
			static DWORD func = 0x627F20;
			int result{};

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
			int result{};

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

			return fs_fopen_file_read_for_thread_multiplayer(filename, file, thread);
		}

		int FS_CreatePath(char* OSPath)
		{
			return reinterpret_cast<int(*)(char*)>(SELECT_VALUE(0x4F5AB0, 0x5AF060))(OSPath);
		}

		void FS_CheckFileSystemStarted()
		{
			assert(*fs_searchpaths);
		}

		std::string filesystem_read_big_file(const char* filename, FsThread thread)
		{
			std::string file_buffer{};

			int handle;
			game::native::FS_FOpenFileReadForThread(filename, &handle, game::native::FsThread::FS_THREAD_DATABASE);

			if (handle > 0)
			{
				constexpr unsigned int BUFF_SIZE = 1024;

				while (true)
				{
					char buffer[BUFF_SIZE];
					auto size_read = game::native::FS_Read(buffer, BUFF_SIZE, handle);

					file_buffer.append(buffer, size_read);

					if (size_read < BUFF_SIZE)
					{
						// We're done!
						break;
					}
				}


				game::native::FS_FCloseHandle(handle);
			}

			return file_buffer;
		}

		XAssetEntry* db_find_x_asset_entry(XAssetType type_, const char* name)
		{
			static DWORD func = SELECT_VALUE(0x585400, 0x4CA450);
			XAssetEntry* result{};

			__asm
			{
				pushad
				push name
				mov edi, type_
				call func
				add esp, 0x4
				mov result, eax
				popad
			}

			return result;
		}

		XAssetEntry* DB_FindXAssetEntry(XAssetType type, const char* name)
		{
			return db_find_x_asset_entry(type, name);
		}

		int DB_XAssetExists(XAssetType type, const char* name)
		{
			Sys_LockRead(db_hashCritSect);
			auto* asset_entry = DB_FindXAssetEntry(type, name);
			Sys_UnlockRead(db_hashCritSect);
			return asset_entry != nullptr;
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

	void initialize(const launcher::mode _mode)
	{
		mode = _mode;

		dvars::initialize();

		native::Cmd_AddCommand = native::Cmd_AddCommand_t(SELECT_VALUE(0x558820, 0x545DF0));
		native::Cmd_RemoveCommand = native::Cmd_RemoveCommand_t(SELECT_VALUE(0x443A30, 0x545E20));

		native::Cbuf_AddText = native::Cbuf_AddText_t(SELECT_VALUE(0x457C90, 0x545680));

		native::Com_Error = native::Com_Error_t(SELECT_VALUE(0x425540, 0x555450));
		native::Com_Filter = native::Com_Filter_t(SELECT_VALUE(0x44EFF0, 0x5B7C30));

		native::DB_GetXAssetName = native::DB_GetXAssetName_t(0x4B7C10);
		native::DB_LoadXAssets = native::DB_LoadXAssets_t(SELECT_VALUE(0x48A8E0, 0x4CD020));
		native::DB_FindXAssetHeader = native::DB_FindXAssetHeader_t(SELECT_VALUE(0x4FF000, 0x4CA620));
		native::DB_IsXAssetDefault = native::DB_IsXAssetDefault_t(SELECT_VALUE(0x4868E0, 0x4CA800));
		native::DB_EnumXAssets = native::DB_EnumXAssets_t(0x5B6A60);

		native::Dvar_RegisterBool = native::Dvar_RegisterBool_t(SELECT_VALUE(0x4914D0, 0x5BE9F0));
		native::Dvar_RegisterString = native::Dvar_RegisterString_t(SELECT_VALUE(0x5197F0, 0x5BEC90));

		native::Dvar_SetIntByName = native::Dvar_SetIntByName_t(SELECT_VALUE(0x5396B0, 0x5BF560));
		native::Dvar_SetFromStringByName = native::Dvar_SetFromStringByName_t(SELECT_VALUE(0x4DD090, 0x5BF740));
		native::Dvar_SetString = native::Dvar_SetString_t(SELECT_VALUE(0x540570, 0x5BF3E0));

		native::Dvar_ForEach = native::Dvar_ForEach_t(SELECT_VALUE(0x536720, 0x5BFFB0));

		native::Dvar_DisplayableValue = native::Dvar_DisplayableValue_t(SELECT_VALUE(0x4AB1D0, 0x5BD260));
		native::Dvar_DisplayableLatchedValue = native::Dvar_DisplayableLatchedValue_t(SELECT_VALUE(0x464F50, 0x5BD290));

		native::G_RunFrame = native::G_RunFrame_t(SELECT_VALUE(0x52EAA0, 0x50CB70));
		native::G_GetWeaponForName = native::G_GetWeaponForName_t(SELECT_VALUE(0x495E40, 0x531070));

		native::MSG_ReadData = native::MSG_ReadData_t(SELECT_VALUE(0, 0x5592A0));

		native::MT_AllocIndex = native::MT_AllocIndex_t(SELECT_VALUE(0x4B9610, 0x562080));

		native::RemoveRefToValue = native::RemoveRefToValue_t(SELECT_VALUE(0x477EA0, 0x565730));

		native::SL_GetStringOfSize = native::SL_GetStringOfSize_t(SELECT_VALUE(0x4E13F0, 0x564650));

		native::Scr_AddEntityNum = native::Scr_AddEntityNum_t(SELECT_VALUE(0x0, 0x56ABC0));
		native::Scr_AddString = native::Scr_AddString_t(SELECT_VALUE(0x4A5600, 0x56AC00));
		native::Scr_AddInt = native::Scr_AddInt_t(SELECT_VALUE(0x42DE20, 0x56AA20));
		native::Scr_AddFloat = native::Scr_AddFloat_t(SELECT_VALUE(0x5349D0, 0x56AA70));

		native::Scr_Notify = native::Scr_Notify_t(SELECT_VALUE(0x4895B0, 0x52B190));
		native::Scr_NotifyLevel = native::Scr_NotifyLevel_t(SELECT_VALUE(0x445E10, 0x56B6B0));
		native::Scr_GetNumParam = native::Scr_GetNumParam_t(SELECT_VALUE(0x4C6FE0, 0x56AA10));
		native::Scr_GetString = native::Scr_GetString_t(SELECT_VALUE(0x497530, 0x56A3D0));
		native::Scr_CastString = native::Scr_CastString_t(SELECT_VALUE(0x447CE0, 0x566EE0));
		native::Scr_ErrorInternal = native::Scr_ErrorInternal_t(SELECT_VALUE(0x42B910, 0x568FD0));

		native::GetObjectType = native::GetObjectType_t(SELECT_VALUE(0x4D8FE0, 0x565C60));

		native::Sys_ShowConsole = native::Sys_ShowConsole_t(SELECT_VALUE(0x470AF0, 0x5CF590));
		native::Sys_Error = native::Sys_Error_t(SELECT_VALUE(0x490D90, 0x5CC3B0));
		native::Sys_Milliseconds = native::Sys_Milliseconds_t(SELECT_VALUE(0x4A1610, 0x5CE740));
		native::Sys_Sleep = native::Sys_Sleep_t(SELECT_VALUE(0x438600, 0x55F460));

		native::PMem_AllocFromSource_NoDebug = native::PMem_AllocFromSource_NoDebug_t(SELECT_VALUE(0x449E50, 0x5C15C0));

		native::Hunk_AllocateTempMemoryHighInternal = native::Hunk_AllocateTempMemoryHighInternal_t(SELECT_VALUE(0x517870, 0x5B6C60));
		native::Hunk_FreeTempMemory = native::Hunk_FreeTempMemory_t(SELECT_VALUE(0x434A40, 0x5B6F90));

		native::VM_Notify = native::VM_Notify_t(SELECT_VALUE(0x610200, 0x569720));

		native::BG_NetDataChecksum = native::BG_NetDataChecksum_t(SELECT_VALUE(0x0, 0x41BB20));

		native::LiveStorage_GetPersistentDataDefVersion = native::LiveStorage_GetPersistentDataDefVersion_t(SELECT_VALUE(0x0, 0x548D60));

		native::LiveStorage_GetPersistentDataDefFormatChecksum = native::LiveStorage_GetPersistentDataDefFormatChecksum_t(SELECT_VALUE(0x0, 0x548D80));

		native::SV_DirectConnect = native::SV_DirectConnect_t(SELECT_VALUE(0x0, 0x572750));
		native::SV_ClientEnterWorld = native::SV_ClientEnterWorld_t(SELECT_VALUE(0x0, 0x571100));
		native::SV_Cmd_TokenizeString = native::SV_Cmd_TokenizeString_t(SELECT_VALUE(0x0, 0x545D40));
		native::SV_Cmd_EndTokenizedString = native::SV_Cmd_EndTokenizedString_t(SELECT_VALUE(0x0, 0x545D70));
		native::SV_SpawnServer = native::SV_SpawnServer_t(SELECT_VALUE(0x0, 0x575020));
		native::SV_GetConfigstring = native::SV_GetConfigstring_t(SELECT_VALUE(0x4C6E30, 0x573D50));
		native::mp::SV_GameSendServerCommand = native::mp::SV_GameSendServerCommand_t(0x573220);
		native::mp::SV_GetGuid = native::mp::SV_GetGuid_t(0x573990);

		native::sp::IsServerRunning = native::sp::IsServerRunning_t(0x45D310);
		native::sp::SV_GameSendServerCommand = native::sp::SV_GameSendServerCommand_t(0x402130);

		native::sp::TeleportPlayer = native::sp::TeleportPlayer_t(0x4CCEE0);

		native::XUIDToString = native::XUIDToString_t(SELECT_VALUE(0x4FAA30, 0x55CC20));

		native::SEH_LocalizeTextMessage = native::SEH_LocalizeTextMessage_t(SELECT_VALUE(0x41EA20, 0x57E240));

		native::CM_TransformedCapsuleTrace = native::CM_TransformedCapsuleTrace_t(SELECT_VALUE(0x4F9B80, 0x541340));

		native::PM_WeaponUseAmmo = native::PM_WeaponUseAmmo_t(SELECT_VALUE(0x463F80, 0x42E930));
		native::PM_playerTrace = native::PM_playerTrace_t(SELECT_VALUE(0x4CE600, 0x421F00));
		native::PM_trace = native::PM_trace_t(SELECT_VALUE(0x544BF0, 0x41CEB0));

		native::Jump_ClearState = native::Jump_ClearState_t(SELECT_VALUE(0x514CE0, 0x4160F0));

		native::Vec3Normalize = native::Vec3Normalize_t(SELECT_VALUE(0x536A20, 0x415EE0));

		native::Cmd_ExecuteSingleCommand = native::Cmd_ExecuteSingleCommand_t(SELECT_VALUE(0x4D6960, 0x5462B0));

		native::Com_Quit_f = native::Com_Quit_f_t(SELECT_VALUE(0x4F48B0, 0x5556B0));

		native::FS_Printf = native::FS_Printf_t(SELECT_VALUE(0x421E90, 0x5AF7C0));
		native::FS_ReadFile = native::FS_ReadFile_t(SELECT_VALUE(0x4D8DF0, 0x5B1FB0));
		native::FS_FreeFile = native::FS_FreeFile_t(0x5AF990);
		native::FS_Read = native::FS_Read_t(0x5AF650);
		native::FS_FCloseHandle = native::FS_FCloseHandle_t(0x5AF170);

		native::player_die = native::player_die_t(SELECT_VALUE(0x0, 0x503460));

		native::LargeLocalResetToMark = native::LargeLocalResetToMark_t(SELECT_VALUE(0x524350, 0x5B7150));

		native::_longjmp = reinterpret_cast<decltype(longjmp)*>(SELECT_VALUE(0x73AC20, 0x7363BC));

		native::sv_cmd_args = reinterpret_cast<native::CmdArgs*>(SELECT_VALUE(0x1757218, 0x1CAA998));
		native::cmd_args = reinterpret_cast<native::CmdArgs*>(SELECT_VALUE(0x1750750, 0x1C978D0));

		native::scrVarGlob = reinterpret_cast<short*>(SELECT_VALUE(0x19AFC80, 0x1E72180));
		native::scrMemTreePub = reinterpret_cast<char**>(SELECT_VALUE(0x196FB00, 0x1E32000));
		native::scrMemTreeGlob = reinterpret_cast<char*>(SELECT_VALUE(0x186DA00, 0x1D6FF00));

		native::scr_function_stack = reinterpret_cast<native::function_stack_t*>(SELECT_VALUE(0x1BF255C, 0x20B4A5C));
		native::scr_VarPub = reinterpret_cast<native::scrVarPub_t*>(SELECT_VALUE(0x0, 0x208E188));
		native::scr_VmPub = reinterpret_cast<native::scrVmPub_t*>(SELECT_VALUE(0x1BF2580, 0x20B4A80));

		native::g_script_error_level = reinterpret_cast<int*>(SELECT_VALUE(0x1BEFCFC, 0x20B21FC));
		native::g_script_error = reinterpret_cast<jmp_buf*>(SELECT_VALUE(0x1BF1D18, 0x20B4218));

		native::g_classMap = reinterpret_cast<native::scr_classStruct_t*>(SELECT_VALUE(0x92D140, 0x8B4300));

		native::svs_clientCount = reinterpret_cast<int*>(SELECT_VALUE(0x0, 0x4B5CF8C));

		native::levelEntityId = reinterpret_cast<unsigned int*>(SELECT_VALUE(0x1BCBCA4, 0x208E1A4));

		native::mp::svs_clients = reinterpret_cast<native::mp::client_t*>(0x4B5CF90);

		native::g_entities = reinterpret_cast<native::gentity_s*>(0x1A66E28);
		native::sp::g_entities = reinterpret_cast<native::sp::gentity_s*>(0x1197AD8);

		native::sp::g_clients = reinterpret_cast<native::sp::gclient_s*>(0x1381D48);

		native::mp::level = reinterpret_cast<native::level_locals_t*>(0x1C6D4D8);

		native::deferredQueue = reinterpret_cast<native::DeferredQueue*>(SELECT_VALUE(0x0, 0x1D55438));

		native::com_codeTimeScale = reinterpret_cast<float*>(SELECT_VALUE(0x1769F1C, 0x1CEF554));

		native::s_criticalSection = reinterpret_cast<RTL_CRITICAL_SECTION*>(SELECT_VALUE(0x1CD5638, 0x5A91048));

		native::logfile = reinterpret_cast<int*>(SELECT_VALUE(0x176B534, 0x1CF0B78));

		native::fs_searchpaths = reinterpret_cast<native::searchpath_s**>(SELECT_VALUE(0x1C2FE78, 0x59BA858));
		native::fs_gamedir = reinterpret_cast<char*>(SELECT_VALUE(0x1C2B220, 0x59A98F8));
		native::fsh = reinterpret_cast<native::fileHandleData_t*>(SELECT_VALUE(0x1C2B540, 0x59B5F20));
		native::com_fileAccessed = reinterpret_cast<int*>(SELECT_VALUE(0x1C2B328, 0x59A9A04));

		native::threadId = reinterpret_cast<unsigned(*)[native::THREAD_CONTEXT_COUNT]>(SELECT_VALUE(0x18576C8, 0x1D6E448));

		native::initialized_0 = reinterpret_cast<int*>(SELECT_VALUE(0x1CE1CA0, 0x5AA3058));
		native::sys_timeBase = reinterpret_cast<int*>(SELECT_VALUE(0x1CE1C98, 0x5AA3050));
		native::sys_counterBase = reinterpret_cast<unsigned __int64*>(SELECT_VALUE(0x1CE1C90, 0x5AA3048));

		native::dvarCount = reinterpret_cast<int*>(SELECT_VALUE(0x1C42398, 0x59CCDD8));
		native::sortedDvars = reinterpret_cast<native::dvar_t**>(SELECT_VALUE(0x1C423C0, 0x59CCE00));

		native::db_hashCritSect = reinterpret_cast<native::FastCriticalSection*>(SELECT_VALUE(0xFA9E7C, 0x18596E4));

		native::g_assetNames = reinterpret_cast<const char**>(SELECT_VALUE(0x92A688, 0x8AAB30));
	}
}
