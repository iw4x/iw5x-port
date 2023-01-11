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
		std::memcpy(soundCopy, &(loadedSound->sound.data), loadedSound->sound.info.data_len);

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
		assert(header.loadSnd);

		auto loadedSound = header.loadSnd;
		if (!iloadedsound::available_sound_data.contains(loadedSound->name))
		{
			console::error("Tried to save sound %s which was never loaded before!\n", loadedSound->name);
			return;
		}

		char* soundData = iloadedsound::available_sound_data[loadedSound->name];

		utils::stream buffer;
		// --- RIF HEADER
		// ChunkID
		const char* chunkID = "RIFF";
		buffer.saveArray(chunkID, 4);

		// ChunkSize
		int subchunk1Size = 16;
		int subchunk2Size = loadedSound->sound.info.data_len;
		int chunkSize = 4 + (8 + subchunk1Size) + (8 + subchunk2Size);
		buffer.save(chunkSize);

		// Format
		const char* format = "WAVE";
		buffer.saveArray(format, 4);

		// --- FMT SUBCHUNK
		// Subchunk1ID
		const char* subchunk1ID = "fmt "; // Mind the space
		buffer.saveArray(subchunk1ID, 4);

		// Subchunk1Size
		buffer.save(subchunk1Size);

		// AudioFormat
		short audioFormat = static_cast<short>(loadedSound->sound.info.format);
		buffer.saveObject(audioFormat);

		// NumChannels
		short numChannels = static_cast<short>(loadedSound->sound.info.channels);
		buffer.saveObject(numChannels);

		// SampleRate
		int sampleRate = loadedSound->sound.info.rate;
		buffer.save(sampleRate);

		// ByteRate
		int byteRate = loadedSound->sound.info.rate * loadedSound->sound.info.channels * loadedSound->sound.info.bits / 8;
		buffer.save(byteRate);

		// BlockAlign
		short blockAlign = static_cast<short>(loadedSound->sound.info.block_size);
		buffer.saveObject(blockAlign);

		// BitsPerSample
		short bitsPerSample = static_cast<short>(loadedSound->sound.info.bits);
		buffer.saveObject(bitsPerSample);


		// --- DATA SUBCHUNK
		const char* subchunk2ID = "data";
		buffer.saveArray(subchunk2ID, 4);

		// Data
		auto dataLength = loadedSound->sound.info.data_len;
		buffer.save(dataLength);
		buffer.save(soundData, dataLength);

		auto exportPath = get_export_path();
		auto outPath = std::format("{}/loaded_sound/{}", exportPath, loadedSound->name);

		utils::io::write_file(outPath, buffer.toBuffer());
	}

	iloadedsound::iloadedsound()
	{
	}
}