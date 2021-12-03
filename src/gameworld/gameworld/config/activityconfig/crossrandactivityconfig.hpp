#ifndef __CROSS_RAND_ACTIVITY_CONFIG_HPP__
#define __CROSS_RAND_ACTIVITY_CONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "item/itembase.h"
#include "servercommon/serverdef.h"
#include "servercommon/configbase.h"

static const int CROSS_RA_CHONGZHI_RANK_MAX_RANK = 10;
static const int CROSS_RA_CONSUME_RANK_MAX_RANK = 10;

struct CrossRandActivityOtherCfg
{
	int chongzhi_rank_join_reward_need;

	int consume_rank_join_reward_need;

};

struct CrossRandActivityChongzhiRankConfig
{
	CrossRandActivityChongzhiRankConfig() : rank(0), need_total_chongzhi(0) {}

	int rank;
	long long need_total_chongzhi;											// 排名奖励需要全服总充值额度
	ItemConfigData rank_join_reward_item;									// 排名奖励
	ItemConfigData personal_reward_item;									// 排名第一奖励
};

struct CrossRandActivityConsumeRankConfig
{
	CrossRandActivityConsumeRankConfig() : rank(0), need_total_consume(0) {}

	int rank;
	long long need_total_consume;											// 排名奖励需要全服总消费额度
	ItemConfigData rank_join_reward_item;									// 排名奖励
	ItemConfigData personal_reward_item;									// 排名第一奖励
};


class CrossRandActivityConfig: public ConfigBase
{
public:
	CrossRandActivityConfig();
	~CrossRandActivityConfig();
	
	bool Init(const std::string &path, std::string *err);

	const CrossRandActivityOtherCfg & GetOtherCfg() { return m_other_cfg; };

	const CrossRandActivityChongzhiRankConfig * GetChongzhiRankCfg(int rank, long long total_chongzhi);

	const CrossRandActivityConsumeRankConfig * GetConsumeRankCfg(int rank, long long total_chongzhi);

private:
	int InitOtherCfg(PugiXmlNode  RootElement);
	int InitChongzhiRankCfg(PugiXmlNode RootElement);
	int InitConsumeRankCfg(PugiXmlNode RootElement);

	CrossRandActivityOtherCfg m_other_cfg;

	int m_chongzhi_rank_max_rank;
	CrossRandActivityChongzhiRankConfig m_chongzhi_rank_cfg_list[CROSS_RA_CHONGZHI_RANK_MAX_RANK + 1];

	int m_consume_rank_max_rank;
	CrossRandActivityConsumeRankConfig m_consume_rank_cfg_list[CROSS_RA_CONSUME_RANK_MAX_RANK + 1];
};

#endif


