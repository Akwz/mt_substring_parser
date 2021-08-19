#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <memory>

#include "task_queue.hpp"
#include "worker_pool.hpp"
#include "activity.hpp"
#include "composite_activity.hpp"

namespace activity
{

class ActivityManager
{
public:
	static std::shared_ptr<ActivityManager> Instance();
	void Process(Activity& activity);
private:
	ActivityManager();
	ActivityManager(const ActivityManager&) = delete;
	ActivityManager& operator=(const ActivityManager&) = delete;

	static void Init();

	static std::shared_ptr<ActivityManager> mInstance;

	std::shared_ptr<worker::TaskQueue> mTasks;
	worker::WorkerPool mWorkers;
};

} // namespace activity