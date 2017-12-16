#include "stdafx.h"
#include "PinholeCam.h"



PinholeCam::PinholeCam()
{
}


PinholeCam::~PinholeCam()
{
}

bool PinholeCam::FetchBlock()
{
	static std::mutex mtx;
	mtx.lock();

	mtx.unlock();
	return false;
}

void PinholeCam::ThreadMain()
{
	while (true)
	{
		if (FetchBlock() == false)
			break;
	}
}

void PinholeCam::run()
{
	unsigned int num_core = std::thread::hardware_concurrency();
	if (num_core == 0)
	{
		std::cout << "Get core count failed" << std::endl;
		return;
	}
	else
	{
		std::cout << "Launching "<< num_core << std::endl;
	}
	std::vector<std::thread> thread_vector;

	for (unsigned int i = 0; i < num_core; i++)
	{
		thread_vector.emplace_back(&PinholeCam::ThreadMain,this);
	}

	for (auto &ti : thread_vector)
	{
		ti.join();
	}
	std::cout << "All threads joined "  << std::endl;
}