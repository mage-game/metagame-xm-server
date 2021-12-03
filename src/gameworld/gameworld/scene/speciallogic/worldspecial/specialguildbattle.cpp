// #include "specialguildbattle.hpp"
// #include "other/event/eventhandler.hpp"
// #include "obj/character/monster.h"
// #include "obj/character/role.h"
// 
// #include "scene/scenemanager.h"
// #include "scene/activityshadow/activityshadow.hpp"
// #include "scene/worldshadow/worldshadow.hpp"
// 
// #include "monster/monsterpool.h"
// #include "item/itempool.h"
// #include "world.h"
// 
// #include "effect/skilleffect/effectbuff.hpp"
// #include "effect/skilleffect/effectinvisible.hpp"
// #include "effect/skilleffect/effectmovespeed.hpp"
// #include "effect/skilleffect/effectreboundhurt.hpp"
// #include "effect/skilleffect/effecthurtdec.hpp"
// 
// #include "engineadapter.h"
// #include "internalcomm.h"
// 
// #include "servercommon/errornum.h"
// #include "servercommon/noticenum.h"
// #include "servercommon/userprotocal/msgsystem.h"
// #include "servercommon/string/gameworldstr.h"
// #include "servercommon/servercommon.h"
// 
// #include "protocal/msgactivity.hpp"
// #include "protocal/msgfight.h"
// 
// #include "gameworld/skill/skillpool.hpp"
// #include "other/route/mailroute.hpp"
// #include "protocal/msgscene.h"
// 
// #include "other/event/eventhandler.hpp"
// #include "global/rank/rankmanager.hpp"
// #include <algorithm>
// #include "global/usercache/usercache.hpp"
// #include "global/activity/activitymanager.hpp"
// #include "ai/monsterai/monsteraibase.hpp"
// #include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
// #include "effect/sceneeffect/sceneeffect.hpp"
// #include "globalconfig/globalconfig.h"
// #include "ai/monsterai/specialai/monsteraibattlefield.hpp"
// #include "effect/skilleffect/effectspecialstate.hpp"
// #include <math.h>
// #include "global/rank/topranker.hpp"
// #include "global/guild/guildmanager.hpp"
// #include "global/guild/guild.hpp"
// #include "global/worldstatus/worldstatus.hpp"
// #include "global/randactivity/randactivitymanager.hpp"
// 
// SpecialGuildBattle::SpecialGuildBattle(Scene *scene):SpecialLogic(scene),m_is_finish(false),m_boss_is_alive(false),
// 	m_next_calc_time(0),m_cur_rare_monster_num(0),m_husong_end_time(0),m_glod_box_total_num(0),m_sliver_box_total_num(0),m_credit_is_change(false),
// 	m_glod_box_id(0), m_sliver_box_id(0), m_common_box_id(0), m_token_id(0), m_refresh_rare_monster_max_num(0), m_boss_refresh_time(0), m_rare_monster_refresh_time(0),
// 	m_next_notice_boss_refresh_time(0), m_next_notice_rare_monster_refresh_time(0), m_last_first_guild_broadcast_time(0), m_last_best_killer_broadcast_time(0)
// {
// }
// 
// SpecialGuildBattle::~SpecialGuildBattle()
// {
// }
// 
// void SpecialGuildBattle::OnActivityStatusChange(int status)
// {
// 	switch (status)
// 	{
// 	case ACTIVITY_STATUS_STANDY:
// 		{
// 			this->OnActivityStandby();
// 		}
// 		break;
// 	case ACTIVITY_STATUS_OPEN:
// 		{
// 			this->OnActivityStart();
// 		}
// 		break;
// 
// 	case ACTIVITY_STATUS_CLOSE:
// 		{
// 			WorldStatus::Instance().OnActivityFinish(ACTIVITY_FIND_TYPE_GUILDBATTLE);
// 			this->OnActivityClose();
// 		}
// 		break;
// 	}
// }
// 
// bool SpecialGuildBattle::CanDestroy()
// {
// 	return m_is_finish;
// }
// 
// void SpecialGuildBattle::Update(unsigned long interval, time_t now_second)
// {
// 	SpecialLogic::Update(interval, now_second);
// 
// 	if (m_is_finish || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE)) return;
// 
// 	this->CheckBornBoss(now_second);
// 	this->CheckRareMonster(now_second);
// 	this->CheckBornGather(now_second);
// 
// 	if (m_credit_is_change && (m_next_calc_time == 0 || static_cast<unsigned int>(now_second) >= m_next_calc_time))
// 	{	
// 		m_credit_is_change = false;
// 		this->CalcGuildRank();
// 		m_next_calc_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg().recalc_time;
// 	}
// 
// 	if (m_luck.next_lucky_timestamp > 0 && now_second >= m_luck.next_lucky_timestamp)
// 	{
// 		m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg().lucky_interval;
// 
// 		this->GaveLuckyItemToUser();
// 	}
// 
// 	{
// 		// 传闻
// 		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_first_guild_broadcast_time + FIRST_GUILD_INTERVAL)
// 		{
// 			m_last_first_guild_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 			if (m_guild_rank_vec.size() > 0)
// 			{
// 				Guild *guild = GuildManager::Instance().GetGuild(m_guild_rank_vec[0].guild_id);
// 				if (NULL != guild)
// 				{
// 					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_first_guild, guild->GetGuildName());
// 					if (length > 0) 
// 					{
// 						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 					}
// 				}
// 
// 			}
// 		}
// 
// 		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_last_best_killer_broadcast_time + BEST_KILLER_INTERVAL)
// 		{
// 			m_last_best_killer_broadcast_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 			int killer_id = this->GetBestKiller();
// 			if (killer_id > 0)
// 			{
// 				JoinerItem * joiner = m_joiner_map[killer_id];
// 				if (NULL != joiner && joiner->kill_role_num > 1)
// 				{
// 					const UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(killer_id);
// 					if (NULL != cache_user)
// 					{
// 						int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_best_killer,
// 							cache_user->uid, cache_user->user_name, (int)cache_user->camp, joiner->kill_role_num);
// 						if (length > 0) 
// 						{
// 							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// }
// 
// void SpecialGuildBattle::CheckBornBoss(time_t now_second)
// {
// 	if (m_boss_is_alive) return;
// 
// 	if (0 != m_next_notice_boss_refresh_time && now_second > m_next_notice_boss_refresh_time)
// 	{
// 		int world_level = WorldShadow::Instance().GetWorldLevel();
// 		const GuildBattleMonsterCfg *monster_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetBossMonsterByWorldLevel(world_level);
// 		if (NULL == monster_cfg)
// 		{
// 			return;
// 		}
// 
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_monster_refresh_prepare_notice, monster_cfg->prepare_notice_time_s, monster_cfg->monster_id);
// 		if (length > 0)
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 
// 		m_next_notice_boss_refresh_time = 0;
// 	}
// 
// 	if (now_second < m_boss_refresh_time) return;
// 
// 	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 
// 	int world_level = WorldShadow::Instance().GetWorldLevel();
// 	const GuildBattleMonsterCfg *monster_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetBossMonsterByWorldLevel(world_level);
// 	if (NULL == monster_cfg)
// 	{
// 		return;
// 	}
// 	
// 	if (0 == m_boss_refresh_time)
// 	{
// 		m_boss_refresh_time = static_cast<unsigned int>(now_second) + monster_cfg->first_refresh_time_s;
// 
// 		if (now_second < m_boss_refresh_time - monster_cfg->prepare_notice_time_s)
// 		{
// 			m_next_notice_boss_refresh_time = m_boss_refresh_time - monster_cfg->prepare_notice_time_s;
// 		}
// 	}
// 
// 	if (now_second >= m_boss_refresh_time && false == m_boss_is_alive && monster_cfg->is_boss == 1)
// 	{
// 		Monster *monster = MONSTERPOOL->CreateMonster(monster_cfg->monster_id, m_scene, other_cfg.boss_refresh_pos);
// 		if (NULL != monster)
// 		{
// 			this->RemoveAllGoods();
// 			m_husong_end_time = 0;
// 			m_glod_box_total_num = 0;
// 			m_sliver_box_total_num = 0;
// 			m_boss_is_alive = true;
// 			this->SyncGlobalInfoToAll();
// 
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_monster_refresh_notice, monster_cfg->monster_id);
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 		}
// 	}
// }
// 
// void SpecialGuildBattle::CheckRareMonster(time_t now_second)
// {
// 	if (0 != m_next_notice_rare_monster_refresh_time && now_second > m_next_notice_rare_monster_refresh_time)
// 	{
// 		int world_level = WorldShadow::Instance().GetWorldLevel();
// 		const GuildBattleMonsterCfg *monster_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetRareMonsterByWorldLevel(world_level);
// 		if (NULL == monster_cfg)
// 		{
// 			return;
// 		}
// 
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_monster_refresh_prepare_notice, monster_cfg->prepare_notice_time_s, monster_cfg->monster_id);
// 		if (length > 0)
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 
// 		m_next_notice_rare_monster_refresh_time = 0;
// 	}
// 
// 	if (now_second < m_rare_monster_refresh_time)
// 	{
// 		return;
// 	}
// 
// 	int world_level = WorldShadow::Instance().GetWorldLevel();
// 	const GuildBattleMonsterCfg *monster_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetRareMonsterByWorldLevel(world_level);
// 	if (NULL == monster_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (0 == m_rare_monster_refresh_time)
// 	{
// 		m_rare_monster_refresh_time = static_cast<unsigned int>(now_second) + monster_cfg->first_refresh_time_s;
// 		if (now_second < m_rare_monster_refresh_time - monster_cfg->prepare_notice_time_s)
// 		{
// 			m_next_notice_rare_monster_refresh_time = m_rare_monster_refresh_time - monster_cfg->prepare_notice_time_s;
// 		}
// 	}
// 
// 	if (now_second >= m_rare_monster_refresh_time && m_cur_rare_monster_num < m_refresh_rare_monster_max_num && monster_cfg->is_rare_monster == 1)
// 	{
// 		for (int i = m_cur_rare_monster_num; i < m_refresh_rare_monster_max_num; ++ i)
// 		{
// 			if (this->RandCreateRareMonster(monster_cfg->monster_id))
// 			{
// 				++m_cur_rare_monster_num;
// 			}
// 		}
// 
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_monster_refresh_notice, monster_cfg->monster_id);
// 		if (length > 0)
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 
// 		m_rare_monster_refresh_time = static_cast<unsigned int>(now_second) + monster_cfg->refresh_interval_s;
// 		m_next_notice_rare_monster_refresh_time = m_rare_monster_refresh_time - monster_cfg->prepare_notice_time_s;
// 	}
// 	else if (now_second >= m_rare_monster_refresh_time)
// 	{
// 		m_rare_monster_refresh_time = static_cast<unsigned int>(now_second) + monster_cfg->refresh_interval_s;
// 		m_next_notice_rare_monster_refresh_time = m_rare_monster_refresh_time - monster_cfg->prepare_notice_time_s;
// 	}
// }
// 
// void SpecialGuildBattle::CheckBornGather(time_t now_second)
// {
// 
// }
// 
// void SpecialGuildBattle::OnRoleEnterScene(Role *role)
// {
// 	if (m_is_finish || INVALID_GUILD_ID == role->GetGuildID() || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
// 	{
// 		this->DelayKickOutRole(role);
// 		return;
// 	}
// 
// 	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
// 	if (NULL == guild_item)
// 	{
// 		guild_item = new GuildItem();
// 		guild_item->guild_id = role->GetGuildID();
// 		guild_item->guild_credit = 0;
// 		guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 		m_guild_map[guild_item->guild_id] = guild_item;
// 	}
// 
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 	if (NULL == joiner_item)
// 	{
// 		joiner_item = new JoinerItem();
// 		joiner_item->uid = role->GetUID();
// 		joiner_item->guild_id = role->GetGuildID();
// 		m_joiner_map[joiner_item->uid] = joiner_item;
// 	}
// 
// 	this->SyncGlobalInfo(role);
// 	this->SyncRoleInfo(role);
// 	this->SendLuckyInfoToUser(role);
// 
// 	EventHandler::Instance().OnEnterGuildBattle(role);
// }
// 
// void SpecialGuildBattle::OnRoleLeaveScene(Role *role, bool is_logout)
// {
// 	if (NULL != role)
// 	{
// 		role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);
// 
// 		JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 		if (NULL == joiner_item) return;
// 
// 		if (0 < joiner_item->husong_goods_id)
// 		{
// 			Posi posi = role->GetPos();
// 			
// 			const GuildBattleGoodsCreditCfg *good_credit_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_item->husong_goods_id);
// 			if (NULL != good_credit_cfg)
// 			{
// 				this->RandDropGoods(posi,good_credit_cfg->gather_id);
// 			}
// 
// 			joiner_item->husong_goods_id = 0;
// 			joiner_item->is_addhudun = 0;
// 		}
// 
// 		this->ClearSpeedEffect(role);
// 	}
// }
// 
// void SpecialGuildBattle::OnCharacterDie(Character *dead, ObjID killer_id)
// {
// 	if (NULL == dead || m_is_finish) return;
// 
// 	Role *killer = m_scene->GetPrincipal(killer_id);
// 
// 	// 死亡者为BOSS或稀有怪
// 	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType() && NULL != killer && Obj::OBJ_TYPE_ROLE == killer->GetObjType())
// 	{
// 		Monster *monster = (Monster *)dead;
// 
// 		GuildItem *guild_item = this->GetGuildItem(killer->GetGuildID());
// 		JoinerItem *joiner_killer = this->GetJoinerItem(killer->GetUID());
// 		if (NULL == guild_item || NULL == joiner_killer)
// 		{
// 			return;
// 		}
// 		
// 		const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 		const GuildBattleMonsterCfg *monster_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetMonsterCfg(monster->GetMonsterId());
// 
// 		if (NULL != monster_cfg && monster_cfg->is_boss == 1 && false != m_boss_is_alive)
// 		{
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_kill_boss_notice,
// 				killer->GetUID(), killer->GetName(), (int)killer->GetCamp() ,monster_cfg->monster_id);
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 
// 			// Boss死亡掉落银宝箱
// 			{
// 				const GuildBattleGoodsCreditCfg *silver_box_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(m_sliver_box_id);
// 				if (NULL != silver_box_cfg)
// 				{
// 					for(int i = 0;i < other_cfg.silver_box_count;i++)
// 					{
// 						const Posi *refresh_pos = LOGIC_CONFIG->GetGuildBattleConfig().GetSilverBoxRandPost();
// 						if (NULL != refresh_pos)
// 						{
// 							if (this->RandDropGoods(*refresh_pos,silver_box_cfg->gather_id))
// 							{
// 								m_sliver_box_total_num++;
// 							}
// 						}
// 					}
// 				}
// 			}
// 
// 			// Boss死亡掉落金箱子
// 			{
// 				const GuildBattleGoodsCreditCfg *gold_box_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(m_glod_box_id);
// 				if (NULL != gold_box_cfg)
// 				{
// 					// 金箱子直接调到击杀者身上,高级护送物替换低级，低级掉落
// 					if (0 != joiner_killer->husong_goods_id)
// 					{
// 						if (m_glod_box_id != joiner_killer->husong_goods_id)
// 						{
// 							Posi posi = killer->GetPos();
// 							this->RandDropGoods(posi,joiner_killer->husong_goods_id);
// 						}
// 						else
// 						{
// 							Posi posi_1 = monster->GetPos();
// 							this->RandDropGoods(posi_1,joiner_killer->husong_goods_id);
// 						}
// 					}
// 
// 					m_glod_box_total_num++;
// 					joiner_killer->husong_goods_id = gold_box_cfg->gather_id;
// 					this->AddMoveSpeedEffect(killer,joiner_killer->husong_goods_id);
// 					this->AddHuDun(killer);
// 
// 					for(int j = m_glod_box_total_num; j < other_cfg.glod_box_count; j++)
// 					{
// 						const Posi refresh_pos = monster->GetPos();
// 						if (this->RandDropGoods(refresh_pos, gold_box_cfg->gather_id))
// 						{
// 							m_glod_box_total_num++;
// 						}
// 					}
// 				}
// 			}
// 
// 			guild_item->guild_credit += monster_cfg->kill_guild_credit;
// 			m_credit_is_change = true;
// 			guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 			joiner_killer->history_get_person_credit += monster_cfg->kill_personal_credit;
// 			this->CheckPersonalScoreReward(killer);
// 			joiner_killer->history_get_guild_credit += monster_cfg->kill_guild_credit;
// 
// 			m_husong_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + monster_cfg->refresh_interval_s;
// 			m_boss_refresh_time = m_husong_end_time;
// 			m_next_notice_boss_refresh_time = m_boss_refresh_time - monster_cfg->prepare_notice_time_s;
// 			m_boss_is_alive = false;
// 			this->SyncGlobalInfoToAll();
// 			this->SyncRoleInfo(killer);
// 			this->NotifyImage(killer);
// 			return;
// 		}
// 
// 		else if (NULL != monster_cfg && monster_cfg->is_rare_monster == 1)
// 		{
// 			guild_item->guild_credit += monster_cfg->kill_guild_credit;
// 			m_credit_is_change = true;
// 			guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 			joiner_killer->history_get_person_credit += monster_cfg->kill_personal_credit;
// 			this->CheckPersonalScoreReward(killer);
// 			joiner_killer->history_get_guild_credit += monster_cfg->kill_guild_credit;
// 			--m_cur_rare_monster_num;
// 
// 			if (m_glod_box_id == joiner_killer->husong_goods_id)
// 			{
// 				Posi posi = dead->GetPos();
// 				const GuildBattleGoodsCreditCfg *token_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(m_token_id);
// 				if (NULL != token_cfg)
// 				{
// 					this->RandDropGoods(posi,token_cfg->gather_id);
// 				}
// 
// 				this->SyncGlobalInfoToAll();
// 				return;
// 			}
// 
// 			if (0 != joiner_killer->husong_goods_id)
// 			{
// 				if (m_glod_box_id != joiner_killer->husong_goods_id && m_token_id != joiner_killer->husong_goods_id)
// 				{
// 					Posi posi = killer->GetPos();
// 					this->RandDropGoods(posi,joiner_killer->husong_goods_id);
// 				}
// 				else
// 				{
// 					Posi posi_1 = monster->GetPos();
// 					this->RandDropGoods(posi_1,joiner_killer->husong_goods_id);
// 				}
// 			}
// 
// 			const GuildBattleGoodsCreditCfg *token_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(m_token_id);
// 			joiner_killer->husong_goods_id = token_cfg->gather_id;
// 			this->AddMoveSpeedEffect(killer,joiner_killer->husong_goods_id);
// 			this->AddHuDun(killer);
// 			
// 			this->SyncGlobalInfoToAll();
// 			this->SyncRoleInfo(killer);
// 			this->NotifyImage(killer);
// 			return;
// 		}
// 	}
// 
// 	// 死亡者为玩家，击杀者击杀数量增加
// 	else if (Obj::OBJ_TYPE_ROLE == dead->GetObjType() && NULL != killer && Obj::OBJ_TYPE_ROLE == killer->GetObjType() && Obj::OBJ_TYPE_ROLE == killer->GetObjType())
// 	{
// 		Role *death = (Role *)dead;
// 
// 		JoinerItem *joiner_killer = this->GetJoinerItem(killer->GetUID());
// 		JoinerItem *joiner_dead = this->GetJoinerItem(death->GetUID());
// 		GuildItem *guild_killer = this->GetGuildItem(killer->GetGuildID());
// 		GuildItem *guild_dead = this->GetGuildItem(death->GetGuildID());
// 
// 		if (NULL == joiner_killer || NULL == joiner_dead || NULL == guild_killer || NULL == guild_dead)
// 		{
// 			return;
// 		}
// 
// 		const GuildBattleGoodsCreditCfg *killer_husong = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_killer->husong_goods_id);
// 		const GuildBattleGoodsCreditCfg *death_husong = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_dead->husong_goods_id);
// 
// 		if (NULL != death_husong && death_husong->gather_id == m_glod_box_id)
// 		{
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_get_glod_box_notice,
// 				killer->GetUID(), killer->GetName(), (int)killer->GetCamp() , death->GetUID(), death->GetName(), (int)death->GetCamp());
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 		}
// 
// 		if (NULL != joiner_killer && guild_dead->guild_id != guild_killer->guild_id)
// 		{
// 			for (std::map<UserID, time_t>::iterator it = joiner_dead->hurt_map.begin(), 
// 				end = joiner_dead->hurt_map.end(); it != end; ++ it)
// 			{
// 				if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer->GetUserId())
// 				{
// 					JoinerItem *joiner_assist = this->GetJoinerItem(UidToInt(it->first));
// 					if (NULL != joiner_assist)
// 					{
// 						joiner_assist->assist++; 
// 						joiner_assist->lianzhan++;
// 						Role *assist_role = m_scene->GetRoleByUID(joiner_assist->uid);
// 						if (NULL != assist_role)
// 						{
// 							this->CheckLianZhanNotice(assist_role, death, true);
// 						}
// 					}
// 				}
// 			}
// 
// 			++ joiner_killer->kill_role_num;
// 			++ joiner_killer->lianzhan;
// 
// 			this->CheckLianZhanNotice(killer, death);
// 
// 			joiner_dead->lianzhan = 0;
// 			joiner_dead->hurt_map.clear();
// 		}
// 
// 		if (NULL == death_husong)
// 		{
// 			this->CalcKillOtherScore(killer);
// 			this->SyncRoleInfo(killer);
// 			return;
// 		}
// 		else if (NULL != death_husong && 0 == joiner_killer->husong_goods_id)
// 		{
// 			joiner_killer->husong_goods_id = joiner_dead->husong_goods_id;
// 			this->AddMoveSpeedEffect(killer,joiner_killer->husong_goods_id);
// 			this->AddHuDun(killer);
// 			joiner_dead->husong_goods_id = 0;
// 			joiner_dead->is_addhudun = 0;
// 		}
// 		else if (NULL != death_husong && NULL != killer_husong)
// 		{
// 			if (death_husong->reward_credit_personal > killer_husong->reward_credit_personal)
// 			{
// 				joiner_killer->husong_goods_id = joiner_dead->husong_goods_id;
// 				this->AddMoveSpeedEffect(killer,joiner_killer->husong_goods_id);
// 				this->AddHuDun(killer);
// 				joiner_dead->husong_goods_id = 0;
// 				joiner_dead->is_addhudun = 0;
// 
// 				Posi posi = killer->GetPos();
// 				this->RandDropGoods(posi,killer_husong->gather_id);
// 			}
// 			else
// 			{
// 				joiner_dead->husong_goods_id = 0;
// 				joiner_dead->is_addhudun = 0;
// 
// 				Posi posi = death->GetPos();
// 				this->RandDropGoods(posi,death_husong->gather_id);
// 			}
// 		}
// 
// 		this->ClearSpeedEffect(death);
// 		this->CalcKillOtherScore(killer);
// 		this->SyncRoleInfo(killer);
// 		this->NotifyImage(killer);
// 		this->NotifyImage(death);
// 		return;
// 	}
// 	
// 	// 玩家被怪物击杀
// 	else
// 	{
// 		Role *death = (Role *)dead;
// 		JoinerItem *joiner_dead = this->GetJoinerItem(death->GetUID());
// 		if (NULL != joiner_dead)
// 		{
// 			joiner_dead->lianzhan = 0;
// 			joiner_dead->hurt_map.clear();
// 
// 			const GuildBattleGoodsCreditCfg *death_husong = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_dead->husong_goods_id);
// 			if (NULL != death_husong)
// 			{
// 				joiner_dead->husong_goods_id = 0;
// 				joiner_dead->is_addhudun = 0;
// 
// 				Posi posi = death->GetPos();
// 				this->RandDropGoods(posi,death_husong->gather_id);
// 				this->ClearSpeedEffect(death);
// 				this->SyncRoleInfo(death);
// 				this->NotifyImage(death);
// 			}
// 			return;
// 		}
// 	}
// }
// 
// bool SpecialGuildBattle::SpecialRelive(Role *role, int realive_type)
// {
// 	if (m_is_finish || NULL == role)
// 	{
// 		return false;
// 	}
// 
// 	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
// 	{
// 		return false;
// 	}
// 
// 	Posi realive_pos(0, 0);
// 	LOGIC_CONFIG->GetGuildBattleConfig().GetGuildBattleEnterInfo(&realive_pos);
// 	if (realive_pos.x == 0 || realive_pos.y == 0)
// 	{
// 		return true;
// 	}
// 	role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 100, realive_pos);
// 
// 	return true;
// }
// 
// bool SpecialGuildBattle::SpecialIsEnemy(Role *role, Obj *obj)
// {
// 	if (NULL == role || NULL == obj || role == obj) return false;
// 
// 	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
// 	{
// 		Role *target = (Role *)obj;
// 		return role->GetGuildID() != target->GetGuildID();
// 	}
// 
// 	return SpecialLogic::SpecialIsEnemy(role, obj);
// }
// 
// void SpecialGuildBattle::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
// {
// 
// }
// 
// void SpecialGuildBattle::OnHurtOther(Character *cha, Character *target)
// {
// 	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
// 	{
// 		Role *hurt_role = (Role *)cha;
// 		Role *target_role = (Role *)target;
// 
// 		if (NULL == hurt_role || NULL == target_role)
// 		{
// 			return;
// 		}
// 
// 		JoinerItem *target_joiner = this->GetJoinerItem(target_role->GetUID());
// 		if (NULL != target_joiner)
// 		{
// 			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
// 		}
// 	}
// }
// 
// SpecialGuildBattle::GuildItem* SpecialGuildBattle::GetGuildItem(GuildID guild_id)
// {
// 	std::map<int, GuildItem *>::iterator iter = m_guild_map.find(guild_id);
// 	if (m_guild_map.end() == iter)
// 	{
// 		return NULL;
// 	}
// 
// 	return iter->second;
// }
// 
// SpecialGuildBattle::JoinerItem* SpecialGuildBattle::GetJoinerItem(int uid)
// {
// 	std::map<int, JoinerItem *>::iterator iter = m_joiner_map.find(uid);
// 	if (m_joiner_map.end() == iter)
// 	{
// 		return NULL;
// 	}
// 
// 	return iter->second;
// }
// 
// void SpecialGuildBattle::OnAddHuDun(Role *role)
// {
// 	if (NULL == role) return;
// 
// 	
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 	if (NULL == joiner_item)
// 		return;
// 
// 	if (joiner_item->husong_goods_id <= 0)
// 		return;
// 
// 	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 
// 	// 主动开启3S护盾效果
// 
// 	if (joiner_item->is_addhudun)
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_BATTLE_HAS_USE_SHILD);
// 		return;
// 	}
// 
// 	EffectDecHurt *effect_inc_hurt = new EffectDecHurt(role->GetId(), 0, other_cfg.shield_times_1, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
// 	effect_inc_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
// 	effect_inc_hurt->SetDecPercent(10000);
// 	role->AddEffect(effect_inc_hurt);
// 
// 	joiner_item->is_addhudun = true;
// 
// 	this->SyncRoleInfo(role);
// 
// }
// 
// void SpecialGuildBattle::AddHuDun(Role *role)
// {
// 	if (NULL == role) return;
// 
// 	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 
// 	// 获取宝箱自动获得1S护盾效果
// 	EffectDecHurt *effect_inc_hurt = new EffectDecHurt(role->GetId(), 0, other_cfg.shield_times_2, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
// 	effect_inc_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
// 	effect_inc_hurt->SetDecPercent(10000);
// 	role->AddEffect(effect_inc_hurt);
// }
// 
// void SpecialGuildBattle::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
// {
// 	if (m_is_finish || NULL == role || gather_id <= 0 || INVALID_GUILD_ID == role->GetGuildID() || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
// 	{
// 		return;
// 	}
// 
// 	const GuildBattleGoodsCreditCfg *gather_credit_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(gather_id);
// 	if (NULL == gather_credit_cfg)
// 	{
// 		return;
// 	}
// 
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 	if (NULL !=joiner_item)
// 	{
// 		if (0 != joiner_item->husong_goods_id)
// 		{
// 			Posi posi = role->GetPos();
// 
// 			const GuildBattleGoodsCreditCfg *good_credit_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_item->husong_goods_id);
// 			if (NULL != good_credit_cfg)
// 			{
// 				this->RandDropGoods(posi,good_credit_cfg->gather_id);
// 			}
// 		}
// 		else
// 		{
// 			joiner_item->is_addhudun = 0;
// 		}
// 		// 采集成功触发护送任务
// 		joiner_item->husong_goods_id = gather_credit_cfg->gather_id;
// 		this->AddMoveSpeedEffect(role,joiner_item->husong_goods_id);
// 		this->AddHuDun(role);
// 		this->SyncRoleInfo(role);
// 		this->NotifyImage(role);
// 
// 		std::set<ObjID>::iterator it = m_gather_list.find(obj_id);
// 		if (it != m_gather_list.end())
// 		{
// 			m_gather_list.erase(*it);
// 		}
// 	}
// }
// 
// int SpecialGuildBattle::GetSpecialParam(Role *role)
// {
// 	if (m_is_finish || NULL == role || INVALID_GUILD_ID == role->GetGuildID() || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
// 	{
// 		return INVAILD_APPEARANCE;
// 	}
// 
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 	if (NULL == joiner_item) 
// 		return INVAILD_APPEARANCE;
// 	else
// 	{
// 		if (m_glod_box_id == joiner_item->husong_goods_id)
// 		{
// 			return GLOD_BOX_APPEARANCE;
// 		}
// 		if (m_sliver_box_id == joiner_item->husong_goods_id)
// 		{
// 			return SLIVER_BOX_APPEARANCE;
// 		}
// 		if (m_common_box_id == joiner_item->husong_goods_id)
// 		{
// 			return COMMON_BOX_APPEARANCE;
// 		}
// 		if (m_token_id == joiner_item->husong_goods_id)
// 		{
// 			return TOKEN_APPEARANCE;
// 		}
// 	}
// 	
// 	return INVAILD_APPEARANCE;
// }
// 
// void SpecialGuildBattle::AddObj(int gather_id ,Posi pos)
// {
// 	if (gather_id <= 0) return;
// 
// 	const GuildBattleGoodsCreditCfg *goods_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(gather_id);
// 	if (NULL != goods_cfg)
// 	{
// 		if (NULL != m_scene)
// 		{
// 			GatherObj *gather_obj = new GatherObj();
// 			gather_obj->Init(NULL, goods_cfg->gather_id, goods_cfg->gather_time_s * 1000, 0,true);
// 			gather_obj->SetPos(pos);
// 			gather_obj->SetSpecialGatherType(SPECIAL_GATHER_TYPE_GUILDBATTLE);
// 			m_scene->AddObj(gather_obj);
// 			if (m_glod_box_id == goods_cfg->gather_id || m_sliver_box_id == goods_cfg->gather_id)
// 			{
// 				m_gather_list.insert(gather_obj->GetId());
// 			}
// 		}
// 	}
// }
// 
// bool SpecialGuildBattle::RandDropGoods(Posi posi,int gather_id)
// {
// 	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 	if (other_cfg.rand_x <= 0 || other_cfg.rand_y <= 0)
// 	{
// 		return false;
// 	}
// 
// 	Posi original_posi = posi;
// 	int i = 0;
// 
//	original_posi.x += RandomNum(-other_cfg.rand_x, other_cfg.rand_x);
//	original_posi.y += RandomNum(-other_cfg.rand_y, other_cfg.rand_y);
// 
// 	while (!m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE,original_posi.x,original_posi.y) && i < 1000)
// 	{
// 		original_posi = posi;
//		original_posi.x += RandomNum(-other_cfg.rand_x, other_cfg.rand_x);
//		original_posi.y += RandomNum(-other_cfg.rand_y, other_cfg.rand_y);
// 		i++;
// 	}
// 
// 	if (i < 1000)
// 	{
// 		this->AddObj(gather_id,original_posi);
// 		return true;
// 	}
// 
// 	return false;
// }
// 
// bool SpecialGuildBattle::RandCreateRareMonster(int monster_id)
// {
// 	const GuildBattleOtherCfg &other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 	const GuildBattleRareMonsterPos *posi = LOGIC_CONFIG->GetGuildBattleConfig().GetRareMonsterRandPost();
// 	if (NULL == posi || other_cfg.rand_x <= 0 || other_cfg.rand_y <= 0)
// 	{
// 		return false;
// 	}
// 
// 	Posi original_posi = posi->rare_monster_pos;
// 	int i = 0;
// 
//	original_posi.x += RandomNum(-other_cfg.rand_x, other_cfg.rand_x);
//	original_posi.y += RandomNum(-other_cfg.rand_y, other_cfg.rand_y);
// 
// 	while (!m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE,original_posi.x,original_posi.y) && i < 1000)
// 	{
// 		original_posi = posi->rare_monster_pos;
//		original_posi.x += RandomNum(-other_cfg.rand_x, other_cfg.rand_x);
//		original_posi.y += RandomNum(-other_cfg.rand_y, other_cfg.rand_y);
// 		i++;
// 	}
// 
// 	if (i < 1000)
// 	{
// 		Monster *monster = MONSTERPOOL->CreateMonster(monster_id, m_scene, original_posi);
// 		if (NULL != monster)
// 		{
// 			return true;
// 		}
// 	}
// 
// 	return false;
// }
// 
// void SpecialGuildBattle::RemoveAllGoods()
// {
// 	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
// 	{
// 		Role *temp_role = m_scene->GetRoleByIndex(i);
// 		JoinerItem * joiner = this->GetJoinerItem(temp_role->GetUID());
// 		if (NULL != joiner)
// 		{
// 			if (m_glod_box_id == joiner->husong_goods_id || m_sliver_box_id == joiner->husong_goods_id)
// 			{
// 				joiner->husong_goods_id = 0;
// 				joiner->is_addhudun = false;
// 				this->ClearSpeedEffect(temp_role);
// 				this->NotifyImage(temp_role);
// 			}
// 		}
// 	}
// 	
// 	if (!m_gather_list.empty())
// 	{
// 		for (std::set<ObjID>::iterator it = m_gather_list.begin();it != m_gather_list.end();it++)
// 		{
// 			m_scene->DeleteObj(*it);
// 		}
// 
// 		m_gather_list.clear();
// 	}
// 
// }
// void SpecialGuildBattle::OnActivityStandby()
// {
// 
// }
// 
// void SpecialGuildBattle::OnActivityStart()
// {
// 	m_is_finish = false;
// 	m_boss_is_alive = false;
// 	m_next_calc_time = 0;
// 	m_cur_rare_monster_num = 0;
// 	m_husong_end_time = 0;
// 	m_glod_box_total_num = 0;
// 	m_sliver_box_total_num = 0;
// 	m_credit_is_change = false;
// 
// 	const GuildBattleOtherCfg& other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 	m_glod_box_id = other_cfg.gold_box_id;
// 	m_sliver_box_id = other_cfg.sliver_box_id;
// 	m_common_box_id = other_cfg.common_box_id;
// 	m_token_id = other_cfg.token_id;
// 	m_refresh_rare_monster_max_num = other_cfg.refresh_rare_monster_count;
// 	m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg().lucky_interval;
// 	m_luck.luck_user_count = 0;
// 	m_guild_rank_vec.clear();
// }
// void SpecialGuildBattle::OnActivityClose()
// {
// 	if (m_is_finish) return;
// 
// 	m_is_finish = true;
// 	this->CalcRewardOnActClose();
// 
// 	//复活所有玩家，防止活动结束时玩家不能复活的问题，这里全补满血
// 	this->AllRoleReLive();
// 
// 	this->KillAllMonster();
// 	this->DelayKickOutAllRole();
// 
// 	// 传闻，广播胜利的公会
// 	{
// 		if (!m_guild_rank_vec.empty())
// 		{
// 			Guild *guild = GuildManager::Instance().GetGuild(m_guild_rank_vec[0].guild_id);
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_close, guild->GetGuildName());
// 			if (length > 0) World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 	}
// 
// 	//最后清理数据
// 	{
// 		std::map<int, GuildItem *>::iterator iter;
// 		for (iter = m_guild_map.begin(); m_guild_map.end() != iter; ++ iter)
// 		{
// 			delete iter->second;
// 		}
// 		m_guild_map.clear();
// 	}
// 
// 	{
// 		std::map<int, JoinerItem *>::iterator iter;
// 		for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++ iter)
// 		{
// 			delete iter->second;
// 		}
// 		m_joiner_map.clear();
// 	}
// 
// 	m_guild_rank_vec.clear();
// 
// 	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
// 	if (NULL != guild_rank)
// 	{
// 		guild_rank->ClearRank(GUILD_RANK_TYPE_TERRITORYWAR);
// 	}
// }
// 
// void SpecialGuildBattle::CalcSubmitGoodsScore(Role *role)
// {
// 	if (m_is_finish || NULL == role || INVALID_GUILD_ID == role->GetGuildID() || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
// 	{
// 		return;
// 	}
// 	
// 	// 成功护送物品，增加积分
// 	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 
// 	if (NULL == guild_item || NULL == joiner_item)
// 	{
// 		return;
// 	}
// 	
// 	if (0 == joiner_item->husong_goods_id)
// 	{
// 		return;
// 	}
// 
// 	{
// 		if(m_glod_box_id == joiner_item->husong_goods_id)
// 		{
// 			m_glod_box_total_num--;
// 			if (m_glod_box_total_num <= 0)
// 			{
// 				m_glod_box_total_num = 0;
// 			}
// 			this->SyncGlobalInfoToAll();
// 
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_commit_glod_box_notice,
// 				role->GetUID(), role->GetName(), (int)role->GetCamp());
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 		}
// 
// 		if (m_sliver_box_id == joiner_item->husong_goods_id)
// 		{
// 			m_sliver_box_total_num--;
// 			if (m_sliver_box_total_num <= 0)
// 			{
// 				m_sliver_box_total_num = 0;
// 			}
// 			this->SyncGlobalInfoToAll();
// 		}
// 		const GuildBattleGoodsCreditCfg *good_credit_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(joiner_item->husong_goods_id);
// 		if (NULL == good_credit_cfg)
// 		{
// 			return ;
// 		}
// 		guild_item->guild_credit += good_credit_cfg->reward_credit_guild;
// 		m_credit_is_change = true;
// 		guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 		joiner_item->history_get_person_credit += good_credit_cfg->reward_credit_personal;
// 		this->CheckPersonalScoreReward(role);
// 		joiner_item->history_get_guild_credit += good_credit_cfg->reward_credit_guild;
// 		joiner_item->husong_goods_id = 0;
// 		joiner_item->is_addhudun = 0;
// 		this->ClearSpeedEffect(role);
// 		this->SyncRoleInfo(role);
// 		this->NotifyImage(role);
// 	}
// }
// 
// void SpecialGuildBattle::CalcKillOtherScore(Role *role)
// {
// 	if (m_is_finish || NULL == role || INVALID_GUILD_ID == role->GetGuildID() || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
// 	{
// 		return;
// 	}
// 
// 	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 
// 	if (NULL == guild_item || NULL == joiner_item)
// 	{
// 		return;
// 	}
// 
// 	const GuildBattleKillOtherCreditCfg *kill_other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetKillOtherCreditCfg(joiner_item->kill_role_num);
// 	if (NULL == kill_other_cfg)
// 		return;
// 	
// 	if (joiner_item->IsFetchCredit(kill_other_cfg->kill_count_index))
// 		return;
// 
// 	guild_item->guild_credit += kill_other_cfg->reward_guild_credit;
// 	m_credit_is_change = true;
// 	guild_item->credit_change_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 	joiner_item->history_get_person_credit += kill_other_cfg->reward_preson_credit;
// 	this->CheckPersonalScoreReward(role);
// 	joiner_item->history_get_guild_credit += kill_other_cfg->reward_guild_credit;
// 
// 	joiner_item->SetFetchCreditFlag(kill_other_cfg->kill_count_index);
// }
// 
// bool SortFuncByScore(const SpecialGuildBattle::RankItem rankitem_1, const SpecialGuildBattle::RankItem rankitem_2)
// {
// 	if (rankitem_1.credit != rankitem_2.credit)
// 	{
// 		return rankitem_1.credit > rankitem_2.credit;
// 	}
// 
// 	return rankitem_1.score_change_time < rankitem_2.score_change_time;
// }
// 
// void SpecialGuildBattle::CalcGuildRank()
// {
// 	if (m_is_finish || !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE)) return;
// 
// 	m_guild_rank_vec.clear();
// 
// 	for(std::map<int,GuildItem *>::iterator it= m_guild_map.begin(); it != m_guild_map.end(); it++)
// 	{
// 		GuildItem *info = it->second;
// 		if (info->guild_credit > 0)
// 		{
// 			m_guild_rank_vec.push_back(RankItem(info->guild_id, info->guild_credit, info->credit_change_time));
// 		}
// 	}
// 
// 	if (!m_guild_rank_vec.empty())
// 	{
// 		std::sort(m_guild_rank_vec.begin(), m_guild_rank_vec.end(), &SortFuncByScore);
// 		this->SyncGlobalInfoToAll();
// 	}
// }
// 
// int SpecialGuildBattle::FindGuildRank(GuildID guild_id)
// {
// 	if (0 >= guild_id)
// 	{
// 		return -1;
// 	}
// 
// 	for(unsigned int i = 0;i < m_guild_rank_vec.size();i++)
// 	{
// 		if (m_guild_rank_vec[i].guild_id == guild_id)
// 		{
// 			return i+1;
// 		}
// 	}
// 
// 	return -1;
// }
// 
// void SpecialGuildBattle::CalcRewardOnActClose()
// {
// 	if (!m_is_finish)
// 	{
// 		return;
// 	}
// 
// 	// 计算最后排名,根据记录的公会积分排一次，确保完全按照公会积分排名次
// 	this->CalcGuildRank();
// 
// 	{
// 		//保存公会争霸排行前十的公会
// 		gglobalprotocal::SyncGuildRankInfo guild_rank_info;
// 		guild_rank_info.guild_rank_type = GUILD_RANK_TYPE_GUILDBATTLE;
// 		guild_rank_info.guild_rank_count = 0;
// 
// 		for(int i = 0; i < 10 && i < static_cast<int>(m_guild_rank_vec.size()); i++)
// 		{
// 			guild_rank_info.guild_rank_list[guild_rank_info.guild_rank_count].guild_id = m_guild_rank_vec[i].guild_id;
// 			guild_rank_info.guild_rank_list[guild_rank_info.guild_rank_count].rank_value = i + 1;
// 			guild_rank_info.guild_rank_count++;
// 
// 			if (0 == WorldStatus::Instance().GetOpenServerGuildBattelID() && RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE) && 0 == i)
// 			{
// 				Guild *guild = GuildManager::Instance().GetGuild(m_guild_rank_vec[i].guild_id);
// 				if (NULL != guild)
// 				{
// 					int tuanzhang_uid = guild->GetGuildMemberManager().GetTuanZhangUid();
// 					UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(tuanzhang_uid);
// 					if (NULL != user_node)
// 					{
// 						WorldStatus::Instance().SetOpenServerGuildBattelID(tuanzhang_uid);
// 						MailContentParam contentparam; contentparam.Reset();
// 
// 						const GuildBattleOtherCfg& other_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg();
// 						for (int item_count = 0; item_count < other_cfg.huizhang_extra_reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
// 						{
// 							if (other_cfg.huizhang_extra_reward[item_count].num > 0)
// 							{
// 								contentparam.AddItem(other_cfg.huizhang_extra_reward[item_count].item_id, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind);
// 								LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, user_node->uid, user_node->user_name, NULL, user_node->level, other_cfg.huizhang_extra_reward[item_count].item_id, NULL, NULL, other_cfg.huizhang_extra_reward[item_count].num, other_cfg.huizhang_extra_reward[item_count].is_bind, 0, 0);
// 							}
// 						}
// 
// 						int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guildbattle_rankreward_open_server);
// 						if (length_1 > 0)
// 						{
// 							MailRoute::MailToUser(IntToUid(tuanzhang_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		RankManager::Instance().SyncGuildRankInfo(&guild_rank_info);
// 		GuildManager::Instance().ClearTerritoryWarRank();
// 
// 		if (!m_guild_rank_vec.empty())
// 		{
// 			WorldStatus::Instance().OnSyncGBMenzhuInfo(m_guild_rank_vec[0].guild_id);
// 		}
// 		else
// 		{
// 			WorldStatus::Instance().OnSyncGBMenzhuInfo(INVALID_GUILD_ID);
// 		}
// 	}
// 
// 	// 给不同排名发奖励
// 	{
// 		std::map<int, JoinerItem *>::iterator iter;
// 
// 		for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++ iter)
// 		{
// 			Role *joimer_role = m_scene->GetRoleByUID(iter->second->uid);
// 			if (NULL != joimer_role)
// 			{
// 				this->SyncRoleInfo(joimer_role);
// 			}
// 
// 			GuildItem *guild_item = this->GetGuildItem(iter->second->guild_id);
// 			//玩家个人积分少于1不能获得奖励
// 			if (1 > iter->second->history_get_person_credit|| NULL == guild_item || guild_item->Invalid())
// 			{
// 				continue;
// 			}
// 			
// 			int guild_rank = this->FindGuildRank(iter->second->guild_id);
// 			const GuildBattleRankRewardCfg *rank_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetRankRewardCfg(guild_rank);
// 			if (NULL != rank_reward_cfg)
// 			{
// 				//发送公会排名邮件奖励
// 
// 				UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(iter->second->uid);
// 				if (NULL == user_node)
// 				{
// 					continue;
// 				}
// 
// 				static MailContentParam contentparam; contentparam.Reset();
// 				contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_GUILDGONGXIAN] = rank_reward_cfg->banggong;
// 				contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG] = rank_reward_cfg->shengwang;
// 				contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_EXP] = rank_reward_cfg->base_role_exp;
// 
// 				for(int item_count = 0; item_count < rank_reward_cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
// 				{
// 					contentparam.AddItem(rank_reward_cfg->reward_item[item_count].item_id, rank_reward_cfg->reward_item[item_count].num, rank_reward_cfg->reward_item[item_count].is_bind);
// 					LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, user_node->uid, user_node->user_name, NULL, user_node->level, rank_reward_cfg->reward_item[item_count].item_id, NULL, NULL, rank_reward_cfg->reward_item[item_count].num, rank_reward_cfg->reward_item[item_count].is_bind, 0, 0);
// 				}
// 
// 				int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guildbattle_rankreward_guild,guild_rank);
// 
// 				if (length_1 > 0)
// 				{
// 					MailRoute::MailToUser(IntToUid(iter->second->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 				}
// 
// 				LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, iter->second->uid, user_node->user_name, NULL, user_node->level, rank_reward_cfg->banggong, NULL, NULL, rank_reward_cfg->base_role_exp, 0, 0, 0);
// 			}
// 		}
// 	}
// 	this->SyncGlobalInfoToAll();
// }
// 
// void SpecialGuildBattle::SyncGlobalInfoToAll()
// {
// 	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
// 	{
// 		Role *temp_role = m_scene->GetRoleByIndex(i);
// 		this->SyncGlobalInfo(temp_role);
// 	}
// }
// 
// void SpecialGuildBattle::SyncGlobalInfo(Role *role)
// {
// 	if (NULL == role || INVALID_GUILD_ID == role->GetGuildID()) return;
// 
// 	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
// 	if (NULL == guild_item)
// 	{
// 		return;
// 	}
// 
// 	static Protocol::SCGBGlobalInfo cmd;
// 	cmd.is_finish = m_is_finish ? 1 : 0;
// 	cmd.self_guild_score = guild_item->guild_credit;
// 	cmd.self_guild_rank = 0;
// 	cmd.rank_count = 0;
// 	cmd.is_boss_alive = m_boss_is_alive ? 1 : 0;
// 	cmd.reserve_2 = 0;
// 	cmd.husong_end_time = m_husong_end_time;
// 	cmd.glod_box_total_num = m_glod_box_total_num;
// 	cmd.sliver_box_total_num = m_sliver_box_total_num;
// 	cmd.boss_curren_hp = 0;
// 	cmd.boss_max_hp = 0;
// 
// 	for (int i = 0; i < (int)m_scene->MonsterNum(); ++ i)
// 	{
// 		Monster *temp_monster = m_scene->GetMonsterByIndex(i);
// 		const GuildBattleMonsterCfg *boss_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetMonsterCfg(temp_monster->GetMonsterId());
// 		if (NULL != boss_cfg && boss_cfg->is_boss == 1)
// 		{
// 			cmd.boss_curren_hp = static_cast<int>(temp_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP));
// 			cmd.boss_max_hp = static_cast<int>(temp_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
// 		}
// 	}
// 	
// 	int gild_size = static_cast<int>(m_guild_rank_vec.size());
// 	for (int i = 0;i < Protocol::SCGBGlobalInfo::RANK_NUM && i < gild_size; ++ i)
// 	{
// 		Protocol::SCGBGlobalInfo::RankItem &rank_item = cmd.rank_list[i];
// 
// 		RankItem guild_item = m_guild_rank_vec[i];
// 
// 		Guild *guild = GuildManager::Instance().GetGuild(guild_item.guild_id);
// 		if (NULL == guild) continue;
// 
// 		rank_item.guild_id = guild_item.guild_id;
// 		rank_item.score = guild_item.credit;
// 		guild->GetGuildName(rank_item.guild_name);
// 
// 		++ cmd.rank_count;
// 		
// 		if (guild_item.guild_id == role->GetGuildID())
// 		{
// 			cmd.self_guild_rank = cmd.rank_count;
// 		}
// 	}
// 
// 	unsigned int sendlen = 0;
// 	if (cmd.rank_count < 10)
// 	{
// 		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - cmd.rank_count);
// 	}
// 	else
// 	{
// 		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - 10);
// 	}
// 	
// 
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
// }
// 
// 
// void SpecialGuildBattle::SyncRoleInfo(Role *role)
// {
// 	if (NULL == role) return;
// 
// 	GuildItem *guild_item = this->GetGuildItem(role->GetGuildID());
// 	if (NULL == guild_item)
// 	{
// 		return;
// 	}
// 
// 	JoinerItem *joiner_item = this->GetJoinerItem(role->GetUID());
// 	if (NULL == joiner_item)
// 	{
// 		return;
// 	}
// 
// 	static Protocol::SCGBRoleInfo cmd;
// 
// 	cmd.kill_role_num = joiner_item->kill_role_num;
// 	cmd.husong_appearance = 0;
// 	cmd.history_get_person_credit = joiner_item->history_get_person_credit;
// 	cmd.history_get_guild_credit = joiner_item->history_get_guild_credit;
// 
// 	if (m_glod_box_id == joiner_item->husong_goods_id)
// 	{
// 		cmd.husong_appearance = GLOD_BOX_APPEARANCE;
// 	}
// 	if (m_sliver_box_id == joiner_item->husong_goods_id)
// 	{
// 		cmd.husong_appearance = SLIVER_BOX_APPEARANCE;
// 	}
// 	if (m_common_box_id == joiner_item->husong_goods_id)
// 	{
// 		cmd.husong_appearance = COMMON_BOX_APPEARANCE;
// 	}
// 	if (m_token_id == joiner_item->husong_goods_id)
// 	{
// 		cmd.husong_appearance = TOKEN_APPEARANCE;
// 	}
// 
// 	cmd.husong_gooods_id = joiner_item->husong_goods_id;
// 	cmd.is_add_hudun = joiner_item->is_addhudun ? 1 : 0;
// 	cmd.reserve_1 = 0;
// 	cmd.reserve_2 = 0;
// 
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
// }
// 
// void SpecialGuildBattle::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
// {
// 	if (NULL != str_buff && str_len > 0) 
// 	{
// 		int sendlen = 0;
// 		static Protocol::SCSystemMsg sm;
// 
// 		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
// 		{
// 			m_scene->SendToRole((const char*)&sm, sendlen);
// 		}
// 	}
// }
// 
// void SpecialGuildBattle::NotifyImage(Role *role)
// {
// 	if (NULL == role) return;
// 
// 	// 广播形象
// 	static Protocol::SCGBBianShenView cmd;
// 	JoinerItem *joiner = this->GetJoinerItem(role->GetUID());
// 	if (NULL != joiner)
// 	{
// 		cmd.obj_id = role->GetId();
// 		cmd.bianshen_image = INVAILD_APPEARANCE;
// 
// 		if (m_glod_box_id == joiner->husong_goods_id)
// 		{
// 			cmd.bianshen_image = GLOD_BOX_APPEARANCE;
// 		}
// 		if (m_sliver_box_id == joiner->husong_goods_id)
// 		{
// 			cmd.bianshen_image = SLIVER_BOX_APPEARANCE;
// 		}
// 		if (m_common_box_id == joiner->husong_goods_id)
// 		{
// 			cmd.bianshen_image = COMMON_BOX_APPEARANCE;
// 		}
// 		if (m_token_id == joiner->husong_goods_id)
// 		{
// 			cmd.bianshen_image = TOKEN_APPEARANCE;
// 		}
// 
// 		role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
// 	}	
// }
// 
// void SpecialGuildBattle::AddMoveSpeedEffect(Role *role, int gather_id)
// {
// 	const GuildBattleGoodsCreditCfg *good_credit_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetGoodsCreditCfg(gather_id);
// 	if (NULL == good_credit_cfg || NULL == role)
// 	{
// 		return ;
// 	}
// 
// 	int skill_level = 0;
// 
// 	if (gather_id == m_glod_box_id)
// 	{
// 		skill_level = 3;
// 	}
// 	else if (gather_id == m_sliver_box_id)
// 	{
// 		skill_level = 2;
// 	}
// 	else if (gather_id == m_token_id)
// 	{
// 		this->ClearSpeedEffect(role);
// 	}
// 	else
// 	{
// 		skill_level = 1;
// 	}
// 
// 	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(role->GetId(), skill_level,30 * 60 * 1000 , EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
// 	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
// 	int value = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (good_credit_cfg->speed_slow_down * ROLE_ATTR_PER));
// 	effect_speed->SetMoveSpeed(-value, -good_credit_cfg->speed_slow_down / 100);
// 	role->AddEffect(effect_speed);
// }
// 
// void SpecialGuildBattle::ClearSpeedEffect(Role *role)
// {
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_MOVESPEED, EBT_CHIHUAN, role->GetId(),EffectBase::PRODUCT_METHOD_SCENE_SKILL);
// }
// 
// void SpecialGuildBattle::CheckPersonalScoreReward(Role *role)
// {
// 	if (NULL == role) return;
// 
// 	JoinerItem * joiner = this->GetJoinerItem(role->GetUID());
// 	if (NULL == joiner) return;
// 
// 	role->GetRoleGoal()->CheckBattleFieldCond(ACTIVITY_TYPE_GUILDBATTLE, joiner->history_get_person_credit);
// 
// 	const GuildBattlePersonalRewardCfg * person_reward_cfg = LOGIC_CONFIG->GetGuildBattleConfig().GetPersonalRewardCfg(joiner->history_get_person_credit);
// 	if (NULL != person_reward_cfg)
// 	{
// 		for(int i = person_reward_cfg->reward_index; i > joiner->fetch_last_personal_reward_index && i > 0; i--)
// 		{
// 			if (joiner->IsFetchPersonalCreditReward(i))
// 			{
// 				continue;
// 			}
// 
// 			const GuildBattlePersonalRewardCfg * reward = LOGIC_CONFIG->GetGuildBattleConfig().GetPersonalRewardCfgByIndex(i);
// 			if (NULL == reward)
// 			{
// 				continue;
// 			}
// 
// 			role->GetKnapsack()->PutListOrMail(reward->reward_item, PUT_REASON_GUILD_BATTEL);
// 			role->AddGuildGongxian(reward->banggong, "GuildBattle");
// 			role->GetRoleShop()->AddChestShopShengwang(reward->shengwang, "GuildBattle");
// 			joiner->SetFetchPersonalRewardFlag(i);
// 
// 			UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);
// 
// 			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[0].item_id, NULL, NULL, reward->reward_item[0].num, reward->reward_item[0].is_bind, 0, 0);
// 			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[1].item_id, NULL, NULL, reward->reward_item[1].num, reward->reward_item[1].is_bind, 0, 0);
// 			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->reward_item[2].item_id, NULL, NULL, reward->reward_item[2].num, reward->reward_item[2].is_bind, 0, 0);
// 			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_GUILDBATTLE, role->GetUID(), role->GetName(), NULL, role->GetLevel(), reward->banggong, NULL, NULL, 0, 0, 0, 0);
// 		}
// 
// 		// 传闻
// 		if (joiner->fetch_last_personal_reward_index != person_reward_cfg->reward_index)
// 		{
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_fetch_target_reward,
// 				role->GetUID(), role->GetName(), (int)role->GetCamp(), person_reward_cfg->require_credit_min);
// 			if (length > 0) 
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 		}
// 
// 		joiner->fetch_last_personal_reward_index = person_reward_cfg->reward_index;
// 	}
// }
// 
// void SpecialGuildBattle::SendLuckyInfoToUser(Role *role)
// {
// 	if (NULL == role)
// 	{
// 		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
// 		m_scene->SendToRole((const char*)&m_luck, sendlen);
// 	}
// 	else
// 	{
// 		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
// 		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_luck)))
// 		{
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_luck, sendlen);
// 		}
// 	}
// }
// 
// void SpecialGuildBattle::GaveLuckyItemToUser()
// {
// 	int luck_people_num = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg().luck_people_num;
// 
// 	int lucky_role_count = 0;
// 	int lucky_role_idx_list[ZHANCHANG_LUACY_ROLE_MAX] = {0};
// 
// 	{
// 		std::map<int, int> idx_map;
// 
// 		int max_rolenum = m_scene->RoleNum();
// 		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
// 		{
// 			idx_map.insert(std::make_pair(RandomNum(1024), i));
// 		}
// 
// 		int loop_count = 0;
// 		while (!idx_map.empty() && lucky_role_count < luck_people_num && ++ loop_count < 128)
// 		{
// 			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
// 			std::map<int, int>::iterator it = idx_map.begin();
// 			std::advance(it, rand_index);
// 
// 			lucky_role_idx_list[lucky_role_count ++] = it->second;
// 			idx_map.erase(it);
// 		}
// 	}
// 
// 	m_luck.luck_user_count = 0;
// 
// 	char str_format[256] = {0};
// 	int max_length = sizeof(str_format) / sizeof(str_format[0]);
// 	int str_length = 0;
// 
// 	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetGuildBattleConfig().GuildBattleGetOtherCfg().lucky_item;
// 
// 	for (int i = 0; i < lucky_role_count && i < ZHANCHANG_LUACY_ROLE_MAX; i++)
// 	{
// 		int role_idx = lucky_role_idx_list[i];
// 		if (role_idx < (int)m_scene->RoleNum())
// 		{
// 			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
// 			if (NULL != lucky_role)
// 			{
// 				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
// 				if (NULL != itembase)
// 				{
// 					lucky_role->GetKnapsack()->PutOrMail(lucky_item, PUT_REASON_GHZB_LUCK_REWARD);
// 				}
// 
// 				if (m_luck.luck_user_count < ZHANCHANG_LUACY_ROLE_MAX)
// 				{
// 					lucky_role->GetName(m_luck.luck_user_namelist[m_luck.luck_user_count]);
// 					++ m_luck.luck_user_count;
// 				}
// 
// 				if (i < 5)	// 最多打包5个名字 不用ZHANCHANG_LUACY_ROLE_MAX 防止改动会导致字符串不够
// 				{
// 					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ", 
// 						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
// 					str_length += temp_length;
// 				}
// 			}
// 		}
// 	}
// 
// 	this->SendLuckyInfoToUser();
// 
// 	{
// 		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_lucky_notice, 
// 			str_format);
// 		if (str_len > 0) 
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 	}
// }
// 
// int SpecialGuildBattle::GetBestKiller()
// {
// 	int best_killer_id = -1;
// 	int best_killer_num = -1;
// 	for (std::map<int, JoinerItem *>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); it++)
// 	{
// 		if (NULL != it->second)
// 		{
// 			if (it->second->kill_role_num > best_killer_num)
// 			{
// 				best_killer_id = it->second->uid;
// 				best_killer_num = it->second->kill_role_num;
// 			}
// 		}
// 	}
// 
// 	return best_killer_id;
// }
// 
// void SpecialGuildBattle::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
// {
// 	if (NULL != kill_role && NULL != dead_role)
// 	{
// 		JoinerItem *killer = this->GetJoinerItem(kill_role->GetUID());
// 		JoinerItem *dead = this->GetJoinerItem(dead_role->GetUID());
// 		if (NULL != killer && NULL != dead)
// 		{
// 			if (!is_assist && dead->lianzhan >= 3)
// 			{
// 				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
// 					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
// 					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
// 				if (str_len > 0) 
// 				{
// 					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
// 				}
// 			}
// 
// 			if (killer->lianzhan > 0)
// 			{
// 				int str_len = 0;
// 				int notice_type = SYS_MSG_ACTIVE_NOTICE;
// 
// 				if (0 == killer->lianzhan % 3)
// 				{
// 					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
// 						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
// 						killer->lianzhan, killer->lianzhan);
// 				}
// 
// 				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
// 			}
// 		}
// 	}
// }