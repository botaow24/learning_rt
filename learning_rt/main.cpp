// learning_rt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "PinholeCam.h"
#include "Scene.h"
int main()
{
	Scene::GetIns().Load();
	PinholeCam::GetIns().run();
	PinholeCam::GetIns().showImage();

	return 0;
}

