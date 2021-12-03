#ifndef __ZODIAC_PARAM_HPP__
#define __ZODIAC_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int ZODIAC_BACKPACK_MAX_GRIDS = 200;	// 虚拟背包格子数
static const int ZODIAC_MAX_NUM = 12;				// 十二生肖最大数量
static const int ZODIAC_SUIPIAN_MAX_NUM = 4;		// 碎片最大数量
static const int ZODIAC_ATTR_MAX_NUM = 4;			// 属性最大数量

//enum ZODIAC_ATTR_TYPE
//{
//	ZODIACI_ATTR_TYPE_MAXHP = 0,									// 生命
//	ZODIACI_ATTR_TYPE_POJIA = 1,									// 破甲
//	ZODIACI_ATTR_TYPE_GONGJI = 2,									// 攻击
//	ZODIACI_ATTR_TYPE_FANGYU = 3,									// 物防
//	ZODIACI_ATTR_TYPE_BAOJI = 4,									// 暴击力
//	ZODIACI_ATTR_TYPE_WUXING_GONGJI = 5,							// 五行攻击
//	ZODIACI_ATTR_TYPE_WUXING_FANGYU = 6,							// 五行防御
//	ZODIACI_ATTR_TYPE_FUJIA_SHANGHAI = 7,							// 附加伤害
//	ZODIACI_ATTR_TYPE_DIKANG_SHANGHAI = 8,							// 抵抗伤害
//	ZODIACI_ATTR_TYPE_PER_POFANG = 9,								// 破防率万分比
//	ZODIACI_ATTR_PVP_ZENGSHANG_PER = 10,                            // pvp增伤万分比
//	ZODIACI_ATTR_PVP_JIANSHANG_PER = 11,							// pvp减伤万分比
//	ZODIACI_ATTR_PVE_ZENGSHANG_PER = 12,                            // pve增伤万分比
//	ZODIACI_ATTR_PVE_JIANSHANG_PER = 13,                            // pve减伤万分比
//	ZODIACI_ATTR_SKILL_SHANGHAI_JIACHENG_PER = 14,					// 技能伤害加成万分比
//	ZODIACI_ATTR_SKILL_SHANGHAI_JIANSHAO_PER = 15,					// 技能伤害减少万分比
//
//	ZODIACI_ATTR_TYPE_COUNT,										// 十二生肖属性最大类型
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

	unsigned short item_id;			// 碎片id
	short zodiac_index;				// 生肖索引
	short suipian_index;			// 碎片索引
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

	short level;					// 等级
	char activate_flag;				// 碎片激活开启标志
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

	int jinghua_num;													// 精华数
	ZodiacItem item_list[ZODIAC_MAX_NUM];
	ZodiacBackpackItem backpack_zodiac_list[ZODIAC_BACKPACK_MAX_GRIDS];	// 背包列表
};

#pragma pack(pop)

typedef char ZodiacParamHex[sizeof(ZodiacParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZodiacParamHex) < 4096);

#endif