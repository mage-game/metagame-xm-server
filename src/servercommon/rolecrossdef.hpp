#ifndef __ROLE_CROSS_DEF_HPP__
#define __ROLE_CROSS_DEF_HPP__

#include "servercommon.h"
#include "servercommon/crossdef.hpp"

#pragma pack(push, 4)

// 角色跨服掉线记录.  用于部分场景掉线重连。在活动的场景逻辑里面设置记录，并不是每个活动都适用掉线重连
struct RoleCrossLogoutRecord
{
	// 注意对齐
	RoleCrossLogoutRecord() { this->Reset(); }
	
	void Reset()
	{
		logout_flag = 0;
		cross_activity_type = 0;

		sceneId = 0;
		sceneKey = 0;
		logout_timestamp = 0;

		param = 0;
	}

	short logout_flag;				// 0表示无效，1表示掉线
	short cross_activity_type;		// 参加的跨服活动类型

	int sceneId;
	int sceneKey;
	unsigned int logout_timestamp;	// 掉线时间戳

	long long param;
};

struct RoleCrossParam
{
	RoleCrossParam() { this->Reset(); }

	void Reset() 
	{
		cross_original_uuid = 0;
		merge_server_id = 0;

		cross_xiuluo_tower_buy_realive_count = 0;
		cross_xiuluo_tower_add_gongji_per = 0;
		cross_xiuluo_tower_add_hp_per = 0;

		cross_1v1_match_time = 0;
		cross_1v1_info_last_clear_time = 0;
		cross_1v1_score = 0;
		cross_1v1_day_join_count = 0;

		cross_1v1_today_buy_times = 0;
		cross_1v1_fetch_score_reward_flag = 0;
		cross_1v1_join_time_reward_flag = 0;
		reserve_sh1 = 0;
		cross_1v1_dur_win_count = 0;
		cross_1v1_dur_win_max_count = 0;
		cross_1v1_curr_activity_begin_timestap = 0;
		cross_1v1_curr_activity_add_score = 0;
		cross_1v1_gongxun = 0;
		memset(cross_1v1_season_reward, -1, sizeof(cross_1v1_season_reward));
		memset(cross_1v1_season_reward_use_old, 0, sizeof(cross_1v1_season_reward_use_old));
		cross_1v1_total_join_times = 0;                                  

		cross_1v1_cur_join_season = 0;
		cross_1v1_total_win_times = 0;

		cross_pasture_today_get_score_times = 0;

		cross_multiuser_challenge_matching_state = -1;
		cross_multiuser_challenge_score = CROSS_MULTIUSER_CHALLENGE_INIT_SCORE;
		cross_multiuser_challenge_total_match_count = 0;
		cross_multiuser_challenge_win_match_count = 0;
		cross_multiuser_challenge_mvp_count = 0;

		cross_pasture_score = 0;

		today_gather_ordinary_crystal_num = 0;
		today_gather_treasure_crystal_num = 0;
		today_kill_boss_num = 0;
		cross_boss_relive_times = 0;
		cross_boss_enter_times_limit = 0;
		cross_boss_scene_level = 0;

		cross_guildbattle_tuanzhang_prof = 0;
		cross_guildbattle_tuanzhang_sex = 0;

		cross_team_fb_pass_flag = 0;
		cross_team_fb_day_count = 0;

		cross_guildbattle_scene_id = 0;
		memset(cross_guild_tuanzhang_name, 0, sizeof(cross_guild_tuanzhang_name));
		cross_guildbattle_server_reward_flag = 0;
		cross_guildbattle_guild_reward_flag = 0;
		cross_guildbattle_recalc_time = 0;

		memset(cross_guildbattle_task_reward_flag, 0, sizeof(cross_guildbattle_task_reward_flag));
		memset(cross_guildbattle_task_record, 0, sizeof(cross_guildbattle_task_record));

		cross_xiuluotower_daily_drop_title_get_num=0;					
		cross_xiuluotower_daily_gold_box_title_get_num = 0;				
		cross_xiuluotower_daily_drop_mount_get_num = 0;	
		cross_guild_battle_daily_drop_wing_get_num = 0;
		cross_xiuluo_tower_gather_buff_end_timestamp = 0;

		cross_multiuser_challenge_dur_win_count = 0;
		memset(cross_3v3_season_reward_use_old, 0, sizeof(cross_3v3_season_reward_use_old));
		memset(cross_3v3_season_reward, -1, sizeof(cross_3v3_season_reward));

		cross_3v3_score_reuse_flag = 0;
		cross_3v3_join_reward_fetch_flag = 0;
		cross_multiuser_challenge_month_match_count = 0;

		cross_3v3_gongxun_reward_fetch_flag = 0;
		cross_3v3_gongxun_value = 0;
		cross_3v3_cur_join_season = 0;
		
		cross_liekun_scene_zone = 0;
		cross_3v3_logout_flag = 0;
		cross_husong_shuijing_enter_type = 0;

		memset(cross_boss_concern_flag, 0, sizeof(cross_boss_concern_flag));

		cross_guildbattle_daily_reward_flag = 0;

		role_cross_logout_record.Reset();

		cross_common_boss_enter_type = 0;
		cross_common_boss_enter_boss_id = 0;
		cross_common_boss_enter_scene_id = 0;

		memset(cross_mizang_boss_concern_flag, 0, sizeof(cross_mizang_boss_concern_flag));
		cross_mizang_boss_today_kill_boss_num = 0;
		cross_mizang_boss_today_gather_ordinary_crystal_num = 0;
		cross_mizang_boss_today_gather_treasure_crystal_num = 0;
		cross_mizang_boss_scene_level = 0;
		cross_mizang_boss_relive_times = 0;

		memset(cross_youming_boss_concern_flag, 0, sizeof(cross_youming_boss_concern_flag));
		cross_youming_boss_today_kill_boss_num = 0;
		cross_youming_boss_today_gather_ordinary_crystal_num = 0;
		cross_youming_boss_today_gather_treasure_crystal_num = 0;
		cross_youming_boss_scene_level = 0;
		cross_youming_boss_relive_times = 0;

		memset(cross_1v1_season_reward_use, 0, sizeof(cross_1v1_season_reward_use));
		memset(cross_3v3_season_reward_use, 0, sizeof(cross_3v3_season_reward_use));

		cross_consume_gold_num = 0;
		cross_daily_consume_gold_num = 0;
		cross_guildbattle_tuanzhang_uuid = 0;

		enter_cross_move_to_pos_x = 0;
		enter_cross_move_to_pos_y = 0;

		cross_1v1_cur_join_season_dayid = 0;
		cross_3v3_cur_join_season_dayid = 0;
	}

	long long cross_original_uuid;										// 原服唯一ID
	int merge_server_id;												// 合服ID
	short cross_xiuluo_tower_buy_realive_count;							// 跨服修罗他购买复活次数
	char cross_xiuluo_tower_add_gongji_per;								// 跨服修罗塔增加攻击比例
	char cross_xiuluo_tower_add_hp_per;									// 跨服修罗塔增加气血比例

	//////////////////////////////////////////	  跨服1v1活动	 ///////////////////////////////////////////////
	unsigned int cross_1v1_match_time;									// 跨服1v1参与匹配时间
	unsigned int cross_1v1_info_last_clear_time;						// 跨服1v1上一次信息清理时间
	int cross_1v1_score;												// 跨服1v1积分（威望）
	int cross_1v1_day_join_count;										// 跨服1v1今日参与次数

	short cross_1v1_today_buy_times;									// 跨服1v1今日购买次数
	short cross_1v1_fetch_score_reward_flag;							// 跨服1v1领取积分奖励标记
	short cross_1v1_join_time_reward_flag;								// 跨服1v1参与次数领取标记
	short reserve_sh1;
	short cross_1v1_dur_win_count;										// 跨服1v1连胜数
	short cross_1v1_dur_win_max_count;									// 跨服1v1最大连胜数
	unsigned int cross_1v1_curr_activity_begin_timestap;				// 跨服1v1本场活动开始时间戳
	int cross_1v1_curr_activity_add_score;								// 跨服1v1本场活动增加的威望
	int cross_1v1_gongxun;                                              // 跨服1v1功勋
	char cross_1v1_season_reward[CROSS_1V1_SEASON_MAX];                 // 跨服1v1 激活戒指

	char cross_1v1_season_reward_use_old[2];							// 跨服1v1 使用戒指(废弃，重新在后面扩展了)
	short cross_1v1_total_join_times;                                   // 跨服1v1 赛季总的参与次数

	int cross_1v1_cur_join_season;                                      // 当前参加1v1第几赛季
	short cross_1v1_total_win_times;                                    // 跨服1v1 赛季总的胜利次数
	//-----------------------------------------------------------------------------------------------------------
	short cross_pasture_today_get_score_times;							// 跨服牧场，今日获得积分的次数

	int cross_multiuser_challenge_matching_state;						// 跨服3V3当前匹配状态
	int cross_multiuser_challenge_score;								// 跨服3V3分数
	int cross_multiuser_challenge_total_match_count;					// 跨服3V3一共比赛场数(暂时没用)
	int cross_multiuser_challenge_win_match_count;						// 跨服3V3获胜比赛场数
	int cross_multiuser_challenge_mvp_count;							// 跨服3V3MVP次数
	

	unsigned int cross_pasture_score;									// 跨服牧场，总积分

	int today_kill_boss_num;											// 跨服boss今日击杀boss数量
	short cross_boss_relive_times;										// 跨服boss场景复活次数
	short cross_boss_enter_times_limit;
	int today_gather_ordinary_crystal_num;								// 跨服boss采集普通水晶次数
	int today_gather_treasure_crystal_num;								// 跨服boss采集珍惜水晶次数

	short cross_boss_scene_level;
	char cross_guildbattle_tuanzhang_prof;
	char cross_guildbattle_tuanzhang_sex;

	int cross_team_fb_pass_flag;										// 跨服组队本通关标记
	int cross_team_fb_day_count;										// 跨服组队本每日次数

	int cross_guildbattle_scene_id;										// 跨服帮派进入场景id
	GameName cross_guild_tuanzhang_name;								// 跨服帮派团长名字
	int cross_guildbattle_server_reward_flag;							// 跨服帮派战全服奖励
	int cross_guildbattle_guild_reward_flag;							// 跨服帮派战帮派奖励
	int cross_guildbattle_recalc_time;
	int cross_guildbattle_task_reward_flag[CROSS_GUILDBATTLE_MAX_SCENE_NUM];							// 跨服帮战任务完成标志
	int cross_guildbattle_task_record[CROSS_GUILDBATTLE_MAX_SCENE_NUM][CROSS_GUILDBATTLE_MAX_TASK_NUM]; // 跨服帮战任务记录

	char cross_xiuluotower_daily_drop_title_get_num;					// 跨服修罗塔掉落称号获得数
	char cross_xiuluotower_daily_gold_box_title_get_num;				// 跨服修罗塔金箱子称号获得数
	char cross_xiuluotower_daily_drop_mount_get_num;					// 跨服修罗塔掉落坐骑获得数
	char cross_guild_battle_daily_drop_wing_get_num;					// 跨服帮派战掉落羽翼获得数

	unsigned int cross_xiuluo_tower_gather_buff_end_timestamp;			// 跨服修罗塔采集buff结束时间戳

	
	short cross_multiuser_challenge_dur_win_count;						// 跨服3V3连胜次数
	char cross_3v3_season_reward_use_old[2];                            // 跨服3v3 正在使用的令牌(存储的是赛季，表示使用是哪个赛季的令牌) 这里废弃，在下面扩展了
	char cross_3v3_season_reward[CROSS_3V3_SEASON_MAX];                 // 跨服3v3 已经激活的令牌(存储的是段位，表示对应赛季激活是哪个段位的令牌)

	char cross_3v3_score_reuse_flag;									// 跨服3v3的积分字段重新启用标记，启用之前要清零，用这个标记
	char cross_3v3_join_reward_fetch_flag;								// 跨服3v3参与奖领取标记（每天清空）
	unsigned short cross_multiuser_challenge_month_match_count;			// 跨服3V3当前赛季参加比赛场数（每赛季清零）

	short cross_3v3_cur_join_season;                                    // 当前参加3v3第几赛季
	short cross_3v3_gongxun_reward_fetch_flag;							// 跨服3v3功勋奖领取标记
	int cross_3v3_gongxun_value;										// 跨服3v3功勋值

	short cross_liekun_scene_zone;										// 跨服猎鲲地带区域
	
	char cross_3v3_logout_flag;											// 跨服3v3掉线标记
	char cross_husong_shuijing_enter_type;								// 跨服护送水晶进入点类型

	unsigned int cross_boss_concern_flag[CROSS_BOSS_SCENE_MAX];			// 跨服boss关注标记	
	int cross_guildbattle_daily_reward_flag;							// 跨服帮派战占领每天奖励

	RoleCrossLogoutRecord role_cross_logout_record;						// 角色跨服掉线记录数据

	short cross_common_boss_enter_type;									// 跨服普通boss进入类型
	unsigned short cross_common_boss_enter_boss_id;						// 跨服普通boss进入bossid
	int cross_common_boss_enter_scene_id;								// 跨服普通boss进入场景id

	unsigned int cross_mizang_boss_concern_flag[CROSS_MIZANG_BOSS_SCENE_MAX];	// 跨服秘藏boss关注标记
	int cross_mizang_boss_today_kill_boss_num;							// 跨服秘藏boss今日击杀boss数量
	int cross_mizang_boss_today_gather_ordinary_crystal_num;			// 跨服boss采集普通水晶次数
	int cross_mizang_boss_today_gather_treasure_crystal_num;			// 跨服boss采集珍惜水晶次数
	short cross_mizang_boss_scene_level;                                // 跨服秘藏boss
	short cross_mizang_boss_relive_times;								// 跨服boss场景复活次数

	unsigned int cross_youming_boss_concern_flag[CROSS_YOUMING_BOSS_SCENE_MAX];	// 跨服秘藏boss关注标记
	int cross_youming_boss_today_kill_boss_num;							// 跨服秘藏boss今日击杀boss数量
	int cross_youming_boss_today_gather_ordinary_crystal_num;			// 跨服boss采集普通水晶次数
	int cross_youming_boss_today_gather_treasure_crystal_num;			// 跨服boss采集珍惜水晶次数
	short cross_youming_boss_scene_level;                                // 跨服秘藏boss
	short cross_youming_boss_relive_times;								// 跨服boss场景复活次数

	char cross_1v1_season_reward_use[CROSS_ACTIVITY_1V1_RING_USE_COUNT];	// 跨服1v1 使用戒指
	char cross_3v3_season_reward_use[CROSS_ACTIVITY_3V3_RING_USE_COUNT];	// 跨服3v3 正在使用的令牌(存储的是赛季，表示使用是哪个赛季的令牌)

	int cross_consume_gold_num;											// 跨服消费元宝
	int cross_daily_consume_gold_num;									// 跨服每日消费元宝
	long long cross_guildbattle_tuanzhang_uuid;							// 跨服六界团长uuid

	short enter_cross_move_to_pos_x;									// 进入跨服场景后直接移动到坐标x
	short enter_cross_move_to_pos_y;									// 进入跨服场景后直接移动到坐标y
	int cross_1v1_cur_join_season_dayid;                                // 当前参加1v1赛季的开启日期
	int cross_3v3_cur_join_season_dayid;                                // 当前参加3v3赛季的开启日期

};

typedef char RoleCrossParamHex[sizeof(RoleCrossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCrossParamHex) < 2048);

#pragma pack(pop)

#endif // __ROLE_CROSS_DEF_HPP__