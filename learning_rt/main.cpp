// learning_rt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<opencv2\opencv.hpp>

int main()
{
	// version 1: test openCV lib
	using namespace cv;
	Mat img = imread("C:\\test\\lena3.tif", CV_LOAD_IMAGE_COLOR);
	imshow("Picture", img);
	waitKey(0);
	return 0;
}

