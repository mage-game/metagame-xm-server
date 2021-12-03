#ifndef __GOLD_VIP_DEF_HPP__
#define __GOLD_VIP_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

static const int GOLD_VIP_CONVERT_SHOP_MAX_COUNT = 16;
static const int GOLD_VIP_CONVERT_SHOP_MUL_MAX_COUNT = 10;				// ÿ��seq����������

#pragma pack(push, 4)

struct GoldVipParam
{
	GoldVipParam() { this->Reset(); }

	void Reset()
	{
		memset(gold_vip_shop_counts_list, 0, sizeof(gold_vip_shop_counts_list));
		gold_vip_active_timestamp = 0;
		day_score = 0;
		shop_active_grade_flag = -1;
		is_active_return_reward = 0;
		can_fetch_return_reward = 0;
		is_not_first_fetch_return_reward = 0;
	}

	char gold_vip_shop_counts_list[GOLD_VIP_CONVERT_SHOP_MAX_COUNT];
	unsigned int gold_vip_active_timestamp; // ����ʱ���
	int day_score;							// ÿ�ճ�ֵ����
	char shop_active_grade_flag;			// �̵꼤��α��
	char is_active_return_reward;			// �Ƿ񼤻������
	char can_fetch_return_reward;			// �ܷ���ȡ��������
	char is_not_first_fetch_return_reward;  // �Ƿ��ǵ�һ����ȡ��������
};

typedef char GoldVipParamHex[sizeof(GoldVipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GoldVipParamHex) < 128);

#pragma pack(pop)

#endif  // __GOLD_VIP_DEF_HPP__

