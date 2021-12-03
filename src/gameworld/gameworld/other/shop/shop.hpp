#ifndef __SHOP_HPP__
#define __SHOP_HPP__

#include <map>
#include <vector>

#include "iclockmodule.h"

#include "gamedef.h"
#include "protocal/msgshop.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;
class Money;

class Shop : public ConfigBase
{
public:
	struct ShopItem
	{
		ShopItem() : item_id(0), price_gold(0), price_bind_gold(0), is_notice(0) {}

		UInt16	item_id;
		int	price_gold;
		int price_bind_gold;
		int is_notice;
	};

	Shop();
	~Shop();

	bool Init(const std::string &configname, std::string *err);

	bool GetShopItemGoldPrice(UInt16 item_id, int *gold_price, int *bind_gold_price);

	void OnBuyItemReq(Role *role, Protocol::CSShopBuy *bs);
	void BuyShopNormalItemReq(Role *role, UInt16 item_id, int num, bool is_bind, bool if_use);

private:	
	typedef std::map<UInt16, ShopItem> ShopItemMap;
	typedef std::map<UInt16, ShopItem>::iterator ShopItemMapIt;

	bool CanBuy(Role *role, int total_price, bool is_bind);

	ShopItemMap	m_shop_item_map;					// 普通物品列表
};

#endif // __SHOP_HPP__

