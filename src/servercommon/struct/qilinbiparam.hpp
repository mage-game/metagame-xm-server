#ifndef __QILINBI_PARAM_HPP__
#define __QILINBI_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"
#include "gameworld/gameworld/config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

enum QILINBIFLAG
{
	QILINBIFLAG_ON = 1,
};

struct QiLinBiParam
{
	static const int MAX_GRADE = 30;						// ������
	static const int MAX_IMAGE_COUNT = 128;					// �������ID
	static const int MAX_STAR_LEVEL = MAX_IMAGE_COUNT * 10;	// ����Ǽ�
	static const int MAX_UP_STAR_ITEM_COUNT = 10;			// ������Ʒ�������
	static const int MAX_LEVEL = 1000;						// ���ȼ�
	static const int MAX_SPECIAL_IMAGE_COUNT = 128;			// �����������ID
	static const int MAX_UPGRADE_LIMIT = 150;				// ��������������ȼ�
	static const int EQUIP_COUNT = 4;						// װ������
	static const int EQUIP_MAX_LEVEL = 100;					// װ�����ȼ�
	static const int SKILL_COUNT = 4;						// ��������
	static const int SKILL_MAX_LEVEL = 100;					// �������ȼ�
	static const int MAX_ACTIVE_IMAGE_BYTE = MAX_IMAGE_COUNT / BIT_COUNT_OF_BYTE;			// ��������ֽ�,�������֧��128������
	static const int MAX_ACTIVE_SPECIAL_IMAGE_BYTE = MAX_SPECIAL_IMAGE_COUNT / BIT_COUNT_OF_BYTE;

	enum QILINGBI_SKILL_INDEX
	{
		QILINGBI_SKILL_INDEX_0,     // ���ܵ����˺���Ѫ����[param_a]%��������ʱ����������[param_b]������(param_c)%���룬��ȴʱ��[param_d]����
		QILINGBI_SKILL_INDEX_1,     // ����������ɫ[fangyu]�����
		QILINGBI_SKILL_INDEX_2,     // ����������ɫ[maxhp]������
		QILINGBI_SKILL_INDEX_3,     // ����������ɫ[gongji]�㹥��

		QILINGBI_SKILL_INDEX_MAX,
	};

	UNSTD_STATIC_CHECK(QILINGBI_SKILL_INDEX_MAX <= SKILL_COUNT);

	QiLinBiParam()
	{
		this->Reset();
	}

	void Reset()
	{
		qilinbi_flag = 0;
		reserve = 0;
		level = 0;
		grade = 0;
		used_imageid = 0;
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
		memset(active_image_flag, 0, sizeof(active_image_flag));
		memset(active_special_image_flag, 0, sizeof(active_special_image_flag));
		clear_upgrade_timestamp = 0;

		memset(equip_level_list, 0, sizeof(equip_level_list));
		memset(skill_level_list, 0, sizeof(skill_level_list));
		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));

		temporary_imageid = 0;
		temporary_imageid_has_select = 0;
		temporary_imageid_invalid_time = 0;

		skill0_last_colddown_time = 0;

		star_level = 0;
		grade_bless_val = 0;
		equip_skill_level = 0;
		capability = 0;
	}

	bool IsQiLinBiActived()
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

	bool IsOnQiLinBi()
	{
		return 0 != (qilinbi_flag & QILINBIFLAG_ON);
	}

	void OnQiLinBi()
	{
		qilinbi_flag |= QILINBIFLAG_ON;
	}

	void OffQiLinBi()
	{
		qilinbi_flag &= ~QILINBIFLAG_ON;
	}

	short qilinbi_flag;
	short level;											// �ȼ�
	short grade;											// ��
	short star_level;										// �ǽ�
	short used_imageid;										// ʹ�õ�����
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե������б�
	short reserve;
	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];	// ����������б�
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
	unsigned int clear_upgrade_timestamp;					// ���ף��ֵ��ʱ��

	short skill_level_list[SKILL_COUNT];					// ���ܵȼ�

	short temporary_imageid;								// ��ǰʹ����ʱ����
	short temporary_imageid_has_select;						// ��ѡ������ʱ����
	unsigned int temporary_imageid_invalid_time;			// ��ʱ������Чʱ��
	unsigned long long skill0_last_colddown_time;			// ����0�����ȴʱ��

	int grade_bless_val;									// ����ף��ֵ
	int equip_skill_level;									// װ�����ܵȼ�

	int capability;											// ս����

	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// �����������

	short equip_level_list[EQUIP_COUNT];					// װ����Ϣ
};

#pragma pack(pop)

typedef char QiLinBiParamHex[sizeof(QiLinBiParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(QiLinBiParamHex) < 512);

#endif  // __QILINBI_PARAM_HPP__

