#include <iostream>
#include <thread>
#include <chrono>

#include "worker_pool.hpp"
#include "activity_manager.hpp"
#include "result_container.hpp"


int main(int argc, char** argv)
{
	auto am = activity::ActivityManager::Instance();

	using retT = data::ResultContainer<int>;

	activity::Activity<retT> act;
	for(size_t i = 0; i < 5; ++i)
	{
		act.tasks.emplace_back(std::function<retT()>([i]() -> retT {
			return retT(i);
		}));
	}

	am->Process<retT>(act);

	std::cout << "Done :" << act.result.Value() << std::endl;
	return 0;
}