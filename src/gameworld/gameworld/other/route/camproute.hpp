#ifndef __CAMPROUTE_HPP__
#define __CAMPROUTE_HPP__

class Role;

class CampRoute
{
public:
	static CampRoute & Instance();

private:
	CampRoute() {}
	~CampRoute() {}
};

#endif // __CAMPROUTE_HPP__

