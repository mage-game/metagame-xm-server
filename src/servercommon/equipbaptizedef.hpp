#ifndef __EQUIP_BAPTIZE_DEF_HPP__
#define __EQUIP_BAPTIZE_DEF_HPP__

#include "servercommon.h"

static const int EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM = 6;	// һ����λ����ϴ������
static const int EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM = 10;			// ��װ�����ӱ���һ��
static const int EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM = 5;			// װ��ϴ��������ɫ�������

enum EQUIP_BAPTIZE_ATTR_COLOR_TYPE
{
	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_INVALID = -1,

	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_WHITE = 0,		// ��
	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_BLUE,				// ��
	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_PURPLE,			// ��
	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_ORANGE,			// ��
	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_RED,				// ��

	EQUIP_BAPTIZE_ATTR_COLOR_TYPE_MAX,
};

UNSTD_STATIC_CHECK(EQUIP_BAPTIZE_ATTR_COLOR_TYPE_MAX == EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM);

enum EQUIP_BAPTIZE_SPECIAL_TYPE
{
	EQUIP_BAPTIZE_SPECIAL_TYPE_NONE,			// ��
	EQUIP_BAPTIZE_SPECIAL_TYPE_ORANGE,			// ��
	EQUIP_BAPTIZE_SPECIAL_TYPE_RED,				// ��
	EQUIP_BAPTIZE_SPECIAL_TYPE_PURPLE,          // ��

	EQUIP_BAPTIZE_SPECIAL_TYPE_MAX,
};

#pragma pack(push, 4)

// ��λ
struct EquipBaptizePartInfo
{
	EquipBaptizePartInfo() { this->Reset(); }

	void Reset()
	{
		memset(baptize_list, 0, sizeof(baptize_list));
		memset(attr_seq_list, 0, sizeof(attr_seq_list));
	}

	int baptize_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];       // ����ֵ
	short attr_seq_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];      // ����seq
};

struct EquipBaptizeParam
{
	EquipBaptizeParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++ i)
		{
			part_info_list[i].Reset();
		}

		memset(open_flag, 0, sizeof(open_flag));
		memset(lock_flag, 0, sizeof(lock_flag));
		used_free_times = 0;
		reserve_ch = 0;
		part_open_flag = 0;
	}

	EquipBaptizePartInfo part_info_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];

	unsigned char open_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
	unsigned char lock_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
	unsigned char used_free_times;
	unsigned char reserve_ch;
	unsigned short part_open_flag;

	UNSTD_STATIC_CHECK(10 == EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM);
};

typedef char EquipBaptizeParamHex[sizeof(EquipBaptizeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EquipBaptizeParamHex) < 1024);

#pragma pack(pop)

#endif