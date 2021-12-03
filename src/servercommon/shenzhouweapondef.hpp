#ifndef __SHENZHOU_WEAPON_DEF_H__
#define __SHENZHOU_WEAPON_DEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/other/chestshop/chestshop.hpp"

static const int SHENZHOU_WEAPON_MAX_SAVE_COUNT = 24;				// 神州六器存储数量(数据占用存储空间)
static const int SHENZHOU_WEAPON_COUNT = 11;						// 神州六器数量(现开启个数)
static const int SHENZHOU_WEAPON_SLOT_COUNT = 8;					// 神州六器槽数量
static const int SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT = 4;			// 神州六最大协助个数
static const int SHENZHOU_ELEMET_MAX_TYPE = 4;						// 神州炼魂元素最大类型
static const int SHENZHOU_HUNYIN_MAX_SLOT = 8;						// 神州魂印槽最大个数
static const int SHENZHOU_HUNYIN_MAX_BOX_REWARD_COUNT = MODE_MAX_COUNT;	// 宝箱获得的奖励列表最大个数
static const int SHENZHOU_XILIAN_MAX_STAR_COUNT = 10;				// 神州洗练属性最大星数

UNSTD_STATIC_CHECK(SHENZHOU_WEAPON_COUNT <= SHENZHOU_WEAPON_MAX_SAVE_COUNT)

#pragma pack(push, 4)

struct ShenzhouHunyinSlot
{
	ShenzhouHunyinSlot() : lingshu_level(0), hunyin_id(0), is_bind(0), reserve1(0) {}

	int lingshu_level;
	ItemID hunyin_id;
	char is_bind;
	char reserve1;		//用作魂槽开启的标志

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

		unsigned int perform_skill_last_time;		// 上次释放技能的时间
		int skill_effect_time_ms;					// 技能持续时间
		int skill_stack_count;						// 技能叠加数
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

	char today_gather_times;												// 今日采集总数
	char today_buy_gather_times;											// 今日购买采集总次数
	short today_exchange_identify_exp_times;								// 今日兑换鉴定经验次数
	short identify_level;													// 鉴定等级
	short identify_star_level;												// 鉴定星级
	int identify_exp;														// 鉴定经验
	int hunqi_jinghua;														// 魂器精华

	WeaponData weapon_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];					// 魂器等级

	int normal_item_num;													// 普通物品数
	int rare_item_num;														// 稀有道具数
	int unique_item_num;													// 绝世道具数

	int box_id;																// 宝箱id
	int today_open_box_free_num;											// 今天使用的开宝箱免费次数
	int box_help_uid_list[SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT];				// 宝箱协助人数
	int today_help_box_num;													// 每天协助别人宝箱的次数
	unsigned int last_free_open_box_time;									// 最后免费开宝箱的时间

	int lingshu_exp;														// 灵枢经验

	int day_free_xilian_times;												// 每日免费洗练次数
	XilianData xilian_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];					// 洗炼数据

	WeaponSkillData skill_data[SHENZHOU_WEAPON_MAX_SAVE_COUNT];				// 技能数据
};

typedef char ShenzhouWeaponParamHex[sizeof(ShenzhouWeaponParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenzhouWeaponParamHex) < 8192);

#pragma pack(pop)

#endif

