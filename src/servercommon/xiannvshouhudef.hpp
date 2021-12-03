#ifndef __XIANNVSHOUHU_DEF_HPP__
#define __XIANNVSHOUHU_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)


struct XiannvShouhuParam
{
	static const int MAX_GRADE = 30;						// ������
	static const int MAX_IMAGE_ID = 20;						// �������ID
	static const int MAX_STAR_LEVEL = MAX_IMAGE_ID * 10;	// ����Ǽ�
	static const int MAX_UP_STAR_ITEM_COUNT = 10;			// ������Ʒ�������

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

	short star_level;										// �Ǽ�
	short grade;											// ��
	int used_imageid;										// ʹ�õ�����
	int grade_bless_val;									// ����ף��ֵ
	int active_image_flag;									// ����������б�
};

#pragma pack(pop)

typedef char XiannvShouhuParamHex[sizeof(XiannvShouhuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XiannvShouhuParamHex) < 512);

#endif  // __XIANNVSHOUHU_DEF_HPP__

