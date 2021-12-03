#ifndef __LOGDEF_H__
#define __LOGDEF_H__

#include "utility/logagent.h"

enum LOG_TYPE
{
	LOG_TYPE_INVALID = 0,
	LOG_TYPE_GOLD_ADD = 1,						// 增加元宝
	LOG_TYPE_GOLD_USE = 2,						// 使用元宝
	LOG_TYPE_GOLD_BIND_ADD = 3,					// 增加绑定元宝
	LOG_TYPE_GOLD_BIND_USE = 4,					// 使用绑定元宝
	LOG_TYPE_COIN_BIND_ADD = 5,					// 增加绑定铜钱
	LOG_TYPE_COIN_BIND_USE = 6,					// 使用绑定铜钱

	LOG_TYPE_ITEM_PUT = 7,						// 获得物品
	LOG_TYPE_ITEM_USE = 8,						// 使用物品
	LOG_TYPE_ITEM_DISCARD = 9,					// 丢弃物品
	LOG_TYPE_ITEM_CONSUME = 10,					// 消耗物品

	LOG_TYPE_PUBLIC_SALE_ADD = 11,				// 拍卖物品
	LOG_TYPE_PUBLIC_SALE_DEL = 12,				// 取消拍卖
	LOG_TYPE_PUBLIC_SALE_BUY = 13,				// 拍卖成功
	LOG_TYPE_PUBLIC_SALE_EXPIRED = 14,			// 拍卖过期

	LOG_TYPE_MAIL_ITEM_IN = 15,					// 邮件获得物品
	LOG_TYPE_MAIL_ITEM_OUT = 16,				// 邮件发送物品

	LOG_TYPE_FORBID_TALK = 17,					// 禁言
	LOG_TYPE_FORBID_ROLE = 18,					// 禁止用户登录

	LOG_TYPE_CHALLENGE_FIELD_GUANGHUI = 19,		// 竞技场光辉
	LOG_TYPE_ROLE_SHENGWANG = 20,				// 角色声望

	LOG_TYPE_P2P_TRADE_AFFIRM = 21,				// p2p交易确认
	LOG_TYPE_FUNCTION_STATUS = 22,				// 功能参与
	LOG_TYPE_ACTIVITY_ZHUXIE_TASK = 23,			// 诛邪活动任务记录
	LOG_TYPE_ACTIVITY_QUNXIANLUANDOU = 24,		// 群仙乱斗活动
	LOG_TYPE_ACTIVITY_XIANMENGZHAN = 25,		// 仙盟战
	LOG_TYPE_ACTIVITY_GONGCHENGZHAN = 26,		// 攻城战

	LOG_TYPE_HUSONG_TASK_ACCEPT = 27,			// 护送任务接受
	LOG_TYPE_HUSONG_TASK_ROB = 28,				// 护送抢劫
	LOG_TYPE_HUSONG_TASK_BEROB = 29,			// 护送被抢劫
	LOG_TYPE_MAZE_BUY_MOVE = 30,				// 迷宫寻宝购买移动次数(无用）
	LOG_TYPE_GUILD_PARTY_DOUBLE_REWARD = 31,	// 仙盟酒会双倍奖励(无用）
	LOG_TYPE_GUILD_PARTY_RESET_GATHER = 32,		// 仙盟酒会重置采集(无用）
	LOG_TYPE_WABAO = 33,						// 仙女掠夺（改为挖宝）
	LOG_TYPE_JILIAN = 34,						// 祭炼仙丹(无用）
	LOG_TYPE_TEAM_FB_OPEN = 35,					// 组队副本开启(无用）
	LOG_TYPE_TEAM_FB_ASSIST = 36,				// 组队副本协助(无用）
	LOG_TYPE_DAILY_TASK_COMPLETE = 37,			// 完成日常任务
	LOG_TYPE_GUILD_TASK_COMPLETE = 38,			// 完成仙盟任务(无用)
	LOG_TYPE_CISHA_TASK_COMPLETE = 39,			// 完成刺杀任务(无用)

	LOG_TYPE_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI = 40,// 开服累计充值奖励
	LOG_TYPE_OPEN_GAME_ACTIVITY_XIANNV = 41,	// 开服活动仙女
	LOG_TYPE_OPEN_GAME_ACTIVITY_MOUNT = 42,		// 开服活动坐骑
	LOG_TYPE_OPEN_GAME_ACTIVITY_STONE = 43,		// 开服活动宝石
	LOG_TYPE_DISCOUNT_BUY = 44,					// 一折抢购
	LOG_TYPE_DAILY_TOTAL_CHONGZHI_REWARD = 45,	// 日常累计充值奖励
	LOG_TYPE_VIP_BUY_LEVEL_REWARD = 46,			// Vip等级奖励
	LOG_TYPE_VIP_UPLEVEL = 47,					// Vip升级
	LOG_TYPE_MONTH_CARD = 48,					// 月卡

	LOG_TYPE_ONLINE_ROLE_NUM = 49,				// 在线人数
	LOG_TYPE_XIANHUN = 50,						// 仙魂(无用）
	LOG_TYPE_YUANLI = 51,						// 元力(无用）
	LOG_TYPE_HONOUR = 52,						// 荣誉(无用）
	LOG_TYPE_REGISTER = 53,						// 注册
	LOG_TYPE_LOGIN = 54,						// 登录
	LOG_TYPE_LOGOUT = 55,						// 登出
	LOG_TYPE_UPGRADE_LEVEL = 56,				// 升级
	LOG_TYPE_CHANGE_AUTHORITY_TYPE = 57,		// 设置权限

	LOG_TYPE_ACCEPT_TASK = 58,					// 接受任务
	LOG_TYPE_COMMIT_TASK = 59,					// 提交任务
	LOG_TYPE_GIVEUP_TASK= 60,					// 放弃任务
	LOG_TYPE_COMPLETE_TASK = 61,				// 完成任务
	
	LOG_TYPE_TOUZIJIHUA = 62,					// 投资计划
	LOG_TYPE_WORLD_EVENT_REWARD = 63,			// 世界事件奖励
	LOG_TYPE_PAY = 64,							// 充值
	LOG_TYPE_PLAT_ONLINE = 65,					// 平台在线(按角色平台名前缀分组统计)

	LOG_TYPE_GET_ITEM = 66,						// 获得物品

	LOG_TYPE_CROSS_HONOR = 67,					// 跨服荣誉

	LOG_TYPE_CROSS_1V1 = 68,					// 跨服1v1

	LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI = 69, // 随机活动-累计充值奖励

	LOG_TYPE_MAIL_SEND_GOLD = 70,				// 邮件发送元宝
	LOG_TYPE_MAIL_GET_GOLD = 71,				// 邮件获得元宝
	LOG_TYPE_TRADE_SUCC = 72,					// 交易详情（包括元宝，物品)
	
	LOG_TYPE_COIN_ADD = 90,						// 增加非绑铜钱
	LOG_TYPE_COIN_USE = 91,						// 使用非绑铜钱
	LOG_TYPE_PAOHUAN_TASK_COMPLETE = 92,		// 完成跑环任务
	LOG_TYPE_NV_WA_SHI = 93,					// 女娲石
	LOG_TYPE_CHENGJIU = 94,						// 成就
	LOG_TYPE_LINGJING = 95,						// 灵精
	LOG_TYPE_HUNLI = 96,						// 魂力
	LOG_TYPE_CHALLENGE_FIELD_BIND_GOLD = 97,	// 竞技场绑定元宝
	LOG_TYPE_PET = 98,							// 宠物
	LOG_TYPE_MAGIC_CARD = 99,					// 魔卡
	LOG_TYPE_CROSS_BOSS_SCORE = 100,			// 跨服boss
	LOG_TYPE_ACTIVITY_TIANJIANGBAO = 101,		// 天降财宝活动记录
	LOG_TYPE_ACTIVITY_GUILDBATTLE = 102,		// 公会争霸
	LOG_TYPE_ACTIVITY_QUESTION = 103,			// 答题-新版
	LOG_TYPE_ACTIVITY_WANGLINGEXPLORE = 104,	// 王陵探险活动记录
	LOG_TYPE_ACTIVITY_TERRITORYWAR = 105,		// 领土战
	LOG_TYPE_ZHUANSHENG = 106,					// 转生
	LOG_TYPE_ROLE_GOAL = 107,					// 个人目标
	LOG_TYPE_GUILD_JOIN = 108,					// 加入仙盟
	LOG_TYPE_PHASE_FB = 109,					// 进阶本
	LOG_TYPE_EXP_FB = 110,						// 经验本
	LOG_TYPE_TEAM_FB = 111,						// 组队本
	LOG_TYPE_PATA_FB = 112,						// 爬塔本
	LOG_TYPE_RUNE_TOWER = 113,					// 符文塔
	LOG_TYPE_KILL_BOSS = 114,					// 击杀boss
	LOG_TYPE_CROSS_JOIN = 115,					// 进入跨服
	LOG_TYPE_GUILD_FB = 116,					// 公会副本
	LOG_TYPE_GUILD_BOSS = 117,					// 公会boss
	LOG_TYPE_GUILD_BONFIRE = 118,				// 女神祝福
	LOG_TYPE_RUNE_TOWER_GUAJI = 119,			// 符文塔挂机时间
	LOG_TYPE_CHAT_RECORD = 120,					// 聊天记录
	LOG_TYPE_CAPABILITY = 121,					// 战力变化
	LOG_TYPE_GUANG_HUI = 122,					// 光辉改变
	LOG_TYPE_RANDACTIVITY_TOTAL_DATA = 123,		// 随机活动
	LOG_TYPE_HESHENLUOSHU = 124,				// 河神洛书
	LOG_TYPE_JINJIESYS_REWARD = 125,			// 进阶系统奖励
	LOG_TYPE_WASH = 126,                        // 装备洗炼

	// 下面的日志是用另一套打印规则，跟之前的有少量不一样，基于LOG2_QUICK16
	LOG2_TYPE_USER_LOGIN = 200,						// 玩家登录
	LOG2_TYPE_USER_LOGOUT = 201,					// 玩家离线
	LOG2_TYPE_ADD_GOLD = 202,						// 获得元宝
	LOG2_TYPE_USE_GOLD = 203,						// 消耗元宝
	LOG2_TYPE_GET_ITEM = 204,						// 获得物品
	LOG2_TYPE_CONSUME_ITEM = 205,					// 消耗物品
	LOG2_TYPE_P2P_TRADE = 206,						// 玩家交易
	LOG2_TYPE_PUBLIC = 207,							// 市场拍卖

	LOG_TYPE_MAX,
};

enum LOG_GET_ITEM_WAY
{
	LOG_GET_ITEM_WAY_XUNBAO = 0,
	LOG_GET_ITEM_WAY_SHOP,
	LOG_GET_ITEM_WAY_COMPOSE,
	LOG_GET_ITEM_WAY_DUIHUAN,

	LOG_GET_ITEM_WAY_MAX,
};

enum LOG_MONEY_TYPE
{
	LOG_MONEY_TYPE_GOLD = 0,
	LOG_MONEY_TYPE_GOLD_BIND,
	LOG_MONEY_TYPE_COIN,
	LOG_MONEY_TYPE_MOJING,
	LOG_MONEY_TYPE_SHENGWANG,
	LOG_MONEY_TYPE_GONGXUN,
	LOG_MONEY_TYPE_WEIWANG,
	LOG_MONEY_TYPE_TREASURE_HUNT_CREDIT,
	LOG_MONEY_TYPE_JINGLING_CREDIT,
	LOG_MONEY_TYPE_HAPPYTREE_GROW,
	LOG_MONEY_TYPE_MYSTERIOUS_SHOP_CREDIT,
	LOG_MONEY_TYPE_GUANGHUI,
	LOG_MONEY_TYPE_BLUE_LINGZHI,
	LOG_MONEY_TYPE_PURPLE_LINGZHI,
	LOG_MONEY_TYPE_ORANGE_LINGZHI,
	LOG_MONEY_TYPE_PRECIOUS_BOSS,
	LOG_MONEY_TYPE_ZHUANZHI_STONE,
	LOG_MONEY_TYPE_HUNJING,
	LOG_MONEY_TYPE_MAX,
};

enum LOG_TYPE_FUNCTION_STATUS_TYPE
{
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_COIN = 0,
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_YUANLI,
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_XIANHUN,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_HUSONG,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_DAILY,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_GUILDTASK,
	LOG_TYPE_FUNCTION_STATUS_TYPE_XUNBAO,
	LOG_TYPE_FUNCTION_STATUS_TYPE_MOVECHESS,

	LOG_TYPE_FUNCTION_STATUS_TYPE_ACTIVITY = 10000,

	LOG_TYPE_FUNCTION_STATUS_TYPE_FB = 20000,
};

enum LOG_CHAT_TYPE
{
	LOG_CHAT_TYPE_SINGLE = 0,
	LOG_CHAT_TYPE_WORLD,
	LOG_CHAT_TYPE_SCENE,
	LOG_CHAT_TYPE_TEAM,
	LOG_CHAT_TYPE_GUILD,
	LOG_CHAT_TYPE_CAMP,
	LOG_CHAT_TYPE_WORLD_QUESTION,
	LOG_CHAT_TYPE_GUILD_QUESTION,

	LOG_CHAT_TYPE_GUILD_NOTICE = 100,
	LOG_CHAT_TYPE_MAX,
};

inline void LOG_QUICK8(LogAgent &log_agent, long long n1, long long n2, const char *z1, const char *z2, long long n3, long long n4, const char *z3, const char *z4)
{
	log_agent.printf(LL_INFO, "%lld\t%lld\t%s\t%s\t%lld\t%lld\t%s\t%s", 
		(long long)n1, 
		(long long)n2,
		(NULL == z1 ? "-" : z1),
		(NULL == z2 ? "-" : z2),
		(long long)n3,
		(long long)n4,
		(NULL == z3 ? "-" : z3),
		(NULL == z4 ? "-" : z4)
		);
}

inline void LOG_QUICK12(LogAgent &log_agent, long long n1, long long n2, const char *z1, const char *z2, long long n3, long long n4, const char *z3, const char *z4, long long n5, long long n6, long long n7, long long n8)
{
	log_agent.printf(LL_INFO, "%lld\t%lld\t%s\t%s\t%lld\t%lld\t%s\t%s\t%lld\t%lld\t%lld\t%lld", 
		(long long)n1, 
		(long long)n2,
		(NULL == z1 ? "-" : z1),
		(NULL == z2 ? "-" : z2),
		(long long)n3,
		(long long)n4,
		(NULL == z3 ? "-" : z3),
		(NULL == z4 ? "-" : z4),
		(long long)n5,
		(long long)n6,
		(long long)n7,
		(long long)n8
		);
}

#endif
