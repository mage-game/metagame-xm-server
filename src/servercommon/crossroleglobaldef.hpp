#ifndef __CROSS_ROLE_GLOBAL_DEF_HPP__
#define __CROSS_ROLE_GLOBAL_DEF_HPP__

#include "servercommon.h"
#include "gameworld/gameworld/obj/character/attribute.hpp"

static const int CROSS_GLOBAL_ATTR_VALUE_MAX_TYPE = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX - CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN;
static const int ADJUST_CAP_MAX_BYTE = 12;
static constexpr int ADJUST_CAP_MAX_COUNT = ADJUST_CAP_MAX_BYTE * 8;									// ÿ�ֵ��������������

enum ADJUST_CAP_TYPE
{
	ADJUST_CAP_TYPE_TITLE = 0,				//�ƺ�
	ADJUST_CAP_TYPE_OTHER = 1,				//����

	ADJUST_CAP_TYPE_COUNT,
};

#pragma pack(push, 4)
/*
* ������õĸ������ݣ���Ҫ��ս��Ҫ����ȥ��
*/
struct RoleCrossGlobalData
{
	RoleCrossGlobalData() { this->Reset(); }

	void Reset()
	{
		little_pet_cur_id = 0;
		memset(adjust_cap_flag_list, 0, sizeof(adjust_cap_flag_list));
		memset(attrs, 0, sizeof(attrs));
	}

	short little_pet_cur_id;															// ��ǰʹ�õ�С����id
	unsigned char adjust_cap_flag_list[ADJUST_CAP_TYPE_COUNT][ADJUST_CAP_MAX_BYTE];			// ���ڱ���ԭ�����ڵ�ս����־�������ط����ñ�־����������ȥ��ս��

	long long attrs[CROSS_GLOBAL_ATTR_VALUE_MAX_TYPE];									// ����GLobalRole�����ս��ת���ɵ�����
};

typedef char RoleCrossGlobalDataHex[sizeof(RoleCrossGlobalData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCrossGlobalDataHex) < 1024);

#pragma pack(pop)

#endif  // __JIANGHUN_DEF_HPP__