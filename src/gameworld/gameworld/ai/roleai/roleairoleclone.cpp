#include "roleairoleclone.hpp"
#include "obj/character/roleshadow.hpp"
#include "gamecommon.h"

RoleAIRoleClone::RoleAIRoleClone(Character *me)
	: RoleAIBase(me, ROLE_SHADOW_TYPE_CLONE_ROLE)
{

}

RoleAIRoleClone::~RoleAIRoleClone()
{

}

void RoleAIRoleClone::AI(unsigned long interval)
{
	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		if (INVALID_OBJ_ID != m_enemy_objid)
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

bool RoleAIRoleClone::IsEnemy(Obj *obj)
{
	if (!obj)
	{
		return false;
	}
	if (obj->GetId() == m_me->GetId())
	{
		return false;
	}
	return true;
}


