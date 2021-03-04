#include "UTime.h"
#include <Windows.h>

namespace UEngine
{
	namespace Utility
	{
		using namespace std::chrono;

		UTime UTime::instance;

		UTime* UTime::Get()
		{
			return &instance;
		}

		UTime::UTime()
		{
			startTime = high_resolution_clock::now();
			prevTime = startTime;
			elapsedSignal = 0;
			totalTime = sinceLast = lastSecond = framePerSecond = 0.0;
			TimeScale = 1.0;
		}

		void UTime::Signal()
		{
			prevTime = currentTime;
			currentTime = high_resolution_clock::now();
			duration<double, std::milli> time_span = currentTime - startTime;
			totalTime = time_span.count() / 1000.0;

			elapsedSignal++;
			double sinceLast = totalTime - lastSecond;
			if (sinceLast >= 0.1)
			{
				framePerSecond = (double)elapsedSignal / sinceLast;
				lastSecond = totalTime;
				elapsedSignal = 0;
			}
		}

		const double UTime::DeltaTime()
		{
			duration<double, std::milli> time_span = currentTime - prevTime;
			return time_span.count() / 1000.0 * TimeScale;
		}

		const float UTime::DeltaTimeF()
		{
			return static_cast<float>(DeltaTime());
		}

		const double UTime::TotalTime()
		{
			return totalTime;
		}

		const double UTime::TotalTimeExact()
		{
			currentTime = high_resolution_clock::now();
			duration<double, std::milli> time_span = currentTime - startTime;
			return  time_span.count() / 1000.0;
		}

		const double UTime::FramePerSecond()
		{
			return framePerSecond;
		}

		void UTime::Restart()
		{
			startTime = high_resolution_clock::now();
			prevTime = startTime;
			elapsedSignal = 0;
			totalTime = sinceLast = lastSecond = framePerSecond = 0.0;
		}

		void UTime::Throttle(double targetHz)
		{
			if (targetHz > 1)
			{
				double framerate = elapsedSignal / (TotalTimeExact() - lastSecond);
				while (framerate > targetHz)
				{
					framerate = elapsedSignal / (TotalTimeExact() - lastSecond);
				}
			}
		}
	}
}
