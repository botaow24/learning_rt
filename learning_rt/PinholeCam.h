#pragma once
const int g_blocksize = 64;
const int g_color_channel = 4;

class Ray;

class PinholeCam
{
	//static part

	static const int height = 960;
	static const int width = 960;
public:
	~PinholeCam() ;

	static PinholeCam & GetIns() {
		static PinholeCam cam;
		return cam; 
	}

	void run();

private:
	glm::vec4 shader(Ray &);
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

};
