#pragma once
#include"rtType.h"

class Ray;

class Triangle
{
public:
	Triangle();
	~Triangle();
	bool Intersect (const Ray & r)const;
private:
	int i;

};

