#pragma once
#ifndef __HASH_UTIL_H__
#define __HASH_UTIL_H__

#include "TypeDefinitions.h"
#include <string>

using std::string;

namespace OGE
{
	namespace util
	{
		class HashUtil
		{
		public:
			
			static inline uint64 getHash(const string& str);
		};

		inline uint64 HashUtil::getHash(const string& str)
		{
			uint64 hash = 0;
			for(uint32 i = 0; i < str.size(); i++)
			{
				hash += str.at(i) << i;
			}

			return hash;
		}
	}
}

#endif