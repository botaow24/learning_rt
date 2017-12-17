#pragma once
const int g_blocksize = 64;
const int g_color_channel = 4;


class PinholeCam
{
	//static part

	static const int height = 1920;
	static const int weight = 1920;
public:
	~PinholeCam() ;

	static PinholeCam & GetIns() {
		static PinholeCam cam;
		return cam; 
	}

	void run();

private:
	PinholeCam() ;
	void ThreadMain();
	bool FetchBlock(int & out_w_idx,int & out_h_idx);
private:

	float image_[height][weight][g_color_channel]; // final image result
};

