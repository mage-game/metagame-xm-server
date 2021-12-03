#pragma once
#ifndef __XIANZUNKA_DEF_HPP__
#define __XIANZUNKA_DEF_HPP__
#include "servercommon/servercommon.h"

#pragma pack(push, 4)

enum XIANZUNKA_TYPE
{
	XIANZUNKA_TYPE_SILVER,			//白银仙尊卡
	XIANZUNKA_TYPE_GOLD,			//黄金仙尊卡
	XIANZUNKA_TYPE_DIAMOND,			//钻石仙尊卡

	XIANZUNKA_TYPE_MAX
};

struct XianZunKaParam
{
	XianZunKaParam() { this->Reset(); }

	void Reset()
	{
		forever_active_flag = 0;
		first_active_reward_flag = 0;
		daily_reward_fetch_flag = 0;

		sent_emails_flag = 0;
		is_first_recharge = 0;
		reserve_sh = 0;

		memset(temporary_valid_end_timestamp_list, 0, sizeof(temporary_valid_end_timestamp_list));
		capability = 0;
	}

	short forever_active_flag;						// 永久激活标记
	char first_active_reward_flag;					// 首次激活奖励标记
	char daily_reward_fetch_flag;					// 每日奖励拿取标记

	char sent_emails_flag;        // 邮件提醒标记
	char is_first_recharge;							// 是否首次充值
	short reserve_sh;

	UNSTD_STATIC_CHECK(3 == XIANZUNKA_TYPE_MAX);
	unsigned int temporary_valid_end_timestamp_list[XIANZUNKA_TYPE_MAX];		// 临时有效 结束时间戳
	int capability;						// 战力
};

#pragma pack(pop)

typedef char XianZunKaParamHex[sizeof(XianZunKaParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianZunKaParamHex) < 128);

#endif
