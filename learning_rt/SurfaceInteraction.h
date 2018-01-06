#pragma once
class Triangle;
class SurfaceInteraction
{
public:
	SurfaceInteraction();
	~SurfaceInteraction();
	glm::vec3 point_;
	glm::vec3 normal_;
	const Triangle *ptr = nullptr;
};

