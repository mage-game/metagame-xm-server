#ifndef __MYSTRIOUS_SHOP_IN_MALL_DEF_HPP__
#define __MYSTRIOUS_SHOP_IN_MALL_DEF_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

static const int MYSTERIOUSSHOP_ITEM_MAX_COUNT = 12;

static const int DISCOUNTSHOP_ITEM_MAX_COUNT = 128;

struct MysteriousShopInMallItemInfo
{
	MysteriousShopInMallItemInfo(): seq(0), state(0){}

	enum MYSTERIOUSSHOP_ITEM_STATE
	{
		HAD_REFRESH_STATE = 0,		// 已刷新
		HAD_BUY_STATE,				// 已购买
	};

	int seq;
	int state;
};

struct MysteriousShopInMallParam
{
	MysteriousShopInMallParam() { this->Reset(); }

	void Reset()
	{
		next_refresh_item_time = 0;

		memset(seq_list, 0, sizeof(seq_list));
		client_remind_flag = 0;
		today_free_count = 0;
		res_sh = 0;
	}

	unsigned int next_refresh_item_time;

	MysteriousShopInMallItemInfo seq_list[MYSTERIOUSSHOP_ITEM_MAX_COUNT];
	char client_remind_flag;
	unsigned char today_free_count;						// 今日免费次数
	short res_sh;
};

struct DisCountShopParam
{
	DisCountShopParam() { this->Reset(); }

	void Reset()
	{
		today_is_refresh = 0;
		today_refresh_level = 0;
		memset(item_buy_count, 0, sizeof(item_buy_count));
	}
	
	short today_is_refresh;
	short today_refresh_level;
	short item_buy_count[DISCOUNTSHOP_ITEM_MAX_COUNT];
};

#pragma pack(pop)

typedef char MysteriousShopInMallParamHex[sizeof(MysteriousShopInMallParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MysteriousShopInMallParamHex) < 256);

typedef char DisCountShopParamHex[sizeof(DisCountShopParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DisCountShopParamHex) < 1024);

#endif // __MYSTRIOUS_SHOP_IN_MALL_DEF_HPP__