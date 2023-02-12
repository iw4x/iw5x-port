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
		[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_SOUND_CURVE, header.data);
		assert(result);
	}

	isndcurve::isndcurve()
	{
	}
}