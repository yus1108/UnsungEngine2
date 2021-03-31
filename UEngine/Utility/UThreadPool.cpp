#include "UThreadPool.h"
#include <iostream>

void UEngine::Utility::UThreadPool::Init(size_t numThreads)
{
	if (numThreads == 0) return;

	Release();
	this->stop_all = false;
	this->numThreads = numThreads;
	pool.reserve(numThreads);
	for (size_t i = 0; i < numThreads; i++)
		pool.emplace_back([this]() { this->WorkerThread(); });
}

void UEngine::Utility::UThreadPool::Release()
{
	stop_all = true;
	condition_variable.notify_all();

	for (auto& thread : pool)
		thread.join();
}

void UEngine::Utility::UThreadPool::WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mutex);
		condition_variable.wait(lock, [this]() { return !this->tasks.empty() || stop_all; });
		if (stop_all && tasks.empty()) return;

		auto task = std::move(tasks.front());
		tasks.pop();
		lock.unlock();

		task();
	}
}

void UEngine::Utility::UThreadPool::AddTask(std::function<void()> task)
{
	if (task == nullptr) return;
	if (numThreads == 0)
	{
		task();
		return;
	}

	std::unique_lock<std::mutex> lock(mutex);
	bool stop = stop_all;
	lock.unlock();

	if (stop_all) throw std::runtime_error("ThreadPool is Paused, cannot add a task now!");

	{
		lock.lock();
		tasks.push(std::move(task));
	}
	condition_variable.notify_one();
}

std::function<void()> UEngine::Utility::Sync::UThreadSync::CreateSyncTask(std::function<void()> task)
{
	if (task == nullptr) return nullptr;

	std::unique_lock<std::mutex> lock(mutex);
	count++;
	lock.unlock();

	return [this, task]()
	{
		task();

		std::unique_lock<std::mutex> lock(mutex);
		int count = --this->count;
		lock.unlock();

		if (count == 0) condition_variable.notify_one();
	};
}

void UEngine::Utility::Sync::UThreadSync::Join()
{
	std::unique_lock<std::mutex> lock(mutex);
	condition_variable.wait(lock, [this]() { return count == 0; });
}
