#ifndef __JILIAN_CONFIG_HPP__
#define __JILIAN_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include <string.h>
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct JilianOtherConfig
{
	JilianOtherConfig() : can_join_times(0), lianlu_scene_id(0), lianlu_pos(0, 0), lianlu_range(0), jilian_times(0),
		 common_jilian_cost(0), super_jilian_cost(0), qte_rate(0), qte_duration_s(0), cost_time_s(0), qte_succ_max_times(0)
	{

	}

	int can_join_times;
	int lianlu_scene_id;
	Posi lianlu_pos;
	int lianlu_range;
	int jilian_times;
	int common_jilian_cost;
	int super_jilian_cost;
	ItemConfigData reward_item;
	int qte_rate;
	int qte_duration_s;
	int cost_time_s;
	int qte_succ_max_times;
};

class JilianConfig : public ConfigBase
{
public:
	JilianConfig();
	~JilianConfig();

	bool Init(const std::string &path, std::string *err);

	JilianOtherConfig & GetOtherCfg() { return m_other_cfg; }
	int GetRewardExp(int role_level, int jilian_type);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);

	JilianOtherConfig m_other_cfg;

	struct RewardConfig
	{
		RewardConfig() : need_level(0), common_exp(0), super_exp(0) {}

		int need_level;
		int common_exp;
		int super_exp;
	};

	int m_reward_cfg_count;
	RewardConfig m_reward_cfg_list[MAX_ROLE_LEVEL];
};

#endif


