#include "specialgiftharvest.hpp"
#include "config/activityconfig/giftharvestconfig.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "internalcomm.h"
#include "gamelog.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/attribute.hpp"
#include "effect/effectbase.hpp"
#include <limits>
#include "servercommon/servercommon.h"
#include "world.h"
#include "gstr.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "servercommon/noticenum.h"
#include "config/logicconfigmanager.hpp"
#include "gameworld/global/randactivity/randactivitymanager.hpp"
#include "gameworld/global/randactivity/impl/randactivitygiftharvest.hpp"
#include "item/itempool.h"

SpecialGiftHarvest::SpecialGiftHarvest(Scene *scene)
	: SpecialLogic(scene)
{
	this->_ResetAllData();
}

SpecialGiftHarvest::~SpecialGiftHarvest()
{

}

void SpecialGiftHarvest::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_activity_open) return;

	if(!m_cur_round_open) return;

	// 围栏区数量
	int fence_cfg_count = LOGIC_CONFIG->GetGiftHarvestConfig().GetFenceCfgCount();

	// 每秒遍历怪物所处范围
	if ((unsigned int)now_second != m_last_check_monster_time) 
	{
		m_last_check_monster_time = (unsigned int)now_second;

		std::set<int> remove_monster_id_set;
		std::set<int> already_has_follow_animal_set;

		int monster_num = static_cast<int>(m_scene->MonsterNum());
		for (int i = 0; i < monster_num; i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (nullptr == monster) continue;

			int get_score = m_monster_info_map[monster->GetMonsterId()].total_get_score;
			if (get_score <= 0) continue;

			// 检查是否被引进安全区里
			const Posi &monster_pos = monster->GetPos();
			for (int index = 0; index < fence_cfg_count; ++index)
			{
				const GiftHarvestFenceConfig *fence_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetFenceCfg(index);
				if (nullptr != fence_cfg && monster_pos.x >= fence_cfg->x1 && monster_pos.x <= fence_cfg->x2 && monster_pos.y >= fence_cfg->y1 && monster_pos.y <= fence_cfg->y2)
				{
					ObjID obj_id = monster->GetMonsterAI()->GetEnemy();
					GiftHarvestPlayerInfo *player_info = this->GetPlayerInfo(obj_id);
					if (nullptr != player_info)
					{
						// 记录积分
						if (player_info->cur_get_score_times < LOGIC_CONFIG->GetGiftHarvestConfig().GetOtherCfg().day_max_get_score_times)
						{
							player_info->cur_get_score += get_score;
							++player_info->cur_get_score_times;

							this->SendPlayerInfoToPlayer(obj_id);
						}
						else
						{
							Role *player = this->_GetRole(obj_id);
							if (nullptr != player)
							{
								player->NoticeNum(errornum::EN_CROSS_PASTURE_GET_SCORE_TIMES_FULL);
							}
						}

						// 排行记录
						if (auto* activity = RandActivityManager::Instance().GetRandActivityGiftHarvest())
						{
							if (Role *player = this->_GetRole(obj_id))
							{
								activity->UpdateRankInfo(player, get_score);
							}
						}
					}

					monster->ForceSetDead();
					remove_monster_id_set.insert(monster->GetMonsterId());
				}
			}

			// 检查是否有多个怪物同时跟着一个玩家
			ObjID enemy_obj_id = monster->GetMonsterAI()->GetEnemy();
			if (already_has_follow_animal_set.find(enemy_obj_id) != already_has_follow_animal_set.end())
			{
				monster->GetMonsterAI()->ForceSetEnemy(INVALID_OBJ_ID);
			}

			already_has_follow_animal_set.insert(enemy_obj_id);
		}

		for (std::set<int>::iterator it = remove_monster_id_set.begin(); it != remove_monster_id_set.end(); ++it)
		{
			this->_FixMonsterNum(*it);
		}

		remove_monster_id_set.clear();
	}
}

void SpecialGiftHarvest::OnAddObj(Obj *obj)
{
	if (nullptr == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *add_role = (Role *)obj;
		UserID uid = add_role->GetUserId();

		// 玩家信息
		GiftHarvestPlayerInfo *player_info = this->GetPlayerInfo(uid);
		if (nullptr == player_info) // 新来的玩家
		{
			player_info = &m_user_info_cache[uid];
			player_info->uid = uid;
			F_STRNCPY(player_info->name, add_role->GetName(), sizeof(player_info->name));
			player_info->cur_get_score = 0;
			player_info->cur_get_score_times = 0;
			player_info->sex = add_role->GetSex();
			player_info->prof = add_role->GetProf();
			player_info->avatar_timestamp = add_role->GetAvatarTimeStamp();

			const GiftHarvestSkillConfig *skill_cfg_list = LOGIC_CONFIG->GetGiftHarvestConfig().GetSkillCfgList();
			for (int index = 0; index < GIFT_HARVEST_SKILL_CFG_MAX_COUNT; ++index)
			{
				player_info->skill_list[index].skill_id = skill_cfg_list[index].skill_id;
				player_info->skill_list[index].next_can_perform_time = 0;
			}

			gamelog::g_log_cross.printf(LL_INFO, "SpecialGiftHarvest First Join user[%d,%s] uid[%d] plat_name[%s]",
				add_role->GetUID(), add_role->GetName(), UidToInt(uid), add_role->GetPlatName());
		}
		else // 回来的玩家
		{

		}

		// 重置位置
		//Posi role_pos = LOGIC_CONFIG->GetGiftHarvestConfig().GetRandomMonsterCreatePos();
		//int MAX_OFFSET = 40;
		//int x_offset = (rand() % MAX_OFFSET) - (MAX_OFFSET / 2);
		//int y_offset = (rand() % MAX_OFFSET) - (MAX_OFFSET / 2);
		//role_pos.x += x_offset;
		//role_pos.y += y_offset;
		//add_role->SetPos(role_pos);

		// 通知信息
		this->SendPlayerInfoToPlayer(add_role);
	}
}

void SpecialGiftHarvest::OnRemoveObj(Obj *obj)
{
	if (!m_activity_open) return;

	if (nullptr == obj) return;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *mon = (Monster *)obj;
		GiftHarvestMonster &info = m_monster_info_map[mon->GetMonsterId()];
		if (info.cur_num > 0)
		{
			--info.cur_num;
			this->_FixMonsterNum(mon->GetMonsterId());
		}
	}
}

void SpecialGiftHarvest::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (nullptr == role) return;

}

bool SpecialGiftHarvest::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (nullptr == role || nullptr == obj)
	{
		return false;
	}

	// 场景内不能打架
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return true;
}

int SpecialGiftHarvest::SpecialRecalcInjure(Character *attacker, Character *injurer, int injure)
{
	if (nullptr == attacker || nullptr == injurer) return 0;

	// 牧场怪被攻击时只掉1血（不能0血，否则吸引不了仇恨）
	if (Obj::OBJ_TYPE_MONSTER == injurer->GetObjType())
	{
		Monster *mon = (Monster*)injurer;
		if (nullptr != mon && MonsterInitParam::AITYPE_SPECIALAI_GIFT_HARVEST == mon->GetAIType())
		{
			return 1;
		}
	}

	return injure;
}

bool SpecialGiftHarvest::PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id)
{
	if(nullptr == role) return false;

	UserID uid = role->GetUserId();
	GiftHarvestPlayerInfo *player_info = this->GetPlayerInfo(uid);
	if (nullptr == player_info)
	{
		return false;
	}

	if (skill_index < 0 || skill_index >= GIFT_HARVEST_SKILL_CFG_MAX_COUNT)
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(player_info->skill_list[skill_index].skill_id);
	if (nullptr == skill)
	{
		return false;
	}

	Character *target_character = nullptr;
	Obj *target_obj = this->GetScene()->GetObj(target_id);
	if (nullptr != target_obj)
	{
		if (Obj::OBJ_TYPE_ROLE != target_obj->GetObjType() && Obj::OBJ_TYPE_MONSTER != target_obj->GetObjType())
		{
			return false;
		}

		target_character = (Character *)target_obj;

		if ((target_obj->GetPos() - role->GetPos()).IsLongerThan(skill->GetAttackDis()))
		{
			role->NoticeNum(errornum::EN_CROSS_PASTURE_DISTANCE_NOT_ENOUGH);
			return false;
		}
	}

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	if (now_time < player_info->skill_list[skill_index].next_can_perform_time)
	{
		role->NoticeNum(errornum::EN_CROSS_PASTURE_CD_NOT_ENOUGH);
		return false;
	}

	//skill->SendSkillTip(role);
	skill->Perform(role, 1, target_character, pos, 0);

	player_info->skill_list[skill_index].next_can_perform_time = now_time + (unsigned int)skill->GetCoolDown(1);
	this->_SyncSkillInfo(role, &player_info->skill_list[skill_index]);

	return true;
}

void SpecialGiftHarvest::OnActivityStatusChange(int activity_status, int round_staus)
{
	// 活动状态
	switch (activity_status)
	{
	case ACTIVITY_STATUS_OPEN:
	{
		this->OnActivityOpen();
	}
	break;

	case ACTIVITY_STATUS_CLOSE:
	{
		this->OnActivityClose();
	}
	break;
	}

	// 轮次状态
	switch (round_staus)
	{
	case GIFT_HARVEST_ROUND_STATUS_STANDY:
	{
		this->ReadyRound();
	}
	break;

	case GIFT_HARVEST_ROUND_STATUS_OPEN:
	{
		this->StartRound();
	}
	break;

	case GIFT_HARVEST_ROUND_STATUS_END:
	{
		this->DrawJoinReward();
		this->StopRound();
	}
	break;
	}

}

void SpecialGiftHarvest::OnActivityOpen()
{
	if (m_activity_open) return;

	// 重置数据
	this->_ResetAllData();

	// 创建怪物
	{
		m_monster_info_map.clear();

		int monster_count = LOGIC_CONFIG->GetGiftHarvestConfig().GetMonsterInfoCfgCount();
		for (int index = monster_count - 1; index >= 0; --index)
		{
			const GiftHarvestMonsterInfoConfig *monster_info_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetMonsterInfoCfgWithIndex(index);
			if (nullptr == monster_info_cfg) continue;

			GiftHarvestMonster &info = m_monster_info_map[monster_info_cfg->monster_id];
			info.monster_id = monster_info_cfg->monster_id;
			info.total_get_score = monster_info_cfg->score;
			info.max_num = monster_info_cfg->max_num;
			info.cur_num = 0;

			this->_FixMonsterNum(monster_info_cfg->monster_id);
		}
	}

	m_activity_open = true;
}

void SpecialGiftHarvest::OnActivityClose()
{
	if (!m_activity_open) return;

	// 删除所有怪物
	{
		int monster_num = static_cast<int>(m_scene->MonsterNum());
		for (int i = 0; i < monster_num; i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (nullptr != monster)
			{
				monster->ForceSetDead();
			}
		}

		m_monster_info_map.clear();
	}

	// 重置数据
	this->_ResetAllData();

	this->DelayKickOutAllRole();
}


void SpecialGiftHarvest::DrawJoinReward()
{
	const auto& other_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetOtherCfg();
	const ItemConfigData *reward_cfg = other_cfg.reward_item_list;
	
	// 参与奖物品
	static MailContentParam contentparam; contentparam.Reset();
	if (nullptr != reward_cfg)
	{
		for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
		{
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg[j].item_id);
			if (reward_item_base != nullptr && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM)
			{
				contentparam.item_list[contentparam.item_count].item_id = reward_cfg[j].item_id;
				contentparam.item_list[contentparam.item_count].num = reward_cfg[j].num;
				contentparam.item_list[contentparam.item_count].is_bind = (reward_cfg[j].is_bind) ? 1 : 0;
				contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
				++contentparam.item_count;
			}
		}
	}

	int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "rand_activity_gift_harvest_join_mail");

	// 为每个玩家发放
	for (auto& player_info: m_user_info_cache)
	{
		if (length > 0 && player_info.second.cur_get_score > 0)
		{
			MailRoute::MailToUser(player_info.first, SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

void SpecialGiftHarvest::ReadyRound()
{
	if(!m_activity_open) return;

	// 检查怪物是否在圈内
	if (int fence_cfg_count = LOGIC_CONFIG->GetGiftHarvestConfig().GetFenceCfgCount())
	{
		int monster_num = static_cast<int>(m_scene->MonsterNum());
		for (int i = 0; i < monster_num; i++)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (nullptr == monster) continue;

			// 重置仇恨目标
			monster->GetMonsterAI()->ForceSetEnemy(INVALID_OBJ_ID);

			// 检查是否被引进安全区里
			const Posi &monster_pos = monster->GetPos();
			for (int index = 0; index < fence_cfg_count && index < GIFT_HARVEST_FENCE_CFG_MAX_COUNT; ++index)
			{
				const GiftHarvestFenceConfig *fence_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetFenceCfg(index);
				if (nullptr != fence_cfg && monster_pos.x >= fence_cfg->x1 && monster_pos.x <= fence_cfg->x2 && monster_pos.y >= fence_cfg->y1 && monster_pos.y <= fence_cfg->y2)
				{
					monster->ForceSetDead();
				}
			}
		}
	}

	// 固定怪物的数量
	for (auto& monster_info: m_monster_info_map)
	{
		this->_FixMonsterNum(monster_info.second.monster_id);
	}
}

void SpecialGiftHarvest::StartRound()
{
	if(!m_activity_open) return;

	m_cur_round_open = true;
}

void SpecialGiftHarvest::StopRound()
{
	this->_ResetRoundData();
	this->DelayKickOutAllRole();
}


bool SpecialGiftHarvest::CanEnter()
{
	return m_cur_round_open;
}


void SpecialGiftHarvest::SendPlayerInfoToPlayer(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCRAGiftHarvestPlayerInfo msg;

	GiftHarvestPlayerInfo *player_info = this->GetPlayerInfo(role->GetUserId());
	msg.round = 0;

	if (nullptr != player_info)
	{
		msg.get_score = player_info->cur_get_score;
		msg.kill_num = player_info->cur_get_score_times;
	}
	else
	{
		msg.get_score = 0;
		msg.kill_num = 0;
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&msg, sizeof(msg));
}

void SpecialGiftHarvest::SendPlayerInfoToPlayer(ObjID obj_id)
{
	Role *role = this->_GetRole(obj_id);
	this->SendPlayerInfoToPlayer(role);
}

GiftHarvestPlayerInfo *SpecialGiftHarvest::GetPlayerInfo(UserID uid)
{
	UserInfoMapIt it = m_user_info_cache.find(uid);
	if (it != m_user_info_cache.end())
	{
		return &it->second;
	}

	return nullptr;
}

GiftHarvestPlayerInfo *SpecialGiftHarvest::GetPlayerInfo(ObjID obj_id)
{
	Role *role = this->_GetRole(obj_id);
	if (nullptr != role)
	{
		UserID uid = role->GetUserId();
		return this->GetPlayerInfo(uid);
	}

	return nullptr;
}


Role *SpecialGiftHarvest::_GetRole(ObjID obj_id)
{
	Obj *obj = m_scene->GetObj(obj_id);
	if (nullptr != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return  (Role *)obj;
	}

	return nullptr;
}

void SpecialGiftHarvest::_ResetAllData()
{
	m_activity_open = false;
	m_cur_round_open = false;
	m_last_check_rank_time = 0;
	m_last_check_monster_time = 0;

	m_user_info_cache.clear();
	m_monster_info_map.clear();
}

void SpecialGiftHarvest::_ResetRoundData()
{
	m_cur_round_open = false;
	m_user_info_cache.clear();
}

void SpecialGiftHarvest::_FixMonsterNum(int monster_id)
{
	GiftHarvestMonster &info = m_monster_info_map[monster_id];

	int create_num = info.cur_num;
	while (create_num < info.max_num)
	{
		Posi monster_pos = LOGIC_CONFIG->GetGiftHarvestConfig().GetRandomMonsterCreatePos();

		// 坐标做个偏移，避免怪物重叠
		const int MAX_OFFSET = 3 * 2;
		int x_offset = (rand() % MAX_OFFSET) - (MAX_OFFSET / 2);
		int y_offset = (rand() % MAX_OFFSET) - (MAX_OFFSET / 2);
		monster_pos.x += x_offset;
		monster_pos.y += y_offset;

		Monster *monster = MONSTERPOOL->CreateMonster(info.monster_id, this->GetScene(), monster_pos);
		if (nullptr != monster)
		{
			++info.cur_num;
		}

		++create_num;
	}
}

void SpecialGiftHarvest::_SyncSkillInfo(Role *role, GiftHarvestSkillItem *skill_info)
{
	if (nullptr == role || nullptr == skill_info) return;

	static Protocol::SCGiftHarvestSkill skinfo;
	skinfo.skill_index = skill_info->skill_id;
	skinfo.next_can_perform_time = skill_info->next_can_perform_time;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&skinfo, sizeof(skinfo));
}
