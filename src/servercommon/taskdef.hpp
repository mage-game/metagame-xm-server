#ifndef __TASK_DEF_HPP__
#define __TASK_DEF_HPP__

#include <memory>
#include "servercommon/basedef.h"
#include "servercommon/servercommon.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TASK_COLOR
{
	TASK_COLOR_INVALID = 0,

	TASK_COLOR_WHITE,															// 白色
	TASK_COLOR_GREEN,															// 绿色
	TASK_COLOR_BLUE,															// 蓝色
	TASK_COLOR_PURPLE,															// 紫色
	TASK_COLOR_ORANGE,															// 橙色

	TASK_COLOR_MAX,
};

enum TASK_TYPE
{
	TASK_TYPE_INVALID = -1,
	TASK_TYPE_TRUNK = 0,														// 主线
	TASK_TYPE_BRANCH = 1,														// 支线
	TASK_TYPE_DAILY = 2,														// 日常
	TASK_TYPE_HUSONG = 3,														// 护送
	TASK_TYPE_GUILD = 4,														// 仙盟任务
	TASK_TYPE_CISHA = 5,														// 刺杀任务
	TASK_TYPE_PAOHUAN = 6,														// 跑环任务
	TASK_TYPE_ZHUANZHI = 8,                                                     // 转职任务
	TASK_COUNT,
};

enum TASK_CONDITION
{
	TASK_CON_INVALID = -1,
	TASK_CON_TALK = 0,															// npc对话
	TASK_CON_KILL_MONSTER,														// 打怪
	TASK_CON_GATHER,															// 采集
	TASK_CON_PASS_FB,															// 完成副本
	TASK_CON_ENTER_SCENE,														// 进入场景
	TASK_CON_COMMIT_TASK,														// 提交任务
	TASK_CON_NOTHING,															// 无需操作
	TASK_CON_DO_OPERATOR,														// 执行某种操作
	TASK_CON_SATISFY_STATUS,													// 满足某种状态
	TASK_CON_ENTER_SCENE_TYPE,													// 进入某种场景类型
	TASK_CON_PICKUP_ITEM,														// 拾取某种物品
	TASK_CON_KILL_BOSS_TYPE,													// 击杀某种怪物类型
	TASK_CON_PASS_FB_LAYER,														// 副本通关到X层
	TASK_CON_PASS_SPECIAL_TASK,													// 特殊任务，客户端发协议告诉完成
	TASK_CON_PASS_DAILY_TASK_FB,												// 完成日常任务副本
	TASK_CON_BIANSHEN_TALK,														// 执行某种操作变身找NPC对话
	TASK_CON_COMMIT_ITEM,														// 提交消耗物品
	TASK_CON_KILL_MONSTER_LEVEL,                                                // 击杀某一等级的怪物
	TASK_CON_MAX,
};

enum TASK_ACCEPT_OP
{
	TASK_ACCEPT_OP_INVALID = 0,
	TASK_ACCEPT_OP_FLY,															// 接受任务时飞行，完成任务后设回正常行走模式														
	TASK_ACCEPT_OP_MOVE,														// 传送到某个地点
	TASK_ACCEPT_OP_FLY_ON_ACCEPT,												// 仅仅接受任务的时候飞模式
	TASK_ACCEPT_OP_MOVE_NORMAL_ON_REMOVE,										// 移除任务的时候设会正常行走模式
	TASK_ACCEPT_OP_ADD_SKILL_ON_ACCEPT,											// 接收任务激活技能
	TASK_ACCEPT_OP_ADD_SKILL_ON_REMOVE,											// 移除任务激活技能
	TASK_ACCEPT_OP_ENTER_GUIDE_FB,												// 进入引导副本，客户端用
	TASK_ACCEPT_OP_OPEN_GUIDE_VIEW,												// 进入引导副本界面，客户端用
	TASK_ACCEPT_OP_ENTER_DAILY_FB,												// 进入日常发布，客户端用
	TASK_ACCEPT_OP_CLIENT_PARAM,												// 客户端使用参数，服务器不验证
	TASK_ACCEPT_OP_MAX,
};

enum TASK_OPERATOR_TYPE
{
	TASK_OPERATOR_TYPE_MOUNT_UPGRADE = 1,										// 坐骑升阶
	TASK_OPERATOR_TYPE_GUILD_DONATE,											// 仙盟捐献
	TASK_OPERATOR_TYPE_STRENGTH,												// 装备强化
	TASK_OPERATOR_TYPE_UPSTAR,													// 装备升星
	TASK_OPERATOR_TYPE_UPLEVEL,													// 装备合成(升级）
	TASK_OPERATOR_TYPE_INHERIT,													// 装备继承
	TASK_OPERATOR_TYPE_EQUIP_RECYCLE,											// 装备回收
	TASK_OPERATOR_TYPE_LIEMING_CHOUHUN,											// 猎命一次
	TASK_OPERATOR_TYPE_LIEMING_HUN_ADD_EXP,										// 命魂升级
	TASK_OPERATOR_TYPE_FULIN,													// 装备附灵
	TASK_OPERATOR_TYPE_KILLBOSS,												// BOSS击杀一次
	TASK_OPERATOR_TYPE_ADD_TEAM,												// 加入队伍
	TASK_OPERATOR_TYPE_INLAY_STONE,												// 镶嵌宝石
	TASK_OPERATOR_TYPE_JOIN_GUILD,												// 加入仙盟
	TASK_OPERATOR_TYPE_SZ_JINJIE,												// 神装进阶
	TASK_OPERATOR_TYPE_SZ_SHENZHU,												// 神装神铸
	TASK_OPERATOR_TYPE_JL_FEISHENG,												// 精灵飞升
	TASK_OPERATOR_TYPE_JL_ZHUANGBEI,											// 精灵装备
	TASK_OPERATOR_TYPE_MOUNT_UP_STAR,											// 坐骑升星
	TASK_OPERATOR_TYPE_WING_UP_STAR,											// 羽翼升星
	TASK_OPERATOR_TYPE_HALO_UP_STAR,											// 光环升星
	TASK_OPERATOR_TYPE_SHENGONG_UP_STAR,										// 神弓升星
	TASK_OPERATOR_TYPE_SHENYI_UP_STAR,											// 神翼升星
	TASK_OPERATOR_TYPE_FIGHT_MOUNT_UP_STAR,										// 战斗坐骑升星
	TASK_OPERATOR_TYPE_SAILING,													// 决斗场 - 航海
	TASK_OPERATOR_TYPE_SAILING_ROB,												// 决斗场 - 抢夺帆船
	TASK_OPERATOR_TYPE_MINING,													// 决斗场 - 挖矿
	TASK_OPERATOR_TYPE_MINING_ROB,												// 决斗场 - 抢夺矿石
	TASK_OPERATOR_TYPE_FIGHTING_CHALLENGE,										// 决斗场 - 挑衅
	TASK_OPERATOR_TYPE_XIANNV_UP_ZIZHI,											// 女神提升资质
	TASK_OPERATOR_TYPE_RUNE_UP_LEVEL,											// 符文升级
	TASK_OPERATOR_TYPE_CROSS_1V1,												// 进入跨服1V1
	TASK_OPERATOR_TYPE_CATCH_FISH,												// 池塘捕鱼
	TASK_OPERATOR_TYPE_FEEDING_FISH,											// 池塘养鱼
	TASK_OPERATOR_TYPE_JINGLING_UP_LEVEL,										// 精灵成长升级
	TASK_OPERATOR_TYPE_JINGLING_WUXING_UP_LEVEL,								// 精灵悟性升级
	TASK_OPERATOR_TYPE_CHAT_ON_WORLD_CHANNEL,									// 在世界频道发言
	TASK_OPERATOR_TYPE_1V1_FIELD,												// 1V1竞技场
	TASK_OPERATOR_TYPE_FOOTPRINT_UP_STAR,										// 足迹升星
	TASK_OPERATOR_TYPE_JINGLINT_CHOUHUN,										// 精灵寻宝
	TASK_OPERATOR_TYPE_WING_UPGRADE,											// 羽翼进阶
	TASK_OPERATOR_TYPE_HALO_UPGRADE,											// 光环进阶
	TASK_OPERATOR_TYPE_SHENGONG_UPGRADE,										// 神弓进阶
	TASK_OPERATOR_TYPE_SHENYI_UPGRADE,											// 神翼进阶
	TASK_OPERATOR_TYPE_PUSH_GUILD_SKILL_UP,										// 点击仙盟技能升级
	TASK_OPERATOR_TYPE_PUT_ON_ZHUANZHI_EQUIP,									// 穿戴转职装备

	TASK_OPERATOR_TYPE_MAX,
};

enum TASK_SATISFY_STATUS_TYPE
{
	TASK_SATISFY_STATUS_TYPE_MOUNT_GRADE = 1,									// 坐骑阶级达到n级
	TASK_SATISFY_STATUS_TYPE_WING_GRADE,										// 羽翼阶级达到n级
	TASK_SATISFY_STATUS_TYPE_HALO_GRADE,										// 光环阶级达到n级
	TASK_SATISFY_STATUS_TYPE_SHENGONG_GRADE,									// 神弓阶级达到n级
	TASK_SATISFY_STATUS_TYPE_SHENYI_GRADE,										// 神翼阶级达到n级
	TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_GRADE,									// 战斗坐骑阶级达到n级

	TASK_SATISFY_STATUS_TYPE_MOUNT_STAR,										// 坐骑星级达到n级
	TASK_SATISFY_STATUS_TYPE_WING_STAR,											// 羽翼星级达到n级
	TASK_SATISFY_STATUS_TYPE_HALO_STAR,											// 光环星级达到n级
	TASK_SATISFY_STATUS_TYPE_SHENGONG_STAR,										// 神弓星级达到n级
	TASK_SATISFY_STATUS_TYPE_SHENYI_STAR,										// 神翼星级达到n级
	TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_STAR,									// 战斗坐骑星级达到n级

	TASK_SATISFY_STATUS_TYPE_JINGLING_MAX_LEVEL,								// 精灵最大达到n级
	TASK_SATISFY_STATUS_TYPE_PUT_JINLING_COUNT,									// 装备精灵数量达到n只

	TASK_SATISFY_STATUS_TYPE_PATA_FB_LEVEL,										// 爬塔副本达到n层
	TASK_SATISFY_STATUS_TYPE_EXP_FB_WAVE,										// 经验副本进入n次

	TASK_SATISFY_STATUS_TYPE_OWN_FRIEND,										// 拥有好友达到n个
	TASK_SATISFY_STATUS_TYPE_CHENGJIU_TITLE,									// 成就成号达到n级

	TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STRENGTH_LEVEL,						// 全身装备强化总等级达到n级
	TASK_SATISFY_STATUS_TYPE_EQUIP_TOTAL_STAR_LEVEL,							// 全身装备星星总等级达到n级

	TASK_SATISFY_STATUS_TYPE_ACTIVE_XIANNV,										// 激活女神n个

	TASK_SATISFY_STATUS_TYPE_INLAY_STONE,										// 镶嵌一个n级的宝石
	TASK_STATIFY_STATUS_TYPE_TUIFU_NORMAL_FB,									// 推图普通副本通关x章x节
	TASK_STATIFY_STATUS_TYPE_TUIFU_HARD_FB,										// 推图精英副本通关x章x节

	TASK_SATISFY_STATUS_TYPE_FOOTPRINT_GRADE,									// 足迹阶级达到n级
	TASK_SATISFY_STATUS_TYPE_FOOTPRINT_STAR,									// 足迹星级达到n级

	TASK_SATISFY_STATUS_TYPE_RED_EQUIP_WEAR_COUNT,								// 身上大于m阶的红装数量达到n件

	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_TIANMING,								    // 转职点亮天命到n级  五转   c_param2代表几级
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_CANGLONG,								    // 转职点亮苍龙到n级  六转
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENGXIAO,								// 转职点亮生肖到n级  七转
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_YUANQI,								    // 转职点亮元气到n级  八转
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENQI,								    // 转职点亮神器到n级  九转
	TASK_SATISFY_STATUS_TYPE_LIGHT_UP_XINGXIU,								    // 转职点亮星宿到n级  十转
	TASK_SATISFY_STATUS_TYPE_CON_TO_PROF6,								        // 达到转职六转的条件
	TASK_SATISFY_STATUS_TYPE_ONE_ZHUAN,											// 完成n转
	TASK_SATISFY_STATUS_TYPE_PUT_ON_ZHUANZHI_EQUIP,								// 穿戴n件转职装备
	TASK_SATISFY_STATUS_TYPE_JINGJIE_LEVEL,										// 境界达到n级
	TASK_SATISFY_STATUS_TYPE_PHASE_FB_LEVEL,									// 进阶副本达到n层
	TASK_SATISFY_STATUS_TYPE_PUT_ON_EQUIP,										// 穿戴n件m级装备
	TASK_SATISFY_STATUS_TYPE_TOWER_DEFEND_FB_ENTER_COUNT,						// 个人塔防副本进入n次
	TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE,									// 击杀n类型boss m 只
	
	TASK_SATISFY_STATUS_TYPE_MAX,
};

static const int MAX_NEXT_TASK_COUNT = 100;


#pragma pack(push, 4)

struct TaskInfo
{
	TaskInfo() : task_id(0), task_ver(0), task_condition(0), progress_num(0), reserve_sh3(0), accept_time(0), reserve_int1(0), reserve_int2(0) {}

	unsigned short task_id;
	char task_ver;
	char task_condition;
	short progress_num;
	short reserve_sh3;
	unsigned int accept_time;
	int reserve_int1;
	int reserve_int2;
};

struct TaskParam
{
	static const int MAX_ACCEPTED_TASK_NUM = 40;

	TaskParam() { this->Reset(); }

	void Reset()
	{
		reserve_sh1 = reserve_int2 = reserve_int = 0;
		task_count = 0;
		memset(taskinfo_list, 0, sizeof(taskinfo_list));
	}

	TaskInfo * GetTaskInfo(unsigned short task_id, int *t_index = NULL)
	{
		for (int i = 0; i < task_count; ++i)
		{
			if (taskinfo_list[i].task_id == task_id)
			{
				if (NULL != t_index) *t_index = i;
				return &taskinfo_list[i];
			}
		}

		return NULL;
	}

	bool AddTask(const TaskInfo &task_info)
	{
		if (task_count >= MAX_ACCEPTED_TASK_NUM)
		{
			return false;
		}

		taskinfo_list[task_count ++] = task_info;
		return true;
	}

	void RemoveByID(unsigned short task_id)
	{
		for (int i = 0; i < task_count; ++i)
		{
			if (taskinfo_list[i].task_id == task_id)
			{
				taskinfo_list[i] = taskinfo_list[task_count - 1];
				-- task_count;
				return;
			}
		}
	}

	void RemoveByIndex(int t_index)
	{
		if (t_index >= 0 && t_index < task_count)
		{
			taskinfo_list[t_index] = taskinfo_list[task_count - 1];
			-- task_count;
		}
	}

	int reserve_int;
	int reserve_int2;
	short reserve_sh1;
	short task_count;
	TaskInfo taskinfo_list[MAX_ACCEPTED_TASK_NUM];
};

typedef char TaskParamHex[sizeof(TaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TaskParamHex) < 2048);


static const int MAX_TASK_RECORD_NUM = 1000;
static const int MAX_TASK_RECORD_BUFF_LEN = sizeof(short) + sizeof(short) * MAX_TASK_RECORD_NUM;

//struct TaskRecordParam
//{
//	short task_count;
//	unsigned short task_item_list[MAX_TASK_RECORD_NUM];
//};
//UNSTD_STATIC_CHECK(sizeof(TaskRecordParam) * 2 < 4096);
//////////////////////////////////////////////////////////  仙榜任务相关  /////////////////////////////////////////////////////////////////
static const int XIANBANG_LEVEL_INTERVAL_MAX = 40;										// 仙榜任务 等级间隔最大 大小 

static const int XIANBANG_OPEN_LEVEL = 41;												// 仙榜任务 开放等级 
static const int XIANBANG_ONEDAY_ACCEPT_MAX = 10;										// 仙榜任务 每日最大领取次数
static const int XIANBANG_ACCEPTED_LIST_MAX = 3;										// 仙榜任务 已接任务列表 大小
static const int XIANBANG_FLUSH_LIST_MAX = 5;											// 仙榜任务 刷新任务列表 大小

static const int XIANBANG_FREE_FLUSH_INTERVAL_S = 30 * 60;								// 仙榜任务 免费刷新间隔

struct XianBangTaskItem
{
	XianBangTaskItem() { this->Reset(); } 

	void Reset() { task_id = 0; task_color = 0; unit_type = 0; }
	bool Invalid() { return 0 == task_id; }

	UInt16 task_id;																		// 任务ID
	char task_color;																	// 任务颜色
	char unit_type;																		// 任务类型
};

struct XianBangTaskParam
{
	XianBangTaskParam() { this->Reset(); }

	void Reset()
	{
		next_free_flush_time = 0;

		memset(accepted_list, 0, sizeof(accepted_list));
		memset(flush_list, 0, sizeof(flush_list));
	}

	unsigned int next_free_flush_time;													// 下次免费刷新时间
	XianBangTaskItem accepted_list[XIANBANG_ACCEPTED_LIST_MAX];							// 已接任务列表
	XianBangTaskItem flush_list[XIANBANG_FLUSH_LIST_MAX];								// 刷新列表
};

typedef char XianBangTaskParamHex[sizeof(XianBangTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianBangTaskParamHex) < 128);

///////////////////////////////////////////////////////////  日常任务相关  /////////////////////////////////////////////////////////////
static const int DAILY_LEVEL_INTERVAL_MAX = 60;											// 
static const int DAILY_LEVEL_TEMP_TASK_MAX = 60;										// 

static const int DAILY_ONEDAY_COMMIT_MAX = 10;											// 每日可提交日常任务最大数量
static const int DAILY_GOLD_COMMIT_REWARD_TIMES = 2;									// 元宝提交任务奖励倍数

enum DAILY_TASK_REWARD_TYPE
{
	DAILY_TASK_REWARD_TYPE_VALID = 0,

	DAILY_TASK_REWARD_TYPE_NORMAL,		// 单倍奖励
	DAILY_TASK_REWARD_TYPE_DOUBLE,		// 双倍奖励
	DAILY_TASK_REWARD_TYPE_TREBLE,		// 三倍奖励

	DAILY_TASK_REWARD_TYPE_MAX,
};

///////////////////////////////////////////////////////////  跑环任务相关  /////////////////////////////////////////////////////////////
static const int PAOHUAN_ONEDAY_COMMIT_MAX = 100;										// 每日可提交跑环任务最大数量
static const int PAOHUAN_LEVEL_INTERVAL_MAX = 60;										// 跑环任务等级区间个数
static const int PAOHUAN_LEVEL_TEMP_TASK_MAX = 60;										// 

///////////////////////////////////////////////////////////  护送任务相关  /////////////////////////////////////////////////////////////
static const int HUSONG_ONEDAY_ACCEPT_MAX = 3;											// 护送每日最大次数
static const int HUSONG_LEVEL_REWARD_CFG_MAX_COUNT = 110;								// 护送任务 等级奖励配置最大值
static const int HUSONG_BUY_TIMES_CFG_MAX_COUNT = 32;									// 护送任务 购买次数配置最大值
static const int HUSONG_TASK_LIMIT_TIME_S = 900;										// 护送任务持续时间

struct HusongTaskParam
{
	HusongTaskParam() { this->Reset(); }

	void Reset() { task_id = 0; task_color = 0; is_first_refresh = 0; accpet_activitytime = 0; refresh_count = 0; is_use_hudun = false; reserve_1 = 0; reserve_2 = 0; }
	void Clear() { task_id = 0; task_color = 0; accpet_activitytime = 0; is_use_hudun = 0;}
	bool Invalid() { return 0 == task_id; }
	bool IsFirstRefresh() { return 0 == is_first_refresh; }

	UInt16 task_id;																		// 任务ID 
	char task_color;																	// 任务颜色
	char is_first_refresh;																// 刷新次数
	unsigned int accpet_activitytime;													// 接受任务时 活动时间
	int refresh_count;																	// 当次任务刷新次数
	char is_use_hudun;																	// 是否使用过护盾
	char reserve_1;
	short reserve_2;
};

typedef char HusongTaskParamHex[sizeof(HusongTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HusongTaskParamHex) < 64);



//////////////////////////////////////////////////////////  天尊卡  废弃  /////////////////////////////////////////////////////////////////

static const int FAIRY_BUDDHA_CARD_START_LEVEL = 150;								    // 仙尊卡开启等级

static const int FAIRY_BUDDHA_CARD_START_DAY = 2;										// 仙尊卡开启时间（开服第几天）

static const int FAIRY_BUDDHA_CARD_CONTINUE_TIME = 60 * 60 * 24 * 30;					// 仙尊卡持续时间为30天

static const int  FAIRY_BUDDHA_CARD_COUNT = 3;                                          // 仙尊卡激活三次变为永久激活  

enum FairyBuddhaCardType
{
	FAIRY_BUDDHA_CARD_INVALID = 0,
	FAIRY_BUDDHA_CARD_BRONZE,             //青铜卡
	FAIRY_BUDDHA_CARD_SILVER,             //白银卡
	FAIRY_BUDDHA_CARD_JEWEL,              //钻石卡
	FAIRY_BUDDHA_CARD_ALL,                //一键激活
	FAIRY_BUDDHA_CARD_MAX,
};

struct FairyBuddhaCard
{
	FairyBuddhaCard() { this->Reset(); }
	void Reset() {
		bronze_timestamp = 0;
		silver_timestamp = 0;
		jewel_timestamp = 0;
		is_forever_open = 0;
		fairy_buddha_card_is_activate = 0;
		bronze_activate_count = 0;
		silver_activate_count = 0;
		jewel_activate_count = 0;
		gold_bind_is_get_flage = 0;
	}
	unsigned int bronze_timestamp;       // 青铜卡时间戳
	unsigned int silver_timestamp;       // 白银卡时间戳
	unsigned int jewel_timestamp;        // 钻石卡时间戳
	char is_forever_open;                // 是否永久激活
	char fairy_buddha_card_is_activate;  // 仙尊卡是否激活
	unsigned short bronze_activate_count;// 青铜卡激活次数
	unsigned short silver_activate_count;// 白银卡激活次数
	unsigned short jewel_activate_count; // 钻石卡激活次数
	int gold_bind_is_get_flage;          // 每日绑元是否领取
};

//////////////////////////////////////////////////////////  仙盟任务相关  /////////////////////////////////////////////////////////////////
static const int MAX_GUILD_TASK_COLOR = 5;												// 最大颜色

static const int MAX_GUILD_REWARD_ITEM_COUNT = 64;

struct GuildTaskParam
{
	GuildTaskParam() { this->Reset(); }

	void Reset()
	{
		
		double_reward_flag = 0;
		is_finish = 0;
		first_task = 0;
		complete_task_count = 0;
		guild_task_max_count = 0;
	}

	char double_reward_flag;													// 双倍奖励领取标记
	char is_finish;																// 仙盟所有任务是否完成
	short complete_task_count;													// 总共的任务次数统计

	int first_task;                  											// 第一个任务ID

	int guild_task_max_count;													// 仙盟任务最大个数

};

typedef char GuildTaskParamHex[sizeof(GuildTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildTaskParamHex) < 128);

//////////////////////////////////////////////////////////  约会任务相关  /////////////////////////////////////////////////////////////////
struct DatingTaskParam
{
	DatingTaskParam() { this->Reset(); }

	void Reset()
	{
		task_id = 0;
	}

	int task_id;															// 已接任务ID
};

typedef char DatingTaskParamHex[sizeof(DatingTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(DatingTaskParamHex) < 32);

//////////////////////////////////////////////////////////  转职任务相关  ////////////////////////////////////////////////////////////////
static const int ROLE_ZHUANZHI_TASK_TOTAL_COUNT_MAX = 33;					//角色总转职任务最大个数

struct ZhuanzhiTaskParam
{
	ZhuanzhiTaskParam() { this->Reset(); }

	void Reset()
	{
		first_task = 0;
		reserve_sh = 0;
		complete_task_count = 0;
	}

	short complete_task_count;													// 总共的任务次数统计
	short reserve_sh;
	int first_task;																// 第一个任务ID
};

typedef char ZhuanzhiTaskParamHex[sizeof(ZhuanzhiTaskParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhuanzhiTaskParamHex) < 64);

#pragma pack(pop)

#endif // __TASK_DEF_HPP__



