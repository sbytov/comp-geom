#include "segment.h"
#include <cassert>

void intersection_test() {
	//intersection tests
	segment s1 = { { 0,0 }, { 10,10 } };
	auto ip = s1.intersection({ { 0,0 }, { 10,0 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<1>(ip).y == 0);
	ip = s1.intersection({ { 0,10 }, { 10,10 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 10);
	assert(std::get<1>(ip).y == 10);
	ip = s1.intersection({ { 0,5 }, { 10,5 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 5);
	assert(std::get<1>(ip).y == 5);
	ip = s1.intersection({ { 0,0 }, { 0,10 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<1>(ip).y == 0);
	ip = s1.intersection({ { 10,0 }, { 10,10 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 10);
	assert(std::get<1>(ip).y == 10);

	ip = s1.intersection({ { 8,2 }, { 8,9 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 8);
	assert(std::get<1>(ip).y == 8);

	ip = s1.intersection({ { 2,4 }, { 5,4 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 4);
	assert(std::get<1>(ip).y == 4);

	ip = s1.intersection({ { 10,0 }, { 0,10 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 5);
	assert(std::get<1>(ip).y == 5);

	ip = s1.intersection({ { -1,-1 }, { 10,-1 } });
	assert(std::get<0>(ip) == false);
	ip = s1.intersection({ { 0,11 }, { 11,11 } });
	assert(std::get<0>(ip) == false);
	ip = s1.intersection({ { -1,-1 }, { -1,10 } });
	assert(std::get<0>(ip) == false);
	ip = s1.intersection({ { 11,1 }, { 11,11 } });
	assert(std::get<0>(ip) == false);

	ip = s1.intersection({ { 11,11 }, { 12,12 } });
	assert(std::get<0>(ip) == false);
	ip = s1.intersection({ { -2,-2 }, { -1,-1 } });
	assert(std::get<0>(ip) == false);

	ip = s1.intersection({ { 10,10 }, { 12,0 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 10);

	ip = s1.intersection({ { -2, 10 }, { 0,0 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);

	//collinear
	ip = s1.intersection({ { 10,10 }, { 12,12 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 10);
	assert(std::get<2>(ip).x == 10);

	ip = s1.intersection({ { -2,-2 }, { 0,0 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<2>(ip).x == 0);

	ip = s1.intersection({ { 8,8 }, { 12,12 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 8);
	assert(std::get<2>(ip).x == 10);

	ip = s1.intersection({ { -2,-2 }, { 2,2 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<2>(ip).x == 2);

	ip = s1.intersection({ { 2,2 }, { 5,5 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 2);
	assert(std::get<2>(ip).x == 5);

	ip = s1.intersection({ { 11,11 }, { 12,12 } });
	assert(std::get<0>(ip) == false);

	ip = s1.intersection({ { -2,-2 }, { -1,-1 } });
	assert(std::get<0>(ip) == false);

	ip = s1.intersection({ { 5,5 }, { 2,2 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 2);
	assert(std::get<2>(ip).x == 5);

	//vertical segments
	segment s2{ {0,0}, {0, 10} };
	ip = s2.intersection({ { -5, 5 }, { 5,5 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<2>(ip).y == 5);

	ip = s2.intersection({ { -5, 0 },{ 5,0 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<2>(ip).y == 0);

	ip = s2.intersection({ { 0,2 },{ 0,7 } });
	assert(std::get<0>(ip) == true);
	assert(std::get<1>(ip).x == 0);
	assert(std::get<1>(ip).y == 2);
	assert(std::get<2>(ip).x == 0);
	assert(std::get<2>(ip).y == 7);
}