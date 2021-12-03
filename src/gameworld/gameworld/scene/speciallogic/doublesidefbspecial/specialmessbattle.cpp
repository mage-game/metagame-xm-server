#include <algorithm>
#include "specialmessbattle.hpp"
#include "world.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/scenemanager.h"
#include "protocal/msgfight.h"
#include "internalcomm.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/speciallogic/guildspecial/specialguildfb.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "doublesidefb/doublesidefbmanager.hpp"
#include "servercommon/string/crossstr.h"
#include "config/monsterreplaceconfig.hpp"
#include "monster/monsterpool.h"
#include "other/cross/rolecross.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "gameworld/item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "gameworld/globalconfig/otherconfig.h"
#include "servercommon/errornum.h"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "other/shop/roleshop.hpp"

SpecialMessBattle::SpecialMessBattle(Scene *scene)
	: SpecialLogic(scene), m_turn(0), m_is_init_finish(false), m_next_redistribute_time(0), m_next_update_rank_time(0), m_kick_out_time(0),
						   m_next_update_role_time(0), m_last_check_time(0), m_boss_obj_id(INVALID_OBJ_ID), m_boss_max_hp(0),m_boss_hp(0), m_last_relive_pos(0)
{
	m_role_info_map.clear();
	m_rank_vec.clear();
	m_hurt_boss_info_vec.clear();
	m_player_reward_record_map.clear();
	m_total_score_rank_vec.clear();
}

SpecialMessBattle::~SpecialMessBattle()
{

}

bool SpecialMessBattle::CanDestroy()
{
	if(!this->IsInCrossServer())
	{
		return ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_MESS_BATTLE_FB);
	}
	else
	{
		return ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB);
	}
}

void SpecialMessBattle::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	// 检查无敌
	if (now_second >= m_last_check_time + 1)
	{
		m_last_check_time = static_cast<unsigned int>(now_second);

		for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
		{
			this->UpdateUser(role_info->second);
		}
	}

	// 发送积分等信息
	if (m_next_update_role_time > 0 && now_second >= m_next_update_role_time)
	{
		for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
		{
			Role *role = this->GetRole(role_info->second.user_key);
			if (NULL == role)	continue;

			this->SendInfoToRole(role);
		}

		this->SendAllRoleScoreInfo();

		m_next_update_role_time = static_cast<unsigned int>(now_second) + other_cfg.update_role_interval_s;
	}
	
	// 刷新排行榜
	if (m_next_update_rank_time > 0 && now_second >= m_next_update_rank_time)
	{
		this->UpdateRank();
		this->SortHurtRank();

		m_next_update_rank_time = static_cast<unsigned int>(now_second) + other_cfg.rank_update_interval_s;
	}

	// 发放奖励、重置boss 并 重置数据
	if(m_next_redistribute_time > 0 && now_second >= m_next_redistribute_time)
	{
		m_next_redistribute_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.redistribute_interval_time_s;

		// 发放奖励
		this->SortHurtRank();
		this->GiveReward();

		// 清空数据
		for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
		{
			role_info->second.total_score += role_info->second.score;
			role_info->second.score = other_cfg.init_score;

			Role *role = this->GetRole(role_info->second.user_key);
			if (NULL == role)	continue;

			this->SendInfoToRole(role);
		}
		this->UpdateRank();

		for(int i = 0; i < static_cast<int>(m_hurt_boss_info_vec.size()); ++i)
		{
			m_hurt_boss_info_vec[i].hurt_val = 0;
		}
		this->SortHurtRank();

		// 重置boss
		{
			if(m_boss_obj_id != INVALID_OBJ_ID)
			{
				m_scene->DeleteObj(m_boss_obj_id);
			}

			this->InitBoss();
		}
		m_turn++;
	}

	// 踢出玩家
	if(m_kick_out_time > 0 && now_second > m_kick_out_time)
	{
		if (!this->IsInCrossServer())
		{
			if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_MESS_BATTLE_FB))
			{
				this->DelayKickOutAllRole();
			}
		}
		else
		{
			this->KickAllToOrginalToServer();
		}

		m_kick_out_time = 0;
	}
}

void SpecialMessBattle::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if(!this->IsInCrossServer())
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			this->DelayKickOutAllRole();
			return;
		}
	}
	
	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	if (!m_is_init_finish && ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB))
	{
		this->Init();
	}

	{
		PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
		if (it == m_role_info_map.end())
		{
			RoleFbInfo role_info;
			role_info.user_key = this->GetRoleKey(role);
			role_info.score = other_cfg.init_score;
			role_info.kill_role_num = 0;
			role_info.consecutive_kill_num = 0;
			role_info.wudi_times = other_cfg.wudi_time_s;
			memset(role_info.rank_list, -1, sizeof(role_info.rank_list));

			PlayerRewardRecordMapIt role_rank_it = m_player_reward_record_map.find(this->GetRoleKey(role));
			if (role_rank_it != m_player_reward_record_map.end())
			{
				memcpy(role_info.rank_list, role_rank_it->second.rank_list, sizeof(role_info.rank_list));
			}
			
			MessBattleRankInfo rank_info;
			rank_info.score = other_cfg.init_score;
			rank_info.user_key = this->GetRoleKey(role);
			role->GetName(rank_info.user_name);

			PlayerHurtBossInfo player_hurt_boss_info;
			player_hurt_boss_info.hurt_val = 0;
			player_hurt_boss_info.user_key = this->GetRoleKey(role);
			role->GetName(player_hurt_boss_info.user_name);

			MessBattleTotalScoreRank total_score_rank;
			total_score_rank.total_score = 0;
			total_score_rank.user_key = this->GetRoleKey(role);
			role->GetName(total_score_rank.user_name);

			if (this->GetRoleKey(role) != -1)
			{
				m_role_info_map[this->GetRoleKey(role)] = role_info;
				m_rank_vec.push_back(rank_info);
				m_hurt_boss_info_vec.push_back(player_hurt_boss_info);
				m_total_score_rank_vec.push_back(total_score_rank);
			}
		}
	}

	this->SendInfoToRole(role);
	this->SendRoleRewardInfo(role);
	if (IsInCrossServer())
	{
		EventHandler::Instance().OnJoinActivity(role, CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB);
	}
	else
	{
		EventHandler::Instance().OnJoinActivity(role, ACTIVITY_TYPE_MESS_BATTLE_FB);
	}
}

void SpecialMessBattle::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role)
	{
		return;
	}

// 	PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
// 	if (it != m_role_info_map.end())
// 	{
// 		for (std::vector<MessBattleRankInfo>::iterator rank_it = m_rank_vec.begin(); rank_it != m_rank_vec.end(); ++rank_it)
// 		{
// 			if (rank_it->user_key == it->second.user_key)
// 			{
// 				m_rank_vec.erase(rank_it);
// 
// 				break;
// 			}
// 		}
// 		 
// 		for (std::vector<PlayerHurtBossInfo>::iterator hurt_rank_it = m_hurt_boss_info_vec.begin(); hurt_rank_it != m_hurt_boss_info_vec.end(); ++hurt_rank_it)
// 		{
// 			if (hurt_rank_it->user_key == it->second.user_key)
// 			{
// 				m_hurt_boss_info_vec.erase(hurt_rank_it);
// 
// 				break;
// 			}
// 		}
// 
// 		for (std::vector<MessBattleTotalScoreRank>::iterator total_rank_it = m_total_score_rank_vec.begin(); total_rank_it != m_total_score_rank_vec.end(); ++total_rank_it)
// 		{
// 			if (total_rank_it->user_key == it->second.user_key)
// 			{
// 				m_total_score_rank_vec.erase(total_rank_it);
// 
// 				break;
// 			}
// 		}
// 
// 		// 记录礼包消息
// 		m_player_reward_record_map[this->GetRoleKey(role)].user_key = this->GetRoleKey(role);
// 		memcpy(m_player_reward_record_map[this->GetRoleKey(role)].rank_list, it->second.rank_list, sizeof(m_player_reward_record_map[this->GetRoleKey(role)].rank_list));
// 
// 		m_role_info_map.erase(it);
// 	}


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
		DoubleSideFBManager::Instance().RequireLeaveFb(role, ACTIVITY_TYPE_MESS_BATTLE_FB, false);
	}
	else
	{
		DoubleSideFBManager::Instance().RequireLeaveFb(role, CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB, false);
	}
}

void SpecialMessBattle::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}
	
	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *dead_monster = (Monster *)dead;
		if (NULL != dead_monster)
		{
			Role *role = (Role*)m_scene->GetObj(killer_id);
			if (NULL != role && Obj::OBJ_TYPE_ROLE == role->GetObjType())
			{
				PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
				if (it != m_role_info_map.end())
				{
					if (dead_monster->IsBossMonster())
					{// 如果是boss

						m_boss_obj_id = INVALID_OBJ_ID;
						this->SortHurtRank();

						// 首先加积分
						for(int i = 0; i < static_cast<int>(m_hurt_boss_info_vec.size()); ++i)
						{
							PlayerInfoMapIt player_info_it = m_role_info_map.find(m_hurt_boss_info_vec[i].user_key);
							if (player_info_it == m_role_info_map.end() || m_hurt_boss_info_vec[i].hurt_val <= 0)
							{
								continue;
							}

							player_info_it->second.score += LOGIC_CONFIG->GetMessBattleFbConfig().GetKillBossGetScoreCfgByRank(i);

							if(player_info_it->second.score > other_cfg.max_score)
							{
								player_info_it->second.score = other_cfg.max_score;
							}
						}
					}
					else
					{// 如果是柱子
						it->second.score += other_cfg.kill_item_get_score;

						if (it->second.score > other_cfg.max_score)
						{
							it->second.score = other_cfg.max_score;
						}
					}
				}
			}

			this->SendInfoToRole(role);
		}
	}
	else if(Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		int change_score = 0, operate_score = 0;

		Role *killer_role = dynamic_cast<Role*>(m_scene->GetObj(killer_id));
		
		// 死者扣分，只有被玩家杀死才扣分
		Role *dead_role =dynamic_cast<Role*>(dead);
		if(NULL != dead_role && NULL != killer_role)
		{
			PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(dead_role));
			if (it != m_role_info_map.end())
			{
				// 只操作大于最小积分的部分
				operate_score = it->second.score - other_cfg.min_score;
				if (operate_score > 0)
				{
					change_score = static_cast<int>(operate_score / 100.0f * other_cfg.snatch_score_per);

					it->second.score -= change_score;
				}

				it->second.consecutive_kill_num = 0;
			}
			this->SendInfoToRole(dead_role);
			{
				PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(killer_role));
				if (it != m_role_info_map.end())
				{
					it->second.score += change_score;

					it->second.kill_role_num++;

					it->second.consecutive_kill_num++;

					if (it->second.score > other_cfg.max_score)
					{
						it->second.score = other_cfg.max_score;
					}

					// 发送击杀传闻
					if (it->second.consecutive_kill_num == 3 || it->second.consecutive_kill_num == 10)
					{
						// 传闻
						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mess_battle_liangsha_notice1,
							UidToInt(killer_role->GetUserId()), killer_role->GetName(), (int)killer_role->GetCamp(),
							it->second.consecutive_kill_num);

						if (length > 0)
						{
							//this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}

				// 发送抢分传闻
				if (change_score >= other_cfg.submit_report_limit_score)
				{
					// 传闻
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mess_battle_snatch_score,
						UidToInt(killer_role->GetUserId()), killer_role->GetName(), (int)killer_role->GetCamp(),
						UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(),
						change_score);

					if (length > 0)
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}
				this->SendInfoToRole(killer_role);
			}
		}
	}
}

bool SpecialMessBattle::SpecialRelive(Role * role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	// 客户端倒计时5秒后请求
	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	if(Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		if (role->ReAlive(100, 100, LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePosByKey(m_last_relive_pos)))
		{
			m_last_relive_pos++;

			if(m_last_relive_pos >= LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePosSize())
			{
				m_last_relive_pos = 0;
			}

			// 加无敌buff
			PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
			if (it != m_role_info_map.end())
			{
				it->second.wudi_times = other_cfg.wudi_time_s;
			}

			return true;
		}
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		// 加无敌buff
		PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
		if (it != m_role_info_map.end())
		{
			it->second.wudi_times = other_cfg.wudi_time_s;
		}
	}

	return false;
}

void SpecialMessBattle::OnAttack(Character * cha, ObjID enemy_id, Attribute delta_hp)
{
	if (delta_hp >= 0)
	{
		return;
	}

	Monster *boss = (Monster *)cha;
	if (NULL == boss || Obj::OBJ_TYPE_MONSTER != boss->GetObjType() || !boss->IsBossMonster())
	{
		return;
	}

	Role *attacher = (Role *)m_scene->GetObj(enemy_id);
	if(NULL == attacher || Obj::OBJ_TYPE_ROLE != attacher->GetObjType())
	{
		return;
	}

	if (m_boss_hp < -delta_hp)
	{
		delta_hp =static_cast<int>(-m_boss_hp);
	}

	for (int i = 0; i < static_cast<int>(m_hurt_boss_info_vec.size()); i++)
	{
		if(m_hurt_boss_info_vec[i].user_key == this->GetRoleKey(attacher))
		{
			m_hurt_boss_info_vec[i].hurt_val += -delta_hp;
		}
	}

	m_boss_hp += delta_hp;
}

void SpecialMessBattle::OnActivityStatusChange(int status)
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

void SpecialMessBattle::OnActivityStart()
{
	if (!m_is_init_finish && m_role_info_map.size()>0)
	{
		this->Init();
	}

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			Posi pos = LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePos();
			role->ResetPos(pos.x, pos.y);
		}
	}
}

void SpecialMessBattle::OnActivityClose()
{
	// 结算奖励
	GiveReward();
	// 结算称号
	CheckRewardOnClose();

	m_turn = 0;
	m_is_init_finish = false;

	//用于通知排行榜前三结算面板
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			if (i < 3)
			{
				if (static_cast<long long>(role->GetUID()) == m_total_score_rank_vec[i].user_key ||
					UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()) == m_total_score_rank_vec[i].user_key)
				{
					this->SendInfoToRole(role);
				}
			}
		}
	}

	m_rank_vec.clear();

	m_next_redistribute_time = 0;
	m_next_update_rank_time = 0;
	m_next_update_role_time = 0;
	m_last_check_time = 0;

	m_role_info_map.clear();
	
	m_boss_obj_id = INVALID_OBJ_ID;
	m_boss_max_hp = 0;
	m_boss_hp = 0;
	m_hurt_boss_info_vec.clear();
	m_last_relive_pos = 0;
	
	m_player_reward_record_map.clear();
	m_total_score_rank_vec.clear();

	this->KillAllMonster();

	this->SetKickOutTime();
}

void SpecialMessBattle::SetKickOutTime()
{
	//const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	m_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 1;
}

void SpecialMessBattle::Init()
{
	if (this->IsInCrossServer())
	{
		if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			return;
		}
	}
	else
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			return;
		}
	}

	m_turn = 1;

	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
	//unsigned int activity_begin_time = this->IsInCrossServer() ? ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB) : ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MESS_BATTLE_FB);
	// 日常活动获取的开启时间实为准备时间
	//this->IsInCrossServer() ? activity_begin_time: activity_begin_time += ACTIVITY_DEFAULT_READY_TIME_S;
	unsigned int activity_begin_time = ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB);
	if (!this->IsInCrossServer())
	{
		activity_begin_time = ActivityShadow::Instance().GetActivityEndTime(ACTIVITY_TYPE_MESS_BATTLE_FB) - other_cfg.activity_open_dur_s;
		if (activity_begin_time < ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			activity_begin_time = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MESS_BATTLE_FB);
		}
	}
	
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; (activity_begin_time + other_cfg.redistribute_interval_time_s) < now; i++)
	{
		m_turn++;

		activity_begin_time += other_cfg.redistribute_interval_time_s;
	}

	// 初始化boss
	this->InitBoss();
	m_is_init_finish = true;
	m_next_redistribute_time = activity_begin_time + other_cfg.redistribute_interval_time_s;
	m_next_update_rank_time = activity_begin_time + other_cfg.rank_update_interval_s;
	m_next_update_role_time = activity_begin_time + other_cfg.update_role_interval_s;

}

static bool MessBattleScoreSort(const MessBattleRankInfo &role_score_1, const MessBattleRankInfo &role_score_2)
{
	return role_score_1.score > role_score_2.score;
}

static bool MessBattleHurtValSort(const PlayerHurtBossInfo &role_score_1, const PlayerHurtBossInfo &role_score_2)
{
	return role_score_1.hurt_val > role_score_2.hurt_val;
}

static bool MessBattleTotalScoreSort(const MessBattleTotalScoreRank &total_score_1,const MessBattleTotalScoreRank & total_score_2)
{
	return total_score_1.total_score > total_score_2.total_score;
}

void SpecialMessBattle::SortRank()
{   
	if (m_total_score_rank_vec.size() <= 0)
	{
		return;
	}
	long long old_first = m_total_score_rank_vec[0].user_key;

	std::sort(m_rank_vec.begin(), m_rank_vec.end(), MessBattleScoreSort);
	std::sort(m_total_score_rank_vec.begin(), m_total_score_rank_vec.end(), MessBattleTotalScoreSort);

	static Protocol::SCMessBattleRankInfo nfri;
	nfri.rank_count = 0;
	memset(nfri.rank_info_list, -1, sizeof(nfri.rank_info_list));

	for(std::vector<MessBattleRankInfo>::const_iterator it = m_rank_vec.begin(); it != m_rank_vec.end() && nfri.rank_count <Protocol::SCMessBattleHurtRankInfo::RANK_ITEM_MAX_COUNT; ++it)
	{
		nfri.rank_info_list[nfri.rank_count].score = it->score;
		nfri.rank_info_list[nfri.rank_count].user_key = it->user_key;
		memcpy(nfri.rank_info_list[nfri.rank_count].user_name, it->user_name, sizeof(nfri.rank_info_list->user_name));

		nfri.rank_count++;
	}

	static Protocol::SCMessBattleToalScoreRank ntci;
	ntci.rank_count = 0;
	memset(ntci.total_score_list, 0, sizeof(ntci.total_score_list));

	std::vector<MessBattleTotalScoreRank>::const_iterator itr = m_total_score_rank_vec.begin();
	for (; itr != m_total_score_rank_vec.end() && ntci.rank_count< Protocol::SCMessBattleToalScoreRank::TOTAL_SOCRE_RANK_MAX; ++itr)
	{
		ntci.total_score_list[ntci.rank_count].total_score = itr->total_score;
		ntci.total_score_list[ntci.rank_count].user_key = itr->user_key;
		F_STRNCPY(ntci.total_score_list[ntci.rank_count].user_name, itr->user_name, sizeof(ntci.total_score_list->user_name));
		ntci.rank_count++;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if(NULL == role)
			continue;
		
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&nfri, sizeof(nfri));
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&ntci, sizeof(ntci));
	}

	if (m_total_score_rank_vec[0].user_key != old_first)
	{
		ActivityMsgManager::RankTopItem first;
		first.uid = m_total_score_rank_vec[0].user_key;
		F_STRNCPY(first.user_name, m_total_score_rank_vec[0].user_name, sizeof(GameName));
		ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
	}

	if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
	{
		ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
		unsigned int i = 0;
		for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < m_total_score_rank_vec.size(); ++i)
		{
			if (m_total_score_rank_vec[i].total_score <= 0)
			{
				break;
			}
			top[i].uid = m_total_score_rank_vec[i].user_key;
			F_STRNCPY(top[i].user_name, m_total_score_rank_vec[i].user_name, sizeof(GameName));
		}
		ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
	}
}

void SpecialMessBattle::SortHurtRank()
{
	std::sort(m_hurt_boss_info_vec.begin(), m_hurt_boss_info_vec.end(), MessBattleHurtValSort);

	static Protocol::SCMessBattleHurtRankInfo nfri;
	nfri.rank_count = 0;
	memset(nfri.rank_info_list, -1, sizeof(nfri.rank_info_list));
	std::vector<PlayerHurtBossInfo>::const_iterator it = m_hurt_boss_info_vec.begin();
	for (; it != m_hurt_boss_info_vec.end() && nfri.rank_count < Protocol::SCMessBattleHurtRankInfo::RANK_ITEM_MAX_COUNT; ++it)
	{
		if (m_boss_max_hp <= 0 || it->hurt_val <= 0)
			continue;

		nfri.rank_info_list[nfri.rank_count].hurt_per = static_cast<int>(floor(it->hurt_val * 1.0f / m_boss_max_hp * 100.0f));
		nfri.rank_info_list[nfri.rank_count].user_key = it->user_key;
		memcpy(nfri.rank_info_list[nfri.rank_count].user_name, it->user_name, sizeof(nfri.rank_info_list->user_name));

		nfri.rank_count++;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL == role)
			continue;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&nfri, sizeof(nfri));
	}
}

void SpecialMessBattle::UpdateRank()
{

	for (std::vector<MessBattleRankInfo>::iterator rank_info = m_rank_vec.begin(); rank_info != m_rank_vec.end(); ++rank_info)
	{
		PlayerInfoMapIt info_itr = m_role_info_map.find(rank_info->user_key);
		if (m_role_info_map.end() != info_itr)
		{
			rank_info->score = info_itr->second.score;
		}
	}
	for (std::vector<MessBattleTotalScoreRank>::iterator total_veci = m_total_score_rank_vec.begin(); total_veci != m_total_score_rank_vec.end(); ++total_veci)
	{
		PlayerInfoMapIt info_itr = m_role_info_map.find(total_veci->user_key);
		if (m_role_info_map.end() != info_itr)
		{
			total_veci->total_score = info_itr->second.total_score + info_itr->second.score;
		}
	}

	this->SortRank();
}

void SpecialMessBattle::GiveReward()
{
	this->UpdateRank();

	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	for (int i = 0; i < static_cast<int>(m_rank_vec.size()); ++i)
	{
		const MessBattleRewardCfg * reward_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetRewardCfgByRank(i);

		Role *role = this->GetRole(m_rank_vec[i].user_key);

		if(NULL != role && NULL != reward_cfg && m_rank_vec[i].score > 0)
		{
			role->GetKnapsack()->PutList(reward_cfg->reward_count, reward_cfg->reward_item_list, PUT_REASON_MESS_BATTLE);

			role->AddCrossHonor(reward_cfg->cross_honor, "MessBattleFb");

			role->GetRoleShop()->AddChestShopShengwang(reward_cfg->shengwang, "MessBattleFb");

			{
				// 发送排名
				//Protocol::SCMessBattleReward uis;

				PlayerInfoMapIt role_info = m_role_info_map.find(m_rank_vec[i].user_key);
				if (role_info != m_role_info_map.end())
				{
					if (m_turn >= 1 && m_turn < MESS_BATTLE_MAX_RANK_COUNT)
					{
						// 排行
						role_info->second.rank_list[m_turn - 1] = i;

						// 荣誉
						role_info->second.rank_list[m_turn - 1] += m_rank_vec[i].score * other_cfg.one_score_to_honor * 100;
					}

// 					UNSTD_STATIC_CHECK(sizeof(uis.reward_list) == sizeof(role_info->second.rank_list));
// 					memcpy(uis.reward_list, role_info->second.rank_list, sizeof(uis.reward_list));
// 
//					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&uis, sizeof(uis));
					this->SendRoleRewardInfo(role);
				}
			}
		}
	}
}

bool SpecialMessBattle::IsInCrossServer()
{
	return CrossConfig::Instance().IsHiddenServer();
}

void SpecialMessBattle::InitBoss()
{
	const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

	int replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(other_cfg.boss_id);
	Monster *boss = MONSTERPOOL->CreateMonster(replace_boss_id, m_scene, other_cfg.boss_posi);

	if (NULL != boss)
	{
		m_boss_obj_id = boss->GetId();

		m_boss_max_hp = boss->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		m_boss_hp = boss->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	}
}

long long SpecialMessBattle::GetRoleKey(Role * role)
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

Role * SpecialMessBattle::GetRole(long long user_key)
{
	if(this->IsInCrossServer())
	{
		return m_scene->GetRoleByUUID(LongLongToUniqueUserID(user_key));
	}
	else
	{
		return m_scene->GetRoleByUID(static_cast<int>(user_key));
	}
}

ObjID SpecialMessBattle::GetObjId(long long user_key)
{
	Role * role = nullptr;
	if (this->IsInCrossServer())
	{
		role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(user_key));
	}
	else
	{
		role = m_scene->GetRoleByUID(static_cast<int>(user_key));
	}
	if (nullptr != role)
	{
		return role->GetId();
	}
	else
	{
		return INVALID_OBJ_ID;
	}
}

int SpecialMessBattle::GetRoleRankByRoleKey(long long role_key)
{
	for (int i = 0; i < static_cast<int>(m_rank_vec.size()); ++i)
	{
		if (m_rank_vec[i].user_key == role_key)
		{
			return i;
		}
	}

	return static_cast<int>(m_rank_vec.size());
}

int SpecialMessBattle::GetRoleTotalRankByRoleKey(long long role_key)
{
	for (int i = 0; i < static_cast<int>(m_total_score_rank_vec.size()); ++i)
	{
		if (m_total_score_rank_vec[i].user_key == role_key)
		{
			return i;

		}
	}

	return static_cast<int>(m_total_score_rank_vec.size());
}

void SpecialMessBattle::SendInfoToRole(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	int rank = this->GetRoleRankByRoleKey(this->GetRoleKey(role));
	int total_rank = this->GetRoleTotalRankByRoleKey(this->GetRoleKey(role));

	static Protocol::SCMessBattleRoleInfo msg;
	PlayerInfoMapIt it = m_role_info_map.find(this->GetRoleKey(role));
	if (it != m_role_info_map.end())
	{
		if (m_boss_max_hp <= 0)
			return;

		msg.turn = m_turn;
		msg.rank = rank;
		msg.total_rank = total_rank;
		msg.boss_hp_per = static_cast<int>(ceil(m_boss_hp * 1.0f / m_boss_max_hp * 100.0f));
		msg.score = it->second.score;
		msg.total_score = it->second.total_score + it->second.score;
		msg.next_redistribute_time = m_next_redistribute_time;
		msg.next_update_rank_time = m_next_update_rank_time;
		msg.kick_out_time = m_kick_out_time;
		msg.is_finish = !m_is_init_finish;
		msg.boss_max_hp = m_boss_max_hp;
		msg.boss_cur_hp = m_boss_hp;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&msg, sizeof(msg));
	}
}

void SpecialMessBattle::KickAllToOrginalToServer()
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

void SpecialMessBattle::SendAllRoleScoreInfo()
{
	static Protocol::SCMessBattleAllRoleScoreInfo msg;
	msg.role_count = 0;
	memset(msg.score_info_list, 0, sizeof(msg.score_info_list));

	for (PlayerInfoMapIt role_info = m_role_info_map.begin(); role_info != m_role_info_map.end(); ++role_info)
	{
		ObjID obj_id = this->GetObjId(role_info->second.user_key);
		if(obj_id == INVALID_OBJ_ID) continue;
		msg.score_info_list[msg.role_count].obj_id = obj_id;
		msg.score_info_list[msg.role_count].score = role_info->second.score;
		msg.role_count++;
	}

	m_scene->SendToRole((const char *)&msg, sizeof(msg));
}

void SpecialMessBattle::SendRoleRewardInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}
	Protocol::SCMessBattleReward mbrwd;
	PlayerInfoMapIt itr= m_role_info_map.find(GetRoleKey(role));
	if (m_role_info_map.end() != itr)
	{
		UNSTD_STATIC_CHECK(sizeof(mbrwd.reward_list) == sizeof(itr->second.rank_list));
		memcpy(mbrwd.reward_list, itr->second.rank_list, sizeof(mbrwd.reward_list));

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&mbrwd, sizeof(mbrwd));
	}

}

void SpecialMessBattle::UpdateUser(RoleFbInfo &user_info)
{
	Role *role = this->GetRole(user_info.user_key);
	if (NULL == role) return;

	{
		if (user_info.wudi_times > 0)
		{
			user_info.wudi_times -= 1;

			this->AddSpecialBuff(role, EBT_WUDI, user_info.wudi_times > 0);
		}
	}
}

void SpecialMessBattle::AddSpecialBuff(Role *role, int ebt_type, bool is_add)
{
	if (NULL == role) return;

	if (is_add)
	{
		role->GetEffectList()->AddSpecialBuff(ebt_type);
	}
	else
	{
		role->GetEffectList()->RemoveSpecialBuff(ebt_type);
	}
}

void SpecialMessBattle::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialMessBattle::CheckRewardOnClose()
{
	if (m_turn > 0)
	{
		//UpdateRank();
		if (!IsInCrossServer())
		{
			for (int i = 0; i < static_cast<int>(m_total_score_rank_vec.size())&& i< MESS_BATTLE_TOP_RANK_MAX; ++i)
			{
				WorldStatus2::Instance().SetMessBattleTopRankUid(i + 1, static_cast<int>(m_total_score_rank_vec[i].user_key));
			}
		}
		else
		{
			for (int i = 0; i < static_cast<int>(m_total_score_rank_vec.size()) && i < MESS_BATTLE_TOP_RANK_MAX; ++i)
			{
				crossgameprotocal::GameCrossMessBattleInfoSync gcmbi;
				gcmbi.unique_user_id = m_total_score_rank_vec[i].user_key;
				gcmbi.rank = i + 1;
				InternalComm::Instance().SendToCross((const char*)&gcmbi, sizeof(gcmbi));
			}
		}

		if (m_total_score_rank_vec.size() == 1)
		{
			int lenth = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mess_battle_rank_top1, m_total_score_rank_vec[0].user_name);
			if (lenth > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_total_score_rank_vec.size() == 2)
		{
			int lenth = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mess_battle_rank_top2,
				m_total_score_rank_vec[0].user_name, m_total_score_rank_vec[1].user_name);
			if (lenth > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
		else if (m_total_score_rank_vec.size() >= 3)
		{
			int lenth = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mess_battle_rank_top3, m_total_score_rank_vec[0].user_name,
				m_total_score_rank_vec[1].user_name, m_total_score_rank_vec[2].user_name);
			if (lenth > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
}