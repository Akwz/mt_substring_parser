#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <memory>
#include <future>
#include <functional>
#include <condition_variable>

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

	template<typename ResultType>
	void Process(Activity<ResultType>& activity)
	{
		std::vector<std::function<void()>> pool_tasks;
		std::vector<std::promise<ResultType>> result_promises(activity.tasks.size());
		std::vector<std::future<ResultType>> result_futures;
		for(size_t i = 0; i < activity.tasks.size(); ++i)
		{
			auto& promise = result_promises[i];
			result_futures.emplace_back(promise.get_future());
			auto wrapped_task = std::function<void()>([prom = std::ref(promise), task = std::move(activity.tasks[i])](){
				ResultType result = task();
				prom.get().set_value(result);
			});
			pool_tasks.emplace_back(wrapped_task);
		}

		std::condition_variable cv;
		std::atomic<bool> processed{false};
		auto result_extraction = std::function<void()>([&cv, &processed, &result_futures, &result_container = activity.result]() mutable {
			for(auto& future : result_futures)
			{
				auto res = future.get();
				result_container.Merge(res);
			}
			processed = true;
			cv.notify_one();
		});
		pool_tasks.emplace_back(result_extraction);

		mTasks->Push(std::move(pool_tasks));

		{
			std::mutex m;
			std::unique_lock<std::mutex> ul(m);
			cv.wait(ul, [&processed](){return processed == true;});
		}
	}
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