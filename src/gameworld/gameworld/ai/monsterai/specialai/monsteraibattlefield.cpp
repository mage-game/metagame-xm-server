#include "monsteraibattlefield.hpp"

#include "obj/character/role.h"
#include "other/roleactivity/roleactivity.hpp"
#include "obj/character/roleshadow.hpp"
#include "ai/roleai/roleaielementfield.hpp"

MonsterAIBattleField::MonsterAIBattleField(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_SPECIALAI_BATTLEFIELD), m_battle_field_side(-1), 
	m_lastenemy_objid(0), m_gongji_total_delta(0), m_gongji_delta_percent(0)
{

}

MonsterAIBattleField::~MonsterAIBattleField()
{

}

void MonsterAIBattleField::Init(const MonsterAIParam &mp)
{
	MonsterAIStaticInitiative::Init(mp);
}

void MonsterAIBattleField::AI(unsigned long interval)
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
				if (m_lastenemy_objid != m_enemy)
				{
					m_lastenemy_objid = m_enemy;
					m_me->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, -m_gongji_total_delta);
					m_gongji_total_delta = 0;
				}

				StartAttack(m_enemy, true);
			}
			else 
			{
				m_me->StopAttack();
				m_enemy = INVALID_OBJ_ID;
				m_lastenemy_objid = INVALID_OBJ_ID;
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

			m_me->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, -m_gongji_total_delta);
			m_gongji_total_delta = 0;
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	default:
		break;
	}
}

void MonsterAIBattleField::AddGongji()
{
	Attribute delta_gongji = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * m_gongji_delta_percent / 100;
	m_me->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji);
	m_gongji_total_delta += delta_gongji;
}

bool MonsterAIBattleField::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		return role->GetRoleActivity()->GetTempZhanchangSide() != m_battle_field_side && role->IsAlive();
	}
	else if (Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
	{
		RoleShadow *role_shadow = (RoleShadow *)obj;
		RoleAIBase *role_shadow_ai = role_shadow->GetRoleAI();
		if (NULL != role_shadow_ai && ROLE_SHADOW_TYPE_ELEMENT_FILED == role_shadow_ai->GetAIType())
		{
			RoleAIElementField *filed_ai = (RoleAIElementField*)role_shadow_ai;
			return role_shadow->IsAlive() && filed_ai->GetSide() != m_battle_field_side;
		}
	}

	return false;
}

bool MonsterAIBattleField::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		return (!check_alive || role->IsAlive()) && role->GetRoleActivity()->GetTempZhanchangSide() == m_battle_field_side && role->IsAlive();
	}
	else if(Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
	{
		RoleShadow *role_shadow = (RoleShadow *)obj;
		RoleAIBase *role_shadow_ai = role_shadow->GetRoleAI();
		if (NULL != role_shadow_ai && ROLE_SHADOW_TYPE_ELEMENT_FILED == role_shadow_ai->GetAIType())
		{
			RoleAIElementField *filed_ai = (RoleAIElementField*)role_shadow_ai;
			return (!check_alive || role_shadow->IsAlive()) && filed_ai->GetSide() == m_battle_field_side;
		}
	}

	return false;
}
