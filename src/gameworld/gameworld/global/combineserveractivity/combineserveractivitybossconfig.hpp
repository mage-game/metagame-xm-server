#ifndef __COMBINE_SERVER_BOSS_CONFIG_HPP__
#define __COMBINE_SERVER_BOSS_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CombineServerActivityBossOtherCfg
{
	CombineServerActivityBossOtherCfg() : scene_id(0), enter_pos(0, 0), common_boss_begin_time(0), common_boss_end_time(0), 
		common_boss_refresh_interval(0), great_boss_begin_time(0), great_boss_end_time(0), common_boss_refresh_time(0)
	{}

	int scene_id;
	Posi enter_pos;
	unsigned int common_boss_begin_time;
	unsigned int common_boss_end_time;
	unsigned int common_boss_refresh_interval;
	unsigned int great_boss_begin_time;
	unsigned int great_boss_end_time;
	unsigned int common_boss_refresh_time;
};

struct CombineServerActivityBossCfg
{
	CombineServerActivityBossCfg() : boss_type(-1), boss_id(0), pos(0, 0)
	{}

	int boss_type;
	int boss_id;
	Posi pos;
};

struct CombineServerActivityGatherCfg
{
	CombineServerActivityGatherCfg() : gather_type(-1), gather_id(0), gather_time_ms(0), count(0)
	{}

	int gather_type;
	int gather_id;
	int gather_time_ms;
	int count;

	ItemConfigData item;
};

struct CombineServerActivityGatherRewardCfg
{
	CombineServerActivityGatherRewardCfg() : gather_id(0), weight(0)
	{}

	int gather_id;
	int weight;

	ItemConfigData item;
};

struct CombineServerActivityGatherRewardCfgVec
{
	CombineServerActivityGatherRewardCfgVec() : total_weight(0)
	{}

	int total_weight;

	std::vector<CombineServerActivityGatherRewardCfg> reward_cfg_vec;
};

struct CombineServerActivityBossRankReward
{
	CombineServerActivityBossRankReward() : day_index(0)
	{}

	int day_index;
	ItemConfigData personal_rank_reward[COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM];			//个人奖励
				
	ItemConfigData master_reward;																//公会会长奖励
	ItemConfigData member_reward;																//公会成员奖励
};

class CombineServerActivityBossConfig : public ConfigBase
{
public:
	CombineServerActivityBossConfig();
	~CombineServerActivityBossConfig();

	enum BOSS_TYPE
	{
		BOSS_TYPE_COMMON = 0,
		BOSS_TYPE_GREAT = 1,
	};

	enum GATHER_TYPE
	{
		GATHER_TYPE_GOLD = 0,
		GATHER_TYPE_SLIVER = 1,

		GATHER_TYPE_MAX,
	};

	enum RANK_TYPE
	{
		RANK_TYPE_PERSONAL = 0,
		RANK_TYPE_GUILD = 1,
	};

	bool Init(const std::string &configname, std::string *err);

	const CombineServerActivityBossOtherCfg & GetCSABossOtherCfg() { return m_other_cfg; }
	const std::map<int, CombineServerActivityBossCfg> & GetCSABossCfgMap() { return m_boss_cfg_map; }
	const CombineServerActivityGatherCfg * GetCSAGatherCfg(int gather_id);
	const CombineServerActivityGatherCfg * GetCSAGatherCfgByGatherType(int gather_type);
	const ItemConfigData * GetGatherRandRewardItem(int gather_id);
	const std::vector<Posi> & GetCSARandomPosiCfg();
	const CombineServerActivityBossRankReward * GetCSABossRankRewardCfg(int day_index);
	const std::vector<int> & GetCSACommonBossRefreshCfg() { return m_common_boss_refresh_time_vec; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBossCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitGatherRewardCfg(PugiXmlNode RootElement);
	int InitGatherPosCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);
	int InitCommonBossRefreshCfg(PugiXmlNode RootElement);
	
	CombineServerActivityBossOtherCfg m_other_cfg;
	std::map<int, CombineServerActivityBossCfg> m_boss_cfg_map;
	std::map<int, CombineServerActivityGatherCfg> m_gather_cfg_map;
	std::map<int, CombineServerActivityGatherRewardCfgVec> m_gather_reward_cfg_map;
	std::vector<Posi> m_gather_pos_vec;
	std::vector<CombineServerActivityBossRankReward> m_rank_reward_vec;

	std::vector<int> m_common_boss_refresh_time_vec;
};

#endif // __COMBINE_SERVER_BOSS_CONFIG_HPP__

