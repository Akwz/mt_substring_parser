#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "task_queue.hpp"


namespace worker
{

extern const size_t DEFAULT_WORKERS_COUNT;

class WorkerPool
{
public:
	static std::shared_ptr<WorkerPool> Instance();
	~WorkerPool();
private:
	WorkerPool();
	WorkerPool(const WorkerPool&) = delete;
	WorkerPool& operator=(const WorkerPool&) = delete;


	void Terminate();
	static void Init();

	std::atomic<bool> mDone;
	static std::shared_ptr<WorkerPool> mInstance;
	std::vector<std::thread> mWorkers;
	TaskQueue mTasks;
};

} // namespace worker