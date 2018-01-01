#include "stdafx.h"
#include "BB3.h"


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

int BB3::GetMaxDim()
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
