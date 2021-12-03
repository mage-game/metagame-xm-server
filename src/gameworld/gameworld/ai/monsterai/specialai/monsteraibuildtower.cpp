#include "monsteraibuildtower.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

MonsterAIBuildTower::MonsterAIBuildTower(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_BUILD_TOWER_TOWER)
{

}

MonsterAIBuildTower::~MonsterAIBuildTower()
{

}

void MonsterAIBuildTower::Init(const MonsterAIParam &mp)
{
	MonsterAIStaticInitiative::Init(mp);
}

void MonsterAIBuildTower::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
	case Character::ATTACK_NONE:
		if (m_enemy != INVALID_OBJ_ID || FindEnemy(interval))
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else 
			{
				m_me->StopAttack();
			}
		}
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (CanStartAttack())				//	战场将帅 除默认技外只有一个群技 且CD=0 会不断SetNextSkill施放 即使enemy已死
		{
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		{
			m_me->StopAttack();
			m_enemy = INVALID_OBJ_ID;
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	default:
		break;
	}
}

bool MonsterAIBuildTower::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return MonsterInitParam::AITYPE_BUILD_TOWER_TOWER != monster->GetAIType() && monster->IsAlive();
	}

	return false;
}

bool MonsterAIBuildTower::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		return role->IsAlive();
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;
		return MonsterInitParam::AITYPE_BUILD_TOWER_TOWER == monster->GetAIType();
	}

	return false;
}


