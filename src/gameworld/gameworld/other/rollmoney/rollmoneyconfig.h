#ifndef __ROLL_MONEY_CONFIG_HPP__
#define __ROLL_MONEY_CONFIG_HPP__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "other/rollmoney/rollmoney.h"
#include <string>

static const int ROLL_MONEY_NUM_MAX_COUNT = 10;
UNSTD_STATIC_CHECK(10 == ROLL_MONEY_NUM_MAX_COUNT);

struct RollMoneyRollCoinCfg
{
	struct ConfigItem
	{
		ConfigItem ()
		{
			memset(num_weight, 0, sizeof(num_weight));
		}

		int num_weight[ROLL_MONEY_NUM_MAX_COUNT];
	};

	RollMoneyRollCoinCfg() {}

	ConfigItem cfg_item_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT];
};

struct RollMoneyRollBindGoldCfg
{
	struct ConfigItem
	{
		ConfigItem ()
		{
			memset(num_weight, 0, sizeof(num_weight));
		}

		int num_weight[ROLL_MONEY_NUM_MAX_COUNT];
	};

	RollMoneyRollBindGoldCfg() {}

	ConfigItem cfg_item_list[ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT];
};

struct RollMoneyOtherCfg
{
	RollMoneyOtherCfg() : active_degree_need(0) {}

	int active_degree_need;
};

class RollMoneyConfig : public ConfigBase
{
public:
	RollMoneyConfig();
	~RollMoneyConfig();

	bool Init(const std::string &path, std::string *err);

	const RollMoneyRollCoinCfg & GetRollCoinCfg() { return m_roll_coin_cfg; }
	const RollMoneyRollBindGoldCfg & GetRollGoldCfg() { return m_roll_bind_gold_cfg; }
	const RollMoneyOtherCfg & GetOtherCfg() { return m_other_cfg; }

private:
	int InitRollCoinCfg(PugiXmlNode RootElement);
	int InitRollGoldCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);

	RollMoneyRollCoinCfg m_roll_coin_cfg;
	RollMoneyRollBindGoldCfg m_roll_bind_gold_cfg;
	RollMoneyOtherCfg m_other_cfg;
};

#endif

