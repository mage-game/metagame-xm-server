#ifndef __CROSS_RANK_REWARD_CONFIG__
#define __CROSS_RANK_REWARD_CONFIG__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include <vector>
#include <map>

struct CrossRankReward
{
	CrossRankReward() : rank_type(0), rank(0), img_item_id(0), part_type(0), img_id(0), limit_value(0) {}

	int rank_type;
	int rank;
	ItemID img_item_id;

	int part_type;
	int img_id;
	int limit_value;

	ItemConfigData reward_item;
};

struct CrossRankRewardDateCfg
{
	int rank_type = -1;
	int reward_day_flag = 0;

	bool IsValid() const
	{
		return rank_type >= 0;
	}

	void SetFlag(int day_index)
	{
		reward_day_flag |= 1 << day_index;
	}

	bool CanReward(int day_index) const
	{
		return 0 != (reward_day_flag & (1 << day_index));
	}
};

// 跨服公会击杀榜
struct CrossGuildKillBossRankReward
{
	CrossGuildKillBossRankReward() {}

	int rank_type = 0; 
	int rank_pos = 0;
	int tuanzhang_title_id = 0;
	int member_title_id = 0;
	int limit_value = 0; 
};

//跨服榜限制配置
struct CrossRankLimitCfg
{
	CrossRankLimitCfg() {}

	int rank_type = 0;
	int limit_open_day = 0;
};

class CrossRankRewardConfig
{
public:
	static CrossRankRewardConfig & Instance()
	{
		static CrossRankRewardConfig _instance;
		return _instance;
	}

	bool Init(const std::string &path, std::string *err);

	const CrossRankReward * GetReward(int rank_type, int rank);
	const CrossRankRewardDateCfg * GetDateCfg(int rank_type);

	bool GetRewardImageTypeById(ItemID img_item_id, int &part_type, int &img_id);

	const CrossGuildKillBossRankReward * GetCrossGuildRankReward(int rank_type, int rank);

	int GetCrossRankLimitOpenDay(int rank_type);

private:
	CrossRankRewardConfig();
	~CrossRankRewardConfig();

	int InitRewardCfg(TiXmlElement *RootElement);
	int InitDateCfg(TiXmlElement *RootElement);
	int InitCrossGuildKillRankCfg(TiXmlElement *RootElement);
	int InitCrossRankLimitCfg(TiXmlElement *RootElement);

	std::map<int, std::vector<CrossRankReward> > m_reward_map;
	std::map<int, CrossRankRewardDateCfg> m_date_cfg_map;

	std::map<int, std::vector<CrossGuildKillBossRankReward> > m_cross_guild_kill_rank_map;

	std::map<int, CrossRankLimitCfg> m_cross_rank_limit_map;
};

#endif // !__CROSS_RANK_REWARD_CONFIG__

