#ifndef __CHALLENGE_FIELD_DEF_HPP__
#define __CHALLENGE_FIELD_DEF_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

#include "servercommon/struct/skilllistparam.h"
#include "servercommon/xianjiandef.hpp"
#include "servercommon/maildef.hpp"

#include "gameworld/gameworld/gamedef.h"

static const int CHALLENGE_FIELD_MAX_USER = 50000;						// 最大玩家个数
static const int CHALLENGE_FIELD_OPPONENT_MAX = 4;						// 对手个数
const static int CHALLENGE_FIELD_MAX_USER_NUM = 500;					// 竞技场玩家最多数量（用于协议）

static const int CHALLENGE_FIELD_SAVE_INTERVAL = 30;					// 保存间隔

static const int CHALLENGE_FIELD_ONCE_OPER_USER_COUNT = 256;			// 竞技场一次操作玩家个数
static const int CHALLENGE_FIELD_INCREASE_STEP = 512;					// 竞技场玩家列表长度增长步长

static const int CHALLENGE_FIELD_WAITING_TIME = 12;						// 竞技场等待超时时间
static const int CHALLENGE_FIELD_READY_TIME = 4;						// 竞技场准备时间
static const int CHALLENGE_FIELD_FIGHT_TIME = 60;						// 竞技场竞技时间
static const int CHALLENGE_FIELD_DELAY_KICK_TIME = 10;					// 竞技场延迟踢人时间

static const int CHALLENGE_FIELD_TOTAL_TIME = CHALLENGE_FIELD_READY_TIME + CHALLENGE_FIELD_FIGHT_TIME;

static const int CHALLENGE_FIELD_REPORT_MAX_COUNT = 20;					// 个人战报最大数量
static const int CHALLENGE_FIELD_JIFEN_REWARD_MAX_COUNT = 32;			// 积分奖励最大数
static const int CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT = 10;			// 幸运排名最大数
static const int CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM = 100;				// 关注榜单前100

static const int CHALLENGE_FIELD_GIVE_RANK_REWARD_HOUR = 19;			// 发放排名奖励的小时（19点）

const static int CHALLENGE_FIELD_ROBOT_SERVER_ID = 2047;				// 机器人标记
const static int CHALLENGE_FIELD_ROBOT_PLAT_TYPE = 0;					// 机器人平台标记

static const int CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT = 10;		// 记录排行最大数量

/////////////////////////////////////// 跨服竞技场 //////////////////////////////////////////////////////
static const int CROSS_CHALLENGE_FIELD_MAX_USER = 1000;					// 跨服竞技场最大玩家个数
static const int CROSS_CHALLENGE_FIELD_SAVE_INTERVAL = 1;				// 跨服竞技场同步间隔
static const int CROSS_CHALLENGE_FIELD_ONCE_OPER_USER_COUNT = 3;		// 跨服竞技场一次同步操作玩家个数

enum CHALLENGE_FIELD_SIDE
{
	CHALLENGE_FIELD_SIDE_SPONSOR = 0,					// 发起方
	CHALLENGE_FIELD_SIDE_OPPONENT,						// 对方
	CHALLENGE_FIELD_SIDE_MAX,
};

enum CHALLENGE_FIELD_STATE
{
	CHALLENGE_FIELD_STATE_WAITING = 0,					// 发起方等待中
	CHALLENGE_FIELD_STATE_READY,						// 准备
	CHALLENGE_FIELD_STATE_FIGHTING,						// 战斗开始
	CHALLENGE_FIELD_STATE_FINISH,						// 战斗完成
};

#pragma pack(push, 4)

struct ChallengeParam
{
	ChallengeParam() { this->Reset(); }

	void Reset()
	{
		plat_type = 0;
		role_id = 0;
		memset(role_name, 0, sizeof(role_name));
		avatar = 0;
		sex = 0;
		prof = 0;
		camp = 0;
		level = 0;

		capability = 0;

		max_hp = 0;
		gongji = 0;
		fangyu = 0;
		mingzhong = 0;
		shanbi = 0;
		baoji = 0;
		jianren = 0;
		move_speed = 0;
		fujiashanghai = 0;
		dikangshanghai = 0;

		jingzhun_per = 0;
		baoji_per = 0;
		kangbao_per = 0;
		pofang_per = 0;
		mianshang_per = 0;

		constant_zengshang = 0;
		constant_mianshang = 0;

		vip_level = 0;
		guild_post = 0;
		guild_id = 0;

		memset(guild_name, 0, sizeof(guild_name));

		server_id = 0;

		avatar_timestamp = 0;
		reserve_char = -1;
		reserve_ch = 0;

		memset(resreve_ch_list, 0, sizeof(resreve_ch_list));
	}

	struct SkillInfo
	{
		SkillInfo() { this->Reset(); }

		void Reset() 
		{
			skill_id = 0;
			level = 0;
		}

		unsigned short skill_id;						// 技能ID
		unsigned short level;							// 技能等级
	};

	int				plat_type;							// 平台类型
	int				role_id;							// ID
	GameName		role_name;							// 名字
	char			avatar;								// 头像
	char			sex;								// 性别
	char			prof;								// 职业
	char			camp;								// 阵营
	int				level;								// 等级

	int				capability;							// 战斗力

	Attribute		max_hp;								// 血量
	Attribute		gongji;								// 攻击
	Attribute		fangyu;								// 防御
	Attribute		mingzhong;							// 命中
	Attribute		shanbi;								// 闪避
	Attribute		baoji;								// 暴击
	Attribute		jianren;							// 坚韧
	Attribute		move_speed;							// 移动速度

	Attribute		fujiashanghai;
	Attribute		dikangshanghai;
	Attribute		jingzhun_per;						// 精准万分比
	Attribute		baoji_per;							// 暴击万分比
	Attribute		kangbao_per;						// 抗暴万分比
	Attribute		pofang_per;							// 破防万分比
	Attribute		mianshang_per;						// 免伤万分比

	Attribute		constant_zengshang;					// 固定增伤
	Attribute		constant_mianshang;					// 固定免伤

	char			vip_level;							// vip等级
	char			guild_post;							// 仙盟职位
	char			reserve_char;					
	char			reserve_ch;							// 保留

	GuildID			guild_id;							// 仙盟id
	GuildName		guild_name;							// 仙盟名字

	int				server_id;							// 所在服务器id
	char			resreve_ch_list[68];				// 保留

	long long		avatar_timestamp;					// 头像修改时间戳

	SkillInfo		skill_list[SkillListParam::MAX_SKILL_NUM]; // 技能信息
};

typedef char ChallengeParamHex[sizeof(ChallengeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeParamHex) < 2048);

struct ChallengeUserCommonData
{
	ChallengeUserCommonData() { this->Reset(); }

	void Reset() 
	{
		continue_wins = 0;
		best_rankpos = -1;
		join_times = 0;
		buy_join_times = 0;
		jifen = 0;
		jifen_reward_flag = 0;
		last_online_time = 0;
		reward_guanghui = 0;
		reward_bind_gold = 0;
		liansheng = 0;
		buy_buff_times = 0;
		reward_day = 0;
		memset(reward_item_list, 0, sizeof(reward_item_list));
	}

	void OnResetData()
	{
		join_times = 0;
		buy_join_times = 0;
		jifen = 0;
		jifen_reward_flag = 0;
		buy_buff_times = 0;
	}

	int continue_wins;
	int best_rankpos;
	int join_times;
	int buy_join_times;
	int jifen;
	int jifen_reward_flag;
	long long last_online_time;
	int reward_guanghui;
	int reward_bind_gold;
	int liansheng;
	int buy_buff_times;
	int reward_day;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

typedef char ChallengeUserCommonDataHex[sizeof(ChallengeUserCommonData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeUserCommonDataHex) < 256);

struct ChallengeUserReport								// 战报
{
	struct ReportItem
	{
		ReportItem() { this->Reset(); }

		void Reset() 
		{
			challenge_time = 0;
			target_uid = 0;
			memset(target_name, 0, sizeof(target_name));
			is_sponsor = 0;
			is_win = 0;
			reserve_sh = 0;
			old_rankpos = 0;
			new_rankpos = 0;
		}

		bool Invalid() const { return (0 == challenge_time); }

		unsigned int		challenge_time;					// 时间
		long long			target_uid;						// 对方id
		GameName			target_name;					// 对方名称
		char				is_sponsor;						// 自己是否为挑战者
		char				is_win;							// 是否胜利
		short				reserve_sh;						// 保留
		unsigned short		old_rankpos;					// 竞技前排名
		unsigned short		new_rankpos;					// 竞技后排名
	};

	ChallengeUserReport()
	{
		this->Reset();
	}

	void Reset()
	{
		reprot_count = 0;
	}

	short reprot_count;
	ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
};

typedef char ChallengeUserReportHex[sizeof(ChallengeUserReport) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeUserReportHex) < 2304);

struct ChallengeFieldDataParam
{
	struct LuckyUserItem
	{
		LuckyUserItem() { this->Reset(); }

		void Reset()
		{
			rank_pos = 0;
			user_id = 0;
			memset(user_name, 0, sizeof(user_name));
		}

		int rank_pos;
		int user_id;
		GameName user_name;
	};

	ChallengeFieldDataParam() { this->Reset(); }

	void Reset()
	{
		next_give_luckyreward_timestamp = 0;
		is_luckyreward_gived = 0;
		is_rankreward_gived = 0;
		rankreward_percent = 0;
		next_give_rankreward_timestamp = 0;

		for (int i = 0; i < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT; ++ i)
		{
			luck_rank_pos_list[i] = -1;
		}
		memset(reward_rank_uid_list, 0, sizeof(reward_rank_uid_list));

		reward_opengame_day = 0;
		clear_data_version = 0;
	}

	unsigned int next_give_luckyreward_timestamp;
	char is_luckyreward_gived;
	char is_rankreward_gived;
	short rankreward_percent;
	unsigned int next_give_rankreward_timestamp;

	UNSTD_STATIC_CHECK(10 == CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT);

	int luck_rank_pos_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	LuckyUserItem lucky_user_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	int reward_rank_uid_list[CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT];				// 记录结算前10名玩家UID 

	int reward_opengame_day;															// 结算开服第几天的奖励
	char clear_data_version;
}; 

typedef char ChallengeFieldDataParamHex[sizeof(ChallengeFieldDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChallengeFieldDataParamHex) < 1024);

#pragma pack(pop)

#endif // __CHALLENGE_FIELD_DEF_HPP__

