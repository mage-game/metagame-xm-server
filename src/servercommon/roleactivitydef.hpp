#ifndef __ROLE_ACTIVITY_DEF_HPP__
#define __ROLE_ACTIVITY_DEF_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/taskdef.hpp"

#pragma pack(push, 4)

struct ZhanchangRoomInfo
{
	ZhanchangRoomInfo() { this->Reset(); }

	void Reset()
	{
		zhanchang_last_leavetime = 0;
		zhanchang_last_roomindex = -1;
		reserve_char = 0;
		reserve_sh = 0;
	}

	bool Valid() { return 0 != zhanchang_last_leavetime && -1 != zhanchang_last_roomindex; }

	void SetRoomInfo(unsigned int leave_time, int room_index) { zhanchang_last_leavetime = leave_time; zhanchang_last_roomindex = room_index; }

	unsigned int zhanchang_last_leavetime;								// 最后离开时间
	char zhanchang_last_roomindex;										// 最后进入房间		
	char reserve_char;
	short reserve_sh;
};

// 开服活动子活动数量
static const int OPEN_SERVER_ACTIVITY_COUNT = RAND_ACTIVITY_TYPE_OPEN_SERVER_END - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN + 1;
// 开服活动子活动信息
struct OpenServerSubActivityInfo
{
	OpenServerSubActivityInfo() : join_timestamp(0), reward_flag(0) {}

	unsigned int join_timestamp;
	int reward_flag;
};

struct RoleActivityParam
{
	RoleActivityParam() { this->Reset(); }

	void Reset()
	{
		battlefield_honor = 0;

		zhanchang_room_info.Reset();

		history_chongzhi = 0;

		day_chongzhi = 0;
		day_consume_coin = 0;

		history_chongzhi_count = 0;
		day_consume_gold = 0;

		hunyan_yanhua_buy_times = 0;
		hunyan_yanhua_use_times = 0;
		hunyan_hongbao_buy_times = 0;
		hunyan_hongbao_use_times = 0;
		hunyan_meigui_buy_times = 0;
		hunyan_meigui_use_times = 0;
		guild_monster_finish_seq = 0;

		zhuxie_reward_next_time = 0;

		oga_capability_reward_flag = 0;
		oga_rolelevel_reward_flag = 0;
		oga_buy_equipment_gift_flag = 0;
		oga_tuan_monthcard_reward_flag = 0;
		oga_tuan_vip_reward_flag = 0;
		oga_tuan_xunbao_reward_flag = 0;
		oga_puton_equipment_reward_flag = 0;

		chongzhi_reward_fetch_flag = 0;

		special_first_chongzhi_timestamp = 0;
		is_daily_first_chongzhi_open = 0;
		is_daily_first_chongzhi_fetch_reward = 0;
		daily_total_chongzhi_fetch_reward_flag = 0;
		daily_first_chongzhi_times = 0;
		special_first_chongzhi_fetch_reward_flag = 0;
		baibeifanli_flag = 0;
		total_chongzhi_want_money_state = 0;
		daily_total_chongzhi_stage_chongzhi = 0;
					
		daily_chongzhi_week_day_idx = -1;
		daily_total_chongzhi_stage = 0;

		cba_has_fetch_login_reward = 0;										
		cba_has_fetch_guild_reward = 0;										
		cba_has_fetch_marry_reward = 0;										
		cba_fetch_online_reward_flag = 0;									
		cba_fetch_uplevel_reward_flag = 0;									
		cba_join_activity_flag = 0;											
		cba_fetch_activity_reward_flag = 0;									
		cba_expfb_satisfy_reward_cond_flag = 0;								
		cba_expfb_fetch_reward_flag = 0;									
		cba_equipfb_satisfy_reward_cond_flag = 0;							
		cba_equipfb_fetch_reward_flag = 0;									
		cba_tdfb_satisfy_reward_cond_flag = 0;								
		cba_tdfb_fetch_reward_flag = 0;										
		cba_challengefb_satisfy_reward_cond_flag = 0;						
		cba_challengefb_fetch_reward_flag = 0;	

		lucky_roll_activity_begin_timestamp = 0;
		lucky_roll_activity_roll_times = 0;

		update_notice_fetch_reward_version = 0;					

		ra_server_panic_buy_begin_timestamp = 0;
		memset(ra_server_panic_buy_numlist, 0, sizeof(ra_server_panic_buy_numlist));

		ra_personal_panic_buy_begin_timestamp = 0;
		memset(ra_personal_panic_buy_numlist, 0, sizeof(ra_personal_panic_buy_numlist));

		ra_chongzhi_rank_begin_timestamp = 0;						
		ra_chongzhi_rank_chongzhi_num = 0;
		ra_chongzhi_rank_chongzhi_last_timestamp = 0;

		ra_consume_gold_rank_begin_timestamp = 0;					
		ra_consume_gold_rank_consume_gold_num = 0;
		ra_consume_gold_rank_consume_last_timestamp = 0;

		ra_consume_gold_fanli_begin_timestamp = 0;							
		ra_consume_gold_fanli_consume_gold = 0;		

		ra_stone_uplevel_begin_timestamp = 0;
		ra_stone_uplevel_can_reward_flag = 0;
		ra_stone_uplevel_fetch_reward_flag = 0;

		ra_day_chongzhi_fanli_begin_timestamp = 0;
		ra_day_chongzhi_fanli_chongzhi_num = 0;
		ra_day_chongzhi_fanli_fetch_reward_flag = 0;

		ra_day_consume_gold_begin_timestamp = 0;				
		ra_day_consume_gold_consume_num = 0;					   
		ra_day_consume_gold_fetch_reward_flag = 0;					

		ra_xiannv_chanmian_uplevel_begin_timestamp = 0;		
		ra_xiannv_chanmian_uplevel_can_reward_flag = 0;		
		ra_xiannv_chanmian_uplevel_fetch_reward_flag = 0;

		ra_mount_upgrade_begin_timestamp = 0;
		ra_mount_upgrade_can_reward_flag = 0;
		ra_mount_upgrade_fetch_reward_flag = 0;

		ra_qibing_uplevel_begin_timestamp = 0;
		ra_qibing_uplevel_can_reward_flag = 0;							
		ra_qibing_uplevel_fetch_reward_flag = 0;	

		ra_wing_upgrade_begin_timestamp = 0;
		ra_wing_upgrade_can_reward_flag = 0;
		ra_wing_upgrade_fetch_reward_flag = 0;

		ra_total_consume_gold_begin_timestamp = 0;				   
		ra_total_consume_gold_consume_num = 0;								
		ra_total_consume_gold_fetch_reward_flag = 0;

		ra_day_active_degree_begin_timestamp = 0;
		ra_day_active_degree_active_count = 0;
		ra_day_active_degree_fetch_reward_flag = 0;

		ra_chestshop_begin_timestamp = 0;
		ra_chestshop_fetch_reward_flag = 0;
		ra_chestshop_xunbao_times = 0;

		ra_quanmin_qifu_begin_timestamp = 0;
		ra_quanmin_qifu_fetch_reward_flag = 0;
		ra_quanmin_qifu_qifu_times = 0;

		ra_kill_boss_begin_timestamp = 0;
		ra_kill_boss_kill_count = 0;

		ra_shouyou_yuxiang_begin_timestamp = 0;					
		ra_shouyou_yuxiang_fetch_reward_flag = 0;							
		ra_shouyou_yuxiang_give_flowers_flag = 0;												

		abandon_ch = 0;
		ra_login_gift_begin_timestamp = 0;						
		ra_login_gift_login_days = 0;												
		ra_login_gift_last_login_dayid = 0;								
		ra_login_gift_has_fetch_accumulate_reward = 0;
		ra_login_gift_fetch_common_reward_flag = 0;					
		ra_login_gift_fetch_vip_reward_flag = 0;	

		yizhandaodi_room_idx = -1;
		yizhandaodi_dead_times = 0;
		yizhandaodi_leave_timestamp = 0;

		guildfb_first_enter_guild_id = 0;
		zai_chongzhi_reward_state = 0;
		ra_today_has_send_bipin_mail = 0;
		ra_total_charge_reward_has_fetch_flag = 0;

		ra_bipin_cap_total_begin_timestamp = 0;					
		ra_bipin_cap_total_fetch_reward_flag = 0;				
		ra_bipin_cap_total_history_max_value = 0;				

		ra_bipin_cap_equip_begin_timestamp = 0;					
		ra_bipin_cap_equip_fetch_reward_flag = 0;				
		ra_bipin_cap_equip_history_max_value = 0;				

		ra_bipin_cap_xiannv_begin_timestamp = 0;				
		ra_bipin_cap_xiannv_fetch_reward_flag = 0;				
		ra_bipin_cap_xiannv_history_max_value = 0;				

		ra_bipin_cap_jingmai_begin_timestamp = 0;				
		ra_bipin_cap_jingmai_fetch_reward_flag = 0;				
		ra_bipin_cap_jingmai_history_max_value = 0;				

		ra_bipin_cap_mount_begin_timestamp = 0;					
		ra_bipin_cap_mount_fetch_reward_flag = 0;				
		ra_bipin_cap_mount_history_max_value = 0;				

		ra_bipin_cap_wing_begin_timestamp = 0;						
		ra_bipin_cap_wing_fetch_reward_flag = 0;					
		ra_bipin_cap_wing_history_max_value = 0;	

		ra_charge_repayment_begin_timestamp = 0;
		ra_charge_repayment_reward_active_flag = 0;
		ra_charge_repayment_reward_fetch_flag = 0;
		ra_charge_repayment_history_charge = 0;

		ra_cornucopia_begin_timestamp = 0;
		ra_cornucopia_history_charge = 0;
		ra_cornucopia_current_lun = 0;
		third_chongzhi_reward_state = 0;
		current_lun_reward_gold = 0;

		memset(csa_qianggou_buynum_list, 0, sizeof(csa_qianggou_buynum_list));
		csa_roll_total_chongzhi = 0;
		csa_roll_chongzhi_num = 0;
		csa_chongzhi_rank_chongzhi_num = 0;
		csa_consume_rank_consume_gold = 0;
		csa_kill_boss_kill_count = 0;

		csa_login_gift_login_days = 0;										
		has_clear_chongzhi_reward_fetch_flag = 0;													
		csa_login_gift_has_fetch_accumulate_reward = 0;						
		csa_login_gift_fetch_common_reward_flag = 0;						
		csa_login_gift_fetch_vip_reward_flag = 0;							
		csa_login_gift_last_login_dayid = 0;		

		memset(csa_personal_panic_buy_numlist, 0, sizeof(csa_personal_panic_buy_numlist));

		ra_danbi_chongzhi_begin_timestamp = 0;
		ra_danbi_chongzhi_reward_has_fetch_flag = 0;
		ra_danbi_chongzhi_reward_can_fetch_flag = 0;

		ra_total_charge_day_begin_timestamp = 0;
		ra_total_charge_day_reward_has_fetch_flag = 0;
		ra_total_charge_day_count = 0;
		ra_total_charge_day_today_has_charged = 0;

		ra_tomorrow_reward_begin_timestamp = 0;
		ra_tomorrow_reward_count = 0;
		reserve_sh3 = 0;

		ra_day_consume_rank_begin_timestamp = 0;
		ra_day_consume_rank_gold_num = 0;
		ra_day_consume_rank_consume_last_timestamp = 0;
		ra_day_chongzhi_rank_begin_timestamp = 0;
		ra_day_chongzhi_rank_gold_num = 0;
		ra_day_chongzhi_rank_chongzhi_last_timestamp = 0;

		ra_total_charge_begin_timestamp = 0;
		ra_total_charge_value = 0;

		ra_double_score_begin_timestamp = 0;

		last_join_combine_server_activity_timestamp = 0;

		ra_time_limit_exchange_equi_begin_tiemstamp = 0;
		ra_time_limit_exchange_jl_begin_tiemstamp = 0;

		memset(ra_time_limit_exchange_exchange_equi_times_list, 0, sizeof(ra_time_limit_exchange_exchange_equi_times_list));
		memset(ra_time_limit_exchange_exchange_jl_times_list, 0, sizeof(ra_time_limit_exchange_exchange_jl_times_list));

		ra_level_lottery_timestamp = 0;
		ra_level_lottery_cur_level = 0;
		ra_level_lottery_free_buy_times = 0;
		ra_lottery_next_free_timestamp = 0;
		ra_lottery_buy_total_times = 0;
		ra_lottery_fetch_reward_flag = 0;

		ra_niu_egg_timestamp = 0;
		ra_niu_egg_cur_chongzhi_value = 0;
		ra_niu_egg_server_reward_has_fetch_reward_flag = 0;

		diff_wd_chongzhi_is_open = 0;
		diff_wd_chongzhi_value = 0;
		diff_wd_chongzhi_stage_fetch_flag = 0;

		ra_bipin_cap_shenzhuang_begin_timestamp = 0;
		ra_bipin_cap_shenzhuang_fetch_reward_flag = 0;
		ra_bipin_cap_shenzhuang_history_max_value = 0;

		ra_bipin_cap_jingling_begin_timestamp = 0;
		ra_bipin_cap_jingling_fetch_reward_flag = 0;
		ra_bipin_cap_jingling_history_max_value = 0;

		ra_bipin_cap_wash_begin_timestamp = 0;
		ra_bipin_cap_wash_fetch_reward_flag = 0;
		ra_bipin_cap_wash_history_max_value = 0;

		ra_zhenbaoge_timestamp = 0;
		ra_zhenbaoge_next_free_flush_timestamp = 0;
		memset(ra_zhenbaoge_add_weight_list, 0, sizeof(ra_zhenbaoge_add_weight_list));
		memset(ra_zhenbaoge_item_list, 0, sizeof(ra_zhenbaoge_item_list));
		ra_zhenbaoge_refresh_gift_fetch_flag = 0;
		ra_zhenbaoge_refresh_times = 0;
		ra_zhenbaoge_reserve_sh = 0;

		ra_charge_reward_timestamp = 0;
		ra_charge_reward_is_first = 1;

		ra_mijingxunbao_timestamp = 0;
		ra_mijingxunbao_next_free_tao_timestamp = 0;
		memset(ra_mijingxunbao_add_weight_list, 0, sizeof(ra_mijingxunbao_add_weight_list));

		ra_jisuchongzhan_timestamp = 0;

		ra_money_tree_last_join_timestamp = 0;
		ra_money_tree_server_reward_has_fetch_reward_flag = 0;
		ra_money_tree_total_times = 0;
		ra_money_tree_free_timestamp = 0;

		ra_daily_love_timestamp = 0;
		ra_daily_love_daily_first_flag = 1;

		ra_kingdraw_join_timestamp = 0;
		memset(ra_kingdraw_draw_times, 0, sizeof(ra_kingdraw_draw_times));
		ra_kingdraw_draw_reserve_sh = 0;
		ra_kingdraw_reward_flag = 0;
		for (int i = 0; i < RA_KING_DRAW_LEVEL_COUNT; ++ i)
		{	
			ra_kingdraw_card_list[i].Reset();
		}

		for (int i = 0; i < RA_KING_DRAW_LEVEL_COUNT; ++ i)
		{
			for (int j = 0; j < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++ j)
			{
				ra_kingdraw_self_weight_list[i][j].Reset();
			}
		}

		ra_total_charge_3_begin_timestamp = 0;
		ra_total_charge_3_cur_total_charge = 0;
		ra_total_charge_3_cur_has_fetch_flag = 0;

		ra_special_appearance_card_passive_use_times = 0;
		ra_special_appearance_passive_has_got_join_reward = 0;
		ra_special_appearance_card_passive_timestamp = 0;

		ra_special_appearance_card_use_times = 0;
		ra_special_appearance_has_got_join_reward = 0;
		ra_special_appearance_card_timestamp = 0;

		ra_any_monster_drop_last_timestamp = 0;
		ra_any_monster_drop_today_add_rate = 0;
		ra_any_monster_drop_total_add_rate = 0;

		ra_total_charge_4_begin_timestamp = 0;
		ra_total_charge_4_cur_total_charge = 0;
		ra_total_charge_4_cur_has_fetch_flag = 0;

		ra_mine_join_timestamp = 0;
		ra_mine_next_refresh_time = 0;
		ra_mine_cur_reward_fetch_flag = 0;
		ra_mine_free_gather_times = 0;
		ra_mine_role_refresh_times = 0;
		memset(ra_mine_type_list, 0, sizeof(ra_mine_type_list));
		memset(ra_mine_gather_count_list, 0, sizeof(ra_mine_gather_count_list));

		ra_guagua_join_timestamp = 0;
		ra_guagua_next_free_time = 0;

		ra_tianming_join_timestamp = 0;

		for (int i = 0; i < RA_TIANMING_LOT_COUNT; ++i)
		{
			ra_tianming_add_lot_times_list[i] = 1;
		}

		ra_tianming_reward_history_list_cur_index = 0;
		ra_tianming_free_chou_times = 0;

		for (int i = 0; i < RA_TIANMING_REWARD_HISTORY_COUNT; ++i)
		{
			ra_tianming_reward_history_list[i].Reset();
		}

		ra_fanfan_join_timestamp = 0;
		ra_fanfan_cur_free_times = 0;
		ra_fanfan_cur_letter_occur_rate = 0;
		ra_fanfan_role_refresh_times = 0;
		ra_fanfan_next_refresh_time = 0;

		for (int i = 0; i < RA_FANFAN_CARD_COUNT; ++ i)
		{
			ra_fanfan_card_type_list[i] = RA_FANFAN_CARD_TYPE_HIDDEN;
		}

		for (int i = 0; i < RA_FANFAN_MAX_WORD_COUNT; ++ i)
		{
			ra_fanfan_word_active_info_list[i].Reset();
		}

		ra_fanfan_hidden_word_info.Reset();

		ra_continue_chongzhi_join_timestamp = 0;
		ra_continue_chongzhi_today_chongzhi = 0;
		ra_continue_chongzhi_can_fetch_reward_flag = 0;
		ra_continue_chongzhi_has_fetch_reward_flag = 0;
		ra_continue_chongzhi_days = 0;
		ra_continue_chongzhi_is_activity_over = 0;
		ra_continue_chongzhi_reserve_sh = 0;

		ra_continue_consume_join_timestamp = 0;
		ra_continue_consume_today_consume_gold_total = 0;
		ra_continue_consume_cur_consume_gold = 0;
		ra_continue_consume_total_days = 0;
		ra_continue_consume_days = 0;
		ra_continue_consume_extra_reward_num = 0;
		ra_continue_consume_the_last_time_consume_day_index = 0;

		ra_army_day_join_timestamp = 0;
		ra_army_day_belong_army_side = INVALID_ARMY_SIDE;
		ra_army_day_active_degree_exchange_flag = 0;
		ra_army_day_reserver_sh = 0;
		memset(ra_army_day_flags_list, 0, sizeof(ra_army_day_flags_list));
		memset(ra_army_day_score_list, 0, sizeof(ra_army_day_score_list));

		ra_circulation_chongzhi_join_timestamp = 0;
		ra_circulation_chongzhi_total_chongzhi = 0;
		ra_circulation_chongzhi_cur_chongzhi_gold = 0;

		ra_extreme_lucky_join_timestamp = 0;
		ra_extreme_lucky_next_flush_timestamp = 0;
		ra_extreme_lucky_day_free_draw_times = 0;
		ra_extreme_lucky_cur_total_draw_times = 0;
		ra_extreme_lucky_flush_times = 0;
		ra_extreme_lucky_cur_gold_draw_times = 0;
		ra_extreme_lucky_total_draw_times = 0;
		ra_extreme_lucky_return_reward_flag = 0;
		memset(ra_extreme_lucky_item_list, 0, sizeof(ra_extreme_lucky_item_list));

		ra_mijingxunbao2_timestamp = 0;
		ra_mijingxunbao2_next_free_tao_timestamp = 0;
		memset(ra_mijingxunbao2_add_weight_list, 0, sizeof(ra_mijingxunbao2_add_weight_list));

		ra_money_tree2_last_join_timestamp = 0;
		ra_money_tree2_server_reward_has_fetch_reward_flag = 0;

		ra_national_day_last_join_timestamp = 0;

		ra_zhenbaoge2_timestamp = 0;
		ra_zhenbaoge2_next_free_flush_timestamp = 0;
		memset(ra_zhenbaoge2_add_weight_list, 0, sizeof(ra_zhenbaoge2_add_weight_list));
		memset(ra_zhenbaoge2_item_list, 0, sizeof(ra_zhenbaoge2_item_list));
		ra_zhenbaoge2_server_fetch_flag = 0;

		ra_promoting_position_join_timestamp = 0;
		ra_promoting_position_next_free_timestamp = 0;
		ra_promoting_position_extra_times = 0;
		ra_promoting_position_start_pos.Reset();
		ra_promoting_position_chongzhi_gold = 0;
		ra_promoting_position_play_times = 0;
		ra_promoting_position_reward_flag = 0;

		ra_treasures_mall_join_timestamp = 0;
		ra_treasures_mall_score = 0;

		memset(open_server_sub_info_list, 0, sizeof(open_server_sub_info_list));

		first_chongzhi_fetch_reward_flag = 0;
		daily_chongzhi_fetch_reward_flag = 0;
		daily_chongzhi_complete_days = 0;
		daily_chongzhi_times_fetch_reward_flag = 0;

		new_cornucopia_value = 0;
		new_cornucopia_day_index = -1;
		new_cornucopia_total_reward_flag = 0;
		memset(new_cornucopia_task_value_list, 0, sizeof(new_cornucopia_task_value_list));

		oga_kill_boss_flag = 0;
		oga_kill_boss_reward_flag = 0;
		oga_kill_boss_join_timestamp = 0;

		oga_gift_shop_flag = 0;

		shuijing_gather_count = 0;
		shuijing_free_relive_count = 0;
		shuijing_day_bind_gold = 0;
		shuijing_day_mojing = 0;
		shuijing_day_shengwang = 0;
		shuijing_gather_buff_time = 0;

		refine_join_timestamp = 0;
		refine_today_buy_time = 0;
		daily_activity_chongzhi = 0;
		refine_reward_gold = 0;

		ra_red_envelope_gift_begin_timestamp = 0;
		ra_red_envelope_gift_reserve_int = 0;
		ra_red_envelope_gift_reward_flag = 0;

		chongzhi_7day_reward_timestamp = 0;
		chongzhi_7day_reward_fetch_day = 0;
		chongzhi_7day_reward_is_fetch = 0;

		memset(collection_exchange_times, 0, sizeof(collection_exchange_times));

		daily_chongzhi_fetch_reward2_flag = 0;
		first_chongzhi_active_flag = 0;

		ra_gold_turntable_begin_timestamp = 0;
		gold_turntable_score = 0;

		ra_continue_chongzhi_join_timestamp_chu = 0;
		ra_continue_chongzhi_today_chongzhi_chu = 0;
		ra_continue_chongzhi_can_fetch_reward_flag_chu = 0;
		ra_continue_chongzhi_has_fetch_reward_flag_chu = 0;
		ra_continue_chongzhi_days_chu = 0;
		ra_is_lei_chongzhi_chu = 0;
		ra_continue_chongzhi_reserve_sh_chu = 0;

		ra_continue_chongzhi_join_timestamp_gao = 0;
		ra_continue_chongzhi_today_chongzhi_gao = 0;
		ra_continue_chongzhi_can_fetch_reward_flag_gao = 0;
		ra_continue_chongzhi_has_fetch_reward_flag_gao = 0;
		ra_continue_chongzhi_days_gao = 0;
		ra_is_lei_chongzhi_gao = 0;		
		ra_continue_chongzhi_reserve2 = 0;
		ra_golden_pig_chongzhi = 0;
		ra_golden_pig_summon_credit = 0;
		ra_golden_pig_timestamp = 0;

		ra_open_server_invest_timestamp = 0;
		ra_open_server_invest_reward_flag = 0;
		memset(ra_open_server_invest_reward_param, 0, sizeof(ra_open_server_invest_reward_param));
		ra_open_server_invest_is_add_active_param = 0;

		memset(xingzuoyiji_gather_box_num, 0, sizeof(xingzuoyiji_gather_box_num));

		memset(ra_red_envelope_gift_consume_gold_num_list, 0, sizeof(ra_red_envelope_gift_consume_gold_num_list));

		ra_single_chongzhi_join_timestamp = 0;
		ra_single_chongzhi_reward_flag = 0;
		ra_single_chongzhi_is_reward_flag = 0;

		memset(ra_xianyuan_treas_list, 0, sizeof(ra_xianyuan_treas_list));
		ra_xianyuan_treas_timestamp = 0;
		ra_xianyuan_treas_all_buy_gift_fetch_flag = 0;

		ra_rush_buying_join_timestamp = 0;
		ra_rush_buying_cur_pahse = 0;
		ra_rush_buying_reserved_ch = 0;
		ra_rush_buying_reserved_sh = 0;
		memset(ra_rush_buying_item_buying_times, 0, sizeof(ra_rush_buying_item_buying_times));

		ra_map_hunt_join_timestamp = 0;
		ra_map_hunt_cur_route_info.Reset();
		ra_map_hunt_has_used_free_xunbao_count = 0;
		ra_map_hunt_return_reward_fetch_flag = 0;
		ra_map_hunt_next_flush_timestamp = 0;
		ra_map_hunt_play_times = 0;

		ra_light_tower_join_timestamp = 0;
		memset(ra_light_tower_info, 0, sizeof(ra_light_tower_info));
		ra_light_tower_layer_extern_reward_fetch_flag = 0;
		ra_light_tower_return_reward_fetch_flag = 0;
		ra_light_tower_cur_index = 0;
		ra_light_tower_cur_layer = 0;
		ra_light_tower_reserve_sh = 0;

		ra_fanfan_reward_flag = 0;
		ra_fanfan_leichou_times = 0;
		ra_light_tower_draw_times = 0;
		ra_mijingxunbao_personal_reward_flag = 0;
		ra_mijingxunbao_role_chou_times = 0;
		ra_zhenbaoge2_flush_times = 0;
		ra_niu_egg_times = 0;

		ra_new_total_charge_begin_timestamp = 0;
		ra_new_total_charge_value = 0;
		ra_new_total_charge_reward_has_fetch_flag = 0;
		csa_boss_daily_kill_count = 0;

		ra_magic_shop_begin_timestamp = 0;
		ra_magic_shop_fetch_reward_flag = 0;
		ra_magic_shop_buy_flag = 0;
		ra_magic_shop_reserve_ch = 0;
		ra_chongzhi_gift_join_act_day = 0;
		ra_magic_shop_chongzhi_value = 0;

		ra_planting_tree_join_timestamp = 0;
		ra_planting_tree_planting_times = 0;
		ra_planting_tree_watering_times = 0;

		ra_fishing_join_timestamp = 0;
		ra_fishing_steal_timestamp = 0;
		ra_fishing_fishing_timestamp = 0;
		ra_fishing_times = 0;
		ra_steal_fish_times = 0;
		ra_be_steal_fish_times = 0;
		memset(ra_fish_info_list, 0, sizeof(ra_fish_info_list));
		ra_fishing_score = 0;
		ra_steal_fish_score = 0;

		ra_crystal_dalaran_join_timestamp = 0;
		ra_crystal_dalaran_count = 0;
		ra_steal_crystal_dalaran_times = 0;
		ra_crystal_dalaran_be_stealed_times = 0;
		ra_steal_crystal_dalaran_timestamp = 0;
		ra_crystal_dalaran_be_stealed_timestamp = 0;

		ra_steal_dalaran_count = 0;
		ra_dalaran_be_stealed_count = 0;
		ra_be_stealed_info->Reset();

		ra_chongzhi_crazy_rebate_join_timestamp = 0;
		ra_chongzhi_crazy_rebate_day_chongzhi_total_count = 0;
		ra_chongzhi_crazy_rebate_had_rebete_count = 0;

		fairy_buddha_card.Reset();

		memset(fish_gear_num_list, 0, sizeof(fish_gear_num_list));
		fish_score = 0;

		ra_perfect_lover_begin_timestamp = 0;
		ra_perfect_lover_type_record_flag = 0;
		ra_perfect_lover_type_fetch_flag = 0;
		ra_perfect_lover_last_marry_role_id = 0;

		ra_upgrade_reward_join_timestamp = 0;
		ra_upgrade_reward_flag = 0;
		ra_upgrade_group_reward_join_timestamp = 0;
		ra_upgrade_group_reward_flag = 0;
		ra_guild_battle_reward_type = -1;
		ra_guild_battle_reward_flag = 0;

		ra_consume_gift_join_timestamp = 0;               // 参与时间戳
		ra_consume_gift_reward_flag = 0;                  // 奖励领取标记
		ra_consume_gift_consume_gold = 0;				  // 消费元宝
		ra_consume_gift_today_join_times = 0;             // 抽奖次数
		ra_consume_gift_roll_seq = 0;                     // 摇奖摇得索引
		ra_consume_gift_roll_num = 0;
		memset(ra_consume_gift_roll_seq_list, 0, sizeof(ra_consume_gift_roll_seq_list));
		memset(ra_consume_gift_roll_num_list, 0, sizeof(ra_consume_gift_roll_num_list));

		ra_limit_buy_join_timestamp = 0;
		memset(ra_limit_buy_had_buy_count, 0, sizeof(ra_limit_buy_had_buy_count));

		ra_collect_treasure_can_join_times = 0;
		ra_collect_treasure_join_timestamp = 0;                               // 参与时间戳
		ra_collect_treasure_join_times = 0;
		ra_collect_treasure_roll_seq = -1;

		ra_happy_cumul_chongzhi_join_timestamp = 0;          // 参与时间戳
		ra_happy_cumul_chongzhi_fetch_flag = 0;              // 奖励领取标记
		ra_happy_cumul_chongzhi_num = 0;

		ra_jinjie_return_join_timestamp = 0;          // 参与时间戳
		ra_jinjie_return_fetch_flag = 0;

		ra_limit_time_rebate_join_timestamp = 0;
		ra_limit_time_rebate_cur_day_is_chongzhi = 0;
		ra_limit_time_rebate_total_chongzhi_days = 0;
		ra_consume_gift_join_theme = 0;                                    
		ra_happy_cumul_chongzhi_join_theme = 0;
		ra_limit_time_rebate_reward_flag = 0;
		ra_limit_time_rebate_cur_day_chongzhi = 0;
		memset(ra_limit_time_rebate_chongzhi_list, 0, sizeof(ra_limit_time_rebate_chongzhi_list));

		ra_time_limit_gift_begin_timestamp = 0;						// 活动开始时间
		ra_time_limit_gift_join_vip_level = 0;						// 限时礼包参与时vip等级
		ra_time_limit_gift_open_flag = 0;							// 限时礼包开启关闭标记
		ra_time_limit_gift_reward_can_fetch_flag1 = 0;				// 已激活档次1的充值信息
		ra_time_limit_gift_reward_has_fetch_flag1 = 0;

		ra_time_limit_gift_reward_can_fetch_flag2 = 0;				// 已激活档次2的充值信息
		ra_time_limit_gift_reward_has_fetch_flag2 = 0;				// 已领取档次2的充值信息

		ra_circulation_chongzhi_2_begin_timestamp = 0;				// 循环充值2活动开始时间
		ra_circulation_chongzhi_2_cur_chongzhi_gold = 0;			// 剩余充值数
		ra_circulation_chongzhi_2_total_chongzhi_gold = 0;

		ra_shakemoney_begin_timestamp = 0;							// 上次参与活动时间
		ra_shakemoney_history_chongzhi_num = 0;						// 疯狂摇钱树活每天的充值数
		ra_shakemoney_fetched_num = 0;

		ra_time_limit_luxury_gift_open_flag = 0;
		ra_time_limit_luxury_gift_bag_begin_timestamp = 0;			// 活动开始时间
		ra_time_limit_luxury_gift_bag_join_vip_level = 0;			// 限时礼包参与时vip等级
		ra_time_limit_luxury_gift_bag_is_already_buy = 0;

		ra_reset_double_chongzhi_join_timestamp = 0;				// 上次参加活动的时间
		ra_reset_double_chongzhi_reward_flag = 0;

		ra_rmb_buy_chest_shop_join_timestamp = 0;					// 参与时间戳
		memset(ra_rmb_buy_chest_shop_buy_count_list, 0, sizeof(ra_rmb_buy_chest_shop_buy_count_list));

		ra_consume_gold_reward_join_timestamp = 0;					// 参与活动时间
		ra_consume_gold_reward_consume_gold = 0;					// 消费金额
		ra_consume_gold_reward_fetch_flag = 0;						// 消费返礼领取奖励标志
		ra_consume_gold_reward_today_vip_level = 0;					// 消费返礼当日vip等级
		ra_consume_gold_reward_activity_day = 0;					// 消费返利达到目标的天数
		ra_reset_double_chongzhi_open_flag = 0;

		ra_consume_for_gift_join_timestamp = 0;
		ra_consume_for_gift_total_consume_gold = 0;
		ra_consume_for_gift_cur_points = 0;
		memset(ra_consume_for_gift_item_exchange_times, 0, sizeof(ra_consume_for_gift_item_exchange_times));

		ra_mijingxunbao3_timestamp = 0;
		ra_mijingxunbao3_next_free_tao_timestamp = 0;
		memset(ra_mijingxunbao3_add_weight_list, 0, sizeof(ra_mijingxunbao3_add_weight_list));
		ra_mijingxunbao3_daily_free_times = 0;
		ra_mijingxunbao3_personal_reward_flag = 0;
		ra_mijingxunbao3_role_chou_times = 0;

		ra_huanlezadan_timestamp = 0;
		ra_huanlezadan_next_free_tao_timestamp = 0;
		memset(ra_huanlezadan_add_weight_list, 0, sizeof(ra_huanlezadan_add_weight_list));
		ra_huanlezadan_daily_free_times = 0;
		ra_huanlezadan_personal_reward_flag = 0;
		ra_huanlezadan_role_chou_times = 0;

		ra_huanleyaojiang_timestamp = 0;
		ra_huanleyaojiang_next_free_tao_timestamp = 0;
		memset(ra_huanleyaojiang_add_weight_list, 0, sizeof(ra_huanleyaojiang_add_weight_list));
		ra_huanleyaojiang_daily_free_times = 0;
		ra_huanleyaojiang_personal_reward_flag = 0;
		ra_huanleyaojiang_role_chou_times = 0;

		collection_exchange_2_join_timestamp = 0;	                // 上次参与活动时间
		memset(collection_exchange_2_times, 0, sizeof(collection_exchange_2_times));

		ra_version_total_charge_begin_timestamp = 0;				// 活动开始时间
		ra_version_total_charge_value = 0;							// 累计充值额
		ra_version_total_charge_reward_has_fetch_flag = 0;

		ra_version_continue_chongzhi_join_timestamp = 0;			
		ra_version_continue_chongzhi_today_chongzhi = 0;		 	
		ra_version_continue_chongzhi_can_fetch_reward_flag = 0;		
		ra_version_continue_chongzhi_has_fetch_reward_flag = 0;		
		ra_version_continue_chongzhi_days = 0;					 	
		ra_version_is_lei_chongzhi = 0;						 	
		ra_version_continue_chongzhi_reserve_sh = 0;

		ra_huanleyaojiang_2_timestamp = 0;								
		ra_huanleyaojiang_2_next_free_tao_timestamp = 0;					
		memset(ra_huanleyaojiang_2_add_weight_list, 0, sizeof(ra_huanleyaojiang_2_add_weight_list));
		ra_huanleyaojiang_2_daily_free_times = 0;										
		ra_huanleyaojiang_2_personal_reward_flag = 0;									
		ra_huanleyaojiang_2_role_chou_times = 0;

		ra_offline_single_charge_join_timestamp_0 = 0;
		ra_offline_single_charge_max_value_0 = 0;											
		memset(ra_offline_single_charge_reward_times_list_0, 0, sizeof(ra_offline_single_charge_reward_times_list_0));

		memset(ra_kingdraw_reward_score, 0, sizeof(ra_kingdraw_reward_score));	//陛下请翻牌奖励总积分

		ra_buy_one_get_one_free_join_timestamp = 0;
		ra_buy_one_get_one_free_buy_flag = 0;
		ra_buy_one_get_one_free_fetch_reward_flag = 0;

		ra_guagua_acc_reward_has_fetch_flag = 0;
		ra_guagua_acc_count = 0;
		rev_sh_01 = 0;

		ra_login_gift_begin_timestamp_0 = 0;
		ra_login_gift_login_days_0 = 0;
		reserve_ch_1_login_gift_0 = 0;
		reserve_ch_2_login_gift_0 = 0;
		ra_login_gift_fetch_common_reward_flag_0 = 0;
		ra_login_gift_fetch_vip_reward_flag_0 = 0;
		ra_login_gift_last_login_dayid_0 = 0;

		ra_everyday_nice_gift_begin_timestamp = 0;
		ra_everyday_nice_gift_can_fetch_reward_flag = 0;
		ra_everyday_nice_gift_have_fetch_reward_flag = 0;
		ra_everyday_nice_gift_is_have_recharge_standard = 0;
		ra_everyday_nice_gift_reserve_sh = 0;
		ra_everyday_nice_gift_everydaty_recharge = 0;
		ra_everyday_nice_gift_recharge_timestamp = 0;

		ra_holiday_guard_kill_monster_count = 0;
		ra_holiday_guard_participate_timestamp = 0;

		ra_total_charge_5_begin_timestamp = 0;
		ra_total_charge_5_cur_total_charge = 0;
		ra_total_charge_5_cur_has_fetch_flag = 0;
		ra_total_charge_5_join_day_index = 0;

		ra_extreme_challenge_last_update_timestamp = 0;				// 增加活动进度的间隔
		ra_extreme_challenge_timestamp = 0;							// 上次参与活动时间
		ra_extreme_challenge_ultimate_have_fetch = 0;
		rev_ch_01 = 0;

		for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++i)
		{
			ra_extreme_challenge_task_info_list[i].Reset();
		}

		ra_chongzhi_gift_begin_timestamp = 0;

		csa_touzjihua_login_day = 0;
		csa_touzijihua_total_fetch_flag = 0;
		csa_touzijihua_buy_flag = 0;
		csa_refine_exp_had_buy = 0;

		memset(csa_foundation_status, 0, sizeof(csa_foundation_status));

		csa_gongchengzhan_win_times = 0;
		csa_had_add_gongchengzhan_win_times = 0;
		res_ch_1 = 0;

		ra_profess_rank_begin_timestamp = 0;
		ra_profess_score = 0;
		ra_profess_to_num = 0;
		ra_profess_from_num = 0;
		ra_profess_rank_last_profess_timestamp = 0;

		ra_jinjie_return2_join_timestamp = 0;
		ra_jinjie_return2_fetch_flag = 0;
		ra_consume_gift_join_opengame_day = 0;

		ra_combine_buy_join_timestamp = 0;								
		memset(ra_combine_buy_item_buy_times_list, 0, sizeof(ra_combine_buy_item_buy_times_list));
		ra_combine_buy_reserve_sh = 0;
		memset(ra_combine_buy_bucket_item_seq_list, -1, sizeof(ra_combine_buy_bucket_item_seq_list));

		had_add_chongzhi_day_count = 0;
		chongzhi_day_reserve_ch = 0;
		total_chongzhi_day_count = 0;				
		memset(chongzhi_day_reward_fetch_flag_list, 0, sizeof(chongzhi_day_reward_fetch_flag_list));
		memset(chongzhi_day_rare_reward_fetch_flag_list, 0, sizeof(chongzhi_day_rare_reward_fetch_flag_list));

		ra_lucky_wish_lucky_value = 0;
		ra_lucky_wish_fetch_reward_need_lucky_value = 0;
		ra_lucky_wish_join_timestamp = 0;
		ra_open_server_mail_send_flag = 0;

		ra_continue_chongzhi_days_2 = 0;
		ra_continue_chongzhi_2_is_lei_chongzhi = 0;									
		ra_continue_chongzhi_join_timestamp_2 = 0;
		ra_continue_chongzhi_today_chongzhi_2 = 0;
		ra_continue_chongzhi_can_fetch_reward_flag_2 = 0;
		ra_continue_chongzhi_has_fetch_reward_flag_2 = 0;

		ra_lottery1_join_timestamp = 0;									
		ra_lottery1_next_free_timestamp = 0;							
		ra_lottery1_fetch_person_reward_flag = 0;							
		ra_lottery_reserve_sh = 0;	
		ra_lottery1_person_chou_times = 0;

		ra_time_limit_gift_reward_can_fetch_flag3 = 0;				// 已激活档次3的充值信息
		ra_time_limit_gift_reward_has_fetch_flag3 = 0;				// 已领取档次3的充值信息

		ra_mentality_total_level_begin_timestamp = 0;				// 根骨全身等级活动开始时间
		ra_mentality_total_level_can_reward_flag = 0;				// 根骨全身等级能领取奖励标记
		ra_mentality_total_level_fetch_reward_flag = 0;				// 根骨全身等级能领取奖励标记

		reserve_sh = 0;

		memset(csa_server_panic_buy_numlist, 0, sizeof(csa_server_panic_buy_numlist));		// 合服全民疯抢购买数量

		ra_open_server_mail_send_reserve_ch = 0;
		memset(today_theme_reward_flag, 0, sizeof(today_theme_reward_flag));

		baibeifanli2_flag = 0;
		baibeifanli2_reserve_sh = 0;

		oga_gift_shop2_flag = 0;

		memset(ra_zero_buy_return_buy_timestamp, 0, sizeof(ra_zero_buy_return_buy_timestamp));
		memset(ra_zero_buy_return_day_fetch_flag, 0, sizeof(ra_zero_buy_return_day_fetch_flag));

		ra_daily_love_2_join_timestamp = 0;
		ra_daily_love_daily_2_has_charge = 0;

		ra_kuang_hai_qing_dian_begin_timestamp = 0;
		ra_kuang_hai_qing_dian_total_score = 0;
		ra_kuang_hai_qing_dian_fetch_reward_flag = 0;
		memset(ra_kuang_hai_qing_dian_score_list, 0, sizeof(ra_kuang_hai_qing_dian_score_list));
		memset(ra_kuang_hai_qing_dian_task_info_list, 0, sizeof(ra_kuang_hai_qing_dian_task_info_list));

		ra_gift_harvest_begin_timestamp = 0;
		ra_gift_harvest_score = 0;
		ra_gift_harvest_get_score_times = 0;
		ra_gift_harvest_reserve_sh = 0;

		last_chongzhi_day = 0;

		ra_marry_me_join_timestamp = 0;					
		ra_marry_me_is_fetch_reward = 0;
		ra_is_has_first_recharge_attr_add = 0;

		ra_lucky_cloud_buy_timestamp = 0;								
		ra_lucky_cloud_buy_count = 0;										
		ra_lucky_cloud_buy_price = 0;

		ra_cracy_buy_begin_timestamp = 0;
		ra_cracy_buy_chongzhi = 0;
		ra_cracy_buy_level = -1;
		memset(ra_cracy_buy_limit, 0, sizeof(ra_cracy_buy_limit));
		
		ra_new_total_charge1_begin_timestamp = 0;							
		ra_new_total_charge1_value = 0;												
		ra_new_total_charge1_reward_has_fetch_flag = 0;								
																					
		ra_new_total_charge2_begin_timestamp = 0;							
		ra_new_total_charge2_value = 0;												
		ra_new_total_charge2_reward_has_fetch_flag = 0;								
																					
		ra_new_total_charge3_begin_timestamp = 0;							
		ra_new_total_charge3_value = 0;												
		ra_new_total_charge3_reward_has_fetch_flag = 0;								

		ra_double_get_begin_timestamp = 0;
		ra_double_get_reward_fetch_flag = 0;

		ra_single_charge1_begin_timestamp = 0;
		ra_single_charge1_reward_fetch_flag = 0;

		ra_single_charge2_begin_timestamp = 0;
		ra_single_charge2_reward_fetch_flag = 0;

		ra_single_charge3_begin_timestamp = 0;
		ra_single_charge3_reward_fetch_flag = 0;

		ra_chongzhi_rank2_begin_timestamp = 0;
		ra_chongzhi_rank2_chongzhi_num = 0;
		ra_chongzhi_rank2_chongzhi_last_timestamp = 0;
	}

	int battlefield_honor;												// 战场荣誉
	ZhanchangRoomInfo zhanchang_room_info;								// 战场房间 信息

	long long history_chongzhi;											// 累计充值数
	int history_chongzhi_count;											// 累计充值次数
	int day_chongzhi;													// 每日充值数

	int day_consume_coin;												// 每日消耗铜币数
	int day_consume_gold;												// 每日消耗元宝		

	char hunyan_yanhua_buy_times;										// 婚宴烟花购买次数					
	char hunyan_yanhua_use_times;										// 婚宴烟花使用次数
	char hunyan_hongbao_buy_times;										// 婚宴红包购买次数
	char hunyan_hongbao_use_times;										// 婚宴红包使用次数
	char hunyan_meigui_buy_times;										// 婚宴玫瑰购买次数
	char hunyan_meigui_use_times;										// 婚宴玫瑰使用次数
	short guild_monster_finish_seq;										// 今天已经打过的仙盟神兽序号

	unsigned int zhuxie_reward_next_time;								// 在场景内下次获得经验的时间

	int oga_capability_reward_flag;										// 开服活动 战斗力冲刺奖励标记
	int oga_rolelevel_reward_flag;										// 开服活动 等级冲刺奖励标记
	int oga_buy_equipment_gift_flag;									// 开服活动 购买装备礼包标记
	int oga_tuan_monthcard_reward_flag;									// 开服活动 团购月卡奖励标记
	int oga_tuan_vip_reward_flag;										// 开服活动 团购Vip奖励标记
	int oga_tuan_xunbao_reward_flag;									// 开服活动 团购寻宝奖励标记

	//////////////////////////////////////////  充值奖励 ///////////////////////////////////////////////
	int chongzhi_reward_fetch_flag;										// 充值奖励领取标记
	unsigned int special_first_chongzhi_timestamp;						// 特殊首冲开始时间戳
	char is_daily_first_chongzhi_open;									// 每日首冲是否开启
	char is_daily_first_chongzhi_fetch_reward;							// 每日首冲奖励是否已经领取
	short daily_total_chongzhi_fetch_reward_flag;						// 累计充值奖励领取标记

	char daily_chongzhi_week_day_idx;									// 记录充值时候所处星期几（用于确定每日充值奖励）
	char daily_total_chongzhi_stage;									// 累计充值当前阶段

	char daily_first_chongzhi_times;									// 每日首冲累计次数(满7次给予额外奖励后置0)
	char special_first_chongzhi_fetch_reward_flag;						// 特殊首冲奖励领取标记
	char baibeifanli_flag;												// 百倍返利购买标记
	char total_chongzhi_want_money_state;								// 累积充值我要变元宝，当前要领的阶段
	int daily_total_chongzhi_stage_chongzhi;							// 当前阶段累计充值数

	//////////////////////////////////////////  封测活动 ///////////////////////////////////////////////
	char cba_has_fetch_login_reward;									// 封测活动是否已经领取当日登录奖励
	char cba_has_fetch_guild_reward;									// 封测活动是否已经领取仙盟奖励
	char cba_has_fetch_marry_reward;									// 封测活动是否已经领取了结婚奖励
	char cba_fetch_online_reward_flag;									// 封测活动是否已经领取了当日的在线奖励
	int cba_fetch_uplevel_reward_flag;									// 封测活动升级奖励领取标记
	int cba_join_activity_flag;											// 封测活动参与活动标记
	int cba_fetch_activity_reward_flag;									// 封测活动领取活动奖励标记
	int cba_expfb_satisfy_reward_cond_flag;								// 封测活动经验本满足奖励条件标记
	int cba_expfb_fetch_reward_flag;									// 封测活动经验本领取奖励标记
	int cba_equipfb_satisfy_reward_cond_flag;							// 封测活动装备本满足奖励条件标记
	int cba_equipfb_fetch_reward_flag;									// 封测活动装备本领取奖励标记
	int cba_tdfb_satisfy_reward_cond_flag;								// 封测活动塔防本满足奖励条件标记
	int cba_tdfb_fetch_reward_flag;										// 封测活动塔防本领取奖励标记
	int cba_challengefb_satisfy_reward_cond_flag;						// 封测活动挑战本满足奖励条件标记
	int cba_challengefb_fetch_reward_flag;								// 封测活动挑战本领取奖励标记

	//////////////////////////////////////////  幸运转盘活动 ///////////////////////////////////////////////
	unsigned int lucky_roll_activity_begin_timestamp;					// 幸运转盘活动开始时间
	int lucky_roll_activity_roll_times;									// 幸运转盘活动摇奖次数

	//////////////////////////////////////////  更新公告  ///////////////////////////////////////////////
	int update_notice_fetch_reward_version;

	//////////////////////////////////////////  随机活动 全民疯抢  ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(16 == RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT);

	unsigned int ra_server_panic_buy_begin_timestamp;					// 全民疯抢活动开始时间
	int ra_server_panic_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT]; // 全民疯抢购买数量

	//////////////////////////////////////////  随机活动 个人疯抢  ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(8 == RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT);

	unsigned int ra_personal_panic_buy_begin_timestamp;					// 个人疯抢活动开始时间
	int ra_personal_panic_buy_numlist[RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; // 个人疯抢购买数量

	//////////////////////////////////////////  随机活动 充值排行  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_rank_begin_timestamp;						// 随机活动充值参加时间
	int ra_chongzhi_rank_chongzhi_num;									// 随机活动充值数据
	unsigned int ra_chongzhi_rank_chongzhi_last_timestamp;				// 随机活动充值时间

	//////////////////////////////////////////  随机活动 消费排行  ///////////////////////////////////////////////
	unsigned int ra_consume_gold_rank_begin_timestamp;					// 随机活动消费开始时间
	int ra_consume_gold_rank_consume_gold_num;							// 随机活动消费数据
	unsigned int ra_consume_gold_rank_consume_last_timestamp;			// 随机活动消费时间

	/////////////////////////////////////////   随机活动 消费返利 ////////////////////////////////////////////////
	unsigned int ra_consume_gold_fanli_begin_timestamp;					// 参与活动开始时间
	int ra_consume_gold_fanli_consume_gold;								// 返利活动消耗元宝

	//////////////////////////////////////////  随机活动 宝石升级  ///////////////////////////////////////////////
	unsigned int ra_stone_uplevel_begin_timestamp;						// 宝石升级活动开始时间
	long long ra_stone_uplevel_can_reward_flag;							// 宝石升级能领取奖励标记
	long long ra_stone_uplevel_fetch_reward_flag;						// 宝石升级领取奖励标记

	////////////////////////////////////////// 随机活动 充值返利活动 ///////////////////////////////////////////////
	unsigned int ra_day_chongzhi_fanli_begin_timestamp;					// 充值返利活动开始时间
	int ra_day_chongzhi_fanli_chongzhi_num;								// 充值返利活动充值元宝
	int ra_day_chongzhi_fanli_fetch_reward_flag;						// 充值返利活动领取标记

	////////////////////////////////////////// 随机活动 每日消费活动 ///////////////////////////////////////////////
	unsigned int ra_day_consume_gold_begin_timestamp;				    // 每日消费活动开始时间
	int ra_day_consume_gold_consume_num;								// 每日消费活动活动消费元宝
	int ra_day_consume_gold_fetch_reward_flag;							// 每日消费活动活动领取标记

	//////////////////////////////////////////  随机活动 仙女缠绵  ///////////////////////////////////////////////
	unsigned int ra_xiannv_chanmian_uplevel_begin_timestamp;			// 仙女缠绵活动开始时间
	long long ra_xiannv_chanmian_uplevel_can_reward_flag;				// 仙女缠绵能领取奖励标记
	long long ra_xiannv_chanmian_uplevel_fetch_reward_flag;				// 仙女缠绵领取奖励标记

	//////////////////////////////////////////  随机活动 坐骑进阶  ///////////////////////////////////////////////
	unsigned int ra_mount_upgrade_begin_timestamp;						// 坐骑进阶活动开始时间
	short ra_mount_upgrade_can_reward_flag;								// 坐骑进阶能领取奖励标记
	short ra_mount_upgrade_fetch_reward_flag;							// 坐骑进阶能领取奖励标记

	//////////////////////////////////////////  随机活动 骑兵升级  ///////////////////////////////////////////////
	unsigned int ra_qibing_uplevel_begin_timestamp;						// 骑兵升级活动开始时间
	short ra_qibing_uplevel_can_reward_flag;							// 骑兵升级能领取奖励标记
	short ra_qibing_uplevel_fetch_reward_flag;							// 骑兵升级能领取奖励标记

	//////////////////////////////////////////  随机活动 根骨全身等级  ///////////////////////////////////////////////
	unsigned int ra_mentality_total_level_begin_timestamp;				// 根骨全身等级活动开始时间
	short ra_mentality_total_level_can_reward_flag;						// 根骨全身等级能领取奖励标记
	short ra_mentality_total_level_fetch_reward_flag;					// 根骨全身等级能领取奖励标记

	//////////////////////////////////////////  随机活动 羽翼进阶  ///////////////////////////////////////////////
	unsigned int ra_wing_upgrade_begin_timestamp;						// 羽翼进阶活动开始时间
	long long ra_wing_upgrade_can_reward_flag;							// 羽翼进阶能领取奖励标记
	long long ra_wing_upgrade_fetch_reward_flag;						// 羽翼进阶能领取奖励标记

	int oga_puton_equipment_reward_flag;								// 开服活动 你取奖励标记

	////////////////////////////////////////// 随机活动 累计消费活动 ///////////////////////////////////////////////
	unsigned int ra_total_consume_gold_begin_timestamp;				    // 累计消费活动开始时间
	int ra_total_consume_gold_consume_num;								// 累计消费活动活动消费元宝
	int ra_total_consume_gold_fetch_reward_flag;						// 累计消费活动活动领取标记

	////////////////////////////////////////// 随机活动 活跃奖励活动 ///////////////////////////////////////////////
	unsigned int ra_day_active_degree_begin_timestamp;				    // 活跃奖励活动开始时间
	int ra_day_active_degree_active_count;								// 活跃奖励活动活跃度度
	int ra_day_active_degree_fetch_reward_flag;							// 活跃奖励活动活动领取标记

	//////////////////////////////////////////  随机活动 奇珍异宝  ///////////////////////////////////////////////
	unsigned int ra_chestshop_begin_timestamp;							// 奇珍异宝活动开始时间
	short ra_chestshop_fetch_reward_flag;								// 奇珍异宝能领取奖励标记
	short ra_chestshop_xunbao_times;									// 奇珍异宝寻宝次数

	//////////////////////////////////////////  随机活动 全民祈福	  ///////////////////////////////////////////////
	unsigned int ra_quanmin_qifu_begin_timestamp;						// 全民祈福活动开始时间
	short ra_quanmin_qifu_fetch_reward_flag;							// 全民祈福领取奖励标记
	short ra_quanmin_qifu_qifu_times;									// 全民祈福次数

	////////////////////////////////////////// 随机活动 击杀boss活动 ///////////////////////////////////////////////
	unsigned int ra_kill_boss_begin_timestamp;							// 击杀boss活动开始时间
	int ra_kill_boss_kill_count;										// 击杀boss活动击杀boss数

	////////////////////////////////////////// 随机活动 手有余香活动 ///////////////////////////////////////////////
	unsigned int ra_shouyou_yuxiang_begin_timestamp;					// 手有余香活动开始时间
	int ra_shouyou_yuxiang_fetch_reward_flag;							// 手有余香活动领取奖励标记
	int ra_shouyou_yuxiang_give_flowers_flag;							// 手有余香送花标记

	////////////////////////////////////////// 登录送礼 击杀boss活动 ///////////////////////////////////////////////
	unsigned int ra_login_gift_begin_timestamp;							// 登录送礼活动开始时间
	short ra_login_gift_login_days;										// 登录送礼 登录天数
	char abandon_ch;													// （废弃的char），之前逻辑有错误
	char ra_login_gift_has_fetch_accumulate_reward;						// 登录送礼 是否已经领取累计登录奖励
	int ra_login_gift_fetch_common_reward_flag;							// 登录送礼 领取普通奖励标记
	int ra_login_gift_fetch_vip_reward_flag;							// 登录送礼 领取vip奖励标记

	////////////////////////////////////////// 一战到底  //////////////////////////////////////////////////////
	short yizhandaodi_room_idx;											// 一战到底房间索引
	short yizhandaodi_dead_times;										// 一战到底死亡次数
	unsigned int yizhandaodi_leave_timestamp;							// 一战到底离开时间

	///////////////////////////////////////////  仙盟副本  //////////////////////////////////////////////////////
	int guildfb_first_enter_guild_id;									// 第一次进入仙盟副本的仙盟ID 防刷	

	char zai_chongzhi_reward_state;										// 再充值状态
	char ra_today_has_send_bipin_mail;									// 今天已经发送比拼邮件
	short reserve_sh;													//

	////////////////////////////////////////// 随机活动 比拼总战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_total_begin_timestamp;					// 比拼总战力开始时间
	int ra_bipin_cap_total_fetch_reward_flag;							// 比拼总战力领取奖励标记
	int ra_bipin_cap_total_history_max_value;							// 比拼总战力历史最大值

	////////////////////////////////////////// 随机活动 比拼装备战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_equip_begin_timestamp;					// 比拼装备战力开始时间
	int ra_bipin_cap_equip_fetch_reward_flag;							// 比拼装备战力领取奖励标记
	int ra_bipin_cap_equip_history_max_value;							// 比拼装备战力历史最大值

	////////////////////////////////////////// 随机活动 比拼仙女战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_xiannv_begin_timestamp;					// 比拼仙女战力开始时间
	int ra_bipin_cap_xiannv_fetch_reward_flag;							// 比拼仙女战力领取奖励标记
	int ra_bipin_cap_xiannv_history_max_value;							// 比拼仙女战力历史最大值

	////////////////////////////////////////// 随机活动 比拼经脉战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_jingmai_begin_timestamp;					// 比拼经脉战力开始时间
	int ra_bipin_cap_jingmai_fetch_reward_flag;							// 比拼经脉战力领取奖励标记
	int ra_bipin_cap_jingmai_history_max_value;							// 比拼经脉战力历史最大值

	////////////////////////////////////////// 随机活动 比拼坐骑战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_mount_begin_timestamp;					// 比拼坐骑战力开始时间
	int ra_bipin_cap_mount_fetch_reward_flag;							// 比拼坐骑战力领取奖励标记
	int ra_bipin_cap_mount_history_max_value;							// 比拼坐骑战力历史最大值

	////////////////////////////////////////// 随机活动 比拼羽翼战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_wing_begin_timestamp;						// 比拼羽翼战力开始时间
	int ra_bipin_cap_wing_fetch_reward_flag;							// 比拼羽翼战力领取奖励标记
	int ra_bipin_cap_wing_history_max_value;							// 比拼羽翼战力历史最大值

	//////////////////////////////////////////  充值回馈 ///////////////////////////////////////////////
	unsigned int ra_charge_repayment_begin_timestamp;					// 充值回馈活动开始时间
	int ra_charge_repayment_reward_active_flag;							// 充值回馈，奖励激活状态
	int ra_charge_repayment_reward_fetch_flag;							// 充值回馈，奖励领取状态
	int ra_charge_repayment_history_charge;								// 充值回馈活动期间的累计充值

	//////////////////////////////////////////  聚宝盆 ///////////////////////////////////////////////
	unsigned int ra_cornucopia_begin_timestamp;							// 上一次聚宝盆活动开始时间
	int ra_cornucopia_history_charge;									// 当前聚宝盆活动期间累计充值
	char ra_cornucopia_current_lun;										// 聚宝盆当前轮数

	char third_chongzhi_reward_state;									// 第三次数值奖励状态
	short current_lun_reward_gold;										// 当前轮次获取金额

	//////////////////////////////////////////  合服活动 ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(3 == COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE);

	int csa_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE]; // 抢购排行抢购数
	int csa_roll_total_chongzhi;											// 合服转盘充值总数
	int csa_roll_chongzhi_num;												// 合服转盘累计充值
	int csa_chongzhi_rank_chongzhi_num;										// 合服充值排行充值数
	int csa_consume_rank_consume_gold;										// 合服消费排行消费元宝
	int csa_kill_boss_kill_count;											// 合服击杀boss活动击杀boss数

	//////////////////////////////////////////  单笔充值活动 ///////////////////////////////////////////////
	unsigned int ra_danbi_chongzhi_begin_timestamp;							// 活动开始时间
	int ra_danbi_chongzhi_reward_can_fetch_flag;							// 已激活的充值信息
	int ra_danbi_chongzhi_reward_has_fetch_flag;							// 已领取的充值信息

	//////////////////////////////////////////  合服活动 ///////////////////////////////////////////////
	short csa_login_gift_login_days;										// 合服登录送礼 登录天数
	char has_clear_chongzhi_reward_fetch_flag;								// 是否已经清理过充值领取标记
	char csa_login_gift_has_fetch_accumulate_reward;						// 合服登录送礼 是否已经领取累计登录奖励
	int csa_login_gift_fetch_common_reward_flag;							// 合服登录送礼 领取普通奖励标记
	int csa_login_gift_fetch_vip_reward_flag;								// 合服登录送礼 领取vip奖励标记
	unsigned int csa_login_gift_last_login_dayid;							// 合服登录送礼 最后登录dayid

	UNSTD_STATIC_CHECK(8 == COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT);
	int csa_personal_panic_buy_numlist[COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; // 合服个人疯抢购买数量

	UNSTD_STATIC_CHECK(16 == COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT);
	int csa_server_panic_buy_numlist[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];	// 合服全民疯抢购买数量

	unsigned int ra_login_gift_last_login_dayid;								// 随机活动登录送礼 最后登录dayid

	//////////////////////////////////////////  累计充值（天数）活动 ///////////////////////////////////////////////
	unsigned int ra_total_charge_day_begin_timestamp;							// 活动开始时间
	short ra_total_charge_day_reward_has_fetch_flag;							// 已领取的奖励
	char ra_total_charge_day_count;												// 累计充值天数
	char ra_total_charge_day_today_has_charged;									// 今天是否已充值过（每日重置）

	//////////////////////////////////////////	  次日福利活动	 ///////////////////////////////////////////////
	unsigned int ra_tomorrow_reward_begin_timestamp;							// 活动开始时间
	short ra_tomorrow_reward_count;												// 可抽奖次数
	short reserve_sh3;															// 保留

	////////////////////////////////////////// 随机活动 每日消费排行 ///////////////////////////////////////////////
	unsigned int ra_day_consume_rank_begin_timestamp;							// 随机活动每日消费排行开始时间
	int ra_day_consume_rank_gold_num;											// 随机活动每日消费排行消耗元宝
	unsigned int ra_day_consume_rank_consume_last_timestamp;					// 随机活动最后消费时间戳

	////////////////////////////////////////// 随机活动 每日充值排行 ///////////////////////////////////////////////
	unsigned int ra_day_chongzhi_rank_begin_timestamp;							// 随机活动每日充值排行开始时间
	int ra_day_chongzhi_rank_gold_num;											// 随机活动每日充值排行充值元宝
	unsigned int ra_day_chongzhi_rank_chongzhi_last_timestamp;					// 随机活动最后充值时间戳

	//////////////////////////////////////////  累计充值活动 ///////////////////////////////////////////////
	unsigned int ra_total_charge_begin_timestamp;								// 活动开始时间
	int ra_total_charge_value;													// 累计充值额
	int ra_total_charge_reward_has_fetch_flag;									// 累计充值活动-已领取的奖励标记

	/////////////////////////////////////////  双倍积分活动 ///////////////////////////////////////////////
	unsigned int ra_double_score_begin_timestamp;								// 双倍积分活动开始时间

	/////////////////////////////////////////  合服时间 ////////////////////////////////////////////
	unsigned int last_join_combine_server_activity_timestamp;					// 合服时间

	/////////////////////////////////////////  限时兑换稀有装备 ////////////////////////////////////////////
	unsigned int ra_time_limit_exchange_equi_begin_tiemstamp;					// 限时兑换活动开始时间
	unsigned int ra_time_limit_exchange_jl_begin_tiemstamp;						// 限时兑换活动开始时间
	int ra_time_limit_exchange_exchange_equi_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];	// 装备物品兑换次数--数组下标与seq()对应
	int ra_time_limit_exchange_exchange_jl_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		// 精灵兑换次数

	//////////////////////////////////////// 金银塔抽奖活动 ////////////////////////////////////////////////
	unsigned int ra_level_lottery_timestamp;									// 上次参与活动时间
	unsigned int ra_level_lottery_cur_level;									// 当前抽奖所在的层
	unsigned int ra_lottery_next_free_timestamp;								// 下次免费购买时间
	int ra_level_lottery_free_buy_times;										// 每日免费购买次数
	int ra_lottery_buy_total_times;												// 累计购买次数
	unsigned int ra_lottery_fetch_reward_flag;									// 领取奖励的标志

	//////////////////////////////////////////	 充值扭蛋活动	 ///////////////////////////////////////////////
	unsigned int ra_niu_egg_timestamp;											// 上次参与活动时间
	unsigned int ra_niu_egg_cur_chongzhi_value;									// 活动期间内剩余的可扭蛋的充值额度
	int ra_niu_egg_server_reward_has_fetch_reward_flag;							// 全服扭蛋次数奖励领取标记

	//////////////////////////////////////////	 每日累计充值	 ///////////////////////////////////////////////
	int diff_wd_chongzhi_is_open;												// 每日累冲活动开启标记
	int diff_wd_chongzhi_value;													// 每日累冲额
	int diff_wd_chongzhi_stage_fetch_flag;										// 每日累冲领取标记

	////////////////////////////////////////// 随机活动 比拼神装战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_shenzhuang_begin_timestamp;						// 比拼神装战力开始时间
	int ra_bipin_cap_shenzhuang_fetch_reward_flag;								// 比拼神装战力领取奖励标记
	int ra_bipin_cap_shenzhuang_history_max_value;								// 比拼神装战力历史最大值

	////////////////////////////////////////// 随机活动 比拼精灵战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_jingling_begin_timestamp;							// 比拼精灵战力开始时间
	int ra_bipin_cap_jingling_fetch_reward_flag;								// 比拼精灵战力领取奖励标记
	int ra_bipin_cap_jingling_history_max_value;								// 比拼精灵战力历史最大值

	////////////////////////////////////////// 随机活动 比拼精炼战力 ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_wash_begin_timestamp;								// 比拼精炼战力开始时间
	int ra_bipin_cap_wash_fetch_reward_flag;									// 比拼精炼战力领取奖励标记
	int ra_bipin_cap_wash_history_max_value;									// 比拼精炼战力历史最大值

	////////////////////////////////////////// 随机活动 珍宝阁 ///////////////////////////////////////////////
	unsigned int ra_zhenbaoge_timestamp;										// 珍宝阁
	unsigned int ra_zhenbaoge_next_free_flush_timestamp;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT == 5);
	ZhenbaogeAddWeightInfo ra_zhenbaoge_add_weight_list[RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT];	// 个人增加权重

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT == 9);
	short ra_zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];			// 可买奖品列表
	short ra_zhenbaoge_refresh_gift_fetch_flag;
	short ra_zhenbaoge_refresh_times;											// 个人手动刷新次数
	short ra_zhenbaoge_reserve_sh;

	////////////////////////////////////////// 随机活动 充值返元宝(2088) ///////////////////////////////////////////////
	unsigned int ra_charge_reward_timestamp;
	int ra_charge_reward_is_first;												// 首冲标志(每天重置)

	//////////////////////////////////////////  秘境寻宝活动 ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao_timestamp;										// 上次参与活动时间
	unsigned int ra_mijingxunbao_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_mijingxunbao_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重

	//////////////////////////////////////////  急速冲战 ///////////////////////////////////////////////////////
	unsigned int ra_jisuchongzhan_timestamp;									// 上次参与活动时间

	//////////////////////////////////////////	  摇钱树活动	 ///////////////////////////////////////////////
	unsigned int ra_money_tree_last_join_timestamp;								// 上次参与活动时间
	int ra_money_tree_server_reward_has_fetch_reward_flag;						// 全服摇钱树次数奖励领取标记
	int ra_money_tree_total_times;												// 抽奖次数
	unsigned int ra_money_tree_free_timestamp;									// 最后免费单抽的时间

	////////////////////////////////////////// 每天爱一次(首冲返元宝百分比) ///////////////////////////////////////////////
	unsigned int ra_daily_love_timestamp;										// 上次参与活动时间
	int ra_daily_love_daily_first_flag;											// 每日首冲

	//////////////////////////////////////////  随机活动-陛下请翻牌活动 /////////////////////////////////////////////////////
	unsigned int ra_kingdraw_join_timestamp;									// 玩家参与活动时间
	unsigned short ra_kingdraw_draw_times[RA_KING_DRAW_LEVEL_COUNT];			// 玩家抽奖次数
	short ra_kingdraw_draw_reserve_sh;
	int ra_kingdraw_reward_flag;												// 累抽奖励领取标记

	UNSTD_STATIC_CHECK(RA_KING_DRAW_LEVEL_COUNT == 3);
	RAKingDrawCardList ra_kingdraw_card_list[RA_KING_DRAW_LEVEL_COUNT];			// 每一个等级的翻牌数据

	UNSTD_STATIC_CHECK(RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT == 5);
	RAKingDrawSelfWeight ra_kingdraw_self_weight_list[RA_KING_DRAW_LEVEL_COUNT][RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT];

	////////////////////////////////////////// 随机活动-累计充值3活动 ///////////////////////////////////////////////
	unsigned int ra_total_charge_3_begin_timestamp;								// 活动开始时间
	int ra_total_charge_3_cur_total_charge;										// 当前阶段总充值
	int ra_total_charge_3_cur_has_fetch_flag;									// 当前阶段已领取标记

	//////////////////////////////////////////  被动特殊形象变身排行活动 ///////////////////////////////////////////////
	short ra_special_appearance_card_passive_use_times;							// 当前被变身次数
	short ra_special_appearance_passive_has_got_join_reward;					// 是否已领取参与奖励
	unsigned int ra_special_appearance_card_passive_timestamp;					// 上次参与活动时间

	//////////////////////////////////////////  特殊形象变身卡排行活动 ///////////////////////////////////////////////
	short ra_special_appearance_card_use_times;									// 当前变身次数
	short ra_special_appearance_has_got_join_reward;							// 是否已领取参与奖励
	unsigned int ra_special_appearance_card_timestamp;							// 上次参与活动时间

	//////////////////////////////////////////  全场景打怪掉落活动 ///////////////////////////////////////////////
	unsigned int ra_any_monster_drop_last_timestamp;							// 上一次打怪掉落物品的时间戳
	int ra_any_monster_drop_today_add_rate;										// 今日已使用活动物品增加属性倍数
	int ra_any_monster_drop_total_add_rate;										// 当前身上累计加的基础属性倍数(不能重置与战力有关)

	//////////////////////////////////////////  累计充值4活动 ///////////////////////////////////////////////
	unsigned int ra_total_charge_4_begin_timestamp;								// 活动开始时间
	int ra_total_charge_4_cur_total_charge;										// 当前阶段总充值
	int ra_total_charge_4_cur_has_fetch_flag;									// 当前阶段已领取标记

	//////////////////////////////////////////  随机活动-挖矿活动 /////////////////////////////////////////////////////////
	unsigned int ra_mine_join_timestamp;										// 玩家参与活动时间
	unsigned int ra_mine_next_refresh_time;										// 下一次的免费刷新时间
	int ra_mine_cur_reward_fetch_flag;											// 当前全服礼包领取标记
	short ra_mine_free_gather_times;											// 免费挖矿次数
	short ra_mine_role_refresh_times;											// 个人换矿次数											

	UNSTD_STATIC_CHECK(RA_MINE_MAX_REFRESH_COUNT == 8);
	char ra_mine_type_list[RA_MINE_MAX_REFRESH_COUNT];							// 当前矿场的矿石

	UNSTD_STATIC_CHECK(RA_MINE_MAX_TYPE_COUNT == 12);
	int ra_mine_gather_count_list[RA_MINE_MAX_TYPE_COUNT];						// 当前挖到的矿石数

	//////////////////////////////////////////  随机活动-顶刮刮活动 /////////////////////////////////////////////////////////
	unsigned int ra_guagua_join_timestamp;										// 玩家参与活动时间
	unsigned int ra_guagua_next_free_time;										// 下一次单刮免费的时间

	//////////////////////////////////////////  随机活动-天命卜卦活动 /////////////////////////////////////////////////////////
	unsigned int ra_tianming_join_timestamp;									// 玩家参与活动时间

	UNSTD_STATIC_CHECK(RA_TIANMING_LOT_COUNT == 6);
	char ra_tianming_add_lot_times_list[RA_TIANMING_LOT_COUNT];					// 竹签加注次数列表
	char ra_tianming_reward_history_list_cur_index;								// 奖励历史记录列表中最旧奖励记录的下标
	char ra_tianming_free_chou_times;											// 天命卜卦活动 - 今天免费抽奖次数

	UNSTD_STATIC_CHECK(RA_TIANMING_REWARD_HISTORY_COUNT == 20);
	RATianMingRewardHistoryItem ra_tianming_reward_history_list[RA_TIANMING_REWARD_HISTORY_COUNT];	// 奖励历史记录

	//////////////////////////////////////////  随机活动-翻翻转活动 /////////////////////////////////////////////////////////
	unsigned int ra_fanfan_join_timestamp;										// 玩家参与活动时间

	char ra_fanfan_cur_free_times;												// 当前免费次数
	char ra_fanfan_cur_letter_occur_rate;										// 当前字出现概率
	unsigned short ra_fanfan_role_refresh_times;								// 个人重置次数

	unsigned int ra_fanfan_next_refresh_time;									// 下一次重置时间

	UNSTD_STATIC_CHECK(40 == RA_FANFAN_CARD_COUNT);
	unsigned char ra_fanfan_card_type_list[RA_FANFAN_CARD_COUNT];				// 卡牌类型列表

	UNSTD_STATIC_CHECK(10 == RA_FANFAN_MAX_WORD_COUNT);
	RAFanFanWordActiveInfo ra_fanfan_word_active_info_list[RA_FANFAN_MAX_WORD_COUNT];	// 字组激活信息列表
	RAFanFanHiddenWordInfo ra_fanfan_hidden_word_info;							// 隐藏字信息

	/////////////////////////////////////////  随机活动-连续充值活动 /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp;							// 玩家参与活动时间
	int ra_continue_chongzhi_today_chongzhi;									// 今日充值数
	short ra_continue_chongzhi_can_fetch_reward_flag;							// 奖励激活标记 位0标记特殊奖励,其他位标记达标奖励, 值0未达标，值1达标
	short ra_continue_chongzhi_has_fetch_reward_flag;							// 奖励已领取标记 位0标记特殊奖励,其他位标记达标奖励, 值0未领取，值1已领取
	char ra_continue_chongzhi_days;												// 连续充值天数
	char ra_continue_chongzhi_is_activity_over;									// 是否连续充值中断	0未中断	1中断
	short ra_continue_chongzhi_reserve_sh;

	//////////////////////////////////////////  随机活动-连续消费活动 /////////////////////////////////////////////////////////
	unsigned int ra_continue_consume_join_timestamp;							// 玩家参与活动时间
	unsigned int ra_continue_consume_today_consume_gold_total;					// 今日总消费元宝
	unsigned int ra_continue_consume_cur_consume_gold;							// 上次领取奖励到目前累积的消费元宝		
	char ra_continue_consume_total_days;										// 消费达标总天数
	char ra_continue_consume_days;												// 当前连续消费达标天数
	char ra_continue_consume_extra_reward_num;									// 额外奖励个数
	char ra_continue_consume_the_last_time_consume_day_index;					// 参与活动的当天

	//////////////////////////////////////////  随机活动-军号嘹亮 /////////////////////////////////////////////////////////
	unsigned int ra_army_day_join_timestamp;									// 玩家参与活动时间
	char ra_army_day_belong_army_side;											// 所属军团
	char ra_army_day_active_degree_exchange_flag;								// 活跃度兑换旗帜标记(每天重置)
	short ra_army_day_reserver_sh;
	UNSTD_STATIC_CHECK(3 == RA_ARMY_DAY_ARMY_SIDE_NUM);
	int ra_army_day_flags_list[RA_ARMY_DAY_ARMY_SIDE_NUM];						// 玩家各军团旗帜数
	int ra_army_day_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];						// 玩家在各军团的积分

	//////////////////////////////////////////  随机活动-循环充值活动 /////////////////////////////////////////////////////////
	unsigned int ra_circulation_chongzhi_join_timestamp;						// 玩家参与活动时间
	unsigned int ra_circulation_chongzhi_total_chongzhi;						// 总充值元宝
	unsigned int ra_circulation_chongzhi_cur_chongzhi_gold;						// 上次领取奖励到目前累积的充值元宝

	//////////////////////////////////////////  随机活动-至尊幸运 /////////////////////////////////////////////////////////
	unsigned int ra_extreme_lucky_join_timestamp;								// 玩家参与活动时间
	unsigned int ra_extreme_lucky_next_flush_timestamp;							// 玩家下次刷新时间
	char ra_extreme_lucky_day_free_draw_times;									// 免费抽奖次数
	char ra_extreme_lucky_cur_total_draw_times;									// 当前轮抽奖次数
	short ra_extreme_lucky_flush_times;											// 刷新次数
	int ra_extreme_lucky_cur_gold_draw_times;									// 当前轮有效抽奖次数(元宝抽)
	short ra_extreme_lucky_total_draw_times;									// 抽奖总次数
	short ra_extreme_lucky_return_reward_flag;									// 返利奖励标志

	UNSTD_STATIC_CHECK(10 == RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT);
	RAExtremeLuckyRewardInfo ra_extreme_lucky_item_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];

	//////////////////////////////////////////  秘境寻宝2活动 ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao2_timestamp;									// 上次参与活动时间
	unsigned int ra_mijingxunbao2_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_mijingxunbao2_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重

	//////////////////////////////////////////	  摇钱树2活动	 ///////////////////////////////////////////////
	unsigned int ra_money_tree2_last_join_timestamp;							// 上次参与活动时间
	int ra_money_tree2_server_reward_has_fetch_reward_flag;						// 全服摇钱树次数奖励领取标记

	//////////////////////////////////////////	  国庆节活动	 ///////////////////////////////////////////////
	unsigned int ra_national_day_last_join_timestamp;							// 上次参与活动时间

	////////////////////////////////////////// 随机活动 珍宝阁2 ///////////////////////////////////////////////
	unsigned int ra_zhenbaoge2_timestamp;										// 珍宝阁
	unsigned int ra_zhenbaoge2_next_free_flush_timestamp;						// 下次刷新时间

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT == 5);
	ZhenbaogeAddWeightInfo ra_zhenbaoge2_add_weight_list[RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT];	// 个人增加权重

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT == 9);
	short ra_zhenbaoge2_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];			// 可买奖品列表
	short ra_zhenbaoge2_server_fetch_flag;

	//////////////////////////////////////////  随机活动-步步高升 /////////////////////////////////////////////////////////
	unsigned int ra_promoting_position_join_timestamp;
	unsigned int ra_promoting_position_next_free_timestamp;						// 下一次的免费抽时间戳
	unsigned short ra_promoting_position_extra_times;							// 额外抽奖次数（充值赠送）
	RAPromotingPositionInfo ra_promoting_position_start_pos;					// 抽奖起点位置
	UNSTD_STATIC_CHECK(2 == sizeof(RAPromotingPositionInfo));
	int ra_promoting_position_chongzhi_gold;									// 活动期间累计充值非绑元宝
	unsigned short ra_promoting_position_play_times;							// 抽奖次数
	unsigned short ra_promoting_position_reward_flag;							// 返利奖励标志

	//////////////////////////////////////////  随机活动-珍宝商城 /////////////////////////////////////////////////////////
	unsigned int ra_treasures_mall_join_timestamp;								// 上次参加活动时间
	int ra_treasures_mall_score;												// 积分

	//////////////////////////////////////////  随机活动-开服活动 /////////////////////////////////////////////////////////
	OpenServerSubActivityInfo open_server_sub_info_list[OPEN_SERVER_ACTIVITY_COUNT];

	//////////////////////////////////////////  每日充值奖励 ///////////////////////////////////////////////
	unsigned short first_chongzhi_fetch_reward_flag;							// 首充奖励领取标记
	short daily_chongzhi_fetch_reward_flag;										// 每日充值奖励领取标记
	short daily_chongzhi_complete_days;											// 每日充值完成天数
	short daily_chongzhi_times_fetch_reward_flag;								// 每日充值累计天数奖励标记

	//////////////////////////////////////////  新聚宝盆 ///////////////////////////////////////////////
	static const int RA_NEW_CORNUCOPIA_TASK_COUNT_MAX = 16;							// 新聚宝盆任务最大数量
	UNSTD_STATIC_CHECK(RA_NEW_CORNUCOPIA_TASK_TYPE_MAX < RA_NEW_CORNUCOPIA_TASK_COUNT_MAX);
	int new_cornucopia_value;														// 新聚宝盆聚宝值
	short new_cornucopia_day_index;													// 活动第几天
	short new_cornucopia_total_reward_flag;											// 新聚宝盆全服奖励领取标记
	unsigned char new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_COUNT_MAX];	// 新聚宝盆任务完成情况

	//////////////////////////////////////////  Boss猎手 ///////////////////////////////////////////////
	long long oga_kill_boss_flag;													// 开服活动 杀boss标记
	int oga_kill_boss_reward_flag;													// 开服活动 杀boss奖励标记
	unsigned int oga_kill_boss_join_timestamp;

	int oga_gift_shop_flag;															// 开服活动 礼包限购标记

	//////////////////////////////////////////  水晶幻境 ///////////////////////////////////////////////
	short shuijing_gather_count;													// 水晶幻境 采集次数
	short shuijing_free_relive_count;												// 水晶幻境 免费复活次数
	int shuijing_day_bind_gold;														// 水晶幻境 今日绑定元宝
	int shuijing_day_mojing;														// 水晶幻境 今日魔晶
	int shuijing_day_shengwang;														// 水晶幻境 今日声望
	unsigned int shuijing_gather_buff_time;											// 水晶幻境 采集不被打断buff时间

	//////////////////////////////////////////  经验炼制 ///////////////////////////////////////////////
	unsigned int refine_join_timestamp;
	short refine_today_buy_time;													// 每日炼制次数																
	unsigned short daily_activity_chongzhi;											// 每日充值活动充值金额
	int refine_reward_gold;															// 总奖励金额	

	//////////////////////////////////////////  随机活动 红包好礼 //////////////////////////////////////
	unsigned int ra_red_envelope_gift_begin_timestamp;									// 红包好礼 红包好礼开始时间
	int ra_red_envelope_gift_consume_gold_num;											// 红包好礼 消费钻石数
	int ra_red_envelope_gift_reserve_int;
	int ra_red_envelope_gift_reward_flag;												// 红包好礼 领取标记

	//////////////////////////////////////////  充值奖励 ///////////////////////////////////////////////
	unsigned int chongzhi_7day_reward_timestamp;										// 充值-七天返利达成时间
	short chongzhi_7day_reward_fetch_day;												// 充值-七天返利领取天数
	short chongzhi_7day_reward_is_fetch;												// 充值-七天返利今天是否领取

	//////////////////////////////////////////  集字活动 ///////////////////////////////////////////////
	int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT];		// 集字活动兑换次数

	//////////////////////////////////////////  每日充值奖励2 ///////////////////////////////////////////////
	short daily_chongzhi_fetch_reward2_flag;											// 每日充值奖励2领取标记
	unsigned short first_chongzhi_active_flag;											// 首冲奖励激活标志

	//////////////////////////////////////////  元宝转盘 ///////////////////////////////////////////////
	unsigned int ra_gold_turntable_begin_timestamp;										// 元宝转盘 开始时间
	int gold_turntable_score;															// 元宝转盘 积分

	/////////////////////////////////////////  连充特惠初-连续充值活动 /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp_chu;								// 玩家参与活动时间
	int ra_continue_chongzhi_today_chongzhi_chu;										// 今日充值数
	short ra_continue_chongzhi_can_fetch_reward_flag_chu;								// 奖励激活标记 位0标记特殊奖励,其他位标记达标奖励, 值0未达标，值1达标
	short ra_continue_chongzhi_has_fetch_reward_flag_chu;								// 奖励已领取标记 位0标记特殊奖励,其他位标记达标奖励, 值0未领取，值1已领取
	char ra_continue_chongzhi_days_chu;													// 连续充值天数
	char ra_is_lei_chongzhi_chu;														// 当天是否已经计入累充
	short ra_continue_chongzhi_reserve_sh_chu;

	/////////////////////////////////////////  连充特惠高-连续充值活动 /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp_gao;								// 玩家参与活动时间
	int ra_continue_chongzhi_today_chongzhi_gao;										// 今日充值数
	short ra_continue_chongzhi_can_fetch_reward_flag_gao;								// 奖励激活标记 
	short ra_continue_chongzhi_has_fetch_reward_flag_gao;								// 奖励已领取标记 
	char ra_continue_chongzhi_days_gao;													// 累计充值天数
	char ra_is_lei_chongzhi_gao;														// 当天是否已经计入累充						
	short ra_continue_chongzhi_reserve2;

	//////////////////////////////////////////  随机活动 红包好礼 //////////////////////////////////////
	int ra_red_envelope_gift_consume_gold_num_list[RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT];	
	
	//////////////////////////////////////////  金猪召唤	///////////////////////////////////////////////
	int ra_golden_pig_chongzhi;															// 活动期间充值数量
	int ra_golden_pig_summon_credit;													// 积分
	unsigned int ra_golden_pig_timestamp;												// 活动开启时间

	/////////////////////////////////////////   开服投资 //////////////////////////////////////////////
	unsigned int ra_open_server_invest_timestamp;										// 活动开启时间
	int ra_open_server_invest_reward_flag;												// 活动奖励标志
	UNSTD_STATIC_CHECK(3 == RA_OPEN_SERVER_INVEST_TYPE_MAX);
	char ra_open_server_invest_reward_param[RA_OPEN_SERVER_INVEST_TYPE_MAX];			// 领取奖励的条件参数
	char ra_open_server_invest_is_add_active_param;										// 今天的活跃度是否已经达标

	/////////////////////////////////////////   星座遗迹 //////////////////////////////////////////////
	UNSTD_STATIC_CHECK(4 == XINGZUOYIJI_BOX_TYPE_MAX);
	short xingzuoyiji_gather_box_num[XINGZUOYIJI_BOX_TYPE_MAX];							// 采集宝箱的数量 

	///////////////////////////////////////// 单返豪礼 ///////////////////////////////////////////////
	unsigned int ra_single_chongzhi_join_timestamp;										// 上次参与活动时间
	int ra_single_chongzhi_reward_flag;													// 领取标记
	int ra_single_chongzhi_is_reward_flag;												// 是否领取过

	////////////////////////////////////////// 聚划算 ///////////////////////////////////////////////////////
	XianyuanTreasInfo ra_xianyuan_treas_list[RAND_ACTIVITY_XIANYUAN_TREAS_COUNT];
	int ra_xianyuan_treas_all_buy_gift_fetch_flag;										// 全购买额外礼包领取标记
	unsigned int ra_xianyuan_treas_timestamp;											// 上次参与活动时间
	
	//////////////////////////////////////////  限时秒杀 ////////////////////////////////////////////////////
	unsigned int ra_rush_buying_join_timestamp;
	char ra_rush_buying_cur_pahse;														// 限时秒杀，玩家当前阶段
	char ra_rush_buying_reserved_ch;
	short ra_rush_buying_reserved_sh;
	char ra_rush_buying_item_buying_times[RA_RUSH_BUYING_SALES_ITEMS_COUNT];			// 玩家抢购次数
	UNSTD_STATIC_CHECK(0 == RA_RUSH_BUYING_SALES_ITEMS_COUNT % 4);

	//////////////////////////////////////////  随机活动-地图寻宝 //////////////////////////////////////////////////
	unsigned int ra_map_hunt_join_timestamp;										// 地图寻宝参与时间戳
	RAMapHuntRouteInfo ra_map_hunt_cur_route_info;									// 当前路线信息
	short ra_map_hunt_has_used_free_xunbao_count;									// 已使用的免费寻宝次数
	short ra_map_hunt_return_reward_fetch_flag;										// 全服奖励拿取标记
	unsigned int ra_map_hunt_next_flush_timestamp;									// 下次免费刷新时间戳
	int ra_map_hunt_play_times;																	// 刷新总次数

	//////////////////////////////////////////  随机活动-灯塔寻宝 /////////////////////////////////////////////////////////
	unsigned int ra_light_tower_join_timestamp;									//参与活动时间
	UNSTD_STATIC_CHECK(5 == RA_LIGHT_TOWER_EXPLORE_MAX_LAYER);
	char ra_light_tower_info[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];					// 所有格子的是否拿取过标记
	char ra_light_tower_layer_extern_reward_fetch_flag;							// 每层额外奖励拿取标记
	char ra_light_tower_return_reward_fetch_flag;								// 全服奖励拿取标记
	char ra_light_tower_cur_index;												// 当前层下标
	short ra_light_tower_cur_layer;												// 玩家所在当前层
	short ra_light_tower_reserve_sh;											

	//////////////////////////////////////////  一些累抽奖励		/////////////////////////////////////////////////////
	int ra_fanfan_reward_flag;													// 翻翻转累抽奖励领取标志
	int ra_fanfan_leichou_times;												// 翻翻转累抽次数

	int ra_light_tower_draw_times;												// 灯塔寻宝玩家抽取次数

	int ra_mijingxunbao_personal_reward_flag;									// 秘境寻宝累抽奖励领取标志
	int ra_mijingxunbao_role_chou_times;										// 秘境寻宝累抽次数

	int ra_zhenbaoge2_flush_times;												// 珍宝阁2抽奖次数

	int ra_niu_egg_times;														// 充值扭蛋次数

	//////////////////////////////////////////  新累计充值活动 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge_begin_timestamp;							// 活动开始时间
	int ra_new_total_charge_value;												// 新累计充值额
	int ra_new_total_charge_reward_has_fetch_flag;								// 新累计充值活动-已领取的奖励标记

	/////////////////////////////////////////   合服boss		//////////////////////////////////////////////
	int csa_boss_daily_kill_count;												// 每日杀合服boss数

	//////////////////////////////////////////   幻装商店活动  ///////////////////////////////////////////////
	unsigned int ra_magic_shop_begin_timestamp;									// 活动开始时间
	char ra_magic_shop_fetch_reward_flag;										// 你充我送 领取奖励标志   
	char ra_magic_shop_buy_flag;												// 购买标志
	char ra_magic_shop_reserve_ch;
	char ra_chongzhi_gift_join_act_day;                                         // 你充我送 记录参加第几天的活动，用于补发奖励
	unsigned int ra_magic_shop_chongzhi_value;									// 你充我送 充值金额

	//////////////////////////////////////////  随机活动-趣味植树 /////////////////////////////////////////////////////////
	unsigned int ra_planting_tree_join_timestamp;								// 玩家参与活动时间
	int ra_planting_tree_planting_times;										// 玩家植树次数
	int ra_planting_tree_watering_times;

	////////////////////////////////////////// 趣味钓鱼 /////////////////////////////////////////////////////////////////////
	unsigned int ra_fishing_join_timestamp;
	unsigned int ra_fishing_steal_timestamp;										// 钓鱼 时间戳
	unsigned int ra_fishing_fishing_timestamp;										// 偷鱼 时间戳
	short ra_fishing_times;
	short ra_steal_fish_times;
	short ra_be_steal_fish_times;
	short ra_fish_info_list[RA_FISHING_FISH_TYPE_COUNT];
	int ra_fishing_score;
	int ra_steal_fish_score;

	////////////////////////////////////////// 达拉然水晶 ////////////////////////////////////////////////////////////////////
	unsigned int ra_crystal_dalaran_join_timestamp;
	unsigned int ra_crystal_dalaran_count;
	short ra_steal_crystal_dalaran_times;
	short ra_crystal_dalaran_be_stealed_times;
	unsigned int ra_steal_crystal_dalaran_timestamp;
	unsigned int ra_crystal_dalaran_be_stealed_timestamp;

	unsigned int ra_steal_dalaran_count;
	unsigned int ra_dalaran_be_stealed_count;
	RandActivityStealNoteCfg ra_be_stealed_info[RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY];	

	///////////////////////////////////////// 狂返元宝 ///////////////////////////////////////////////////////////////////////
	unsigned int ra_chongzhi_crazy_rebate_join_timestamp;
	int ra_chongzhi_crazy_rebate_day_chongzhi_total_count;
	int ra_chongzhi_crazy_rebate_had_rebete_count;


	//////////////////////////////////////////  天尊卡  废弃 ///////////////////////////////////////////////
	FairyBuddhaCard fairy_buddha_card;

	//////////////////////////////////////////  钓鱼活动 /////////////////////////////////////////////////////////
	int fish_gear_num_list[FISHING_GEAR_MAX_COUNT];										// 拥有的法宝
	int fish_score;

	//////////////////////////////////////////  完美情人  ///////////////////////////////////////////////
	unsigned int ra_perfect_lover_begin_timestamp;			                    // 完美情人开始时间
	short ra_perfect_lover_type_record_flag;									// 结婚类型记录标记
	short ra_perfect_lover_type_fetch_flag;									    // 结婚奖励领取标记
	int ra_perfect_lover_last_marry_role_id;									// 上次求婚的角色id

	//////////////////////////////////////////  全民进阶（开服活动）  ///////////////////////////////////////////////
	unsigned int ra_upgrade_reward_join_timestamp;								
	int	ra_upgrade_reward_flag;													//全民进阶领取标记

	//////////////////////////////////////////  全民总动员（开服活动）///////////////////////////////////////////////
	unsigned int ra_upgrade_group_reward_join_timestamp;						
	int	ra_upgrade_group_reward_flag;											//全民总动员领取标记

	/////////////////////////////////////////	帮派争霸(开服活动)	///////////////////////////////////////
	short ra_guild_battle_reward_type;									// 帮派争霸奖励类型
	short ra_guild_battle_reward_flag;									// 帮派争霸奖励领取标记

	//////////////////////////////////////////  消费领奖 /////////////////////////////////////////////////////////
	unsigned int ra_consume_gift_join_timestamp;               // 参与时间戳
	unsigned int ra_consume_gift_reward_flag;                  // 奖励领取标记
	int ra_consume_gift_consume_gold;						   // 消费元宝
	short ra_consume_gift_today_join_times;                    // 抽奖次数
	char ra_consume_gift_roll_seq;                             // 摇奖摇得索引
	char ra_consume_gift_roll_num;                             // 摇奖摇得的数量
	char ra_consume_gift_roll_seq_list[RA_CONSUM_GIFT_MAX_ROLL_COUNT];		// 摇奖摇得索引列表
	char ra_consume_gift_roll_num_list[RA_CONSUM_GIFT_MAX_ROLL_COUNT];			// 摇奖摇得的数量列表

	//////////////////////////////////////////  每日限购 /////////////////////////////////////////////////////////
	unsigned int ra_limit_buy_join_timestamp;                               // 参与时间戳
	char ra_limit_buy_had_buy_count[RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ]; //购买次数

	//////////////////////////////////////////  聚宝盆 /////////////////////////////////////////////////////////
	char ra_collect_treasure_can_join_times;                  // 可参与次数
	char ra_collect_treasure_join_times;                      // 参与次数
	char ra_collect_treasure_roll_seq;                        // 摇奖索引
	unsigned int ra_collect_treasure_join_timestamp;          // 参与时间戳

	//////////////////////////////////////////  欢乐累充 /////////////////////////////////////////////////////////
	unsigned int ra_happy_cumul_chongzhi_join_timestamp;          // 参与时间戳
	unsigned int ra_happy_cumul_chongzhi_fetch_flag;              // 奖励领取标记
	int ra_happy_cumul_chongzhi_num;                              // 充值数量

	//////////////////////////////////////////  进阶返还 /////////////////////////////////////////////////////////
	unsigned int ra_jinjie_return_join_timestamp;          // 参与时间戳
	unsigned int ra_jinjie_return_fetch_flag;              // 奖励领取标记

	/////////////////////////////////////// 限时反馈 ////////////////////////////////////////////////////////////////////////
	unsigned int ra_limit_time_rebate_join_timestamp;
	int ra_limit_time_rebate_reward_flag;
	int ra_limit_time_rebate_cur_day_chongzhi;
	int ra_limit_time_rebate_chongzhi_list[RA_LIMIT_TIME_REBATE_MAX_DAY];
	char ra_limit_time_rebate_cur_day_is_chongzhi;
	char ra_limit_time_rebate_total_chongzhi_days;

	/////////////////////////////////////// 进阶活动上次参与主题 用于奖励补发//////////////////////////////////////////////////////////////
	char ra_consume_gift_join_theme;                                    // 消费领奖 上次参与主题
	char ra_happy_cumul_chongzhi_join_theme;                            // 欢乐累充 
	
	//////////////////////////////////////////  限时礼包 ///////////////////////////////////////////////
	unsigned int ra_time_limit_gift_begin_timestamp;						// 活动开始时间
	short ra_time_limit_gift_join_vip_level;								// 限时礼包参与时vip等级
	short ra_time_limit_gift_open_flag;										// 限时礼包开启关闭标记
	int ra_time_limit_gift_reward_can_fetch_flag1;							// 已激活档次1的充值信息
	int ra_time_limit_gift_reward_has_fetch_flag1;							// 已领取档次1的充值信息
	int ra_time_limit_gift_reward_can_fetch_flag2;							// 已激活档次2的充值信息
	int ra_time_limit_gift_reward_has_fetch_flag2;							// 已领取档次2的充值信息

	/////////////////////////////////////////   随机活动 循环充值2	///////////////////////////////////////////////
	unsigned int ra_circulation_chongzhi_2_begin_timestamp;				// 循环充值2活动开始时间
	int ra_circulation_chongzhi_2_cur_chongzhi_gold;					// 剩余充值数
	int ra_circulation_chongzhi_2_total_chongzhi_gold;					// 活动总充值数

	//////////////////////////////////////////随机活动  疯狂摇钱树 //////////////////////////////////////////////////////
	unsigned int ra_shakemoney_begin_timestamp;							// 上次参与活动时间
	int ra_shakemoney_history_chongzhi_num;								// 疯狂摇钱树活每天的充值数
	int ra_shakemoney_fetched_num;										// 疯狂摇钱树已领数

	//////////////////////////////////////// 限时豪礼 ////////////////////////////////////////////////
	int ra_time_limit_luxury_gift_open_flag;								// 限时豪礼开关标记
	unsigned int ra_time_limit_luxury_gift_bag_begin_timestamp;				// 活动开始时间
	short ra_time_limit_luxury_gift_bag_join_vip_level;						// 限时礼包参与时vip等级
	short ra_time_limit_luxury_gift_bag_is_already_buy;						// 是否已经购买礼包

	////////////////////////////////////////// 普天同庆(双倍充值) ///////////////////////////////////////////////
	unsigned int ra_reset_double_chongzhi_join_timestamp;				// 上次参加活动的时间
	unsigned int ra_reset_double_chongzhi_reward_flag;					// 奖励领取标志

	//////////////////////////////////////////  RMB购买礼包活动	///////////////////////////////////////////////
	unsigned int ra_rmb_buy_chest_shop_join_timestamp;								// 参与时间戳
	char ra_rmb_buy_chest_shop_buy_count_list[RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT];	// 购买列表
	UNSTD_STATIC_CHECK(RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT % 4 == 0);

	/////////////////////////////////////////	随机活动 消费返礼		///////////////////////////////////////////////
	unsigned int ra_consume_gold_reward_join_timestamp;					// 参与活动时间
	int ra_consume_gold_reward_consume_gold;							// 消费金额
	char ra_consume_gold_reward_fetch_flag;								// 消费返礼领取奖励标志
	char ra_consume_gold_reward_today_vip_level;						// 消费返礼当日vip等级
	char ra_consume_gold_reward_activity_day;							// 消费返利达到目标的天数

	////////////////////////////////////////// 普天同庆(双倍充值) ///////////////////////////////////////////////
	char ra_reset_double_chongzhi_open_flag;                            // 活动开启，客户端标记第一次特殊显示

	//////////////////////////////////////////  消费有礼 /////////////////////////////////////////////////////////
	unsigned int ra_consume_for_gift_join_timestamp;					// 参与时间
	int ra_consume_for_gift_total_consume_gold;							// 活动期间总消费非绑元宝
	int ra_consume_for_gift_cur_points;									// 可用积分
	unsigned char ra_consume_for_gift_item_exchange_times[RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT];	//交换次数
	UNSTD_STATIC_CHECK(0 == RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT % 4);

	//////////////////////////////////////////  秘境寻宝3活动 ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao3_timestamp;									// 上次参与活动时间
	unsigned int ra_mijingxunbao3_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_mijingxunbao3_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重
	int ra_mijingxunbao3_daily_free_times;										// 秘境寻宝3每日免费次数
	int ra_mijingxunbao3_personal_reward_flag;									// 秘境寻宝3累抽奖励领取标志
	int ra_mijingxunbao3_role_chou_times;										// 秘境寻宝3累抽次数

	//////////////////////////////////////////  欢乐砸蛋活动 ///////////////////////////////////////////////////////
	unsigned int ra_huanlezadan_timestamp;										// 上次参与活动时间
	unsigned int ra_huanlezadan_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_huanlezadan_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重
	int ra_huanlezadan_daily_free_times;										// 欢乐砸蛋每日免费次数
	int ra_huanlezadan_personal_reward_flag;									// 欢乐砸蛋累抽奖励领取标志
	int ra_huanlezadan_role_chou_times;											// 欢乐砸蛋累抽次数

	//////////////////////////////////////////  欢乐摇奖活动 ///////////////////////////////////////////////////////
	unsigned int ra_huanleyaojiang_timestamp;									// 上次参与活动时间
	unsigned int ra_huanleyaojiang_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_huanleyaojiang_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重
	int ra_huanleyaojiang_daily_free_times;										// 欢乐摇奖每日免费次数
	int ra_huanleyaojiang_personal_reward_flag;									// 欢乐摇奖累抽奖励领取标志
	int ra_huanleyaojiang_role_chou_times;										// 欢乐摇奖累抽次数

	//////////////////////////////////////////  集月饼活动 ///////////////////////////////////////////////
	unsigned int collection_exchange_2_join_timestamp;											// 上次参与活动时间
	int collection_exchange_2_times[RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT];		// 集字活动兑换次数

	///////////////////////////////////////// 版本累计充值 /////////////////////////////////////////////////////
	unsigned int ra_version_total_charge_begin_timestamp;							// 活动开始时间
	int ra_version_total_charge_value;												// 累计充值额
	int ra_version_total_charge_reward_has_fetch_flag;								// 累计充值活动-已领取的奖励标记

	/////////////////////////////////////////  版本连续充值  /////////////////////////////////////////////////////////
	unsigned int ra_version_continue_chongzhi_join_timestamp;						// 玩家参与活动时间
	int ra_version_continue_chongzhi_today_chongzhi;		 						// 今日充值数
	short ra_version_continue_chongzhi_can_fetch_reward_flag;						// 奖励激活标记 位0标记特殊奖励,其他位标记达标奖励, 值0未达标，值1达标
	short ra_version_continue_chongzhi_has_fetch_reward_flag;						// 奖励已领取标记 位0标记特殊奖励,其他位标记达标奖励, 值0未领取，值1已领取
	char ra_version_continue_chongzhi_days;					 						// 连续充值天数
	char ra_version_is_lei_chongzhi;						 						// 当天是否已经计入累充
	short ra_version_continue_chongzhi_reserve_sh;

	//////////////////////////////////////////  欢乐摇奖2活动 ///////////////////////////////////////////////////////
	unsigned int ra_huanleyaojiang_2_timestamp;										// 上次参与活动时间
	unsigned int ra_huanleyaojiang_2_next_free_tao_timestamp;						// 下一次免费淘宝时间
	MiJingXunBaoAddWeightInfo ra_huanleyaojiang_2_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// 个人增加权重
	int ra_huanleyaojiang_2_daily_free_times;										// 欢乐摇奖每日免费次数
	int ra_huanleyaojiang_2_personal_reward_flag;									// 欢乐摇奖累抽奖励领取标志
	int ra_huanleyaojiang_2_role_chou_times;

	/////////////////////////////////////////	线下单笔充值0		////////////////////////////////////////////////////
	unsigned int ra_offline_single_charge_join_timestamp_0;
	int ra_offline_single_charge_max_value_0;															//最大档位
	int ra_offline_single_charge_reward_times_list_0[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];		//奖励领取次数

	//////////////////////////////////////////  随机活动-陛下请翻牌活动(补充) /////////////////////////////////////////////////////
	int ra_kingdraw_reward_score[RA_KING_DRAW_LEVEL_COUNT];				//陛下请翻牌奖励总积分

	//////////////////////////////////////// 买一送一 //////////////////////////////////////////////////////////////////////
	unsigned int ra_buy_one_get_one_free_join_timestamp;				// 参与时间
	long long ra_buy_one_get_one_free_buy_flag;							// 购买标记
	long long ra_buy_one_get_one_free_fetch_reward_flag;				// 领取标记
	
	////////////////////////////////////////// 登录送礼 分渠道 ///////////////////////////////////////////////
	// 2217
	unsigned int ra_login_gift_begin_timestamp_0;							// 登录送礼活动开始时间
	short ra_login_gift_login_days_0;										// 登录送礼 登录天数
	char reserve_ch_1_login_gift_0;
	char reserve_ch_2_login_gift_0;
	int ra_login_gift_fetch_common_reward_flag_0;							// 登录送礼 领取普通奖励标记
	int ra_login_gift_fetch_vip_reward_flag_0;								// 登录送礼 领取vip奖励标记
	unsigned int ra_login_gift_last_login_dayid_0;							// 随机活动登录送礼 最后登录dayid

	//////////////////////////////////////////  随机活动 每日好礼  ///////////////////////////////////////////////
	unsigned int ra_everyday_nice_gift_begin_timestamp;						// 活动开始时间
	int ra_everyday_nice_gift_can_fetch_reward_flag;						// 领取奖励标记
	int ra_everyday_nice_gift_have_fetch_reward_flag;						// 已经领取标记
	short ra_everyday_nice_gift_is_have_recharge_standard;					// 是否充值达标
	short ra_everyday_nice_gift_reserve_sh;									// 领取奖励天数
	int ra_everyday_nice_gift_everydaty_recharge;							// 今日充值数量
	unsigned int ra_everyday_nice_gift_recharge_timestamp;					// 充值时间戳

	//////////////////////////////////////// 随机活动--顶呱呱 //////////////////////////////////////////////////////////////////
	unsigned short ra_guagua_acc_reward_has_fetch_flag;					// 累计奖励
	short ra_guagua_acc_count;									        // 累计次数
	short rev_sh_01;

	//////////////////////////////////////////  随机活动 节日守护  ///////////////////////////////////////////////
	int ra_holiday_guard_kill_monster_count;								// 节日守护杀怪数量
	unsigned int ra_holiday_guard_participate_timestamp;					// 节日守护参与时间

	//////////////////////////////////////////  累计充值5活动 ///////////////////////////////////////////////
	unsigned int ra_total_charge_5_begin_timestamp;									// 活动开始时间
	int ra_total_charge_5_cur_total_charge;											// 当前阶段总充值
	int ra_total_charge_5_cur_has_fetch_flag;										// 当前可以领取标志
	short ra_total_charge_5_join_day_index;											// 累计充值5参与活动距离活动开启第x天

	//////////////////////////////////////////  随机活动 极限挑战  ///////////////////////////////////////////////
	char ra_extreme_challenge_ultimate_have_fetch;							// 最终奖励已经领取
	char rev_ch_01;
	RAExremeChallengeTaskInfo ra_extreme_challenge_task_info_list[RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM];	// 任务
	unsigned int ra_extreme_challenge_last_update_timestamp;				// 增加活动进度的间隔
	unsigned int ra_extreme_challenge_timestamp;							// 上次参与活动时间

	//////////////////////////////////////////  随机活动 你充我送  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_gift_begin_timestamp;									// 活动开始时间

	//////////////////////////////////////////  新合服活动 ///////////////////////////////////////////////
	unsigned int csa_touzijihua_total_fetch_flag;						// 投资计划领取奖励标志
	unsigned short csa_touzjihua_login_day;								// 投资计划购买起登录天数
	char csa_touzijihua_buy_flag;										// 是否购买合服投资计划
	char csa_refine_exp_had_buy;										// 是否购买过经验

	CSAFoundationStatus csa_foundation_status[COMBINE_SERVER_MAX_FOUNDATION_TYPE];	//成长基金购买状态
	UNSTD_STATIC_CHECK(sizeof(CSAFoundationStatus) % 4 == 0);
	short csa_gongchengzhan_win_times;									// 攻城战胜利次数，这个值很特殊，下次合服要继承这个数据，不清掉
	char csa_had_add_gongchengzhan_win_times;							// 是否增加过攻城战次数,用这个值判断能不能加，这个值要清掉
	char res_ch_1;

	//////////////////////////////////////////  随机活动 表白排行榜  ///////////////////////////////////////////////
	unsigned int ra_profess_rank_begin_timestamp;						// 表白排行榜活动 开始时间
	int ra_profess_score;												// 表白排行榜活动 表白积分
	int ra_profess_to_num;												// 表白排行榜活动 表白次数
	int ra_profess_from_num;											// 表白排行榜活动 被表白次数
	unsigned int ra_profess_rank_last_profess_timestamp;				// 表白排行榜活动 最新表白时间

	//////////////////////////////////////////  进阶返还2 /////////////////////////////////////////////////////////
	unsigned int ra_jinjie_return2_join_timestamp;          // 参与时间戳
	unsigned int ra_jinjie_return2_fetch_flag;              // 奖励领取标记

	/////////////////////////////////////// 进阶活动上次参与主题 用于奖励补发//////////////////////////////////////////////////////////////
	int ra_consume_gift_join_opengame_day;                                    // 消费领奖 上次参与时的开服天数

	//////////////////////////////////////		组合购买		/////////////////////////////////////////
	unsigned int ra_combine_buy_join_timestamp;											// 参与时间
	unsigned char ra_combine_buy_item_buy_times_list[RA_COMBINE_BUY_MAX_ITEM_COUNT];	// 购买物品数量
	char ra_combine_buy_bucket_item_seq_list[RA_COMBINE_BUY_BUCKET_ITEM_COUNT];			// 购物车物品
	short ra_combine_buy_reserve_sh;

	////////////////////////////////////////	天天返利			/////////////////////////////////////
	char had_add_chongzhi_day_count;										// 能否增加充值天数
	char chongzhi_day_reserve_ch;
	short total_chongzhi_day_count;											// 累计充值达到要求天数
	unsigned int chongzhi_day_reward_fetch_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];		// 奖励领取标记
	unsigned int chongzhi_day_rare_reward_fetch_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];	// 珍稀奖励领取标记

	////////////////////////////////////////	幸运许愿			/////////////////////////////////////
	unsigned int ra_lucky_wish_lucky_value;										// 幸运值
	unsigned int ra_lucky_wish_fetch_reward_need_lucky_value;					// 领大奖所需幸运值
	unsigned int ra_lucky_wish_join_timestamp;									// 参与时间

	////////////////////////////////////////	开服活动邮件通知标记			/////////////////////////////////////
	char ra_open_server_mail_send_flag;									// 标记，0代表未发送，1代表已发送，每日重置
	char ra_open_server_mail_send_reserve_ch;
	/////////////////////////////////////////  限时连充-连续充值活动 /////////////////////////////////////////////////////////
	char ra_continue_chongzhi_days_2;												// 累计充值天数
	char ra_continue_chongzhi_2_is_lei_chongzhi;									// 当天是否已计入累充				
	unsigned int ra_continue_chongzhi_join_timestamp_2;								// 玩家参与活动时间
	int ra_continue_chongzhi_today_chongzhi_2;										// 今日充值数
	short ra_continue_chongzhi_can_fetch_reward_flag_2;								// 奖励激活标记 
	short ra_continue_chongzhi_has_fetch_reward_flag_2;								// 奖励已领取标记 

	/////////////////////////////////////////  随机活动 - 周末狂欢活动 /////////////////////////////////////////////////////////
	unsigned int ra_lottery1_join_timestamp;										// 玩家参与活动时间
	unsigned int ra_lottery1_next_free_timestamp;									// 玩家下一次免费时间
	unsigned short ra_lottery1_fetch_person_reward_flag;							// 领取个人奖励标记
	short ra_lottery_reserve_sh;													// 
	unsigned int ra_lottery1_person_chou_times;										// 个人累计抽奖次数

	//////////////////////////////////////////  限时礼包 ///////////////////////////////////////////////
	int ra_time_limit_gift_reward_can_fetch_flag3;									// 已激活档次3的充值信息
	int ra_time_limit_gift_reward_has_fetch_flag3;									// 已领取档次3的充值信息

	unsigned char today_theme_reward_flag[TODAY_THEME_REWARD_MAX_BYTE];				// 今日主题领取标记

	//////////////////////////////////////////  百倍返利2 ///////////////////////////////////////////////
	short baibeifanli2_flag;												// 百倍返利2购买标记
	short baibeifanli2_reserve_sh;

	//////////////////////////////////////////  礼包限购2 ///////////////////////////////////////////////
	int oga_gift_shop2_flag;															// 礼包限购2标记

	//////////////////////////////////////////  零元购（扩展后数据已脏，废弃） ///////////////////////////////////////////////
	unsigned int ra_zero_buy_return_buy_timestamp_dirty[3];							// 零元购购买时间戳 - 脏数据，废弃
	unsigned int ra_zero_buy_return_day_fetch_flag_dirty[3];						// 零元购每日领取元宝标记 - 脏数据，废弃
	
	////////////////////////////////////////// 每天爱一次2(首冲返元宝百分比) ///////////////////////////////////////////////
	unsigned int ra_daily_love_2_join_timestamp;						// 上次参与活动时间
	int ra_daily_love_daily_2_has_charge;								// 是否充值

	//////////////////////////////////////////  随机活动 狂嗨庆典  ///////////////////////////////////////////////
	unsigned int ra_kuang_hai_qing_dian_begin_timestamp;								// 狂嗨庆典 开始时间
	short ra_kuang_hai_qing_dian_total_score;											// 狂嗨庆典 当前总嗨点
	short ra_kuang_hai_qing_dian_fetch_reward_flag;										// 狂嗨庆典 领取奖励标记
	unsigned char ra_kuang_hai_qing_dian_score_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];	// 狂嗨庆典 每个task所获的点数（注意，并非与task_id对应，而是配置的index）
	int ra_kuang_hai_qing_dian_task_info_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];// 狂嗨庆典 每个task的每次完成进度（对应的完成后清0，用以记录下一次）

	//////////////////////////////////////////  随机活动 礼物收割  ///////////////////////////////////////////////
	unsigned int ra_gift_harvest_begin_timestamp;						// 表白排行榜活动 开始时间
	int ra_gift_harvest_score;											// 礼物收割活动 总积分
	short ra_gift_harvest_get_score_times;								// 礼物收割活动 总击杀数
	short ra_gift_harvest_reserve_sh;

	////////////////////////////////////////// 每日累充 /////////////////////////////////////////////////// /////
	int last_chongzhi_day;													// 上一次充值时间(每日累充补发奖励)

	////////////////////////////////////////// 我们结婚吧 ///////////////////////////////////////////////
	unsigned int ra_marry_me_join_timestamp;						// 上次参与活动时间
	short ra_marry_me_is_fetch_reward;								// 是否领取奖励
	short ra_is_has_first_recharge_attr_add;						// 是否拥有首充属性加成

	//////////////////////////////////////////  零元购 ///////////////////////////////////////////////
	unsigned int ra_zero_buy_return_buy_timestamp[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	// 零元购购买时间戳
	unsigned int ra_zero_buy_return_day_fetch_flag[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	// 零元购每日领取元宝标记

	//////////////////////////////////////////  随机活动 幸运云购  ///////////////////////////////////////////////
	unsigned int ra_lucky_cloud_buy_timestamp;								// 幸运云购参与时间
	short ra_lucky_cloud_buy_count;											// 本轮购买次数
	short ra_lucky_cloud_buy_price;											// 记录本轮购买的单价（因为每轮单价不一样，返还元宝需要确定单价）

	//////////////////////////////////////////  随机活动 狂欢大乐购  ///////////////////////////////////////////////
	unsigned int ra_cracy_buy_begin_timestamp;								// 狂欢抢购开始时间
	int ra_cracy_buy_chongzhi;												// 累计充值
	int ra_cracy_buy_level;													// 所在层数
	int ra_cracy_buy_limit[RA_MAX_CRACY_BUY_NUM_LIMIT];						// 物品当前购买量
	//////////////////////////////////////////  新累计充值活动1 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge1_begin_timestamp;							// 活动开始时间
	int ra_new_total_charge1_value;												// 新累计充值额
	int ra_new_total_charge1_reward_has_fetch_flag;								// 新累计充值活动-已领取的奖励标记
	//////////////////////////////////////////  新累计充值活动2 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge2_begin_timestamp;							// 活动开始时间
	int ra_new_total_charge2_value;												// 新累计充值额
	int ra_new_total_charge2_reward_has_fetch_flag;								// 新累计充值活动-已领取的奖励标记
	//////////////////////////////////////////  新累计充值活动3 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge3_begin_timestamp;							// 活动开始时间
	int ra_new_total_charge3_value;												// 新累计充值额
	int ra_new_total_charge3_reward_has_fetch_flag;								// 新累计充值活动-已领取的奖励标记

	////////////////////////////////////////  双倍元宝  ///////////////////////////////////////////////////////
	unsigned int ra_double_get_begin_timestamp;							// 活动开始时间
	int ra_double_get_reward_fetch_flag;								// 双倍元宝奖励获取奖励

	////////////////////////////////////////// 单笔充值1 //////////////////////////////////////////////////////
	unsigned int ra_single_charge1_begin_timestamp;
	int ra_single_charge1_reward_fetch_flag;

	////////////////////////////////////////// 单笔充值2 //////////////////////////////////////////////////////
	unsigned int ra_single_charge2_begin_timestamp;
	int ra_single_charge2_reward_fetch_flag;

	////////////////////////////////////////// 单笔充值3 //////////////////////////////////////////////////////
	unsigned int ra_single_charge3_begin_timestamp;
	int ra_single_charge3_reward_fetch_flag;

	//////////////////////////////////////////  随机活动 充值排行2  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_rank2_begin_timestamp;						// 随机活动充值参加时间
	int ra_chongzhi_rank2_chongzhi_num;									// 随机活动充值数据
	unsigned int ra_chongzhi_rank2_chongzhi_last_timestamp;				// 随机活动充值时间
}; 

typedef char RoleActivityParamHex[sizeof(RoleActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleActivityParamHex) < 10240);

#pragma pack(pop)

#endif // __ROLE_ACTIVITY_DEF_HPP__

