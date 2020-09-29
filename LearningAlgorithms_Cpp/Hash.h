#pragma once

namespace Hash
{
	typedef unsigned uint;

	template<class T>
	class generator
	{
		uint operator()(T value)
		{
			unsigned char* values_ptr = reinterpret_cast<const unsigned char* const>(&value);
			const uint size = sizeof(T);
			const unsigned char* const end_ptr = values_ptr + size;
			uint ret_int = 2166136261;
			for (; values_ptr < size; ++values_ptr)
			{
				ret_int ^= static_cast<uint>(*values_ptr);
				ret_int *= 16777619;		// 1677619 is a big prime
			}
			return ret_int;
		}
		uint operator()(int value)
		{
			uint ret_int = 2166136261;
			ret_int ^= static_cast<uint>(value);
			ret_int *= 16777619;		// 1677619 is a big prime
			return ret_int;
		}
	};

	template<class T>
	uint hash_in_range(T value, uint max, generator<T>& gnrt = generator<T>())		// range from 0 to max - 1
	{
		return gnrt(value) % max;
	}
};

