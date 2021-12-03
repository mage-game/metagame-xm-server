#ifndef __FAIRY_DEF_HPP__
#define __FAIRY_DEF_HPP__

#include "servercommon.h"

static const int FAIRY_TREE_MAX_LEVEL = 10;					// 最大等级
static const int FAIRY_TREE_MAX_GRADE = 10;					// 最大阶
static const int FAIRY_TREE_DRAW_MAX_REWARD_CFG_COUNT = 100;// 抽奖最多奖励配置
static const int FAIRY_TREE_DRAW_MAX_REWARD_COUNT = 10;		// 抽奖最多奖励

#pragma pack(push, 4)

struct FairyTreeParam
{
	FairyTreeParam() { this->Reset(); }

	void Reset()
	{
		online_time_s_for_money = 0;
		online_time_s_for_gift = 0;

		level = 1;

		grade = 0;
		grade_val = 0;

		funny_farm_score = 0;
		special_draw_times = 0;
	}

	unsigned int online_time_s_for_money;	// 在线累计时间，用于产出元宝和铜币
	unsigned int online_time_s_for_gift;	// 在线累计时间，用于产出礼物

	int level;								// 仙树等级
	
	int grade;								// 阶数
	int grade_val;							// 进阶祝福值

	int funny_farm_score;					// 跨服趣味农场积分
	int special_draw_times;					// 特殊抽奖次数
};

typedef char FairyTreeParamHex[sizeof(FairyTreeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FairyTreeParamHex) < 64);

#pragma pack(pop)

#endif
