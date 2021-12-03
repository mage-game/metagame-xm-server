#include "specialmillionaire.hpp"
#include "obj/character/role.h"
#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "protocal/msgactivity.hpp"
#include "config/activityconfig/millionaireconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "other/activedegree/activedegree.hpp"

SpecialMillionaire::SpecialMillionaire(Scene *scene):SpecialLogic(scene), m_is_finish(false)
{
}

SpecialMillionaire::~SpecialMillionaire()
{
}

void SpecialMillionaire::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_STANDY:
		{
			this->OnActivityStandby();
		}
		break;
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

bool SpecialMillionaire::CanDestroy()
{
	return m_is_finish;
}

void SpecialMillionaire::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
	if (NULL != other_cfg && gather_id == other_cfg->exp_gather_id && other_cfg->exp_reward > 0)
	{
		long long add_exp = static_cast<long long>((role->GetLevel() + 50) * other_cfg->exp_reward);
		role->AddExp(add_exp, "MillionairExpGather", Role::EXP_ADD_REASON_DEFAULT);
	}

	const GatherCfg *gather_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetGatherCfg(gather_id);
	if (NULL != gather_cfg && gather_cfg->is_notice > 0)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_gather_good,
			role->GetUID(), role->GetName(), (int)role->GetCamp(), gather_id);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void SpecialMillionaire::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster * monster = (Monster *)dead;
		const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
		if (NULL != other_cfg && monster->GetMonsterId() == other_cfg->exp_boss_id && 0 == ActivityShadow::Instance().GetMillionairBossNextFreshTime())
		{
			unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			ActivityShadow::Instance().SetMillionairBossNextFreshTime(now_sec + other_cfg->boss_refresh_interval);
			LOGIC_CONFIG->GetMillionaireConfig().RefreshExpGather();
			ActivityShadow::Instance().SendMillionaireBoxNoticeNextTime(NULL);

			Role *killer = m_scene->GetPrincipal(killer_id);
			if (NULL != killer)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_kill_boss,
					monster->GetMonsterId(), killer->GetUID(), killer->GetName(), (int)killer->GetCamp());
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void SpecialMillionaire::OnRoleEnterScene(Role *role)
{
	if (m_is_finish || NULL == role || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		this->DelayKickOutRole(role);
		return;
	}

	role->SendMillionaireInfo();
	ActivityShadow::Instance().SendMillionaireBoxNoticeNextTime(role);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_DAFUHAO);
}

void SpecialMillionaire::OnRoleLeaveScene(Role *role, bool is_logout)
{

}

void SpecialMillionaire::OnActivityStandby()
{

}

void SpecialMillionaire::OnUseFrozenSkill(Role *role, ObjID target_obj_id)
{
	if (NULL == role) return;

	const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
	if (NULL == other_cfg) return;

	Obj *target_obj = m_scene->GetObj(target_obj_id);
	if (NULL == target_obj || Obj::OBJ_TYPE_ROLE != target_obj->GetObjType()) return;

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data) return;

	if (common_data->millionaire_use_skill_times >= other_cfg->frozen_can_use_times)
	{
		role->NoticeNum(errornum::EN_MILLIONAIRE_SKILL_TIME_LIMIT);
		return;
	}

	unsigned int now_sce = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sce < common_data->millionaire_valid_perform_skill_time)
	{
		role->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
		return;
	}

	if ((target_obj->GetPos() - role->GetPos()).IsLongerThan(other_cfg->frozen_distance))
	{
		role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
	}

	Role *target_role = (Role *)target_obj;
	EffectSpecialState *effect_frozen = new EffectSpecialState(role->GetId(), 1, other_cfg->frozen_continue_time * 1000, EBT_FROZEN, 1);
	effect_frozen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	target_role->AddEffect(effect_frozen);

	long long add_exp = static_cast<long long> ((role->GetLevel() + 50) * other_cfg->frozen_exp_reward);
	if (add_exp > 0)
	{
		role->AddExp(add_exp, "MillionaireUseSkill", Role::EXP_ADD_REASON_DEFAULT);
	}

	role->GetKnapsack()->PutListOrMail(other_cfg->frozen_reward_item, PUT_REASON_MIllIONAIRE);
	common_data->millionaire_valid_perform_skill_time = now_sce + other_cfg->frozen_cloddown_time;
	common_data->millionaire_use_skill_times++;

	role->SendMillionaireInfo();

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_frozen,
		role->GetUID(), role->GetName(), (int)role->GetCamp(), target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp(),
		target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp());
	if (length > 0)
	{
		this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
	}
}

void SpecialMillionaire::OnActivityStart()
{
	m_is_finish = false;
}

void SpecialMillionaire::OnActivityClose()
{
	if (m_is_finish) return;

	m_is_finish = true;

	this->DelayKickOutAllRole();
}

void SpecialMillionaire::SyncGlobalInfoToAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->SyncGlobalInfo(temp_role);
	}
}

void SpecialMillionaire::SyncGlobalInfo(Role *role)
{
	if (NULL == role) return;
}


void SpecialMillionaire::SyncRoleInfo(Role *role)
{
	if (NULL == role) return;
}

void SpecialMillionaire::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}
