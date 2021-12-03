#ifndef __DISCOUNT_BUY_CONFIG_HPP__
#define __DISCOUNT_BUY_CONFIG_HPP__

#include "servercommon/struct/discountbuyparam.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct DiscountBuyPhaseConfig
{
	struct ConfigItem
	{
		ConfigItem() : phase(0), active_level(0), last_time_s(0), begin_open_day(0),end_open_day(0){}

		int phase; 
		int active_level;
		long long last_time_s;
		int begin_open_day;
		int end_open_day;
		ItemConfigData notice_mail_item;
	};

	DiscountBuyPhaseConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[DISCOUNT_BUY_PHASE_MAX_COUNT];
};

struct DiscountBuyItemConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 5000;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT >= DISCOUNT_BUY_PHASE_MAX_COUNT * DISCOUNT_BUY_ITEM_PER_PHASE);

	struct ConfigItem
	{
		ConfigItem() : seq(0), phase(0), item_seq(0), price(0), buy_limit_count(0) {}

		int seq;
		int phase;
		int item_seq;
		int price;
		int buy_limit_count;
		ItemConfigData item;
	};

	DiscountBuyItemConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class DiscountBuyConfig : public ConfigBase
{
public:
	DiscountBuyConfig();
	~DiscountBuyConfig();

	bool Init(std::string path, std::string *err);

	int GetPhaseCfgCount() { return m_pahse_cfg.cfg_count; }
	const DiscountBuyPhaseConfig::ConfigItem * GetPhaseCfg(int phase);
	const DiscountBuyItemConfig::ConfigItem * GetItemCfg(int seq);

private:
	int InitPhaseCfg(PugiXmlNode RootElement);
	int InitItemCfg(PugiXmlNode RootElement);

	DiscountBuyPhaseConfig m_pahse_cfg;
	DiscountBuyItemConfig m_item_cfg;
};

#endif // __DISCOUNT_BUY_CONFIG_HPP__

