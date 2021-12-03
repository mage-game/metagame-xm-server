#ifndef __FIGHTING_DEF_HPP__
#define __FIGHTING_DEF_HPP__

#include "servercommon/servercommon.h"
#include "gameworld/gameworld/gamedef.h"

static const int FIGHTING_MAX_ROB_HISTORY_COUNT = 20;					// 允许存储的最大被抢劫历史记录
static const int FIGHTING_CHALLENGE_OPPONENT_COUNT = 4;					// 挑衅 - 对手个数
static const int FIGHTING_CHALLENGE_DEFAULT_TIMES = 6;					// 挑衅 - 每天默认给玩家的挑战次数
static const int FIGHTING_FAKE_MINING_SAILING_DATA_COUNT = 8;			// 决斗场 - 挖矿航海里的机器人数量（刘琦说起码两页，那就8咯）
//static const int FIGHTING_FAKE_MINING_SAILING_BEEN_ROB_COUNT = 5;		// 决斗场 - 挖矿航海里的机器人最多被打劫次数（避免重复被同一个玩家打劫）

#pragma pack(push, 4)

struct FightingRobHistoryItem
{
	FightingRobHistoryItem() { this->Reset(); }
	
	void Reset()
	{
		rober_uid = 0;
		rob_time = 0;
		has_revenge = 0;
		quality = 0;
		reserve_sh = 0;
	}

	int rober_uid;														// 抢劫者uid
	unsigned int rob_time;												// 抢劫时间
	char has_revenge;													// 是否已报仇
	char quality;														// 被抢劫的矿品质
	short reserve_sh;
};

struct FightingChellangeOpponentInfo
{
	FightingChellangeOpponentInfo() { this->Reset(); }

	void Reset()
	{
		random_uid = 0;
		random_name_index = -1;
		has_win = 0;
		reserve_ch = 0;
		capability = 0;
		memset(reserve_list, 0, sizeof(reserve_list));
	}

	int random_uid;														// 随机到的本服玩家ID
	short random_name_index;											// 生成的对手名字随机数（客户端根据这个去随机名字列表对应找名字）
	char has_win;														// 是否已经战胜了这个对手
	char reserve_ch;
	int capability;
	char reserve_list[28];
};

// 机器人挖矿/航海数据
struct FightingFakeMiningSailingData
{
	FightingFakeMiningSailingData() { this->Reset(); }

	void Reset()
	{
		random_index = -1;
		reserve_sh = 0;
		cur_quality = 0;
		cur_been_rob_times = 0;
		end_time = 0;
		capability = 0;
		memset(reserve_list, 0, sizeof(reserve_list));
	}

	int random_index;													// 随机数（用于确定玩家名字、职业、性别等信息）
	short reserve_sh;
	char cur_quality;													// 当前矿品质
	char cur_been_rob_times;											// 当前矿被（主角）抢劫次数
	unsigned int end_time;												// 当前挖矿结束的时间戳
	int capability;
	char reserve_list[28];
};

struct FightingParam
{
	FightingParam() { this->Reset(); }

	void Reset()
	{
		mining_today_times = 0;
		mining_today_buy_times = 0;
		mining_today_rob_times = 0;
		mining_been_rob_last_index = 0;
		mining_been_rob_need_notify = 0;
		reserve_ch3 = 0;
		mining_cur_quality = 0;
		mining_cur_been_rob_times = 0;
		mining_end_time = 0;
		for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
		{
			mining_been_rob_list[i].Reset();
			sailing_been_rob_list[i].Reset();
		}

		sailing_today_times = 0;
		sailing_today_buy_times = 0;
		sailing_today_rob_times = 0;
		sailing_been_rob_last_index = 0;
		sailing_been_rob_need_notify = 0;
		reserve_ch4 = 0;
		sailing_cur_quality = 0;
		sailing_cur_been_rob_times = 0;
		sailing_end_time = 0;
		for (int i = 0; i < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++i)
		{
			challenge_opponent_lsit[i].Reset();
		}

		for (int i = 0; i < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT; ++i)
		{
			mining_fake_list[i].Reset();
			sailing_fake_list[i].Reset();
		}

		challenge_score = 0;
		challenge_next_auto_reflush_time = 0;
		challenge_left_times = FIGHTING_CHALLENGE_DEFAULT_TIMES;
		challenge_vip_buy_times = 0;
		reserve_ch1 = 0;
		reserve_ch2 = 0;
		for (int i = 0; i < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++i)
		{
			challenge_opponent_lsit[i].Reset();
		}

		memset(reserve_list, 0, sizeof(0));
	}

	char mining_today_times;											// 今日已挖矿次数
	char mining_today_buy_times;										// 今日已购买次数
	char mining_today_rob_times;										// 今日已抢劫矿次数
	char mining_been_rob_last_index;									// 挖矿被抢劫历史最新那条记录的下标
	char mining_been_rob_need_notify;									// 是否有被抢劫记录需要通知玩家（玩家请求一次抢劫列表后还原）
	char reserve_ch3;
	char mining_cur_quality;											// 当前矿品质
	char mining_cur_been_rob_times;										// 当前矿被抢劫次数
	unsigned int mining_end_time;										// 当前挖矿结束的时间戳
	FightingRobHistoryItem mining_been_rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];	// 挖矿被抢劫历史列表（循环替换记录）
	FightingFakeMiningSailingData mining_fake_list[FIGHTING_FAKE_MINING_SAILING_DATA_COUNT]; // 决斗场 - 机器人挖矿数据

	char sailing_today_times;											// 今日已航海次数
	char sailing_today_buy_times;										// 今日已购买次数
	char sailing_today_rob_times;										// 今日已抢劫次数
	char sailing_been_rob_last_index;									// 航海被抢劫历史最新那条记录的下标
	char sailing_been_rob_need_notify;									// 是否有被抢劫记录需要通知玩家（玩家请求一次抢劫列表后还原）
	char reserve_ch4;
	char sailing_cur_quality;											// 当前航海品质
	char sailing_cur_been_rob_times;									// 当前次被抢劫次数
	unsigned int sailing_end_time;										// 当前航海结束的时间戳
	FightingRobHistoryItem sailing_been_rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];	// 航海被抢劫历史列表（循环替换记录）
	FightingFakeMiningSailingData sailing_fake_list[FIGHTING_FAKE_MINING_SAILING_DATA_COUNT]; // 决斗场 - 机器人航海数据

	int challenge_score;												// 挑衅 - 总积分
	unsigned int challenge_next_auto_reflush_time;						// 挑衅 - 上一次自动刷新时间（定时自动刷新挑衅列表）
	char challenge_left_times;											// 挑衅 - 当前剩余挑战次数
	char challenge_vip_buy_times;										// 挑衅 - VIP已购买的挑战次数
	char reserve_ch1;
	char reserve_ch2;
	FightingChellangeOpponentInfo challenge_opponent_lsit[FIGHTING_CHALLENGE_OPPONENT_COUNT];// 挑衅 - 匹配到的对手列表

	char reserve_list[152];
};

typedef char FightingParamHex[sizeof(FightingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FightingParamHex) < 4096);

#pragma pack(pop)

#endif

