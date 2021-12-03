#ifndef __CHEST_SHOP_HPP__
#define __CHEST_SHOP_HPP__

#include <map>
#include <vector>
#include <string>

#include "servercommon/roleshopdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

enum CHEST_SHOP_MODE
{
	CHEST_SHOP_MODE_INVALID = 0,
	CHEST_SHOP_MODE_1,					// 极品寻宝1次				
	CHEST_SHOP_MODE_10,					// 极品寻宝10次				
	CHEST_SHOP_MODE_30,					// 极品寻宝30次
	CHEST_SHOP_MODE1_1 = 4,				// 巅峰寻宝1次
	CHEST_SHOP_MODE1_10,				// 巅峰寻宝10次
	CHEST_SHOP_MODE1_30,				// 巅峰寻宝30次
	CHEST_SHOP_MODE2_1 = 7,				// 至尊寻宝1次
	CHEST_SHOP_MODE2_10,				// 至尊寻宝10次
	CHEST_SHOP_MODE2_30,				// 至尊寻宝30次
	CHEST_SHOP_JL_MODE_1 = 10,									
	CHEST_SHOP_JL_MODE_10,									
	CHEST_SHOP_JL_MODE_50,
	CHEST_SHOP_MODE3_1 = 13,			// 寻宝1次
	CHEST_SHOP_MODE3_10,				// 寻宝10次
	CHEST_SHOP_MODE3_30,				// 寻宝30次
	CHEST_SHOP_MODE_MAX,
};


enum CHEST_SHOP_DAY_CFG_INDEX
{
	CHEST_SHOP_DAY_0 = 0,
	CHEST_SHOP_DAY_8,
	CHEST_SHOP_DAY_15,
	CHEST_SHOP_DAY_30,
	CHEST_SHOP_DAY_MAX
};

struct ChestShopItem
{
	ChestShopItem() : item_id(0), is_broadcast(false), can_repeat(false), lucky_rate(0), is_record(false), is_bind(false)
	{
		memset(rate_list, 0, sizeof(rate_list));
		memset(useitem_rate_list, 0, sizeof(useitem_rate_list));
	}

	ItemID item_id;
	int rate_list[CHEST_SHOP_MODE_MAX];
	int useitem_rate_list[CHEST_SHOP_MODE_MAX];
	bool is_broadcast;
	bool can_repeat;
	int lucky_rate;
	bool is_record;
	bool is_bind;
};

struct ChestShopItemMap
{
	ChestShopItemMap() {}

	std::map<UInt16, ChestShopItem> item_map;
};

// 珍稀物品配置
struct RareChestShopCfg
{
	RareChestShopCfg() : seq(0), weight(0), is_broadcast(0), min_buy_count(0), lucky_rate(0)
	{

	}
	
	int seq;
	ItemConfigData item;
	int weight;
	int is_broadcast;
	int min_buy_count;
	int lucky_rate;
};


static const int MODE_1_COUNT = 1;						
static const int MODE_10_COUNT = 10;					
static const int MODE_30_COUNT = 30;
static const int MODE_50_COUNT = 50;
static const int MODE_MAX_COUNT = MODE_50_COUNT;

class ChestShop : public ConfigBase
{
public:
	ChestShop();
	~ChestShop();

	bool Init(const std::string &configname, std::string *err);

	void Buy(Role *role, int shop_type, int mode);	// 寻宝
	void RareBuy(Role *role, int times);			// 奇珍异宝抽奖

	ItemID GetChestShopUseItem(int shop_type, int mode);		// 获取寻宝钥匙

	void GMBuyTest(Role *role, int chest_shop_type, int mode, std::map<unsigned short, int> &itemmap);

	int GetFreeCD1(int mode);
	int GetAddScore(int short_type);

	int GetFreeCD30() { return m_free_cd_s_30; }
	int GetFreeTimePerDay(){return m_free_1_times_per_day;}
	int GetFreeTimeJingLingPerDay(){return m_free_1_times_jingling_per_day;}
	ItemID GetFirstBuyHitEquipId(int prof, int shop_type);
	ItemID GetFirstBuyHitJinglingId();
	const RareChestShopCfg * GetRareChestShopReward(const int need_buy_times, bool *is_get_rare);

private:
	int InitChestShopItemList(PugiXmlNode ItemListElement, int day_index);
	int InitOther(PugiXmlNode RootElement);
	int InitRareChestShop(PugiXmlNode RootElement);

	static const int CHEST_SHOP_MAX_ITEM_NUM = 300;		// 物品最大个数

	//typedef std::map<UInt16, ChestShopItem> ChestShopItemMap;
	typedef std::map<UInt16, ChestShopItem>::iterator ChestShopItemMapIt;

	int GetModeCount(int mode);
	ChestShopItemMap *GetChestShopItemMap(Role *role);

	ChestShopItemMap m_chest_item_map[CHEST_SHOP_DAY_MAX];

	int m_replace_item_id[CHEST_SHOP_TYPE_MAX];
	int m_rare_item_id;
	int m_gold_list[CHEST_SHOP_MODE_MAX];

	// CD
	int m_free_cd_s_1;
	int m_free_cd1_s_1;
	int m_free_cd2_s_1;

	int m_free_cd_s_30;
	int m_free_1_times_per_day;

	// 积分
	int m_add_score;
	int m_add_score1;
	int m_add_score2;
	int m_add_score3;

	int m_free_jingling_cd_s_1;
	int m_free_1_times_jingling_per_day;
	int m_add_jingling_score;

	// 极品第一次
	ItemID m_first_buy_hit_equipid_1;
	ItemID m_first_buy_hit_equipid_2;
	ItemID m_first_buy_hit_equipid_3;
	ItemID m_first_buy_hit_equipid_4;

	// 巅峰第一次
	ItemID m_first_buy_2_hit_equipid;

	// 至尊第一次
	ItemID m_first_buy_3_hit_equipid;

	ItemID m_first_buy_4_hit_equipid;

	ItemID m_first_buy_hit_jinglingid_1;
	ItemID m_first_buy_hit_jinglingid_2;
	ItemID m_first_buy_hit_jinglingid_3;
	ItemID m_first_buy_hit_jinglingid_4;

	ItemID m_equip_use_itemid;
	ItemID m_equip_10_use_itemid;
	ItemID m_equip_30_use_itemid;

	ItemID m_equip1_use_itemid;
	ItemID m_equip1_10_use_itemid;
	ItemID m_equip1_30_use_itemid;

	ItemID m_equip2_use_itemid;
	ItemID m_equip2_10_use_itemid;
	ItemID m_equip2_30_use_itemid;

	ItemID m_equip3_use_itemid;
	ItemID m_equip3_10_use_itemid;
	ItemID m_equip3_30_use_itemid;

	ItemID m_jingling_use_itemid;
	ItemID m_jingling_10_use_itemid;
	ItemID m_jingling_50_use_itemid;


	int m_one_rare_shop_consume_gold;
	int m_ten__rare_shop_consume_gold;

	int m_rare_shop_total_weight;
	std::vector<RareChestShopCfg> m_rare_chest_reward_vec;
};

#endif // __CHEST_SHOP_HPP__

