#include "roleaibase.hpp"
#include "obj/character/roleshadow.hpp"
#include "serverlogic.h"
#include "config/logicconfigmanager.hpp"
#include "skill/roleskillcondition.hpp"
#include "globalconfig/globalconfig.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "gamecommon.h"

RoleAIBase::RoleAIBase(Character *me, int aitype)
	: AIBase(me), m_aitype(aitype), m_birth_pos(0, 0), m_enemy_objid(INVALID_OBJ_ID), m_current_runtime(0), m_follow_interval(0)
{
	m_follow_interval = GLOBALCONFIG->GetOtherConfig_Monster_Follow_Interval();
}

RoleAIBase::~RoleAIBase()
{

}

void RoleAIBase::ForceSetEnemy(ObjID obj_id)
{
	if (obj_id != m_me->GetId())
	{
		m_enemy_objid = obj_id;
	}
}

void RoleAIBase::Follow(unsigned long interval)
{
	if (!m_me->CanCharacterAction(false, true))
	{
		m_me->MoveStop();
		return;
	}

	Obj *obj = m_me->GetScene()->GetObj(m_enemy_objid);
	if (NULL == obj)
	{
		m_enemy_objid = INVALID_OBJ_ID;
		return;
	}

	if (m_me->IsStatic())
	{
		Posi deltaP = obj->GetPos() - m_me->GetPos();
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		m_me->SetDir(dir, distance);
		m_current_runtime = 0;
		return;
	}

	m_current_runtime += interval;

	if (m_current_runtime >= m_follow_interval)
	{
		Posi deltaP = obj->GetPos() - m_me->GetPos();
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		if (m_me->GetDir() != dir)
		{
			m_me->SetDir(dir, distance);
		}

		m_current_runtime = 0;
		return;
	}
}

bool RoleAIBase::CanStartAttack()
{
	if (!m_me->CanCharacterAction(false, false))
	{
		return false;
	}

	Obj *obj = m_me->GetScene()->GetObj(m_enemy_objid);
	if (NULL == obj || !Obj::IsCharactor(obj->GetObjType()))
	{
		m_enemy_objid = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	Character *target = (Character*)obj;
	if (!this->IsEnemy(target))
	{
		m_enemy_objid = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	if (!target->IsAlive())
	{
		m_enemy_objid = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	Posi target_pos = target->GetPos();
	return m_me->CheckNextSkillDistance(target, &target_pos);
}

void RoleAIBase::ForceStartAttack()
{
	if (!m_me->CanCharacterAction(false, false))
	{
		m_me->StopAttack();
		return;
	}

	m_me->StartAttack(m_enemy_objid);
	this->SetNextSkill();
}

void RoleAIBase::InitCondSkillList()
{
	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; i++)
	{
		const SkillManager::SkillItem *skill_item = m_me->GetSkillManager()->GetSkillItem(i);
		if (NULL == skill_item || skill_item->Invalid()) continue;

		int cond_type = LOGIC_CONFIG->GetRoleSkillCondCfg().GetSkillCondType(skill_item->skill_id);
		if (cond_type >= 0 && cond_type < ROLE_SHADOW_SKILL_COND_MAX)
		{
			RoleCondSkill &rolecond_skill = m_cond_skill_list[cond_type];
			if (rolecond_skill.count < SkillListParam::MAX_SKILL_NUM)
			{
				rolecond_skill.skill_index_list[rolecond_skill.count++] = i;
			}
		}
	}
}

void RoleAIBase::SetNextSkill()
{
	// ¼ì²éÈ«¾ÖCD
	if (!m_me->CheckGlobalCD()) return;
	
	for (int i = 0; i < ROLE_SHADOW_SKILL_COND_MAX; i++)
	{
		RoleCondSkill &rolecond_skill = m_cond_skill_list[i];
		if (this->PerformNextSkillFromList(rolecond_skill.count, rolecond_skill.skill_index_list))
		{
			return;
		}
	}
}

bool RoleAIBase::PerformNextSkillFromList(int count, unsigned short skill_index_list[SkillListParam::MAX_SKILL_NUM])
{
	int min_count = count >= SkillListParam::MAX_SKILL_NUM ? SkillListParam::MAX_SKILL_NUM : count;

	for (int i = min_count - 1; i >= 0; --i)
	{
		int skill_index = skill_index_list[i];
		if (m_me->GetSkillManager()->CheckSkillCoolDown(skill_index))
		{
			ObjID target_id = m_enemy_objid;

			Posi target_pos = m_me->GetPos();
			if (target_id != m_me->GetId())
			{
				Obj *target_obj = m_me->GetScene()->GetObj(target_id);
				if (NULL != target_obj) target_pos = target_obj->GetPos();
			}

			if (this->PerformSkill(skill_index, target_pos, target_id))
			{
				return true;
			}
		}
	}

	return false;
}

bool RoleAIBase::PerformSkill(int skill_index, const Posi &pos, ObjID target_id, int check_flag, int special_param, int skill_data)
{
	if (m_me->PerformSkill(skill_index, pos, target_id, check_flag, special_param, skill_data))
	{
		const SkillManager::SkillItem *skill_item = m_me->GetSkillManager()->GetSkillItem(skill_index);
		if (NULL != skill_item)
		{
			unsigned long end_time = static_cast<unsigned long>(ServerLogic::GetInstServerLogic()->GetPiTime() + LOGIC_CONFIG->GetRoleSkillCondCfg().GetGlobalCD(skill_item->skill_id));
			m_me->SetCooldownEndTime(end_time);
		}

		return true;
	}

	return false;
}

void RoleAIBase::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{

}

void RoleAIBase::Update(unsigned long interval, unsigned int now_second)
{

}

void RoleAIBase::Init(const ChallengeParam &mp)
{
	this->InitCondSkillList();
}

bool RoleAIBase::IsEnemy(Obj *obj)
{
	if (NULL != obj && obj->GetObjType() == Obj::OBJ_TYPE_ROLE && Obj::IsCharactor(obj->GetObjType()))
	{
		Role *cha = (Role*)obj;
		if (cha->IsAlive())
		{
			if (Role::MOVE_MODE_FLY == cha->GetMoveMode())
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

bool RoleAIBase::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
	{
		RoleShadow *roleshadow = (RoleShadow *)obj;
		return (!check_alive || roleshadow->IsAlive());
	}

	return false;
}

