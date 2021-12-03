#ifndef __SHENZHOU_WEAPON_DEF_H__
#define __SHENZHOU_WEAPON_DEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/other/chestshop/chestshop.hpp"

static const int SHENZHOU_WEAPON_MAX_SAVE_COUNT = 24;				// ���������洢����(����ռ�ô洢�ռ�)
static const int SHENZHOU_WEAPON_COUNT = 11;						// ������������(�ֿ�������)
static const int SHENZHOU_WEAPON_SLOT_COUNT = 8;					// ��������������
static const int SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT = 4;			// ���������Э������
static const int SHENZHOU_ELEMET_MAX_TYPE = 4;						// ��������Ԫ���������
static const int SHENZHOU_HUNYIN_MAX_SLOT = 8;						// ���ݻ�ӡ��������
static const int SHENZHOU_HUNYIN_MAX_BOX_REWARD_COUNT = MODE_MAX_COUNT;	// �����õĽ����б�������
static const int SHENZHOU_XILIAN_MAX_STAR_COUNT = 10;				// ����ϴ�������������

UNSTD_STATIC_CHECK(SHENZHOU_WEAPON_COUNT <= SHENZHOU_WEAPON_MAX_SAVE_COUNT)

#pragma pack(push, 4)

struct ShenzhouHunyinSlot
{
	ShenzhouHunyinSlot() : lingshu_level(0), hunyin_id(0), is_bind(0), reserve1(0) {}

	int lingshu_level;
	ItemID hunyin_id;
	char is_bind;
	char reserve1;		//������ۿ����ı�־

	void Reset()
	{
		lingshu_level = 0;
		hunyin_id = 0;
		is_bind = 0;
		reserve1 = 0;
	}

	bool Invalid()
	{
		return hunyin_id <= 0;
	}
};

struct ShenzhouWeaponParam
{
	ShenzhouWeaponParam() { this->Reset(); }

	struct WeaponSkillData
	{
		WeaponSkillData(): perform_skill_last_time(0), skill_effect_time_ms(0), skill_stack_count(0) {}

		unsigned int perform_skill_last_time;		// �ϴ��ͷż��ܵ�ʱ��
		int skill_effect_time_ms;					// ���ܳ���ʱ��
		int skill_stack_count;						// ���ܵ�����
	};

	struct WeaponData
	{
		WeaponData():weapon_level(0), hunyin_suit_level(0)
		{
			memset(weapon_slot_level, 0, sizeof(weapon_slot_level));
			memset(element_level, 0, sizeof(element_level));
			memset(hunyin_slot, 0, sizeof(hunyin_slot));
		}

		int weapon_level;
		int hunyin_suit_level;
		short weapon_slot_level[SHENZHOU_WEAPON_SLOT_COUNT];
		short element_level[SHENZHOU_ELEMET_MAX_TYPE];
		ShenzhouHunyinSlot hunyin_slot[SHENZHOU_HUNYIN_MAX_SLOT];
	};

	struct XilianData
	{
		XilianData() :xilian_slot_open_falg(0)
		{
			this->Reset();
		}

		void Reset()
		{
			xilian_slot_open_falg = 0;
			memset(xilian_shuxing_type, 0, sizeof(xilian_shuxing_type));
			memset(xilian_shuxing_star, 0, sizeof(xilian_shuxing_star));
			memset(xilian_shuxing_value, 0, sizeof(xilian_shuxing_value));
		}

		int xilian_slot_open_falg;
		char xilian_shuxing_type[SHENZHOU_HUNYIN_MAX_SLOT];
		char xilian_shuxing_star[SHENZHOU_HUNYIN_MAX_SLOT];
		int xilian_shuxing_value[SHENZHOU_HUNYIN_MAX_SLOT];
	};

	void Reset()
	{
		today_gather_times = 0;
		today_buy_gather_times = 0;
		today_exchange_identify_exp_times = 0;
		identify_level = 0;
		identify_star_level = 0;
		identify_exp = 0;
		hunqi_jinghua = 0;
	
		memset(weapon_data, 0, sizeof(weapon_data));

		normal_item_num = 0;
		rare_item_num = 0;
		unique_item_num = 0;

		box_id = 0;
		today_open_box_free_num = 0;
		memset(box_help_uid_list, 0, sizeof(box_help_uid_list));
		last_free_open_box_time = 0;

		lingshu_exp = 0;

		day_free_xilian_times = 0;
		memset(xilian_data, 0, sizeof(xilian_data));

		memset(skill_data, 0, sizeof(skill_data));
	}

	char today_gather_times;												// ���ղɼ�����
	char today_buy_gather_times;											// ���չ���ɼ��ܴ���
	short today_exchange_identify_exp_times;								// ���նһ������������
	short identify_level;													// �����ȼ�
	short identify_star_level;												// �����Ǽ�
	int identify_exp;														// ��������
	int hunqi_jinghua;														// ��������

	WeaponData weapon_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];					// �����ȼ�

	int normal_item_num;													// ��ͨ��Ʒ��
	int rare_item_num;														// ϡ�е�����
	int unique_item_num;													// ����������

	int box_id;																// ����id
	int today_open_box_free_num;											// ����ʹ�õĿ�������Ѵ���
	int box_help_uid_list[SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT];				// ����Э������
	int today_help_box_num;													// ÿ��Э�����˱���Ĵ���
	unsigned int last_free_open_box_time;									// �����ѿ������ʱ��

	int lingshu_exp;														// ���ྭ��

	int day_free_xilian_times;												// ÿ�����ϴ������
	XilianData xilian_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];					// ϴ������

	WeaponSkillData skill_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];				// ��������
};

typedef char ShenzhouWeaponParamHex[sizeof(ShenzhouWeaponParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenzhouWeaponParamHex) < 8192);

#pragma pack(pop)

#endif

