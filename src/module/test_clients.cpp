#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>
#include <utils/string.hpp>

#include "game/game.hpp"
#include "test_clients.hpp"
#include "command.hpp"
#include "scheduler.hpp"

bool test_clients::can_add()
{
	auto i = 0;
	while (i < *game::native::svs_clientCount)
	{
		if (game::native::mp::svs_clients[i].header.state == game::native::clientState_t::CS_FREE)
		{
			// Free slot was found
			break;
		}

		++i;
	}

	return i < *game::native::svs_clientCount;
}

game::native::gentity_s* test_clients::sv_add_test_client()
{
	if (!test_clients::can_add())
	{
		return nullptr;
	}

	assert(game::native::Dvar_FindVar("sv_running")->current.enabled);
	assert(game::native::Dvar_FindVar("sv_maxclients")->current.integer <= *game::native::svs_clientCount);

	static auto bot_port = 0;
	char user_info[1024] = {0};
	char xuid[32] = {0};

	const std::uint64_t random_xuid = std::rand();
	game::native::XUIDToString(&random_xuid, xuid);

	// Most basic string the game will accept.
	_snprintf_s(user_info, _TRUNCATE,
		"connect bot%d \"\\invited\\0\\cg_predictItems\\1\\cl_anonymous\\0\\color\\4\\head\\default\\model\\multi\\snaps\\20\\rate\\5000\\name\\bot%d\\xuid\\%s\\xnaddr\\%s\\natType\\2\\protocol\\%d\\checksum\\%u\\statver\\%d %u\\qport\\%d\"",
		bot_port, bot_port, xuid, "", game::native::GetProtocolVersion(), game::native::BG_NetDataChecksum(),
		game::native::LiveStorage_GetPersistentDataDefVersion(), game::native::LiveStorage_GetPersistentDataDefFormatChecksum(), bot_port);

	game::native::netadr_s adr;
	std::memset(&adr, 0, sizeof(game::native::netadr_s));

	game::native::SV_Cmd_TokenizeString(user_info);

	adr.port = static_cast<std::uint16_t>(bot_port++);
	game::native::NetAdr_SetType(&adr, game::native::netadrtype_t::NA_BOT);

	game::native::SV_DirectConnect(adr);

	game::native::SV_Cmd_EndTokenizedString();

	// Find the bot
	auto idx = 1;
	for (idx = 0; idx < *game::native::svs_clientCount; idx++)
	{
		if (game::native::mp::svs_clients[idx].header.state == game::native::clientState_t::CS_FREE)
			continue;

		if (game::native::mp::svs_clients[idx].header.netchan.remoteAddress.type == adr.type
			&& game::native::mp::svs_clients[idx].header.netchan.remoteAddress.port == adr.port)
			break; // Found them
	}

	if (idx == *game::native::svs_clientCount)
	{
		// Failed to add test client
		return nullptr;
	}

	const auto client = &game::native::mp::svs_clients[idx];
	client->bIsTestClient = 1;

	game::native::SV_SendClientGameState(client);

	game::native::usercmd_s cmd;
	std::memset(&cmd, 0, sizeof(game::native::usercmd_s));

	game::native::SV_ClientEnterWorld(client, &cmd);

	assert(client->gentity != nullptr);

	return client->gentity;
}

void test_clients::gscr_add_test_client()
{
	const auto* ent = test_clients::sv_add_test_client();

	if (ent != nullptr)
	{
		game::native::Scr_AddEntityNum(ent->s.number, 0);
	}
}

void test_clients::spawn(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		scheduler::once([]()
		{
			auto* ent = sv_add_test_client();
			if (ent == nullptr) return;

			game::native::Scr_AddEntityNum(ent->s.number, 0);
			scheduler::once([ent]()
			{
				game::native::Scr_AddString("autoassign");
				game::native::Scr_AddString("team_marinesopfor");
				game::native::Scr_Notify(ent, static_cast<std::uint16_t>(game::native::SL_GetString("menuresponse", 0)), 2);

				scheduler::once([ent]()
				{
					game::native::Scr_AddString(utils::string::va("class%i", std::rand() % 5));
					game::native::Scr_AddString("changeclass");
					game::native::Scr_Notify(ent, static_cast<std::uint16_t>(game::native::SL_GetString("menuresponse", 0)), 2);
				}, scheduler::pipeline::server, 2s);

			}, scheduler::pipeline::server, 1s);

		}, scheduler::pipeline::server, 2s * (i + 1));
	}
}

void test_clients::scr_shutdown_system_mp_stub(unsigned char sys)
{
	game::native::SV_DropAllBots();
	reinterpret_cast<void (*)(unsigned char)>(0x569E30)(sys);
}

__declspec(naked) void test_clients::reset_reliable_mp()
{
	// client_t ptr is in esi
	__asm
	{
		cmp [esi + 0x41CB4], 1 // bIsTestClient
		jnz resume

		mov eax, [esi + 0x20E70] // Move reliableSequence to eax
		mov [esi + 0x20E74], eax // Move eax to reliableAcknowledge

	resume:
		push 0x5CE740 // Sys_Milliseconds
		retn
	}
}

bool test_clients::check_timeouts(const game::native::mp::client_t* client)
{
	return (!client->bIsTestClient || client->header.state == game::native::clientState_t::CS_ZOMBIE)
		&& client->header.netchan.remoteAddress.type != game::native::netadrtype_t::NA_LOOPBACK;
}

__declspec(naked) void test_clients::check_timeouts_stub_mp()
{
	__asm
	{
		push eax
		pushad

		lea esi, [ebx - 0x2146C]
		push esi
		call test_clients::check_timeouts
		add esp, 4

		mov [esp + 0x20], eax
		popad
		pop eax

		test al, al

		push 0x576DD3
		retn
	}
}

void test_clients::post_load()
{
	if (game::is_mp()) this->patch_mp();
	else return; // No sp/dedi bots for now :(

	command::add("spawnBot", [](const command::params& params)
	{
		if (params.size() < 2)
		{
			return;
		}

		const auto count = std::atoi(params.get(1));
		test_clients::spawn(count);
	});
}

void test_clients::patch_mp()
{
	utils::hook::nop(0x639803, 5); // LiveSteamServer_RunFrame
	utils::hook::nop(0x5CD65A, 5); // Do not crash for silly fatal error

	utils::hook::set<BYTE>(0x572879, 0xEB); // Skip checks in SV_DirectConnect
	utils::hook::set<BYTE>(0x5728D4, 0xEB);
	utils::hook::set<BYTE>(0x57293D, 0xEB);

	utils::hook(0x50C147, &test_clients::scr_shutdown_system_mp_stub, HOOK_CALL).install()->quick(); // G_ShutdownGame
	utils::hook(0x57BBF9, &test_clients::reset_reliable_mp, HOOK_CALL).install()->quick(); // SV_SendMessageToClient
	utils::hook(0x576DCC, &test_clients::check_timeouts_stub_mp, HOOK_JUMP).install()->quick(); // SV_CheckTimeouts

	// Replace nullsubbed gsc func "GScr_AddTestClient" with our spawn
	utils::hook::set<void(*)()>(0x8AC8DC, test_clients::gscr_add_test_client);
}

REGISTER_MODULE(test_clients);
