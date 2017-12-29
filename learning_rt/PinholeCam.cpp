#include "stdafx.h"
#include "PinholeCam.h"
#include<opencv2\opencv.hpp>
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
	if (current_h_idx * g_blocksize >= height)
	{
		return false;
	}

	mtx.lock();
	out_w_idx = current_w_idx;
	out_h_idx = current_h_idx;

	current_w_idx++;
	// current row done, fetch next one
	if (current_w_idx * g_blocksize >= width)
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
	const int hw = width / 2;
	const int hh = height / 2;
	const glm::vec3 lookat = cent + glm::vec3(half_room * (w - hw)/ hw , half_room * (h - hh) / hw,  0.0f);

	r.o_ = eye;
	r.d_ = glm::normalize(lookat - eye);
	r.tMax_ = std::numeric_limits<float>::infinity();
}

void PinholeCam::ThreadMain()
{
	while (true)
	{
		glm::vec4 image_blocks[g_blocksize][g_blocksize] = {};
		int w_base_idx, h_base_idx;
		if (FetchBlock(w_base_idx, h_base_idx) == false)
			break;
		// do rendering

		for (int w_idx = 0; w_idx < g_blocksize; w_idx++)
		{
			for (int h_idx = 0; h_idx < g_blocksize; h_idx++)
			{
				Ray main_ray;
				int pixel_w = w_idx + w_base_idx * g_blocksize;
				int pixel_h = h_idx + h_base_idx * g_blocksize;
				GenRay(pixel_w, pixel_h, main_ray);
				if(pixel_w == pixel_h)
					image_blocks[w_idx][h_idx] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				else
					image_blocks[w_idx][h_idx] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		
		// copy image blocks back
		for (int w_idx = 0; w_idx < g_blocksize; w_idx++)
		{
			for (int h_idx = 0; h_idx < g_blocksize; h_idx++)
			{
				int pixel_w = w_idx + w_base_idx * g_blocksize;
				int pixel_h = h_idx + h_base_idx * g_blocksize;
				image_[pixel_w][pixel_h] = image_blocks[w_idx][h_idx];
				if(pixel_w > height)
					std::cout <<"w"<< pixel_h << std::endl;
				if (pixel_h > width)
					std::cout << "h" <<pixel_h << std::endl;

			}
		}

		//printf("rendering blocks %d %d\n", w_idx, h_idx);
	}
}

void PinholeCam::run()
{
	unsigned int num_core = std::thread::hardware_concurrency();
	num_core = 1;
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

void PinholeCam::showImage()
{
	auto clp = [](float a) { if (a > 255) return 255.0f; else if (a < 0)return 0.0f; return a; };
	cv::Mat img(cv::Size(width, height), CV_8UC4);
	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++)
		{

			auto &pixel = img.at<cv::Vec4b>(w, h);
			pixel[0] = (uchar)clp(image_[w][h].b * 255);
			pixel[1] = (uchar)clp(image_[w][h].g * 255);
			pixel[2] = (uchar)clp(image_[w][h].r * 255);
			pixel[3] = (uchar)(255);

		}

	}
	cv::namedWindow("result");
	cv::imshow("result", img);
	cv::imwrite("test.bmp", img);
	cv::waitKey(0);
}