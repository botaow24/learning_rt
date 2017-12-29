#pragma once

class Triangle;
class Scene
{
public:
	static Scene & GetIns() { static Scene ms; return ms; }
	~Scene();

	void Load();

private:
	Scene();
	void objLoader(const std::string & objname,const std::string & folder_path);

	tinyobj::attrib_t attrib_;
	std::vector<tinyobj::shape_t> shapes_;
	std::vector<tinyobj::material_t> materials_;

	std::vector<Triangle> triangle_v_;
};

