#pragma once

class exporter final : public module
{
	

#define DEFINE_OG_FUNCTION(name, addr)\
typedef void (*##name##_t)();\
##name##_t name = (##name##_t)addr

public:
	void post_load() override;

private:
	static void load_common_zones();
	static void event_loop();
	static void perform_common_initialization();
	static void dump_map(const command::params& params);
	static void add_commands();
};

