#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <memory>

#include "activity.hpp"
#include "composite_activity.hpp"

namespace activity
{

class ActivityManager
{
public:
	static ActivityManager& Instance();

private:
	ActivityManager() = delete;
	ActivityManager(const ActivityManager&) = delete;
	ActivityManager& operator=(const ActivityManager&) = delete;

	static void Init(size_t pool_size);

	static std::unique_ptr<ActivityManager> mInstance;

	std::vector<std::thread> mThreadPool;
	// std::queue<Activity> mQueue;
};

} // namespace activity