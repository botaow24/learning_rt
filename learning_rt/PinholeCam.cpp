#include "stdafx.h"
#include "PinholeCam.h"
#include "rtType.h"
#include "Ray.h"

PinholeCam::PinholeCam()
{
}


PinholeCam::~PinholeCam()
{
}

// fetching blocks row by row
bool PinholeCam::FetchBlock(int & out_w_idx, int & out_h_idx)
{
	static std::mutex mtx;

	// image blocks index
	// every block has 64 by 64 pixels (g_blocksize = 64)
	static int current_w_idx = 0;
	static int current_h_idx = 0;

	// row id out of range, image done;
	if (current_h_idx * g_blocksize > height)
	{
		return false;
	}

	mtx.lock();
	out_w_idx = current_w_idx;
	out_h_idx = current_h_idx;

	current_w_idx++;
	// current row done, fetch next one
	if (current_w_idx * g_blocksize > weight)
	{
		current_w_idx = 0;
		current_h_idx++;
	}

	mtx.unlock();
	return true;
}

void PinholeCam::GenRay(int w, int h, Ray & r)
{
	const glm::vec3 eye = glm::vec3(0.0f, 0.0f, -4.0f);
	const glm::vec3 cent = glm::vec3(0.0f, 0.0f, 0.0f);
	const float half_room = 4;
	const int hw = weight / 2;
	const int hh = height / 2;
	const glm::vec3 lookat = cent + glm::vec3(half_room * (w - hw)/ hw , half_room * (h - hh) / hw,  0.0f);

	r.o_ = eye;
	r.d_ = glm::normalize(lookat - eye);
	r.tMax_ = std::numeric_limits<float>::infinity();
}

void PinholeCam::ThreadMain()
{
	glm::vec4 image_blocks[g_blocksize][g_blocksize] = {};
	int w_idx, h_idx;
	while (true)
	{
		if (FetchBlock(w_idx, h_idx) == false)
			break;

		for (int x_idx = 0; x_idx < g_blocksize; x_idx++)
		{
			for (int y_idx = 0; y_idx < g_blocksize; y_idx ++ )
			{
				Ray();
				int pixel_x = x_idx + w_idx * g_blocksize;
				int pixel_y = y_idx + h_idx * g_blocksize;
			}
		}

		
		// do rendering
		// copy image blocks back


		//printf("rendering blocks %d %d\n", w_idx, h_idx);
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