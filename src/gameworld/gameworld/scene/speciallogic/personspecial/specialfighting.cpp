#include "specialfighting.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "obj/character/roleshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/fighting/rolefighting.hpp"
#include "global/globalrole/fighting/fightingconfig.hpp"
#include "protocal/msgfighting.h"
#include "other/multimount/multimount.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"

SpecialFighting::SpecialFighting(Scene *scene) : SpecialLogic(scene), m_can_destroy(false), m_challenger_uid(0), m_challenger_obj_id(INVALID_OBJ_ID), m_opponent_obj_id(INVALID_OBJ_ID), m_start_fighting_time(0), m_can_fight(false)
{
}

SpecialFighting::~SpecialFighting()
{
	
}

void SpecialFighting::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_can_fight && now_second > m_start_fighting_time)
	{
		Obj * obj = m_scene->GetObj(m_opponent_obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
		{
			RoleShadow *role_shadow = (RoleShadow*)obj;
			role_shadow->ForceSetEnemy(m_challenger_obj_id);
		}

		m_can_fight = true;
	}
}

void SpecialFighting::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;

	RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
	if (NULL == fighting)
	{
		this->DelayKickOutAllRole();
		return;
	}

	m_challenger_uid = role->GetUID();
	m_challenger_obj_id = role->GetId();
	m_role_old_nuqi = role->GetNuqi();

	role->GetMountManager()->GoonMount(false);
	role->GetMultiMount()->MountOn(false);
	role->GetFightMountManager()->GoonFightMount(false, false);

	// 设置角色信息
	{
		role->GetSkillManager()->ClearAllCD();
		role->AddNuqi(-GLOBALCONFIG->GetMaxNuqi());
		//role->GetEffectList()->ClearAllDeBuffEffect();
		role->GetEffectList()->ClearFightEffect(EffectBase::CLEAR_REASON_ENTER_SCENE);
		role->Recover();
	}

	// 创建RoleShadow
	{
		RoleShadow* role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_CHALLENGE_FIELD);
		ChallengeParam param;
		RoleAppearance role_appearance;
		fighting->ConstructOpponentParam(param, role_appearance);

		role_shadow->Init(param, role_appearance, 0, 0);
		role_shadow->SetPos(fighting->GetCurOpponentPos());
		m_scene->AddObj(role_shadow);

		m_opponent_obj_id = role_shadow->GetId();
	}

	m_start_fighting_time = (unsigned int)(EngineAdapter::Instance().Time() + READY_TIME_S);

	static Protocol::SCFightingCountDownNotify fcdn;
	fcdn.start_fighting_time = m_start_fighting_time;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fcdn, sizeof(fcdn));
}

void SpecialFighting::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != m_scene->GetObj(m_opponent_obj_id)) // 对手没有打死就退出场景，当作失败
	{
		RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
		if (NULL != fighting)
		{
			fighting->OnFightingResult(false);
		}
	}

	int role_new_nuqi = role->GetNuqi();
	int delta_nuqi = m_role_old_nuqi - role_new_nuqi;
	// 还原怒气，加满气血
	{
		role->AddNuqi(delta_nuqi);

		if (!role->IsAlive())
		{
			role->ReAlive(100, 100, role->GetPos(), true);
		}
		else
		{
			role->Recover();
		}
	}

	m_can_destroy = true;
}

bool SpecialFighting::OnTimeout()
{
	if (NULL == m_scene->GetObj(m_opponent_obj_id))
	{
		return false;
	}

	Role* role = m_scene->GetPrincipal(m_challenger_obj_id);
	if (NULL == role)
	{
		return false;
	}

	RoleFighting* fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
	if (NULL != fighting)
	{
		fighting->OnFightingResult(false);
		m_can_destroy = true;
	}

	return false;
}

void SpecialFighting::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	bool challenger_win = false;
	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType() && m_opponent_obj_id == killer_id)
	{
		challenger_win = false;
	}
	else if(Obj::OBJ_TYPE_ROLE_SHADOW == dead->GetObjType() && m_challenger_obj_id == killer_id)
	{
		challenger_win = true;
	}

	RoleFighting* fighting = GlobalRoleManager::Instance().GetRoleFighting(m_challenger_uid);
	if (NULL != fighting)
	{
		fighting->OnFightingResult(challenger_win);
		m_can_destroy = true;
	}

	//this->DelayKickOutAllRole(); 由客户端主动退出场景（显示结果界面）
}

bool SpecialFighting::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!m_can_fight || role->GetId() == obj->GetId()) return false;

	if (Obj::IsCharactor(obj->GetObjType()))
	{
		Character *chara = (Character*)obj;
		return chara->IsAlive();
	}

	return false;
}