#include "skillmanager.hpp"
#include "skillbase.hpp"
#include "skillpool.hpp"
#include "skilldef.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "engineadapter.h"

#include "scene/scene.h"

#include "protocal/msgrole.h"

#include "globalconfig/globalconfig.h"
#include "skillfunction.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/shop/roleshop.hpp"

#include "item/itemextern.hpp"

#include "world.h"
#include "gamelog.h"
#include <stdlib.h>

bool SkillFunction::LearnSkill(Role *role, short skill_id)
{	
	if (NULL == role) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const SkillBase * skill_base = SKILLPOOL->GetSkill(skill_id);
	if (NULL == skill_base)
	{
		return false;
	}


	if (!role->IsProfSuitable(skill_base->GetProfLimit())) 
	{
		role->NoticeNum(errornum::EN_ROLE_PROF_NOT_SUITABLE);

		return false;
	}
	int old_skill_level = 0;

	SkillManager::SkillItem skill_item;
	bool ret = role->GetSkillManager()->GetSkillByID(skill_id, &skill_item);
	if (ret)
	{
		old_skill_level = skill_item.level;
	}

	if (old_skill_level >= skill_base->GetMaxLevel())
	{
		role->NoticeNum(errornum::EN_SKILL_MAX_LEVEL_LIMIT);
		return false;
	}

	int target_skill_level = old_skill_level+1;//技能升级

	if (role->GetLevel() < skill_base->GetLearnLevelLimit(target_skill_level))
	{
		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return false;
	}

	int cost_coin = skill_base->GetLearnCoin(target_skill_level);
	if (!knapsack->GetMoney()->AllCoinMoreThan(cost_coin))
	{
		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}
	int mojing_cost = skill_base->GetLearnMojing(target_skill_level);
	if (role->GetRoleShop()->GetChestShopMojing() < mojing_cost)
	{
		role->NoticeNum(errornum::EN_MOJING_NOT_ENOUGH);
		return false;
	}

	ItemID cost_item_id = skill_base->GetLearnItemCostId(target_skill_level);
	int cost_item = skill_base->GetLearnItemCost(target_skill_level);
	//int own_item_count = role->GetKnapsack()->Count(cost_item_id);
	
	if (cost_item <= 0 && cost_coin <= 0 && mojing_cost <= 0)
	{
		return false;
	}

	if (cost_item > 0)
	{
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff;
		stuff.item_id = cost_item_id;
		stuff.num = cost_item;
		stuff.buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "LearnSkill"))
		{
			return false;
		}
	}

	if (mojing_cost > 0 && !role->GetRoleShop()->AddChestShopMojing(-1 * mojing_cost, "LearnSkill"))
	{
		return false;
	}
	
	Int64 bindcoin_cost = 0, nobindcoin_cost = 0;
	if(cost_coin > 0 && !knapsack->GetMoney()->UseAllCoin(cost_coin, "LearnSkill", true, &nobindcoin_cost, &bindcoin_cost)) return false;

	ret = role->GetSkillManager()->AddSkill(skill_id, target_skill_level);

	gamelog::g_log_debug.printf(LL_DEBUG, "LearnSkill role(%d,%s) skill(%d,%d) cost_bind_coin[%d] cost_coin[%d] cost_item[%d] mojing_cost[%d] got %d",
		role->GetUID(), role->GetName(), skill_id, target_skill_level, (int)bindcoin_cost, (int)nobindcoin_cost, cost_item, mojing_cost, ret);

	int total_skill_level = role->GetSkillManager()->GetSkillTotalLevel();
	EventHandler::Instance().OnUpSkillLevel(role, skill_id, old_skill_level, target_skill_level, total_skill_level);

	return true;
}

bool SkillFunction::LearnSkillActiveAll(Role *role)
{
	if (NULL == role||Obj::OBJ_TYPE_ROLE != role->GetObjType()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	SkillListParam tmp;
	int low_lev = role->GetSkillManager()->GetRoleActiveSkillList(&tmp);
	SkillListParam oldParam = tmp;

	bool all_up_fin = true;  //所有技能都升级完成
	std::set<int> set_skill_idx;
	while (low_lev < MAX_SKILL_LEVEL)
	{
		all_up_fin = true;
		for (int i = 0; i < tmp.count; ++i)
		{
			if (tmp.skill_list[i].level > low_lev) {
				all_up_fin = false;
				continue;
			}
			if (tmp.skill_list[i].exp) continue;
			const SkillBase *skill = SKILLPOOL->GetSkill(tmp.skill_list[i].skill_id);
			if (nullptr == skill)
			{
				continue;
			}

			int mojing_cost = skill->GetLearnMojing(tmp.skill_list[i].level+1);
			if (role->GetRoleShop()->GetChestShopMojing() < mojing_cost
				|| (role->GetLevel() < skill->GetLearnLevelLimit(tmp.skill_list[i].level+1))
				|| tmp.skill_list[i].level >= skill->GetMaxLevel()) {
				tmp.skill_list[i].exp = 1;
				continue;
			}

			if (mojing_cost > 0)
			{
				if (!role->GetRoleShop()->AddChestShopMojing(-1 * mojing_cost, "LearnSkill")) {
					all_up_fin = true;
					break;
				}
				all_up_fin = false;
				tmp.skill_list[i].level++;
				set_skill_idx.insert(i);
			}
		}
		if (all_up_fin) {
			for (std::set<int>::iterator itr = set_skill_idx.begin(); itr != set_skill_idx.end(); ++itr)
			{
				SkillListParam::__changeskill chang_skill = tmp.skill_list[*itr];
				role->GetSkillManager()->AddSkill(chang_skill.skill_id, chang_skill.level);
				gamelog::g_log_debug.printf(LL_DEBUG, "LearnSkill role(%d,%s) skill(%d,%d)",
					role->GetUID(), role->GetName(), chang_skill.skill_id, chang_skill.level);

				int total_skill_level = role->GetSkillManager()->GetSkillTotalLevel();
				EventHandler::Instance().OnUpSkillLevel(role, chang_skill.skill_id, oldParam.skill_list[*itr].level, chang_skill.level, total_skill_level);
			}
			return true;
		}
		++low_lev;
	}
	return true;
}

bool SkillFunction::LearnSkillPassiveAll(Role *role)
{
	if (NULL == role || Obj::OBJ_TYPE_ROLE != role->GetObjType()) return false;

	SkillManager * skill_manager = role->GetSkillManager();
	Knapsack *knapsack = role->GetKnapsack();
	if (!skill_manager || !knapsack)
	{
		return false;
	}
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	for (int i = 0; i < PASSIVE_SKILL_COUNT; ++i)
	{
		const UInt16 skill_id = PASSIVE_SKILL_ID_LIST[i];
		const SkillBase * skill_base = SKILLPOOL->GetSkill(skill_id);
		if (NULL == skill_base)
		{
			return false;
		}
		int target_skill_level=0;
		SkillManager::SkillItem skill_item;
		if (skill_manager->GetSkillByID(skill_id, &skill_item))
		{
			target_skill_level = skill_item.level;
		}
		ItemID cost_item_id = skill_base->GetLearnItemCostId(target_skill_level+1);
		int own_item_count = role->GetKnapsack()->Count(cost_item_id);
		int cost_item = 0; 
		
		for (int j = 0; j < MAX_SKILL_LEVEL; ++j)
		{
			if (target_skill_level > skill_base->GetMaxLevel()){
				break;
			}
			if (role->GetLevel() < skill_base->GetLearnLevelLimit(target_skill_level+1)) {
				break;
			}
			ItemID target_item_id = skill_base->GetLearnItemCostId(target_skill_level+1);
			if(cost_item_id!=target_item_id){
				break;
			}
			int target_cost_item= skill_base->GetLearnItemCost(target_skill_level+1);
			if(own_item_count<cost_item+target_cost_item){
				break;
			}
			target_skill_level += 1;
			cost_item += target_cost_item;
		}

		if (cost_item > 0)
		{
			static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
			ItemExtern::ItemConsumeConfig stuff;
			stuff.item_id = cost_item_id;
			stuff.num = cost_item;
			stuff.buyable = false;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list))
			{
				return false;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(role, consume_list, "LearnSkillPassiveAll"))
			{
				return false;
			}
		}
		else
		{
			continue;
		}

		int ret = role->GetSkillManager()->AddSkill(skill_id, target_skill_level);

		gamelog::g_log_debug.printf(LL_DEBUG, "LearnSkill role(%d,%s) skill(%d,%d)  cost_item[%d]  got %d",
			role->GetUID(), role->GetName(), skill_id, target_skill_level, cost_item, ret);

		int total_skill_level = role->GetSkillManager()->GetSkillTotalLevel();
		EventHandler::Instance().OnUpSkillLevel(role, skill_id,skill_item.level , target_skill_level, total_skill_level);
	}
	return true;
}
