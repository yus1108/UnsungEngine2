#pragma once
#include <chrono>

namespace UEngine
{
	namespace Utility
	{
		// Allow Singleton, but also be instantiated
		class UTime
		{
		public:
			static UTime* Get();
			UTime();
			~UTime() = default;

		private:
#pragma data_seg(".ioshare")
		static UTime instance;
#pragma data_seg()


		private:
			std::chrono::high_resolution_clock::time_point startTime;
			std::chrono::high_resolution_clock::time_point currentTime;
			std::chrono::high_resolution_clock::time_point prevTime;
			double totalTime, sinceLast, lastSecond, framePerSecond;
			unsigned int elapsedSignal;

		public:
			double TimeScale;

			void Signal();
			const double DeltaTime();
			const float DeltaTimeF();
			const double TotalTime();
			const double TotalTimeExact();
			const double FramePerSecond();
			void Restart();
			void Throttle(double targetHz);
		};
	}
}
