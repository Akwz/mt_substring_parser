#include <condition_variable>

#include "activity_manager.hpp"

namespace activity
{

std::shared_ptr<ActivityManager> ActivityManager::mInstance{nullptr};

ActivityManager::ActivityManager()
	: mTasks{std::make_shared<worker::TaskQueue>()}
	, mWorkers{mTasks}
{}

std::shared_ptr<ActivityManager> ActivityManager::Instance()
{
	if(!mInstance) Init();
	return mInstance;
}

void ActivityManager::Init()
{
	mInstance = std::shared_ptr<ActivityManager>(new ActivityManager);
}

void ActivityManager::Process(Activity& activity)
{
	mTasks->Push(activity.tasks);
}

} // namespace activity