#include <std_include.hpp>

#include "iloadedsound.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include <module/console.hpp>

namespace asset_dumpers
{
	std::unordered_map<std::string, char*> iloadedsound::available_sound_data{};

	void iloadedsound::duplicate_sound_data(game::native::LoadedSound* loadedSound)
	{
		char* soundCopy = new char[loadedSound->sound.info.data_len];
		ZeroMemory(soundCopy, loadedSound->sound.info.data_len);
		std::memcpy(soundCopy, loadedSound->sound.data, loadedSound->sound.info.data_len);

		available_sound_data.insert({ loadedSound->name, soundCopy });
	}

	void iloadedsound::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.loadSnd);

		// No conversion
		out.loadSnd = header.loadSnd;
	}

	void iloadedsound::write(const iw4::native::XAssetHeader& header)
	{
		// copy sound data
		if (header.loadSnd)
		{
			header.loadSnd->sound.data = available_sound_data[header.loadSnd->name];

			[[maybe_unused]] bool result = exporter::get_api()->write(iw4::native::ASSET_TYPE_LOADED_SOUND, header.data);
			assert(result);
		}
		else
		{
			console::error("Tried to dump null loaded sound");
		}
	}

	iloadedsound::iloadedsound()
	{
	}
}