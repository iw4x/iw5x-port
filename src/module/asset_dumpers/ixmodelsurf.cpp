#include <std_include.hpp>

#include "ixmodelsurf.hpp"
#include "../asset_dumper.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "module/console.hpp"
#include "module/command.hpp"

namespace asset_dumpers
{

	void ixmodelsurf::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.modelSurfs);

		auto iw4_model_surfs = local_allocator.allocate<iw4::native::XModelSurfs>();
		auto native_model_surfs = header.modelSurfs;

#define SET_MEMBER_NO_CONVERSION(member_name) iw4_physpreset->##member_name## = (native_physpreset->##member_name##)

		out.modelSurfs = iw4_model_surfs;
	}

	void ixmodelsurf::write(const iw4::native::XAssetHeader& header)
	{
		// actually... don't!
	}

	ixmodelsurf::ixmodelsurf()
	{
	}
}