#include "specialyaoshouguangchang.hpp"

#include "gamecommon.h"

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
#include "config/activityconfig/yaoshouguangchangconfig.hpp"
#include "monster/monsterpool.h"

#include "world.h"
#include "gamelog.h"
#include "global/worldstatus//worldstatus.hpp"

#include "obj/otherobj/fallingitem.h"

#include "obj/character/monster.h"

#include "yaoshouguangchangmgr.hpp"
 
SpecialYaoShouGuangChang::SpecialYaoShouGuangChang(Scene *scene)
	: SpecialLogic(scene), m_is_finish(false), m_state(YAOSHOUGUANGCHANG_STATUS_TYPE_INVALID), m_wave_index(0), m_fb_lv(0)
{
	m_fb_info.Reset();
}

SpecialYaoShouGuangChang::~SpecialYaoShouGuangChang()
{

}

void SpecialYaoShouGuangChang::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (YAOSHOUGUANGCHANG_STATUS_TYPE_START == m_state)
	{
		this->CheckSceneMonsterStatus(interval, now_second);
	}
}

void SpecialYaoShouGuangChang::OnRoleEnterScene(Role *role)
{
	if (0 == m_fb_lv)
	{
		m_fb_lv = role->GetLevel(); 

		m_fb_info.Reset();
		m_fb_info.fb_lv = m_fb_lv;
	}

	this->AddUserScoreHelper(role->GetUserId(), 100);

	YaoShouGuangChangMgr::Instance().OnGetYSGCState(role);

	this->NotifyFbInfoToRole(NULL, Protocol::SCYaoShouGuangChangFBInfo::YAOSHOUGUANGCHANG_NOTIFY_ENTER_SCENE);
}

void SpecialYaoShouGuangChang::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		Role *kill_role = m_scene->GetPrincipal(killer_id);
		if (NULL != kill_role)
		{
			YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(m_fb_lv);
			if (NULL != logic_cfg)
			{
				if (monster->IsBossMonster())
				{
					this->AddUserScoreHelper(kill_role->GetUserId(), logic_cfg->kill_boss_score);
				}
				else
				{
					this->AddUserScoreHelper(kill_role->GetUserId(), logic_cfg->kill_monster_score);
				}
			}
		}

		this->NotifyFbInfoToRole(NULL, Protocol::SCYaoShouGuangChangFBInfo::YAOSHOUGUANGCHANG_NOTIFY_SCORE_CHANGE);
	}
}

void SpecialYaoShouGuangChang::OnRemoveObj(Obj *obj)
{
	this->NotifyFbInfoToRole(NULL, Protocol::SCYaoShouGuangChangFBInfo::YAOSHOUGUANGCHANG_NOTIFY_INVALID);
}

bool SpecialYaoShouGuangChang::OnTimeout()
{
	this->FBFinish(false);
	
	this->DelayKickOutAllRole();

	return false;
}


bool SpecialYaoShouGuangChang::SpecialIsEnemy(Role *role, Obj *obj)
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

bool SpecialYaoShouGuangChang::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
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

void SpecialYaoShouGuangChang::OnActivityStart()
{
	m_state = YAOSHOUGUANGCHANG_STATUS_TYPE_START;
	m_is_finish = false;
	m_wave_index = 0;

	this->FlushMonsterHelper();
}

void SpecialYaoShouGuangChang::OnActivityStop()
{
	this->FBFinish(false);

	this->DelayKickOutAllRole();

	m_state = YAOSHOUGUANGCHANG_STATUS_TYPE_STOP;
}

bool SpecialYaoShouGuangChang::ActivityIsStop() 
{ 
	return m_state >= YAOSHOUGUANGCHANG_STATUS_TYPE_STOP; 
}

void SpecialYaoShouGuangChang::NotifyFbInfoToRole(Role *role, int reason)
{
	m_fb_info.reason = reason;
	m_fb_info.fb_lv = m_fb_lv;
	m_fb_info.wave_index = m_wave_index;
	m_fb_info.role_num = m_scene->RoleNum();
	m_fb_info.monster_num = m_scene->MonsterNum();

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

void SpecialYaoShouGuangChang::OnAllMonsterDead()
{
	if (m_is_finish)
	{
		return;
	}

	YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(m_fb_lv);
	if (NULL != logic_cfg)
	{
		if (m_wave_index >= logic_cfg->wave_count)
		{
			this->FBFinish(true);
		}
		else 
		{
			this->FlushMonsterHelper();
		}
	}

	this->NotifyFbInfoToRole(NULL, Protocol::SCYaoShouGuangChangFBInfo::YAOSHOUGUANGCHANG_NOTIFY_WAVE_FINISH);
}

void SpecialYaoShouGuangChang::FlushMonsterHelper()
{
	if (m_fb_lv <= 0)   
	{
		return;
	}

	YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(m_fb_lv);
	if (NULL != logic_cfg)
	{
		if (m_wave_index >= 0 && m_wave_index < logic_cfg->wave_count)
		{
			YaoShouGuangChangLogicCfg::WaveCfg *wave_cfg = &logic_cfg->wave_list[m_wave_index];
			
			for (int i = 0; i < wave_cfg->pt_count; i++)
			{
				YaoShouGuangChangLogicCfg::WavePointCfg *pt_cfg = &wave_cfg->pt_list[i];

				for (int j = 0; j < pt_cfg->monster_num; j++)
				{
					Posi rand_pos = gamecommon::GetDisRandPos(pt_cfg->flush_pos, pt_cfg->flush_range);
					if (!m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, rand_pos.x, rand_pos.y))
					{
						rand_pos = pt_cfg->flush_pos;
					}

					MONSTERPOOL->CreateMonster(pt_cfg->monster_id, m_scene, rand_pos);
				}
			}

			++ m_wave_index;
		}
	}

	this->NotifyFbInfoToRole(NULL, Protocol::SCYaoShouGuangChangFBInfo::YAOSHOUGUANGCHANG_NOTIFY_INVALID);
}

void SpecialYaoShouGuangChang::AddUserScoreHelper(const UserID &user_id, int score)
{
	if (m_is_finish)
	{
		return;
	}

	int i = 0;

	for (i = 0; i < m_fb_info.user_count && i < Protocol::YAOSHOUGUANGCHANG_ROOM_USER_MAX; i++)
	{
		if (m_fb_info.user_list[i].uid == UidToInt(user_id))
		{
			m_fb_info.user_list[i].score += score;

			return;
		}
	}

	if (i >= 0 && i < Protocol::YAOSHOUGUANGCHANG_ROOM_USER_MAX)
	{
		m_fb_info.user_list[i].uid = UidToInt(user_id);
		m_fb_info.user_list[i].score = score;

		++ m_fb_info.user_count;
	}
}

void SpecialYaoShouGuangChang::FBFinish(bool is_pass)
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
			role_common_p->yaoshouguangchang_score = m_fb_info.user_list[i].score;
			role_common_p->yaoshouguangchang_lv = m_fb_lv;

			if (role_common_p->yaoshouguangchang_score > role_common_p->yaoshouguangchang_max_score)
			{
				role_common_p->yaoshouguangchang_max_score = role_common_p->yaoshouguangchang_score;
			}

			WorldStatusCommonDataParam &world_common_p = WorldStatus::Instance().GetCommonData();
			if (role_common_p->yaoshouguangchang_score > world_common_p.ysgc_topuserinfo.quanfu_topscore)  // 我这次的积分比世界的更高
			{
				world_common_p.ysgc_topuserinfo.quanfu_topscore = role_common_p->yaoshouguangchang_score;
				world_common_p.ysgc_topuserinfo.quanfu_topscore_uid = role->GetUID();
				role->GetName(world_common_p.ysgc_topuserinfo.quanfu_topscore_name);
			}

			YaoShouGuangChangMgr::Instance().NotifyYSGCReward(role);
		}
		else 
		{
			YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(m_fb_lv);
			if (NULL != logic_cfg && logic_cfg->total_score > 0)
			{
				int score = m_fb_info.user_list[i].score;

				double score_factor = (score * 1.0 / logic_cfg->total_score);
				if (score_factor > 1.0) score_factor = 1.0;

				long long exp = static_cast<long long>(logic_cfg->reward_base_exp * 0.6 + logic_cfg->reward_base_exp * 0.4 * score_factor);
				int bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6 + logic_cfg->reward_base_coin * 0.4 * score_factor);

				static MailContentParam contentparam; contentparam.Reset();

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_EXP, exp);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_COIN, bind_coin);

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_yaoshouguangchang_reward_content);
				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(m_fb_info.user_list[i].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}

		gamelog::g_log_fb.printf(LL_INFO, " SpecialYaoShouGuangChang::FBFinish user[%d] fb_lv[%d], score[%d]", m_fb_info.user_list[i].uid, m_fb_lv, m_fb_info.user_list[i].score);
	}
}















