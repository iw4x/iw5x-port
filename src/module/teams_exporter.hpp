#pragma once

#include "module/exporter.hpp"

class teams_exporter final : public module
{
public:
	static void export_team(const std::string& teams);


private:
	static void decide_fallback(const std::string& vocal_prefix);
	static void parse_dump_bodytypes(const std::string& team_name);
	static void change_soundaliases_prefix(std::string& csv_path, const std::string& old_prefix, const std::string& new_prefix);
};

