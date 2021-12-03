#ifndef __FB_DEF_HPP__
#define __FB_DEF_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push, 4)

//////////////////////////////////////////////////////////  装备副本  //////////////////////////////////////////////////////////
static const int FB_EQUIP_MAX_MYSTERYLAYER_NUM = 16;																// 神秘层数量
static const int FB_EQUIP_MAX_GOODS_NUM_PER_MYSTERYLAYER = 6;														// 每层神秘宝藏数量
static const int FB_EQUIP_MAX_LAYER_ID = 71;																		// 最大层号
static const int FB_EQUIP_MAX_GOODS_SEQ = FB_EQUIP_MAX_MYSTERYLAYER_NUM * FB_EQUIP_MAX_GOODS_NUM_PER_MYSTERYLAYER;	// 最大神秘层商品编号

//////////////////////////////////////////////////////////  单人塔防副本  //////////////////////////////////////////////////////////
static const int AUTO_FB_FREE_WITH_LEVEL = 70;																		// 指定等级以上，副本扫荡免费

struct EquipFBInfo
{
	enum EFB_FLAG
	{
		EFB_FLAG_DOUBLE_DROP = 0x1,	
	};

	EquipFBInfo() { this->Reset(); }

	void Reset()
	{
		max_layer_today_entered = 0;
		max_passed_real_layer = 0;
		flag = 0;
		max_layer_today_pass = 0;
		today_multiple_buy_times = 0;
		member_min_layer = 0;
		can_give_prelayer_reward = 1;
		is_record_today_max_pass_level = 1;
		memset(mysterylayer_list, 0, sizeof(mysterylayer_list));
		reserve_sh_1 = 0;
		reserve_sh_2 = 0;
	}

	void DayReset()
	{
		max_layer_today_entered = 0;
		flag = 0;
		max_layer_today_pass = 0;
		today_multiple_buy_times = 0;
		is_record_today_max_pass_level = 1;
		memset(mysterylayer_list, 0, sizeof(mysterylayer_list));
	}

	char max_layer_today_entered;																					// 今天进入的最大层数
	char max_passed_real_layer;																						// 通关过的最高层，不包含神秘层
	short flag;																										// 按位，EFB_FLAG
	char mysterylayer_list[FB_EQUIP_MAX_GOODS_SEQ];																	// 神秘层商品购买记录
	char max_layer_today_pass;																						// 今天最大通关层数
	char today_multiple_buy_times;																					// 今天多倍奖励购买次数
	char member_min_layer;																							// 组队成员中最小历史通过数
	char can_give_prelayer_reward;																					// 能否获得历史最大通关层奖励（进入副本时，按队伍的最小通关数确定奖励层数）
	char is_record_today_max_pass_level;																			// 是否记录今天最大通关层数（用作多倍奖励，策划需求，跟队进入的按今天第一次参与副本的通关层数给奖励）
	char reserve_sh_1;
	short reserve_sh_2;
};

struct EquipFBParam
{
	EquipFBParam() { this->Reset(); }

	void Reset()
	{
		personal_equipfb_info.Reset();
		team_equipfb_info.Reset();
	}

	EquipFBInfo personal_equipfb_info;																				// 单人装备本信息
	EquipFBInfo team_equipfb_info;																					// 组队装备本信息
};

typedef char EquipFBParamHex[sizeof(EquipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EquipFBParamHex) < 512);

//////////////////////////////////////////////////////////  仙魂塔  //////////////////////////////////////////////////////////
static const int MAX_FBXHT_NORMAL_XIANHUNTA_LAYER = 80;																// 仙魂塔普通最大关卡
static const int MAX_FBXHT_JINGYING_XIANHUNTA_LAYER = 7;															// 仙魂塔精英最大关卡
static const int MAX_FBXHT_REWARD_ITEM_COUNT = 3;																	// 关卡最大奖励物品数量

enum EN_FBXIANHUNTA_TYPE
{
	EN_FBXIANHUNTA_TYPE_NORMAL = 1,																					// 普通仙魂塔
	EN_FBXIANHUNTA_TYPE_JINGYING,																					// 精英仙魂塔
};

struct XianhuntaFBPassNewsItem
{
	XianhuntaFBPassNewsItem() : news_id(0), uid(0), capability(0), pass_time(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int news_id;																					// 记录id，越大越近
	int uid;																						// uid
	GameName user_name;																				// 角色名
	int capability;																					// 战斗力
	int pass_time;																					// 通关时间
};

struct XianhuntaFBParam
{
	XianhuntaFBParam() { this->Reset(); }

	void Reset()
	{
		passed_normal_level = 0; passed_jingying_level = 0;
		is_fetched_reward = 0;
		left_xianhun = 0; left_reward_item_count = 0;
		today_normal_fail_times = 0; today_jingying_fail_times = 0;
		normal_enterable_time = 0; jingying_enterable_time = 0;
	}

	short passed_normal_level;																		// 已经通关的普通关卡
	char passed_jingying_level;																		// 已经通关的精英关卡
	char is_fetched_reward;																			// 是否领取过奖励

	int left_xianhun;																				// 积累的仙魂
	short left_reward_item_count;																	// 积累的道具数

	char today_normal_fail_times;																	// 当前普通关卡今天失败次数
	char today_jingying_fail_times;																	// 当前精英关卡今天失败次数
	unsigned int normal_enterable_time;																// 当前普通关卡可进入时间
	unsigned int jingying_enterable_time;															// 当前精英关卡可进入时间
};

typedef char XianhuntaParamHex[sizeof(XianhuntaFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianhuntaParamHex) < 64);


//////////////////////////////////////////////////////////  VIP副本  //////////////////////////////////////////////////////////
static const int FB_VIP_NUM_MAX = 16;								// VIP副本最大数量

struct VipFBParam
{
	VipFBParam() { this->Reset(); }

	void Reset()
	{
		memset(today_times_list, 0, sizeof(today_times_list));
		is_pass_flag = 0;
	}

	char today_times_list[FB_VIP_NUM_MAX];
	int is_pass_flag;
};

typedef char VipFBParamHex[sizeof(VipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(VipFBParamHex) < 64);

////////////////////////////////////////////////////////// 组队副本/////////////////////////////////////////////////////////
static const int FB_TEAM_ROOM_MAX_NUM = 50;

enum TEAM_FB_MODE													// 困难模式
{
	TEAM_FB_MODE_EASY = 0,											// 简单									
	TEAM_FB_MODE_NORMAL,											// 普通
	TEAM_FB_MODE_HARD,												// 困难

	TEAM_FB_MAX_MODE,
};

//////////////////////////////////////////////////////////  塔防  //////////////////////////////////////////////////////////
struct TowerDefendFBParam
{
	TowerDefendFBParam() { this->Reset(); }

	void Reset()
	{
		personal_max_pass_level = -1;
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
		personal_last_level_star = 0;

		team_attrtype = 0;
		reserve_ch = 0;
		team_member_min_pass = 0;
		team_max_wave_passed = 0;

		personal_holiday_guard_join_times = 0;
		reserver_sh = 0;
		personal_holiday_guard_kill_monster = 0;
		today_multiple_buy_times = 0;
		is_new_player = 0;
		reserve_ch_2 = 0;
	}

	void OnResetData()
	{
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
		personal_holiday_guard_join_times = 0;
		today_multiple_buy_times = 0;
	}

	char personal_max_pass_level;							// 单人塔防 最大通关数
	char personal_join_times;								// 单人塔防 参与次数
	char personal_buy_join_times;							// 单人塔防 购买参与次数
	char personal_auto_fb_free_times;						// 单人塔防 免费扫荡次数

	short personal_item_buy_join_time;						// 单人塔防 道具购买参与次数
	short personal_last_level_star;							// 当前最高层次的星级		

	char team_attrtype;										// 组队塔防加成类型
	char reserve_ch;
	short team_member_min_pass;								// 组队守护成员最小整数波
	int	team_max_wave_passed;								// 组队塔防消灭的最大波数

	char personal_holiday_guard_join_times;					// 节日守护 参与次数
	char reserver_sh;
	short personal_holiday_guard_kill_monster;				// 节日守护 击杀数量

	char today_multiple_buy_times;							// 今天多倍奖励购买次数
	char is_new_player;
	short reserve_ch_2;
};

typedef char TowerDefendParamHex[sizeof(TowerDefendFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TowerDefendParamHex) < 64);

////////////////////////////////////////////////////////// 妖兽祭坛/////////////////////////////////////////////////////////
static const int YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM = 4;
struct YaoShouJiTanTeamFbRoleAttrInfo
{
	YaoShouJiTanTeamFbRoleAttrInfo() : uid(0), attr(0) {}

	void Reset()
	{
		uid = 0;
		attr = 0;
	}

	bool IsEmpty()
	{ 
		return uid == 0;
	}

	int uid;
	int attr;
};

////////////////////////////////////////////////////////// 迷宫仙府/////////////////////////////////////////////////////////

static const int MIGONGXIANFU_NORMAL_LAYER_NUM = 5;		// 迷宫仙府普通层数量
static const int MIGONGXIANFU_LAYER_NUM = 7;			// 迷宫仙府最大层数
static const int MIGONGXIANFU_DOOR_NUM = 5;				// 迷宫仙府传送点最大数
static const int MIGONGXIANFU_LAYER_FIRST_INDEX = 0;	// 第1层索引
static const int MIGONGXIANFU_BOSS_LAYER_INDEX = 5;		// Boss层索引	
static const int MIGONGXIANFU_HIDE_LAYER_INDEX = 6;		// 隐藏层索引

UNSTD_STATIC_CHECK(MIGONGXIANFU_NORMAL_LAYER_NUM < MIGONGXIANFU_LAYER_NUM);

enum MIGONGXIANFU_STATUS_TYPE
{
	MGXF_DOOR_STATUS_NONE = 0,
	MGXF_DOOR_STATUS_TO_PRVE,
	MGXF_DOOR_STATUS_TO_HERE,
	MGXF_DOOR_STATUS_TO_NEXT,
	MGXF_DOOR_STATUS_TO_HIDE,
	MGXF_DOOR_STATUS_TO_BOSS,
	MGXF_DOOR_STATUS_TO_FIRST,
};

enum MIGONGXIANFU_LAYER_TYPE
{
	MGXF_LAYER_TYPE_NORMAL = 0,								// 普通层
	MGXF_LAYER_TYPE_BOSS,									// Boss层
	MGXF_LAYER_TYPE_HIDE,									// 隐藏层
};

//////////////////////////////////////////////////////////  新装备本(武器本)  //////////////////////////////////////////////////////////
const static int NEQFB_MAX_CHAPTER = 24;                     // 共x章节
const static int NEQFB_MAX_LEVEL_PER_CHAPTER = 8;            // 每个章节x关
const static int NEQFB_MAX_STAR = 3;
const static int NEQFB_PASS_LAYER_BASE_VAL = 10000;
const static int NEQFB_MAX_DROP_COUNT = 32;

const static int NEQFB_ROLL_ITYPE_0_ITEM_COUNT = 2;
const static int NEQFB_ROLL_ITYPE_1_ITEM_COUNT = 5;
const static int NEQFB_ROLL_ITYPE_2_ITEM_COUNT = 1;
const static int NEQFB_ROLLPOOL_TOTAL_COUNT = 8;            // 翻牌奖励总个数

const static int NEQFB_PICK_FALL_ITEM_RECORD_COUNT = 8;     // 记录个人拾取掉落的个数

UNSTD_STATIC_CHECK(NEQFB_ROLLPOOL_TOTAL_COUNT == NEQFB_ROLL_ITYPE_0_ITEM_COUNT + NEQFB_ROLL_ITYPE_1_ITEM_COUNT + NEQFB_ROLL_ITYPE_2_ITEM_COUNT);

const static int NEQFB_CAN_ROLL_ITYPE_1_LIMIT_COUNT = 2;
const static int NEQFB_MAX_ROLL_TIMES = 8;
const static int NEQFB_MAX_GOLD_ROLL_TIMES = 3;
const static int NEQFB_MAX_FREE_ROLL_TIMES = 5;

UNSTD_STATIC_CHECK(NEQFB_MAX_ROLL_TIMES == NEQFB_MAX_GOLD_ROLL_TIMES + NEQFB_MAX_FREE_ROLL_TIMES);
UNSTD_STATIC_CHECK(NEQFB_ROLL_ITYPE_0_ITEM_COUNT >= NEQFB_CAN_ROLL_ITYPE_1_LIMIT_COUNT);
UNSTD_STATIC_CHECK(NEQFB_ROLLPOOL_TOTAL_COUNT >= NEQFB_MAX_ROLL_TIMES);

struct NewEquipFBLevel
{
	NewEquipFBLevel() { this->Reset(); }

	void Reset()
	{
		join_times = 0;
		max_star = 0;
		reserve_sh = 0;
	}

	char join_times;
	char max_star;
	short reserve_sh;
};

struct NewEquipFBChapter
{
	NewEquipFBChapter() { this->Reset(); }

	void Reset()
	{
		reward_flag = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		memset(level_list, 0, sizeof(level_list));
	}

	char reward_flag;
	char reserve_ch;
	short reserve_sh;
	NewEquipFBLevel level_list[NEQFB_MAX_LEVEL_PER_CHAPTER];
};

struct NewEquipFBParam
{
	NewEquipFBParam() { this->Reset(); }

	void Reset()
	{
		today_vip_buy_times = 0;
		today_item_buy_times = 0;
		today_fight_all_times = 0;
		reserve_sh_3 = 0;
		reserve_int = 0;
		memset(chapter_list, 0, sizeof(chapter_list));
	}

	short today_vip_buy_times;
	short today_item_buy_times;
	short today_fight_all_times;
	short reserve_sh_3;
	int reserve_int;
	NewEquipFBChapter chapter_list[NEQFB_MAX_CHAPTER];
};

typedef char NewEquipFBParamHex[sizeof(NewEquipFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(NewEquipFBParamHex) < 2048);

//////////////////////////////////////////////////防具材料本/////////////////////////////
static const int ARMORDEFEND_SKILL_MAX_COUNT = 2;			// 单人塔防最大技能数
struct ArmorDefendFBParam
{
	ArmorDefendFBParam() { this->Reset(); }

	void Reset()
	{
		personal_max_pass_level = -1;
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0; 
		personal_item_buy_join_time = 0;
		reserve_sh = 0;
	}

	void OnResetData()
	{
		personal_join_times = 0;
		personal_buy_join_times = 0;
		personal_auto_fb_free_times = 0;
		personal_item_buy_join_time = 0;
	}

	char personal_max_pass_level;						// 单人塔防 最大通关数
	char personal_join_times;							// 单人塔防 参与次数
	char personal_buy_join_times;						// 单人塔防 购买参与次数
	char personal_auto_fb_free_times;					// 单人塔防 免费扫荡次数

	short personal_item_buy_join_time;					// 单人塔防 道具购买参与次数
	short reserve_sh;
};

typedef char ArmorDefendParamHex[sizeof(ArmorDefendFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ArmorDefendParamHex) < 64);

//////////////////////////////////////////////////////////  剧情副本  //////////////////////////////////////////////////////////
static const int FB_STORY_MAX_COUNT = 20;			// 剧情本最大数量

struct StoryFBParam
{
	struct FbItem
	{
		FbItem() { this->Reset(); }

		void Reset()
		{
			is_pass = 0;
			today_times = 0;
			reserved = 0;
		}

		char is_pass;
		char today_times;
		short reserved;
	};

	StoryFBParam() { this->Reset(); }
	void Reset()
	{
		curr_fb_index = 0;
		for (int i = 0; i < FB_STORY_MAX_COUNT; ++i)
		{
			fb_list[i].Reset();
		}
	}

	int curr_fb_index;
	FbItem fb_list[FB_STORY_MAX_COUNT];
};

typedef char StoryFBParamHex[sizeof(StoryFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(StoryFBParamHex) < 2048);

//////////////////////////////////////////////////////////  魔戒副本  //////////////////////////////////////////////////////////
static const int MOJIE_FB_HUMAN_PIAN_MAX_COUNT = 8;						// 人类篇副本最大值
static const int MOJIE_FB_AIREN_PIAN_MAX_COUNT = 8;						// 矮人篇副本最大值
static const int MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT = 8;			// 精灵章节最大值
static const int MOJIE_FB_JINGLING_PIAN_ZHANGJIE_TASK_MAX_COUNT = 10;	// 精灵章节的任务数最大值
static const int MOJIE_FB_SKILL_MAX_COUNT = 30;							// 魔戒技能最大值

// 精灵任务章节
enum MOJIEFB_JINGLINGPIAN_CHAPTER_TYPE
{
	MOJIEFB_FANGYU_TASK = 0,		// 防御到达
	MOJIEFB_ZHUANGBEI_TASK,			// 装备到达
	MOJIEFB_BOSS_TASK,				// 击杀boss
	MOJIEFB_SHENZHU_TASK,			// 神铸达到
};

// 魔戒技能
enum MOJIEFB_JINENG
{
	MOJIEFB_SKILL_SHEHUN = 0,		// 摄魂：击杀怪物增加20%经验 
	MOJIEFB_SKILL_HUIFU,			// 恢复：攻击时有5%概率回复1000点气血
	MOJIEFB_SKILL_CHIDUN,			// 迟钝：攻击时有10%概率使目标速度减少40%，持续2秒
	MOJIEFB_SKILL_CHUMO,			// 除魔：对怪物伤害增加10% 
	MOJIEFB_SKILL_JIANSHANG,		// 减伤：同时受到3个怪物攻击时，所受伤害减少30%
	MOJIEFB_SKILL_ZHUFU,			// 祝福：击杀怪物装备掉率增加20%
	MOJIEFB_SKILL_JUEJING,			// 绝境：周围有5个怪物时，所有技能伤害对怪物提高20%
	MOJIEFB_SKILL_YANSHOU,			// 严守：血量低于30%时，收到伤害减少20%
	MOJIEFB_SKILL_ZUIJI,			// 追击：对血量低于30%玩家进行攻击时，伤害增加20%
	MOJIEFB_SKILL_DONGCHA,			// 洞察：受到攻击时有20%几率减少20%的伤害
	MOJIEFB_SKILL_KUANGMBAO,		// 狂暴：攻击时有20%几率增加10%伤害
	MOJIEFB_SKILL_JIANSHE,			// 溅射：攻击时有10%触发，对除目标以外的3个角色造成30%的伤害
	MOJIEFB_SKILL_SHUNSHI,			// 顺势：敌人（非boss）气血再5%以下时，攻击时有10%概率直接击杀
	MOJIEFB_SKILL_TUJING,			// 突进：攻击时有5%概率对目标额外造成2000点伤害
	MOJIEFB_SKILL_POJIA,			// 破甲：攻击时有10%概率无视对方30%防御
	MOJIEFB_SKILL_KUANGLV,			// 狂怒：自己气血低于10%时，暴击概率增加3%
	MOJIEFB_SKILL_GUANGHUI,		// 光辉：pvp时造成的伤害增加8%，收到伤害减少8%
	MOJIEFB_SKILL_ZHIBAO,			// 治爆：收到暴击时，有20%几率降低30%伤害
};

struct MojieFBParam
{

	MojieFBParam(){this->Reset(); }
	void Reset()
	{
		mojie_human_pian_task_mark = -1;
		mojie_airen_pian_task_mark = -1;
		mojie_jingling_pian_task_mark = -1;
	}

	int mojie_human_pian_task_mark;						// 魔戒人类篇完成标记
	int mojie_airen_pian_task_mark;						// 魔戒矮人篇完成标记
	int mojie_jingling_pian_task_mark;					// 魔戒精灵篇完成标记
};

typedef char MojieFBParamHex[sizeof(MojieFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MojieFBParamHex) < 128);

//////////////////////////////////////////////////////////  挑战副本  //////////////////////////////////////////////////////////
struct ChallengeFBParam
{
	const static int LEVEL_MAX_COUNT = 30;
	
	struct Level
	{
		Level()	{ this->Reset(); }

		void Reset()
		{
			pass_time = 0;
			is_pass = 0;
			state = 0;
			fight_layer = 0;
			history_max_reward = 0;
			use_count = 0;
			res_ch = 0;
		}

		void OnResetData()
		{
			pass_time = 0;
			fight_layer = 0;
			use_count = 0;
		}

		unsigned int pass_time;
		char is_pass;
		char fight_layer;
		short state;
		short history_max_reward;//奖励星级
		char use_count;
		char res_ch;
	};

	ChallengeFBParam() { this->Reset(); }

	void Reset()
	{
		enter_count = 0;
		buy_count = 0;

		for (int i = 0; i < LEVEL_MAX_COUNT; ++ i)
		{
			level_list[i].Reset();
		}
	}

	void OnResetData()
	{
		enter_count = 0;
		buy_count = 0;

		for (int i = 0; i < LEVEL_MAX_COUNT; ++ i)
		{
			level_list[i].OnResetData();
		}
	}

	Level level_list[LEVEL_MAX_COUNT];
	int enter_count;
	int buy_count;
};

typedef char ChallengeFBParamHex[sizeof(ChallengeFBParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeFBParamHex) < 768);

//////////////////////////////////////////////////////////  日常副本  //////////////////////////////////////////////////////////
static const int FB_RECORD_DROP_ITEM_MAX_NUM = 7;

static const int FB_EXP_MAX_WAVE = 300;			// 经验本最大波数
static const int MAX_DAILY_FB_NUM = 15;

enum DAILYFB_TYPE
{
	DAILYFB_TYPE_EXP = 0,		// 经验本
	DAILYFB_TYPE_COIN,			// 铜币本
	DAILYFB_TYPE_DAOJU,			// 道具本
};

struct DailyFbParam
{
	DailyFbParam() { this->Reset(); }

	void Reset()
	{
		expfb_history_pass_max_wave = 0;
		expfb_today_pay_times = 0;
		expfb_today_enter_times = 0;
		expfb_history_enter_times = 0;
		last_quit_fb_time = 0;
		expfb_last_exp = 0;
		expfb_record_max_exp = 0;
	}
	
	short expfb_history_pass_max_wave;				// 历史最大波数
	short expfb_today_pay_times;					// 今天的购买次数
	short expfb_today_enter_times;					// 今天进入次数
	unsigned short expfb_history_enter_times;		// 历史进入次数
	unsigned int last_quit_fb_time;					// 最后一次的退出时间
	long long expfb_last_exp;						// 上一次获得的经验，奖励找回用
	long long expfb_record_max_exp;					// 通关历史获得经验最高
};
typedef char DailyFbParamHex[sizeof(DailyFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DailyFbParamHex) < 512);

//////////////////////////////////////////////////////////  阶段副本  //////////////////////////////////////////////////////////
static const int FB_PHASE_MAX_COUNT = 32;		// 阶段本最大数量
static const int FB_PHASE_MAX_LEVEL = 30;		// 每个进阶本小关卡最大数

struct PhaseFbParam
{
	struct FbItem
	{
		FbItem() { this->Reset(); }

		void Reset()
		{
			is_pass = 0;
			today_times = 0;
			today_buy_times = 0;
			reserved = 0;
		}

		char is_pass;			// 已经通过了第几层(不清)
		char today_times;		// 今日已经用掉的次数
		char today_buy_times;	// 今日购买的次数
		char reserved;
	};

	PhaseFbParam() { this->Reset(); }
	void Reset()
	{
		curr_fb_index = 0;
		for (int i = 0; i < FB_PHASE_MAX_COUNT; ++i)
		{
			fb_list[i].Reset();
		}
		curr_fb_level = 0;
	}

	int curr_fb_index;
	FbItem fb_list[FB_PHASE_MAX_COUNT];
	int curr_fb_level;
};

typedef char PhaseFbParamHex[sizeof(PhaseFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PhaseFbParamHex) < 2048);

//////////////////////////////////////////////////////////  跨服组队副本  //////////////////////////////////////////////////////////
static const int NEW_FB_DROP_ITEM_MAX = 15;								// 单层掉落最大物品数
static const int NEW_FB_DELAY_SEND_TIME = 3;							// 副本延迟发奖励时间（秒）
static const int CROSS_TEAM_FB_GONGJI_BUFF_ADDON = 10;					// 跨服组满人攻击加成百分比

//////////////////////////////////////////////////////////  组队装备副本  //////////////////////////////////////////////////////////
static const int MAX_TEAM_EQUIP_FB_LAYER = 20;							// 组队副本最大层数
static const int TEAM_EQUIP_FB_ROLE_MAX = 4;							// 组队副本进入最大角色数
static const int TEAM_EQUIP_FB_GONGJI_BUFF_ADDON = 10;					// 跨服组满人攻击加成百分比
static const int TEAM_EQUIP_FB_DELAY_SEND_TIME = 3;						// 副本延迟发奖励时间（秒）

enum NEW_FB_STATE_TYPE
{
	NEW_FB_STATE_TYPE_DEFAULT = 0,										// 默认
	NEW_FB_STATE_TYPE_SUCCEED,											// 通关成功
	NEW_FB_STATE_TYPE_FAIL,												// 通关失败
};

struct FBDropItemInfo
{
	FBDropItemInfo() : item_id(0), is_bind(0), is_first(0), num(0) {}

	ItemID item_id;
	char is_bind;
	char is_first;
	int num;
};

//////////////////////////////////////////////////////////  推图副本  //////////////////////////////////////////////////////////
static const int TUTUI_FB_TYPE_NUM_MAX = 2;								// 推图副本类型最大数
static const int TUITU_FB_CHAPER_NUM_MAX = 50;							// 推图副本章节最大数
static const int TUITU_FB_LEVLE_NUM_MAX = 20;							// 推图副本关卡最大数
static const int TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX = 16;				// 推图副本章节星级奖励配置最大数

enum TUITU_FB_TYPE
{
	TUITU_FB_TYPE_NORMAL = 0,
	TUITU_FB_TYPE_HARD,

	TUITU_FB_TYPE_MAX
};

UNSTD_STATIC_CHECK(TUTUI_FB_TYPE_NUM_MAX >= TUITU_FB_TYPE_MAX);

struct TuituFbParam
{
	struct LayerInfo
	{
		LayerInfo() : pass_star(0), reward_flag(0), reserve_sh(0){}

		char pass_star;
		char reward_flag;
		short reserve_sh;
	};

	struct ChapterInfo
	{
		ChapterInfo() : is_pass_chapter(0), reserve_ch(0), reserve_sh(0), total_star(0), star_reward_flag(0) {}

		char is_pass_chapter;
		char reserve_ch;
		short reserve_sh;
		short total_star;
		short star_reward_flag;
		LayerInfo level_list[TUITU_FB_LEVLE_NUM_MAX];
	};

	struct FbInfo
	{
		FbInfo() { this->Reset(); }
		void Reset()
		{
			pass_chapter = -1;
			pass_level = -1;
			today_join_times = 0;
			buy_join_times = 0;

			memset(chapter_list, 0, sizeof(chapter_list));
		}

		short pass_chapter;				//已通过最大章节
		short pass_level;				//已通过最大关卡等级
		short today_join_times;			//今日进入次数
		short buy_join_times;			//购买次数
		ChapterInfo chapter_list[TUITU_FB_CHAPER_NUM_MAX];
	};

	TuituFbParam() { this->Reset(); }
	void Reset()
	{
		enter_fb_type = 0;
		enter_chapter = 0;
		enter_level = 0;
		reserve_ch = 0;
		for (int i = 0; i < TUTUI_FB_TYPE_NUM_MAX; ++i)
		{
			fb_list[i].Reset();
		}
	}

	void ResetData()
	{
		for (int i = 0; i < TUTUI_FB_TYPE_NUM_MAX; ++i)
		{
			fb_list[i].today_join_times = 0;
			fb_list[i].buy_join_times = 0;

			for (int j = 0; j < TUITU_FB_CHAPER_NUM_MAX; ++j)
			{
				for (int k = 0; k < TUITU_FB_LEVLE_NUM_MAX; ++k)
				{
					fb_list[i].chapter_list[j].level_list[i].reward_flag = 0;
				}
			}
		}
	}

	char enter_fb_type;
	char enter_chapter;
	char enter_level;
	char reserve_ch;
	FbInfo fb_list[TUTUI_FB_TYPE_NUM_MAX];
};

typedef char TuituFbParamHex [sizeof(TuituFbParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TuituFbParamHex) < 20480);

//////////////////////////////////////////////// 秘藏boss	//////////////////////////////////////

struct PreciousBossParam
{
	PreciousBossParam()
	{
		this->Reset();
	}

	enum PRECIOUS_BOSS_TASK_TYPE
	{
		PRECIOUS_BOSS_TASK_TYPE_BOSS = 0,
		PRECIOUS_BOSS_TASK_TYPE_COMMON_MONSTER,
		PRECIOUS_BOSS_TASK_TYPE_GATHER,
		PRECIOUS_BOSS_TASK_TYPE_GREAT_MONSTER,

		PRECIOUS_BOSS_TASK_TYPE_MAX,
	};

	void Reset()
	{
		this->ResetTaskList();
	}

	void ResetTaskList()
	{
		for (int i = 0; i < PRECIOUS_BOSS_TASK_TYPE_MAX; ++i)
		{
			task_list[i].Reset();
		}
	}

	struct TaskParam
	{
		TaskParam()
		{
			this->Reset();
		}

		void Reset()
		{
			task_condition = 0;
			task_type = -1;
			is_finish = 0;
		}

		short task_condition;					// 任务条件数量
		char task_type;							// 任务类型
		char is_finish;							// 是否完成
	};

	TaskParam task_list[PRECIOUS_BOSS_TASK_TYPE_MAX];
};

typedef char PreciousBossParamHex[sizeof(PreciousBossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PreciousBossParamHex) < 64);

/////////////////////////// 个人boss //////////////////////////////////////////
static const int MAX_PERSON_BOSS_LAYER = 40;     // 个人副本最大层数

struct PersonBossParam
{
	PersonBossParam()
	{
		this->Reset();
	}

	void Reset() 
	{
		memset(person_boss_today_enter_times, 0, sizeof(person_boss_today_enter_times));
	}
	char person_boss_today_enter_times[MAX_PERSON_BOSS_LAYER + 1];    // 个人boss，每个boss单独次数
};

typedef char PersonBossParamHex[sizeof(PersonBossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PersonBossParamHex) < 128);
////////////////////////////////神魔boss/////////////////////////////////////////////////////////////
static const int GODMAGIC_BOSS_MAX_HISTROY_RECROD = 5;						    // 神魔boss最大击杀记录
static const int MAX_GODMAGIC_BOSS_PER_SCENE = 20;							    // 每个场景boss最大个数
static const int GODMAGIC_BOSS_MAX_MONSTER_NUM = 200;							// 神魔boss小怪最大数量
static const int GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL = 60;						// 神魔boss最大普通水晶
static const int GODMAGIC_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM = 60;				// 神魔boss最大珍惜水晶
static const int GODMAGIC_BOSS_SCENE_MAX = 10;									// 神魔boss场景最大个数
static const int GODMAGIC_BOSS_MAX_DROP_RECROD = 20;							// 神魔boss最大掉落记录

#pragma pack(pop)

#endif // __FB_DEF_HPP__



