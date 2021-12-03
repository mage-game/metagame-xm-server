#include "roleairoleboss.hpp"
#include "obj/character/roleshadow.hpp"
#include "gamecommon.h"

RoleAIRoleBoss::RoleAIRoleBoss(Character *me)
	: RoleAIBase(me, ROLE_SHADOW_TYPE_ROLE_BOSS)
{

}

RoleAIRoleBoss::~RoleAIRoleBoss()
{

}

void RoleAIRoleBoss::AI(unsigned long interval)
{
	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		if (INVALID_OBJ_ID == m_enemy_objid || this->IsOutRange())
		{
			m_enemy_objid = INVALID_OBJ_ID;
			this->GoBack();
		}
		else
		{
			if (this->CanStartAttack())
			{
				this->ForceStartAttack();
			}
			else
			{
				this->Follow(interval);
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
			this->SetNextSkill();
		}
		break;

	case Character::ATTACK_OUTOFRANG:
		this->Follow(interval);
		break;
	}
}

bool RoleAIRoleBoss::IsEnemy(Obj *obj)
{
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return true;
	}

	return false;
}

void RoleAIRoleBoss::GoBack()
{
	if (m_birth_pos != m_me->GetPos())
	{
		Posi deltaP = m_birth_pos - m_me->GetPos();
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		m_me->SetDir(dir, distance);
	}
}

bool RoleAIRoleBoss::IsOutRange()
{
	bool need_goback = false;
	if ((m_me->GetPos() - m_birth_pos).IsLongerThan(50))
	{
		need_goback = true;
	}
	
	return need_goback;
}

