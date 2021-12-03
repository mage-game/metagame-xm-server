#include "itemfunction.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "item/itempool.h"
#include "item/itemextern.hpp"
#include "item/expense/norexitem.hpp"
#include "item/knapsack.h"

#include "obj/character/role.h"

#include "gamelog.h"

#include "config/logicconfigmanager.hpp"
#include "globalconfig/composeconfig.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/equipmentfunction.h"

#include "other/event/eventhandler.hpp"

#include "protocal/msgitem.h"
#include "equipment/equipmentmanager.h"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"

// 物品合成
bool ItemFunction::ItemCompose(Role *role, int product_seq, int product_num, int compose_type)
{
	if (role->GetSublock()->CheckHasLock()) return false;

	const ProductConfig * compose_config = LOGIC_CONFIG->GetComposeConfig().GetProductCfg(product_seq);
	if (NULL == compose_config)
	{
		role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_CANNOT);
		return false;
	}

	const ItemBase *itembase = ITEMPOOL->GetItem(compose_config->m_product_id);
	if (NULL == itembase) return false;
	
	//圣印合成
	{
		const NorexItem* norex_item_cfg = dynamic_cast<const NorexItem*>(itembase);
		if (NULL != norex_item_cfg)
		{
			short use_type = norex_item_cfg->GetUseType();
			switch (use_type)
			{
				case NorexItem::I_NOREX_ITEM_ITEM_SEAL:
					return role->GetSeal()->ComposeSeal(product_num, compose_config);
					break;

				default:
					break;
			}
		}
	}
// 去除根据目标堆叠数 限制 每次合成数量(2018-10-27-高聪)
//	if (product_num <= 0 || product_num > itembase->GetPileLimit()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!Composable(role, itembase))
	{
		return false;
	}

	int bind_product_count = 0;

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	UNSTD_STATIC_CHECK(MAX_COMPOSE_STUFF_TYPE <= ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM);

	{
		// 选材 并且 检查总量
		for (int i = 0; i < MAX_COMPOSE_STUFF_TYPE; ++i)
		{
			if (ItemBase::INVALID_ITEM_ID != compose_config->m_stuff_id_list[i])
			{
				if (compose_config->m_stuff_count_list[i] <= 0) return false;

				static ItemExtern::ItemList item_list;
				item_list.Reset();

				if (!knapsack->GetListOfItem(compose_config->m_stuff_id_list[i], ItemNamespace::MAX_KNAPSACK_GRID_NUM,
					item_list.bind_index_list, item_list.bind_num_list, &(item_list.bind_count),
					item_list.nobind_index_list, item_list.nobind_num_list, &(item_list.nobind_count)))
				{
					return false;
				}

				int need_total_count = product_num * compose_config->m_stuff_count_list[i];
				int own_item_count = item_list.GetTotalCount();

				if (own_item_count < need_total_count)
				{
// 					if (Protocol::CSItemCompose::COMPOSE_TYPE_EXCHANGE == compose_type)
// 					{
// 						role->NoticeItemLack(compose_config->m_stuff_id_list[i], need_total_count - own_item_count);
// 					}
// 					else
// 					{
// 						role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
// 					}

					role->NoticeItemLack(compose_config->m_stuff_id_list[i], need_total_count - own_item_count);

					return false;
				}

				{
					stuff_list[stuff_count].item_id = compose_config->m_stuff_id_list[i];
					stuff_list[stuff_count].num = need_total_count;
					stuff_list[stuff_count].buyable = false;
					++ stuff_count;
				}

				if (bind_product_count < product_num)
				{
					int bind_count = item_list.GetBindCount();
					int max_bind_product_count = bind_count / compose_config->m_stuff_count_list[i];

					if (bind_count % compose_config->m_stuff_count_list[i] > 0) ++max_bind_product_count;

					if (bind_product_count < max_bind_product_count) bind_product_count = max_bind_product_count;
				}
			}
		}

		if (0 == stuff_count) return false;

		if (bind_product_count > product_num) bind_product_count = product_num;
	}


	short pile_limit = itembase->GetPileLimit();
	if (pile_limit < 1)
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}
	int need_grid_num = 0;
	if (bind_product_count > 0)
	{
		need_grid_num += (bind_product_count / pile_limit) + 1;
	}
	if (product_num - bind_product_count > 0)
	{
		need_grid_num += (product_num - bind_product_count) / pile_limit + 1;
	}


	if (!knapsack->CheckEmptyGridNoLessThan(need_grid_num))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	// 检查铜钱
	if (compose_config->m_coin > 0 && !knapsack->GetMoney()->AllCoinMoreThan(product_num * compose_config->m_coin))
	{
		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}

	// 检查女娲石
	if (compose_config->m_nv_wa_shi > 0 && role->GetNvWaShi() < product_num * compose_config->m_nv_wa_shi)
	{
		role->NoticeNum(errornum::EN_ROLE_NV_WA_SHI_NOT_ENOUGH);
		return false;
	}

	{
		// 合成是绑定优先
		ItemExtern::ItemConsumeList consume_list;
		if (!ItemExtern::GetConsumeListByOrder(role, false, stuff_count, stuff_list, &consume_list)) return false;

		Int64 bindcoin = compose_config->m_coin, nobindcoin = 0;
		if (compose_config->m_coin > 0)
		{
			if (!knapsack->GetMoney()->UseAllCoin(product_num * compose_config->m_coin, "Compose")) return false;
		}

		if (compose_config->m_nv_wa_shi > 0)
		{
			if (!role->AddNvWaShi(-1 * product_num * compose_config->m_nv_wa_shi, "Compose")) return false;
		}

		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "Compose")) return false;
		
		int grid_index = -1;
		if (bind_product_count > 0 && !knapsack->Put(ItemConfigData(compose_config->m_product_id, true, bind_product_count), PUT_REASON_COMPOSE, &grid_index)) return false;
		
		if (ItemBase::I_TYPE_EQUIPMENT == itembase->GetItemType())  // 装备仙品
		{	
			if(grid_index >= 0)
			{
				ItemGridData item_grid_data;
				role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);

				if (EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_COMPOSE, 0))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}

				grid_index = -1;
			}
		}

		if (product_num > bind_product_count && !knapsack->Put(ItemConfigData(compose_config->m_product_id, false, product_num - bind_product_count), PUT_REASON_COMPOSE, &grid_index)) return false;

		if (ItemBase::I_TYPE_EQUIPMENT == itembase->GetItemType())  // 装备仙品
		{	
			if(grid_index >= 0)
			{
				ItemGridData item_grid_data;
				role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);

				if (EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_COMPOSE, 0))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
		}

		role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_DAY_COMPOSE_NUM, product_num);

		UNSTD_STATIC_CHECK(4 == MAX_COMPOSE_STUFF_TYPE);

		gamelog::g_log_item.printf(LL_INFO,"[ItemFunction::ItemCompose Succ][user[%s %d]"
			"[product_item_name:%s id:%d bind_count:%d nobind_count:%d "
			"cost_itemid0:%d num0:%d "
			"cost_itemid1:%d num1:%d "
			"cost_itemid2:%d num2:%d "
			"cost_itemid3:%d num3:%d "
			"cost_bindcoin:%d coin:%d]",
			role->GetName(), UidToInt(role->GetUserId()), 
			itembase->GetItemName(), (int)itembase->GetItemId(), bind_product_count, product_num - bind_product_count, 
			compose_config->m_stuff_id_list[0], product_num * compose_config->m_stuff_count_list[0],
			compose_config->m_stuff_id_list[1], product_num * compose_config->m_stuff_count_list[1],
			compose_config->m_stuff_id_list[2], product_num * compose_config->m_stuff_count_list[2],
			compose_config->m_stuff_id_list[3], product_num * compose_config->m_stuff_count_list[3],
			(int)bindcoin, (int)nobindcoin);
	}

	if (Protocol::CSItemCompose::COMPOSE_TYPE_EXCHANGE == compose_type)
	{
		role->NoticeNum(noticenum::NT_EQUIP_EXCHANGE_SUCC);

		if (compose_config->m_coin > 0)
		{
			ROLE_LOG_QUICK10(LOG_TYPE_GET_ITEM, role, compose_config->m_product_id, LOG_GET_ITEM_WAY_COMPOSE, NULL, NULL, LOG_MONEY_TYPE_COIN, compose_config->m_coin * product_num, 
				role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi());
		}

		for (int i = 0; i < MAX_COMPOSE_STUFF_TYPE; ++ i)
		{
			if (compose_config->m_stuff_count_list[i] > 0)
			{
				ROLE_LOG_QUICK10(LOG_TYPE_GET_ITEM, role, compose_config->m_product_id, LOG_GET_ITEM_WAY_COMPOSE, NULL, NULL, 
					compose_config->m_stuff_id_list[i], product_num * compose_config->m_stuff_count_list[i], 
					role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi());
			}
		}
	}	
	else
	{
		role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
	}

	if (1 == compose_config->is_broadcast)
	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "baizhuan_equip_uplevel", UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(),
			compose_config->m_product_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}

	EventHandler::Instance().OnItemCompose(role, compose_config->m_product_id, product_num);

	
	return true;
}

bool ItemFunction::Composable(Role *role, const ItemBase *itembase)
{
	
	return true;
}
