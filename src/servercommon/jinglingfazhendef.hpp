#ifndef __JINGLINGFAZHEN_DEF_HPP__
#define __JINGLINGFAZHEN_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)


struct JinglingFazhenParam
{
	static const int MAX_GRADE = 30;						// 最大阶数
	static const int MAX_IMAGE_ID = 20;						// 最大形象ID
	static const int MAX_SPECIAL_IMAGE_ID = 31;				// 最大特殊形象ID
	static const int MAX_UPGRADE_LIMIT = 10;				// 特殊形象进阶最大等级

	 JinglingFazhenParam()
	{
		this->Reset();
	}

	void Reset()
	{
		reserve = 0;
		grade = 0;
		grade_bless_val = 0;
		used_imageid = 0;
		active_image_flag = 0;
		clear_upgrade_time = 0;

		active_special_image_flag = 0;

		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));
	}

	bool IsJinglingFazhenActived()
	{
		return active_image_flag != 0;
	}

	bool IsImageActive(int attr_id) 
	{
		return 0 != (active_image_flag & (1 << attr_id)); 
	}

	void ActiveImage(int attr_id) 
	{
		active_image_flag |= (1 << attr_id); 
	}

	bool IsSpecialImageActive(int attr_id) 
	{
		return 0 != (active_special_image_flag & (1 << attr_id)); 
	}

	void ActiveSpecialImage(int attr_id) 
	{
		active_special_image_flag |= (1 << attr_id); 
	}

	short reserve;										
	short grade;											// 阶
	int used_imageid;										// 使用的形象
	int grade_bless_val;									// 进阶祝福值
	int active_image_flag;									// 激活的形象列表
	unsigned int clear_upgrade_time;						// 清空祝福值的时间

	int active_special_image_flag;							// 特殊形象
	char special_img_grade_list[MAX_SPECIAL_IMAGE_ID + 1];	// 特殊形象阶数
};

#pragma pack(pop)

typedef char  JinglingFazhenParamHex[sizeof( JinglingFazhenParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof( JinglingFazhenParamHex) < 512);

#endif  // __JINGLINGFAZHEN_DEF_HPP__

