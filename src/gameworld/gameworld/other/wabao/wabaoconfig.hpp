#ifndef __WABAO_CONFIG_HPP__
#define __WABAO_CONFIG_HPP__

#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/campdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct WabaoBaozangRewardItemConfig
{
	WabaoBaozangRewardItemConfig(): index(0), reward_type(0), weight(0) {}
	
	int index;
	int reward_type;
	int weight;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct WabaoBaotuConfig							// ±¶Õº≈‰÷√
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;
	struct ConfigItem
	{
		ConfigItem() : total_degree(0), baotu_count(0) {}

		int total_degree;
		int baotu_count;
	};

	WabaoBaotuConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct WabaoBaozangRewardConfig					// ±¶œ‰Ω±¿¯¿‡–Õ≈‰÷√
{
	const static int MAX_REWARD_LIMIT = 400;

	enum BAOZANG_TYPE
	{
		BAOZANG_TYPE_INVALID,
		BAOZANG_TYPE_GOLD,
		BAOZANG_TYPE_VIOLET_GOLD,
		BAOZANG_TYPE_SILVERY,
		BAOZANG_TYPE_OTHER,

		BAOZANG_TYPE_MAX
	};

	struct ConfigItem
	{
		ConfigItem() : reward_type(0), reward_limit(0), total_weight(0) {}

		int reward_type;
		int reward_limit;
		int total_weight;
		std::vector<WabaoBaozangRewardItemConfig> cfg_vec;
	};
	WabaoBaozangRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[BAOZANG_TYPE_MAX];
};

struct WabaoRefreshPosConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : scene_id(0), pos(0, 0) {}

		int scene_id;
		Posi pos;
	};

	WabaoRefreshPosConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct WabaoOtherConfig
{
	WabaoOtherConfig() : quick_complete_cost(0), succ_prob(0), max_reward_limit(0), max_baotu_limit(0), shouhuzhe_time(0), open_level(0), init_baotu_times(0) {}

	int quick_complete_cost;
	int succ_prob;
	int max_reward_limit;
	int max_baotu_limit;
	unsigned int shouhuzhe_time;
	int open_level;
	int init_baotu_times;
};


class WabaoConfig : public ConfigBase
{
public:
	WabaoConfig();
	~WabaoConfig();

	bool Init(std::string path, std::string *err);

	bool GetRefreshPos(int *scene_id, int *pos_x, int *pos_y);
	const WabaoOtherConfig &GetOtherCfg() { return m_other_cfg; }
	const WabaoBaotuConfig &GetBaotuCfg() { return m_baotu_cfg; }
	const WabaoBaozangRewardConfig &GetBaozangRewardCfg() { return m_baozang_reward_cfg; };

private:

	int InitRefreshPosCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBaozangRewardCfg(PugiXmlNode RootElement);
	int InitBaotuCfg(PugiXmlNode RootElement);
	int InitBaozangRewardPoolCfg(PugiXmlNode RootElement);

	WabaoRefreshPosConfig m_refresh_pos_cfg;
	WabaoOtherConfig m_other_cfg;
	WabaoBaozangRewardConfig m_baozang_reward_cfg;
	WabaoBaotuConfig m_baotu_cfg;
};

#endif // __WABAOCONFIG_HPP__

