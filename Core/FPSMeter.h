#ifndef __FPS_METER_H__
#define __FPS_METER_H__

#include "Timer.h"

namespace OGE
{
	namespace util
	{
		class FPSMeter
		{
		public:

			void begin();
			void stop();

			uint32 getFPS();

		private:

			uint64 lastTime;
			uint32 fps;
		};

		inline void FPSMeter::begin()
		{	
			lastTime = Timer::getCurrentTime().ticks.QuadPart;
		}	

		inline void FPSMeter::stop()
		{
			Time time = Timer::getCurrentTime();
			double seconds = (time.ticks.QuadPart - lastTime) / (double)time.frequency.QuadPart;
			fps = (uint32)(1.0 / seconds);
			lastTime = time.ticks.QuadPart;
		}

		inline uint32 FPSMeter::getFPS()
		{
			return fps;
		}
	}
}

#endif