#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipmentfunction.h"
#include "equipment/equipment.h"

#include "item/itempool.h"
#include "item/itemfunction.h"
#include "item/itemextern.hpp"

#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "gamelog.h"

#include "globalconfig/globalconfig.h"
#include "globalconfig/equipconfig.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

#include "servercommon/servercommon.h"
#include <stdlib.h>

// 装备继承
bool EquipFunction::EquipInherit(Role *role, short equip_index1, short equip_index2, bool is_puton1, bool is_puton2,  short inherit_type,  short cost_type)
{
// 	if (NULL == role)
// 	{
// 		return false;
// 	}
// 
// 	if(inherit_type != COMMON_INHERIT && inherit_type != LUCKY_INHERIT) return false;
// 	if(cost_type != COST_COIN && cost_type != COST_GOLD) return false;
// 	if(LUCKY_INHERIT == inherit_type && COST_GOLD != cost_type) return false;
// 
// 	if (role->GetSublock()->CheckHasLock()) return false;
// 
// 	Knapsack *knapsack = role->GetKnapsack();
// 	EquipmentManager *equip_mgr = role->GetEquipmentManager();
// 
// 	if (knapsack->IsLock())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return false;
// 	}
// 
// 	ItemGridData equip_data1;
// 	const Equipment *equip1 = NULL;
// 	ItemGridData equip_data2;
// 	const Equipment *equip2 = NULL;
// 
// 	EQUIP_FUNCTION_GET_EQUIP(is_puton1, equip_index1, equip_data1, equip1);
// 	EQUIP_FUNCTION_GET_EQUIP(is_puton2, equip_index2, equip_data2, equip2);
// 
// 	EquipmentParam *equip_param1 = (EquipmentParam *)equip_data1.param;
// 	EquipmentParam *equip_param2 = (EquipmentParam *)equip_data2.param;
// 	
// 	if (equip1->GetEquipType() == Equipment::E_TYPE_GOUYU || equip2->GetEquipType() == Equipment::E_TYPE_GOUYU)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_GOUYU_NOT_SUPPORT);
// 		return false;
// 	}
// 
// 	if (equip1->GetEquipType() != equip2->GetEquipType())
// 	{
// 		role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_INHERIT_NEED_SAME_TYPE);
// 		return false;
// 	}
// 
// 	const InheritConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetInheritCfg();
// 	if (NULL == cfg)
// 	{
// 		return false;
// 	}
// 
// 	int need_cost_coin = cfg->coin;
// 	int need_cost_gold = cfg->gold;
// 	int need_lucky_cost_gold = cfg->xy_gold;
// 	Int64 nobind_coin_cost = 0, bind_coin_cost = 0;
// 
// 	if (cost_type == COST_COIN && !knapsack->GetMoney()->AllCoinMoreThan(need_cost_coin))
// 	{
// 		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	if (COMMON_INHERIT == inherit_type && cost_type == COST_GOLD && !knapsack->GetMoney()->GoldMoreThan(need_cost_gold))
// 	{
// 		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	if (LUCKY_INHERIT == inherit_type && cost_type == COST_GOLD && !knapsack->GetMoney()->GoldMoreThan(need_lucky_cost_gold))
// 	{
// 		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	// 消耗
// 	{
// 		if (cost_type == COST_COIN && !knapsack->GetMoney()->UseAllCoin(need_cost_coin, "EquipInherit", true, &nobind_coin_cost, &bind_coin_cost)) 
// 		{
// 			return false;
// 		}
// 
// 		if (COMMON_INHERIT == inherit_type && cost_type == COST_GOLD && !knapsack->GetMoney()->UseGold(need_cost_gold, "EquipInherit")) 
// 		{
// 			return false;
// 		}
// 
// 		if (LUCKY_INHERIT == inherit_type && cost_type == COST_GOLD && !knapsack->GetMoney()->UseGold(need_lucky_cost_gold, "EquipInherit")) 
// 		{
// 			return false;
// 		}
// 	}
// 
// 	//成功
// 	{
// 		Knapsack *knapsack = role->GetKnapsack();
// 		if (knapsack->IsLock())
// 		{
// 			role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 			return false;
// 		}
// 
// 		if (true == equip_data2.is_bind || true == equip_data1.is_bind)
// 		{
// 			knapsack->GridSetBind(equip_index2, false);
// 			knapsack->SetItemDirty(equip_index2);
// 		}
// 
// 		gamelog::g_log_equipment.buff_printf("Before Inherit: equip1: index[%d] strengthen_level[%d] start_level[%d] fuling[%d]", equip_index1, equip_param1->strengthen_level, equip_param1->star_level, equip_param1->fuling_level);
// 		gamelog::g_log_equipment.buff_printf("Before Inherit: equip2: index[%d] strengthen_level[%d] start_level[%d] fuling[%d]", equip_index2, equip_param2->strengthen_level, equip_param2->star_level, equip_param2->fuling_level);
// 
// 		if(inherit_type == COMMON_INHERIT) //煅造传承 ,继承主装备的强化等级、星级、附灵
// 		{
// 			equip_param2->strengthen_level = std::max(equip_param1->strengthen_level, equip_param2->strengthen_level);
// 			equip_param2->star_level = std::max(equip_param1->star_level, equip_param2->star_level);
// 			equip_param2->fuling_level = std::max(equip_param1->fuling_level, equip_param2->fuling_level);
// 			equip_param2->shen_level = std::max(equip_param1->shen_level, equip_param2->shen_level);
// 
// 			equip_param1->strengthen_level = 0;
// 			equip_param1->star_level = 0;
// 			equip_param1->fuling_level = 0;
// 			equip_param1->shen_level = 0;
// 		}
// 
// 		if(inherit_type == LUCKY_INHERIT)  //幸运传承：继承主装备的幸运属性
// 		{
// 			equip_param2->has_lucky = std::max(equip_param1->has_lucky, equip_param2->has_lucky);
// 			equip_param1->has_lucky = 0;
// 		}
// 		
// 		if(is_puton1) equip_mgr->SetEquipGridDirty(equip_index1);
// 		if(!is_puton1) knapsack->SetItemDirty(equip_index1);
// 
// 		if(is_puton2) equip_mgr->SetEquipGridDirty(equip_index2);
// 		if(!is_puton2) knapsack->SetItemDirty(equip_index2);
// 
// 		if (is_puton1 || is_puton2)
// 		{
// 			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);	
// 		}
// 
// 		role->NoticeNum(noticenum::NT_EQUIP_EXTEND_SUCC);
// 	}
// 
// 	// 传闻
// 	{
// 		const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(equip_data2.item_id);
// 		if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType() && equip_item->GetLimitLevel() >= 400)
// 		{
// 			const char *equip_param_string = EquipParamDataClientString(UidToInt(role->GetUserId()), equip_item->GetItemId(), true, equip_data2.invalid_time, equip_param2);
// 			if (equip_param_string)
// 			{
// 				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_inherit,
// 					UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), (int)equip_item->GetItemId(), equip_param_string);
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_AND_ROLL);
// 			}
// 		}
// 	}
// 
// 	//日志
// 	{
// 		gamelog::g_log_equipment.buff_printf("[EquipFunction::EquipInherit Succ][user[%s %d] role_level[%d], [equip_id1:%d equip_id2:%d" 
// 			"cost coin:%d cost bind_coin:%d cost gold: %d] [equip_param2:] fuling_level[%d] strength_level[%d] star_level[%d] ",
// 			role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), (int)equip1->GetItemId(), (int)equip2->GetItemId(), 
// 			(int)nobind_coin_cost, (int)bind_coin_cost, (int) need_cost_gold, equip_param2->fuling_level, equip_param2->strengthen_level, equip_param2->star_level);
// 
// 		gamelog::g_log_equipment.commit_buff(LL_INFO);
// 	}
// 
// 	EventHandler::Instance().OnInheritEquipOperate(role, equip_index1, equip_index2);

	return true;
}