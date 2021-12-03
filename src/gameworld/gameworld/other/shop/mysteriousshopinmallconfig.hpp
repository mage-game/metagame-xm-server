#ifndef __MYSTRIOUS_SHOP_IN_MALL_CONFIG_HPP__
#define __MYSTRIOUS_SHOP_IN_MALL_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/mysteriousshopdef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>

class Role;

static const int MAX_REFRESH_COUNT = 5;

struct MysteriousShopInMallOtherCfg
{
	MysteriousShopInMallOtherCfg() : item_count(0), consume_diamond(0), all_consume_diamond(0), replacement_id(0), consume_replacement(0), all_consume_replacement(0), can_use_item_refresh(false)
	{
	}

	int item_count;
	int consume_diamond;
	int all_consume_diamond;
	int replacement_id;
	int consume_replacement;
	int all_consume_replacement;
	bool can_use_item_refresh;
};

struct MysteriousShopFlushTimeCfg
{
	int seq = 0;
	bool is_auto_refresh = false;
	int refresh_time = 0;
	int give_free_refesh_count = 0;
};

struct MysteriousShopItem
{
	MysteriousShopItem() : seq(0), price(0), buy_limit(0), refresh_weight(0), dicount(0), levelopen_min(0), levelopen_max(0), openday_min(0), openday_max(0), is_broadcast(false), can_broadcast(true)
	{}

	int seq;
	int price;
	int buy_limit;
	int refresh_weight;
	int dicount;
	int levelopen_min;
	int levelopen_max;
	int openday_min;
	int openday_max;

	bool is_broadcast;
	bool can_broadcast;

	ItemConfigData item;
};

struct MysteriousShopItemConfig
{
	MysteriousShopItemConfig()
	{}

	std::vector<MysteriousShopItem> mysterious_shop_item_vec;
};

struct DiscountShopItem
{
	DiscountShopItem() : seq(0), price(0), buy_limit(0), levelopen_min(0), levelopen_max(0), openday_min(0), openday_max(0)
	{}

	int seq;
	int price;
	int buy_limit;
	int levelopen_min;
	int levelopen_max;
	int openday_min;
	int openday_max;

	ItemConfigData item;
};

class MysteriousShopInMallConfig : public ConfigBase
{
public:
	MysteriousShopInMallConfig();
	~MysteriousShopInMallConfig();

	bool Init(const std::string &configname, std::string *err);
	const MysteriousShopInMallOtherCfg * GetMysteriousShopInMallOtherCfg(){ return &m_other_cfg; }
	const MysteriousShopFlushTimeCfg *GetFlushTimeCfg(time_t now);

	const MysteriousShopItem * GetMysteriousShopItemCfg(int seq);

	void GetRandSeqList(Role *role, int seq_list[], int count);
	bool CheckSeq(int seq);

	int GetDiscountShopSeqList(int seq_list[], int role_level);

	const DiscountShopItem * GetDiscountShopItemCfg(int seq);

private:
	int InitOtherConfig(PugiXmlNode RootElement);
	int InitMysteriousShopFlushConfig(PugiXmlNode RootElement);
	int InitMysteriousShopItemConfig(PugiXmlNode RootElement);
	int InitDiscountShopItemConfig(PugiXmlNode RootElement);


	MysteriousShopInMallOtherCfg m_other_cfg;

	std::vector<MysteriousShopFlushTimeCfg> m_myste_shop_flush_time_vec;

	MysteriousShopItemConfig m_mysterious_shop_item_cfg;

	int max_seq;

	std::vector<DiscountShopItem> m_discount_shop_item_vec;
};

#endif // __MYSTRIOUS_SHOP_IN_MALL_CONFIG_HPP__

