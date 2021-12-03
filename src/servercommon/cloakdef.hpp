#ifndef __CLOAK_DEF_HPP__
#define __CLOAK_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)


struct CloakParam
{
	static const int MAX_LEVEL = 1000;						// 最大等级
	static const int MAX_UP_LEVEL_ITEM_COUNT = 10;			// 升级最大数量
	static const int MAX_IMAGE_COUNT = 128;					// 最大形象ID
	static const int MAX_SPECIAL_IMAGE_ID = 63;				// 最大特殊形象ID
	static const int MAX_UPGRADE_LIMIT = 150;				// 特殊形象进阶最大等级

	static const int EQUIP_COUNT = 4;						// 装备数量
	static const int EQUIP_MAX_LEVEL = 100;					// 装备最大等级
	static const int SKILL_COUNT = 4;						// 技能数量
	static const int SKILL_MAX_LEVEL = 100;					// 技能最大等级
	static const int SHUXINGDAN_MAX_TYPE = 2;				// 属性丹类型最大数

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

	int level;												// 等级
	int cur_exp;											// 当前经验值
	int used_imageid;										// 使用的形象
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量列表
	int reserve;									
	unsigned long long active_special_image_flag;			// 特殊形象

	short equip_level_list[EQUIP_COUNT];					// 装备信息
	short skill_level_list[SKILL_COUNT];					// 技能等级

	unsigned long long skill_last_colddown_time[SKILL_COUNT];	// 技能冷却时间列表

	int equip_skill_level;									// 装备技能等级

	int capability;											// 战斗力

	char special_img_grade_list[MAX_SPECIAL_IMAGE_ID + 1];	// 特殊形象阶数
};

#pragma pack(pop)

typedef char CloakParamHex[sizeof(CloakParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CloakParamHex) < 512);

#endif  // __FOOTPRINT_DEF_HPP__

