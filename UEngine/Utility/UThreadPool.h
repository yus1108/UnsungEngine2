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
			UThreadPool(size_t numThreads = 0) { Init(numThreads); }
			~UThreadPool() { Release(); }

		public:
			void Init(size_t numThreads = 0);
			void Release();

		protected:
			size_t numThreads{ 0 };

		private:
			bool stop_all{ false };
			std::vector<std::thread> pool;
			std::queue<std::function<void()>> tasks;
			std::condition_variable condition_variable;
			std::mutex mutex;

		private:
			void WorkerThread();

		public:
			void AddTask(std::function<void()> task);
		};

		namespace Sync
		{
			class UThreadPool final : public UEngine::Utility::UThreadPool
			{
			public:
				UThreadPool(size_t numThreads = 0) : UEngine::Utility::UThreadPool(numThreads) {}

			private:
				std::mutex mutex;
				std::condition_variable condition_variable;
				unsigned count{ 0 };

			public:
				void AddSyncTask(std::function<void()> task);
				void Join();
			};
		}
	}
}
