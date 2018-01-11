#pragma once
#include "BB3.h"
class Triangle;
class Ray;
class SurfaceInteraction;

class BVHNode
{
public:
	BVHNode() {}
	~BVHNode() { delete child[0], delete child[1]; }
	size_t start_id;
	size_t num;
	BB3 bb_;
	char axis_;        // xyz axis
	BVHNode *child[2] = {nullptr,nullptr};
};

class BVHFlatNode
{
public:
	BB3 bb_;
	size_t start_id_or_second_child;
	size_t number_of_triangle;
	char axis_;        // xyz axis
};

class BVH
{
public:
	enum class SplitMethod { equal_size, SAH };
	BVH();
	~BVH();
	
	bool Intersect(const Ray & r, SurfaceInteraction & surf, const Triangle * &)const;

	//BVHNode *root_;
	void BVHBuilder(std::vector<Triangle *>&);
private:
	BVHNode * BVHBuilder(std::vector<Triangle *>&, size_t start, size_t end);
	size_t buildFlatBVH(BVHNode * node_root);
private:
	std::vector<BVHFlatNode> bvh_node_;
	std::vector<Triangle *> triangle_order_list_;
	SplitMethod sm = SplitMethod::SAH;
};

