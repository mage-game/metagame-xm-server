#ifndef __CARD_ZU_DEF_H__
#define __CARD_ZU_DEF_H__

#include "servercommon.h"

static const int CARDZU_MAX_CARD_ID = 177;									// ���������ID
static const int CARDZU_MAX_ZUHE_ID = 63;									// ��������������
static const int CARDZU_TYPE_MAX_COUNT = 4;									// ���������������

static const int CARDZU_CARD_STAR_MAX_COUNT = 6;							// ���������������
static const int CARDZU_ZUHE_MAX_LEVEL = 15;								// ������ϵȼ����������

enum CARDZU_CHOUCARD_TYPE
{
	CARDZU_CHOUCARD_TYPE_COIN = 0,											// ͭ�ҵ���
	CARDZU_CHOUCARD_TYPE_GOLD,												// Ԫ������
	CARDZU_CHOUCARD_TYPE_GOLD_BATCH,										// Ԫ��ʮ����
	CARDZU_CHOUCARD_TYPE_GOLD_BIND,											// ���굥��
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

	int lingli;																	// ��������

	long long ll_reserve_1;
	long long ll_reserve_2;

	UNSTD_STATIC_CHECK(63 == CARDZU_MAX_ZUHE_ID);
	short zuhe_level_list[CARDZU_MAX_ZUHE_ID + 1];								// ������ϵĵȼ��б�

	UNSTD_STATIC_CHECK(177 == CARDZU_MAX_CARD_ID);
	short card_count_list[CARDZU_MAX_CARD_ID + 1];								// ���Ʊ��������п��Ƶ�����

	char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];						// �������ͭ�ҵ������
	char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];					// ������հ��굥�����

	char coin_choucard_less_times[CARDZU_TYPE_MAX_COUNT];						// ͭ�ҵ��飬����3�����µĴ�����ÿ10������1��3�ǻ����ϣ�
	char has_first_gold_choucard[CARDZU_TYPE_MAX_COUNT];						// �Ƿ�Ԫ��������ˣ���һ�αض�3�ǻ����ϣ�
	char has_first_gold_bind_choucard[CARDZU_TYPE_MAX_COUNT];					// �Ƿ���굥����ˣ���һ�αض�3�ǻ����ϣ�
};

typedef char CardzuParamHex[sizeof(CardzuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CardzuParamHex) < 2048);

#pragma pack(pop)

#endif

