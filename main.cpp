#include <iostream>
#include <thread>
#include <chrono>

#include "worker_pool.hpp"
#include "activity_manager.hpp"
#include "result_container.hpp"


int main(int argc, char** argv)
{
	auto am = activity::ActivityManager::Instance();
	activity::Activity<data::ResultContainer> act;
	am->Process<data::ResultContainer>(act);

	std::cout << "Done" << std::endl;
	return 0;
}