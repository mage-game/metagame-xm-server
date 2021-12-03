// 跨服装备系统相关定义

#ifndef __CROSS_EQUIP_DEF_HPP__
#define __CROSS_EQUIP_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int CROSS_EQUIP_MAX_ORDER = 10;				// 斗气装备最大阶数

enum CROSS_EQUIP_DOUQIDAN_TYPE
{
	CROSS_EQUIP_DOUQIDAN_TYPE_BEGIN, 

	CROSS_EQUIP_DOUQIDAN_TYPE_SMALL = CROSS_EQUIP_DOUQIDAN_TYPE_BEGIN,
	CROSS_EQUIP_DOUQIDAN_TYPE_MID,
	CROSS_EQUIP_DOUQIDAN_TYPE_BIG,

	CROSS_EQUIP_DOUQIDAN_TYPE_MAX,
};

enum CROSS_EQUIP_INDEX
{
	CROSS_EQUIP_INDEX_MIN,

	CROSS_EQUIP_INDEX_WUQI = CROSS_EQUIP_INDEX_MIN,						// 武器
	CROSS_EQUIP_INDEX_TOUKUI,											// 头盔
	CROSS_EQUIP_INDEX_YIFU,												// 衣服
	CROSS_EQUIP_INDEX_HUSHOU,											// 护手
	CROSS_EQUIP_INDEX_YAODAI,											// 腰带
	CROSS_EQUIP_INDEX_KUZI,												// 裤子
	CROSS_EQUIP_INDEX_XIEZI,											// 鞋子
	CROSS_EQUIP_INDEX_ZHUOZI,											// 镯子
	CROSS_EQUIP_INDEX_XIANGLIAN,										// 项链
	CROSS_EQUIP_INDEX_JIEZHI,											// 戒指

	CROSS_EQUIP_INDEX_MAX,
};

struct CrossEquipParam
{
	void Reset()
	{
		douqi_grade = 0;
		today_xiulian_times = 0;
		douqi_exp = 0;
		reserve_sh = 0;
		chuanshi_fragment = 0;
		memset(douqi_dan_used_count, 0, sizeof(douqi_dan_used_count));

		for (int i = 0; i < CROSS_EQUIP_INDEX_MAX; i++)
		{
			equip_list[i].Reset();
		}
	}

	unsigned short douqi_grade;					// 斗气阶段
	unsigned short today_xiulian_times;			// 每天修炼次数

	long long douqi_exp;						// 斗气经验

	unsigned short douqi_dan_used_count[CROSS_EQUIP_DOUQIDAN_TYPE_MAX];		// 斗气丹使用次数
	UNSTD_STATIC_CHECK(CROSS_EQUIP_DOUQIDAN_TYPE_MAX == 3);

	short reserve_sh;	

	int chuanshi_fragment;						// 传世碎片

	ItemDataWrapper equip_list[CROSS_EQUIP_INDEX_MAX];
}; 

using CrossEquipParamHex = char[sizeof(CrossEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossEquipParamHex) < 3072);

#pragma pack(pop)

#endif // __CROSS_EQUIP_DEF_HPP__