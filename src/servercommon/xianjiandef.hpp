#ifndef __XIANJIAN_DEF_HPP__
#define __XIANJIAN_DEF_HPP__

#include "servercommon.h"

static const int MAX_XIANJIAN_COUNT = 8;
static const int MAX_XIANJIAN_LEVEL = 100;
static const int MAX_XIANJIAN_SOUL_COUNT = 14;
static const int MAX_XIANJIAN_SOUL_SKILL_SLOT_COUNT = 8;
static const int XIANJIAN_SOUL_HOLE_COUNT = 4;
static const int MAX_XIANJIAN_SOUL_LEVEL = 20;
static const int JIANXIN_SLOT_PER_XIANJIAN = 7;

static const int XIANJIAN_SOUL_ACTIVE_ALL_HOLE_FLAG = 0x0F;
UNSTD_STATIC_CHECK(4 == XIANJIAN_SOUL_HOLE_COUNT);

static const int MAX_XIANJIAN_SHUXINGDAN_TYPE = 3;

enum SOUL_SKILL
{
	SOUL_SKILL_PERCENT_XIXUE = 0,
	SOUL_SKILL_REGULAR_XIXUE = 1,
	SOUL_SKILL_HURT_ROLE_INC_PERCENT = 2,
	SOUL_SKILL_HURT_MONSTER_INC_PERCENT = 3,
	SOUL_SKILL_XUANXUN = 4,
	SOUL_SKILL_HURT_INC_PERCENT_ONE_TIMES = 5,
	SOUL_SKILL_WUDI = 6,
	SOUL_SKILL_ROLE_HURT_DEC_PERCENT = 7,
	SOUL_SKILL_MONSTER_HURT_DEC_PERCENT = 8,
	SOUL_SKILL_JIANSHU = 9,
	SOUL_SKILL_GATHER_SPEED_UP = 10,
	SOUL_SKILL_HURT_BUILDING_INC_PERCENT = 11,
	SOUL_SKILL_JIANFANG = 12,
	SOUL_SKILL_GATHER_NO_INTERRUPT = 13,

	SOUL_SKILL_MAX, 
};

#pragma pack(push, 4)

struct JianxinSlot
{
	JianxinSlot() { this->Reset(); }
	
	void Reset() { level = 0; is_lock = 0; reserve_ch = 0; curr_attr_value = 0; replace_attr_value = 0; reserve_int = 0; }

	short level;
	char is_lock;
	char reserve_ch;

	short curr_attr_value;
	short replace_attr_value;

	int reserve_int;
};

struct XianjianItem
{
	XianjianItem() { this->Reset(); }
	
	void Reset() 
	{ 
		xj_level = 0; 
		reserve_sh = 0; 
		next_uplevel_timestamp = 0; 

		for (int i = 0; i < JIANXIN_SLOT_PER_XIANJIAN; ++ i)
		{
			jianxin_slot_list[i].Reset();
		}
	}

	bool IsActive() { return xj_level > 0; }

	int GetJianxinLockCount() 
	{
		int lock_count = 0;
		for (int i = 0; i < JIANXIN_SLOT_PER_XIANJIAN; ++ i)
		{
			if (0 != jianxin_slot_list[i].is_lock)
			{
				++ lock_count;
			}
		}

		return lock_count;
	}

	int GetJianxinActiveCount()
	{
		int active_count = 0;
		for (int i = 0; i < JIANXIN_SLOT_PER_XIANJIAN; ++ i)
		{
			if (jianxin_slot_list[i].level > 0)
			{
				++ active_count;
			}
		}

		return active_count;
	}
	
	short xj_level;
	short reserve_sh;
	unsigned int next_uplevel_timestamp;

	JianxinSlot jianxin_slot_list[JIANXIN_SLOT_PER_XIANJIAN];
};

struct XianjianSoulItem
{
	XianjianSoulItem() { this->Reset(); }

	void Reset() { hole_active_state = 0; reserve_ch = 0; level = 0; cur_exp = 0; skill_last_perform_timestamp = 0; }

	bool IsHoleActive(int hole_idx)
	{
		if (hole_idx >= 0 && hole_idx < XIANJIAN_SOUL_HOLE_COUNT)
		{
			return 0 != (hole_active_state & (1 << hole_idx));
		}

		return false;
	}

	bool IsAllHoleActive() 
	{
		return XIANJIAN_SOUL_ACTIVE_ALL_HOLE_FLAG == (hole_active_state & XIANJIAN_SOUL_ACTIVE_ALL_HOLE_FLAG);
	}

	char hole_active_state;
	char reserve_ch;
	short level;
	int cur_exp;

	unsigned int skill_last_perform_timestamp;
};

struct XianJianJieItem
{
	XianJianJieItem() { this->Reset(); }

	void Reset() 
	{
		level = 0;
		reserve_sh = 0;
		bless_val = 0;
	}

	short level;
	short reserve_sh;
	int bless_val;
};

struct XianJianParam
{
	XianJianParam() { this->Reset(); }

	void Reset()
	{
		select_xj_id = -1;
		reserve_1 = 0;
		reserve_3 = 0;
		first_active_real_online_time_s = 0;

		for (int i = 0; i < MAX_XIANJIAN_COUNT; ++i)
		{
			xj_list[i].Reset();
			jie_item_list[i].Reset();
		}

		for (int i = 0; i < MAX_XIANJIAN_SOUL_SKILL_SLOT_COUNT; ++ i)
		{
			active_skill_idlist[i] = -1;
		}

		for (int i = 0; i < MAX_XIANJIAN_SOUL_COUNT; ++ i)
		{
			soul_list[i].Reset();
		}

		memset(reserve_ll_list, 0, sizeof(reserve_ll_list));
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	char select_xj_id;
	char reserve_1;
	short reserve_3;
	int first_active_real_online_time_s;
	XianjianItem xj_list[MAX_XIANJIAN_COUNT];
	long long reserve_ll_list[8];										// ÒÔ·ÀÍØÕ¹

	short active_skill_idlist[MAX_XIANJIAN_SOUL_SKILL_SLOT_COUNT];
	XianjianSoulItem soul_list[MAX_XIANJIAN_SOUL_COUNT];

	int shuxingdan_list[MAX_XIANJIAN_SHUXINGDAN_TYPE];
	XianJianJieItem jie_item_list[MAX_XIANJIAN_COUNT];
};

typedef char XianJianParamHex[sizeof(XianJianParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianJianParamHex) < 2560);

#pragma pack(pop)

#endif  // __XIANJIAN_DEF_HPP__

