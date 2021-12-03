#ifndef __MOLONG_CONFIG_H__
#define __MOLONG_CONFIG_H__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include <map>

const static int MOLONG_REWARD_CONFIG_MAX_COUNT = 64;		// 魔龙最大奖励配置数
const static int MOLONG_DAY_MAX_MOVE_STEP = 8;				// 魔龙每日最大移动步数
const static int MOLONG_MAX_MOVE_STEP = 30;					// 最大移动步数
const static int MOLONG_MAX_RANK_STEP = 99;				    // 头衔最大等级

struct MolongRewardConfig
{
	MolongRewardConfig() : fanli_rate(0), virtual_type(0) {}

	int fanli_rate;
	ItemConfigData reward_item;
	char virtual_type;
};

struct MolongMoveConfig
{
	MolongMoveConfig() : step(0), consume_gold(0) {}

	int step;
	int consume_gold;
};

struct MoLongRankConfig
{
	MoLongRankConfig() : value_percent(0), war_value(0), cumulate_gold(0) {}

		int value_percent;
		int war_value;
		int cumulate_gold;    
};

class MolongConfig : public ConfigBase
{
public:
	MolongConfig();
	~MolongConfig();

	bool Init(std::string path, std::string *err);

	const MolongRewardConfig * GetRewardCfg(int loop, int total_move_step);
	const MolongMoveConfig * GetMoveCfg(int move_step);
	int GetMaxLoop() { return m_max_loop; }
	int GetDayMaxMoveStep() { return m_day_max_move_step; }
	int GetDayMaxConsumeGold() { return m_move_cfglist[m_day_max_move_step].consume_gold; }
	MoLongRankConfig * GetRankGradeParamCfg(int m_grade);

private:
	int InitMoveCfg(TiXmlElement *RootElement);
	int InitRewardCfg(TiXmlElement *RootElement);
	int InitRankCfg(TiXmlElement *RootElement);

	int m_max_loop;
	MolongRewardConfig m_reward_cfglist[MOLONG_REWARD_CONFIG_MAX_COUNT];

	int m_day_max_move_step;
	MolongMoveConfig m_move_cfglist[MOLONG_DAY_MAX_MOVE_STEP + 1];

	std::map<int, MoLongRankConfig> m_rank_cfg;
};

#endif
