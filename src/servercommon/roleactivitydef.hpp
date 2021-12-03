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

	unsigned int zhanchang_last_leavetime;								// ����뿪ʱ��
	char zhanchang_last_roomindex;										// �����뷿��		
	char reserve_char;
	short reserve_sh;
};

// ������ӻ����
static const int OPEN_SERVER_ACTIVITY_COUNT = RAND_ACTIVITY_TYPE_OPEN_SERVER_END - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN + 1;
// ������ӻ��Ϣ
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

		ra_consume_gift_join_timestamp = 0;               // ����ʱ���
		ra_consume_gift_reward_flag = 0;                  // ������ȡ���
		ra_consume_gift_consume_gold = 0;				  // ����Ԫ��
		ra_consume_gift_today_join_times = 0;             // �齱����
		ra_consume_gift_roll_seq = 0;                     // ҡ��ҡ������
		ra_consume_gift_roll_num = 0;
		memset(ra_consume_gift_roll_seq_list, 0, sizeof(ra_consume_gift_roll_seq_list));
		memset(ra_consume_gift_roll_num_list, 0, sizeof(ra_consume_gift_roll_num_list));

		ra_limit_buy_join_timestamp = 0;
		memset(ra_limit_buy_had_buy_count, 0, sizeof(ra_limit_buy_had_buy_count));

		ra_collect_treasure_can_join_times = 0;
		ra_collect_treasure_join_timestamp = 0;                               // ����ʱ���
		ra_collect_treasure_join_times = 0;
		ra_collect_treasure_roll_seq = -1;

		ra_happy_cumul_chongzhi_join_timestamp = 0;          // ����ʱ���
		ra_happy_cumul_chongzhi_fetch_flag = 0;              // ������ȡ���
		ra_happy_cumul_chongzhi_num = 0;

		ra_jinjie_return_join_timestamp = 0;          // ����ʱ���
		ra_jinjie_return_fetch_flag = 0;

		ra_limit_time_rebate_join_timestamp = 0;
		ra_limit_time_rebate_cur_day_is_chongzhi = 0;
		ra_limit_time_rebate_total_chongzhi_days = 0;
		ra_consume_gift_join_theme = 0;                                    
		ra_happy_cumul_chongzhi_join_theme = 0;
		ra_limit_time_rebate_reward_flag = 0;
		ra_limit_time_rebate_cur_day_chongzhi = 0;
		memset(ra_limit_time_rebate_chongzhi_list, 0, sizeof(ra_limit_time_rebate_chongzhi_list));

		ra_time_limit_gift_begin_timestamp = 0;						// ���ʼʱ��
		ra_time_limit_gift_join_vip_level = 0;						// ��ʱ�������ʱvip�ȼ�
		ra_time_limit_gift_open_flag = 0;							// ��ʱ��������رձ��
		ra_time_limit_gift_reward_can_fetch_flag1 = 0;				// �Ѽ����1�ĳ�ֵ��Ϣ
		ra_time_limit_gift_reward_has_fetch_flag1 = 0;

		ra_time_limit_gift_reward_can_fetch_flag2 = 0;				// �Ѽ����2�ĳ�ֵ��Ϣ
		ra_time_limit_gift_reward_has_fetch_flag2 = 0;				// ����ȡ����2�ĳ�ֵ��Ϣ

		ra_circulation_chongzhi_2_begin_timestamp = 0;				// ѭ����ֵ2���ʼʱ��
		ra_circulation_chongzhi_2_cur_chongzhi_gold = 0;			// ʣ���ֵ��
		ra_circulation_chongzhi_2_total_chongzhi_gold = 0;

		ra_shakemoney_begin_timestamp = 0;							// �ϴβ���ʱ��
		ra_shakemoney_history_chongzhi_num = 0;						// ���ҡǮ����ÿ��ĳ�ֵ��
		ra_shakemoney_fetched_num = 0;

		ra_time_limit_luxury_gift_open_flag = 0;
		ra_time_limit_luxury_gift_bag_begin_timestamp = 0;			// ���ʼʱ��
		ra_time_limit_luxury_gift_bag_join_vip_level = 0;			// ��ʱ�������ʱvip�ȼ�
		ra_time_limit_luxury_gift_bag_is_already_buy = 0;

		ra_reset_double_chongzhi_join_timestamp = 0;				// �ϴβμӻ��ʱ��
		ra_reset_double_chongzhi_reward_flag = 0;

		ra_rmb_buy_chest_shop_join_timestamp = 0;					// ����ʱ���
		memset(ra_rmb_buy_chest_shop_buy_count_list, 0, sizeof(ra_rmb_buy_chest_shop_buy_count_list));

		ra_consume_gold_reward_join_timestamp = 0;					// ����ʱ��
		ra_consume_gold_reward_consume_gold = 0;					// ���ѽ��
		ra_consume_gold_reward_fetch_flag = 0;						// ���ѷ�����ȡ������־
		ra_consume_gold_reward_today_vip_level = 0;					// ���ѷ�����vip�ȼ�
		ra_consume_gold_reward_activity_day = 0;					// ���ѷ����ﵽĿ�������
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

		collection_exchange_2_join_timestamp = 0;	                // �ϴβ���ʱ��
		memset(collection_exchange_2_times, 0, sizeof(collection_exchange_2_times));

		ra_version_total_charge_begin_timestamp = 0;				// ���ʼʱ��
		ra_version_total_charge_value = 0;							// �ۼƳ�ֵ��
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

		memset(ra_kingdraw_reward_score, 0, sizeof(ra_kingdraw_reward_score));	//�����뷭�ƽ����ܻ���

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

		ra_extreme_challenge_last_update_timestamp = 0;				// ���ӻ���ȵļ��
		ra_extreme_challenge_timestamp = 0;							// �ϴβ���ʱ��
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

		ra_time_limit_gift_reward_can_fetch_flag3 = 0;				// �Ѽ����3�ĳ�ֵ��Ϣ
		ra_time_limit_gift_reward_has_fetch_flag3 = 0;				// ����ȡ����3�ĳ�ֵ��Ϣ

		ra_mentality_total_level_begin_timestamp = 0;				// ����ȫ��ȼ����ʼʱ��
		ra_mentality_total_level_can_reward_flag = 0;				// ����ȫ��ȼ�����ȡ�������
		ra_mentality_total_level_fetch_reward_flag = 0;				// ����ȫ��ȼ�����ȡ�������

		reserve_sh = 0;

		memset(csa_server_panic_buy_numlist, 0, sizeof(csa_server_panic_buy_numlist));		// �Ϸ�ȫ�������������

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

	int battlefield_honor;												// ս������
	ZhanchangRoomInfo zhanchang_room_info;								// ս������ ��Ϣ

	long long history_chongzhi;											// �ۼƳ�ֵ��
	int history_chongzhi_count;											// �ۼƳ�ֵ����
	int day_chongzhi;													// ÿ�ճ�ֵ��

	int day_consume_coin;												// ÿ������ͭ����
	int day_consume_gold;												// ÿ������Ԫ��		

	char hunyan_yanhua_buy_times;										// �����̻��������					
	char hunyan_yanhua_use_times;										// �����̻�ʹ�ô���
	char hunyan_hongbao_buy_times;										// �������������
	char hunyan_hongbao_use_times;										// ������ʹ�ô���
	char hunyan_meigui_buy_times;										// ����õ�幺�����
	char hunyan_meigui_use_times;										// ����õ��ʹ�ô���
	short guild_monster_finish_seq;										// �����Ѿ�����������������

	unsigned int zhuxie_reward_next_time;								// �ڳ������´λ�þ����ʱ��

	int oga_capability_reward_flag;										// ����� ս������̽������
	int oga_rolelevel_reward_flag;										// ����� �ȼ���̽������
	int oga_buy_equipment_gift_flag;									// ����� ����װ��������
	int oga_tuan_monthcard_reward_flag;									// ����� �Ź��¿��������
	int oga_tuan_vip_reward_flag;										// ����� �Ź�Vip�������
	int oga_tuan_xunbao_reward_flag;									// ����� �Ź�Ѱ���������

	//////////////////////////////////////////  ��ֵ���� ///////////////////////////////////////////////
	int chongzhi_reward_fetch_flag;										// ��ֵ������ȡ���
	unsigned int special_first_chongzhi_timestamp;						// �����׳忪ʼʱ���
	char is_daily_first_chongzhi_open;									// ÿ���׳��Ƿ���
	char is_daily_first_chongzhi_fetch_reward;							// ÿ���׳影���Ƿ��Ѿ���ȡ
	short daily_total_chongzhi_fetch_reward_flag;						// �ۼƳ�ֵ������ȡ���

	char daily_chongzhi_week_day_idx;									// ��¼��ֵʱ���������ڼ�������ȷ��ÿ�ճ�ֵ������
	char daily_total_chongzhi_stage;									// �ۼƳ�ֵ��ǰ�׶�

	char daily_first_chongzhi_times;									// ÿ���׳��ۼƴ���(��7�θ�����⽱������0)
	char special_first_chongzhi_fetch_reward_flag;						// �����׳影����ȡ���
	char baibeifanli_flag;												// �ٱ�����������
	char total_chongzhi_want_money_state;								// �ۻ���ֵ��Ҫ��Ԫ������ǰҪ��Ľ׶�
	int daily_total_chongzhi_stage_chongzhi;							// ��ǰ�׶��ۼƳ�ֵ��

	//////////////////////////////////////////  ��� ///////////////////////////////////////////////
	char cba_has_fetch_login_reward;									// ����Ƿ��Ѿ���ȡ���յ�¼����
	char cba_has_fetch_guild_reward;									// ����Ƿ��Ѿ���ȡ���˽���
	char cba_has_fetch_marry_reward;									// ����Ƿ��Ѿ���ȡ�˽�齱��
	char cba_fetch_online_reward_flag;									// ����Ƿ��Ѿ���ȡ�˵��յ����߽���
	int cba_fetch_uplevel_reward_flag;									// �������������ȡ���
	int cba_join_activity_flag;											// ����������
	int cba_fetch_activity_reward_flag;									// �����ȡ��������
	int cba_expfb_satisfy_reward_cond_flag;								// ������鱾���㽱���������
	int cba_expfb_fetch_reward_flag;									// ������鱾��ȡ�������
	int cba_equipfb_satisfy_reward_cond_flag;							// ���װ�������㽱���������
	int cba_equipfb_fetch_reward_flag;									// ���װ������ȡ�������
	int cba_tdfb_satisfy_reward_cond_flag;								// ������������㽱���������
	int cba_tdfb_fetch_reward_flag;										// �����������ȡ�������
	int cba_challengefb_satisfy_reward_cond_flag;						// �����ս�����㽱���������
	int cba_challengefb_fetch_reward_flag;								// �����ս����ȡ�������

	//////////////////////////////////////////  ����ת�̻ ///////////////////////////////////////////////
	unsigned int lucky_roll_activity_begin_timestamp;					// ����ת�̻��ʼʱ��
	int lucky_roll_activity_roll_times;									// ����ת�̻ҡ������

	//////////////////////////////////////////  ���¹���  ///////////////////////////////////////////////
	int update_notice_fetch_reward_version;

	//////////////////////////////////////////  ���� ȫ�����  ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(16 == RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT);

	unsigned int ra_server_panic_buy_begin_timestamp;					// ȫ��������ʼʱ��
	int ra_server_panic_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT]; // ȫ�������������

	//////////////////////////////////////////  ���� ���˷���  ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(8 == RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT);

	unsigned int ra_personal_panic_buy_begin_timestamp;					// ���˷������ʼʱ��
	int ra_personal_panic_buy_numlist[RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; // ���˷�����������

	//////////////////////////////////////////  ���� ��ֵ����  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_rank_begin_timestamp;						// ������ֵ�μ�ʱ��
	int ra_chongzhi_rank_chongzhi_num;									// ������ֵ����
	unsigned int ra_chongzhi_rank_chongzhi_last_timestamp;				// ������ֵʱ��

	//////////////////////////////////////////  ���� ��������  ///////////////////////////////////////////////
	unsigned int ra_consume_gold_rank_begin_timestamp;					// �������ѿ�ʼʱ��
	int ra_consume_gold_rank_consume_gold_num;							// ������������
	unsigned int ra_consume_gold_rank_consume_last_timestamp;			// ��������ʱ��

	/////////////////////////////////////////   ���� ���ѷ��� ////////////////////////////////////////////////
	unsigned int ra_consume_gold_fanli_begin_timestamp;					// ������ʼʱ��
	int ra_consume_gold_fanli_consume_gold;								// ���������Ԫ��

	//////////////////////////////////////////  ���� ��ʯ����  ///////////////////////////////////////////////
	unsigned int ra_stone_uplevel_begin_timestamp;						// ��ʯ�������ʼʱ��
	long long ra_stone_uplevel_can_reward_flag;							// ��ʯ��������ȡ�������
	long long ra_stone_uplevel_fetch_reward_flag;						// ��ʯ������ȡ�������

	////////////////////////////////////////// ���� ��ֵ����� ///////////////////////////////////////////////
	unsigned int ra_day_chongzhi_fanli_begin_timestamp;					// ��ֵ�������ʼʱ��
	int ra_day_chongzhi_fanli_chongzhi_num;								// ��ֵ�������ֵԪ��
	int ra_day_chongzhi_fanli_fetch_reward_flag;						// ��ֵ�������ȡ���

	////////////////////////////////////////// ���� ÿ�����ѻ ///////////////////////////////////////////////
	unsigned int ra_day_consume_gold_begin_timestamp;				    // ÿ�����ѻ��ʼʱ��
	int ra_day_consume_gold_consume_num;								// ÿ�����ѻ�����Ԫ��
	int ra_day_consume_gold_fetch_reward_flag;							// ÿ�����ѻ���ȡ���

	//////////////////////////////////////////  ���� ��Ů����  ///////////////////////////////////////////////
	unsigned int ra_xiannv_chanmian_uplevel_begin_timestamp;			// ��Ů������ʼʱ��
	long long ra_xiannv_chanmian_uplevel_can_reward_flag;				// ��Ů��������ȡ�������
	long long ra_xiannv_chanmian_uplevel_fetch_reward_flag;				// ��Ů������ȡ�������

	//////////////////////////////////////////  ���� �������  ///////////////////////////////////////////////
	unsigned int ra_mount_upgrade_begin_timestamp;						// ������׻��ʼʱ��
	short ra_mount_upgrade_can_reward_flag;								// �����������ȡ�������
	short ra_mount_upgrade_fetch_reward_flag;							// �����������ȡ�������

	//////////////////////////////////////////  ���� �������  ///////////////////////////////////////////////
	unsigned int ra_qibing_uplevel_begin_timestamp;						// ����������ʼʱ��
	short ra_qibing_uplevel_can_reward_flag;							// �����������ȡ�������
	short ra_qibing_uplevel_fetch_reward_flag;							// �����������ȡ�������

	//////////////////////////////////////////  ���� ����ȫ��ȼ�  ///////////////////////////////////////////////
	unsigned int ra_mentality_total_level_begin_timestamp;				// ����ȫ��ȼ����ʼʱ��
	short ra_mentality_total_level_can_reward_flag;						// ����ȫ��ȼ�����ȡ�������
	short ra_mentality_total_level_fetch_reward_flag;					// ����ȫ��ȼ�����ȡ�������

	//////////////////////////////////////////  ���� �������  ///////////////////////////////////////////////
	unsigned int ra_wing_upgrade_begin_timestamp;						// ������׻��ʼʱ��
	long long ra_wing_upgrade_can_reward_flag;							// �����������ȡ�������
	long long ra_wing_upgrade_fetch_reward_flag;						// �����������ȡ�������

	int oga_puton_equipment_reward_flag;								// ����� ��ȡ�������

	////////////////////////////////////////// ���� �ۼ����ѻ ///////////////////////////////////////////////
	unsigned int ra_total_consume_gold_begin_timestamp;				    // �ۼ����ѻ��ʼʱ��
	int ra_total_consume_gold_consume_num;								// �ۼ����ѻ�����Ԫ��
	int ra_total_consume_gold_fetch_reward_flag;						// �ۼ����ѻ���ȡ���

	////////////////////////////////////////// ���� ��Ծ����� ///////////////////////////////////////////////
	unsigned int ra_day_active_degree_begin_timestamp;				    // ��Ծ�������ʼʱ��
	int ra_day_active_degree_active_count;								// ��Ծ�������Ծ�ȶ�
	int ra_day_active_degree_fetch_reward_flag;							// ��Ծ��������ȡ���

	//////////////////////////////////////////  ���� �����챦  ///////////////////////////////////////////////
	unsigned int ra_chestshop_begin_timestamp;							// �����챦���ʼʱ��
	short ra_chestshop_fetch_reward_flag;								// �����챦����ȡ�������
	short ra_chestshop_xunbao_times;									// �����챦Ѱ������

	//////////////////////////////////////////  ���� ȫ����	  ///////////////////////////////////////////////
	unsigned int ra_quanmin_qifu_begin_timestamp;						// ȫ�������ʼʱ��
	short ra_quanmin_qifu_fetch_reward_flag;							// ȫ������ȡ�������
	short ra_quanmin_qifu_qifu_times;									// ȫ��������

	////////////////////////////////////////// ���� ��ɱboss� ///////////////////////////////////////////////
	unsigned int ra_kill_boss_begin_timestamp;							// ��ɱboss���ʼʱ��
	int ra_kill_boss_kill_count;										// ��ɱboss���ɱboss��

	////////////////////////////////////////// ���� �������� ///////////////////////////////////////////////
	unsigned int ra_shouyou_yuxiang_begin_timestamp;					// ����������ʼʱ��
	int ra_shouyou_yuxiang_fetch_reward_flag;							// ����������ȡ�������
	int ra_shouyou_yuxiang_give_flowers_flag;							// ���������ͻ����

	////////////////////////////////////////// ��¼���� ��ɱboss� ///////////////////////////////////////////////
	unsigned int ra_login_gift_begin_timestamp;							// ��¼������ʼʱ��
	short ra_login_gift_login_days;										// ��¼���� ��¼����
	char abandon_ch;													// ��������char����֮ǰ�߼��д���
	char ra_login_gift_has_fetch_accumulate_reward;						// ��¼���� �Ƿ��Ѿ���ȡ�ۼƵ�¼����
	int ra_login_gift_fetch_common_reward_flag;							// ��¼���� ��ȡ��ͨ�������
	int ra_login_gift_fetch_vip_reward_flag;							// ��¼���� ��ȡvip�������

	////////////////////////////////////////// һս����  //////////////////////////////////////////////////////
	short yizhandaodi_room_idx;											// һս���׷�������
	short yizhandaodi_dead_times;										// һս������������
	unsigned int yizhandaodi_leave_timestamp;							// һս�����뿪ʱ��

	///////////////////////////////////////////  ���˸���  //////////////////////////////////////////////////////
	int guildfb_first_enter_guild_id;									// ��һ�ν������˸���������ID ��ˢ	

	char zai_chongzhi_reward_state;										// �ٳ�ֵ״̬
	char ra_today_has_send_bipin_mail;									// �����Ѿ����ͱ�ƴ�ʼ�
	short reserve_sh;													//

	////////////////////////////////////////// ���� ��ƴ��ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_total_begin_timestamp;					// ��ƴ��ս����ʼʱ��
	int ra_bipin_cap_total_fetch_reward_flag;							// ��ƴ��ս����ȡ�������
	int ra_bipin_cap_total_history_max_value;							// ��ƴ��ս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴװ��ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_equip_begin_timestamp;					// ��ƴװ��ս����ʼʱ��
	int ra_bipin_cap_equip_fetch_reward_flag;							// ��ƴװ��ս����ȡ�������
	int ra_bipin_cap_equip_history_max_value;							// ��ƴװ��ս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ��Ůս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_xiannv_begin_timestamp;					// ��ƴ��Ůս����ʼʱ��
	int ra_bipin_cap_xiannv_fetch_reward_flag;							// ��ƴ��Ůս����ȡ�������
	int ra_bipin_cap_xiannv_history_max_value;							// ��ƴ��Ůս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ����ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_jingmai_begin_timestamp;					// ��ƴ����ս����ʼʱ��
	int ra_bipin_cap_jingmai_fetch_reward_flag;							// ��ƴ����ս����ȡ�������
	int ra_bipin_cap_jingmai_history_max_value;							// ��ƴ����ս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ����ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_mount_begin_timestamp;					// ��ƴ����ս����ʼʱ��
	int ra_bipin_cap_mount_fetch_reward_flag;							// ��ƴ����ս����ȡ�������
	int ra_bipin_cap_mount_history_max_value;							// ��ƴ����ս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ����ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_wing_begin_timestamp;						// ��ƴ����ս����ʼʱ��
	int ra_bipin_cap_wing_fetch_reward_flag;							// ��ƴ����ս����ȡ�������
	int ra_bipin_cap_wing_history_max_value;							// ��ƴ����ս����ʷ���ֵ

	//////////////////////////////////////////  ��ֵ���� ///////////////////////////////////////////////
	unsigned int ra_charge_repayment_begin_timestamp;					// ��ֵ�������ʼʱ��
	int ra_charge_repayment_reward_active_flag;							// ��ֵ��������������״̬
	int ra_charge_repayment_reward_fetch_flag;							// ��ֵ������������ȡ״̬
	int ra_charge_repayment_history_charge;								// ��ֵ������ڼ���ۼƳ�ֵ

	//////////////////////////////////////////  �۱��� ///////////////////////////////////////////////
	unsigned int ra_cornucopia_begin_timestamp;							// ��һ�ξ۱�����ʼʱ��
	int ra_cornucopia_history_charge;									// ��ǰ�۱����ڼ��ۼƳ�ֵ
	char ra_cornucopia_current_lun;										// �۱��赱ǰ����

	char third_chongzhi_reward_state;									// ��������ֵ����״̬
	short current_lun_reward_gold;										// ��ǰ�ִλ�ȡ���

	//////////////////////////////////////////  �Ϸ�� ///////////////////////////////////////////////
	UNSTD_STATIC_CHECK(3 == COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE);

	int csa_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE]; // ��������������
	int csa_roll_total_chongzhi;											// �Ϸ�ת�̳�ֵ����
	int csa_roll_chongzhi_num;												// �Ϸ�ת���ۼƳ�ֵ
	int csa_chongzhi_rank_chongzhi_num;										// �Ϸ���ֵ���г�ֵ��
	int csa_consume_rank_consume_gold;										// �Ϸ�������������Ԫ��
	int csa_kill_boss_kill_count;											// �Ϸ���ɱboss���ɱboss��

	//////////////////////////////////////////  ���ʳ�ֵ� ///////////////////////////////////////////////
	unsigned int ra_danbi_chongzhi_begin_timestamp;							// ���ʼʱ��
	int ra_danbi_chongzhi_reward_can_fetch_flag;							// �Ѽ���ĳ�ֵ��Ϣ
	int ra_danbi_chongzhi_reward_has_fetch_flag;							// ����ȡ�ĳ�ֵ��Ϣ

	//////////////////////////////////////////  �Ϸ�� ///////////////////////////////////////////////
	short csa_login_gift_login_days;										// �Ϸ���¼���� ��¼����
	char has_clear_chongzhi_reward_fetch_flag;								// �Ƿ��Ѿ��������ֵ��ȡ���
	char csa_login_gift_has_fetch_accumulate_reward;						// �Ϸ���¼���� �Ƿ��Ѿ���ȡ�ۼƵ�¼����
	int csa_login_gift_fetch_common_reward_flag;							// �Ϸ���¼���� ��ȡ��ͨ�������
	int csa_login_gift_fetch_vip_reward_flag;								// �Ϸ���¼���� ��ȡvip�������
	unsigned int csa_login_gift_last_login_dayid;							// �Ϸ���¼���� ����¼dayid

	UNSTD_STATIC_CHECK(8 == COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT);
	int csa_personal_panic_buy_numlist[COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; // �Ϸ����˷�����������

	UNSTD_STATIC_CHECK(16 == COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT);
	int csa_server_panic_buy_numlist[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];	// �Ϸ�ȫ�������������

	unsigned int ra_login_gift_last_login_dayid;								// ������¼���� ����¼dayid

	//////////////////////////////////////////  �ۼƳ�ֵ��������� ///////////////////////////////////////////////
	unsigned int ra_total_charge_day_begin_timestamp;							// ���ʼʱ��
	short ra_total_charge_day_reward_has_fetch_flag;							// ����ȡ�Ľ���
	char ra_total_charge_day_count;												// �ۼƳ�ֵ����
	char ra_total_charge_day_today_has_charged;									// �����Ƿ��ѳ�ֵ����ÿ�����ã�

	//////////////////////////////////////////	  ���ո����	 ///////////////////////////////////////////////
	unsigned int ra_tomorrow_reward_begin_timestamp;							// ���ʼʱ��
	short ra_tomorrow_reward_count;												// �ɳ齱����
	short reserve_sh3;															// ����

	////////////////////////////////////////// ���� ÿ���������� ///////////////////////////////////////////////
	unsigned int ra_day_consume_rank_begin_timestamp;							// ����ÿ���������п�ʼʱ��
	int ra_day_consume_rank_gold_num;											// ����ÿ��������������Ԫ��
	unsigned int ra_day_consume_rank_consume_last_timestamp;					// �����������ʱ���

	////////////////////////////////////////// ���� ÿ�ճ�ֵ���� ///////////////////////////////////////////////
	unsigned int ra_day_chongzhi_rank_begin_timestamp;							// ����ÿ�ճ�ֵ���п�ʼʱ��
	int ra_day_chongzhi_rank_gold_num;											// ����ÿ�ճ�ֵ���г�ֵԪ��
	unsigned int ra_day_chongzhi_rank_chongzhi_last_timestamp;					// ��������ֵʱ���

	//////////////////////////////////////////  �ۼƳ�ֵ� ///////////////////////////////////////////////
	unsigned int ra_total_charge_begin_timestamp;								// ���ʼʱ��
	int ra_total_charge_value;													// �ۼƳ�ֵ��
	int ra_total_charge_reward_has_fetch_flag;									// �ۼƳ�ֵ�-����ȡ�Ľ������

	/////////////////////////////////////////  ˫�����ֻ ///////////////////////////////////////////////
	unsigned int ra_double_score_begin_timestamp;								// ˫�����ֻ��ʼʱ��

	/////////////////////////////////////////  �Ϸ�ʱ�� ////////////////////////////////////////////
	unsigned int last_join_combine_server_activity_timestamp;					// �Ϸ�ʱ��

	/////////////////////////////////////////  ��ʱ�һ�ϡ��װ�� ////////////////////////////////////////////
	unsigned int ra_time_limit_exchange_equi_begin_tiemstamp;					// ��ʱ�һ����ʼʱ��
	unsigned int ra_time_limit_exchange_jl_begin_tiemstamp;						// ��ʱ�һ����ʼʱ��
	int ra_time_limit_exchange_exchange_equi_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];	// װ����Ʒ�һ�����--�����±���seq()��Ӧ
	int ra_time_limit_exchange_exchange_jl_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		// ����һ�����

	//////////////////////////////////////// �������齱� ////////////////////////////////////////////////
	unsigned int ra_level_lottery_timestamp;									// �ϴβ���ʱ��
	unsigned int ra_level_lottery_cur_level;									// ��ǰ�齱���ڵĲ�
	unsigned int ra_lottery_next_free_timestamp;								// �´���ѹ���ʱ��
	int ra_level_lottery_free_buy_times;										// ÿ����ѹ������
	int ra_lottery_buy_total_times;												// �ۼƹ������
	unsigned int ra_lottery_fetch_reward_flag;									// ��ȡ�����ı�־

	//////////////////////////////////////////	 ��ֵŤ���	 ///////////////////////////////////////////////
	unsigned int ra_niu_egg_timestamp;											// �ϴβ���ʱ��
	unsigned int ra_niu_egg_cur_chongzhi_value;									// ��ڼ���ʣ��Ŀ�Ť���ĳ�ֵ���
	int ra_niu_egg_server_reward_has_fetch_reward_flag;							// ȫ��Ť������������ȡ���

	//////////////////////////////////////////	 ÿ���ۼƳ�ֵ	 ///////////////////////////////////////////////
	int diff_wd_chongzhi_is_open;												// ÿ���۳��������
	int diff_wd_chongzhi_value;													// ÿ���۳��
	int diff_wd_chongzhi_stage_fetch_flag;										// ÿ���۳���ȡ���

	////////////////////////////////////////// ���� ��ƴ��װս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_shenzhuang_begin_timestamp;						// ��ƴ��װս����ʼʱ��
	int ra_bipin_cap_shenzhuang_fetch_reward_flag;								// ��ƴ��װս����ȡ�������
	int ra_bipin_cap_shenzhuang_history_max_value;								// ��ƴ��װս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ����ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_jingling_begin_timestamp;							// ��ƴ����ս����ʼʱ��
	int ra_bipin_cap_jingling_fetch_reward_flag;								// ��ƴ����ս����ȡ�������
	int ra_bipin_cap_jingling_history_max_value;								// ��ƴ����ս����ʷ���ֵ

	////////////////////////////////////////// ���� ��ƴ����ս�� ///////////////////////////////////////////////
	unsigned int ra_bipin_cap_wash_begin_timestamp;								// ��ƴ����ս����ʼʱ��
	int ra_bipin_cap_wash_fetch_reward_flag;									// ��ƴ����ս����ȡ�������
	int ra_bipin_cap_wash_history_max_value;									// ��ƴ����ս����ʷ���ֵ

	////////////////////////////////////////// ���� �䱦�� ///////////////////////////////////////////////
	unsigned int ra_zhenbaoge_timestamp;										// �䱦��
	unsigned int ra_zhenbaoge_next_free_flush_timestamp;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT == 5);
	ZhenbaogeAddWeightInfo ra_zhenbaoge_add_weight_list[RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT];	// ��������Ȩ��

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT == 9);
	short ra_zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];			// ����Ʒ�б�
	short ra_zhenbaoge_refresh_gift_fetch_flag;
	short ra_zhenbaoge_refresh_times;											// �����ֶ�ˢ�´���
	short ra_zhenbaoge_reserve_sh;

	////////////////////////////////////////// ���� ��ֵ��Ԫ��(2088) ///////////////////////////////////////////////
	unsigned int ra_charge_reward_timestamp;
	int ra_charge_reward_is_first;												// �׳��־(ÿ������)

	//////////////////////////////////////////  �ؾ�Ѱ��� ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao_timestamp;										// �ϴβ���ʱ��
	unsigned int ra_mijingxunbao_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_mijingxunbao_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��

	//////////////////////////////////////////  ���ٳ�ս ///////////////////////////////////////////////////////
	unsigned int ra_jisuchongzhan_timestamp;									// �ϴβ���ʱ��

	//////////////////////////////////////////	  ҡǮ���	 ///////////////////////////////////////////////
	unsigned int ra_money_tree_last_join_timestamp;								// �ϴβ���ʱ��
	int ra_money_tree_server_reward_has_fetch_reward_flag;						// ȫ��ҡǮ������������ȡ���
	int ra_money_tree_total_times;												// �齱����
	unsigned int ra_money_tree_free_timestamp;									// �����ѵ����ʱ��

	////////////////////////////////////////// ÿ�찮һ��(�׳巵Ԫ���ٷֱ�) ///////////////////////////////////////////////
	unsigned int ra_daily_love_timestamp;										// �ϴβ���ʱ��
	int ra_daily_love_daily_first_flag;											// ÿ���׳�

	//////////////////////////////////////////  ����-�����뷭�ƻ /////////////////////////////////////////////////////
	unsigned int ra_kingdraw_join_timestamp;									// ��Ҳ���ʱ��
	unsigned short ra_kingdraw_draw_times[RA_KING_DRAW_LEVEL_COUNT];			// ��ҳ齱����
	short ra_kingdraw_draw_reserve_sh;
	int ra_kingdraw_reward_flag;												// �۳齱����ȡ���

	UNSTD_STATIC_CHECK(RA_KING_DRAW_LEVEL_COUNT == 3);
	RAKingDrawCardList ra_kingdraw_card_list[RA_KING_DRAW_LEVEL_COUNT];			// ÿһ���ȼ��ķ�������

	UNSTD_STATIC_CHECK(RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT == 5);
	RAKingDrawSelfWeight ra_kingdraw_self_weight_list[RA_KING_DRAW_LEVEL_COUNT][RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT];

	////////////////////////////////////////// ����-�ۼƳ�ֵ3� ///////////////////////////////////////////////
	unsigned int ra_total_charge_3_begin_timestamp;								// ���ʼʱ��
	int ra_total_charge_3_cur_total_charge;										// ��ǰ�׶��ܳ�ֵ
	int ra_total_charge_3_cur_has_fetch_flag;									// ��ǰ�׶�����ȡ���

	//////////////////////////////////////////  ������������������л ///////////////////////////////////////////////
	short ra_special_appearance_card_passive_use_times;							// ��ǰ���������
	short ra_special_appearance_passive_has_got_join_reward;					// �Ƿ�����ȡ���뽱��
	unsigned int ra_special_appearance_card_passive_timestamp;					// �ϴβ���ʱ��

	//////////////////////////////////////////  ��������������л ///////////////////////////////////////////////
	short ra_special_appearance_card_use_times;									// ��ǰ�������
	short ra_special_appearance_has_got_join_reward;							// �Ƿ�����ȡ���뽱��
	unsigned int ra_special_appearance_card_timestamp;							// �ϴβ���ʱ��

	//////////////////////////////////////////  ȫ������ֵ��� ///////////////////////////////////////////////
	unsigned int ra_any_monster_drop_last_timestamp;							// ��һ�δ�ֵ�����Ʒ��ʱ���
	int ra_any_monster_drop_today_add_rate;										// ������ʹ�û��Ʒ�������Ա���
	int ra_any_monster_drop_total_add_rate;										// ��ǰ�����ۼƼӵĻ������Ա���(����������ս���й�)

	//////////////////////////////////////////  �ۼƳ�ֵ4� ///////////////////////////////////////////////
	unsigned int ra_total_charge_4_begin_timestamp;								// ���ʼʱ��
	int ra_total_charge_4_cur_total_charge;										// ��ǰ�׶��ܳ�ֵ
	int ra_total_charge_4_cur_has_fetch_flag;									// ��ǰ�׶�����ȡ���

	//////////////////////////////////////////  ����-�ڿ� /////////////////////////////////////////////////////////
	unsigned int ra_mine_join_timestamp;										// ��Ҳ���ʱ��
	unsigned int ra_mine_next_refresh_time;										// ��һ�ε����ˢ��ʱ��
	int ra_mine_cur_reward_fetch_flag;											// ��ǰȫ�������ȡ���
	short ra_mine_free_gather_times;											// ����ڿ����
	short ra_mine_role_refresh_times;											// ���˻������											

	UNSTD_STATIC_CHECK(RA_MINE_MAX_REFRESH_COUNT == 8);
	char ra_mine_type_list[RA_MINE_MAX_REFRESH_COUNT];							// ��ǰ�󳡵Ŀ�ʯ

	UNSTD_STATIC_CHECK(RA_MINE_MAX_TYPE_COUNT == 12);
	int ra_mine_gather_count_list[RA_MINE_MAX_TYPE_COUNT];						// ��ǰ�ڵ��Ŀ�ʯ��

	//////////////////////////////////////////  ����-���ιλ /////////////////////////////////////////////////////////
	unsigned int ra_guagua_join_timestamp;										// ��Ҳ���ʱ��
	unsigned int ra_guagua_next_free_time;										// ��һ�ε�����ѵ�ʱ��

	//////////////////////////////////////////  ����-�������Ի /////////////////////////////////////////////////////////
	unsigned int ra_tianming_join_timestamp;									// ��Ҳ���ʱ��

	UNSTD_STATIC_CHECK(RA_TIANMING_LOT_COUNT == 6);
	char ra_tianming_add_lot_times_list[RA_TIANMING_LOT_COUNT];					// ��ǩ��ע�����б�
	char ra_tianming_reward_history_list_cur_index;								// ������ʷ��¼�б�����ɽ�����¼���±�
	char ra_tianming_free_chou_times;											// �������Ի - ������ѳ齱����

	UNSTD_STATIC_CHECK(RA_TIANMING_REWARD_HISTORY_COUNT == 20);
	RATianMingRewardHistoryItem ra_tianming_reward_history_list[RA_TIANMING_REWARD_HISTORY_COUNT];	// ������ʷ��¼

	//////////////////////////////////////////  ����-����ת� /////////////////////////////////////////////////////////
	unsigned int ra_fanfan_join_timestamp;										// ��Ҳ���ʱ��

	char ra_fanfan_cur_free_times;												// ��ǰ��Ѵ���
	char ra_fanfan_cur_letter_occur_rate;										// ��ǰ�ֳ��ָ���
	unsigned short ra_fanfan_role_refresh_times;								// �������ô���

	unsigned int ra_fanfan_next_refresh_time;									// ��һ������ʱ��

	UNSTD_STATIC_CHECK(40 == RA_FANFAN_CARD_COUNT);
	unsigned char ra_fanfan_card_type_list[RA_FANFAN_CARD_COUNT];				// ���������б�

	UNSTD_STATIC_CHECK(10 == RA_FANFAN_MAX_WORD_COUNT);
	RAFanFanWordActiveInfo ra_fanfan_word_active_info_list[RA_FANFAN_MAX_WORD_COUNT];	// ���鼤����Ϣ�б�
	RAFanFanHiddenWordInfo ra_fanfan_hidden_word_info;							// ��������Ϣ

	/////////////////////////////////////////  ����-������ֵ� /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp;							// ��Ҳ���ʱ��
	int ra_continue_chongzhi_today_chongzhi;									// ���ճ�ֵ��
	short ra_continue_chongzhi_can_fetch_reward_flag;							// ���������� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ֵ꣬1���
	short ra_continue_chongzhi_has_fetch_reward_flag;							// ��������ȡ��� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ȡ��ֵ1����ȡ
	char ra_continue_chongzhi_days;												// ������ֵ����
	char ra_continue_chongzhi_is_activity_over;									// �Ƿ�������ֵ�ж�	0δ�ж�	1�ж�
	short ra_continue_chongzhi_reserve_sh;

	//////////////////////////////////////////  ����-�������ѻ /////////////////////////////////////////////////////////
	unsigned int ra_continue_consume_join_timestamp;							// ��Ҳ���ʱ��
	unsigned int ra_continue_consume_today_consume_gold_total;					// ����������Ԫ��
	unsigned int ra_continue_consume_cur_consume_gold;							// �ϴ���ȡ������Ŀǰ�ۻ�������Ԫ��		
	char ra_continue_consume_total_days;										// ���Ѵ��������
	char ra_continue_consume_days;												// ��ǰ�������Ѵ������
	char ra_continue_consume_extra_reward_num;									// ���⽱������
	char ra_continue_consume_the_last_time_consume_day_index;					// �����ĵ���

	//////////////////////////////////////////  ����-�������� /////////////////////////////////////////////////////////
	unsigned int ra_army_day_join_timestamp;									// ��Ҳ���ʱ��
	char ra_army_day_belong_army_side;											// ��������
	char ra_army_day_active_degree_exchange_flag;								// ��Ծ�ȶһ����ı��(ÿ������)
	short ra_army_day_reserver_sh;
	UNSTD_STATIC_CHECK(3 == RA_ARMY_DAY_ARMY_SIDE_NUM);
	int ra_army_day_flags_list[RA_ARMY_DAY_ARMY_SIDE_NUM];						// ��Ҹ�����������
	int ra_army_day_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];						// ����ڸ����ŵĻ���

	//////////////////////////////////////////  ����-ѭ����ֵ� /////////////////////////////////////////////////////////
	unsigned int ra_circulation_chongzhi_join_timestamp;						// ��Ҳ���ʱ��
	unsigned int ra_circulation_chongzhi_total_chongzhi;						// �ܳ�ֵԪ��
	unsigned int ra_circulation_chongzhi_cur_chongzhi_gold;						// �ϴ���ȡ������Ŀǰ�ۻ��ĳ�ֵԪ��

	//////////////////////////////////////////  ����-�������� /////////////////////////////////////////////////////////
	unsigned int ra_extreme_lucky_join_timestamp;								// ��Ҳ���ʱ��
	unsigned int ra_extreme_lucky_next_flush_timestamp;							// ����´�ˢ��ʱ��
	char ra_extreme_lucky_day_free_draw_times;									// ��ѳ齱����
	char ra_extreme_lucky_cur_total_draw_times;									// ��ǰ�ֳ齱����
	short ra_extreme_lucky_flush_times;											// ˢ�´���
	int ra_extreme_lucky_cur_gold_draw_times;									// ��ǰ����Ч�齱����(Ԫ����)
	short ra_extreme_lucky_total_draw_times;									// �齱�ܴ���
	short ra_extreme_lucky_return_reward_flag;									// ����������־

	UNSTD_STATIC_CHECK(10 == RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT);
	RAExtremeLuckyRewardInfo ra_extreme_lucky_item_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];

	//////////////////////////////////////////  �ؾ�Ѱ��2� ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao2_timestamp;									// �ϴβ���ʱ��
	unsigned int ra_mijingxunbao2_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_mijingxunbao2_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��

	//////////////////////////////////////////	  ҡǮ��2�	 ///////////////////////////////////////////////
	unsigned int ra_money_tree2_last_join_timestamp;							// �ϴβ���ʱ��
	int ra_money_tree2_server_reward_has_fetch_reward_flag;						// ȫ��ҡǮ������������ȡ���

	//////////////////////////////////////////	  ����ڻ	 ///////////////////////////////////////////////
	unsigned int ra_national_day_last_join_timestamp;							// �ϴβ���ʱ��

	////////////////////////////////////////// ���� �䱦��2 ///////////////////////////////////////////////
	unsigned int ra_zhenbaoge2_timestamp;										// �䱦��
	unsigned int ra_zhenbaoge2_next_free_flush_timestamp;						// �´�ˢ��ʱ��

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT == 5);
	ZhenbaogeAddWeightInfo ra_zhenbaoge2_add_weight_list[RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT];	// ��������Ȩ��

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT == 9);
	short ra_zhenbaoge2_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];			// ����Ʒ�б�
	short ra_zhenbaoge2_server_fetch_flag;

	//////////////////////////////////////////  ����-�������� /////////////////////////////////////////////////////////
	unsigned int ra_promoting_position_join_timestamp;
	unsigned int ra_promoting_position_next_free_timestamp;						// ��һ�ε���ѳ�ʱ���
	unsigned short ra_promoting_position_extra_times;							// ����齱��������ֵ���ͣ�
	RAPromotingPositionInfo ra_promoting_position_start_pos;					// �齱���λ��
	UNSTD_STATIC_CHECK(2 == sizeof(RAPromotingPositionInfo));
	int ra_promoting_position_chongzhi_gold;									// ��ڼ��ۼƳ�ֵ�ǰ�Ԫ��
	unsigned short ra_promoting_position_play_times;							// �齱����
	unsigned short ra_promoting_position_reward_flag;							// ����������־

	//////////////////////////////////////////  ����-�䱦�̳� /////////////////////////////////////////////////////////
	unsigned int ra_treasures_mall_join_timestamp;								// �ϴβμӻʱ��
	int ra_treasures_mall_score;												// ����

	//////////////////////////////////////////  ����-����� /////////////////////////////////////////////////////////
	OpenServerSubActivityInfo open_server_sub_info_list[OPEN_SERVER_ACTIVITY_COUNT];

	//////////////////////////////////////////  ÿ�ճ�ֵ���� ///////////////////////////////////////////////
	unsigned short first_chongzhi_fetch_reward_flag;							// �׳佱����ȡ���
	short daily_chongzhi_fetch_reward_flag;										// ÿ�ճ�ֵ������ȡ���
	short daily_chongzhi_complete_days;											// ÿ�ճ�ֵ�������
	short daily_chongzhi_times_fetch_reward_flag;								// ÿ�ճ�ֵ�ۼ������������

	//////////////////////////////////////////  �¾۱��� ///////////////////////////////////////////////
	static const int RA_NEW_CORNUCOPIA_TASK_COUNT_MAX = 16;							// �¾۱��������������
	UNSTD_STATIC_CHECK(RA_NEW_CORNUCOPIA_TASK_TYPE_MAX < RA_NEW_CORNUCOPIA_TASK_COUNT_MAX);
	int new_cornucopia_value;														// �¾۱���۱�ֵ
	short new_cornucopia_day_index;													// ��ڼ���
	short new_cornucopia_total_reward_flag;											// �¾۱���ȫ��������ȡ���
	unsigned char new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_COUNT_MAX];	// �¾۱�������������

	//////////////////////////////////////////  Boss���� ///////////////////////////////////////////////
	long long oga_kill_boss_flag;													// ����� ɱboss���
	int oga_kill_boss_reward_flag;													// ����� ɱboss�������
	unsigned int oga_kill_boss_join_timestamp;

	int oga_gift_shop_flag;															// ����� ����޹����

	//////////////////////////////////////////  ˮ���þ� ///////////////////////////////////////////////
	short shuijing_gather_count;													// ˮ���þ� �ɼ�����
	short shuijing_free_relive_count;												// ˮ���þ� ��Ѹ������
	int shuijing_day_bind_gold;														// ˮ���þ� ���հ�Ԫ��
	int shuijing_day_mojing;														// ˮ���þ� ����ħ��
	int shuijing_day_shengwang;														// ˮ���þ� ��������
	unsigned int shuijing_gather_buff_time;											// ˮ���þ� �ɼ��������buffʱ��

	//////////////////////////////////////////  �������� ///////////////////////////////////////////////
	unsigned int refine_join_timestamp;
	short refine_today_buy_time;													// ÿ�����ƴ���																
	unsigned short daily_activity_chongzhi;											// ÿ�ճ�ֵ���ֵ���
	int refine_reward_gold;															// �ܽ������	

	//////////////////////////////////////////  ���� ������� //////////////////////////////////////
	unsigned int ra_red_envelope_gift_begin_timestamp;									// ������� �������ʼʱ��
	int ra_red_envelope_gift_consume_gold_num;											// ������� ������ʯ��
	int ra_red_envelope_gift_reserve_int;
	int ra_red_envelope_gift_reward_flag;												// ������� ��ȡ���

	//////////////////////////////////////////  ��ֵ���� ///////////////////////////////////////////////
	unsigned int chongzhi_7day_reward_timestamp;										// ��ֵ-���췵�����ʱ��
	short chongzhi_7day_reward_fetch_day;												// ��ֵ-���췵����ȡ����
	short chongzhi_7day_reward_is_fetch;												// ��ֵ-���췵�������Ƿ���ȡ

	//////////////////////////////////////////  ���ֻ ///////////////////////////////////////////////
	int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT];		// ���ֻ�һ�����

	//////////////////////////////////////////  ÿ�ճ�ֵ����2 ///////////////////////////////////////////////
	short daily_chongzhi_fetch_reward2_flag;											// ÿ�ճ�ֵ����2��ȡ���
	unsigned short first_chongzhi_active_flag;											// �׳影�������־

	//////////////////////////////////////////  Ԫ��ת�� ///////////////////////////////////////////////
	unsigned int ra_gold_turntable_begin_timestamp;										// Ԫ��ת�� ��ʼʱ��
	int gold_turntable_score;															// Ԫ��ת�� ����

	/////////////////////////////////////////  �����ػݳ�-������ֵ� /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp_chu;								// ��Ҳ���ʱ��
	int ra_continue_chongzhi_today_chongzhi_chu;										// ���ճ�ֵ��
	short ra_continue_chongzhi_can_fetch_reward_flag_chu;								// ���������� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ֵ꣬1���
	short ra_continue_chongzhi_has_fetch_reward_flag_chu;								// ��������ȡ��� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ȡ��ֵ1����ȡ
	char ra_continue_chongzhi_days_chu;													// ������ֵ����
	char ra_is_lei_chongzhi_chu;														// �����Ƿ��Ѿ������۳�
	short ra_continue_chongzhi_reserve_sh_chu;

	/////////////////////////////////////////  �����ػݸ�-������ֵ� /////////////////////////////////////////////////////////
	unsigned int ra_continue_chongzhi_join_timestamp_gao;								// ��Ҳ���ʱ��
	int ra_continue_chongzhi_today_chongzhi_gao;										// ���ճ�ֵ��
	short ra_continue_chongzhi_can_fetch_reward_flag_gao;								// ���������� 
	short ra_continue_chongzhi_has_fetch_reward_flag_gao;								// ��������ȡ��� 
	char ra_continue_chongzhi_days_gao;													// �ۼƳ�ֵ����
	char ra_is_lei_chongzhi_gao;														// �����Ƿ��Ѿ������۳�						
	short ra_continue_chongzhi_reserve2;

	//////////////////////////////////////////  ���� ������� //////////////////////////////////////
	int ra_red_envelope_gift_consume_gold_num_list[RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT];	
	
	//////////////////////////////////////////  �����ٻ�	///////////////////////////////////////////////
	int ra_golden_pig_chongzhi;															// ��ڼ��ֵ����
	int ra_golden_pig_summon_credit;													// ����
	unsigned int ra_golden_pig_timestamp;												// �����ʱ��

	/////////////////////////////////////////   ����Ͷ�� //////////////////////////////////////////////
	unsigned int ra_open_server_invest_timestamp;										// �����ʱ��
	int ra_open_server_invest_reward_flag;												// �������־
	UNSTD_STATIC_CHECK(3 == RA_OPEN_SERVER_INVEST_TYPE_MAX);
	char ra_open_server_invest_reward_param[RA_OPEN_SERVER_INVEST_TYPE_MAX];			// ��ȡ��������������
	char ra_open_server_invest_is_add_active_param;										// ����Ļ�Ծ���Ƿ��Ѿ����

	/////////////////////////////////////////   �����ż� //////////////////////////////////////////////
	UNSTD_STATIC_CHECK(4 == XINGZUOYIJI_BOX_TYPE_MAX);
	short xingzuoyiji_gather_box_num[XINGZUOYIJI_BOX_TYPE_MAX];							// �ɼ���������� 

	///////////////////////////////////////// �������� ///////////////////////////////////////////////
	unsigned int ra_single_chongzhi_join_timestamp;										// �ϴβ���ʱ��
	int ra_single_chongzhi_reward_flag;													// ��ȡ���
	int ra_single_chongzhi_is_reward_flag;												// �Ƿ���ȡ��

	////////////////////////////////////////// �ۻ��� ///////////////////////////////////////////////////////
	XianyuanTreasInfo ra_xianyuan_treas_list[RAND_ACTIVITY_XIANYUAN_TREAS_COUNT];
	int ra_xianyuan_treas_all_buy_gift_fetch_flag;										// ȫ������������ȡ���
	unsigned int ra_xianyuan_treas_timestamp;											// �ϴβ���ʱ��
	
	//////////////////////////////////////////  ��ʱ��ɱ ////////////////////////////////////////////////////
	unsigned int ra_rush_buying_join_timestamp;
	char ra_rush_buying_cur_pahse;														// ��ʱ��ɱ����ҵ�ǰ�׶�
	char ra_rush_buying_reserved_ch;
	short ra_rush_buying_reserved_sh;
	char ra_rush_buying_item_buying_times[RA_RUSH_BUYING_SALES_ITEMS_COUNT];			// �����������
	UNSTD_STATIC_CHECK(0 == RA_RUSH_BUYING_SALES_ITEMS_COUNT % 4);

	//////////////////////////////////////////  ����-��ͼѰ�� //////////////////////////////////////////////////
	unsigned int ra_map_hunt_join_timestamp;										// ��ͼѰ������ʱ���
	RAMapHuntRouteInfo ra_map_hunt_cur_route_info;									// ��ǰ·����Ϣ
	short ra_map_hunt_has_used_free_xunbao_count;									// ��ʹ�õ����Ѱ������
	short ra_map_hunt_return_reward_fetch_flag;										// ȫ��������ȡ���
	unsigned int ra_map_hunt_next_flush_timestamp;									// �´����ˢ��ʱ���
	int ra_map_hunt_play_times;																	// ˢ���ܴ���

	//////////////////////////////////////////  ����-����Ѱ�� /////////////////////////////////////////////////////////
	unsigned int ra_light_tower_join_timestamp;									//����ʱ��
	UNSTD_STATIC_CHECK(5 == RA_LIGHT_TOWER_EXPLORE_MAX_LAYER);
	char ra_light_tower_info[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];					// ���и��ӵ��Ƿ���ȡ�����
	char ra_light_tower_layer_extern_reward_fetch_flag;							// ÿ����⽱����ȡ���
	char ra_light_tower_return_reward_fetch_flag;								// ȫ��������ȡ���
	char ra_light_tower_cur_index;												// ��ǰ���±�
	short ra_light_tower_cur_layer;												// ������ڵ�ǰ��
	short ra_light_tower_reserve_sh;											

	//////////////////////////////////////////  һЩ�۳齱��		/////////////////////////////////////////////////////
	int ra_fanfan_reward_flag;													// ����ת�۳齱����ȡ��־
	int ra_fanfan_leichou_times;												// ����ת�۳����

	int ra_light_tower_draw_times;												// ����Ѱ����ҳ�ȡ����

	int ra_mijingxunbao_personal_reward_flag;									// �ؾ�Ѱ���۳齱����ȡ��־
	int ra_mijingxunbao_role_chou_times;										// �ؾ�Ѱ���۳����

	int ra_zhenbaoge2_flush_times;												// �䱦��2�齱����

	int ra_niu_egg_times;														// ��ֵŤ������

	//////////////////////////////////////////  ���ۼƳ�ֵ� ///////////////////////////////////////////////
	unsigned int ra_new_total_charge_begin_timestamp;							// ���ʼʱ��
	int ra_new_total_charge_value;												// ���ۼƳ�ֵ��
	int ra_new_total_charge_reward_has_fetch_flag;								// ���ۼƳ�ֵ�-����ȡ�Ľ������

	/////////////////////////////////////////   �Ϸ�boss		//////////////////////////////////////////////
	int csa_boss_daily_kill_count;												// ÿ��ɱ�Ϸ�boss��

	//////////////////////////////////////////   ��װ�̵�  ///////////////////////////////////////////////
	unsigned int ra_magic_shop_begin_timestamp;									// ���ʼʱ��
	char ra_magic_shop_fetch_reward_flag;										// ������� ��ȡ������־   
	char ra_magic_shop_buy_flag;												// �����־
	char ra_magic_shop_reserve_ch;
	char ra_chongzhi_gift_join_act_day;                                         // ������� ��¼�μӵڼ���Ļ�����ڲ�������
	unsigned int ra_magic_shop_chongzhi_value;									// ������� ��ֵ���

	//////////////////////////////////////////  ����-Ȥζֲ�� /////////////////////////////////////////////////////////
	unsigned int ra_planting_tree_join_timestamp;								// ��Ҳ���ʱ��
	int ra_planting_tree_planting_times;										// ���ֲ������
	int ra_planting_tree_watering_times;

	////////////////////////////////////////// Ȥζ���� /////////////////////////////////////////////////////////////////////
	unsigned int ra_fishing_join_timestamp;
	unsigned int ra_fishing_steal_timestamp;										// ���� ʱ���
	unsigned int ra_fishing_fishing_timestamp;										// ͵�� ʱ���
	short ra_fishing_times;
	short ra_steal_fish_times;
	short ra_be_steal_fish_times;
	short ra_fish_info_list[RA_FISHING_FISH_TYPE_COUNT];
	int ra_fishing_score;
	int ra_steal_fish_score;

	////////////////////////////////////////// ����Ȼˮ�� ////////////////////////////////////////////////////////////////////
	unsigned int ra_crystal_dalaran_join_timestamp;
	unsigned int ra_crystal_dalaran_count;
	short ra_steal_crystal_dalaran_times;
	short ra_crystal_dalaran_be_stealed_times;
	unsigned int ra_steal_crystal_dalaran_timestamp;
	unsigned int ra_crystal_dalaran_be_stealed_timestamp;

	unsigned int ra_steal_dalaran_count;
	unsigned int ra_dalaran_be_stealed_count;
	RandActivityStealNoteCfg ra_be_stealed_info[RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY];	

	///////////////////////////////////////// ��Ԫ�� ///////////////////////////////////////////////////////////////////////
	unsigned int ra_chongzhi_crazy_rebate_join_timestamp;
	int ra_chongzhi_crazy_rebate_day_chongzhi_total_count;
	int ra_chongzhi_crazy_rebate_had_rebete_count;


	//////////////////////////////////////////  ����  ���� ///////////////////////////////////////////////
	FairyBuddhaCard fairy_buddha_card;

	//////////////////////////////////////////  ���� /////////////////////////////////////////////////////////
	int fish_gear_num_list[FISHING_GEAR_MAX_COUNT];										// ӵ�еķ���
	int fish_score;

	//////////////////////////////////////////  ��������  ///////////////////////////////////////////////
	unsigned int ra_perfect_lover_begin_timestamp;			                    // �������˿�ʼʱ��
	short ra_perfect_lover_type_record_flag;									// ������ͼ�¼���
	short ra_perfect_lover_type_fetch_flag;									    // ��齱����ȡ���
	int ra_perfect_lover_last_marry_role_id;									// �ϴ����Ľ�ɫid

	//////////////////////////////////////////  ȫ����ף��������  ///////////////////////////////////////////////
	unsigned int ra_upgrade_reward_join_timestamp;								
	int	ra_upgrade_reward_flag;													//ȫ�������ȡ���

	//////////////////////////////////////////  ȫ���ܶ�Ա���������///////////////////////////////////////////////
	unsigned int ra_upgrade_group_reward_join_timestamp;						
	int	ra_upgrade_group_reward_flag;											//ȫ���ܶ�Ա��ȡ���

	/////////////////////////////////////////	��������(�����)	///////////////////////////////////////
	short ra_guild_battle_reward_type;									// �������Խ�������
	short ra_guild_battle_reward_flag;									// �������Խ�����ȡ���

	//////////////////////////////////////////  �����콱 /////////////////////////////////////////////////////////
	unsigned int ra_consume_gift_join_timestamp;               // ����ʱ���
	unsigned int ra_consume_gift_reward_flag;                  // ������ȡ���
	int ra_consume_gift_consume_gold;						   // ����Ԫ��
	short ra_consume_gift_today_join_times;                    // �齱����
	char ra_consume_gift_roll_seq;                             // ҡ��ҡ������
	char ra_consume_gift_roll_num;                             // ҡ��ҡ�õ�����
	char ra_consume_gift_roll_seq_list[RA_CONSUM_GIFT_MAX_ROLL_COUNT];		// ҡ��ҡ�������б�
	char ra_consume_gift_roll_num_list[RA_CONSUM_GIFT_MAX_ROLL_COUNT];			// ҡ��ҡ�õ������б�

	//////////////////////////////////////////  ÿ���޹� /////////////////////////////////////////////////////////
	unsigned int ra_limit_buy_join_timestamp;                               // ����ʱ���
	char ra_limit_buy_had_buy_count[RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ]; //�������

	//////////////////////////////////////////  �۱��� /////////////////////////////////////////////////////////
	char ra_collect_treasure_can_join_times;                  // �ɲ������
	char ra_collect_treasure_join_times;                      // �������
	char ra_collect_treasure_roll_seq;                        // ҡ������
	unsigned int ra_collect_treasure_join_timestamp;          // ����ʱ���

	//////////////////////////////////////////  �����۳� /////////////////////////////////////////////////////////
	unsigned int ra_happy_cumul_chongzhi_join_timestamp;          // ����ʱ���
	unsigned int ra_happy_cumul_chongzhi_fetch_flag;              // ������ȡ���
	int ra_happy_cumul_chongzhi_num;                              // ��ֵ����

	//////////////////////////////////////////  ���׷��� /////////////////////////////////////////////////////////
	unsigned int ra_jinjie_return_join_timestamp;          // ����ʱ���
	unsigned int ra_jinjie_return_fetch_flag;              // ������ȡ���

	/////////////////////////////////////// ��ʱ���� ////////////////////////////////////////////////////////////////////////
	unsigned int ra_limit_time_rebate_join_timestamp;
	int ra_limit_time_rebate_reward_flag;
	int ra_limit_time_rebate_cur_day_chongzhi;
	int ra_limit_time_rebate_chongzhi_list[RA_LIMIT_TIME_REBATE_MAX_DAY];
	char ra_limit_time_rebate_cur_day_is_chongzhi;
	char ra_limit_time_rebate_total_chongzhi_days;

	/////////////////////////////////////// ���׻�ϴβ������� ���ڽ�������//////////////////////////////////////////////////////////////
	char ra_consume_gift_join_theme;                                    // �����콱 �ϴβ�������
	char ra_happy_cumul_chongzhi_join_theme;                            // �����۳� 
	
	//////////////////////////////////////////  ��ʱ��� ///////////////////////////////////////////////
	unsigned int ra_time_limit_gift_begin_timestamp;						// ���ʼʱ��
	short ra_time_limit_gift_join_vip_level;								// ��ʱ�������ʱvip�ȼ�
	short ra_time_limit_gift_open_flag;										// ��ʱ��������رձ��
	int ra_time_limit_gift_reward_can_fetch_flag1;							// �Ѽ����1�ĳ�ֵ��Ϣ
	int ra_time_limit_gift_reward_has_fetch_flag1;							// ����ȡ����1�ĳ�ֵ��Ϣ
	int ra_time_limit_gift_reward_can_fetch_flag2;							// �Ѽ����2�ĳ�ֵ��Ϣ
	int ra_time_limit_gift_reward_has_fetch_flag2;							// ����ȡ����2�ĳ�ֵ��Ϣ

	/////////////////////////////////////////   ���� ѭ����ֵ2	///////////////////////////////////////////////
	unsigned int ra_circulation_chongzhi_2_begin_timestamp;				// ѭ����ֵ2���ʼʱ��
	int ra_circulation_chongzhi_2_cur_chongzhi_gold;					// ʣ���ֵ��
	int ra_circulation_chongzhi_2_total_chongzhi_gold;					// ��ܳ�ֵ��

	//////////////////////////////////////////����  ���ҡǮ�� //////////////////////////////////////////////////////
	unsigned int ra_shakemoney_begin_timestamp;							// �ϴβ���ʱ��
	int ra_shakemoney_history_chongzhi_num;								// ���ҡǮ����ÿ��ĳ�ֵ��
	int ra_shakemoney_fetched_num;										// ���ҡǮ��������

	//////////////////////////////////////// ��ʱ���� ////////////////////////////////////////////////
	int ra_time_limit_luxury_gift_open_flag;								// ��ʱ���񿪹ر��
	unsigned int ra_time_limit_luxury_gift_bag_begin_timestamp;				// ���ʼʱ��
	short ra_time_limit_luxury_gift_bag_join_vip_level;						// ��ʱ�������ʱvip�ȼ�
	short ra_time_limit_luxury_gift_bag_is_already_buy;						// �Ƿ��Ѿ��������

	////////////////////////////////////////// ����ͬ��(˫����ֵ) ///////////////////////////////////////////////
	unsigned int ra_reset_double_chongzhi_join_timestamp;				// �ϴβμӻ��ʱ��
	unsigned int ra_reset_double_chongzhi_reward_flag;					// ������ȡ��־

	//////////////////////////////////////////  RMB��������	///////////////////////////////////////////////
	unsigned int ra_rmb_buy_chest_shop_join_timestamp;								// ����ʱ���
	char ra_rmb_buy_chest_shop_buy_count_list[RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT];	// �����б�
	UNSTD_STATIC_CHECK(RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT % 4 == 0);

	/////////////////////////////////////////	���� ���ѷ���		///////////////////////////////////////////////
	unsigned int ra_consume_gold_reward_join_timestamp;					// ����ʱ��
	int ra_consume_gold_reward_consume_gold;							// ���ѽ��
	char ra_consume_gold_reward_fetch_flag;								// ���ѷ�����ȡ������־
	char ra_consume_gold_reward_today_vip_level;						// ���ѷ�����vip�ȼ�
	char ra_consume_gold_reward_activity_day;							// ���ѷ����ﵽĿ�������

	////////////////////////////////////////// ����ͬ��(˫����ֵ) ///////////////////////////////////////////////
	char ra_reset_double_chongzhi_open_flag;                            // ��������ͻ��˱�ǵ�һ��������ʾ

	//////////////////////////////////////////  �������� /////////////////////////////////////////////////////////
	unsigned int ra_consume_for_gift_join_timestamp;					// ����ʱ��
	int ra_consume_for_gift_total_consume_gold;							// ��ڼ������ѷǰ�Ԫ��
	int ra_consume_for_gift_cur_points;									// ���û���
	unsigned char ra_consume_for_gift_item_exchange_times[RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT];	//��������
	UNSTD_STATIC_CHECK(0 == RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT % 4);

	//////////////////////////////////////////  �ؾ�Ѱ��3� ///////////////////////////////////////////////////////
	unsigned int ra_mijingxunbao3_timestamp;									// �ϴβ���ʱ��
	unsigned int ra_mijingxunbao3_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_mijingxunbao3_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��
	int ra_mijingxunbao3_daily_free_times;										// �ؾ�Ѱ��3ÿ����Ѵ���
	int ra_mijingxunbao3_personal_reward_flag;									// �ؾ�Ѱ��3�۳齱����ȡ��־
	int ra_mijingxunbao3_role_chou_times;										// �ؾ�Ѱ��3�۳����

	//////////////////////////////////////////  �����ҵ�� ///////////////////////////////////////////////////////
	unsigned int ra_huanlezadan_timestamp;										// �ϴβ���ʱ��
	unsigned int ra_huanlezadan_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_huanlezadan_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��
	int ra_huanlezadan_daily_free_times;										// �����ҵ�ÿ����Ѵ���
	int ra_huanlezadan_personal_reward_flag;									// �����ҵ��۳齱����ȡ��־
	int ra_huanlezadan_role_chou_times;											// �����ҵ��۳����

	//////////////////////////////////////////  ����ҡ��� ///////////////////////////////////////////////////////
	unsigned int ra_huanleyaojiang_timestamp;									// �ϴβ���ʱ��
	unsigned int ra_huanleyaojiang_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_huanleyaojiang_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��
	int ra_huanleyaojiang_daily_free_times;										// ����ҡ��ÿ����Ѵ���
	int ra_huanleyaojiang_personal_reward_flag;									// ����ҡ���۳齱����ȡ��־
	int ra_huanleyaojiang_role_chou_times;										// ����ҡ���۳����

	//////////////////////////////////////////  ���±�� ///////////////////////////////////////////////
	unsigned int collection_exchange_2_join_timestamp;											// �ϴβ���ʱ��
	int collection_exchange_2_times[RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT];		// ���ֻ�һ�����

	///////////////////////////////////////// �汾�ۼƳ�ֵ /////////////////////////////////////////////////////
	unsigned int ra_version_total_charge_begin_timestamp;							// ���ʼʱ��
	int ra_version_total_charge_value;												// �ۼƳ�ֵ��
	int ra_version_total_charge_reward_has_fetch_flag;								// �ۼƳ�ֵ�-����ȡ�Ľ������

	/////////////////////////////////////////  �汾������ֵ  /////////////////////////////////////////////////////////
	unsigned int ra_version_continue_chongzhi_join_timestamp;						// ��Ҳ���ʱ��
	int ra_version_continue_chongzhi_today_chongzhi;		 						// ���ճ�ֵ��
	short ra_version_continue_chongzhi_can_fetch_reward_flag;						// ���������� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ֵ꣬1���
	short ra_version_continue_chongzhi_has_fetch_reward_flag;						// ��������ȡ��� λ0������⽱��,����λ��Ǵ�꽱��, ֵ0δ��ȡ��ֵ1����ȡ
	char ra_version_continue_chongzhi_days;					 						// ������ֵ����
	char ra_version_is_lei_chongzhi;						 						// �����Ƿ��Ѿ������۳�
	short ra_version_continue_chongzhi_reserve_sh;

	//////////////////////////////////////////  ����ҡ��2� ///////////////////////////////////////////////////////
	unsigned int ra_huanleyaojiang_2_timestamp;										// �ϴβ���ʱ��
	unsigned int ra_huanleyaojiang_2_next_free_tao_timestamp;						// ��һ������Ա�ʱ��
	MiJingXunBaoAddWeightInfo ra_huanleyaojiang_2_add_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT];	// ��������Ȩ��
	int ra_huanleyaojiang_2_daily_free_times;										// ����ҡ��ÿ����Ѵ���
	int ra_huanleyaojiang_2_personal_reward_flag;									// ����ҡ���۳齱����ȡ��־
	int ra_huanleyaojiang_2_role_chou_times;

	/////////////////////////////////////////	���µ��ʳ�ֵ0		////////////////////////////////////////////////////
	unsigned int ra_offline_single_charge_join_timestamp_0;
	int ra_offline_single_charge_max_value_0;															//���λ
	int ra_offline_single_charge_reward_times_list_0[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];		//������ȡ����

	//////////////////////////////////////////  ����-�����뷭�ƻ(����) /////////////////////////////////////////////////////
	int ra_kingdraw_reward_score[RA_KING_DRAW_LEVEL_COUNT];				//�����뷭�ƽ����ܻ���

	//////////////////////////////////////// ��һ��һ //////////////////////////////////////////////////////////////////////
	unsigned int ra_buy_one_get_one_free_join_timestamp;				// ����ʱ��
	long long ra_buy_one_get_one_free_buy_flag;							// ������
	long long ra_buy_one_get_one_free_fetch_reward_flag;				// ��ȡ���
	
	////////////////////////////////////////// ��¼���� ������ ///////////////////////////////////////////////
	// 2217
	unsigned int ra_login_gift_begin_timestamp_0;							// ��¼������ʼʱ��
	short ra_login_gift_login_days_0;										// ��¼���� ��¼����
	char reserve_ch_1_login_gift_0;
	char reserve_ch_2_login_gift_0;
	int ra_login_gift_fetch_common_reward_flag_0;							// ��¼���� ��ȡ��ͨ�������
	int ra_login_gift_fetch_vip_reward_flag_0;								// ��¼���� ��ȡvip�������
	unsigned int ra_login_gift_last_login_dayid_0;							// ������¼���� ����¼dayid

	//////////////////////////////////////////  ���� ÿ�պ���  ///////////////////////////////////////////////
	unsigned int ra_everyday_nice_gift_begin_timestamp;						// ���ʼʱ��
	int ra_everyday_nice_gift_can_fetch_reward_flag;						// ��ȡ�������
	int ra_everyday_nice_gift_have_fetch_reward_flag;						// �Ѿ���ȡ���
	short ra_everyday_nice_gift_is_have_recharge_standard;					// �Ƿ��ֵ���
	short ra_everyday_nice_gift_reserve_sh;									// ��ȡ��������
	int ra_everyday_nice_gift_everydaty_recharge;							// ���ճ�ֵ����
	unsigned int ra_everyday_nice_gift_recharge_timestamp;					// ��ֵʱ���

	//////////////////////////////////////// ����--������ //////////////////////////////////////////////////////////////////
	unsigned short ra_guagua_acc_reward_has_fetch_flag;					// �ۼƽ���
	short ra_guagua_acc_count;									        // �ۼƴ���
	short rev_sh_01;

	//////////////////////////////////////////  ���� �����ػ�  ///////////////////////////////////////////////
	int ra_holiday_guard_kill_monster_count;								// �����ػ�ɱ������
	unsigned int ra_holiday_guard_participate_timestamp;					// �����ػ�����ʱ��

	//////////////////////////////////////////  �ۼƳ�ֵ5� ///////////////////////////////////////////////
	unsigned int ra_total_charge_5_begin_timestamp;									// ���ʼʱ��
	int ra_total_charge_5_cur_total_charge;											// ��ǰ�׶��ܳ�ֵ
	int ra_total_charge_5_cur_has_fetch_flag;										// ��ǰ������ȡ��־
	short ra_total_charge_5_join_day_index;											// �ۼƳ�ֵ5��������������x��

	//////////////////////////////////////////  ���� ������ս  ///////////////////////////////////////////////
	char ra_extreme_challenge_ultimate_have_fetch;							// ���ս����Ѿ���ȡ
	char rev_ch_01;
	RAExremeChallengeTaskInfo ra_extreme_challenge_task_info_list[RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM];	// ����
	unsigned int ra_extreme_challenge_last_update_timestamp;				// ���ӻ���ȵļ��
	unsigned int ra_extreme_challenge_timestamp;							// �ϴβ���ʱ��

	//////////////////////////////////////////  ���� �������  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_gift_begin_timestamp;									// ���ʼʱ��

	//////////////////////////////////////////  �ºϷ�� ///////////////////////////////////////////////
	unsigned int csa_touzijihua_total_fetch_flag;						// Ͷ�ʼƻ���ȡ������־
	unsigned short csa_touzjihua_login_day;								// Ͷ�ʼƻ��������¼����
	char csa_touzijihua_buy_flag;										// �Ƿ���Ϸ�Ͷ�ʼƻ�
	char csa_refine_exp_had_buy;										// �Ƿ��������

	CSAFoundationStatus csa_foundation_status[COMBINE_SERVER_MAX_FOUNDATION_TYPE];	//�ɳ�������״̬
	UNSTD_STATIC_CHECK(sizeof(CSAFoundationStatus) % 4 == 0);
	short csa_gongchengzhan_win_times;									// ����սʤ�����������ֵ�����⣬�´κϷ�Ҫ�̳�������ݣ������
	char csa_had_add_gongchengzhan_win_times;							// �Ƿ����ӹ�����ս����,�����ֵ�ж��ܲ��ܼӣ����ֵҪ���
	char res_ch_1;

	//////////////////////////////////////////  ���� ������а�  ///////////////////////////////////////////////
	unsigned int ra_profess_rank_begin_timestamp;						// ������а� ��ʼʱ��
	int ra_profess_score;												// ������а� ��׻���
	int ra_profess_to_num;												// ������а� ��״���
	int ra_profess_from_num;											// ������а� ����״���
	unsigned int ra_profess_rank_last_profess_timestamp;				// ������а� ���±��ʱ��

	//////////////////////////////////////////  ���׷���2 /////////////////////////////////////////////////////////
	unsigned int ra_jinjie_return2_join_timestamp;          // ����ʱ���
	unsigned int ra_jinjie_return2_fetch_flag;              // ������ȡ���

	/////////////////////////////////////// ���׻�ϴβ������� ���ڽ�������//////////////////////////////////////////////////////////////
	int ra_consume_gift_join_opengame_day;                                    // �����콱 �ϴβ���ʱ�Ŀ�������

	//////////////////////////////////////		��Ϲ���		/////////////////////////////////////////
	unsigned int ra_combine_buy_join_timestamp;											// ����ʱ��
	unsigned char ra_combine_buy_item_buy_times_list[RA_COMBINE_BUY_MAX_ITEM_COUNT];	// ������Ʒ����
	char ra_combine_buy_bucket_item_seq_list[RA_COMBINE_BUY_BUCKET_ITEM_COUNT];			// ���ﳵ��Ʒ
	short ra_combine_buy_reserve_sh;

	////////////////////////////////////////	���췵��			/////////////////////////////////////
	char had_add_chongzhi_day_count;										// �ܷ����ӳ�ֵ����
	char chongzhi_day_reserve_ch;
	short total_chongzhi_day_count;											// �ۼƳ�ֵ�ﵽҪ������
	unsigned int chongzhi_day_reward_fetch_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];		// ������ȡ���
	unsigned int chongzhi_day_rare_reward_fetch_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];	// ��ϡ������ȡ���

	////////////////////////////////////////	������Ը			/////////////////////////////////////
	unsigned int ra_lucky_wish_lucky_value;										// ����ֵ
	unsigned int ra_lucky_wish_fetch_reward_need_lucky_value;					// �����������ֵ
	unsigned int ra_lucky_wish_join_timestamp;									// ����ʱ��

	////////////////////////////////////////	������ʼ�֪ͨ���			/////////////////////////////////////
	char ra_open_server_mail_send_flag;									// ��ǣ�0����δ���ͣ�1�����ѷ��ͣ�ÿ������
	char ra_open_server_mail_send_reserve_ch;
	/////////////////////////////////////////  ��ʱ����-������ֵ� /////////////////////////////////////////////////////////
	char ra_continue_chongzhi_days_2;												// �ۼƳ�ֵ����
	char ra_continue_chongzhi_2_is_lei_chongzhi;									// �����Ƿ��Ѽ����۳�				
	unsigned int ra_continue_chongzhi_join_timestamp_2;								// ��Ҳ���ʱ��
	int ra_continue_chongzhi_today_chongzhi_2;										// ���ճ�ֵ��
	short ra_continue_chongzhi_can_fetch_reward_flag_2;								// ���������� 
	short ra_continue_chongzhi_has_fetch_reward_flag_2;								// ��������ȡ��� 

	/////////////////////////////////////////  ���� - ��ĩ�񻶻 /////////////////////////////////////////////////////////
	unsigned int ra_lottery1_join_timestamp;										// ��Ҳ���ʱ��
	unsigned int ra_lottery1_next_free_timestamp;									// �����һ�����ʱ��
	unsigned short ra_lottery1_fetch_person_reward_flag;							// ��ȡ���˽������
	short ra_lottery_reserve_sh;													// 
	unsigned int ra_lottery1_person_chou_times;										// �����ۼƳ齱����

	//////////////////////////////////////////  ��ʱ��� ///////////////////////////////////////////////
	int ra_time_limit_gift_reward_can_fetch_flag3;									// �Ѽ����3�ĳ�ֵ��Ϣ
	int ra_time_limit_gift_reward_has_fetch_flag3;									// ����ȡ����3�ĳ�ֵ��Ϣ

	unsigned char today_theme_reward_flag[TODAY_THEME_REWARD_MAX_BYTE];				// ����������ȡ���

	//////////////////////////////////////////  �ٱ�����2 ///////////////////////////////////////////////
	short baibeifanli2_flag;												// �ٱ�����2������
	short baibeifanli2_reserve_sh;

	//////////////////////////////////////////  ����޹�2 ///////////////////////////////////////////////
	int oga_gift_shop2_flag;															// ����޹�2���

	//////////////////////////////////////////  ��Ԫ������չ���������࣬������ ///////////////////////////////////////////////
	unsigned int ra_zero_buy_return_buy_timestamp_dirty[3];							// ��Ԫ������ʱ��� - �����ݣ�����
	unsigned int ra_zero_buy_return_day_fetch_flag_dirty[3];						// ��Ԫ��ÿ����ȡԪ����� - �����ݣ�����
	
	////////////////////////////////////////// ÿ�찮һ��2(�׳巵Ԫ���ٷֱ�) ///////////////////////////////////////////////
	unsigned int ra_daily_love_2_join_timestamp;						// �ϴβ���ʱ��
	int ra_daily_love_daily_2_has_charge;								// �Ƿ��ֵ

	//////////////////////////////////////////  ���� �������  ///////////////////////////////////////////////
	unsigned int ra_kuang_hai_qing_dian_begin_timestamp;								// ������� ��ʼʱ��
	short ra_kuang_hai_qing_dian_total_score;											// ������� ��ǰ���˵�
	short ra_kuang_hai_qing_dian_fetch_reward_flag;										// ������� ��ȡ�������
	unsigned char ra_kuang_hai_qing_dian_score_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];	// ������� ÿ��task����ĵ�����ע�⣬������task_id��Ӧ���������õ�index��
	int ra_kuang_hai_qing_dian_task_info_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];// ������� ÿ��task��ÿ����ɽ��ȣ���Ӧ����ɺ���0�����Լ�¼��һ�Σ�

	//////////////////////////////////////////  ���� �����ո�  ///////////////////////////////////////////////
	unsigned int ra_gift_harvest_begin_timestamp;						// ������а� ��ʼʱ��
	int ra_gift_harvest_score;											// �����ո� �ܻ���
	short ra_gift_harvest_get_score_times;								// �����ո� �ܻ�ɱ��
	short ra_gift_harvest_reserve_sh;

	////////////////////////////////////////// ÿ���۳� /////////////////////////////////////////////////// /////
	int last_chongzhi_day;													// ��һ�γ�ֵʱ��(ÿ���۳䲹������)

	////////////////////////////////////////// ���ǽ��� ///////////////////////////////////////////////
	unsigned int ra_marry_me_join_timestamp;						// �ϴβ���ʱ��
	short ra_marry_me_is_fetch_reward;								// �Ƿ���ȡ����
	short ra_is_has_first_recharge_attr_add;						// �Ƿ�ӵ���׳����Լӳ�

	//////////////////////////////////////////  ��Ԫ�� ///////////////////////////////////////////////
	unsigned int ra_zero_buy_return_buy_timestamp[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	// ��Ԫ������ʱ���
	unsigned int ra_zero_buy_return_day_fetch_flag[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	// ��Ԫ��ÿ����ȡԪ�����

	//////////////////////////////////////////  ���� �����ƹ�  ///////////////////////////////////////////////
	unsigned int ra_lucky_cloud_buy_timestamp;								// �����ƹ�����ʱ��
	short ra_lucky_cloud_buy_count;											// ���ֹ������
	short ra_lucky_cloud_buy_price;											// ��¼���ֹ���ĵ��ۣ���Ϊÿ�ֵ��۲�һ��������Ԫ����Ҫȷ�����ۣ�

	//////////////////////////////////////////  ���� �񻶴��ֹ�  ///////////////////////////////////////////////
	unsigned int ra_cracy_buy_begin_timestamp;								// ��������ʼʱ��
	int ra_cracy_buy_chongzhi;												// �ۼƳ�ֵ
	int ra_cracy_buy_level;													// ���ڲ���
	int ra_cracy_buy_limit[RA_MAX_CRACY_BUY_NUM_LIMIT];						// ��Ʒ��ǰ������
	//////////////////////////////////////////  ���ۼƳ�ֵ�1 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge1_begin_timestamp;							// ���ʼʱ��
	int ra_new_total_charge1_value;												// ���ۼƳ�ֵ��
	int ra_new_total_charge1_reward_has_fetch_flag;								// ���ۼƳ�ֵ�-����ȡ�Ľ������
	//////////////////////////////////////////  ���ۼƳ�ֵ�2 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge2_begin_timestamp;							// ���ʼʱ��
	int ra_new_total_charge2_value;												// ���ۼƳ�ֵ��
	int ra_new_total_charge2_reward_has_fetch_flag;								// ���ۼƳ�ֵ�-����ȡ�Ľ������
	//////////////////////////////////////////  ���ۼƳ�ֵ�3 ///////////////////////////////////////////////
	unsigned int ra_new_total_charge3_begin_timestamp;							// ���ʼʱ��
	int ra_new_total_charge3_value;												// ���ۼƳ�ֵ��
	int ra_new_total_charge3_reward_has_fetch_flag;								// ���ۼƳ�ֵ�-����ȡ�Ľ������

	////////////////////////////////////////  ˫��Ԫ��  ///////////////////////////////////////////////////////
	unsigned int ra_double_get_begin_timestamp;							// ���ʼʱ��
	int ra_double_get_reward_fetch_flag;								// ˫��Ԫ��������ȡ����

	////////////////////////////////////////// ���ʳ�ֵ1 //////////////////////////////////////////////////////
	unsigned int ra_single_charge1_begin_timestamp;
	int ra_single_charge1_reward_fetch_flag;

	////////////////////////////////////////// ���ʳ�ֵ2 //////////////////////////////////////////////////////
	unsigned int ra_single_charge2_begin_timestamp;
	int ra_single_charge2_reward_fetch_flag;

	////////////////////////////////////////// ���ʳ�ֵ3 //////////////////////////////////////////////////////
	unsigned int ra_single_charge3_begin_timestamp;
	int ra_single_charge3_reward_fetch_flag;

	//////////////////////////////////////////  ���� ��ֵ����2  ///////////////////////////////////////////////
	unsigned int ra_chongzhi_rank2_begin_timestamp;						// ������ֵ�μ�ʱ��
	int ra_chongzhi_rank2_chongzhi_num;									// ������ֵ����
	unsigned int ra_chongzhi_rank2_chongzhi_last_timestamp;				// ������ֵʱ��
}; 

typedef char RoleActivityParamHex[sizeof(RoleActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleActivityParamHex) < 10240);

#pragma pack(pop)

#endif // __ROLE_ACTIVITY_DEF_HPP__

