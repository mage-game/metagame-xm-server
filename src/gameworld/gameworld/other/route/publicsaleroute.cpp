#include "obj/character/role.h"

#include "internalcomm.h"
#include "engineadapter.h"

#include "item/itembase.h"
#include "item/itempool.h"

#include "servercommon/tradedef.hpp"
#include "servercommon/errornum.h"
#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/struct/itemlistparam.h"
#include "gamelog.h"

#include "other/event/eventhandler.hpp"

#include "gameworld/gamelog.h"

#include "config/logicconfigmanager.hpp"
#include "publicsaleroute.hpp"

#include "global/trade/userpublicsalemanager.hpp"
#include "globalconfig/globalconfig.h"
#include "item/knapsack.h"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"

PublicSaleRoute & PublicSaleRoute::Instance()
{
	static PublicSaleRoute publicsale;
	return publicsale;
}

void PublicSaleRoute::OnAddPublicSaleItem(Role *role, Protocol::CSAddPublicSaleItem *add_sale_req)
{
	if (role->GetSublock()->CheckHasLock()) return;
	if (add_sale_req->sale_item_type <= PUBLIC_SALE_ITEM_TYPE_INVALID || add_sale_req->sale_item_type >= PUBLIC_SALE_ITEM_TYPE_MAX) return;
	if (add_sale_req->price_type <= PUBLIC_SALE_PRICE_TYPE_INVALID  || add_sale_req->price_type >= PUBLIC_SALE_PRICE_TYPE_MAX) return;

	//���۽�ң�������Ԫ���ļ۸�
	if (add_sale_req->sale_item_type == PUBLIC_SALE_ITEM_TYPE_COIN && add_sale_req->price_type != PUBLIC_SALE_PRICE_TYPE_GOLD) return;

	if (add_sale_req->sale_index < 0 || add_sale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return;

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (role->GetLevel() < PUBLICSALE_LEVEL_LIMIT)
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_ROLE_LEVEL_LIMIT);
		return;
	}

	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_LOGIN_TIME_LIMIT);
		return;
	}

	// ��������̫�󣬲߻�Ҫ���
	if (PUBLIC_SALE_PRICE_TYPE_GOLD == add_sale_req->price_type && add_sale_req->price > GLOBALCONFIG->GetPublicSaleMaxPrice())
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_HIGGHEST_PRICE);
		return;
	}

	//���۵�����Ʒ������Ƿ�����Ʒ
	SaleItemParam saleitem_param;

	{
		if(PUBLIC_SALE_ITEM_TYPE_ITEM == add_sale_req->sale_item_type)
		{
			if (add_sale_req->item_num <= 0 || add_sale_req->price <= 0 || add_sale_req->price > 99999999) 
			{
				return;
			}

			if (add_sale_req->knapsack_index < 0)
			{
				return;
			}

			ItemGridData item_data;
			if (!role->GetKnapsack()->GetItemGridData(add_sale_req->knapsack_index, &item_data))
			{	
				role->NoticeNum(errornum::EN_PUBLICSALE_NOT_HAVE_ITEM);
				return;
			}

			// ��Ʒ�������
			if (item_data.num < add_sale_req->item_num) 
			{	
				role->NoticeNum(errornum::EN_PUBLICSALE_NOT_ENOUGH_ITEM);
				return;
			}

			// ��Ʒ�Ƿ��ܽ���
			if (!ItemBase::CanTrade(&item_data))
			{
				role->NoticeNum(errornum::EN_PUBLICSALE_ITEM_CANNOT_SALE);
				return;
			}

			const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);
			if (NULL == item_base) return;

			// ��Ʒ�������ͼ��
			if (item_base->GetSearchType() <= 0)
			{
				role->NoticeNum(errornum::EN_PUBLICSALE_ITEM_CANNOT_SALE);
				return;
			}

			item_data.WrapTo(&saleitem_param.item_wrapper);
			saleitem_param.item_wrapper.num = add_sale_req->item_num;
			saleitem_param.sale_type = item_base->GetSearchType();
			saleitem_param.color = item_base->GetColor();
			saleitem_param.level = item_base->GetLimitLevel();
			saleitem_param.prof = item_base->GetLimitProf();

			if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				saleitem_param.order = ((Equipment *)item_base)->GetOrder();
			}
		}
	}

	//���۵��ǽ�ң�������Ƿ�����
	{
		if(PUBLIC_SALE_ITEM_TYPE_COIN == add_sale_req->sale_item_type)
		{
			if (add_sale_req->sale_value <= 0 || add_sale_req->price <= 0 || add_sale_req->price > 99999999)
			{
				return;
			}

			if (!role->GetKnapsack()->GetMoney()->CoinMoreThan(add_sale_req->sale_value))
			{
				role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
				return;
			}

			saleitem_param.sale_type = PUBLICSALE_COIN_SALE_TYPE;
			saleitem_param.color = ItemBase::I_QUALITY_RED;
		}
	}

	gglobalprotocal::AddPublicSaleItem gg_add_sale_req;
	// ����
	{
		gg_add_sale_req.db_index = role->GetUserId().db_index;
		gg_add_sale_req.role_id = role->GetUserId().role_id;
		role->GetName(gg_add_sale_req.role_name);
		gg_add_sale_req.is_to_world = add_sale_req->is_to_world;
		gg_add_sale_req.camp = role->GetCamp();

		gg_add_sale_req.item_param = saleitem_param;
		gg_add_sale_req.item_param.item_wrapper.gold_price = 0;

		gg_add_sale_req.item_param.sale_index = add_sale_req->sale_index;
		gg_add_sale_req.item_param.price = add_sale_req->price;

		gg_add_sale_req.item_param.sale_value = add_sale_req->sale_value;
		gg_add_sale_req.item_param.price_type = add_sale_req->price_type;
		gg_add_sale_req.item_param.sale_item_type = add_sale_req->sale_item_type;

		gg_add_sale_req.item_param.sale_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		gg_add_sale_req.item_param.due_time = gg_add_sale_req.item_param.sale_time + GLOBALCONFIG->GetPublicSaleSaleOutTime(); 
	}
	

	// �۳�
	{
		if (PUBLIC_SALE_ITEM_TYPE_COIN == add_sale_req->sale_item_type)
		{
			if (!role->GetKnapsack()->GetMoney()->UseCoin(add_sale_req->sale_value, "UseByPublicSale")) return;
		}

		if (PUBLIC_SALE_ITEM_TYPE_ITEM == add_sale_req->sale_item_type)
		{
			if (!role->GetKnapsack()->ConsumeItemByIndex(add_sale_req->knapsack_index, gg_add_sale_req.item_param.item_wrapper.num, "AddPublicSaleItem")) return;
		}
	}
	
	UserPublicSaleManager::Instance().OnAddPublicSaleItem(role, &gg_add_sale_req);

	role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_PUBLICSALE_NUM, 1);

	gamelog::g_log_publicsale.printf(LL_INFO, "[Publicsale::AddPublicSale Succ][user[%d %s] %s]",
		UidToInt(role->GetUserId()), (const char*)role->GetName(), SaleItemParamLog(&gg_add_sale_req.item_param));
}

void PublicSaleRoute::OnRemovePublicSaleItem(Role *role, Protocol::CSRemovePublicSaleItem *remove_sale_req)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (remove_sale_req->sale_index < 0 || remove_sale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return;

	gglobalprotocal::RemovePublicSaleItem gg_remove_sale_req;

	gg_remove_sale_req.db_index = role->GetUserId().db_index;
	gg_remove_sale_req.role_id = role->GetUserId().role_id;
	gg_remove_sale_req.sale_index = remove_sale_req->sale_index;

	UserPublicSaleManager::Instance().OnRemovePublicSaleItem(role, &gg_remove_sale_req);

	gamelog::g_log_publicsale.printf(LL_INFO, "[Publicsale::RemovePublicSale Succ][user[%d %s] sale_index:%d]", 
		UidToInt(UserID((int)gg_remove_sale_req.db_index, (int)gg_remove_sale_req.role_id)), (const char*)role->GetName(), (int)gg_remove_sale_req.sale_index);
}

void PublicSaleRoute::OnBuyPublicSaleItem(Role *role, Protocol::CSBuyPublicSaleItem *buy_publicsale)
{
	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_LOGIN_TIME_LIMIT);
		return;
	}

	if (!GLOBALCONFIG->IsAllowGmPublicSale() && Role::AUTHORITY_TYPE_TEST == role->GetAuthority())
	{
		return;
	}

	if (role->GetSublock()->CheckHasLock()) return;

	if (buy_publicsale->sale_item_type <= PUBLIC_SALE_ITEM_TYPE_INVALID || buy_publicsale->sale_item_type >= PUBLIC_SALE_ITEM_TYPE_MAX) return;
	if (buy_publicsale->price_type <= PUBLIC_SALE_PRICE_TYPE_INVALID  || buy_publicsale->price_type >= PUBLIC_SALE_PRICE_TYPE_MAX) return;

	//�����ң�������Ԫ���ļ۸�
	if (PUBLIC_SALE_ITEM_TYPE_COIN == buy_publicsale->sale_item_type && buy_publicsale->price_type != PUBLIC_SALE_PRICE_TYPE_GOLD) return;

	if (buy_publicsale->sale_index < 0 || buy_publicsale->sale_index >= PUBLICSALE_MAX_ITEM_COUNT || buy_publicsale->price <= 0 ||
		buy_publicsale->seller_uid <= 0 || buy_publicsale->seller_uid == UidToInt(role->GetUserId()))
	{
		return;
	}

	{
		// ������Ʒ�������Ʒ�ϸ���
		if (PUBLIC_SALE_ITEM_TYPE_ITEM == buy_publicsale->sale_item_type)
		{
			if (buy_publicsale->item_id <= 0 || buy_publicsale->item_num <= 0) return;

			if (NULL == ITEMPOOL->GetItem(buy_publicsale->item_id)) return; 
		}

		// ����ͭ�ң����ͭ�Һϸ���
		if (PUBLIC_SALE_ITEM_TYPE_COIN == buy_publicsale->sale_item_type)
		{
			if (buy_publicsale->sale_value <= 0) return;
		}
	}
	
	{
		// Ԫ���۸�
		if (PUBLIC_SALE_PRICE_TYPE_GOLD == buy_publicsale->price_type)
		{
			if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(buy_publicsale->price))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}

		// ͭǮ�۸�
		if (PUBLIC_SALE_PRICE_TYPE_COIN == buy_publicsale->price_type)
		{
			if (!role->GetKnapsack()->GetMoney()->CoinMoreThan(buy_publicsale->price))
			{
				role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
				return;
			}
		}
	}
	
	// ����
	{
		if (PUBLIC_SALE_PRICE_TYPE_GOLD == buy_publicsale->price_type)
		{
			role->GetKnapsack()->GetMoney()->UseGold(buy_publicsale->price, "PublicSaleBuy", false);
		}
		
		if (PUBLIC_SALE_PRICE_TYPE_COIN == buy_publicsale->price_type)
		{
			role->GetKnapsack()->GetMoney()->UseCoin(buy_publicsale->price, "PublicSaleBuy");
		}
	}

	gglobalprotocal::BuyPublicSaleItem gg_buy_sale_req;

	gg_buy_sale_req.db_index = role->GetUserId().db_index;
	gg_buy_sale_req.role_id = role->GetUserId().role_id;
	F_STRNCPY(gg_buy_sale_req.role_name, role->GetName(), sizeof(GameName));

	UserID seller_user_id = IntToUid(buy_publicsale->seller_uid);
	
	gg_buy_sale_req.item_id = buy_publicsale->item_id;
	gg_buy_sale_req.item_num = buy_publicsale->item_num;
	gg_buy_sale_req.sale_value = buy_publicsale->sale_value;
	gg_buy_sale_req.price = buy_publicsale->price;
	gg_buy_sale_req.sale_item_type = buy_publicsale->sale_item_type;
	gg_buy_sale_req.price_type = buy_publicsale->price_type;

	gg_buy_sale_req.seller_db_index = seller_user_id.db_index;
	gg_buy_sale_req.seller_role_id = seller_user_id.role_id;
	gg_buy_sale_req.sale_index = buy_publicsale->sale_index;
	
	UserPublicSaleManager::Instance().OnBuyPublicSaleItem(role, &gg_buy_sale_req);

	if (PUBLIC_SALE_ITEM_TYPE_ITEM == buy_publicsale->sale_item_type)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(gg_buy_sale_req.item_id);
		if (NULL != itembase)
		{
			gamelog::g_log_publicsale.printf(LL_INFO, "[Publicsale::BuyPublicSaleItem Succ][user[%d %s] seller[%d] item_id:%d item_name:%s item_num:%d gold_price:%d] trace[sale_index:%d]",
				UidToInt(UserID((int)gg_buy_sale_req.db_index, (int)gg_buy_sale_req.role_id)), (const char*)role->GetName(),
				UidToInt(UserID((int)gg_buy_sale_req.seller_db_index, (int)gg_buy_sale_req.seller_role_id)),
				(int)gg_buy_sale_req.item_id, NULL == itembase ? "" : (const char*)itembase->GetItemName(), (int)gg_buy_sale_req.item_num, 
				(int)gg_buy_sale_req.price, (int)gg_buy_sale_req.sale_index);
		}
	}
	
	if (PUBLIC_SALE_ITEM_TYPE_COIN == buy_publicsale->sale_item_type)
	{
		//����ͭǮ��־����
	}
	
}

void PublicSaleRoute::OnGetPublicSaleUserItemList(Role *role, Protocol::CSPublicSaleGetUserItemList *get_publicsale_itemlist)
{
	UserPublicSaleManager::Instance().OnGetPublicSaleItemList(role);
}

void PublicSaleRoute::OnSendPublicSaleItemInfoToWorld(Role *role, Protocol::CSPublicSaleSendItemInfoToWorld *send_publicsale_iteminfo)
{
	if (role->GetSublock()->CheckHasLock()) return;

	if (send_publicsale_iteminfo->sale_index < 0 || send_publicsale_iteminfo->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return;

	if (role->GetLevel() < PUBLICSALE_LEVEL_LIMIT)
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_ROLE_LEVEL_LIMIT);
		return;
	}

	//�߻������г�ߺ�����
	//if (!role->GetKnapsack()->GetMoney()->UseAllCoin(PUBLICSALE_TO_WORLD_COST, "PublicSaleBroadcast"))
	//{
	//	role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
	//	return;
	//}

	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_PUBLICSALE_LOGIN_TIME_LIMIT);
		return;
	}

	gglobalprotocal::PublicSaleSendItemInfoToWorld gg_send_iteminfo_toworld;
	gg_send_iteminfo_toworld.db_index = role->GetUserId().db_index;
	gg_send_iteminfo_toworld.role_id = role->GetUserId().role_id;
	gg_send_iteminfo_toworld.camp = role->GetCamp();
	gg_send_iteminfo_toworld.sale_index = send_publicsale_iteminfo->sale_index;
	role->GetName(gg_send_iteminfo_toworld.role_name);
	
	UserPublicSaleManager::Instance().OnSendPublicSaleItemInfoToWorld(role, &gg_send_iteminfo_toworld);
}
