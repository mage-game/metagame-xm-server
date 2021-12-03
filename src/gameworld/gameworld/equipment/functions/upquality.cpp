#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/servercommon.h"

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
#include "other/event/eventhandler.hpp"
#include "equipment/equipmentmanager.h"
#include "other/sublock/sublock.hpp"

// 装备升品质
bool EquipFunction::EquipUpQuality(Role *role, bool is_puton, bool select_bind_first, short equip_index)
{
 	if (NULL == role)
 	{
 		return false;
 	}
 
 	if (role->GetSublock()->CheckHasLock()) return false;
 
 	Knapsack *knapsack = role->GetKnapsack();
 	EquipmentManager *equip_mgr = role->GetEquipmentManager();
 
 	if (knapsack->IsLock())
 	{
 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return false;
 	}
 
 	ItemGridData equip_data;
 	const Equipment *equip = NULL;
 
 	//is_puton = true;
 	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);
 
 	EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
 
 	const UpQualityConfig *up_quality_cfg = GLOBALCONFIG->GetEquipConfig().GetUpQualityCfg(equip->GetEquipType(), equip_param->quality);
	const UpQualityConfig *up_quality_cfg_next = GLOBALCONFIG->GetEquipConfig().GetUpQualityCfg(equip->GetEquipType(), equip_param->quality + 1);
	if (NULL == up_quality_cfg || NULL == up_quality_cfg_next)
 	{
 		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPQUALITY);
 		return false;
 	}
 
 	if (!EquipFunction::CanUpQuality(equip->GetEquipType()))
 	{
 		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPQUALITY);
 		return false;
 	}
 
 	if (equip->GetLimitLevel() < up_quality_cfg_next->equip_level)
 	{
 		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPQUALITY);
 		return false;
 	}
 
 	char old_quality = static_cast<char>(equip_param->quality);
 
 	static ItemExtern::ItemConsumeList consume_list; 
 	consume_list.Reset();
 	//Int64 bindcoin_cost = 0;
 
 	{
 		// 获取可消耗的背包材料列表
 
 		bool nobind_first = true;
 		if (select_bind_first)					// 手动选择了绑定优先
 		{
 			nobind_first = false;
 		}
 	
 		short stuff_count = 0;
 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
 		stuff_list[stuff_count].item_id = up_quality_cfg_next->stuff_id;
 		stuff_list[stuff_count].num = up_quality_cfg_next->stuff_count;
 		stuff_list[stuff_count].buyable = false;
 		++ stuff_count;
 
 		if (!ItemExtern::GetConsumeListByOrder(role, nobind_first, stuff_count, stuff_list, &consume_list, true))
 		{
 			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
 			return false;
 		}
 	}
 
 	{
 		// 检查金币
 		Money *money = knapsack->GetMoney();
 		if (up_quality_cfg->coin > 0 && !money->CoinBindMoreThan(up_quality_cfg->coin))
 		{
 			role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
 			return false;
 		}
 	}
 
 	{
		// 铜钱
		Money *money = knapsack->GetMoney();
		if (up_quality_cfg->coin > 0 && !money->UseCoinBind(up_quality_cfg->coin, "UpQuality")) return false;

		//bindcoin_cost = up_quality_cfg->coin;

 		// 消耗物品
 		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "UpQuality")) return false;
 	}

	const UpQualityConfig *before_quality_level_cfg = GLOBALCONFIG->GetEquipConfig().GetUpQualityCfg(equip->GetEquipType(), equip_param->quality);
	if (before_quality_level_cfg == NULL)
	{
		return 0;
	}
	int last_color = before_quality_level_cfg->color;
 
 	equip_param->quality += 1;
 
 	if (is_puton)
 	{
 		// 设置绑定
 		if (consume_list.bind_item_num > 0)
 		{
 			equip_mgr->GridSetBind(equip_index, false);
 		}
 
 		equip_mgr->SetEquipGridDirty(equip_index);
 
 		equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_QUALITY);
 
 		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);	
 	}
 	else
 	{
 		// 设置绑定
 		if (consume_list.bind_item_num > 0)
 		{
 			knapsack->GridSetBind(equip_index, false);
 		}
 		knapsack->SetItemDirty(equip_index);
 	}
 
 	role->NoticeNum(noticenum::NT_EQUIP_UPQUALITY_SUCC);
 
 	gamelog::g_log_equipment.printf(LL_INFO,"[EquipFunction::EquipUpQuality Succ][user[%s %d] role_level[%d], [equip_name:%s equip_id:%d "
 		"quality:%d new_quality:%d]",
 		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(), 
 		(int)old_quality, (int)equip_param->quality);
 
	// TODO
 	EventHandler::Instance().OnEquipUpQuality(role, is_puton, old_quality, equip_param->quality);

	/*
 	StonesOnPart stone_info;
	{
 		if (is_puton)
 		{
 			stone_info = role->GetStoneManager()->GetStonePartByEquipIndex(equip_index);
 		}
 		else
 		{
 			stone_info = role->GetStoneManager()->GetStonePartByEquipType(equip->GetEquipType());
 		}
 	}
	*/


	//全服提醒
	if (up_quality_cfg_next->is_notice) {
 		const char *equip_param_string = EquipParamDataClientString(UidToInt(role->GetUserId()), equip->GetItemId(), true, equip_data.invalid_time, equip_param, NULL);
 		if (NULL != equip_param_string)
 		{
 			//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality,
 			//	UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), (int)equip->GetItemId(), equip_param_string, (int)equip_param->quality + 1);

			int length = 0;
			int quality_color = 0;
			

			const UpQualityConfig *quality_cfg_2 = GLOBALCONFIG->GetEquipConfig().GetUpQualityCfg(equip->GetEquipType(), equip_param->quality);
			if (quality_cfg_2 == NULL)
			{
				return 0;
			}
			quality_color = quality_cfg_2->color;
			if (quality_color == last_color)
			{
				quality_color = 6;
			}

			switch (quality_color)
			{
			case 1:
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_1,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
				}
				break;

			case 2:
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_2,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
				}
				break;

			case 3:
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_3,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
				}
				break;
				
			case 4:
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_4,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
				}
				break;
				
			case 5:
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_5,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
				}
				break;

		   case 6:
			   {
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upquality_6,
						UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip->GetItemName(), (int)equip->GetItemId(), equip_param_string);
			   }
			   break;

			default:
				break;
			}

 			if (length > 0)
 			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				/*
 				if (equip_param->quality >= ItemBase::I_QUALITY_ORANGE)
 				{
 					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
 				}
 				else if (equip_param->quality >= ItemBase::I_QUALITY_PURPLE)
 				{
 					// World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
 				}
				*/
 			}
 		}
	}

	return true;
}
