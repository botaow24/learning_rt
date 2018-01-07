#pragma once
const int g_blocksize = 64;
const int g_color_channel = 4;

class Ray;

class PinholeCam
{
	//static part

	static const int height = 640 ;
	static const int width = 640;
public:
	~PinholeCam() ;

	static PinholeCam & GetIns() {
		static PinholeCam cam;
		return cam; 
	}

	void run();

private:
	glm::vec4 PathIntegrator(const Ray &);
	glm::vec4 simpleShader(Ray &);
	PinholeCam() ;
	void ThreadMain();
	bool FetchBlock(int & out_w_idx,int & out_h_idx);
	void GenRay(int w, int h, Ray & r);
	void DispThread();
	void showImage();
private:
	glm::vec4 image_[width][height]; // final image result
	cv::Mat img_;
	bool rendering_done = false;

	std::mt19937 mt;
	std::uniform_real_distribution<float> zero_one;
};
