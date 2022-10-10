#pragma once

class dvar final : public module
{
public:
	void post_load() override;

	static int values_equal(unsigned char type, game::native::DvarValue val0, game::native::DvarValue val1);
	static bool has_latched_value(const game::native::dvar_t* dvar);

private:
	static void com_dvar_dump_single(const game::native::dvar_t* dvar, void* user_data);
	static void com_dvar_dump(int channel, const char* match);

	static void list_single(const game::native::dvar_t* dvar, void* user_data);

	static void dump_f(const command::params& params);
	static void list_f(const command::params& params);
};
