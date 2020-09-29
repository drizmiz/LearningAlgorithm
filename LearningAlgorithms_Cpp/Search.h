#pragma once

#ifndef _SEARCH_H
#define _SEARCH_H

#include <vector>
#include <string>

#include "Hash.h"

namespace Search
{
	typedef unsigned uint;

	class boyer_moore_searcher
	{
		typedef std::string string;
	private:
		std::vector<int> right;
		string pattern;
	public:
		boyer_moore_searcher(const string& pat) :pattern(pat), right(512, -1)		// 512 is the max that a char can hold
		{
			for (uint i = 0; i < pat.size(); ++i)
				right[pat[i]] = i;
		}
		uint search(const string& str)
		{
			const uint text_size = str.size();
			const uint pat_size = pattern.size();
			for (uint i = 0; i <= text_size - pat_size; )
			{
				int skip = 0;
				for (uint j = pat_size; j >= 1; --j)
					if (str[i + j] != pattern[j])
					{
						skip = j - right[str[i + j]];
						if (skip < 1)
							skip = 1;
						break;
					}
				if (skip == 0)
					return i;
				i += skip;
			}
			return text_size;
		}
	};
	

};

#endif // _SEARCH_H

