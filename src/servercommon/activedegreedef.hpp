#ifndef __ACTIVE_DEGREE_DEF_HPP__
#define __ACTIVE_DEGREE_DEF_HPP__

#include "servercommon/servercommon.h"

enum ACTIVEDEGREE_TYPE
{
	ACTIVEDEGREE_TYPE_ONLIME_TIME,								// 在线小时数
	ACTIVEDEGREE_TYPE_PHASE_FB,									// 进阶副本
	ACTIVEDEGREE_TYPE_EXP_FB,									// 经验副本
	ACTIVEDEGREE_TYPE_DAILY_TASK,								// 完成日常任务次数
	ACTIVEDEGREE_TYPE_GUILD_TASK,								// 公会任务
	ACTIVEDEGREE_TYPE_GUILD_BOX,								// 公会宝箱
	ACTIVEDEGREE_TYPE_HUSONG,									// 护送
	ACTIVEDEGREE_TYPE_FULI_BOSS,								// 福利BOSS
	ACTIVEDEGREE_TYPE_PATA_FB,									// 爬塔副本
	ACTIVEDEGREE_TYPE_TEAM_FB,									// 组队副本
	ACTIVEDEGREE_TYPE_GCZ,										// 攻城战
	ACTIVEDEGREE_TYPE_YSZC,										// 元素战场
	ACTIVEDEGREE_TYPE_GUILDBATTLE,								// 公会争霸
	ACTIVEDEGREE_TYPE_LINGTUZHAN,								// 领土战
	ACTIVEDEGREE_TYPE_WLTX,										// 王陵探险
	ACTIVEDEGREE_TYPE_TJCB,										// 天降财宝
	ACTIVEDEGREE_TYPE_DAFUHAO,									// 大富豪
	ACTIVEDEGREE_TYPE_WORLD_BOSS,								// 世界BOSS
	ACTIVEDEGREE_TYPE_RUNE_TOWER,								// 符文塔
	ACTIVEDEGREE_TYPE_WABAO,									// 挖宝
	ACTIVEDEGREE_TYPE_ACTIVE_BOSS,								// 杀活跃boss数
	ACTIVEDEGREE_TYPE_SONG_HUA,									// 送花数
	ACTIVEDEGREE_TYPE_VIP_BOSS,									// 杀vipboss数
	ACTIVEDEGREE_TYPE_XINGZUOYIJI_GATHER,						// 星座遗迹采集数
	ACTIVEDEGREE_TYPE_ANCIENT_YIJI_GATHER,						// 远古遗迹采集数
	ACTIVEDEGREE_TYPE_MIKU_BOSS,							    // 精英（秘窟）boss任务数
	ACTIVEDEGREE_TYPE_QINGYUAN_SHENGDI,							// 情缘圣地任务数
	ACTIVEDEGREE_TYPE_CHALLENGEFIELD_TIMES,						// 参与1v1竞技场次数
	ACTIVEDEGREE_TYPE_EQUIP_FB,									// 装备副本 材料武器
	ACTIVEDEGREE_TYPE_JOIN_FISHING,								// 参与钓鱼活动
	ACTIVEDEGREE_TYPE_PAOHUAN_TASK,								// 跑环任务

	ACTIVEDEGREE_TYPE_BUILD_TOWER_FB,							// 塔防副本
	ACTIVEDEGREE_TYPE_TOWERDEFEND_FB,							// 防具材料
	ACTIVEDEGREE_TYPE_CHALLENGE_FB,								// 挑战副本
	ACTIVEDEGREE_TYPE_ARMORDEFEND_FB,							// 守护副本
	ACTIVEDEGREE_TYPE_TOWERDEFEND_TEAM,							// 组队守护
	ACTIVEDEGREE_TYPE_EQUIP_TEAM_FB,							// 组队爬塔
	ACTIVEDEGREE_TYPE_JINGLING_GATHER,							// 宠物奇遇

	ACTIVEDEGREE_TYPE_MIZANG_BOSS,								// 密藏BOSS
	ACTIVEDEGREE_TYPE_CROSS_BOSS,								// 跨服BOSS(远古)
	ACTIVEDEGREE_TYPE_NUM,

	ACTIVEDEGREE_TYPE_MAX = 64,
};

UNSTD_STATIC_CHECK(ACTIVEDEGREE_TYPE_NUM <= ACTIVEDEGREE_TYPE_MAX);

static const int ACTIVEDEGREE_REWARD_ITEM_MAX_NUM = 8;			// 活跃奖励物品最大个数
static const int ACTIVEDEGREE_REWARD_LEVEL_MAX_NUM = 10;		// 活跃经验奖励等级范围最大划分个数
static const int ACTIVEDEGREE_REWARD_MAX_DAY = 31;				// 活跃按天奖励最大天数

UNSTD_STATIC_CHECK(0 == ACTIVEDEGREE_REWARD_ITEM_MAX_NUM % 4);

enum ACTIVEDEGREE_REWARD_STATUS
{
	ACT_DEGREE_REWARD_STATUS_CAN_NOT_FETCH = 0,			// 不可领取
	ACT_DEGREE_REWARD_STATUS_CAN_FETCH,					// 可领取
	ACT_DEGREE_RWARD_STATUS_BE_FETCHED,					// 已领取
};

#pragma pack(push, 4)

struct ActiveDegreeParam
{
	ActiveDegreeParam() { this->Reset(); }

	void Reset()
	{
		day_online_time = 0;
		total_degree = 0;
		memset(activedegree_list, 0, sizeof(activedegree_list));
		memset(reward_flag_list, 0, sizeof(reward_flag_list));
		activedegree_fetch_flag = 0;
		memset(reward_on_day_flag_list, 0, sizeof(reward_on_day_flag_list));
	}

	int day_online_time;
	int total_degree;
	char activedegree_list[ACTIVEDEGREE_TYPE_MAX];
	char reward_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];

	unsigned long long activedegree_fetch_flag;					// 活跃度领取标志
	char reward_on_day_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM]; // 根据每个月第几天获取的奖励标识
};

typedef char ActiveDegreeHex[sizeof(ActiveDegreeParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(ActiveDegreeHex) < 256);

#pragma pack(pop)

#endif // __ACTIVE_DEGREE_DEF_HPP__

