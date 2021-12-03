#ifndef __HALO_DEF_HPP__
#define __HALO_DEF_HPP__

#include "servercommon.h"
#include "bitmapfunction.hpp"

#pragma pack(push, 4)


struct HaloParam
{
	static const int MAX_GRADE = 30;						// 最大阶数
	static const int MAX_IMAGE_COUNT = 128;					// 最大形象ID
	static const int MAX_STAR_LEVEL = MAX_IMAGE_COUNT * 10;	// 最大星级
	static const int MAX_UP_STAR_ITEM_COUNT = 10;			// 升星物品最大数量
	static const int MAX_LEVEL = 1000;						// 最大等级
	static const int MAX_SPECIAL_IMAGE_COUNT = 128;			// 最大特殊形象ID
	static const int MAX_UPGRADE_LIMIT = 150;				// 特殊形象进阶最大等级
	static const int EQUIP_COUNT = 4;						// 装备数量
	static const int EQUIP_MAX_LEVEL = 151;					// 装备最大等级
	static const int SKILL_COUNT = 4;						// 技能数量
	static const int SKILL_MAX_LEVEL = 100;					// 技能最大等级
	static const int MAX_ACTIVE_IMAGE_BYTE = MAX_IMAGE_COUNT / BIT_COUNT_OF_BYTE;			// 形象最大字节,现在最大支持128个形象
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

	short level;											// 等级
	short grade;											// 阶
	short old_grade_bless_val;								// 进阶祝福值
	short used_imageid;										// 使用的形象
	short shuxingdan_count;									// 属性丹数量
	short chengzhangdan_count;								// 成长丹数量

	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];	// 激活的形象列表
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
	unsigned int clear_upgrade_time;						// 清空祝福值的时间
	unsigned int reward_limit_time;							// 返还进阶丹的时间

	short equip_level_list[EQUIP_COUNT];					// 装备信息
	short skill_level_list[SKILL_COUNT];					// 技能等级

	unsigned long long skill_last_colddown_time[SKILL_COUNT];	// 技能冷却时间列表
	short star_level;										// 星级
	short reserved;
	int grade_bless_val;									// 进阶祝福值
	int equip_skill_level;									// 装备技能等级

	int capability;											// 战斗力

	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// 特殊形象阶数
};

#pragma pack(pop)

typedef char HaloParamHex[sizeof(HaloParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HaloParamHex) < 768);

#endif  // __HALO_DEF_HPP__

