#ifndef __OTHER_MODULE_DATA_HPP__
#define __OTHER_MODULE_DATA_HPP__

#include "gameworld/gameworld/other/turntable/turntableconfig.h"
#include "gameworld/gameworld/other/upgraderush/upgraderushconfig.hpp"
#include "gameworld/gameworld/config/touzijihuaconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

struct OtherModuleDataParam								// 一些小模块的数据内容
{
	OtherModuleDataParam() 
	{
		this->Reset();
	}

	void Reset()
	{
		monthcard_active_timestamp = 0;
		monthcard_last_day = 0;
		monthcard_next_reward_day_idx = 0;
		monthcard_buy_times = 0;

		phase_fb_pass_flag = 0;

		memset(turntable_lucky_value, 0, sizeof(turntable_lucky_value));

		wabao_complete_times = 0;
		wabao_buy_join_times = 0;
		wabao_scene_id = 0;
		wabao_pos_x = 0;
		wabao_pos_y = 0;

		team_fb_open_times = 0;
		team_fb_assist_times = 0;

		last_reset_role_module_data_dayi = 0;
		last_reset_role_module_data_timestamp = 0;

		jilian_type = 0;
		jilian_is_working = 0;
		jilian_progress = 0;
		jilian_once_join = 0;

		reseve_short = 0;
		reserve_char = 0;
		jilian_qte_succ_times = 0;

		zhanshendian_pass_level = 0;

		touzijihua_active_plan_0 = 0;
		touzijihua_active_plan_1 = 0;
		touzijihua_plan_0_reward_flag = 0;				
		touzijihua_plan_1_reward_flag = 0;				

		roll_money_gold_fetch_reward_times = 0;			
		roll_money_coin_fetch_reward_times = 0;			

		roll_money_gold_num_0 = 0;						
		roll_money_gold_num_1 = 0;						
		roll_money_gold_num_2 = 0;						
		roll_money_gold_roll_times = 0;					

		roll_money_coin_num_0 = 0;						
		roll_money_coin_num_1 = 0;						
		roll_money_coin_num_2 = 0;						
		roll_money_coin_num_3 = 0;						

		roll_money_coin_num_4 = 0;							
		roll_money_coin_num_5 = 0;							
		roll_money_coin_num_6 = 0;							
		roll_money_coin_roll_times = 0;		
		roll_money_already_roll_gold_num = 0;

		zhanshendian_yesterday_pass_level = 0;
		touzhijihua_active_highest_plan = -1;
		reserve_sh = 0;
		m_zhanshendian_fetch_day_reward_timestamp = 0;

		world_event_fetch_reward_flag = 0;
		world_event_find_npc_flag = 0;
		world_event_curr_find_npc_id = 0;
		world_event_light_word_flag = 0;
		world_event_find_npc_score = 0;
		world_event_kill_rand_boss_score = 0;
		world_event_shenmozhixi_score = 0;
		world_event_xukongzhimen_score = 0;
		world_event_quanmintafang_score = 0;
		world_event_jiqingpk_score = 0;
		world_event_tiannvsanhua_score = 0;

		patafb_pass_level = 0;
		patafb_today_level = 0;
		week_add_charm = 0;

		new_touzijihua_buy_time = 0;
		new_touzijihua_reward_flag = 0;
		new_touzijihua_vip_reward_flag = 0;
		new_touzijihua_gold_bind_flag = 0;

		touzijihua_active_plan_2 = 0;
		new_touzijihua_first_reward_flag = 0;
		touzijihua_plan_2_reward_flag = 0;

		wabao_baotu_count = 0;
		wabao_reward_type = 0;
		baotu_fetch_flag = 0;
		shouhuzhe_time = 0;

		day_add_charm = 0;

		zhuanzhi_tianming = 0;						// 转职天命等级
		zhuanzhi_canglong = 0;						// 转职苍龙等级
		zhuanzhi_shengxiao = 0;					// 转职生肖等级
		zhuanzhi_yuanqi = 0;						// 转职元气等级
		zhuanzhi_shenqi = 0;						// 转职神器等级
		zhuanzhi_xingxiu = 0;                      // 转职星宿等级
		touzijihua_kill_boss_num = 0;

		touzijihua_plan_3_reward_flag = 0;
		touzijihua_active_plan_3 = 0;
		res_ch = 0;

		molong_curr_loop = 0;
		molong_accumulate_consume_gold = 0;
		molong_today_consume_gold = 0;
		molong_total_move_step = 0;	
		molong_today_move_step = 0;
		molong_rank_grade = 0;	
		molong_rank_cumulate_gold = 0;

		daily_task_can_fetch_flag = 0;
		daily_task_fetch_flag = 0;
		daily_task_exp_ball_valid_time = 0;
		daily_task_exp_ball_exp = 0;

		knapsack_grid_auto_add_count = 0;

		touzijihua_active_fb_plan = 0;
		touzijihua_fb_plan_reward_flag = 0;

		touzijihua_active_boss_plan = 0;
		touzijihua_boss_plan_reward_flag = 0;
		touzijihua_kill_boss_num = 0;

		touzijihua_kill_shenyun_boss_num = 0;						
		touzijihua_active_shenyun_boss_plan = 0;					
		touzijihua_shenyun_boss_plan_reward_flag = 0;				
		reserve_sh1 = 0;

		memset(upgrade_return_end_time_list, 0, sizeof(upgrade_return_end_time_list));	// 进阶结束返还时间列表
		memset(foundation_next_reward_index, 0, sizeof(foundation_next_reward_index));	// 成长基金领取标记
	}

	unsigned int monthcard_active_timestamp;			// 月卡激活时间
	int monthcard_last_day;								// 月卡持续时间
	int monthcard_next_reward_day_idx;					// 月卡下次奖励日
	int monthcard_buy_times;							// 月卡购买次数

	int phase_fb_pass_flag;								// 阶段副本通关标记(废弃)

	short turntable_lucky_value[TURNTABLE_TYPE_MAX_COUNT];// 转盘幸运值

	char wabao_complete_times;							// 挖宝完成次数
	char wabao_buy_join_times;							// 购买挖宝次数
	char jilian_once_join;								// 是否曾经参与祭炼
	char roll_money_already_roll_gold_num;				// 已经摇过元宝数字
	int wabao_scene_id;									// 挖宝场景id
	int wabao_pos_x;									// 挖宝坐标x
	int wabao_pos_y;									// 挖宝坐标y

	short team_fb_open_times;							// 组队副本开启次数
	short team_fb_assist_times;							// 组队副本协助次数

	unsigned int last_reset_role_module_data_dayi;		// 最后重置角色模块数据dayid
	unsigned int last_reset_role_module_data_timestamp;	// 最后一次重置角色模块数据的时间

	char jilian_type;									// 祭炼类型
	char jilian_is_working;								// 是否正在祭炼
	short jilian_progress;								// 祭炼进度

	short reseve_short;
	char reserve_char;
	char jilian_qte_succ_times;							// 祭炼QTE成功次数

	short zhanshendian_pass_level;						// 战神殿最高通关
	char touzijihua_active_plan_0;						// 投资计划激活计划0
	char touzijihua_active_plan_1;						// 投资计划激活计划1

	short touzijihua_plan_0_reward_flag;				// 投资计划0奖励标记
	short touzijihua_plan_1_reward_flag;				// 投资计划1奖励标记

	short roll_money_gold_fetch_reward_times;			// 摇钱领取元宝奖励次数
	short roll_money_coin_fetch_reward_times;			// 摇钱领取铜币奖励次数

	char roll_money_gold_num_0;							// 摇钱元宝数字0
	char roll_money_gold_num_1;							// 摇钱元宝数字1
	char roll_money_gold_num_2;							// 摇钱元宝数字2
	char roll_money_gold_roll_times;					// 摇钱roll元宝数字次数

	char roll_money_coin_num_0;							// 摇钱铜币数字0
	char roll_money_coin_num_1;							// 摇钱铜币数字1
	char roll_money_coin_num_2;							// 摇钱铜币数字2
	char roll_money_coin_num_3;							// 摇钱铜币数字3

	char roll_money_coin_num_4;							// 摇钱铜币数字4
	char roll_money_coin_num_5;							// 摇钱铜币数字5
	char roll_money_coin_num_6;							// 摇钱铜币数字6
	char roll_money_coin_roll_times;					// 摇钱roll铜币数字次数

	short zhanshendian_yesterday_pass_level;			// 昨日战神的最高通关
	char touzhijihua_active_highest_plan;
	char reserve_sh;									// 保留
	unsigned int m_zhanshendian_fetch_day_reward_timestamp;	// 战神的每日奖励领取时间戳

	int	world_event_fetch_reward_flag;					// 世界事件 领奖标记
	short world_event_find_npc_flag;					// 世界事件 寻找npc标记
	char world_event_curr_find_npc_id;					// 世界事件	当前寻找npc id
	char world_event_light_word_flag;					// 世界事件 点亮字标记
	int world_event_find_npc_score;						// 世界事件	寻找npc积分
	int world_event_kill_rand_boss_score;				// 世界事件 随机boss积分
	int world_event_shenmozhixi_score;					// 世界事件 神魔之隙积分
	int world_event_xukongzhimen_score;					// 世界事件 虚空之门积分
	int world_event_quanmintafang_score;				// 世界事件 全民塔防积分
	int world_event_jiqingpk_score;						// 世界事件 激情pk积分
	int world_event_tiannvsanhua_score;					// 世界事件 天女散花积分

	short patafb_pass_level;							// 爬塔通关层数
	short patafb_today_level;							// 爬塔今日通关层数
	int week_add_charm;									// 每周新增魅力

	unsigned int new_touzijihua_buy_time;				// 新投资计划购买时间
	int new_touzijihua_reward_flag;						// 新投资计划奖励标记
	int new_touzijihua_vip_reward_flag;					// 新投资计划vip奖励标记
	int new_touzijihua_gold_bind_flag;					// 新投资计划绑钻奖励标记

	char touzijihua_active_plan_2;						// 投资计划激活计划2
	char new_touzijihua_first_reward_flag;				// 新投资计划立返标志
	short touzijihua_plan_2_reward_flag;				// 投资计划2奖励标记

	int wabao_baotu_count;								// 宝图数量
	int wabao_reward_type;								// 挖宝奖励宝箱类型
	unsigned int baotu_fetch_flag;						// 宝图领取标志
	unsigned int shouhuzhe_time;						// 击杀守护者boss时间

	int day_add_charm;									// 每日新增魅力

	unsigned int upgrade_return_end_time_list[UPGRADE_RUSH_MAX_TYPE_COUNT]; // 进阶结束返还时间列表

	char zhuanzhi_tianming;						// 转职天命等级
	char zhuanzhi_canglong;						// 转职苍龙等级
	char zhuanzhi_shengxiao;					// 转职生肖等级
	char zhuanzhi_yuanqi;						// 转职元气等级
	char zhuanzhi_shenqi;						// 转职神器等级
	char zhuanzhi_xingxiu;                      // 转职星宿等级
	short touzijihua_kill_boss_num;				//投资计划击杀的boss数
	
	short touzijihua_plan_3_reward_flag;				// 投资计划3奖励标记
	char touzijihua_active_plan_3;						// 投资计划激活计划3
	char res_ch;

	int molong_curr_loop;								// 魔龙当前轮
	int molong_accumulate_consume_gold;					// 魔龙当前累计消费
	int molong_today_consume_gold;						// 魔龙今日消费
	short molong_total_move_step;						// 魔龙总共移动步数
	short molong_today_move_step;						// 魔龙今日移动步数
	int molong_rank_grade;								// 魔龙头衔等级
	int molong_rank_cumulate_gold;						// 魔龙头衔累计消费
	short daily_task_can_fetch_flag;					// 日常任务可领取标记
	short daily_task_fetch_flag;						// 日常任务已领取标记
	unsigned int daily_task_exp_ball_valid_time;		// 日常任务经验球失效时间
	long long daily_task_exp_ball_exp;					// 日常任务经验球经验

	char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];       // 成长基金领取标记

	int	knapsack_grid_auto_add_count;					// 背包格子自增数

	short touzijihua_active_fb_plan;					//投资计划副本激活
	short touzijihua_fb_plan_reward_flag;				//投资计划副本领取

	short touzijihua_active_boss_plan;					//投资计划boss激活
	short touzijihua_boss_plan_reward_flag;				//投资计划boss领取

	short touzijihua_kill_shenyun_boss_num;						//投资计划击杀的神域boss数
	short touzijihua_active_shenyun_boss_plan;					//投资计划神域boss激活
	short touzijihua_shenyun_boss_plan_reward_flag;				//投资计划神域boss领取
	short reserve_sh1;
				
};

#pragma pack(pop)

typedef char OtherModuleDataParamHex[sizeof(OtherModuleDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(OtherModuleDataParamHex) < 1024);

#endif  // __OTHER_MODULE_DATA_HPP__


