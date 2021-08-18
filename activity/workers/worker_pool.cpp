#include <thread>
#include <algorithm>
#include <iostream>
#include <chrono>

#include "worker_pool.hpp"

namespace worker
{

const size_t DEFAULT_WORKERS_COUNT{4};
std::shared_ptr<WorkerPool> WorkerPool::mInstance{nullptr};

WorkerPool::WorkerPool()
	: mDone(false)
{}

WorkerPool::~WorkerPool()
{
	Terminate();
}

std::shared_ptr<WorkerPool> WorkerPool::Instance()
{
	if(!mInstance) Init();
	return WorkerPool::mInstance;
}

void WorkerPool::Init()
{
	const size_t workers_count = std::thread::hardware_concurrency();

	mInstance = std::shared_ptr<WorkerPool>(new WorkerPool);
	for(size_t i = 0; i < (workers_count ? workers_count : DEFAULT_WORKERS_COUNT); ++i)
	{
		auto& flag = mInstance->mDone;
		auto worker_task = [&flag](size_t id){
			while(!flag)
			{
				std::this_thread::sleep_for(std::chrono::seconds(2));
				std::cout << "Thread Executes: " << id << std::endl;
			}
			std::cout << "Thread finished: " << id << std::endl;
		};
		mInstance->mWorkers.emplace_back(std::thread(worker_task, i));
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