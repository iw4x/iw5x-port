#pragma once

#include "game/structs.iw4.hpp"
#include <utils/memory.hpp>

class asset_dumper : public module
{

#define RAPIDJSON_STR(str) (str == nullptr ? rapidjson::Value(rapidjson::kNullType) : static_cast<rapidjson::Value>(rapidjson::StringRef(str)))

public:

	asset_dumper();

	iw4::native::XAssetHeader dump(game::native::XAssetHeader header, bool clear_memory = false)
	{
		if (converted_assets.contains(header.data) && !clear_memory)
		{
			return iw4::native::XAssetHeader{ converted_assets.at(header.data) };
		}

		iw4::native::XAssetHeader out{};

		convert(header, out);

		if (!clear_memory)
		{
			converted_assets[header.data] = out.data;
		}

		write(out);
		
		if (clear_memory)
		{
			local_allocator.clear();
		}

		return out;
	}

protected:

	utils::memory::allocator local_allocator;
	std::unordered_map<void*, void*> converted_assets{};

	std::string export_path() {
		return "out";
	};

	virtual void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) = 0;
	virtual void write(const iw4::native::XAssetHeader& header) = 0;
};

