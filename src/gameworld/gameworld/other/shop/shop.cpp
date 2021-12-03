#include "shop.hpp"

#include "engineadapter.h"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "item/itemextern.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gamelog.h"

#include "servercommon/configcommon.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"

Shop::Shop()
{

}

Shop::~Shop()
{

}

bool Shop::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "Shop", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load Shop config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	PugiXmlNode ShopElement = RootElement.child("item");
	if (ShopElement.empty())
	{
		sprintf(errinfo,"%s: xml node error in node [config->shop].", configname.c_str());
		*err = errinfo;
		return false;
	}

	PugiXmlNode ShopItemElement = ShopElement.child("data");

	while (!ShopItemElement.empty())
	{
		ShopItem shopitem;

		if (!PugiGetSubNodeValue(ShopItemElement, "itemid", shopitem.item_id ) || shopitem.item_id  <= 0)
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item->item_id]\nid[%d].", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}
		if (NULL == ITEMPOOL->GetItem(shopitem.item_id))
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item->item_id] id[%d] cannot be get from item pool.", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(ShopItemElement, "gold", shopitem.price_gold) || shopitem.price_gold <= 0)
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item->gold] id[%d]", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(ShopItemElement, "bind_gold", shopitem.price_bind_gold) || shopitem.price_bind_gold < 0 || (shopitem.price_bind_gold != 0 && shopitem.price_bind_gold < shopitem.price_gold))
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item->price_bind_gold] id[%d]", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}

		if (shopitem.price_gold != 0 && shopitem.price_bind_gold != 0 && shopitem.price_gold != shopitem.price_bind_gold)
		{
			sprintf(errinfo, "%s:\nxml [%d] price_gold and price_bind_gold is diffrence", configname.c_str(), shopitem.item_id);
			*err = errinfo;
			return false;
		}

		if (shopitem.price_gold <= 0 && shopitem.price_bind_gold <= 0)
		{
			sprintf(errinfo,"%s:\nxml [%d] price_gold and price_bind_gold is 0", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(ShopItemElement, "is_notice", shopitem.is_notice) || shopitem.is_notice < 0)
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item->is_notice] id[%d]", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}

		ShopItemMapIt it = m_shop_item_map.find(shopitem.item_id);
		if (it != m_shop_item_map.end())
		{
			sprintf(errinfo,"%s:\nxml node error in node [config->shop->item id[%d]] repeat", configname.c_str(), shopitem.item_id);
			*err =  errinfo;
			return false;
		}
		else
		{
			m_shop_item_map[shopitem.item_id] = shopitem;
		}
		
		ShopItemElement = ShopItemElement.next_sibling();
	}

	return true;
}

void Shop::OnBuyItemReq(Role *role, Protocol::CSShopBuy *bs)
{
	if (role->GetSublock()->CheckHasLock()) return;

	this->BuyShopNormalItemReq(role, bs->item_id, bs->item_num, bs->is_bind >0 , bs->if_use > 0);
}

void Shop::BuyShopNormalItemReq(Role *role, UInt16 item_id, int num, bool is_bind, bool if_use)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return;
	
	if (num <= 0 || num > itembase->GetPileLimit()) return;

	if (!ItemBase::CanMerge(item_id) && 1 != num) return;

	ShopItemMapIt it = m_shop_item_map.find(item_id);
	if (it == m_shop_item_map.end())
	{	
		role->NoticeNum(errornum::EN_SHOP_ITEM_NOT_EXIST);
		return;
	}

	int price = is_bind ? it->second.price_bind_gold : it->second.price_gold;
	if (price <= 0)
	{
		return;
	}

	int total_price = price * num;

	if (!this->CanBuy(role, total_price, is_bind)) 
	{
		return;
	}

	Int64 consume_gold = 0, consume_gold_bind = 0;

	char consume_type[32] = {0};
	SNPRINTF(consume_type, sizeof(consume_type), "ShopBuy|%d", item_id);

	if (!is_bind)
	{
		consume_gold = total_price;
		role->GetKnapsack()->GetMoney()->UseGold(consume_gold, consume_type);
	}
	else
	{
		consume_gold_bind = total_price;
		role->GetKnapsack()->GetMoney()->UseGoldBind(consume_gold_bind, consume_type);
	}

	ItemConfigData item_config_data(item_id, (consume_gold_bind > 0), num);

	int out_index = 0;

	auto put_reason = PUT_REASON_SHOP_BUY;
	if (if_use)
	{
		put_reason = PUT_REASON_NO_NOTICE;
	}
	if (!role->GetKnapsack()->Put(item_config_data, put_reason, &out_index, 0, static_cast<int>(consume_gold)))
	{
		return;
	}

	role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);

	if (0 != it->second.is_notice)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shop_buy_notice, 
			UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), (int)item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	ItemExtern::BuyItemInfo buy_info;
	buy_info.item_id = item_config_data.item_id;
	buy_info.item_count = num;
	buy_info.consume_gold = static_cast<int>(consume_gold + consume_gold_bind);
	EventHandler::Instance().OnShopBuyItem(role, 1, &buy_info, consume_gold, consume_gold_bind);	

	if (if_use)
	{
		role->GetKnapsack()->Use(out_index, num);
	}

	role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_SHOP_BUY_NUM, num);
}

bool Shop::GetShopItemGoldPrice(UInt16 item_id, int *gold_price, int *bind_gold_price)
{
	if (NULL == gold_price || NULL == bind_gold_price) return false;

	ShopItemMapIt it = m_shop_item_map.find(item_id);
	if (it == m_shop_item_map.end()) return false;
	if (it->second.price_gold <= 0 && it->second.price_bind_gold <= 0) return false;

	*gold_price = it->second.price_gold;
	*bind_gold_price = it->second.price_bind_gold;

	return true;
}

bool Shop::CanBuy(Role *role, int total_price, bool is_bind)
{
	Knapsack *knapsack = role->GetKnapsack();
	Money *money = knapsack->GetMoney();

	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!knapsack->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	if (!is_bind && !money->GoldMoreThan(total_price))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (is_bind && !money->GoldBindMoreThan(total_price))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return false;
	}

	return true;
}
