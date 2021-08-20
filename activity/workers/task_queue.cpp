#include <mutex>

#include "task_queue.hpp"


namespace worker
{

void TaskQueue::Push(std::function<void()>& task)
{
	std::lock_guard<std::mutex> lg(mQueueMutex);
	mQueue.emplace(task);
}

void TaskQueue::Push(std::vector<std::function<void()>>&& tasks)
{
	std::lock_guard<std::mutex> lg(mQueueMutex);
	for(const auto& task : tasks)
	{
		mQueue.emplace(task);
	}
}

} // namespace worker