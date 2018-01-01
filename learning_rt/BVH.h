#pragma once
#include "BB3.h"
class Triangle;
class Ray;

class BVHNode
{
public:
	BVHNode() {}
	~BVHNode() { delete child[0], delete child[1]; }
	size_t start_id;
	size_t num;
	BB3 bb_;
	BVHNode *child[2] = {nullptr,nullptr};
};


class BVH
{
public:
	enum class SplitMethod { equal_size };
	BVH();
	~BVH();
	
	bool Intersect(const Ray & r, glm::vec3 & out_barycentric, const Triangle * &)const;
	BVHNode * BVHBuilder(std::vector<Triangle *>&, size_t start, size_t end);
	
	std::vector<Triangle *> triangle_order_list_;
	SplitMethod sm = SplitMethod::equal_size;
	BVHNode *root_;
};

