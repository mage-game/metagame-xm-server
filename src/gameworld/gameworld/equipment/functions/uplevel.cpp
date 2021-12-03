#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipmentfunction.h"
#include "equipment/equipment.h"

#include "item/itempool.h"
#include "item/itemfunction.h"
#include "item/itemextern.hpp"
#include "obj/character/role.h"
#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "gamelog.h"

#include "globalconfig/globalconfig.h"
#include "globalconfig/equipconfig.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

#include "servercommon/servercommon.h"
#include <stdlib.h>

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "protocal/msgother.h"
#include "protocal/msgrole.h"
#include "equipment/equipmentmanager.h"
#include "other/shop/shop.hpp"
#include "other/event/eventhandler.hpp"
#include "other/vip/vip.hpp"
#include "other/sublock/sublock.hpp"
#include "equipment/stonemananger.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

// 装备升级
/*
bool EquipFunction::EquipUpLevel(Role * role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index,
									bool use_protect_item, int use_protect_num, bool isGM, int * coin_cost, int * gold_cost)
{
	if (NULL == role)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	EquipmentManager *equip_mgr = role->GetEquipmentManager();
	if (equip_mgr == NULL)
	{
		return false;
	}

	ItemGridData equip_data;		// 物品在角色里的属性，数量、是否绑定等等
	const Equipment *equip = NULL;	// 物品基础属性

	is_puton = true;
	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);

	if (NULL == equip) { return false; }

	//武器不能升级
	if (equip->GetEquipType() == Equipment::E_TYPE_WUQI)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
		return false;
	}

	//EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;	// 装备可变的附加属性
	int grid_up_level = equip_mgr->GetEquipGridUpLevel(equip_index);

	if (grid_up_level >= EquipConfig::MAX_STRENGTH_LEVEL)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND);
		return false;
	}

	const UpLevelConfig *up_level_cfg = GLOBALCONFIG->GetEquipConfig().GetUpLevelCfg(equip->GetEquipType() - Equipment::E_TYPE_MIN, grid_up_level + 1);	// 强化配置表
	if (NULL == up_level_cfg)	// 检查强化等级
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return false;
	}

	const UInt16 old_up_level = grid_up_level;

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表

		bool nobind_first = true;
		if (select_bind_first)					// 手动选择了绑定优先
		{
			nobind_first = false;
		}

		short stuff_count = 0;

		// 材料1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = up_level_cfg->stuff_id;
		stuff_list[stuff_count].num = up_level_cfg->stuff_count;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;
		++stuff_count;

		// 幸运符
		if (use_protect_item && up_level_cfg->protect_stuff_id > 0 && use_protect_num > 0)
		{
			stuff_list[stuff_count].item_id = up_level_cfg->protect_stuff_id;
			stuff_list[stuff_count].num = use_protect_num;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}

		if (!ItemExtern::GetConsumeListByOrder(role, nobind_first, stuff_count, stuff_list, &consume_list, true))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	{
		// 检查金币和元宝
		Money *money = knapsack->GetMoney();
		if (up_level_cfg->coin > 0 && !money->AllCoinMoreThan(up_level_cfg->coin))
		{
			role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
			return false;
		}

		if (consume_list.need_gold_num > 0 && !money->AllGoldMoreThan(consume_list.need_gold_num))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	{
		// 物品
		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "EquipUpLevel")) return false;

		// 元宝
		Money *money = knapsack->GetMoney();
		if (consume_list.need_gold_num > 0)
		{
			if (!money->UseAllGold(consume_list.need_gold_num, "EquipUpLevel")) return false;
			if (NULL != gold_cost) *gold_cost = consume_list.need_gold_num;

			EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_list.need_gold_num, 0);
		}

		// 金币
		if (up_level_cfg->coin > 0)
		{
			if (!money->UseAllCoin(up_level_cfg->coin, "EquipUpLevel", true)) return false;
			if (NULL != coin_cost) *coin_cost = up_level_cfg->coin;

			EventHandler::Instance().OnStrengthenConsumeCoin(role, up_level_cfg->coin);
		}
	}

	{
		int vip_add_suc_rate = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_STRENGTH_SUC_ADD_RATE);
		int protect_item_add_rate = up_level_cfg->protect_add_rate * use_protect_num;
		short operate_result = 1;
		if ((RandomNum(100) < up_level_cfg->base_succ_rate + vip_add_suc_rate + protect_item_add_rate)) // 强化成功
		{
			equip_mgr->AddEquiGridUplevel(equip_index, 1);	// 升级加到装备格子里		
		}
		else	// 升级失败
		{
			operate_result = 0;
			if (!use_protect_item && up_level_cfg->is_downlevel_on_fail == Equipment::E_STRENGTHEN_DOWNLEVEL_ON_FAIL)
			{
				equip_mgr->AddEquiGridUplevel(equip_index, -1);	// 强化失败掉级
			}
		}

		if (is_puton)
		{
			equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_UPLEVEL);
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
		}

		if (is_puton)
		{
			// 设置绑定
			// 			if (consume_list.bind_item_num > 0)
			// 			{
			// 				equip_mgr->GridSetBind(equip_index, false);
			// 			}

			equip_mgr->SetEquipGridDirty(equip_index);
		}
		else
		{
			// 设置绑定
			// 			if (consume_list.bind_item_num > 0)
			// 			{
			// 				knapsack->GridSetBind(equip_index, false);
			// 			}

			knapsack->SetItemDirty(equip_index);
		}

		role->SendOperateResult(Protocol::SCOperateResult::OP_EQUIP_STRENGTHEN, operate_result);
	}

	if (isGM)
	{
		printf("[EquipFunction::EquipUpLevel Succ][user[%s %d] role_level[%d] [equip_name:%s equip_id:%d "
			"up_level:%d new_up_level:%d cost_gold:%d "
			"cost_stuff_id:%d cost_stuff_count:%d"
			"cost_stuff_bind_count:%d cost_lucky_item:%d cost_luck_count:%d]\n",
			role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(),
			(int)old_up_level, (int)equip_mgr->GetEquipGridUpLevel(equip_index), (int)consume_list.need_gold_num,
			(int)up_level_cfg->stuff_id, (int)up_level_cfg->stuff_count,
			(int)consume_list.bind_item_num, (int)up_level_cfg->protect_stuff_id, use_protect_num);
	}

	gamelog::g_log_equipment.printf(LL_INFO, "[EquipFunction::EquipUpLevel Succ][user[%s %d] role_level[%d] [equip_name:%s equip_id:%d "
		"up_level:%d new_up_level:%d cost_gold:%d "
		"cost_stuff_id:%d cost_stuff_count:%d"
		"cost_stuff_bind_count:%d cost_lucky_item:%d cost_luck_count:%d]",
		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(),
		(int)old_up_level, (int)equip_mgr->GetEquipGridUpLevel(equip_index), (int)consume_list.need_gold_num,
		(int)up_level_cfg->stuff_id, (int)up_level_cfg->stuff_count,
		(int)consume_list.bind_item_num, (int)up_level_cfg->protect_stuff_id, use_protect_num);

	EventHandler::Instance().OnUpLevelEquipOperate(role, equip_index, old_up_level, equip_mgr->GetEquipGridUpLevel(equip_index));

	return true;
}
//bool EquipFunction::EquipUpLevel(Role *role, short equip_index1, short equip_index2, short equip_index3)
//{
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
	// 	if (!knapsack->HaveSpace())
	// 	{
	// 		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
	// 		return false;
	// 	}
	// 
	// 	ItemGridData equip_data1;
	// 	const Equipment *equip1 = NULL;
	// 	ItemGridData equip_data2;
	// 	const Equipment *equip2 = NULL;
	// 	ItemGridData equip_data3;
	// 	const Equipment *equip3 = NULL;
	// 
	// 	EQUIP_FUNCTION_GET_EQUIP(false, equip_index1, equip_data1, equip1);
	// 	EQUIP_FUNCTION_GET_EQUIP(false, equip_index2, equip_data2, equip2);
	// 	EQUIP_FUNCTION_GET_EQUIP(false, equip_index3, equip_data3, equip3);
	// 
	// 	EquipmentParam *equip_param1 = (EquipmentParam *)equip_data1.param;
	// 	EquipmentParam *equip_param2 = (EquipmentParam *)equip_data2.param;
	// 	EquipmentParam *equip_param3 = (EquipmentParam *)equip_data3.param;
	// 
	// 	if (equip1->GetEquipType() == Equipment::E_TYPE_GOUYU || equip2->GetEquipType() == Equipment::E_TYPE_GOUYU ||
	// 		equip3->GetEquipType() == Equipment::E_TYPE_GOUYU)
	// 	{
	// 		role->NoticeNum(errornum::EN_EQUIP_GOUYU_NOT_SUPPORT);
	// 		return false;
	// 	}
	// 
	// 	if (equip_data1.item_id != equip_data2.item_id || equip_data1.item_id != equip_data3.item_id)
	// 	{
	// 		role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_UPLEVEL_NOT_ENOUGH);
	// 		return false;
	// 	}
	// 
	// 	const UpLevelConfig * uplevel_config = GLOBALCONFIG->GetEquipConfig().GetUpLevelCfg(equip_data1.item_id);
	// 	if (NULL == uplevel_config)
	// 	{
	// 		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
	// 		return false;
	// 	}
	// 
	// 	//检查女娲石
	// 	{
	// 		if (role->GetNvWaShi() < uplevel_config->m_nv_wa_shi)
	// 		{
	// 			role->NoticeNum(errornum::EN_NV_WA_SHI_NOT_ENOUGH);
	// 			return false;
	// 		}
	// 	}
	// 
	// 	ItemID item_id1 = equip_data1.item_id;
	// 	ItemID item_id2 = equip_data2.item_id;
	// 	ItemID item_id3 = equip_data3.item_id;
	// 
	// 	//计算最终获得物品的各个属性值
	// 	bool is_bind_new_equip = equip_data1.is_bind || equip_data2.is_bind || equip_data3.is_bind;
	// 	int max_invalid_time = std::max(std::max(equip_data1.invalid_time, equip_data2.invalid_time), equip_data3.invalid_time);
	// 
	// 	char max_strengthen_level = std::max(std::max(equip_param1->strengthen_level, equip_param2->strengthen_level), equip_param3->strengthen_level);
	// 	char max_star_level = std::max(std::max(equip_param1->star_level, equip_param2->star_level), equip_param3->star_level);
	// 	char max_fuling_level = std::max(std::max(equip_param1->fuling_level, equip_param2->fuling_level), equip_param3->fuling_level);
	// 	char max_shen_level = std::max(std::max(equip_param1->shen_level, equip_param2->shen_level), equip_param3->shen_level);
	// 	char max_has_lucky = std::max(std::max(equip_param1->has_lucky, equip_param2->has_lucky), equip_param3->has_lucky);
	// 	
	// 	static int xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM] = {0};
	// 	UNSTD_STATIC_CHECK(sizeof(xianpin_type_list) == sizeof(equip_param1->xianpin_type_list));
	// 	memcpy(xianpin_type_list, equip_param1->xianpin_type_list, sizeof(xianpin_type_list));
	// 
	// 	// 消耗
	// 	{
	// 		short index_list[3] = {equip_index1, equip_index2, equip_index3};
	// 		short num_list[3] = {1, 1, 1};
	// 		if (!knapsack->ConsumeItemByIndexList(3, index_list, num_list, "UplevelCost", true)) return false;
	// 		
	// 		int cost_nv_wa_shi = (int)abs(uplevel_config->m_nv_wa_shi) * -1;
	// 		if (uplevel_config->m_nv_wa_shi > 0 && !role->AddNvWaShi(cost_nv_wa_shi, "UplevelCost")) return false;
	// 	}
	// 
	// 	//成功
	// 	{
	// 		EquipmentParam *equip_param = new EquipmentParam();
	// 		equip_param->strengthen_level = max_strengthen_level;
	// 		equip_param->star_level = max_star_level;
	// 		equip_param->fuling_level = max_fuling_level;
	// 		equip_param->shen_level = max_shen_level;
	// 		equip_param->has_lucky = max_has_lucky;
	// 
	// 		UNSTD_STATIC_CHECK(sizeof(xianpin_type_list) == sizeof(equip_param->xianpin_type_list));
	// 		memcpy(equip_param->xianpin_type_list, xianpin_type_list, sizeof(equip_param->xianpin_type_list));
	// 
	// 		ItemDataWrapper item_wrapper(uplevel_config->m_new_equip_id, 1, is_bind_new_equip, 
	// 			equip_param, max_invalid_time, 0);
	// 		if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE)) return false;
	// 
	// 		role->NoticeNum(noticenum::NT_EQUIP_UPLEVEL_SUCC);
	// 	}
	// 
	// 	gamelog::g_log_equipment.printf(LL_INFO, "[EquipFunction::EquipUpLevel Succ][user[%s %d] role_level[%d], [equip_id1:%d equip_id2:%d equip_id3:%d"
	// 		"new_equip_id:%d cost_nvwashi:%d]",
	// 		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), (int)item_id1, (int)item_id2, (int)item_id3, 
	// 		item_id1, (int)uplevel_config->m_nv_wa_shi);
	// 
	// 	EventHandler::Instance().OnEquipUpLevel(role);

//	return true;
//}
*/
//bool EquipFunction::EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, char use_protect_item, bool isGM)
//{
//	if (!role || !is_puton || use_protect_item < 0)
//	{
//		return false;
//	}
//	EquipmentManager *equip_mgr = role->GetEquipmentManager();
//	if (!equip_mgr)
//	{
//		return false;
//	}
//
//	ItemGridData equip_data;
//	if (!equip_mgr->GetEquipmentGridData(equip_index, &equip_data))
//	{
//		role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
//		return false;
//	}
//
//	const Equipment* old_baseinfo = equip_mgr->GetEquipBaseInfoByIndex(equip_index);
//	if (!old_baseinfo)
//	{
//		role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
//		return false;
//	}
//
//	//武器不能升级
//	if (old_baseinfo->GetEquipType() == Equipment::E_TYPE_WUQI)
//	{
//		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
//		return false;
//	}
//
//	const UpLevelConfig* levelup_cfg = GLOBALCONFIG->GetEquipConfig().GetUpLevelCfg(old_baseinfo->GetItemId());
//	if (!levelup_cfg)
//	{//这个装备无法升级
//		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
//		return false;
//	}
//
//	const ItemBase* new_itembase = ITEMPOOL->GetItem(levelup_cfg->new_itemid);
//	if (!new_itembase)
//	{//升满
//		role->NoticeNum(errornum::EN_EQUIP_FUNC_LEVELUP_IS_MAX_LEVEL);
//		return false;
//	}
//
//	if (new_itembase->GetLimitLevel() > role->GetLevel())
//	{//等级不足
//		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
//		return false;
//	}
//
//	
//	
//	//-----------------------------------------------检测消耗------------------------------------------------------
//	/**/
//	ItemExtern::ItemConsumeConfigList stuff_list;
//
//	stuff_list.add_item({ levelup_cfg->stuff_id, is_auto_buy, levelup_cfg->stuff_count });
//	if (use_protect_item > 0)
//	{
//		stuff_list.add_item({ levelup_cfg->protect_stuff_id, false, use_protect_item});
//	}
//
//	stuff_list.add_coin(COIN_TYPE::BIND_COIN, levelup_cfg->coin);
//	//----------------------------消耗物品------------------------------------
//	if (!stuff_list.done(*role, "EquipLevelUp"))
//		return false;
//
//
//	//---------------------------------升级概率---------------------------------------
//	int rand_odds = RandomNum(100);
//	int odds = levelup_cfg->base_succ_rate;
//	if (use_protect_item > 0)
//	{
//		odds += levelup_cfg->protect_add_rate * use_protect_item;
//	}
//
//	const ItemBase * target_item_base = NULL;
//
//	if (rand_odds >= odds)
//	{
//		role->NoticeNum(errornum::EN_EQUIP_FUNC_LEVELUP_ODDS_FAILD);
//		if (levelup_cfg->previous_itemid == 0)
//		{
//			return true;
//		}
//
//		target_item_base = ITEMPOOL->GetItem(levelup_cfg->previous_itemid);
//	}
//	else
//	{
//		role->NoticeNum(errornum::EN_EQUIP_FUNC_LEVELUP_SUCC);
//		target_item_base = dynamic_cast<const ItemBase*>(new_itembase);
//	}
//
//	if (target_item_base)
//	{
//		//这里不该使用会返回失败的函数，因为物品已经消耗
//		//equip_mgr->TransformToNewEquip(equip_index, target_item_base->GetItemId());
//		equip_data.item_id = target_item_base->GetItemId();
//		equip_mgr->SetEquipmentGridData(equip_index, &equip_data);
//		EventHandler::Instance().OnUpLevelEquipOperate(role, equip_index, target_item_base->GetLimitLevel(), target_item_base->GetLimitLevel());
//	}
//	
//	return true;
//}

// 装备升级（xxqy）
bool EquipFunction::EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, short equip_index /*bool use_protect_stuff*/)
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

	is_puton = true;
	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);

	EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;

	const UpLevelConfig * uplevel_config = GLOBALCONFIG->GetEquipConfig().GetUpLevelCfg(equip_data.item_id);
	if (NULL == uplevel_config)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
		return false;
	}

	if (!EquipFunction::CanStrength(equip->GetEquipType()))
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPLEVEL);
		return false;
	}

	const ItemBase *new_itembase = (const Equipment *)ITEMPOOL->GetItem(uplevel_config->m_new_equip_id);
	if (NULL == new_itembase || new_itembase->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return false;
	}
	const Equipment *new_equip = (const Equipment *)new_itembase;

	{
		// 检查装备等级
		if (is_puton && new_equip->GetLimitLevel() > role->GetLevel())
		{
			role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
			return false;
		}
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	//Int64 bindcoin_cost = 0, nobindcoin_cost = 0;

	{
		// 获取可消耗的背包材料列表

		bool nobind_first = true;
		if (select_bind_first)					// 手动选择了绑定优先
		{
			nobind_first = false;
		}

		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		if (uplevel_config->m_stuff_id_1 > 0)
		{
			stuff_list[stuff_count].item_id = uplevel_config->m_stuff_id_1;
			stuff_list[stuff_count].num = uplevel_config->m_stuff_count_1;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}

		if (uplevel_config->m_stuff_id_2 > 0)
		{
			stuff_list[stuff_count].item_id = uplevel_config->m_stuff_id_2;
			stuff_list[stuff_count].num = uplevel_config->m_stuff_count_2;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}

		if (uplevel_config->m_stuff_id_3 > 0)
		{
			stuff_list[stuff_count].item_id = uplevel_config->m_stuff_id_3;
			stuff_list[stuff_count].num = uplevel_config->m_stuff_count_3;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}

		if (0 == stuff_count)
		{
			role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
			return false;
		}

		//// 保护符
		//if (ItemBase::INVALID_ITEM_ID == uplevel_config->m_protect_id) use_protect_stuff = false;

		//if (uplevel_config->m_protect_id > 0 && use_protect_stuff)
		//{
		//	stuff_list[stuff_count].item_id = uplevel_config->m_protect_id;
		//	stuff_list[stuff_count].num = 1;
		//	stuff_list[stuff_count].buyable = false;
		//	++stuff_count;
		//}

		if (!ItemExtern::GetConsumeListByOrder(role, nobind_first, stuff_count, stuff_list, &consume_list, true))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	//{
	//	// 检查金币
	//	Money *money = knapsack->GetMoney();
	//	if (!money->CoinBindMoreThan(uplevel_config->m_coin))
	//	{
	//		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
	//		return false;
	//	}
	//}

	{
		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "UpLevel"))
		{
			return false;
		}

		// 铜钱
		//Money *money = knapsack->GetMoney();
		//if (!money->UseCoinBind(uplevel_config->m_coin, "UpLevel")) return false;

		//bindcoin_cost = uplevel_config->m_coin;
	}

	{
		// 生效

		short strength_drop_level = 0;

		//if (!use_protect_stuff)
		//{
		//	strength_drop_level = uplevel_config->GetStrengthDropLevel();
		//}

		// 计算强化等级
		short old_strength_level = equip_param->strengthen_level;
		short new_strength_level = old_strength_level - strength_drop_level;
		if (old_strength_level >= 1)
		{
			if (new_strength_level < 1) new_strength_level = 1;	// 强化最低掉为1级
		}
		if (new_strength_level < 0) new_strength_level = 0;

		equip_param->strengthen_level = (char)new_strength_level;

		//if (new_strength_level != old_strength_level)
		//{
		//	equip_param->strength_lucky_val = 0;
		//}

		// 换装备
		if (is_puton)
		{
			// 设置绑定
			if (consume_list.bind_item_num > 0)
			{
				equip_mgr->GridSetBind(equip_index, false);
			}

			if (!role->GetEquipmentManager()->TransformToNewEquip(equip_index, uplevel_config->m_new_equip_id)) return false;

			equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_UPLEVEL);
		}
		else
		{
			// 设置绑定
			if (consume_list.bind_item_num > 0)
			{
				knapsack->GridSetBind(equip_index, false);
			}

			knapsack->GridChangeEquipId(equip_index, uplevel_config->m_new_equip_id, true);
		}

		role->NoticeNum(noticenum::NT_EQUIP_UPLEVEL_SUCC);

		if (new_equip->GetLimitLevel() >= 50)
		{
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

			const char *equip_param_string = EquipParamDataClientString(UidToInt(role->GetUserId()), new_equip->GetItemId(), true, equip_data.invalid_time, equip_param, NULL);
			if (NULL != equip_param_string)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_uplevel,
					UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), (int)new_equip->GetItemId(), equip_param_string);
				if (length > 0)
				{
					if (new_equip->GetLimitLevel() >= 60)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}
		}
	}

	//const Equipment *equip_old = (const Equipment *)ITEMPOOL->GetItem(equip_data.item_id);
	gamelog::g_log_equipment.printf(LL_INFO, "[EquipFunction::EquipUpLevel Succ][user[%s %d] role_level[%d], [equip_name:%s equip_id:%d "
		"new_equip_name:%s new_equip_id:%d ]",
		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(),
		new_equip->GetItemName(), (int)new_equip->GetItemId());

	EventHandler::Instance().OnUpLevelEquipOperate(role, is_puton, new_equip->GetEquipType(), new_equip->GetColor(), equip_param->quality, new_equip->GetLimitLevel());
	TianShuXunZhu::CheckTuMoTianShu(role);

	return true;
}