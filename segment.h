#pragma once
#include <tuple>
#include <iostream>
#include "point.h"

struct segment {
	point begin;
	point end;

	segment(const point& first, const point& second) {
		bool firstLess = first < second;
		begin = firstLess ? first : second;
		end = firstLess ? second : first;
	}

	float y(float x) const {
		float m = slope();
		float y = begin.y + (x - begin.x)*m;
		return y;
	}

	float slope() const {
		return (end.y - begin.y) / (end.x - begin.x);
	}

	std::tuple<bool, point, point> intersection(const segment& s) const {
		float dx = end.x - begin.x;
		float dy = end.y - begin.y;
		float s_dx = s.end.x - s.begin.x;
		float s_dy = s.end.y - s.begin.y;
		float xdiff = begin.x - s.begin.x;
		float ydiff = begin.y - s.begin.y;

		float s_numer = -ydiff*dx + xdiff*dy;
		float s_denom = s_dx*dy - s_dy*dx;
		float numer = ydiff*s_dx - xdiff*s_dy;
		float denom = dx*s_dy - dy*s_dx;

		if (s_denom == 0 || denom == 0) { //collinear or parallel?
			if (begin.x == end.x && begin.x == s.begin.x //special test for vertical
				|| begin.y == s.begin.x + s.slope()*(begin.x - s.begin.x) //collinear
				) 
			{
				if (s.begin.x > end.x ||
					s.end.x < begin.x) {
					//don't overlap 
					return{ false, point{}, point{} };
				}
				else {
					//overlap
					auto l = s.begin < begin ? begin : s.begin;
					auto u = s.end > end ? end : s.end;
					return{ true, l, u };
				}
			}
			else {
				//parallel
				return{ false, point{}, point{} };
			}
		}

		float s_p = s_numer / s_denom;
		float  p = numer / denom;

		bool intersect = p >= 0 && p <= 1 && s_p >= 0 && s_p <= 1;
		point ip;
		if (intersect) {
			ip.x = begin.x * (1 - p) + end.x * p;
			ip.y = begin.y * (1 - p) + end.y * p;
		}
		return{ intersect, ip, ip };
	}
};


inline std::ostream & operator << (std::ostream &out, const segment &c)
{
	out << "[" << c.begin << " to " << c.end << "]" << std::endl;
	return out;
}


void intersection_test();