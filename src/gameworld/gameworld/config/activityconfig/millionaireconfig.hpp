#ifndef _MILLIONAIRECONFIG_HPP_
#define _MILLIONAIRECONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "gamedef.h"
#include "servercommon/configbase.h"

#include <vector>

struct PosCfg
{
	PosCfg() : scene_id(0) {}

	int scene_id;
	Posi born_pos;
};

struct OtherCfg
{
	OtherCfg()
	{
		this->Reset();
	}
	void Reset()
	{
		refresh_interval_s = 0;
		role_can_gather_max_times = 0;
		level_limit = 0;
		scene_id = 0;
		enter_pos.x = 0;
		enter_pos.y = 0;

		exp_boss_id = 0;
		boss_born_pos.x = 0;
		boss_born_pos.y = 0;
		boss_first_refresh_time = 0;
		boss_refresh_interval = 0;
		exp_gather_id = 0;
		exp_gather_exit_time = 0;
		exp_gather_gather_time = 0;
		exp_reward = 0;

		frozen_can_use_times = 0;
		frozen_cloddown_time = 0;
		frozen_continue_time = 0;
		frozen_exp_reward = 0;
		frozen_distance = 0;
	}

	int refresh_interval_s;
	int role_can_gather_max_times;
	int level_limit;
	int scene_id;
	Posi enter_pos;

	int exp_boss_id;
	Posi boss_born_pos;
	int boss_first_refresh_time;
	int boss_refresh_interval;
	int exp_gather_id;
	int exp_gather_exit_time;
	int exp_gather_gather_time;
	long long exp_reward;

	int frozen_can_use_times;
	int frozen_cloddown_time;
	int frozen_continue_time;
	long long frozen_exp_reward;
	int frozen_distance;
	ItemConfigData frozen_reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct GatherCfg
{
	GatherCfg():gather_id(0), gather_time(0), gather_exist_time(0), gather_total_count(0), is_notice(0){}
	
	int gather_id;
	int gather_time;
	int gather_exist_time;
	int gather_total_count;
	int is_notice;
};

struct ExtraRewardCfg
{
	ExtraRewardCfg():extra_reward_index(0){}

	int extra_reward_index;
	ItemConfigData reward_item1;
	ItemConfigData reward_item2;
	ItemConfigData reward_item3;
};

struct RewardPoolCfg
{
	RewardPoolCfg():gather_id(0), reward_index(0),weight(0){}

	int gather_id;
	int reward_index;
	int weight;
	ItemConfigData reward_item;
};

class MillionaireConfig : public ConfigBase
{
public:
	MillionaireConfig();
	~MillionaireConfig();

	bool Init(const std::string &configname, std::string *err);

	void RefreshAll();
	void RefreshExpGather();
	int GetRefreshIntervalS() { return m_other_cfg.refresh_interval_s; }
	const OtherCfg* GetOhterCfg(){return &m_other_cfg;}
	const ExtraRewardCfg* GetExtraReward(int extra_index);
	const RewardPoolCfg* GetRewardPool(int gather_id, int index);
	const RewardPoolCfg* GetRewardPoolReward(int gather_id);
	const std::map<int, GatherCfg> & GetGatherCfgMap(){return m_gather_map;}
	const GatherCfg * GetGatherCfg(int gather_id);

	bool IsCanGather(Role *role);
	
	void CalcGatherReward(Role *role, int gather_id);
	int GetMillionaireEnterInfo(Posi *pos);

private:
	int InitPosCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitExtraReward(PugiXmlNode RootElement);
	int InitRewardPool(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitExpGatherPosCfg(PugiXmlNode RootElement);

	OtherCfg m_other_cfg;

	int m_total_box_count;
	std::map<int, GatherCfg> m_gather_map;
	std::vector<PosCfg> m_pos_vec;
	std::map<int,ExtraRewardCfg> m_extra_reward;
	std::map<int, std::map<int,RewardPoolCfg> > m_reward_pool_map;
	std::vector<PosCfg> m_exp_gather_pos;
};

#endif

