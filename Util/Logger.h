#ifndef __CLOGGER_H__
#define __CLOGGER_H__

#include "ILogger.h"
#include <fstream>

using std::ofstream;
using std::ios;

namespace OGE
{
	class CLogger : public ILogger
	{
	public:
		CLogger(const string& rPath);
		~CLogger(void);

		void writeLogMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg);
	private:
		ofstream logFile;
	};

	inline CLogger::CLogger(const string& rPath) 
	{
		logFile.open(rPath, ios::app);
	}

	inline CLogger::~CLogger()
	{
		logFile.close();
	}

	inline void CLogger::writeLogMsg(const string& fileName,
								uint32 lineNumber,
								const string& msg)
	{
		time_t seconds_t = time(NULL);
		tm date;
		localtime_s(&date, &seconds_t);

		char years[5];
		char months[4];
		char days[4];
		char hours[4];
		char minutes[4];
		char seconds[4];
	
		sprintf_s(years, 5, "%d", (date.tm_year + 1900));
		sprintf_s(months, 4, "%d", date.tm_mon);
		sprintf_s(days, 4, "%d", date.tm_mday);
		sprintf_s(hours, 4, "%d", date.tm_hour);
		sprintf_s(minutes, 4, "%d", date.tm_min);
		sprintf_s(seconds, 4, "%d", date.tm_sec);

		string date_str = "";
		date_str += years; date_str += ":";
		date_str += months; date_str += ":";
		date_str += days; date_str += " ";
		date_str += hours; date_str += ":";
		date_str += minutes; date_str += ":";
		date_str += seconds;

		
		if(logFile.is_open())
		{
			logFile << "File : " << fileName << "\nLine : " << lineNumber 
				<< "\nDate : " << date_str << "\nmsg : " << msg << "\n\n";
		}
	}
}

#endif
