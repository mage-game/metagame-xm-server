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

	UPGRADE_TYPE_0,           // 灵珠
	UPGRADE_TYPE_1,           // 仙宝
	UPGRADE_TYPE_2,           // 灵童
	UPGRADE_TYPE_3,           // 灵弓
	UPGRADE_TYPE_4,           // 灵骑
	UPGRADE_TYPE_5,           // 尾焰
	UPGRADE_TYPE_6,           // 手环
	UPGRADE_TYPE_7,           // 尾巴
	UPGRADE_TYPE_8,           // 飞宠

	UPGRADE_TYPE_MAX,
};

static const int UPGRADE_MAX_GRADE = 30;									// 最大阶数
static const int UPGRADE_MAX_IMAGE_BYTE = 32;								// 用来存储形象ID所需字节数
static const int UPGRADE_IMAGE_MAX_COUNT = UPGRADE_MAX_IMAGE_BYTE * 8;		// 最大形象ID数量 id范围[1-255]
static const int UPGRADE_MAX_LEVEL = 1000;									// 最大等级
static const int UPGRADE_MAX_IMAGE_GRADE = 150;								// 形象进阶最大等级
static const int UPGRADE_EQUIP_COUNT = 4;									// 装备数量
static const int UPGRADE_EQUIP_MAX_LEVEL = 150;								// 装备最大等级
static const int UPGRADE_SKILL_COUNT = 4;									// 技能数量
static const int UPGRADE_SKILL_MAX_LEVEL = 100;								// 技能最大等级
static const int NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;							// 清除祝福值时间



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

	short fight_out_flag;									// 出战标记
	short reser_sh;											// 
	short level;											// 等级
	short star_level;										// 星级
	short grade;											// 阶
	short used_imageid;										// 使用的形象
	int grade_bless_val;									// 进阶祝福值

	short temporary_imageid;								// 当前使用临时形象
	short temporary_imageid_has_select;						// 已选定的临时形象

	int equip_skill_level;									// 装备技能等级

	int capability;											// 战斗力

	UInt32 clear_upgrade_time;								// 清空祝福值的时间
	UInt32 last_upgrade_time;								//上一次进阶成功时间
	UInt32 temporary_imageid_invalid_time;					// 临时形象有效时间
	UInt64 skill0_last_colddown_time;						// 技能0最后冷却时间

	UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];			// 属性丹数量列表
	short equip_level_list[UPGRADE_EQUIP_COUNT];			// 装备信息
	short skill_level_list[UPGRADE_SKILL_COUNT];			// 技能等级
	UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];			// 形象激活标记
	UInt8 img_grade_list[UPGRADE_IMAGE_MAX_COUNT];			// 形象阶数列表
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



