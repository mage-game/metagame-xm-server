#include "specialcampstation.hpp"

#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"


#include "scene/activityshadow/activityshadow.hpp"

#include "monster/monsterpool.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "internalcomm.h"
#include "other/event/eventhandler.hpp"

SpecialCampStation::SpecialCampStation(Scene *scene, int camp_type) 
	: SpecialLogic(scene), m_camp_type(camp_type)
{

}

SpecialCampStation::~SpecialCampStation()
{

}

void SpecialCampStation::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
}

void SpecialCampStation::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (role->GetCamp() != m_camp_type)
	{
	}
}

void SpecialCampStation::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{

}

void SpecialCampStation::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	
}

void SpecialCampStation::OnCharacterDie(Character *dead, ObjID killer_id)
{
	
}

bool SpecialCampStation::SpecialIsEnemy(Role *role, Obj *obj)
{
	UNSTD_STATIC_CHECK(CAMP_TYPE_MAX >= 4);

	// 特定类型ai的怪物，特定阵营不能攻击
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		const static int friend_monster_ai[CAMP_TYPE_MAX] = 
		{
			-1,
			MonsterInitParam::AITYPE_CISHA_CAMP1,
			MonsterInitParam::AITYPE_CISHA_CAMP2,
			MonsterInitParam::AITYPE_CISHA_CAMP3,
		};

		Monster *monster = (Monster *)obj;

		if (monster->GetAIType() == friend_monster_ai[(int)role->GetCamp()])
		{
			return false;
		}
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialCampStation::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		const static int friend_monster_ai[CAMP_TYPE_MAX] = 
		{
			-1,
			MonsterInitParam::AITYPE_CISHA_CAMP1,
			MonsterInitParam::AITYPE_CISHA_CAMP2,
			MonsterInitParam::AITYPE_CISHA_CAMP3,
		};

		Monster *monster = (Monster *)obj;

		if (monster->GetAIType() == friend_monster_ai[(int)role->GetCamp()])
		{
			return true;
		}
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}



