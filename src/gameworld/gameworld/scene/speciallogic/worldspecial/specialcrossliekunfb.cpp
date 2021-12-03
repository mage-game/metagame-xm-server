#include "specialcrossliekunfb.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "protocal/msgfight.h"
#include "servercommon/errornum.h"
#include "world.h"
#include "gstr.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/noticenum.h"
#include "internalcomm.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "obj/otherobj/fallingitem.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "gameworld/config/monsterreplaceconfig.hpp"
#include "other/cross/rolecross.h"
#include "gameworld/item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "global/worldstatus/worldstatus.hpp"

SpecialCrossLieKunFb::SpecialCrossLieKunFb(Scene *scene)
	: SpecialLogic(scene), m_first(true), m_activity_open(false), m_cur_zone(-1), 
	m_init_jy_boss_id(false), m_is_enter_main_zone(false), m_enter_close(false), m_enter_close_timestamp(0), m_check_user_time(0)
{
	
}

SpecialCrossLieKunFb::~SpecialCrossLieKunFb()
{
}

void SpecialCrossLieKunFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_activity_open)
	{
		return;
	}

	if (m_first)
	{
		m_cur_zone = CrossLieKunFbConfig::Instance().GetZoneIndexBySceneId(this->GetSceneId());
		if (m_cur_zone < LIEKUN_ZONE_TYPE_MAIN || m_cur_zone >= LIEKUN_ZONE_TYPE_COUNT)
		{
			return;
		}
		m_first = false;

		// 保存bossid
		int boos_id_list[LIEKUN_BOSS_TYPE_COUNT] = { 0 };
		CrossLieKunFbConfig::Instance().GetBossIdList(this->GetSceneId(), boos_id_list);

		const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();
		m_enter_close_timestamp = static_cast<unsigned int>(now_second) + other_cfg.enter_time_limit_s;

		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; i++)
		{
			m_boss_list[i].Reset();
			m_boss_list[i].boss_id = boos_id_list[i];
			if (!this->IsMainZone() && LIEKUN_BOSS_TYPE_MAIN == i)
			{
				m_boss_list[i].boss_next_flush_timestamp = static_cast<unsigned int>(now_second) + other_cfg.refresh_zone_boss_s;
			}
			else if (LIEKUN_BOSS_TYPE_MAIN != i)
			{
				m_boss_list[i].boss_next_flush_timestamp = static_cast<unsigned int>(now_second);
			}
		}
	}

	const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(m_cur_zone);
	if (NULL == zone_cfg)
	{
		gamelog::g_log_activity.printf(LL_ERROR, "SpecialCrossLieKunFb::Update zone[%d] scene[%d] error. Zone config could not be found", 
			(int)m_cur_zone, this->GetSceneId());

		return;
	}

	if (!m_init_jy_boss_id)
	{
		m_init_jy_boss_id = true;

		for (int i = LIEKUN_BOSS_TYPE_JY_1; i < LIEKUN_BOSS_TYPE_COUNT; i++)
		{
			// 根据跨服所有区玩家等级来创建boss
			m_boss_list[i].replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByCrossBossLevel(zone_cfg->boss_id_arr[i]);
		}
	}

	// 创建精英boss
	Monster *jy_boss = NULL;
	for (int i = LIEKUN_BOSS_TYPE_JY_1; i < LIEKUN_BOSS_TYPE_COUNT; i++)
	{
		if (!m_boss_list[i].boss_live_flag && 0 != m_boss_list[i].boss_next_flush_timestamp && now_second >= m_boss_list[i].boss_next_flush_timestamp && m_boss_list[i].replace_boss_id > 0)
		{
			jy_boss = MONSTERPOOL->CreateMonster(m_boss_list[i].replace_boss_id, m_scene, gamecommon::GetDisRandPos(zone_cfg->boss_pos_arr[i], 3));
			if (NULL == jy_boss)
			{
				continue;
			}

			m_boss_list[i].born_pos = zone_cfg->boss_pos_arr[i];
			m_boss_list[i].boss_obj = jy_boss->GetId();
			m_boss_list[i].BossReset();
			this->FlushGather(true, m_boss_list[i].boss_id);

			m_boss_list[i].boss_next_flush_timestamp = 0;
			m_boss_list[i].boss_live_flag = true;
		}
	}

	if (now_second >= m_enter_close_timestamp)
	{
		m_enter_close = true;
	}

	// 检查区域boss创建
	if (this->IsMainZone())
	{
		this->CreateMainZoneBoss(now_second);
	}
	else
	{
		this->CreateMinorZoneBoss(now_second);
	}

	const unsigned int now = static_cast<unsigned int>(now_second);
	if (now >= m_check_user_time)
	{
		m_check_user_time = now + 1; //定时一秒检查一次
	
		// 检查boss归属
		this->CheckAllBossBound();
	}

	this->SyncFBSceneInfo();
}

void SpecialCrossLieKunFb::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (!m_activity_open)
	{
		return;
	}

	if (delta_hp >= 0)
	{
		return;
	}

	if (NULL == cha) return;

	Obj *obj = m_scene->GetObj(enemy_id);
	if (NULL == obj)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType() && Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Role *role = (Role *)obj;
		if (NULL == role) return;

		Monster *monster = (Monster *)cha;
		if (NULL == monster) return;

		if (monster->IsBossMonster())
		{
			long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

			for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; i++)
			{
				if (monster->GetMonsterId() == m_boss_list[i].replace_boss_id)
				{
					UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
					std::map<UniqueUserID, BossItem::GuildHurt>::iterator it = m_boss_list[i].guild_hurt_map.find(guild_id);
					if (m_boss_list[i].guild_hurt_map.end() == it)
					{
						BossItem::GuildHurt guild_hurt;

						guild_hurt.guild_id = guild_id;
						guild_hurt.hurt -= delta_hp;
						guild_hurt.logout_timestamp = 0;
						guild_hurt.uuid_map.insert(std::make_pair(uuid, delta_hp));

						m_boss_list[i].guild_hurt_map.insert(std::make_pair(guild_id, guild_hurt));
						//m_boss_list[i].guild_hurt_map[guild_id].uuid_map.insert(std::make_pair(uuid, guild_hurt.hurt));
					}
					else
					{
						it->second.hurt -= delta_hp;
						it->second.uuid_map[uuid] -= delta_hp;
						it->second.logout_timestamp = 0;
					}
					break;
				}
			}
		}
	}
}

void SpecialCrossLieKunFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (!m_activity_open)
	{
		return;
	}

	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;
		if (monster->IsBossMonster())
		{
			for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; i++)
			{
				if (monster->GetMonsterId() == m_boss_list[i].replace_boss_id)
				{
					// 区域boss只创建一次，故不重新计算
					if (i != LIEKUN_BOSS_TYPE_MAIN)
					{
						int refresh_s = 0;
						const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();
						if (this->IsMainZone())
						{
							refresh_s = other_cfg.refresh_main_zone_boss_s;
						}
						else
						{
							refresh_s = other_cfg.refresh_minor_zone_boss_s;
						}

						m_boss_list[i].boss_next_flush_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + refresh_s;
					}

					// 获取归属信息
					UniqueUserID highest_dps_guild_id = INVALID_UNIQUE_USER_ID;

					// 检查归属，检查后立即将boss存活标记重置，下面代码有用到这个标记
					this->CheckSingleBossBound(i);
					m_boss_list[i].boss_live_flag = false;
					highest_dps_guild_id = m_boss_list[i].guild_uuid;

					// 跨服仙盟击杀榜
					WorldStatus::Instance().AddGuildKillBossNum(highest_dps_guild_id, m_scene->GetSceneType());

					if (INVALID_UNIQUE_USER_ID != highest_dps_guild_id)
					{
						if (i == LIEKUN_BOSS_TYPE_MAIN)  // 杀死区域BOSS要同步击杀信息回原服
						{
							this->SyncKillZoneBossGuildId(highest_dps_guild_id);
							this->SyncFBLiveFlagInfo();
						}
						
						if (i == LIEKUN_BOSS_TYPE_MAIN && this->IsMainZone())		// 杀死的是主区域的区域boss
						{
							GuildMap::iterator it = m_guild_map.find(highest_dps_guild_id);
							if (it != m_guild_map.end())
							{
								int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_liekunfb_kill_main_zone_boss",
									it->second.guild_id.user_id.db_index, it->second.guild_name);
								if (length > 0)
								{
									World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
								}
							}
						}
						else if (i == LIEKUN_BOSS_TYPE_MAIN && !this->IsMainZone())	// 杀死的是副区域的区域boss
						{
							this->FlushPartal(false);

							GuildMap::iterator it = m_guild_map.find(highest_dps_guild_id);
							if (it != m_guild_map.end())
							{
								int title_id = CrossLieKunFbConfig::Instance().GetOtherCfg().title_item.item_id;
								char str_key[64] = { 0 };
								SNPRINTF(str_key, sizeof(str_key), "cross_liekunfb_kill_minor_zone_boss_%d", (int)m_cur_zone);
								int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), str_key,
									it->second.guild_id.user_id.db_index, it->second.guild_name, monster->GetMonsterId(), title_id);

								if (length > 0)
								{
									World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
								}
							}
						}
						else if (i != LIEKUN_BOSS_TYPE_MAIN && !m_enter_close)		// 杀死的是精英boss
						{
							GuildMap::iterator it = m_guild_map.find(highest_dps_guild_id);
							if (it != m_guild_map.end())
							{
								char str_key[64] = { 0 };
								SNPRINTF(str_key, sizeof(str_key), "cross_liekunfb_kill_jy_boss_%d", (int)m_cur_zone);
								int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_liekunfb_kill_jy_boss_0",
									it->second.guild_id.user_id.db_index, it->second.guild_name);
								
								if (length > 0)
								{
									World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
								}
							}
						}

						this->FlushGather(false, m_boss_list[i].boss_id);
					}

					m_boss_list[i].boss_obj = 0;
					break;
				}
			}
		}
	}

	this->SyncFBSceneInfo();
}

void SpecialCrossLieKunFb::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;

		if (role->GetGuildID() <= 0)  // 没有帮派直接T出
		{
			role->BackToOriginalServer();
			return;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		if (m_guild_map.end() == m_guild_map.find(guild_id))
		{
			SpecialCrossLieKunFb::GuildItem guild_item;
			guild_item.guild_id = guild_id;
			role->GetGuildName(guild_item.guild_name);

			m_guild_map.insert(std::make_pair(guild_id, guild_item));
		}
	}
}

void SpecialCrossLieKunFb::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (IsMainZone())
	{
		if (!m_is_enter_main_zone)                 //第一个进入记录boss创建时间
		{
			const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

			m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.refresh_zone_boss_s;
			m_is_enter_main_zone = true;
		}
	}

	this->SyncFBSceneInfo();
	EventHandler::Instance().OnEnterCrossLieKun(role);
}

void SpecialCrossLieKunFb::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
	{
		if (!m_boss_list[i].boss_live_flag)
		{
			continue;
		}

		Monster *monster = m_scene->GetMonster(m_boss_list[i].boss_obj);
		if (NULL == monster)
		{
			continue;
		}

		UniqueUserID guild_id = this->GetGuildUniqueUserId(role);
		std::map<UniqueUserID, BossItem::GuildHurt>::iterator guild_hurt_it = m_boss_list[i].guild_hurt_map.find(guild_id);
		if(guild_hurt_it != m_boss_list[i].guild_hurt_map.end())
		{
			BossItem::GuildHurt &guild_hurt = guild_hurt_it->second;

			guild_hurt.uuid_map.erase(uuid);

			if (!is_logout)  // 主动退出就清除归属
			{
				if (guild_hurt.uuid_map.size() <= 0)
				{
					if (m_boss_list[i].guild_uuid == guild_hurt.guild_id)
					{
						m_boss_list[i].guild_id = 0;
						m_boss_list[i].guild_uuid = INVALID_UNIQUE_USER_ID;
						//printf("\n Clear owner leave ... \n");
					}
					m_boss_list[i].guild_hurt_map.erase(guild_hurt_it);
				}
			}
			else
			{
				// 最后一名玩家掉线设置离线标记
				if (guild_hurt.uuid_map.size() <= 0)
				{
					guild_hurt.logout_timestamp = EngineAdapter::Instance().Time();
				}
			}			
		}
	}

	if (is_logout && m_activity_open)  // 掉线记录
	{
		RoleCrossLogoutRecord record;
		record.logout_flag = (short)1;
		record.cross_activity_type = (short)CROSS_ACTIVITY_TYPE_LIEKUN_FB;
		record.logout_timestamp = (unsigned int)EngineAdapter::Instance().Time();
		record.param = m_cur_zone;
		role->GetRoleCross()->SetCrossLogoutRecord(record);
	}

	if (!role->IsAlive())
	{
		role->ReAlive(100, 100, role->GetPos(), true);
	}
	else
	{
		role->Recover();
	}
}

bool SpecialCrossLieKunFb::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return true;
	}

	//const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		// 免费复活,回到安全区
		Posi realive_pos;
		m_scene->GetTownPoint(role,&realive_pos);
		{

			role->ReAlive(100, 100, realive_pos);

			return true;
		}
	}
// 	else if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
// 	{
// 		SyncUserInfoOnBack *common_user = ActivityShadow::Instance().GetCrossUserInfoOnBack(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
// 		if (NULL == common_user)
// 		{
// 			return false;
// 		}
// 
// 		// 元宝原地复活
// 		if (other_cfg.relive_need_gold > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.relive_need_gold, "SpecialCrossShengYinFb::SpecialRelive.Use gold."))
// 		{
// 			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return true;
// 		}
// 
// 		role->ReAlive(100, 100);
// 
// 		common_user->cost_gold_in_cross += other_cfg.relive_need_gold;
// 
// 		return true;
// 	}

	return false;
}

bool SpecialCrossLieKunFb::SpecialCanReAlive(Role * role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	return true;
// 	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
// 	{
// 		return true;
// 	}
// 	else if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
// 	{
// 		const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();
// 		if (other_cfg.relive_need_gold > 0 && !role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.relive_need_gold))
// 		{
// 			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return false;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
}

void SpecialCrossLieKunFb::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role)
	{
		return;
	}

	// 采集的是传送门
	if (gather_id == CrossLieKunFbConfig::Instance().GetOtherCfg().portal_id)
	{
		// 进入条件限制
		BossItem *boss_item = this->GetBossItemById(m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_id);
		if (NULL == boss_item)
		{
			return;
		}

		const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(LIEKUN_ZONE_TYPE_MAIN);
		if (NULL == zone_cfg)
		{
			return;
		}

		Posi enter_pos = zone_cfg->enter_pos;
		int scene_id = zone_cfg->scene_id;
		World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, CROSS_PUBLIC_SCENE_KEY, enter_pos);
	}
	else
	{
		// 采集条件限制
		const CrossLieKunRewardPosCfg *pos_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunRewardPosCfgByGatherId(m_cur_zone, gather_id);
		if (NULL == pos_cfg)
		{
			return;
		}

		BossItem *boss_item = this->GetBossItemById(pos_cfg->boss_id);
		if (NULL == boss_item)
		{
			return;
		}

		// 采集的角色不是击杀boss的帮派
		UniqueUserID guid_uuid = this->GetGuildUniqueUserId(role);
		if (boss_item->guild_uuid != guid_uuid)
		{
			gstr::SendError(role, "non_guild_can_not_gather");
			return;
		}

		CrossLieKunFBUser *user = ActivityShadow::Instance().GetCrossLieKunFBUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		if (NULL == user)
		{
			return;
		}

		int boss_type = -1;
		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
		{
			if (pos_cfg->boss_id == m_boss_list[i].boss_id)
			{
				boss_type = i;
				break;
			}
		}
		if (boss_type < 0)
		{
			return;
		}

		++user->gather_times[m_cur_zone][boss_type];

		int item_count = 0;
		int suipian_id = 0;
		ItemConfigData gather_item_list[LIEKUN_MAX_ITEM_COUNT];
		memset(gather_item_list, 0, sizeof(gather_item_list));

		bool is_drop_suipian = CrossLieKunFbConfig::Instance().GetCrossLieKunRewardItemList(gather_id, gather_item_list, item_count, suipian_id);
		if (is_drop_suipian)
		{
			int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "cross_liekunfb_role_get_suipian",
				role->GetUID(), role->GetName(), (int)role->GetCamp(), m_boss_list[boss_type].replace_boss_id, suipian_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		role->GetKnapsack()->PutList(item_count, gather_item_list, PUT_REASON_CROSS_FB_LIEKUN_GATHER);

		role->NoticeNum(noticenum::NT_CROSS_LIEKUN_FB_GATHER_SUCC);
	}	
}

bool SpecialCrossLieKunFb::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	int gather_id = gather->GatherId();
	if (gather_id == CrossLieKunFbConfig::Instance().GetOtherCfg().portal_id)
	{
		// 进入条件限制
		BossItem *boss_item = this->GetBossItemById(m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_id);
		if (NULL == boss_item)
		{
			return false;
		}

		// 进入传送门的角色不是击杀boss的帮派
		UniqueUserID guid_uuid = this->GetGuildUniqueUserId(role);
		if (boss_item->guild_uuid != guid_uuid)
		{
			gstr::SendError(role, "cross_liekunfb_guild_not_entry_portal");
			return false;
		}
	}
	else
	{
		// 采集条件限制
		const CrossLieKunRewardPosCfg *pos_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunRewardPosCfgByGatherId(m_cur_zone, gather_id);
		if (NULL == pos_cfg)
		{
			return false;
		}

		BossItem *boss_item = this->GetBossItemById(pos_cfg->boss_id);
		if (NULL == boss_item)
		{
			return false;
		}

		// 采集的角色不是击杀boss的帮派
		UniqueUserID guid_uuid = this->GetGuildUniqueUserId(role);
		if (boss_item->guild_uuid != guid_uuid)
		{
			gstr::SendError(role, "non_guild_can_not_gather");
			return false;
		}

		CrossLieKunFBUser *user = ActivityShadow::Instance().GetCrossLieKunFBUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
		if (NULL == user)
		{
			return false;
		}

		int boss_type = -1;
		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
		{
			if (pos_cfg->boss_id == m_boss_list[i].boss_id)
			{
				boss_type = i;
				break;
			}
		}
		if (boss_type < 0)
		{
			return false;
		}

		if (user->gather_times[m_cur_zone][boss_type] >= CrossLieKunFbConfig::Instance().GetOtherCfg().gather_times)
		{
			role->NoticeNum(errornum::EN_CROSS_LIEKUN_FB_GATHER_TOO_MUCH);
			return false;
		}
	}

	return true;
}

bool SpecialCrossLieKunFb::SpecialCanPick(Role *role, FallingItem *falling_item)
{
	if (NULL == role || NULL == falling_item)
	{
		return false;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	int monster_id = falling_item->GetMonsterId();
	const MonsterInitParam *monster_cfg = MONSTERPOOL->GetMonsterParam(monster_id);
	if (NULL == monster_cfg)
	{
		return false;
	}

	if (MonsterInitParam::MONSTER_TYPE_BOSS == monster_cfg->monster_type && MonsterInitParam::BOSS_TYPE_CROSS_BOSS == monster_cfg->boss_type)
	{
		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; i++)
		{
			if (monster_id == m_boss_list[i].replace_boss_id)
			{
				BossItem *boss_item = this->GetBossItemById(m_boss_list[i].boss_id);
				if (NULL == boss_item)
				{
					continue;
				}

				UniqueUserID guid_uuid = this->GetGuildUniqueUserId(role);
				if (boss_item->guild_uuid != guid_uuid)
				{
					gstr::SendError(role, "non_guild_can_not_pickup");
					return false;
				}
				else
				{
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

void SpecialCrossLieKunFb::FlushGather(bool boss_is_live, int boss_id)
{
	const CrossLieKunRewardPosCfg *pos_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunRewardPosCfgByBossId(m_cur_zone, boss_id);
	if (NULL == pos_cfg)
	{
		return;
	}	

	if (boss_is_live)
	{
		m_scene->DeleteGatherObjByGatherId(pos_cfg->gather_id);

		// 清除采集物的采集次数
		int boss_type = LIEKUN_BOSS_TYPE_MAIN;
		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
		{
			if (boss_id == m_boss_list[i].boss_id)
			{
				boss_type = i;
				break;
			}
		}

		ActivityShadow::Instance().ClearCrossLieKunFBUserGatherTimes(m_cur_zone, boss_type);
	}
	else
	{
		const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, pos_cfg->gather_id, other_cfg.gather_time_s * 1000, 0, false);
		SpecialCrossLieKunFb::BossItem * boss_item = this->GetBossItemById(boss_id);
		if (nullptr != boss_item)
		{
			GuildMap::iterator itr = m_guild_map.find(boss_item->guild_uuid);
			if (itr != m_guild_map.end())
			{
				gather_obj->SetSpecialGatherType(SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS);
				gather_obj->InitCrossGuildBattleBoss(itr->second.guild_name);
			}
		}

		Posi init_pos;
		init_pos = pos_cfg->gather_flush_pos;
		int offset = pos_cfg->gather_item_offset;

		bool is_set_pos = false;
		for (int k = 0; k < 10; ++k)
		{
			if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, init_pos.x, init_pos.y))
			{
				gather_obj->SetPos(init_pos);
				is_set_pos = true;
				break;
			}

			init_pos.x += (RandomNum(offset) - offset / 2);
			init_pos.y += (RandomNum(offset) - offset / 2);
		}

		if (!is_set_pos)
		{
			gather_obj->SetPos(init_pos);
		}

		m_scene->AddObj(gather_obj);
	}
}

void SpecialCrossLieKunFb::FlushPartal(bool boss_is_live)
{
	const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

	if (boss_is_live)
	{
		m_scene->DeleteGatherObjByGatherId(other_cfg.portal_id);
	}
	else
	{
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, other_cfg.portal_id, other_cfg.portal_time_s * 1000, 0, false);

		Posi init_pos;
		init_pos = other_cfg.portal_flush_pos;
		int offset = other_cfg.portal_item_offset;

		bool is_set_pos = false;
		for (int k = 0; k < 10; ++k)
		{
			if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, init_pos.x, init_pos.y))
			{
				gather_obj->SetPos(init_pos);
				is_set_pos = true;
				break;
			}

			init_pos.x += (RandomNum(offset) - offset / 2);
			init_pos.y += (RandomNum(offset) - offset / 2);
		}

		if (!is_set_pos)
		{
			gather_obj->SetPos(init_pos);
		}

		m_scene->AddObj(gather_obj);
	}	
}

void SpecialCrossLieKunFb::SyncFBSceneInfo()
{
	static Protocol::SCCrossLieKunFBSceneInfo fbio;

	fbio.zone = m_cur_zone;
	for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
	{
		fbio.boss_info_list[i].boss_id = m_boss_list[i].replace_boss_id;
		fbio.boss_info_list[i].boss_objid = m_boss_list[i].boss_obj;
		fbio.guild_id[i] = m_boss_list[i].guild_id;
		fbio.boss_next_flush_timestamp[i] = m_boss_list[i].boss_next_flush_timestamp;
	}

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbio, sizeof(fbio));
		}
	}
}

void SpecialCrossLieKunFb::SyncFBLiveFlagInfo()
{
	static Protocol::SCCrossLieKunFBLiveFlagInfo fbio;

	fbio.zone = m_cur_zone;
	fbio.is_main_live_flag = m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_live_flag;

	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbio, sizeof(fbio));
		}
	}
}

void SpecialCrossLieKunFb::SyncKillZoneBossGuildId(UniqueUserID highest_dps_guild_id)
{
	if (INVALID_UNIQUE_USER_ID == highest_dps_guild_id)
	{
		return;
	}

	crossgameprotocal::GameCrossLieKunFBKillZoneBossSync gclkfbkzbs;
	gclkfbkzbs.uuid = UniqueUserIDToLongLong(highest_dps_guild_id);
	gclkfbkzbs.zone = m_cur_zone;
	gclkfbkzbs.kill_zone_boss_guild_id = UidToInt(highest_dps_guild_id.user_id);

	InternalComm::Instance().SendToCross((const char*)&gclkfbkzbs, sizeof(gclkfbkzbs));
}

int SpecialCrossLieKunFb::GetSceneInfo()
{
	return (int)m_scene->RoleNum();
}

void SpecialCrossLieKunFb::OnActivityOpen()
{
	if (m_activity_open)
	{
		return;
	}

	m_first = true;
	m_activity_open = true;
	m_init_jy_boss_id = false;
	m_is_enter_main_zone = false;
}

void SpecialCrossLieKunFb::OnActivityClose()
{
	if (!m_activity_open)
	{
		return;
	}

	m_activity_open = false;

	{
		m_enter_close = false;
		m_enter_close_timestamp = 0;
		m_check_user_time = 0;
		m_guild_map.clear();
		this->KillAllMonster();
		this->FlushPartal(true);

		for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
		{
			this->FlushGather(true, m_boss_list[i].boss_id);
			m_boss_list[i].Reset();
		}
	}

	this->KickAllToOrginalToServer();

}

bool SpecialCrossLieKunFb::IsMainZone()
{
	if (LIEKUN_ZONE_TYPE_MAIN == m_cur_zone)
	{
		return true;
	}

	return false;
}

void SpecialCrossLieKunFb::CreateMainZoneBoss(time_t now_second)
{
	const int role_num = (int)m_scene->RoleNum();
	if (role_num <= 0)
	{
		return;
	}

	Monster *zone_boss = NULL;
	if (!m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_live_flag && now_second >= m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp && m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp > 0)
	{
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp = 0;
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_live_flag = true;

		int total_role_level = 0;
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL == role) continue;

			total_role_level += role->GetLevel();
		}

		int boss_level = total_role_level / role_num;
		if (boss_level <= 0)
		{
			return;
		}

		const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(m_cur_zone);
		if (NULL == zone_cfg)
		{
			return;
		}

		// 根据跨服内所有玩家等级来刷boss
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByCrossBossLevel(zone_cfg->boss_id_arr[LIEKUN_BOSS_TYPE_MAIN], boss_level);
		zone_boss = MONSTERPOOL->CreateMonster(m_boss_list[LIEKUN_BOSS_TYPE_MAIN].replace_boss_id, m_scene, gamecommon::GetDisRandPos(zone_cfg->boss_pos_arr[LIEKUN_BOSS_TYPE_MAIN], 3));
		if (NULL == zone_boss)
		{
			return;
		}

		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].born_pos = zone_cfg->boss_pos_arr[LIEKUN_BOSS_TYPE_MAIN];
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_obj = zone_boss->GetId();
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].BossReset();

		this->SyncFBLiveFlagInfo();
	}
}

void SpecialCrossLieKunFb::CreateMinorZoneBoss(time_t now_second)
{
	Monster *zone_boss = NULL;
	if (!m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_live_flag && now_second >= m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp && m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp > 0)
	{
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_next_flush_timestamp = 0;
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_live_flag = true;

		const int role_num = (int)m_scene->RoleNum();
		int total_role_level = 0;
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL == role) continue;

			total_role_level += role->GetLevel();
		}

		if (role_num <= 0)
		{
			return;
		}
		int boss_level = total_role_level / role_num;
		if (boss_level <= 0)
		{
			return;
		}
		
		const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(m_cur_zone);
		if (NULL == zone_cfg)
		{
			return;
		}

		// 根据跨服内所有玩家等级来刷boss
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByCrossBossLevel(zone_cfg->boss_id_arr[LIEKUN_BOSS_TYPE_MAIN], boss_level);
		zone_boss = MONSTERPOOL->CreateMonster(m_boss_list[LIEKUN_BOSS_TYPE_MAIN].replace_boss_id, m_scene, gamecommon::GetDisRandPos(zone_cfg->boss_pos_arr[LIEKUN_BOSS_TYPE_MAIN], 3));
		if (NULL == zone_boss)
		{
			return;
		}

		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].born_pos = zone_cfg->boss_pos_arr[LIEKUN_BOSS_TYPE_MAIN];
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].boss_obj = zone_boss->GetId();
		m_boss_list[LIEKUN_BOSS_TYPE_MAIN].BossReset();

		{
			//int title_id = CrossLieKunFbConfig::Instance().GetOtherCfg().title_item.item_id;
			int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "cross_liekunfb_flush_minor_zone_boss");
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}

		this->SyncFBLiveFlagInfo();
	}
}

UniqueUserID SpecialCrossLieKunFb::GetGuildUniqueUserId(Role *role)
{
	if (NULL == role) return INVALID_UNIQUE_USER_ID;

	int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
	UniqueUserID guild_id(plat_type, IntToUid(role->GetGuildID()));

	return guild_id;
}

SpecialCrossLieKunFb::BossItem *SpecialCrossLieKunFb::GetBossItemById(int boss_id)
{
	for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
	{
		if (m_boss_list[i].boss_id == boss_id)
		{
			return &m_boss_list[i];
		}
	}

	return NULL;
}

void SpecialCrossLieKunFb::CheckAllBossBound()
{
	for (int i = LIEKUN_BOSS_TYPE_MAIN; i < LIEKUN_BOSS_TYPE_COUNT; ++i)
	{
		CheckSingleBossBound(i);
	}
}

void SpecialCrossLieKunFb::CheckSingleBossBound(int boss_type/*, long long *high_hurt_role_uuid = NULL */)
{
	if (boss_type < LIEKUN_BOSS_TYPE_MAIN || boss_type >= LIEKUN_BOSS_TYPE_COUNT)
	{
		return;
	}

	if (!m_boss_list[boss_type].boss_live_flag)  // BOSS已被标记死亡则不计算归属
	{
		return;
	}

	const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

	Monster *monster = m_scene->GetMonster(m_boss_list[boss_type].boss_obj);
	if (NULL == monster)
	{
		return;
	}

	std::map<UniqueUserID, BossItem::GuildHurt>::iterator guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.begin();
	for (; guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end();)
	{
		BossItem::GuildHurt &guild_hurt = guild_hurt_it->second;
		std::map<long long, long long>::iterator uuid_it = guild_hurt.uuid_map.begin();
		for (; uuid_it != guild_hurt.uuid_map.end();)
		{
			UniqueUserID uuid = LongLongToUniqueUserID(uuid_it->first);
			Role *temp_role = m_scene->GetRoleByUUID(uuid);
			if (NULL == temp_role)
			{
				uuid_it++;
				continue;
			}

			if ((temp_role->GetPos() - monster->GetPos()).IsLongerThan(other_cfg.effective_range)) // 不在boss范围内
			{
				guild_hurt.uuid_map.erase(uuid_it++);
			}
			else
			{
				uuid_it++;
			}
		}

		if (guild_hurt.logout_timestamp > 0 && (guild_hurt.logout_timestamp + CORSS_LIEKUN_RETAIN_OWNER_TIME_S) <= EngineAdapter::Instance().Time())
		{
			// 掉线时间已经超时，就清掉
			guild_hurt.uuid_map.clear();
			//printf("\n Clear guild_hurt timeout... \n");
		}
		else if (guild_hurt.logout_timestamp > 0)
		{
			// 掉线没有超时， 保留
			guild_hurt_it++;
			continue;
		}

		if (guild_hurt.uuid_map.size() <= 0)
		{
			// 清掉该归属
			if (m_boss_list[boss_type].guild_uuid == guild_hurt.guild_id)
			{
				m_boss_list[boss_type].guild_id = 0;
				m_boss_list[boss_type].guild_uuid = INVALID_UNIQUE_USER_ID;
				//printf("\n Clear owner  ... \n");
			}
			m_boss_list[boss_type].guild_hurt_map.erase(guild_hurt_it++);
		}
		else
		{
			guild_hurt_it++;
		}
	}

	// 获取dps最高的帮派
// 	long long highest_dps_hurt_guild = 0;
// 	UniqueUserID highest_dps_guild_id = INVALID_UNIQUE_USER_ID;
// 	guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.begin();
// 	for (; guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end(); ++guild_hurt_it)
// 	{
// 		BossItem::GuildHurt &guild_hurt = guild_hurt_it->second;
// 		if (guild_hurt.hurt >= highest_dps_hurt_guild)
// 		{
// 			highest_dps_hurt_guild = guild_hurt.hurt;
// 			highest_dps_guild_id = guild_hurt_it->first;
// 		}
// 	}

	std::vector<BossItem::GuildHurt*> hurt_vec;
	for (guild_hurt_it = m_boss_list[boss_type].guild_hurt_map.begin(); guild_hurt_it != m_boss_list[boss_type].guild_hurt_map.end(); ++guild_hurt_it)
	{
		hurt_vec.push_back(&guild_hurt_it->second);
	}

	std::sort(hurt_vec.begin(), hurt_vec.end(), [](const BossItem::GuildHurt * hurt1, const BossItem::GuildHurt * hurt2) {return hurt1->hurt > hurt2->hurt; });

	{
		// 设置新归属或失去归属
		if (hurt_vec.size() <= 0)
		{
			m_boss_list[boss_type].guild_id = 0;
			m_boss_list[boss_type].guild_uuid = INVALID_UNIQUE_USER_ID;
		}
		else
		{
			m_boss_list[boss_type].guild_id = UidToInt(hurt_vec[0]->guild_id.user_id);
			m_boss_list[boss_type].guild_uuid = hurt_vec[0]->guild_id;
		}
	}

	if (hurt_vec.size() > 0)
	{
		Protocol::SCCrossLieKunFBBossHurtInfo hurt_info;
		hurt_info.boss_id = m_boss_list[boss_type].boss_id;
		hurt_info.count = 0;
		for (unsigned int i = 0; i < hurt_vec.size(); ++i)
		{
			hurt_vec[i]->rank = i;
			if (hurt_info.count < Protocol::SCCrossLieKunFBBossHurtInfo::BOSS_HURT_INFO_RANK_MAX)
			{
				auto itr = m_guild_map.find(hurt_vec[i]->guild_id);
				if (itr != m_guild_map.end())
				{
					hurt_info.info_list[hurt_info.count].guild_id = UidToInt(hurt_vec[i]->guild_id.user_id);
					hurt_info.info_list[hurt_info.count].hurt = hurt_vec[i]->hurt;
					F_STRNCPY(hurt_info.info_list[hurt_info.count].guild_name, itr->second.guild_name, sizeof(GuildName));
					hurt_info.count++;
				}
			}
		}

		const int MAX_GET_ROLE = 256;
		ObjID obj_list[MAX_GET_ROLE] = { 0 };
		int count = m_scene->GetZoneMatrix()->GetObjByCircleArea(m_boss_list[boss_type].born_pos, other_cfg.effective_range + 2, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
		{
			Role * role = m_scene->GetPrincipal(obj_list[i]);
			if (nullptr != role)
			{
				hurt_info.own_guild_rank = hurt_vec.size();
				hurt_info.own_guild_hurt = 0;
				std::map<UniqueUserID, BossItem::GuildHurt>::iterator hurt_it = m_boss_list[boss_type].guild_hurt_map.find(this->GetGuildUniqueUserId(role));
				if (hurt_it != m_boss_list[boss_type].guild_hurt_map.end())
				{
					hurt_info.own_guild_hurt = hurt_it->second.hurt;
					hurt_info.own_guild_rank = hurt_it->second.rank;
				}
				//printf("-----------%d,%lld\n",role->GetUID(),hurt_info.own_guild_hurt);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&hurt_info, sizeof(hurt_info));
			}
		}
	}
	// 取出归属帮派中的最高DPS角色uuid
// 	if (NULL != high_hurt_role_uuid)
// 	{
// 		// 获取帮派中dps最高的玩家,该玩家在场景内
// 		long long highest_dps_hurt_role = 0;
// 		long long highest_dps_uuid = 0;
// 		std::map<long long, long long>::iterator uuid_it = m_boss_list[boss_type].guild_hurt_map[highest_dps_guild_id].uuid_map.begin();
// 		for (; uuid_it != m_boss_list[boss_type].guild_hurt_map[highest_dps_guild_id].uuid_map.end(); ++uuid_it)
// 		{
// 			UniqueUserID uuid = LongLongToUniqueUserID(uuid_it->first);
// 			Role *temp_role = m_scene->GetRoleByUUID(uuid);
// 			if (NULL == temp_role)
// 			{
// 				continue;
// 			}
// 
// 			if (uuid_it->second >= highest_dps_hurt_role)
// 			{
// 				highest_dps_hurt_role = uuid_it->second;
// 				highest_dps_uuid = uuid_it->first;
// 			}
// 		}
// 
// 		*high_hurt_role_uuid = highest_dps_uuid;
// 	}
}

void SpecialCrossLieKunFb::KickAllToOrginalToServer()
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
