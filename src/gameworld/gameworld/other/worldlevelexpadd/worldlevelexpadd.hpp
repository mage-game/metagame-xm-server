#ifndef _WORLDLEVELEXPADD_HPP_
#define _WORLDLEVELEXPADD_HPP_
#include "scene/worldshadow/worldshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/worldlevelexpadd/worldlevelexpaddconfig.hpp"

class WorldLevelDifferenceExpAdd 
{
public:
	static WorldLevelDifferenceExpAdd & Instance();
	int GetWorldLevelDifferenceExpAddPercent(int role_level);
	

private:
	WorldLevelDifferenceExpAdd();
	~WorldLevelDifferenceExpAdd();
};

#endif
