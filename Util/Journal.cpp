#include "Journal.h"

namespace OGE
{
	Journal* Journal::pJournal = NULL;
	string Journal::logFilePath = "../Log.txt";
	string Journal::errorFilePath = "../ErrorLog.txt";

	void Journal::writeMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg)
	{
		
		pLogger->writeLogMsg(fileName,
								lineNumber,
								msg);
	}

	void Journal::writeErrorMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg)
	{
		pErrorLogger->writeLogMsg(fileName,
								lineNumber,
								msg);

	}
}	