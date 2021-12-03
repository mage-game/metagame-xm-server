/******************************************************************************************
FileName: campparam.hpp
Author: dreamwang
Description: 军团在数据库中的定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GUILD_PARAM_HPP__
#define __GUILD_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/guilddef.hpp"

#include "servercommon/struct/itemlistparam.h"

#define SERIALIZE_GUILD_PARAM(SERIALIZER, PARAM)					\
{																	\
	SERIALIZER.Reset((void*)&PARAM, sizeof(PARAM));					\
	SERIALIZER.MoveCurPos(sizeof(PARAM));							\
}	

#define DESERIALIZE_GUILD_PARAM(DESERIALIZER, PARAM)				\
{																	\
	if (DESERIALIZER.Size() <= sizeof(PARAM))						\
	{																\
	memcpy(&PARAM, DESERIALIZER.Ptr(), DESERIALIZER.Size());		\
	}																\
}

static const int GUILD_MAZE_LAYER_MAX = 5;			// 迷宫层数
static const int GUILD_MAZE_LAYER_DOOR_MAX = 5;		// 迷宫门数

class GuildApplyForParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct ApplyFor
	{
		char change_state;
		char index; 

		int uid;
		unsigned int applyfor_time;
	};

	int count;
	ApplyFor apply_for_list[MAX_APPLY_FOR_COUNT];
};

class GuildMemberParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct MemberCommonParam
	{
		void Reset()
		{
			last_signin_timestamp = 0;
			signin_count_day = 0;
			has_join_guild_battle = 0;
			reserve_sh = 0;
			gongzi = 0;
		}

		unsigned int last_signin_timestamp;							// 上次工会签到时间
		char signin_count_day;										// 月签到次数
		char has_join_guild_battle;									// 是否参与了上一次六界战场
		short reserve_sh;
		int gongzi;													// 工资
	};

	typedef char MemberCommonDataParamHex[sizeof(MemberCommonParam) * 2 + 1];
	UNSTD_STATIC_CHECK(sizeof(MemberCommonDataParamHex) < 256);

	struct Member
	{
		char change_state;
		char index;

		int uid;
		char post;

		int gongxian;
		int total_gongxian;

		long long join_time;
		long long last_login_time;

		int lucky_color;
		
		int today_guild_pao_saizi_times;					// 每天公会抛骰子次数
		long long today_last_guild_pao_saizi_time;			// 最后一次抛骰子时间
		int today_guild_saizi_score;						// 每天骰子积分

		int maze_layer;
		long long maze_complete_time;

		MemberCommonParam common_data;
	};

	int count; 
	Member member_list[MAX_MEMBER_COUNT];
};

class GuildEventParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct Event
	{
		char change_state;
		char index;

		int event_type;
		GameName event_owner;
		int event_owner_post;
		unsigned int event_time;
		char big_event;
		char cannot_remove;

		int param0;
		int param1;
		int param2;
		int param3;
		GuildEventStrParam sparam0;
	};

	int count; 
	Event event_list[MAX_ALL_GUILD_EVENT_COUNT];
};

struct GuildCommonDataParam
{
	GuildCommonDataParam() { this->Reset(); }

	void Reset()
	{
		kill_world_boss_count = 0;
		xianmeng_bipin_kill_boss_count = 0;
		guild_yesterday_capability = 0;

		kill_csa_boss_num = 0;
		guild_beast_today_open_times = 0;

		guild_fb_open_times = 0;
		guild_bonfire_open_times = 0;

		xianmengzhan_rank_value = 0;

		totem_level = 0;
		totem_exp = 0;

		boss_normal_call_count = 0;
		boss_super_call_count = 0;
		boss_level = 0;
		boss_exp = 0;

		curr_week_worldboss_kill_num = 0;
		last_week_worldboss_kill_num = 0;
		last_kill_world_boss_time = 0;

		boss_super_call_uid = 0;

		totem_exp_today = 0;

		default_member_count = 0;
		extend_member_count = 0;

		guildbattle_rank = 0;
		territorywar_rank = 0;
		daily_kill_redbag_boss_times = 0;
		daily_guild_relive_times = 0;

		memset(maze_layer_list, 0, sizeof(maze_layer_list));
		is_auto_clear = 1;

		join_guild_battle_flag = 0;
		avatar_timestamp = 0;
		avatar_changed = 0;
		
		today_yunbiao_isstart = 0;
		today_yunbiao_rob_count = 0;

		join_new_guild_battle_flag = 0;

		guild_tianci_tongbi_day_count = 0;

		today_kill_cross_boss_num = 0;
		
		guild_total_gongzi = 0;

		cross_goal_kill_cross_boss = 0;
		cross_goal_kill_baizhan_boss = 0;
		corss_goal_guild_flag = 0;
		
		guild_kill_boss_num = 0;
	}

	int kill_world_boss_count;
	int xianmeng_bipin_kill_boss_count;
	int guild_yesterday_capability;

	short kill_csa_boss_num;
	short guild_beast_today_open_times;
	
	short guild_fb_open_times;
	short guild_bonfire_open_times;

	long long xianmengzhan_rank_value;

	int totem_level;
	int totem_exp;

	char boss_normal_call_count;
	char boss_super_call_count;
	short boss_level;
	int boss_exp;

	int curr_week_worldboss_kill_num;
	int last_week_worldboss_kill_num;
	unsigned int last_kill_world_boss_time;

	int boss_super_call_uid;

	int totem_exp_today;

	int default_member_count;
	int extend_member_count;

	int guildbattle_rank;
	int territorywar_rank;
	int daily_kill_redbag_boss_times;
	int daily_guild_relive_times;

	short maze_layer_list[GUILD_MAZE_LAYER_MAX];
	short is_auto_clear;

	int join_guild_battle_flag;
	unsigned long long avatar_timestamp;
	char avatar_changed;
	
	char today_yunbiao_isstart;
	short today_yunbiao_rob_count;

	int join_new_guild_battle_flag;

	int guild_tianci_tongbi_day_count;

	int today_kill_cross_boss_num;
	
	int guild_total_gongzi;							// 仙盟总工资
	int cross_goal_kill_cross_boss;					// 跨服目标击杀神域boss
	int cross_goal_kill_baizhan_boss;				// 跨服目标击杀百战boss
	int corss_goal_guild_flag;

	int guild_kill_boss_num;						// 仙盟榜boss击杀数
};

typedef char GuildCommonParamDataHex[sizeof(GuildCommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildCommonParamDataHex) < 512);

typedef char GuildBossParamDataHex[sizeof(GuildBossItem) * GUILD_BOSS_MAX_LEVEL * 2  + 1];
UNSTD_STATIC_CHECK(sizeof(GuildBossParamDataHex) < 2048);

typedef char GuildStorgeDataHex[sizeof(ItemDataWrapper) * GUILD_STORAGE_MAX_GRID_NUM * 2  + 1];
UNSTD_STATIC_CHECK(sizeof(GuildStorgeDataHex) < 24576);

typedef char GuildEnemyInfoHex[sizeof(GuildEnemyInfo) * GUILD_ENEMY_RANK_MAX_COUNT * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildEnemyInfoHex) < 13200);

class GuildBaseParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	GuildID guild_id;
	GuildName guild_name;
	long long create_time;

	int guild_level;
	int guild_exp;
	unsigned int guild_last_uplevel_timestamp;

	int storage_use_limit;
	int shop_use_limit;
	unsigned int last_update_dayid;
	unsigned int last_active_time;
	int warning_dismiss_days;
	int applyfor_setup;

	char camp;
	GuildNotice notice;

	int auto_kickout_setup;

	int guild_max_capability;

	int applyfor_need_capability;
	int applyfor_need_level;

	int callin_times;

	short monster_finish_seq;
	GameName monster_finish_username;

	GuildCommonDataParam common_data_param;

	int active_degree;

	GuildBossItem guild_boss_list[GUILD_BOSS_MAX_LEVEL];
	ItemDataWrapper storge_item_list[GUILD_STORAGE_MAX_GRID_NUM];

	GuildEnemyInfo guild_enemy_list[GUILD_ENEMY_RANK_MAX_COUNT];
};

class AllGuildParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct GuildParam
	{
		GuildBaseParam base_param;
		GuildApplyForParam apply_for_param;
		GuildMemberParam member_param;
		GuildEventParam event_param;
	};

	int count;
	GuildParam guild_list[MAX_ONCE_GUILD_COUNT];
};

#endif  // __GUILD_PARAM_HPP__

