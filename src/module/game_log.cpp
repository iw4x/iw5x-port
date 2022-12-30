#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include "game/game.hpp"

#include <utils/hook.hpp>

#include "game_log.hpp"
#include "scheduler.hpp"
#include "file_system.hpp"
#include "scripting.hpp"
#include "console.hpp"

const game::native::dvar_t* game_log::g_log;
const game::native::dvar_t* game_log::g_logSync;

int game_log::log_file = 0;

void game_log::g_log_printf(const char* fmt, ...)
{
	char buf[1024] = {0};
	char out[1024] = {0};

	va_list va;
	va_start(va, fmt);
	vsnprintf_s(buf, _TRUNCATE, fmt, va);
	va_end(va);

	if (!log_file)
	{
		return;
	}

	const auto time = game::native::mp::level->time / 1000;
	const auto len = sprintf_s(out, "%3i:%i%i %s", time / 60, time % 60 / 10, time % 60 % 10, buf);

	file_system::write(out, len, log_file);
}

void game_log::gscr_log_print()
{
	char buf[1024] = {0};
	std::size_t out_chars = 0;

	for (std::size_t i = 0; i < game::native::Scr_GetNumParam(); ++i)
	{
		const auto* value = game::native::Scr_GetString(i);
		const auto len = std::strlen(value);

		out_chars += len;
		if (out_chars >= sizeof(buf))
		{
			// Do not overflow the buffer
			break;
		}

		strncat_s(buf, value, _TRUNCATE);
	}

	g_log_printf("%s", buf);
}

void game_log::g_init_game_stub()
{
	console::info("------- Game Initialization -------\n");
	console::info("gamename: %s\n", reinterpret_cast<const char*>(0x7FFC68));
	console::info("gamedate: %s\n", __DATE__);

	const auto* log = g_log->current.string;

	if (*log == '\0')
	{
		console::info("Not logging to disk.\n");
	}
	else
	{
		file_system::open_file_by_mode(log, &log_file, game::native::FS_APPEND_SYNC);

		if (!log_file)
		{
			console::info("WARNING: Couldn't open logfile: %s\n", log);
		}
		else
		{
			console::info("Logging to disk: '%s'.\n", log);
			g_log_printf("------------------------------------------------------------\n");
			g_log_printf("InitGame\n");
		}
	}	

	utils::hook::invoke<void>(0x5C2800);
}

void game_log::g_shutdown_game_stub(int free_scripts)
{
	console::info("==== ShutdownGame (%d) ====\n", free_scripts);

	if (log_file)
	{
		g_log_printf("ShutdownGame:\n");
		g_log_printf("------------------------------------------------------------\n");

		game::native::FS_FCloseFile(log_file);
		log_file = 0;
	}

	utils::hook::invoke<void>(0x50C100, free_scripts);
}

void game_log::exit_level_stub()
{
	g_log_printf("ExitLevel: executed\n");
}

void game_log::post_load()
{
	if (!game::is_mp())
	{
		return;
	}

	utils::hook::set<void(*)()>(0x8AC858, gscr_log_print);

	utils::hook(0x50D135, g_init_game_stub, HOOK_CALL).install()->quick();
	scripting::on_shutdown(g_shutdown_game_stub);

	utils::hook(0x50D5F4, exit_level_stub, HOOK_JUMP).install()->quick();

	scheduler::once([]
	{
		g_log = game::native::Dvar_RegisterString("g_log", "games_mp.log",
			game::native::DVAR_ARCHIVE, "Log file name");
		g_logSync = game::native::Dvar_RegisterBool("g_logSync", false,
			game::native::DVAR_NONE, "Enable synchronous logging");
	}, scheduler::pipeline::main);
}

REGISTER_MODULE(game_log)
