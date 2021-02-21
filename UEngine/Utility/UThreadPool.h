#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace UEngine
{
	namespace Utility
	{
		class UThreadPool
		{
		public:
			static UThreadPool* Get() { return &instance; }
			UThreadPool() = default;
			~UThreadPool() { Release(); }

		private:
			static UThreadPool instance;

		public:
			void Init(size_t numThreads = 8);
			void Release();

		private:
			bool stop_all{ false };
			size_t numThreads{ 0 };
			std::vector<std::thread> pool;
			std::queue<std::function<void()>> tasks;
			std::condition_variable cv_threadpool;
			std::mutex m_threadpool;

		private:
			void WorkerThread();

		public:
			void AddTask(std::function<void()> task);
		};


	}
}
