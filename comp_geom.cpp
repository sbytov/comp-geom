// comp_geom.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <cassert>
#include "segment.h"

enum class kind {
	begin = 0,
	end= 1,
	intersect = 2
};

struct event {
	kind k;
	point p;
	segment* s1;
	segment* s2;
};

std::multiset<event> pq;
int cur_x;
struct status_comparator {
	bool operator() (segment* const& lhs, segment* const& rhs) const {
		auto lhs_y = lhs->y(cur_x);
		auto rhs_y = rhs->y(cur_x);
		if (lhs_y == rhs_y) {
			return lhs->end.y > rhs->end.y;
		}
		return lhs_y > rhs_y;
	}
};
std::set<segment*, status_comparator> status;

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

void calc_intersection(const segment& s1, const segment& s2) {
    std::cout << "test intersection: " << s1 << " and " << s2 << std::endl;


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
		cur_x = e->p.x;
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
				auto range = status.equal_range(e->s1);
				for (auto it = range.first; it != range.second; it++) {
					if (*it == e->s1) {
						status.erase(it); break;
					}
				}
			}
			break;
		}
	}

    return 0;
}
