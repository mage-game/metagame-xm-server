#ifndef __DAY_COUNTER_HPP__
#define __DAY_COUNTER_HPP__

#include <string>
#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "config/fbconfig.hpp"

class Role;

// 提供统一的每日限制次数的逻辑，目前只用于需要限制次数的副本，接下来慢慢把每日限次逻辑合进来


// 目前需要限次的逻辑有
//			1: 副本 (0-127)
//			2: 其他

// 目前允许的最大次数限制是25次	A ~ Z (在daycountrecorder.cpp中定义)

class DayCounter
{
public:
	DayCounter();
	~DayCounter();

	void SetRole(Role *role) { m_role = role; }

	enum
	{
		DAYCOUNT_ID_FB_START = 0,											// 副本开始
		DAYCOUNT_ID_FB_XIANNV = 1,											//
		DAYCOUNT_ID_FB_COIN = 2,
		DAYCOUNT_ID_FB_WING = 3,
		DAYCOUNT_ID_FB_XIULIAN = 4,
		DAYCOUNT_ID_FB_QIBING = 5,
		DAYCOUNT_ID_FB_EXP = 6,												// 经验本
		DAYCOUNT_ID_FB_PATA = 7,											// 爬塔
		DAYCOUNT_ID_FB_STORY = 8,											// 剧情本										
		DAYCOUNT_ID_FB_PHASE_MOUNT = 9,										// 进阶本-坐骑
		DAYCOUNT_ID_FB_PHASE_SHIZHUANG_BODY = 10,							// 进阶本-时装
		DAYCOUNT_ID_FB_PHASE_SHIZHUANG_WUQI = 11,							// 进阶本-神兵
		DAYCOUNT_ID_FB_PHASE_LINGTONG = 12,									// 进阶本-灵童
		DAYCOUNT_ID_FB_PHASE_FIGHT_MOUNT = 13,								// 进阶本-战骑
		DAYCOUNT_ID_FB_PHASE_LINGGONG = 14,									// 进阶本-灵弓
		DAYCOUNT_ID_FB_PHASE_LINGQI = 15,									// 进阶本-灵骑

		DAYCOUNT_ID_FB_END = MAX_FB_NUM - 1,								// 副本结束+

		DAYCOUNT_ID_EVALUATE = 61,											// 评价次数
		DAYCOUNT_ID_DAILYFB_EXP_FREE_TIMES = 62,							// 日常经验副本免费次数
		DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT = 63,							// 护送任务 领取个数
		DAYCOUNT_ID_SHUIJING_GATHER = 64,									// 水晶采集 次数
		DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES = 65,							// 妖兽祭坛参加次数
		DAYCOUNT_ID_FREE_CHEST_BUY_1 = 66,									// 一次寻宝免费次数
		DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT = 67,							// 日常任务 提交个数
		DAYCOUNT_ID_HUSONG_ROB_COUNT = 68,									// 护送抢劫次数
		DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT = 69,							// 护送任务 vip购买次数
		DAYCOUNT_ID_HUSONG_REFRESH_COLOR_FREE_TIMES = 70,					// 护送任务 免费刷新次数
		DAYCOUNT_ID_DAILYFB_COIN_FREE_TIMES = 71,							// 日常铜币副本免费次数
		DAYCOUNT_ID_GUILD_TASK_COMPLETE_COUNT = 72,							// 仙盟任务完成次数
		DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES = 73,			// 日常领取全部任务完成奖励次数
		DAYCOUNT_ID_ANSWER_QUESTION_COUNT = 74,								// 答题次数-旧版
		DAYCOUNT_ID_VIP_FREE_REALIVE = 75,									// VIP免费复活次数
		DAYCOUNT_ID_CHALLENGE_BUY_JOIN_TIMES = 76,							// 挑战副本购买参与次数
		DAYCOUNT_ID_CHALLENGE_FREE_AUTO_FB_TIMES = 77,						// 挑战副本免费扫荡次数
		DAYCOUNT_ID_BUY_ENERGY_TIMES = 78,									// 购买体力次数
		DAYCOUNT_ID_MAZE_JOIN_FLAG = 79,									// 迷宫寻宝参与标记
		DAYCOUNT_ID_WABAO = 80,												// 挖宝
		DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES = 82,						// 组队塔防参与次数 
		DAYCOUNT_ID_GCZ_DAILY_REWARD_TIMES = 83,							// 攻城战领取每日奖励次数
		DAYCOUNT_ID_XIANMENGZHAN_RANK_REWARD_TIMES = 84,					// 仙盟战排名奖励次数
		DAYCOUNT_ID_MOBAI_CHENGZHU_REWARD_TIMES = 85,						// 膜拜城主次数
		DAYCOUNT_ID_GUILD_ZHUFU_TIMES = 86,									// 仙盟运势祝福次数
		DAYCOUNT_ID_KILL_REDNAME_REWARD_TIMES = 87,							// 击杀红名玩家获取经验次数
		DAYCOUNT_KILL_OTHER_CAMP_COUNT = 88,								// 击杀其他阵营玩家 双倍奖励次数
		DAYCOUNT_ID_DATING_INVITE = 89,										// 约会邀请
		DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE = 90,							// 跨服3V3比赛次数
		DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD = 91,			// 跨服3V3每日奖励
		DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES = 92,							// 迷宫仙府参与次数
		DAYCOUNT_ID_JOIN_YAOSHOUGUANGCHANG = 93,							// 参加妖兽广场每日次数
		DAYCOUNT_ID_JOIN_SUOYAOTA = 94,										// 参加锁妖塔每日次数
		DAYCOUNT_ID_GATHAR_SELF_BONFIRE = 95,								// 采集自己仙盟篝火 每日次数
		DAYCOUNT_ID_GATHAR_BONFIRE_TOTAL = 96,								// 采集仙盟篝火 总次数
		DAYCOUNT_ID_DABAO_BOSS_BUY_COUNT = 97,								// 购买打宝地图进入次数
		DAYCOUNT_ID_DABAO_ENTER_COUNT = 98,									// 打宝地图进入次数
		DAYCOUNT_ID_WABAO_GATHAR_SUCC_COUNT = 99,							// 挖宝采集成功 最大次数
		DAYCOUNT_ID_DAILYFB_DAOJU_FREE_TIMES = 100,							// 道具副本免费次数
		DAYCOUNT_ID_JINGHUA_GATHER_COUNT = 101,								// 精华采集次数
		DAYCOUNT_ID_CAMP_GAOJIDUOBAO = 102,									// 师门高级夺宝
		DAYCOUNT_ID_FREE_CHEST_JINGLING_BUY_1 = 103,						// 精灵一次寻宝免费次数
		DAYCOUNT_ID_GUILD_REWARD = 104,										// 仙盟奖励
		DAYCOUNT_ID_GUILD_BONFIRE_ADD_MUCAI = 105,							// 仙盟篝火捐献木材次数
		DAYCOUNT_ID_ACTIVE_ENTER_COUNT = 106,								// 活跃boss地图进入次数
		DAYCOUNT_ID_JINGLING_SKILL_COUNT = 107,								// 精灵技能免费刷新次数
		DAYCOUNT_ID_BUY_MIKU_WERARY = 108,									// 购买秘窟疲劳值
		DAYCOUNT_ID_MONEY_TREE_COUNT = 109,									// 摇钱树抽奖次数
		DAYCOUNT_ID_JING_LING_HOME_ROB_COUNT = 110,							// 精灵之家掠夺次数
		DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE = 111,						// 精灵探险挑战次数
		DAYCOUNT_ID_JING_LING_EXPLORE_RESET = 112,							// 精灵探险重置次数
		DAYCOUNT_ID_NEQ_FREE_AUTO_FB = 113,									// 新装备本免费扫荡次数
		DAYCOUNT_ID_BUY_ACTIVE_BOSS_WERARY = 114,							// 购买活跃boss疲劳值
		DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES = 115,							// 上古boss进入次数
		DAYCOUNT_ID_PERSON_BOSS_ENTER_TIMES = 116,                          // 个人boss进入次数
		DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES = 117,							// 建塔本购买次数
		DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES = 118,						// 建塔本进入次数
		DAYCOUNT_ID_ROB_GUILD_BIAOCHE_COUNT = 119,							// 抢劫其他帮派镖车次数
		DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES = 120,                         // 精英组队副本参与次数
		DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT = 121,				// 奇遇boss击杀次数
		DAYCOUNT_ID_PERSON_BOSS_BUY_TIMES = 122,                            // 个人boss购买次数
		DAYCOUNT_ID_COMMIT_PAOHUAN_TASK_COUNT = 123,						// 跑环任务 提交个数

		DAYCOUNT_ID_MAX,
	};

	UNSTD_STATIC_CHECK(MAX_FB_NUM == 60);
	UNSTD_STATIC_CHECK(DAYCOUNT_ID_MAX <= MAX_DAYCOUNT_COUNT_ID);

	void Init(Role *role, const DayCountData daycount_data);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);	

	bool DayCountCheck(int day_count_id, int max_count);
	void DayCountIncrease(int day_count_id);
	void DayCountReduce(int day_count_id);
	void DayCountReset(int day_count_id);
	void DayCountSet(int day_count_id, int count);

	int GetDayCount(int day_count_id);
	void GetCountData(DayCountData day_count_data); 

	void SendDayCounterInfo();
	void GMResetDayCount();

private:
	void SendDayCounterItemInfo(int day_count_id);

	Role *m_role;

	DayCountData m_daycount_data;
};

#endif // __DAY_COUNTER_HPP__

