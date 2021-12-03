#ifndef __GOLDVIP_CONFIG_HPP__
#define __GOLDVIP_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/goldvipdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gameworld/gameworld/item/itembase.h"


// º§ªÓ
struct GoldVipActiveCfg
{
	GoldVipActiveCfg() : need_level(0), convert_rate(0), need_gold(0), return_gold(0), continue_days(0), active_convert_gold(0), kill_monster_exp_add_per(0), gold_vip_title_id(0) {}

	int need_level;
	int convert_rate;
	int need_gold;
	int return_gold;
	int continue_days;
	int active_convert_gold;
	int kill_monster_exp_add_per;
	int gold_vip_title_id;
	int count_down;
};

// …ÃµÍ
struct GoldVipShopCfg
{
	struct ConfigItem
	{

		ConfigItem() : seq(0), limit_times(0), consume_type(0), consume_val(0) 
		{

		}

		int seq;
		int limit_times;
		int consume_type;
		int consume_val;

		ItemConfigData reward_item;
	};

	GoldVipShopCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[GOLD_VIP_CONVERT_SHOP_MAX_COUNT];
};

struct GoldVipMultiCostList
{
	GoldVipMultiCostList() : multiple_cost_id(0), multiple_count(0){}

	struct CostItem
	{
		CostItem() : multiple_cost_id(0), times_min(0), times_max(0), price_multile(0) {}

		int multiple_cost_id;
		int times_min;
		int times_max;
		int price_multile;
	};

	int multiple_cost_id;
	int multiple_count;
	CostItem item_list[GOLD_VIP_CONVERT_SHOP_MUL_MAX_COUNT];
};

class GoldVipConfig : public ConfigBase
{
public:

	GoldVipConfig();
	~GoldVipConfig();

	bool Init(const std::string &path, std::string *err);

	const GoldVipActiveCfg * GetGoldVipActiveCfg();
	const int GetGoldVipExpAdd(){return m_active_cfg.kill_monster_exp_add_per;}
	const int GetGoldVipTitleID(){return m_active_cfg.gold_vip_title_id;}
	const GoldVipShopCfg::ConfigItem * GetGoldVipShopRewardCfg(int seq);
	int GetMultipleCost(int shop_seq, int times);

private:
	int InitGoldVipActiveCfg(PugiXmlNode RootElement);
	int InitGoldVipShopCfg(PugiXmlNode RootElement);
	int InitGoldVipShopMulCfg(PugiXmlNode RootElement);

	GoldVipActiveCfg m_active_cfg;
	GoldVipShopCfg m_shop_cfg;

	std::map<int, GoldVipMultiCostList> m_multiplecost_map;

};

#endif // __GOLDVIP_CONFIG_HPP__

