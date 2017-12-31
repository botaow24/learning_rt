#pragma once

class Ray;

class Triangle
{
public:
	Triangle(int idx, tinyobj::mesh_t *, tinyobj::attrib_t *);
	~Triangle();
	int GetMatId() const;
	int GetIdx() const { return p_idx; }
	bool Intersect (const Ray & r , glm::vec3 & out_barycentric,float & out_thit)const;
	static bool Intersect(const Ray & r,const glm::vec3(& p)[3], glm::vec3 & out_barycentric, float & out_thit);
private:
	const int p_idx;

	const tinyobj::mesh_t * mesh_;
	const tinyobj::attrib_t * attrib_;
};

