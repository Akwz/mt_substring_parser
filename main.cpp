#include <iostream>
#include <thread>
#include <chrono>

#include "worker_pool.hpp"


int main(int argc, char** argv)
{
	auto inst = worker::WorkerPool::Instance();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	// inst->Terminate();
	std::cout << "Done" << std::endl;
	return 0;
}