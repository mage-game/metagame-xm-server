#ifndef _BOSSCARD_DEF_HPP_
#define _BOSSCARD_DEF_HPP_

#include "servercommon/servercommon.h"

static const int BOSS_CARD_STORE_MAX_BYTE = 64;															// 用来存储所有图鉴激活标记所需字节数
static const int BOSS_CARD_MAX_COUNT = BOSS_CARD_STORE_MAX_BYTE * 8;									// 最多boss图鉴数量
static const int BOSS_CARD_MAX_TYPE = 63;                                                               // boss图鉴最大卡组类型

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

	unsigned char card_can_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// 可激活标记
	unsigned char card_has_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// 已激活标记
};
struct BossCardParam
{
	BossCardParam() { this->Reset(); }

	void Reset() 
	{
		card_group_reward_fetch_flag = 0;
		memset(card_item_list, 0, sizeof(card_item_list));
	}

	unsigned long long card_group_reward_fetch_flag;                  // 卡组集齐奖励领取标记
	BossCardItem card_item_list[CARD_TYPE_MAX];
};

typedef char BossCardParamHex[sizeof(BossCardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BossCardParamHex) < 512);

#pragma pack(pop)

#endif
