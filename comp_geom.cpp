// comp_geom.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <cassert>


struct point {
	int x;
	int y;
};

struct segment {
	point begin;
	point end;

	float y(float x) const {
		float m = slope();
		float y = begin.y + (x - begin.x)*m;
		return y;
	}

	float slope() const {
		return (end.y - begin.y) / (end.x - begin.x);
	}

	std::pair<bool, point> intersection(const segment& s) {
		float dx = end.x - begin.x;
		float dy = end.y - begin.y;
		float s_dx = s.end.x - s.begin.x;
		float s_dy = s.end.y - s.begin.y;
		float xdiff = begin.x - s.begin.x;
		float ydiff = begin.y - s.begin.y;

		float s_numer = -ydiff*dx + xdiff*dy;
		float s_denom = s_dx*dy - s_dy*dx;
		float s_p = s_numer / s_denom;

		float numer = ydiff*s_dx - xdiff*s_dy;
		float denom = dx*s_dy - dy*s_dx;
		float  p = numer / denom;



		bool intersect = p >= 0 && p <= 1 && s_p >= 0 && s_p <= 1;
		point ip;
		if (intersect) {
			ip.x = begin.x * (1 - p) + end.x * p;
			ip.y = begin.y * (1 - p) + end.y * p;
		}
		return{ intersect, ip };
	}
};

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


bool operator==(const point& lhs, const point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator<(const event& lhs, const event& rhs) {
	if (lhs.p == rhs.p) {
		return lhs.k < rhs.k;
	}
	else if (lhs.p.x == rhs.p.x) {
		return lhs.p.y < rhs.p.y;
	}
	return lhs.p.x < rhs.p.x;
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

std::ostream & operator << (std::ostream &out, const point &c)
{
	out << "x=" << c.x << "; y=" << c.y;
	return out;
}

std::ostream & operator << (std::ostream &out, const segment &c)
{
    out << "[" << c.begin << " to " << c.end << "]" << std::endl;
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
//intersection tests
	segment s1 = { point{0,0}, point{10,10} };
	auto ip = s1.intersection({ point{ 0,0 }, point{ 10,0 } });
	assert(ip.first == true);
	assert(ip.second.x == 0);
	assert(ip.second.y == 0);
	ip = s1.intersection({ point{ 0,10 }, point{ 10,10 } });
	assert(ip.first == true);
	assert(ip.second.x == 10);
	assert(ip.second.y == 10);
	ip = s1.intersection({ point{ 0,5 }, point{ 10,5 } });
	assert(ip.first == true);
	assert(ip.second.x == 5);
	assert(ip.second.y == 5);
	ip = s1.intersection({ point{ 0,0 }, point{ 0,10 } });
	assert(ip.first == true);
	assert(ip.second.x == 0);
	assert(ip.second.y == 0);
	ip = s1.intersection({ point{ 10,0 }, point{ 10,10 } });
	assert(ip.first == true);
	assert(ip.second.x == 10);
	assert(ip.second.y == 10);

	ip = s1.intersection({ point{ 8,2 }, point{ 8,9 } });
	assert(ip.first == true);
	assert(ip.second.x == 8);
	assert(ip.second.y == 8);

	ip = s1.intersection({ point{ 2,4 }, point{ 5,4 } });
	assert(ip.first == true);
	assert(ip.second.x == 4);
	assert(ip.second.y == 4);

	ip = s1.intersection({ point{ 10,0 }, point{ 0,10 } });
	assert(ip.first == true);
	assert(ip.second.x == 5);
	assert(ip.second.y == 5);

	ip = s1.intersection({ point{ -1,-1 }, point{ 10,-1 } });
	assert(ip.first == false);
	ip = s1.intersection({ point{ 0,11 }, point{ 11,11 } });
	assert(ip.first == false);
	ip = s1.intersection({ point{ -1,-1 }, point{ -1,10 } });
	assert(ip.first == false);
	ip = s1.intersection({ point{ 11,1 }, point{ 11,11 } });
	assert(ip.first == false);

	ip = s1.intersection({ point{ 11,11 }, point{ 12,12 } });
	assert(ip.first == false);
	ip = s1.intersection({ point{ -2,-2 }, point{ -1,-1 } });
	assert(ip.first == false);

	ip = s1.intersection({ point{ 10,10 }, point{ 12,0 } });
	assert(ip.first == true);
	assert(ip.second.x == 10);

	ip = s1.intersection({ point{ -2, 10 }, point{ 0,0 } });
	assert(ip.first == true);
	assert(ip.second.x == 0);

	//ip = s1.intersection({ point{ 10,10 }, point{ 12,12 } });
	//assert(ip.first == true);
	//assert(ip.second.x == 10);
	//ip = s1.intersection({ point{ -2,-2 }, point{ 0,0 } });
	//assert(ip.first == true);
	//assert(ip.second.x == 0);
//end intersection tests

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

