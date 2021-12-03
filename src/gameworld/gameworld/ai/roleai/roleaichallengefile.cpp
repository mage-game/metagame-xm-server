#include "roleaichallengefile.hpp"
#include "obj/character/roleshadow.hpp"
#include "gamecommon.h"

RoleAIChallengefile::RoleAIChallengefile(Character *me)
	: RoleAIBase(me, ROLE_SHADOW_TYPE_CHALLENGE_FIELD), m_interval(0), m_wailt_ms(0)
{

}

RoleAIChallengefile::~RoleAIChallengefile()
{

}

void RoleAIChallengefile::AI(unsigned long interval)
{
	if (m_wailt_ms > 0 && m_interval < m_wailt_ms)
	{
		m_interval += interval;
		return;
	}
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

