#include "stdafx.h"
#include "BVH.h"
#include "Triangle.h"
#include "BB3.h"
#include "Ray.h"
BVH::BVH()
{
}


BVH::~BVH()
{
}

size_t BVH::buildFlatBVH(BVHNode * node_root)
{
	BVHFlatNode flat;
	flat.bb_ = node_root->bb_;
	flat.number_of_triangle = node_root->num;
	bvh_node_.emplace_back(flat);
	size_t current_idx = bvh_node_.size() - 1;;

	if (node_root->num != 0) // leaf case
	{
		bvh_node_[current_idx].start_id_or_second_child = node_root->start_id;
		return current_idx;
	}
	else // (node_root->num != 0)
	{
		buildFlatBVH(node_root->child[0]);
		size_t second_idx = buildFlatBVH(node_root->child[1]);
		bvh_node_[current_idx].start_id_or_second_child = second_idx;
		return current_idx;
	}
}

bool BVH::Intersect(const Ray & r, SurfaceInteraction & surf, const Triangle * &tri_hit) const
{
	bool hit = false;
	tri_hit = nullptr;
	size_t currentindex = 0;
	std::vector<size_t> to_visit;
	while (true)
	{
		const BVHFlatNode &  current = bvh_node_[currentindex];
		float a, b;
		if (current.bb_.Intersect(r, a, b))
		{
			if (current.number_of_triangle > 0)
			{
				for (size_t st = current.start_id_or_second_child; st < current.start_id_or_second_child + current.number_of_triangle; st++)
				{
					glm::vec3 bec;
					float tHit;
					const Triangle & tri = * triangle_order_list_[st];
					if (tri.Intersect(r, bec, tHit) == true)
					{
						r.tMax_ = tHit;
						tri_hit = &tri;
						hit = true;
					}
				}
				if (to_visit.size() == 0)
					break;
				currentindex = to_visit.back();
				to_visit.pop_back();
			}
			else // current.number_of_triangle > 0
			{
				to_visit.emplace_back(current.start_id_or_second_child);
				currentindex = currentindex++;
			}
		}
		else
		{
			if (to_visit.size() == 0)
				break;
			currentindex = to_visit.back();
			to_visit.pop_back();
		}
	}
	return hit;
}

void BVH::BVHBuilder(std::vector<Triangle*>&tri_list)
{
	auto root = BVHBuilder(tri_list,0, tri_list.size());
	buildFlatBVH(root);
	delete root;
	std::cout << "bvh" << bvh_node_.size() << std::endl;
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
