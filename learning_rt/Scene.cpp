#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Load()
{
	objLoader("scene_1_obj/scene01.obj");
}

void Scene::objLoader(const std::string & path)
{
	std::ifstream ifp(path);
	if (ifp.good() == false)
	{
		std::cout << "Failed to load scene Obj @" << path << std::endl;
	}
	std::stringstream ss;
	ss << ifp.rdbuf();

	std::string buffer;
	while (ss.good())
	{
		ss >> buffer;
	}
}

void Scene::mtlLoader(const std::string & path)
{
	std::ifstream ifp(path);
	if (ifp.good() == false)
	{
		std::cout << "Failed to load scene Mtl"<< path << std::endl;
	}
	std::stringstream ss;
	ss << ifp.rdbuf();
	
	std::string buffer;
	while (ss.good())
	{
		ss >> buffer;
	}


}
