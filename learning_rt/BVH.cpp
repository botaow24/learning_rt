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

size_t BVH::buildFlatBVH(BVHNode * node_root,int deepth)
{
	bvh_deep = std::max(deepth, bvh_deep);
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
		buildFlatBVH(node_root->child[0], deepth + 1);
		size_t second_idx = buildFlatBVH(node_root->child[1], deepth + 1);
		bvh_node_[current_idx].start_id_or_second_child = second_idx;
		bvh_node_[current_idx].axis_ = node_root->axis_;
		return current_idx;
	}

}

bool BVH::Intersect(const Ray & r, SurfaceInteraction & surf, const Triangle * &tri_hit) const
{
	bool hit = false;
	tri_hit = nullptr;
	size_t currentindex = 0;
	std::vector<size_t> to_visit;
	to_visit.reserve(bvh_deep);

	bool axis_positive[3];
	axis_positive[0] = r.d_.x > 0; // x direction positive 
	axis_positive[1] = r.d_.y > 0; // y direction positive 
	axis_positive[2] = r.d_.z > 0; // z direction positive 

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

					float tHit;
					const Triangle & tri = * triangle_order_list_[st];
					if (tri.Intersect(r, surf, tHit) == true)
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
				if (axis_positive[current.axis_] == true) // visit closer node first
				{
					to_visit.emplace_back(current.start_id_or_second_child);
					currentindex = currentindex++;
				}
				else
				{
					to_visit.emplace_back(currentindex + 1);
					currentindex = current.start_id_or_second_child;
				}
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
	buildFlatBVH(root,1);
	delete root;
	std::cout << "bvh" << bvh_node_.size() <<" deepth " << bvh_deep << std::endl;
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
	if (tri_num <= 1)
	{
		triangle_order_list_.emplace_back(tri_list[start]);
		auto node = new BVHNode;
		node->start_id = triangle_order_list_.size() - 1;
		node->num = 1;
		node->bb_ = allbb;
		return node;
	}
	// get max dim
	char dim = allCent.GetMaxDim();
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
			&tri_list[end - 1] + 1,    // avoid overflow
			[dim](const Triangle *a,
				const Triangle *b) {return a->GetBB().Cent().max_[dim] < b->GetBB().Cent().max_[dim]; });
	}
		break;
	case  BVH::SplitMethod::SAH:
	default:
	{
		if (tri_num < 4) // smaller than 4, use equal_size
		{
			std::nth_element(&tri_list[start], &tri_list[mid],
				&tri_list[end - 1] + 1,    // avoid overflow
				[dim](const Triangle *a,
					const Triangle *b) {return a->GetBB().Cent().max_[dim] < b->GetBB().Cent().max_[dim]; });
			break;
		}
		else // if (tri_num < 4)
		{
			//----------------------------------------------------
			// devided into buckets
			const int n_count = 12;
			struct Bucket 
			{
				int count_triangles_ = 0;
				BB3 bb_;
			};
			Bucket Buckets[n_count];

			for (size_t i = start; i < end; i++)
			{
				int bidx = (int)((allCent.invRange() * (tri_list[i]->GetBB().Cent().max_ - allCent.min_))[dim] * n_count);
				bidx = std::min(bidx, n_count - 1);
				bidx = std::max(bidx, 0); // impossible
				Buckets[bidx].count_triangles_++;
				Buckets[bidx].bb_ = Buckets[bidx].bb_.Union(tri_list[i]->GetBB());
			}
			//-----------------------------------------------------
			// split cost
			float mincost = std::numeric_limits<float>::infinity();
			int best_choose = 0;
			for (int sp_at = 0; sp_at < n_count - 1; sp_at++)
			{
				float cost;
				BB3 bb1, bb2;
				int count1 = 0, count2 = 0;
				for (int i = 0; i <= sp_at; i++)
				{
					bb1 = bb1.Union(Buckets[i].bb_);
					count1 += Buckets[i].count_triangles_;
				}
				for(int i = sp_at + 1; i < n_count - 1; i++)
				{
					bb2 = bb2.Union(Buckets[i].bb_);
					count2 += Buckets[i].count_triangles_;
				}
				cost = 1 + (count1 * bb1.SurfaceArea() + count2 * bb2.SurfaceArea()) / allbb.SurfaceArea();
				if (mincost > cost)
				{
					best_choose = sp_at;
					mincost = cost;
				}
			}
			if (mincost > tri_num) // don't div. create leaft
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
			else
			{
				auto tri_mid = std::partition(&tri_list[start],
					&tri_list[end - 1] + 1,    // avoid overflow
					[=](const Triangle * a) {                                
					int bidx = (int)((allCent.invRange() * (a->GetBB().Cent().max_ - allCent.min_))[dim] * n_count);
					bidx = std::min(bidx, n_count - 1);
					bidx = std::max(bidx, 0); // impossible
					return bidx <= best_choose;	}
				);

				mid = tri_mid - &tri_list[0];
			}

		}
	}
		break;
	}
	auto root = new BVHNode;
	auto child0 = BVHBuilder(tri_list, start, mid);
	auto child1 = BVHBuilder(tri_list, mid, end);
	root->child[0] = child0;
	root->child[1] = child1;
	root->axis_ = dim;
	root->start_id = 0;
	root->num = 0;
	root->bb_ = allbb;
	return root;
}
