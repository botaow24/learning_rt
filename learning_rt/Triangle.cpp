#include "stdafx.h"
#include "Triangle.h"

#include"Ray.h"

Triangle::Triangle()
{
}


Triangle::~Triangle()
{
}


bool Triangle::Intersect(const Ray & r) const
{
	glm::vec3 p0, p1, p2;

	// move Ray to (0,0,0)
	glm::vec3 p0t = p0 - r.o_;
	glm::vec3 p1t = p1 - r.o_;
	glm::vec3 p2t = p2 - r.o_;

	//permute
	glm::vec3 rd = r.Permute2Z( r.d_);
	p0t = r.Permute2Z(p0t);
	p1t = r.Permute2Z(p1t);
	p2t = r.Permute2Z(p2t);

	// shear to Z direction
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
	// all zero
	if (b01 + b12 + b20 == 0.0f)
		return false;

}
