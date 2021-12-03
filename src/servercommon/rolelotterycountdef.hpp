#ifndef __ROLE_LOTTERY_COUNT_DEF_HPP__
#define __ROLE_LOTTERY_COUNT_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"
#include "servercommon/roleshopdef.hpp"

typedef unsigned short UserLotteryCount;					// �齱����
typedef ItemID LotterySequence;							// �齱�滻������

static const int MAX_LOTTERY_REPLACE_SEQUENCE_COUNT = 32;

#pragma pack(push, 4)

enum LOTTERY_REPLACE_TYPE
{
	LOTTERY_REPLACE_TYPE_INVALID = -1,

	LOTTERY_REPLACE_TYPE_LITTERY_PET = 0,										// С����
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP = CHEST_SHOP_TYPE_EQUIP,				// װ��Ѱ��
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP1 = CHEST_SHOP_TYPE_EQUIP1,			// װ��Ѱ��1
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP2 = CHEST_SHOP_TYPE_EQUIP2,			// װ��Ѱ��2
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_JINGLING = CHEST_SHOP_TYPE_JINGLING,		// ����Ѱ��
	LOTTERY_REPLACE_TYPE_CHEST_SHOP_EQUIP3 = CHEST_SHOP_TYPE_EQUIP3,			// װ��Ѱ��3
	LOTTERY_REPLACE_TYPE_6 = 6,													// Ԥ�����ͣ�����3��λ�ø�Ѱ����
	LOTTERY_REPLACE_TYPE_7 = 7,													// Ԥ������
	LOTTERY_REPLACE_TYPE_8 = 8,													// Ԥ������
	LOTTERY_REPLACE_TYPE_CHINESE_ZODIAC = 9,									// ��Ф����
	LOTTERY_REPLACE_TYPE_SHENGESYS = 10,										// �ǻ�����
	LOTTERY_REPLACE_TYPE_SHENZHOUWEAPON = 11,									// ����ٻ�
	LOTTERY_REPLACE_TYPE_MYSTERIOUS_SHOP = 12,									// �����̵�ˢ�´���
	LOTTERY_REPLACE_TYPE_13 = 13,												// Ԥ������
	LOTTERY_REPLACE_TYPE_14 = 14,												// Ԥ������

	LOTTERY_REPLACE_TYPE_MAX
};

enum LOTTERY_SEQUENCE_TYPE
{
	LOTTERY_SEQUENCE_TYPE_INVALID = -1,

	LOTTERY_SEQUENCE_TYPE_RARE = 0,												// ��ϡ����
	LOTTERY_SEQUENCE_TYPE_GARBAGE = 1,											// ��������

	LOTTERY_SEQUENCE_TYPE_MAX
};

struct RoleLotteryCountParam
{
	RoleLotteryCountParam() { this->Reset(); }

	void Reset()
	{
		memset(lottery_count, 0, sizeof(lottery_count));
	}

	UserLotteryCount lottery_count[LOTTERY_REPLACE_TYPE_MAX][MAX_LOTTERY_REPLACE_SEQUENCE_COUNT];					// �齱����
};

typedef char RoleLotteryCountParamHex[sizeof(RoleLotteryCountParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(RoleLotteryCountParamHex) < 2048);

#pragma pack(pop)

#endif // __ROLE_LOTTERY_COUNT_DEF_HPP__

