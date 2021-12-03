#ifndef __CLOSEDBETA_CONFIG_HPP__
#define __CLOSEDBETA_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CloseBetaOtherCfg
{
	ItemConfigData guild_reward_item;
	ItemConfigData marry_reward_item;
};

struct CloseBetaOnlineTimeRewardCfg
{
	const static int ONLINE_REWARD_MAX_COUNT = 8;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_online_time_s(0), reward_gold(0) {}

		int seq;
		int need_online_time_s;
		int reward_gold;
	};

	CloseBetaOnlineTimeRewardCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[ONLINE_REWARD_MAX_COUNT];
};

struct CloseBetaLoginRewardCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : login_day(0), reward_gold(0) {}

		int login_day;
		int reward_gold;
	};

	CloseBetaLoginRewardCfg() : max_login_day(0) {}

	int max_login_day;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct CloseBetaUplevelRewardCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_level(0), reward_gold(0) {}

		int seq;
		int need_level;
		int reward_gold;
	};

	CloseBetaUplevelRewardCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct CloseBetaVipLevelCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : role_level(0), vip_level(0) {}

		int role_level;
		int vip_level;
	};

	CloseBetaVipLevelCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct CloseBetaEquipFBCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chapter(0), need_level(0), need_star(0), reward_gold_bind(0) {}

		int seq;
		int need_chapter;
		int need_level;
		int need_star;
		int reward_gold_bind;
	};

	CloseBetaEquipFBCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct CloseBetaTowerdefendFBCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_level(0), reward_gold(0) {}

		int seq;
		int need_level;
		int reward_gold;
	};

	CloseBetaTowerdefendFBCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct CloseBetaJoinActivityRewardCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : seq(0), activity_type(0), reward_gold(0) {}

		int seq;
		int activity_type;
		int reward_gold;
	};

	CloseBetaJoinActivityRewardCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class ClosedBetaConfig : public ConfigBase
{
public:
	ClosedBetaConfig();
	~ClosedBetaConfig();

	bool Init(const std::string &path, std::string *err);
	CloseBetaOtherCfg & GetOtherCfg() { return m_other_cfg; }

	const CloseBetaLoginRewardCfg::ConfigItem * GetLoginRewardCfg(int login_day);
	const CloseBetaUplevelRewardCfg::ConfigItem * GetUplevelRewardCfg(int seq);
	const CloseBetaVipLevelCfg::ConfigItem * GetVipLevelCfg(int role_level);

	const CloseBetaJoinActivityRewardCfg::ConfigItem * GetJoinActivityCfgBySeq(int seq);
	const CloseBetaJoinActivityRewardCfg::ConfigItem * GetJoinActivityCfgByType(int activity_type);

	const CloseBetaOnlineTimeRewardCfg::ConfigItem * GetOnlineTimeCfgBySeq(int seq);

	const CloseBetaEquipFBCfg::ConfigItem * GetEquipFBCfg(int seq);
	const CloseBetaEquipFBCfg::ConfigItem * GetEquipFBCfg(int chapter, int level);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLoginRewardCfg(PugiXmlNode RootElement);
	int InitUplevelRewardCfg(PugiXmlNode RootElement);
	int InitViplevelCfg(PugiXmlNode RootElement);
	int InitEquipFBCfg(PugiXmlNode RootElement);
	int InitJoinActivityRewardCfg(PugiXmlNode RootElement);
	int InitOnlineTimeRewardCfg(PugiXmlNode RootElement);

	CloseBetaOtherCfg m_other_cfg;
	CloseBetaLoginRewardCfg m_login_reward_cfg;
	CloseBetaUplevelRewardCfg m_uplevel_reward_cfg;
	CloseBetaVipLevelCfg m_vip_level_cfg;
	CloseBetaEquipFBCfg m_equipfb_cfg;
	CloseBetaTowerdefendFBCfg m_tdfb_cfg;
	CloseBetaJoinActivityRewardCfg m_join_activity_reward_cfg;
	CloseBetaOnlineTimeRewardCfg m_online_time_reward_cfg;
};

#endif // __CLOSEDBETA_CONFIG_HPP__

