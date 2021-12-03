#ifndef __HALO_DEF_HPP__
#define __HALO_DEF_HPP__

#include "servercommon.h"
#include "bitmapfunction.hpp"

#pragma pack(push, 4)


struct HaloParam
{
	static const int MAX_GRADE = 30;						// ������
	static const int MAX_IMAGE_COUNT = 128;					// �������ID
	static const int MAX_STAR_LEVEL = MAX_IMAGE_COUNT * 10;	// ����Ǽ�
	static const int MAX_UP_STAR_ITEM_COUNT = 10;			// ������Ʒ�������
	static const int MAX_LEVEL = 1000;						// ���ȼ�
	static const int MAX_SPECIAL_IMAGE_COUNT = 128;			// �����������ID
	static const int MAX_UPGRADE_LIMIT = 150;				// ��������������ȼ�
	static const int EQUIP_COUNT = 4;						// װ������
	static const int EQUIP_MAX_LEVEL = 151;					// װ�����ȼ�
	static const int SKILL_COUNT = 4;						// ��������
	static const int SKILL_MAX_LEVEL = 100;					// �������ȼ�
	static const int MAX_ACTIVE_IMAGE_BYTE = MAX_IMAGE_COUNT / BIT_COUNT_OF_BYTE;			// ��������ֽ�,�������֧��128������
	static const int MAX_ACTIVE_SPECIAL_IMAGE_BYTE = MAX_SPECIAL_IMAGE_COUNT / BIT_COUNT_OF_BYTE;

	HaloParam()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;
		grade = 0;
		old_grade_bless_val = 0;
		used_imageid = 0;
		shuxingdan_count = 0;
		chengzhangdan_count = 0;

		memset(active_image_flag, 0, sizeof(active_image_flag));
		memset(active_special_image_flag, 0, sizeof(active_special_image_flag));
		clear_upgrade_time = 0;
		reward_limit_time = 0;
		memset(equip_level_list, 0, sizeof(equip_level_list));
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));

		memset(skill_last_colddown_time, 0, sizeof(skill_last_colddown_time));
		star_level = 0;
		reserved = 0;
		grade_bless_val = 0;
		equip_skill_level = 0;
		capability = 0;
	}

	bool IsHaloActived()
	{
		return grade > 0;
	}

	bool IsImageActive(int attr_id) 
	{
		return BitmapFunction::IsBitSet(active_image_flag, sizeof(active_image_flag), attr_id);
	}

	void ActiveImage(int attr_id) 
	{
		BitmapFunction::SetBit(active_image_flag, sizeof(active_image_flag), attr_id);
	}

	bool IsSpecialImageActive(int attr_id) 
	{
		return BitmapFunction::IsBitSet(active_special_image_flag, sizeof(active_special_image_flag), attr_id);
	}

	void ActiveSpecialImage(int attr_id) 
	{
		BitmapFunction::SetBit(active_special_image_flag, sizeof(active_special_image_flag), attr_id);
	}

	short level;											// �ȼ�
	short grade;											// ��
	short old_grade_bless_val;								// ����ף��ֵ
	short used_imageid;										// ʹ�õ�����
	short shuxingdan_count;									// ���Ե�����
	short chengzhangdan_count;								// �ɳ�������

	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];	// ����������б�
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
	unsigned int clear_upgrade_time;						// ���ף��ֵ��ʱ��
	unsigned int reward_limit_time;							// �������׵���ʱ��

	short equip_level_list[EQUIP_COUNT];					// װ����Ϣ
	short skill_level_list[SKILL_COUNT];					// ���ܵȼ�

	unsigned long long skill_last_colddown_time[SKILL_COUNT];	// ������ȴʱ���б�
	short star_level;										// �Ǽ�
	short reserved;
	int grade_bless_val;									// ����ף��ֵ
	int equip_skill_level;									// װ�����ܵȼ�

	int capability;											// ս����

	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// �����������
};

#pragma pack(pop)

typedef char HaloParamHex[sizeof(HaloParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HaloParamHex) < 768);

#endif  // __HALO_DEF_HPP__

