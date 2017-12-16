#pragma once
class PinholeCam
{
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
	bool FetchBlock();
	const int height_ = 1920;
	const int weight_ = 1920;
	const int blocksize_ = 64;
};

