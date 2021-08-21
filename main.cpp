#include <iostream>
#include <thread>
#include <chrono>

#include "worker_pool.hpp"
#include "activity_manager.hpp"
#include "result_container.hpp"


int main(int argc, char** argv)
{
	auto am = activity::ActivityManager::Instance();

	using retT = data::ResultContainer<size_t>;
	size_t ress = 0;
	activity::Activity<retT> act;
	for(size_t i = 0; i < 100000; ++i)
	{
		act.tasks.emplace_back(std::function<retT()>([i]() -> retT {
			return retT(i);
		}));
		ress += i;
	}

	am->Process<retT>(act);

	std::cout << "Done :" << act.result.Value() << " " << ress << std::endl;
	return 0;
}