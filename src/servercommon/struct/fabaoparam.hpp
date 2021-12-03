#ifndef __FABAO_PARAM_HPP__
#define __FABAO_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push) 
#pragma pack(4)

struct FabaoParam
{
	static const int MAX_GRADE = 30;						// ������
	static const int MAX_IMAGE_COUNT = 128;						// �������ID
	static const int MAX_SPECIAL_IMAGE_COUNT = 128;				// �����������ID
	static const int MAX_UPGRADE_LIMIT = 127;				// ��������������ȼ�
	static const int SKILL_COUNT = 4;						// ��������
	static const int SKILL_MAX_LEVEL = 100;					// �������ȼ�
	static const int EQUIP_COUNT = 4;						// װ������
	static const int EQUIP_MAX_LEVEL = 151;					// װ�����ȼ�
	static const int DISTINGUISH_IMG_SPECIALIMG_NUM = 1000; // ������������������ֵ
	static const int MAX_ACTIVE_IMAGE_BYTE = MAX_IMAGE_COUNT / BIT_COUNT_OF_BYTE;			// ��������ֽ�,�������֧��128������
	static const int MAX_ACTIVE_SPECIAL_IMAGE_BYTE = MAX_SPECIAL_IMAGE_COUNT / BIT_COUNT_OF_BYTE;

	FabaoParam()
	{
		this->Reset();
	}

	void Reset()
	{
		grade = 0;
		used_imageid = 0;
		used_special_id = 0;
		shuxingdan_count = 0;
		memset(active_image_flag, 0, sizeof(active_image_flag));
		memset(active_special_image_flag, 0, sizeof(active_special_image_flag));
		clear_bless_time = 0;

		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));
		grade_bless_val = 0;
		last_upgrade_succ_time = 0;
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(equip_level_list, 0, sizeof(equip_level_list));
		equip_skill_level = 0;
		capability = 0;
		is_used_special_imaged = 0;
	}

	void ActiveImage(int attr_id)
	{
		BitmapFunction::SetBit(active_image_flag, sizeof(active_image_flag), attr_id);
	}

	bool IsFabaoActived()
	{
		return grade > 0;
	}

	bool IsImageActived(int attr_id)
	{
		return BitmapFunction::IsBitSet(active_image_flag, sizeof(active_image_flag), attr_id);
	}

	bool IsSpecialImageActive(int attr_id)
	{
		return BitmapFunction::IsBitSet(active_special_image_flag, sizeof(active_special_image_flag), attr_id);
	}

	void ActiveSpecialImage(int attr_id)
	{
		BitmapFunction::SetBit(active_special_image_flag, sizeof(active_special_image_flag), attr_id);
	}

	short grade;									        // ����
	short used_imageid;									    // ʹ�õ�����
	short used_special_id;                                  // ʹ�õ���������
	short shuxingdan_count;							        // ���Ե�����

	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];									// ���������
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// �������������
	unsigned int clear_bless_time;						    // ���ף��ֵ��ʱ��
	int grade_bless_val;									// ����ף��ֵ
	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// ������������б�
	unsigned int last_upgrade_succ_time;					// ��һ�ν��׳ɹ���ʱ��
	short skill_level_list[SKILL_COUNT];					// ���ܵȼ��б��±�Ϊ����
	short equip_level_list[EQUIP_COUNT];					// װ���ȼ���Ϣ,�±�Ϊװ��������ֵΪ�ȼ�
	int equip_skill_level;									// װ�����ܵȼ�
	int capability;											// ս����
	int is_used_special_imaged;                             //�Ƿ�ʹ�ûû�����
};

#pragma pack(pop)

typedef char FabaoParamHex[sizeof(FabaoParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FabaoParamHex) < 512)

#endif