#pragma once

class game_log final : public module
{
public:
	static_assert(offsetof(game::native::level_locals_t, time) == 0x4A8);

	void post_load() override;

	static void g_log_printf(const char* fmt, ...);

private:
	static const game::native::dvar_t* g_log;
	static const game::native::dvar_t* g_logSync;

	static int log_file;

	static void gscr_log_print();

	static void g_init_game_stub();

	static void g_shutdown_game_stub(int free_scripts);

	static void exit_level_stub();
};
