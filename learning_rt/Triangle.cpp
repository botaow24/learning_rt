#include "stdafx.h"
#include "Triangle.h"

#include"Ray.h"
#include "BB3.h"
#include "SurfaceInteraction.h"
Triangle::Triangle(int idx, tinyobj::mesh_t *ptr_m, tinyobj::attrib_t * ptr_a)
	:p_idx(idx), mesh_(ptr_m), attrib_(ptr_a)
{
}


Triangle::~Triangle()
{
}

float Triangle::Area()
{
	glm::vec3 p[3];
	for (int i = 0; i < 3; i++)
	{
		int vid = mesh_->indices[p_idx + i].vertex_index;
		p[i] = glm::vec3(attrib_->vertices[vid * 3], attrib_->vertices[vid * 3 + 1], attrib_->vertices[vid * 3 + 2]);
	}
	return glm::length(glm::cross(p[2] - p[0], p[1] - p[0])) / 2;
}

glm::vec3 Triangle::SampleOne(double r1,double r2)
{
	glm::vec3 p[3];
	for (int i = 0; i < 3; i++)
	{
		int vid = mesh_->indices[p_idx + i].vertex_index;
		p[i] = glm::vec3(attrib_->vertices[vid * 3], attrib_->vertices[vid * 3 + 1], attrib_->vertices[vid * 3 + 2]);
	}

	float sq1 = (float)sqrt(r1);
	//float sq2 = sqrt(r2);

	return (1-sq1) * p[0] + (sq1 * (1-(float)r2)) * p[1] + ((float)r2 * sq1) * p[2];
}

BB3 Triangle::GetBB() const
{
	BB3 bb[3];
	for (int i = 0; i < 3; i++)
	{
		int vid = mesh_->indices[p_idx + i].vertex_index;
		auto temp = glm::vec3(attrib_->vertices[vid * 3], attrib_->vertices[vid * 3 + 1], attrib_->vertices[vid * 3 + 2]);
		bb[i] = BB3(temp);
	}
	return bb[0].Union(bb[1].Union(bb[2]));
}

int Triangle::GetMatId() const
{
	return mesh_->material_ids[p_idx / 3];
}


bool Triangle::Intersect(const Ray & r,glm::vec3 & out_barycentric, float & out_thit) const
{
	glm::vec3 p[3];
	for (int i = 0; i < 3; i++)
	{
		int vid = mesh_->indices[p_idx + i].vertex_index;
		p[i] = glm::vec3(attrib_->vertices[vid * 3], attrib_->vertices[vid * 3 + 1], attrib_->vertices[vid * 3 + 2]);
	}
	return Intersect(r,p,out_barycentric,out_thit);
}

bool Triangle::Intersect(const Ray & r, SurfaceInteraction & surf, float & out_thit) const
{
	glm::vec3 p[3];
	for (int i = 0; i < 3; i++)
	{
		int vid = mesh_->indices[p_idx + i].vertex_index;
		p[i] = glm::vec3(attrib_->vertices[vid * 3], attrib_->vertices[vid * 3 + 1], attrib_->vertices[vid * 3 + 2]);
	}
	return Intersect(r, p, surf, out_thit);
}

bool Triangle::Intersect(const Ray & r, const glm::vec3(&p)[3], glm::vec3 & out_barycentric, float & out_thit) 
{
	// move Ray to (0,0,0)
	glm::vec3 p0t = p[0] - r.o_;
	glm::vec3 p1t = p[1] - r.o_;
	glm::vec3 p2t = p[2] - r.o_;

	//permute
	glm::vec3 rd = r.Permute2Z(r.d_);
	p0t = r.Permute2Z(p0t);
	p1t = r.Permute2Z(p1t);
	p2t = r.Permute2Z(p2t);

	// shear to Z direction
	// 
	p0t.x += r.sx * p0t.z;
	p0t.y += r.sy * p0t.z;
	p1t.x += r.sx * p1t.z;
	p1t.y += r.sy * p1t.z;
	p2t.x += r.sx * p2t.z;
	p2t.y += r.sy * p2t.z;

	// find intersect on (0,0,z)
	// page 161
	// Barycentric test on (0,0)

	float b01 = p0t.x * p1t.y - p1t.x * p0t.y;
	float b12 = p1t.x * p2t.y - p2t.x * p1t.y;
	float b20 = p2t.x * p0t.y - p0t.x * p2t.y;

	// TODO: double ver
	if (b01 == 0.0f || b12 == 0.0f || b20 == 0.0f)
	{

	}
	// Barycentric test failed (not all same side)
	if ((b01 < 0.0f || b12 < 0.0f || b20 < 0.0f) && (b01 > 0.0f || b12 > 0.0f || b20 > 0.0f))
		return false;

	// all zero, on the edge for all three edges
	float bsum = b01 + b12 + b20;
	if (bsum == 0.0f)
		return false;
	// b0 + b1 + b2  = 1; 
	// all >= 0
	float b0 = b12 / bsum;
	float b1 = b20 / bsum;
	float b2 = b01 / bsum;
	// shear z
	p0t.z *= r.sz;
	p1t.z *= r.sz;
	p2t.z *= r.sz;
	// distance
	float dis = p0t.z * b0 + p1t.z * b1 + p2t.z * b2;
	if (dis < 0 || dis > r.tMax_)
		return false;
	out_barycentric = glm::vec3(b0, b1, b2);
	out_thit = dis;
	return true;
}

bool Triangle::Intersect(const Ray & r, const glm::vec3(&p)[3], SurfaceInteraction & surf, float & out_thit)const
{
	// move Ray to (0,0,0)
	glm::vec3 p0t = p[0] - r.o_;
	glm::vec3 p1t = p[1] - r.o_;
	glm::vec3 p2t = p[2] - r.o_;

	//permute
	glm::vec3 rd = r.Permute2Z(r.d_);
	p0t = r.Permute2Z(p0t);
	p1t = r.Permute2Z(p1t);
	p2t = r.Permute2Z(p2t);

	// shear to Z direction
	// 
	p0t.x += r.sx * p0t.z;
	p0t.y += r.sy * p0t.z;
	p1t.x += r.sx * p1t.z;
	p1t.y += r.sy * p1t.z;
	p2t.x += r.sx * p2t.z;
	p2t.y += r.sy * p2t.z;

	// find intersect on (0,0,z)
	// page 161
	// Barycentric test on (0,0)

	float b01 = p0t.x * p1t.y - p1t.x * p0t.y;
	float b12 = p1t.x * p2t.y - p2t.x * p1t.y;
	float b20 = p2t.x * p0t.y - p0t.x * p2t.y;

	// TODO: double ver
	if (b01 == 0.0f || b12 == 0.0f || b20 == 0.0f)
	{

	}
	// Barycentric test failed (not all same side)
	if ((b01 < 0.0f || b12 < 0.0f || b20 < 0.0f) && (b01 > 0.0f || b12 > 0.0f || b20 > 0.0f))
		return false;

	// all zero, on the edge for all three edges
	float bsum = b01 + b12 + b20;
	if (bsum == 0.0f)
		return false;
	// b0 + b1 + b2  = 1; 
	// all >= 0
	float b0 = b12 / bsum;
	float b1 = b20 / bsum;
	float b2 = b01 / bsum;
	// shear z
	p0t.z *= r.sz;
	p1t.z *= r.sz;
	p2t.z *= r.sz;
	// distance
	float dis = p0t.z * b0 + p1t.z * b1 + p2t.z * b2;
	if (dis < 0 || dis > r.tMax_)
		return false;
	surf.point_ = b0 * p[0] + b1 * p[1] + b2 * p[2];
	surf.ptr = this;
	glm::vec3 norm[3];
	GetNorm(norm);
	surf.normal_ = b0 * norm[0] + b1 * norm[1] + b2 * norm[2];
	if (glm::dot(surf.normal_, r.d_) > 0.0f)
	{
		surf.normal_ = -surf.normal_;
	}
	out_thit = dis;
	return true;
}

void Triangle::GetNorm(glm::vec3(&p)[3]) const
{
	for (int i = 0; i < 3; i++)
	{
		int nid = mesh_->indices[p_idx + i].normal_index;
		p[i] = glm::vec3(attrib_->normals[nid * 3], attrib_->normals[nid * 3 + 1], attrib_->normals[nid * 3 + 2]);
	}
}

