#ifndef __FEIXIAN_PARAM_HPP__
#define __FEIXIAN_PARAM_HPP__

#include "servercommon.h"
#include "struct/itemlistparam.h"

#pragma pack(push, 4)

struct FeiXianParam
{
	FeiXianParam() { this->Reset(); }

	enum E_INDEX_FEIXIAN
	{
		E_INDEX_WUQI = 0,											// 武器
		E_INDEX_YIFU,												// 铠甲
		E_INDEX_HUSHOU,												// 护手
		E_INDEX_YAODAI,												// 腰带
//		E_INDEX_HUTUI,												// 护腿
		E_INDEX_TOUKUI,												// 头盔
		E_INDEX_XIANGLIAN,											// 项链
		E_INDEX_SHOUZHUO,											// 手镯
		E_INDEX_JIEZHI,												// 戒指
		E_INDEX_XIEZI,												// 鞋子
		E_INDEX_YUPEI,												// 玉佩

		E_INDEX_MAX,
	};

	void Reset()
	{
		for (int i = 0; i < E_INDEX_MAX; ++i)
		{
			equip_list[i].Reset();
		}
	}

	ItemDataWrapper equip_list[E_INDEX_MAX]; // 飞仙装备列表
};

typedef char FeiXianParamHex[sizeof(FeiXianParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FeiXianParamHex) < 4096);

#pragma pack(pop)
#endif
