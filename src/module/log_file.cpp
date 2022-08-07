#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include "game/game.hpp"
#include "game/engine/scoped_critical_section.hpp"

#include "log_file.hpp"
#include "file_system.hpp"

const char* log_file::log_file_name;

int log_file::opening_qconsole = 0;
int log_file::com_console_log_open_failed = 0;

const game::native::dvar_t* log_file::com_logfile;

void log_file::com_open_log_file()
{
	time_t aclock;
	char time_buffer[32]{};

	if (game::native::Sys_IsMainThread() && !opening_qconsole)
	{
		opening_qconsole = 1;
		tm new_time{};

		_time64(&aclock);
		_localtime64_s(&new_time, &aclock);

		file_system::open_file_by_mode(log_file_name, game::native::logfile, game::native::FS_APPEND_SYNC);

		asctime_s(time_buffer, sizeof(time_buffer), &new_time);
		info("logfile opened on %s\n", time_buffer);
		opening_qconsole = 0;
		com_console_log_open_failed = *game::native::logfile == 0;
	}
}

void log_file::com_log_print_message(const std::string& msg)
{
	char print_buffer[0x40]{};

	game::engine::scoped_critical_section crit_sect_lock(game::native::CRITSECT_CONSOLE, game::native::SCOPED_CRITSECT_NORMAL);

	if (!game::native::FS_Initialized())
	{
		return;
	}

	if (!*game::native::logfile)
	{
		com_open_log_file();
	}

	if (*game::native::logfile)
	{
		static auto log_next_time_stamp = true;
		if (log_next_time_stamp)
		{
			const auto len = sprintf_s(print_buffer, "[%10i] ", game::native::Sys_Milliseconds());
			file_system::write(print_buffer, len, *game::native::logfile);
		}

		log_next_time_stamp = (msg.find('\n') != std::string::npos);
		file_system::write(msg.data(), static_cast<int>(msg.size()), *game::native::logfile);
	}
}

void log_file::info(const char* fmt, ...)
{
	char msg[0x1000]{};
	va_list argptr;

	va_start(argptr, fmt);
	vsnprintf_s(msg, _TRUNCATE, fmt, argptr);
	va_end(argptr);

	if (com_logfile && com_logfile->current.integer)
	{
		com_log_print_message(msg);
	}
}

void log_file::post_load()
{
	// The game closes the logfile handle in Com_Quit_f

	com_logfile = game::native::Dvar_RegisterInt("logfile", 1,
		0, 2, 0, "Write to log file - 0 = disabled, 1 = async file write, 2 = Sync every write");

	log_file_name = SELECT_VALUE("console_sp.log", "console_mp.log", "console_mp_dedicated.log");
}

REGISTER_MODULE(log_file)
