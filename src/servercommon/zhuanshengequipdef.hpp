#ifndef _ZHUANSHENGEQUIP_DEF_HPP_
#define _ZHUANSHENGEQUIP_DEF_HPP_

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static const int ZHUANSHENG_ATTR_LEVEL_LIMIT = 20;				// ת���ȼ�������������
static const int ZHUANSHENG_EQUIP_CFG_MAX_COUNT_LIMIT = 100;	// ת��װ�������������� 
static const int ZHUANSHENG_CHOU_CFG_LIMIT = 200;				// ת���齱������������
static const int ZHUANSHENG_EQUIP_GRADE_LIMIT = 10;				// ת��װ����������
static const int ZHUANSHENG_XIUWEI_CFG_LIMIT = 5;				// ת��ÿ����Ϊ�һ�����
static const int ZHUANSHENG_EQUIP_COUNT_MAX = 8;				// װ����λ����
static const int ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT = 100;		// ������������
static const int ZHUANSHENG_EQUIP_SECTION_MAX_COUNT = 10;		// ת���齱���ֶ���
static const int ZHUANSHENG_EQUIP_XIUWEI_DAN_COUNT = 4;			// ת����Ϊ������

enum ZHUANSHENG_EQUIP_TYPE 
{
	ZHUANSHENG_EQUIP_TYPE_1 = 0,
	ZHUANSHENG_EQUIP_TYPE_2,
	ZHUANSHENG_EQUIP_TYPE_3,
	ZHUANSHENG_EQUIP_TYPE_4,
	ZHUANSHENG_EQUIP_TYPE_5,
	ZHUANSHENG_EQUIP_TYPE_6,
	ZHUANSHENG_EQUIP_TYPE_7,
	ZHUANSHENG_EQUIP_TYPE_8,

	ZHUANSHENG_EQUIP_TYPE_MAX,
};

enum ZHUANSHENG_RAND_ATTR_S_TYPE
{
	ZRS_TYPE_ADDEXP_MIN = 200,
	ZRS_TYPE_ADDEXP_PER = ZRS_TYPE_ADDEXP_MIN,				// ��־�����ֱ�
	ZRS_TYPE_ADDCOIN_PER,									// ����ͭǮ��ֱ�	
	ZRS_TYPE_ADDEXP_MAX,
};

UNSTD_STATIC_CHECK(ZRS_TYPE_ADDEXP_MAX < 256);

#pragma pack(push, 4)

struct ZhuanshenEuipGridItem
{
	ZhuanshenEuipGridItem() { this->Reset(); }

	void Reset()
	{
		equip_id = 0;
		is_bind = 0;
		reserve_ch = 0;
	}

	unsigned short equip_id;
	char is_bind;
	char reserve_ch;
};

struct ZhuanShengEquipParam
{
	ZhuanShengEquipParam() { this->Reset(); }

	void Reset()
	{
		memset(inuse_equip_list, 0, sizeof(inuse_equip_list));
		reserve_sh1 = 0;
		reserve_sh2 = 0;

		last_time_free_chou_timestamp = 0;
		personal_xiuwei = 0;
		zhuansheng_level = 0;
		day_change_times = 0;

		for (int i = 0; i < ZHUANSHENG_EQUIP_TYPE_MAX; i++)
		{
			zhuansheng_equip_list[i].Reset();
		}
	}

	unsigned short inuse_equip_list[ZHUANSHENG_EQUIP_COUNT_MAX];					// ÿ����λ��Ӧװ��ID
	short reserve_sh1;
	short reserve_sh2;

	unsigned int last_time_free_chou_timestamp;									// �ϴ���ѳ�ʱ���
	int personal_xiuwei;														// ������Ϊ
	short zhuansheng_level;														// ��ǰת���ȼ�
	short day_change_times;														// ����һ�����

	ItemDataWrapper zhuansheng_equip_list[ZHUANSHENG_EQUIP_TYPE_MAX];
};

typedef char ZhuanShengEquipParamHex[sizeof(ZhuanShengEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhuanShengEquipParamHex) < 2048);

#pragma pack(pop)

#endif

