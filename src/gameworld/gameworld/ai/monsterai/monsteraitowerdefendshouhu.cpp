#include "monsteraitowerdefendshouhu.hpp"

#include "gamecommon.h"
#include "obj/character/monster.h"

#include "scene/scene.h"

MonsterAITowerDefendShouhu::MonsterAITowerDefendShouhu(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_SHOUHU)
{

}

MonsterAITowerDefendShouhu::~MonsterAITowerDefendShouhu()
{

}

void MonsterAITowerDefendShouhu::Init(const MonsterAIParam &mp)
{
	MonsterAIStaticInitiative::Init(mp);
}

void MonsterAITowerDefendShouhu::AI(unsigned long interval)
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

bool MonsterAITowerDefendShouhu::IsEnemy(Obj *obj)
{
	if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		if (obj == m_me)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool MonsterAITowerDefendShouhu::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		if (obj == m_me)
		{
			return true;
		}
		return false;
	}

	return true;
}

