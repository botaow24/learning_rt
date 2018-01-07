#pragma once
class Triangle;
class Ray;
class SurfaceInteraction
{
public:
	SurfaceInteraction();
	~SurfaceInteraction();
	Ray SpawnRay(const glm::vec3 & dir);
	glm::vec3 point_;
	glm::vec3 normal_;
	const Triangle *ptr = nullptr;
};

