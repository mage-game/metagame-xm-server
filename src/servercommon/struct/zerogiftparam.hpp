#ifndef __ZERO_GIFT_PARAM_HPP__
#define __ZERO_GIFT_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

static const int ZERO_GIFT_PHASE_MAX = 5;
static const int ZERO_GIFT_GOD_COSTUME_MAX = 5;
static const int ZERO_GIFT_GOD_LEVEL_PHASE = 3;

struct ZeroGiftPhaseInfo
{
	ZeroGiftPhaseInfo(): state(0), timestamp(0){}

	enum ZERO_GIFT_STATE
	{
		UN_ACTIVE_STATE = 0,		// 未达到购买等级，未激活
		ACTIVE_STATE,				// 达到购买等级，可购买
		HAD_BUY_STATE,				// 已购买
		HAD_FETCH_STATE,			// 已经领取奖励
	};

	int state;
	unsigned int timestamp;
};

struct ZeroGiftGodCostumeInfo
{
	ZeroGiftGodCostumeInfo() : buy_state(0), reward_flag(0) {}
	enum BUY_STATE
	{
		UN_BUY_STATE = 0,			// 未购买
		HAD_BUY_STATE = 1,			// 已购买
	};

	int buy_state;
	int reward_flag;
};

struct ZeroGiftParam
{
	ZeroGiftParam() { this->Reset(); }

	void Reset()
	{
		memset(zero_gift_list, 0, sizeof(zero_gift_list));
		memset(fetch_flag_list, 0, sizeof(fetch_flag_list));
		memset(god_costume_list, 0, sizeof(god_costume_list));
	}

	ZeroGiftPhaseInfo zero_gift_list[ZERO_GIFT_PHASE_MAX];
	int fetch_flag_list[ZERO_GIFT_PHASE_MAX];

	ZeroGiftGodCostumeInfo god_costume_list[ZERO_GIFT_GOD_COSTUME_MAX];
};

#pragma pack(pop)

typedef char ZeroGiftParamHex[sizeof(ZeroGiftParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZeroGiftParamHex) < 256);

#endif // __ZERO_GIFT_PARAM_HPP__



