/******************************************************************************************
FileName: crossdef.hpp
Author: dreamwang
Description: cross相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __CROSS_DEF_HPP__
#define __CROSS_DEF_HPP__

#include "servercommon.h"
#include "servercommon/activitydef.hpp"
#include "gameworld/gameworld/gamedef.h"

static const int CROSS_CONNECT_ASYN_TIME_OUT_MS = 10000;		// 异步连接cross超时限制 毫秒

static const int CROSS_PROTOCAL_VERSION = 1;					// cross通讯协议版本号

static const int CROSS_USER_DB_INDEX = 2046;					// 跨服玩家固定db_index
static const int CROSS_USER_CROSS_ACTIVITY_MARK_BIT = 14;		// 跨服玩家role_id分两部分 cross_activity_type 7bit + 自增分配 14bit
static const int CROSS_USER_INC_ID_MAX = 16383;					// 自增分配id最大值

static const int WAIT_SCENE_MAX = 16;							// 一个跨服活动等待场景最大数

static const int CROSS_ROLE_LOGOUT_LIMIT_TIME_S = 60;			// 跨服掉线重连最大时间限制(秒)

inline bool IsCrossUser(const UserID &user_id)
{
	return (CROSS_USER_DB_INDEX == user_id.db_index);
}

inline int GetCrossUserCrossActivityType(const UserID &user_id)
{
	if (IsCrossUser(user_id))
	{
		int cross_activity_type = (user_id.role_id >> CROSS_USER_CROSS_ACTIVITY_MARK_BIT);
		if (cross_activity_type > 0 && cross_activity_type < CROSS_ACTIVITY_TYPE_MAX)
		{
			return cross_activity_type + CROSS_ACTIVITY_TYPE_INVALID;
		}
	}

	return CROSS_ACTIVITY_TYPE_INVALID;
}

struct UniqueServerID
{
	UniqueServerID() : plat_type(-1), server_id(0) {}
	UniqueServerID(const UniqueServerID &v) : plat_type(v.plat_type), server_id(v.server_id) {}
	UniqueServerID(int _plat_type, int _server_id) : plat_type(_plat_type), server_id(_server_id) {}

	bool operator<(const UniqueServerID &right) const
	{
		return plat_type < right.plat_type || (plat_type == right.plat_type && server_id < right.server_id);
	}

	bool operator==(const UniqueServerID &right) const
	{
		return plat_type == right.plat_type && server_id == right.server_id;
	}

	bool operator!=(const UniqueServerID &right) const
	{
		return plat_type != right.plat_type || server_id != right.server_id;
	}

	int plat_type;												// 平台类型
	int server_id;												// 服ID
};

static const UniqueServerID INVALID_UNIQUE_SERVER_ID(-1, 0);

struct UniqueUserID
{
	UniqueUserID() : plat_type(-1), user_id(INVALID_USER_ID) {}
	UniqueUserID(const UniqueUserID &v) : plat_type(v.plat_type), user_id(v.user_id) {}
	UniqueUserID(int _plat_type, const UserID &_user_id) : plat_type(_plat_type), user_id(_user_id) {}

	bool operator<(const UniqueUserID& right) const
	{
		return plat_type < right.plat_type || (plat_type == right.plat_type && user_id < right.user_id);
	}

	bool operator==(const UniqueUserID &right) const
	{
		return plat_type == right.plat_type && user_id == right.user_id;
	}

	bool operator!=(const UniqueUserID &right) const
	{
		return plat_type != right.plat_type || user_id != right.user_id;
	}

	int plat_type;												// 平台类型
	UserID user_id;												// UserID
};

static const UniqueUserID INVALID_UNIQUE_USER_ID(-1, INVALID_USER_ID);

inline long long UniqueUserIDToLongLong(const UniqueUserID &unique_user_id)
{
	return TranslateInt64(unique_user_id.plat_type, UidToInt(unique_user_id.user_id));
}

inline UniqueUserID LongLongToUniqueUserID(long long uuid)
{
	UniqueUserID unique_user_id;
	unique_user_id.plat_type = static_cast<int>(uuid >> 32);
	unique_user_id.user_id = IntToUid(static_cast<int>(uuid & 0xffffffff));
	return unique_user_id;
}

//-------------------------------------------------------------------------------------
static const int CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS = 7;	
static const int CROSS_ACTIVITY_XIULUO_TOWER_ROLE_CAPABILITY = 30000;
static const int CROSS_ACTIVITY_XIULUO_TOWER_ROLE_LEVEL = 60;
static const int CROSS_ACTIVITY_XIULUO_TOWER_ROOM_USER_MAX_NUM = 50;
static const int CROSS_ACTIVITY_XIULUO_TOWER_ROOM_MAX = 20;
static const int CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER = 10;
static const int CROSS_ACTIVITY_XIULUO_TOWER_REALIVE_POINT_MAX = 6;
static const int CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY = 1;
static const int CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM = 10;
static const int CROSS_ACTIVITY_XIULUO_TOWER_REWARD_CFG_MAX = 10;
static const int CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_CFG_MAX = 10;
static const int CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_LEVEL_CFG_MAX = 2;
static const int CROSS_ACTIVITY_XIULUO_TOWER_DUR_DEAD_BUFF_MAX = 5;
static const int CROSS_ACTIVITY_XIULUO_TOWER_DUR_KILL_BUFF_MAX = 5;
static const int CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX = 100;			// 跨服修罗塔掉落日志最大数
static const int CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX = 5;		// 跨服修罗塔掉落限制配置最大数
static const int CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM = 3;
static const int CROSS_ACTIVITY_XIULUO_TOWER_MAX_SCORE_REWARD = 10;
static const int CROSS_XIULUO_TOWER_GATER_ITEM_NUM_MAX = 24;

struct CrossXiuluoTowerUser
{
	CrossXiuluoTowerUser() : uuid(0), prof(0), sex(0), camp(0), rank_pos(0), level(0), cur_layer(0), max_layer(0), total_kill_count(0), 
		kill_role_count(0), kill_boss_count(0), cur_layer_kill_count(0), not_drop_layer_count(0), kill_reward_cross_honor(0), score_reward_cross_honor(0), immediate_realive_count(0), 
		continually_kills(0), continually_deads(0), finish_time(0), buff_num(0), reward_layer(-1), score(0), score_reward_flag(0), is_brocast_gather_buff(false)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	long long uuid;
	char prof;
	char sex;
	char camp;
	char rank_pos;
	int level;
	int cur_layer;
	int max_layer;
	int total_kill_count;
	int kill_role_count;
	int kill_boss_count;
	int cur_layer_kill_count;
	int not_drop_layer_count;
	int kill_reward_cross_honor;
	int score_reward_cross_honor;
	int immediate_realive_count;
	int continually_kills;
	int continually_deads;
	int finish_time;
	GameName user_name;
	short buff_num;
	short reward_layer;
	int score;
	int score_reward_flag;
	bool is_brocast_gather_buff;
};

struct CrossXiuluoTowerUserRankItem
{
	CrossXiuluoTowerUserRankItem() : uuid(0), max_layer(0), finish_time(0), prof(0), camp(0), reserve_sh(0) 
	{
		memset(user_name, 0, sizeof(user_name));
	}

	long long uuid;
	GameName user_name;
	int max_layer;
	int finish_time;
	char prof;
	char camp;
	short reserve_sh;
};

enum CROSS_XIULUO_TOWER_DROP_LOG_TYPE
{
	CROSS_XIULUO_TOWER_DROP_LOG_TYPE_MONSTER = 1,		// 怪物掉落
	CROSS_XIULUO_TOWER_DROP_LOG_TYPE_GOLD_BOX = 2,		// 金箱子掉落

	CROSS_XIULUO_TOWER_DROP_LOG_TYPE_MAX
};

struct CrossXiuluoTowerDropLog
{
	CrossXiuluoTowerDropLog() : log_type(0), timestamp(0), item_id(0), item_num(0)
	{
		memset(name, 0, sizeof(name));
	}

	int log_type;
	GameName name;
	unsigned int timestamp;
	unsigned short item_id;
	short item_num;
};

struct CrossXiuLuoTowerBossInfo
{
	CrossXiuLuoTowerBossInfo() :monster_id(0), max_hp(0), cur_hp(0)
	{
	}
	int monster_id;
	ObjID obj_id;
	long long max_hp;
	long long cur_hp;
};

//-------------------------------------------------------------------------------------

//跨服1v1------------------------------------------------------------------------------
static const int CROSS_1V1_SEASON_MAX = 32;										// 1v1 最大赛季
static const int CROSS_ACTIVITY_1V1_ROLE_CAPABILITY = 300;						// 1V1限制战斗力  30000
static const int CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS = 7;							// 1V1一周天数		
static const int CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT = 5;						// 本周战绩数量
static const int CROSS_ACTIVITY_1V1_USER_RANK_MAX = 100;						// 排行榜最大人数
static const int CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT = 3;				// 参与次数配置最大数
static const int CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT = 13;					// 积分奖励配置最大数
static const int CROSS_ACTIVITY_1V1_RANK_CFG_MAX_COUNT = 8;						// 排行奖励配置最大数

static const int CROSS_ACTIVITY_1V1_GRADE_TYPE_MAX = 34;                        // 1v1最高段位
static const int CROSS_ACTIVITY_1V1_RING_TYPE_MAX = 37;                         // 1v1戒指
static const int CROSS_ACTIVITY_1V1_RING_USE_COUNT = 4;							// 1v1戒指装备数量

UNSTD_STATIC_CHECK(32 <= CROSS_1V1_SEASON_MAX)

enum CROSS_ACTIVITY_PVP_REWARD_TYPE                        // 跨服1v1、3v3赛季奖励
{
	CROSS_ACTIVITY_PVP_REWARD_TYPE_RING = 0,               // 跨服1v1戒指
	CROSS_ACTIVITY_PVP_REWARD_TYPE_CARD = 1,               // 跨服3v3令牌
};


struct CrossActivity1v1FightUnit
{
	CrossActivity1v1FightUnit()
	{
		fight_seq = 0; rand_per_attr = 0;
		uuid1 = 0; prof1 = 0; sex1 = 0; camp1 = 0; is_with_robot1 = 0; level1 = 0; capability1 = 0; memset(user_name1, 0, sizeof(user_name1));
		uuid2 = 0; prof2 = 0; sex2 = 0; camp2 = 0; is_with_robot2 = 0; level2 = 0; capability2 = 0; memset(user_name2, 0, sizeof(user_name2));
	}

	int fight_seq;
	int rand_per_attr;
	long long uuid1;
	char prof1;
	char sex1;
	char camp1;
	char is_with_robot1;
	int level1;
	long long capability1;
	GameName user_name1;
	long long uuid2;
	char prof2;
	char sex2;
	char camp2;
	char is_with_robot2;
	int level2;
	long long capability2;
	GameName user_name2;
};

struct CrossWeekRecord1v1
{
	CrossWeekRecord1v1()
	{
		result = 0;
		add_score = 0;
		uuid = 0;
		oppo_plat_type = 0;
		oppo_server_id = 0;
		oppo_role_uid = 0;
		oppo_capability = 0;
		memset(oppo_name, 0, sizeof(oppo_name));
	}

	short result;				// 1：胜利，0：失败
	short add_score;			// 本次获得积分
	long long uuid;				// 自己的唯一ID
	int oppo_plat_type;			// 对手平台类型
	int oppo_server_id;			// 对手所在服ID
	int oppo_role_uid;			// 对手uid
	long long oppo_capability;	// 对手战力
	GameName oppo_name;			// 对手名字

};

struct CrossActivity1V1RankItem
{
	CrossActivity1V1RankItem() : plat_type(0), role_id(0), level(0), prof(0), sex(0), score(0), capability(0)
	{
		memset(name, 0, sizeof(name));
	}

	int plat_type;
	int role_id;
	GameName name;
	short level;
	char prof;
	char sex;
	int score;
	long long capability;
};

#pragma pack(push, 4)

// struct CrossActivity1V1RankItem
// {
// 	CrossActivity1V1RankItem() : plat_type(0), role_id(0), level(0), prof(0), sex(0), score(0), max_dur_win_count(0), win_percent(0), capability(0) 
// 	{
// 		memset(name, 0, sizeof(name));
// 	}
// 
// 	int plat_type;
// 	int role_id;
// 	int oppo_server_id;			// 对手所在服ID
// 	GameName name;
// 	short level;
// 	char prof;
// 	char sex;
// 	int score;
// 	int max_dur_win_count;
// 	int win_percent;
// 	int capability;
// 	RoleAppearance appearance;
// };

#pragma pack(pop)


//-------------------------------------------------------------------------------------

//跨服3v3------------------------------------------------------------------------------

static const int CROSS_MULTIUSER_CHALLENGE_INIT_SCORE = 0;					// 积分初始值
static const int CROSS_MULTIUSER_CHALLENGE_MIN_SCORE = 0;					// 积分最小值
static const int CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT = 3;			// 跨服3v3一方参赛人数
static const int CROSS_MULTIUSER_CHALLENGE_MATCHING_INTERVAL = 5;			// 跨服3V3每次匹配的时间间隔
static const int CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM = 3;				// 跨服3V3据点数量
static const int CROSS_MULTIUSER_CHALLENGE_MATCH_DELAY_KICK_TIME_S = 35;	// 一场比赛延迟踢人时间
static const int CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_ADD_SCORE_INTERVAL = 1; // 比赛中据点定时加分时间间隔
static const int CROSS_MULTIUSER_CHALLENGE_MATCH_ASSIST_TIME_LIMIT = 5;		// 比赛计算助攻有效时间
static const int CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT = 15;// 积分排行奖励最大数量
static const int CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX = 100;				// 排行榜最大人数
static const int CROSS_3V3_SEASON_MAX = 32;									// 3V3最大赛季数
static const int CROSS_ACTIVITY_3V3_RING_USE_COUNT = 4;						// 3v3戒指装备数量

UNSTD_STATIC_CHECK(32 <= CROSS_3V3_SEASON_MAX)

enum CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE
{
	CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE,						// 匹配队友中
	CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT,						// 匹配对手中
	CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_FINISH,						// 匹配完成
	CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL,						// 匹配取消
};

enum CROSS_MULTIUSER_CHALLENGE_SIDE
{
	CROSS_MULTIUSER_CHALLENGE_SIDE_0,										// 对战方0
	CROSS_MULTIUSER_CHALLENGE_SIDE_1,										// 对战方1
	CROSS_MULTIUSER_CHALLENGE_SIDE_MAX
};

enum CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE
{
	CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_INVALID = 0,
	CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_SCORE,
	CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MVP,
	CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX,
};

#pragma pack(push, 4)

struct CrossMultiuserChallengeUserRankItem
{
	CrossMultiuserChallengeUserRankItem() : plat_type(0), server_id(0), role_id(0), level(0), prof(0), sex(0), match_total_count(0), 
		win_percent(0), jingjie(0), dur_win_count(0), rank_value(0), capability(0)
	{
		memset(name, 0, sizeof(name));
	}

	int plat_type;
	int server_id;
	int role_id;
	GameName name;
	short level;
	char prof;
	char sex;
	short match_total_count;
	char win_percent;
	char jingjie;
	int dur_win_count;
	int rank_value;
	int capability;
};

#pragma pack(pop)

#pragma pack(push, 4)

struct SyncUserInfoOnBack
{
	SyncUserInfoOnBack() : uuid(0), cost_gold_in_cross(0)
	{

	}
	
	long long uuid;
	int cost_gold_in_cross;
};

#pragma pack(pop)

//-------------------------------------------------------------------------------------
#pragma pack(push, 4)

static const int CROSS_RECORD_SAVE_INTERVAL = 30;			// 个人跨服记录保存间隔

struct UserCrossRecordData									// 个人跨服记录数据 会用在协议中 注意对齐
{
	UserCrossRecordData() { this->Reset(); }

	void Reset()
	{
		cross_score_1v1 = 0;
		cross_1v1_day_join_count = 0;
		cross_1v1_dur_win_count = 0;
		cross_1v1_total_join_times = 0;
		cross_1v1_total_wim_times = 0;			
		cross_1v1_gongxun = 0;
		cross_1v1_left_hp = 0;
		cross_1v1_add_honor = 0;
		has_sync_cross_1v1_record_to_role = 0;
		user_capabilit = 0;
		memset(reserve_ch_list, 0, sizeof(reserve_ch_list));
		cross_day_lose_1v1_count = 0;
		cross_1v1_day_match_fail_count = 0;
		cross_max_dur_win_1v1_count = 0;
		cross_1v1_xiazhu_seq = 0;
		cross_1v1_fetch_xiazhu_reward = 0;
		cross_1v1_xiazhu_gold = 0;

		cross_multiuser_challenge_score = CROSS_MULTIUSER_CHALLENGE_INIT_SCORE;
		cross_multiuser_challenge_match_total_count = 0;
		cross_multiuser_challenge_match_win_count = 0;
		cross_multiuser_challenge_mvp_count = 0;
		cross_multiuser_challenge_add_honor = 0;
		has_sync_cross_multiuser_challenge_to_role = 0;

		cross_tuanzhan_score = 0;
		cross_tuanzhan_is_win = 0;
		cross_tuanzhan_side_rank = 0;
		cross_tuanzhan_has_sync_to_role = 0;

		cross_pasture_need_sync_to_role = 0;
		cross_pasture_get_score = 0;
		cross_pasture_get_score_times = 0;

		cross_boss_relive_times = 0;
		cross_boss_need_sync_to_role = 0;
		today_gather_ordinary_crystal_times = 0;							// 今日已采集普通水晶次数
		today_gather_treasure_crystal_times = 0;							// 今日已采集珍惜水晶次数
		tire_value = 0;														// 疲劳值

		cross_common_need_sync_to_role = 0;
		cross_common_use_gold = 0;

		attend_activity_num = 0;	
		cross_3v3_add_gongxun = 0;
		cross_attend_need_sync_to_role = 0;									
		reserve_ch = 0;
		reserve_sh = 0;

		cross_mizang_boss_relive_times = 0;	
		cross_mizang_boss_need_sync_to_role = 0;
		cross_mizang_boss_today_gather_ordinary_crystal_times = 0;	
		cross_mizang_boss_today_gather_treasure_crystal_times = 0;
		cross_mizang_boss_tire_value = 0;

		cross_youming_boss_relive_times = 0;
		cross_youming_boss_need_sync_to_role = 0;	
		cross_youming_boss_today_gather_ordinary_crystal_times = 0;	
		cross_youming_boss_today_gather_treasure_crystal_times = 0;	
		cross_youming_boss_tire_value = 0;
	}

	int cross_score_1v1;
	int cross_1v1_day_join_count;
	int cross_1v1_dur_win_count;										// 跨服1v1-连赢次数
	short cross_1v1_total_join_times;									// 跨服1v1-参加次数
	short cross_1v1_total_wim_times;									// 跨服1v1-胜利次数
	int cross_1v1_gongxun;												// 跨服1v1-功勋
	int cross_1v1_left_hp;
	short cross_1v1_add_honor;
	short has_sync_cross_1v1_record_to_role;
	int user_capabilit;
	char reserve_ch_list[72];											// 保留
	short cross_day_lose_1v1_count;
	short cross_1v1_day_match_fail_count;
	short cross_max_dur_win_1v1_count;
	char cross_1v1_xiazhu_seq;
	char cross_1v1_fetch_xiazhu_reward;
	short cross_1v1_xiazhu_gold;

	short cross_multiuser_challenge_add_honor;
	int cross_multiuser_challenge_score;									// 3v3积分
	short cross_multiuser_challenge_match_total_count;
	short cross_multiuser_challenge_match_win_count;
	short cross_multiuser_challenge_mvp_count;
	short has_sync_cross_multiuser_challenge_to_role;						// 是否已经同步到角色

	unsigned int cross_tuanzhan_score;										// 团战获得积分
	short cross_tuanzhan_is_win;											// 团战是否胜利，0 失败， 1 胜利
	short cross_tuanzhan_side_rank;											// 团战所在阵营内排名
	short cross_tuanzhan_has_sync_to_role;									// 团战数据是否已同步	
	short cross_pasture_need_sync_to_role;									// 跨服牧场数据是否需要同步(之前保留字段)

	int cross_pasture_get_score;											// 跨服牧场获得的积分
	int cross_pasture_get_score_times;										// 跨服牧场获得积分的次数

	short cross_boss_relive_times;											// 跨服boss场景使用的复活次数
	short cross_boss_need_sync_to_role;										// 跨服boss数据是否需要同步	
	int today_gather_ordinary_crystal_times;							    // 今日已采集普通水晶次数
	int today_gather_treasure_crystal_times;							    // 今日已采集珍惜水晶次数
	int tire_value;														    // 疲劳值

	// 通用数据回传-在跨服里消耗的内容
	int cross_common_need_sync_to_role;										// 通用数据 - 数据是否需要同步
	int cross_common_use_gold;												// 通用数据 - 消耗的元宝

	int attend_activity_num;												// 是否已经参加该活动（保存活动号）

	int cross_3v3_add_gongxun;                                              // 跨服3v3-功勋
	char cross_attend_need_sync_to_role;									// 参加该活动数据是否需要同步
	char reserve_ch;														// 保留
	short reserve_sh;

	short cross_mizang_boss_relive_times;											// 跨服秘藏boss场景使用的复活次数
	short cross_mizang_boss_need_sync_to_role;										// 跨服秘藏boss数据是否需要同步	
	int cross_mizang_boss_today_gather_ordinary_crystal_times;						// 跨服秘藏boss今日已采集普通水晶次数
	int cross_mizang_boss_today_gather_treasure_crystal_times;						// 跨服秘藏boss今日已采集珍惜水晶次数
	int cross_mizang_boss_tire_value;                                               // 跨服秘藏boss疲劳

	short cross_youming_boss_relive_times;
	short cross_youming_boss_need_sync_to_role;										// 跨服秘藏boss数据是否需要同步	
	int cross_youming_boss_today_gather_ordinary_crystal_times;						// 跨服秘藏boss今日已采集普通水晶次数
	int cross_youming_boss_today_gather_treasure_crystal_times;						// 跨服秘藏boss今日已采集珍惜水晶次数
	int cross_youming_boss_tire_value;                                               // 跨服秘藏boss疲劳
};

static const int CROSS_TUANZHAN_SCENE_KEY = 1;								// 团战场景KEY
static const int CROSS_TUANZHAN_RANK_LIST_SIZE = 20;						// 团战排名列表数量
static const int CROSS_TUANZHAN_RELIVE_POS_MAX_COUNT = 10;					// 复活点最大数量
static const int CROSS_TUANZHAN_PILLA_MAX_COUNT = 6;						// 柱子最大数量
static const int CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT = 10;					// 活动开启时间最配置数
static const int CROSS_TUANZHAN_ASSIST_VALID_TIME_S = 5;					// 玩家死亡前一段时间内的攻击者算是助攻者
static const int CROSS_TUANZHAN_NOTIFY_SCORE_RANK_LIST_TIME_S = 4;			// 每隔一段时间广播最新的积分排行榜信息
static const int CROSS_TUANZHAN_WINNER_REWARD_ITEM_MAX_COUNT = 2;			// 胜利阵营前几名奖励物品最大数量
static const int CROSS_TUANZHAN_DEFAULT_READY_TIME_S = 60;					// 活动默认等待时间（gm开活动时）
static const int CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S = 1200;					// 活动默认开启时间（gm开活动时）
static const int CROSS_TUANZHAN_SCORE_REWARD_CFG_MAX = 20;					// 跨服团战积分奖励配置最大数量
static const int CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX = 10;			// 跨服团战配置最大等级段
static const int CROSS_TUANZHAN_RANK_REWARD_CFG_MAX = 20;					// 跨服团战排行奖励配置最大数量

static const int CROSS_ACTIVITY_DEFAULT_READY_TIME_S = 60;					// 活动默认等待时间（gm开活动时）
static const int CROSS_ACTIVITY_DEFAULT_OPEN_TIME_S = 1200;					// 活动默认开启时间（gm开活动时）

enum CROSS_TUANZHAN_SIDE
{
	CROSS_TUANZHAN_SIDE_MIN = 0,

	CROSS_TUANZHAN_SIDE_0 = CROSS_TUANZHAN_SIDE_MIN,						// 对战方0
	CROSS_TUANZHAN_SIDE_1,													// 对战方1

	CROSS_TUANZHAN_SIDE_MAX
};

typedef char UserCrossRecordDataHex[sizeof(UserCrossRecordData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(UserCrossRecordDataHex) < 512);

#pragma pack(pop)

//-------------------------------------------------------------------------------------

static const int CROSS_PUBLIC_SCENE_KEY = 1;								// 跨服通用场景KEY
static const int CROSS_PASTURE_OPEN_TIME_MAX_COUNT = 10;					// 活动开启时间最配置数
static const int CROSS_PASTURE_RELIVE_POS_MAX_COUNT = 150;					// 怪物刷新坐标最大数量
static const int CROSS_PASTURE_MONSTER_CFG_MAX_COUNT = 50;					// 农场怪物配置最大数量
static const int CROSS_PASTURE_FENCE_CFG_MAX_COUNT = 6;						// 农场栅栏配置最大数量
static const int CROSS_PASTURE_SKILL_CFG_MAX_COUNT = 4;						// 农场技能配置最大数量
static const int CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT = 10;				// 活动开启时间最大配置数

//-------------------------------------------------------------------------------------
static const int CROSS_BOSS_MAX_HISTROY_RECROD = 5;						    // 跨服boss最大击杀记录
static const int MAX_CROSS_BOSS_PER_SCENE = 20;							    // 每个场景boss最大个数
static const int CROSS_BOSS_MAX_MONSTER_NUM = 200;							// 跨服boss小怪最大数量
static const int CROSS_BOSS_MAX_ORDINARY_CRYSTAL = 60;						// 跨服boss最大普通水晶
static const int CROSS_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM = 60;				// 跨服boss最大珍惜水晶
static const int CROSS_BOSS_SCENE_MAX = 5;									// 跨服boss场景最大个数
static const int CROSS_BOSS_MAX_DROP_RECROD = 20;							// 跨服boss最大掉落记录

static const int CROSS_BOSS_SCENE_KEY = 1;									// 跨服BOSS场景KEY
const static int CROSS_BOSS_MAX_GATHER_NUM = 70;							// 最大采集物数量
const static int CROSS_BOSS_MAX_RANK_NUM = 20;								// 跨服boss的伤害排名最大个数
static const int CROSS_BOSS_KILL_MONSTER_REWARD_COUNT = 10;					// 跨服boss击杀小怪奖励分段数量


//------------------------------------------------------------------------------------- 跨服组队
static const int CROSS_TEAM_FB_ROLE_MAX = 4;								// 跨服组队每个房间最大角色数
static const int CROSS_TEAM_FB_ROOM_MAX = 500;								// 跨服组队最大房间数

//------------------------------------------------------------------------------------- 跨服温泉
static const int CROSS_HS_SCENE_KEY = 1;									// 跨服温泉场景KEY

static const int CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX = 100;		// 跨服六界掉落日志最大数

//--------------------------------------------------------------------------------------   跨服排行榜
static const int CROSS_RANK_REWARD_COUNT_OLD = 1;								// 跨服排行榜奖励
static const int CROSS_RANK_REWARD_COUNT_REST = 4;								// 跨服排行榜奖励
static const int CROSS_RANK_REWARD_COUNT = CROSS_RANK_REWARD_COUNT_REST + CROSS_RANK_REWARD_COUNT_OLD;	// 跨服排行榜奖励

//---------------------------------------------------------------------------------------   跨服秘藏boss
static const int CROSS_MIZANG_BOSS_SCENE_MAX = 5;									// 跨服秘藏boss场景最大个数
static const int CROSS_MIZANG_BOSS_MAX_ORDINARY_CRYSTAL = 60;						// 跨服秘藏boss最大普通水晶
static const int MAX_CROSS_MIZANG_BOSS_PER_SCENE = 20;							    // 每个场景boss最大个数
static const int CROSS_MIZANG_BOSS_MAX_MONSTER_NUM = 200;							// 跨服秘藏boss小怪最大数量
static const int CROSS_MIZANG_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM = 60;				// 跨服秘藏boss最大珍惜水晶

//---------------------------------------------------------------------------------------   跨服幽冥boss
static const int CROSS_YOUMING_BOSS_SCENE_MAX = 5;									// 跨服幽冥boss场景最大个数
static const int CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL = 60;						// 跨服幽冥boss最大普通水晶
static const int MAX_CROSS_YOUMING_BOSS_PER_SCENE = 20;							    // 每个场景boss最大个数
static const int CROSS_YOUMING_BOSS_MAX_MONSTER_NUM = 200;							// 跨服幽冥boss小怪最大数量
static const int CROSS_YOUMING_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM = 60;				// 跨服幽冥boss最大珍惜水晶

//---------------------------------------------------------------------------------------

enum CROSS_TEAM_FB_STATE_TYPE
{
	CROSS_TEAM_FB_STATE_TYPE_READY = 0,										// 准备中
	CROSS_TEAM_FB_STATE_TYPE_WAIT,											// 等待中
	CROSS_TEAM_FB_STATE_TYPE_LEADER,										// 队长
	CROSS_TEAM_FB_STATE_TYPE_START,											// 副本已开始
};

enum CROSS_TEAM_FB_OPTION_TYPE
{
	CROSS_TEAM_FB_STATE_OPTION_INVALID = 0,

	CROSS_TEAM_FB_STATE_OPTION_CREATE,										// 创建房间
	CROSS_TEAM_FB_STATE_OPTION_JOIN,										// 加入房间
	CROSS_TEAM_FB_STATE_OPTION_REMOVE,										// 移出房间
	CROSS_TEAM_FB_STATE_OPTION_QUIT,										// 退出房间
	CROSS_TEAM_FB_STATE_OPTION_READY,										// 准备/取消准备
	CROSS_TEAM_FB_STATE_OPTION_START,										// 开始副本
	CROSS_TEAM_FB_STATE_OPTION_ENTER,										// 进入副本
	CROSS_TEAM_FB_STATE_OPTION_FINDMATE,									// 发送寻找队友传闻
	CROSS_TEAM_FB_STATE_OPTION_SET_AUTO_START,								// 设置自动开始

	CROSS_TEAM_FB_STATE_OPTION_MAX,
};

enum CROSS_TEAM_FB_OPTION2_TYPE
{
	CROSS_TEAM_FB_OPTION2_TYPE_INVALID = 0,

	CROSS_TEAM_FB_OPTION2_TYPE_DELETE_ROOM,									// 关闭房间
	CROSS_TEAM_FB_OPTION2_TYPE_FB_END,										// 副本结束

	CROSS_TEAM_FB_OPTION2_TYPE_MAX,
};

struct CrossTeamFBUserInfo
{
	CrossTeamFBUserInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		uuid = 0;
		prof = 0;
		sex = 0;
		camp = 0;
		user_state = CROSS_TEAM_FB_STATE_TYPE_READY;

		memset(name, 0, sizeof(name));

		capability = 0;
		layer = -1;
		room = -1;
	}

	long long uuid;
	char prof;
	char sex;
	char camp;
	char user_state;
	GameName name;
	int capability;
	int layer;
	int room;
};

struct CrossTeamFBRoomInfo
{
	CrossTeamFBRoomInfo() : need_capability(0), password(0), is_auto_start(0), leader_prof(0), leader_sex(0), fb_state(0), user_count(0)
	{
		memset(leader_name, 0, sizeof(leader_name));
		memset(user_uuid, 0, sizeof(user_uuid));
	}

	UniqueServerID unique_server_id;
	int need_capability;
	int password;
	int is_auto_start;
	GameName leader_name;
	char leader_prof;
	char leader_sex;
	char fb_state;
	char user_count;
	long long user_uuid[CROSS_TEAM_FB_ROLE_MAX];
};

struct CrossGuildBattleUserInfo
{
	CrossGuildBattleUserInfo() : role_id(INVALID_UNIQUE_USER_ID), guild_id(INVALID_UNIQUE_USER_ID),old_score(0), score(0),sos_times(0),kill_role_num(0),guild_post(0),
		hurt_roles(0), kill_flags(0), hurt_flags(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UniqueUserID role_id;
	UniqueUserID guild_id;
	int old_score;
	int score;
	int sos_times;
	GameName user_name;
	int kill_role_num;
	char guild_post;
	long long hurt_roles;							//对玩家伤害
	int kill_flags;									//击杀旗帜数
	long long hurt_flags;							//对旗帜伤害
};


struct CrossGuildBattleUserScoreRankInfo
{
	CrossGuildBattleUserScoreRankInfo() : role_id(INVALID_UNIQUE_USER_ID), guild_id(INVALID_UNIQUE_USER_ID), score(0) {}

	bool operator <(const CrossGuildBattleUserScoreRankInfo &other) const
	{
		return score > other.score;
	}


	UniqueUserID role_id;
	UniqueUserID guild_id;
	int score;
};

struct CrossGuildBattleRankItem
{
	CrossGuildBattleRankItem() : score(0), prof(0), sex(0), wuqi_id(0),shizhuang_wuqi(0),shizhuang_wuqi_is_special(0),
		shizhuang_body(0),shizhuang_body_is_special(0),halo_used_imageid(0),toushi_used_imageid(0),qilinbi_used_imageid(0),
		mask_used_imageid(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
		memset(guild_tuanzhang_name, 0, sizeof(guild_tuanzhang_name));
	}

	GuildName guild_name;
	GameName guild_tuanzhang_name;
	UniqueUserID guild_id;
	int score;
	char prof;
	char sex;
	ItemID wuqi_id;              // 武器ID
	char shizhuang_wuqi;          // 时装武器
	char shizhuang_wuqi_is_special;      // 时装武器
	char shizhuang_body;          // 时装身体
	char shizhuang_body_is_special;      // 时装身体
	short halo_used_imageid;        // 光环形象
	short toushi_used_imageid;        // 头饰形象id
	short qilinbi_used_imageid;        // 麒麟臂形象id
	short mask_used_imageid;        // 面具形象id
};

struct CrossGuildBattleSceneInfo
{
	CrossGuildBattleSceneInfo() {}

	CrossGuildBattleRankItem rank_list[CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM];
};

struct CrossGuildBattleRankLogItem
{
	CrossGuildBattleRankLogItem()
		: server_id(0)
		, score(0), tuanzhang_uuid(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
	}

	void Reset()
	{
		server_id = 0;
		score = 0;
		tuanzhang_uuid = 0;
		memset(guild_name, 0, sizeof(guild_name));
	}

	GuildName	guild_name;
	int			server_id;
	int			score;
	long long   tuanzhang_uuid;
};

struct CrossGuildBattleSceneLogInfo
{
	CrossGuildBattleSceneLogInfo() {}

	void Reset()
	{
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM; ++i)
		{
			rank_list[i].Reset();
		}
	}

	CrossGuildBattleRankLogItem rank_list[CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM];
};

struct CrossGuildBattleDropLog
{
	CrossGuildBattleDropLog() : timestamp(0), item_id(0), item_num(0)
	{
		memset(name, 0, sizeof(name));
	}

	GameName name;
	unsigned int timestamp;
	unsigned short item_id;
	short item_num;
};

////////////////////////////////////////////// 月黑风高 //////////////////////////////////////////////
static const int CROSS_DARK_NIGHT_MONSTER_POS_MAX_COUNT = 150;					// 怪物刷新坐标最大数量
static const int CROSS_DARK_NIGHT_RANK_COUNT = 20;								// 排行榜客户端人数
static const int CROSS_DARK_NIGHT_MONSTER_MAX_COUNT = 20;						// 怪物最大数量
static const int CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT = 10;						// 活动开启时间最配置数
static const int CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX = 5;						// boss位置数(一次刷新数)
static const int CROSS_DARK_NIGHT_BOSS_MAX_COUNT = 10;							// boss最大数量

//------------------------------------------------------------------------------------- 跨服猎鲲地带
enum LIEKUN_BOSS_TYPE
{
	LIEKUN_BOSS_TYPE_MAIN = 0,		// 区域BOSS
	LIEKUN_BOSS_TYPE_JY_1,			// 精英BOSS1
	LIEKUN_BOSS_TYPE_JY_2,			// 精英BOSS2
	LIEKUN_BOSS_TYPE_JY_3,			// 精英BOSS3
	LIEKUN_BOSS_TYPE_JY_4,			// 精英BOSS4

	LIEKUN_BOSS_TYPE_COUNT,
};

enum LIEKUN_ZONE_TYPE
{
	LIEKUN_ZONE_TYPE_MAIN = 0,		// 主区域
	LIEKUN_ZONE_TYPE_MINOR_1,		// 副区域1
	LIEKUN_ZONE_TYPE_MINOR_2,		// 副区域2
	LIEKUN_ZONE_TYPE_MINOR_3,		// 副区域3
	LIEKUN_ZONE_TYPE_MINOR_4,		// 副区域4

	LIEKUN_ZONE_TYPE_COUNT,
};

struct CrossLieKunFBSceneInfo
{
	CrossLieKunFBSceneInfo() : role_num(0), kill_zone_boss_guild_id(0) {}

	int role_num;
	GuildID kill_zone_boss_guild_id;
};

struct CrossLieKunFBUser
{
	CrossLieKunFBUser() : uuid(0), zone(0)
	{
		memset(gather_times, 0, sizeof(gather_times));
	}

	long long uuid;

	int zone;
	int gather_times[LIEKUN_ZONE_TYPE_COUNT][LIEKUN_BOSS_TYPE_COUNT];
};

//////////////////////////////////////////////// 跨服边境之地 相关定义 ////////////////////////////////////////////////
enum CROSS_BIANJINGZHIDI_TASK_TYPE
{
	CROSS_BIANJINGZHIDI_TYPE_INVALID = 0,
	CROSS_BIANJINGZHIDI_TYPE_GATHER = 1,													// 跨服边境之地 采集
	CROSS_BIANJINGZHIDI_TYPE_KILL_MONSTER = 2,												// 跨服边境之地 击杀野怪
	CROSS_BIANJINGZHIDI_TYPE_KILL_ROLE = 3,													// 跨服边境之地 击杀玩家
	CROSS_BIANJINGZHIDI_TYPE_KILL_BOSS = 4,													// 跨服边境之地 击杀boss
	CROSS_BIANJINGZHIDI_TYPE_MAX,
};

static const int CROSS_BIANJINGZHIDI_TASK_MAX = 5;											// 跨服边境之地 任务最大个数
static const int CROSS_BIANJINGZHIDI_REFLUSH_NOTICE_TIME = 10;								// 跨服边境之地 刷新提示时间

static const int CROSS_BIANJINGZHIDI_MAX_REWARD_ITEM_COUNT = 10;							// 跨服边境之地 奖励物品最大数
static const int CROSS_BIANJINGZHIDI_OWNER_ROLE_AND_BOSS_DISTANCE = 25;						// 跨服边境之地 归属权最大距离
static const int CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE = 5;										// 跨服边境之地 BOSS最大类型数

// 跨服边境之地 任务信息
struct CrossBianJingZhiDiTaskInfo															// 角色身上拥有的跨服边境之地任务信息 
{
	CrossBianJingZhiDiTaskInfo() : task_id(0), can_limit_flag(false),
		task_type(0), param_id(0), param_count(0), cur_param_value(0), is_finish(0), is_double_reward(false), is_fetch_reward(false),
		item_count(0) {}

	bool IsFinish() { return is_finish != 0; }
	bool IsMatch(short _task_type, int _param_id)
	{
		return task_type == _task_type && param_id == _param_id;
	}

	void AddParamVal()
	{
		++cur_param_value;

		if (cur_param_value >= param_count)
		{
			is_finish = 1;
		}
	}

	bool IsDoubleReward() { return is_double_reward; }
	bool IsFetchReward() { return is_fetch_reward; }

	short task_id;																	// 任务ID
	bool can_limit_flag;															// 是否可限时双倍

	int task_type;
	int param_id;																	// 任务关联的id(采集物，怪物)
	int param_count;																// 任务关联的采集物或怪物的数量
	int cur_param_value;															// 当前完成值
	int is_finish;																	// 是否完成任务
	bool is_double_reward;
	bool is_fetch_reward;

	int item_count;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

// 跨服边境之地 角色信息
struct CrossBianJingZhiDiUser
{
	CrossBianJingZhiDiUser() : user_id(INVALID_UNIQUE_USER_ID), is_send_item_info(false), task_finish_num(0), is_fetch_reward(false), lianzhan(0), assist(0),
		is_brocast_gather_buff(false), gathering_gather_id(0), bianjingzhidi_buff_timestamp(0), sos_times(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(gather_box_count, 0, sizeof(gather_box_count));
	}

	UniqueUserID user_id;
	GameName user_name;

	bool is_send_item_info;															// 跨服边境之地 完成全部任务显示结算面板
	int task_finish_num;
	bool is_fetch_reward;															// 跨服边境之地 完成任务额外奖励
	int lianzhan;
	int assist;
	bool is_brocast_gather_buff;
	int gathering_gather_id;														// 正在采集的采集物ID
	unsigned int bianjingzhidi_buff_timestamp;										// 跨服边境之地 玩家buff时间
	short sos_times;																// 召集次数

	char gather_box_count[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];						// 采集每个宝箱类型次数
	std::vector<CrossBianJingZhiDiTaskInfo> bianjingzhidi_task_list;				// 跨服边境之地 玩家的任务信息
	std::map<int, int> role_get_item_map;											// 跨服边境之地 玩家获得物品信息<item_id, item_num>
	std::map<UniqueUserID, time_t> hurt_map;										// 伤害我的用户列表
};

typedef std::map<UniqueUserID, CrossBianJingZhiDiUser> CrossBianJingZhiDiUserMap;
typedef std::map<UniqueUserID, CrossBianJingZhiDiUser>::iterator CrossBianJingZhiDiUserMapIt;



////////////////////////////////////////////// 跨服活动数据 //////////////////////////////////////////////
struct CrossActivityData
{
	static const int CROSS_DATA_LENGTH = 32;

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	char data_list[CROSS_DATA_LENGTH];
};

struct CrossActivityParam // 跨服活动数据保存跨服中
{
	CrossActivityParam() { this->Reset(); }

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	CrossActivityData data_list[CROSS_ACTIVITY_TYPE_MAX];
};

struct ActivityDataParam
{
	ActivityDataParam() { this->Reset(); }

	void Reset()
	{
		season_count = 1;
		day_index = 0;
		mon_day_index = 0;
		reserve_ch = 0;
		activity_begin_day_index = 0;
	};

	char season_count;	// 第几赛季
	char day_index;		// 当前赛季天数
	char mon_day_index;	// 当前月天数
	char reserve_ch;
	int activity_begin_day_index;
};

static const int CROSS_RA_CHONGZHI_RANK_MAX_NUM = 50;						// 跨服随机活动-充值排行最大个数
static const int CROSS_RA_CONSUME_RANK_MAX_NUM = 50;						// 跨服随机活动-消费排行最大个数

#endif // __CROSS_DEF_HPP__

