#ifndef __MYSTERIOUS_SHOP_HPP__
#define __MYSTERIOUS_SHOP_HPP__

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

class Mysteriousshop : public ConfigBase
{
public:
	
	static const int MAX_SHOP_ITEM_NUM = 70;
	static const int MAX_SALE_ITEM_NUM = 9;		// 展示出售物品给玩家购买

	enum PRICETYPE
	{
		PRICETYPE_GOLD = 0,
		PRICETYPE_BIND_GOLD,
		PRICETYPE_COIN,

		PRICETYPE_MAX,
	};

	struct SaleItem
	{
		SaleItem() : seq(0), buynum(0) {}

		int seq;
		int buynum;
	};

	struct ShopItem
	{
		ShopItem() : seq(0), item_id(0), isbind(0), price_type(0), price(0), count_limit(0), weight(0) {}

		int seq;
		UInt16	item_id;
		int isbind;
		int price_type;
		int price;
		int count_limit;
		int weight;
	};

	struct RefreshPosItem
	{
		RefreshPosItem() : sceneid(0), posx(0), posy(0) {}

		int sceneid;
		int posx;
		int posy;
	};

	struct OtherCfg
	{
		OtherCfg() : refreshtime(0), existtime(0) {}

		int refreshtime;
		int existtime;
	};

	Mysteriousshop();
	~Mysteriousshop();

	bool Init(const std::string &configname, std::string *err);

	void Update(unsigned long interval, time_t now_second);
	void BuyShopNormalItemReq(Role *role, int shopitem_seq, int num);

	void SyncRefreshNpcPosToWrold();
	void SyncRefreshNpcPosToRole(Role *role);

	void SyncShopItemInfo();

	void GmNextStatus();

private:
	int InitShopItemConfig(PugiXmlNode RootElement);
	int InitRefreshPosConfig(PugiXmlNode RootElement);
	int InitOtherConfig(PugiXmlNode RootElement);
	void CalcSaleItemSeqList();

	ShopItem m_shop_item_cfglist[MAX_SHOP_ITEM_NUM];
	typedef std::vector<RefreshPosItem> RefreshPosVec;
	RefreshPosVec m_refrespos_vec;
	OtherCfg m_other_cfg;
	int m_max_shopitem_seq;
	int m_saleitem_total_weight;

	SaleItem m_sale_item_list[MAX_SALE_ITEM_NUM];
	int m_next_refreshtime;
	int m_dissappeartime;
	int m_npc_sceneid;
	Posi m_npc_pos;
};

#endif

