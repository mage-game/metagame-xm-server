#ifndef _BOSSCARD_DEF_HPP_
#define _BOSSCARD_DEF_HPP_

#include "servercommon/servercommon.h"

static const int BOSS_CARD_STORE_MAX_BYTE = 64;															// �����洢����ͼ�������������ֽ���
static const int BOSS_CARD_MAX_COUNT = BOSS_CARD_STORE_MAX_BYTE * 8;									// ���bossͼ������
static const int BOSS_CARD_MAX_TYPE = 63;                                                               // bossͼ�����������

#pragma pack(push, 4)

enum CardType
{
	CARD_TYPE_BOSS = 0,

	CARD_TYPE_MAX,
};

struct BossCardItem
{
	BossCardItem() { this->Reset(); }

	void Reset()
	{
		memset(card_can_active_flag, 0, sizeof(card_can_active_flag));
		memset(card_has_active_flag, 0, sizeof(card_has_active_flag));
	}

	unsigned char card_can_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// �ɼ�����
	unsigned char card_has_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// �Ѽ�����
};
struct BossCardParam
{
	BossCardParam() { this->Reset(); }

	void Reset() 
	{
		card_group_reward_fetch_flag = 0;
		memset(card_item_list, 0, sizeof(card_item_list));
	}

	unsigned long long card_group_reward_fetch_flag;                  // ���鼯�뽱����ȡ���
	BossCardItem card_item_list[CARD_TYPE_MAX];
};

typedef char BossCardParamHex[sizeof(BossCardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BossCardParamHex) < 512);

#pragma pack(pop)

#endif
