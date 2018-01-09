#include "stdafx.h"
#include "SurfaceInteraction.h"
#include "Ray.h"

SurfaceInteraction::SurfaceInteraction()
{
}


SurfaceInteraction::~SurfaceInteraction()
{
}

Ray SurfaceInteraction::SpawnRay(const glm::vec3 & dir)
{
	Ray r(point_ + normal_ * 0.0001f, dir);
	return r;
}
