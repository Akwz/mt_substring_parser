#include <thread>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cassert>

#include "worker_pool.hpp"

namespace worker
{

const size_t DEFAULT_WORKERS_COUNT{4};

WorkerPool::WorkerPool(std::shared_ptr<TaskQueue> queue)
	: mTasks{queue}
	, mDone{false}
{
	assert(mTasks);
	Init();
}

WorkerPool::~WorkerPool()
{
	Terminate();
}

void WorkerPool::Init()
{
	const size_t workers_count = std::thread::hardware_concurrency();
	for(size_t i = 0; i < (workers_count ? workers_count : DEFAULT_WORKERS_COUNT); ++i)
	{
		auto worker_task = [&flag = mDone, tasks = mTasks](size_t id){
			while(!flag)
			{
				std::function<void()> func;
				if(tasks->Fetch(func))
				{
					func();
				}
			}
		};
		mWorkers.emplace_back(std::thread(worker_task, i));
	}
}

void WorkerPool::Terminate()
{
	mDone = true;
	for(auto& worker : mWorkers)
	{
		worker.join();
	}
}

} // namespace worker