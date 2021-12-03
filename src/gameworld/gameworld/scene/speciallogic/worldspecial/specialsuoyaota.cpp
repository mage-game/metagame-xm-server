#include "specialsuoyaota.hpp"

#include "gamecommon.h"

#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "engineadapter.h"

#include "scene/scene.h"
#include "scene/scenemanager.h"

#include "config/marryconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/commondata.hpp"
#include "other/route/mailroute.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "item/itempool.h"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/suoyaotaconfig.hpp"

#include "world.h"
#include "gamelog.h"

#include "obj/otherobj/fallingitem.h"

#include "suoyaotamgr.hpp"

#include "global/worldstatus/worldstatus.hpp"

SpecialSuoYaoTa::SpecialSuoYaoTa(Scene *scene)
	: SpecialLogic(scene), m_is_finish(false), m_state(SUOYAOTA_STATUS_TYPE_INVALID), m_fb_lv(0)
{
	m_fb_info.Reset();
}

SpecialSuoYaoTa::~SpecialSuoYaoTa()
{

}

void SpecialSuoYaoTa::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
}

void SpecialSuoYaoTa::OnRoleEnterScene(Role *role)
{
	if (0 == m_fb_lv)
	{
		m_fb_lv = role->GetLevel(); 
		
		m_fb_info.Reset();
		m_fb_info.fb_lv = m_fb_lv;

		this->InitSYTTask();
	}

	this->AddUserScoreHelper(role->GetUserId(), 100);

	SuoYaoTaMgr::Instance().OnGetSYTState(role);

	this->NotifyFbInfoToRole(role, Protocol::SCSuoYaoTaFBInfo::SUOYAOTA_NOTIFY_ENTER_SCENE);
}

void SpecialSuoYaoTa::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		Role *kill_role = m_scene->GetPrincipal(killer_id);
		if (NULL != kill_role)
		{
			SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(m_fb_lv);
			if (NULL != logic_cfg)
			{
				if (monster->IsBossMonster())
				{
					this->AddUserScoreHelper(kill_role->GetUserId(), logic_cfg->kill_boss_score);

					this->FBFinish(true);
				}
				else
				{
					this->AddUserScoreHelper(kill_role->GetUserId(), logic_cfg->kill_monster_score);
				}
			}
		}

		this->OnTaskParamChange(SUOYAOTA_TASK_TYPE_KILL_MONSTER, monster->GetMonsterId());

		this->NotifyFbInfoToRole(NULL, Protocol::SCSuoYaoTaFBInfo::SUOYAOTA_NOTIFY_SCORE_CHANGE);
	}
}

bool SpecialSuoYaoTa::OnTimeout()
{
	this->FBFinish(false);
	
	this->DelayKickOutAllRole();

	return false;
}

void SpecialSuoYaoTa::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(m_fb_lv);
	if (NULL != logic_cfg)
	{
		this->AddUserScoreHelper(role->GetUserId(), logic_cfg->gathar_score);
		this->OnTaskParamChange(SUOYAOTA_TASK_TYPE_GATHAR, gather_id);

		this->NotifyFbInfoToRole(NULL, Protocol::SCSuoYaoTaFBInfo::SUOYAOTA_NOTIFY_SCORE_CHANGE);
	}
}

bool SpecialSuoYaoTa::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return monster->IsAlive();
		}
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialSuoYaoTa::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role*)obj;
			return (!check_alive || friend_role->IsAlive());
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			return false;
		}
		break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

void SpecialSuoYaoTa::OnActivityStart()
{
	m_state = SUOYAOTA_STATUS_TYPE_START;
	m_is_finish = false;

	this->FlushMonsterHelper();
}

bool SpecialSuoYaoTa::ActivityIsStop() 
{ 
	return m_state >= SUOYAOTA_STATUS_TYPE_STOP; 
}

void SpecialSuoYaoTa::NotifyFbInfoToRole(Role *role, int reason)
{
	m_fb_info.reason = reason;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&m_fb_info, sizeof(m_fb_info));
	}
	else
	{
		if (NULL != m_scene)
		{
			m_scene->SendToRole((const char *)&m_fb_info, sizeof(m_fb_info));
		}
	}
}

void SpecialSuoYaoTa::OnActivityStop()
{
	this->FBFinish(false);

	this->DelayKickOutAllRole();

	m_state = SUOYAOTA_STATUS_TYPE_STOP;
}

void SpecialSuoYaoTa::FlushMonsterHelper()
{
	if (m_fb_lv <= 0)   
	{
		return;
	}

	SceneMonsterLogic *monster_logic = m_scene->GetMonsterLogic();

	SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(m_fb_lv);
	if (NULL != logic_cfg)
	{
		for (int i = 0; i < logic_cfg->monster_pt_count; i++)
		{
			SuoYaoTaLogicCfg::MonsterPt *monter_pt = &logic_cfg->monster_list[i];
			
			Posi pos = gamecommon::GetDisRandPos(monter_pt->flush_pos, monter_pt->rand_range);
			monster_logic->AddMonsterGenerate(m_scene, 0, 1, monter_pt->flush_interval, pos, monter_pt->monster_id);
		}
	}
}

void SpecialSuoYaoTa::AddUserScoreHelper(const UserID &user_id, int score)
{
	if (m_is_finish)
	{
		return;
	}

	int i = 0;

	for (i = 0; i < m_fb_info.user_count && i < Protocol::SUOYAOTA_ROOM_USER_MAX; i++)
	{
		if (m_fb_info.user_list[i].uid == UidToInt(user_id))
		{
			m_fb_info.user_list[i].score += score;

			return;
		}
	}

	if (i >= 0 && i < Protocol::SUOYAOTA_ROOM_USER_MAX)
	{
		m_fb_info.user_list[i].uid = UidToInt(user_id);
		m_fb_info.user_list[i].score = score;

		++ m_fb_info.user_count;
	}
}

void SpecialSuoYaoTa::InitSYTTask()
{
	if (m_fb_lv <= 0)   
	{
		return;
	}

	UNSTD_STATIC_CHECK(Protocol::SUOYAOTA_TASK_MAX == SUOYAOTA_TASK_MAX);

	SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(m_fb_lv);
	if (NULL != logic_cfg)
	{
		for (int i = 0; i < Protocol::SUOYAOTA_TASK_MAX; i++)
		{
			m_fb_info.task_list[i].task_type = logic_cfg->task_list[i].task_type;
			m_fb_info.task_list[i].param_id = logic_cfg->task_list[i].param_id;
			m_fb_info.task_list[i].param_num = 0;
			m_fb_info.task_list[i].param_max = logic_cfg->task_list[i].param_max;
		}
	}
}

void SpecialSuoYaoTa::OnTaskParamChange(int task_type, int param0)
{
	int step_idx = 0;
	if (this->GetTaskCurrStepIdx(&step_idx) && step_idx >= 0 && step_idx < Protocol::SUOYAOTA_TASK_MAX)
	{
		if (m_fb_info.task_list[step_idx].task_type == task_type && m_fb_info.task_list[step_idx].param_id == param0)
		{
			++ m_fb_info.task_list[step_idx].param_num;
		}
	}
}

bool SpecialSuoYaoTa::GetTaskCurrStepIdx(int *step_idx)
{
	for (int i = 0; i < Protocol::SUOYAOTA_TASK_MAX; i++)
	{
		if (m_fb_info.task_list[i].param_num < m_fb_info.task_list[i].param_max)
		{
			if (NULL != step_idx) *step_idx = i;
			return true;
		}
	}

	return false;
}

void SpecialSuoYaoTa::FBFinish(bool is_pass)
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;

	for (int i = 0; i < m_fb_info.user_count; i++)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_fb_info.user_list[i].uid));
		if (NULL != role) 
		{
			CommonDataParam *role_common_p = role->GetCommonDataParam();
			role_common_p->suoyaota_score = m_fb_info.user_list[i].score;
			role_common_p->suoyaota_lv = m_fb_lv;

			if (role_common_p->suoyaota_score > role_common_p->suoyaota_max_score)
			{
				role_common_p->suoyaota_max_score = role_common_p->suoyaota_score;
			}

			WorldStatusCommonDataParam &world_common_p = WorldStatus::Instance().GetCommonData();
			if (role_common_p->suoyaota_score > world_common_p.syt_topuserinfo.quanfu_topscore)	// 我这次的积分比世界的更高
			{
				world_common_p.syt_topuserinfo.quanfu_topscore = role_common_p->suoyaota_score;
				world_common_p.syt_topuserinfo.quanfu_topscore_uid = role->GetUID();
				role->GetName(world_common_p.syt_topuserinfo.quanfu_topscore_name);
			}

			SuoYaoTaMgr::Instance().NotifySYTReward(role);
		}
		else 
		{
			SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(m_fb_lv);
			if (NULL != logic_cfg && logic_cfg->total_score > 0)
			{
				int score = m_fb_info.user_list[i].score;

				float score_factor = (score * 1.0f / logic_cfg->total_score);
				if (score_factor > 1.0f) score_factor = 1.0f;

				long long exp = static_cast<long long>(logic_cfg->reward_base_exp * 0.6f + logic_cfg->reward_base_exp * 0.4f * score_factor);
				int bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6f + logic_cfg->reward_base_coin * 0.4f * score_factor);

				static MailContentParam contentparam; contentparam.Reset();

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, exp);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, bind_coin);

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_suoyaota_reward_content);
				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(m_fb_info.user_list[i].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}

		gamelog::g_log_fb.printf(LL_INFO, " SpecialSuoYaoTa::FBFinish user[%d] fb_lv[%d], score[%d]", m_fb_info.user_list[i].uid, m_fb_lv, m_fb_info.user_list[i].score);
	}
}




