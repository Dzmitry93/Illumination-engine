#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include "TypeDefinitions.h"
#include "Debug.h"
#include <string>
#include <algorithm>

using std::string;
using std::reverse;

namespace OGE
{
	namespace util
	{
		class StringUtil
		{
		public:
			
			static string NumberToString(int64);
		};
	}
}

#endif
