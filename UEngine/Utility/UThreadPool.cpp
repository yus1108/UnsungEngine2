#include "UThreadPool.h"
#include <iostream>

UEngine::Utility::UThreadPool UEngine::Utility::UThreadPool::instance;

void UEngine::Utility::UThreadPool::Init(size_t numThreads)
{
	Release();
	this->numThreads = numThreads;
	pool.reserve(numThreads);
	for (size_t i = 0; i < numThreads; i++)
		pool.emplace_back([this]() { this->WorkerThread(); });
}

void UEngine::Utility::UThreadPool::Release()
{
	stop_all = true;
	cv_threadpool.notify_all();

	for (auto& thread : pool)
		thread.join();
}

void UEngine::Utility::UThreadPool::WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_threadpool);
		cv_threadpool.wait(lock, [this]() { return !this->tasks.empty() || stop_all; });
		if (stop_all && tasks.empty()) return;

		auto task = std::move(tasks.front());
		tasks.pop();
		lock.unlock();

		task();
	}
}

void UEngine::Utility::UThreadPool::AddTask(std::function<void()> task)
{
	std::unique_lock<std::mutex> lock(m_threadpool);
	bool stop = stop_all;
	lock.unlock();

	if (stop_all) throw std::runtime_error("ThreadPool is Paused, cannot add a task now!");

	{
		lock.lock();
		tasks.push(std::move(task));
	}
	cv_threadpool.notify_one();
}
