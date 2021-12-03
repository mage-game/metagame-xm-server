#ifndef __ROLE_GOAL_PARAM_HPP__
#define __ROLE_GOAL_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int PERSONAL_GOAL_COND_MAX = 3;
static const int MAX_REWARD_LIST_COUNT = 3;
static const int PERSONAL_GOAL_CHAPTER_MAX = 100;
static const int FIELD_GOAL_SKILL_TYPE_MAX = 8;

#pragma pack(push) 
#pragma pack(4)

struct RoleGoalParam
{
	RoleGoalParam()
	{
		this->Reset();
	}

	void Reset()
	{
		cur_chapter = 0;
		old_chapter = 0;
		memset(data_list, 0, sizeof(data_list));

		field_goal_can_fetch_flag = 0;
		field_goal_fetch_flag = 0;

		absorb_blood_next_time = 0;
		role_goal_record.Reset();

		memset(skill_level_list, 0, sizeof(skill_level_list));
	}

	struct RoleGoalRecord
	{
		RoleGoalRecord()
		{
			this->Reset();
		}

		void Reset()
		{
			complete_total_num_1 = 0;
			complete_total_num_3 = 0;
			complete_total_num_4 = 0;
			complete_total_num_5 = 0;
			complete_total_num_6 = 0;
			complete_total_num_10 = 0;
			complete_total_num_11 = 0;
			complete_total_num_12 = 0;
			complete_total_num_13 = 0;
			complete_total_num_32 = 0;
			complete_total_num_33_1 = 0;
			complete_total_num_33_2 = 0;
			complete_total_num_33_3 = 0;
			complete_total_num_33_4 = 0;
			complete_total_num_33_5 = 0;
			complete_total_num_34 = 0;
			complete_total_num_35 = 0;
			complete_total_num_36_1 = 0;
			complete_total_num_36_2 = 0;
			complete_total_num_36_3 = 0;
			complete_total_num_36_4 = 0;
			complete_total_num_36_5 = 0;
			complete_total_num_36_6 = 0;
			complete_total_num_36_7 = 0;
			complete_total_num_36_8 = 0;
			complete_total_num_38 = 0;
			complete_total_num_39_1 = 0;
			complete_total_num_39_2 = 0;
			complete_total_num_39_3 = 0;
			complete_total_num_39_4 = 0;
			complete_total_num_39_5 = 0;
		}

		int complete_total_num_1;
		int complete_total_num_3;
		int complete_total_num_4;
		int complete_total_num_5;
		int complete_total_num_6;
		int complete_total_num_10;
		int complete_total_num_11;
		int complete_total_num_12;
		int complete_total_num_13;
		int complete_total_num_32;
		int complete_total_num_33_1;
		int complete_total_num_33_2;
		int complete_total_num_33_3;
		int complete_total_num_33_4;
		int complete_total_num_33_5;
		int complete_total_num_34;
		int complete_total_num_35;
		int complete_total_num_36_1;
		int complete_total_num_36_2;
		int complete_total_num_36_3;
		int complete_total_num_36_4;
		int complete_total_num_36_5;
		int complete_total_num_36_6;
		int complete_total_num_36_7;
		int complete_total_num_36_8;
		int complete_total_num_38;
		int complete_total_num_39_1;
		int complete_total_num_39_2;
		int complete_total_num_39_3;
		int complete_total_num_39_4;
		int complete_total_num_39_5;
	};

	int cur_chapter;
	int old_chapter;
	int data_list[PERSONAL_GOAL_COND_MAX];

	int field_goal_can_fetch_flag;
	int field_goal_fetch_flag;

	unsigned int absorb_blood_next_time;

	RoleGoalRecord role_goal_record;

	short skill_level_list[FIELD_GOAL_SKILL_TYPE_MAX];
};

#pragma pack(pop)

typedef char RoleGoalParamHex[sizeof(RoleGoalParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleGoalParamHex) < 768)


#endif // __ROLE_GOAL_PARAM_HPP__

