#ifndef _BAIZHAN_EQUIP_DEF_HPP_
#define _BAIZHAN_EQUIP_DEF_HPP_

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push, 4)

static const int MAX_BAIZHAN_SUIT_NUM = 2;				// 角色可以有的最多的套装

//装备-------------------------------------------------------------------------------------------

enum E_INDEX_BAIZHAN
{
	E_INDEX_BAIZHAN_WUQI = 0,											// 武器
	E_INDEX_BAIZHAN_YIFU,												// 铠甲
	E_INDEX_BAIZHAN_HUSHOU,												// 护手
	E_INDEX_BAIZHAN_YAODAI,												// 腰带
	E_INDEX_BAIZHAN_TOUKUI,												// 头盔
	E_INDEX_BAIZHAN_XIANGLIAN,											// 项链
	E_INDEX_BAIZHAN_SHOUZHUO,											// 手镯
	E_INDEX_BAIZHAN_JIEZHI,												// 戒指
	E_INDEX_BAIZHAN_XIEZI,												// 鞋子
	E_INDEX_BAIZHAN_YUPEI,												// 玉佩

	E_INDEX_BAIZHAN_MAX,
};

struct BaizhanEquipItem
{
	BaizhanEquipItem() { this->Reset(); }

	void Reset()
	{
		reserve_int = 0;
		reserve_int1 = 0;
		baizhan_equip_data.Reset();
	}

	int reserve_int;										// 
	int reserve_int1;										// 
	ItemDataWrapper baizhan_equip_data;						// 装备数据
};

//套装-------------------------------------------------------------------------------------------

enum BAIZHAN_SUIT_TYPE
{
	BAIZHAN_SUIT_TYPE_INVALID = -1,
	BAIZHAN_SUIT_TYPE_1 = 0,		// 套装1

	BAIZHAN_SUIT_TYPE_MAX,
};

UNSTD_STATIC_CHECK(BAIZHAN_SUIT_TYPE_MAX <= MAX_BAIZHAN_SUIT_NUM);

struct BaizhanOneSuitInfo
{
	BaizhanOneSuitInfo() { this->Reset(); }

	void Reset()
	{
		memset(order_list, 0, sizeof(order_list));
		reserve_sh = 0;
	}

	UInt8 order_list[E_INDEX_BAIZHAN_MAX];	// 装备部位的阶数
	UInt16 reserve_sh;
};

struct BaizhanEquipParam
{
	BaizhanEquipParam() { this->Reset(); }

	void Reset()
	{

		capability = 0;
		reserve_int1 = 0;

		for (int i = 0; i < E_INDEX_BAIZHAN_MAX; i++)
		{
			baizhan_equip_list[i].Reset();
		}

		for (int i = 0; i < MAX_BAIZHAN_SUIT_NUM; i ++)
		{
			suit_order_list[i].Reset();
		}
	}

	int capability;																// 战斗力
	int reserve_int1;															// 

	BaizhanEquipItem baizhan_equip_list[E_INDEX_BAIZHAN_MAX];					// 装备数据
	BaizhanOneSuitInfo suit_order_list[MAX_BAIZHAN_SUIT_NUM];					// 套装
};

typedef char BaizhanEquipParamHex[sizeof(BaizhanEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BaizhanEquipParamHex) < 4096);

#pragma pack(pop)

#endif

