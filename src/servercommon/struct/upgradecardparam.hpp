#ifndef __UPGRADE_CARD_BUY_DEF_HPP__
#define __UPGRADE_CARD_BUY_DEF_HPP__

#include "servercommon/servercommon.h"

static const int UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT = 20;

struct UpgradeCardBuyParam
{
	UpgradeCardBuyParam() { this->Reset(); }

	void Reset()
	{
		ra_upgrade_card_buy_flag_1 = 0;
		ra_upgrade_card_buy_flag_2 = 0;
		last_reset_time = 0;
		memset(today_upgrade_system_grade, 0, sizeof(today_upgrade_system_grade));
	}

	long long ra_upgrade_card_buy_flag_1;					// 活动2199购买flag
	long long ra_upgrade_card_buy_flag_2;					// 活动2229购买flag
	long long last_reset_time;								// 上次更新时间

	short today_upgrade_system_grade[UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT];

	UNSTD_STATIC_CHECK(UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT < 64); // long long
};

using UpgradeCardBuyParamHex = char[sizeof(UpgradeCardBuyParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(UpgradeCardBuyParamHex) < 256);

#endif  // __UPGRADE_CARD_BUY_DEF_HPP__