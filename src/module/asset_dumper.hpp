#pragma once

#include "game/structs.iw4.hpp"

class asset_dumper : public module
{

#define RAPIDJSON_STR(str) (str == nullptr ? rapidjson::Value(rapidjson::kNullType) : static_cast<rapidjson::Value>(rapidjson::StringRef(str)))

public:

	asset_dumper();

	void dump(game::native::XAssetHeader header)
	{
		iw4::native::XAssetHeader out{};
		convert(header, out);
		write(out);
	}

protected:
	std::string export_path() {
		return "out";
	};

	virtual void convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out) = 0;
	virtual void write(const iw4::native::XAssetHeader& header) = 0;
};

