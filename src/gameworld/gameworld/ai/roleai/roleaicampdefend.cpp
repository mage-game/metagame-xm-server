#include "roleaicampdefend.hpp"
#include "obj/character/roleshadow.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "scene/scene.h"
#include "gamecommon.h"

RoleAICampDefend::RoleAICampDefend(Character *me)
	: RoleAIBase(me, ROLE_SHADOW_TYPE_CAMPDEFEND)
{

}

RoleAICampDefend::~RoleAICampDefend()
{

}

void RoleAICampDefend::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	Obj *obj = m_me->GetScene()->GetObj(attacker);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		CampDefendMgr::Instance().OnDiaoXiangAttack(role, -delta_hp);
	}
}

void RoleAICampDefend::AI(unsigned long interval)
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


