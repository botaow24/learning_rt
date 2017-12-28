#include "stdafx.h"
#include "Ray.h"


Ray::Ray()
	:tMax_(std::numeric_limits<float>::infinity())
{
}

Ray::Ray(const glm::vec3 & _o, const glm::vec3 & _d, float _tMax /*= std::numeric_limits<float>::infinity()*/)
	:o_(_o),d_(_d),tMax_(_tMax)
{
}


Ray::~Ray()
{
}
