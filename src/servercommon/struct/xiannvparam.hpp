#ifndef __XIANNV_PARAM_HPP__
#define __XIANNV_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push) 
#pragma pack(4)

static const int MAX_XIANNV_COUNT = 7;
static const int MAX_XIANNV_POS = 4;
static const int MAX_XIANNV_HUANHUA_COUNT = 128;						// 最大仙女幻化数量
static const int MAX_XIANNV_UPGRADE_STUFF_EXP = 100000;					// 单个材料升阶的最大经验

const static int XIANNV_SHENGWU_MAX_ID = 3;								// 圣物最大ID
const static int XIANNV_SHENGWU_CHOU_COMSUEM_MAX_COUNT= 20;				// 圣物抽经验消耗最大行数
const static int XIANNV_SHENGWU_GONGMING_MAX_GRID_ID = 28;				// 圣物共鸣格子最大ID
const static int XIANNV_SHENGWU_MILING_TYPE_COUNT = 5;					// 灵液觅灵类型个数
const static int XIANNV_SHENGWU_MILING_FONT_COUNT = 6;					// 灵液觅灵结果数字个数
const static int XIANNV_SHENGWU_CHOU_EXP_COUNT = 6;						// 抽经验（回忆）抽出来的经验碎片个数
static const int MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE = MAX_XIANNV_HUANHUA_COUNT / BIT_COUNT_OF_BYTE; //特殊形象最大字节

enum XiannvShengwuSkillType
{
	XIANNV_SHENGWU_SKILL_TYPE_1 = 1,									// 1）技能直接增加万分之Y点当前圣器基础属性
	XIANNV_SHENGWU_SKILL_TYPE_2,										// 2）技能直接增加Y点所有圣器基础属性
	XIANNV_SHENGWU_SKILL_TYPE_3,										// 3）技能增幅全仙女属性万分之Y
	XIANNV_SHENGWU_SKILL_TYPE_4,										// 4）受到伤害时X%概率触发，生成当前生命值万分之Y的护盾，持续Z秒，冷却A秒

	XIANNV_SHENGWU_SKILL_TYPE_COUNT
};

struct XiannvItem
{
	XiannvItem() { this->Reset(); }

	void Reset()
	{
		xn_level = 0;
		xn_zizhi = 0;
	}

	short xn_level;
	short xn_zizhi;
};

struct NvshenShengwuItem
{
	NvshenShengwuItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		reserve_sh = 0;
		exp = 0;
	}

	short level;
	short reserve_sh;
	int exp;
};

struct XiannvParam
{
	bool IsXnActived()
	{
		return active_xiannv_flag != 0;
	}

	bool IsXiannvActive(int xiannv_id) 
	{
		return 0 != (active_xiannv_flag & (1 << xiannv_id));
	}

	void ActiveXiannv(int xiannv_id) 
	{
		active_xiannv_flag |= (1 << xiannv_id); 
	}

	bool IsHuanHuaActive(int huanhua_id) 
	{
		return BitmapFunction::IsBitSet(active_huanhua_flag, sizeof(active_huanhua_flag), huanhua_id);
	}

	void ActiveHuanHua(int huanhua_id) 
	{
		BitmapFunction::SetBit(active_huanhua_flag, sizeof(active_huanhua_flag), huanhua_id);
	}

	bool IsShengwuActive(int shengwu_id)
	{
		if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return false;
		}
		return 0 != all_shengwu_list[shengwu_id].level;
	}

	void ActiveShengwu(int shengwu_id)
	{
		if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return;
		}
		if (0 != all_shengwu_list[shengwu_id].level)
		{
			return;
		}
		all_shengwu_list[shengwu_id].level = 1;
	}

	void Reset()
	{
		active_xiannv_flag = 0;
		memset(active_huanhua_flag, 0, sizeof(active_huanhua_flag));
		huanhua_id = -1;
		reserved = 0;
		memset(xiannv_name, 0, sizeof(xiannv_name));
		reserve_1 = 0;
		reserve_2 = 0;

		for (int i = 0; i < MAX_XIANNV_HUANHUA_COUNT; ++i)
		{
			xiannv_huanhua_level[i] = 0;			
		}

		for (int i = 0; i < MAX_XIANNV_COUNT; ++i)
		{
			xn_item_list[i].Reset();
		}

		memset(pos_list, -1, sizeof(pos_list));

		shengwu_lingye = 0;
		cur_gold_miling_times = 0;
		shengwu_chou_id = -1;
		memset(shengwu_chou_exp_list, 0, sizeof(shengwu_chou_exp_list));
		upgrade_shengwu_id = -1;
		memset(upgrade_exp_list, 0, sizeof(upgrade_exp_list));
		memset(all_shengwu_list, 0, sizeof(all_shengwu_list));
		memset(grid_level_list, 0, sizeof(grid_level_list));
		day_free_miling_times = 0;
		day_fetch_ling_time = 0;
		for (int i = 0; i < XIANNV_SHENGWU_MILING_FONT_COUNT; ++i)
		{
			miling_list[i] = -1; // 初始化成无效类型
		}
		reserve_sh = 0;
		shengwu_essence = 0;

		capability = 0;
	}

	int active_xiannv_flag;							// 激活的仙女列表
	unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];							// 激活的幻化列表
	short huanhua_id;									// 幻化id
	short reserved;
	GameName xiannv_name[MAX_XIANNV_COUNT];				// 名字
	int reserve_1;										// 保留1
	int reserve_2;										// 保留2
	int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];	// 仙女幻化等级

	XiannvItem xn_item_list[MAX_XIANNV_COUNT];
	char pos_list[MAX_XIANNV_POS];						// 仙女阵形位置 0为主战位

	int shengwu_lingye;									// 圣物灵液
	short cur_gold_miling_times;						// 当前次元宝觅灵次数（领取奖励后清零）
	short shengwu_chou_id;								// 未领取的抽奖exp所属圣物id
	short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// 未领取的抽奖exp奖励
	int upgrade_shengwu_id;							// 未领取的升阶exp所属圣物id
	int upgrade_exp_list[XIANNV_SHENGWU_MAX_ID + 1];	// 未领取的升阶exp奖励
	NvshenShengwuItem all_shengwu_list[XIANNV_SHENGWU_MAX_ID + 1];

	short grid_level_list[XIANNV_SHENGWU_GONGMING_MAX_GRID_ID + 1];
	char day_free_miling_times;							// 当天免费觅灵次数
	char day_fetch_ling_time;							// 当天获取灵液次数

	char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];	// 当前抽到的灵类型列表
	short reserve_sh;
	int shengwu_essence;								// 已有的仙器精华
	int capability;										// 战斗力
};

#pragma pack(pop)

typedef char XiannvParamHex[sizeof(XiannvParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XiannvParamHex) < 2048);

#endif // __XIANNV_PARAM_HPP__
