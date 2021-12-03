#include "monsteraixianmengzhanguardboss.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/scene.h"

MonsterAIXianMengZhanGuardBoss::MonsterAIXianMengZhanGuardBoss(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_GUARDBOSS),
	m_guild_id(INVALID_GUILD_ID), m_defend_area(-1)
{

}

MonsterAIXianMengZhanGuardBoss::~MonsterAIXianMengZhanGuardBoss()
{

}

bool MonsterAIXianMengZhanGuardBoss::IsEnemy(Obj *obj)
{
	bool is_enemy = false;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		is_enemy = role->IsAlive();
	}

	return is_enemy;
}

bool MonsterAIXianMengZhanGuardBoss::IsFriend(Obj *obj, bool check_alive)
{
	bool is_friend = false;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		is_friend = !check_alive || monster->IsAlive();
	}

	return is_friend;
}
