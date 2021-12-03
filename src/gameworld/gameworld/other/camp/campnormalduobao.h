#ifndef __CAMPNORMALDUOBAO_H__
#define __CAMPNORMALDUOBAO_H__

#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "servercommon/campdef.hpp"
#include <map>
#include "obj/character/monster.h"

class Role;

class CampNormalDuobao
{
public:
	static CampNormalDuobao & Instance();

	void OnEnter(Role *role, int sceneid);
	void OnExist(Role *role);

protected:
	CampNormalDuobao();
	~CampNormalDuobao();
};

#endif

