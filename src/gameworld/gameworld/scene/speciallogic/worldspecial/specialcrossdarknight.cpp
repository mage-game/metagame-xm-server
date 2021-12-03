#include "specialcrossdarknight.hpp"
#include "gameworld/engineadapter.h"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "obj/character/role.h"
#include "other/cross/rolecross.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "obj/otherobj/gatherobj.h"
#include "item/knapsack.h"
#include "other/route/mailroute.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/string/gameworldstr.h"
#include "scene/scenemanager.h"
#include "servercommon/userprotocal/msgsystem.h"
#include <algorithm>
#include "scene/worldshadow/worldshadow.hpp"
#include "protocal/msgfight.h"
#include "servercommon/errornum.h"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/otherconfig.h"
#include "servercommon/userprotocal/msgcode.h"
#include "protocal/msgrole.h"
#include "global/worldstatus/worldstatus.hpp"
#include "gameworld/global/activity/activitymsgmanager.h"

SpecialCrossDarkNight::SpecialCrossDarkNight(Scene *scene) : SpecialLogic(scene), m_next_check_reward_timestamp(0),
		m_next_sync_rankinfo_timestamp(0), m_next_broadcast_timestamp(0), m_next_give_score_time_stamp(0),
		m_need_sync_rank(false),m_cur_boss_index(0), is_finsh(false), m_give_reward_index(0), m_next_sync_bossinfo_time(0),
		m_need_sync_boss_info(false)
{
	m_deader_killer_map.clear();
}

SpecialCrossDarkNight::~SpecialCrossDarkNight()
{
}

bool SpecialCrossDarkNightSortCompare(const CrossDarkNightRankItem &s1, const CrossDarkNightRankItem &s2)
{
	return s1.rank_val > s2.rank_val;
}

void SpecialCrossDarkNight::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *add_role = (Role *)obj;
		UniqueUserID uuid = add_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

		CrossDarkNightPlayerInfo *player_info = this->GetPlayerInfo(uuid);
		if (NULL != player_info)
		{
			player_info->uuid = uuid;
			player_info->obj_id = obj->GetId();
			player_info->box_count = 0;
			F_STRNCPY(player_info->name, add_role->GetName(), sizeof(player_info->name));
		}
		else
		{
			WorldStatus::Instance().OnRoleJoinDarkNight(add_role->GetLevel());

			player_info = &m_user_info_map[uuid];
			player_info->uuid = uuid;
			player_info->obj_id = obj->GetId();
			player_info->box_count = 0;
			F_STRNCPY(player_info->name, add_role->GetName(), sizeof(player_info->name));
		}
		this->SendRoleInfo(add_role);
		this->SyncBossInfoToUser(add_role);
		this->SyncCheckRewardTimeStamp(add_role);
		EventHandler::Instance().OnEnterCrossDarkNight(add_role);
	}
}

void SpecialCrossDarkNight::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_next_check_reward_timestamp != 0 && now_second >= m_next_check_reward_timestamp)
	{
		m_next_check_reward_timestamp = 0;
		this->CheckGiveReward();
		this->OnFlushBoss();
		this->SyncCheckRewardTimeStamp();
	}

	if (now_second >= m_next_sync_rankinfo_timestamp || m_need_sync_rank)
	{
		m_need_sync_rank = false;
		m_next_sync_rankinfo_timestamp = (unsigned int)now_second + CROSS_DARK_NIGHT_SYNC_RANK_INTERVAL;
		this->SyncRankinfoToUser();
	}

	if (now_second >= m_next_broadcast_timestamp)
	{
		m_next_broadcast_timestamp = (unsigned int)now_second + CROSS_DARK_NIGHT_BROADCAST_RANK_INTERVAL;

		if ((int)m_rank_vec.size() > 0)
		{
			std::sort(m_rank_vec.begin(), m_rank_vec.end(), SpecialCrossDarkNightSortCompare);

			if (m_rank_vec[0].rank_val > 0)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_dark_night_first_boradcast,
					m_rank_vec[0].game_name);
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}
	}

	if (now_second >= m_next_give_score_time_stamp)
	{
		const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();
		m_next_give_score_time_stamp = (unsigned int)now_second + other_cfg.score_reward_interval_s;
		this->GiveScoreToAllPlayer();
	}

	if (now_second >= m_next_sync_bossinfo_time && m_need_sync_boss_info)
	{
		this->SyncBossInfoToUser();
	}
}

void SpecialCrossDarkNight::OnActivityOpen()
{
	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			Posi pos = CrossDarkNightConfig::Instance().GetRandRealivePosi();
			role->ResetPos(pos.x, pos.y);
		}
	}

	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();

	m_next_check_reward_timestamp = 0;
	
	m_cur_boss_index = 0;
	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; ++i)
	{
		m_boss_info[i].Reset();
	}

	m_next_give_score_time_stamp = now_second + other_cfg.score_reward_interval_s;
	m_give_reward_index = 0;

	this->OnFlushBoss();
	this->SyncBossInfoToUser();
	this->SyncCheckRewardTimeStamp();
}

void SpecialCrossDarkNight::OnActivityClose()
{
	is_finsh = true;
	this->CheckGiveReward();

	for (int i = 0; i < static_cast<int>(m_scene->RoleNum()); ++i)
	{
		Role * role = m_scene->GetRoleByIndex(i);
		this->SendRoleInfo(role);
	}

	m_user_info_map.clear();
	m_rank_vec.clear();
	this->DelayKickOutAllRole();
}

void SpecialCrossDarkNight::OnFlushBoss()
{
	if (m_cur_boss_index > m_give_reward_index)
	{
		return;
	}

	m_cur_boss_index++;
	const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();
	m_next_check_reward_timestamp = (unsigned int)EngineAdapter::Instance().Time() + other_cfg.flush_monster_interval_s;
	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; ++i)
	{
		CrossDarkNightBossInfo & boss_info = m_boss_info[i];
		if (boss_info.boss_status == DARKNIGHT_BOSSSTATUS_EXIST)
		{
			continue;
		}

		const CrossDarkNightBossCfg *monster_cfg = CrossDarkNightConfig::Instance().GetBossInfoByIndex(i,boss_info.cur_boss_index);
		if (NULL == monster_cfg)
		{
			continue;
		}

		Posi monster_pos = CrossDarkNightConfig::Instance().GetRandomMonsterCreatePosByMonsterId(monster_cfg->boss_id);

		const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(monster_cfg->boss_id);
		if (NULL != monster_param)
		{
// 			int level = WorldStatus::Instance().GetDarkNightAverageLevel();
// 			int add_maxhp = (int)(monster_param->hp * (0.00009f * level * level - 0.06f * level + 9.975f));
			Monster* monster=MONSTERPOOL->CreateMonster(monster_cfg->boss_id, m_scene, monster_pos);
			if (NULL != monster)
			{
				boss_info.max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				boss_info.cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				boss_info.boss_status = DARKNIGHT_BOSSSTATUS_EXIST;
			}
		}

		boss_info.cur_boss_index++;
		boss_info.monster_id = monster_cfg->boss_id;
		boss_info.posi = monster_pos;
		boss_info.hurt_map.clear();

		//printf("----Flush:(%d,%d)\n", monster_pos.x, monster_pos.y);
	}

	this->SyncBossInfoToUser();
}

void SpecialCrossDarkNight::CheckGiveReward()
{
	const CrossDarkNightBoxCfg *box_cfg = CrossDarkNightConfig::Instance().GetBoxInfo();
	if (NULL == box_cfg || m_give_reward_index >= m_cur_boss_index) return;

	m_give_reward_index++;
	UserInfoMapIt it = m_user_info_map.begin();
	for (;it != m_user_info_map.end(); it ++)
	{
		Role *temp_role = this->GetRole(it->second.obj_id);
		if (NULL == temp_role) continue;

		if (!temp_role->IsAlive())
		{
			this->RobBoxOnRelive(temp_role); //死亡退出场景扣死亡抢宝箱；
		}

		if (it->second.box_count <= 0)
		{
			this->SendRoleInfo(temp_role);
			continue;
		}

		ItemConfigData temp_reward;
		ItemConfigData temp_reward2;						// 这个是当结算时部分是第一个奖励，部分是第二个奖励的情况，用来发第一个奖励用的

		if ((it->second.total_reward_box_count + it->second.box_count) > box_cfg->another_reward_times)
		{
			temp_reward = box_cfg->reward_item2;
			if (it->second.total_reward_box_count >= box_cfg->another_reward_times)
			{
				temp_reward.num *= it->second.box_count;
			}
			else
			{
				int temp_reward_count = it->second.box_count - (box_cfg->another_reward_times - it->second.total_reward_box_count);
				temp_reward.num *= temp_reward_count;

				temp_reward2 = box_cfg->reward_item;
				temp_reward2.num *= (box_cfg->another_reward_times - it->second.total_reward_box_count);
			}
		}
		else
		{
			temp_reward = box_cfg->reward_item;
			temp_reward.num *= it->second.box_count;
		}

		it->second.total_reward_box_count += it->second.box_count;
		it->second.box_count = 0;

		if (!temp_role->GetKnapsack()->CheckEmptyGridNoLessThan(2))
		{
			static MailContentParam contentparam; contentparam.Reset();

			const ItemBase *item_base = ITEMPOOL->GetItem(temp_reward.item_id);
			if (NULL != item_base)
			{
				contentparam.item_list[0].item_id = temp_reward.item_id;
				contentparam.item_list[0].num = temp_reward.num;
				contentparam.item_list[0].is_bind = (temp_reward.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
			}

			const ItemBase *item2_base = ITEMPOOL->GetItem(temp_reward2.item_id);
			if (NULL != item2_base)
			{
				contentparam.item_list[1].item_id = temp_reward2.item_id;
				contentparam.item_list[1].num = temp_reward2.num;
				contentparam.item_list[1].is_bind = (temp_reward2.is_bind) ? 1 : 0;
				contentparam.item_list[1].invalid_time = item2_base->CalInvalidTime();
			}

			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToCrossUser(it->second.uuid, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
		else
		{
			temp_role->GetKnapsack()->Put(temp_reward, PUT_REASON_CROSS_DARK_NIGHT_REWARD);

			if (0 != temp_reward2.item_id)
			{
				temp_role->GetKnapsack()->Put(temp_reward2, PUT_REASON_CROSS_DARK_NIGHT_REWARD);
			}
		}

		it->second.reward_item[m_give_reward_index - 1] = temp_reward;
		it->second.reward_item2[m_give_reward_index - 1] = temp_reward2;

		this->BroadcastPlayerInfo(it->second.obj_id);
		this->SendRoleInfo(temp_role);
	}
	
	m_rank_vec.clear();
}

CrossDarkNightPlayerInfo *SpecialCrossDarkNight::GetPlayerInfo(UniqueUserID uuid)
{
	UserInfoMapIt it = m_user_info_map.find(uuid);
	if (it != m_user_info_map.end())
	{
		return &it->second;
	}

	return NULL;
}

CrossDarkNightPlayerInfo *SpecialCrossDarkNight::GetPlayerInfo(ObjID obj_id)
{
	Role *role = this->GetRole(obj_id);
	if (NULL != role)
	{
		UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
		return this->GetPlayerInfo(uuid);
	}

	return NULL;
}

Role *SpecialCrossDarkNight::GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return NULL;
}

void SpecialCrossDarkNight::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role) return;

	const CrossDarkNightBoxCfg *box_cfg = CrossDarkNightConfig::Instance().GetBoxInfo();
	if (NULL == box_cfg) return;

	if (gather_id != box_cfg->gather_id) return;

	CrossDarkNightPlayerInfo * player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == player_info) return;

	player_info->box_count ++;

	this->OnUpdateRank(role->GetRoleCross()->GetCrossOriginalUniqueUserID(), player_info->box_count);

	this->SendRoleInfo(role);

	this->BroadcastPlayerInfo(role->GetId());
}

void SpecialCrossDarkNight::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha || INVALID_OBJ_ID == killer_objid) return;

	CrossDarkNightPlayerInfo * killer_info = this->GetPlayerInfo(killer_objid);
	if (NULL == killer_info) return;

	const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();

	bool is_need_sync = false;
	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *monster = (Monster*)dead_cha;

		if (monster->IsBossMonster())
		{
			const CrossDarkNightBossCfg * boss_cfg = CrossDarkNightConfig::Instance().GetBossInfoById(monster->GetMonsterId());
			CrossDarkNightBossInfo* boss_info = this->GetBossInfo(monster->GetMonsterId());
			if (NULL != boss_cfg && NULL != boss_info)
			{
				const CrossDarkNightBoxCfg *box_cfg = CrossDarkNightConfig::Instance().GetBoxInfo();
				if (NULL != box_cfg)
				{	
					// boss掉落及奖励根据地图人数进行调整
					int drop_num = 0;
					int reward_num = 0;
					//int beg_idx =0;
					std::vector<Posi> rand_pos_list;
					CrossDarkNightConfig::Instance().GetDropPosiListById(monster->GetMonsterId(), rand_pos_list);
					CrossDarkNightConfig::Instance().GetBossDropAndRewardNumByRoleNum(&drop_num, &reward_num, boss_cfg, (int)m_scene->RoleNum());
					for (int i = 0; i < drop_num && i < (int)rand_pos_list.size(); i++)
					{
// 						Posi monster_pos = monster->GetPos();    // 默认boss坐标点
// 						//Posi rand_pos = CrossDarkNightConfig::Instance().GetRandFreshBoxPosi(); // 随机池随机一个坐标
// 						Posi rand_pos;
// 						this->DarkNightDropToHelper(monster_pos, 6 , &beg_idx, rand_pos);
// 						for (int i = 0; i < 100; i++)
// 						{
// 							if (m_scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_GATHER, rand_pos.x, rand_pos.y))
// 							{
// 								monster_pos = rand_pos;                                     // 随机到一个可用位置进行替换
// 								break;
// 							}
// 							rand_pos = gamecommon::GetDisRandPos(rand_pos, 6);
// 						}

						Posi rand_pos = rand_pos_list[i];
						if (!m_scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_GATHER, rand_pos.x, rand_pos.y))
						{
							rand_pos = gamecommon::GetValidRandPos(monster->GetPos(), 6, m_scene);
						}

						// 生成采集物
						GatherObj *gather_obj = new GatherObj();
						gather_obj->SetPos(rand_pos);
						gather_obj->Init(NULL, box_cfg->gather_id, box_cfg->gather_time_s * 1000, 0, true, box_cfg->disappear_time_s);
						m_scene->AddObj(gather_obj);
					}
					
					killer_info->box_count += reward_num;
				}

				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_dark_night_kill_boss,
					killer_info->name);
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}

				this->OnAddScore(killer_info->uuid, boss_cfg->score);

				std::map<UniqueUserID, HurtItemInfo>::iterator it = boss_info->hurt_map.begin();
				for (; it != boss_info->hurt_map.end() && it->second.uuid != killer_info->uuid; it++)
				{
					this->OnAddScore(it->second.uuid, other_cfg.boss_assis_score);
				}

				boss_info->hurt_map.clear();
				boss_info->cur_hp = 0;
				boss_info->max_hp = 0;
				boss_info->boss_status = DARKNIGHT_BOSSSTATUS_NONEXIST;

				is_need_sync = true;
			}
		}
		else
		{
			const CrossDarkNightMonsterCfg * monster_cfg = CrossDarkNightConfig::Instance().GetMonsterInfoCfg(monster->GetMonsterId());
			if (NULL != monster_cfg)
			{
				int rand_val = RandomNum(100);
				if (rand_val <= monster_cfg->box_rate && monster_cfg->reward_num > 0)
				{
					killer_info->box_count += monster_cfg->reward_num;
				}

				this->OnAddScore(killer_info->uuid, monster_cfg->score);
			}
		}

		this->OnUpdateRank(killer_info->uuid, killer_info->box_count);
		this->BroadcastPlayerInfo(killer_info->obj_id);

		if (is_need_sync)
		{
			this->SyncCheckRewardTimeStamp();
			this->SyncBossInfoToUser();
		}
	}

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		CrossDarkNightPlayerInfo * dead_player_info = this->GetPlayerInfo(dead_cha->GetId());
		if (NULL == dead_player_info) return;

		const CrossDarkNightBoxCfg *box_cfg = CrossDarkNightConfig::Instance().GetBoxInfo();
		if (NULL == box_cfg) return;

		m_deader_killer_map[dead_player_info->uuid] = killer_info->uuid;
//		改到复活
// 		if (dead_player_info->box_count > 0)
// 		{
// 			int temp_cout = (int)ceil(dead_player_info->box_count * 0.5);
// 
// 			dead_player_info->box_count -= temp_cout;
// 			if (dead_player_info->box_count < 0)
// 			{
// 				dead_player_info->box_count = 0;
// 			}
// 
// 			killer_info->box_count += temp_cout;
// 
// 			this->OnUpdateRank(dead_player_info->uuid, dead_player_info->box_count);
// 			this->OnUpdateRank(killer_info->uuid, killer_info->box_count);
// 
// 			this->BroadcastPlayerInfo(killer_info->obj_id);
// 			this->BroadcastPlayerInfo(dead_player_info->obj_id);
// 		}

		this->OnAddScore(killer_info->uuid, other_cfg.kill_role_score);

		std::map<UniqueUserID, HurtItemInfo>::iterator it = dead_player_info->hurt_map.begin();
		for (; it != dead_player_info->hurt_map.end() && it->second.uuid != killer_info->uuid; it ++)
		{
			this->OnAddScore(it->second.uuid, other_cfg.role_assis_score);
		}

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_dark_night_kill_role,
			killer_info->name, dead_player_info->name);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
		}

		dead_player_info->hurt_map.clear();
	}
}

void SpecialCrossDarkNight::OnAddScore(UniqueUserID uuid, int score)
{
	if (score <= 0) return;

	CrossDarkNightPlayerInfo * player_info = this->GetPlayerInfo(uuid);
	if (NULL == player_info) return;

	Role *role = m_scene->GetRoleByUUID(uuid);
	if (NULL == role) return;

	player_info->score += score;

	const std::vector<CrossDarkNightScoreCfg> &score_vec = CrossDarkNightConfig::Instance().GetScoreVec();
	for (int i = 0; i < (int)score_vec.size(); i ++)
	{
		if (0 == (player_info->score_reward_flag & (1 << i)) && player_info->score >= score_vec[i].score)
		{
			if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(score_vec[i].item_count))
			{
				static MailContentParam contentparam; contentparam.Reset();

				for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; j ++)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(score_vec[i].reward_item_list[j].item_id);
					if (NULL != item_base)
					{
						contentparam.item_list[j].item_id = score_vec[i].reward_item_list[j].item_id;
						contentparam.item_list[j].num = score_vec[i].reward_item_list[j].num;
						contentparam.item_list[j].is_bind = (score_vec[i].reward_item_list[j].is_bind) ? 1 : 0;
						contentparam.item_list[j].invalid_time = item_base->CalInvalidTime();
					}
				}

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToCrossUser(player_info->uuid, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
			else
			{
				role->GetKnapsack()->PutList(score_vec[i].item_count, score_vec[i].reward_item_list, PUT_REASON_CROSS_DARK_NIGHT_REWARD);
			}

			if (score_vec[i].cross_honor > 0)
			{
				role->AddCrossHonor(score_vec[i].cross_honor, __FUNCTION__);
			}

			player_info->score_reward_flag |= (1 << i);
			
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_dark_night_score_reward,
// 				player_info->name, score_vec[i].score);
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
		}
	}

	this->SendRoleInfo(role);
}

void SpecialCrossDarkNight::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	CrossDarkNightPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == player_info) return;

	const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();

	const CrossDarkNightBoxCfg *box_cfg = CrossDarkNightConfig::Instance().GetBoxInfo();
	if (NULL == box_cfg) return;

	if (!role->IsAlive())
	{	
		this->RobBoxOnRelive(role); //死亡退出场景扣死亡抢宝箱；
	}

	if (player_info->box_count > 0)
	{
		int reward_count = (int)floor(player_info->box_count * (other_cfg.leave_reward_percent / 100.0));
		if (reward_count < 0)
		{
			reward_count = 0;
		}

		ItemConfigData temp_item;
		ItemConfigData temp_item2;				// 这个是当结算时部分是第一个奖励，部分是第二个奖励的情况，用来发第一个奖励用的
		if (reward_count > 0)
		{
			if ((player_info->total_reward_box_count + reward_count) > box_cfg->another_reward_times)
			{
				temp_item = box_cfg->reward_item2;
				if (player_info->total_reward_box_count >= box_cfg->another_reward_times)
				{
					temp_item.num *= reward_count;
				}
				else
				{
					int temp_reward_count = reward_count - (box_cfg->another_reward_times - player_info->total_reward_box_count);
					temp_item.num *= temp_reward_count;

					temp_item2 = box_cfg->reward_item;
					temp_item2.num *= (box_cfg->another_reward_times - player_info->total_reward_box_count);
				}
			}
			else
			{
				temp_item = box_cfg->reward_item;
				temp_item.num *= reward_count;
			}
		}

		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(2))
		{
			static MailContentParam contentparam; contentparam.Reset();

			const ItemBase *item_base = ITEMPOOL->GetItem(temp_item.item_id);
			if (NULL != item_base)
			{
				contentparam.item_list[0].item_id = temp_item.item_id;
				contentparam.item_list[0].num = temp_item.num;
				contentparam.item_list[0].is_bind = (temp_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
			}

			const ItemBase *item2_base = ITEMPOOL->GetItem(temp_item2.item_id);
			if (NULL != item2_base)
			{
				contentparam.item_list[1].item_id = temp_item2.item_id;
				contentparam.item_list[1].num = temp_item2.num;
				contentparam.item_list[1].is_bind = (temp_item2.is_bind) ? 1 : 0;
				contentparam.item_list[1].invalid_time = item2_base->CalInvalidTime();
			}

			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToCrossUser(player_info->uuid, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
		else
		{
			role->GetKnapsack()->Put(temp_item, PUT_REASON_CROSS_DARK_NIGHT_REWARD);
			role->GetKnapsack()->Put(temp_item2, PUT_REASON_CROSS_DARK_NIGHT_REWARD);
		}

		player_info->total_reward_box_count += reward_count;
		player_info->box_count = 0;
	}

	std::vector<CrossDarkNightRankItem>::iterator it = m_rank_vec.begin();
	for (; it != m_rank_vec.end(); it++)
	{
		if (it->uuid == role->GetRoleCross()->GetCrossOriginalUniqueUserID())
		{
			m_rank_vec.erase(it);
			break;
		}
	}

	if (is_logout)
	{
		role->BackToOriginalServer();
	}
}

void SpecialCrossDarkNight::SendRoleInfo(Role *role)
{
	if (NULL == role) return;

	CrossDarkNightPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == player_info) return;

	static Protocol::SCCrossDarkNightUserInfo msg;
	msg.score = player_info->score;
	msg.box_count = player_info->box_count;
	msg.total_reward_box_count = player_info->total_reward_box_count;
	msg.is_finish = is_finsh;
	msg.reward_count = m_give_reward_index;

	for (int i = 0; i < m_give_reward_index; i++)
	{
		msg.info_list[i].reward_item.item_id = player_info->reward_item[i].item_id;
		msg.info_list[i].reward_item.num = player_info->reward_item[i].num;
		msg.info_list[i].reward_item2.item_id = player_info->reward_item2[i].item_id;
		msg.info_list[i].reward_item2.num = player_info->reward_item2[i].num;
	}
	int len = sizeof(msg) - sizeof(Protocol::SCCrossDarkNightUserInfo::RewardInfo)*(CROSS_DARK_NIGHT_BOSS_MAX_COUNT - m_give_reward_index);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, len);
}

void SpecialCrossDarkNight::OnUpdateRank(UniqueUserID uuid, int box_count)
{
	bool is_new = true;
	m_need_sync_rank = true;
	std::vector<CrossDarkNightRankItem>::iterator it = m_rank_vec.begin();
	for (;it != m_rank_vec.end(); it ++)
	{
		if (uuid == it->uuid)
		{
			it->rank_val = box_count;
			is_new = false;
			break;
		}
	}

	if (box_count == 0)
	{
		if (is_new)       // 新来的宝箱还为0   不加入排行
		{
			return;
		}
		else              // 宝箱为0   从排行榜删除
		{
			m_rank_vec.erase(it);
			return;
		}
	}

	CrossDarkNightPlayerInfo * playerinfo = this->GetPlayerInfo(uuid);
	if (is_new && NULL != playerinfo)
	{
		CrossDarkNightRankItem temp_item;
		temp_item.rank_val = box_count;
		temp_item.uuid = uuid;
		F_STRNCPY(temp_item.game_name, playerinfo->name, sizeof(GameName));

		m_rank_vec.push_back(temp_item);
	}
}

void SpecialCrossDarkNight::SyncRankinfoToUser()
{
	static Protocol::SCCrossDarkNightRankInfo msg;

	if (m_rank_vec.size() <= 0)
	{
		msg.rank_count = 0;
		int sendlen = sizeof(msg) - (CROSS_DARK_NIGHT_RANK_COUNT - msg.rank_count) * sizeof(msg.rank_list[0]);
		m_scene->SendToRole((const char*)&msg, sendlen);
		return;
	}
	UniqueUserID old_first = m_rank_vec[0].uuid;

	std::sort(m_rank_vec.begin(), m_rank_vec.end(), SpecialCrossDarkNightSortCompare);

	int rank_count = 0;
	for (int i = 0; i < (int)m_rank_vec.size() && i < CROSS_DARK_NIGHT_RANK_COUNT; i++)
	{
		msg.rank_list[i].rank_val = m_rank_vec[i].rank_val;
		F_STRNCPY(msg.rank_list[i].name, m_rank_vec[i].game_name, sizeof(GameName));

		rank_count ++;
	}
	msg.rank_count = rank_count;

	int sendlen = sizeof(msg) - (CROSS_DARK_NIGHT_RANK_COUNT - rank_count) * sizeof(msg.rank_list[0]);
	m_scene->SendToRole((const char*)&msg, sendlen);

	if (old_first != m_rank_vec[0].uuid)
	{
		ActivityMsgManager::RankTopItem first;
		first.uid = UniqueUserIDToLongLong(m_rank_vec[0].uuid);
		F_STRNCPY(first.user_name, m_rank_vec[0].game_name, sizeof(GameName));
		ActivityMsgManager::Instance().RankFirstMsg(m_scene, first);
	}

	if (ActivityMsgManager::Instance().IsNeedRankTopMsg(m_scene))
	{
		ActivityMsgManager::RankTopItem top[ACTIVITY_RANK_TOP_MSG_MAX];
		int i = 0;
		for (; i < ACTIVITY_RANK_TOP_MSG_MAX && i < (int)m_rank_vec.size(); ++i)
		{
			top[i].uid = UniqueUserIDToLongLong(m_rank_vec[i].uuid);
			F_STRNCPY(top[i].user_name, m_rank_vec[i].game_name, sizeof(GameName));
		}
		ActivityMsgManager::Instance().RankTopMsg(m_scene, top, i);
	}
}

void SpecialCrossDarkNight::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialCrossDarkNight::SyncBossInfoToUser(Role *role)
{
	static Protocol::SCCrossDarkNightBossInfo msg;
	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; ++i)
	{
		msg.boss_info[i].monster_id = m_boss_info[i].monster_id;
		msg.boss_info[i].posi = m_boss_info[i].posi;
		msg.boss_info[i].max_hp = m_boss_info[i].max_hp;
		msg.boss_info[i].cur_hp = m_boss_info[i].cur_hp;
		msg.boss_info[i].boss_status = m_boss_info[i].boss_status;
		msg.boss_info[i].reserve_sh = 0;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
		
	}
	else
	{
		m_scene->SendToRole((const char *)&msg, sizeof(msg));
		m_need_sync_boss_info = false;
		m_next_sync_bossinfo_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 1;
	}
}

void SpecialCrossDarkNight::BroadcastPlayerInfo(ObjID obj_id, Role *target_role)
{
	CrossDarkNightPlayerInfo * player_info = this->GetPlayerInfo(obj_id);
	if (NULL == player_info) return;

	static Protocol::SCCrossDarkNightPlayerInfoBroadcast msg;
	msg.obj_id = obj_id;
	msg.box_count = player_info->box_count;

	if (target_role == nullptr)
	{
		m_scene->SendToRole((const char *)&msg, sizeof(msg));
	}
	else
	{
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&msg, sizeof(msg));
	}
}

void SpecialCrossDarkNight::SyncCheckRewardTimeStamp(Role *role)
{
	static Protocol::SCCrossDarkNightRewardTimestampInfo msg;

	msg.next_check_reward_timestamp = m_next_check_reward_timestamp;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
	}
	else
	{
		m_scene->SendToRole((const char *)&msg, sizeof(msg));
	}
}

void SpecialCrossDarkNight::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha || INVALID_OBJ_ID == enemy_id || delta_hp > 0)
	{
		return;
	}

	Obj *enemy = m_scene->GetObj(enemy_id);
	if (NULL == enemy)
	{
		return;
	}

	if (cha->GetObjType() == Obj::OBJ_TYPE_MONSTER && Obj::OBJ_TYPE_ROLE == enemy->GetObjType())
	{
		Monster *monster = (Monster*)cha;
		Role *attack_role = (Role*)enemy;

		if (monster->IsBossMonster())
		{
			CrossDarkNightBossInfo *boss_info = this->GetBossInfo(monster->GetMonsterId());
			if (NULL != boss_info)
			{
				boss_info->cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				std::map<UniqueUserID, HurtItemInfo>::iterator it = boss_info->hurt_map.find(attack_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
				if (it == boss_info->hurt_map.end())
				{
					HurtItemInfo temp_hurt;
					temp_hurt.uuid = attack_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

					boss_info->hurt_map[temp_hurt.uuid] = temp_hurt;
				}
				m_need_sync_boss_info = true;
			}
		}
	}

	if (cha->GetObjType() == Obj::OBJ_TYPE_ROLE && Obj::OBJ_TYPE_ROLE == enemy->GetObjType())
	{
		Role *attack_role = (Role*)enemy;

		CrossDarkNightPlayerInfo * player_info= this->GetPlayerInfo(cha->GetId());
		if (NULL != player_info)
		{
			std::map<UniqueUserID, HurtItemInfo>::iterator it = player_info->hurt_map.find(attack_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			if (it == player_info->hurt_map.end())
			{
				HurtItemInfo temp_hurt;
				temp_hurt.uuid = attack_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

				player_info->hurt_map[temp_hurt.uuid] = temp_hurt;
			}
		}
	}
}

void SpecialCrossDarkNight::GiveScoreToAllPlayer()
{
	const CrossDarkNightOtherCfg &other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();

	UserInfoMapIt it = m_user_info_map.begin();
	for (; it != m_user_info_map.end(); ++it)
	{
		this->OnAddScore(it->second.uuid, other_cfg.reward_score);
	}
}

void SpecialCrossDarkNight::OnPlayerOpera(Role *role)
{
	if (NULL == role) return;
	if ((int)m_rank_vec.size() <= 0) return;

	static Protocol::SCCrossDarkNightTopPlayerPosi msg;
	std::sort(m_rank_vec.begin(), m_rank_vec.end(), SpecialCrossDarkNightSortCompare);

	UniqueUserID self_uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	UniqueUserID target_uuid = m_rank_vec[0].uuid;
	if (target_uuid == self_uuid)      // 自己就是榜首
	{
		if ((int)m_rank_vec.size() > 1)
		{
			target_uuid = m_rank_vec[1].uuid;
		}
		else 
		{
			return;
		}
	}

	Role *target = m_scene->GetRoleByUUID(target_uuid);
	if (NULL == target) return;

	msg.obj_id = target->GetId();
	msg.posi = target->GetPos();

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
}

bool SpecialCrossDarkNight::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi birth_pos = CrossDarkNightConfig::Instance().GetRandRealivePosi();
		if (role->ReAliveTargetScene(m_scene->GetSceneID(), birth_pos))
		{
			this->RobBoxOnRelive(role);
			return true;
		}
	}

	return false;
}

void SpecialCrossDarkNight::OnAOIRoleEnter(Role *role, Role *target_role)
{
	if (role == nullptr || target_role == nullptr)
	{
		return;
	}

	this->BroadcastPlayerInfo(role->GetId(), target_role);
}

bool SpecialCrossDarkNight::DarkNightDropToHelper(const Posi &pos, int extra_dis, int * begin_idx, Posi & out_pos)
{
	if (NULL != begin_idx && extra_dis > 0)
	{
		// 位置偏移
		static Posi POS_OFFSET_LIST[] = {
			Posi(0, 0),   Posi(-1, 0),  Posi(-1, -1), Posi(0, -1),  Posi(1, -1),  Posi(1, 0),   Posi(1, 1),   Posi(0, 1),   Posi(-1, 1),
			Posi(-1, -2), Posi(0, -2),  Posi(1, -2),  Posi(2, -2),  Posi(2, -1),  Posi(2, 0),   Posi(2, 1),   Posi(2, 2),   Posi(1, 2),
			Posi(0, 2),   Posi(-1, 2),  Posi(-2, 2),  Posi(-2, 1),  Posi(-2, 0),  Posi(-2, -1), Posi(-2, -2), Posi(-2, -3), Posi(-1, -3),
			Posi(0, -3),  Posi(1, -3),  Posi(2, -3),  Posi(3, -3),  Posi(3, -2),  Posi(3, -1),  Posi(3, 0),   Posi(3, 1),   Posi(3, 2),
			Posi(3, 3),   Posi(2, 3),   Posi(1, 3),   Posi(0, 3),   Posi(-1, 3),  Posi(-2, 3),  Posi(-3, 3),  Posi(-3, 2),  Posi(-3, 1),
			Posi(-3, 0),  Posi(-3, -1), Posi(-3, -2), Posi(-3, -3), Posi(-3, -4), Posi(-2, -4), Posi(-1, -4), Posi(0, -4),  Posi(1, -4),
			Posi(2, -4),  Posi(3, -4),  Posi(4, -4),  Posi(4, -3),  Posi(4, -2),  Posi(4, -1),  Posi(4, 0),   Posi(4, 1),   Posi(4, 2),
			Posi(4, 3),   Posi(4, 4),   Posi(3, 4),   Posi(2, 4),   Posi(1, 4),   Posi(0, 4),   Posi(-1, 4),  Posi(-2, 4),  Posi(-3, 4),
			Posi(-4, 4),  Posi(-4, 3),  Posi(-4, 2),  Posi(-4, 1),  Posi(-4, 0),  Posi(-4, -1), Posi(-4, -2), Posi(-4, 3),  Posi(-4, -4)
		};

		static int MAX_POS_COUNT = sizeof(POS_OFFSET_LIST) / sizeof(POS_OFFSET_LIST[0]);
		static bool is_init = false;
		if (!is_init)
		{
			is_init = true;
			for (int i = 0; i < MAX_POS_COUNT; i++)
			{
				POS_OFFSET_LIST[i].x *= extra_dis;
				POS_OFFSET_LIST[i].y *= extra_dis;
			}
		}

		// 最多20次，找到一个有效位置
		for (int i = 0; i < 20; i++)
		{
			*begin_idx = (*begin_idx) % MAX_POS_COUNT;
			out_pos.x = pos.x + POS_OFFSET_LIST[*begin_idx].x;
			out_pos.y = pos.y + POS_OFFSET_LIST[*begin_idx].y;
			(*begin_idx)++;

			if (m_scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_GATHER,out_pos.x,out_pos.y))
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

void SpecialCrossDarkNight::RobBoxOnRelive(Role * dead_role)
{
	if (nullptr == dead_role)
	{
		return;
	}

	UniqueUserID dead_uuid = dead_role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossDarkNightPlayerInfo * dead_dnpl = GetPlayerInfo(dead_uuid);
	std::map<UniqueUserID, UniqueUserID>::iterator itr = m_deader_killer_map.find(dead_uuid);
	if (m_deader_killer_map.end() == itr)
	{
		return;
	}
	CrossDarkNightPlayerInfo * kill_dnpl = GetPlayerInfo(itr->second);

	if (nullptr != dead_dnpl && nullptr != kill_dnpl && dead_dnpl->box_count > 0)
	{
		int temp_cout = (int)ceil(dead_dnpl->box_count * 0.5);

		dead_dnpl->box_count -= temp_cout;
		if (dead_dnpl->box_count < 0)
		{
			dead_dnpl->box_count = 0;
		}

		kill_dnpl->box_count += temp_cout;

		m_deader_killer_map.erase(itr);

		this->OnUpdateRank(dead_dnpl->uuid, dead_dnpl->box_count);
		this->OnUpdateRank(kill_dnpl->uuid, kill_dnpl->box_count);

		this->BroadcastPlayerInfo(kill_dnpl->obj_id);
		this->BroadcastPlayerInfo(dead_dnpl->obj_id);
	}
}

CrossDarkNightBossInfo * SpecialCrossDarkNight::GetBossInfo(int monster_id)
{
	if (monster_id <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; ++i)
	{
		if (m_boss_info[i].monster_id == monster_id)
		{
			return &m_boss_info[i];
		}
	}
	return NULL;
}