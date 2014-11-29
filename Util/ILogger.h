#ifndef __ILOGGGER_H__
#define __ILOGGGER_H__

#include "TypeDefinitions.h"
#include <string>
#include <ctime>

using std::string;

namespace OGE
{
	class ILogger
	{
	public:

		virtual void writeLogMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg) = 0;
	};
}

#endif

