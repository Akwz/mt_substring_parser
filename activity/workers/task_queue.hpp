#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <functional>
#include <memory>


namespace worker
{

class TaskQueue
{
public:
	TaskQueue() = default;

	void Push(std::function<void()>& task);
	void Push(std::vector<std::function<void()>>&& tasks);

	template<typename ContainerType>
	bool Fetch(ContainerType& container)
	{
		std::lock_guard<std::mutex> lg(mQueueMutex);
		if(!mQueue.empty())
		{
			auto value = mQueue.front();
			container = std::move(value);
			mQueue.pop();
			return true;
		}
		return false;
	}
private:
	TaskQueue(const TaskQueue&) = delete;
	TaskQueue& operator=(const TaskQueue&) = delete;

	std::mutex mQueueMutex;
	std::queue<std::function<void()>> mQueue;
};

} // namespace worker