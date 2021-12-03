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
		E_INDEX_WUQI = 0,											// ����
		E_INDEX_YIFU,												// ����
		E_INDEX_HUSHOU,												// ����
		E_INDEX_YAODAI,												// ����
//		E_INDEX_HUTUI,												// ����
		E_INDEX_TOUKUI,												// ͷ��
		E_INDEX_XIANGLIAN,											// ����
		E_INDEX_SHOUZHUO,											// ����
		E_INDEX_JIEZHI,												// ��ָ
		E_INDEX_XIEZI,												// Ь��
		E_INDEX_YUPEI,												// ����

		E_INDEX_MAX,
	};

	void Reset()
	{
		for (int i = 0; i < E_INDEX_MAX; ++i)
		{
			equip_list[i].Reset();
		}
	}

	ItemDataWrapper equip_list[E_INDEX_MAX]; // ����װ���б�
};

typedef char FeiXianParamHex[sizeof(FeiXianParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FeiXianParamHex) < 4096);

#pragma pack(pop)
#endif
