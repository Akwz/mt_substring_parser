#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <memory>

#include "activity.hpp"
#include "composite_activity.hpp"


namespace activity
{

class ActivityDispatcher
{
public:
	static ActivityDispatcher& Instance();

	void DispatchActivity(CompositeActivity&& activity);

private:
	ActivityDispatcher() = delete;
	ActivityDispatcher(const ActivityDispatcher&) = delete;
	ActivityDispatcher& operator=(const ActivityDispatcher&) = delete;

	static void Init(size_t pool_size);

	static std::unique_ptr<ActivityDispatcher> mInstance;

	std::vector<std::thread> mThreadPool;
	std::queue<Activity> mQueue;
};

} // namespace activity