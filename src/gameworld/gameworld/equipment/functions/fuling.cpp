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

#include "protocal/msgother.h"
#include "engineadapter.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

// 装备附灵
bool EquipFunction::EquipFuling(Role *role, bool is_puton, bool select_bind_first, short equip_index)
{
// 	if (NULL == role)
// 	{
// 		return false;
// 	}
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
// 	ItemGridData equip_data;
// 	const Equipment *equip = NULL;
// 
// 	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);
// 
// 	if (NULL == equip)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_NOT_EXIST);
// 		return false;
// 	}
// 
// 	if (equip->GetEquipType() == Equipment::E_TYPE_GOUYU)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_GOUYU_NOT_SUPPORT);
// 		return false;
// 	}
// 
// 	EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
// 
// 	const EquipFulingConfig *fuling_cfg = GLOBALCONFIG->GetEquipConfig().GetFulingCfg(equip_param->fuling_level + 1);
// 	if (NULL == fuling_cfg)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_CANNOT_FULING);
// 		return false;
// 	}
// 
// 	if (equip->GetLimitLevel() < fuling_cfg->equip_level_limit)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_CANNOT_FULING);
// 		return false;
// 	}
// 
// 	if (equip->GetColor() < fuling_cfg->equip_color_limit)
// 	{
// 		role->NoticeNum(errornum::EN_EQUIP_CANNOT_FULING);
// 		return false;
// 	}
// 
// 	char old_fuling_level = equip_param->fuling_level;
// 
// 	int need_stuff_id = 0;
// 	int need_stuf_count = 0;
// 	if (Equipment::IsWuqiType(equip->GetEquipType()))
// 	{
// 		need_stuff_id = fuling_cfg->wuqi_stuff_id;
// 		need_stuf_count = fuling_cfg->wuqi_stuff_count;
// 	}
// 
// 	if (Equipment::IsShipinType(equip->GetEquipType()))
// 	{
// 		need_stuff_id = fuling_cfg->shipin_stuff_id;
// 		need_stuf_count = fuling_cfg->shipin_stuff_count;
// 	}
// 
// 	if (Equipment::IsFangJuType(equip->GetEquipType()))
// 	{
// 		need_stuff_id = fuling_cfg->fangju_stuff_id;
// 		need_stuf_count = fuling_cfg->fangju_stuff_count;
// 	}
// 
// 	if (Equipment::IsHujiaType(equip->GetEquipType()))
// 	{
// 		need_stuff_id = fuling_cfg->hujia_stuff_id;
// 		need_stuf_count = fuling_cfg->hujia_stuff_count;
// 	}
// 
// 	static ItemExtern::ItemConsumeList consume_list;
// 	consume_list.Reset();
// 	Int64 bindcoin_cost = 0, nobindcoin_cost = 0;
// 	Money *money = knapsack->GetMoney();
// 
// 	//检查
// 	{
// 		short stuff_count = 0;
// 		static ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
// 		stuff_list[stuff_count].item_id = need_stuff_id;
// 		stuff_list[stuff_count].num = need_stuf_count;
// 		stuff_list[stuff_count].buyable = false;
// 		++ stuff_count;
// 
// 		if (!ItemExtern::GetConsumeListByOrder(role, !select_bind_first, stuff_count, stuff_list, &consume_list, true))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	{
// 		if (!money->AllCoinMoreThan(fuling_cfg->coin))
// 		{
// 			role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
// 			return false;
// 		}
// 	}
// 
// 	//消耗
// 	{
// 		// 物品
// 		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "Fuling"))
// 		{
// 			return false;
// 		}
// 
// 		// 铜钱
// 		if (!money->UseAllCoin(fuling_cfg->coin, "Fuling", true, &nobindcoin_cost, &bindcoin_cost))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	++ equip_param->fuling_level;
// 
// 	if (is_puton)
// 	{
// 		if (consume_list.bind_item_num > 0)
// 		{
// 			equip_mgr->GridSetBind(equip_index, false);
// 		}
// 
// 		equip_mgr->SetEquipGridDirty(equip_index);
// 		equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_FULING);
// 
// 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);					
// 	}
// 	else
// 	{
// 		if (consume_list.bind_item_num > 0)
// 		{
// 			knapsack->GridSetBind(equip_index, false);
// 		}
// 
// 		knapsack->SetItemDirty(equip_index);
// 	}
// 
// 	role->NoticeNum(noticenum::NT_EQUIP_FULING_SUCC);
// 	EventHandler::Instance().OnFuLingEquipOperate(role, equip_index);
// 
// 	// 传闻
// 	{
// 		if (fuling_cfg->is_broadcast > 0)
// 		{
// 			const char *equip_param_string = EquipParamDataClientString(UidToInt(role->GetUserId()), equip->GetItemId(), true, equip_data.invalid_time, equip_param);
// 			if (equip_param_string)
// 			{
// 				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_fuling,
// 					UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), (int)equip->GetItemId(), equip_param_string);
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_AND_ROLL);
// 			}
// 		}
// 	}
// 
// 	// 日志
// 	{
// 		gamelog::g_log_equipment.printf(LL_INFO,"[EquipFunction::Fuling Succ][user[%s %d] role_level[%d], [equip_name:%s equip_id:%d "
// 			"old_fuling_level:%d new_fuling_level:%d cost_coin:%d cost_bind_coin:%d]",
// 			role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(), 
// 			(int)old_fuling_level, (int)equip_param->fuling_level, (int)nobindcoin_cost, (int)bindcoin_cost);
// 	}

	return true;
}

