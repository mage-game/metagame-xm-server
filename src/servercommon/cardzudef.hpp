#ifndef __CARD_ZU_DEF_H__
#define __CARD_ZU_DEF_H__

#include "servercommon.h"

static const int CARDZU_MAX_CARD_ID = 177;									// 卡牌最大卡牌ID
static const int CARDZU_MAX_ZUHE_ID = 63;									// 卡牌组合最大数量
static const int CARDZU_TYPE_MAX_COUNT = 4;									// 卡组类型最大数量

static const int CARDZU_CARD_STAR_MAX_COUNT = 6;							// 卡牌星星最大数量
static const int CARDZU_ZUHE_MAX_LEVEL = 15;								// 卡牌组合等级配置最大数

enum CARDZU_CHOUCARD_TYPE
{
	CARDZU_CHOUCARD_TYPE_COIN = 0,											// 铜币单抽
	CARDZU_CHOUCARD_TYPE_GOLD,												// 元宝单抽
	CARDZU_CHOUCARD_TYPE_GOLD_BATCH,										// 元宝十连抽
	CARDZU_CHOUCARD_TYPE_GOLD_BIND,											// 绑钻单抽
};

#pragma pack(push, 4)

struct CardzuParam
{
	CardzuParam() { this->Reset(); }

	void Reset()
	{
		lingli = 0;
		ll_reserve_1 = 0;
		ll_reserve_2 = 0;

		memset(zuhe_level_list, 0, sizeof(zuhe_level_list));
		memset(card_count_list, 0, sizeof(card_count_list));
		memset(today_coin_buy_card_times, 0, sizeof(today_coin_buy_card_times));
		memset(today_gold_bind_buy_card_times, 0, sizeof(today_gold_bind_buy_card_times));
		memset(coin_choucard_less_times, 0, sizeof(coin_choucard_less_times));
		memset(has_first_gold_choucard, 0, sizeof(has_first_gold_choucard));
		memset(has_first_gold_bind_choucard, 0, sizeof(has_first_gold_bind_choucard));
	}

	int lingli;																	// 卡牌灵力

	long long ll_reserve_1;
	long long ll_reserve_2;

	UNSTD_STATIC_CHECK(63 == CARDZU_MAX_ZUHE_ID);
	short zuhe_level_list[CARDZU_MAX_ZUHE_ID + 1];								// 卡牌组合的等级列表

	UNSTD_STATIC_CHECK(177 == CARDZU_MAX_CARD_ID);
	short card_count_list[CARDZU_MAX_CARD_ID + 1];								// 卡牌背包里所有卡牌的数量

	char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];						// 卡组今日铜币单抽次数
	char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];					// 卡组今日绑钻单抽次数

	char coin_choucard_less_times[CARDZU_TYPE_MAX_COUNT];						// 铜币单抽，连续3星以下的次数（每10次至少1次3星或以上）
	char has_first_gold_choucard[CARDZU_TYPE_MAX_COUNT];						// 是否元宝单抽过了（第一次必定3星或以上）
	char has_first_gold_bind_choucard[CARDZU_TYPE_MAX_COUNT];					// 是否绑钻单抽过了（第一次必定3星或以上）
};

typedef char CardzuParamHex[sizeof(CardzuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CardzuParamHex) < 2048);

#pragma pack(pop)

#endif

