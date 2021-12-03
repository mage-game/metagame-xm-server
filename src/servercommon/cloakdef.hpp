#ifndef __CLOAK_DEF_HPP__
#define __CLOAK_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)


struct CloakParam
{
	static const int MAX_LEVEL = 1000;						// ���ȼ�
	static const int MAX_UP_LEVEL_ITEM_COUNT = 10;			// �����������
	static const int MAX_IMAGE_COUNT = 128;					// �������ID
	static const int MAX_SPECIAL_IMAGE_ID = 63;				// �����������ID
	static const int MAX_UPGRADE_LIMIT = 150;				// ��������������ȼ�

	static const int EQUIP_COUNT = 4;						// װ������
	static const int EQUIP_MAX_LEVEL = 100;					// װ�����ȼ�
	static const int SKILL_COUNT = 4;						// ��������
	static const int SKILL_MAX_LEVEL = 100;					// �������ȼ�
	static const int SHUXINGDAN_MAX_TYPE = 2;				// ���Ե����������

	CloakParam()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;
		cur_exp = 0;
		used_imageid = 0;
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
		reserve = 0;
		active_special_image_flag = 0;

		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));
		memset(equip_level_list, 0, sizeof(equip_level_list));
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(skill_last_colddown_time, 0, sizeof(skill_last_colddown_time));

		equip_skill_level = 0;
		capability = 0;
	}

	bool IsCloakActived()
	{
		return level != 0;
	}

	bool IsSpecialImageActive(int attr_id) 
	{
		return 0 != (active_special_image_flag & ((UInt64)1 << attr_id));
	}

	void ActiveSpecialImage(int attr_id) 
	{
		active_special_image_flag |= ((UInt64)1 << attr_id);
	}

	int level;												// �ȼ�
	int cur_exp;											// ��ǰ����ֵ
	int used_imageid;										// ʹ�õ�����
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե������б�
	int reserve;									
	unsigned long long active_special_image_flag;			// ��������

	short equip_level_list[EQUIP_COUNT];					// װ����Ϣ
	short skill_level_list[SKILL_COUNT];					// ���ܵȼ�

	unsigned long long skill_last_colddown_time[SKILL_COUNT];	// ������ȴʱ���б�

	int equip_skill_level;									// װ�����ܵȼ�

	int capability;											// ս����

	char special_img_grade_list[MAX_SPECIAL_IMAGE_ID + 1];	// �����������
};

#pragma pack(pop)

typedef char CloakParamHex[sizeof(CloakParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CloakParamHex) < 512);

#endif  // __FOOTPRINT_DEF_HPP__

