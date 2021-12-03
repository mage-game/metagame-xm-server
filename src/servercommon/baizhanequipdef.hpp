#ifndef _BAIZHAN_EQUIP_DEF_HPP_
#define _BAIZHAN_EQUIP_DEF_HPP_

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push, 4)

static const int MAX_BAIZHAN_SUIT_NUM = 2;				// ��ɫ�����е�������װ

//װ��-------------------------------------------------------------------------------------------

enum E_INDEX_BAIZHAN
{
	E_INDEX_BAIZHAN_WUQI = 0,											// ����
	E_INDEX_BAIZHAN_YIFU,												// ����
	E_INDEX_BAIZHAN_HUSHOU,												// ����
	E_INDEX_BAIZHAN_YAODAI,												// ����
	E_INDEX_BAIZHAN_TOUKUI,												// ͷ��
	E_INDEX_BAIZHAN_XIANGLIAN,											// ����
	E_INDEX_BAIZHAN_SHOUZHUO,											// ����
	E_INDEX_BAIZHAN_JIEZHI,												// ��ָ
	E_INDEX_BAIZHAN_XIEZI,												// Ь��
	E_INDEX_BAIZHAN_YUPEI,												// ����

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
	ItemDataWrapper baizhan_equip_data;						// װ������
};

//��װ-------------------------------------------------------------------------------------------

enum BAIZHAN_SUIT_TYPE
{
	BAIZHAN_SUIT_TYPE_INVALID = -1,
	BAIZHAN_SUIT_TYPE_1 = 0,		// ��װ1

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

	UInt8 order_list[E_INDEX_BAIZHAN_MAX];	// װ����λ�Ľ���
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

	int capability;																// ս����
	int reserve_int1;															// 

	BaizhanEquipItem baizhan_equip_list[E_INDEX_BAIZHAN_MAX];					// װ������
	BaizhanOneSuitInfo suit_order_list[MAX_BAIZHAN_SUIT_NUM];					// ��װ
};

typedef char BaizhanEquipParamHex[sizeof(BaizhanEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BaizhanEquipParamHex) < 4096);

#pragma pack(pop)

#endif

