#include "StringUtil.h"

namespace OGE
{
	namespace util
	{
		string StringUtil::NumberToString(int64 value)
		{
			string number;
			uint8 index = 0;

			if(value == 0)
			{
				return "0";
			}

			while(value > 0)
			{
				number.push_back(48 + (value % 10));
				value = value / 10;
				index++;
			}

			reverse(number.begin(), number.end());

			return number;
		}
	}
}