#ifndef _UPGRADEPARAM_HPP__
#define _UPGRADEPARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"
#include "gameworld/gameworld/config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

enum UPGRADE_TYPE
{
	UPGRADE_TYPE_INVALID = -1,

	UPGRADE_TYPE_0,           // ����
	UPGRADE_TYPE_1,           // �ɱ�
	UPGRADE_TYPE_2,           // ��ͯ
	UPGRADE_TYPE_3,           // �鹭
	UPGRADE_TYPE_4,           // ����
	UPGRADE_TYPE_5,           // β��
	UPGRADE_TYPE_6,           // �ֻ�
	UPGRADE_TYPE_7,           // β��
	UPGRADE_TYPE_8,           // �ɳ�

	UPGRADE_TYPE_MAX,
};

static const int UPGRADE_MAX_GRADE = 30;									// ������
static const int UPGRADE_MAX_IMAGE_BYTE = 32;								// �����洢����ID�����ֽ���
static const int UPGRADE_IMAGE_MAX_COUNT = UPGRADE_MAX_IMAGE_BYTE * 8;		// �������ID���� id��Χ[1-255]
static const int UPGRADE_MAX_LEVEL = 1000;									// ���ȼ�
static const int UPGRADE_MAX_IMAGE_GRADE = 150;								// ����������ȼ�
static const int UPGRADE_EQUIP_COUNT = 4;									// װ������
static const int UPGRADE_EQUIP_MAX_LEVEL = 150;								// װ�����ȼ�
static const int UPGRADE_SKILL_COUNT = 4;									// ��������
static const int UPGRADE_SKILL_MAX_LEVEL = 100;								// �������ȼ�
static const int NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;							// ���ף��ֵʱ��



struct UpgradeBaseData
{

	UpgradeBaseData()
	{
		this->Reset();
	}

	enum FIGHTOUTFLAG
	{
		FIGHT_OUT_FLAG_ON = 1,
	};

	void Reset()
	{
		fight_out_flag = 0;
		reser_sh = 0;
		level = 0;
		star_level = 0;
		grade = 0;
		used_imageid = 0;
		grade_bless_val = 0;

		temporary_imageid = 0;
		temporary_imageid_has_select = 0;

		equip_skill_level = 0;

		capability = 0;
		
		clear_upgrade_time = 0;
		last_upgrade_time = 0;
		temporary_imageid_invalid_time = 0;
		skill0_last_colddown_time = 0;

		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
		memset(equip_level_list, 0, sizeof(equip_level_list));
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(active_img_flag, 0, sizeof(active_img_flag));
		memset(img_grade_list, 0, sizeof(img_grade_list));
	}

	bool IsSysActive() { return grade > 0; }

	bool IsImageActive(int image_id) 
	{
		if (image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
		{
			return false;
		}

		return BitmapFunction::IsBitSet(active_img_flag, sizeof(active_img_flag), image_id);
	}

	void ActiveImage(int image_id)
	{
		if (image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
		{
			return;
		}

		BitmapFunction::SetBit(active_img_flag, sizeof(active_img_flag), image_id);
	}

	bool UnActiveImage(int image_id)
	{
		if (image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
		{
			return false;
		}

		return BitmapFunction::ClearBit(active_img_flag, sizeof(active_img_flag), image_id);
	}

	bool IsFightOut()
	{
		return 0 != (fight_out_flag & FIGHT_OUT_FLAG_ON);
	}

	void OnFightOut()
	{
		fight_out_flag |= FIGHT_OUT_FLAG_ON;
	}

	void OffFightOut()
	{
		fight_out_flag &= ~FIGHT_OUT_FLAG_ON;
	}

	short fight_out_flag;									// ��ս���
	short reser_sh;											// 
	short level;											// �ȼ�
	short star_level;										// �Ǽ�
	short grade;											// ��
	short used_imageid;										// ʹ�õ�����
	int grade_bless_val;									// ����ף��ֵ

	short temporary_imageid;								// ��ǰʹ����ʱ����
	short temporary_imageid_has_select;						// ��ѡ������ʱ����

	int equip_skill_level;									// װ�����ܵȼ�

	int capability;											// ս����

	UInt32 clear_upgrade_time;								// ���ף��ֵ��ʱ��
	UInt32 last_upgrade_time;								//��һ�ν��׳ɹ�ʱ��
	UInt32 temporary_imageid_invalid_time;					// ��ʱ������Чʱ��
	UInt64 skill0_last_colddown_time;						// ����0�����ȴʱ��

	UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];			// ���Ե������б�
	short equip_level_list[UPGRADE_EQUIP_COUNT];			// װ����Ϣ
	short skill_level_list[UPGRADE_SKILL_COUNT];			// ���ܵȼ�
	UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];			// ���󼤻���
	UInt8 img_grade_list[UPGRADE_IMAGE_MAX_COUNT];			// ��������б�
};

struct UpgradeManagerParam
{
	UpgradeManagerParam() {}

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	UpgradeBaseData data_list[UPGRADE_TYPE_MAX];
};

#pragma pack(pop)

typedef char UpgradeManagerParamHex[sizeof(UpgradeManagerParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(UpgradeManagerParamHex) < 15360);

#endif



