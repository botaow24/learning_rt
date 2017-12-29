#pragma once
const int g_blocksize = 64;
const int g_color_channel = 4;

class Ray;

class PinholeCam
{
	//static part

	static const int height = 1920/2;
	static const int width = 1920/2;
public:
	~PinholeCam() ;

	static PinholeCam & GetIns() {
		static PinholeCam cam;
		return cam; 
	}

	void run();
	void showImage();
private:
	PinholeCam() ;
	void ThreadMain();
	bool FetchBlock(int & out_w_idx,int & out_h_idx);
	void GenRay(int w, int h, Ray & r);
private:

	glm::vec4 image_[width][height]; // final image result
};
