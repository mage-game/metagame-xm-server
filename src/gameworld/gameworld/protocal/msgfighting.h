#ifndef __MSG_FIGHTING_H__
#define __MSG_FIGHTING_H__

#pragma pack(push) 
#pragma pack(4)

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/fightingdef.hpp"

// 决斗场系列协议

namespace Protocol
{
	// 5955  决斗场请求
	class CSFightingReq
	{
	public:
		enum ReqType
		{
			REQ_TYPE_M_MINING_INFO = 0,							// 挖矿-拉取能抢的矿点信息（服务端默认下发N个，不发所有的，够用就行）
			REQ_TYPE_M_BEEN_ROB_INFO,							// 挖矿-拉取被抢劫的信息
			REQ_TYPE_M_REFLUSH_MINING,							// 挖矿-刷新矿的颜色
			REQ_TYPE_M_START_MINING,							// 挖矿-开始挖矿
			REQ_TYPE_M_BUY_TIMES,								// 挖矿-购买挖矿次数
			REQ_TYPE_M_ROB,										// 挖矿-请求抢矿，param1 NULL  param2 玩家UID（保证不会抢错）
			REQ_TYPE_M_ROB_ROBOT,								// 挖矿-请求抢夺机器人，param1 机器人下标（0-7）
			REQ_TYPE_M_REVENGE,									// 挖矿-复仇，param1 被抢矿记录index
			REQ_TYPE_M_FETCH_REWARD,							// 挖矿-领取挖矿奖励

			REQ_TYPE_S_SAILING_INFO = 9,						// 航海-拉取能抢的航行信息（服务端默认下发N个，不发所有的，够用就行）
			REQ_TYPE_S_BEEN_ROB_INFO,							// 航海-拉取被抢劫的信息
			REQ_TYPE_S_REFLUSH_SAILING,							// 航海-刷新航行的颜色
			REQ_TYPE_S_START_SAILING,							// 航海-开始航行
			REQ_TYPE_S_BUY_TIMES,								// 航海-购买航行次数
			REQ_TYPE_S_ROB,										// 航海-请求抢夺，param1 NULL  param2 玩家UID（保证不会抢错）
			REQ_TYPE_S_ROB_ROBOT,								// 航海-请求抢夺机器人，param1 机器人下标（0-7）
			REQ_TYPE_S_REVENGE,									// 航海-复仇，param1 被抢矿记录index
			REQ_TYPE_S_FETCH_REWARD,							// 航海-领取航行奖励

			REQ_TYPE_C_INFO = 18,								// 挑衅-请求列表信息
			REQ_TYPE_C_REFLUSH,									// 挑衅-刷新列表
			REQ_TYPE_C_BUY_FIGHTING_TIMES,						// 挑衅-VIP购买挑战次数
			REQ_TYPE_C_FIGHTING,								// 挑衅-挑战对手
		};

		CSFightingReq();
		MessageHeader header;

		short req_type;
		short param1;
		int param2;
	};

	// 挖矿相关协议 ================================================================================================================================

	// 5956  挖矿基础信息
	class SCFightingMiningBaseInfo
	{
	public:
		SCFightingMiningBaseInfo();
		MessageHeader header;

		char today_mining_times;											// 今日已挖矿次数
		char today_buy_times;												// 今日已购买次数
		char today_rob_mine_times;											// 今日已抢劫矿次数
		char mining_quality;												// 当前矿类型
		char mining_been_rob_times;											// 矿被抢劫次数（可领取奖励时，方便客户端计算奖励）
		char reserve_ch;
		short reserve_sh;
		unsigned int mining_end_time;										// 当前矿收获的时间戳
	};

	// 5957  被抢劫历史列表
	class SCFightingMiningBeenRobList
	{
	public:
		SCFightingMiningBeenRobList();
		MessageHeader header;

		struct HistoryItem
		{
			GameName rober_name;												// 抢劫者名字
			unsigned int rob_time;												// 抢劫时间
			char has_revenge;													// 是否已报仇
			char real_index;													// 在数据库存储里面的下标（用于复仇）
			char cur_quality;													// 矿品质
			char rober_sex;
			char rober_prof;
			char reserve_ch;
			short reserve_sh;
			int rober_capability;
		};

		HistoryItem rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];					// 被抢劫历史列表
	};

	// 5958  矿列表
	class SCFightingMiningList
	{
	public:
		SCFightingMiningList();
		MessageHeader header;

		const static int MAX_NOTIFY_COUNT = 50;					// 最大下发个数

		struct ItemInfo
		{
			int owner_uid;										// 大于0说明此变量生效，否则表示机器人，用random_index
			int random_index;									// 大于等于0说明此变量生效，用于确定机器人名字、职业、性别等等属性

			GameName owner_name;
			unsigned int mining_end_time;						// 当前矿收获时间
			char cur_quality;									// 当前矿品质
			char robot_index;									// 机器人所在下标（0-7）给客户端请求挑战时使用
			char owner_sex;										// 矿主性别
			char owner_prof;									// 矿主职业
			int owner_capability;								// 矿主战力
			short owner_level;									// 矿主等级
			short reserve_sh;
		};

		int mine_count;											// 实际有效数量
		ItemInfo mine_list[MAX_NOTIFY_COUNT];
	};

	// 挑衅相关协议 ================================================================================================================================

	// 5959 挑衅 - 基础信息
	class SCFightingChallengeBaseInfo
	{
	public:
		SCFightingChallengeBaseInfo();
		MessageHeader header;

		int challenge_score;												// 挑衅 - 总积分
		char challenge_limit_times;											// 挑衅 - 当前剩余挑战次数
		char vip_buy_times;													// 已购买的次数（不同VIP最大次数不一样）
		short reserve_sh;
		unsigned int next_add_challenge_timestamp;							// 下一次增加挑战次数的时间戳
		unsigned int next_auto_reflush_time;								// 下一次自动刷新列表时间
	};

	// 5960 战斗结果通知
	class SCFightingResultNotify
	{
	public:
		SCFightingResultNotify();
		MessageHeader header;

		struct ItemInfo
		{
			ItemID item_id;													// 奖励物品ID
			short num;														// 奖励物品数量
		};

		char is_win;														// 是否胜利
		char fighting_type;													// 战斗类型，详见 ChallengeType

		short is_skip;														// 是否为扫荡,0不是1是
		long long reward_exp;												// 奖励经验值
		ItemInfo reward_item_list[MAX_ATTACHMENT_ITEM_NUM];					// 奖励物品列表
	};
	
	// 5961 挑衅 - 战斗列表
	class SCFightingChallengeList
	{
	public:
		SCFightingChallengeList();
		MessageHeader header;

		struct OpponentInfo
		{
			char camp;
			char prof;
			char sex;
			char has_win;
			short reserve_sh;
			short random_name_index; // 生成的对手名字随机数（客户端根据这个去随机名字列表对应找名字，若这个值为-1，说明用本服玩家名，则用下面的name）
			GameName name;
			int capability;

			RoleAppearance appearance;
		};

		OpponentInfo opponent_list[FIGHTING_CHALLENGE_OPPONENT_COUNT];		// 对手信息
	};

	// 航海相关协议 ================================================================================================================================

	// 5962  航海基础信息
	class SCFightingSailingBaseInfo
	{
	public:
		SCFightingSailingBaseInfo();
		MessageHeader header;

		char today_sailing_times;											// 今日已次数
		char today_buy_times;												// 今日已购买次数
		char today_rob_times;												// 今日已抢劫次数
		char sailing_quality;												// 当前品质
		char sailing_been_rob_times;										// 被抢劫次数（可领取奖励时，方便客户端计算奖励）
		char reserve_ch;
		short reserve_sh;
		unsigned int sailing_end_time;										// 当前矿收获的时间戳
	};

	// 5963  被抢劫历史列表
	class SCFightingSailingBeenRobList
	{
	public:
		SCFightingSailingBeenRobList();
		MessageHeader header;

		struct HistoryItem
		{
			GameName rober_name;												// 抢劫者名字
			unsigned int rob_time;												// 抢劫时间
			char has_revenge;													// 是否已报仇
			char real_index;													// 在数据库存储里面的下标（用于复仇）
			char cur_quality;													// 矿品质
			char rober_sex;
			char rober_prof;
			char reserve_ch;
			short reserve_sh;
			int rober_capability;
		};

		HistoryItem rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];			// 被抢劫历史列表
	};

	// 5964  航行列表
	class SCFightingSailingList
	{
	public:
		SCFightingSailingList();
		MessageHeader header;

		const static int MAX_NOTIFY_COUNT = 50;					// 最大下发个数

		struct ItemInfo
		{
			int owner_uid;										// 大于0说明此变量生效，否则表示机器人，用random_index
			int random_index;									// 大于等于0说明此变量生效，用于确定机器人名字、职业、性别等等属性

			GameName owner_name;
			unsigned int sailing_end_time;						// 当前矿收获时间
			char cur_quality;									// 当前矿品质
			char robot_index;									// 机器人所在下标（0-7）给客户端请求挑战时使用
			char owner_sex;										// 船主性别
			char owner_prof;									// 船主职业
			int owner_capability;								// 船主战力
			short owner_level;									// 船主等级
			short reserve_sh;
		};

		int sailing_count;										// 实际有效数量
		ItemInfo sailing_list[MAX_NOTIFY_COUNT];
	};


	// 5965 战斗开始倒计时通知
	class SCFightingCountDownNotify
	{
	public:
		SCFightingCountDownNotify();
		MessageHeader header;

		unsigned int start_fighting_time;						// 开始战斗时间戳
	};

	// 5966  挖矿，航海-有新的抢夺记录（发送给被掠夺玩家本人，没查看之前一直有提示，重新登录也一样）
	class SCFightingBeenRobNotify
	{
	public:
		SCFightingBeenRobNotify();
		MessageHeader header;

		char type;												// 0 挖矿，1 航海
		char reserve_ch;
		short reserve_sh;
	};

	// 5967  挖矿，航海-有新的抢夺记录（广播给所有人，只在抢夺时发一次）
	class SCFightingRobingNotify
	{
	public:
		SCFightingRobingNotify();
		MessageHeader header;

		GameName rober_name;
		GameName been_rob_name;
		char type;												// 0 挖矿，1 航海
		char quality;											// 品质
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif


