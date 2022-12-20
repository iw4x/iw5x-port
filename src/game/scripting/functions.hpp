#pragma once

namespace scripting
{
	std::string find_token(std::uint32_t id);
	std::string find_token_single(std::uint32_t id);
	unsigned int find_token_id(const std::string& name);

	int find_function_index(const std::string& name, bool prefer_global);
}
