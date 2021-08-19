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
	WorkerPool(std::shared_ptr<TaskQueue> queue);
	~WorkerPool();
private:
	WorkerPool(const WorkerPool&) = delete;
	WorkerPool& operator=(const WorkerPool&) = delete;

	void Init();
	void Terminate();

	std::atomic<bool> mDone;
	std::vector<std::thread> mWorkers;
	std::shared_ptr<TaskQueue> mTasks;
};

} // namespace worker