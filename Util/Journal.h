#ifndef __JOURNAL_H__
#define __JOURNAL_H__

#include "Logger.h"
#include <ctime>

namespace OGE
{
	class Journal
	{
	public:
		Journal(void);
		~Journal(void);

		static Journal* openJournal();
		
		void writeErrorMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg);

		void writeMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg);

		void close();

	private:
		static string logFilePath;
		static string errorFilePath;

		static Journal* pJournal;

		ILogger* pLogger;
		ILogger* pErrorLogger;
	};

	inline Journal::Journal(void)
	{
		pLogger = new CLogger(logFilePath);
		pErrorLogger = new CLogger(errorFilePath);
	}

	inline Journal::~Journal(void){}

	inline Journal* Journal::openJournal()
	{
		if(pJournal == NULL)
		{
			pJournal = new Journal();
		}

		return pJournal;
	}

	inline void Journal::close()
	{
		if(pLogger != NULL)
		{
			delete pLogger;
		}
		if(pErrorLogger != NULL)
		{
			delete pErrorLogger;
		}
	}
}

#endif
