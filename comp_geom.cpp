// comp_geom.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <cassert>
#include <utility>
#include <algorithm>
#include "segment.h"

enum class kind {
	begin = 0,
	intersect = 1,
	end= 2,
};

struct event {
	kind k;
	point p;
	segment* s1;
	segment* s2;
};

std::multiset<event> pq;
float cur_x;
using segment_ptr = segment*;


struct status_comparator {
	bool operator() (segment_ptr const& lhs, segment_ptr const& rhs) const {
		auto lhs_y = (*lhs).y(cur_x);
		auto rhs_y = (*rhs).y(cur_x);
		if (lhs_y == rhs_y) {
			auto lhs_slope = (*lhs).slope();
			auto rhs_slope = (*rhs).slope();
			if (lhs_slope == rhs_slope) {
				return lhs->end.y > rhs->end.y;
			}
			return lhs_slope > rhs_slope;
		}
		return lhs_y > rhs_y;
	}
};

std::set<segment_ptr, status_comparator> status;
std::vector<point> intersections;

bool operator<(const event& lhs, const event& rhs) {
	if (lhs.p == rhs.p) {
		return lhs.k < rhs.k;
	}
	return lhs.p < rhs.p;
}

std::ostream & operator << (std::ostream &out, const kind &c)
{
	switch (c)
	{
	case kind::begin: 
		out << "begin";
		break;
	case kind::end:
		out << "end";
		break;
	case kind::intersect:
		out << "inter";
		break;
	}
	return out;
}

struct convex {
	convex(std::vector<point> points) {
		for (int i = 0; i < points.size()-1; i++) {
			segments.push_back(segment{ points[i], points[i + 1] });
		}
		segments.push_back(segment{ *(points.end() - 1), points[0] });
	}

	std::vector<segment> segments;
};

void add_segments(convex& c) {
	for (auto& s : c.segments) {
		if (s.begin.x < s.end.x) {
			pq.insert(event{kind::begin, s.begin, &s, nullptr});
			pq.insert(event{kind::end, s.end, &s, nullptr});
		}
		else {
			pq.insert(event{kind::begin, s.end, &s, nullptr});
			pq.insert(event{kind::end, s.begin, &s, nullptr});
		}
	}
}

void calc_intersection(segment& s1, segment& s2) {
    std::cout << "test intersection: " << s1 << " and " << s2 << std::endl;
	auto i = s1.intersection(s2);
	if (std::get<0>(i)) {
		auto i_start = std::get<1>(i);
		if (i_start.x >= cur_x)
		{
			if (i_start == s1.begin /*|| i_start == s1.end*/
				|| i_start == s2.begin /*|| i_start == s2.end*/) {
                std::cout << "+++ add intersection at " << i_start;
				intersections.push_back(i_start);
			} else {
				auto i_end = std::get<2>(i);
				pq.insert(event{kind::intersect, i_start, &s1, &s2});
                std::cout << "+++ add intersection at " << i_start;
				if (!(i_start == i_end)) {
					pq.insert(event{kind::intersect, i_end, &s1, &s2});
                    std::cout << "+++ add intersection at " << i_end;
				}
			}
		}
	}

}

int main()
{
	intersection_test();

	convex c1 = { { point{0,0}, point{10,10}, point{20, 0} } };
	convex c2 = { { point{1,1}, point{11,11}, point{21, 1} } };
	add_segments(c1);
	add_segments(c2);


	for (auto& e : pq) {
		std::cout << e.k << "| " << e.p << " of " << "[" << e.s1->begin << " - " << e.s1->end << "]" << std::endl;
	}

	for (auto e = pq.begin(); e != pq.end(); e++) {
        auto prev_x = cur_x;
		cur_x = (float)e->p.x;
        std::cout << "--------------- cur_x is " << cur_x << "--------------------" << std::endl;
		switch (e->k) {
			case kind::begin:
			{
				auto res = status.insert(e->s1);
                if (res.second)
                {
                    auto it = res.first;
                    if (it != status.begin())
                    {
                        auto prev = it;
                        prev--;
                        calc_intersection(**it, **(prev));
                    } 
                    if (it != (--status.end()))
                    {
                        auto next = it;
                        next++;
                        calc_intersection(**it, **(next));
                    }
                }
                else
                {
                    std::cout << "WARNING: segment insertion failed as such segment already exists, this is unexpected" << std::endl;
                }
			}
			break;
			case kind::end:
			{
				auto it = status.find(e->s1);
				std::cout << "remove " << *e->s1 << std::endl;
				auto next = status.erase(it);
				if (next != status.end() && next != status.begin()) {
					calc_intersection(**(--next), **(next));
				}
			}
			break;
			case kind::intersect:
			{	
				intersections.push_back(e->p);
                auto temp_curx = cur_x;
                cur_x = (prev_x + cur_x)/2;
				auto it1 = status.find(e->s1);
				auto it2 = status.find(e->s2);
				bool order = e->s1->y(cur_x) > e->s2->y(cur_x);
				if (!order) std::swap(it1, it2);
				auto upper = status.end(), lower = status.end();
				if (it1 != status.begin())
					upper = std::prev(it1);
				if (it2 != status.end())
					lower = std::next(it2);
				std::swap((segment_ptr&)*it1, (segment_ptr&)*it2);
                cur_x = temp_curx;
				if (upper != status.end())
					calc_intersection(**it1, **upper);
				if (lower != status.end())
					calc_intersection(**it2, **lower);
			}
			break;
		}
	}


    for (auto& i : intersections)
    {
        std::cout << i << std::endl;
    }
    return 0;
}
