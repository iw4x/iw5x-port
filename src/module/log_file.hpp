#pragma once

class log_file final : public module
{
public:
	void post_load() override;

	static void com_log_print_message(const std::string& msg);
	static void info(const char* fmt, ...);

	static const game::native::dvar_t* com_logfile;

private:
	static const char* log_file_name;

	static int opening_qconsole;
	static int com_console_log_open_failed;

	static void com_open_log_file();
};
