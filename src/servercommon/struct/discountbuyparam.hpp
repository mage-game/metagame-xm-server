#ifndef __DISCOUNT_BUY_PARAM_HPP__
#define __DISCOUNT_BUY_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

const static int DISCOUNT_BUY_PHASE_MAX_COUNT = 256;
const static int DISCOUNT_BUY_ITEM_PER_PHASE = 16;

//struct DiscountBuyPhase
//{
//	DiscountBuyPhase() { this->Reset(); }
//
//	void Reset()
//	{
//		close_timestamp = 0;
//		memset(buy_count_list, 0, sizeof(buy_count_list));
//		resrver_2 = 0;
//	}
//
//	unsigned int close_timestamp;
//	char buy_count_list[DISCOUNT_BUY_ITEM_PER_PHASE];
//	short resrver_2;
//};

struct DiscountBuyPhase
{
	DiscountBuyPhase() { this->Reset(); }

	void Reset()
	{
		close_timestamp = 0;
		memset(buy_count_list, 0, sizeof(buy_count_list));
	}

	unsigned int close_timestamp;
	short buy_count_list[DISCOUNT_BUY_ITEM_PER_PHASE];
};

struct DiscountBuyParam
{
	DiscountBuyParam() { this->Reset(); }

	void Reset()
	{
		reserve_int_1 = 0;
		reserve_int_2 = 0;

		for (int i = 0; i < DISCOUNT_BUY_PHASE_MAX_COUNT; ++ i)
		{
			phase_list[i].Reset();
		}
	}

	int reserve_int_1;
	int reserve_int_2;

	DiscountBuyPhase phase_list[DISCOUNT_BUY_PHASE_MAX_COUNT];
};

#pragma pack(pop)

typedef char DiscountBuyParamHex[sizeof(DiscountBuyParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DiscountBuyParamHex) < 20480);

#endif // __DISCOUNT_BUY_PARAM_HPP__



