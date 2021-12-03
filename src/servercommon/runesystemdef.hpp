#ifndef __RUNE_SYSTEM_DEF_HPP__
#define __RUNE_SYSTEM_DEF_HPP__

#include "servercommon.h"

static const int RUNE_SYSTEM_BAG_MAX_GRIDS = 200;								// 背包最大格子数量不可变 数据库
static const int RUNE_SYSTEM_SLOT_MAX_NUM = 10;									// 符文槽最大数
static const int RUNE_SYSTEM_RUNE_MAX_LEVEL = 200;								// 符文最大等级
static const int RUNE_SYSTEM_XUNBAO_RUNE_MAX_COUNT = 10;						// 寻宝得符文最大数量
static const int RUNE_SYSTEM_BOX_REWAED_MAX_ITEM = 8;							// 符文宝箱最大物品

enum RUNE_QUALITY_KIND
{
	RUNE_QUALITY_KIND_WHITE = 0,				// 白色符文
	RUNE_QUALITY_KIND_BLUE,						// 蓝色符文
	RUNE_QUALITY_KIND_PURPLE,					// 紫色符文
	RUNE_QUALITY_KIND_ORANGE,					// 橙色符文
	RUNE_QUALITY_KIND_RED,						// 红色符文
	RUNE_QUALITY_KIND_PINK,                     // 粉色符文

	RUNE_QUALITY_KIND_MAX,
};

enum RUNE_TYPE
{
	RUNE_TYPE_GONGJI = 0,														// 攻击
	RUNE_TYPE_MAXHP,															// 生命
	RUNE_TYPE_BAOJI,															// 暴击
	RUNE_TYPE_SHANBI,															// 闪避
	RUNE_TYPE_EXP,																// 经验
	RUNE_TYPE_MINGZHONG,														// 命中
	RUNE_TYPE_FANGYU,															// 防御
	RUNE_TYPE_KANGBAO,															// 抗暴
	RUNE_TYPE_HUOLI,															// 活力
	RUNE_TYPE_PROTECT,															// 守护
	RUNE_TYPE_FENGRUI,															// 锋锐
	RUNE_TYPE_JIANREN,															// 坚韧
	RUNE_TYPE_JINGZHUN,															// 精准
	RUNE_TYPE_XIANYOU,															// 仙佑
	RUNE_TYPE_CHANGSHENG,														// 长生
	RUNE_TYPE_JIANGANG,															// 金刚
	RUNE_TYPE_KUANGBAO,															// 狂暴
	RUNE_TYPE_FENSUI,															// 粉碎
	RUNE_TYPE_SHALU,															// 杀戮
	RUNE_TYPE_RUNEJINGHUA,														// 符文精华（放最后面）
	RUNE_TYPE_WUSHIYIJI,														// 无视一击
	RUNE_TYPE_WUSHUANG,                                                         // 无双
	RUNE_TYPE_QIANGGONG,                                                        // 强攻
	RUNE_TYPE_QIANGREN,                                                         // 强韧
	RUNE_TYPE_XIANGONG,                                                         // 仙攻
	RUNE_TYPE_XIANBAO,                                                          // 仙暴
	RUNE_TYPE_LINGQIAO,                                                         // 灵巧

	RUNE_TYPE_MAX,																//最大符文类型
};

enum RUNE_ATTR_TYPE
{
	RUNE_ATTR_TYPE_INVALID = 0,
	RUNE_ATTR_TYPE_GONGJI,											// 攻击
	RUNE_ATTR_TYPE_MAXHP,											// 生命
	RUNE_ATTR_TYPE_BAOJI,											// 暴击
	RUNE_ATTR_TYPE_SHANBI,											// 闪避
	RUNE_ATTR_TYPE_EXP_FETCH,										// 怪物经验(%)
	RUNE_ATTR_TYPE_MINGZHONG,										// 命中
	RUNE_ATTR_TYPE_JIANREN,											// 坚韧
	RUNE_ATTR_TYPE_FANGYU,											// 防御
	RUNE_ATTR_TYPE_WUQI_GONGJI,										// 武器攻击(%)
	RUNE_ATTR_TYPE_FANGJU_HP,										// 防具生命(%)
	RUNE_ATTR_TYPE_FANGJU_SHANBI,									// 防具闪避(%)
	RUNE_ATTR_TYPE_FANGJU_FANGYU,									// 防具防御(%)
	RUNE_ATTR_TYPE_FANGJU_JIANREN,									// 防具坚韧(%)
	RUNE_ATTR_TYPE_PER_POFANG,										// 破防百分比（增伤率）
	RUNE_ATTR_TYPE_WUQI_BAOJI,                                      // 武器暴击(%)
	RUNE_ATTR_TYPE_PER_MIANSHANG,                                   // 免伤率
	RUNE_ATTR_TYPE_SHOUSHI_GONGJI,                                  // 首饰攻击(戒指、手镯、项链、护符)
	RUNE_ATTR_TYPE_SHOUSHI_BAOJI,                                   // 首饰暴击(戒指、手镯、项链、护符)
	RUNE_ATTR_TYPE_PER_POJIA,                                       // 破甲
	RUNE_ATTR_TYPE_PER_EQUIP_GONGJI,                                // 全身普通装备攻击加成万分比
	RUNE_ATTR_TYPE_FUJIA_SHANGHAI,									// 附加伤害

	RUNE_ATTR_TYPE_COUNT,											// 符文属性最大类型
};

// 效果为百分比加成
enum RUNE_SPECIAL_EFFECT_TYPE_PERCEN
{
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID = 0,
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_GONGJI,					// 增加武器攻击
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_HP,						// 增加防具生命
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_SHANBI,					// 增加防具闪避
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_FANGYU,					// 增加防具防御
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_JIANREN,				// 增加防具坚韧
	RUNE_SPECIAL_EFFECT_TYPE_ADD_KILL_MONSTER_EXP,					// 增加杀怪经验
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_BAOJI,                    // 增加武器暴击
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_GONGJI,                // 增加首饰攻击
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_BAOJI,                 // 增加首饰暴击
	RUNE_SPECIAL_EFFECT_TYPE_ADD_EQUIP_PER_GONGJI,					// 增加全身普通装备攻击

	RUNE_SPECIAL_EFFECT_TYPE_COUNT									// 特殊效果最大类型
};

enum RUNE_BOX_TYPE
{
	RUNE_BOX_TYPE_INVALID = 0,
	RUNE_BOX_TYPE_JINGHUA,				// 精华宝箱，可以开出精华
	RUNE_BOX_TYPE_RUNE_AND_JINGHUA,		// 符文宝箱，可以开出精华和符文

	RUNE_BOX_TYPE_MAX,
};

enum RUNE_WEIGHT_TYPE
{
	RUNE_WEIGHT_TYPE_DEFAULT = 0,     // 默认权重
	RUNE_WEIGHT_TYPE_X,			      // 累计X次权重
	RUNE_WEIGHT_TYPE_X1,		      // 累计X1次权重
};

#pragma pack(push, 4)

struct RuneParam
{
	RuneParam() { this->Reset(); }

	void Reset()
	{
		quality = -1;
		type = -1;
		level = 0;
	}

	char quality;				// 品质
	char type;					// 类型
	short level;				// 等级

	bool IsEmpty() const { return level <= 0 || quality < 0 || type < 0; }
};

struct RuneAwakenParm
{
	RuneAwakenParm(){this->Reset();}

	void Reset()
	{
		maxhp = 0;
		gongji = 0;
		fangyu = 0;
		mingzhong = 0;
		shanbi = 0;
		baoji = 0;
		jianren = 0;
		add_per = 0;	
	}

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;		// 未使用-以后可扩展
	Attribute shanbi;			// 未使用-以后可扩展
	Attribute baoji;			// 未使用-以后可扩展
	Attribute jianren;		// 未使用-以后可扩展
	int add_per;		// 效果增幅
};

struct RuneZhuLingParam
{
	RuneZhuLingParam() { this->Reset(); }
	void Reset()
	{
		grade = 0;
		zhuling_bless = 0;
	}

	int grade;
	int zhuling_bless;
};

struct RuneSystemParam
{
	RuneSystemParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
		{
			rune_grid[i].Reset();
		}

		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
		{
			rune_grid_awaken[i].Reset();
		}

		for (int i = 0; i < RUNE_SYSTEM_BAG_MAX_GRIDS; ++i)
		{
			virtual_bag_list[i].Reset();
		}

		rune_suipian = 0;
		rune_jinghua = 0;
		magic_crystal = 0;

		next_free_xunbao_timestamp = 0;
		rune_slot_open_flag = 1;
		free_xunbao_times = 0;
		fb_fetch_time_count = 0;

		fb_pass_layer = 0;
		fb_today_layer = 0;
		fb_offline_time = 3600 * 12;
		fb_add_exp = 0;
		fb_add_jinghua = 0;
		fb_add_equip_blue = 0;
		fb_add_equip_purple = 0;
		fb_add_equip_orange = 0;

		is_need_recalc_awaken = 0;
		baodi = 0;

		rune_gold_awaken_times = 0;
		rune_common_awaken_times = 0;

		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++ i)
		{
			run_zhuling_list[i].Reset();
		}

		zhuling_slot_bless = 0;
		total_xunbao_times = 0;
	}

	UNSTD_STATIC_CHECK(10 == RUNE_SYSTEM_SLOT_MAX_NUM);
	UNSTD_STATIC_CHECK(200 == RUNE_SYSTEM_BAG_MAX_GRIDS);

	RuneParam rune_grid[RUNE_SYSTEM_SLOT_MAX_NUM];							// 符文格子数据
	RuneParam virtual_bag_list[RUNE_SYSTEM_BAG_MAX_GRIDS];					// 虚拟背包

	int rune_suipian;														// 符文碎片
	int rune_jinghua;														// 符文精华
	int magic_crystal;														// 符文水晶

	unsigned int next_free_xunbao_timestamp;								// 符文寻宝下次免费时间戳
	unsigned short rune_slot_open_flag;										// 符文槽开启标记(最后一位用来记录符文合成是否开启)
	char free_xunbao_times;													// 免费寻宝次数

	char fb_fetch_time_count;												// 符文塔领取离线时间次数
	int fb_pass_layer;														// 符文塔通关层数
	int fb_today_layer;														// 符文塔今日层数
	int fb_offline_time;													// 符文塔离线时间
	long long fb_add_exp;													// 符文塔今日获得经验
	int fb_add_jinghua;														// 符文塔今日获得精华
	int fb_add_equip_blue;													// 符文塔今日获得蓝装
	int fb_add_equip_purple;												// 符文塔今日获得紫装
	int fb_add_equip_orange;												// 符文塔今日获得橙装

	short is_need_recalc_awaken;											// 是否需要重算觉醒属性 1需要， 0不需要
	short baodi;
	RuneAwakenParm rune_grid_awaken[RUNE_SYSTEM_SLOT_MAX_NUM];				// 符文格觉醒

	int rune_gold_awaken_times;												// 符文元宝觉醒次数
	int rune_common_awaken_times;											// 符文普通觉醒次数

	RuneZhuLingParam run_zhuling_list[RUNE_SYSTEM_SLOT_MAX_NUM];
	int zhuling_slot_bless;
	unsigned int total_xunbao_times;										// 历史寻宝次数
};

typedef char RuneSystemParamHex[sizeof(RuneSystemParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RuneSystemParamHex) < 8192);

#pragma pack(pop)
#endif
