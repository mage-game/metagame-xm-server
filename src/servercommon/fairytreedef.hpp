#ifndef __FAIRY_DEF_HPP__
#define __FAIRY_DEF_HPP__

#include "servercommon.h"

static const int FAIRY_TREE_MAX_LEVEL = 10;					// ���ȼ�
static const int FAIRY_TREE_MAX_GRADE = 10;					// ����
static const int FAIRY_TREE_DRAW_MAX_REWARD_CFG_COUNT = 100;// �齱��ཱ������
static const int FAIRY_TREE_DRAW_MAX_REWARD_COUNT = 10;		// �齱��ཱ��

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

	unsigned int online_time_s_for_money;	// �����ۼ�ʱ�䣬���ڲ���Ԫ����ͭ��
	unsigned int online_time_s_for_gift;	// �����ۼ�ʱ�䣬���ڲ�������

	int level;								// �����ȼ�
	
	int grade;								// ����
	int grade_val;							// ����ף��ֵ

	int funny_farm_score;					// ���Ȥζũ������
	int special_draw_times;					// ����齱����
};

typedef char FairyTreeParamHex[sizeof(FairyTreeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FairyTreeParamHex) < 64);

#pragma pack(pop)

#endif
