#pragma once

namespace gsc
{
	std::optional<std::pair<std::string, std::string>> find_function(const char* pos);

	unsigned int scr_get_object(unsigned int index);
	unsigned int scr_get_const_string(unsigned int index);
	unsigned int scr_get_const_istring(unsigned int index);
	void scr_validate_localized_string_ref(int parm_index, const char* token, int token_len);
	void scr_get_vector(unsigned int index, float* vector_value);
	int scr_get_int(unsigned int index);
	float scr_get_float(unsigned int index);

	int scr_get_pointer_type(unsigned int index);
	int scr_get_type(unsigned int index);
	const char* scr_get_type_name(unsigned int index);

	void scr_error(const char* error);
}
