#ifndef MAZECONFIG_HPP
#define MAZECONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/mountparam.hpp"
#include "item/itembase.h"
#include "servercommon/mazedef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "obj/character/attribute.hpp"


struct MazeJifenRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 16);

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_jifen(0) {}

		int seq;
		int need_jifen;
		ItemConfigData reward_item;
	};

	MazeJifenRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class MazeConfig : public ConfigBase
{
public:
	MazeConfig();
	~MazeConfig();

	bool Init(const std::string &configname, std::string *err);

	bool RandReward(int &item_count, int &coin);
	int GetBuyCostGold(int buy_times);
	int GetRewardItemID() { return m_box_item_id; }
	int GetFindItemCount() { return m_find_item_count; }
	const MazeJifenRewardConfig::ConfigItem * GetJifenReward(int seq);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitBoxCfg(PugiXmlNode RootElement);
	int InitBuyCfg(PugiXmlNode RootElement);
	int InitJifenRewardCfg(PugiXmlNode RootElement);

	int m_box_item_id;
	int m_find_item_count;

	struct MazeBox
	{
		MazeBox() : weight(0), item_count(0), coin(0) {}
		int weight;
		int item_count;
		int coin;
	};
	static const int MAX_BOX_REWARD_COUNT = 32;
	int m_box_total_weight;
	int m_box_reward_count;
	MazeBox m_box_list[MAX_BOX_REWARD_COUNT];

	static const int MAX_BUY_TIMES = 19;
	UNSTD_STATIC_CHECK(MAX_BUY_TIMES + MazeParam::MAX_FREEMOVE_TIMES <= 127);

	int m_buy_list[MAX_BUY_TIMES + 1];

	MazeJifenRewardConfig m_jifen_reward_cfg;
};

#endif

