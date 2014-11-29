#ifndef __GUID_UTIL_H__
#define __GUID_UTIL_H__

#include "StringUtil.h"
#include "Timer.h"
#include <string>

using std::string;

namespace OGE
{
	namespace util
	{
		class GUIDUtil
		{
		public:
			static string generateRandomGUID();
		};

		inline string GUIDUtil::generateRandomGUID()
		{
			string guid;
			uint64 currentTime;
			
			currentTime = Timer::getCurrentTime().ticks.QuadPart;
			guid = StringUtil::NumberToString(currentTime);
			
			return guid;
		}
	}
}

#endif