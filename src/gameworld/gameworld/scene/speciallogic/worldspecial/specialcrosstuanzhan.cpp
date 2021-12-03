#include "specialcrosstuanzhan.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "engineadapter.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "internalcomm.h"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/attribute.hpp"
#include "effect/effectbase.hpp"
#include <limits>
#include "servercommon/string/crossstr.h"
#include "servercommon/servercommon.h"
#include "world.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/cross/rolecross.h"
#include <functional>
#include "item/knapsack.h"
#include "protocal/msgfight.h"
#include "servercommon/errornum.h"
#include "other/shenyin/shenyin.hpp"
#include "item/itemextern.hpp"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"
#include "globalconfig/otherconfig.h"
#include "config/logicconfigmanager.hpp"
#include "protocal/msgrole.h"

bool operator<(const CrossTuanzhanPlayerInfo &player_info1, const CrossTuanzhanPlayerInfo &player_info2)
{
	if (player_info1.score == player_info2.score)
	{
		return (player_info1.kill_num < player_info2.kill_num);
	}

	return (player_info1.score < player_info2.score);
}

bool operator>(const CrossTuanzhanPlayerInfo &player_info1, const CrossTuanzhanPlayerInfo &player_info2)
{
	if (player_info1.score == player_info2.score)
	{
		return (player_info1.kill_num > player_info2.kill_num);
	}

	return (player_info1.score > player_info2.score);
}


void CrossTuanzhanSideInfo::SidePlayerNumChange(int side, int change_value)
{
	if (side < CROSS_TUANZHAN_SIDE_MIN || side >= CROSS_TUANZHAN_SIDE_MAX) return;
	if (0 == change_value) return;

	side_player_num_list[side] += change_value;

	if (side_player_num_list[side] <= 0)
	{
		side_player_num_list[side] = 0;
	}
}

void CrossTuanzhanSideInfo::SideScoreChange(int side, int change_value)
{
	if (side < CROSS_TUANZHAN_SIDE_MIN || side >= CROSS_TUANZHAN_SIDE_MAX) return;
	if (change_value <= 0) return; // 暂无减积分的需求，过滤掉

	side_score_list[side] += change_value;
}

void CrossTuanzhanSideInfo::RecalWinnerSide()
{
	UNSTD_STATIC_CHECK(2 == CROSS_TUANZHAN_SIDE_MAX);

	if (side_score_list[CROSS_TUANZHAN_SIDE_0] == side_score_list[CROSS_TUANZHAN_SIDE_1])
	{
		winner_side = CROSS_TUANZHAN_SIDE_MAX;
	}
	else
	{
		if (side_score_list[CROSS_TUANZHAN_SIDE_0] > side_score_list[CROSS_TUANZHAN_SIDE_1]) 
		{
			winner_side = CROSS_TUANZHAN_SIDE_0;
		}
		else
		{
			winner_side = CROSS_TUANZHAN_SIDE_1;
		}
	}
}

int CrossTuanzhanSideInfo::GetMinPlayerNumSide() const
{
	UNSTD_STATIC_CHECK(2 == CROSS_TUANZHAN_SIDE_MAX);

	int min_num_side = CROSS_TUANZHAN_SIDE_0;
	if (side_player_num_list[CROSS_TUANZHAN_SIDE_0] != side_player_num_list[CROSS_TUANZHAN_SIDE_1])
	{
		if (side_player_num_list[CROSS_TUANZHAN_SIDE_0] < side_player_num_list[CROSS_TUANZHAN_SIDE_1]) 
		{
			min_num_side = CROSS_TUANZHAN_SIDE_0;
		}
		else
		{
			min_num_side = CROSS_TUANZHAN_SIDE_1;
		}
	}

	return min_num_side;
}

SpecialCrossTuanzhan::SpecialCrossTuanzhan(Scene *scene)
: SpecialLogic(scene), m_activity_open(false), m_fighting(false), m_next_add_static_score_time(0), m_next_add_pilla_score_time(0), m_next_send_rank_data_time(0),
m_next_rand_user_side_time(0), m_last_broacast_time(0), m_fight_start_time(0), m_activity_end_time(0), m_player_rank_list_count(0)
{

}

SpecialCrossTuanzhan::~SpecialCrossTuanzhan()
{

}

void SpecialCrossTuanzhan::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_activity_open)
	{
		return;
	}

	if (m_next_send_rank_data_time > 0 && now_second >= m_next_send_rank_data_time)
	{
		this->SendSideInfoToPlayer();
		this->SendRankListToPlayer();
		m_next_send_rank_data_time = now_second + CROSS_TUANZHAN_NOTIFY_SCORE_RANK_LIST_TIME_S;
	}

	if (m_next_add_static_score_time > 0 && now_second >= m_next_add_static_score_time)
	{
		this->AddStaticScore();
		m_next_add_static_score_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().scene_add_score_time;
	}

	if (m_next_add_pilla_score_time > 0 && now_second >= m_next_add_pilla_score_time)
	{
		this->AddPillaScore();
		m_next_add_pilla_score_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().pillar_time;
	}

	if (m_last_broacast_time > 0)
	{
		if (m_next_rand_user_side_time > 0 && m_last_broacast_time < m_next_rand_user_side_time - 10 && now_second >= m_next_rand_user_side_time - 10)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_tuanzhan_broast_camp_change, 10);
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}

		if (m_next_rand_user_side_time > 0 && m_last_broacast_time < m_next_rand_user_side_time - 5 && now_second >= m_next_rand_user_side_time - 5)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_tuanzhan_broast_camp_change, 5);
			if (length > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
			}
		}

		m_last_broacast_time = now_second;
	}

	if (m_next_rand_user_side_time > 0 && now_second >= m_next_rand_user_side_time)
	{
		this->OnRecalcUserSide();
		m_next_rand_user_side_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().rand_user_side_time;
		this->SendActivityStateToPlayer();

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_tuanzhan_broast_camp_switch);
		if (length > 0)
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length);
		}
	}

	if (!m_fighting && m_fight_start_time > 0 && now_second >= m_fight_start_time)
	{
		m_fighting = true;

		m_next_add_static_score_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().scene_add_score_time;
		m_next_add_pilla_score_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().pillar_time;
		m_next_rand_user_side_time = now_second + CrossTuanzhanConfig::Instance().GetOtherCfg().rand_user_side_time;
		m_next_send_rank_data_time = now_second + CROSS_TUANZHAN_NOTIFY_SCORE_RANK_LIST_TIME_S;
		m_last_broacast_time = now_second;
	}
}

void SpecialCrossTuanzhan::NotifySystemMsg(char *str_buff, int str_len)
{
	if (NULL != str_buff && str_len > 0)
	{
		int sendlen = 0;
		Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, SYS_MSG_ACTIVE_NOTICE))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialCrossTuanzhan::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	CrossTuanzhanPlayerInfo *character_info = this->GetPlayerInfo(cha->GetId());
	CrossTuanzhanPlayerInfo *attacker_info = this->GetPlayerInfo(enemy_id);
	if (NULL != character_info && NULL != attacker_info)
	{
		character_info->attack_user_map[attacker_info->uuid] = EngineAdapter::Instance().Time();
	}
}

void SpecialCrossTuanzhan::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (!m_activity_open || !m_fighting)
	{
		return;
	}

	if (NULL == dead_cha) return;

	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())
	{
		const CrossTuanzhanOtherCfg &other_cfg = CrossTuanzhanConfig::Instance().GetOtherCfg();

		CrossTuanzhanPlayerInfo *killer_info = this->GetPlayerInfo(killer_objid);
		UniqueUserID killer_uuid = INVALID_UNIQUE_USER_ID;

		if (NULL != killer_info)
		{
			killer_uuid = killer_info->uuid;

			// 杀人者数据更新
			++ killer_info->kill_num;
			++ killer_info->dur_kill_num;

			killer_info->dur_been_kill_num = 0;
			killer_info->score += other_cfg.kill_score;
			//m_side_info.SideScoreChange(killer_info->side, other_cfg.kill_score);

			this->RecalDeadRewardBuff(this->GetRole(killer_objid));

			this->UpdateUserRank(killer_info);
			this->BroadcastDurKillInfo(killer_objid, killer_info->kill_num);
			this->SendPlayerInfoToPlayer(this->GetRole(killer_objid));

			static unsigned int kill_num_chuanwen_list[4] = {10, 30, 50, 100}; // 指定击杀数才发传闻

			if (kill_num_chuanwen_list[0] == killer_info->kill_num || kill_num_chuanwen_list[1] == killer_info->kill_num ||
				kill_num_chuanwen_list[2] == killer_info->kill_num || kill_num_chuanwen_list[3] == killer_info->kill_num)
			{
				// 杀人数传闻
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_tuanzhan_kill_num_content,
					UidToInt(killer_info->uuid.user_id), killer_info->name, killer_info->camp, killer_info->kill_num);
				if (length > 0)
				{
					this->NotifySystemMsgToScene(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}

		CrossTuanzhanPlayerInfo *dead_character_info = this->GetPlayerInfo(dead_cha->GetId());
		if (NULL != dead_character_info)
		{
			// 被杀者数据更新
			++ dead_character_info->been_kill_num;
			++ dead_character_info->dur_been_kill_num;

			dead_character_info->dur_kill_num = 0;

			this->BroadcastDurKillInfo(dead_cha->GetId(), dead_character_info->dur_kill_num);
			this->SendPlayerInfoToPlayer(static_cast<Role*>(dead_cha));

			// 助攻者数据更新
			time_t valid_attack_time = EngineAdapter::Instance().Time() - CROSS_TUANZHAN_ASSIST_VALID_TIME_S;

			for (CrossTuanzhanPlayerInfo::AttackerTimeMapIt attacker_it = dead_character_info->attack_user_map.begin(); attacker_it != dead_character_info->attack_user_map.end(); ++ attacker_it)
			{
				if (attacker_it->second >= valid_attack_time)
				{
					CrossTuanzhanPlayerInfo *assistor = this->GetPlayerInfo(attacker_it->first);
					if (NULL != assistor && assistor->uuid != killer_uuid)
					{
						++ assistor->assist_kill_num;
						assistor->score += other_cfg.assist_score;
						//m_side_info.SideScoreChange(assistor->side, other_cfg.assist_score);

						this->UpdateUserRank(assistor);
						this->SendPlayerInfoToPlayer(this->GetRole(assistor->obj_id));
					}
				}
			}
			dead_character_info->attack_user_map.clear();
		}
	}
	else if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		CrossTuanzhanPlayerInfo *killer_info = this->GetPlayerInfo(killer_objid);
		CrossTuanzhanPillaInfo *pilla_info = this->GetPillaInfo(dead_cha->GetId());

		if (NULL != killer_info && NULL != pilla_info)
		{
			pilla_info->owner_side = killer_info->side;
			pilla_info->owner_unique_user_id = killer_info->uuid;

			int copy_len = sizeof(pilla_info->owner_name);
			strncpy(pilla_info->owner_name, killer_info->name, copy_len);
			pilla_info->owner_name[copy_len - 1] = '\0';

			const CrossTuanzhanPillaCfg &pilla_cfg = CrossTuanzhanConfig::Instance().GetPillaCfg();
			int monster_id = (0 == killer_info->side) ? pilla_cfg.pilla_list[pilla_info->index].pilla_side_0 : pilla_cfg.pilla_list[pilla_info->index].pilla_side_1;

			Monster *pilla = MONSTERPOOL->CreateMonster(monster_id, this->GetScene(), pilla_info->position);
			if (NULL != pilla)
			{
				pilla_info->monster_id = pilla->GetMonsterId();
				pilla_info->obj_id = pilla->GetId();
			}

			this->SendPillaInfoToToAllPlayer(pilla_info);

			// 占领柱子传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_tuanzhan_occupy_pilla_content,
				killer_info->side, UidToInt(killer_info->uuid.user_id), killer_info->name, killer_info->camp);
			if (length > 0)
			{
				this->NotifySystemMsgToScene(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
			}
		}
	}

}

void SpecialCrossTuanzhan::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *add_role = (Role *)obj;
		UniqueUserID uuid = add_role->GetRoleCross()->GetCrossOriginalUniqueUserID();

		CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(uuid);
		if (NULL == player_info) // 新来的玩家
		{
			player_info = &m_user_info_cache[uuid];
			player_info->uuid = uuid;
			player_info->camp = add_role->GetCamp();
			player_info->role_level = add_role->GetLevel();
			
			int copy_len = sizeof(player_info->name);
			strncpy(player_info->name, add_role->GetName(), copy_len);
			player_info->name[copy_len - 1] = '\0';
		}

		if (player_info->side == CROSS_TUANZHAN_SIDE_MAX)
		{
			player_info->side = m_side_info.GetMinPlayerNumSide(); // 分配到人数最少的阵营
		}
		m_side_info.SidePlayerNumChange(player_info->side, 1);

		player_info->obj_id = obj->GetId();

		this->RecalDeadRewardBuff(add_role);
		add_role->Recover();

		if (player_info->hp_on_leave_scene > 0)
		{
			if (player_info->hp_on_leave_scene > add_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP))
			{
				player_info->hp_on_leave_scene = add_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			}

			Attribute add_hp = player_info->hp_on_leave_scene - add_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			add_role->ChangeHp(&add_hp, true);
		}

		this->SendActivityStateToPlayer(add_role);
		this->SendPlayerInfoToPlayer(add_role);
		this->SendAllPillaInfoToPlayer(add_role);
		this->SendSideInfoToPlayer(add_role);
		this->SendRankListToPlayer(add_role);
	}
}

void SpecialCrossTuanzhan::OnRecalcUserSide()
{
	m_side_info.Reset();

	UserInfoMap in_scene_user_map;
	in_scene_user_map.clear();

	for (UserInfoMapIt user_info_it = m_user_info_cache.begin(); user_info_it != m_user_info_cache.end(); ++user_info_it)
	{
		Role *role = this->GetRole(user_info_it->second.obj_id);
		if (NULL != role)
		{
			in_scene_user_map[user_info_it->first] = user_info_it->second;
		}
		else
		{
			user_info_it->second.side = CROSS_TUANZHAN_SIDE_MAX;
		}
	}

	const int map_size = static_cast<int>(in_scene_user_map.size());
	for (int i = 0; i < map_size; ++i)
	{
		UserInfoMapIt it = in_scene_user_map.begin();
		int rand_index = RandomNum(static_cast<int>(in_scene_user_map.size()));
		std::advance(it, rand_index);

		UserInfoMapIt find_it = m_user_info_cache.find(it->second.uuid);
		if (m_user_info_cache.end() != find_it)
		{
			find_it->second.side = m_side_info.GetMinPlayerNumSide();
			m_side_info.SidePlayerNumChange(find_it->second.side, 1);

			Role *role = this->GetRole(find_it->second.obj_id);
			if (NULL != role)
			{
				this->SendPlayerInfoToPlayer(role, true);
			}
		}
	}

	{
		// 删除所有通天柱
		for (int index = 0; index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++index)
		{
			Monster *pilla = static_cast<Monster*>(m_scene->GetObj(m_pilla_list[index].obj_id));
			if (NULL != pilla && Obj::OBJ_TYPE_MONSTER == pilla->GetObjType())
			{
				pilla->ForceSetDead();
			}

			m_pilla_list[index].Reset();
		}
	}

	{
		// 创建通天柱
		const CrossTuanzhanPillaCfg &pilla_cfg = CrossTuanzhanConfig::Instance().GetPillaCfg();
		for (int index = 0; index < pilla_cfg.pilla_count && index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++index)
		{
			Monster *pilla = MONSTERPOOL->CreateMonster(pilla_cfg.pilla_list[index].pilla_id, this->GetScene(), pilla_cfg.pilla_list[index].pilla_pos);
			if (NULL != pilla)
			{
				CrossTuanzhanPillaInfo &pilla_info = m_pilla_list[index];
				pilla_info.index = pilla_cfg.pilla_list[index].index;
				pilla_info.monster_id = pilla->GetMonsterId();
				pilla_info.obj_id = pilla->GetId();
				pilla_info.position = pilla_cfg.pilla_list[index].pilla_pos;

				this->SendPillaInfoToToAllPlayer(&pilla_info);
			}
		}
	}
}

bool SpecialCrossTuanzhan::SpecialRelive(Role *role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		const Posi enter_pos = CrossTuanzhanConfig::Instance().GetRandomRelivePos();
		if (!role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 10, enter_pos))
		{
			return false;
		}

		this->RecalDeadRewardBuff(role, SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER);
	}
	else
	{
		// 每天免费复活
		bool is_guild_times_relive = false;
		CommonDataParam *common_p = role->GetCommonDataParam();
		if (common_p != NULL)
		{
			const OtherOtherConfig &other_cfg = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg();
			if (common_p->today_use_free_relive_num < other_cfg.today_free_relive_num)
			{
				common_p->today_use_free_relive_num++;

				role->SyncCommonInfo(Protocol::SCIT_TODAY_FREE_RELIVE_NUM, common_p->today_use_free_relive_num);

				is_guild_times_relive = true;
			}
		}

		if (!is_guild_times_relive)
		{
			// 计算消耗
			static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
			{
				const ItemID REALIVE_ITEM_ID = GLOBALCONFIG->GetRealiveItemId();

				ItemExtern::ItemConsumeConfig stuff;
				stuff.item_id = REALIVE_ITEM_ID;
				stuff.num = 1;
				stuff.buyable = true;

				if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list, true))
				{
					return false;
				}
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "SpecialCrossTuanzhan::SpecialRelive"))
			{
				return false;
			}
		}

		// 生效
		{
			role->AddOrResetDayRevivalTimes(false);
			role->ReAlive(100, 100, role->GetPos(), true);
		}

		this->RecalDeadRewardBuff(role, 100);
	}

	return true;
}

void SpecialCrossTuanzhan::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role) return;

	UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(uuid);

	if (NULL != player_info)
	{
		player_info->obj_id = INVALID_OBJ_ID;
		player_info->hp_on_leave_scene = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		m_side_info.SidePlayerNumChange(player_info->side, -1);
	}

	{
		Protocol::SCReturnOriginalServer ros;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ros, sizeof(ros));
	}
}

bool SpecialCrossTuanzhan::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj)
	{
		return false;
	}

	if (!m_fighting)
	{
		return false;
	}

	bool is_enemy = false;

	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			const CrossTuanzhanPlayerInfo *player_info1 = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			const CrossTuanzhanPlayerInfo *player_info2 = this->GetPlayerInfo(obj->GetId());
			Role *target_role = (Role*)obj;

			if (NULL != player_info1 && NULL != player_info2)
			{
				bool is_friend = (player_info1->side == player_info2->side);
				is_enemy = target_role->IsAlive() && !is_friend;
			}
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			const CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			CrossTuanzhanPillaInfo *pilla_info = this->GetPillaInfo(obj->GetId());
			Monster *monster = (Monster *)obj;

			if (NULL != pilla_info && NULL != player_info)
			{
				bool is_friend = (player_info->side == pilla_info->owner_side);
				is_enemy = monster->IsAlive() && !is_friend;
			}
		}
		break;
	}

	return is_enemy;
}

bool SpecialCrossTuanzhan::SpecialCanPerformSkill(Role *role)
{
	if (m_fighting)
	{
		return true;
	}

	return false;
}

bool SpecialCrossTuanzhan::SpecialCanSupplyEffect(Role *role, int supply_type)
{
	if (SUPPLY_TYPE_ROLE_HP == supply_type)
	{
		return false;
	}

	return true;
}

int SpecialCrossTuanzhan::GetSpecialParam(Role *role)
{
	if (NULL != role)
	{
		const CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		if (NULL != player_info)
		{
			return player_info->side * 10000 + player_info->kill_num;
		}
	}

	return 0;
}

void SpecialCrossTuanzhan::OnFetchScoreReward(Role *role)
{
	CrossTuanzhanPlayerInfo *user_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == user_info)
	{
		return;
	}

	const CrossTuanzhanScoreRewardCfg *final_cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanScoreRewardCfg(user_info->role_level, user_info->score);
	if (NULL != final_cfg)
	{
		if (user_info->score_reward_fetch_seq <= final_cfg->seq)
		{
			const CrossTuanzhanScoreRewardCfg *cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanScoreRewardCfgBySeq(user_info->role_level, user_info->score_reward_fetch_seq);
			if (NULL == cfg)
			{
				return;
			}

			if (cfg->reward_item_count > 0)
			{
				if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->reward_item_count))
				{
					role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return;
				}

				role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_CROSS_TUANZHAN_FETCH);
			}

			// 神印抽奖货币
			if (cfg->reward_currency > 0)
			{
				role->GetShenYin()->AddShenYinChouHunScore(cfg->reward_currency);
			}
			
			if (cfg->cross_honor > 0)
			{
				role->AddCrossHonor(cfg->cross_honor, "SpecialCrossTuanzhan::OnFetchScoreReward");
			}

			user_info->score_reward_fetch_seq++;

			this->SendPlayerInfoToPlayer(role);
		}
	}
}

void SpecialCrossTuanzhan::OnActivityOpen()
{
	if (m_activity_open)
	{
		return;
	}

	{
		// 创建通天柱
		const CrossTuanzhanPillaCfg &pilla_cfg = CrossTuanzhanConfig::Instance().GetPillaCfg();
		for (int index = 0; index < pilla_cfg.pilla_count && index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++ index)
		{
			Monster *pilla = MONSTERPOOL->CreateMonster(pilla_cfg.pilla_list[index].pilla_id, this->GetScene(), pilla_cfg.pilla_list[index].pilla_pos);
			if (NULL != pilla)
			{
				CrossTuanzhanPillaInfo &pilla_info = m_pilla_list[index];
				pilla_info.index = pilla_cfg.pilla_list[index].index;
				pilla_info.monster_id = pilla->GetMonsterId();
				pilla_info.obj_id = pilla->GetId();
				pilla_info.position = pilla_cfg.pilla_list[index].pilla_pos;
			}
		}
	}

	m_fight_start_time = EngineAdapter::Instance().Time() + CrossTuanzhanConfig::Instance().GetOtherCfg().scene_standby_dur_s;
	m_activity_end_time = ActivityShadow::Instance().GetCrossActivityEndTime(CROSS_ACTIVITY_TYPE_TUANZHAN);

	m_activity_open = true;
}

bool static PlayerRankSortFunc(const CrossTuanzhanRankPlayerInfo &rank_item_1, const CrossTuanzhanRankPlayerInfo &rank_item_2)
{
	return rank_item_1.score > rank_item_2.score;
}

void SpecialCrossTuanzhan::OnActivityClose()
{
	if (!m_activity_open)
	{
		return;
	}

	{
		this->SendSideInfoToPlayer();
		this->SendRankListToPlayer();
		this->SendResultToAllPlayer();
		this->SyncResultToCross();
	}

	{
		//排行榜奖励
		std::vector<CrossTuanzhanRankPlayerInfo> player_rank_list;
		for (int index = 0; index < m_player_rank_list_count; ++index)
		{
			if (INVALID_UNIQUE_USER_ID != m_player_rank_list[index].uuid)
			{
				player_rank_list.push_back(m_player_rank_list[index]);
			}
		}

		std::sort(player_rank_list.begin(), player_rank_list.end(), &PlayerRankSortFunc);

		int rank = 0;
		for (std::vector<CrossTuanzhanRankPlayerInfo>::iterator it = player_rank_list.begin(); it != player_rank_list.end(); ++it)
		{
			++rank;

			const CrossTuanzhanRankRewardCfg *cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanRankRewardCfg(it->role_level, rank);
			if (NULL != cfg)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CROSS_HONOR, cfg->cross_honor);
				contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_SHENMOZHILI, cfg->reward_currency);

				int item_count = 0;
				for (int i = 0; i < cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list[i].item_id);
					if (NULL != item_base)
					{
						contentparam.item_list[item_count].item_id = cfg->reward_item_list[i].item_id;
						contentparam.item_list[item_count].num = cfg->reward_item_list[i].num;
						contentparam.item_list[item_count].is_bind = (cfg->reward_item_list[i].is_bind) ? 1 : 0;
						contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();

						++item_count;
					}
				}

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_tuanzhan_reward_by_score_mail_content, rank);
				if (length > 0)
				{
					MailRoute::MailToCrossUser(it->uuid, SYSTEM_MAIL_REASON_INVALID, contentparam);

					CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(it->uuid);
					if (NULL != player_info)
					{
						player_info->is_fetch_rank_reward = 1;
					}
				}
			}
		}
	}

	for (UserInfoMapIt user_info_it = m_user_info_cache.begin(); user_info_it != m_user_info_cache.end(); ++user_info_it)
	{
		//排行榜参与奖
		const CrossTuanzhanRankRewardCfg *cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanRankRewardCfg(user_info_it->second.role_level, 999);
		if (NULL != cfg && 1 != user_info_it->second.is_fetch_rank_reward)
		{
			static MailContentParam contentparam; contentparam.Reset();

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CROSS_HONOR, cfg->cross_honor);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_SHENMOZHILI, cfg->reward_currency);

			int item_count = 0;
			for (int i = 0; i < cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list[i].item_id);
				if (NULL != item_base)
				{
					contentparam.item_list[item_count].item_id = cfg->reward_item_list[i].item_id;
					contentparam.item_list[item_count].num = cfg->reward_item_list[i].num;
					contentparam.item_list[item_count].is_bind = (cfg->reward_item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();

					++item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_tuanzhan_reward_by_score_mail_content2);
			if (length > 0)
			{
				MailRoute::MailToCrossUser(user_info_it->second.uuid, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		if (user_info_it->second.score > 0)
		{
			const CrossTuanzhanScoreRewardCfg *final_cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanScoreRewardCfg(user_info_it->second.role_level , user_info_it->second.score);
			if (NULL != final_cfg)
			{
				for (int seq = user_info_it->second.score_reward_fetch_seq; seq <= final_cfg->seq; ++seq)
				{
					const CrossTuanzhanScoreRewardCfg *cfg = CrossTuanzhanConfig::Instance().GetCrossTuanzhanScoreRewardCfgBySeq(user_info_it->second.role_level, seq);
					if (NULL == cfg) continue;

					static MailContentParam contentparam; contentparam.Reset();
					// 神印抽奖货币
					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CROSS_HONOR, cfg->cross_honor);
					contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_SHENMOZHILI, cfg->reward_currency);

					int item_count = 0;
					for (int i = 0; i < cfg->reward_item_count && item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
					{
						const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list[i].item_id);
						if (NULL != item_base)
						{
							contentparam.item_list[item_count].item_id = cfg->reward_item_list[i].item_id;
							contentparam.item_list[item_count].num = cfg->reward_item_list[i].num;
							contentparam.item_list[item_count].is_bind = (cfg->reward_item_list[i].is_bind) ? 1 : 0;
							contentparam.item_list[item_count].invalid_time = item_base->CalInvalidTime();

							++item_count;
						}
					}

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_tuanzhan_reward_by_score_unfetch_content);
					if (length > 0)
					{
						MailRoute::MailToCrossUser(user_info_it->first, SYSTEM_MAIL_REASON_INVALID, contentparam);
						user_info_it->second.score_reward_fetch_seq++;
					}
				}
			}
		}
	}

	{
		// 删除所有通天柱
		for (int index = 0; index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++index)
		{
			Monster *pilla = static_cast<Monster*>(m_scene->GetObj(m_pilla_list[index].obj_id));
			if (NULL != pilla && Obj::OBJ_TYPE_MONSTER == pilla->GetObjType())
			{
				pilla->ForceSetDead();
			}

			m_pilla_list[index].Reset();
		}
	}

	{
		// 重置排行榜
		for (int index = 0; index < CROSS_TUANZHAN_RANK_LIST_SIZE; ++index)
		{
			m_player_rank_list[index].Reset();
		}

		m_player_rank_list_count = 0;
	}

	m_side_info.Reset();
	m_user_info_cache.clear();

	m_fight_start_time = 0;
	m_activity_end_time = 0;
	m_next_add_static_score_time = 0;
	m_next_add_pilla_score_time = 0;
	m_next_send_rank_data_time = 0;

	m_fighting = false;
	m_activity_open = false;
}

void SpecialCrossTuanzhan::AddStaticScore()
{
	unsigned int add_score = static_cast<unsigned int>(CrossTuanzhanConfig::Instance().GetOtherCfg().static_score);

	for (UserInfoMapIt user_info_it = m_user_info_cache.begin(); user_info_it != m_user_info_cache.end(); ++ user_info_it)
	{
		if (user_info_it->second.obj_id != INVALID_OBJ_ID)
		{
			user_info_it->second.score += add_score;
			//m_side_info.SideScoreChange(user_info_it->second.side, add_score);

			this->UpdateUserRank(&(user_info_it->second));
			this->SendPlayerInfoToPlayer(this->GetRole(user_info_it->second.obj_id));
		}
	}
}

void SpecialCrossTuanzhan::AddPillaScore()
{
	unsigned int pilla_owner_add_score = static_cast<unsigned int>(CrossTuanzhanConfig::Instance().GetOtherCfg().pillar_player_score);
	unsigned int pilla_owner_side_add_score = static_cast<unsigned int>(CrossTuanzhanConfig::Instance().GetOtherCfg().pillar_teammate_score);

	for (int pilla_index = 0; pilla_index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++ pilla_index)
	{
		int pilla_side = m_pilla_list[pilla_index].owner_side;
		UniqueUserID owner_uuid = m_pilla_list[pilla_index].owner_unique_user_id;

		if (CROSS_TUANZHAN_SIDE_MAX != pilla_side) // 柱子被人占领了
		{
			for (UserInfoMapIt user_info_it = m_user_info_cache.begin(); user_info_it != m_user_info_cache.end(); ++ user_info_it)
			{
				if (user_info_it->second.obj_id != INVALID_OBJ_ID && user_info_it->second.side == pilla_side)
				{
					if (user_info_it->second.uuid == owner_uuid) // 占领柱子的人加积分
					{
						user_info_it->second.score += pilla_owner_add_score;
						//m_side_info.SideScoreChange(pilla_side, pilla_owner_add_score);
					}
					else // 占领柱子的阵营其他玩家加积分
					{
						user_info_it->second.score += pilla_owner_side_add_score;
						//m_side_info.SideScoreChange(pilla_side, pilla_owner_side_add_score);
					}

					this->SendPlayerInfoToPlayer(this->GetRole(user_info_it->second.obj_id));
					this->UpdateUserRank(&(user_info_it->second));
				}
			}
		}
	}
}

void SpecialCrossTuanzhan::RecalDeadRewardBuff(Role *role, int hp_percent)
{
	// 检查触发连续被杀buff

	if (NULL == role) return;

	CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != player_info)
	{
		int skill_id = GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_CROSS_TUANZHAN, 0);
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, skill_id);

		const CrossTuanzhanOtherCfg &other_cfg = CrossTuanzhanConfig::Instance().GetOtherCfg();
		if (other_cfg.weak_buff_hp_percent > 0 && player_info->dur_been_kill_num >= (unsigned int)other_cfg.weak_buff_need_been_kill)
		{
			int add_percent = other_cfg.weak_buff_hp_percent;
			long long add_maxhp = (long long)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * (add_percent / 100.0f));

			EffectBuff *buff = new EffectBuff(INVALID_OBJ_ID, skill_id, 3600 * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp, add_percent);
			buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			buff->SetClearOnDie(true);

			role->AddEffect(buff);

			if (hp_percent > 0)
			{
				role->Recover();
				Attribute max_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = static_cast<Attribute>(max_hp * (hp_percent / 100.0));
				Attribute del_hp = cur_hp - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				role->ChangeHp(&del_hp, true);
			}
		}
	}

}

void SpecialCrossTuanzhan::UpdateUserRank(const CrossTuanzhanPlayerInfo *player_info)
{
	if (NULL == player_info) return;

	for (int index = 0; index < m_player_rank_list_count; ++ index)
	{
		CrossTuanzhanRankPlayerInfo &rank_item = m_player_rank_list[index];
		if (rank_item.uuid == player_info->uuid) // 已在排行榜里，直接更新数据
		{
			rank_item.score = player_info->score;
			rank_item.side = player_info->side;

			return;
		}
	}

	if (m_player_rank_list_count < CROSS_TUANZHAN_RANK_LIST_SIZE)
	{
		CrossTuanzhanRankPlayerInfo &rank_item = m_player_rank_list[m_player_rank_list_count];
		rank_item.uuid = player_info->uuid;
		rank_item.side = player_info->side;
		rank_item.score = player_info->score;
		rank_item.role_level = player_info->role_level;

		int copy_len = sizeof(rank_item.name);
		strncpy(rank_item.name, player_info->name, copy_len);
		rank_item.name[copy_len - 1] = '\0';

		++ m_player_rank_list_count;
	}
	else
	{
		int min_score_index = 0;
		unsigned int min_score = UINT_MAX;

		// 找到积分最少的玩家下标
		for (int index = 0; index < m_player_rank_list_count && index < CROSS_TUANZHAN_RANK_LIST_SIZE; ++ index)
		{
			CrossTuanzhanRankPlayerInfo &player_info = m_player_rank_list[index];
			if (player_info.score < (unsigned int)min_score)
			{
				min_score = player_info.score;
				min_score_index = index;
			}
		}

		CrossTuanzhanRankPlayerInfo &min_score_player_info = m_player_rank_list[min_score_index];
		if (player_info->score > min_score_player_info.score)
		{
			min_score_player_info.uuid = player_info->uuid;
			min_score_player_info.side = player_info->side;
			min_score_player_info.score = player_info->score;
			min_score_player_info.role_level = player_info->role_level;

			int copy_len = sizeof(min_score_player_info.name);
			strncpy(min_score_player_info.name, player_info->name, copy_len);
			min_score_player_info.name[copy_len - 1] = '\0';
		}
	}
}

void SpecialCrossTuanzhan::SendActivityStateToPlayer(Role *role)
{
	static Protocol::SCCrossTuanzhanStateNotify state;
	state.fight_start_time = (unsigned int)m_fight_start_time;
	state.activity_end_time = (unsigned int)m_activity_end_time;
	state.rand_side_time = (unsigned int)m_next_rand_user_side_time;

	if (NULL == role)
	{
		m_scene->SendToRole((char*)&state, sizeof(state));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&state, sizeof(state));
	}
}

void SpecialCrossTuanzhan::SendPlayerInfoToPlayer(Role *role, bool is_broacast)
{
	if (NULL == role)
	{
		return;
	}

	CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != player_info)
	{
		static Protocol::SCCrossTuanzhanPlayerInfo tzpi;
		tzpi.obj_id = role->GetId();
		tzpi.is_broacast = is_broacast ? 1 : 0;
		tzpi.side = player_info->side;
		tzpi.score = player_info->score;
		tzpi.kill_num = player_info->kill_num;
		tzpi.dur_kill_num = player_info->dur_kill_num;
		tzpi.assist_kill_num = player_info->assist_kill_num;
		tzpi.score_reward_fetch_seq = player_info->score_reward_fetch_seq;
		
		if (is_broacast)
		{
			m_scene->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&tzpi, sizeof(tzpi));
		}
		else
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&tzpi, sizeof(tzpi));
		}
	}
}

void SpecialCrossTuanzhan::SendAllPillaInfoToPlayer(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCCrossTuanzhanPillaInfo pilla_info;
	pilla_info.pilla_list_count = 0;

	for (int pilla_index = 0; pilla_index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++ pilla_index)
	{
		if (pilla_info.pilla_list_count < CROSS_TUANZHAN_PILLA_MAX_COUNT)
		{
			Protocol::SCCrossTuanzhanPillaInfo::PillaInfo &proto_info = pilla_info.pilla_list[pilla_info.pilla_list_count];
			proto_info.monster_id = m_pilla_list[pilla_index].monster_id;
			proto_info.obj_id = m_pilla_list[pilla_index].obj_id;
			proto_info.owner_side = m_pilla_list[pilla_index].owner_side;
			proto_info.index = m_pilla_list[pilla_index].index;

			int copy_len = sizeof(proto_info.owner_name);
			strncpy(proto_info.owner_name, m_pilla_list[pilla_index].owner_name, copy_len);
			proto_info.owner_name[copy_len - 1] = '\0';

			++ pilla_info.pilla_list_count;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&pilla_info, sizeof(pilla_info));
}

void SpecialCrossTuanzhan::SendPillaInfoToToAllPlayer(const CrossTuanzhanPillaInfo *info)
{
	if (NULL == info)
	{
		return;
	}

	static Protocol::SCCrossTuanzhanPillaInfo pilla_info;
	pilla_info.pilla_list_count = 0;

	{
		Protocol::SCCrossTuanzhanPillaInfo::PillaInfo &proto_info = pilla_info.pilla_list[pilla_info.pilla_list_count];
		proto_info.monster_id = info->monster_id;
		proto_info.obj_id = info->obj_id;
		proto_info.owner_side = info->owner_side;
		proto_info.index = info->index;

		int copy_len = sizeof(proto_info.owner_name);
		strncpy(proto_info.owner_name, info->owner_name, copy_len);
		proto_info.owner_name[copy_len - 1] = '\0';

		++ pilla_info.pilla_list_count;
	}

	m_scene->SendToRole((char*)&pilla_info, sizeof(pilla_info));
}

void SpecialCrossTuanzhan::SendSideInfoToPlayer(Role *role)
{
// 	// 阵营积分排行
// 	static Protocol::SCCrossTuanzhanSideInfo side_info;
// 	memcpy(side_info.side_score_list, m_side_info.side_score_list, sizeof(side_info.side_score_list));
// 
// 	if (NULL == role)
// 	{
// 		m_scene->SendToRole((char*)&side_info, sizeof(side_info));
// 	}
// 	else
// 	{
// 		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&side_info, sizeof(side_info));
// 	}
}

void SpecialCrossTuanzhan::SendRankListToPlayer(Role *role)
{
	// 玩家积分排行
	static Protocol::SCCrossTuanzhanRankInfo rank_info;
	rank_info.rank_list_count = m_player_rank_list_count;

	memset(rank_info.rank_list, 0, sizeof(rank_info.rank_list));
	for (int index = 0; index < m_player_rank_list_count && index < CROSS_TUANZHAN_RANK_LIST_SIZE; ++ index)
	{
		rank_info.rank_list[index].side = m_player_rank_list[index].side;
		rank_info.rank_list[index].score = m_player_rank_list[index].score;

		int copy_len = sizeof(rank_info.rank_list[index].name);
		strncpy(rank_info.rank_list[index].name, m_player_rank_list[index].name, copy_len);
		rank_info.rank_list[index].name[copy_len - 1] = '\0';

		CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(m_player_rank_list[index].uuid);
		if (NULL != player_info)
		{
			rank_info.rank_list[index].kill = player_info->kill_num;
			rank_info.rank_list[index].assist = player_info->assist_kill_num;
		}
	}

	int send_len = sizeof(rank_info) - (CROSS_TUANZHAN_RANK_LIST_SIZE - rank_info.rank_list_count) * sizeof(rank_info.rank_list[0]);
	if (NULL == role)
	{
		m_scene->SendToRole((char*)&rank_info, send_len);
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&rank_info, send_len);
	}
}

void SpecialCrossTuanzhan::BroadcastDurKillInfo(ObjID obj_id, unsigned int dur_kill_num)
{
	Role *role = this->GetRole(obj_id);
	if (NULL != role)
	{
		static Protocol::SCCrossTuanzhanPlayerDurKillInfo pdki;
		pdki.obj_id = obj_id;
		pdki.dur_kill_num = dur_kill_num;
		m_scene->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&pdki, sizeof(pdki));
	}
}

void SpecialCrossTuanzhan::NotifySystemMsgToScene(char *str_buff, int str_len, int notice_type)
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

void SpecialCrossTuanzhan::SendResultToAllPlayer()
{
	Protocol::SCCrossTuanzhanResultInfo tzri;

	m_side_info.RecalWinnerSide();
	int winner_side = m_side_info.winner_side;

	int scene_role_num = (int)m_scene->RoleNum();
	for (int i = 0; i < scene_role_num; ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			CrossTuanzhanPlayerInfo *player_info = this->GetPlayerInfo(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			if (NULL != player_info)
			{
				tzri.personal_score = player_info->score;
				
				tzri.side_score = 0;
				if (player_info->side >= CROSS_TUANZHAN_SIDE_MIN && player_info->side < CROSS_TUANZHAN_SIDE_MAX)
				{
					tzri.side_score = m_side_info.side_score_list[player_info->side];
				}
				
				tzri.result = 0;
				if (player_info->side == winner_side) 
				{
					tzri.result = 1;
				}

				EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&tzri, sizeof(tzri));
			}
		}
	}

}

void SpecialCrossTuanzhan::SyncResultToCross()
{
// 	m_side_info.RecalWinnerSide();
// 
// 	crossgameprotocal::GameCrossTuanzhanSyncFightResult fight_result;
// 	std::vector<CrossTuanzhanPlayerInfo> side_player_list[CROSS_TUANZHAN_SIDE_MAX];
// 
// 	for (UserInfoMapIt info_it = m_user_info_cache.begin(); info_it != m_user_info_cache.end(); ++ info_it)
// 	{
// 		if (info_it->second.side >= CROSS_TUANZHAN_SIDE_MIN && info_it->second.side < CROSS_TUANZHAN_SIDE_MAX)
// 		{
// 			side_player_list[info_it->second.side].push_back(info_it->second);
// 		}
// 	}
// 
// 	for (int side = CROSS_TUANZHAN_SIDE_MIN; side < CROSS_TUANZHAN_SIDE_MAX; ++ side)
// 	{
// 		std::vector<CrossTuanzhanPlayerInfo> &cur_side_player_list = side_player_list[side];
// 		std::sort(cur_side_player_list.begin(), cur_side_player_list.end(), std::greater<CrossTuanzhanPlayerInfo>());
// 
// 		int side_player_len = static_cast<int>(cur_side_player_list.size());
// 		for (int index = 0; index < side_player_len; ++ index)
// 		{
// 			fight_result.uuid = UniqueUserIDToLongLong(cur_side_player_list[index].uuid);
// 			fight_result.score = cur_side_player_list[index].score;
// 			fight_result.side_rank = index + 1;
// 
// 			if (cur_side_player_list[index].side == m_side_info.winner_side) 
// 			{
// 				fight_result.user_is_win = 1;
// 			}
// 			else 
// 			{
// 				fight_result.user_is_win = 0;
// 			}
// 
// 			InternalComm::Instance().SendToCross((const char *)&fight_result, sizeof(fight_result));
// 		}
// 	}
}

Role *SpecialCrossTuanzhan::GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return NULL;
}

CrossTuanzhanPlayerInfo *SpecialCrossTuanzhan::GetPlayerInfo(UniqueUserID uuid)
{
	UserInfoMapIt it = m_user_info_cache.find(uuid);
	if (it != m_user_info_cache.end())
	{
		return &it->second;
	}

	return NULL;
}

CrossTuanzhanPlayerInfo *SpecialCrossTuanzhan::GetPlayerInfo(ObjID obj_id)
{
	Role *role = this->GetRole(obj_id);
	if (NULL != role)
	{
		UniqueUserID uuid = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
		return this->GetPlayerInfo(uuid);
	}

	return NULL;
}

CrossTuanzhanPillaInfo *SpecialCrossTuanzhan::GetPillaInfo(ObjID obj_id)
{
	for (int index = 0; index < CROSS_TUANZHAN_PILLA_MAX_COUNT; ++ index)
	{
		if (m_pilla_list[index].obj_id == obj_id)
		{
			return &m_pilla_list[index];
		}
	}

	return NULL;
}

