#ifndef __XIANNVSHOUHU_DEF_HPP__
#define __XIANNVSHOUHU_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)


struct XiannvShouhuParam
{
	static const int MAX_GRADE = 30;						// 最大阶数
	static const int MAX_IMAGE_ID = 20;						// 最大形象ID
	static const int MAX_STAR_LEVEL = MAX_IMAGE_ID * 10;	// 最大星级
	static const int MAX_UP_STAR_ITEM_COUNT = 10;			// 升星物品最大数量

	XiannvShouhuParam()
	{
		this->Reset();
	}

	void Reset()
	{
		star_level = 0;
		grade = 0;
		grade_bless_val = 0;
		used_imageid = 0;
		active_image_flag = 0;
	}

	bool IsXiannvShouhuActived()
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

	short star_level;										// 星级
	short grade;											// 阶
	int used_imageid;										// 使用的形象
	int grade_bless_val;									// 进阶祝福值
	int active_image_flag;									// 激活的形象列表
};

#pragma pack(pop)

typedef char XiannvShouhuParamHex[sizeof(XiannvShouhuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XiannvShouhuParamHex) < 512);

#endif  // __XIANNVSHOUHU_DEF_HPP__

