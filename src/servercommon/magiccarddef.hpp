#ifndef __MAGIC_CARD_DEF_H__
#define __MAGIC_CARD_DEF_H__

#include "servercommon.h"

static const int MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT = 4;							// 魔卡位置最大种类限制
static const int MAGIC_CARD_MAX_LIMIT_COUNT = 27;								// 魔卡最大卡牌数量
static const int MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT = 16;					// 魔卡抽卡奖品最大数量

static const int MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX = 10;						// 魔卡最大强化等级

enum MAGIC_CARD_COLOR_TYPE
{
	MAGIC_CARD_COLOR_TYPE_BLUE = 0,											// 蓝色
	MAGIC_CARD_COLOR_TYPE_PURPLE,											// 紫色
	MAGIC_CARD_COLOR_TYPE_ORANGE,											// 橙色
	MAGIC_CARD_COLOR_TYPE_RED,												// 红色

	MAGIC_CARD_COLOR_TYPE_COLOR_COUNT,
};

#pragma pack(push, 4)

struct MagicCardSlotItem
{
	MagicCardSlotItem() : card_id(0), strength_level(0), exp(0) {}

	short card_id;
	short strength_level;
	int exp;
};

struct MagicCardParam
{
	MagicCardParam() { this->Reset(); }

	void Reset()
	{
		today_purple_free_chou_card_times = 0;
		all_card_has_exchange_flag = 0;

		memset(all_card_num_list, 0, sizeof(all_card_num_list));
		memset(card_slot_list, 0, sizeof(card_slot_list));
	}

	int today_purple_free_chou_card_times;									// 紫卡抽奖今日已免费抽卡次数

	UNSTD_STATIC_CHECK(MAGIC_CARD_MAX_LIMIT_COUNT < 31);
	int all_card_has_exchange_flag;											// 所有卡牌是否被兑换过的标记

	UNSTD_STATIC_CHECK(MAGIC_CARD_MAX_LIMIT_COUNT == 27);
	unsigned short all_card_num_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];

	UNSTD_STATIC_CHECK(MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT == 4);				// 限制位置数量只能为4，否则数据无法扩展
	MagicCardSlotItem card_slot_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT][MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT]; // 所有颜色卡槽列表	
};

typedef char MagicCardParamHex[sizeof(MagicCardParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MagicCardParamHex) < 512);

#pragma pack(pop)

#endif

