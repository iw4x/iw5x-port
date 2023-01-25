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
		assert(header.loadSnd);

		auto loaded_sound = header.loadSnd;
		if (!iloadedsound::available_sound_data.contains(loaded_sound->name))
		{
			console::error("Tried to save sound %s which was never loaded before!\n", loaded_sound->name);
			return;
		}

		//
		if (header.loadSnd->name == "amb_emitters/emt_mtl_chainlink_rattle2.wav"s)
		{
			printf("");
		}
		//

		char* sound_data = iloadedsound::available_sound_data[loaded_sound->name];

		utils::stream buffer;
		// --- RIF HEADER
		// ChunkID
		const char* chunk_id = "RIFF";
		buffer.saveArray(chunk_id, 4);

		// ChunkSize
		int sub_chunk_1_size = 16;
		int sub_chunk_2_size = loaded_sound->sound.info.data_len;
		int chunk_size = 4 + (8 + sub_chunk_1_size) + (8 + sub_chunk_2_size);
		buffer.save(chunk_size);

		// Format
		const char* format = "WAVE";
		buffer.saveArray(format, 4);

		// --- FMT SUBCHUNK
		// Subchunk1ID
		const char* sub_chunk_1_id = "fmt "; // Mind the space
		buffer.saveArray(sub_chunk_1_id, 4);

		// Subchunk1Size
		buffer.save(sub_chunk_1_size);

		// AudioFormat
		short audio_formt = static_cast<short>(loaded_sound->sound.info.format);
		buffer.saveObject(audio_formt);

		// NumChannels
		short num_channels = static_cast<short>(loaded_sound->sound.info.channels);
		buffer.saveObject(num_channels);

		// SampleRate
		int sampleRate = loaded_sound->sound.info.rate;
		buffer.save(sampleRate);


		auto data_length = loaded_sound->sound.info.data_len;
		assert(data_length / (loaded_sound->sound.info.bits / 8) == loaded_sound->sound.info.samples);

		// ByteRate
		int byteRate = loaded_sound->sound.info.rate * loaded_sound->sound.info.channels * loaded_sound->sound.info.bits / 8;
		buffer.save(byteRate);

		// BlockAlign
		short block_align = static_cast<short>(loaded_sound->sound.info.block_size);
		buffer.saveObject(block_align);

		// BitsPerSample
		short bits_per_sample = static_cast<short>(loaded_sound->sound.info.bits);
		buffer.saveObject(bits_per_sample);


		// --- DATA SUBCHUNK
		const char* sub_chunk_2_id = "data";
		buffer.saveArray(sub_chunk_2_id, 4);

		// Data
		buffer.save(data_length);
		buffer.save(sound_data, data_length);

		auto export_path = get_export_path();
		auto out_path = std::format("{}/loaded_sound/{}", export_path, loaded_sound->name);

		utils::io::write_file(out_path, buffer.toBuffer());
	}

	iloadedsound::iloadedsound()
	{
	}
}