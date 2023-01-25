#include <std_include.hpp>

#include "isndcurve.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#define IW4X_SND_CURVE 1

namespace asset_dumpers
{
	void isndcurve::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.sndCurve);

		auto iw4_sndcurve = local_allocator.allocate<iw4::native::SndCurve>();
		auto native_sndcurve = header.sndCurve;
		iw4_sndcurve->filename = local_allocator.duplicate_string(std::format("{}.5x",  native_sndcurve->filename));
		iw4_sndcurve->knotCount = std::min(static_cast<unsigned short>(16), native_sndcurve->knotCount);

		if (native_sndcurve->knotCount > iw4_sndcurve->knotCount)
		{
			console::warn("Warning! The sound curve %s has too many knots. IW4 does not support that many, so it might sound weird.\n", native_sndcurve->filename);
		}

		for (auto side = 0; side < 2; side++)
		{
			for (auto knot = 0; knot < 16; knot++)
			{
				iw4_sndcurve->knots[knot][side] = native_sndcurve->knots[knot][side];
			}
		}

		out.sndCurve = iw4_sndcurve;
	}

	void isndcurve::write(const iw4::native::XAssetHeader& header)
	{
		auto asset = header.sndCurve;

		utils::memory::allocator str_duplicator;
		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		assert(asset->filename);

		output.AddMember("version", IW4X_SND_CURVE, allocator);
		output.AddMember("filename", RAPIDJSON_STR(asset->filename), allocator);
		output.AddMember("knotCount", asset->knotCount, allocator);

		rapidjson::Value knots_array(rapidjson::kArrayType);
		for (auto knot = 0; knot < 16; knot++)
		{
			rapidjson::Value knots_sides_array(rapidjson::kArrayType);
			for (auto side = 0; side < 2; side++)
			{
				knots_sides_array.PushBack(asset->knots[knot][side], allocator);
			}

			knots_array.PushBack(knots_sides_array, allocator);
		}

		output.AddMember("knots", knots_array, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);

		output.Accept(writer);

		utils::io::write_file(std::format("{}/{}/{}.iw4x.json", get_export_path(), game::native::g_assetNames[game::native::ASSET_TYPE_SOUND_CURVE], asset->filename), buff.GetString());
	}

	isndcurve::isndcurve()
	{
	}
}