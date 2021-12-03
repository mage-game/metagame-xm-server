#ifndef __SHIZHUANG_PARAM_HPP__
#define __SHIZHUANG_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"

enum SHIZHUANG_TYPE
{
	SHIZHUANG_TYPE_INVALID = -1,

	SHIZHUANG_TYPE_WUQI = 0,
	SHIZHUANG_TYPE_BODY,
// 	SHIZHUANG_TYPE_FOOT,
// 	SHIZHUANG_TYPE_GUANGHUAN,

	SHIZHUANG_TYPE_MAX,
};

const static int SHIZHUANG_MAX_IMG_COUNT = 128; 
const static int SHIZHUANG_CROSS_RANK_REWARD_INDEX = 30;

const static int SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT = 128; // 时装最大特殊形象ID

static const int SHIZHUANG_MAX_ACTIVE_IMAGE_BYTE = SHIZHUANG_MAX_IMG_COUNT / BIT_COUNT_OF_BYTE;			// 形象最大字节,现在最大支持128个形象
static const int SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE = SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT / BIT_COUNT_OF_BYTE;

//
//static const int SZXY_SHENBIN_PART = 0;		// 幸运神兵部位
//static const int SZXY_SHENBIN_INDEX = 2;	// 幸运神兵时装编号
//static const int SZXY_FUSHI_PART = 1;		// 幸运服饰部位
//static const int SZXY_FUSHI_INDEX = 2;		// 幸运服饰时装编号
//const static int SHIZHUANG_MAX_LEVEL = 100;

enum SZ_ACT_SPECIAL_TYPE
{
	SZ_ACT_SPECIAL_TYPE_SHENBIN = 1,	// 至尊神兵
	SZ_ACT_SPECIAL_TYPE_FUSHI = 2,		// 至尊服饰
};

#pragma pack(push) 
#pragma pack(4)

struct ShizhuangItem
{
	static const int SKILL_COUNT = 4;       //技能数量
	static const int EQUIP_COUNT = 4;       //装备数量
	ShizhuangItem() { this->Reset(); }

	void Reset()
	{
		use_img = 0;
		use_special_img = 0;
		grade = 0;
		is_use_special_img = 0;
		grade_bless = 0;
		memset(img_active_flag, 0, sizeof(img_active_flag));
		memset(special_img_active_flag, 0, sizeof(special_img_active_flag));
		memset(special_img_grade_list, 0, sizeof(special_img_grade_list));
		memset(valid_timestamp_list, 0, sizeof(valid_timestamp_list));
		shuxingdan_count = 0;
		res_sh = 0;
		memset(skill_level_list,0,sizeof(skill_level_list));
		memset(equip_level_list,0,sizeof(equip_level_list));
		last_upgrade_succ_time = 0;
		equip_skill_level = 0;
		clear_bless_value_time = 0;
		capability = 0;
	}

	bool IsSpecialImageActive(int special_img_id)
	{
		return BitmapFunction::IsBitSet(special_img_active_flag, sizeof(special_img_active_flag), special_img_id);
	}

	char use_img;					// 当前使用的形象ID
	char use_special_img;			// 当前使用的特殊形象ID（这个优先级高）
	char grade;						// 阶数
	char is_use_special_img;        // 是否使用特殊形象(幻化形象),0不使用,1使用
	int grade_bless;				// 进阶祝福值
	unsigned char img_active_flag[SHIZHUANG_MAX_ACTIVE_IMAGE_BYTE];	// 形象激活标记
	unsigned char special_img_active_flag[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE]; // 特殊形象激活标记
	short special_img_grade_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
	unsigned int valid_timestamp_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];	// 特殊形象有效期（0代表无限）
	short shuxingdan_count;                                             // 属性丹数量
	short res_sh;  
	short skill_level_list[SKILL_COUNT];                                //技能等级
	short equip_level_list[EQUIP_COUNT];                                //装备等级
	unsigned int last_upgrade_succ_time;                                //上一次进阶成功的时间
	int equip_skill_level;                                              //装备技能等级
	unsigned int clear_bless_value_time;                                //清空祝福值的时间
	int capability;                                                    // 战斗力     
};

struct ShizhuangParam
{
	static const int MAX_GRADE = 30;						// 最大阶数
	static const int EQUIP_MAX_LEVEL = 151;					// 装备最大等级

	ShizhuangParam() { this->Reset(); }

	void Reset()
	{
		reserve_ll = 0;
		reserve_ll2 = 0;

		for (int i = 0; i < SHIZHUANG_TYPE_MAX; ++ i)
		{
			item_list[i].Reset();
		}
	}

	ShizhuangItem *GetShizhuangItem(int type)
	{
		if (type <= SHIZHUANG_TYPE_INVALID || type >= SHIZHUANG_TYPE_MAX)
		{
			return nullptr;
		}
		return &item_list[type];
	}

	long long reserve_ll;
	long long reserve_ll2;
	ShizhuangItem item_list[SHIZHUANG_TYPE_MAX];
	
};
#pragma pack(pop)

typedef char ShizhuangParamHex[sizeof(ShizhuangParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShizhuangParamHex) < 4096)


#endif // __SHIZHUANG_PARAM_HPP__
