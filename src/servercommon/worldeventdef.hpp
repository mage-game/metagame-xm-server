#ifndef __WORLD_EVENT_DEF_HPP__
#define __WORLD_EVENT_DEF_HPP__

#include "servercommon/servercommon.h"

static const int MAX_WORLD_EVENT_SCORE = 2000000000;																	// 世界事件分数最大值 20亿
const static int WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT = 1;															// NPC存在的场景数
const static int WORLD_EVENT_NPC_COUNT_PER_SCENE = 24;																	// 每个场景存在的NPC数量
const static int WORLD_EVENT_NPC_MAX_COUNT = WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT * WORLD_EVENT_NPC_COUNT_PER_SCENE;	// 最多存在的NPC数量

const static int WORLD_EVENT_NPC_TYPE_MAX_COUNT = 8;																	// NPC类型数
const static int WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE = 3;																// NPC存在数量
UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_TYPE_MAX_COUNT <= 16);

const static int WORLD_EVENT_POS_CONFIG_MAX_COUNT_PER_SCENE = 64;														// 每个场景坐标配置最大数
const static int WORLD_EVENT_POS_CONFIG_MAX_COUNT = 64;																	// 坐标配置最大数

UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_MAX_COUNT == WORLD_EVENT_NPC_TYPE_MAX_COUNT * WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE);

const static int WORLD_EVENT_LOGIC_OPEN_MAX_TIMES_PER_DAY = 16;															// 事件逻辑一天最多开启次数
const static int WORLD_EVENT_OBJ_TYPE_MAX_COUNT = 32;																	// 世界事件对象类型最大数

const static int WORLD_EVENT_RANDOM_BOSS_POS_COUNT = 32;																// 随机boss最多在32个点刷新
const static int WORLD_EVENT_RANDOM_BOSS_TYPE_MAX = 8;																	// 随机boss总共有8种
const static int WORLD_EVENT_RANDOM_FLUSH_BOSS_MAX = 16;																// 最多同时刷16个boss

const static int WORLD_EVENT_PKROLE_POS_COUNT = 32;																		// PK玩家最多在32个点刷新
const static int WORLD_EVENT_PKROLE_MAX = 16;																			// 最多同时刷N个PK对象
const static int WORLD_EVENT_PKROLE_MAX_RANKPOS = 12;																	// 参与PK玩家在排行榜最大的位置 e
const static int WORLD_EVENT_PKROLE_PER_FLUSH_NUM = 3;																	// 每次刷3个

const static int WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX = 12;																// 最多同时有8个虚空之门
const static int WORLD_EVENT_XUKONGZHIMENG_MONSTER_MAX = 16;															// 怪有16种
const static int WORLD_EVENT_XUKONGZHIMENG_DISAPPER_POS_MAX = 4;														// 每个虚空之门 消失点的最大个数 也就是路径个数

const static int WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX = 4;																// 最多同时有4个塔
const static int WORLD_EVENT_QUANMINGTAFANG_MONSTER_MAX = 16;															// 怪有16种
const static int WORLD_EVENT_QUANMINGTAFANG_START_POS_MAX = 4;															// 每个塔 有4个开始路径点

const static int WORLD_EVENT_TITLE_WORD_MAX_NUM = 8;																	// 需要点亮的文字最大数
UNSTD_STATIC_CHECK(WORLD_EVENT_TITLE_WORD_MAX_NUM <= 8);

const static int WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT = 20;															// 世界事件神魔之隙传送门数量

const static int WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT = 4;															// 天女散花 怪物数量
const static int WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT = 13;													// 天女散花 怪物移动位置最大数量
const static int WORLD_EVENT_TIANNVSHANHUA_FALLING_ITEM_MAX_COUNT = 64;													// 天女山花 掉落物品配置
const static int WORLD_EVENT_TIANNVSHANHUA_REFRESH_FALLING_ITEM_INTERVAL_S = 20;										// 天女散花 散花间隔
const static int WORLD_EVENT_TIANNVSHANHUA_MONSTER_REST_TIME_S = 4;														// 天女散花	怪物休息时间

const static int WORLD_EVENT_REWARD_GRADE_COUNT = 4;																	// 世界事件 等级阶数

enum WORLD_EVENT_LOGIC_TYPE 
{
	WORLD_EVENT_LOGIC_TYPE_INVALID = 0,						
	WORLD_EVENT_LOGIC_TYPE_STAND ,																						// 准备开始
	WORLD_EVENT_LOGIC_TYPE_START,																						// 逻辑开始
	WORLD_EVENT_LOGIC_TYPE_STOP,																						// 逻辑结束
	WORLD_EVENT_LOGIC_TYPE_MAX,		
};

enum WORLD_EVENT_TYPE 
{
	WORLD_EVENT_TYPE_INVALID = 0,

	WORLD_EVENT_TYPE_QUANMINTAFANG,																						// 全民塔防
	WORLD_EVENT_TYPE_SHENMOZHIXI,																						// 神魔之隙
	WORLD_EVENT_TYPE_XUKONGZHIMEN,																						// 虚空之门
	WORLD_EVENT_TYPE_KILL_RANDOM_BOSS,																					// 击杀随机boss
	WORLD_EVENT_TYPE_FIND_NPC,																							// 寻找NPC
	WORLD_EVENT_TYPE_JIQINGPK,																							// 激情PK
	WORLD_EVENT_TYPE_TIANNVSANHUA,																						// 天女散花

	WORLD_EVENT_TYPE_MAX,
};

enum WORLD_EVENT_OBJ_TYPE
{
	WORLD_EVENT_OBJ_INVALID = 0,

	WORLD_EVENT_OBJ_NPC,																								// NPC对象
	WORLD_EVENT_OBJ_SHENMOZHIXI,																						// 神魔之井对象
	WORLD_EVENT_OBJ_XUKONGZHIMENG,																						// 时空之门对象
	WORLD_EVENT_OBJ_TOWER,																								// 塔对象	
	WORLD_EVENT_OBJ_ZHUAGUI_NPC,																						// 抓鬼NPC	
																														
	WORLD_EVENT_OBJ_MAX,
};

struct WorldEventParam
{
	WorldEventParam() { this->Reset(); }

	void Reset()
	{
		curr_event_type = 0;
		jiqingpk_flush_num = 0;
		reserve_int_1 = 0;
		reserve_int_2 = 0;
		memset(score_list, 0, sizeof(score_list));
	}

	int curr_event_type;																								// 当前时间类型
	int jiqingpk_flush_num;																								// 激情PK活动当前刷了多少个
	int reserve_int_1;																									// 保留
	int reserve_int_2;																									// 保留
	int score_list[WORLD_EVENT_TYPE_MAX];																				// 时间积分
};

#endif // __WORLD_EVENT_DEF_HPP__


