#include <std_include.hpp>
#include "functions.hpp"

#include <utils/string.hpp>

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

namespace scripting
{
	int find_function_index(const std::string& name, const bool prefer_global)
	{
		const auto target = utils::string::to_lower(name);
		auto first = xsk::gsc::iw5::resolver::function_id;
		auto second = xsk::gsc::iw5::resolver::method_id;
		if (!prefer_global)
		{
			std::swap(first, second);
		}

		const auto first_res = first(target);
		if (first_res)
		{
			return first_res;
		}

		const auto second_res = second(target);
		if (second_res)
		{
			return second_res;
		}

		return -1;
	}

	std::uint32_t parse_token_id(const std::string& name)
	{
		if (name.starts_with("_ID"))
		{
			return static_cast<std::uint32_t>(std::strtol(name.substr(3).data(), nullptr, 10));
		}

		return 0;
	}

	std::string find_token(std::uint32_t id)
	{
		return xsk::gsc::iw5::resolver::token_name(static_cast<std::uint16_t>(id));
	}

	std::string find_token_single(std::uint32_t id)
	{
		return xsk::gsc::iw5::resolver::token_name(static_cast<std::uint16_t>(id));
	}

	unsigned int find_token_id(const std::string& name)
	{
		const auto id = xsk::gsc::iw5::resolver::token_id(name);
		if (id)
		{
			return id;
		}

		const auto parsed_id = parse_token_id(name);
		if (parsed_id)
		{
			return parsed_id;
		}

		return game::native::SL_GetCanonicalString(name.data());
	}

	game::native::scr_call_t get_function_by_index(const std::uint32_t index)
	{
		static const auto function_table = SELECT_VALUE(0x186C68C, 0x1D6EB34, 0x1C152A4);
		static const auto method_table = SELECT_VALUE(0x184CDB0, 0x1D4F258, 0x1BF59C8);

		if (index < 0x1C7)
		{
			return reinterpret_cast<game::native::scr_call_t*>(function_table)[index - 1];
		}

		return reinterpret_cast<game::native::scr_call_t*>(method_table)[index];
	}

	game::native::scr_call_t find_function(const std::string& name, const bool prefer_global)
	{
		const auto index = find_function_index(name, prefer_global);
		if (index < 0) return nullptr;

		return get_function_by_index(index);
	}
}
