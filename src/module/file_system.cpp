#include <std_include.hpp>
#include <loader/module_loader.hpp>

#include <utils/hook.hpp>

#include "game/game.hpp"

#include "file_system.hpp"

static utils::hook::detour sys_default_install_path_hook;

static const game::native::dvar_t** fs_homepath;
static const game::native::dvar_t** fs_debug;

static FILE* file_for_handle(int f)
{
	assert(!game::native::fsh[f].zipFile);
	assert(game::native::fsh[f].handleFiles.file.o);

	return game::native::fsh[f].handleFiles.file.o;
}

static unsigned int file_write(const void* ptr, unsigned int len, FILE* stream)
{
	return std::fwrite(ptr, 1, len, stream);
}

static FILE* file_open_append_text(const char* filename)
{
	FILE* file;
	const auto err = fopen_s(&file, filename, "at");
	if (err == 0)
	{
		return file;
	}

	printf("Couldn't open file: %s\n", filename);
	return nullptr;
}

static void replace_separators(char* path)
{
	char* src, * dst;

	bool was_sep = false;

	for (src = path, dst = path; *src; ++src)
	{
		if (*src == '/' || *src == '\\')
		{
			if (!was_sep)
			{
				was_sep = true;
				*dst++ = '\\';
			}
		}
		else
		{
			was_sep = false;
			*dst++ = *src;
		}
	}
	*dst = 0;
}

static void build_os_path_for_thread(const char* base, const char* game, const char* qpath, char* ospath, game::native::FsThread thread)
{
	assert(base);
	assert(qpath);
	assert(ospath);

	if (!game)
	{
		game = "";
	}
	else if (!game[0])
	{
		game = game::native::fs_gamedir;
	}

	auto len_base = std::strlen(base);
	auto len_game = std::strlen(game);
	auto len_qpath = std::strlen(qpath);
	if (len_game + 1 + len_base + len_qpath + 1 >= 256)
	{
		if (thread)
		{
			*ospath = '\0';
			return;
		}

		game::native::Com_Error(game::native::ERR_FATAL, "\x15" "FS_BuildOSPath: os path length exceeded\n");
	}

	std::memcpy(ospath, base, len_base);
	ospath[len_base] = '/';

	std::memcpy(&ospath[len_base + 1], game, len_game);
	ospath[len_base + 1 + len_game] = '/';

	std::memcpy(ospath + len_base + 2 + len_game, qpath, len_qpath + 1);
	replace_separators(ospath);
}

static game::native::FsThread get_current_thread()
{
	if (game::native::Sys_IsMainThread())
	{
		return game::native::FS_THREAD_MAIN;
	}
	if (game::native::Sys_IsDatabaseThread())
	{
		return game::native::FS_THREAD_DATABASE;
	}
	if (game::native::Sys_IsStreamThread())
	{
		return game::native::FS_THREAD_STREAM;
	}
	if (game::native::Sys_IsRenderThread())
	{
		return game::native::FS_THREAD_BACKEND;
	}
	if (game::native::Sys_IsServerThread())
	{
		return game::native::FS_THREAD_SERVER;
	}
	return game::native::FS_THREAD_INVALID;
}

static void* handle_for_file_current_thread()
{
	return game::native::FS_HandleForFile(get_current_thread());
}

static int open_file_append(const char* filename)
{
	char ospath[MAX_PATH]{};

	game::native::FS_CheckFileSystemStarted();
	const auto* basepath = (*fs_homepath)->current.string;
	build_os_path_for_thread(basepath, game::native::fs_gamedir, filename, ospath, game::native::FS_THREAD_MAIN);
	if ((*fs_debug)->current.integer)
	{
		printf("FS_FOpenFileAppend: %s\n", ospath);
	}

	if (game::native::FS_CreatePath(ospath))
	{
		return 0;
	}

	auto* f = file_open_append_text(ospath);
	if (!f)
	{
		return 0;
	}

	auto h = reinterpret_cast<int>(handle_for_file_current_thread());
	game::native::fsh[h].zipFile = nullptr;
	strncpy_s(game::native::fsh[h].name, filename, _TRUNCATE);
	game::native::fsh[h].handleFiles.file.o = f;
	game::native::fsh[h].handleSync = 0;

	if (!game::native::fsh[h].handleFiles.file.o)
	{
		game::native::FS_FCloseFile(h);
		h = 0;
	}

	return h;
}

static const char* sys_default_install_path_stub()
{
	static auto current_path = std::filesystem::current_path().string();
	return current_path.data();
}

int file_system::open_file_by_mode(const char* qpath, int* f, game::native::fsMode_t mode)
{
	auto r = 6969;
	auto sync = 0;

	switch (mode)
	{
	case game::native::FS_READ:
		*game::native::com_fileAccessed = TRUE;
		r = game::native::FS_FOpenFileReadForThread(qpath, f, game::native::FS_THREAD_MAIN);
		break;
	case game::native::FS_APPEND_SYNC:
		sync = 1;
	case game::native::FS_APPEND:
		*f = open_file_append(qpath);
		r = 0;
		if (!*f )
		{
			r = -1;
		}
		break;
	default:
		game::native::Com_Error(game::native::ERR_FATAL, "\x15" "FSH_FOpenFile: bad mode");
		break;
	}

	if (!f)
	{
		return r;
	}

	if (*f)
	{
		game::native::fsh[*f].fileSize = r;
		game::native::fsh[*f].streamed = 0;
	}

	game::native::fsh[*f].handleSync = sync;
	return r;
}

int file_system::write(const char* buffer, int len, int h)
{
	game::native::FS_CheckFileSystemStarted();
	if (!h)
	{
		return 0;
	}

	auto* f = file_for_handle(h);
	auto* buf = const_cast<char*>(buffer);
	auto remaining = len;
	auto tries = 0;
	while (remaining)
	{
		auto block = remaining;
		auto written = static_cast<int>(file_write(buf, block, f));
		if (!written)
		{
			if (tries)
			{
				return 0;
			}
			tries = 1;
		}

		if (written == -1)
		{
			return 0;
		}

		remaining -= written;
		buf += written;
	}

	if (game::native::fsh[h].handleSync)
	{
		std::fflush(f);
	}

	return len;
}

void file_system::post_load()
{
	fs_homepath = reinterpret_cast<const game::native::dvar_t**>(
		SELECT_VALUE(0x1C2B538, 0x59ADD18, 0x585E8F0));
	fs_debug = reinterpret_cast<const game::native::dvar_t**>(
		SELECT_VALUE(0x1C2B32C, 0x59A9A08, 0x585A5E4));

	// Make open-iw5 work outside of the game directory
	sys_default_install_path_hook.create(SELECT_VALUE(0x487E50, 0x5C4A80, 0x535F80), &sys_default_install_path_stub);

	// fs_basegame
	utils::hook::set<const char*>(SELECT_VALUE(0x629031, 0x5B0FD1, 0x526F5C), "userraw");
}

REGISTER_MODULE(file_system)
