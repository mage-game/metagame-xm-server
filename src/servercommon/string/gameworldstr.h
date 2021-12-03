#ifndef __GAMEWORLDSTR_H__
#define __GAMEWORLDSTR_H__

#include "servercommon/guilddef.hpp"

enum OPEN_RANDGIFT_BROADCAST_TYPE
{
	OPEN_RANDGIFT_BROADCAST_ID_INVALID = 0,
	OPEN_RANDGIFT_BROADCAST_ID_CHOCO,					// 打开巧克力播报
	OPEN_RANDGIFT_BROADCAST_ID_SPRING_FESTIVAL_GIFT,	// 新春豪华礼包
	OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM,
};

namespace gamestring
{
	extern char GAMESTRING_BUF[2048];

	extern char *g_link_upstar;
	extern char *g_link_fuling;
	extern char *g_link_lieming;
	extern char *g_link_jingling_uplevel;
	extern char *g_link_jingling_upgrade;
	extern char *g_link_jingling_strength;
	extern char *g_link_chengjiu_title;
	extern char *g_link_xianjie;
	extern char *g_link_inherit;
	extern char *g_link_jingling_phantom;
	extern char *g_link_jingling_soul;

	extern char *g_gaoji_chestshop_item_broadcast;
	extern char *g_chuji_equip_chestshop_item_broadcast;
	extern char *g_chuji_jingling_chestshop_item_broadcast;
	extern char *g_chestshop_get_xiannv_notice;
	extern char *g_chestshop_get_xiannv_content;

	extern char *g_gengu_broadcast;
	extern char *g_mentality_upgrade_max;
	extern char *g_mentality_combine_upgrade;
	extern char *g_mentality_combine_learn_skill;

	extern char *g_guild_open_guild_fb;
	extern char *g_guild_box_open_content;

	extern char *g_kill_common_scene_boss;
	extern char *g_kill_fb_boss;
	extern char *g_refresh_boss;
	extern char *g_pick_body_drop;
	extern char *g_pick_body_drop_equipment;

	extern char *g_kill_world_boss;
	extern char *g_worldboss_person_hurt_reward;
	extern char *g_worldboss_guild_hurt_reward;
	extern char *g_worldboss_killer_guild_reward;
	extern char *g_worldboss_week_rank_reward;
	extern char *g_worldboss_red_paper_reward;
	extern char *g_worldboss_hudun_reward;
	extern char *g_worldboss_hudun_reward_broadcast;
	extern char *g_worldboss_refresh;
	extern char *g_cross_boss_enter_notice;
	extern char *g_cross_boss_enter2_notice;
	extern char *g_cross_boss_enter3_notice;
	extern char *g_dabao_boss_refresh_notice;
	extern char *g_precious_boss_refresh;
	extern char *g_active_boss_rank_reward_mail_context;

	extern char *g_common_scene_kill_role;

	extern char *g_item_equip_shenzhu_total;
	extern char *g_item_equip_shenzhu_up_shen_level;
	extern char *g_item_equip_upstar_total;
	extern char *g_item_equip_up_star;
	extern char *g_item_equip_upquality;
	extern char *g_item_equip_upquality_1;
	extern char *g_item_equip_upquality_2;
	extern char *g_item_equip_upquality_3;
	extern char *g_item_equip_upquality_4;
	extern char *g_item_equip_upquality_5;
	extern char *g_item_equip_upquality_6;
	extern char *g_item_equip_strengthen;
	extern char *g_item_equip_total_strengthen;
	extern char *g_item_equip_compound_1;
	extern char *g_item_equip_compound_2;
	extern char *g_item_equip_uplevel;
	extern char *g_item_pick;
	extern char *g_item_pick_equipment;
	extern char *g_item_get_equipment_send_to_guild;
	extern char *g_item_invalid_subiect;
	extern char *g_item_invalid_content;
	extern char *g_item_stone_up_level;
	extern char *g_item_stone_up_level_single_part;
	extern char *g_item_stone_up_level_consume;
	extern char *g_item_stone_up_level_consume_has_diamond;
	extern char *g_item_eternity_up_level;
	extern char *g_item_eternity_up_level_1;
	extern char *g_item_eternity_suit;

	extern char *g_wing_upgrade;
	extern char *g_wing_active_content;
	extern char *g_xiannv_active_content;
	extern char *g_xiannv_shengwu_miling;
	extern char *g_xiannv_shengwu_upgrade;
	extern char *g_xiannv_shengwu_skill_upgrade;
	extern char *g_xiannv_gongming_upgrade;
	extern char *g_halo_upgrade;
	extern char *g_shengong_upgrade;
	extern char *g_shenyi_upgrade;
	extern char *g_footprint_upgrade;
	extern char *g_img_fuling_skill_level_up;
	extern char *g_img_fuling_level_up;

	extern char *g_cloak_up_level;
	extern char *g_cloak_imge;
	extern char *g_fight_mount_upgrade;
	extern char *g_shenbing_level_up;
	extern char *g_shenbing_upgrade;

	extern char *g_role_goal_reward_content;

	extern char *vip_uplevel;
	extern char *vip_uplevel_personal;
	extern char *g_gold_vip_active_content;
	extern char *g_time_limit_vip_active_content;

	extern char *g_knapsack_put_subiect;
	extern char *g_knapsack_put_content;

	extern char *g_zhuanzhi;
	extern char *g_change_prof;

	extern char *g_shizhuang_upgrade;
	extern char *g_fabao_upgrade;

	extern char *g_mount_upgrade;
	extern char *g_mount_fly_up;
	extern char *g_mount_active_content;

	extern char *g_mojie_uplevel;

	extern char *g_money_chongzhi_subject;
	extern char *g_money_chongzhi_content;

	extern char *g_bekilled_notice_subject;
	extern char *g_bekilled_notice_content;
	extern char *g_evil_bekilled_notice_content;
	extern char *g_flower_notice;
	extern char *g_flower_notice_anonymity;
	extern char *g_team_create_notice;

	extern char *g_yaojiang_notice;

	extern char *g_open_randgift[OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM];

	extern char *g_succnotice_mentility_lift;
	extern char *g_errornotice_enter_scene_limit_level;
	extern char *g_errornotice_enter_fb_limit_level;
	extern char *g_errornotice_lack_item;

	extern char *g_role_rename_notice;
	extern char *g_role_rename_subject;
	extern char *g_role_rename_content;

	extern char *g_role_set_sex_notice;
	extern char *g_role_set_sex_subject;
	extern char *g_role_set_sex_content;

	extern char *g_famous_man_type_1;
	extern char *g_famous_man_type_2;
	extern char *g_famous_man_type_3;
	extern char *g_famous_man_type_4;
	extern char *g_famous_man_type_5;
	extern char *g_famous_man_type_6;

	extern char *g_husong_accept_notice;
	extern char *g_husong_rob_notice;
	extern char *g_husong_berob_mail_subject;
	extern char *g_husong_berob_mail_content;
	extern char *g_husong_luckydraw_mail_bindcoin_content;
	extern char *g_husong_rob_mail_subject;
	extern char *g_husong_rob_mail_content;
	extern char *g_husong_rob_mail_level_limit_content;
	extern char *g_husong_rob_mail_times_limit_content;
	extern char *g_husong_overtime_mail_subject;
	extern char *g_husong_overtime_mail_content;
	extern char *g_husong_refresh_comsume;

	extern char *g_equipfb_compensation_roll_subject;
	extern char *g_equipfb_compensation_roll_context;
	extern char *g_new_equip_fb_pass;
	extern char *g_runetower_first_reward;
	extern char *g_equipfb_jump_subject;
	extern char *g_equipfb_personal_jump_content;
	extern char *g_equipfb_personal_notice;
	extern char *g_equipfb_team_notice;

	extern char *g_zhuanzhifb_teammate_can_not_enter;

	extern char *g_pvpzhanchang_lianzhan_notice1;
	extern char *g_pvpzhanchang_lianzhan_notice2;
	extern char *g_pvpzhanchang_lianzhan_notice3;
	extern char *g_pvpzhanchang_lianzhan_notice4;
	extern char *g_pvpzhanchang_lianzhan_notice5;
	extern char *g_pvpzhanchang_lianzhan_notice6;
	extern char *g_pvpzhanchang_lianzhan_notice7;
	extern char *g_pvpzhanchang_lianzhan_notice8;
	extern char *g_pvpzhanchang_lianzhan_notice;
	extern char *g_pvpzhanchang_stoplianzhan_notice;

	extern char *g_qunxian_luandou_weak_side_protect_notice;
	extern char *g_qunxian_luandou_shenshi_disappear;

	extern char *g_worldactivity_trigger_subject;
	extern char *g_worldactivity_trigger_content;

	extern char *g_xiannvcifu_trigger_subject;
	extern char *g_xiannvcifu_trigger_content;
	extern char *g_xiannvcifu_joiner_subject;
	extern char *g_xiannvcifu_joiner_content;

	extern char *g_yaowanglaixi_alluser_mail_subject;
	extern char *g_yaowanglaixi_alluser_mail_content;
	extern char *g_yaowanglaixi_progress_gift_mail_subject;
	extern char *g_yaowanglaixi_progress_gift_mail_content;

	extern char *g_gongchengzhan_door_die_atk_side_notice;
	extern char *g_gongchengzhan_door_die_def_side_notice;
	extern char *g_gongchengzhan_finish_occupy_notice;
	extern char *g_gongchengzhan_finish_not_occupy_notice;
	extern char *g_gongchengzhan_def_win_subiect;
	extern char *g_gongchengzhan_def_win_content;
	extern char *g_gongchengzhan_def_lose_subiect;
	extern char *g_gongchengzhan_def_lose_content;
	extern char *g_gcz_cz_win_subiect;
	extern char *g_gcz_cz_win_content;
	extern char *g_gcz_rankreward_subiect;
	extern char *g_gcz_rankreward_content;
	extern char *g_gcz_rankreward_fail_content;
	extern char *g_gongchengzhan_atk_door_notice;
	extern char *g_gongchengzhan_atk_flag_notice;
	extern char *g_gongchengzhan_enter_tower_error;
	extern char *g_gongchengzhan_def_win_change_notice;
	extern char *g_shenbing_reward_subject;
	extern char *g_shenbing_reward_content;
	extern char *g_shenbing_give_reward_notice;
	extern char *g_shenbing_no_reward_notice;
	extern char *g_gongchengzhan_relive_cd_left;
	extern char *g_fail_reward_content;
	extern char *g_gcz_pomen_shou_notice;
	extern char *g_gcz_pomen_gong_notice;
	extern char *g_gcz_zhanqi_notice;

	extern char *g_guildbattle_close;
	extern char *g_guildbattle_rankreward_guild;
	extern char *g_guildbattle_collective_reward;
	extern char *g_guildbattle_rankreward_person;
	extern char *g_guildbattle_rankreward_open_server;
	extern char *g_guildbattle_monster_refresh_prepare_notice;
	extern char *g_guildbattle_monster_refresh_notice;
	extern char *g_guildbattle_kill_boss_notice;
	extern char *g_guildbattle_get_glod_box_notice;
	extern char *g_guildbattle_commit_glod_box_notice;
	extern char *g_guildbattle_first_guild;
	extern char *g_guildbattle_best_killer;
	extern char *g_guildbattle_fetch_target_reward;
	extern char *g_guildbattle_point_boss_blood_notice;
	extern char *g_guildbattle_server_reward_content;
	extern char *g_guildbattle_guild_reward_content;
	extern char *g_guildbattle_team_reward_content;
	extern char *g_guildbattle_on_attack_warning;
	extern char *g_guildbattle_on_flag_hp_warning;

	extern char *g_territorywar_kill_role;
	extern char *g_territorywar_result;
	extern char *g_territorywar_rankreward_open_server;
	extern char *g_territorywar_monster_build_be_attack;
	extern char *g_territorywar_monster_build_be_kill;
	extern char *g_territorywar_center_build_be_occupy_my_side;
	extern char *g_territorywar_center_build_be_occupy_other_side;

	extern char *g_xianmengzhan_reward_subject;
	extern char *g_xianmengzhan_reward_content;
	extern char *g_xianmengzhan_occupy_center_area_notice;
	extern char *g_xianmengzhan_occupy_defend_area_notice;
	extern char *g_xianmengzhan_occupy_guild_defend_area_notice;
	extern char *g_xianmengzhan_call_danyao_boss_notice;
	extern char *g_xianmengzhan_use_danyao_notice;
	extern char *g_xianmengzhan_kill_notice;
	extern char *g_xianmengzhan_standy;
	extern char *g_xianmengzhan_start;
	extern char *g_xianmengzhan_close;
	extern char *g_xianmengzhan_join_reward_subject;
	extern char *g_xianmengzhan_join_reward_content;
	extern char *g_xianmengzhan_relive_cd_left;
	extern char *g_xianmengzhan_lost_defend_area;
	extern char *g_xianmengzhan_guild_in_rank_context;
	extern char *g_xianmengzhan_guild_not_in_rank_context;

	extern char *g_nationalboss_reward_subject;
	extern char *g_nationalboss_reward_content_top10;
	extern char *g_nationalboss_reward_content_other;
	extern char *g_kill_national_boss;
	extern char *g_nationalboss_task_reward_subject;
	extern char *g_nationalboss_task_reward_content;
	extern char *g_activity_national_boss_end;

	extern char *g_oscc_all_role_subject;
	extern char *g_oscc_all_role_content;
	extern char *g_oscc_day_1_subject;
	extern char *g_oscc_day_1_content;
	extern char *g_oscc_day_2_subject;
	extern char *g_oscc_day_2_content;
	extern char *g_oscc_day_3_subject;
	extern char *g_oscc_day_3_content;
	extern char *g_oscc_day_4_subject;
	extern char *g_oscc_day_4_content;
	extern char *g_oscc_day_5_subject;
	extern char *g_oscc_day_5_content;
	extern char *g_oscc_day_6_subject;
	extern char *g_oscc_day_6_content;
	extern char *g_oscc_day_7_subject;
	extern char *g_oscc_day_7_content;
	extern char *g_oscc_day_8_subject;
	extern char *g_oscc_day_8_content;

	extern char *g_csa_all_role_subject;
	extern char *g_csa_all_role_content;
	extern char *g_csa_day_chongzhi_reward_subject;
	extern char *g_csa_day_chongzhi_reward_content;
	extern char *g_csa_day_chongzhi_notice;

	extern char *g_zhuxie_boss_flush_notice;
	extern char *g_zhuxie_kill_boss_notice;
	extern char *g_zhuxie_task_reward_subject;
	extern char *g_zhuxie_task_reward_content;
	extern char *g_zhuxie_rob_task_notice;

	extern char *g_dur_login_lottory_notice;

	extern char *g_open_game_day_chongzhi_reward_subject;
	extern char *g_open_game_day_chongzhi_reward_content;
	extern char *g_open_game_open_game_exchange_notice;
	extern char *g_open_game_day_chongzhi_notice;

	extern char *g_open_game_activity_capability_rank_reward_subject;
	extern char *g_open_game_activity_capability_rank_reward_content;
	extern char *g_open_game_activity_guild_level_rank_reward_content;
	extern char *g_open_game_activity_guild_world_boss_rank_reward_content;

	extern char *g_turntable_great_gift;
	extern char *g_turntable_millionaire;

	extern char *g_cscz_atk_notice;
	extern char *g_cscz_kill_notice;

	extern char *g_shop_buy_notice;

	extern char *g_xianbang_finish_all_task_subject;
	extern char *g_xianbang_finish_all_task_content;

	extern char *g_common_activity_flush_monster_notice;
	extern char *g_common_activity_flush_hongbao_notice;
	extern char *g_common_activity_huanle_dengmi_notice;
	extern char *g_common_activity_chongzhi_fanli_subject;
	extern char *g_common_activity_chongzhi_fanli_content;
	extern char *g_common_activity_use_yurencard_notice;
	extern char *g_common_activity_role_shadow_flush_notice;
	extern char *g_common_activity_role_shadow_flush_notice2;
	extern char *g_common_activity_role_shadow_end_notice;

	extern char *g_mount_bless_low;
	extern char *g_mount_bless_high;
	extern char *g_mount_bless_super;
	extern char *g_qibing_upgrade;

	extern char *g_strength_bless_low;
	extern char *g_strength_bless_high;
	extern char *g_strength_bless_super;

	extern char *g_card_bless_low;
	extern char *g_card_bless_high;
	extern char *g_card_bless_super;

	extern char *g_shenjiao_bless_low;
	extern char *g_shenjiao_bless_high;
	extern char *g_shenjiao_bless_super;

	extern char *g_chestshop_bless_low;
	extern char *g_chestshop_bless_high;
	extern char *g_chestshop_bless_super;

	extern char *g_petgrow_bless_low;
	extern char *g_petgrow_bless_high;
	extern char *g_petgrow_bless_super;

	extern char *g_mystery_bless_low;
	extern char *g_mystery_bless_high;
	extern char *g_mystery_bless_super;

	extern char *g_micropc_download_reward;
	extern char *g_micropc_login_reward;

	extern char *g_marry_reward_suject;
	extern char *g_marry_reward_content;

	extern char *g_combine_qingdian_day_chongzhi_reward_subject;
	extern char *g_combine_qingdian_day_chongzhi_reward_content;
	extern char *g_combine_qingdian_wczb_owner_reward_subject;
	extern char *g_combine_qingdian_wczb_owner_reward_content;
	extern char *g_combine_qingdian_wczb_camp_reward_subject;
	extern char *g_combine_qingdian_wczb_camp_reward_content;
	extern char *g_combine_qingdian_fanli_subject;
	extern char *g_combine_qingdian_fanli_content;
	extern char *g_csqd_day_consume_reward_subject;
	extern char *g_csqd_day_consume_reward_content;
	extern char *g_csqd_lucky_roll_notice;
	extern char *g_csqd_wczb_win_notice;

	extern char *g_daily_task_finish_subject;
	extern char *g_daily_task_finish_content;

	extern char *g_paohuan_task_phase_subject;
	extern char *g_paohuan_task_phase_content;
	extern char *g_paohuan_task_finish_notice;

	extern char *g_msg_guild_welcome1;
	extern char *g_msg_guild_welcome2;
	extern char *g_msg_guild_welcome3;
	extern char *g_msg_guild_welcome4;
	extern char *g_msg_guild_welcome5;
	extern char *g_msg_guild_welcome6;
	extern char *g_msg_guild_welcome7;
	extern char *g_msg_guild_welcome8;

	extern char *g_guild_sos_husong;

	extern char *g_guild_boss_create;
	extern char *g_guild_supper_boss_create;
	extern char *g_guild_call_in;
	extern char *g_guild_storage_put_item;
	extern char *g_guild_storage_take_item;
	extern char *g_guild_memberkilled;
	extern char *g_guild_boss_red_bag;
	extern char *g_guild_pao_saizi;
	extern char *g_guild_pao_saizi_rank3;
	extern char *g_guild_pao_saizi_rank2;
	extern char *g_guild_pao_saizi_rank1;
	extern char *g_guild_saizi_rank_msg1;
	extern char *g_guild_saizi_rank_msg2;
	extern char *g_guild_saizi_reward_mail;
	extern char *g_guild_maze_rank_reward;

	extern char *g_guild_flag_first_be_hit_notice;
	extern char *g_guild_flag_be_attacked_notice;
	extern char *g_guild_flag_be_occupied_notice;
	extern char *g_guild_officer_be_killed;
	extern char *g_guild_chengyuan_be_killed;
	extern char *g_guild_box_invite_assist;

	extern char *g_qingyuan_rank_top3;
	extern char *g_add_capability_rank_top3;
	extern char *g_add_charm_rank_top3;
	extern char *g_guild_activity_team_reward_notice;
	///////////////////////////////////////	 封测活动  /////////////////////////////////////////
	extern char *g_close_beta_activity_gold_reward_subject;
	extern char *g_close_beta_activity_gold_reward_content;

	///////////////////////////////////////	 通用活动  /////////////////////////////////////////
	extern char *g_common_activity_equip_strength_reward_subject;
	extern char *g_common_activity_equip_strength_reward_content;

	///////////////////////////////////////	群仙乱斗  /////////////////////////////////////////
	extern char *g_qunxianluandou_score_reward_subject;
	extern char *g_qunxianluandou_score_reward_content;
	extern char *g_qunxianluandou_side_win_subject;
	extern char *g_qunxianluandou_side_win_content;
	extern char *g_qunxianluandou_side_lose_subject;
	extern char *g_qunxianluandou_side_lose_content;
	extern char *g_qunxianluandou_qunxian_open_server;
	extern char *g_qunxianluandou_first_side;

	///////////////////////////////////// 怪物入侵 ////////////////////////////////////////
	extern char *g_monster_invade_refresh;

	///////////////////////////////////// 大富豪 ////////////////////////////////////////
	extern char *g_millionaire_boss_refresh;
	extern char *g_millionaire_frozen;
	extern char *g_millionaire_gather_times;
	extern char *g_millionaire_gather_good;
	extern char *g_millionaire_kill_boss;

	///////////////////////////////////// 走棋子 ////////////////////////////////////////
	extern char *g_movechess_good_item;

	///////////////////////////////////// 答题 ////////////////////////////////////////
	extern char *g_question_rank_broadcast;
	extern char *g_question_rank_reward;
	extern char *g_question_fast_world_reward;
	extern char *g_question_fast_guild_reward;
	extern char *g_question_fast_world_reward_mail;
	extern char *g_question_fast_guild_reward_mail;
	extern char *g_question_guild_rank_reward;
	extern char *g_question_guild_rank_other_reward;
	extern char *g_question_system_message_world;
	extern char *g_question_system_message_guild;

	///////////////////////////////////////	 月卡  /////////////////////////////////////////
	extern char *g_month_card_reward_per_day_subject;
	extern char *g_month_card_reward_per_day_content;
	extern char *g_month_card_reward_on_buy_subject;
	extern char *g_month_card_reward_on_buy_content;
	extern char *g_month_card_buy_notice;

	///////////////////////////////////////	 排名竞技场  /////////////////////////////////////////
	extern char *g_challenge_field_lucky_reward_subject;
	extern char *g_challenge_field_lucky_reward_content;
	extern char *g_challenge_field_rank_reward_subject;
	extern char *g_challenge_field_rank_reward_content;
	extern char *g_challenge_field_rank_drop_subject;
	extern char *g_challenge_field_rank_drop_content;
	extern char *g_challenge_field_rank_pos_notice;
	extern char *g_challenge_field_rank_pos_notice1;
	extern char *g_challenge_field_rank_pos_notice2;
	extern char *g_challenge_field_rank_pos_notice3;
	extern char *g_challenge_field_close_rank_pos_notice1;
	extern char *g_challenge_field_close_rank_pos_notice2;
	extern char *g_challenge_field_close_rank_pos_notice3;
	extern char *g_challenge_field_rank_pos_first_changed;
	extern char *g_challenge_field_liansheng_notice;
	extern char *g_challenge_field_best_rank_reward_content;
	extern char *g_fighting_challenge_rank_rward_content;
	extern char *g_challenge_field_rank_close_reward_content;

	///////////////////////////////////////	 充值  /////////////////////////////////////////
	extern char *g_chongzhi_reward_subject;
	extern char *g_chongzhi_reward_content;
	extern char *g_chongzhi_reward_coin_content;
	extern char *g_chongzhi_special_first_recharge_reward_content;
	extern char *g_chongzhi_daily_first_recharge_reward_content;
	extern char *g_chongzhi_daily_total_recharge_reward_content;
	extern char *g_chongzhi_daily_total_recharge_send_to_world;
	extern char *g_chongzhi_first_chongzhi_reward_content;
	extern char *g_chongzhi_zai_chongzhi_reward_content;
	extern char *g_chongzhi_san_chongzhi_reward_content;
	extern char *g_chongzhi_total_chongzhi_reward_content;
	extern char *g_chongzhi_danbi_chongzhi_reward_content;
	extern char *g_chongzhi_diff_weeekday_chongzhi_reward_content;
	extern char *g_chongzhi_daily_recharge_reward2_content;
	extern char *g_touzi_level;
	extern char *g_touzi_week_card;
	extern char *g_touzi_fb_plan;
	extern char *g_touzi_boss_plan;
	extern char *g_touzi_shenyu_boss_plan;
	extern char *g_zero_gift;
	extern char *g_discount_buy;
	extern char *g_discount_buy_mail_content;
	extern char *g_time_limit_gift;
	extern char *g_time_limit_gift_2;
	extern char *g_panic_buy;

	///////////////////////////////////////	 阵营  /////////////////////////////////////////
	extern char* g_camp_gongxun_rank_content;
	extern char* g_camp_gongxun_value_content;
	extern char* g_campdefend_sf_reward_subject;
	extern char* g_campdefend_sf_reward_content;
	extern char* g_campdefend_gf_reward_subject;
	extern char* g_campdefend_gf_reward_content;
	extern char* g_campdefend_diaoxiang_bekilled;

	///////////////////////////////////////	 组队副本  /////////////////////////////////////////
	extern char *g_team_fb_open_reward_content;
	extern char *g_team_fb_assist_reward_content;
	extern char *g_team_fb_teammate_assist_times_limit;
	extern char *g_team_fb_teammate_level_limit;
	extern char *g_team_fb_teammate_can_not_enter;
	extern char *g_equipfb_team_jump_subject;
	extern char *g_equipfb_team_jump_content;
	extern char *g_team_equip_fb_pass_notice;

	///////////////////////////////////////	 称号管理  /////////////////////////////////////////
	extern char *g_title_get_title;
	extern char *g_title_lost_title;
	extern char *g_title_get_pata_title_notice;

	///////////////////////////////////////	 称号管理  /////////////////////////////////////////
	extern char *g_jilian_start_reward_subject;
	extern char *g_jilian_start_reward_content;

	///////////////////////////////////////  仙盟任务  //////////////////////////////////////////
	extern char *g_guild_task_complete_all_reward_subject;
	extern char *g_guild_task_complete_all_reward_content;

	///////////////////////////////////////  刺杀任务  //////////////////////////////////////////
	extern char *g_cisha_task_complete_reward_subject;
	extern char *g_cisha_task_complete_reward_content;

	///////////////////////////////////////  情缘  //////////////////////////////////////////
	extern char *g_qingyuan_marry_reward_content;
	extern char *g_marry_success_notice;
	extern char *g_marry_fb_can_not_enter;
	extern char *g_qingyuan_equip_uplevel_notice;
	extern char *g_qingyuan_equip_upquality_notice_1;
	extern char *g_qingyuan_equip_upquality_notice_2;
	extern char *g_qingyuan_divorce_mail_subject;
	extern char *g_qingyuan_divorce_mail_content;
	extern char *g_qingyuan_divorce_notice;
	extern char *g_hunyan_hongbao_notice;
	extern char *g_hunyan_xianmeng_invite_notice;
	extern char *g_hunyan_open_hunyan_notice;
	extern char *g_hunyan_open_person_yanhui_notice;
	extern char *g_hunyan_zhufu_notice;
	extern char *g_marry_seeking_notice;
	extern char *g_qingyuan_active_card_notice;
	extern char *g_hunyan_zhufu_reward_notice;
	extern char *g_hunyan_subject_notice;
	extern char *g_qingyuan_tuodan1;
	extern char *g_qingyuan_tuodan2;
	extern char *g_hunyan_reward_notice;
	extern char *g_lovecontract_finish;
	extern char *g_marry_zhufu1;
	extern char *g_marry_zhufu2;
	extern char *g_marry_zhufu3;
	extern char *g_marry_zhufu4;
	extern char *g_marry_zhufu5;
	extern char *g_marry_zhufu6;
	extern char *g_marry_zhufu7;
	extern char *g_marry_songhua;
	extern char *g_hunyan_zhufu1;
	extern char *g_hunyan_zhufu2;
	extern char *g_hunyan_zhufu3;
	extern char *g_hunyan_sahua;

	/////////////////////////////////////// 攻城战 //////////////////////////////////////////
	extern char *g_zhangong_reward_content;
	extern char *g_win_huiyuan_reward_content;
	extern char *g_win_huizhang_reward_content;
	extern char *g_gongchengzhan_team_reward_content;
	extern char *g_last_chenzhu_reward_content;
	extern char *g_gcz_rankreward_open_server;

	/////////////////////////////////////// 攻城战 //////////////////////////////////////////
	extern char *g_mojie_jinglingpian_reward_notice;

	///////////////////////////////////////  抽奖  //////////////////////////////////////////
	extern char *g_luck_roll_reward_item_notice;
	extern char *g_luck_roll_reward_gold_notice;

	///////////////////////////////////////  天降财宝 //////////////////////////////////////////
	extern char *g_tianjiangcaibao_kill_boss_moster_notice;
	extern char *g_tianjiangcaibao_reflush_qianduoduo_notice;
	extern char *g_tianjiangcaibao_reflush_bigqian_notice;
	extern char *g_tianjiangcaibao_already_reflush_bigqian_notice;
	extern char *g_tianjiangcaibao_already_reflush_qianduoduo_notice;
	extern char *g_tianjiangcaibao_goblin_reflush_notice;
	extern char *g_tianjiangcaibao_kill_goblin;
	extern char *g_tianjiangcaibao_finish_task;

	///////////////////////////////////////  王陵探险  //////////////////////////////////////////
	extern char *g_wanglingexplore_reflush_boss_notice;
	extern char *g_wanglingexplore_already_reflush_monster_notice;
	extern char *g_wanglingexplore_wangling_boss_come;
	extern char *g_wanglingexplore_kill_boss;
	extern char *g_wanglingexplore_finish_task;
	extern char *g_wanglingexplore_finish_task_getgift;
	///////////////////////////////////////  星座遗迹 /////////////////////////////////////////
	extern char *g_xingzuoyiji_refresh_notice;
	extern char *g_xingzuoyiji_box_refresh_notice;
	extern char *g_xingzuoyiji_normal_end_notice;
	extern char *g_xingzuoyiji_good_end_notice;
	extern char *g_xingzuoyiji_gather_common_box;
	extern char *g_xingzuoyiji_gather_molong_box;

	///////////////////////////////////////  温泉答题 /////////////////////////////////////////
	extern char *g_hotspring_use_snow;
	extern char *g_hotspring_use_massage;
	extern char *g_hotspring_rank_first_player;
	extern char *g_hotspring_find_lover;

	//////////////////////////////////////  随机活动  ////////////////////////////////////////
	extern char *g_rand_activity_consume_gold_fanli_reward_content;
	extern char *g_rand_activity_strengthen_reward_content;
	extern char *g_rand_activity_chongzhi_rank_reward_content;
	extern char *g_rand_activity_chongzhi_rank_reward_content_reserve;
	extern char *g_rand_activity_chongzhi_rank2_reward_content;
	extern char *g_rand_activity_chongzhi_rank2_reward_content_reserve;
	extern char *g_rand_activity_consume_gold_rank_reward_content;
	extern char *g_rand_activity_consume_gold_rank_reward_content_reserve;
	extern char *g_rand_activity_day_chongzhi_fanli_reward_content;
	extern char *g_rand_activity_day_consume_gold_reward_content;
	extern char *g_rand_activity_chestshop_reward_content;
	extern char *g_rand_activity_total_consume_gold_reward_content;
	extern char *g_rand_activity_day_active_degree_reward_content;
	extern char *g_rand_activity_stone_total_level_reward_content;
	extern char *g_rand_activity_xiannv_chanmian_uplevel_reward_content;
	extern char *g_rand_activity_mount_upgrade_reward_content;
	extern char *g_rand_activity_qibing_upgrade_reward_content;
	extern char *g_rand_activity_mentality_total_level_reward_content;
	extern char *g_rand_activity_wing_upgrade_reward_content;
	extern char *g_rand_activity_kill_boss_reward_content;
	extern char *g_rand_activity_day_online_gift_reward_content;
	extern char *g_rand_activity_day_online_gift_reward_notice;
	extern char *g_rand_activity_quanmin_qifu_reward_content;
	extern char *g_rand_activity_xianmeng_bipin_reward_content;
	extern char *g_rand_activity_xianmeng_jueqi_reward_content;
	extern char *g_rand_activity_login_gift_reward_content;
	extern char *g_rand_activity_bipin_rank_reward_content;
	extern char *g_rand_activity_bipin_rank_reward_notice;
	extern char *g_rand_activity_bipin_all_reward_content;
	extern char *g_rand_activity_bipin_topuser_change_notice;
	extern char *g_rand_activity_bipin_topuser_change_content;
	extern char *g_rand_activity_bipin_day_change_mail_content;
	extern char *g_rand_activity_single_charge_reward_content;
	extern char *g_rand_activity_single_charge_reward_content2;
	extern char *g_rand_activity_single_charge_reward_content3;
	extern char *g_rand_activity_single_charge_reward_content4;
	extern char *g_rand_activity_single_charge_reward_content5;
	extern char *g_rand_activity_single_charge_reward_content_multi;
	extern char *g_rand_activity_double_get_reward_content;
	extern char *g_rand_activity_cornucopia_reward_content;
	extern char *g_rand_activity_new_cornucopia_reward_content;
	extern char *g_rand_activity_double_charge_content;
	extern char *g_rand_activity_total_charge_day_reward_content;
	extern char *g_rand_activity_tomorrow_reward_content;
	extern char *g_rand_activity_day_rank_chongzhi_reweard_content;
	extern char *g_rand_activity_day_rank_chongzhi_reweard_content_reserve;
	extern char *g_rand_activity_day_rank_consume_reweard_content;
	extern char *g_rand_activity_total_charge_bu_reward_content;
	extern char *g_rand_activity_total_charge_reward_chuanwen_content;
	extern char *g_rand_activity_new_total_charge_reward_chuanwen_content;
	extern char *g_rand_activity_new_total_charge_reward_chuanwen_content1;
	extern char *g_rand_activity_new_total_charge_reward_chuanwen_content2;
	extern char *g_rand_activity_new_total_charge_reward_chuanwen_content3;
	extern char *g_rand_activity_level_lottery_reward_content;
	extern char *g_rand_activity_chongzhi_niuegg_reward_content;
	extern char *g_rand_activity_zhenbaoge_zhenxi_reward_content;
	extern char *g_rand_activity_xianyuan_treas_reward_content;
	extern char *g_rand_activity_xianyuan_treas_all_buy_content;
	extern char *g_rand_activity_mijingxunbao_zhenxi_reward_content;
	extern char *g_rand_activity_jisuchongzhan_reward_content;
	extern char *g_rand_activity_money_tree_reward_content_gold;
	extern char *g_rand_activity_money_tree_reward_content_item;
	extern char *g_rand_activity_daily_love_reward_content;
	extern char *g_rand_activity_king_draw_reward_content;
	extern char *g_rand_activity_king_draw_return_reward_content;
	extern char *g_rand_activity_total_charge3_reward_content;
	extern char *g_rand_activity_appearace_passive_rank_reward_content;
	extern char *g_rand_activity_appearace_passive_rank_join_reward_content;
	extern char *g_rand_activity_appearace_rank_reward_content;
	extern char *g_rand_activity_appearace_rank_join_reward_content;
	extern char *g_rand_activity_total_charge4_reward_content;
	extern char *g_rand_activity_mine_server_reward_content;
	extern char *g_rand_activity_mine_role_refresh_content;
	extern char *g_rand_activity_mine_exchange_reward_content;
	extern char *g_rand_activity_tian_ming_divination_reward_content;
	extern char *g_rand_activity_tian_ming_divination_rare_reward;
	extern char *g_rand_activity_fanfan_reward_item_content;
	extern char *g_rand_activity_fanfan_exchange_word_reward_content;
	extern char *g_rand_activity_continue_chongzhi_reward_content;
	extern char *g_rand_activity_continue_consume_reward_content;
	extern char *g_rand_activity_army_day_win_army_reward_content;
	extern char *g_rand_activity_circulation_chongzhi_reward_content;
	extern char *g_rand_activity_extreme_lucky_reward_content;
	extern char *g_rand_activity_extreme_lucky_reward_return_content;
	extern char *g_rand_activity_mijingxunbao2_zhenxi_reward_content;
	extern char *g_rand_activity_money_tree2_reward_content_gold;
	extern char *g_rand_activity_money_tree2_reward_content_item;
	extern char *g_rand_activity_zhenbaoge2_zhenxi_reward_content;
	extern char *g_rand_activity_promoting_position_reward_item_content;
	extern char *g_rand_activity_promoting_position_reward_gold_content;
	extern char *g_rand_activity_black_market_give_item_content;
	extern char *g_rand_activity_black_market_return_gold_content;
	extern char *g_rand_activity_black_market_final_get_reward;
	extern char *g_rand_activity_role_uplevel_content;
	extern char *g_rand_activity_pata_content;
	extern char *g_rand_activity_exp_fb_content;
	extern char *g_rand_activity_upgrade_mount_content;
	extern char *g_rand_activity_upgrade_halo_content;
	extern char *g_rand_activity_upgrade_wing_content;
	extern char *g_rand_activity_upgrade_shengong_content;
	extern char *g_rand_activity_upgrade_shenyi_content;
	extern char *g_rand_activity_first_charge_tuan_content;
	extern char *g_rand_activity_day_total_charge_content;
	extern char *g_rand_activity_upgrade_mount_total_content;
	extern char *g_rand_activity_upgrade_halo_total_content;
	extern char *g_rand_activity_upgrade_wing_total_content;
	extern char *g_rand_activity_upgrade_shengong_total_content;
	extern char *g_rand_activity_upgrade_shenyi_total_content;
	extern char *g_rand_activity_equip_strength_level_total_content;
	extern char *g_rand_activity_stone_level_total_content;
	extern char *g_rand_activity_upgrade_mount_rank_content;
	extern char *g_rand_activity_upgrade_halo_rank_content;
	extern char *g_rand_activity_upgrade_wing_rank_content;
	extern char *g_rand_activity_upgrade_shengong_rank_content;
	extern char *g_rand_activity_upgrade_shenyi_rank_content;
	extern char *g_rand_activity_upgrade_qilinbi_rank_content;
	extern char *g_rand_activity_upgrade_toushi_rank_content;
	extern char *g_rand_activity_upgrade_yaoshi_rank_content;
	extern char *g_rand_activity_upgrade_shizhuang_rank_content;
	extern char *g_rand_activity_upgrade_shenbing_rank_content;
	extern char *g_rand_activity_upgrade_lingtong_rank_content;
	extern char *g_rand_activity_upgrade_linggong_rank_content;
	extern char *g_rand_activity_upgrade_lingqi_rank_content;
	extern char *g_rand_activity_upgrade_fabao_rank_content;
	extern char *g_rand_activity_upgrade_footprint_rank_content;
	extern char *g_rand_activity_upgrade_flypet_rank_content;
	extern char *g_rand_activity_upgrade_weiyan_rank_content;
	extern char *g_rand_activity_equip_strength_level_rank_content;
	extern char *g_rand_activity_stone_total_level_rank_content;
	extern char *g_rand_activity_xiannv_cap_rank_content;
	extern char *g_rand_activity_jingling_cap_rank_content;
	extern char *g_rand_activity_upgrade_fight_mount_rank_content;
	extern char *g_rand_activity_total_cap_rank_content;
	extern char *g_rand_activity_boss_killer_total_content;
	extern char *g_rand_activity_marry_me;
	extern char *g_rand_activity_bipin_two;
	extern char *g_rand_activity_bipin;
	extern char *g_rand_activity_mount_rank_unclaimed_content;
	extern char *g_rand_activity_wing_rank_unclaimed_content;
	extern char *g_rand_activity_shizhuang_rank_unclaimed_content;
	extern char *g_rand_activity_shenbing_rank_unclaimed_content;
	extern char *g_rand_activity_fabao_rank_unclaimed_content;
	extern char *g_rand_activity_footprint_rank_unclaimed_content;
	extern char *g_rand_activity_halo_rank_unclaimed_content;
	extern char *g_rand_activity_shengong_rank_unclaimed_content;
	extern char *g_rand_activity_shenyi_rank_unclaimed_content;
	extern char *g_rand_activity_qilinbi_rank_unclaimed_content;
	extern char *g_rand_activity_toushi_rank_unclaimed_content;
	extern char *g_rand_activity_yaoshi_rank_unclaimed_content;
	extern char *g_rand_activity_lingtong_rank_unclaimed_content;
	extern char *g_rand_activity_linggong_rank_unclaimed_content;
	extern char *g_rand_activity_lingqi_rank_unclaimed_content;
	extern char *g_rand_activity_stone_rank_unclaimed_content;
	extern char *g_rand_activity_equip_rank_unclaimed_content;
	extern char *g_rand_activity_xiannv_cap_rank_unclaimed_content;
	extern char *g_rand_activity_jingling_cap_rank_unclaimed_content;
	extern char *g_rand_activity_upgrade_fight_mount_rank_unclaimed_content;
	extern char *g_rand_activity_upgrade_flypet_rank_unclaimed_content;
	extern char *g_rand_activity_upgrade_weiyan_rank_unclaimed_content;
	extern char *g_rand_activity_total_cap_rank_unclaimed_content;

	extern char *g_rand_activity_summon_golden_pig;
	extern char *g_rand_activity_hurt_golden_pig;
	extern char *g_rand_activity_kill_golden_pig;
	extern char *g_rand_activity_summon_golden_pig_reward_content;
	extern char *g_rand_activity_kill_golden_reward_content;

	extern char *g_rand_activity_openserver_invest;
	extern char *g_rand_activity_openserver_invest_reward;
	extern char *g_rand_activity_single_chongzhi_reward_content;
	extern char *g_rand_activity_rush_buying_begin;
	extern char *g_rand_activity_map_hunt_get_rare_city_content;
	extern char *g_rand_activity_check_reward;
	extern char *g_rand_activity_light_tower_explore_reward_content;
	extern char *g_rand_activity_magic_shop_reward_content;
	extern char *g_rand_activity_chongzhi_crazy_rebate;
	extern char *g_rand_activity_perfect_lover_reward_item_content;
	extern char *g_ra_jinjie_return_reward_subject;
	extern char *g_ra_jinjie_return_reward_content;
	extern char *g_rand_activity_check_reward_common_mail_content;

	extern char *g_rand_activity_time_limit_gift_reward_content;
	extern char *g_rand_activity_time_limit_gift_reward_mail;
	extern char *g_circulation_chongzhi_not_ra_reward_content;
	extern char *g_rand_activity_shake_money_reward_content;
	extern char *g_rand_activity_time_limit_luxury_gifts_content;
	extern char *g_rand_activity_double_chongzhi_reward_content;
	extern char *g_rand_activity_expense_nice_gift_mail_content;
	extern char *g_rand_activity_version_total_charge_reward_content;
	extern char *g_rand_activity_version_continue_mail_content;
	extern char *g_rand_activity_huanleyaojiang_2_zhenxi_reward_content;
	extern char *g_rand_activity_offline_single_charge_reward_content;
	extern char *g_rand_activity_guagua_reward_content;
	extern char *g_rand_activity_guagua_acc_reward_mail;
	extern char *g_rand_activity_uplevel_return_reward_mail;
	////////////////////////////////////// 秘境寻宝3 ///////////////////////////////////////
	extern char *g_rand_activity_mijingxunbao3_zhenxi_reward_content;

	////////////////////////////////////// 欢乐砸蛋 ///////////////////////////////////////
	extern char *g_rand_activity_huanlezadan_zhenxi_reward_content;

	////////////////////////////////////// 欢乐摇奖 ///////////////////////////////////////
	extern char *g_rand_activity_huanleyaojiang_zhenxi_reward_content;

	///////////////////////////////////// 累计充值5 /////////////////////////////////////////////////////
	extern char *g_rand_activity_total_charge5_reward_content;

	///////////////////////////////////////  无双  //////////////////////////////////////////
	extern char *g_wushuang_fb_pass_reward_content;

	///////////////////////////////////////  鱼池  //////////////////////////////////////////
	extern char *g_fish_pool_be_steal_content;

	///////////////////////////////////////  世界事件  //////////////////////////////////////////
	extern char *g_world_event_event_reward_content;
	extern char *g_world_event_set_yurenka_appearance;
	extern char *g_world_event_be_set_yurenka_appearance;

	///////////////////////////////////////  红包  /////////////////////////////////////////
	extern char *g_red_paper_fetch_succ_notice;
	extern char *g_red_paper_fetch_over_notice;
	extern char *g_red_paper_lucky_user_notice;
	extern char *g_red_paper_give_left_money_content;
	extern char *g_red_paper_create_notice;
	extern char *g_red_paper_create_notice2;
	extern char *g_red_paper_singal_chat;
	extern char *g_command_speak_create_command;

	///////////////////////////////////////  兑换商店  /////////////////////////////////////////
	extern char *g_convershop_guild_notice;
	extern char *g_convershop_world_notice;

	///////////////////////////////////////	 一站到底  /////////////////////////////////////////
	extern char *g_yizhandaodi_fininsh_brocast;
	extern char *g_yizhandaodi_buff_broast;
	extern char *g_yizhandaodi_broast_cur_role_num;
	extern char *g_yizhandaodi_reward_subject;
	extern char *g_yizhandaodi_reward_rank_content;
	extern char *g_yizhandaodi_reward_norank_content;
	extern char *g_yizhandao_jisha_count_notice;
	extern char *g_yizhandaodi_lucky_notice;
	extern char *g_yizhandaodi_lucky_reward_subject;
	extern char *g_yizhandaodi_lucky_reward_content;
	extern char *g_yizhandaodi_dead_times_reward_content;
	extern char *g_yizhandaodi_kickout_notice;
	extern char *g_yizhandaodi_first_replace_notice;
	extern char *g_yizhandaodi_finish_rank_top1;
	extern char *g_yizhandaodi_finish_rank_top2;
	extern char *g_yizhandaodi_finish_rank_top3;
	extern char *g_yizhandaodi_kickout_mail_content;

	///////////////////////////////////////	 塔防副本  /////////////////////////////////////////
	extern char *g_team_towerdefend_teammate_level_limit;
	extern char *g_team_towerdefend_teammate_can_not_enter;
	extern char *g_team_towerdefend_teammate_join_times_limit;
	extern char *g_team_towerdefend_teammate_attr_type_error;
	extern char *g_team_towerdefend_pass_wave;
	extern char *g_team_towerdefend_pass_reward;

	///////////////////////////////////////	 五行挂机  /////////////////////////////////////////
	extern char *g_wuxingguaji_kill_boss_notice;
	extern char *g_wuxingguaji_boss_flush_notice;
	extern char *g_wuxingguaji_reward_content;
	extern char *g_wuxingguaji_lucky_notice;
	extern char *g_wuxingguaji_lucky_reward_subject;
	extern char *g_wuxingguaji_lucky_reward_content;

	///////////////////////////////////////	 妖兽广场  /////////////////////////////////////////
	extern char *g_yaoshouguangchang_reward_content;
	extern char *g_suoyaota_reward_content;

	///////////////////////////////////////	 仙盟运势  /////////////////////////////////////////
	extern char *g_guild_lucky_info_orange_notice;

	///////////////////////////////////////	 角色转阵营  /////////////////////////////////////////
	extern char *g_role_change_camp_notice;

	///////////////////////////////////////	 卡牌  /////////////////////////////////////////
	extern char *g_card_uplevel_notice;

	///////////////////////////////////////	 精华护送  /////////////////////////////////////////
	extern char *g_jinghua_refresh_notice;
	extern char *g_jinghua_husong_timeout_subject;
	extern char *g_jinghua_husong_timeout_content;

	///////////////////////////////////////	 神秘商店  /////////////////////////////////////////
	extern char *g_mysteriousrefresh_notice;
	extern char *g_mysteriousshop_refresh_good_item;
	extern char *g_mysteriousshop_buy_good_item;

	//////////////////////////////////////元宝转盘 ////////////////////////////////////////////
	extern char *g_gold_turntable_notice;

	///////////////////////////////////////	 土豪金 /////////////////////////////////////////
	extern char *g_tuhaojin_activate_notic;

	///////////////////////////////////////	 大表情 /////////////////////////////////////////
	extern char *g_bigchatface_activate_notice;

	///////////////////////////////////////	 神装  /////////////////////////////////////////
	extern char *g_shenzhuang_downlevel;
	extern char *g_shenzhuang_jinjie;

	///////////////////////////////////////	 神州六器 /////////////////////////////////////////
	extern char *g_shenzhou_weapon_gather_content;
	extern char *g_shenzhou_gather_refresh_content;
	extern char *g_shenzhou_help_box_reward;
	extern char *g_shenzhou_gather_top_class_stone;
	extern char *g_shenzhou_weapon_active;
	extern char *g_shenzhou_skill_up_level;
	extern char *g_shenzhou_identify_up_level;
	extern char *g_shenzhou_lianhun_active;
	extern char *g_shenzhou_lianhun_level_up;
	extern char *g_shenzhou_xilian;
	extern char *g_shenzhou_treasure;

	///////////////////////////////////////	 宝宝 /////////////////////////////////////////
	extern char *g_baby_upgrade_mail;
	extern char *g_baby_uplevel_mail;
	extern char *g_baby_upgrade;
	extern char *g_baby_subject_mail;
	extern char *g_baby_qifu_succ;

	///////////////////////////////////////	 魔龙 /////////////////////////////////////////
	extern char *g_molong_reward_content;
	extern char *g_molong_reward_content_item;

	///////////////////////////////////////	 宠物 /////////////////////////////////////////
	extern char *g_pet_upgrade;
	extern char *g_pet_reward;
	extern char *g_pet_active_suit;

	///////////////////////////////////////	 牧场精灵 /////////////////////////////////////////
	extern char *g_pasture_spirit_lucky_draw_content;

	///////////////////////////////////////	 双人坐骑 /////////////////////////////////////////
	extern char *g_multi_mount_upgrade_content;

	///////////////////////////////////////	 个性窗口 /////////////////////////////////////////
	extern char *g_personalize_window_bubble_rim_active_content;

	///////////////////////////////////////	 魔卡 /////////////////////////////////////////
	extern char *g_magic_card_reward_content;

	///////////////////////////////////////	 精灵 /////////////////////////////////////////
	extern char *g_jingling_halo_upgrade_notice;
	extern char *g_jingling_fazhen_upgrade_notice;
	extern char *g_jingling_wuxing_notice1;
	extern char *g_jingling_wuxing_notice2;
	extern char *g_jingling_card;
	extern char *g_jingling_refresh_purple;

	/////////////////////////////////////// 转生 ///////////////////////////////////////
	extern char *g_zhuansheng_level_up_notice;
	extern char *g_zhuansheng_notice;

	//////////////////////////////////////  套装 ///////////////////////////////////////////////
	extern char *g_equip_suit_shishi_active_content;
	extern char *g_equip_suit_chuanshuo_active_content;

	/////////////////////////////////////// 水晶 ///////////////////////////////////////////////
	extern char *g_big_shuijing_refresh;
	extern char *g_big_shuijing_already_refresh;
	extern char *g_big_shuijing_gather;
	extern char *g_gather_type_shuijing;
	extern char *g_finish_shuijing_task;

	/////////////////////////////////////// 符文系统 //////////////////////////////////////////
	extern char *g_runesystem_one_xunbao_notice;
	extern char *g_runesystem_ten_xunbao_notice;
	extern char *g_runesystem_pass_layer;
	extern char *g_runesystem_up_level_send_to_world;
	extern char *g_runesystem_zhuling;

	/////////////////////////////////////// 日常副本 //////////////////////////////////////////
	extern char *g_dailyfb_cant_enter_fb;

	/////////////////////////////////////// 组队装备副本 //////////////////////////////////////////
	extern char *g_team_equip_fb_notice;

	/////////////////////////////////////// 爬塔副本	//////////////////////////////////////////
	extern char *g_pata_fb_pass_layer;

	/////////////////////////////////////// 神格 //////////////////////////////////////////
	extern char *g_chou_jiang_msg;
	extern char *g_shenge_combine_msg;
	extern char *g_shenge_total_level;
	extern char *g_shenge_active_new_page;
	extern char *g_shenge_active_combine;
	extern char *g_shenge_zhangkong_system_msg;
	extern char *g_shenge_xilian_system_msg;

	/////////////////////////////////////// 星座 //////////////////////////////////////////
	extern char *g_xintu_finish_combine;
	extern char *g_xintu_finish_chapter;
	extern char *g_xingzuo_update_limit;
	extern char *g_xingzuo_taozhuang;
	extern char *g_xingzuo_equip_level;

	//////////////////////////////////////	封神 //////////////////////////////////////////
	extern char *g_finish_chapter;


	extern char *g_chongjihaoli;

	//////////////////////////////////////  活跃奖励 /////////////////////////////////////////
	extern char *g_active_degree;

	//////////////////////////////////////  被杀 ////////////////////////////////////////////
	extern char *g_be_killed_by_role_in_static_scene;
	extern char *g_be_killed_in_boss_scene;

	///////////////////////////////////////	 合服活动  /////////////////////////////////////////
	extern char *g_csa_rank_reward_content;
	extern char *g_csa_rank_reward_notice_qianggou;
	extern char *g_csa_gcz_winner_reward_content;
	extern char *g_csa_gcz_camp_reward_content;
	extern char *g_csa_xmz_winner_reward_content;
	extern char *g_csa_xmz_guild_reward_content;
	extern char *g_csa_gcz_winner_notice;
	extern char *g_csa_xmz_winner_notice;
	extern char *g_csa_single_charge_reward_content;
	extern char *g_csa_charge_reward_double_content;
	extern char *g_csa_roll_reward_notice;
	extern char *g_csa_boss_guild_rank_master_reward_content;
	extern char *g_csa_boss_guild_rank_member_reward_content;
	extern char *g_csa_touzijihua_auto_fetch_reward_content;
	extern char *g_csa_foundation_auto_fetch_reward_content;
	extern char *g_csa_logingift_auto_fetch_reward_content;
	extern char *g_csa_boss_refresh_greatboss;
	extern char *g_csa_boss_killall_greatboss;
	extern char *g_welcome_mail_content;
	extern char *g_cardzu_active_content;

	/////////////////////////////////////// 跨服 1v1 /////////////////////////////////////////
	extern char *g_cross_1v1_dur_kill_notice;
	extern char *g_cross_1v1_end_dur_kill_notice;
	extern char *g_cross_1v1_xiazhu_reward_subject;
	extern char *g_cross_1v1_xiazhu_reward_content;
	extern char *g_cross_1v1_activity_close_notice;

	///////////////////////////////////////	 跨服修罗塔  /////////////////////////////////////////
	extern char *g_cross_xiuluo_tower_up_layer_notice;
	extern char *g_cross_xiuluo_tower_finish_notice;
	extern char *g_cross_xiuluo_tower_reward_content;
	extern char *g_cross_xiuluo_tower_rank_reward_content;
	extern char *g_cross_rank_week_reward_content;
	extern char *g_cross_xiuluo_tower_get_buff;
	extern char *g_cross_xiuluo_tower_rap_buff;
	extern char *g_cross_xiuluo_tower_get_reward;
	extern char *g_cross_xiuluo_tower_reward_subject;
	extern char *g_cross_xiuluo_tower_first_reward_subject;
	extern char *g_cross_xiuluo_tower_kill_boss;
	extern char *g_cross_xiuluo_tower_boss_refresh;
	extern char *g_cross_xiuluo_tower_boss_already_refresh;
	extern char *g_xiuluo_tower_gold_box_reward_mail_content;
	
	///////////////////////////////////////	 好友送礼  /////////////////////////////////////////
	extern char *g_friend_gift_shou_content;
	extern char *g_msg_gift_send_singlechat;
	extern char *g_msg_friend_song_hua_chat_content;
	extern char *g_friend_song_hua_content;
	extern char *g_friend_heli_songhua_content;
	extern char *g_friend_heli_egg_content;

	///////////////////////////////////////	 排行称号  /////////////////////////////////////////
	extern char *g_man_charm_rank_title;
	extern char *g_girl_charm_rank_title;
	extern char *g_lose_charm_rank_title;
	extern char *g_add_world_question_rank_title;
	extern char *g_lose_world_question_rank_title;

	///////////////////////////////////////  好友经验瓶 ////////////////////////////////////////
	extern char *g_friend_exp_bottle_search_friend;

	///////////////////////////////////////  神兽		///////////////////////////////////////
	extern char *g_shenshou_compose_content;
	extern char *g_shenshou_huanling_draw;

	///////////////////////////////////////  典籍翻牌		///////////////////////////////////////
	extern char *g_dianji_draw_reward_content;

	////////////////////////////////趣味钓鱼///////////////////////////////
	extern char *g_be_steal_fish_content;
	extern char *g_fishing_broadcast;
	extern char *g_steal_fish_rank_content;
	extern char *g_be_steal_fish_rank_content;

	/////////////////////////////////// 植树 //////////////////////////////////////////////////////////
	extern char *g_rand_activity_planting_tree_canyu_reward;
	extern char *g_rand_activity_planting_tree_planting_reward;
	extern char *g_rand_activity_planting_tree_watering_reward;

	//////////////////////////////////达拉然水晶//////////////////////////////
	extern char *g_steal_crystal_dalaran_reward_content;
	extern char *g_crystal_dalaran_be_stealed_reward_content;
	extern char *g_steal_crystal_dalaran_broadcast1;
	extern char *g_steal_crystal_dalaran_broadcast2;
	extern char *g_steal_crystal_dalaran_broadcast3;
	extern char *g_be_steal_mail_content;

	////////////////////////////////// 元素之心///////////////////////////////////////////
	extern char *g_element_heart_xilian;

	////////////////////////////////// 神印系统///////////////////////////////////////////
	extern char *g_link_shenyin;

	extern char *g_shengxingzhuli_mount;
	extern char *g_shengxingzhuli_fazhen;
	extern char *g_shengxingzhuli_halo;
	extern char *g_shengxingzhuli_jingling_halo;
	extern char *g_shengxingzhuli_jingling_fazhen;
	extern char *g_shengxingzhuli_pifeng;
	extern char *g_shengxingzhuli_wing;

	extern char *g_shengxingzhuli_mail_subject;
	extern char *g_shengxingzhuli_mail_content;

	//////////////////////////////	 上古boss  /////////////////////////////////////////
	extern char *g_shanggu_boss_hide_flush_notice;

	//////////////////////////////	 守护小鬼  /////////////////////////////////////////
	extern char *g_impguard_remind_mail_content;
	extern char *g_get_imp_type_1_notice;
	extern char *g_get_imp_type_2_notice;

	///////////////////////////////// 每日好礼  ////////////////////////////////////////
	extern char *g_everyday_nice_gift_fetch_reward;

	/////////////////////////////////  进阶系统奖励	/////////////////////////////////////
	extern char *g_jinjiesys_reward_active;
	extern char *g_jinjiesys_reward_mail_content;

	///////////////////////////////////////  神器特殊形象激活 ////////////////////////////////////////
	extern char *g_shenqi_special_img_active;

	///////////////////////////////////////	 角色大小目标   /////////////////////////////////////////
	extern char *g_role_big_small_goal_active_reward;

	//////////////////////////////////// 限时战力过期		/////////////////////////////////////////
	extern char *g_time_limit_cap_img_mail_context;

	//////////////////////////////////// 神兵图录	/////////////////////////////////////////
	extern char *g_red_equip_collect_notice_content;
	extern char *g_red_equip_collect_act_reward_mail_content;
	extern char *g_red_equip_collect_fetch_title_notice_content;
	extern char *g_orange_equip_collect_notice_content;

	//////////////////////////////////// 全民比拼   /////////////////////////////////////////
	extern char *g_mount_national_competition_content;
	extern char *g_wing_national_competition_content;
	extern char *g_shizhuang_national_competition_content;
	extern char *g_shenbing_national_competition_content;
	extern char *g_fabao_national_competition_content;
	extern char *g_footprint_national_competition_content;
	extern char *g_halo_national_competition_content;
	extern char *g_shengong_national_competition_content;
	extern char *g_qilinbi_national_competition_content;
	extern char *g_toushi_national_competition_content;
	extern char *g_yaoshi_national_competition_content;
	extern char *g_lingtong_national_competition_content;
	extern char *g_linggong_national_competition_content;
	extern char *g_lingqi_national_competition_content;
	extern char *g_fight_mount_national_competition_content;
	extern char *g_flypet_national_competition_content;
	extern char *g_weiyan_national_competition_content;
	extern char *g_shenyi_national_competition_content;

	//////////////////////////////////// 仙尊卡   /////////////////////////////////////////
	extern char *g_xian_zun_ka_stale_dated;

	//////////////////////////////////// boss图鉴   /////////////////////////////////////////
	extern char *g_boss_card_activity_this_type;

	//////////////////////////////////// 装备洗炼  /////////////////////////////////////////
	extern char *g_equip_baptize_four_purple_notice;
	extern char *g_equip_baptize_orange_notice;
	extern char *g_equip_baptize_red_notice;
	extern char *g_equip_baptize_orange_and_red_notice;

	//////////////////////////////////// 夫妻家园  /////////////////////////////////////////
	extern char *g_spousehome_buy_room_for_lover;
	extern char *g_spousehome_equip_furniture_for_lover;

	//////////////////////////////////// 每日一爱2 /////////////////////////////////////////
	extern char *g_rand_activity_daily_love_2_reward_content;

	/////////////////////////////////////// 名将 //////////////////////////////////////////
	extern char *g_greate_soldier_draw_content;
	extern char *g_greate_fetch_reward_content;

	///////////////////////////////////////	 跨服随机活动   /////////////////////////////////////////
	extern char  *g_cross_ra_chongzhi_rank_mvp_reward;
	extern char  *g_cross_ra_chongzhi_rank_join_reward;
	extern char  *g_cross_ra_chongzhi_rank_low_mvp_reward;
	extern char  *g_cross_ra_chongzhi_rank_low_join_reward;

	extern char  *g_cross_ra_consume_rank_mvp_reward;
	extern char  *g_cross_ra_consume_rank_join_reward;
	extern char  *g_cross_ra_consume_rank_low_mvp_reward;
	extern char  *g_cross_ra_consume_rank_low_join_reward;

	///////////////////////////////////// 天书寻主 /////////////////////////////////////////////
	extern char *g_tianshuxunzhu_fretch_reward;

	/////////////////////////////////////// 幸运云购 ///////////////////////////////////////////////////
	extern char *g_lucky_cloud_buy_reward_mail;		// 邮件 大奖
	extern char *g_lucky_cloud_buy_return_mail;		// 邮件 返还元宝
	extern char *g_lucky_cloud_buy_no_reward_mail;		// 邮件 无中奖告知
	extern char *g_lucky_cloud_buy_min_reward_mail;	// 邮件 保底奖励
	extern char *g_lucky_cloud_buy_reward;				// 传闻 大奖
	extern char *g_lucky_cloud_buy_noreward;			// 传闻 未满人数，返回元宝
	extern char *g_lucky_cloud_buy_can_join;			// 传闻 本轮开始

	////////////////////////////////////// 神话 /////////////////////////////////////////
	extern char *g_myth_uplevel_broadcast;
	extern char *g_myth_activate_broadcast;
	extern char *g_myth_resonance;
	extern char *g_myth_compose_broadcast;

	///////////////////////////////////////	 周末装备   /////////////////////////////////////////
	extern char *g_tianshenhuti_combine_notice;
	extern char *g_weekend_score_boss_rank_reward;

	bool StringInit(const char *filename, std::string *error);
}
	
#endif // __GAMEWORLDSTR_H__

