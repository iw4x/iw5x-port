#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include <utils/hook.hpp>

#include "game/game.hpp"
#include "log_file.hpp"
#include "scheduler.hpp"

const game::native::dvar_t* log_file::g_log;
const game::native::dvar_t* log_file::g_logSync;

FILE* log_file::log_fsh = nullptr;

void log_file::g_log_printf(const char* fmt, ...)
{
	char buf[1024] = {0};
	char out[1024] = {0};

	va_list va;
	va_start(va, fmt);
	vsnprintf_s(buf, _TRUNCATE, fmt, va);
	va_end(va);

	if (log_fsh == nullptr)
	{
		return;
	}

	_snprintf_s(out, _TRUNCATE, "%3i:%i%i %s",
		game::native::level->time / 1000 / 60,
		game::native::level->time / 1000 % 60 / 10,
		game::native::level->time / 1000 % 60 % 10,
		buf);

	fprintf(log_fsh, "%s", out);
	fflush(log_fsh);
}

void log_file::gscr_log_print()
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

void log_file::g_init_game_stub()
{
	printf("------- Game Initialization -------\n");
	printf("gamename: %s", reinterpret_cast<const char*>(0x7FFC68));
	printf("gamedate: %s\n", __DATE__);

	const auto* log = g_log->current.string;

	if (*log == '\0')
	{
		printf("Not logging to disk.\n");
	}
	else
	{
		log_fsh = _fsopen(log, "a", _SH_DENYWR);

		if (log_fsh == nullptr)
		{
			printf("WARNING: Couldn't open logfile: %s\n", log);
		}
		else
		{
			printf("Logging to disk: '%s'.\n", log);
			g_log_printf("------------------------------------------------------------\n");
			g_log_printf("InitGame\n");
		}
	}	

	utils::hook::invoke<void>(0x5C2800);
}

void log_file::g_shutdown_game_stub(int free_scripts)
{
	printf("==== ShutdownGame (%d) ====\n", free_scripts);

	if (log_fsh != nullptr)
	{
		g_log_printf("ShutdownGame:\n");
		g_log_printf("------------------------------------------------------------\n");

		fclose(log_fsh);
		log_fsh = nullptr;
	}

	utils::hook::invoke<void>(0x50C100, free_scripts);
}

void log_file::exit_level_stub()
{
	printf("ExitLevel: executed\n");
}

void log_file::post_load()
{
	if (!game::is_mp())
	{
		return;
	}

	utils::hook::set<void(*)()>(0x8AC858, gscr_log_print);

	utils::hook(0x50D135, g_init_game_stub, HOOK_CALL).install()->quick();

	utils::hook(0x573C82, g_shutdown_game_stub, HOOK_CALL).install()->quick();
	utils::hook(0x573D3A, g_shutdown_game_stub, HOOK_CALL).install()->quick();

	utils::hook(0x50D5F4, exit_level_stub, HOOK_JUMP).install()->quick();

	scheduler::once([]
	{
		g_log = game::native::Dvar_RegisterString("g_log", "games_mp.log",
			game::native::DVAR_ARCHIVE, "Log file name");
		g_logSync = game::native::Dvar_RegisterBool("g_logSync", false,
			game::native::DVAR_NONE, "Enable synchronous logging");
	}, scheduler::pipeline::main);
}

REGISTER_MODULE(log_file)
