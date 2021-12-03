#ifndef __MOUNTPARAM_HPP__
#define __MOUNTPARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"
#include "gameworld/gameworld/config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

struct MountParam
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

	MountParam()
	{
		this->Reset();
	}

	enum MOUNTFLAG
	{
		MOUNTFLAG_ON = 1,
	};

	void Reset()
	{
		mount_flag = 0;
		reserve = 0;
		level = 0;
		grade = 0;
		old_grade_bless_val = 0;
		used_imageid = 0;
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
		memset(active_image_flag, 0, sizeof(active_image_flag));
		memset(active_special_image_flag, 0, sizeof(active_special_image_flag));
		clear_upgrade_time = 0;

		memset(equip_level_list, 0, sizeof(equip_level_list));
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));

		temporary_imageid = 0;
		temporary_imageid_has_select = 0;
		temporary_imageid_invalid_time = 0;

		skill0_last_colddown_time = 0;

		star_level = 0;
		reserve_1 = 0;
		grade_bless_val = 0;
		equip_skill_level = 0;
		capability = 0;
		last_upgrade_time = 0;
	}

	bool IsMountActived()
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

	bool IsOnMount()
	{
		return 0 != (mount_flag & MOUNTFLAG_ON);
	}

	void OnMount()
	{
		mount_flag |= MOUNTFLAG_ON; 
	}

	void OffMount()
	{
		mount_flag &= ~MOUNTFLAG_ON;
	}

	short mount_flag;
	short reserve;
	short level;											// �ȼ�
	short grade;											// ��
	short old_grade_bless_val;								// ����ף��ֵ(����)
	short used_imageid;										// ʹ�õ�����
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե������б�
	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];	// ����������б�
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];		// ��������
	unsigned int clear_upgrade_time;						// ���ף��ֵ��ʱ��

	short equip_level_list[EQUIP_COUNT];					// װ����Ϣ
	short skill_level_list[SKILL_COUNT];					// ���ܵȼ�

	short temporary_imageid;								// ��ǰʹ����ʱ����
	short temporary_imageid_has_select;						// ��ѡ������ʱ����
	unsigned int temporary_imageid_invalid_time;			// ��ʱ������Чʱ��
	unsigned long long skill0_last_colddown_time;			// ����0�����ȴʱ��

	short star_level;										// �ǽ�
	short reserve_1;
	int grade_bless_val;									// ����ף��ֵ
	int equip_skill_level;									// װ�����ܵȼ�

	int capability;											// ս����

	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// �����������
	unsigned int last_upgrade_time;                         //��һ�ν��׳ɹ�ʱ��
};

#pragma pack(pop)

typedef char MountParamHex[sizeof(MountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MountParamHex) < 768);

#endif



