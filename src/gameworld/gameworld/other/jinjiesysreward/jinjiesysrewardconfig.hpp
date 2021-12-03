#ifndef __JINJIE_SYS_REWARD_CONFIG_HPP__
#define __JINJIE_SYS_REWARD_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "servercommon/struct/jinjiesysrewardparam.hpp"
#include <vector>

class TiXmlElement;

struct JinjieRewardItemCfg
{
	int reward_type = -1;
	int system_type = -1;
	int openserver_day = 0;
	int open_level = 0;
	unsigned int duration_time_s = 0;
	int grade = -1;
	ItemConfigData reward_item;
	int cost = 0;
	int param_0 = -1;
};

struct JinjieRewardAttrCfg
{
	int add_per = 0;
};

class JinjiesysRewardConfig : public ConfigBase
{
public:
	JinjiesysRewardConfig() = default;
	~JinjiesysRewardConfig() = default;

	enum REWARD_TYPE
	{
		REWARD_TYPE_SPECIAL_IMG = 0,
		REWARD_TYPE_TITLE = 1,

		REWARD_TYPE_MAX,
	};

	bool Init(std::string path, std::string *err);

	inline bool CanUseCfg(int reward_type, int system_type);

	std::vector<JinjieRewardItemCfg *> GetActiveItemCfgVec(int grade, int system_type);
	const JinjieRewardItemCfg * GetItemCfg(int reward_type, int system_type);
	const JinjieRewardAttrCfg * GetAttrCfg(int system_type);

private:
	int InitItemCfg(TiXmlElement *RootElement);
	int InitAttrCfg(TiXmlElement *RootElement);

	JinjieRewardItemCfg m_reward_item_cfg_list[REWARD_TYPE_MAX][JINJIE_TYPE_MAX];
	JinjieRewardAttrCfg m_attr_cfg_list[JINJIE_TYPE_MAX];
};

bool JinjiesysRewardConfig::CanUseCfg(int reward_type, int system_type)
{
	if (reward_type < 0 || reward_type >= REWARD_TYPE_MAX || reward_type >= JINJIE_REWARD_TYPE_MAX_COUNT)
	{
		return false;
	}

	if (system_type < 0 || system_type >= JINJIE_TYPE_MAX || system_type >= JINJIE_TYPE_MAX_COUNT)
	{
		return false;
	}

	return m_reward_item_cfg_list[reward_type][system_type].system_type >= 0;
}

#endif // !__JINJIE_SYS_REWARD_CONFIG_HPP__
