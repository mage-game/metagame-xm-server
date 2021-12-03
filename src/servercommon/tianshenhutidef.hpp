#ifndef _TIANSHENHUTI_DEF_HPP_
#define _TIANSHENHUTI_DEF_HPP_

#include "servercommon.h"
#include "struct/global/autoparam.hpp"

static const int TIANSHENHUTI_EQUIP_MAX_COUNT = 8;								// װ����λ����
static const int TIANSHENHUTI_BACKPACK_MAX_COUNT = 120;							// ������������

static const int TIANSHENHUTI_MAX_EQUIP_TYPE = 3;								// �����װ����
static const int TIANSHENHUTI_MAX_EQUIP_LEVEL = 10;								// ���װ������
static const int TIANSHENHUTI_MAX_EQUIP_ID = TIANSHENHUTI_MAX_EQUIP_TYPE * TIANSHENHUTI_EQUIP_MAX_COUNT * TIANSHENHUTI_MAX_EQUIP_LEVEL;// ���װ��ID

typedef unsigned short TianshenEquipID;
const static TianshenEquipID INVALID_TIANSHEN_EQUIP_ID = 0;

#pragma pack(push, 4)

struct TianshenhutiParam
{
	TianshenhutiParam() { this->Reset(); }

	void Reset()
	{
		used_free_times = 0;
		accumulate_times = 0;

		roll_score = 0;
		next_free_roll_time = 0;

		accumulate_reward_flag = 0;

		memset(inuse_equip_list, 0, sizeof(inuse_equip_list));
		memset(backpack_equip_list, 0, sizeof(backpack_equip_list));
	}

	short used_free_times;														// ��ѱ��
	short accumulate_times;														// �ۼƴ���

	int roll_score;																// �齱����
	unsigned int next_free_roll_time;											// �´���ѳ齱ʱ��

	TianshenEquipID inuse_equip_list[TIANSHENHUTI_EQUIP_MAX_COUNT];				// ÿ����λ��Ӧװ��ID
	TianshenEquipID backpack_equip_list[TIANSHENHUTI_BACKPACK_MAX_COUNT];		// ������ӵ�е�����װ���б�

	long long accumulate_reward_flag;
};

using AutoTianshenhutiParam = AutoParamTemplate<TianshenhutiParam>;

// typedef char TianshenhutiParamHex[sizeof(TianshenhutiParam) * 2 + 1];
// UNSTD_STATIC_CHECK(sizeof(TianshenhutiParamHex) < 1024);

#pragma pack(pop)

#endif

