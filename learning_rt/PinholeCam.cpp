#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include "PinholeCam.h"
#include <thread>
#include <mutex>

#include "rtType.h"
#include "Ray.h"
#include "Scene.h"
#include "Triangle.h"
#include "SurfaceInteraction.h"
PinholeCam::PinholeCam()
	:img_(cv::Size(width, height), CV_8UC4)
{
	mt.seed((unsigned int)time(nullptr));
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
	static int counter = 0;

	mtx.lock();
	// row id out of range, image done;
	if (current_h_idx * g_blocksize >= height)
	{
		mtx.unlock();
		return false;
	}
	out_w_idx = current_w_idx;
	out_h_idx = current_h_idx;

	current_w_idx++;
	// current row done, fetch next one
	if (current_w_idx * g_blocksize >= width)
	{
		current_w_idx = 0;
		current_h_idx++;
	}
	printf("\r%f %% ",counter * 1.0f/ (height* width / g_blocksize/ g_blocksize) * 100);
	counter++;
	mtx.unlock();
	return true;
}

void PinholeCam::GenRay(int w, int h, Ray & r)
{
	const glm::vec3 eye = glm::vec3(0.0f, 5.0f, 15.2f);
	const glm::vec3 cent = eye + glm::vec3(0.0f,0.0f,-8.6595f);
	const float half_room =  5;
	const int hw = width / 2;
	const int hh = height / 2;
	// switch h,w
	// upside down
	const glm::vec3 lookat = cent + glm::vec3( half_room * (h - hh) / hw, - half_room * (w - hw) / hw,  0.0f);

	r.o_ = eye;
	r.d_ = glm::normalize(lookat - eye);
	r.tMax_ = std::numeric_limits<float>::infinity();
}

void PinholeCam::DispThread()
{
	while (rendering_done == false)
	{
		showImage();
		cv::waitKey(100);
	}
	showImage();
	cv::imwrite("test.bmp", img_);

	cv::waitKey(0);
}

glm::vec4 PinholeCam::PathIntegrator(const Ray & ray)
{
	Ray main_ray = ray;
	glm::vec4 result(0.0f) , beta(1.0f);
	const int max_deep = 15;
	for (int iter_num = 0;; iter_num++)
	{
		SurfaceInteraction surf;
		const Triangle *ptr = nullptr;
		Scene::GetIns().findIntersectBVH(main_ray, surf, ptr);
		if (ptr == nullptr || iter_num >= max_deep)
			break;
		int idx = ptr->GetMatId();
		const tinyobj::material_t & mat = Scene::GetIns().GetMat()[idx];
		beta *= glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f);

		if (true)
		{
			auto &li = Scene::GetIns().li;
			glm::vec3 li_pos = li->GenOneSample();
			glm::vec3 dir = glm::normalize(li_pos - surf.point_);
			Ray final_ray = surf.SpawnRay(dir);
			//(surf.point_, );
			final_ray.initRay();
			SurfaceInteraction surffn;
			const Triangle *trfn = nullptr;
			Scene::GetIns().findIntersectBVH(final_ray, surffn, trfn);

			if (trfn != nullptr && li->testMesh(trfn->GetMesh()))
			{
				/*
				glm::vec3 w = glm::normalize(surf.normal_);
				glm::vec3 u = glm::normalize(fabs(w.x) > 0.1 ? glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), w) : glm::cross(glm::vec3(1.0, 0.0, 0.0f), w));
				glm::vec3 v = glm::normalize(glm::cross(w, u));

				glm::vec3 sw = glm::normalize(surffn.normal_);
				glm::vec3 su = glm::normalize(fabs(sw.x) > 0.1 ? glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), sw) : glm::cross(glm::vec3(1.0, 0.0, 0.0f), sw));
				glm::vec3 sv = glm::normalize(glm::cross(sw, su));

				double cos_a_max = sqrt(1 - sp.rad*sp.rad / (x - sp.p).dot(x - sp.p));
				*/
				/*
				float  length = glm::length(surffn.point_ - final_ray.o_);
				if (length < 0.0001)
				{
					result += beta* glm::vec4(1.0f);
				}
				else
				{
					result += (beta * glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f)) / (length);
				}
				*/
				glm::vec4 eng = beta * li->color_ * abs(glm::dot(surf.normal_, dir) * 2 * abs(glm::dot(surffn.normal_, dir)));
				result +=  eng;
			}
		}

		float mb = std::max(beta[0], std::max(beta[1], beta[2]));
		if (iter_num > 0 && zero_one(mt) > mb)
			break;

		// diffuse
		if (true)
		{
			float r1 = zero_one(mt) * 2 * 3.14159265f;
			float r2 = zero_one(mt);
			float sqr2 = sqrt(r2);
			float mx = std::max(mat.diffuse[0], std::max(mat.diffuse[1], mat.diffuse[2]));

			float  length = glm::length(surf.point_ - main_ray.o_);

			glm::vec3 w = glm::normalize(surf.normal_);
			glm::vec3 u = glm::normalize(fabs(w.x) > 0.1 ? glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), w) : glm::cross(glm::vec3(1.0, 0.0, 0.0f), w));
			glm::vec3 v = glm::normalize(glm::cross(w,u));
			glm::vec3 direct = (u*cos(r1)*sqr2 + v*sin(r1)*sqr2 + w*sqrt(1 - r2));
			Ray nr= surf.SpawnRay(direct);
			main_ray = nr;
		}
		if (glm::length(beta) <= 1 + 1e-6f)
			break;

	}
	return result;
}

glm::vec4 PinholeCam::simpleShader(Ray &ray)
{
	SurfaceInteraction surf;
	const Triangle *ptr = nullptr;
	Scene::GetIns().findIntersectBVH(ray, surf, ptr);
	if (ptr != nullptr)
	{
		int idx = ptr->GetMatId();
		const tinyobj::material_t & mat = Scene::GetIns().GetMat()[idx];
		return glm::vec4(mat.diffuse[0], mat.diffuse[1],mat.diffuse[2],1.0f);
		//return glm::vec4(1.0f);
	}
	return glm::vec4(0.0f);
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
				glm::dvec4 dv(0.0);
				Ray main_ray;
				int pixel_w = w_idx + w_base_idx * g_blocksize;
				int pixel_h = h_idx + h_base_idx * g_blocksize;
				GenRay(pixel_w, pixel_h, main_ray);
				const int sample_count = 128;
				for(int i = 0; i < sample_count;i++)
				{
					dv += PathIntegrator(main_ray);
				}
				image_blocks[w_idx][h_idx] = glm::vec4(dv / (double)sample_count);
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
			/*
			int pixel_w = w_idx + w_base_idx * g_blocksize;
			int pixel_h_bg = h_base_idx * g_blocksize;
			std::copy(&image_blocks[w_idx][0], &image_blocks[w_idx][g_blocksize], &image_[pixel_w][pixel_h_bg]);
			*/
		}

		//printf("rendering blocks %d %d\n", w_idx, h_idx);
	}
}

void PinholeCam::run()
{
	std::thread displayThread(&PinholeCam::DispThread, this);

	unsigned int num_core = std::thread::hardware_concurrency();
	//num_core = 1;
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
	auto t1 = std::chrono::high_resolution_clock::now();
	for (unsigned int i = 0; i < num_core; i++)
	{
		thread_vector.emplace_back(&PinholeCam::ThreadMain,this);
	}
	for (auto &ti : thread_vector)
	{
		ti.join();
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "All threads joined "  << std::endl;
	std::cout << "It tooks "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() <<"milliseconds" << std::endl;
	rendering_done = true;
	displayThread.join();
}

void PinholeCam::showImage()
{
	auto clp = [](float a) { if (a > 255) return 255.0f; else if (a < 0)return 0.0f; return a; };
	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++)
		{

			auto &pixel = img_.at<cv::Vec4b>(w, h);
			pixel[0] = (uchar)clp(image_[w][h].b * 255);
			pixel[1] = (uchar)clp(image_[w][h].g * 255);
			pixel[2] = (uchar)clp(image_[w][h].r * 255);
			pixel[3] = (uchar)(255);

		}

	}
	if (height > 960)
	{
		cv::Mat disp;
		cv::resize(img_, disp, cv::Size(960, 960));
		cv::imshow("result", disp);
	}
	else
	{
		cv::imshow("result", img_);
	}
}