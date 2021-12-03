#include "specialexpfb.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgfight.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgfb.h"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "gameworld/other/fb/dailyfbconfig.hpp"
#include <vector>
#include "global/team/team.hpp"
#include "world.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "global/usercache/usercache.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "serverlogic.h"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "other/fb/roledailyfb.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "globalconfig/otherconfig.h"

SpecialExpFB::SpecialExpFB(Scene *scene)
	: SpecialLogic(scene), m_is_finish(false), m_is_pass(false), 
	m_timeout_time(0), m_start_time(0), m_total_allmonster_num(0), m_kill_allmonster_num(0), m_wave(1), m_wave_refresh_count(0), 
	m_average_level(0), m_member_num(0), m_next_refresh_monster_time(0), m_is_first_enter_enter(0), 
	m_add_monster_maxhp_per(0), m_add_monster_gongji_per(0), m_add_monster_fangyu_per(0), m_pass_kick_out_time(0)
{
}

SpecialExpFB::~SpecialExpFB()
{
	for (std::map<int, MemberItem *>::iterator it = m_member_map.begin(); it != m_member_map.end(); it++)
	{
		delete it->second;
	}

	m_member_map.clear();
}

void SpecialExpFB::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}
	if (EngineAdapter::Instance().Time() >= m_timeout_time)
	{
		m_is_pass = true;
	}
	this->CreateWaveMonsters();
};

void SpecialExpFB::CreateWaveMonsters()
{
	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
	if (m_total_allmonster_num - m_kill_allmonster_num > exp_other_cfg.start_refresh_monster_count)
	{
		return;
	}

	if (1 == m_wave && m_wave_refresh_count >= exp_other_cfg.fir_wave_refresh_count)
	{
		++m_wave;
		m_wave_refresh_count = 0;
	}
	else if (2 == m_wave && m_wave_refresh_count >= exp_other_cfg.sec_wave_refresh_count)
	{
		++m_wave;
		m_wave_refresh_count = 0;
	}
	else if (m_wave >= exp_other_cfg.total_wave && m_wave_refresh_count >= exp_other_cfg.other_wave_refresh_count)
	{
		return;
	}
	
	// 延迟5秒刷怪 +3秒的场景进度条时间
	if (EngineAdapter::Instance().Time() - m_start_time <= 8)
	{
		return;
	}

	// 刷怪cd
	if (ServerLogic::GetInstServerLogic()->GetPiTime() < m_next_refresh_monster_time && m_next_refresh_monster_time != 0)
	{
		return;
	}
	
	if (0 == m_timeout_time)
	{
		int total_level = 0;
		
		Role * role = NULL;
		for (std::map<int, MemberItem *>::iterator it = m_member_map.begin(); it != m_member_map.end(); it++)
		{
			role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it->first));
			if (NULL != role)
			{
				total_level += role->GetLevel();
			}
		}

		if (m_member_map.size() <= 0) return;

		m_average_level = static_cast<int>(total_level / static_cast<int>(m_member_map.size()));

		if (NULL != role && 1 == m_member_map.size() && 1 == role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes())
		{
			m_timeout_time = EngineAdapter::Instance().Time() + exp_other_cfg.first_time_limit;
			m_average_level = 0;
			m_is_first_enter_enter = 1;
		}
		else
		{
			m_timeout_time = EngineAdapter::Instance().Time() + exp_other_cfg.time_limit;
		}

		this->SyncFBSceneLogicInfoAll();
	}

	const ExpFbWaveItem *wave_item = LOGIC_CONFIG->GetDailyFbCfg().GetExpFbWaveItemCfg(m_average_level);
	if (NULL == wave_item)
	{
		return;
	}
	
	std::vector<ExpFbWaveMonsterPos> &pos_vec = LOGIC_CONFIG->GetDailyFbCfg().GetExpMonsterPos();

	int pos_count = static_cast<int>(pos_vec.size());
	if (pos_count <= 0)
	{
		return;
	}
	
	{
		int one_bo_time = exp_other_cfg.one_bo_time;
		int two_bo_time = exp_other_cfg.two_bo_time; 

		// 第一次进入经验副本波数时间特殊处理
		if (1 == m_is_first_enter_enter && one_bo_time > 50 && two_bo_time > 50)
		{
			one_bo_time -= 50;
			two_bo_time -= 50;
		}

		for (int i = 0; i < exp_other_cfg.monster_count; ++ i)
		{
			time_t start_long_time = EngineAdapter::Instance().Time() - m_start_time;
			int monster_id = 0;

			if (start_long_time > one_bo_time && start_long_time <= (one_bo_time + two_bo_time) && 1 == m_wave )
			{
				m_wave++;
			}
			else if(start_long_time > (one_bo_time + two_bo_time) && 2 == m_wave)
			{
				m_wave++;
			}

			if (1 == m_wave)
			{
				monster_id = wave_item->monster_id;
			}
			else if (2 == m_wave)
			{
				monster_id = wave_item->monster_id2;
			}
			else
			{
				monster_id = wave_item->monster_id3;
			}

			Attribute add_maxhp = 0, add_gongji = 0, add_fangyu = 0;
			{
				const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(monster_id);
				if (nullptr != monster_param)
				{
					if (m_add_monster_maxhp_per > 0)
					{
						add_maxhp = static_cast<Attribute>(m_add_monster_maxhp_per * ROLE_ATTR_PER * monster_param->hp);
					}
					if (m_add_monster_gongji_per > 0)
					{
						add_gongji = static_cast<Attribute>(m_add_monster_gongji_per * ROLE_ATTR_PER * monster_param->gongji);
					}
					if (m_add_monster_fangyu_per > 0)
					{
						add_fangyu = static_cast<Attribute>(m_add_monster_fangyu_per * ROLE_ATTR_PER * monster_param->fangyu);
					}
				}
			}

			MONSTERPOOL->CreateMonster(monster_id, m_scene, pos_vec[i % pos_count].pos, add_maxhp, add_gongji, add_fangyu);

			m_total_allmonster_num++;
		}
	}

	++m_wave_refresh_count;

	this->SyncFBSceneLogicInfoAll();
	m_next_refresh_monster_time = ServerLogic::GetInstServerLogic()->GetPiTime() + LOGIC_CONFIG->GetDailyFbCfg().GetExpRefreshCdByMemberNum(m_member_num);

	return;
}

SpecialExpFB::MemberItem *SpecialExpFB::GetMemberItem(int uid)
{
	std::map<int, MemberItem *>::iterator iter = m_member_map.find(uid);
	if (m_member_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void SpecialExpFB::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	// 前3次保底经验
	{	
		if (!is_logout)
		{
			int enter_times = role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes();
			if (enter_times <= 3)
			{
				MemberItem *member_item = this->GetMemberItem(role->GetUID());
				if (NULL != member_item)
				{
					long long baodi_exp = LOGIC_CONFIG->GetDailyFbCfg().GetExpBaodiByEnterTimes(enter_times);
					if (member_item->exp < baodi_exp)
					{
						role->AddExp(baodi_exp - member_item->exp, "ExpFB_baodi", Role::EXP_ADD_REASON_EXP_FB_BAODI);
						member_item->exp = baodi_exp;
					}
				}
			}
		}
	}

	m_member_num--;
	role->GetRoleDailyFb()->SetLastQuitFbTime((unsigned int)(EngineAdapter::Instance().Time()));
	this->SyncFBSceneLogicInfoAll();
	this->SyncFBSceneLogicInfo(role->GetUID(), true);
}

void SpecialExpFB::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		return;
	}

	if (0 == m_start_time)
	{
		m_start_time = EngineAdapter::Instance().Time();
	}

	MemberItem *member_item = this->GetMemberItem(role->GetUID());
	if (NULL == member_item)
	{
		member_item = new MemberItem();
		member_item->uid = role->GetUID();
		member_item->record_max_exp = role->GetRoleDailyFb()->GetExpFbRecordMaxExp();
		m_member_map[member_item->uid] = member_item;
	}

	this->AddBuff(role);
	
	this->CreateWaveMonsters();

	m_member_num++;
	
	const OtherTeamfbMonsterAttrConfig* monster_upattr_cfg = LOGIC_CONFIG->GetOtherCfg().GetTeamFbMonsterAttrCfg(m_member_num);
	if (nullptr != monster_upattr_cfg)
	{
		m_add_monster_maxhp_per = monster_upattr_cfg->monster_hp_percent;
		m_add_monster_gongji_per = monster_upattr_cfg->monster_gongji_percent;
		m_add_monster_fangyu_per = monster_upattr_cfg->monster_fangyu_percent;
	}

	this->SyncFBSceneLogicInfoAll();
}

void SpecialExpFB::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		m_kill_allmonster_num++;

		const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();

		Obj *killer_obj = m_scene->GetObj(killer_id);

		if(NULL != killer_obj && Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
		{
			Role *killer = m_scene->GetPrincipal(killer_id);
			if (NULL != killer)
			{
//				Monster *monster = (Monster *)dead;
				// 其他队员获得额外经验
				//long long extra_exp = (long long)(monster->GetRewardExp() * (exp_other_cfg.extra_exp_per * ROLE_ATTR_PER)) / (member_num - 1);
				long long extra_exp = 0;
				const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();

				for (std::map<int, MemberItem *>::iterator it = m_member_map.begin(); it != m_member_map.end(); it++)
				{
					Role *member_role = m_scene->GetRoleByUID(it->first);
					if (NULL == member_role)
					{
						continue;
					}
					// 计算经验系数
					{
						extra_exp = LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(member_role->GetLevel(), 0);	// 根据等级获取经验
						double exp_factor = 0;
						// 根据当前波数获取经验系数
						switch (m_wave)
						{
						case 1: { exp_factor = exp_other_cfg.exp_factor1; } break;
						case 2: { exp_factor = exp_other_cfg.exp_factor2; } break;
						case 3: { exp_factor = exp_other_cfg.exp_factor3; } break;
						}
						extra_exp = static_cast<long long>(extra_exp * exp_factor);							// 根据经验系数提供经验值
					}

					MemberItem *member_item = it->second;
					if (NULL == member_item) continue;
					
					int add_percent = 0;
					// 根据队友数量计算经验加成
					{
						int member_count = m_scene->RoleNum();
						if (2 == member_count)
						{
							add_percent = 15;
						}
						else if (3 == member_count)
						{
							add_percent = 30;
						}
					}
					member_role->AddExp(extra_exp, "ExpFB", Role::EXP_ADD_EXP_FB, NULL, add_percent);
				}

				this->SyncFBSceneLogicInfoAll();
			}
		}

		if (m_wave >= exp_other_cfg.total_wave && m_wave_refresh_count >= exp_other_cfg.other_wave_refresh_count && this->IsAllMonsterDead())
		{
			this->OnExpFBFinish();
		}
	}
}

void SpecialExpFB::OnAddExp(Role *role, long long add_exp, int reason)
{
	if (NULL == role || add_exp < 0) return;

	MemberItem * member_item = this->GetMemberItem(role->GetUID());
	if (NULL == member_item) return;
	member_item->exp += add_exp;

	role->GetRoleDailyFb()->SetExpFbLastExp(member_item->exp);
}

bool SpecialExpFB::OnTimeout()
{
	if (EngineAdapter::Instance().Time() >= m_timeout_time)
	{
		this->OnExpFBFinish();
		return false;
	}

	return true;
}

void SpecialExpFB::OnExpFBFinish()
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_is_pass = true;
	
	this->KillAllMonster();
	this->SyncFBSceneLogicInfoAll();

	for (std::map<int, MemberItem *>::iterator it = m_member_map.begin(); it != m_member_map.end(); it++)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it->first);
		if (NULL != user_node)
		{
			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_EXP_FB, user_node->uid, user_node->user_name, NULL, user_node->level, m_wave, NULL, NULL);
		}
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			EventHandler::Instance().OnPassExpFb(temp_role);
		}
	}
	m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
}

void SpecialExpFB::OnPayBuff(Role *role)
{
	if (NULL == role) return;

	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();

	MemberItem *member_item = this->GetMemberItem(role->GetUID());
	if (NULL == member_item)
	{
		return;
	}

	if (member_item->buff_times >= exp_other_cfg.max_buff_time)
	{
		role->NoticeNum(errornum::EN_EXP_FB_GUWU_LIMIT);
		return;
	}

	if (member_item->buff_times * exp_other_cfg.buff_add_gongji_per >= 10000)
	{
		role->NoticeNum(errornum::EN_EXP_FB_GUWU_LIMIT);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(exp_other_cfg.buff_cost))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(exp_other_cfg.buff_cost, "ExpFbGuWuCost"))
	{
		return;
	}

	member_item->buff_times++;
	this->AddBuff(role);
	role->NoticeNum(noticenum::NT_EQUI_EXP_FB_GUWU_SUCC);

	this->SyncFBSceneLogicInfo(role->GetUID());
}

void SpecialExpFB::OnAutoPayBuff(Role * role)
{
	if (NULL == role) return;

	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();

	MemberItem *member_item = this->GetMemberItem(role->GetUID());
	if (NULL == member_item)
	{
		return;
	}
	if (member_item->buff_times >= exp_other_cfg.max_buff_time)
	{
		role->NoticeNum(errornum::EN_EXP_FB_GUWU_LIMIT);
		return;
	}

	if (member_item->buff_times * exp_other_cfg.buff_add_gongji_per >= 10000)
	{
		role->NoticeNum(errornum::EN_EXP_FB_GUWU_LIMIT);
		return;
	}
	const int diff_times = exp_other_cfg.max_buff_time - member_item->buff_times;
	const int cost = diff_times * exp_other_cfg.buff_cost;

	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(cost))
	{
		role->NoticeNum(errornum::EN_EXP_FB_GUWU_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(cost, "ExpFbGuWuCost"))
	{
		return;
	}

	member_item->buff_times += static_cast<char>(diff_times);
	
	if (member_item->buff_times <= 0)
	{
		return;
	}
	
	role->NoticeNum(noticenum::NT_EQUI_EXP_FB_AUTO_GUWU_SUCC);
	this->AddBuff(role);

	this->SyncFBSceneLogicInfo(role->GetUID());
}

void SpecialExpFB::OnPayBuffReq(Role * role, bool is_auto)
{
	if (NULL == role) return;

	if (is_auto)
	{
		this->OnAutoPayBuff(role);
	}
	else
	{
		this->OnPayBuff(role);
	}
}

void SpecialExpFB::AddBuff(Role *role)
{
	if (NULL == role) return;

	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();

	MemberItem *member_item = this->GetMemberItem(role->GetUID());
	if (NULL == member_item || member_item->buff_times <= 0)
	{
		return;
	}

	int guwu_per = member_item->buff_times * exp_other_cfg.buff_add_gongji_per;
	if (guwu_per > 10000)
	{
		guwu_per = 10000;
	}

	EffectIncHurt *effect = new EffectIncHurt(role->GetId(), 0, exp_other_cfg.time_limit * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect->SetIncPercent(guwu_per);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	role->AddEffect(effect);
}

bool SpecialExpFB::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role) return false;
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 10, realive_pos)) return true;
		}
	}

	return false;
}

bool SpecialExpFB::SpecialIsEnemy(Role *role, Obj *obj)
{	
	if (NULL == obj || NULL == role) return false;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return true;
}

void SpecialExpFB::SyncFBSceneLogicInfoAll()
{
	for (std::map<int, MemberItem *>::iterator it = m_member_map.begin(); it != m_member_map.end(); it++)
	{
		this->SyncFBSceneLogicInfo(it->first);
	}
}

void SpecialExpFB::SyncFBSceneLogicInfo(int uid, bool is_leave)
{
	Role *role = m_scene->GetRoleByUID(uid);
	if (NULL == role)
	{
		return;
	}

	MemberItem *member_item = this->GetMemberItem(uid);
	if (NULL == member_item)
	{
		return;
	}

	static Protocol::SCExpFbInfo cmd; 
	cmd.reset();

	cmd.time_out_stamp = m_timeout_time;
	cmd.scene_type = this->GetSceneType();

	cmd.is_finish = m_is_finish ? 1 : 0;
	if (is_leave) 
	{
		cmd.is_finish = 1;
	}

	cmd.exp = member_item->exp;
	cmd.guwu_times = member_item->buff_times;
	cmd.team_member_num = m_member_num;
	cmd.wave = m_wave;
	cmd.kill_allmonster_num = m_kill_allmonster_num;
	cmd.start_time = m_start_time;
	cmd.exp_percent = 0;

	if (m_is_pass && 0 == member_item->record_max_exp && member_item->exp > 0)
	{
		role->GetRoleDailyFb()->SetExpFbRecordMaxExp(member_item->exp);
		cmd.exp_percent = 100;
	}
	else if (m_is_pass && member_item->exp > member_item->record_max_exp)
	{
		role->GetRoleDailyFb()->SetExpFbRecordMaxExp(member_item->exp);
		cmd.exp_percent = static_cast<int>((member_item->exp - member_item->record_max_exp) * 100.0 / member_item->record_max_exp);
	}
	
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}