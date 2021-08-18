#include <mutex>

#include "task_queue.hpp"


namespace worker
{

void TaskQueue::Push(std::function<void()>&& task)
{
	std::lock_guard<std::mutex> lg(mQueueMutex);
	mQueue.emplace(task);
}

std::unique_ptr<std::function<void()>> TaskQueue::Fetch()
{
	std::unique_ptr<std::function<void()>> result{nullptr};
	std::lock_guard<std::mutex> lg(mQueueMutex);
	if(!mQueue.empty())
	{
		auto value = mQueue.front();
		result = std::make_unique<std::function<void()>>(value);
		mQueue.pop();
	}
	return result;
}

} // namespace worker