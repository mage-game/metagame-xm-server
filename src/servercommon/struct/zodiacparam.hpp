#ifndef __ZODIAC_PARAM_HPP__
#define __ZODIAC_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int ZODIAC_BACKPACK_MAX_GRIDS = 200;	// ���ⱳ��������
static const int ZODIAC_MAX_NUM = 12;				// ʮ����Ф�������
static const int ZODIAC_SUIPIAN_MAX_NUM = 4;		// ��Ƭ�������
static const int ZODIAC_ATTR_MAX_NUM = 4;			// �����������

//enum ZODIAC_ATTR_TYPE
//{
//	ZODIACI_ATTR_TYPE_MAXHP = 0,									// ����
//	ZODIACI_ATTR_TYPE_POJIA = 1,									// �Ƽ�
//	ZODIACI_ATTR_TYPE_GONGJI = 2,									// ����
//	ZODIACI_ATTR_TYPE_FANGYU = 3,									// ���
//	ZODIACI_ATTR_TYPE_BAOJI = 4,									// ������
//	ZODIACI_ATTR_TYPE_WUXING_GONGJI = 5,							// ���й���
//	ZODIACI_ATTR_TYPE_WUXING_FANGYU = 6,							// ���з���
//	ZODIACI_ATTR_TYPE_FUJIA_SHANGHAI = 7,							// �����˺�
//	ZODIACI_ATTR_TYPE_DIKANG_SHANGHAI = 8,							// �ֿ��˺�
//	ZODIACI_ATTR_TYPE_PER_POFANG = 9,								// �Ʒ�����ֱ�
//	ZODIACI_ATTR_PVP_ZENGSHANG_PER = 10,                            // pvp������ֱ�
//	ZODIACI_ATTR_PVP_JIANSHANG_PER = 11,							// pvp������ֱ�
//	ZODIACI_ATTR_PVE_ZENGSHANG_PER = 12,                            // pve������ֱ�
//	ZODIACI_ATTR_PVE_JIANSHANG_PER = 13,                            // pve������ֱ�
//	ZODIACI_ATTR_SKILL_SHANGHAI_JIACHENG_PER = 14,					// �����˺��ӳ���ֱ�
//	ZODIACI_ATTR_SKILL_SHANGHAI_JIANSHAO_PER = 15,					// �����˺�������ֱ�
//
//	ZODIACI_ATTR_TYPE_COUNT,										// ʮ����Ф�����������
//};

#pragma pack(push, 4)

struct ZodiacBackpackItem
{
	ZodiacBackpackItem() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		zodiac_index = -1;
		suipian_index = -1;
		reserve_sh = 0;
	}

	unsigned short item_id;			// ��Ƭid
	short zodiac_index;				// ��Ф����
	short suipian_index;			// ��Ƭ����
	short reserve_sh;

	bool IsEmpty() const { return item_id <= 0 || zodiac_index < 0 || suipian_index < 0; }
};

struct ZodiacItem
{
	ZodiacItem()
	{
		this->Reset();
	}

	void Reset()
	{
		level = -1;
		activate_flag = 0;
		reserve_ch = 0;
	}
	
	bool IsActivateSuipian()
	{
		return activate_flag != 0;
	}

	bool IsActivateSuipian(int suipian_index)
	{
		return 0 != (activate_flag & (1 << suipian_index));	}

	void ActivateSuipian(int shengqi_index)
	{
		activate_flag |= (1 << shengqi_index);
	}

	short level;					// �ȼ�
	char activate_flag;				// ��Ƭ�������־
	char reserve_ch;

	bool IsEmpty() const { return level < 0 || activate_flag <= 0; }
};

struct ZodiacParam
{
	ZodiacParam() { this->Reset(); }

	void Reset()
	{
		jinghua_num = 0;
		for (int i = 0; i < ZODIAC_MAX_NUM; ++i)
		{
			item_list[i].Reset();
		}

		for (int i = 0; i < ZODIAC_BACKPACK_MAX_GRIDS; ++i)
		{
			backpack_zodiac_list[i].Reset();
		}
	}

	UNSTD_STATIC_CHECK(12 == ZODIAC_MAX_NUM);
	UNSTD_STATIC_CHECK(200 == ZODIAC_BACKPACK_MAX_GRIDS);

	int jinghua_num;													// ������
	ZodiacItem item_list[ZODIAC_MAX_NUM];
	ZodiacBackpackItem backpack_zodiac_list[ZODIAC_BACKPACK_MAX_GRIDS];	// �����б�
};

#pragma pack(pop)

typedef char ZodiacParamHex[sizeof(ZodiacParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZodiacParamHex) < 4096);

#endif