#ifndef __COUPLE_MOUNT_DEF_H__
#define __COUPLE_MOUNT_DEF_H__

#include "servercommon.h"

static const int COUPLEMOUNT_MAX_GRADE = 120;								//最大阶数
static const int COUPLEMOUNT_MAX_ID = 128;									//最大ID
static const int COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT = 31;                 //幻化ID最大值

#pragma pack(push, 4)

struct MultiMountParamInfo
{
	MultiMountParamInfo() { this->Reset(); }

	void Reset()
	{
		is_mount_active = 0;
		is_img_active = 0;
		reserve_sh = 0;

		grade_bless = 0;
		grade = 0;
	}

	char is_mount_active;													// 坐骑是否已激活
	char is_img_active;														// 坐骑形象是否已激活
	short reserve_sh;

	short grade_bless;														// 进阶祝福值
	short grade;															// 阶数
};

struct CoupleMountParam
{
	CoupleMountParam() { this->Reset(); }

	void Reset()
	{
		cur_select_mount_id = 0;
		cur_is_riding = 0;
		reserve_ch = 0;

		for (int mount_id = 0; mount_id < COUPLEMOUNT_MAX_ID; ++ mount_id)
		{
			mount_list[mount_id].Reset();
		}
		//改成默认不激活，需要使用道具激活
		//mount_list[0].is_mount_active = 1; // 第一只坐骑默认激活

		special_img_active_flag = 0;
		cur_use_img_id = 0;
		reserve_sh = 0;

		memset(mount_special_image_level_list, 0, sizeof(mount_special_image_level_list));
		reserve_ch_1 = 0;
	}

	short cur_select_mount_id;												// 当前选中的坐骑
	char cur_is_riding;														// 当前是否正在骑乘中
	char reserve_ch;

	UNSTD_STATIC_CHECK(31 == COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT);
	unsigned int special_img_active_flag;		// 特殊坐骑激活位标记, 0未激活，1激活
	short cur_use_img_id;				// 当前使用的特殊形象id,0不使用任何特殊形象
	short reserve_sh;

	char mount_special_image_level_list[COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT];
	char reserve_ch_1;

	// 坐骑id从1开始，所以数组长度直接用最大坐骑id，数组下标 = 坐骑id - 1
	MultiMountParamInfo mount_list[COUPLEMOUNT_MAX_ID];
};

typedef char CoupleMountParamHex[sizeof(CoupleMountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CoupleMountParamHex) < 2560);

#pragma pack(pop)

#endif

