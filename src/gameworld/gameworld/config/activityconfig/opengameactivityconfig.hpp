#ifndef __OPEN_GAME_ACTIVITY_CONFIG_HPP__
#define __OPEN_GAME_ACTIVITY_CONFIG_HPP__

#include <string>
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itempool.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"
#include "equipment/equipment.h"
#include "servercommon/activitydef.hpp"
#include <vector>

struct OGAOtherConfig
{
	OGAOtherConfig() : baibeifanli_price(0), baibeifanli_level_limit(0), baibeifanli_end_time(0), baibeifanli2_price(0), baibeifanli2_level_limit(0), baibeifanli2_open_time(0), baibeifanli2_end_time(0){}

	int baibeifanli_price;
	ItemConfigData baibeifanli_item;
	int baibeifanli_level_limit;
	int baibeifanli_end_time;

	int baibeifanli2_price;
	ItemConfigData baibeifanli2_item;
	int baibeifanli2_level_limit;

	int baibeifanli2_open_time;
	int baibeifanli2_end_time;
};

struct OGAPutonEquipmentRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 32);

	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_jie(0), need_color(0), need_num(0), global_fetch_times(0), item_count(0) {}

		int seq;
		int need_jie;
		int need_color;
		int need_num;
		int global_fetch_times;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	OGAPutonEquipmentRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct OGACapabilityRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 32);

	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), capability(0), global_fetch_times(0), item_count(0) {}

		int seq;
		int capability;
		int global_fetch_times;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	OGACapabilityRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct OGARoleLevelRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 32);

	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), level(0), global_fetch_times(0), item_count(0) {}

		int seq;
		int level;
		int global_fetch_times;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	OGARoleLevelRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct OGAEquipmentGiftConfig
{
	const static int MAX_SEQ = 2;

	struct ConfigItem
	{
		ConfigItem() : seq(0), prof(0), price(0) {}

		int seq;
		int prof;
		int price;

		ItemConfigData item;
	};

	OGAEquipmentGiftConfig() {}
	
	typedef std::vector<OGAEquipmentGiftConfig::ConfigItem> EquipmentGiftVec;
	EquipmentGiftVec equipmentgift_vec;
};

struct OGAKillBossReward
{
	OGAKillBossReward() : boss_seq_1(0), boss_seq_2(0), boss_seq_3(0), boss_seq_4(0) {}

	int boss_seq_1;
	int boss_seq_2;
	int boss_seq_3;
	int boss_seq_4;
	ItemConfigData reward_item;
};

struct OGAKillBossConfig
{
	OGAKillBossConfig() 
	{
		memset(kill_boss_id_list, 0, sizeof(kill_boss_id_list));
	}

	int kill_boss_id_list[OGA_BOSS_MAX_SEQ];
	OGAKillBossReward kill_boss_reward_list[OGA_BOSS_REWARD_MAX_SEQ];
};

struct OGAGiftShopConfig
{
	OGAGiftShopConfig() : level(0), seq(0), price(0) {}

	int level;
	int seq;
	int price;
	ItemConfigData gift_item_list[OGA_GIFT_MAX_TYPE];
};

class OpenGameActivityConfig : public ConfigBase
{
public:
	OpenGameActivityConfig();
	~OpenGameActivityConfig();

	bool Init(const std::string &configname, std::string *err);

	const OGAOtherConfig & GetOtherConfig();
	const OGAPutonEquipmentRewardConfig::ConfigItem * GetPutonEquipmentReward(int seq);
	const OGACapabilityRewardConfig::ConfigItem * GetCapabiblityReward(int seq);
	const OGARoleLevelRewardConfig::ConfigItem * GetRolelevelReward(int seq);
	const OGAEquipmentGiftConfig::ConfigItem * GetEquipmentGiftItemCfg(int seq, int prof);
	const OGAGiftShopConfig * GetGiftShopConfig(int level, int seq);
	const OGAGiftShopConfig * GetGiftShop2Config(int level, int seq);

	int GetPutonEquipmentActFlag(ItemGridData equipment_list[Equipment::E_INDEX_MAX]);
	int GetKillBossSeq(int boss_id);
	const OGAKillBossReward * GetKillBossReward(int seq);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitPutonEquipmentRewardCfg(PugiXmlNode RootElement);
	int InitCapabilityRewardCfg(PugiXmlNode RootElement);
	int InitRoleLevelRewardCfg(PugiXmlNode RootElement);
	int InitEquipmentGiftRewardCfg(PugiXmlNode RootElement);
	int InitKillBossIdConfig(PugiXmlNode RootElement);
	int InitKillBossRewardConfig(PugiXmlNode RootElement);
	int InitGiftShopConfig(PugiXmlNode RootElement);
	int InitGiftShop2Config(PugiXmlNode RootElement);

	OGAOtherConfig m_other_cfg;
	OGAPutonEquipmentRewardConfig m_puton_equipment_reward_cfg;
	OGACapabilityRewardConfig m_capability_reward_cfg;
	OGARoleLevelRewardConfig m_rolelevel_reward_cfg;
	OGAEquipmentGiftConfig m_equipmentgift_cfg;
	OGAKillBossConfig m_kill_boss_cfg;
	std::vector<OGAGiftShopConfig> m_gift_shop_cfg_vec;
	std::vector<OGAGiftShopConfig> m_gift_shop_2_cfg_vec;	//Àñ°üÏÞ¹º2
};

#endif // __OPEN_GAME_ACTIVITY_CONFIG_HPP__

