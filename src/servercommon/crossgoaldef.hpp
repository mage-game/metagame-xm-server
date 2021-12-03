#ifndef __CROSS_GOAL_PARAM_HPP__
#define __CROSS_GOAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int CROSS_GOAL_TASK_COUNT_MAX = 16;

enum CORSS_GAOL_REWARD_TYPE
{
	CORSS_GAOL_REWARD_TYPE_INVALID = -1,

	CORSS_GAOL_REWARD_TYPE_UNDONE = 0,					// 未完成
	CORSS_GAOL_REWARD_TYPE_CAN_FETCH = 1,				// 可领取
	CORSS_GAOL_REWARD_TYPE_FINISH = 2,					// 已领取

	CORSS_GAOL_REWARD_TYPE_MAX
};

#pragma pack(push) 
#pragma pack(4)

struct CrossGoalParam
{
	CrossGoalParam()
	{
		this->Reset();
	}

	void Reset()
	{
		memset(fetch_reward_flag, 0, sizeof(fetch_reward_flag));
		kill_cross_boss_num = 0;
		cross_boss_role_killer = 0;
		kill_baizhan_boss_num = 0;
		finish_baizhan_task_num = 0;
		memset(guild_fetch_reward_flag, 0, sizeof(guild_fetch_reward_flag));
		guild_notify_second = 0;
	}

	char fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // 任务状态标记 0 未完成 1 可领取 2 已领取
	unsigned short kill_cross_boss_num;		//击杀神域boss数
	unsigned short cross_boss_role_killer;	//神域boss场景杀人数
	unsigned short kill_baizhan_boss_num;		//击杀百战boss数
	unsigned short finish_baizhan_task_num;		//完成百战boss数
	char guild_fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // 任务状态标记 0 未完成 1 可领取 2 已领取
	unsigned int guild_notify_second;
};

#pragma pack(pop)

typedef char CrossGoalParamHex[sizeof(CrossGoalParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossGoalParamHex) < 512)


#endif // __ROLE_GOAL_PARAM_HPP__

