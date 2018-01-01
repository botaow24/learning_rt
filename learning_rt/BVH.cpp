#include "stdafx.h"
#include "BVH.h"
#include "Triangle.h"
#include "BB3.h"
BVH::BVH()
{
}


BVH::~BVH()
{
}

bool BVH::Intersect(const Ray & r, glm::vec3 & out_barycentric, const Triangle * &) const
{
	return false;
}

BVHNode * BVH::BVHBuilder(std::vector<Triangle *>& tri_list, size_t start, size_t end)
{
	size_t tri_num = end - start;
	BB3 allbb;
	BB3 allCent;
	for (size_t i = start; i < end; i++)
	{
		BB3 Tri = tri_list[i]->GetBB();
		allbb = allbb.Union(Tri);
		allCent = allCent.Union(Tri.Cent());
	}
	// only one node, return left
	if (tri_num == 1)
	{
		triangle_order_list_.emplace_back(tri_list[start]);
		auto node = new BVHNode;
		node->start_id = triangle_order_list_.size() - 1;
		node->num = 1;
		node->bb_ = allbb;
		return node;
	}
	// get max dim
	int dim = allCent.GetMaxDim();
	// can't split
	if (allCent.max_[dim] == allCent.min_[dim])
	{
		auto node = new BVHNode;
		node->start_id = triangle_order_list_.size();
		node->num = tri_num;
		node->bb_ = allbb;
		for (size_t i = start; i < end; i++)
		{
			triangle_order_list_.emplace_back(tri_list[i]);
		}
		return node;
	}
	size_t mid = (start + end) / 2;
	switch (sm)
	{
	case BVH::SplitMethod::equal_size:
	{
		std::nth_element(&tri_list[start], &tri_list[mid],
			&tri_list[end - 1] + 1,
			[dim](const Triangle *a,
				const Triangle *b) {
			return a->GetBB().Cent().max_[dim] < b->GetBB().Cent().max_[dim];
		});
	}
		break;
	default:
		break;
	}
	auto root = new BVHNode;
	auto child0 = BVHBuilder(tri_list, start, mid);
	auto child1 = BVHBuilder(tri_list, mid, end);
	root->child[0] = child0;
	root->child[1] = child1;

	root->start_id = 0;
	root->num = 0;
	root->bb_ = allbb;
	return root;
}
