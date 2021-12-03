#ifndef __FABAO_PARAM_HPP__
#define __FABAO_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push) 
#pragma pack(4)

struct FabaoParam
{
	static const int MAX_GRADE = 30;						// 最大阶数
	static const int MAX_IMAGE_COUNT = 128;						// 最大形象ID
	static const int MAX_SPECIAL_IMAGE_COUNT = 128;				// 最大特殊形象ID
	static const int MAX_UPGRADE_LIMIT = 127;				// 特殊形象进阶最大等级
	static const int SKILL_COUNT = 4;						// 技能数量
	static const int SKILL_MAX_LEVEL = 100;					// 技能最大等级
	static const int EQUIP_COUNT = 4;						// 装备数量
	static const int EQUIP_MAX_LEVEL = 151;					// 装备最大等级
	static const int DISTINGUISH_IMG_SPECIALIMG_NUM = 1000; // 区分形象和特殊形象差值
	static const int MAX_ACTIVE_IMAGE_BYTE = MAX_IMAGE_COUNT / BIT_COUNT_OF_BYTE;			// 形象最大字节,现在最大支持128个形象
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

	short grade;									        // 阶数
	short used_imageid;									    // 使用的形象
	short used_special_id;                                  // 使用的特殊形象
	short shuxingdan_count;							        // 属性丹数量

	unsigned char active_image_flag[MAX_ACTIVE_IMAGE_BYTE];									// 激活的形象
	unsigned char active_special_image_flag[MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 激活的特殊形象
	unsigned int clear_bless_time;						    // 清空祝福值的时间
	int grade_bless_val;									// 进阶祝福值
	char special_img_grade_list[MAX_SPECIAL_IMAGE_COUNT];	// 特殊形象阶数列表
	unsigned int last_upgrade_succ_time;					// 上一次进阶成功的时间
	short skill_level_list[SKILL_COUNT];					// 技能等级列表，下标为索引
	short equip_level_list[EQUIP_COUNT];					// 装备等级信息,下标为装备索引，值为等级
	int equip_skill_level;									// 装备技能等级
	int capability;											// 战斗力
	int is_used_special_imaged;                             //是否使用幻化形象
};

#pragma pack(pop)

typedef char FabaoParamHex[sizeof(FabaoParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FabaoParamHex) < 512)

#endif