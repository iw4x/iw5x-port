#pragma once

#define BINARY_PAYLOAD_SIZE 0x0A000000
#define TLS_PAYLOAD_SIZE 0x2000

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4458)
#pragma warning(disable: 6297)
#pragma warning(disable: 6385)
#pragma warning(disable: 6386)
#pragma warning(disable: 26451)
#pragma warning(disable: 26444)
#pragma warning(disable: 26451)
#pragma warning(disable: 26495)
#pragma warning(disable: 28020)

#define _REGEX_MAX_STACK_COUNT 0

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <MsHTML.h>
#include <MsHtmHst.h>
#include <ExDisp.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <corecrt_io.h>
#include <fcntl.h>
#include <shellapi.h>
#include <csetjmp>

// min and max is required by gdi, therefore NOMINMAX won't work
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <atomic>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <regex>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <zlib.h>
#include <zstd.h>

#define PRIu64
#include <diff.h>
#include <patch.h>
#undef PRIu64

#include <tomcrypt.h>

#include <gsl/gsl>

#include <udis86.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ws2_32.lib")

#pragma warning(pop)

#pragma warning(disable: 4100)
#pragma warning(disable: 26812)

#include "resource.hpp"

using namespace std::literals;

extern __declspec(thread) char tls_data[TLS_PAYLOAD_SIZE];
