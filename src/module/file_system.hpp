#pragma once

class file_system final : public module
{
public:
	static_assert(sizeof(game::native::fileHandleData_t) == 0x11C);

	void post_load() override;

	static int open_file_by_mode(const char* qpath, int* f, game::native::fsMode_t mode);
	static int write(const char* buffer, int len, int h);
};
