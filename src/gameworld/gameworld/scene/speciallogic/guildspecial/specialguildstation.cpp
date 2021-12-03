#include "specialguildstation.hpp"

#include "engineadapter.h"
#include "world.h"
#include "gstr.h"

#include "globalconfig/globalconfig.h"
#include "obj/character/monster.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"
#include "internalcomm.h"
#include "obj/character/role.h"
#include "monster/drop/droppool.hpp"
#include "gamecommon.h"
#include "other/route/guildroute.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "servercommon/noticenum.h"

#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "monster/monsterpool.h"
#include "servercommon/errornum.h"

#include "gameworld/protocal/msgfight.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "servercommon/activitydef.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/roleguild.hpp"
#include "item/knapsack.h"
#include "other/mount/mountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/shop/roleshop.hpp"

#include <algorithm>
#include "other/itemlevelreward/itemlevelreward.hpp"

SpecialGuildStation::SpecialGuildStation(Scene *scene)
	: SpecialLogic(scene), m_call_boss_uid(0), m_boss_level(0), m_boss_id(0), m_boss_objid(INVALID_OBJ_ID), m_is_super_boss(false), m_is_notice_boss_timelimit(false), m_boss_dayid(0),
	m_next_bless_time(0), m_next_update_rank_time(0), m_next_change_star_time(0), m_cur_star_level(0),
	m_tianci_tongbi_is_open(false), m_tianci_tongbi_open_time(0), m_tianci_tongbi_close_time(0), m_tianci_tongbi_tree_maturity_degree(0),
	m_tianci_tongbi_readiness_time(0), m_tianci_tongbi_npc_pos(0, 0), m_gather_num(0), m_next_tianci_tongbi_check_gather_time(0)
{

}

SpecialGuildStation::~SpecialGuildStation()
{

}

void SpecialGuildStation::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_boss_dayid != EngineAdapter::Instance().DayID())
	{
		m_boss_dayid = EngineAdapter::Instance().DayID();
	}

	if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GUILD_BOSS) && (m_boss_id != 0 || !m_gathar_objid_list.empty()))
	{
		this->ResetGuildBoss();
	}

	if (now_second >= m_next_bless_time)
	{
		m_next_bless_time = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().totem_bless_interval;

		this->TotemBless();
	}

	if (!m_tianci_tongbi_is_open && ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_BOSS))
	{
		// 刷新排行榜
		if (m_next_update_rank_time > 0 && now_second >= m_next_update_rank_time)
		{
			m_next_update_rank_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().rank_update_interval_s;
			this->SortHurtRank();
			this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_RANK);
		}

		// 检查星级
		this->CheckUpdateStarLevel((unsigned int)now_second);

		// 检查剩余时间传闻
		this->CheckNoticeBossTimeLimit(now_second);
	}

	// 天赐铜币
	if (m_tianci_tongbi_is_open)
	{
		if (0 != m_tianci_tongbi_open_time && now_second >= m_tianci_tongbi_open_time)
		{
			m_tianci_tongbi_open_time = 0;
			m_gather_num = 0;

			this->TianCiTongBiRefreshGather();
			this->SendTianCiTongBiRoleGatherInfo(0);
		}

		if (0 != m_tianci_tongbi_close_time && now_second >= m_tianci_tongbi_close_time)
		{
			this->OnTianCiTongBiClose();
		}
		//else
		//{
		//	if (0 != m_next_tianci_tongbi_check_gather_time && now_second >= m_next_tianci_tongbi_check_gather_time)
		//	{
		//		this->TianCiTongBiCheckGather();
		//		m_next_tianci_tongbi_check_gather_time += 1;
		//	}

		//	if (0 != m_next_refresh_tianci_tongbi_rank_time && now_second >= m_next_refresh_tianci_tongbi_rank_time)
		//	{
		//		//this->RefreshTianCiTongBiRank();
		//		m_next_refresh_tianci_tongbi_rank_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 2;
		//	}
		//}
	}
}

void SpecialGuildStation::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster *)cha;
		if (monster->GetMonsterId() == m_boss_id && monster->GetId() == m_boss_objid)
		{
			Obj *obj = m_scene->GetObj(enemy_id);
			if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *role = (Role *)obj;

				if (m_boss_hurt_uid_list.find(role->GetUID()) == m_boss_hurt_uid_list.end())
				{
					ROLE_LOG_QUICK6(LOG_TYPE_GUILD_BOSS, role, role->GetLevel(), m_boss_id, "Join", NULL);
				}
				m_boss_hurt_uid_list[role->GetUID()] = role->GetRoleGuild()->GetLastLeaveGuildTime();

				// 计算玩家伤害值
				this->CalcRoleHurtBossVal(role, delta_hp);
			}
		}
	}
}

void SpecialGuildStation::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;
		if (monster->GetMonsterId() == m_boss_id && monster->GetId() == m_boss_objid)
		{
			this->GuildBossReward();
			this->GiveRankReward();
			this->CreateBossGather();

			m_call_boss_uid = 0;
			m_boss_id = 0;
			m_boss_objid = INVALID_OBJ_ID;
			m_next_change_star_time = 0;

			m_boss_hurt_uid_list.clear();

			this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_SCENE);

			// 加公会图腾经验
			Obj* killer_obj = m_scene->GetObj(killer_id);
			if (NULL != killer_obj)
			{
				if (Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
				{
					Role *killer_role = (Role*)killer_obj;
					
					Guild *guild = GuildManager::Instance().GetGuild(killer_role->GetUserId());
					if (NULL != guild)
					{
						const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(m_boss_level);
						if (NULL != boss_cfg)
						{
							guild->AddTotemExp(boss_cfg->totem_exp_reward);
							EventHandler::Instance().OnKillGuildBoss(killer_role);
						}
					}
				}
			}
		}
	}
}

void SpecialGuildStation::OnRoleEnterScene(Role *role)
{
	if (0 != m_boss_id)
	{
		this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_SCENE, role);

		Guild * guild = GuildManager::Instance().GetGuild(m_scene->GetSceneKey());
		if (NULL != guild)
		{
			guild->SyncGuildBossInfo(role->GetNetId());
		}
	}

	if(!m_tianci_tongbi_is_open)
	{
		// 添加玩家
		PlayerInfoMapIt it = m_role_info_map.find(role->GetUID());
		if (it == m_role_info_map.end())
		{
			//玩家信息
			RoleFbInfo info;
			info.gather_num = 0;
			m_role_info_map[role->GetUID()] = info;

			//伤害boss信息
			HurtBossInfo hurt_info;
			hurt_info.role_id = role->GetUID();
			role->GetName(hurt_info.user_name);
			m_hurt_boss_info_vec.push_back(hurt_info);
		}
	}

	if (m_tianci_tongbi_is_open)
	{
		if (NULL != role)
		{
			if (m_guild_tianci_tongbi_user_map.find(role->GetUserId()) == m_guild_tianci_tongbi_user_map.end())
			{
				GuildTianCiTongBiUser tianci_tongbi_user;
				tianci_tongbi_user.obj_id = role->GetId();
				tianci_tongbi_user.user_id = role->GetUserId();
				tianci_tongbi_user.reward_mojing_num = 0;
				tianci_tongbi_user.gold_bind_num = 0;
				tianci_tongbi_user.total_gather_exp_count = 0;
				tianci_tongbi_user.user_level = role->GetLevel();
				tianci_tongbi_user.user_camp = role->GetCamp();
				role->GetName(tianci_tongbi_user.user_name);

				m_guild_tianci_tongbi_user_map[role->GetUserId()] = tianci_tongbi_user;
			}
		}

		this->SendGuildTianCiTongBiNpcInfo(role);
		this->RefreshTianCiTongBiRank();

		if (m_tianci_tongbi_big_coin_objid_set.empty())
		{
			this->SendTianCiTongBiRoleGatherInfo(0);
		}
		else
		{
			this->SendTianCiTongBiRoleGatherInfo(1);
		}
	}
}

void SpecialGuildStation::OnRoleLeaveScene(Role *role, bool is_logout)
{
	//玩家掉线或退出该场景，帮玩家提交
	this->OnTianCiTongBiUseGather(role, true);
}

void SpecialGuildStation::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		return;
	}

	if (m_gathar_objid_list.find(obj_id) != m_gathar_objid_list.end())
	{
		m_gathar_objid_list.erase(obj_id);

// 		GatherObj *gather_obj = m_scene->GetGatherObj(obj_id);
// 		if (NULL != gather_obj)
// 		{
//			role->GetKnapsack()->GetMoney()->AddGoldBind(gather_obj->GetData(), "GuildBossGather");
//
//		}

		// 增加采集次数
		PlayerInfoMapIt iter = m_role_info_map.find(role->GetUID());
		if (iter != m_role_info_map.end())
		{
			iter->second.gather_num += 1;
		}
		else
		{
			RoleFbInfo info;
			info.gather_num = 1;
			m_role_info_map[role->GetUID()] = info;
		}

		const GuildBossStarLevelCfg *star_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetStarLevelCfg(m_cur_star_level);
		if (nullptr != star_cfg)
		{
			role->GetKnapsack()->PutListOrMail(star_cfg->reward_item_list, PUT_REASON_GUILD_BOSS_GATHER);
		}

		this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_ROLE_INFO_CHANGE, role);
	}

	// 天赐铜币
	if (m_tianci_tongbi_is_open && m_tianci_tongbi_big_coin_objid_set.empty())
	{
		for (int i = 0; i < GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX; i++)
		{
			if (obj_id == m_tianci_tongbi_gather_list[i].obj_id)
			{
				m_tianci_tongbi_gather_list[i].obj_id = INVALID_OBJ_ID;
				m_tianci_tongbi_gather_list[i].next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 2;
				break;
			}
		}

		int gather_type = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiGatherType(gather_id);
		if (GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID < gather_type && gather_type < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX)
		{
			GuildTianCiTongBiUser *tianci_tongbi_user = this->GetGuildTianCiTongBiUser(role->GetUserId());
			if (NULL != tianci_tongbi_user)
			{
				++tianci_tongbi_user->gather_count_list[gather_type];
			}

			--m_gather_num;
			this->TianCiTongBiRoleGatherChange(role, gather_type);

			role->GetMountManager()->GoonMount(false, true, true);

			//flag 用来判断当前采集物 默认0是增加成熟度的采集物 1是大铜币 (为了区分两者下发数量)
			this->SendTianCiTongBiRoleGatherInfo(0);

			role->NoticeNum(noticenum::NT_TIANCI_TONGBI_GATHER_SUCC);
		}
	}
	else if(m_tianci_tongbi_is_open)
	{
		this->OnRoleGatherBigCoin(role, dynamic_cast<GatherObj *>(m_scene->GetObj(obj_id)));
	}
}

bool SpecialGuildStation::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	if (!m_tianci_tongbi_is_open)
	{
		const GuildBossStarLevelCfg *star_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetStarLevelCfg(m_cur_star_level);
		if (nullptr == star_cfg)
		{
			return false;
		}

		if (star_cfg->gather_id != gather->GatherId())
		{
			return false;
		}

		PlayerInfoMapIt iter = m_role_info_map.find(role->GetUID());
		if (iter != m_role_info_map.end())
		{
			if (iter->second.gather_num >= star_cfg->limit_num)
			{
				role->NoticeNum(errornum::EN_HUNYAN_GATHAR_TOO_MUCH);
				return false;
			}
		}
	}
	else
	{
		// 天赐铜币
//		int gather_type = role->GetRoleActivity()->GetGuildTianCiTongBiGatherType();
		if (!role->GetRoleGuild()->IsCanJoinTianCiTongBi())
		{
			gstr::SendError(role, "already_join_tiancitongbi");
			return false;
		}

		if (SPECIAL_GATHER_TYPE_BIGCOIN == gather->GetSpecialGatherType())
		{
			if (role->GetRoleGuild()->GetGuilaTianCiTongBiRewardLimit() >= LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiCoinExpCountLimit())
			{
				gstr::SendError(role, "en_guild_tianci_tongbi_daily_limit");
				return false;
			}
		}

		auto it = m_guild_xianci_tongbi_gather_type_map.find(role->GetUID());
		if (it != m_guild_xianci_tongbi_gather_type_map.end())
		{
			if (it->second > GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID && it->second < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX)
			{
				gstr::SendError(role, "en_guild_tainci_tongbi_already_gather");
				return false;
			}
		}

		role->GetRoleGuild()->SetGuilaTianCiTongBiJoinGuild(role->GetGuildID());
	}

	return true;
}

void SpecialGuildStation::OnAOIRoleEnter(Role *role, Role *target_role)
{
	if (NULL == role || NULL == target_role)
	{
		return;
	}

	Protocol::SCGuildTianCiTongBiGatherAOIInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.gather_type = 0;

	auto it = m_guild_xianci_tongbi_gather_type_map.find(role->GetUID());
	if (it != m_guild_xianci_tongbi_gather_type_map.end())
	{
		cmd.gather_type = it->second;
	}

	EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&cmd, sizeof(cmd));
}

bool SpecialGuildStation::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}
	else
	{
		return SpecialLogic::SpecialIsEnemy(role, obj);
	}
}

bool SpecialGuildStation::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return true;
	}
	else
	{
		return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
	}
}

bool SpecialGuildStation::SpecialRelive(Role *role, int realive_type)
{
	return SpecialLogic::SpecialRelive(role, realive_type);
}

bool SpecialGuildStation::CallGuildBoss(Role *role, int boss_level, bool is_super_boss)
{
	if (role == nullptr) return false;

	const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(boss_level);
	if (NULL == boss_cfg)
	{
		return false;
	}

	Monster *monster = MONSTERPOOL->CreateMonster(boss_cfg->boss_id, m_scene, boss_cfg->pos);
	if (NULL != monster)
	{
		m_call_boss_uid = role->GetUID();
		m_boss_level = boss_level;
		m_boss_id = boss_cfg->boss_id;
		m_boss_objid = monster->GetId();
		m_is_super_boss = is_super_boss;
		m_is_notice_boss_timelimit = false;

		{
			//开始计算星级、排行榜
			const GuildBossStarLevelCfg *star_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetStarLevelCfg(GUILD_BOSS_STAR_LEVEL_COUNT);
			if (nullptr != star_cfg)
			{
				m_next_change_star_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + star_cfg->cost_time_s;
				m_cur_star_level = GUILD_BOSS_STAR_LEVEL_COUNT;
			}

			m_next_update_rank_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().rank_update_interval_s;
		}

		this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_STAR_LEVEL_CHANGE);
		return true;
	}

	return false;
}

void SpecialGuildStation::ResetGuildBoss()
{
	if (INVALID_OBJ_ID != m_boss_objid)
	{
		Obj *obj = m_scene->GetObj(m_boss_objid);
		if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster *monster = (Monster *)obj;
			if (monster->GetMonsterId() == m_boss_id)
			{
				monster->ForceSetDead();
			}
		}
	}

	m_call_boss_uid = 0;
	m_boss_id = 0;
	m_boss_objid = INVALID_OBJ_ID;

	this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_SCENE);

	for (std::set<ObjID>::iterator it = m_gathar_objid_list.begin(); it != m_gathar_objid_list.end(); it++)
	{
		m_scene->DeleteObj(*it);
	}
	m_gathar_objid_list.clear();

	m_boss_hurt_uid_list.clear();

	m_next_update_rank_time = 0;
}

void SpecialGuildStation::CheckUpdateStarLevel(unsigned int now_second)
{
	if (m_next_change_star_time <= 0 || now_second < m_next_change_star_time)
	{
		return;
	}

	int star_level = m_cur_star_level - 1;
	const GuildBossStarLevelCfg *star_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetStarLevelCfg(star_level);
	if (nullptr == star_cfg)
	{
		return;
	}

	if (0 == star_cfg->cost_time_s)
	{
		m_next_change_star_time = 0;
		m_cur_star_level = star_level;
	}
	else
	{
		m_next_change_star_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + star_cfg->cost_time_s;
		m_cur_star_level = star_level;
	}

	this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_STAR_LEVEL_CHANGE);
}

void SpecialGuildStation::CalcRoleHurtBossVal(Role *role, Attribute delta_hp)
{
	if (nullptr == role)
	{
		return;
	}

	int role_id = role->GetUID();
	auto it = std::find_if(m_hurt_boss_info_vec.begin(), m_hurt_boss_info_vec.end(), [role_id](const HurtBossInfo &cmp_item) {
		if (role_id == cmp_item.role_id)
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
	}
	else
	{
		HurtBossInfo hurt_info;
		hurt_info.role_id = role->GetUID();
		role->GetName(hurt_info.user_name);
		hurt_info.hurt_val += -delta_hp;
		m_hurt_boss_info_vec.emplace_back(hurt_info);
	}

	this->SortHurtRank();
}

void SpecialGuildStation::CheckNoticeBossTimeLimit(time_t now_second)
{
	UInt32 activity_end_time = ActivityShadow::Instance().GetActivityEndTime(ACTIVITY_TYPE_GUILD_BOSS);
	int notice_left_min = LOGIC_CONFIG->GetGuildActivityCfg().GetBossNoticeLeftMin();
	UInt32 notice_left_time = notice_left_min * SECOND_PER_MIN;

	if (now_second + notice_left_time >= activity_end_time && activity_end_time > 0 && !m_is_notice_boss_timelimit)
	{
		int sendlen = 0;

		if (m_boss_id != 0)
		{
			sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_boss_kill_boss_timelimit", notice_left_min);
		}
		else if (!m_gathar_objid_list.empty())
		{
			sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guild_boss_gather_timelimit", notice_left_min);
		}

		if (sendlen > 0)
		{
			this->NotifySystemMsg(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);

			m_is_notice_boss_timelimit = true;
		}	
	}
}

void SpecialGuildStation::SendGuildBossInfo(int notify_type, Role *role)
{
	Protocol::SCGuildBossActivityInfo info;
	info.notify_type = notify_type;
	info.boss_id = m_boss_id;
	info.boss_level = m_boss_level;
	info.boss_objid = m_boss_objid;
	info.is_super_boss = m_is_super_boss ? 1 : 0;
	info.totem_totem_exp = 0; 
	info.cur_star_level = m_cur_star_level;
	info.gather_num = 0;
	info.next_change_star_time = m_next_change_star_time;

	info.rank_count = 0;
	memset(info.rank_info_list, 0, sizeof(info.rank_info_list));

	for (std::vector<HurtBossInfo>::const_iterator it = m_hurt_boss_info_vec.begin(); it != m_hurt_boss_info_vec.end() && info.rank_count < Protocol::SCGuildBossActivityInfo::RANK_ITEM_MAX_COUNT; ++it)
	{
		if (it->hurt_val <= 0)
		{
			continue;
		}

		info.rank_info_list[info.rank_count].user_id = it->role_id;
		memcpy(info.rank_info_list[info.rank_count].user_name, it->user_name, sizeof(info.rank_info_list->user_name));
		info.rank_info_list[info.rank_count].hurt_val = it->hurt_val;

		info.rank_count++;
	}

	if (NULL != role)
	{
		info.totem_totem_exp = role->GetRoleGuild()->GetTotemExp();
		PlayerInfoMapIt iter = m_role_info_map.find(role->GetUID());
		if (iter != m_role_info_map.end())
		{
			info.gather_num = iter->second.gather_num;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, sizeof(info));
	}
	else
	{
		for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
		{
			Role * temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				info.totem_totem_exp = temp_role->GetRoleGuild()->GetTotemExp();
				PlayerInfoMapIt iter = m_role_info_map.find(temp_role->GetUID());
				if (iter != m_role_info_map.end())
				{
					info.gather_num = iter->second.gather_num;
				}

				EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&info, sizeof(info));
			}
		}
	}

}

void SpecialGuildStation::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialGuildStation::OnRoleGatherBigCoin(Role *role, GatherObj *bigcoin)
{
	if (NULL == role || NULL == bigcoin || !m_tianci_tongbi_is_open) return;

	if (SPECIAL_GATHER_TYPE_BIGCOIN == bigcoin->GetSpecialGatherType())
	{
		GuildTianCiTongBiUser *tianci_tongbi_user = this->GetGuildTianCiTongBiUser(role->GetUserId());
		if (NULL != tianci_tongbi_user)
		{
			// 获得绑元
// 			if (role->GetRoleGuild()->GetGuilaTianCiTongBiRewardLimit() >= LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiAcquireGoldBindLimit())
// 			{
// 				gstr::SendError(role, "en_guild_tianci_tongbi_daily_limit");
// 			}
// 			else
// 			{
// 				int reward_num = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiCoinRewardInfo();
// 				tianci_tongbi_user->gold_bind_num += reward_num;
// 
// 				role->NoticeNum(noticenum::NT_TIANCI_TONGBI_GATHER_SUCC);
// 
// 				Money *money = role->GetKnapsack()->GetMoney();
// 				if (nullptr == money)
// 				{
// 					return;
// 				}
// 
// 				this->RefreshTianCiTongBiRank();
// 				money->AddGoldBind(reward_num, "GuildTianCiTongBi");
// 
// 				role->GetRoleGuild()->AddGuilaTianCiTongBiRewardLimit(reward_num);
// 			}

			// 获得经验
			if (role->GetRoleGuild()->GetGuilaTianCiTongBiRewardLimit() < LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiCoinExpCountLimit())
			{
				//int reward_exp = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiCoinRewardExp(role->GetLevel());

				//策划需求，修改增加经验规则（2018-1-3）
				long long reward_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

				role->AddExp(reward_exp, "OnRoleGatherBigCoin", Role::EXP_ADD_TIANCITONGBI);

				tianci_tongbi_user->total_gather_exp_count += 1;
				tianci_tongbi_user->total_exp += reward_exp;
				role->GetRoleGuild()->AddGuilaTianCiTongBiRewardLimit(1);

				this->RefreshTianCiTongBiRank();

				role->NoticeNum(noticenum::NT_TIANCI_TONGBI_GATHER_SUCC);
			}

			//int dayid = EngineAdapter::Instance().DayID();
			//if (0 == role->GetDayCounter()->GetDayCount(dayid))
			//{
			//	role->GetDayCounter()->DayCountIncrease(dayid);
			//}
		}

		m_tianci_tongbi_big_coin_objid_set.erase(bigcoin->GetId());
		this->SendTianCiTongBiRoleGatherInfo(1);

		if (m_tianci_tongbi_big_coin_objid_set.empty())
		{
			m_gather_num = 0;
			this->TianCiTongBiRefreshGather();
		}
	}
}

void SpecialGuildStation::BroadcastGatherInfo(Role *role, int gather_type)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCGuildTianCiTongBiGatherAOIInfo cmd;
	cmd.obj_id = role->GetId();
	cmd.gather_type = gather_type;
	

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}

void SpecialGuildStation::OnOpenTianCiTongBi()
{
	if (!m_tianci_tongbi_is_open)
	{
		this->OnTianCiTongBiOpen();
	}
}

void SpecialGuildStation::ForceTianCiTongBiNextState()
{
	if (m_tianci_tongbi_is_open)
	{
		m_tianci_tongbi_close_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
	else
	{
		this->OnTianCiTongBiOpen();
	}
}

void SpecialGuildStation::OnTianCiTongBiUseGather(Role *role, bool is_auto)
{
	if (NULL == role || !m_tianci_tongbi_is_open) return;

//	int gather_type = role->GetRoleActivity()->GetGuildTianCiTongBiGatherType();

	auto it = m_guild_xianci_tongbi_gather_type_map.find(role->GetUID());
	if (it == m_guild_xianci_tongbi_gather_type_map.end())
	{
		if (!is_auto)
		{
			gstr::SendError(role, "en_guild_tianci_tongbi_commit_error");
		}
		return;
	}

	if (it->second <= GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID || it->second >= GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX)
	{
		if (!is_auto)
		{
			gstr::SendError(role, "en_guild_tianci_tongbi_commit_error");
		}
		return;
	}

	int MAX_TOUCH_DISTANCE = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiNpcMaxTouchDistance();

	// 检查与npc距离，自动提交时无需检查
	if (!is_auto && (m_tianci_tongbi_npc_pos - role->GetPos()).IsLongerThan(MAX_TOUCH_DISTANCE)) return;

	if (const TianCiTongBiGather *tctbg = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiGatherReward(it->second))
	{
		GuildTianCiTongBiUser *tianci_tongbi_user = this->GetGuildTianCiTongBiUser(role->GetUserId());
		if (NULL != tianci_tongbi_user)
		{
			tianci_tongbi_user->reward_mojing_num += tctbg->reward_mojing_num;
			m_tianci_tongbi_tree_maturity_degree += tctbg->reward_maturity;

			this->TianCiTongBiRoleGatherChange(role, GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID);

			m_guild_xianci_tongbi_gather_type_map[role->GetUID()] = GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID;

			gstr::SendNotice(role, "nt_tianci_tongbi_use_gather_succ");

			role->GetRoleShop()->AddChestShopMojing(tctbg->reward_mojing_num, "TianCiTongBi");

			//int dayid = EngineAdapter::Instance().DayID();
			//if (0 == role->GetDayCounter()->GetDayCount(dayid))
			//{
			//	role->GetDayCounter()->DayCountIncrease(dayid);
			//}
		}
	}

	if (m_tianci_tongbi_tree_maturity_degree >= LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiFlushBigCoinNeedMaturity())
	{
		m_tianci_tongbi_tree_maturity_degree = 0;
		m_gather_num = 0;
		this->TianCiTongBiRefreshBigCoin();
	}
	else
	{
		this->SendTianCiTongBiRoleGatherInfo(0);
	}

	this->RefreshTianCiTongBiRank();
}

void SpecialGuildStation::SendTianCiTongBiRankInfo(Role *role)
{
	static Protocol::SCGuildTianCiTongBiRankInfo gtctbri;
	gtctbri.tianci_tongbi_close_time = m_tianci_tongbi_close_time;
	gtctbri.rank_count = 0;
	gtctbri.reserver = 0;

	int i = 1;
	for (GuildTianCiTongBiRankItemVecIt it = m_guild_tianci_tongbi_rank_vec.begin(); it != m_guild_tianci_tongbi_rank_vec.end(); ++it, ++i)
	{
		if (gtctbri.rank_count >= GUILD_TIANCI_TONGBI_RANK_NUM) break;

		it->rank_info = i;
		gtctbri.rank_item_list[gtctbri.rank_count] = *it;
		++gtctbri.rank_count;
	}

	int sendlen = sizeof(gtctbri) - (GUILD_TIANCI_TONGBI_RANK_NUM - gtctbri.rank_count) * sizeof(gtctbri.rank_item_list[0]);

	if (NULL == role)
	{
		m_scene->SendToRole((const char*)&gtctbri, sendlen);
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gtctbri, sendlen);
	}
}

void SpecialGuildStation::AddTreeMaturity(int maturity)
{
	m_tianci_tongbi_tree_maturity_degree += maturity;

	if (m_tianci_tongbi_tree_maturity_degree >= LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiFlushBigCoinNeedMaturity())
	{
		m_tianci_tongbi_tree_maturity_degree = 0;
		m_gather_num = 0;
		this->TianCiTongBiRefreshBigCoin();
	}
}

Role * SpecialGuildStation::GetRole(GuildTianCiTongBiUser &dps_user)
{
	Obj *obj = m_scene->GetObj(dps_user.obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		if (role->GetUserId() == dps_user.user_id)
		{
			return role;
		}
	}

	return NULL;
}

GuildTianCiTongBiUser * SpecialGuildStation::GetGuildTianCiTongBiUser(const UserID user_id)
{
	GuildTianCiTongBiUserMapIt it = m_guild_tianci_tongbi_user_map.find(user_id);
	if (it != m_guild_tianci_tongbi_user_map.end())
	{
		return &it->second;
	}

	return NULL;
}

void SpecialGuildStation::OnTianCiTongBiOpen()
{
	m_guild_tianci_tongbi_user_map.clear();
	m_guild_tianci_tongbi_rank_vec.clear();

	m_tianci_tongbi_is_open = true;

	for (int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			GuildTianCiTongBiUser tianci_tongbi_user;
			tianci_tongbi_user.obj_id = role->GetId();
			tianci_tongbi_user.user_id = role->GetUserId();
			tianci_tongbi_user.reward_mojing_num = 0;
			tianci_tongbi_user.gold_bind_num = 0;
			tianci_tongbi_user.total_gather_exp_count = 0;
			tianci_tongbi_user.user_level = role->GetLevel();
			tianci_tongbi_user.user_camp = role->GetCamp();
			role->GetName(tianci_tongbi_user.user_name);

			m_guild_tianci_tongbi_user_map[role->GetUserId()] = tianci_tongbi_user;
		}
	}

	m_tianci_tongbi_open_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiStandyTime();
	m_tianci_tongbi_close_time = m_tianci_tongbi_open_time + LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiLastTime();
	m_tianci_tongbi_tree_maturity_degree = 0;
	//m_next_refresh_tianci_tongbi_rank_time = m_tianci_tongbi_open_time + 1;
	m_next_tianci_tongbi_check_gather_time = m_tianci_tongbi_open_time + 1;
	m_tianci_tongbi_readiness_time = m_tianci_tongbi_open_time - static_cast<unsigned int>(EngineAdapter::Instance().Time());
	LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTreeNpcInfo(m_tianci_tongbi_npc_pos);

	this->SendTianCiTongBiRankInfo();
	this->SendGuildTianCiTongBiNpcInfo();
}

void SpecialGuildStation::OnTianCiTongBiClose()
{
	this->RefreshTianCiTongBiRank();
	this->ClearTianCiTongBiGahter();
	this->ClearTianCiTongBiBigCoin();

	m_tianci_tongbi_is_open = false;

	static Protocol::SCGuildTianCiTongBiResult gtctbr;

	int rank_pos = 0;
	for (GuildTianCiTongBiRankItemVecIt it = m_guild_tianci_tongbi_rank_vec.begin(); it != m_guild_tianci_tongbi_rank_vec.end(); ++it)
	{
		GuildTianCiTongBiUser *user = this->GetGuildTianCiTongBiUser(IntToUid((*it).uid));
		if (nullptr == user)
		{
			return;
		}

		{
			Role *role = this->GetRole(*user);
			if (NULL != role)
			{
				gtctbr.rank_pos = rank_pos + 1;
				gtctbr.reward_mojing_num = user->reward_mojing_num;
				gtctbr.gold_bind_num = user->gold_bind_num;
				gtctbr.total_exp = user->total_exp;

				memcpy(gtctbr.gather_count_list, user->gather_count_list, sizeof(gtctbr.gather_count_list));

				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gtctbr, sizeof(gtctbr));
			}

			if (0 == rank_pos)
			{
				GuildName guild_name = { 0 };
				GuildShadow::Instance().GetGuildName(m_scene->GetSceneKey(), guild_name);

				int len = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_tiancitongbi_close_notice",
					guild_name, UidToInt(user->user_id), user->user_name, user->user_camp);
				if (len > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
		}

		++rank_pos;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->TianCiTongBiRoleGatherChange(role, GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID);
		}
	}

	//m_scene->DeleteObj(m_tree_id);
	//m_tree_id = INVALID_OBJ_ID;

	m_gather_num = 0;

	//Protocol::SCGuildSyncTianCiTongBi gsyctb;
	//gsyctb.guild_id = m_scene->GetSceneKey();
	//gsyctb.is_open = 0;
	//InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char *)&gsyctb, sizeof(gsyctb));

	Guild *guild = GuildManager::Instance().GetGuild(m_scene->GetSceneKey());
	if (nullptr == guild)
	{
		return;
	}

	guild->SetTianCiTongBiOpentatus(false);

}

void SpecialGuildStation::TianCiTongBiCheckGather()
{
	//unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	//for (int i = 0; i < GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX; i++)
	//{
	//	if (INVALID_OBJ_ID == m_tianci_tongbi_gather_list[i].obj_id && 0 != m_tianci_tongbi_gather_list[i].next_refresh_time && now >= m_tianci_tongbi_gather_list[i].next_refresh_time)
	//	{
	//		int gather_id = LOGIC_CONFIG->GetGuildActivityCfg().RandTianCiTongBiGatherId();
	//		Posi gather_pos(0, 0); int pos_range = 0;
	//		if (LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiGatherPos(gather_pos.x, gather_pos.y, pos_range))
	//		{
	//			if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, gather_pos.x, gather_pos.y))
	//			{
	//				GatherObj *gather_obj = new GatherObj();
	//				gather_obj->Init(NULL, gather_id, 3000, 0, true);
	//				gather_obj->SetPos(gather_pos);
	//				m_scene->AddObj(gather_obj);
	//
	//				m_tianci_tongbi_gather_list[i].obj_id = gather_obj->GetId();
	//				m_tianci_tongbi_gather_list[i].next_refresh_time = 0;
	//			}
	//		}
	//	}
	//}

	//this->RefreshTianCiTongBiRank();
}

void SpecialGuildStation::TianCiTongBiRefreshGather()
{
	int gather_pos_num;

	TianCiTongBiGatherRefreshPos *gather_pos_list = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiGatherPos(gather_pos_num);
	if (nullptr == gather_pos_list)
	{
		return;
	}

	for (int i = 0; i < gather_pos_num && i < GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX; i++)
	{
		//int gather_id = LOGIC_CONFIG->GetGuildActivityCfg().RandTianCiTongBiGatherId();
		//int pos_range = 0;
		//
		//for (int loop_times = 0; loop_times < 50; loop_times++)
		//{
		//	gather_pos = gamecommon::GetDisRandPos(gather_pos, pos_range);
		//
		//	if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, gather_pos.x, gather_pos.y))
		//	{
		//		break;
		//	}
		//}

		Posi gather_pos(0, 0);
		gather_pos.x = gather_pos_list[i].pos_x;
		gather_pos.y = gather_pos_list[i].pos_y;

		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, gather_pos_list[i].gather_id, 3000, 0, true);
		gather_obj->SetPos(gather_pos);
		m_scene->AddObj(gather_obj);

		m_tianci_tongbi_gather_list[i].obj_id = gather_obj->GetId();
		m_tianci_tongbi_gather_list[i].next_refresh_time = 0;
		++m_gather_num;

	}

	this->SendTianCiTongBiRoleGatherInfo(0);
}

void SpecialGuildStation::TianCiTongBiRefreshBigCoin()
{
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				this->TianCiTongBiRoleGatherChange(role, GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID);
			}
		}
	}

	this->ClearTianCiTongBiGahter();

	int gather_id = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiCoinGatherId();
	int disappear_time = LOGIC_CONFIG->GetGuildActivityCfg().GetCoinGatherDisappearTime();

	const int TIANCI_TONGBI_FLUSH_BIGCOIN_NUM = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiFlushBigCoinNum();

	if (TIANCI_TONGBI_FLUSH_BIGCOIN_NUM <= 0)
	{
		return;
	}

	for (int i = 0; i < TIANCI_TONGBI_FLUSH_BIGCOIN_NUM; i++)
	{
		Posi init_bigcoin_pos(0, 0); int pos_range = 0;
		LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiBigCoinPos(init_bigcoin_pos.x, init_bigcoin_pos.y, pos_range);

		Posi rand_pos = init_bigcoin_pos;
		rand_pos = gamecommon::GetValidRandPos(init_bigcoin_pos, pos_range, m_scene);

		GatherObj *bigcoin_obj = new GatherObj();
		bigcoin_obj->Init(NULL, gather_id, 5000, 0, true, disappear_time);
		bigcoin_obj->SetPos(rand_pos);
		bigcoin_obj->SetSpecialGatherType(SPECIAL_GATHER_TYPE_BIGCOIN);
		m_scene->AddObj(bigcoin_obj);

		m_tianci_tongbi_big_coin_objid_set.insert(bigcoin_obj->GetId());
	}

	this->SendTianCiTongBiRoleGatherInfo(1);
}


void SpecialGuildStation::TianCiTongBiRoleGatherChange(Role * role, int gather_type)
{
	if (nullptr == role)
	{
		return;
	}

	m_guild_xianci_tongbi_gather_type_map[role->GetUID()] = gather_type;

	if (GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID < gather_type && gather_type < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX)
	{
		Attribute movespeed_dec_value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (30 / 41.3f));

		EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, 2, 7200 * 1000, 1, EffectBase::PRODUCT_METHOD_HUSONG);
		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect_buff->SetSave(false);
		effect_buff->AddMoveSpeedEffect(-movespeed_dec_value, -100);
		role->AddEffect(effect_buff);
	}
	else
	{
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_HUSONG, 2);
	}

	// 广播
	this->BroadcastGatherInfo(role, gather_type);
}

bool static TianCiTongBiSortFunc(const GuildTianCiTongBiRankItem &rank_item_1, const GuildTianCiTongBiRankItem &rank_item_2)
{
// 	if (rank_item_1.coin_bind == rank_item_2.coin_bind)
// 	{
		return rank_item_1.reward_mojing_num > rank_item_2.reward_mojing_num;
//	}

//	return rank_item_1.coin_bind > rank_item_2.coin_bind;
}

void SpecialGuildStation::RefreshTianCiTongBiRank(Role *role)
{
	m_guild_tianci_tongbi_rank_vec.clear();
	m_guild_tianci_tongbi_rank_vec.reserve(m_guild_tianci_tongbi_user_map.size());

	for (GuildTianCiTongBiUserMapIt it = m_guild_tianci_tongbi_user_map.begin(); it != m_guild_tianci_tongbi_user_map.end(); ++it)
	{

		GuildTianCiTongBiRankItem rank_item;
		rank_item.uid = UidToInt(it->first);
		rank_item.reward_mojing_num = it->second.reward_mojing_num;
		rank_item.coin_bind = it->second.gold_bind_num;
		rank_item.total_gather_exp_count = it->second.total_gather_exp_count;
		F_STRNCPY(rank_item.user_name, it->second.user_name, sizeof(GameName));

		m_guild_tianci_tongbi_rank_vec.push_back(rank_item);
	}

	std::sort(m_guild_tianci_tongbi_rank_vec.begin(), m_guild_tianci_tongbi_rank_vec.end(), &TianCiTongBiSortFunc);

	this->SendTianCiTongBiRankInfo(role);
}

void SpecialGuildStation::ClearTianCiTongBiGahter()
{
	for (int i = 0; i < GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX; i++)
	{
		Obj *obj = m_scene->GetObj(m_tianci_tongbi_gather_list[i].obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(obj->GetId());
		}

		m_tianci_tongbi_gather_list[i].obj_id = INVALID_OBJ_ID;
		m_tianci_tongbi_gather_list[i].next_refresh_time = 0;
	}
}

void SpecialGuildStation::ClearTianCiTongBiBigCoin()
{
	for (std::set<ObjID>::iterator it = m_tianci_tongbi_big_coin_objid_set.begin(); it != m_tianci_tongbi_big_coin_objid_set.end(); ++it)
	{
		Obj *obj = m_scene->GetObj(*it);
		if (nullptr != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(obj->GetId());
		}
	}

	m_tianci_tongbi_big_coin_objid_set.clear();
}

void SpecialGuildStation::SendGuildTianCiTongBiNpcInfo(Role * role)
{
	Protocol::SCGuildTianCiTongBiNpcInfo scgtctbni;
	scgtctbni.npc_x = m_tianci_tongbi_npc_pos.x;
	scgtctbni.npc_y = m_tianci_tongbi_npc_pos.y;
	scgtctbni.tianci_tongbi_readiness_time = m_tianci_tongbi_readiness_time;

	if (nullptr == role)
	{
		m_scene->SendToRole((const char*)&scgtctbni, sizeof(scgtctbni));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scgtctbni, sizeof(scgtctbni));
	}
}

//flag 用来判断当前采集物 默认0是增加成熟度的采集物 1是大铜币 (为了区分两者下发数量)
void SpecialGuildStation::SendTianCiTongBiRoleGatherInfo(int flag)
{
	Protocol::SCGuildTianCiTongBiUserGatherChange gtctbugc;

	//判断是增加成熟度的采集物 还是 大铜币采集物 以此区分两者下发数量    发送大铜币采集物类型
	if (flag)
	{
		gtctbugc.gather_type = SPECIAL_GATHER_TYPE_BIGCOIN;
		gtctbugc.gather_num = (int)m_tianci_tongbi_big_coin_objid_set.size();
		gtctbugc.tianci_tongbi_max_gather_num = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiFlushBigCoinNum();
	}
	else
	{
		gtctbugc.gather_type = 0;
		gtctbugc.gather_num = m_gather_num;
		gtctbugc.tianci_tongbi_max_gather_num = GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX;
	}

	// 树的成熟度及最大成熟度
	gtctbugc.tianci_tongbi_tree_maturity_degree = m_tianci_tongbi_tree_maturity_degree;
	gtctbugc.tianci_tongbi_tree_max_maturity_degree = LOGIC_CONFIG->GetGuildActivityCfg().GetTianCiTongBiFlushBigCoinNeedMaturity();

	m_scene->SendToRole((const char*)&gtctbugc, sizeof(gtctbugc));
}

void SpecialGuildStation::GuildBossReward()
{
	const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(m_boss_level);
	if (NULL == boss_cfg)
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_scene->GetSceneKey());
	if (NULL == guild)
	{
		return;
	}

	unsigned int time_now = (unsigned int)EngineAdapter::Instance().Time();
	const ItemConfigData &item_data = m_is_super_boss ? boss_cfg->super_item_reward : boss_cfg->normal_item_reward;

	static MailContentParam contentparam; contentparam.Reset();
	const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL != item_base)
	{
		contentparam.item_count = 1;
		contentparam.item_list[0].item_id = item_data.item_id;
		contentparam.item_list[0].num = item_data.num;
		contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guild_boss_reward_content);
		if (length <= 0)
		{
			return;
		}
	}

	for (std::map<int, unsigned int>::iterator it = m_boss_hurt_uid_list.begin(); it != m_boss_hurt_uid_list.end(); it++)
	{
		// 最后离开公会时间不超过1小时不得奖励，防刷
		if (it->second + 3600 >= time_now)
		{
			continue;
		}

		MailRoute::MailToUser(IntToUid(it->first), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}

void SpecialGuildStation::SortHurtRank()
{
	if (m_hurt_boss_info_vec.empty())
	{
		return;
	}

	std::sort(m_hurt_boss_info_vec.begin(), m_hurt_boss_info_vec.end());
}

void SpecialGuildStation::GiveRankReward()
{
	if (m_hurt_boss_info_vec.empty())
	{
		return;
	}

	this->SortHurtRank();

	for (int i = 0; i < static_cast<int>(m_hurt_boss_info_vec.size()); ++i)
	{
		const GuildBossRankRewardCfg * reward_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetRankRewardCfg(i + 1);
		if (NULL == reward_cfg)
		{
			continue;
		}

		// 发送排名奖励
		gamecommon::SendMultiMailToRole(IntToUid(m_hurt_boss_info_vec[i].role_id), reward_cfg->reward_item_list, reward_cfg->reward_count, NULL, false, "guild_boss_hurt_rank_reward", ACTIVITY_TYPE_GUILD_BOSS, i + 1);
	}

	m_hurt_boss_info_vec.clear();
}

void SpecialGuildStation::CreateBossGather()
{
// 	const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(m_boss_level);
// 	if (NULL == boss_cfg)
// 	{
// 		return;
// 	}

	// 清除残留采集物
	if (!m_gathar_objid_list.empty())
	{
		for (std::set<ObjID>::iterator it = m_gathar_objid_list.begin(); it != m_gathar_objid_list.end(); it++)
		{
			m_scene->DeleteObj(*it);
		}
		m_gathar_objid_list.clear();
	}

	const GuildBossStarLevelCfg *star_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetStarLevelCfg(m_cur_star_level);
	if (nullptr == star_cfg)
	{
		return;
	}

	int gather_count = LOGIC_CONFIG->GetGuildActivityCfg().GetBossGatherCount();
	for (int i = 0; i < gather_count && i < 1000; i++)
	{
		const GuildBossGatherConfig *gather_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossGatherCfg(i);
		if (NULL != gather_cfg)
		{
			GatherObj * gather_obj = new GatherObj();
			gather_obj->Init(NULL, star_cfg->gather_id, 2000, 0, true, 3600, 1);
			gather_obj->SetPos(gather_cfg->pos);
//			gather_obj->SetData(boss_cfg->boss_gather_bind_gold);
			m_scene->AddObj(gather_obj);

			m_gathar_objid_list.insert(gather_obj->GetId());
		}
	}

	// 刷新数据
	for (unsigned int i = 0; i < m_scene->RoleNum(); i++)
	{
		Role * temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			PlayerInfoMapIt iter = m_role_info_map.find(temp_role->GetUID());
			if (iter != m_role_info_map.end())
			{
				iter->second.gather_num = 0;

				this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_ROLE_INFO_CHANGE, temp_role);
			}
		}
	}
}

void SpecialGuildStation::TotemBless()
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_BOSS))
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_scene->GetSceneKey());
	if (NULL == guild)
	{
		return;
	}

	const GuildTotemCfg::ConfigItem *totem_cfg = LOGIC_CONFIG->GetGuildConfig().GetTutemConfig(guild->GetTotemLevel());
	if (NULL == totem_cfg)
	{
		return;
	}

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->AddExp(totem_cfg->bless_exp, "TotemBless", Role::EXP_ADD_REASON_DEFAULT);
			role->GetRoleGuild()->AddTotemExp(totem_cfg->bless_exp);
			this->SendGuildBossInfo(Protocol::NOTIFY_INFO_TYPE_ROLE_INFO_CHANGE, role);
		}
	}
}
