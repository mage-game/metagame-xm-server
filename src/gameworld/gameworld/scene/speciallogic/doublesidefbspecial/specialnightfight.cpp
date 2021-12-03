#include <algorithm>
#include "specialnightfight.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/scenemanager.h"
#include "protocal/msgfight.h"
#include "internalcomm.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/monsterreplaceconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/cross/rolecross.h"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/speciallogic/guildspecial/specialguildfb.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "doublesidefb/doublesidefbmanager.hpp"
#include "item/knapsack.h"
#include "gameworld/gameworld/monster/monsterpool.h"
#include "gameworld/global/worldstatus2/worldstatus2.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "other/shop/roleshop.hpp"

#include "gstr.h"

SpecialNightFight::SpecialNightFight(Scene *scene)
	: SpecialLogic(scene), 
	m_is_finish(0), m_turn(0), m_is_init_finish(false), m_red_side_num(0), m_blue_side_num(0),
	m_boss_objid(INVALID_OBJ_ID), m_boss_left_hp_per(0), m_boss_max_hp(0), 
	m_next_redistribute_time(0), m_next_get_score_time(0), m_next_update_rank_time(0), m_kick_out_time(0), m_next_update_role_time(0)
{
	m_role_info_map.clear();
	m_score_rank_vec.clear();
	m_player_reward_record_map.clear();
}

SpecialNightFight::~SpecialNightFight()
{

}

bool SpecialNightFight::CanDestroy()
{
	if(!this->IsInCrossServer())
	{
		return ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	}
	else
	{
		return ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	}
}

void SpecialNightFight::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_finish)
	{
		//if (!this->IsInCrossServer())
		//{
		//	if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_NIGHT_FIGHT_FB) && m_kick_out_time == 0)
		//	{
		//		this->OnActivityClose();
		//	}
		//}

		const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

		// 刷新人物信息
		if (m_next_update_role_time > 0 && now_second >= m_next_update_role_time)
		{
			for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
			{
				Role *role = this->GetRole(role_info->second.user_key);
				if (NULL == role)	continue;

				this->SendInfoToRole(role);
			}

			this->SendAllRoleScoreInfo();

			this->SendRedSideListInfo();

			m_next_update_role_time = static_cast<unsigned int>(now_second) + other_cfg.update_role_interval_s;
		}

		// 刷新排行榜
		if (m_next_update_rank_time > 0 && now_second >= m_next_update_rank_time)
		{
			this->UpdateScoreRank();
			this->SendHurtBossRankInfo();

			m_next_update_rank_time = static_cast<unsigned int>(now_second) + other_cfg.rank_update_interval_s;
		}

		// 获取积分
		if (m_next_get_score_time > 0 && now_second >= m_next_get_score_time)
		{
			for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
			{
				Role *role = this->GetRole(role_info->second.user_key);
				if (NULL == role)	continue;

				role_info->second.score += other_cfg.interval_time_get_score;

				this->SendInfoToRole(role);
			}

			m_next_get_score_time = static_cast<unsigned int>(now_second) + other_cfg.interval_time_s;
		}

		// 发放奖励、刷新boss 并 重新分配人员
		if (m_next_redistribute_time > 0 && now_second >= m_next_redistribute_time)
		{
			m_next_redistribute_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.redistribute_interval_time_s;

			this->GiveScoreRankReward();
			this->CheckFlushBoss();
			this->CheckRedistribute(m_next_redistribute_time);
			this->UpdateScoreRank();

			m_turn++;
			this->SendRedSideListInfo();
		}
	}

	// 踢出玩家
	if(m_kick_out_time > 0 && now_second > m_kick_out_time)
	{	
		if (!this->IsInCrossServer())
		{
			this->DelayKickOutAllRole();
		}
		else
		{
			this->KickAllToOrginalToServer();
		}

		m_kick_out_time = 0;
	}
}

void SpecialNightFight::Init()
{
	if (this->IsInCrossServer())
	{
		if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			return;
		}
	}
	else
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			return;
		}
	}

	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_turn = 1;

	// 本服日常活动获取的开启时间实为准备时间，现利用结束时间推算开始时间
	//unsigned int activity_begin_time = this->IsInCrossServer() ? ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB) : ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	//this->IsInCrossServer() ? activity_begin_time : activity_begin_time += ACTIVITY_DEFAULT_READY_TIME_S;
	unsigned int activity_begin_time = ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	if (!this->IsInCrossServer())
	{
		activity_begin_time = ActivityShadow::Instance().GetActivityEndTime(ACTIVITY_TYPE_NIGHT_FIGHT_FB) - other_cfg.activity_open_dur_s;
		if (activity_begin_time < ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			activity_begin_time = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_NIGHT_FIGHT_FB);
		}
	}

	for (int i = 0; (activity_begin_time + other_cfg.redistribute_interval_time_s) < now; i++)
	{
		m_turn++;

		activity_begin_time += other_cfg.redistribute_interval_time_s;
	}

	m_next_get_score_time = activity_begin_time + other_cfg.interval_time_s;
	m_next_redistribute_time = activity_begin_time + other_cfg.redistribute_interval_time_s;
	m_next_update_rank_time = activity_begin_time + other_cfg.rank_update_interval_s;
	m_next_update_role_time = activity_begin_time + other_cfg.update_role_interval_s;

	m_is_init_finish = true;
}

void SpecialNightFight::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if(!this->IsInCrossServer())
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			this->DelayKickOutAllRole();
			return;
		}
	}
	
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if(!m_is_init_finish && ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB))
	{
		this->Init();

		this->CheckFlushBoss();
	}

	{
		PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
		if (it == m_role_info_map.end())
		{
			RoleFbInfo role_info;
			role_info.user_key = this->GetRoleKey(role);
			role_info.is_red_side = m_red_side_num > m_blue_side_num ? false : true;
			role_info.score = other_cfg.init_score;
			role_info.total_score = 0;
			role_info.kill_role_num = 0;
			memset(role_info.rank_list, -1, sizeof(role_info.rank_list));

			PlayerRewardRecordMapIt role_rank_it = m_player_reward_record_map.find(this->GetRoleKey(role));
			if (role_rank_it != m_player_reward_record_map.end())
			{
				memcpy(role_info.rank_list, role_rank_it->second.rank_list, sizeof(role_info.rank_list));
			}

			ScoreRankInfo rank_info;
			rank_info.score = other_cfg.init_score;
			rank_info.user_key = this->GetRoleKey(role);
			rank_info.obj_id = role->GetId();
			rank_info.is_red_side = m_red_side_num > m_blue_side_num ? 0 : 1;
			role->GetName(rank_info.user_name);

			TotalScoreRank total_rank;
			total_rank.total_score = 0;
			total_rank.user_key = this->GetRoleKey(role);
			role->GetName(total_rank.user_name);

			if (this->GetRoleKey(role) != -1)
			{
				m_role_info_map[this->GetRoleKey(role)] = role_info;
				m_score_rank_vec.push_back(rank_info);
				m_total_score_vec.push_back(total_rank);
			}

			role_info.is_red_side ? m_red_side_num++ : m_blue_side_num++;
		}
		else
		{
			RoleFbInfo &role_info = it->second;
			role_info.user_key = this->GetRoleKey(role);

			// 轮数变更需要重新分配
			bool is_flush = (0 != role_info.next_redistribute_time && now >= role_info.next_redistribute_time);
			if (is_flush)
			{
				role_info.is_red_side = m_red_side_num > m_blue_side_num ? false : true;
				role_info.score = other_cfg.init_score;
			}

			ScoreRankInfo * rank_info = this->GetRoleRankInfoByRoleKey(role_info.user_key);
			if (rank_info)
			{
				rank_info->obj_id = role->GetId();
				rank_info->user_key = this->GetRoleKey(role);
				if (is_flush)
				{
					rank_info->is_red_side = m_red_side_num > m_blue_side_num ? 0 : 1;
				}
			}

			role_info.is_red_side ? m_red_side_num++ : m_blue_side_num++;
		}
	}

	this->SendRedSideListInfo();
	this->SendInfoToRole(role);

	if (this->IsInCrossServer())
	{
		EventHandler::Instance().OnJoinActivity(role, CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	}
	else
	{
		EventHandler::Instance().OnJoinActivity(role, ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	}
}

void SpecialNightFight::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role)
	{
		return;
	}

	PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
	if (it != m_role_info_map.end())
	{
		//for (std::vector<ScoreRankInfo>::iterator rank_it = m_score_rank_vec.begin(); rank_it != m_score_rank_vec.end(); ++rank_it)
		//{
		//	if (rank_it->user_key == it->second.user_key)
		//	{
		//		m_score_rank_vec.erase(rank_it);
		//
		//		break;
		//	}
		//}

		//for (std::vector<TotalScoreRank>::iterator total_itr = m_total_score_vec.begin(); total_itr != m_total_score_vec.end(); ++total_itr)
		//{
		//	if (total_itr->user_key == it->second.user_key)
		//	{
		//		m_total_score_vec.erase(total_itr);
		//
		//		break;
		//	}
		//}

		// 记录礼包消息
		m_player_reward_record_map[this->GetRoleKey(role)].user_key = this->GetRoleKey(role);
		memcpy(m_player_reward_record_map[this->GetRoleKey(role)].rank_list, it->second.rank_list, sizeof(m_player_reward_record_map[this->GetRoleKey(role)].rank_list));

		if(it->second.is_red_side)
		{
			m_red_side_num--;
		}
		else
		{
			m_blue_side_num--;
		}

		//m_role_info_map.erase(it);
	}

	if (!role->IsAlive())
	{
		role->ReAlive(100, 100, role->GetPos(), true);
	}
	else
	{
		role->Recover();
	}

	if(!this->IsInCrossServer())
	{
		DoubleSideFBManager::Instance().RequireLeaveFb(role, ACTIVITY_TYPE_NIGHT_FIGHT_FB, false);
	}
	else
	{
		DoubleSideFBManager::Instance().RequireLeaveFb(role, CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB, false);
	}

	this->SendRedSideListInfo();
}

void SpecialNightFight::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster *)cha;

		if (monster->IsBossMonster())
		{
			Attribute cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			m_boss_left_hp_per = static_cast<int>(cur_hp * 1.00f / max_hp * 10000.00f);

			Obj *obj = m_scene->GetObj(enemy_id);
			if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *role = (Role *)obj;

				// 计算玩家伤害值
				this->CalcRoleHurtBossVal(role, delta_hp);
			}
		}
	}
}

void SpecialNightFight::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}
	
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		bool is_red_side = false;
		Obj *obj = m_scene->GetObj(killer_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *killer_role = (Role*)obj;
			PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(killer_role));
			if (it != m_role_info_map.end())
			{
				it->second.score += other_cfg.kill_monster_add_score;
				is_red_side = it->second.is_red_side;
			}

			this->SendInfoToRole(killer_role);

			//如果是boss
			Monster *dead_monster = (Monster *)dead;
			if (nullptr != dead_monster && dead_monster->IsBossMonster())
			{
				{
					m_boss_objid = INVALID_OBJ_ID;
					m_boss_left_hp_per = 0;
					m_boss_max_hp = 0;

					this->CheckGiveScoreOnBossDie(is_red_side);
					this->BroadcastBossDie(killer_role, dead_monster->GetMonsterId(), is_red_side);
				}


				// 通知场景内玩家
				for (unsigned int i = 0; i < m_scene->RoleNum(); i++)
				{
					Role *temp_role = m_scene->GetRoleByIndex(i);
					if (nullptr == temp_role)
					{
						continue;
					}

					this->SendInfoToRole(temp_role);
				}
			}
		}
	}
	else if(NULL != dead && Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		int add_score = 0, reduce_score = 0;

		Obj *kill_obj = m_scene->GetObj(killer_id);

		if (NULL != kill_obj && Obj::OBJ_TYPE_ROLE == kill_obj->GetObjType())
		{
			// 死者扣分，只有被玩家杀死才扣分
			Role *dead_role = (Role*)dead;
			PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(dead_role));
			if (it != m_role_info_map.end())
			{
				reduce_score = static_cast<int>(it->second.score / 100.0f * other_cfg.killed_reduce_score_per);
				add_score = static_cast<int>(it->second.score / 100.0f * other_cfg.kill_get_score_per);

				it->second.score -= reduce_score;

				if (it->second.score < other_cfg.min_score)
				{
					it->second.score = other_cfg.min_score;
				}
			}

			// 击杀者加分
			{
				Role *killer_role = (Role*)kill_obj;
				PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(killer_role));
				if (it != m_role_info_map.end())
				{
					it->second.kill_role_num++;

					it->second.score += add_score;
				}

				// 助攻者加分
				static const int RANGE = other_cfg.assisting_area;
				static ObjID targer_list[32] = { INVALID_OBJ_ID };
				int count = m_scene->GetZoneMatrix()->GetObjByArea(dead_role->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));

				for (int i = 0; i < count; i++)
				{
					Role *assisting_role = (Role*)m_scene->GetObj(targer_list[i]);
					if (NULL != assisting_role && Obj::OBJ_TYPE_ROLE == assisting_role->GetObjType()
						&& this->GetRoleKey(killer_role) != this->GetRoleKey(assisting_role) && this->GetRoleKey(dead_role) != this->GetRoleKey(assisting_role))
					{
						PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(assisting_role));
						if (it != m_role_info_map.end())
						{
							it->second.score += other_cfg.assisting_add_score;
						}
					}
				}

				this->SendInfoToRole(dead_role);
				this->SendInfoToRole(killer_role);
			}
		}
	}
}

bool SpecialNightFight::SpecialRelive(Role * role, int realive_type)
{
	if(NULL == role)
	{
		return false;
	}

	// 客户端倒计时5秒后请求

	if(Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		if (role->ReAlive(100, 100, LOGIC_CONFIG->GetNightFightFbConfig().GetRelievePos())) return true;
	}

	return false;
}

bool SpecialNightFight::SpecialIsEnemy(Role * role, Obj * obj)
{
	if(NULL == role)
	{
		return true;
	}

	switch (obj->GetObjType())
	{
		case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;
			if (NULL == target_role)
			{
				return true;
			}

			PlayerInfoMapIt role_it = m_role_info_map.find(this->GetRoleKey(role));
			PlayerInfoMapIt target_role_it = m_role_info_map.find(this->GetRoleKey(target_role));
			if (role_it != m_role_info_map.end() && target_role_it != m_role_info_map.end())
			{
				if(role_it->second.is_red_side ^ target_role_it->second.is_red_side)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		break;

		case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			if (NULL == monster)
			{
				return true;
			}

			return monster->IsAlive();
		}
		break;

		default:
			break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialNightFight::OnActivityStatusChange(int status)
{
	if (ACTIVITY_STATUS_OPEN == status)
	{
		this->OnActivityStart();
	}
	else if (ACTIVITY_STATUS_CLOSE == status)
	{
		this->OnActivityClose();
	}
}

void SpecialNightFight::OnActivityStart()
{
	if (!m_is_init_finish && m_role_info_map.size()>0)
	{
		this->Init();

		this->CheckFlushBoss();
	}

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			Posi pos = LOGIC_CONFIG->GetNightFightFbConfig().GetRelievePos();
			role->ResetPos(pos.x, pos.y);
		}
	}
}

void SpecialNightFight::OnActivityClose()
{
	m_is_finish = 1;

	GiveScoreRankReward();
	CheckRewardOnClose();

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		this->SendInfoToRole(role);
	}

	m_role_info_map.clear();
	m_score_rank_vec.clear();
	m_player_reward_record_map.clear();
	m_total_score_vec.clear();
	CheckClearBossRank();

	m_turn = 0;
	m_is_init_finish = false;
	m_red_side_num = 0;
	m_blue_side_num = 0;
	m_next_redistribute_time = 0;
	m_next_get_score_time = 0;
	m_next_update_rank_time = 0;
	//m_kick_out_time = 0;
	m_next_update_role_time = 0;

	

	this->KillAllMonster();
	this->SetKickOutTime();
}

void SpecialNightFight::SetKickOutTime()
{
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

	m_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.delay_kick_out_time;
}

static bool NightFightScoreSort(const ScoreRankInfo &role_score_1, const ScoreRankInfo &role_score_2)
{
	return role_score_1.score > role_score_2.score;
}

void SpecialNightFight::SortScoreRank()
{
	if (m_total_score_vec.size() <= 0)
	{
		return;
	}
	long long old_first = m_total_score_vec[0].user_key;

	std::sort(m_score_rank_vec.begin(), m_score_rank_vec.end(), NightFightScoreSort);
	std::sort(m_total_score_vec.begin(), m_total_score_vec.end(), [](const TotalScoreRank & rank1, const TotalScoreRank & rank2) {return rank1.total_score > rank2.total_score;});

	static Protocol::SCNightFightRankInfo nfri;
	nfri.rank_count = 0;
	memset(nfri.rank_info_list, -1, sizeof(nfri.rank_info_list));

	for(std::vector<ScoreRankInfo>::const_iterator it = m_score_rank_vec.begin(); it != m_score_rank_vec.end(); ++it)
	{
		Role *role = this->GetRole(it->user_key);
		if (NULL == role)
		{
			continue;
		}

		if ((unsigned int)nfri.rank_count >= static_array_size(nfri.rank_info_list))
		{
			break;
		}

		nfri.rank_info_list[nfri.rank_count].obj_id = role->GetId();
		nfri.rank_info_list[nfri.rank_count].is_red_side = it->is_red_side;
		nfri.rank_info_list[nfri.rank_count].score = it->score;
		nfri.rank_info_list[nfri.rank_count].user_key = it->user_key;
		F_STRNCPY(nfri.rank_info_list[nfri.rank_count].user_name, it->user_name, sizeof(GameName));

		nfri.rank_count++;
	}

	static Protocol::SCNightFightTotalScoreRank nftc;
	nftc.rank_count = 0;
	memset(nftc.total_rank_list, 0, sizeof(nftc.total_rank_list));

	std::vector<TotalScoreRank>::const_iterator itr = m_total_score_vec.begin();
	for (; itr != m_total_score_vec.end() && nftc.rank_count < Protocol::SCNightFightTotalScoreRank::TOTAL_SOCRE_RANK_MAX; ++itr)
	{
		nftc.total_rank_list[nftc.rank_count].user_key = itr->user_key;
		nftc.total_rank_list[nftc.rank_count].total_score = itr->total_score;
		F_STRNCPY(nftc.total_rank_list[nftc.rank_count].user_name, itr->user_name, sizeof(GameName));
		nftc.rank_count++;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL == role)
			continue;
		
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&nfri, sizeof(nfri));
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&nftc, sizeof(nftc));
	}

	if (old_first != m_total_score_vec[0].user_key)
	{
		ActivityMsgManager::RankTopItem first;
		first.uid = m_total_score_vec[0].user_key;
		F_STRNCPY(first.user_name, m_total_score_vec[0].user_name, sizeof(GameName));
		ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
	}
	
	if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
	{
		ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
		int i = 0;
		for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < (int)m_total_score_vec.size(); ++i)
		{
			if (m_total_score_vec[i].total_score <= 0)
			{
				break;
			}
			top[i].uid = m_total_score_vec[i].user_key;
			F_STRNCPY(top[i].user_name, m_total_score_vec[i].user_name, sizeof(GameName));
		}

		ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
	}
}

void SpecialNightFight::OnGetPlayerPosi(Role *role, int rank)
{
	if (NULL == role || rank < 0 || rank >= (int)m_score_rank_vec.size()) return;
	if ((int)m_score_rank_vec.size() <= 0) return;

	static Protocol::SCNightFightPlayerPosi msg;
	std::sort(m_score_rank_vec.begin(), m_score_rank_vec.end(), NightFightScoreSort);

	Role *target = m_scene->GetPrincipal(m_score_rank_vec[rank].obj_id);
	if (NULL == target) return;

	msg.rank = rank;
	msg.obj_id = target->GetId();
	msg.posi = target->GetPos();

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void SpecialNightFight::CalcRoleHurtBossVal(Role *role, Attribute delta_hp)
{
	if (nullptr == role)
	{
		return;
	}

	long long role_key = this->GetRoleKey(role);
	auto it = std::find_if(m_hurt_boss_info_vec.begin(), m_hurt_boss_info_vec.end(), [role_key](const HurtBossInfo &cmp_item) {
		if (role_key == cmp_item.user_key)
		{
			return true;
		}
		else
		{
			return false;
		}
	});


	if (it != m_hurt_boss_info_vec.end())
	{
		(*it).hurt_val += -delta_hp;
		(*it).hurt_per = static_cast<int>((*it).hurt_val * 1.00f / m_boss_max_hp * 10000.00f);
	}
	else
	{
		PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
		if (it != m_role_info_map.end())
		{
			HurtBossInfo hurt_info;
			hurt_info.user_key = role_key;
			hurt_info.is_red_side = it->second.is_red_side;
			role->GetName(hurt_info.user_name);
			hurt_info.hurt_val += -delta_hp;
			hurt_info.hurt_per = static_cast<int>(hurt_info.hurt_val * 1.00f / m_boss_max_hp * 10000.00f);

			m_hurt_boss_info_vec.emplace_back(hurt_info);
		}
	}

	this->SortBossHurtRank();
}

void SpecialNightFight::UpdateScoreRank()
{
	for(PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
	{
		for(std::vector<ScoreRankInfo>::iterator rank_info = m_score_rank_vec.begin(); rank_info != m_score_rank_vec.end(); ++rank_info)
		{
			if(role_info->second.user_key == rank_info->user_key)
			{
				rank_info->score = role_info->second.score;
				rank_info->is_red_side = role_info->second.is_red_side ? 1 : 0;

				break;
			}
		}
	}

	for (std::vector<TotalScoreRank>::iterator total_rank = m_total_score_vec.begin(); total_rank != m_total_score_vec.end(); ++total_rank)
	{
		PlayerInfoMapIt info_itr = m_role_info_map.find(total_rank->user_key);
		if (info_itr != m_role_info_map.end())
		{
			total_rank->total_score = info_itr->second.total_score + info_itr->second.score;
		}
	}

	this->SortScoreRank();
}

void SpecialNightFight::GiveScoreRankReward()
{
	this->UpdateScoreRank();

	for (int i = 0; i < static_cast<int>(m_score_rank_vec.size()); ++i)
	{
		const RewardCfg * reward_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetRewardCfgByRank(i);

		Role *role = this->GetRole(m_score_rank_vec[i].user_key);

		if(NULL != role && NULL != reward_cfg)
		{
			role->GetKnapsack()->PutList(reward_cfg->reward_count, reward_cfg->reward_item_list, PUT_REASON_NIGHT_FIGHT);

			role->AddCrossHonor(reward_cfg->cross_honor, "NightFightFb");

			role->GetRoleShop()->AddChestShopShengwang(reward_cfg->shengwang, "NightFightFb");

			{
				// 发送排名
				Protocol::SCNightFightReward uis;

				PlayerInfoMapIt role_info = m_role_info_map.find(m_score_rank_vec[i].user_key);
				if (role_info != m_role_info_map.end())
				{
					if(m_turn >= 1 || m_turn < NIGHT_FIGHT_MAX_RANK_COUNT)
					{
						role_info->second.rank_list[m_turn - 1] = i;
					}

					UNSTD_STATIC_CHECK(sizeof(uis.reward_list) == sizeof(role_info->second.rank_list));
					memcpy(uis.reward_list, role_info->second.rank_list, sizeof(uis.reward_list));

					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&uis, sizeof(Protocol::SCNightFightReward));
				}
			}
		}
	}
}

void SpecialNightFight::SortBossHurtRank()
{
	if (m_hurt_boss_info_vec.empty())
	{
		return;
	}

	std::sort(m_hurt_boss_info_vec.begin(), m_hurt_boss_info_vec.end());
}

bool SpecialNightFight::IsInCrossServer()
{
	return CrossConfig::Instance().IsHiddenServer();
}

long long SpecialNightFight::GetRoleKey(Role * role)
{
	if(NULL == role)
	{
		return -1;
	}

	if (!this->IsInCrossServer())
	{
		return static_cast<long long>(role->GetUID());
	}
	else
	{
		return UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	}
}

Role * SpecialNightFight::GetRole(long long user_key)
{
	if (this->IsInCrossServer())
	{
		return m_scene->GetRoleByUUID(LongLongToUniqueUserID(user_key));
	}
	else
	{
		return m_scene->GetRoleByUID(static_cast<int>(user_key));
	}
}

int SpecialNightFight::GetRoleRankByRoleKey(long long role_key)
{
	for (int i = 0; i < static_cast<int>(m_score_rank_vec.size()); ++i)
	{
		if(m_score_rank_vec[i].user_key == role_key)
		{
			return i;
		}
	}

	return static_cast<int>(m_score_rank_vec.size());
}

int SpecialNightFight::GetRoleTotalRankByRoleKey(long long role_key)
{
	for (int i = 0; i < static_cast<int>(m_total_score_vec.size()); ++i)
	{
		if (m_total_score_vec[i].user_key == role_key)
		{
			return i;
		}
	}

	return static_cast<int>(m_total_score_vec.size());
}

ScoreRankInfo * SpecialNightFight::GetRoleRankInfoByRoleKey(long long role_key)
{
	for (int i = 0; i < static_cast<int>(m_score_rank_vec.size()); ++i)
	{
		if (m_score_rank_vec[i].user_key == role_key)
		{
			return &m_score_rank_vec[i];
		}
	}

	return NULL;
}

void SpecialNightFight::CheckFlushBoss()
{
	this->CheckClearBossRank();

	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
	int replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(other_cfg.boss_id);

	if (m_boss_objid != INVALID_OBJ_ID)
	{
		Obj *obj = m_scene->GetObj(m_boss_objid);
		if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster *monster = (Monster *)obj;
			if (monster->GetMonsterId() == replace_boss_id)
			{
				monster->ForceSetDead();
			}
		}
	}	

	Monster *boss = MONSTERPOOL->CreateMonster(replace_boss_id, m_scene, other_cfg.boss_born_pos.GetPos());
	if (NULL != boss)
	{
		m_boss_objid = boss->GetId();
		m_boss_max_hp = boss->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		m_boss_left_hp_per = 10000;
	}
}

void SpecialNightFight::CheckGiveScoreOnBossDie(bool is_red_side)
{
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

	for (unsigned int i = 0; i < m_scene->RoleNum(); i ++)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (nullptr == temp_role)
		{
			continue;
		}

		//击杀boss奖励积分改为同阵营所有成员
// 		if ((other_cfg.boss_born_pos.GetPos() - temp_role->GetPos()).IsLongerThan(other_cfg.kill_get_score_range))
// 		{
// 			continue;
// 		}

		PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(temp_role));
		if (it != m_role_info_map.end() && (is_red_side == it->second.is_red_side))
		{
			it->second.score += other_cfg.add_score;
		}
	}
}

void SpecialNightFight::BroadcastBossDie(Role *killer_role, int boss_id, bool is_red_side)
{
	if (nullptr == killer_role)
	{
		return;
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "night_fight_kill_boss_notice", is_red_side ? 1 : 0, killer_role->GetUID(), killer_role->GetName(), (int)killer_role->GetCamp(), boss_id);
	if (length > 0)
	{
		m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
	}
}

void SpecialNightFight::CheckClearBossRank()
{
	if (m_hurt_boss_info_vec.empty())
	{
		return;
	}

	m_hurt_boss_info_vec.clear();
}

void SpecialNightFight::CheckRedistribute(int next_time)
{
	m_red_side_num = 0, m_blue_side_num = 0;
	int leave_member_num = m_role_info_map.size();
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

	// 修正在场景的人数
	if (leave_member_num > (int)m_scene->RoleNum())
	{
		leave_member_num = (int)m_scene->RoleNum();
	}

	for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
	{
		role_info->second.total_score += role_info->second.score;
		role_info->second.score = other_cfg.init_score;

		Role *role = this->GetRole(role_info->second.user_key);
		if (NULL == role)	continue;

		int side = RandomNum(2);

		if (abs(m_red_side_num - m_blue_side_num) < leave_member_num)
		{
			// 0表示红方
			if (0 == side)
			{
				leave_member_num--;
				role_info->second.is_red_side = true;
				m_red_side_num++;
			}
			else
			{
				leave_member_num--;
				role_info->second.is_red_side = false;
				m_blue_side_num++;
			}
		}
		else
		{
			if (m_red_side_num > m_blue_side_num)
			{
				leave_member_num--;
				role_info->second.is_red_side = false;
				m_blue_side_num++;
			}
			else
			{
				leave_member_num--;
				role_info->second.is_red_side = true;
				m_red_side_num++;
			}
		}

		role_info->second.next_redistribute_time = next_time;

		this->SendInfoToRole(role);
	}
}

void SpecialNightFight::SendInfoToRole(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	int rank = this->GetRoleRankByRoleKey(this->GetRoleKey(role));
	int total_rank = this->GetRoleTotalRankByRoleKey(this->GetRoleKey(role));

	static Protocol::SCNightFightRoleInfo msg;
	PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
	if (it != m_role_info_map.end())
	{
		msg.turn = m_turn;
		msg.rank = rank;
		msg.total_rank = total_rank;
		msg.score = it->second.score;
		msg.total_score = it->second.total_score + it->second.score;
		msg.kill_role_num = it->second.kill_role_num;
		msg.is_red_side = it->second.is_red_side ? 1 : 0;
		msg.next_redistribute_time = m_next_redistribute_time;
		msg.next_get_score_time = m_next_get_score_time;
		msg.next_update_rank_time = m_next_update_rank_time;
		msg.kick_out_time = m_kick_out_time;
		msg.is_finish = m_is_finish;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&msg, sizeof(msg));
	}
}

void SpecialNightFight::SendRedSideListInfo()
{
	static Protocol::SCNightFightRedSideListInfo msg;
	msg.red_side_count = 0;
	memset(msg.red_side_list, 0, sizeof(msg.red_side_count));


	for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
	{
		Role *role = this->GetRole(role_info->second.user_key);
		if (NULL == role)
		{
			continue;
		}

		if(role_info->second.is_red_side)
		{
			msg.red_side_list[msg.red_side_count] = role->GetId();
			msg.red_side_count++;
		}
	}

	m_scene->SendToRole((const char *)&msg, sizeof(msg));
}

void SpecialNightFight::KickAllToOrginalToServer()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->ReAlive(100, 100);
			role->BackToOriginalServer();
		}
	}
}

void SpecialNightFight::SendAllRoleScoreInfo()
{
	static Protocol::SCNightFightAllRoleScoreInfo msg;
	msg.role_count = 0;
	memset(msg.score_info_list, 0, sizeof(msg.score_info_list));

	for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
	{
		Role *role = this->GetRole(role_info->second.user_key);
		if (NULL == role)
		{
			continue;
		}

		msg.score_info_list[msg.role_count].obj_id = role->GetId();
		msg.score_info_list[msg.role_count].score = role_info->second.score;
		msg.role_count++;
	}

	m_scene->SendToRole((const char *)&msg, sizeof(msg));
}

void SpecialNightFight::SendHurtBossRankInfo(Role *role)
{
	static Protocol::SCNightFightBossRankInfo info;
	info.boss_left_hp_per = m_boss_left_hp_per;

	info.boss_rank_count = 0;
	memset(info.boss_rank_info_list, 0, sizeof(info.boss_rank_info_list));

	for (std::vector<HurtBossInfo>::const_iterator it = m_hurt_boss_info_vec.begin(); it != m_hurt_boss_info_vec.end() && info.boss_rank_count < Protocol::SCNightFightBossRankInfo::BOSS_RANK_ITEM_MAX_COUNT; ++it)
	{
		if (it->hurt_val <= 0)
		{
			continue;
		}

		info.boss_rank_info_list[info.boss_rank_count].user_key = it->user_key;
		info.boss_rank_info_list[info.boss_rank_count].is_red_side = it->is_red_side ? 1 : 0;
		memcpy(info.boss_rank_info_list[info.boss_rank_count].user_name, it->user_name, sizeof(info.boss_rank_info_list->user_name));
		info.boss_rank_info_list[info.boss_rank_count].hurt_val = it->hurt_val;
		info.boss_rank_info_list[info.boss_rank_count].hurt_per = it->hurt_per;

		info.boss_rank_count++;
	}

	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, sizeof(info));
	}
	else
	{
		m_scene->SendToRole((const char*)&info, sizeof(info));
	}
}

void SpecialNightFight::CheckRewardOnClose()
{
	if (m_turn > 0)
	{
		//UpdateScoreRank();
		
		if (!IsInCrossServer())
		{
			for (int i = 0; i < static_cast<int>(m_total_score_vec.size()) && i < NIGHT_FIGHT_RANK_TOP_MAX; ++i)
			{
				WorldStatus2::Instance().SetNightFightTopRankUid(i + 1, static_cast<int>(m_total_score_vec[i].user_key));
			}
		}
		else
		{
			for (int i = 0; i < static_cast<int>(m_total_score_vec.size()) && i < NIGHT_FIGHT_RANK_TOP_MAX; ++i)
			{
				crossgameprotocal::GameCrossnNightFightInfoSync gcnfi;
				gcnfi.unique_user_id = m_total_score_vec[i].user_key;
				gcnfi.rank = i + 1;
				InternalComm::Instance().SendToCross((const char *)&gcnfi, sizeof(gcnfi));
			}
		}

		if (m_total_score_vec.size() == 1)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "night_fight_rank_top1", m_total_score_vec[0].user_name);
			if (lenth > 0)
			{
				m_scene->NotifySystemMsg(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_total_score_vec.size() == 2)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "night_fight_rank_top2",
				m_total_score_vec[0].user_name, m_total_score_vec[1].user_name);
			if (lenth > 0)
			{
				m_scene->NotifySystemMsg(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_total_score_vec.size() >= 3)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "night_fight_rank_top3",
				m_total_score_vec[0].user_name, m_total_score_vec[1].user_name, m_total_score_vec[2].user_name);
			if (lenth > 0)
			{
				m_scene->NotifySystemMsg(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
}
