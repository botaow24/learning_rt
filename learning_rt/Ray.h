#pragma once
class Ray
{
public:
	Ray();
	Ray(const glm::vec3 &_o, const glm::vec3 & _d,float _tMax = std::numeric_limits<float>::infinity());
	~Ray();
	glm::vec3 o_;
	glm::vec3 d_;
	mutable float tMax_;
};

