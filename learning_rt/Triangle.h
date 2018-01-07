#pragma once

class Ray;
class BB3;
class SurfaceInteraction;
class Triangle
{
public:
	Triangle(int idx, tinyobj::mesh_t *, tinyobj::attrib_t *);
	~Triangle();
	float Area();
	glm::vec3 SampleOne(double r1, double r2);
	BB3 GetBB() const;
	const tinyobj::mesh_t * GetMesh()const { return mesh_; }
	int GetMatId() const;
	int GetIdx() const { return p_idx; }
	bool Intersect (const Ray & r , glm::vec3 & out_barycentric,float & out_thit)const;
	bool Intersect(const Ray & r, SurfaceInteraction &, float & out_thit)const;
	static bool Intersect(const Ray & r,const glm::vec3(& p)[3], glm::vec3 & out_barycentric, float & out_thit);
	bool Intersect(const Ray & r, const glm::vec3(&p)[3], SurfaceInteraction &, float & out_thit) const;
private:
	void GetNorm(glm::vec3(&p)[3]) const;
	const int p_idx;

	const tinyobj::mesh_t * mesh_;
	const tinyobj::attrib_t * attrib_;
};

