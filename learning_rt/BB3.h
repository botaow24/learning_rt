#pragma once
class Ray;
class BB3
{
public:
	BB3():min_(std::numeric_limits<float>::infinity()), max_(-std::numeric_limits<float>::infinity()){};
	BB3(glm::vec3);
	~BB3();
	BB3 Union(const BB3 &);
	BB3 Cent();
	float SurfaceArea();
	char GetMaxDim();
	glm::vec3 invRange() const ;
	bool Intersect( const Ray & ,float & tClose, float &tFar) const;
	glm::vec3 min_;
	glm::vec3 max_;
};

