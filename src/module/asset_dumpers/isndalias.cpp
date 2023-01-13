#include <std_include.hpp>

#include "isndalias.hpp"
#include "../asset_dumper.hpp"
#include "../exporter.hpp"
#include "../console.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace asset_dumpers
{
	const std::unordered_map<unsigned int, unsigned int> isndalias::channel_mapping
	{
		{ game::native::SndChannel::SND_CHANNEL_PHYSICS, iw4::native::SndChannel::SND_CHANNEL_PHYSICS },
		{ game::native::SndChannel::SND_CHANNEL_AMBDIST1, iw4::native::SndChannel::SND_CHANNEL_AMBDIST1 },
		{ game::native::SndChannel::SND_CHANNEL_AMBDIST2, iw4::native::SndChannel::SND_CHANNEL_AMBDIST2 },
		{ game::native::SndChannel::SND_CHANNEL_AUTO, iw4::native::SndChannel::SND_CHANNEL_AUTO },
		{ game::native::SndChannel::SND_CHANNEL_AUTO2, iw4::native::SndChannel::SND_CHANNEL_AUTO2 },
		{ game::native::SndChannel::SND_CHANNEL_AUTODOG, iw4::native::SndChannel::SND_CHANNEL_AUTODOG },
		{ game::native::SndChannel::SND_CHANNEL_BULLETIMPACT, iw4::native::SndChannel::SND_CHANNEL_BULLETIMPACT },
		{ game::native::SndChannel::SND_CHANNEL_BULLETWHIZBY, iw4::native::SndChannel::SND_CHANNEL_BULLETWHIZBY },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSIVEIMPACT, iw4::native::SndChannel::SND_CHANNEL_EXPLOSIVEIMPACT },
		{ game::native::SndChannel::SND_CHANNEL_ELEMENT, iw4::native::SndChannel::SND_CHANNEL_ELEMENT },
		{ game::native::SndChannel::SND_CHANNEL_AUTO2D, iw4::native::SndChannel::SND_CHANNEL_AUTO2D },
		{ game::native::SndChannel::SND_CHANNEL_VEHICLE, iw4::native::SndChannel::SND_CHANNEL_VEHICLE },
		{ game::native::SndChannel::SND_CHANNEL_VEHICLELIMITED, iw4::native::SndChannel::SND_CHANNEL_VEHICLELIMITED },
		{ game::native::SndChannel::SND_CHANNEL_MENU, iw4::native::SndChannel::SND_CHANNEL_MENU },
		{ game::native::SndChannel::SND_CHANNEL_BODY, iw4::native::SndChannel::SND_CHANNEL_BODY },
		{ game::native::SndChannel::SND_CHANNEL_BODY2D, iw4::native::SndChannel::SND_CHANNEL_BODY2D },
		{ game::native::SndChannel::SND_CHANNEL_RELOAD, iw4::native::SndChannel::SND_CHANNEL_RELOAD },
		{ game::native::SndChannel::SND_CHANNEL_RELOAD2D, iw4::native::SndChannel::SND_CHANNEL_RELOAD2D },
		{ game::native::SndChannel::SND_CHANNEL_ITEM, iw4::native::SndChannel::SND_CHANNEL_ITEM },
		{ game::native::SndChannel::SND_CHANNEL_EFFECTS1, iw4::native::SndChannel::SND_CHANNEL_EFFECTS1 },
		{ game::native::SndChannel::SND_CHANNEL_EFFECTS2, iw4::native::SndChannel::SND_CHANNEL_EFFECTS2 },
		{ game::native::SndChannel::SND_CHANNEL_WEAPON, iw4::native::SndChannel::SND_CHANNEL_WEAPON },
		{ game::native::SndChannel::SND_CHANNEL_WEAPON2D, iw4::native::SndChannel::SND_CHANNEL_WEAPON2D },
		{ game::native::SndChannel::SND_CHANNEL_NONSHOCK, iw4::native::SndChannel::SND_CHANNEL_NONSHOCK },
		{ game::native::SndChannel::SND_CHANNEL_VOICE, iw4::native::SndChannel::SND_CHANNEL_VOICE },
		{ game::native::SndChannel::SND_CHANNEL_LOCAL, iw4::native::SndChannel::SND_CHANNEL_LOCAL },
		{ game::native::SndChannel::SND_CHANNEL_LOCAL2, iw4::native::SndChannel::SND_CHANNEL_LOCAL2 },
		{ game::native::SndChannel::SND_CHANNEL_LOCAL3, iw4::native::SndChannel::SND_CHANNEL_LOCAL3 },
		{ game::native::SndChannel::SND_CHANNEL_AMBIENT, iw4::native::SndChannel::SND_CHANNEL_AMBIENT },
		{ game::native::SndChannel::SND_CHANNEL_HURT, iw4::native::SndChannel::SND_CHANNEL_HURT },
		{ game::native::SndChannel::SND_CHANNEL_PLAYER1, iw4::native::SndChannel::SND_CHANNEL_PLAYER1 },
		{ game::native::SndChannel::SND_CHANNEL_PLAYER2, iw4::native::SndChannel::SND_CHANNEL_PLAYER2 },
		{ game::native::SndChannel::SND_CHANNEL_MUSIC, iw4::native::SndChannel::SND_CHANNEL_MUSIC },
		{ game::native::SndChannel::SND_CHANNEL_MUSICNOPAUSE, iw4::native::SndChannel::SND_CHANNEL_MUSICNOPAUSE },
		{ game::native::SndChannel::SND_CHANNEL_MISSION, iw4::native::SndChannel::SND_CHANNEL_MISSION },
		{ game::native::SndChannel::SND_CHANNEL_ANNOUNCER, iw4::native::SndChannel::SND_CHANNEL_ANNOUNCER },
		{ game::native::SndChannel::SND_CHANNEL_SHELLSHOCK, iw4::native::SndChannel::SND_CHANNEL_SHELLSHOCK },

		// 🆕
		{ game::native::SndChannel::SND_CHANNEL_ALARM1, iw4::native::SndChannel::SND_CHANNEL_AUTO },

		{ game::native::SndChannel::SND_CHANNEL_EXPLOSIONDIST1, iw4::native::SndChannel::SND_CHANNEL_BODY },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSIONDIST2, iw4::native::SndChannel::SND_CHANNEL_BODY },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSIVEIMPACT, iw4::native::SndChannel::SND_CHANNEL_BODY },

		{ game::native::SndChannel::SND_CHANNEL_ELEMENT, iw4::native::SndChannel::SND_CHANNEL_ELEMENT },
		{ game::native::SndChannel::SND_CHANNEL_ELEMENT_INT, iw4::native::SndChannel::SND_CHANNEL_ELEMENT },
		{ game::native::SndChannel::SND_CHANNEL_ELEMENT_EXT, iw4::native::SndChannel::SND_CHANNEL_ELEMENT },

		{ game::native::SndChannel::SND_CHANNEL_BULLETFLESH1, iw4::native::SndChannel::SND_CHANNEL_BULLETIMPACT },
		{ game::native::SndChannel::SND_CHANNEL_BULLETFLESH2, iw4::native::SndChannel::SND_CHANNEL_BULLETIMPACT },

		{ game::native::SndChannel::SND_CHANNEL_EXPLOSION1, iw4::native::SndChannel::SND_CHANNEL_WEAPON },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSION2, iw4::native::SndChannel::SND_CHANNEL_WEAPON },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSION3, iw4::native::SndChannel::SND_CHANNEL_WEAPON },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSION4, iw4::native::SndChannel::SND_CHANNEL_WEAPON },
		{ game::native::SndChannel::SND_CHANNEL_EXPLOSION5, iw4::native::SndChannel::SND_CHANNEL_WEAPON },

		{ game::native::SndChannel::SND_CHANNEL_EFFECTS3, iw4::native::SndChannel::SND_CHANNEL_EFFECTS2 },
		{ game::native::SndChannel::SND_CHANNEL_EFFECTS2D1, iw4::native::SndChannel::SND_CHANNEL_WEAPON2D },
		{ game::native::SndChannel::SND_CHANNEL_EFFECTS2D2, iw4::native::SndChannel::SND_CHANNEL_WEAPON2D },
		{ game::native::SndChannel::SND_CHANNEL_NORESTRICT, iw4::native::SndChannel::SND_CHANNEL_AUTO },
		{ game::native::SndChannel::SND_CHANNEL_NORESTRICT2D, iw4::native::SndChannel::SND_CHANNEL_AUTO2D },
		{ game::native::SndChannel::SND_CHANNEL_AIRCRAFT, iw4::native::SndChannel::SND_CHANNEL_AUTO },
		{ game::native::SndChannel::SND_CHANNEL_VEHICLE2D, iw4::native::SndChannel::SND_CHANNEL_AUTO2D },
		{ game::native::SndChannel::SND_CHANNEL_WEAPON_DIST, iw4::native::SndChannel::SND_CHANNEL_AUTO2D },
		{ game::native::SndChannel::SND_CHANNEL_WEAPON_MID, iw4::native::SndChannel::SND_CHANNEL_WEAPON },

		{ game::native::SndChannel::SND_CHANNEL_NONSHOCK2, iw4::native::SndChannel::SND_CHANNEL_NONSHOCK },
		{ game::native::SndChannel::SND_CHANNEL_GRONDO3D, iw4::native::SndChannel::SND_CHANNEL_AUTO },
		{ game::native::SndChannel::SND_CHANNEL_GRONDO2D, iw4::native::SndChannel::SND_CHANNEL_AUTO2D },

		{ game::native::SndChannel::SND_CHANNEL_CRITICAL, iw4::native::SndChannel::SND_CHANNEL_MISSION },

	};

	void isndalias::convert(const game::native::XAssetHeader& header, iw4::native::XAssetHeader& out)
	{
		assert(header.sound);

		auto iw4_sound = local_allocator.allocate<iw4::native::snd_alias_list_t>();
		const auto native_sound = header.sound;

		iw4_sound->aliasName = native_sound->aliasName;
		iw4_sound->count = native_sound->count;
		iw4_sound->head = local_allocator.allocate_array<iw4::native::snd_alias_t>(iw4_sound->count);

		for (size_t i = 0; i < iw4_sound->count; i++)
		{
			auto iw4_alias = &iw4_sound->head[i];
			auto native_alias = &native_sound->head[i];

			///
			if (native_alias->aliasName == "emt_mx_airport_dining"s)
			{
				printf("");
			}
			///

			iw4_alias->aliasName = native_alias->aliasName;
			iw4_alias->subtitle = native_alias->subtitle;
			iw4_alias->secondaryAliasName = native_alias->secondaryAliasName;
			iw4_alias->chainAliasName = native_alias->chainAliasName;
			iw4_alias->soundFile = native_alias->soundFile;
			iw4_alias->sequence = native_alias->sequence;

			iw4_alias->volMin = native_alias->volMin;
			iw4_alias->volMax = native_alias->volMax;
			iw4_alias->pitchMin = native_alias->pitchMin;
			iw4_alias->pitchMax = native_alias->pitchMax;
			iw4_alias->distMin = native_alias->distMin;
			iw4_alias->distMax = native_alias->distMax;

			assert(native_alias->flags.channel == native_alias->flags.channel);

			auto native_channel = static_cast<game::native::SndChannel>(native_alias->flags.channel);

			assert(native_channel < game::native::SndChannel::SND_CHANNEL_COUNT);
			assert(native_channel >= 0);

			assert(native_alias->flags.type == native_alias->soundFile->type);

			if (!channel_mapping.contains(native_channel))
			{
				assert(false);
			}

			iw4_alias->flags.intValue = 0;
			iw4_alias->flags.looping = native_alias->flags.looping;
			iw4_alias->flags.isMaster = native_alias->flags.isMaster;
			iw4_alias->flags.isSlave = native_alias->flags.isSlave;
			iw4_alias->flags.fullDryLevel = native_alias->flags.fullDryLevel;
			iw4_alias->flags.noWetLevel = native_alias->flags.noWetLevel;
			iw4_alias->flags.type = native_alias->flags.type;
			iw4_alias->flags.unknown = native_alias->flags.unknown;
			iw4_alias->flags.unk_is3D = native_alias->flags.unk_is3D;
			iw4_alias->flags.channel = channel_mapping.at(native_channel);

			assert((native_alias->flags.intValue & 0x180) == (iw4_alias->flags.intValue & 0x180));

			assert(iw4_alias->flags.channel < iw4::native::SndChannel::SND_CHANNEL_COUNT);
			assert(iw4_alias->flags.channel >= 0);

			iw4_alias->slavePercentage = native_alias->slavePercentage;
			iw4_alias->probability = native_alias->probability;
			iw4_alias->lfePercentage = native_alias->lfePercentage;
			iw4_alias->centerPercentage = native_alias->centerPercentage;
			iw4_alias->startDelay = native_alias->startDelay;

			iw4_alias->volumeFalloffCurve = local_allocator.allocate<iw4::native::SndCurve>();
			iw4_alias->volumeFalloffCurve->filename = native_alias->volumeFalloffCurve->filename;
			iw4_alias->volumeFalloffCurve->knotCount = std::min(static_cast<unsigned short>(8), native_alias->volumeFalloffCurve->knotCount);

			if (native_alias->volumeFalloffCurve->knotCount > iw4_alias->volumeFalloffCurve->knotCount)
			{
				console::warn("Warning! The sound %s has too many knots in its volume falloff curve. IW4 does not support that many, so it might sound weird.\n", iw4_alias->aliasName);
			}

			for (auto side = 0; side < 2; side++)
			{
				for (auto knot = 0; knot < 8;knot++)
				{
					iw4_alias->volumeFalloffCurve->knots[knot][side] = native_alias->volumeFalloffCurve->knots[knot][side];
				}
			}

			iw4_alias->envelopMin = native_alias->envelopMin;
			iw4_alias->envelopMax = native_alias->envelopMax;
			iw4_alias->envelopPercentage = native_alias->envelopPercentage;

			iw4_alias->speakerMap = native_alias->speakerMap;

			if (iw4_alias->soundFile)
			{
				switch (iw4_alias->soundFile->type)
				{
					// LOADED
				case iw4::native::snd_alias_type_t::SAT_LOADED:
				{
					// Save the LoadedSound subasset
					exporter::dump(game::native::XAssetType::ASSET_TYPE_LOADED_SOUND, { iw4_alias->soundFile->u.loadSnd });
				}
					break;

					// STREAMED
				case iw4::native::snd_alias_type_t::SAT_STREAMED:
				{
					std::string file_name = iw4_alias->soundFile->u.streamSnd.name;

					if (iw4_alias->soundFile->u.streamSnd.dir)
					{
						file_name = std::format("{}/{}", iw4_alias->soundFile->u.streamSnd.dir, file_name);
					}

					auto fullPath = std::format("{}/sound/{}", get_export_path(), file_name);
					auto destinationDirectory = std::format("{}/sound/{}", get_export_path(), iw4_alias->soundFile->u.streamSnd.dir);
					auto internalPath = std::format("sound/{}", file_name);

					utils::io::create_directory(destinationDirectory);
					std::ofstream destination(fullPath, std::ios::binary);

					auto contents = game::native::filesystem_read_big_file(internalPath.data(), game::native::FS_THREAD_DATABASE);

					destination.write(contents.data(), contents.size());

					break;
				}

				default:
					console::error("Error dumping sound alias %s: unknown format %d\n", iw4_alias->aliasName, iw4_alias->soundFile->type);
					return;
				}
			}
			else
			{
				console::error("Error dumping sound alias %s: NULL soundfile!\n", iw4_alias->aliasName);
				return;
			}
		}

		out.sound = iw4_sound;
	}

	void isndalias::write(const iw4::native::XAssetHeader& header)
	{
		auto ents = header.sound;

		if (ents->count == 0) {
			// Very weird but it happens, notably on mp_crash_snow
			// Soundaliases with a zero-count list crash iw4 so we skip them
			// They should be empty anyway
			return;
		}

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();
		utils::memory::allocator str_duplicator;

		// Format is pretty transparent from iw3 to iw4, so no conversion is necessary!
		rapidjson::Value head(rapidjson::kArrayType);

		for (size_t i = 0; i < ents->count; i++)
		{
			auto alias = ents->head[i];

			rapidjson::Value channelMaps(rapidjson::kArrayType);

			for (size_t j = 0; j < 2; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					auto channel_map = alias.speakerMap->channelMaps[j][k];
					rapidjson::Value speakers(rapidjson::kArrayType);

					for (auto speakerIndex = 0; speakerIndex < channel_map.speakerCount; speakerIndex++)
					{
						auto game_speaker = channel_map.speakers[speakerIndex];

						rapidjson::Value speaker(rapidjson::kObjectType);
						speaker.AddMember("levels0", game_speaker.numLevels > 0 ? game_speaker.levels[0] : 0, allocator);
						speaker.AddMember("levels1", game_speaker.numLevels > 1 ? game_speaker.levels[1] : 0, allocator);
						speaker.AddMember("numLevels", game_speaker.numLevels, allocator);
						speaker.AddMember("speaker", game_speaker.speaker, allocator);
						speakers.PushBack(speaker, allocator);
					}

					rapidjson::Value channelMap(rapidjson::kObjectType);
					channelMap.AddMember("entryCount", channel_map.speakerCount, allocator);
					channelMap.AddMember("speakers", speakers, allocator);
					channelMaps.PushBack(channelMap, allocator);
				}
			}

			rapidjson::Value speakerMap(rapidjson::kObjectType);
			speakerMap.AddMember("channelMaps", channelMaps, allocator);
			speakerMap.AddMember("isDefault", alias.speakerMap->isDefault, allocator);
			speakerMap.AddMember("name", RAPIDJSON_STR(alias.speakerMap->name), allocator);

			std::string file_name{};
			if (alias.soundFile)
			{
				switch (alias.soundFile->type)
				{
					// LOADED
					case iw4::native::snd_alias_type_t::SAT_LOADED:
					{
						file_name = alias.soundFile->u.loadSnd->name;
					}
					break;

					// STREAMED
					case iw4::native::snd_alias_type_t::SAT_STREAMED:
					{
						file_name = alias.soundFile->u.streamSnd.name;

						if (alias.soundFile->u.streamSnd.dir)
						{
							file_name = std::format("{}/{}", alias.soundFile->u.streamSnd.dir, file_name);
						}
					}
					break;
				}
			}

			rapidjson::Value json_alias(rapidjson::kObjectType);
			json_alias.AddMember("aliasName", RAPIDJSON_STR(alias.aliasName), allocator);
			json_alias.AddMember("centerPercentage", alias.centerPercentage, allocator);
			json_alias.AddMember("chainAliasName", RAPIDJSON_STR(alias.chainAliasName), allocator);
			json_alias.AddMember("distMax", alias.distMax, allocator);
			json_alias.AddMember("distMin", alias.distMin, allocator);
			json_alias.AddMember("envelopMax", alias.envelopMax, allocator);
			json_alias.AddMember("envelopMin", alias.envelopMin, allocator);
			json_alias.AddMember("envelopPercentage", alias.envelopPercentage, allocator);
			json_alias.AddMember("flags", alias.flags.intValue, allocator);
			json_alias.AddMember("lfePercentage", alias.lfePercentage, allocator);
			json_alias.AddMember("mixerGroup", rapidjson::Value(rapidjson::kNullType), allocator);
			json_alias.AddMember("pitchMax", alias.pitchMax, allocator);
			json_alias.AddMember("pitchMin", alias.pitchMin, allocator);
			json_alias.AddMember("probability", alias.probability, allocator);
			json_alias.AddMember("secondaryAliasName", RAPIDJSON_STR(alias.secondaryAliasName), allocator);
			json_alias.AddMember("sequence", alias.sequence, allocator);
			json_alias.AddMember("slavePercentage", alias.slavePercentage, allocator);
			json_alias.AddMember("speakerMap", speakerMap, allocator);
			json_alias.AddMember("soundFile", RAPIDJSON_STR(str_duplicator.duplicate_string(file_name)), allocator);
			json_alias.AddMember("startDelay", alias.startDelay, allocator);
			json_alias.AddMember("subtitle", RAPIDJSON_STR(alias.subtitle), allocator);
			json_alias.AddMember("type", alias.soundFile->type, allocator);
			json_alias.AddMember("volMax", alias.volMax, allocator);
			json_alias.AddMember("volMin", alias.volMin, allocator);
			json_alias.AddMember("volumeFalloffCurve", RAPIDJSON_STR(alias.volumeFalloffCurve->filename), allocator);

			head.PushBack(json_alias, allocator);
		}

		output.AddMember("aliasName", RAPIDJSON_STR(ents->aliasName), allocator);
		output.AddMember("count", ents->count, allocator);
		output.AddMember("head", head, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		utils::io::write_file(std::format("{}/sounds/{}.json", get_export_path(), ents->aliasName), dump);
	}

	isndalias::isndalias()
	{

		command::add("dumpSound", [&](const command::params& params)
			{
				if (params.size() < 2) return;

				auto name = params[1];

				if (name == "*"s)
				{
					std::vector<game::native::XAssetHeader> headers{};

					game::native::DB_EnumXAssets(game::native::XAssetType::ASSET_TYPE_SOUND, [](game::native::XAssetHeader header, void* data) {
						auto headers = reinterpret_cast<std::vector<game::native::XAssetHeader>*>(data);

						headers->push_back(header);

						}, &headers, false);

					for (auto header : headers)
					{
						dump(header, true);
					}
				}
				else
				{
					auto header = game::native::DB_FindXAssetHeader(game::native::XAssetType::ASSET_TYPE_SOUND, name, false);

					if (header.data)
					{
						if (!is_already_dumped(header))
						{
							exporter::add_to_source(game::native::XAssetType::ASSET_TYPE_SOUND, name);
						}

						dump(header, true);
						console::info("successfullly dumped sound %s!\n", name);
					}
					else
					{
						console::info("could not dump sound %s from the database (cannot find it)\n", name);
					}
				}
			});

	}
}