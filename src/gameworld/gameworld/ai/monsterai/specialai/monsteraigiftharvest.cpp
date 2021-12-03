#include "monsteraigiftharvest.hpp"
#include "scene/scene.h"
#include "obj/character/role.h"

MonsterAIGiftHarvest::MonsterAIGiftHarvest(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_GIFT_HARVEST)
{

}

MonsterAIGiftHarvest::~MonsterAIGiftHarvest()
{

}

void MonsterAIGiftHarvest::OnRoleDie(ObjID obj_id)
{
	if (m_enemy == obj_id)
	{
		m_enemy = INVALID_OBJ_ID;
		m_me->StopAttack();
	}
}

void MonsterAIGiftHarvest::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	// MonsterAIBase::OnAttack(attacker, skill_id, delta_hp); 不走基类逻辑
	if (attacker == m_enemy) return;

	Role *fenshen_owner = m_me->GetScene()->GetPrincipal(attacker);
	if (NULL != fenshen_owner)
	{
		attacker = fenshen_owner->GetId();
	}

	++ m_attacker_times_map[attacker];

	if (m_attacker_times_map[attacker] >= GIFT_HARVEST_MONSTER_HATE_MAX_ATTACK_TIMES)
	{
		m_enemy = attacker;
		//this->StartAttack(m_enemy, true);
		m_attacker_times_map.clear();
	}
}

void MonsterAIGiftHarvest::AI(unsigned long interval)
{
	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		{
			if (INVALID_OBJ_ID != m_enemy)
			{
				if (this->IsOutRange())
				{
					m_enemy = INVALID_OBJ_ID;
					m_me->StopAttack();
				}
				else
				{
					this->Follow(m_enemy, interval);
				}
// 				if (this->CanStartAttack())
// 				{
// 					this->StartAttack(m_enemy, true);
// 				}
// 				else
// 				{
// 					if (this->IsOutRange())
// 					{
// 						m_enemy = INVALID_OBJ_ID;
// 						m_me->StopAttack();
// 					}
// 					else
// 					{
// 						this->Follow(m_enemy, interval);
// 					}
// 				}
			}
			else
			{
				m_me->StopAttack();
			}
		}
		break;
	}
}

bool MonsterAIGiftHarvest::IsOutRange()
{
	if (INVALID_OBJ_ID == m_enemy)
	{
		return true;
	}
	
	Obj *obj = m_me->GetScene()->GetObj(m_enemy);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return true;
	}

	bool outof_range = (obj->GetPos() - m_me->GetPos()).IsLongerThan(m_run_range);
	return outof_range;
}

