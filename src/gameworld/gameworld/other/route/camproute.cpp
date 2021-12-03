#include "camproute.hpp"

CampRoute & CampRoute::Instance()
{
	static CampRoute cr;
	return cr;
}

