#include "item/itemextern.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"

#include "engineadapter.h"

#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"

#include "obj/character/attribute.hpp"
#include "obj/character/role.h"

#include "other/shop/shop.hpp"
#include "other/sublock/sublock.hpp"
#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"
#include "protocal/msgitem.h"
#include "gameworld/gameworld/obj/character/role.h"
#include "gameworld/gameworld/other/shop/roleshop.hpp"

#include <set>
#include <algorithm>

bool ItemExtern::CheckIndexListNoDuplicate(short index_list_num, short *index_list)
{
	if (index_list_num <= 0) return true;
	if (NULL == index_list) return false;

	std::set<short> index_set;

	for (int i = 0; i < index_list_num; i++)
	{
		if (index_set.find(index_list[i]) != index_set.end()) return false;

		index_set.insert(index_list[i]);
	}

	return true;
}

bool ItemExtern::CheckItemByIndexList(Role *role, ItemID item_id, short check_num, short index_list_num, short *index_list, ItemConsumeInfo *item_consume_info, short *consume_num)
{
	if (NULL == role || ItemBase::INVALID_ITEM_ID == item_id || check_num < 0 || index_list_num <= 0 ||
		index_list_num > ItemNamespace::MAX_KNAPSACK_GRID_NUM || NULL == index_list || NULL == item_consume_info) return false;

	if (NULL != item_consume_info)
	{
		item_consume_info->consume_bind_item_num = 0;
		item_consume_info->consume_nobind_item_num = 0;
	}

	if (NULL != consume_num) *consume_num = 0;

	if (0 == check_num) return true;

	int count = 0;
	ItemGridData item_data;
	Knapsack *knapsack = role->GetKnapsack();

	for (int i = 0; i < index_list_num; ++i)
	{
		if (!knapsack->GetItemGridData(index_list[i], &item_data)) return false;

		if (item_id == item_data.item_id)
		{
			int valid_num = item_data.num - item_consume_info->consume_num_list[i];		// consume_num_list已记录了消耗信息
			if (valid_num > 0)
			{
				if (count + valid_num > check_num) valid_num = check_num - count;

				count += valid_num;
				item_consume_info->consume_num_list[i] += valid_num;

				if (item_data.is_bind)
				{
					item_consume_info->consume_bind_item_num += valid_num;
				}
				else
				{
					item_consume_info->consume_nobind_item_num += valid_num;
				}

				if (NULL != consume_num) *consume_num = count;

				if (count >= check_num) return true;
			}
		}
	}

	return false;
}

//bool ItemExtern::CheckItemByIndex(Role *role, ItemID item_id, short check_num, short knapsack_index, ItemGridData *item_data)
//{
//	if (NULL == role || 0 == item_id || check_num <= 0 || NULL == item_data) return false;
//
//	if (!role->GetKnapsack()->GetItemGridData(knapsack_index, item_data))
//	{
//		role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
//		return false;
//	}
//
//	if (item_data->item_id != item_id || item_data->num < check_num)
//	{
//		role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
//		return false;
//	}
//
//	return true;
//}
//
bool ItemExtern::PutInItemConfigList(int list_num, ItemConfigData *config_list, int *index_count, ItemID item_id, int item_num, bool is_bind)
{
	if (list_num <= 0 || NULL == config_list || NULL == index_count || *index_count > list_num || ItemBase::INVALID_ITEM_ID == item_id || item_num <= 0) return false;

	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return false;

	if (!ItemBase::CanMerge(item_id))
	{
		if (1 != item_num) return false;
	}
	else
	{
		for (int k = 0; k < *index_count; ++k)
		{
			if (config_list[k].item_id == item_id && ((0 != config_list[k].is_bind) == is_bind))
			{
				config_list[k].num += item_num;
				if (config_list[k].num > itembase->GetPileLimit())
				{
					item_num = config_list[k].num - itembase->GetPileLimit();
					config_list[k].num = itembase->GetPileLimit();
				}
				else
				{
					item_num = 0;
					break;
				}
			}
		}
	}

	if (item_num > 0)
	{
		for (; *index_count < list_num; (*index_count)++)
		{
			if (item_num <= 0) break;

			config_list[*index_count].item_id = item_id;
			config_list[*index_count].is_bind = is_bind;

			if (item_num > itembase->GetPileLimit())
			{
				config_list[*index_count].num = itembase->GetPileLimit();
				item_num -= itembase->GetPileLimit();
			}
			else
			{
				config_list[*index_count].num = item_num;
				item_num = 0;
			}
		}
	}

	return 0 == item_num;
}

int ItemExtern::GetGoldPriceByNum(ItemID item_id, short total_num, int total_gold_price, short num)
{
	if (total_num <= 0 || total_gold_price <= 0 || num <= 0) return 0;

	int unit_gold_price = 0;
	int unit_bind_gold_price = 0;
	LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(item_id, &unit_gold_price, &unit_bind_gold_price);
	if (unit_gold_price <= 0) unit_gold_price = total_gold_price;

	int ret_gold_price = num * unit_gold_price;
	if (ret_gold_price > total_gold_price) ret_gold_price = total_gold_price;

	return ret_gold_price;
}

bool ItemExtern::GetConsumeListByOrder(Role *role, bool nobind_first, int stuff_num, ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM], ItemConsumeList *result_list, bool notify_lack)
{
	/************************************************************************/
	/* 规则如下：
	如果非绑优先(nobind_first)
	先扣非绑再扣绑定
	如果绑定优先
	则优先扣绑定再扣非绑*/
	/************************************************************************/

	if (NULL == role || stuff_num <= 0 || stuff_num > ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM || 
		NULL == stuff_list || NULL == result_list) return false;

	int tmp_consume_num = stuff_num;
	ItemConsumeConfig tmp_consume_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	for (int i = 0; i < tmp_consume_num && i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++i)
	{
		if (stuff_list[i].num <= 0) return false;

		tmp_consume_list[i] = stuff_list[i];
	}

	static ItemList item_list_array[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	for (int i = 0; i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++i)
	{
		item_list_array[i].Reset();
	}

	result_list->Reset();

	Knapsack *knapsack = role->GetKnapsack();

	{
		// 获取每种物品的绑定列表和非绑定列表
		for (int i = 0;i < tmp_consume_num && i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++i)
		{
			if (!knapsack->GetListOfItem(tmp_consume_list[i].item_id, ItemNamespace::MAX_KNAPSACK_GRID_NUM,
				item_list_array[i].bind_index_list, item_list_array[i].bind_num_list, &(item_list_array[i].bind_count),
				item_list_array[i].nobind_index_list, item_list_array[i].nobind_num_list, &(item_list_array[i].nobind_count)))
			{
				return false;
			}
		}
	}

	{
		// 检查每种物品总量够不够
		for (int i = 0; i < tmp_consume_num && i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++i)
		{
			int count = item_list_array[i].GetTotalCount();
			if (count < tmp_consume_list[i].num)
			{
				if (!tmp_consume_list[i].buyable)						// 不能用元宝的就直接失败
				{
					if (notify_lack)
					{
						Protocol::SCLackItem li;
						li.item_id = tmp_consume_list[i].item_id;
						li.item_count = tmp_consume_list[i].num - count;
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&li, sizeof(li));
					}

					return false;			// 不能用元宝的就直接失败
				}

				// 换算成元宝					
				int auto_buy_num = tmp_consume_list[i].num - count;
				tmp_consume_list[i].num = count;	
				int price = 0;
				int bind_price = 0;
				if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(tmp_consume_list[i].item_id, &price, &bind_price)) 
				{
					if (notify_lack)
					{
						Protocol::SCLackItem li;
						li.item_id = tmp_consume_list[i].item_id;
						li.item_count = auto_buy_num;
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&li, sizeof(li));
					}
					
					role->NoticeNum(errornum::EN_SHOP_ITEM_NOT_EXIST);
					return false;
				}

				result_list->need_gold_num += (price * auto_buy_num);
				result_list->need_bind_gold_num += (bind_price * auto_buy_num);
				if (bind_price > 0)
				{
					result_list->real_need_bind_gold_num += (bind_price * auto_buy_num);
				}
				else
				{
					result_list->real_need_gold_num += (price * auto_buy_num);
				}

				if (result_list->buy_count < (int)(sizeof(result_list->buy_list) / sizeof(result_list->buy_list[0])))
				{
					result_list->buy_list[result_list->buy_count].item_id = tmp_consume_list[i].item_id;
					result_list->buy_list[result_list->buy_count].item_count = auto_buy_num;
					result_list->buy_list[result_list->buy_count].consume_gold = (price * auto_buy_num);
					result_list->buy_list[result_list->buy_count].consume_bind_gold = (bind_price * auto_buy_num);
					++ result_list->buy_count;
				}
			}
		}
	}

	for (int i = 0; i < tmp_consume_num && i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++i)
	{
		if (result_list->count >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) break;

		if (!SelectItemByOrderHelper(&item_list_array[i], tmp_consume_list[i].num, result_list, nobind_first)) return false;				
	}

	return true;
}

bool ItemExtern::GetItemDataWrapperFromConfig(const ItemConfigData *item_cfg, ItemDataWrapper *item_wrapper)
{
	if (NULL == item_cfg || NULL == item_wrapper) return false;

	const ItemBase *itembase = ITEMPOOL->GetItem(item_cfg->item_id);
	if (NULL == itembase) return false;

	item_wrapper->item_id = item_cfg->item_id;
	item_wrapper->num = item_cfg->num;
	item_wrapper->is_bind = item_cfg->is_bind ? 1 : 0;
	item_wrapper->invalid_time = itembase->CalInvalidTime();

	return true;
}

void ItemExtern::SendRewardListInfo(Role *role, ItemConfigData *reward_list, int reward_num, int notice_type, int mojing)
{
	if (NULL == role) return;

	if (notice_type <= Protocol::SCRewardListInfo::NOTICE_TYPE_INVAILD || notice_type >= Protocol::SCRewardListInfo::NOTICE_TYPE_MAX) return;

	static Protocol::SCRewardListInfo cmd;

	cmd.notice_reward_type = notice_type;
	cmd.mojing = mojing;
	cmd.reward_num = 0;

	if (NULL != reward_list)
	{
		for(int i = 0; i < reward_num && i < Protocol::SCRewardListInfo::REWARD_LIST_MAX_NUM; i++)
		{
			if (reward_list[i].item_id > 0 && reward_list[i].num > 0)
			{
				cmd.reward_list[cmd.reward_num].item_id = reward_list[i].item_id;
				cmd.reward_list[cmd.reward_num].num = reward_list[i].num;
				cmd.reward_list[cmd.reward_num].is_bind = reward_list[i].is_bind;
				cmd.reward_num++;
			}
			
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), 
		(const char*)&cmd, sizeof(cmd) - sizeof(cmd.reward_list[0]) * (Protocol::SCRewardListInfo::REWARD_LIST_MAX_NUM - cmd.reward_num));
}

bool ItemExtern::ConsumeItem(Role* role, ItemConsumeList &consume_list, const char * reason)
{
	if (NULL == role || NULL == reason) return false;

	if (role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	Money * role_money = role->GetKnapsack()->GetMoney();
	if (nullptr == role_money)
	{
		return false;
	}

	// 检查
	Int64 real_gold_bind = role_money->GetGoldBind();
	Int64 need_bind_gold_num = consume_list.real_need_bind_gold_num;
	Int64 need_gold_num = need_bind_gold_num - real_gold_bind;
	need_gold_num = need_gold_num < 0 ? 0 : need_gold_num;
	need_bind_gold_num = need_gold_num > 0 ? real_gold_bind : need_bind_gold_num;
	if (need_bind_gold_num > 0 && !role_money->GoldBindMoreThan(need_bind_gold_num))
	{
		return false;
	}

	// 检查
	need_gold_num += consume_list.real_need_gold_num;
	if (need_gold_num > 0 && !role_money->GoldMoreThan(need_gold_num))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	// 检查 & 消耗
	if (consume_list.count > 0 && !role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, reason))
	{
		role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	// 已检查 消耗
	if (need_bind_gold_num > 0) role_money->UseGoldBind(need_bind_gold_num, reason);
	if (need_gold_num > 0) role_money->UseGold(need_gold_num, reason, true);

	EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, need_gold_num, need_bind_gold_num);
	return true;
}

bool ItemExtern::CheckBindGoldAndGoldByConsumeList(Role * role, const ItemConsumeList & consume_list)
{
	if (NULL == role) return false;

	if (role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	Money * role_money = role->GetKnapsack()->GetMoney();
	if (nullptr == role_money)
	{
		return false;
	}

	// 检查
	Int64 real_gold_bind = role_money->GetGoldBind();
	Int64 need_bind_gold_num = consume_list.real_need_bind_gold_num;
	Int64 need_gold_num = need_bind_gold_num - real_gold_bind;
	need_gold_num = need_gold_num < 0 ? 0 : need_gold_num;
	need_bind_gold_num = need_gold_num > 0 ? real_gold_bind : need_bind_gold_num;
	if (need_bind_gold_num > 0 && !role_money->GoldBindMoreThan(need_bind_gold_num))
	{
		return false;
	}

	// 检查
	need_gold_num += consume_list.real_need_gold_num;
	if (need_gold_num > 0 && !role_money->GoldMoreThan(need_gold_num))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	return true;
}

bool ItemExtern::SelectItemByOrderHelper(ItemList *item_list, int need_count, ItemConsumeList *result_list, bool nobind_first)
{
	if (NULL == item_list || NULL == result_list || need_count < 0) return false;

	if (need_count == 0) return true;

	int have_nobind_count = item_list->GetNobindCount();
	int have_bind_count = item_list->GetBindCount();

	if (have_bind_count + have_nobind_count < need_count) return false;

	int nobind_need_count = 0;
	int bind_need_count = 0;

	if (nobind_first)
	{
		// 非绑定优先
		if (have_nobind_count >= need_count)
		{
			nobind_need_count = need_count;
			bind_need_count = 0;
		}
		else
		{
			nobind_need_count = have_nobind_count;
			bind_need_count = need_count - have_nobind_count;
		}
	}
	else
	{
		// 绑定优先
		if (have_bind_count >= need_count)
		{
			bind_need_count = need_count;
			nobind_need_count = 0;
		}
		else
		{
			bind_need_count = have_bind_count;
			nobind_need_count = need_count - have_bind_count;
		}
	}

	{
		// 查找非绑
		int now_nobind_count = 0;
		for (int i = 0; i < item_list->nobind_count && nobind_need_count > 0; ++i)	// 遍历这个物品的非绑列表
		{
			if (result_list->count >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) break;

			if (now_nobind_count + item_list->nobind_num_list[i] < nobind_need_count)
			{
				result_list->index_list[result_list->count] = item_list->nobind_index_list[i];
				result_list->num_list[result_list->count] = item_list->nobind_num_list[i];
				++ result_list->count;

				now_nobind_count += item_list->nobind_num_list[i];
			}
			else
			{
				result_list->index_list[result_list->count] = item_list->nobind_index_list[i];
				result_list->num_list[result_list->count] = nobind_need_count - now_nobind_count;
				++ result_list->count;

				now_nobind_count = nobind_need_count;
			}

			if (now_nobind_count >= nobind_need_count) break;
		}

		result_list->nobind_item_num += now_nobind_count;
	}

	{
		// 查找绑定
		int now_bind_count = 0;
		for (int i = 0; i < item_list->bind_count && bind_need_count > 0; ++i)	// 遍历这个物品的绑定列表
		{
			if (result_list->count >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) break;

			if (now_bind_count + item_list->bind_num_list[i] < bind_need_count)
			{
				result_list->index_list[result_list->count] = item_list->bind_index_list[i];
				result_list->num_list[result_list->count] = item_list->bind_num_list[i];
				++ result_list->count;

				now_bind_count += item_list->bind_num_list[i];
			}
			else
			{
				result_list->index_list[result_list->count] = item_list->bind_index_list[i];
				result_list->num_list[result_list->count] = bind_need_count - now_bind_count;
				++ result_list->count;

				now_bind_count = bind_need_count;
			}

			if (now_bind_count >= bind_need_count) break;
		}

		result_list->bind_item_num += now_bind_count;
	}

	return true;
}

ItemExtern::ItemConsumeList g_consume_list; 


int ItemExtern::ItemConsumeConfigList::consume_item_by_bagindex(Role& role, size_t index, short max_count, std::string reason)
{
	Knapsack* bag = role.GetKnapsack();
	if (index >= static_array_size(bag->m_grid_list))
	{
		return 0;
	}

	ItemGridData& grid = bag->m_grid_list[index];
	const ItemBase* itembase = ItemPool::Instance()->GetItem(grid.item_id);
	if (!itembase)
	{
		return 0;
	}

	auto consume_num = (std::min)(grid.num, max_count);
	if (consume_num <= 0)
	{
		return 0;
	}

	if (grid.gold_price > 0)
	{
		grid.gold_price -= ItemExtern::GetGoldPriceByNum(grid.item_id, grid.num, grid.gold_price, consume_num);
	}

	grid.num -= consume_num;
	if (grid.num <= 0)
	{
		grid.Clear();
	}

	bag->SetItemDirty(index);

	const char* equip_data_string = ItemGridDataLog(itembase, grid);
	if (equip_data_string)
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[ItemExtern::ItemConsumeConfigList::consume_item_by_bagindex Succ][user[%s %d] equipment_data:%s]",
			role.GetName(), UidToInt(role.GetUserId()), equip_data_string);

		const char* item_param_string = ItemGridQuickLog(itembase, grid);
		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, &role, itembase->GetItemId(), consume_num, reason.c_str(), item_param_string);
	}
	else if (itembase->IsRecord())
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[ItemExtern::ItemConsumeConfigList::consume_item_by_bagindex Succ][user[%s %d] item_id:%d item_name:%s(%d) consume_num:%d remain_num:%d reason:%s]",
			role.GetName(), UidToInt(role.GetUserId()), (int)itembase->GetItemId(), itembase->GetItemName(), (int)grid.is_bind, consume_num, (int)grid.num, reason.c_str());

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, &role, itembase->GetItemId(), consume_num, reason.c_str(), NULL);
	}

	return consume_num;
}

bool ItemExtern::ItemConsumeConfigList::done(Role& role, std::string reason)
{
	//检查能否完成本次消耗， 转换自动购买物品到货币。 绑定元宝到元宝
	if (!check(role))
		return false;
	Knapsack* bag = role.GetKnapsack();
	//物品
	for (auto& item : m_item_list)
	{
		auto all_count = item.second.count;
		for (auto index : item.second.m_item_index)
		{
			all_count -= consume_item_by_bagindex(role, index, all_count, reason);
			if (all_count == 0)
				break;
		}
	}

	//货币
	for (auto& coin : m_coin_list)
	{
		switch (coin.first)
		{
		case COIN_TYPE::BIND_COIN:
			bag->GetMoney()->UseCoinBind(coin.second, reason.c_str());
			break;
		case COIN_TYPE::BIND_GOLD:
			bag->GetMoney()->UseGoldBind(coin.second, reason.c_str());
			break;
		case COIN_TYPE::UNBIND_COIN:
			bag->GetMoney()->UseCoin(coin.second, reason.c_str());
			break;
		case COIN_TYPE::UNBIND_GOLD:
			bag->GetMoney()->UseGold(coin.second, reason.c_str());
			break;
		case COIN_TYPE::MOJING:
			role.GetRoleShop()->AddChestShopMojing(-static_cast<int>(coin.second), reason.c_str());
			break;
		default:
			break;
		}
	}

	return true;
}

bool ItemExtern::ItemConsumeConfigList::check(Role& role)
{
	if (!checkitem(role))
		return false;
	if (!checkcoin(role))
		return false;

	return true;
}

bool ItemExtern::ItemConsumeConfigList::checkcoin(Role& role)
{
	Money* money = role.GetKnapsack()->GetMoney();
	if (money->IsLock())
	{
		role.NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	auto it = m_coin_list.find(COIN_TYPE::BIND_COIN);
	if (it != m_coin_list.end())
	{//绑定铜钱，如果不足转换至铜钱
		auto bind_coin = money->GetCoinBind();
		if (bind_coin < it->second)
		{
			add_coin(COIN_TYPE::UNBIND_COIN, it->second - bind_coin);
			it->second = bind_coin;
		}
	}

	it = m_coin_list.find(COIN_TYPE::BIND_GOLD);
	if (it != m_coin_list.end())
	{//绑定元宝，如果不足转换至元宝
		auto bind_gold = money->GetGoldBind();
		if (bind_gold < it->second)
		{
			add_coin(COIN_TYPE::UNBIND_GOLD, it->second - bind_gold);
			it->second = bind_gold;
		}
	}

	it = std::find_if(m_coin_list.begin(), m_coin_list.end(), [&](decltype(m_coin_list)::value_type& value) ->bool {
		switch (value.first)
		{
		case COIN_TYPE::BIND_COIN://绑定铜钱不足转换铜钱.特殊处理.
		case COIN_TYPE::BIND_GOLD://绑定元宝不足转换元宝.特殊处理.
			return false;
		case COIN_TYPE::UNBIND_GOLD:
			if (money->GoldMoreThan(value.second))
			{
				return false;
			}
			role.NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return true;
		case COIN_TYPE::UNBIND_COIN:
			if (money->CoinMoreThan(value.second))
			{
				return false;
			}
			role.NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
			return true;
		case COIN_TYPE::MOJING:
			if (role.GetRoleShop()->GetChestShopMojing() >= value.second)
			{
				return false;
			}
			role.NoticeNum(errornum::EN_MOJING_NOT_ENOUGH);
			return true;
		default://本函数未处理的类型,检测不通过
			return true;
		}
	});

	return it == m_coin_list.end();
}

bool ItemExtern::ItemConsumeConfigList::checkitem(Role& role)
{
	Knapsack* bag = role.GetKnapsack();
	if (bag->IsLock())
	{
		role.NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	auto it = std::find_if(m_item_list.begin(), m_item_list.end(), [&](decltype(m_item_list)::value_type& value)->bool {
		int bind_item_count = 0;
		int unbind_item_count = 0;
		for (int i = 0; i < bag->m_valid_grid_index_end && i < (int)static_array_size(bag->m_grid_list); i++)
		{
			auto& item = bag->m_grid_list[i];
			if (item.Invalid() || item.item_id != value.first)
			{
				continue;
			}

			//记录数量以及index
			if (item.is_bind)
			{
				bind_item_count += item.num;
				value.second.m_item_index.push_front(i);
			}
			else
			{
				unbind_item_count += item.num;
				value.second.m_item_index.push_back(i);
			}

			//足够绑定数量跳出，优先消耗绑定材料。
			if (bind_item_count >= value.second.count) {
				return false;
			}
		}

		int all_item_count = bind_item_count + unbind_item_count;
		if (all_item_count >= value.second.count)
		{
			return false;
		}

		if (!value.second.buyable)
		{//材料不足，又没有自动购买
			role.NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return true;
		}

		// 换算成元宝					
		int auto_buy_num = value.second.count - all_item_count;
		int unbind_price = 0;
		int bind_price = 0;
		if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(value.first, &unbind_price, &bind_price))
		{//无法自动购买
			Protocol::SCLackItem li;
			li.item_id = value.first;
			li.item_count = auto_buy_num;
			EngineAdapter::Instance().NetSend(role.GetNetId(), (const char*)&li, sizeof(li));
			role.NoticeNum(errornum::EN_SHOP_ITEM_NOT_EXIST);
			return true;
		}

		value.second.count = all_item_count;
		if (bind_price > 0)
		{
			add_coin(COIN_TYPE::BIND_GOLD, bind_price * auto_buy_num);
		}
		else if (unbind_price > 0)
		{
			add_coin(COIN_TYPE::UNBIND_GOLD, unbind_price * auto_buy_num);
		}
		else
		{
			return true;
		}
		return false;
	});

	//判断完所有元素 返回检查成功
	return it == m_item_list.end();
}

void ItemExtern::ItemConsumeConfigList::add_item(const ItemConsumeConfig& config)
{
	if (config.num <= 0)
	{
		return;
	}

	auto it = m_item_list.find(config.item_id);
	if (it == m_item_list.end())
	{
		m_item_list.insert(std::make_pair(config.item_id, item_num_info(config.num, config.buyable)));
		return;
	}
	it->second.count += config.num;
	if (config.buyable)
	{
		it->second.buyable = config.buyable;
	}
}


void ItemExtern::ItemConsumeConfigList::add_coin(COIN_TYPE typ, Int64 num) {
	if (num <= 0)
	{
		return;
	}
	m_coin_list[typ] += num;
}