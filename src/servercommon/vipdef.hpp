#ifndef __VIP_DEF_HPP__
#define __VIP_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

enum VIP_TYPE
{
	VIP_NONE = 0,															// 非VIP

	VIP_EXPERIENCE,															// 体验卡
	VIP_WEEK,																// 周卡
	VIP_MON,																// 月卡
	VIP_HALF_YEAR,															// 半年卡

	VIP_COUNT,
};

enum VIP_EXP_ADDON_TYPE
{
	VIP_EXP_ADDON_TYPE_INVALID = 0,


	VIP_EXP_ADDON_TYPE_LOGIN,												// 每天登录	 加VIP经验 
	VIP_EXP_ADDON_TYPE_ONLINE,												// 每小时在线 加VIP经验
	VIP_EXP_ADDON_TYPE_CONSUME,												// 消费1元宝  加VIP经验
	VIP_EXP_ADDON_TYPE_FIRST_BE_VIP,										// 第一次成为VIP 加VIP经验

	VIP_EXP_ADDON_TYPE_MAX,
};

enum VIP_WELFARE_TYPE
{
	VIP_WELFARE_NONE = 0,

	VIP_WELFARE_DAY,														// 领取日福利
	VIP_WELFARE_WEEK,														// 领取周福利
	VIP_WELFARE_BUFF,														// 领取vip祝福

	VIP_WELFARE_COUNT,
};

enum VIP_AUTH_TYPE
{
	VAT_TRANSFER = 0,											// 传送
	VAT_FINISH_ALL_DAILY_TASK = 1,								// 一键完成日常
	VAT_BUY_HUSONG_TIMES = 2,									// 购买护送次数
	VAT_RA_LOGIN_GIFT_VIP_REWARD = 3,							// 随机活动登录送礼vip礼包
	VAT_STRENGTH_SUC_ADD_RATE = 4,								// 强化成功率+X%
	VAT_VIP_EXTRA_EXP = 5,										// VIP额外经验加成
	//VAT_ENTER_BOSS_HOME = 6,									// 能否进入BOSS之家
	VAT_BUY_SYT_TIMES = 7,										// 可购买锁妖塔次数
	VAT_BUY_YSGC_TIMES = 8,										// 可购买妖兽广场次数
	VAT_BUY_FREE_REVIVAL_TIMES = 9,								// 免费复活次数
	VAT_FOLLOW_STORE = 10,										// 随身仓库
	VAT_FOLLOW_SHOP = 11,										// 随身药店
	VAT_OFFLINE_EXP_RATE_2 = 12,								// 离线经验2倍领取
	VAT_FB_EXP_BUY_TIMES = 13,									// 经验本购买次数
	VAT_FB_COIN_BUY_TIMES = 14,									// 铜币本购买次数 
	VAT_FB_1V1_BUY_TIMES = 15,									// 1v1购买次数
	VAT_WABAO_BUY_JOIN_TIMES = 16,								// 挖宝购买次数
	VAT_FB_DAOJU_BUY_TIMES = 17,								// 道具本购买次数
	VAT_FB_MENTALITY_CLEAN_CD = 18,								// 经脉清CD
	VAT_OFFLINE_EXP_RATE_3 = 19,								// 离线经验3倍领取
	VAT_GUILD_BOX_COUNT = 20,									// 仙盟宝箱数量
	VAT_FB_STROY_COUNT = 21,									// 剧情副本购买次数
	VAT_FB_PAHSE_COUNT = 22,									// 阶段副本购买次数
	VAT_FB_HOTSPRINT_EXTRA_REWARD = 23,							// 温泉活动经验额外加成(百分比加成)
	VAT_FB_TEAM_EQUIP_COUNT= 24,								// 组队装备副本购买掉落次数
	VAT_FB_MIKU_BOSS_BUY_WEARY = 25,							// 秘窟boss购买疲劳值
	VAT_TOWERDEFEND_FB_FREE_AUTO_TIMES = 26,					// 塔防免费扫荡次数
	VAT_TOWERDEFEND_FB_BUY_TIMES = 27,							// 塔防购买次数（防具）

	VAT_XIANNV_SHENGWU_AUTO_FETCH = 28,							// 女神法则（圣物）自动选择碎片
	VAT_XIANNV_SHENGWU_MULTI_CHOU = 29,							// 女神法则（圣物）十次抽
	VAT_FIGHTING_DAY_BUY_CHALLENGE_TIMES = 30,					// 决斗场 - 挑衅购买次数
	VAT_TUITU_FB_NORMAL_BUY_TIMES = 31,							// 推图普通副本购买次数
	VAT_TUITU_FB_HARD_BUY_TIMES = 32,							// 推图精英副本购买次数
	VAT_FIGHTING_MINING_BUY_TIMES = 33,							// 决斗场 - 挖矿购买次数
	VAT_FIGHTING_SAILING_BUY_TIMES = 34,						// 决斗场 - 航海购买次数
	VAT_NEQ_FB_BUY_TIMES = 35,									// 武器推图本(新装备本)购买次数
	VAT_BUILD_TOWER_FB_BUY_TIMES = 36,							// 建塔本购买次数
	VAT_BABY_BOSS_BUY_ENTER_TIMES = 37,							// 宝宝boss进入次数
	VAT_ARMOR_DEFEND_FB_BUY_TIMES = 38,							// 防具材料本购买次数(塔防)
	VAT_SHANGGU_BOSS_BUY_ENTER_TIMES = 39,						// 上古boss进入次数
	VAT_SHANGGU_BOSS_EXTRA_TIRE_VALUE = 40,                     // 上古boss额外疲劳值
	VAT_PERSON_BOSS_ENTER_TIMES = 41,                           // 个人boss进入次数
	VAT_DABAO_BOSS_ENTER_TIMES = 43,                            // 套装boss进入次数
	VAT_VIP_OPEN_KNAPSACK_GRID = 44,							// VIP开启背包格子
	VAT_PERSON_BOSS_BUY_TIMES = 45,                             // 个人boss购买次数
	VAT_CHALLENGE_FB_BUY_TIMES = 46,							// 品质本购买次数
	VAT_GUILD_TASK_ONE_KEY = 47,								// 一键仙盟任务权限
	VAT_PAOHUAN_TASK_ONE_KEY = 48,								// 一键跑环任务权限
	VAT_MAX = 49,
};

enum QIFU_TYPE												// 祈福类型
{
	QIFU_TYPE_INVALID = 0,

	QIFU_TYPE_BUYCOIN,										// 祈福模块中的摇钱
	QIFU_TYPE_BUYYUANLI,									// 祈福模块中的摇元力
	QIFU_TYPE_BUYXIANHUN,									// 祈福模块中的摇精魂

	QIFU_TYPE_MAX,
};

static const int MAX_VIP_LEVEL = 20;						// VIP最高等级为10级
static const int VIP_TIMEOUT_REMIDE_DAYS = 3;				// VIP提醒天数为3天

#pragma pack(push, 4)

struct VipParam
{
	VipParam() { this->Reset(); }

	void Reset()
	{
		vip_level = 0;
		fetch_qifu_buycoin_reward_flag = 0;
		gold_buycoin_times = 0;
		gold_buyyuanli_times = 0;
		gold_buyxianhun_times = 0;
		vip_exp = 0;
		fetch_level_reward_flag = 0;
		fetch_qifu_buyyuanli_reward_flag = 0;
		fetch_qifu_buyxianhun_reward_flag = 0;
		free_buycoin_times = 0;
		free_buyyuanli_times = 0;
		free_buyxianhun_times = 0;

		qifu_last_free_buycoin_timestamp = 0;
		qifu_last_free_buyyuanli_timestamp = 0;
		qifu_last_free_buyxianhun_timestamp = 0;

		vip_week_gift_times = 0;
		time_limit_vip_time = 0;

		reward_vip_exp_flag = 0;
		reserve_sh = 0;
	}

	char vip_level;
	char fetch_qifu_buycoin_reward_flag;
	short gold_buycoin_times;

	short gold_buyyuanli_times;
	short gold_buyxianhun_times;
	int vip_exp;
	int fetch_level_reward_flag;

	char fetch_qifu_buyyuanli_reward_flag;
	char fetch_qifu_buyxianhun_reward_flag;
	short free_buycoin_times;
	short free_buyyuanli_times;
	short free_buyxianhun_times;

	unsigned int qifu_last_free_buycoin_timestamp;
	unsigned int qifu_last_free_buyyuanli_timestamp;
	unsigned int qifu_last_free_buyxianhun_timestamp;

	int vip_week_gift_times;
	
	unsigned int time_limit_vip_time;

	short reward_vip_exp_flag;
	short reserve_sh;

};

typedef char VipParamHex[sizeof(VipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(VipParamHex) < 128);

#pragma pack(pop)

#endif  // __MENTALITY_DEF_HPP__

