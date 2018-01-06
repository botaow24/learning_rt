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
	BVHNode *child[2] = {nullptr,nullptr};
};

class BVHFlatNode
{
public:
	BB3 bb_;
	size_t start_id_or_second_child;
	size_t number_of_triangle;
};

class BVH
{
public:
	enum class SplitMethod { equal_size };
	BVH();
	~BVH();
	
	bool Intersect(const Ray & r, SurfaceInteraction & surf, const Triangle * &)const;


	SplitMethod sm = SplitMethod::equal_size;
	//BVHNode *root_;


	void BVHBuilder(std::vector<Triangle *>&);
private:
	BVHNode * BVHBuilder(std::vector<Triangle *>&, size_t start, size_t end);
	size_t buildFlatBVH(BVHNode * node_root);
	std::vector<BVHFlatNode> bvh_node_;
	std::vector<Triangle *> triangle_order_list_;
};

