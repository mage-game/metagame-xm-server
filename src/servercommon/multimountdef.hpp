#ifndef __MULTI_MOUNT_DEF_H__
#define __MULTI_MOUNT_DEF_H__

#include "servercommon.h"

static const int MULTIMOUNT_MAX_GRADE = 20;									//最大阶数
static const int MULTIMOUNT_MAX_ID = 5;										//最大ID
static const int MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT = 31;                  //幻化ID最大值
static const int MULTIMOUNT_EQUIP_TYPE_NUM = 8;								//装备个数

#pragma pack(push, 4)

struct MultiMountParamItem
{
	MultiMountParamItem() { this->Reset(); }

	void Reset()
	{
		is_mount_active = 0;
		is_img_active = 0;
		reserve_sh = 0;

		grade_bless = 0;
		grade = 0;

		memset(reserve_ll, 0, sizeof(reserve_ll));
	}

	char is_mount_active;													// 坐骑是否已激活
	char is_img_active;														// 坐骑形象是否已激活
	short reserve_sh;

	short grade_bless;														// 进阶祝福值
	short grade;															// 阶数

	long long reserve_ll[3];
};

struct MultiMountParam
{
	MultiMountParam() { this->Reset(); }

	void Reset()
	{
		cur_select_mount_id = 0;
		cur_is_riding = 0;
		reserve_ch = 0;

		for (int mount_id = 0; mount_id < MULTIMOUNT_MAX_ID; ++ mount_id)
		{
			mount_list[mount_id].Reset();
		}

		mount_list[0].is_mount_active = 1; // 第一只坐骑默认激活

		special_img_active_flag = 0;
		cur_use_img_id = 0;
		reserve_sh = 0;

		memset(equip_level_list, 0, sizeof(equip_level_list));

		memset(reserve_ll_list, 0, sizeof(reserve_ll_list));
	}

	short cur_select_mount_id;												// 当前选中的坐骑
	char cur_is_riding;														// 当前是否正在骑乘中
	char reserve_ch;

	long long reserve_ll_list[5]; 

	// 坐骑id从1开始，所以数组长度直接用最大坐骑id，数组下标 = 坐骑id - 1
	MultiMountParamItem mount_list[MULTIMOUNT_MAX_ID];

	UNSTD_STATIC_CHECK(31 == MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT);
	int special_img_active_flag;		// 特殊坐骑激活位标记, 0未激活，1激活
	short cur_use_img_id;				// 当前使用的特殊形象id,0不使用任何特殊形象
	short reserve_sh;

	UNSTD_STATIC_CHECK(8 == MULTIMOUNT_EQUIP_TYPE_NUM);
	short equip_level_list[MULTIMOUNT_EQUIP_TYPE_NUM];						// 坐骑装备等级列表
};

typedef char MultiMountParamHex[sizeof(MultiMountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MultiMountParamHex) < 1024);

#pragma pack(pop)

#endif

