#include "stdafx.h"
#include "BB3.h"
#include "Ray.h"

BB3::BB3(glm::vec3 v)
	:min_(v),max_(v)
{
}


BB3::~BB3()
{
}

BB3 BB3::Union(const BB3 &_other)
{
	BB3 result;
	result.max_.x = std::max(_other.max_.x, this->max_.x);
	result.max_.y = std::max(_other.max_.y, this->max_.y);
	result.max_.z = std::max(_other.max_.z, this->max_.z);

	result.min_.x = std::min(_other.min_.x, this->min_.x);
	result.min_.y = std::min(_other.min_.y, this->min_.y);
	result.min_.z = std::min(_other.min_.z, this->min_.z);
	return result;
}

BB3 BB3::Cent()
{
	return BB3((max_+ min_)/2.0f);
}

float BB3::SurfaceArea()
{
	if (std::isfinite(min_.x) == false)
		return 0.0f;
	glm::vec3 diff = max_ - min_;
	float s1 = diff.x * diff.y;
	float s2 = diff.y * diff.z;
	float s3 = diff.z * diff.x;
	return (s1 + s2 + s3) * 2;
}

char BB3::GetMaxDim()
{
	glm::vec3 diff = max_ - min_;
	if (diff.x > diff.y)
	{
		if (diff.x > diff.z)
			return 0;
		else
			return 2;
	}
	else
	{
		if (diff.y > diff.z)
			return 1;
		else
			return 2;
	}
	return 0;
}

glm::vec3 BB3::invRange() const
{
	glm::vec3 r(0.0f);
	glm::vec3 diff = max_ - min_;
	for (int i = 0; i < 3; i++)
	{
		if (diff[i] > 0)
		{
			r[i] = 1.0f / diff[i];
		}
	}
	return r;
}

bool BB3::Intersect(const Ray &r,float & tClose, float & tFar) const
{
	float t0 = 0;
	float t1 = r.tMax_;
	for (int i = 0; i < 3; i++)
	{
		float invRay = 1 / r.d_[i];
		float near = (min_[i] - r.o_[i]) * invRay;
		float far = (max_[i] - r.o_[i])  * invRay;

		// TODO: robust intersection
		if (near > far)
			std::swap(near, far);
		t0 = std::max(t0,near);
		t1 = std::min(t1, far);
		if (t0 > t1)
			return false;
	}
	tClose = t0;
	tFar = t1;
	return true;
}
