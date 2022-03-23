#pragma once

class test_clients final : public module
{
public:
	void post_load() override;

private:
	static void patch_mp();

	static bool can_add();
	static game::native::gentity_s* sv_add_test_client();
	static void spawn();

	static void scr_shutdown_system_mp_stub(unsigned char sys);

	static void reset_reliable_mp();

	static bool check_timeouts(const game::native::mp::client_t* client);
	static void check_timeouts_stub_mp();
};
