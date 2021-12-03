#include "obj/character/monster.h"
#include "monsteraipassive.hpp"

MonsterAIPassive::MonsterAIPassive(Character *me)
	: MonsterAIMoveAround(me, MonsterInitParam::AITYPE_PASSIVE)
{

}

MonsterAIPassive::~MonsterAIPassive()
{

}

void MonsterAIPassive::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::ATTACK_NONE:
	case Character::MOVE_RUN:
		if ((m_enemy == INVALID_OBJ_ID && !CheckHateDrive()) || IsOutRange())
		{
			if (m_enemy != INVALID_OBJ_ID)
			{
				m_rest_time = 0;		// 让其立刻回去 不然主动怪就会在该处休息 则可以在这段时间中不断攻击它
				m_enemy = INVALID_OBJ_ID;
				m_me->MoveStop();
				OnEnemyLost();
			}
			MoveAround(interval);
		}
		else
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else
			{
				Follow(m_enemy,interval);
			}
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (this->CanStartAttack())
		{
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		Follow(m_enemy, interval);
		break;
	}
}

bool MonsterAIPassive::FindEnemy()
{
	if (this->CheckHateDrive()) return true;

	this->OnEnemyLost();
	
	return false;
}

bool MonsterAIPassive::IsAttackWhenOnAttack(ObjID attacker)
{
	return m_birth_pos == m_me->GetPos() && (INVALID_OBJ_ID == m_enemy);
}


