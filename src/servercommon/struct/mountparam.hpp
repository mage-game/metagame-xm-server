#ifndef __MOUNTPARAM_HPP__
#define __MOUNTPARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"
#include "gameworld/gameworld/config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

struct MountParam
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
	short level;											// 等级
	short grade;											// 阶
	short old_grade_bless_val;								// 进阶祝福值(废弃)
	short used_imageid;										// 使用的形象
	unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量列表
	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];	// 激活的形象列表
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];		// 特殊形象
	unsigned int clear_upgrade_time;						// 清空祝福值的时间

	short equip_level_list[EQUIP_COUNT];					// 装备信息
	short skill_level_list[SKILL_COUNT];					// 技能等级

	short temporary_imageid;								// 当前使用临时形象
	short temporary_imageid_has_select;						// 已选定的临时形象
	unsigned int temporary_imageid_invalid_time;			// 临时形象有效时间
	unsigned long long skill0_last_colddown_time;			// 技能0最后冷却时间

	short star_level;										// 星阶
	short reserve_1;
	int grade_bless_val;									// 进阶祝福值
	int equip_skill_level;									// 装备技能等级

	int capability;											// 战斗力

	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// 特殊形象阶数
	unsigned int last_upgrade_time;                         //上一次进阶成功时间
};

#pragma pack(pop)

typedef char MountParamHex[sizeof(MountParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MountParamHex) < 768);

#endif



