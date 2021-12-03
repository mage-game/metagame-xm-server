#include "roleequipfbmanager.hpp"
#include "gameworld/world.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/fb/rolefbequip.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "servercommon/errornum.h"
#include "item/knapsack.h"

RoleEquipFBManager & RoleEquipFBManager::Instance()
{
	static RoleEquipFBManager refbm;
	return refbm;
}

RoleEquipFBManager::RoleEquipFBManager()
{

}

RoleEquipFBManager::~RoleEquipFBManager()
{

}

void RoleEquipFBManager::Update(unsigned long interval)
{
	
}

void RoleEquipFBManager::OnEnterScene(Role *role, int scene_key, int scene_id)
{
	
}

void RoleEquipFBManager::OnLeaveScene(Role *role, int scene_key)
{
	
}

void RoleEquipFBManager::CheckRollState(Role *role)
{
	if (role == nullptr) return;

	RoleRollInfoMapIt info_it = m_role_roll_info_map.find(role->GetUID());
	if (info_it == m_role_roll_info_map.end())
	{
		return;
	}

	int times_left = info_it->second.max_free_roll_times - info_it->second.free_rolled_times;
	if (times_left > 0)
	{
		int reward_item_count = 0;
		NeqRollItemToRole reward_item_list[NEQFB_ROLLPOOL_TOTAL_COUNT];

		for (int i = 0; i < times_left && reward_item_count < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
		{
			int rand_roll_index = this->RandRollItemHelper(info_it->second.max_free_roll_times + info_it->second.gold_rolled_times, info_it->second.hit_list, info_it->second.roll_list);
			if (rand_roll_index < 0 || rand_roll_index >= NEQFB_ROLLPOOL_TOTAL_COUNT)
			{
				break;
			}

			++info_it->second.free_rolled_times;
			info_it->second.hit_list[rand_roll_index] = true;
			reward_item_list[reward_item_count] = info_it->second.roll_list[rand_roll_index];
			++reward_item_count;
		}

		// 发送邮件
		static MailContentParam contentparam; contentparam.Reset();

		for (int i = 0; i < reward_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(reward_item_list[i].item_cfg.item_id);
			if (NULL == item_base) continue;
			if (MAX_ATTACHMENT_ITEM_NUM <= i) break;

			contentparam.item_list[i].item_id = reward_item_list[i].item_cfg.item_id;
			contentparam.item_list[i].num = reward_item_list[i].item_cfg.num;
			contentparam.item_list[i].is_bind = reward_item_list[i].item_cfg.is_bind ? 1 : 0;
			contentparam.item_list[i].invalid_time = item_base->CalInvalidTime();

			gamelog::g_log_fb.printf(LL_INFO, "CheckRollState user[%s %d] GetRollItem[%s]",
				role->GetName(), role->GetUID(), this->RollItemQuickLog(reward_item_list[i]));
		}

		int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_equipfb_compensation_roll_subject);
		int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_equipfb_compensation_roll_context, info_it->second.roll_chapter + 1, info_it->second.roll_level + 1);

		if (length_1 > 0 && length_2 > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	info_it->second.max_free_roll_times = 0; // 这样做标记着这轮翻牌已经结束了，玩家再也不能再继续这轮翻牌
	info_it->second.free_rolled_times = 0;

	if (times_left > 0)
	{
		this->SendNeqRollInfo(role, Protocol::SCNeqRollInfo::RIREASON_END, -1);
	}
}

void RoleEquipFBManager::StartRoll(Role *role, int star, int chapter, int level)
{
	if (role == nullptr || star <= 0 || chapter < 0 || chapter >= NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return;
	}
	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	RoleRollInfo &info = m_role_roll_info_map[role->GetUID()];

	this->CheckRollState(role);

	info.max_free_roll_times = LOGIC_CONFIG->GetNeqFBConfig().GetRollTimes(star);
	info.free_rolled_times = 0;
	info.gold_rolled_times = 0;
	info.roll_chapter = chapter;
	info.roll_level = level;

	memset(info.roll_list, 0, sizeof(info.roll_list));
	memset(info.hit_list, 0, sizeof(info.hit_list));


	bool ret = LOGIC_CONFIG->GetNeqFBConfig().GetRollPool(info.roll_list, lvl_cfg->chapter, lvl_cfg->level);
	if (!ret)
	{
		gamelog::g_log_serious_error.printf(LL_INFO, "RoleEquipFBManager::StartRoll role %d get pool fail", role->GetUID());
		return;
	}

	for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
	{
		info.roll_list[i].item_seq = i;
	}

	Protocol::SCNeqRollPool scnrp;
	for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
	{
		scnrp.roll_item_list[i].item_id = info.roll_list[i].item_cfg.item_id;
		scnrp.roll_item_list[i].is_bind = (info.roll_list[i].item_cfg.is_bind == true ? 1 : 0);
		scnrp.roll_item_list[i].num = info.roll_list[i].item_cfg.num;
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scnrp, sizeof(scnrp));

	gamelog::g_log_fb.printf(LL_INFO, "StartRoll user[%s %d] RollItemList[%s]",
		role->GetName(), role->GetUID(), this->RollItemListQuickLog(info.roll_list));

	this->SendNeqRollInfo(role, Protocol::SCNeqRollInfo::RIREASON_START, -1);
}

char * RoleEquipFBManager::RollItemListQuickLog(NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT])
{
	const static int ROLL_ITEM_QUICK_LOG_MAX_LEN = 256;
	static char quick_log_buff[ROLL_ITEM_QUICK_LOG_MAX_LEN * NEQFB_ROLLPOOL_TOTAL_COUNT] = { 0 };

	memset(quick_log_buff, 0, sizeof(quick_log_buff));

	UNSTD_STATIC_CHECK(8 == NEQFB_ROLLPOOL_TOTAL_COUNT);

	SNPRINTF(quick_log_buff, sizeof(quick_log_buff),
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d], "
		"RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d] ",
		(int)roll_list[0].item_seq, (int)roll_list[0].i_type, (int)roll_list[0].item_cfg.item_id, (int)roll_list[0].item_cfg.num, (roll_list[0].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[1].item_seq, (int)roll_list[1].i_type, (int)roll_list[1].item_cfg.item_id, (int)roll_list[1].item_cfg.num, (roll_list[1].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[2].item_seq, (int)roll_list[2].i_type, (int)roll_list[2].item_cfg.item_id, (int)roll_list[2].item_cfg.num, (roll_list[2].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[3].item_seq, (int)roll_list[3].i_type, (int)roll_list[3].item_cfg.item_id, (int)roll_list[3].item_cfg.num, (roll_list[3].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[4].item_seq, (int)roll_list[4].i_type, (int)roll_list[4].item_cfg.item_id, (int)roll_list[4].item_cfg.num, (roll_list[4].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[5].item_seq, (int)roll_list[5].i_type, (int)roll_list[5].item_cfg.item_id, (int)roll_list[5].item_cfg.num, (roll_list[5].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[6].item_seq, (int)roll_list[6].i_type, (int)roll_list[6].item_cfg.item_id, (int)roll_list[6].item_cfg.num, (roll_list[6].item_cfg.is_bind ? 1 : 0),
		(int)roll_list[7].item_seq, (int)roll_list[7].i_type, (int)roll_list[7].item_cfg.item_id, (int)roll_list[7].item_cfg.num, (roll_list[7].item_cfg.is_bind ? 1 : 0));

	return quick_log_buff;
}

void RoleEquipFBManager::OnNeqRollReq(Role *role)
{
	if (role == nullptr) return;

	RoleRollInfoMapIt info_it = m_role_roll_info_map.find(role->GetUID());
	if (info_it == m_role_roll_info_map.end()) // 没执行过startroll
	{
		return;
	}

	if (info_it->second.max_free_roll_times <= 0) // 如果小于等于0，则角色其实没有执行过StartRoll
	{
		return;
	}

	if (info_it->second.free_rolled_times + info_it->second.gold_rolled_times >= NEQFB_MAX_ROLL_TIMES)
	{
		return;
	}

	int rand_roll_index = this->RandRollItemHelper(info_it->second.free_rolled_times + info_it->second.gold_rolled_times, info_it->second.hit_list, info_it->second.roll_list);
	if (rand_roll_index < 0 || rand_roll_index >= NEQFB_ROLLPOOL_TOTAL_COUNT)
	{
		return;
	}

	NeqRollItemToRole &result_item = info_it->second.roll_list[rand_roll_index];

	int need_gold = 0;

	if (info_it->second.free_rolled_times >= info_it->second.max_free_roll_times)
	{
		if (info_it->second.gold_rolled_times >= NEQFB_MAX_GOLD_ROLL_TIMES)
		{
			return;
		}
		need_gold = LOGIC_CONFIG->GetNeqFBConfig().GetGoldRollCost(info_it->second.gold_rolled_times + 1);
		if (need_gold <= 0) return;
	}

	if (need_gold > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "OnNeqRollReq"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		++info_it->second.gold_rolled_times;
	}
	else
	{
		++info_it->second.free_rolled_times;
	}

	info_it->second.hit_list[rand_roll_index] = true;

	role->GetKnapsack()->PutOrMail(result_item.item_cfg, PUT_REASON_NEQ_ROLL);

	this->SendNeqRollInfo(role, Protocol::SCNeqRollInfo::RIREASON_ROLL, result_item.item_seq);

	gamelog::g_log_fb.printf(LL_INFO, "OnNeqRollReq user[%s %d] GetRollItem[%s]",
		role->GetName(), role->GetUID(), this->RollItemQuickLog(result_item));
}

void RoleEquipFBManager::SendNeqRollInfo(Role *role, short reason, short hit_seq)
{
	if (role == nullptr) return;
	RoleRollInfoMapIt info_it = m_role_roll_info_map.find(role->GetUID());
	if (info_it == m_role_roll_info_map.end())
	{
		return;
	}

	Protocol::SCNeqRollInfo scinfo;
	scinfo.reason = reason;
	scinfo.max_free_roll_times = info_it->second.max_free_roll_times;
	scinfo.free_roll_times = info_it->second.free_rolled_times;
	scinfo.gold_roll_times = info_it->second.gold_rolled_times;
	scinfo.hit_seq = static_cast<char>(hit_seq);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scinfo, sizeof(scinfo));
}

int RoleEquipFBManager::RandRollItemHelper(int already_roll_times, bool hit_list[NEQFB_ROLLPOOL_TOTAL_COUNT], NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT])
{
	if (already_roll_times >= 0 && already_roll_times < NEQFB_MAX_ROLL_TIMES)
	{
		int total_weight = 0;
		int weight_list[NEQFB_ROLLPOOL_TOTAL_COUNT]; memset(weight_list, 0, sizeof(weight_list));

		for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
		{
			if (hit_list[i])																		// 被抽过的不要
			{
				continue;
			}

			NeqRollItemToRole &roll_item = roll_list[i];
			if (0 != roll_item.i_type && already_roll_times < NEQFB_CAN_ROLL_ITYPE_1_LIMIT_COUNT)	// 还没抽够2次，只能得到0类物品
			{
				continue;
			}

			weight_list[i] = roll_item.weight;
			total_weight += roll_item.weight;
		}

		if (total_weight > 0)
		{
			int rand_value = RandomNum(total_weight);

			for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
			{
				if (weight_list[i] > rand_value)
				{
					return i;
				}

				rand_value -= weight_list[i];
			}
		}
	}

	return -1;
}

char * RoleEquipFBManager::RollItemQuickLog(const NeqRollItemToRole &roll_item)
{
	const static int ROLL_ITEM_QUICK_LOG_MAX_LEN = 256;
	static char quick_log_buff[ROLL_ITEM_QUICK_LOG_MAX_LEN] = { 0 };

	memset(quick_log_buff, 0, sizeof(quick_log_buff));

	SNPRINTF(quick_log_buff, sizeof(quick_log_buff), "RollItem[%d],[i_type=%d][item_id:%d, num:%d, is_bind:%d] ",
		(int)roll_item.item_seq, (int)roll_item.i_type, (int)roll_item.item_cfg.item_id, (int)roll_item.item_cfg.num, (roll_item.item_cfg.is_bind ? 1 : 0));

	return quick_log_buff;
}