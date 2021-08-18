#pragma once

#include <queue>
#include <mutex>
#include <functional>
#include <memory>


namespace worker
{

class TaskQueue
{
public:
	void Push(std::function<void()>&& task);
	std::unique_ptr<std::function<void()>> Fetch();
private:
	std::mutex mQueueMutex;
	std::queue<std::function<void()>> mQueue;
};

} // namespace worker