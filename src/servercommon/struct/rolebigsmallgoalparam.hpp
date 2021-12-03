#ifndef __ROLE_BIG_SMALL_GOAL_PARAM_HPP__
#define __ROLE_BIG_SMALL_GOAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX = 2;
static const int ROLE_BIG_SMALL_GOAL_SYSTEM_COUNT = 14;

enum ROLE_BIG_SMALL_GOAL_SYSTEM_TYPE
{
	ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID = -1,

	ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE = 0,               // 战魂
	ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV,                 // 仙女
	ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG,              // 仙宠
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON,        // 异火
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE,                 // 星辉
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU,               // 龙器
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN,                // 铭纹
	ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN,       // 普通装备全身强化
	ROLE_BIG_SMALL_GOAL_SYSTEM_STONE,				   // 全身宝石
	ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC,		   // 生肖
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI,				   // 圣器
	ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER,		   // 神魔(名将)
	ROLE_BIG_SMALL_GOAL_SYSTEM_MAX,
};

enum ROLE_BIG_SMALL_GOAL_CONDITION_TYPE
{
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_INVALID = 0,
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1,               // 装备    每次重新统计品质数量       param1  品质   param2  数量
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2,               // 升级    每次取最大值               param1  等级
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3,               // 激活    等于各系统配表的index      param1  index
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4,               // 获得    每次执行加一               param1  品质   param2  数量
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_5,               // 激活数  等于激活数                 param1  品质   param2  数量
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_MAX

};

enum ROLE_BIG_SMALL_GOAL_REWARD_TYPE
{
	ROLE_BIG_SMALL_GOAL_REWARD_TYPE_INVALID = -1,

	ROLE_BIG_SMALL_GOAL_REWARD_TYPE_SMALL = 0,
	ROLE_BIG_SMALL_GOAL_REWARD_TYPE_BIG = 1,

	ENUM_ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX
};

#pragma pack(push) 
#pragma pack(4)

struct RoleBigSmallGoalRewardParam
{
	struct GoalItemInfo
	{
		GoalItemInfo() : goal_progress(0), active_flag(0), active_in_limit(0), fetch_flag(0), res_ch(0){}

		int goal_progress;
		char active_flag;
		char active_in_limit;
		char fetch_flag;
		char res_ch;
	};

	struct GoalSystemInfo
	{
		GoalSystemInfo() : open_func_time_stamp(0), active_special_attr(0), res_ch(0), res_sh(0)
		{
			memset(goal_item_list, 0, sizeof(goal_item_list));
		}

		unsigned int open_func_time_stamp;
		char active_special_attr;
		char res_ch;
		short res_sh;
		GoalItemInfo goal_item_list[ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX];
	};

	void Reset()
	{
		memset(goal_info_list, 0, sizeof(goal_info_list));
	}

	RoleBigSmallGoalRewardParam()
	{
		this->Reset();
	}
	GoalSystemInfo goal_info_list[ROLE_BIG_SMALL_GOAL_SYSTEM_COUNT];
};

typedef char RoleBigSmallGoalRewardParamHex[sizeof(RoleBigSmallGoalRewardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleBigSmallGoalRewardParamHex) < 1024);

#pragma pack(pop)
#endif  // __ROLE_BIG_SMALL_GOAL_PARAM_HPP__

