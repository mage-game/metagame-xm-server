#ifndef __SHENGE_SYSTEM_DEF_HPP__
#define __SHENGE_SYSTEM_DEF_HPP__

#include "servercommon.h"

static const int SHENGE_SYSTEM_BAG_MAX_GRIDS = 250;									// 背包最大格子数量不可变 数据库
static const int SHENGE_SYSTEM_MAX_SHENGE_PAGE = 5;									// 最大神格页 不可变 数据库
static const int SHENGE_SYSTEM_CUR_SHENGE_PAGE = 3;									// 当期神格页数
UNSTD_STATIC_CHECK(SHENGE_SYSTEM_CUR_SHENGE_PAGE <= SHENGE_SYSTEM_MAX_SHENGE_PAGE);
static const int SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID = 16;							// 当前普通神格格子数 
static const int SHENGE_SYSTEM_MAX_SHENGE_GRID = 20;								// 普通神格最大格子数 不可变 数据库
UNSTD_STATIC_CHECK(SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID <= SHENGE_SYSTEM_MAX_SHENGE_GRID);

static const int SHENGE_SYSTEM_MAX_SHENGE_LEVEL = 400;								// 最大神格等级
static const int SHENGE_SYSTEM_PER_SHENGE_PAGE_MAX_ZHONGJI_SHENGE_COUNT = 4;		// 神格页终极神格最大个数
static const int SHENGE_SYSTEM_RECOVER_TIME_INTERVAL = 5;							// 回血间隔

static const int SHENGE_SYSTEM_QUALITY_MAX_VALUE = 4;								// 神格区域数最大值(掌控的格子数)
static const int SHENGE_SYSTEM_ZHANGKONG_RAND_EXP_COUNT = 3;						// 掌控可以随机的经验大小个数
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL = 200;							// 掌控最高等级
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_GRADE = 40;							// 掌控最高阶数
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_STAR = 5;								// 掌控最高星星数

static const int SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM = 3;
static const int SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM = 7;
static const int SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM = 10;
static const int SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT = 9;

enum SHENGE_QUALITY
{
	SHENGE_QUALITY_BLUE,															// 蓝色
	SHENGE_QUALITY_PURPLE,															// 紫色
	SHENGE_QUALITY_ORANGE,															// 橙色
	SHENGE_QUALITY_RED,																// 红色
	SHENGE_QUALITY_GOLD,															// 金色
	SHENGE_QUALITY_PING,															// 粉色

	SHENGE_QUALITY_COUNT															// 最大颜色阶级
};

enum SHENGE_TYPE
{
	SHENGE_TYPE_HP = 0,																//气血
	SHENGE_TYPE_GONGJI,																//攻击
	SHENGE_TYPE_FANGYU,																//防御
	SHENGE_TYPE_MINGZHONG,															//命中
	SHENGE_TYPE_SHANBI,																//闪避
	SHENGE_TYPE_ADD_SHANGHAI,														//增伤
	SHENGE_TYPE_DIKANG_SHANGHAI,													//减伤
	SHENGE_TYPE_SPEED,																//移速
	SHENGE_TYPE_LUCKY,																//幸运
	SHENGE_TYPE_GREEDY,																//贪婪
	SHENGE_TYPE_NORMAL_COUNT,														//普通神格类型

	SHENGE_TYPE_GONGFA = SHENGE_TYPE_NORMAL_COUNT,									//攻伐
	SHENGE_TYPE_JIANJIA,															//坚甲
	SHENGE_TYPE_JIXING,																//疾行
	SHENGE_TYPE_DONGCHA,															//洞察
	SHENGE_TYPE_HARVEST,															//丰收

	SHENGE_TYPE_COUNT																// 最大神格类型
};

enum SHENGE_SYSTEM_ATTR_TYPE
{
	SHENGE_SYSTEM_ATTR_TYPE_MAXHP,													// 最大血量
	SHENGE_SYSTEM_ATTR_TYPE_GONGJI,													// 攻击
	SHENGE_SYSTEM_ATTR_TYPE_FANGYU,													// 防御
	SHENGE_SYSTEM_ATTR_TYPE_ADD_SHANGHAI,											// 暴击
	SHENGE_SYSTEM_ATTR_TYPE_DIKANG_SHANGHAI,										// 抗暴
	SHENGE_SYSTEM_ATTR_TYPE_SPEED,													// 移速
	SHENGE_SYSTEM_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI,									// 命中
	SHENGE_SYSTEM_ATTR_TYPE_SHANBI,													// 闪避
	SHENGE_SYSTEM_ATTR_TYPE_ADD_ITEM_DROP,											// 增加野外掉落
	SHENGE_SYSTEM_ATTR_TYPE_ADD_EXP,												// 增加经验

	SHENGE_SYSTEM_ATTR_TYPE_COUNT,													// 神格属性最大类型
};

// 效果为百分比加成
enum SHENGE_SPECIAL_EFFECT_TYPE_PERCEN
{
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP = 0,							// 增加经验
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP,						// 增加野外掉落
	SHENGE_SPECIAL_EFFECT_TYPE_DIKANG_SHANGHAI,						// 被玩家攻击时减伤
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHANBI,							// 增加玩家闪避
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_DEC_FENGYU,						// 增加防御忽视(实际是增加伤害)
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_ROLE_DAMAGE,						// 增加对玩家伤害
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_BOSS_DAMAGE,						// 增加对boss伤害
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHENGE_ATTR,						// 增加神格属性
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER,							// 增加回血属性

	SHENGE_SPECIAL_EFFECT_TYPE_COUNT								// 特殊效果最大类型
};

enum SHENGE_SYSTEM_XILIAN_ATTR_TYPE
{
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_MAXHP,													// 最大血量
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_GONGJI,													// 攻击
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_FANGYU,													// 防御
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI,									// 命中
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_SHANBI,													// 闪避
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_ADD_SHANGHAI,											// 暴击
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_DIKANG_SHANGHAI,											// 抗暴

	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_COUNT,													// 神格神躯属性最大类型
};

#pragma pack(push, 4)

struct ShengeParamData
{
	ShengeParamData() { this->Reset(); }

	void Reset()
	{
		quality = -1;
		type = -1;
		level = 0;
	}

	char quality;				// 阶级
	char type;					// 类型
	short level;				// 等级

	bool IsEmpty() const { return level <= 0 || quality < 0 || type < 0; }
};

struct ShengeSinglePage
{
	ShengeSinglePage() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_MAX_SHENGE_GRID; ++i)
		{
			shenge_grid[i].Reset();
		}
	}

	UNSTD_STATIC_CHECK(20 == SHENGE_SYSTEM_MAX_SHENGE_GRID)
		ShengeParamData shenge_grid[SHENGE_SYSTEM_MAX_SHENGE_GRID];							// 神格格子
};

struct ZhangkongParam
{   
	ZhangkongParam() { this->Reset(); }

	void Reset()
	{
		level = 0;
		exp = 0;
	}

	int level;
	int exp;
};

// 神格神躯洗炼
//struct AttrInfo
//{
//	AttrInfo() { this->Reset(); }
//	void Reset()
//	{
//		qianghua_times = 0;
//		attr_point = -1;
//		attr_value = 0;
//		reserve_sh = 0;
//	}
//
//	short qianghua_times;
//	short attr_point;
//	short attr_value;
//	short reserve_sh;
//};

struct AttrInfo
{
	AttrInfo() { this->Reset(); }
	void Reset()
	{
		qianghua_times = 0;
		attr_point = -1;
		attr_value = 0;
	}

	short qianghua_times;
	short attr_point;
	int attr_value;
};

struct ShengeShenquAttrInfo
{
	ShengeShenquAttrInfo() { this->Reset(); }
	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ i)
		{
			attr_info[i].Reset();
		}
	}

	AttrInfo attr_info[SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM];
};

struct SingleShengeShenqu
{
	SingleShengeShenqu() { this->Reset(); }
	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++ i)
		{
			shenqu_attr_list[i].Reset();
		}
	}

	ShengeShenquAttrInfo shenqu_attr_list[SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM];
};

struct ShengeSystemParam
{
	ShengeSystemParam() { this->Reset(); }

	void Reset()
	{
		using_shenge_page_index = 0;
		free_choujiang_times = 0;
		is_can_get_fisrt_ten_chou = 0;
		reserve_ch = 0;
		next_free_chou_timestamp = 0;
		marrow_score = 0;

		for (int i = 0; i < SHENGE_SYSTEM_MAX_SHENGE_PAGE; ++i)
		{
			shenge_page_list[i].Reset();
		}

		for (int i = 0; i < SHENGE_SYSTEM_BAG_MAX_GRIDS; ++i)
		{
			virtual_bag_list[i].Reset();
		}

		for (int i = 0; i < SHENGE_SYSTEM_QUALITY_MAX_VALUE; ++i)
		{
			zhangkong_level_list[i].Reset();
		}

		memset(history_set_total_level, 0, sizeof(history_set_total_level));

		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM; ++i)
		{
			shenqu_list[i].Reset();
		}

		memset(shenqu_history_cap_list, 0, sizeof(shenqu_history_cap_list));
	}

	char using_shenge_page_index;																	// 使用的神格页
	char free_choujiang_times;																		// 免费抽奖次数
	char is_can_get_fisrt_ten_chou;																	// 第一次十连抽标志
	char reserve_ch;
	unsigned int next_free_chou_timestamp;															// 到下次免费抽奖的剩余cd
	int marrow_score;																				// 神格碎片
	UNSTD_STATIC_CHECK(5 == SHENGE_SYSTEM_MAX_SHENGE_PAGE)
		ShengeSinglePage shenge_page_list[SHENGE_SYSTEM_MAX_SHENGE_PAGE];							// 神格页数据
	UNSTD_STATIC_CHECK(250 == SHENGE_SYSTEM_BAG_MAX_GRIDS)
		ShengeParamData virtual_bag_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];								// 虚拟背包
	int history_set_total_level[SHENGE_SYSTEM_MAX_SHENGE_PAGE];										// 历史装备最高等级
	ZhangkongParam zhangkong_level_list[SHENGE_SYSTEM_QUALITY_MAX_VALUE];							// 掌控

	SingleShengeShenqu shenqu_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
	int shenqu_history_cap_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
};

typedef char ShengeSystemParamHex[sizeof(ShengeSystemParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengeSystemParamHex) < 8192);

#pragma pack(pop)

#endif
