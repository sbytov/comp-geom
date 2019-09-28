#pragma once
#include <iostream>

struct point {
	int x;
	int y;
};

inline bool operator==(const point& lhs, const point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator<(const point& lhs, const point& rhs) {
	if (lhs.x == rhs.x) {
		return lhs.y < rhs.y;
	}
	return lhs.x < rhs.x;
}

inline bool operator>(const point& lhs, const point& rhs) {
	return !(lhs < rhs || lhs == rhs);
}

inline std::ostream& operator << (std::ostream &out, const point &c)
{
	out << "x=" << c.x << "; y=" << c.y;
	return out;
}

