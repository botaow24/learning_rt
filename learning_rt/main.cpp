// learning_rt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include "PinholeCam.h"
#include "Scene.h"
int main()
{
	Scene::GetIns().Load();
	PinholeCam::GetIns().run();

	return 0;
}

