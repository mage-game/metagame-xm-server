#ifndef _UPGRADESYSCRIT_HPP_
#define _UPGRADESYSCRIT_HPP_
#include "scene/worldshadow/worldshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "upgradesyscritconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "obj/character/role.h"



class UpgradeSysCrit 
{
public:
	static UpgradeSysCrit & Instance();
	int CalculateCritRate(Role* role, int type);
	int CalculateCritValue(Role* role, int type);

	

private:
	UpgradeSysCrit();
	~UpgradeSysCrit();
	int UpgradeSysTypeToJinjieActThemeType(int type);
};

#endif /*_UPGRADESYSCRIT_HPP_*/
