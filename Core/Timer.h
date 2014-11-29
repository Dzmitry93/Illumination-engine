#ifndef __TIMER_H__
#define __TIMER_H__

#include "TypeDefinitions.h"
#include <Windows.h>

namespace OGE
{
	namespace util
	{
		struct Time
		{
			uint64 currentTime;
			LARGE_INTEGER frequency;
			LARGE_INTEGER ticks;
		};

		class Timer
		{
		public:
			static Time getCurrentTime();
		};

		inline Time Timer::getCurrentTime()
		{
			LARGE_INTEGER ticks, frequency;
			Time time;

			QueryPerformanceCounter(&ticks);
			QueryPerformanceFrequency(&frequency);

			time.frequency = frequency;
			time.ticks = ticks;
			time.currentTime = ticks.QuadPart / frequency.QuadPart;

			return time;
		}
	}
}

#endif
