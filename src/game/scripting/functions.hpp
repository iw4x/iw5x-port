#pragma once
#include "game/game.hpp"

namespace scripting
{
	std::string find_token(std::uint32_t id);
	std::string find_token_single(std::uint32_t id);
	unsigned int find_token_id(const std::string& name);

	int find_function_index(const std::string& name, bool prefer_global);
	game::native::scr_call_t get_function_by_index(std::uint32_t index);
	game::native::scr_call_t find_function(const std::string& name, bool prefer_global);
}
