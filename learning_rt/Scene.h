#pragma once

class Triangle;
class Ray;
class Scene
{
public:
	static Scene & GetIns() { static Scene ms; return ms; }
	~Scene();

	void Load();
	void findIntersectNoAccel (Ray & r, glm::vec3 & bec,const Triangle * & tri_hit) const;
	const std::vector<tinyobj::material_t> & GetMat()const  {	return materials_;	};
private:
	Scene();

	void initNoAccel();
	void objLoader(const std::string & objname,const std::string & folder_path);

	tinyobj::attrib_t attrib_;
	std::vector<tinyobj::shape_t> shapes_;
	std::vector<tinyobj::material_t> materials_;

	std::vector<Triangle> triangle_v_;
};

