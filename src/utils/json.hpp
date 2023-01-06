#pragma once

#include "rapidjson/document.h"

namespace utils::json
{
	template<class T>
	rapidjson::Value make_json_array(T* arr, size_t length, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
	{
		rapidjson::Value json_arr(rapidjson::kArrayType);

		for (size_t i = 0; i < length; i++)
		{
			rapidjson::Value value(arr[i]);
			json_arr.PushBack(value, allocator);
		}

		return json_arr;
	}
}
