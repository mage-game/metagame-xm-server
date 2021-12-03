#ifndef SHENGXINGZHULI_DEF_HPP
#define SHENGXINGZHULI_DEF_HPP

#include "servercommon.h"

#pragma pack(push, 4)

struct ShengxingzhuliParam
{
	ShengxingzhuliParam()
	{
		this->Reset();
	}

	void Reset()
	{
		today_reward_fetch_stage = 0;
		chongzhi_today = 0;
		today_reward_stage = 0;
	}

	int today_reward_fetch_stage;			// 是否已经领取今日份的奖励
	int chongzhi_today;					// 今天充值的钱
	int today_reward_stage;							// 当前档位

};

typedef char ShengxingzhuliParamHex[sizeof(ShengxingzhuliParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengxingzhuliParamHex) < 32);

#pragma pack(pop)
#endif