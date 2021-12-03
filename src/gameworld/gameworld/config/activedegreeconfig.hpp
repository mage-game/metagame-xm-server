#ifndef __ACTIVE_DEGREE_CONFIG_HPP__
#define __ACTIVE_DEGREE_CONFIG_HPP__

#include "servercommon/activedegreedef.hpp"
#include "item/knapsack.h"
#include <vector>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct ActiveDegreeInfo
{
	ActiveDegreeInfo() : type(-1), max_times(0), add_degree(0), add_exp(0), exp_factor_type(0){}

	bool Invalid() { return -1 == type; }

	int type;
	int max_times;
	int add_degree;
	int add_exp;
	double exp_factor_type;
};

struct ActiveDegreeReward
{
	ActiveDegreeReward() : degree_limit(0), is_notice(0) {}

	int reward_index;
	int degree_limit;
	ItemConfigData item;
	short is_notice;
};

struct ActiveDegreeExpLevelRatio
{
	ActiveDegreeExpLevelRatio() : min_level(0), max_level(0), exp_ratio(0) {}

	int min_level;
	int max_level;
	double exp_ratio;
};

class ActiveDegreeConfig : public ConfigBase
{
public:
	ActiveDegreeConfig();
	~ActiveDegreeConfig();

	bool Init(const std::string &configname, std::string *err);

	const ActiveDegreeInfo *GetActiveDegreeInfo(int type);
	const ActiveDegreeReward *GetActiveDegreeReward(int index);
	const ActiveDegreeExpLevelRatio *getActiveDegreeExpLevelRatio(int index);
	int GetMaxOnLineTime() { return m_max_online_time; }

	const ActiveDegreeReward *GetActiveDegreeRewardOnDay(int open_day, int index); // 按天数获取活跃度阈值奖励

private:
	int InitActiveConfig(PugiXmlNode RootElement);
	int InitRewardConfig(PugiXmlNode RootElement);
	int InitExpLevelConfig(PugiXmlNode RootElement);
	int InitRewardOnDayConfig(PugiXmlNode RootElement);
	int InitOtherConfig(PugiXmlNode RootElement);

	ActiveDegreeInfo m_active_degree_list[ACTIVEDEGREE_TYPE_NUM];
	ActiveDegreeReward m_reward_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];
	ActiveDegreeExpLevelRatio m_explevel_ratio_list[ACTIVEDEGREE_REWARD_LEVEL_MAX_NUM];
	ActiveDegreeReward m_reward_on_day_list[ACTIVEDEGREE_REWARD_MAX_DAY + 1][ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];  // 1-31 按天数

	int m_max_online_time;
};

#endif	// __ACTIVE_DEGREE_CONFIG_HPP__

