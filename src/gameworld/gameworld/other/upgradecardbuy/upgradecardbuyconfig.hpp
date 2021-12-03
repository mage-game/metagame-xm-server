#ifndef __UPGRADE_CARD_BUY_CONFIG_HPP__
#define __UPGRADE_CARD_BUY_CONFIG_HPP__

#include "servercommon/struct/upgradecardparam.hpp"
#include "servercommon/configbase.h"
#include <map>

#define UPGRADE_CARD_BUY_CFG() LOGIC_CONFIG->GetUpgradeCardBuyConfig()

struct UpgradeCardInfo
{
	int opengame_day = 0;
	short grade = 0;
	ItemID item_id = 0;
	int need_gold = 0;
	int act_theme = 0;
};

struct UpgradeCardBuyOther
{
	int opengame_day_limit = 0;
	int opengame_day_limit_2 = 0;
};

class UpgradeCardBuyConfig : public ConfigBase
{
public:
	UpgradeCardBuyConfig();
	~UpgradeCardBuyConfig();

	bool Init(const std::string &configname, std::string *err);

	UpgradeCardInfo * GetUpgradeCardInfo(int activity_id, int opengame_day, short grade);
	UpgradeCardInfo * GetUpgradeCardInfo(int activity_id, int opengame_day, short grade, int theme);
	//int GetOpengameDayLimit() { return m_other_cfg.opengame_day_limit; }
	const UpgradeCardBuyOther & GetOpengameDayLimit() { return m_other_cfg; }

private:
	int InitUpgradeCardBuyConfig(TiXmlElement *root_element);
	int InitOther(TiXmlElement *root_element);

private:

	std::map<int, std::vector<UpgradeCardInfo>> m_upgrade_card_buy_info_map;
	UpgradeCardBuyOther                         m_other_cfg;

};

#endif // __UPGRADE_CARD_BUY_CONFIG_HPP__