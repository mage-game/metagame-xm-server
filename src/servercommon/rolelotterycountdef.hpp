#ifndef __ROLE_LOTTERY_COUNT_DEF_HPP__
#define __ROLE_LOTTERY_COUNT_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"
#include "servercommon/roleshopdef.hpp"

typedef unsigned short UserLotteryCount;					// 抽奖次数
typedef ItemID LotterySequence;							// 抽奖替换的序列

static const int MAX_LOTTERY_REPLACE_SEQUENCE_COUNT = 32;

#pragma pack(push, 4)

enum LOTTERY_REPLACE_TYPE
{
	LOTTERY_REPLACE_TYPE_INVALID = -1,

	LOTTERY_REPLACE_TYPE_LITTERY_PET = 0,										// 小宠物
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP = CHEST_SHOP_TYPE_EQUIP,				// 装备寻宝
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP1 = CHEST_SHOP_TYPE_EQUIP1,			// 装备寻宝1
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP2 = CHEST_SHOP_TYPE_EQUIP2,			// 装备寻宝2
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_JINGLING = CHEST_SHOP_TYPE_JINGLING,		// 精灵寻宝
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP3 = CHEST_SHOP_TYPE_EQUIP3,			// 装备寻宝3
	LOTTERY_REPLACE_TYPE_6 = 6,													// 预留类型，留了3个位置给寻宝用
	LOTTERY_REPLACE_TYPE_7 = 7,													// 预留类型
	LOTTERY_REPLACE_TYPE_8 = 8,													// 预留类型
	LOTTERY_REPLACE_TYPE_CHINESE_ZODIAC = 9,									// 生肖龙珠
	LOTTERY_REPLACE_TYPE_SHENGESYS = 10,										// 星辉召星
	LOTTERY_REPLACE_TYPE_SHENZHOUWEAPON = 11,									// 异火觅火
	LOTTERY_REPLACE_TYPE_MYSTERIOUS_SHOP = 12,									// 神秘商店刷新次数
	LOTTERY_REPLACE_TYPE_13 = 13,												// 预留类型
	LOTTERY_REPLACE_TYPE_14 = 14,												// 预留类型

	LOTTERY_REPLACE_TYPE_MAX
};

enum LOTTERY_SEQUENCE_TYPE
{
	LOTTERY_SEQUENCE_TYPE_INVALID = -1,

	LOTTERY_SEQUENCE_TYPE_RARE = 0,												// 珍稀序列
	LOTTERY_SEQUENCE_TYPE_GARBAGE = 1,											// 垃圾序列

	LOTTERY_SEQUENCE_TYPE_MAX
};

struct RoleLotteryCountParam
{
	RoleLotteryCountParam() { this->Reset(); }

	void Reset()
	{
		memset(lottery_count, 0, sizeof(lottery_count));
	}

	UserLotteryCount lottery_count[LOTTERY_REPLACE_TYPE_MAX][MAX_LOTTERY_REPLACE_SEQUENCE_COUNT];					// 抽奖次数
};

typedef char RoleLotteryCountParamHex[sizeof(RoleLotteryCountParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(RoleLotteryCountParamHex) < 2048);

#pragma pack(pop)

#endif // __ROLE_LOTTERY_COUNT_DEF_HPP__

