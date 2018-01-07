#pragma once

class Triangle;
class Ray;
class BVH;
class SurfaceInteraction;

class Light
{
public:
	Light():zero_one(0.0, 1.0) {mt.seed((unsigned int)time(nullptr)); }

	bool testMesh( const tinyobj::shape_t * s)const { return s == li_mesh; };
	bool testMesh(const tinyobj::mesh_t * s)const { return s == &li_mesh->mesh; };
	glm::vec3 GenOneSample();
	glm::vec4 color_;

	const tinyobj::shape_t * li_mesh;
	std::vector<Triangle *> li_triangle_;
	std::vector<float> area;
	float total_area_ = 0;
	std::mt19937 mt;
	std::discrete_distribution<> dst;
	std::uniform_real_distribution<double> zero_one;
};

class Scene
{
public:
	static Scene & GetIns() { static Scene ms; return ms; }
	~Scene();

	void Load();
	void findIntersectNoAccel (Ray & r, glm::vec3 & bec,const Triangle * & tri_hit) const;
	void findIntersectBVH(Ray & r, SurfaceInteraction & surf, const Triangle * & tri_hit) const;
	const std::vector<tinyobj::material_t> & GetMat()const  {	return materials_;	};
	Light *li = nullptr;
private:
	Scene();
	void initLi();
	void initNoAccel();
	void initBVH();
	void objLoader(const std::string & objname,const std::string & folder_path);

	tinyobj::attrib_t attrib_;
	std::vector<tinyobj::shape_t> shapes_;
	std::vector<tinyobj::material_t> materials_;

	std::vector<Triangle> triangle_v_;
	BVH * bvh = nullptr;

	const std::string light_name = "pCube2";
};

