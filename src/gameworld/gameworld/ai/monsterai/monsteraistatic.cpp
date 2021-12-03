#include "monsteraistatic.hpp"

#include "gamecommon.h"
#include "obj/character/monster.h"

#include "scene/scene.h"

MonsterAIStatic::MonsterAIStatic(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_STATIC)
{

}

MonsterAIStatic::~MonsterAIStatic()
{

}

void MonsterAIStatic::AI(unsigned long interval)
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
		if (m_enemy == INVALID_OBJ_ID || IsOutRange())
		{
			m_enemy = INVALID_OBJ_ID;
			GoBack();
		}
		else
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else
			{
				Follow(m_enemy, interval);
			}
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		SetNextSkill(m_enemy);
		break;
	case Character::ATTACK_OUTOFRANG:
		Follow(m_enemy,interval);
		break;
	}
}

void MonsterAIStatic::GoBack()
{
	if (m_birth_pos != m_me->GetPos())
	{
		Posi deltaP = m_birth_pos - m_me->GetPos();

		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

		m_me->SetDir(dir, distance);
	}
}

bool MonsterAIStatic::IsAttackWhenOnAttack(ObjID attacker)
{
	return m_birth_pos == m_me->GetPos() && (INVALID_OBJ_ID == m_enemy);
}

