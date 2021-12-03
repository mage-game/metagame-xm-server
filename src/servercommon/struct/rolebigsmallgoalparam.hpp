#ifndef __ROLE_BIG_SMALL_GOAL_PARAM_HPP__
#define __ROLE_BIG_SMALL_GOAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX = 2;
static const int ROLE_BIG_SMALL_GOAL_SYSTEM_COUNT = 14;

enum ROLE_BIG_SMALL_GOAL_SYSTEM_TYPE
{
	ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID = -1,

	ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE = 0,               // ս��
	ROLE_BIG_SMALL_GOAL_SYSTEM_XIANNV,                 // ��Ů
	ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG,              // �ɳ�
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON,        // ���
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE,                 // �ǻ�
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU,               // ����
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN,                // ����
	ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN,       // ��ͨװ��ȫ��ǿ��
	ROLE_BIG_SMALL_GOAL_SYSTEM_STONE,				   // ȫ��ʯ
	ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC,		   // ��Ф
	ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI,				   // ʥ��
	ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER,		   // ��ħ(����)
	ROLE_BIG_SMALL_GOAL_SYSTEM_MAX,
};

enum ROLE_BIG_SMALL_GOAL_CONDITION_TYPE
{
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_INVALID = 0,
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1,               // װ��    ÿ������ͳ��Ʒ������       param1  Ʒ��   param2  ����
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2,               // ����    ÿ��ȡ���ֵ               param1  �ȼ�
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3,               // ����    ���ڸ�ϵͳ����index      param1  index
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4,               // ���    ÿ��ִ�м�һ               param1  Ʒ��   param2  ����
	ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_5,               // ������  ���ڼ�����                 param1  Ʒ��   param2  ����
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

