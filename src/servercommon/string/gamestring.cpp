#include "common/config/xmlconfig.h"
#include "servercommon/string/gameworldstr.h"

namespace gamestring
{
	char GAMESTRING_BUF[2048] = {0};

	///////////////////////////////////////  拍卖  /////////////////////////////////////////
	char *g_publicsale_item_full_subject;
	char *g_publicsale_item_full_content;
	char *g_publicsale_item_cancel_subject;
	char *g_publicsale_item_cancel_content;
	char *g_publicsale_item_expire_subject;
	char *g_publicsale_item_expire_content;
	char *g_publicsale_sell_succ_subject;
	char *g_publicsale_sell_succ_content;
	char *g_publicsale_sell_to_world;
	char *g_publicsale_buy_succ_subject;
	char *g_publicsale_buy_succ_content;
	char *g_publicsale_buy_fail_subject;
	char *g_publicsale_buy_fail_content;


	///////////////////////////////////////  军团  /////////////////////////////////////////
	char *g_create_guild_succ_public_notice;
	char *g_create_lv2_guild_succ_public_notice;
	char *g_msg_join_guild;
	char *g_msg_leave_guild;
	char *g_create_guild_mail_subject;
	char *g_create_guild_mail_content;
	char *g_dimiss_guild_mail_subject;
	char *g_dimiss_guild_mail_content;
	char *g_auto_dimiss_guild_mail_subject;
	char *g_auto_dimiss_guild_mail_content;
	char *g_gm_dimiss_guild_mail_subject;
	char *g_gm_dimiss_guild_mail_content;
	char *g_guild_downgrade_subject;
	char *g_guild_downgrade_content;
	char *g_guild_warning_dismiss_subject;
	char *g_guild_warning_dismiss_content;
	char *g_guild_warning_dismiss_byexp_subject;
	char *g_guild_warning_dismiss_byexp_content;
	char *g_guild_upgrade_level_subject;
	char *g_guild_upgrade_level_content;
	char *g_join_guild_mail_subject;
	char *g_join_guild_mail_content;
	char *g_bekicked_guild_mail_subject;
	char *g_bekicked_guild_mail_content;
	char *g_guild_tuanzhang_appointed_subject;
	char *g_guild_tuanzhang_appointed_content;

	char *g_guild_tuanzhang_delated_subject;
	char *g_guild_tuanzhang_delated_content;

	char *g_msg_appointed_subject;
	char *g_msg_futuanzhang_be_appointed;
	char *g_msg_zhanglao_be_appointed;
	char *g_msg_jingying_be_appointed;
	char *g_msg_hufa_be_appointed;

	char *g_msg_guild_leave_post;
	char *g_guild_rename_notice;
	char *g_guild_call_beast_content;
	char *g_guild_kill_beast_content;
	char *g_guild_rename_subject;
	char *g_guild_rename_content;
	char *g_guild_guilduplevel;

	char *g_msg_guild_juanxian_bygold;
	char *g_msg_guild_juanxian_byitem;
	char *g_msg_guild_juanxian_bycoin;

	char *g_guild_open_bonfire;
	char *g_guild_open_guild_fb;
	char *g_guild_box_assist_subject;
	char *g_guild_box_assist_content;
	char *g_guild_box_open_content;
	char *g_guild_boss_call_content;
	char *g_guild_boss_reward_content;

	char *g_msg_guild_welcome1;
	char *g_msg_guild_welcome2;
	char *g_msg_guild_welcome3;
	char *g_msg_guild_welcome4;
	char *g_msg_guild_welcome5;
	char *g_msg_guild_welcome6;
	char *g_msg_guild_welcome7;
	char *g_msg_guild_welcome8;

	char *g_guild_sos_husong;

	char *g_guild_boss_create;
	char *g_guild_supper_boss_create;
	
	char *g_guild_call_in;
	char *g_guild_memberkilled;

	char *g_guild_storage_put_item;
	char *g_guild_storage_take_item;
	char *g_guild_boss_red_bag;
	char *g_guild_pao_saizi;
	char *g_guild_pao_saizi_rank3;
	char *g_guild_pao_saizi_rank2;
	char *g_guild_pao_saizi_rank1;
	char *g_guild_saizi_rank_msg1;
	char *g_guild_saizi_rank_msg2;
	char *g_guild_saizi_reward_mail;
	char *g_guild_maze_rank_reward;

	char *g_guild_flag_first_be_hit_notice;
	char *g_guild_flag_be_attacked_notice;
	char *g_guild_flag_be_occupied_notice;
	char *g_guild_officer_be_killed;
	char *g_guild_chengyuan_be_killed;
	char *g_guild_box_invite_assist;

	char *g_qingyuan_rank_top3;
	char *g_add_capability_rank_top3;
	char *g_add_charm_rank_top3;

	char *g_guild_activity_team_reward_notice;

	///////////////////////////////////////	 传闻链接   /////////////////////////////////////////
	char *g_link_upstar;
	char *g_link_fuling;
	char *g_link_lieming;
	char *g_link_jingling_uplevel;
	char *g_link_jingling_upgrade;
	char *g_link_chengjiu_title;
	char *g_link_xianjie;
	char *g_link_inherit;
	char *g_link_jingling_strength;
	char *g_link_jingling_phantom;
	char *g_link_jingling_soul;

	///////////////////////////////////////	 寻宝   /////////////////////////////////////////
	char *g_chuji_equip_chestshop_item_broadcast;
	char *g_chuji_jingling_chestshop_item_broadcast;
	char *g_gaoji_chestshop_item_broadcast;
	char *g_chestshop_get_xiannv_notice;
	char *g_chestshop_get_xiannv_content;

	char *g_gengu_broadcast;
	char *g_mentality_upgrade_max;
	char *g_mentality_combine_upgrade;
	char *g_mentality_combine_learn_skill;

	char *g_bekilled_notice_subject;
	char *g_bekilled_notice_content;
	char *g_evil_bekilled_notice_content;

	char *g_flower_notice;
	char *g_flower_notice_anonymity;
	char *g_team_create_notice;

	char *g_yaojiang_notice;

	///////////////////////////////////////	 邮件  /////////////////////////////////////////
	char *g_mail_receiver_full_subject;
	char *g_mail_receiver_full_content;

	///////////////////////////////////////	 怪物  /////////////////////////////////////////
	char *g_kill_common_scene_boss;
	char *g_kill_fb_boss;
	char *g_refresh_boss;
	char *g_pick_body_drop;
	char *g_pick_body_drop_equipment;

	///////////////////////////////////////	 战斗  /////////////////////////////////////////
	char *g_common_scene_kill_role;

	///////////////////////////////////////	 物品  /////////////////////////////////////////
	char *g_item_equip_compound_1;
	char *g_item_equip_compound_2;
	char *g_item_equip_total_strengthen;
	char *g_item_equip_shenzhu_total;
	char *g_item_equip_shenzhu_up_shen_level;
	char *g_item_equip_upstar_total;
	char *g_item_equip_up_star;
	char *g_item_equip_upquality;
	char *g_item_equip_upquality_1;
	char *g_item_equip_upquality_2;
	char *g_item_equip_upquality_3;
	char *g_item_equip_upquality_4;
	char *g_item_equip_upquality_5;
	char *g_item_equip_upquality_6;
	char *g_item_equip_strengthen;
	char *g_item_equip_uplevel;
	char *g_item_pick;
	char *g_item_pick_equipment;
	char *g_item_get_equipment_send_to_guild;
	char *g_item_invalid_subiect;
	char *g_item_invalid_content;
	char *g_item_stone_up_level;
	char *g_item_stone_up_level_single_part;
	char *g_item_stone_up_level_consume;
	char *g_item_stone_up_level_consume_has_diamond;
	char *g_item_eternity_up_level;
	char *g_item_eternity_up_level_1;
	char *g_item_eternity_suit;

	///////////////////////////////////////	 羽翼  /////////////////////////////////////////
	char *g_wing_upgrade;
	char *g_wing_active_content;

	///////////////////////////////////////	 女神  ////////////////////////////////////////
	char *g_xiannv_active_content;
	char *g_xiannv_shengwu_miling;
	char *g_xiannv_shengwu_upgrade;
	char *g_xiannv_shengwu_skill_upgrade;
	char *g_xiannv_gongming_upgrade;

	///////////////////////////////////////	 个人目标  /////////////////////////////////////////
	char *g_role_goal_reward_content;

	///////////////////////////////////////  VIP /////////////////////////////////////////
	char *vip_uplevel;
	char *vip_uplevel_personal;
	char *g_gold_vip_active_content;
	char *g_time_limit_vip_active_content;

	///////////////////////////////////////	 背包  /////////////////////////////////////////
	char *g_knapsack_put_subiect;
	char *g_knapsack_put_content;

	///////////////////////////////////////	 角色  /////////////////////////////////////////
	char *g_zhuanzhi;
	char *g_change_prof;

	char *g_shizhuang_upgrade;

	///////////////////////////////////////	 坐骑  /////////////////////////////////////////
	char *g_mount_upgrade;
	char *g_mount_fly_up;
	char *g_qibing_upgrade;
	char *g_mount_active_content;

	///////////////////////////////////////  光环  /////////////////////////////////////////
	char *g_halo_upgrade;

	///////////////////////////////////////	 神弓  /////////////////////////////////////////
	char *g_shengong_upgrade;

	///////////////////////////////////////	 神翼  /////////////////////////////////////////
	char *g_shenyi_upgrade;

	///////////////////////////////////////	 战斗坐骑  /////////////////////////////////////////
	char *g_fight_mount_upgrade;

	///////////////////////////////////////  足迹  /////////////////////////////////////////
	char *g_footprint_upgrade;

	///////////////////////////////////////  形象赋灵	//////////////////////////////////////
	char *g_img_fuling_skill_level_up;
	char *g_img_fuling_level_up;

	///////////////////////////////////////  披风  /////////////////////////////////////////
	char *g_cloak_up_level;
	char *g_cloak_imge;

	///////////////////////////////////////	 魔戒  /////////////////////////////////////////
	char *g_mojie_uplevel;

	///////////////////////////////////////	 灵刃  /////////////////////////////////////////
	char *g_shenbing_level_up;

	///////////////////////////////////////	 神兵  /////////////////////////////////////////
	char *g_shenbing_upgrade;

	///////////////////////////////////////  法宝  /////////////////////////////////////////
	char *g_fabao_upgrade;

	///////////////////////////////////////	 充值  /////////////////////////////////////////
	char *g_chongzhi_reward_subject;
	char *g_chongzhi_reward_content;
	char *g_chongzhi_reward_coin_content;
	char *g_chongzhi_special_first_recharge_reward_content;
	char *g_chongzhi_daily_first_recharge_reward_content;
	char *g_chongzhi_daily_total_recharge_reward_content;
	char *g_chongzhi_daily_total_recharge_send_to_world;
	char *g_chongzhi_first_chongzhi_reward_content;
	char *g_chongzhi_zai_chongzhi_reward_content;
	char *g_chongzhi_san_chongzhi_reward_content;
	char *g_chongzhi_total_chongzhi_reward_content;
	char *g_chongzhi_danbi_chongzhi_reward_content;
	char *g_chongzhi_diff_weeekday_chongzhi_reward_content;
	char *g_chongzhi_daily_recharge_reward2_content;
	char *g_touzi_level;
	char *g_touzi_week_card;
	char *g_touzi_fb_plan;
	char *g_touzi_boss_plan;
	char *g_touzi_shenyu_boss_plan;
	char *g_zero_gift;
	char *g_discount_buy;
	char *g_discount_buy_mail_content;
	char *g_time_limit_gift;
	char *g_time_limit_gift_2;
	char *g_panic_buy;

	///////////////////////////////////////	 开随机礼包  /////////////////////////////////////////
	char *g_open_randgift[OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM];

	///////////////////////////////////////	 群仙乱斗  /////////////////////////////////////////
	char *g_qunxianluandou_score_reward_subject;
	char *g_qunxianluandou_score_reward_content;
	char *g_qunxianluandou_side_win_subject;
	char *g_qunxianluandou_side_win_content;
	char *g_qunxianluandou_side_lose_subject;
	char *g_qunxianluandou_side_lose_content;
	char *g_qunxianluandou_qunxian_open_server;
	char *g_qunxianluandou_first_side;

	///////////////////////////////////// 怪物入侵 ////////////////////////////////////////
	char *g_monster_invade_refresh;

	///////////////////////////////////// 大富豪 ////////////////////////////////////////
	char *g_millionaire_boss_refresh;
	char *g_millionaire_frozen;
	char *g_millionaire_gather_times;
	char *g_millionaire_gather_good;
	char *g_millionaire_kill_boss;

	///////////////////////////////////// 走棋子 ////////////////////////////////////////
	char *g_movechess_good_item;

	///////////////////////////////////// 答题 ////////////////////////////////////////
	char *g_question_rank_broadcast;
	char *g_question_rank_reward;
	char *g_question_fast_world_reward;
	char *g_question_fast_guild_reward;
	char *g_question_fast_world_reward_mail;
	char *g_question_fast_guild_reward_mail;
	char *g_question_guild_rank_reward;
	char *g_question_guild_rank_other_reward;
	char *g_question_system_message_world;
	char *g_question_system_message_guild;

	///////////////////////////////////////	 月卡  /////////////////////////////////////////
	char *g_month_card_reward_per_day_subject;
	char *g_month_card_reward_per_day_content;
	char *g_month_card_reward_on_buy_subject;
	char *g_month_card_reward_on_buy_content;
	char *g_month_card_buy_notice;

	///////////////////////////////////////	 排名竞技场  /////////////////////////////////////////
	char *g_challenge_field_lucky_reward_subject;
	char *g_challenge_field_lucky_reward_content;
	char *g_challenge_field_rank_reward_subject;
	char *g_challenge_field_rank_reward_content;
	char *g_challenge_field_rank_drop_subject;
	char *g_challenge_field_rank_drop_content;
	char *g_challenge_field_rank_pos_notice;
	char *g_challenge_field_rank_pos_notice1;
	char *g_challenge_field_rank_pos_notice2;
	char *g_challenge_field_rank_pos_notice3;
	char *g_challenge_field_close_rank_pos_notice1;
	char *g_challenge_field_close_rank_pos_notice2;
	char *g_challenge_field_close_rank_pos_notice3;
	char *g_challenge_field_rank_pos_first_changed;
	char *g_challenge_field_liansheng_notice;
	char *g_challenge_field_best_rank_reward_content;
	char *g_fighting_challenge_rank_rward_content;
	char *g_challenge_field_rank_close_reward_content;

	///////////////////////////////////////	 money  /////////////////////////////////////////
	char *g_money_chongzhi_subject;
	char *g_money_chongzhi_content;

	///////////////////////////////////////	 成功提示  ///////////////////////////////////
	char *g_succnotice_mentility_lift;

	///////////////////////////////////////	 错误提示  ///////////////////////////////////
	char *g_errornotice_enter_scene_limit_level;
	char *g_errornotice_enter_fb_limit_level;
	char *g_errornotice_lack_item;

	///////////////////////////////////////	 世界BOSS  /////////////////////////////////////////
	char *g_kill_world_boss;
	char *g_worldboss_person_hurt_reward;
	char *g_worldboss_guild_hurt_reward;
	char *g_worldboss_killer_guild_reward;
	char *g_worldboss_week_rank_reward;
	char *g_worldboss_red_paper_reward;
	char *g_worldboss_hudun_reward;
	char *g_worldboss_hudun_reward_broadcast;
	char *g_worldboss_refresh;
	char *g_cross_boss_enter_notice;
	char *g_cross_boss_enter2_notice;
	char *g_cross_boss_enter3_notice;
	char *g_dabao_boss_refresh_notice;
	char *g_precious_boss_refresh;
	char *g_active_boss_rank_reward_mail_context;

	///////////////////////////////////////	 角色改名  /////////////////////////////////////////
	char *g_role_rename_notice;
	char *g_role_rename_subject;
	char *g_role_rename_content;

	///////////////////////////////////////	 角色变性  /////////////////////////////////////////
	char *g_role_set_sex_notice;
	char *g_role_set_sex_subject;
	char *g_role_set_sex_content;

	///////////////////////////////////////	 排行榜称号  /////////////////////////////////////////
	char *g_toprank_new_first_broadcast;
	char *g_toprank_change_first_broadcast;
	char *g_toprank_capability_top_user_changed_notice;
	char *g_toprank_capability_user_login;
	char *g_toprank_ten_capability_user_login;
	char *g_meilirank_capability_user_login;
	char *g_man_charm_rank_title;
	char *g_girl_charm_rank_title;
	char *g_lose_charm_rank_title;
	char *g_add_world_question_rank_title;
	char *g_lose_world_question_rank_title;

	///////////////////////////////////////  好友经验瓶 ///////////////////////////////////////////
	char *g_friend_exp_bottle_search_friend;

	///////////////////////////////////////	 名人堂  /////////////////////////////////////////
	char *g_famous_man_type_1;
	char *g_famous_man_type_2;
	char *g_famous_man_type_3;
	char *g_famous_man_type_4;
	char *g_famous_man_type_5;
	char *g_famous_man_type_6;

	///////////////////////////////////////	 护送  /////////////////////////////////////////
	char *g_husong_accept_notice;
	char *g_husong_rob_notice;
	char *g_husong_berob_mail_subject;
	char *g_husong_berob_mail_content;
	char *g_husong_rob_mail_subject;
	char *g_husong_rob_mail_content;
	char *g_husong_rob_mail_level_limit_content;
	char *g_husong_rob_mail_times_limit_content;
	char *g_husong_overtime_mail_subject;
	char *g_husong_overtime_mail_content;
	char *g_husong_refresh_comsume;
	
	///////////////////////////////////////	 日常任务  /////////////////////////////////////////
	char *g_daily_task_finish_subject;
	char *g_daily_task_finish_content;

	///////////////////////////////////////	 跑环任务  /////////////////////////////////////////
	char *g_paohuan_task_phase_subject;
	char *g_paohuan_task_phase_content;
	char *g_paohuan_task_finish_notice;

	///////////////////////////////////////	 副本  /////////////////////////////////////////
	char *g_equipfb_compensation_roll_subject;
	char *g_equipfb_compensation_roll_context;
	char *g_new_equip_fb_pass;
	char *g_runetower_first_reward;

	///////////////////////////////////////	 转职副本  /////////////////////////////////////////
	char *g_zhuanzhifb_teammate_can_not_enter;

	///////////////////////////////////////	 pvp战场  /////////////////////////////////////////
	char *g_pvpzhanchang_lianzhan_notice1;
	char *g_pvpzhanchang_lianzhan_notice2;
	char *g_pvpzhanchang_lianzhan_notice3;
	char *g_pvpzhanchang_lianzhan_notice4;
	char *g_pvpzhanchang_lianzhan_notice5;
	char *g_pvpzhanchang_lianzhan_notice6;
	char *g_pvpzhanchang_lianzhan_notice7;
	char *g_pvpzhanchang_lianzhan_notice8;
	char *g_pvpzhanchang_lianzhan_notice;

	char *g_pvpzhanchang_stoplianzhan_notice;

	char *g_qunxian_luandou_weak_side_protect_notice;
	char *g_qunxian_luandou_shenshi_disappear;


	///////////////////////////////////////	 攻城战  /////////////////////////////////////////
	char *g_gongchengzhan_finish_occupy_notice;
	char *g_zhangong_reward_content;
	char *g_win_huiyuan_reward_content;
	char *g_win_huizhang_reward_content;
	char *g_gongchengzhan_team_reward_content;
	char *g_last_chenzhu_reward_content;
	char *g_gcz_rankreward_open_server;
	char *g_fail_reward_content;
	char *g_gcz_pomen_shou_notice;
	char *g_gcz_pomen_gong_notice;
	char *g_gcz_zhanqi_notice;

	///////////////////////////////////////	 魔戒  /////////////////////////////////////////
	char *g_mojie_jinglingpian_reward_notice;

	///////////////////////////////////////	 公会争霸  /////////////////////////////////////////
	char *g_guildbattle_close;
	char *g_guildbattle_rankreward_guild;
	char *g_guildbattle_collective_reward;
	char *g_guildbattle_rankreward_person;
	char *g_guildbattle_rankreward_open_server;

	char *g_guildbattle_monster_refresh_prepare_notice;
	char *g_guildbattle_monster_refresh_notice;
	char *g_guildbattle_kill_boss_notice;
	char *g_guildbattle_get_glod_box_notice;
	char *g_guildbattle_commit_glod_box_notice;
	char *g_guildbattle_first_guild;
	char *g_guildbattle_best_killer;
	char *g_guildbattle_fetch_target_reward;
	char *g_guildbattle_point_boss_blood_notice;
	char *g_guildbattle_server_reward_content;
	char *g_guildbattle_guild_reward_content;
	char *g_guildbattle_team_reward_content;
	char *g_guildbattle_on_attack_warning;
	char *g_guildbattle_on_flag_hp_warning;

	///////////////////////////////////////	 领土战  /////////////////////////////////////////
	char *g_territorywar_kill_role;
	char *g_territorywar_result;
	char *g_territorywar_rankreward_open_server;

	char *g_territorywar_monster_build_be_attack;
	char *g_territorywar_monster_build_be_kill;
	char *g_territorywar_center_build_be_occupy_my_side;
	char *g_territorywar_center_build_be_occupy_other_side;

	///////////////////////////////////////	 仙盟战  /////////////////////////////////////////
	char *g_xianmengzhan_reward_subject;
	char *g_xianmengzhan_reward_content;
	char *g_xianmengzhan_occupy_center_area_notice;
	char *g_xianmengzhan_occupy_defend_area_notice;
	char *g_xianmengzhan_occupy_guild_defend_area_notice;
	char *g_xianmengzhan_call_danyao_boss_notice;
	char *g_xianmengzhan_use_danyao_notice;
	char *g_xianmengzhan_kill_notice;
	char *g_xianmengzhan_standy;
	char *g_xianmengzhan_start;
	char *g_xianmengzhan_close;
	char *g_xianmengzhan_join_reward_subject;
	char *g_xianmengzhan_join_reward_content;
	char *g_xianmengzhan_relive_cd_left;
	char *g_xianmengzhan_lost_defend_area;
	char *g_xianmengzhan_guild_in_rank_context;
	char *g_xianmengzhan_guild_not_in_rank_context;

	char *g_nationalboss_reward_subject;
	char *g_nationalboss_reward_content_top10;
	char *g_nationalboss_reward_content_other;
	char *g_kill_national_boss;
	char *g_nationalboss_task_reward_subject;
	char *g_nationalboss_task_reward_content;
	char *g_activity_national_boss_end;

	///////////////////////////////////////	 结婚  /////////////////////////////////////////
	char *g_marry_reserve_subject;
	char *g_marry_reserve_content;
	char *g_marry_zhunbei_notice;
	char *g_marry_zhunbei_subject;
	char *g_marry_zhunbei_content;
	char *g_marry_baitiandi_finish;
	char *g_marry_hunyan_invite_subject;
	char *g_marry_hunyan_invite_content;
	char *g_marry_hunyan_zhunbei_notice;
	char *g_marry_hunyan_bless_subject;
	char *g_marry_hunyan_req_succ;
	char *g_marry_baitiandi_start;
	char *g_marry_reward_suject;
	char *g_marry_reward_content;
	char *g_zhuxie_boss_flush_notice;
	char *g_zhuxie_kill_boss_notice;
	char *g_zhuxie_task_reward_subject;
	char *g_zhuxie_task_reward_content;
	char *g_zhuxie_rob_task_notice;
	char *g_buy_open_game_shop_limit_item;

	char *g_open_game_activity_capability_rank_reward_subject;
	char *g_open_game_activity_capability_rank_reward_content;
	char *g_open_game_activity_guild_level_rank_reward_content;
	char *g_open_game_activity_guild_world_boss_rank_reward_content;

	///////////////////////////////////////// 转轮 ////////////////////////////////////////
	char *g_turntable_great_gift;
	char *g_turntable_millionaire;

	///////////////////////////////////////	 商城  /////////////////////////////////////////
	char *g_shop_buy_notice;

	///////////////////////////////////////	 微端登陆  /////////////////////////////////////////
	char *g_micropc_download_reward;
	char *g_micropc_login_reward;

	///////////////////////////////////////	 阵营  /////////////////////////////////////////
	char *g_camp_gongxun_rank_content;
	char *g_camp_gongxun_value_content;
	char* g_campdefend_sf_reward_subject;
	char* g_campdefend_sf_reward_content;
	char* g_campdefend_gf_reward_subject;
	char* g_campdefend_gf_reward_content;
	char* g_campdefend_diaoxiang_bekilled;

	///////////////////////////////////////	 组队副本  /////////////////////////////////////////
	char *g_team_fb_open_reward_content;
	char *g_team_fb_assist_reward_content;
	char *g_team_fb_teammate_assist_times_limit;
	char *g_team_fb_teammate_level_limit;
	char *g_team_fb_teammate_can_not_enter;
	char *g_equipfb_team_jump_subject;
    char *g_equipfb_team_jump_content;
	char *g_team_equip_fb_pass_notice;

	///////////////////////////////////////  称号管理  //////////////////////////////////////////
	char *g_title_get_title;
	char *g_title_lost_title;
	char *g_title_get_pata_title_notice;

	///////////////////////////////////////  祭炼仙丹  //////////////////////////////////////////
	char *g_jilian_start_reward_subject;
	char *g_jilian_start_reward_content;

	///////////////////////////////////////  仙盟任务  //////////////////////////////////////////
	char *g_guild_task_complete_all_reward_subject;
	char *g_guild_task_complete_all_reward_content;

	///////////////////////////////////////  刺杀任务  //////////////////////////////////////////
	char *g_cisha_task_complete_reward_subject;
	char *g_cisha_task_complete_reward_content;

	///////////////////////////////////////  情缘  //////////////////////////////////////////
	char *g_qingyuan_marry_reward_content;
	char *g_marry_success_notice;
	char *g_marry_fb_can_not_enter;
	char *g_qingyuan_equip_uplevel_notice;
	char *g_qingyuan_equip_upquality_notice_1;
	char *g_qingyuan_equip_upquality_notice_2;
	char *g_qingyuan_divorce_mail_subject;
	char *g_qingyuan_divorce_mail_content;
	char *g_qingyuan_divorce_notice;
	char *g_hunyan_hongbao_notice;
	char *g_hunyan_xianmeng_invite_notice;
	char *g_hunyan_open_hunyan_notice;
	char *g_hunyan_open_person_yanhui_notice;
	char *g_hunyan_zhufu_notice;
	char *g_marry_seeking_notice;
	char *g_qingyuan_active_card_notice;
	char *g_hunyan_zhufu_reward_notice;
	char *g_hunyan_subject_notice;
	char *g_qingyuan_tuodan1;
	char *g_qingyuan_tuodan2;
	char *g_hunyan_reward_notice;
	char *g_lovecontract_finish;
	char *g_marry_zhufu1;
	char *g_marry_zhufu2;
	char *g_marry_zhufu3;
	char *g_marry_zhufu4;
	char *g_marry_zhufu5;
	char *g_marry_zhufu6;
	char *g_marry_zhufu7;
	char *g_marry_songhua;
	char *g_hunyan_zhufu1;
	char *g_hunyan_zhufu2;
	char *g_hunyan_zhufu3;
	char *g_hunyan_sahua;

	///////////////////////////////////////  抽奖  //////////////////////////////////////////
	char *g_luck_roll_reward_item_notice;
	char *g_luck_roll_reward_gold_notice;


	/////////////////////////////////////// 天降财宝  //////////////////////////////////////////
	char *g_tianjiangcaibao_kill_boss_moster_notice;
	char *g_tianjiangcaibao_reflush_qianduoduo_notice;
	char *g_tianjiangcaibao_reflush_bigqian_notice;
	char *g_tianjiangcaibao_already_reflush_bigqian_notice;
	char *g_tianjiangcaibao_already_reflush_qianduoduo_notice;
	char *g_tianjiangcaibao_goblin_reflush_notice;
	char *g_tianjiangcaibao_kill_goblin;
	char *g_tianjiangcaibao_finish_task;

	/////////////////////////////////////// 王陵探险财宝  //////////////////////////////////////////
	char *g_wanglingexplore_reflush_boss_notice;
	char *g_wanglingexplore_already_reflush_monster_notice;
	char *g_wanglingexplore_wangling_boss_come;
	char *g_wanglingexplore_finish_task;
	char *g_wanglingexplore_kill_boss;
	char *g_wanglingexplore_start;
	char *g_wanglingexplore_end;
	char *g_wanglingexplore_standby;
	char *g_wanglingexplore_finish_task_getgift;

	//////////////////////////////////////  星座遗迹	/////////////////////////////////////////////
	char *g_xingzuoyiji_refresh_notice;
	char *g_xingzuoyiji_box_refresh_notice;
	char *g_xingzuoyiji_normal_end_notice;
	char *g_xingzuoyiji_good_end_notice;
	char *g_xingzuoyiji_gather_common_box;
	char *g_xingzuoyiji_gather_molong_box;

	//////////////////////////////////////  温泉答题	/////////////////////////////////////////////
	char *g_hotspring_use_snow;
	char *g_hotspring_use_massage;
	char *g_hotspring_rank_first_player;
	char *g_hotspring_find_lover;

	//////////////////////////////////////  随机活动  ////////////////////////////////////////
	char *g_rand_activity_consume_gold_fanli_reward_content;
	char *g_rand_activity_strengthen_reward_content;
	char *g_rand_activity_chongzhi_rank_reward_content;
	char *g_rand_activity_chongzhi_rank_reward_content_reserve;
	char *g_rand_activity_chongzhi_rank2_reward_content;
	char *g_rand_activity_chongzhi_rank2_reward_content_reserve;
	char *g_rand_activity_consume_gold_rank_reward_content;
	char *g_rand_activity_consume_gold_rank_reward_content_reserve;
	char *g_rand_activity_day_chongzhi_fanli_reward_content;
	char *g_rand_activity_day_consume_gold_reward_content;
	char *g_rand_activity_chestshop_reward_content;
	char *g_rand_activity_total_consume_gold_reward_content;
	char *g_rand_activity_day_active_degree_reward_content;
	char *g_rand_activity_stone_total_level_reward_content;
	char *g_rand_activity_xiannv_chanmian_uplevel_reward_content;
	char *g_rand_activity_mount_upgrade_reward_content;
	char *g_rand_activity_qibing_upgrade_reward_content;
	char *g_rand_activity_mentality_total_level_reward_content;
	char *g_rand_activity_wing_upgrade_reward_content;
	char *g_rand_activity_kill_boss_reward_content;
	char *g_rand_activity_day_online_gift_reward_content;
	char *g_rand_activity_day_online_gift_reward_notice;
	char *g_rand_activity_quanmin_qifu_reward_content;
	char *g_rand_activity_xianmeng_bipin_reward_content;
	char *g_rand_activity_xianmeng_jueqi_reward_content;
	char *g_rand_activity_login_gift_reward_content;
	char *g_rand_activity_bipin_rank_reward_content;
	char *g_rand_activity_bipin_rank_reward_notice;
	char *g_rand_activity_bipin_all_reward_content;
	char *g_rand_activity_bipin_topuser_change_notice;
	char *g_rand_activity_bipin_topuser_change_content;
	char *g_rand_activity_bipin_day_change_mail_content;
	char *g_rand_activity_single_charge_reward_content;
	char *g_rand_activity_single_charge_reward_content2;
	char *g_rand_activity_single_charge_reward_content3;
	char *g_rand_activity_single_charge_reward_content4;
	char *g_rand_activity_single_charge_reward_content5;
	char *g_rand_activity_single_charge_reward_content_multi;
	char *g_rand_activity_double_get_reward_content;
	char *g_rand_activity_cornucopia_reward_content;
	char *g_rand_activity_new_cornucopia_reward_content;
	char *g_rand_activity_double_charge_content;
	char *g_rand_activity_total_charge_day_reward_content;
	char *g_rand_activity_tomorrow_reward_content;
	char *g_rand_activity_day_rank_chongzhi_reweard_content;
	char *g_rand_activity_day_rank_chongzhi_reweard_content_reserve;
	char *g_rand_activity_day_rank_consume_reweard_content;
	char *g_rand_activity_total_charge_bu_reward_content;
	char *g_rand_activity_total_charge_reward_chuanwen_content;
	char *g_rand_activity_new_total_charge_reward_chuanwen_content;
	char *g_rand_activity_new_total_charge_reward_chuanwen_content1;
	char *g_rand_activity_new_total_charge_reward_chuanwen_content2;
	char *g_rand_activity_new_total_charge_reward_chuanwen_content3;
	char *g_rand_activity_level_lottery_reward_content;
	char *g_rand_activity_chongzhi_niuegg_reward_content;
	char *g_rand_activity_zhenbaoge_zhenxi_reward_content;
	char *g_rand_activity_xianyuan_treas_reward_content;
	char *g_rand_activity_xianyuan_treas_all_buy_content;
	char *g_rand_activity_mijingxunbao_zhenxi_reward_content;
	char *g_rand_activity_jisuchongzhan_reward_content;
	char *g_rand_activity_money_tree_reward_content_gold;
	char *g_rand_activity_money_tree_reward_content_item;
	char *g_rand_activity_daily_love_reward_content;
	char *g_rand_activity_king_draw_reward_content;
	char *g_rand_activity_king_draw_return_reward_content;
	char *g_rand_activity_total_charge3_reward_content;
	char *g_rand_activity_appearace_passive_rank_reward_content;
	char *g_rand_activity_appearace_passive_rank_join_reward_content;
	char *g_rand_activity_appearace_rank_reward_content;
	char *g_rand_activity_appearace_rank_join_reward_content;
	char *g_rand_activity_total_charge4_reward_content;
	char *g_rand_activity_mine_server_reward_content;
	char *g_rand_activity_mine_role_refresh_content;
	char *g_rand_activity_mine_exchange_reward_content;
	char *g_rand_activity_tian_ming_divination_reward_content;
	char *g_rand_activity_tian_ming_divination_rare_reward;
	char *g_rand_activity_fanfan_reward_item_content;
	char *g_rand_activity_fanfan_exchange_word_reward_content;
	char *g_rand_activity_continue_chongzhi_reward_content;
	char *g_rand_activity_continue_consume_reward_content;
	char *g_rand_activity_army_day_win_army_reward_content;
	char *g_rand_activity_circulation_chongzhi_reward_content;
	char *g_rand_activity_extreme_lucky_reward_content;
	char *g_rand_activity_extreme_lucky_reward_return_content;
	char *g_rand_activity_mijingxunbao2_zhenxi_reward_content;
	char *g_rand_activity_money_tree2_reward_content_gold;
	char *g_rand_activity_money_tree2_reward_content_item;
	char *g_rand_activity_zhenbaoge2_zhenxi_reward_content;
	char *g_rand_activity_promoting_position_reward_item_content;
	char *g_rand_activity_promoting_position_reward_gold_content;
	char *g_rand_activity_black_market_give_item_content;
	char *g_rand_activity_black_market_return_gold_content;
	char *g_rand_activity_black_market_final_get_reward;
	char *g_rand_activity_role_uplevel_content;
	char *g_rand_activity_pata_content;
	char *g_rand_activity_exp_fb_content;
	char *g_rand_activity_upgrade_mount_content;
	char *g_rand_activity_upgrade_halo_content;
	char *g_rand_activity_upgrade_wing_content;
	char *g_rand_activity_upgrade_shengong_content;
	char *g_rand_activity_upgrade_shenyi_content;
	char *g_rand_activity_first_charge_tuan_content;
	char *g_rand_activity_day_total_charge_content;
	char *g_rand_activity_upgrade_mount_total_content;
	char *g_rand_activity_upgrade_halo_total_content;
	char *g_rand_activity_upgrade_wing_total_content;
	char *g_rand_activity_upgrade_shengong_total_content;
	char *g_rand_activity_upgrade_shenyi_total_content;
	char *g_rand_activity_equip_strength_level_total_content;
	char *g_rand_activity_stone_level_total_content;
	char *g_rand_activity_upgrade_mount_rank_content;
	char *g_rand_activity_upgrade_halo_rank_content;
	char *g_rand_activity_upgrade_wing_rank_content;
	char *g_rand_activity_upgrade_shengong_rank_content;
	char *g_rand_activity_upgrade_shenyi_rank_content;
	char *g_rand_activity_upgrade_qilinbi_rank_content;
	char *g_rand_activity_upgrade_toushi_rank_content;
	char *g_rand_activity_upgrade_yaoshi_rank_content;
	char *g_rand_activity_upgrade_lingtong_rank_content;
	char *g_rand_activity_upgrade_linggong_rank_content;
	char *g_rand_activity_upgrade_lingqi_rank_content;
	char *g_rand_activity_upgrade_shizhuang_rank_content;
	char *g_rand_activity_upgrade_shenbing_rank_content;
	char *g_rand_activity_upgrade_fabao_rank_content;
	char *g_rand_activity_upgrade_footprint_rank_content;
	char *g_rand_activity_upgrade_flypet_rank_content;
	char *g_rand_activity_upgrade_weiyan_rank_content;
	char *g_rand_activity_equip_strength_level_rank_content;
	char *g_rand_activity_stone_total_level_rank_content;
	char *g_rand_activity_xiannv_cap_rank_content;
	char *g_rand_activity_jingling_cap_rank_content;
	char *g_rand_activity_upgrade_fight_mount_rank_content;
	char *g_rand_activity_total_cap_rank_content;
	char *g_rand_activity_boss_killer_total_content;
	char *g_rand_activity_marry_me;
	char *g_rand_activity_bipin_two;
	char *g_rand_activity_bipin;

	char *g_rand_activity_mount_rank_unclaimed_content;
	char *g_rand_activity_wing_rank_unclaimed_content;
	char *g_rand_activity_shizhuang_rank_unclaimed_content;
	char *g_rand_activity_shenbing_rank_unclaimed_content;
	char *g_rand_activity_fabao_rank_unclaimed_content;
	char *g_rand_activity_footprint_rank_unclaimed_content;
	char *g_rand_activity_halo_rank_unclaimed_content;
	char *g_rand_activity_shengong_rank_unclaimed_content;
	char *g_rand_activity_shenyi_rank_unclaimed_content;
	char *g_rand_activity_qilinbi_rank_unclaimed_content;
	char *g_rand_activity_toushi_rank_unclaimed_content;
	char *g_rand_activity_yaoshi_rank_unclaimed_content;
	char *g_rand_activity_lingtong_rank_unclaimed_content;
	char *g_rand_activity_linggong_rank_unclaimed_content;
	char *g_rand_activity_lingqi_rank_unclaimed_content;
	char *g_rand_activity_stone_rank_unclaimed_content;
	char *g_rand_activity_equip_rank_unclaimed_content;
	char *g_rand_activity_xiannv_cap_rank_unclaimed_content;
	char *g_rand_activity_jingling_cap_rank_unclaimed_content;
	char *g_rand_activity_upgrade_fight_mount_rank_unclaimed_content;
	char *g_rand_activity_upgrade_flypet_rank_unclaimed_content;
	char *g_rand_activity_upgrade_weiyan_rank_unclaimed_content;
	char *g_rand_activity_total_cap_rank_unclaimed_content;

	char *g_rand_activity_summon_golden_pig;
	char *g_rand_activity_hurt_golden_pig;
	char *g_rand_activity_kill_golden_pig;
	char *g_rand_activity_summon_golden_pig_reward_content;
	char *g_rand_activity_kill_golden_reward_content;
	
	char *g_rand_activity_openserver_invest;
	char *g_rand_activity_openserver_invest_reward;
	char *g_rand_activity_single_chongzhi_reward_content;
	char *g_rand_activity_rush_buying_begin;
	char *g_rand_activity_map_hunt_get_rare_city_content;
	char *g_rand_activity_check_reward;
	char *g_rand_activity_light_tower_explore_reward_content;
	char *g_rand_activity_magic_shop_reward_content;
	char *g_rand_activity_chongzhi_crazy_rebate;
	char *g_rand_activity_perfect_lover_reward_item_content;

	char *g_ra_jinjie_return_reward_subject;
	char *g_ra_jinjie_return_reward_content;

	char *g_rand_activity_check_reward_common_mail_content;
	char *g_rand_activity_guagua_reward_content;
	char *g_rand_activity_guagua_acc_reward_mail;
	char *g_rand_activity_uplevel_return_reward_mail;

	/////////////////////////////////////// 秘境寻宝3 //////////////////////////////////////////////////
	char *g_rand_activity_mijingxunbao3_zhenxi_reward_content;

	/////////////////////////////////////// 欢乐砸蛋 ///////////////////////////////////////////////////
	char *g_rand_activity_huanlezadan_zhenxi_reward_content;

	/////////////////////////////////////// 欢乐摇奖 ///////////////////////////////////////////////////
	char *g_rand_activity_huanleyaojiang_zhenxi_reward_content;

	char *g_rand_activity_time_limit_gift_reward_content;
	char *g_rand_activity_time_limit_gift_reward_mail;
	char *g_circulation_chongzhi_not_ra_reward_content;
	char *g_rand_activity_shake_money_reward_content;
	char *g_rand_activity_time_limit_luxury_gifts_content;
	char *g_rand_activity_double_chongzhi_reward_content;
	char *g_rand_activity_expense_nice_gift_mail_content;
	char *g_rand_activity_version_total_charge_reward_content;
	char *g_rand_activity_version_continue_mail_content;
	char *g_rand_activity_huanleyaojiang_2_zhenxi_reward_content;
	char *g_rand_activity_offline_single_charge_reward_content;
	///////////////////////////////////// 累计充值5 /////////////////////////////////////////////////////
	char *g_rand_activity_total_charge5_reward_content;
	///////////////////////////////////////  无双  //////////////////////////////////////////
	char *g_wushuang_fb_pass_reward_content;

	///////////////////////////////////////  鱼池  //////////////////////////////////////////
	char *g_fish_pool_be_steal_content;

	///////////////////////////////////////  世界事件  //////////////////////////////////////////
	char *g_world_event_event_reward_content;
	char *g_world_event_set_yurenka_appearance;
	char *g_world_event_be_set_yurenka_appearance;

	///////////////////////////////////////  红包  /////////////////////////////////////////
	char *g_red_paper_fetch_succ_notice;
	char *g_red_paper_fetch_over_notice;
	char *g_red_paper_lucky_user_notice;
	char *g_red_paper_give_left_money_content;
	char *g_red_paper_create_notice;
	char *g_red_paper_create_notice2;
	char *g_red_paper_singal_chat;
	char *g_command_speak_create_command;

	///////////////////////////////////////  兑换商店  /////////////////////////////////////////
	char *g_convershop_guild_notice;
	char *g_convershop_world_notice;

	///////////////////////////////////////	 一站到底  /////////////////////////////////////////
	char *g_yizhandaodi_fininsh_brocast;
	char *g_yizhandaodi_buff_broast;
	char *g_yizhandaodi_broast_cur_role_num;
	char *g_yizhandaodi_reward_subject;
	char *g_yizhandaodi_reward_rank_content;
	char *g_yizhandaodi_reward_norank_content;
	char *g_yizhandao_jisha_count_notice;
	char *g_yizhandaodi_lucky_notice;
	char *g_yizhandaodi_lucky_reward_subject;
	char *g_yizhandaodi_lucky_reward_content;
	char *g_yizhandaodi_dead_times_reward_content;
	char *g_yizhandaodi_kickout_notice;
	char *g_yizhandaodi_first_replace_notice;
	char *g_yizhandaodi_finish_rank_top1;
	char *g_yizhandaodi_finish_rank_top2;
	char *g_yizhandaodi_finish_rank_top3;
	char *g_yizhandaodi_kickout_mail_content;


	///////////////////////////////////////	 塔防副本  /////////////////////////////////////////
	char *g_team_towerdefend_teammate_level_limit;
	char *g_team_towerdefend_teammate_can_not_enter;
	char *g_team_towerdefend_teammate_join_times_limit;
	char *g_team_towerdefend_teammate_attr_type_error;
	char *g_team_towerdefend_pass_wave;
	char *g_team_towerdefend_pass_reward;

	///////////////////////////////////////	 五行挂机  /////////////////////////////////////////
	char *g_wuxingguaji_kill_boss_notice;
	char *g_wuxingguaji_boss_flush_notice;
	char *g_wuxingguaji_reward_content;
	char *g_wuxingguaji_lucky_notice;
	char *g_wuxingguaji_lucky_reward_subject;
	char *g_wuxingguaji_lucky_reward_content;

	///////////////////////////////////////	 妖兽广场  /////////////////////////////////////////
	char *g_yaoshouguangchang_reward_content;
	char *g_suoyaota_reward_content;

	///////////////////////////////////////	 仙盟运势 /////////////////////////////////////////
	char *g_guild_lucky_info_orange_notice;

	///////////////////////////////////////	 角色转阵营  /////////////////////////////////////////
	char *g_role_change_camp_notice;

	///////////////////////////////////////	 卡牌  /////////////////////////////////////////
	char *g_card_uplevel_notice;

	///////////////////////////////////////	 精华护送  /////////////////////////////////////////
	char *g_jinghua_refresh_notice;
	char *g_jinghua_husong_timeout_subject;
	char *g_jinghua_husong_timeout_content;

	///////////////////////////////////////	 神秘商店  //////////////////////////////////////////
	char *g_mysteriousrefresh_notice;
	char *g_mysteriousshop_refresh_good_item;
	char *g_mysteriousshop_buy_good_item;

	/////////////////////////////////////// 元宝转盘	 ///////////////////////////////////////////
	char *g_gold_turntable_notice;

	///////////////////////////////////////	 土豪金 /////////////////////////////////////////
	char *g_tuhaojin_activate_notic;

	///////////////////////////////////////	 大表情 /////////////////////////////////////////
	char *g_bigchatface_activate_notice;

	///////////////////////////////////////	 神装  /////////////////////////////////////////
	char *g_shenzhuang_downlevel;
	char *g_shenzhuang_jinjie;

	///////////////////////////////////////	 神州六器 /////////////////////////////////////////
	char *g_shenzhou_weapon_gather_content;
	char *g_shenzhou_gather_refresh_content;
	char *g_shenzhou_help_box_reward;
	char *g_shenzhou_gather_top_class_stone;
	char *g_shenzhou_weapon_active;
	char *g_shenzhou_skill_up_level;
	char *g_shenzhou_identify_up_level;
	char *g_shenzhou_lianhun_active;
	char *g_shenzhou_lianhun_level_up;
	char *g_shenzhou_xilian;
	char *g_shenzhou_treasure;

	///////////////////////////////////////	 宝宝 /////////////////////////////////////////
	char *g_baby_upgrade_mail;
	char *g_baby_uplevel_mail;
	char *g_baby_upgrade;
	char *g_baby_subject_mail;
	char *g_baby_qifu_succ;

	///////////////////////////////////////	 魔龙 /////////////////////////////////////////
	char *g_molong_reward_content;
	char *g_molong_reward_content_item;

	///////////////////////////////////////	 宠物 /////////////////////////////////////////
	char *g_pet_upgrade;
	char *g_pet_reward;
	char *g_pet_active_suit;

	///////////////////////////////////////	 牧场精灵 /////////////////////////////////////////
	char *g_pasture_spirit_lucky_draw_content;

	///////////////////////////////////////	 双人坐骑 /////////////////////////////////////////
	char *g_multi_mount_upgrade_content;

	///////////////////////////////////////	 个性窗口 /////////////////////////////////////////
	char *g_personalize_window_bubble_rim_active_content;

	///////////////////////////////////////	 魔卡 /////////////////////////////////////////
	char *g_magic_card_reward_content;

	///////////////////////////////////////	 精灵 /////////////////////////////////////////
	char *g_jingling_halo_upgrade_notice;
	char *g_jingling_fazhen_upgrade_notice;
	char *g_jingling_wuxing_notice1;
	char *g_jingling_wuxing_notice2;
	char *g_jingling_card;
	char *g_jingling_refresh_purple;

	///////////////////////////////////////	 合服活动  /////////////////////////////////////////
	char *g_csa_rank_reward_content;
	char *g_csa_rank_reward_notice_qianggou;
	char *g_csa_gcz_winner_reward_content;
	char *g_csa_gcz_camp_reward_content;
	char *g_csa_xmz_winner_reward_content;
	char *g_csa_xmz_guild_reward_content;
	char *g_csa_gcz_winner_notice;
	char *g_csa_xmz_winner_notice;
	char *g_csa_single_charge_reward_content;
	char *g_csa_charge_reward_double_content;
	char *g_csa_roll_reward_notice;
	char *g_csa_boss_guild_rank_master_reward_content;
	char *g_csa_boss_guild_rank_member_reward_content;
	char *g_csa_touzijihua_auto_fetch_reward_content;
	char *g_csa_foundation_auto_fetch_reward_content;
	char *g_csa_logingift_auto_fetch_reward_content;
	char *g_csa_boss_refresh_greatboss;
	char *g_csa_boss_killall_greatboss;

	char *g_welcome_mail_content;
	char *g_cardzu_active_content;

	/////////////////////////////////////// 跨服 1v1 /////////////////////////////////////////
	char *g_cross_1v1_dur_kill_notice;
	char *g_cross_1v1_end_dur_kill_notice;
	char *g_cross_1v1_xiazhu_reward_subject;
	char *g_cross_1v1_xiazhu_reward_content;
	char *g_cross_1v1_join_reward_subject;
	char *g_cross_1v1_join_reward_content;
	char *g_cross_1v1_week_score_reward_subject;
	char *g_cross_1v1_week_score_reward_content;
	char *g_cross_1v1_week_rank_reward_content;
	char *g_cross_1v1_activity_open_notice;
	char *g_cross_1v1_activity_close_notice;

	///////////////////////////////////////	 跨服修罗塔  /////////////////////////////////////////
	char *g_cross_xiuluo_tower_up_layer_notice;
	char *g_cross_xiuluo_tower_finish_notice;
	char *g_cross_xiuluo_tower_reward_content;
	char *g_cross_xiuluo_tower_rank_reward_content;
	char *g_cross_rank_week_reward_content;
	char *g_cross_xiuluo_tower_get_buff;
	char *g_cross_xiuluo_tower_rap_buff;
	char *g_cross_xiuluo_tower_get_reward;
	char *g_cross_xiuluo_tower_reward_subject;
	char *g_cross_xiuluo_tower_first_reward_subject;
	char *g_cross_xiuluo_tower_kill_boss;
	char *g_cross_xiuluo_tower_boss_refresh;
	char *g_cross_xiuluo_tower_boss_already_refresh;
	char *g_xiuluo_tower_gold_box_reward_mail_content;

	///////////////////////////////////////	 跨服3v3   /////////////////////////////////////////
	char *g_cross_multiuser_challenge_score_rank_reward_subject;
	char *g_cross_multiuser_challenge_score_rank_reward_content;
	char *g_cross_multiuser_challenge_open_notice;
	char *g_cross_multiuser_challenge_close_notice;
	char *g_cross_multiuser_challenge_day_reward_mail_content;
	char *g_cross_multiuser_challenge_finish_reward_mail_content;
	char *g_cross_multiuser_challenge_gongxun_reward_mail_content;
	char *g_cross_multiuser_challenge_season_score_reward_content;
	char *g_cross_multiuser_challenge_season_card_reward_content;

	char *g_cross_team_fb_fine_mate_notice;
	char *g_cross_team_fb_fine_mate_not_capa_notice;

	///////////////////////////////////////	 跨服团战   /////////////////////////////////////////
	char *g_cross_tuanzhan_occupy_pilla_content;
	char *g_cross_tuanzhan_reward_winner_content;
	char *g_cross_tuanzhan_reward_rank_winner_content;
	char *g_cross_tuanzhan_reward_loser_content;
	char *g_cross_tuanzhan_kill_num_content;
	char *g_cross_tuanzhan_reward_by_score_mail_content;
	char *g_cross_tuanzhan_reward_by_score_mail_content2;
	char *g_cross_tuanzhan_reward_by_score_unfetch_content;
	char *g_cross_tuanzhan_broast_camp_change;
	char *g_cross_tuanzhan_broast_camp_switch;

	//////////////////////////////////////// 跨服帮派战 ////////////////////////////////////////
	char *g_cross_guildbattle_own_flag_succc;
	char *g_cross_guildbattle_own_city_succc;
	char *g_cross_guildbattle_on_attack_warning;
	char *g_cross_guildbattle_on_flag_hp_warning;

	///////////////////////////////////////	 跨服BOSS   /////////////////////////////////////////
	char *g_cross_boss_refresh_boss_content;
	char *g_cross_boss_refresh_cross_boss_new;
	char *g_cross_boss_refresh_boss2_content;
	char *g_cross_boss_refresh_boss3_content;
	char *g_cross_boss_treasure_gather_times_left;
	char *g_cross_boss_gather_times_left;
	char *g_be_killed_in_cross_boss;

	/////////////////////////////////////  跨服排行榜		/////////////////////////////////////
	char *g_cross_add_cap_rank_content;
	char *g_cross_add_charm_rank_content;
	char *g_cross_add_couple_rank_qingyuan_content;

	////////////////////////////////////// 月黑风高 /////////////////////////////////////////
	char *g_cross_dark_night_first_boradcast;
	char *g_cross_dark_night_kill_boss;
	char *g_cross_dark_night_kill_role;
	char *g_cross_dark_night_score_reward;
	char *g_cross_dark_night_rank_top1;
	char *g_cross_dark_night_rank_top2;
	char *g_cross_dark_night_rank_top3;

	//////////////////////////////////////// 乱斗战场 ////////////////////////////////////////
	char *g_mess_battle_snatch_score;
	char *g_mess_battle_liangsha_notice1;
	char *g_mess_battle_rank_top1;
	char *g_mess_battle_rank_top2;
	char *g_mess_battle_rank_top3;

	/////////////////////////////////////// 转生 ///////////////////////////////////////
	char *g_zhuansheng_level_up_notice;
	char *g_zhuansheng_notice;
	
	/////////////////////////////////////// 好友送礼 ///////////////////////////////////////
	char *g_friend_gift_shou_content;
	char *g_msg_gift_send_singlechat;
	char *g_msg_friend_song_hua_chat_content;
	char *g_friend_song_hua_content;
	char *g_friend_heli_songhua_content;
	char *g_friend_heli_egg_content;

	/////////////////////////////////////// 套装 ///////////////////////////////////////////////
	char *g_equip_suit_shishi_active_content;
	char *g_equip_suit_chuanshuo_active_content;

	/////////////////////////////////////// 水晶 ///////////////////////////////////////////////
	char *g_big_shuijing_refresh;
	char *g_big_shuijing_already_refresh;
	char *g_big_shuijing_gather;
	char *g_gather_type_shuijing;
	char *g_finish_shuijing_task;

	/////////////////////////////////////// 符文系统 //////////////////////////////////////////
	char *g_runesystem_one_xunbao_notice;
	char *g_runesystem_ten_xunbao_notice;
	char *g_runesystem_pass_layer;
	char *g_runesystem_up_level_send_to_world;
	char *g_runesystem_zhuling;

	/////////////////////////////////////// 日常副本 //////////////////////////////////////////
	char *g_dailyfb_cant_enter_fb;

	/////////////////////////////////////// 组队装备副本 //////////////////////////////////////////
	char *g_team_equip_fb_notice;

	/////////////////////////////////////// 爬塔副本 //////////////////////////////////////////
	char *g_pata_fb_pass_layer;

	/////////////////////////////////////// 神格 //////////////////////////////////////////
	char *g_chou_jiang_msg;
	char *g_shenge_combine_msg;
	char *g_shenge_total_level;
	char *g_shenge_active_new_page;
	char *g_shenge_active_combine;
	char *g_shenge_zhangkong_system_msg;
	char *g_shenge_xilian_system_msg;

	/////////////////////////////////////// 星座 //////////////////////////////////////////
	char *g_xintu_finish_combine;
	char *g_xintu_finish_chapter;
	char *g_xingzuo_update_limit;
	char *g_xingzuo_taozhuang;
	char *g_xingzuo_equip_level;

	/////////////////////////////////////// 封神 //////////////////////////////////////////
	char *g_finish_chapter;

	/////////////////////////////////////// 冲级豪礼 //////////////////////////////////////////
	char *g_chongjihaoli;

	/////////////////////////////////////// 活跃奖励 //////////////////////////////////////////
	char *g_active_degree;

	/////////////////////////////////////// 被杀传闻 //////////////////////////////////////////
	char *g_be_killed_by_role_in_static_scene;
	char *g_be_killed_in_boss_scene;

	/////////////////////////////////////// 神兽		/////////////////////////////////////////
	char *g_shenshou_compose_content;
	char *g_shenshou_huanling_draw;

	/////////////////////////////////////// 典籍翻牌		/////////////////////////////////////////
	char *g_dianji_draw_reward_content;

	///////////////////////////////趣味钓鱼/////////////////////////////////
	char *g_be_steal_fish_content;
	char *g_fishing_broadcast;
	char *g_steal_fish_rank_content;
	char *g_be_steal_fish_rank_content;

	///////////////////////////////植树////////////////////////////////////
	char *g_rand_activity_planting_tree_canyu_reward;
	char *g_rand_activity_planting_tree_planting_reward;
	char *g_rand_activity_planting_tree_watering_reward;

	//////////////////////////////////达拉然水晶//////////////////////////////
	char *g_steal_crystal_dalaran_reward_content;
	char *g_crystal_dalaran_be_stealed_reward_content;
	char *g_steal_crystal_dalaran_broadcast1;
	char *g_steal_crystal_dalaran_broadcast2;
	char *g_steal_crystal_dalaran_broadcast3;
	char *g_be_steal_mail_content;

	///////////////////////////////// 元素之心////////////////////////////////
	char *g_element_heart_xilian;

	///////////////////////////////// 神印系统////////////////////////////////
	char *g_link_shenyin;

	///////////////////////////////////////	 升星助力  /////////////////////////////////////////

	char *g_shengxingzhuli_mount;
	char *g_shengxingzhuli_fazhen;
	char *g_shengxingzhuli_halo;
	char *g_shengxingzhuli_jingling_halo;
	char *g_shengxingzhuli_jingling_fazhen;
	char *g_shengxingzhuli_pifeng;
	char *g_shengxingzhuli_wing;

	char *g_shengxingzhuli_mail_subject;
	char *g_shengxingzhuli_mail_content;


	//////////////////////////////	 上古boss  /////////////////////////////////////////
	char *g_shanggu_boss_hide_flush_notice;
	char *g_refresh_one_boss;

	//////////////////////////////	 守护小鬼  /////////////////////////////////////////
	char *g_impguard_remind_mail_content;
	char *g_get_imp_type_1_notice;
	char *g_get_imp_type_2_notice;

	///////////////////////////////// 每日好礼  ////////////////////////////////////////
	char *g_everyday_nice_gift_fetch_reward;

	/////////////////////////////////  进阶系统奖励	/////////////////////////////////////
	char *g_jinjiesys_reward_active;
	char *g_jinjiesys_reward_mail_content;

	///////////////////////////////////////	 神器特殊形象激活   /////////////////////////////////////////
	char *g_shenqi_special_img_active;

	///////////////////////////////////////	 角色大小目标   /////////////////////////////////////////
	char *g_role_big_small_goal_active_reward;

	//////////////////////////////////// 限时战力过期		/////////////////////////////////////////
	char *g_time_limit_cap_img_mail_context;

	//////////////////////////////////// 神兵图录	/////////////////////////////////////////
	char *g_red_equip_collect_notice_content;
	char *g_red_equip_collect_act_reward_mail_content;
	char *g_red_equip_collect_fetch_title_notice_content;
	char *g_orange_equip_collect_notice_content;
	//////////////////////////////////// 全民比拼   /////////////////////////////////////////
	char *g_mount_national_competition_content;
	char *g_wing_national_competition_content;
	char *g_shizhuang_national_competition_content;
	char *g_shenbing_national_competition_content;
	char *g_fabao_national_competition_content;
	char *g_footprint_national_competition_content;
	char *g_halo_national_competition_content;
	char *g_shengong_national_competition_content;
	char *g_qilinbi_national_competition_content;
	char *g_toushi_national_competition_content;
	char *g_yaoshi_national_competition_content;
	char *g_lingtong_national_competition_content;
	char *g_linggong_national_competition_content;
	char *g_lingqi_national_competition_content;
	char *g_fight_mount_national_competition_content;
	char *g_flypet_national_competition_content;
	char *g_weiyan_national_competition_content;
	char *g_shenyi_national_competition_content;

	//////////////////////////////////// 仙尊卡  /////////////////////////////////////////
	char *g_xian_zun_ka_stale_dated;

	//////////////////////////////////// boss图鉴   /////////////////////////////////////////
	char *g_boss_card_activity_this_type;

	//////////////////////////////////// 装备洗炼  /////////////////////////////////////////
	char *g_equip_baptize_four_purple_notice;
	char *g_equip_baptize_orange_notice;
	char *g_equip_baptize_red_notice;
	char *g_equip_baptize_orange_and_red_notice;

	//////////////////////////////////// 夫妻家园  /////////////////////////////////////////
	char *g_spousehome_buy_room_for_lover;
	char *g_spousehome_equip_furniture_for_lover;

	/////////////////////////////////////// 名将 //////////////////////////////////////////
	char *g_greate_soldier_draw_content;
	char *g_greate_fetch_reward_content;

	///////////////////////////////////////	 跨服随机活动   /////////////////////////////////////////
	char  *g_cross_ra_chongzhi_rank_mvp_reward;
	char  *g_cross_ra_chongzhi_rank_join_reward;
	char  *g_cross_ra_chongzhi_rank_low_mvp_reward;
	char  *g_cross_ra_chongzhi_rank_low_join_reward;

	char  *g_cross_ra_cloud_purchase_return_gold;
	char  *g_cross_ra_cloud_purchase_reward_score;
	char  *g_cross_ra_cloud_purchase_get_reward;
	char  *g_cross_ra_cloud_reward_notic;
	char  *g_cross_ra_cloud_purchase_act_close;

	char  *g_cross_ra_consume_rank_mvp_reward;
	char  *g_cross_ra_consume_rank_join_reward;
	char  *g_cross_ra_consume_rank_low_mvp_reward;
	char  *g_cross_ra_consume_rank_low_join_reward;
	/////////////////////////////////////////////////////////////////////////////////////////////

	char *g_rand_activity_daily_love_2_reward_content;

	///////////////////////////////////// 天书寻主 /////////////////////////////////////////////
	char *g_tianshuxunzhu_fretch_reward;

	/////////////////////////////////////// 幸运云购 ///////////////////////////////////////////////////
	char *g_lucky_cloud_buy_reward_mail;		// 邮件 大奖
	char *g_lucky_cloud_buy_return_mail;		// 邮件 返还元宝
	char *g_lucky_cloud_buy_no_reward_mail;		// 邮件 无中奖告知
	char *g_lucky_cloud_buy_min_reward_mail;	// 邮件 保底奖励
	char *g_lucky_cloud_buy_reward;				// 传闻 大奖
	char *g_lucky_cloud_buy_noreward;			// 传闻 未满人数，返回元宝
	char *g_lucky_cloud_buy_can_join;			// 传闻 本轮开始

	////////////////////////////////////// 神话系统 /////////////////////////////////////////
	char *g_myth_uplevel_broadcast;
	char *g_myth_activate_broadcast;
	char *g_myth_resonance;
	char *g_myth_compose_broadcast;

	///////////////////////////////////////	 周末装备 /////////////////////////////////////////
	char *g_tianshenhuti_combine_notice;
	char *g_weekend_score_boss_rank_reward;

	#define INIT_GAME_STR(PATH, STR, FMT)\
		{\
			bool ret = false;\
			std::string s_tmp;\
			ret = config.get_item("/GameString/" PATH, &s_tmp);\
			if (!ret)\
			{\
				*error = PATH;\
				return false;\
			}\
			STR = new char[(int)s_tmp.size() + 1];\
			strcpy(STR, s_tmp.c_str());\
			ret = CheckFmt(STR, FMT);\
			if (!ret)\
			{\
				*error = PATH;\
				return false;\
			}\
		}

	bool CheckFmt(const char *str, const char *fmt)
	{
		const char *fmt_p = fmt;
		const char *str_p = str;
		while (*fmt_p)
		{
			while (*str_p && *str_p++ != '%');
			if (*str_p != *fmt_p++)
			{
				return false;
			}
		}
		return true;
	}

	bool StringInit(const char *filename, std::string *error)
	{
		XMLConfig config;
		if (!config.read(filename))
		{
			return false;
		}

		///////////////////////////////////////////////    拍卖   /////////////////////////////////////
		INIT_GAME_STR("Global/Mail/PublicSale/ItemFull/Subject", g_publicsale_item_full_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/ItemFull/Content", g_publicsale_item_full_content, "dd");
		INIT_GAME_STR("Global/Mail/PublicSale/ItemCancel/Subject", g_publicsale_item_cancel_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/ItemCancel/Content", g_publicsale_item_cancel_content, "dd");
		INIT_GAME_STR("Global/Mail/PublicSale/ItemExpire/Subject", g_publicsale_item_expire_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/ItemExpire/Content", g_publicsale_item_expire_content, "dd");
		INIT_GAME_STR("Global/Mail/PublicSale/SellSucc/Subject", g_publicsale_sell_succ_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/SellSucc/Content", g_publicsale_sell_succ_content, "ddsdd");
		INIT_GAME_STR("Global/Mail/PublicSale/BuySucc/Subject", g_publicsale_buy_succ_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/BuySucc/Content", g_publicsale_buy_succ_content, "dddd");
		INIT_GAME_STR("Global/Mail/PublicSale/BuyFail/Subject", g_publicsale_buy_fail_subject, "");
		INIT_GAME_STR("Global/Mail/PublicSale/BuyFail/Content", g_publicsale_buy_fail_content, "dddd");


		//////////////////////////////////////////////////   邮件    //////////////////////////////////////
		INIT_GAME_STR("Global/Mail/Mail/ReceiverFull/Subject", g_mail_receiver_full_subject, "");
		INIT_GAME_STR("Global/Mail/Mail/ReceiverFull/Content", g_mail_receiver_full_content, "");


		/////////////////////////////////////////////////   军团    ////////////////////////////////
		INIT_GAME_STR("Global/Mail/Guild/CreateGuild/Subject", g_create_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/CreateGuild/Content", g_create_guild_mail_content, "");
		INIT_GAME_STR("Global/Mail/Guild/DismissGuild/Subject", g_dimiss_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/DismissGuild/Content", g_dimiss_guild_mail_content, "s");
		INIT_GAME_STR("Global/Mail/Guild/AutoDismissGuild/Subject", g_auto_dimiss_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/AutoDismissGuild/Content", g_auto_dimiss_guild_mail_content, "");
		INIT_GAME_STR("Global/Mail/Guild/GmDismissGuild/Subject", g_gm_dimiss_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/GmDismissGuild/Content", g_gm_dimiss_guild_mail_content, "s");
		INIT_GAME_STR("Global/Mail/Guild/GuildDownGrade/Subject", g_guild_downgrade_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/GuildDownGrade/Content", g_guild_downgrade_content, "dd");
		INIT_GAME_STR("Global/Mail/Guild/WarningDismiss/Subject", g_guild_warning_dismiss_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/WarningDismiss/Content", g_guild_warning_dismiss_content, "d");
		INIT_GAME_STR("Global/Mail/Guild/WarningDismissByNotEnoughExp/Subject", g_guild_warning_dismiss_byexp_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/WarningDismissByNotEnoughExp/Content", g_guild_warning_dismiss_byexp_content, "d");
		INIT_GAME_STR("Global/Mail/Guild/UpgradeLevel/Subject", g_guild_upgrade_level_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/UpgradeLevel/Content", g_guild_upgrade_level_content, "d");
		INIT_GAME_STR("Global/Mail/Guild/JoinGuild/Subject", g_join_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/JoinGuild/Content", g_join_guild_mail_content, "s");
		INIT_GAME_STR("Global/Mail/Guild/BeKickedGuild/Subject", g_bekicked_guild_mail_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/BeKickedGuild/Content", g_bekicked_guild_mail_content, "");
		INIT_GAME_STR("Global/Mail/Guild/GuildTuanZhangAppointed/Subject", g_guild_tuanzhang_appointed_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/GuildTuanZhangAppointed/Content", g_guild_tuanzhang_appointed_content, "ss");

		INIT_GAME_STR("Global/Mail/Guild/GuildTuanZhangDelated/Subject", g_guild_tuanzhang_delated_subject, "");
		INIT_GAME_STR("Global/Mail/Guild/GuildTuanZhangDelated/Content", g_guild_tuanzhang_delated_content, "ss");

		INIT_GAME_STR("Global/Guild/CreateGuildSuccPublicNotice", g_create_guild_succ_public_notice, "dsdsd");
		INIT_GAME_STR("Global/Guild/CreateLv2GuildSuccPublicNotice", g_create_lv2_guild_succ_public_notice, "dsdsdd");
		INIT_GAME_STR("Global/Guild/MsgJoinGuild", g_msg_join_guild, "dsd");
		INIT_GAME_STR("Global/Guild/MsgLeaveGuild", g_msg_leave_guild, "dsd");
		INIT_GAME_STR("Global/Guild/MsgAppointedSubject", g_msg_appointed_subject, "");
		INIT_GAME_STR("Global/Guild/MsgFutuanzhangBeAppointed", g_msg_futuanzhang_be_appointed, "dsd");
		INIT_GAME_STR("Global/Guild/MsgZhanglaoBeAppointed", g_msg_zhanglao_be_appointed, "dsd");
		INIT_GAME_STR("Global/Guild/MsgJingyingBeAppointed", g_msg_jingying_be_appointed, "dsd");
		INIT_GAME_STR("Global/Guild/MsgHufaBeAppointed", g_msg_hufa_be_appointed, "dsd");

		INIT_GAME_STR("Global/Guild/MsgGuildLeavePost", g_msg_guild_leave_post, "dsd");
		INIT_GAME_STR("Global/Guild/GuildRenameNotice", g_guild_rename_notice, "dsddsd");
		INIT_GAME_STR("Global/Guild/callBeastContent", g_guild_call_beast_content, "d");
		INIT_GAME_STR("Global/Guild/killBeastContent", g_guild_kill_beast_content, "dsd");
		INIT_GAME_STR("Global/Guild/GuildRenameSubject", g_guild_rename_subject, "");
		INIT_GAME_STR("Global/Guild/GuildRenameContent", g_guild_rename_content, "ss");
		INIT_GAME_STR("Global/Guild/GuildUpLevel", g_guild_guilduplevel, "sd");
		INIT_GAME_STR("Global/Guild/MsgGuildJuanxianBygold", g_msg_guild_juanxian_bygold, "dsddd");
		INIT_GAME_STR("Global/Guild/MsgMemberJuanxianByItem", g_msg_guild_juanxian_byitem, "dsdd");
		INIT_GAME_STR("Global/Guild/MsgMemberJuanxianByCoin", g_msg_guild_juanxian_bycoin, "dsddd");
		INIT_GAME_STR("Global/Guild/GuildOpenBonfire", g_guild_open_bonfire, "ssdddd");
		INIT_GAME_STR("Global/Guild/GuildOpenGuildFB", g_guild_open_guild_fb, "");

		INIT_GAME_STR("Global/Guild/GuildBoxAssistSubject", g_guild_box_assist_subject, "");
		INIT_GAME_STR("Global/Guild/GuildBoxAssistContent", g_guild_box_assist_content, "dsd");
		INIT_GAME_STR("Global/Guild/GuildBoxOpenContent", g_guild_box_open_content, "");
		INIT_GAME_STR("Global/Guild/GuildBossCallContent", g_guild_boss_call_content, "");
		INIT_GAME_STR("Global/Guild/GuildBossRewardContent", g_guild_boss_reward_content, "");

		INIT_GAME_STR("Global/PublicSale/SellToWorld", g_publicsale_sell_to_world, "dsddddd");

		INIT_GAME_STR("Global/Guild/MsgGuildWelcome1", g_msg_guild_welcome1, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome2", g_msg_guild_welcome2, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome3", g_msg_guild_welcome3, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome4", g_msg_guild_welcome4, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome5", g_msg_guild_welcome5, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome6", g_msg_guild_welcome6, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome7", g_msg_guild_welcome7, "");
		INIT_GAME_STR("Global/Guild/MsgGuildWelcome8", g_msg_guild_welcome8, "");

		INIT_GAME_STR("Global/Guild/GuildSoSHusong", g_guild_sos_husong, "dsdddd");

		INIT_GAME_STR("Global/Guild/GuildBossCreate", g_guild_boss_create, "");
		INIT_GAME_STR("Global/Guild/GuildSupperBossCreate", g_guild_supper_boss_create, "");

		INIT_GAME_STR("Global/Guild/GuildCallIn", g_guild_call_in, "dsd");
		INIT_GAME_STR("Global/Guild/GuildMemberKilled", g_guild_memberkilled, "dsddsddsdsddddd");

		INIT_GAME_STR("Global/Guild/GuildStoragePutItem", g_guild_storage_put_item, "dsddsd");
		INIT_GAME_STR("Global/Guild/GuildStorageTakeItem", g_guild_storage_take_item, "dsddsd");
		INIT_GAME_STR("Global/Guild/GuildBossRedbag", g_guild_boss_red_bag, "d");
		INIT_GAME_STR("Global/Guild/GuildPaoSaizi", g_guild_pao_saizi, "sddd");
		INIT_GAME_STR("Global/Guild/GuildPaoSaiziRank3", g_guild_pao_saizi_rank3, "sdsdsd");
		INIT_GAME_STR("Global/Guild/GuildPaoSaiziRank2", g_guild_pao_saizi_rank2, "sdsd");
		INIT_GAME_STR("Global/Guild/GuildPaoSaiziRank1", g_guild_pao_saizi_rank1, "sd");
		INIT_GAME_STR("Global/Guild/GuildSaiziRankMsg1", g_guild_saizi_rank_msg1, "sd");
		INIT_GAME_STR("Global/Guild/GuildSaiziRankMsg2", g_guild_saizi_rank_msg2, "sds");
		INIT_GAME_STR("Global/Guild/GuildSaiziRewardMail", g_guild_saizi_reward_mail, "d");
		INIT_GAME_STR("Global/Guild/GuildMazeRankReward", g_guild_maze_rank_reward, "d");

		INIT_GAME_STR("Global/Guild/guild_flag_first_be_hit", g_guild_flag_first_be_hit_notice, "d")
		INIT_GAME_STR("Global/Guild/guild_flag_be_attacked_notice", g_guild_flag_be_attacked_notice, "dd")
		INIT_GAME_STR("Global/Guild/guild_flag_be_occupied", g_guild_flag_be_occupied_notice, "d")
		INIT_GAME_STR("Global/Guild/officer_be_killed_notice_content", g_guild_officer_be_killed, "ddsd");
		INIT_GAME_STR("Global/Guild/chenyuan_be_killed_notice_content", g_guild_chengyuan_be_killed, "ddsdsddddd");
		INIT_GAME_STR("Global/Guild/guild_box_invite_assist", g_guild_box_invite_assist, "dsd");

		INIT_GAME_STR("Global/Guild/QingyuanRank_Top3_Notice", g_qingyuan_rank_top3, "dsddsddd");
		INIT_GAME_STR("Global/Guild/Add_Capability_Rank_Top3_Notice", g_add_capability_rank_top3, "dsddd");
		INIT_GAME_STR("Global/Guild/Add_Charm_Rank_Top3_Notice", g_add_charm_rank_top3, "dsddd");

		INIT_GAME_STR("Global/Guild/guild_activity_team_reward_notice", g_guild_activity_team_reward_notice, "ddd");

		///////////////////////////////////////	 排行榜称号  /////////////////////////////////////////
		INIT_GAME_STR("Global/Rank/top_new_first_broadcast", g_toprank_new_first_broadcast, "dsdd");
		INIT_GAME_STR("Global/Rank/top_change_first_broadcast", g_toprank_change_first_broadcast, "dsddsdd");
		INIT_GAME_STR("Global/Rank/g_toprank_capability_top_user_changed_notice", g_toprank_capability_top_user_changed_notice, "dsddsd");
		INIT_GAME_STR("Global/Rank/top_capability_user_login", g_toprank_capability_user_login, "dsd");
		INIT_GAME_STR("Global/Rank/top_ten_capability_user_login", g_toprank_ten_capability_user_login, "ds");
		INIT_GAME_STR("Global/Rank/g_meilirank_capability_user_login", g_meilirank_capability_user_login, "dsd");
		INIT_GAME_STR("Global/Rank/man_charm_rank_title", g_man_charm_rank_title, "");
		INIT_GAME_STR("Global/Rank/girl_charm_rank_title", g_girl_charm_rank_title, "");
		INIT_GAME_STR("Global/Rank/lose_charm_rank_title", g_lose_charm_rank_title, "");
		INIT_GAME_STR("Global/Rank/add_world_question_rank_title", g_add_world_question_rank_title, "");
		INIT_GAME_STR("Global/Rank/lose_world_question_rank_title", g_lose_world_question_rank_title, "");

		INIT_GAME_STR("GameWorld/Mentality/gengu_broadcast", g_gengu_broadcast, "dsddd");
		INIT_GAME_STR("GameWorld/Mentality/mentality_upgrade_max", g_mentality_upgrade_max, "dsddd");
		INIT_GAME_STR("GameWorld/Mentality/mentality_combine_upgrade", g_mentality_combine_upgrade, "dsddd");
		INIT_GAME_STR("GameWorld/Mentality/mentality_combine_learn_skill", g_mentality_combine_learn_skill, "dsdddd");

		//////////////////////////////////////   好友经验瓶  /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/friend_exp_bottle/search_friend", g_friend_exp_bottle_search_friend, "dsdd");
		
		//////////////////////////////////////////////////   传闻链接   //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Link/upstar", g_link_upstar, "dsddsd");
		INIT_GAME_STR("GameWorld/Link/fuling", g_link_fuling, "dsdds");
		INIT_GAME_STR("GameWorld/Link/lieming", g_link_lieming, "dsdd");
		INIT_GAME_STR("GameWorld/Link/jingling_uplevel", g_link_jingling_uplevel, "dsddsd");
		INIT_GAME_STR("GameWorld/Link/jingling_upgrade", g_link_jingling_upgrade, "dsddd");
		INIT_GAME_STR("GameWorld/Link/jingling_strength", g_link_jingling_strength, "dsddd");
		INIT_GAME_STR("GameWorld/Link/chengjiu_title", g_link_chengjiu_title, "dsdd");
		INIT_GAME_STR("GameWorld/Link/xianjie", g_link_xianjie, "dsdd");
		INIT_GAME_STR("GameWorld/Link/inherit", g_link_inherit, "dsdds");
		INIT_GAME_STR("GameWorld/Link/jingling_phantom", g_link_jingling_phantom, "dsdd");
		INIT_GAME_STR("GameWorld/Link/jingling_soul", g_link_jingling_soul, "dsdd");

		//////////////////////////////////////////////////   寻宝   //////////////////////////////////////
		INIT_GAME_STR("GameWorld/ChestShop/ChujiEquipGetItemBroadcast", g_chuji_equip_chestshop_item_broadcast, "dsds");
		INIT_GAME_STR("GameWorld/ChestShop/ChujiJinglingGetItemBroadcast", g_chuji_jingling_chestshop_item_broadcast, "dsds");
		INIT_GAME_STR("GameWorld/ChestShop/GaojiGetItemBroadcast", g_gaoji_chestshop_item_broadcast, "dsds");
		INIT_GAME_STR("GameWorld/ChestShop/GetXiannvCardNotice", g_chestshop_get_xiannv_notice, "dsdd");
		INIT_GAME_STR("GameWorld/ChestShop/GetXiannvCardContent", g_chestshop_get_xiannv_content, "dsdd");

		INIT_GAME_STR("GameWorld/YaoJiang/yaojiang_notice", g_yaojiang_notice, "dsdd");

		//////////////////////////////////////////////////   怪物    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Monster/KillCommonSceneBoss", g_kill_common_scene_boss, "dsds");
		INIT_GAME_STR("GameWorld/Monster/KillFBBoss", g_kill_fb_boss, "ss");
		INIT_GAME_STR("GameWorld/Monster/RefreshBoss", g_refresh_boss, "ss");
		INIT_GAME_STR("GameWorld/Monster/PickBodyDrop", g_pick_body_drop, "dsdddddd");
		INIT_GAME_STR("GameWorld/Monster/PickBodyDropEquipment", g_pick_body_drop_equipment, "dsdddsddd");

		//////////////////////////////////////////////////   怪物    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Combat/KillRole", g_common_scene_kill_role, "dsddsddsdddsd");

		//////////////////////////////////////////////////   物品    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Item/stone_up_level", g_item_stone_up_level, "dsdd");
		INIT_GAME_STR("GameWorld/Item/equip_strengthen", g_item_equip_strengthen, "dsddd");
		INIT_GAME_STR("GameWorld/Item/equip_strength_total", g_item_equip_total_strengthen, "dsdddd");
		INIT_GAME_STR("GameWorld/Item/equip_compound_1", g_item_equip_compound_1, "dsddsds");
		INIT_GAME_STR("GameWorld/Item/equip_compound_2", g_item_equip_compound_2, "dsddsdds");
		INIT_GAME_STR("GameWorld/Item/equip_shenzhu_total", g_item_equip_shenzhu_total, "dsdd");
		INIT_GAME_STR("GameWorld/Item/equip_shenzhu_up_shen_level", g_item_equip_shenzhu_up_shen_level, "dsddsdd");
		INIT_GAME_STR("GameWorld/Item/equip_upstar_total", g_item_equip_upstar_total, "dsdddd");
		INIT_GAME_STR("GameWorld/Item/equip_up_star", g_item_equip_up_star, "dsddsd");
		INIT_GAME_STR("GameWorld/Item/equip_uplevel", g_item_equip_uplevel, "dsdds");
		//INIT_GAME_STR("GameWorld/Item/equip_up_quality", g_item_equip_upquality, "dsddsd");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_1", g_item_equip_upquality_1, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_2", g_item_equip_upquality_2, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_3", g_item_equip_upquality_3, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_4", g_item_equip_upquality_4, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_5", g_item_equip_upquality_5, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/equip_up_quality_6", g_item_equip_upquality_6, "dsdsds");
		INIT_GAME_STR("GameWorld/Item/pick", g_item_pick, "dsddd");
		INIT_GAME_STR("GameWorld/Item/pick_equipment", g_item_pick_equipment, "dsddds");
		INIT_GAME_STR("GameWorld/Item/pick_equipment_send_to_guild", g_item_get_equipment_send_to_guild, "sddddddds");
		INIT_GAME_STR("Global/Mail/Item/invalid_subiect", g_item_invalid_subiect, "");
		INIT_GAME_STR("Global/Mail/Item/invalid_content", g_item_invalid_content, "s");
		INIT_GAME_STR("GameWorld/Item/stone_up_level_single_part", g_item_stone_up_level_single_part, "dsddd"); 
		INIT_GAME_STR("GameWorld/Item/stone_up_level_consume", g_item_stone_up_level_consume, "");
		INIT_GAME_STR("GameWorld/Item/stone_up_level_consume_has_diamond", g_item_stone_up_level_consume_has_diamond, "d");
		INIT_GAME_STR("GameWorld/Item/eternity_up_level", g_item_eternity_up_level, "dsddsd");
		INIT_GAME_STR("GameWorld/Item/eternity_up_level_1", g_item_eternity_up_level_1, "dsddsds");
		INIT_GAME_STR("GameWorld/Item/eternity_suit", g_item_eternity_suit, "dsds");

		//////////////////////////////////////////////// 羽翼 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/WingInfo/wing_upgrade", g_wing_upgrade, "dsdddd");
		INIT_GAME_STR("GameWorld/WingInfo/wing_active_content", g_wing_active_content, "dsddd");

		//////////////////////////////////////////////// 女神 ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/XiannvInfo/xiannv_active_content", g_xiannv_active_content, "dsdd"); 
		INIT_GAME_STR("GameWorld/XiannvInfo/xiannv_shengwu_miling", g_xiannv_shengwu_miling, "dsdd");
		INIT_GAME_STR("GameWorld/XiannvInfo/xiannv_shengwu_upgrade", g_xiannv_shengwu_upgrade, "dsddd");
		INIT_GAME_STR("GameWorld/XiannvInfo/xiannv_shengwu_skill_upgrade", g_xiannv_shengwu_skill_upgrade, "dsddd");
		INIT_GAME_STR("GameWorld/XiannvInfo/xiannv_gongming_upgrade", g_xiannv_gongming_upgrade, "dsdd");

		//////////////////////////////////////////////// 个人目标 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RoleGoal/reward_mail_content", g_role_goal_reward_content, "");

		//////////////////////////////////////////////// 光环 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/HaloInfo/halo_upgrade", g_halo_upgrade, "dsdddd");

		//////////////////////////////////////////////// 神弓 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShengongInfo/shengong_upgrade", g_shengong_upgrade, "dsddd");

		//////////////////////////////////////////////// 神翼 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenyiInfo/shenyi_upgrade", g_shenyi_upgrade, "dsddd");

		//////////////////////////////////////////////// 足迹 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/FootPrint/footprint_upgrade", g_footprint_upgrade, "dsddd");

		//////////////////////////////////////////////// 形象赋灵	/////////////////////////////////////
		INIT_GAME_STR("GameWorld/ImgFuling/skill_level_up", g_img_fuling_skill_level_up, "dsdddd");
		INIT_GAME_STR("GameWorld/ImgFuling/level_up", g_img_fuling_level_up, "dsddd");

		//////////////////////////////////////////////// 披风 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Cloak/cloak_uplevel", g_cloak_up_level, "dsdd");
		INIT_GAME_STR("GameWorld/Cloak/cloak_image", g_cloak_imge, "dsddd");

		//////////////////////////////////////////////// 灵刃 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenBingInfo/shenbing_level_up", g_shenbing_level_up, "dsdd");

		//////////////////////////////////////////////// 神兵 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenBingUpgrade/shenbing_upgrade", g_shenbing_upgrade, "dsddd");

		//////////////////////////////////////////////// 战斗坐骑 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/FightMountInfo/fight_mount_upgrade", g_fight_mount_upgrade, "dsdddd");

		////////////////////////////////////////////////  VIP  ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/VipInfo/vip_uplevel", vip_uplevel, "dsdd");
		INIT_GAME_STR("GameWorld/VipInfo/vip_uplevel_personal", vip_uplevel_personal, "d");
		INIT_GAME_STR("GameWorld/VipInfo/gold_vip_active_content", g_gold_vip_active_content, "dsd");
		INIT_GAME_STR("GameWorld/VipInfo/time_limit_vip_active_content", g_time_limit_vip_active_content, "dsd");

		//////////////////////////////////////////////////   背包    //////////////////////////////////////
		INIT_GAME_STR("Global/Mail/Knapsack/put_subiect", g_knapsack_put_subiect, "");
		INIT_GAME_STR("Global/Mail/Knapsack/put_content", g_knapsack_put_content, "");

		//////////////////////////////////////////////////   角色    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Role/zhuanzhi", g_zhuanzhi, "dsdd");
		INIT_GAME_STR("GameWorld/Role/change_prof", g_change_prof, "dsddd");

		//////////////////////////////////////////////////   时装    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Shizhuang/upgrade", g_shizhuang_upgrade, "dsddd");

		//////////////////////////////////////////////////   坐骑    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Mount/mount_upgrade", g_mount_upgrade, "dsdddd");
		INIT_GAME_STR("GameWorld/Mount/mount_fly_up", g_mount_fly_up, "dsddd");
		INIT_GAME_STR("GameWorld/Mount/qibing_upgrade", g_qibing_upgrade, "dsddd");
		INIT_GAME_STR("GameWorld/Mount/mount_active_content", g_mount_active_content, "dsddd");

		//////////////////////////////////////////////////   魔戒    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Mojie/mojie_uplevel", g_mojie_uplevel, "dsdddd");

		//////////////////////////////////////////////////	 杂项  ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Sundry/bekilled_notice_subject", g_bekilled_notice_subject, "");
		INIT_GAME_STR("GameWorld/Sundry/bekilled_notice_content", g_bekilled_notice_content, "dsd");
		INIT_GAME_STR("GameWorld/Sundry/evil_bekilled_notice_content", g_evil_bekilled_notice_content, "dsdd");

		INIT_GAME_STR("GameWorld/Sundry/flower_notice", g_flower_notice, "dsddsddsdddsd");
		INIT_GAME_STR("GameWorld/Sundry/flower_notice_anonymity", g_flower_notice_anonymity, "dsdd");
		INIT_GAME_STR("GameWorld/team/team_create_notice", g_team_create_notice, "d");

		//////////////////////////////////////////////////   法宝    //////////////////////////////////////
		INIT_GAME_STR("GameWorld/Fabao/upgrade", g_fabao_upgrade, "dsdddd");

		//////////////////////////////////////////////////	充值  ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Chongzhi/chongzhi_reward_subject", g_chongzhi_reward_subject, "");
		INIT_GAME_STR("GameWorld/Chongzhi/chongzhi_reward_content", g_chongzhi_reward_content, "dd");
		INIT_GAME_STR("GameWorld/Chongzhi/chongzhi_reward_coin_content", g_chongzhi_reward_coin_content, "dd");
		INIT_GAME_STR("GameWorld/Chongzhi/special_first_recharge_reward_content", g_chongzhi_special_first_recharge_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/daily_first_recharge_reward_content", g_chongzhi_daily_first_recharge_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/daily_total_recharge_reward_content", g_chongzhi_daily_total_recharge_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/daily_total_recharge_send_to_world", g_chongzhi_daily_total_recharge_send_to_world, "dsdd");
		INIT_GAME_STR("GameWorld/Chongzhi/first_chongzhi_reward_content", g_chongzhi_first_chongzhi_reward_content, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/zai_chongzhi_reward_content", g_chongzhi_zai_chongzhi_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/total_chongzhi_reward_content", g_chongzhi_total_chongzhi_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/danbi_chongzhi_reward_content", g_chongzhi_danbi_chongzhi_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/Chongzhi/weeekday_chongzhi_reward_content", g_chongzhi_diff_weeekday_chongzhi_reward_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/san_chongzhi_reward_content", g_chongzhi_san_chongzhi_reward_content, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/daily_recharge_reward2_content", g_chongzhi_daily_recharge_reward2_content, "");
		INIT_GAME_STR("GameWorld/Chongzhi/touzi/touzi_level", g_touzi_level, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/touzi/touzi_week_card", g_touzi_week_card, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/touzi/touzi_fb_plan", g_touzi_fb_plan, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/touzi/touzi_boss_plan", g_touzi_boss_plan, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/touzi/touzi_shenyu_boss_plan", g_touzi_shenyu_boss_plan, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/zero_gift", g_zero_gift, "dsd");
		INIT_GAME_STR("GameWorld/Chongzhi/discount_buy", g_discount_buy, "dsdd");
		INIT_GAME_STR("GameWorld/Chongzhi/discount_buy_mail_content", g_discount_buy_mail_content, "d");
		INIT_GAME_STR("GameWorld/Chongzhi/time_limit_gift", g_time_limit_gift, "dsdd");
		INIT_GAME_STR("GameWorld/Chongzhi/time_limit_gift_2", g_time_limit_gift_2, "dsdd");
		INIT_GAME_STR("GameWorld/Chongzhi/panic_buy", g_panic_buy, "dsd");

		//////////////////////////////////////////////////	 开随机礼包  ////////////////////////////////////////////
		UNSTD_STATIC_CHECK(2 == OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM - 1);
		INIT_GAME_STR("GameWorld/OpenRandGift/randgift_1", g_open_randgift[1], "dsddd");
		INIT_GAME_STR("GameWorld/OpenRandGift/randgift_2", g_open_randgift[2], "dsddd");

		///////////////////////////////////////	 群仙乱斗  ///////////////////////////////////////////////////
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/score_reward_subject", g_qunxianluandou_score_reward_subject, "");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/score_reward_content", g_qunxianluandou_score_reward_content, "dd");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/side_win_reward_subject", g_qunxianluandou_side_win_subject, "");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/side_win_reward_content", g_qunxianluandou_side_win_content, "");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/side_lose_reward_subject", g_qunxianluandou_side_lose_subject, "");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/side_lose_reward_content", g_qunxianluandou_side_lose_content, "");
		INIT_GAME_STR("Global/Mail/QunXianLuanDou/qunxian_open_server", g_qunxianluandou_qunxian_open_server, "");
		INIT_GAME_STR("GameWorld/QunXianLuanDou/first_side", g_qunxianluandou_first_side, "d");

		////////////////////////////////////// 怪物入侵 ///////////////////////////////////////////////////////
		INIT_GAME_STR("Global/MONSTER_INVADE//monster_refresh", g_monster_invade_refresh, "");

		////////////////////////////////////// 大富豪 ///////////////////////////////////////////////////////
		INIT_GAME_STR("Global/MILLIONAIRE//millionaire_boss_refresh", g_millionaire_boss_refresh, "d");
		INIT_GAME_STR("Global/MILLIONAIRE//millionaire_frozen", g_millionaire_frozen, "dsddsddsd");
		INIT_GAME_STR("Global/MILLIONAIRE//millionaire_gather_times", g_millionaire_gather_times, "dsdd");
		INIT_GAME_STR("Global/MILLIONAIRE//millionaire_gather_good", g_millionaire_gather_good, "dsdd");
		INIT_GAME_STR("Global/MILLIONAIRE//millionaire_kill_boss", g_millionaire_kill_boss, "ddsd");

		////////////////////////////////////// 走棋子 ///////////////////////////////////////////////////////
		INIT_GAME_STR("Global/MOVECHESS//get_good_item", g_movechess_good_item, "dsdd");

		////////////////////////////////////// 答题 ///////////////////////////////////////////////////////
		INIT_GAME_STR("Global/QUESTION//question_rank_broadcast", g_question_rank_broadcast, "dsd");
		INIT_GAME_STR("Global/QUESTION//question_rank_reward", g_question_rank_reward, "");
		INIT_GAME_STR("Global/QUESTION//fast_world_question_reward", g_question_fast_world_reward, "dsddsddsd");
		INIT_GAME_STR("Global/QUESTION//fast_guild_question_reward", g_question_fast_guild_reward, "dsddsddsd");
		INIT_GAME_STR("Global/QUESTION//fast_world_question_reward_mail", g_question_fast_world_reward_mail, "d");
		INIT_GAME_STR("Global/QUESTION//fast_guild_question_reward_mail", g_question_fast_guild_reward_mail, "d");
		INIT_GAME_STR("Global/QUESTION//guild_question_rank_reward", g_question_guild_rank_reward, "d");
		INIT_GAME_STR("Global/QUESTION//guild_question_rank_other_reward", g_question_guild_rank_other_reward, "");
		INIT_GAME_STR("Global/QUESTION//question_system_message_world", g_question_system_message_world, "s");
		INIT_GAME_STR("Global/QUESTION//question_system_message_guild", g_question_system_message_guild, "s");

		//////////////////////////////////////////////////	 money  ////////////////////////////////////////////
		INIT_GAME_STR("Global/Mail/Money/chongzhi_subject", g_money_chongzhi_subject, "");
		INIT_GAME_STR("Global/Mail/Money/chongzhi_content", g_money_chongzhi_content, "d");

		///////////////////////////////////////	 月卡  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/MonthCard/RewardSubjectPerDay", g_month_card_reward_per_day_subject, "");
		INIT_GAME_STR("GameWorld/MonthCard/RewardContentPerDay", g_month_card_reward_per_day_content, "");
		INIT_GAME_STR("GameWorld/MonthCard/RewardSubjectOnBuy", g_month_card_reward_on_buy_subject, "");
		INIT_GAME_STR("GameWorld/MonthCard/RewardContentOnBuy", g_month_card_reward_on_buy_content, "");
		INIT_GAME_STR("GameWorld/MonthCard/BuyMonthCard", g_month_card_buy_notice, "dsd");

		///////////////////////////////////////	 排名竞技场  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ChallengeField/LuckRewardSubject", g_challenge_field_lucky_reward_subject, "");
		INIT_GAME_STR("GameWorld/ChallengeField/LuckRewardContent", g_challenge_field_lucky_reward_content, "");
		INIT_GAME_STR("GameWorld/ChallengeField/RankRewardSubject", g_challenge_field_rank_reward_subject, "");
		INIT_GAME_STR("GameWorld/ChallengeField/RankRewardContent", g_challenge_field_rank_reward_content, "d");
		INIT_GAME_STR("GameWorld/ChallengeField/RankDropSubject", g_challenge_field_rank_drop_subject, "");
		INIT_GAME_STR("GameWorld/ChallengeField/RankDropContent", g_challenge_field_rank_drop_content, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/RankPosNotice", g_challenge_field_rank_pos_notice, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/RankPosNotice1", g_challenge_field_rank_pos_notice1, "sss");
		INIT_GAME_STR("GameWorld/ChallengeField/RankPosNotice2", g_challenge_field_rank_pos_notice2, "ss");
		INIT_GAME_STR("GameWorld/ChallengeField/RankPosNotice3", g_challenge_field_rank_pos_notice3, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/CloseRankPosNotice1", g_challenge_field_close_rank_pos_notice1, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/CloseRankPosNotice2", g_challenge_field_close_rank_pos_notice2, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/CloseRankPosNotice3", g_challenge_field_close_rank_pos_notice3, "s");
		INIT_GAME_STR("GameWorld/ChallengeField/RankPosFirstChanged", g_challenge_field_rank_pos_first_changed, "dsdddsd");
		INIT_GAME_STR("GameWorld/ChallengeField/LianShengNotice", g_challenge_field_liansheng_notice, "dsdd");
		INIT_GAME_STR("GameWorld/ChallengeField/BestRankRewardContent", g_challenge_field_best_rank_reward_content, "dd");
		INIT_GAME_STR("GameWorld/ChallengeField/CloseRankRewardContent", g_challenge_field_rank_close_reward_content, "d");

		///////////////////////////////////////	 决斗场-挑衅积分排名  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Fighting/ChallengeRankRewardContent", g_fighting_challenge_rank_rward_content, "d");

		//////////////////////////////////////////////////	 正确提示  ////////////////////////
		INIT_GAME_STR("GameWorld/SuccNotice/mentility_lift", g_succnotice_mentility_lift, "dsddd");

		//////////////////////////////////////////////////	 错误提示  ////////////////////////
		INIT_GAME_STR("GameWorld/ErrorNotice/enter_scene_limit_level", g_errornotice_enter_scene_limit_level, "d");
		INIT_GAME_STR("GameWorld/ErrorNotice/enter_fb_limit_level", g_errornotice_enter_fb_limit_level, "d");
		INIT_GAME_STR("GameWorld/ErrorNotice/lack_item", g_errornotice_lack_item, "d");

		//////////////////////////////////////////////////	 世界BOSS   ////////////////////////////////////////////
		INIT_GAME_STR("Global/Boss/kill_world_boss", g_kill_world_boss, "dsdd");
		INIT_GAME_STR("Global/Boss/person_hurt_reward", g_worldboss_person_hurt_reward, "");
		INIT_GAME_STR("Global/Boss/guild_hurt_reward", g_worldboss_guild_hurt_reward, "");
		INIT_GAME_STR("Global/Boss/killer_guild_reward", g_worldboss_killer_guild_reward, "");
		INIT_GAME_STR("Global/Boss/week_rank_reward", g_worldboss_week_rank_reward, "");
		INIT_GAME_STR("Global/Boss/red_paper_reward", g_worldboss_red_paper_reward, "");
		INIT_GAME_STR("Global/Boss/hudun_reward", g_worldboss_hudun_reward, "");
		INIT_GAME_STR("Global/Boss/hudun_reward_broadcast", g_worldboss_hudun_reward_broadcast, "dsdddd");
		INIT_GAME_STR("Global/Boss/world_boss_refresh", g_worldboss_refresh, "");
		INIT_GAME_STR("Global/Boss/cross_boss_enter_notice", g_cross_boss_enter_notice, "dsdd");
		INIT_GAME_STR("Global/Boss/cross_boss_enter_notice_2", g_cross_boss_enter2_notice, "dsdd");
		INIT_GAME_STR("Global/Boss/cross_boss_enter_notice_3", g_cross_boss_enter3_notice, "dsdd");
		INIT_GAME_STR("Global/Boss/dabao_boss_refresh_notice", g_dabao_boss_refresh_notice, "d");
		INIT_GAME_STR("Global/Boss/precious_boss_refresh", g_precious_boss_refresh, "");
		INIT_GAME_STR("Global/Boss/active_boss_rank_reward_mail_context", g_active_boss_rank_reward_mail_context, "dd");

		///////////////////////////////////////	 上古boss  ////////////////////////////////////////////////////////////////////
		INIT_GAME_STR("Global/Boss/shanggu_boss_hide_flush_notice", g_shanggu_boss_hide_flush_notice, "dsddd");
		INIT_GAME_STR("Global/Boss/refresh_one_boss", g_refresh_one_boss, "dd");
		
		///////////////////////////////////////	 角色改名  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RoleResetName/role_rename_notice", g_role_rename_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/RoleResetName/role_rename_subject", g_role_rename_subject, "");
		INIT_GAME_STR("GameWorld/RoleResetName/role_rename_content", g_role_rename_content, "ss");

		///////////////////////////////////////	 角色变性  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RoleSetSex/role_set_sex_notice", g_role_set_sex_notice, "dsd");
		INIT_GAME_STR("GameWorld/RoleSetSex/role_set_sex_subject", g_role_set_sex_subject, "");
		INIT_GAME_STR("GameWorld/RoleSetSex/role_set_sex_content", g_role_set_sex_content, "dsd");

		///////////////////////////////////////	 名人堂  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_1", g_famous_man_type_1, "dsdd");
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_2", g_famous_man_type_2, "dsdd");
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_3", g_famous_man_type_3, "dsdd");
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_4", g_famous_man_type_4, "dsddd");
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_5", g_famous_man_type_5, "dsdd");
		INIT_GAME_STR("GameWorld/FamousMan/famous_man_type_6", g_famous_man_type_6, "dsdd");

		//////////////////////////////////////  护送  ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/HuSong/accept_notice", g_husong_accept_notice, "dsd");
		INIT_GAME_STR("GameWorld/HuSong/rob_notice", g_husong_rob_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/HuSong/berob_mail/subject", g_husong_berob_mail_subject, "");
		INIT_GAME_STR("GameWorld/HuSong/berob_mail/content", g_husong_berob_mail_content, "dsd");
		INIT_GAME_STR("GameWorld/HuSong/rob_mail/subject", g_husong_rob_mail_subject, "");
		INIT_GAME_STR("GameWorld/HuSong/rob_mail/content", g_husong_rob_mail_content, "dsd");
		INIT_GAME_STR("GameWorld/HuSong/rob_mail/level_limit_content", g_husong_rob_mail_level_limit_content, "d");
		INIT_GAME_STR("GameWorld/HuSong/rob_mail/times_limit_content", g_husong_rob_mail_times_limit_content, "d");
		INIT_GAME_STR("GameWorld/HuSong/overtime_mail/subject", g_husong_overtime_mail_subject, "");
		INIT_GAME_STR("GameWorld/HuSong/overtime_mail/content", g_husong_overtime_mail_content, "");
		INIT_GAME_STR("GameWorld/HuSong/refresh_comsume", g_husong_refresh_comsume, "dd");

		////////////////////////////////////////  日常任务  ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/DailyTask/task_finish_subject", g_daily_task_finish_subject, "");
		INIT_GAME_STR("GameWorld/DailyTask/task_finish_content", g_daily_task_finish_content, "");

		//////////////////////////////////////  跑环任务  ////////////////////////////////////////////
		
		INIT_GAME_STR("GameWorld/PaohuanTask/task_phase_finish_subject", g_paohuan_task_phase_subject, "");
		INIT_GAME_STR("GameWorld/PaohuanTask/task_phase_finish_content", g_paohuan_task_phase_content, "");
		INIT_GAME_STR("GameWorld/PaohuanTask/task_finish_notice", g_paohuan_task_finish_notice, "dsd");
		

		///////////////////////////////////////	 副本  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/EquipFB/equip_fb_subject", g_equipfb_compensation_roll_subject, "");
		INIT_GAME_STR("GameWorld/EquipFB/equip_fb_mail_content", g_equipfb_compensation_roll_context, "dd");
		INIT_GAME_STR("GameWorld/EquipFB/pass_fb", g_new_equip_fb_pass, "dsdd");

		INIT_GAME_STR("GameWorld/RuneTower/runetower_first_reward", g_runetower_first_reward, "d");
		
		///////////////////////////////////////	 转职副本  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ZhuanzhiFB/teammate_can_not_enter", g_zhuanzhifb_teammate_can_not_enter, "dsd");

		///////////////////////////////////////	 pvp战场  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice1", g_pvpzhanchang_lianzhan_notice1, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice2", g_pvpzhanchang_lianzhan_notice2, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice3", g_pvpzhanchang_lianzhan_notice3, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice4", g_pvpzhanchang_lianzhan_notice4, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice5", g_pvpzhanchang_lianzhan_notice5, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice6", g_pvpzhanchang_lianzhan_notice6, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice7", g_pvpzhanchang_lianzhan_notice7, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/liangzhan_notice8", g_pvpzhanchang_lianzhan_notice8, "dsdd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/lianzhan_notice", g_pvpzhanchang_lianzhan_notice, "dsddd");
		INIT_GAME_STR("GameWorld/pvp_zhanchang/stop_lianzhan_notice", g_pvpzhanchang_stoplianzhan_notice, "dsddsdd");

		INIT_GAME_STR("GameWorld/QunXianLuanDou/WeakSideProtectNotice", g_qunxian_luandou_weak_side_protect_notice, "ddd");
		INIT_GAME_STR("GameWorld/QunXianLuanDou/ShengShiDisappear", g_qunxian_luandou_shenshi_disappear, "dsd");

		///////////////////////////////////////	 攻城战  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/GongChengZhan/finish_occupy_notice", g_gongchengzhan_finish_occupy_notice, "dsdsd");
		INIT_GAME_STR("GameWorld/GongChengZhan/zhangong_reward_content", g_zhangong_reward_content, "d");
		INIT_GAME_STR("GameWorld/GongChengZhan/win_huiyuan_reward_content", g_win_huiyuan_reward_content, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/win_huizhang_reward_content", g_win_huizhang_reward_content, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/gongchengzhan_team_reward_content", g_gongchengzhan_team_reward_content, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/last_chenzhu_reward_content", g_last_chenzhu_reward_content, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/gcz_rankreward_open_server", g_gcz_rankreward_open_server, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/fail_reward_content", g_fail_reward_content, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/gcz_pomen_shou_notice", g_gcz_pomen_shou_notice, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/gcz_pomen_gong_notice", g_gcz_pomen_gong_notice, "");
		INIT_GAME_STR("GameWorld/GongChengZhan/gcz_zhanqi_notice", g_gcz_zhanqi_notice, "ds");
		
		///////////////////////////////////////	 魔戒  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/MoJieFb/jinglingpian_reward_content", g_mojie_jinglingpian_reward_notice, "dds");
		

		///////////////////////////////////////	 公会争霸  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/GuildBattle/guildbattle_close", g_guildbattle_close, "s");
		INIT_GAME_STR("GameWorld/GuildBattle/guildbattle_rankreward_guild", g_guildbattle_rankreward_guild, "d");
		INIT_GAME_STR("GameWorld/GuildBattle/guildbattle_collective_reward", g_guildbattle_collective_reward, "");
		INIT_GAME_STR("GameWorld/GuildBattle/guildbattle_rankreward_person", g_guildbattle_rankreward_person, "");
		INIT_GAME_STR("GameWorld/GuildBattle/guildbattle_rankreward_open_server", g_guildbattle_rankreward_open_server, "");
		INIT_GAME_STR("GameWorld/GuildBattle/monster_refresh_prepare_notice", g_guildbattle_monster_refresh_prepare_notice, "dd");
		INIT_GAME_STR("GameWorld/GuildBattle/monster_refresh_notice", g_guildbattle_monster_refresh_notice, "d");
		INIT_GAME_STR("GameWorld/GuildBattle/kill_boss_notice", g_guildbattle_kill_boss_notice, "s");
		INIT_GAME_STR("GameWorld/GuildBattle/get_glod_box_notice", g_guildbattle_get_glod_box_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/GuildBattle/commit_glod_box_notice", g_guildbattle_commit_glod_box_notice, "dsd");
		INIT_GAME_STR("GameWorld/GuildBattle/first_guild", g_guildbattle_first_guild, "s");
		INIT_GAME_STR("GameWorld/GuildBattle/best_killer", g_guildbattle_best_killer, "dsdd");
		INIT_GAME_STR("GameWorld/GuildBattle/fetch_target_reward", g_guildbattle_fetch_target_reward, "dsdd");
		INIT_GAME_STR("GameWorld/GuildBattle/point_boss_blood_notice", g_guildbattle_point_boss_blood_notice, "s");
		INIT_GAME_STR("GameWorld/GuildBattle/server_reward_content", g_guildbattle_server_reward_content, "");
		INIT_GAME_STR("GameWorld/GuildBattle/guild_reward_content", g_guildbattle_guild_reward_content, "");
		INIT_GAME_STR("GameWorld/GuildBattle/team_reward_content", g_guildbattle_team_reward_content, "d");
		INIT_GAME_STR("GameWorld/GuildBattle/on_attack_warning", g_guildbattle_on_attack_warning, "");
		INIT_GAME_STR("GameWorld/GuildBattle/on_flag_hp_warning", g_guildbattle_on_flag_hp_warning, "d");

		///////////////////////////////////////	 领土战  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/TerritoryWar/territorywar_kill_role", g_territorywar_kill_role, "dsddsd");
		INIT_GAME_STR("GameWorld/TerritoryWar/territorywar_result", g_territorywar_result, "d");
		INIT_GAME_STR("GameWorld/TerritoryWar/territorywar_rankreward_open_server", g_territorywar_rankreward_open_server, "");

		INIT_GAME_STR("GameWorld/TerritoryWar/monster_build_be_attack", g_territorywar_monster_build_be_attack, "d");
		INIT_GAME_STR("GameWorld/TerritoryWar/monster_build_be_kill", g_territorywar_monster_build_be_kill, "d");
		INIT_GAME_STR("GameWorld/TerritoryWar/center_build_be_occupy_my_side", g_territorywar_center_build_be_occupy_my_side, "d");
		INIT_GAME_STR("GameWorld/TerritoryWar/center_build_be_occupy_other_side", g_territorywar_center_build_be_occupy_other_side, "d");
		///////////////////////////////////////	 仙盟战  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/XianMengZhan/reward_subject", g_xianmengzhan_reward_subject, "");
		INIT_GAME_STR("GameWorld/XianMengZhan/reward_content", g_xianmengzhan_reward_content, "d");
		INIT_GAME_STR("GameWorld/XianMengZhan/occupy_center_area_notice", g_xianmengzhan_occupy_center_area_notice, "s");
		INIT_GAME_STR("GameWorld/XianMengZhan/occupy_defend_area_notice", g_xianmengzhan_occupy_defend_area_notice, "sd");
		INIT_GAME_STR("GameWorld/XianMengZhan/occupy_guild_defend_area_notice", g_xianmengzhan_occupy_guild_defend_area_notice, "ssd");
		INIT_GAME_STR("GameWorld/XianMengZhan/call_danyao_boss_notice", g_xianmengzhan_call_danyao_boss_notice, "sdds");
		INIT_GAME_STR("GameWorld/XianMengZhan/use_danyao_notice", g_xianmengzhan_use_danyao_notice, "dd");
		INIT_GAME_STR("GameWorld/XianMengZhan/kill_notice", g_xianmengzhan_kill_notice, "d");
		INIT_GAME_STR("GameWorld/XianMengZhan/xianmengzhan_standy", g_xianmengzhan_standy, "");
		INIT_GAME_STR("GameWorld/XianMengZhan/xianmengzhan_start", g_xianmengzhan_start, "");
		INIT_GAME_STR("GameWorld/XianMengZhan/xianmengzhan_close", g_xianmengzhan_close, "s");
		INIT_GAME_STR("GameWorld/XianMengZhan/join_reward_subject", g_xianmengzhan_join_reward_subject, "");
		INIT_GAME_STR("GameWorld/XianMengZhan/join_reward_content", g_xianmengzhan_join_reward_content, "");
		INIT_GAME_STR("GameWorld/XianMengZhan/relive_cd_left", g_xianmengzhan_relive_cd_left, "d");
		INIT_GAME_STR("GameWorld/XianMengZhan/lost_defend_notice", g_xianmengzhan_lost_defend_area, "s");
		INIT_GAME_STR("GameWorld/XianMengZhan/reward_guild_in_rank_content", g_xianmengzhan_guild_in_rank_context, "d");
		INIT_GAME_STR("GameWorld/XianMengZhan/reward_guild_not_in_rank_content", g_xianmengzhan_guild_not_in_rank_context, "");

		INIT_GAME_STR("GameWorld/NationalBoss/reward_subject", g_nationalboss_reward_subject, "");
		INIT_GAME_STR("GameWorld/NationalBoss/reward_content_top10", g_nationalboss_reward_content_top10, "d");
		INIT_GAME_STR("GameWorld/NationalBoss/reward_content_other", g_nationalboss_reward_content_other, "");
		INIT_GAME_STR("GameWorld/NationalBoss/kill_boss", g_kill_national_boss, "dsdd");
		INIT_GAME_STR("GameWorld/NationalBoss/task_reward_subject", g_nationalboss_task_reward_subject, "");
		INIT_GAME_STR("GameWorld/NationalBoss/task_reward_content", g_nationalboss_task_reward_content, "dd");
		INIT_GAME_STR("GameWorld/NationalBoss/ActivityBossEnd", g_activity_national_boss_end, "ds");

		/////////////////////////////////////// 转轮 ///////////////////////////////////////
		INIT_GAME_STR("GameWorld/TurnTable/get_great_gift", g_turntable_great_gift, "dsddd");
		INIT_GAME_STR("GameWorld/TurnTable/millionaire", g_turntable_millionaire, "dsd");

		///////////////////////////////////////	 结婚  /////////////////////////////////////////
		INIT_GAME_STR("Global/Marry/ReserveSubject", g_marry_reserve_subject, "");
		INIT_GAME_STR("Global/Marry/ReserveContent", g_marry_reserve_content, "dsd");
		INIT_GAME_STR("Global/Marry/ZhunBeiNotice", g_marry_zhunbei_notice, "dsddsdd");
		INIT_GAME_STR("Global/Marry/ZhunBeiSubject", g_marry_zhunbei_subject, "");
		INIT_GAME_STR("Global/Marry/ZhunBeiContent", g_marry_zhunbei_content, "");
		INIT_GAME_STR("Global/Marry/BaiTianDiFinish", g_marry_baitiandi_finish, "dsddsd");
		INIT_GAME_STR("Global/Marry/HunyanInviteSubject", g_marry_hunyan_invite_subject, "");
		INIT_GAME_STR("Global/Marry/HunyanInviteContent", g_marry_hunyan_invite_content, "dsddsd");
		INIT_GAME_STR("Global/Marry/HunyanZhunbeiNotice", g_marry_hunyan_zhunbei_notice, "dsddsd");
		INIT_GAME_STR("Global/Marry/HunyanBlessSubject", g_marry_hunyan_bless_subject, "");
		INIT_GAME_STR("Global/Marry/ReserveReqSucc", g_marry_hunyan_req_succ, "dsddsddd");
		INIT_GAME_STR("Global/Marry/BaiTianDiStart", g_marry_baitiandi_start, "dsddsd");
		INIT_GAME_STR("GameWorld/Marry/marry_reward_subject", g_marry_reward_suject, "");
		INIT_GAME_STR("GameWorld/Marry/marry_reward_content", g_marry_reward_content, "d");
		INIT_GAME_STR("Global/Marry/hunyan_zhufu1", g_hunyan_zhufu1, "dsd");
		INIT_GAME_STR("Global/Marry/hunyan_zhufu2", g_hunyan_zhufu2, "dsd");
		INIT_GAME_STR("Global/Marry/hunyan_zhufu3", g_hunyan_zhufu3, "dsd");
		INIT_GAME_STR("Global/Marry/hunyan_sahua", g_hunyan_sahua, "dsd");

		///////////////////////////////////////	 诛邪  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ActivityZhuxie/boss_flush", g_zhuxie_boss_flush_notice, "sddd");
		INIT_GAME_STR("GameWorld/ActivityZhuxie/kill_boss", g_zhuxie_kill_boss_notice, "dsds");
		INIT_GAME_STR("GameWorld/ActivityZhuxie/task_reward_subject", g_zhuxie_task_reward_subject, "");
		INIT_GAME_STR("GameWorld/ActivityZhuxie/task_reward_content", g_zhuxie_task_reward_content, "");
		INIT_GAME_STR("GameWorld/ActivityZhuxie/rob_task_notice", g_zhuxie_rob_task_notice, "dsddsdd");
		
		///////////////////////////////////////	 商城  /////////////////////////////////////////
		INIT_GAME_STR("Global/Shop/buy_open_game_shop_limit_item", g_buy_open_game_shop_limit_item, "dsdd");
		INIT_GAME_STR("GameWorld/Shop/shop_buy_notice", g_shop_buy_notice, "dsdd");

		///////////////////////////////////////	 开服活动  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/OpenGameActivity/Capability_Rank_Reward_Subject", g_open_game_activity_capability_rank_reward_subject, "");
		INIT_GAME_STR("GameWorld/OpenGameActivity/Capability_Rank_Reward_Content", g_open_game_activity_capability_rank_reward_content, "d");
		INIT_GAME_STR("GameWorld/OpenGameActivity/Guild_Level_Rank_Reward_Content", g_open_game_activity_guild_level_rank_reward_content, "d");
		INIT_GAME_STR("GameWorld/OpenGameActivity/Guild_World_Boss_Rank_Reward_Content", g_open_game_activity_guild_world_boss_rank_reward_content, "d");

		///////////////////////////////////////	 阵营  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Camp/gongxun_rank_content", g_camp_gongxun_rank_content, "d");
		INIT_GAME_STR("GameWorld/Camp/gongxun_value_content", g_camp_gongxun_value_content, "d");
		INIT_GAME_STR("GameWorld/Camp/campdefend_sf_reward_subject", g_campdefend_sf_reward_subject, "");
		INIT_GAME_STR("GameWorld/Camp/campdefend_sf_reward_content", g_campdefend_sf_reward_content, "d");
		INIT_GAME_STR("GameWorld/Camp/campdefend_gf_reward_subject", g_campdefend_gf_reward_subject, "");
		INIT_GAME_STR("GameWorld/Camp/campdefend_gf_reward_content", g_campdefend_gf_reward_content, "d");
		INIT_GAME_STR("GameWorld/Camp/campdefend_diaoxiang_bekilled", g_campdefend_diaoxiang_bekilled, "ddsdddsd");

		///////////////////////////////////////	 组队副本  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/TeamFB/open_reward_content", g_team_fb_open_reward_content, "d");
		INIT_GAME_STR("GameWorld/TeamFB/assist_reward_content", g_team_fb_assist_reward_content, "d");
		INIT_GAME_STR("GameWorld/TeamFB/teammate_can_not_enter", g_team_fb_teammate_can_not_enter, "dsd");
		INIT_GAME_STR("GameWorld/TeamFB/teammate_level_limit", g_team_fb_teammate_level_limit, "dsd");
		INIT_GAME_STR("GameWorld/TeamFB/teammate_assist_times_limit", g_team_fb_teammate_assist_times_limit, "dsd");
		INIT_GAME_STR("GameWorld/EquipFB/team_jump_mail_subject", g_equipfb_team_jump_subject, "");
		INIT_GAME_STR("GameWorld/EquipFB/team_jump_mail_content", g_equipfb_team_jump_content, "ddd");
		INIT_GAME_STR("GameWorld/EquipFB/team_equip_fb_pass_notice", g_team_equip_fb_pass_notice, "sd");

		/////////////////////////////////////////  称号管理   ///////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/TitleManage/get_title", g_title_get_title, "s");
		INIT_GAME_STR("GameWorld/TitleManage/lost_title", g_title_lost_title, "s");
		INIT_GAME_STR("GameWorld/TitleManage/get_patafb_title", g_title_get_pata_title_notice, "dsddd");

		/////////////////////////////////////////  祭炼仙丹   ///////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/JiLian/reward_subject", g_jilian_start_reward_subject, "");
		INIT_GAME_STR("GameWorld/JiLian/reward_content", g_jilian_start_reward_content, "");

		/////////////////////////////////////////  仙盟任务   ///////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/GuildTask/reward_subject", g_guild_task_complete_all_reward_subject, "");
		INIT_GAME_STR("GameWorld/GuildTask/reward_content", g_guild_task_complete_all_reward_content, "");

		/////////////////////////////////////////  刺杀任务   ///////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/CiShaTask/reward_subject", g_cisha_task_complete_reward_subject, "");
		INIT_GAME_STR("GameWorld/CiShaTask/reward_content", g_cisha_task_complete_reward_content, "");

		///////////////////////////////////////  情缘  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Qingyuan/marry_reward_content", g_qingyuan_marry_reward_content, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_succ", g_marry_success_notice, "dsddsdd");
		INIT_GAME_STR("GameWorld/Qingyuan/fb_teammate_can_not_enter", g_marry_fb_can_not_enter, "dsd");
		INIT_GAME_STR("GameWorld/Qingyuan/up_level_notice", g_qingyuan_equip_uplevel_notice, "dsddd");
		INIT_GAME_STR("GameWorld/Qingyuan/up_quality_notice_1", g_qingyuan_equip_upquality_notice_1, "dsddd");
		INIT_GAME_STR("GameWorld/Qingyuan/up_quality_notice_2", g_qingyuan_equip_upquality_notice_2, "dsdd");
		INIT_GAME_STR("GameWorld/Qingyuan/mail_divorce_subject", g_qingyuan_divorce_mail_subject, "");
		INIT_GAME_STR("GameWorld/Qingyuan/mail_divorce_content", g_qingyuan_divorce_mail_content, "s");
		INIT_GAME_STR("GameWorld/Qingyuan/divorce_notice", g_qingyuan_divorce_notice, "ss");
		INIT_GAME_STR("GameWorld/Qingyuan/hongbao_notice", g_hunyan_hongbao_notice, "dsd");
		INIT_GAME_STR("GameWorld/Qingyuan/hunyan_invite", g_hunyan_xianmeng_invite_notice, "dsddsdd");
		INIT_GAME_STR("GameWorld/Qingyuan/open_hunyan", g_hunyan_open_hunyan_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/Qingyuan/person_yanhui", g_hunyan_open_person_yanhui_notice, "dsd");
		INIT_GAME_STR("GameWorld/Qingyuan/hunyan_zhufu_notice", g_hunyan_zhufu_notice, "dsddsddsd");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_seeking_notice", g_marry_seeking_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/Qingyuan/active_card_notice", g_qingyuan_active_card_notice, "dsddd");
		INIT_GAME_STR("GameWorld/Qingyuan/hunyan_zhufu_reward_notice", g_hunyan_zhufu_reward_notice, "s");
		INIT_GAME_STR("GameWorld/Qingyuan/hunyan_subject_notice", g_hunyan_subject_notice, "");
		INIT_GAME_STR("GameWorld/Qingyuan/qingyuan_tuodan1", g_qingyuan_tuodan1, "dsd");
		INIT_GAME_STR("GameWorld/Qingyuan/qingyuan_tuodan2", g_qingyuan_tuodan2, "dsd");
		INIT_GAME_STR("GameWorld/Qingyuan/hunyan_reward_notice", g_hunyan_reward_notice, "");
		INIT_GAME_STR("GameWorld/Qingyuan/lovecontract_finish", g_lovecontract_finish, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu1", g_marry_zhufu1, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu2", g_marry_zhufu2, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu3", g_marry_zhufu3, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu4", g_marry_zhufu4, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu5", g_marry_zhufu5, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu6", g_marry_zhufu6, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_zhufu7", g_marry_zhufu7, "");
		INIT_GAME_STR("GameWorld/Qingyuan/marry_songhua", g_marry_songhua, "");

		///////////////////////////////////////  抽奖  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ActivityLuckRoll/reward_item_notice", g_luck_roll_reward_item_notice, "dsdd");
		INIT_GAME_STR("GameWorld/ActivityLuckRoll/reward_gold_notice", g_luck_roll_reward_gold_notice, "dsdd");

		///////////////////////////////////////  天降财宝 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/reward_notice", g_tianjiangcaibao_kill_boss_moster_notice, "dsd");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/reflush_qianduoduo_notice", g_tianjiangcaibao_reflush_qianduoduo_notice, "d");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/reflush_bigqian_notice", g_tianjiangcaibao_reflush_bigqian_notice, "d");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/already_reflush_bigqian_notice", g_tianjiangcaibao_already_reflush_bigqian_notice, "");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/already_reflush_qianduoduo_notice", g_tianjiangcaibao_already_reflush_qianduoduo_notice, "");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/goblin_reflush_notice", g_tianjiangcaibao_goblin_reflush_notice, "");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/kill_goblin", g_tianjiangcaibao_kill_goblin, "dsd");
		INIT_GAME_STR("GameWorld/ActivityTianJiangCaiBao/finish_task", g_tianjiangcaibao_finish_task, "dsdd");

		///////////////////////////////////////  王陵探险 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/reflush_notice", g_wanglingexplore_reflush_boss_notice, "dd");
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/already_reflush_notice", g_wanglingexplore_already_reflush_monster_notice, "d");
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/kill_boss", g_wanglingexplore_kill_boss, "dsdd");
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/wangling_boss_come",g_wanglingexplore_wangling_boss_come,"d");
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/finish_task",g_wanglingexplore_finish_task,"dsdd")
		INIT_GAME_STR("GameWorld/ActivityWangLingExplore/finish_task_getgift",g_wanglingexplore_finish_task_getgift,"dsd");

		///////////////////////////////////////	 星座遗迹 ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/XingzuoYiji/refresh_notice", g_xingzuoyiji_refresh_notice, "d");
		INIT_GAME_STR("GameWorld/XingzuoYiji/box_refresh_notice", g_xingzuoyiji_box_refresh_notice, "d");
		INIT_GAME_STR("GameWorld/XingzuoYiji/normal_end_notice", g_xingzuoyiji_normal_end_notice, "d");
		INIT_GAME_STR("GameWorld/XingzuoYiji/good_end_notice", g_xingzuoyiji_good_end_notice, "d");
		INIT_GAME_STR("GameWorld/XingzuoYiji/gather_common_box", g_xingzuoyiji_gather_common_box, "ddsd");
		INIT_GAME_STR("GameWorld/XingzuoYiji/gather_molong_box", g_xingzuoyiji_gather_molong_box, "ddsd");

		///////////////////////////////////////	 温泉答题 ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Hotspring/action_use_snow", g_hotspring_use_snow, "dsddsd");
		INIT_GAME_STR("GameWorld/Hotspring/action_use_massage", g_hotspring_use_massage, "dsddsd");
		INIT_GAME_STR("GameWorld/Hotspring/rank_first_player", g_hotspring_rank_first_player, "dsd");
		INIT_GAME_STR("GameWorld/Hotspring/find_lover", g_hotspring_find_lover ,"dsddsd");

		///////////////////////////////////////  随机活动  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/consume_gold_fanli_reward_content", g_rand_activity_consume_gold_fanli_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/equipstrengthen_reward_content", g_rand_activity_strengthen_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_rank_reward_content", g_rand_activity_chongzhi_rank_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_rank_reward_content_reserve", g_rand_activity_chongzhi_rank_reward_content_reserve, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_rank2_reward_content", g_rand_activity_chongzhi_rank2_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_rank2_reward_content_reserve", g_rand_activity_chongzhi_rank2_reward_content_reserve, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/consume_gold_rank_reward_content", g_rand_activity_consume_gold_rank_reward_content, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/consume_gold_rank_reward_content_reserve", g_rand_activity_consume_gold_rank_reward_content_reserve, "dddd");
		
		INIT_GAME_STR("GameWorld/RandActivity/day_chongzhi_fanli_reward_content", g_rand_activity_day_chongzhi_fanli_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/day_consume_gold_reward_content", g_rand_activity_day_consume_gold_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/chestshop", g_rand_activity_chestshop_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/total_consume_gold_reward_content", g_rand_activity_total_consume_gold_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/day_active_degree_reward_content", g_rand_activity_day_active_degree_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/stone_total_level", g_rand_activity_stone_total_level_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/xiannv_chanmian_uplevel", g_rand_activity_xiannv_chanmian_uplevel_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/mount_upgrade", g_rand_activity_mount_upgrade_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/qibing_upgrade", g_rand_activity_qibing_upgrade_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/mentality_total_level", g_rand_activity_mentality_total_level_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/wing_upgrade", g_rand_activity_wing_upgrade_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/kill_boss_reward_content", g_rand_activity_kill_boss_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/day_online_gift_reward_content", g_rand_activity_day_online_gift_reward_content,"");
		INIT_GAME_STR("GameWorld/RandActivity/day_online_gift_reward_notice", g_rand_activity_day_online_gift_reward_notice, "sd");
		INIT_GAME_STR("GameWorld/RandActivity/quanmin_qifu", g_rand_activity_quanmin_qifu_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/xianmeng_bipin", g_rand_activity_xianmeng_bipin_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/xianmeng_jueqi", g_rand_activity_xianmeng_jueqi_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/login_gift", g_rand_activity_login_gift_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_rank_reward_content", g_rand_activity_bipin_rank_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_rank_reward_notice", g_rand_activity_bipin_rank_reward_notice, "sdd");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_all_reward_content", g_rand_activity_bipin_all_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_topuser_change_notice", g_rand_activity_bipin_topuser_change_notice, "ddsddsdd");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_topuser_change_content", g_rand_activity_bipin_topuser_change_content, "ddsdd");
		INIT_GAME_STR("GameWorld/RandActivity/bipin_day_change_mail_content", g_rand_activity_bipin_day_change_mail_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content", g_rand_activity_single_charge_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content2", g_rand_activity_single_charge_reward_content2, "d");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content3", g_rand_activity_single_charge_reward_content3, "d");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content4", g_rand_activity_single_charge_reward_content4, "d");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content5", g_rand_activity_single_charge_reward_content5, "d");
		INIT_GAME_STR("GameWorld/RandActivity/single_charge_reward_content_multi", g_rand_activity_single_charge_reward_content_multi, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/double_get_reward_content", g_rand_activity_double_get_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/cornucopia_reward_content", g_rand_activity_cornucopia_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/new_cornucopia_reward_content", g_rand_activity_new_cornucopia_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/double_charge_content", g_rand_activity_double_charge_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/total_charge_day_reward_content", g_rand_activity_total_charge_day_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/tomorrow_reward_content", g_rand_activity_tomorrow_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/day_chongzhi_rank_reward_content", g_rand_activity_day_rank_chongzhi_reweard_content, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/day_chongzhi_rank_reward_content_reserve", g_rand_activity_day_rank_chongzhi_reweard_content_reserve, "dddd");
		INIT_GAME_STR("GameWorld/RandActivity/day_consume_rank_reward_content", g_rand_activity_day_rank_consume_reweard_content, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/total_charge_bu_reward_content", g_rand_activity_total_charge_bu_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/total_charge_reward_chuanwen_content", g_rand_activity_total_charge_reward_chuanwen_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/new_total_charge_reward_chuanwen_content", g_rand_activity_new_total_charge_reward_chuanwen_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/new_total_charge_reward_chuanwen_content1", g_rand_activity_new_total_charge_reward_chuanwen_content1, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/new_total_charge_reward_chuanwen_content2", g_rand_activity_new_total_charge_reward_chuanwen_content2, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/new_total_charge_reward_chuanwen_content3", g_rand_activity_new_total_charge_reward_chuanwen_content3, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/level_lottery_reward_content", g_rand_activity_level_lottery_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_niuegg_reward_content", g_rand_activity_chongzhi_niuegg_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/zhenbaoge_zhenxi_reward_content", g_rand_activity_zhenbaoge_zhenxi_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/xianyuan_treas_mail_content", g_rand_activity_xianyuan_treas_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/xianyuan_treas_all_buy_mail_content", g_rand_activity_xianyuan_treas_all_buy_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/mijingxunbao_zhenxi_reward_content", g_rand_activity_mijingxunbao_zhenxi_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/jisuchongzhan_reward_content", g_rand_activity_jisuchongzhan_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/money_tree_reward_content_gold", g_rand_activity_money_tree_reward_content_gold, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/money_tree_reward_content_item", g_rand_activity_money_tree_reward_content_item, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/daily_love_reward_content_gold", g_rand_activity_daily_love_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/king_draw_reward_content", g_rand_activity_king_draw_reward_content, "dsdd"); 
		INIT_GAME_STR("GameWorld/RandActivity/king_draw_return_reward_content", g_rand_activity_king_draw_return_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/total_charge3_reward_content", g_rand_activity_total_charge3_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/appearace_passive_rank_reward_content", g_rand_activity_appearace_passive_rank_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/appearace_passive_rank_join_reward_content", g_rand_activity_appearace_passive_rank_join_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/appearace_rank_reward_content", g_rand_activity_appearace_rank_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/appearace_rank_join_reward_content", g_rand_activity_appearace_rank_join_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/total_charge4_reward_content", g_rand_activity_total_charge4_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/mine_server_reward_content", g_rand_activity_mine_server_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/mine_role_refresh_content", g_rand_activity_mine_role_refresh_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/mine_exchange_reward_content", g_rand_activity_mine_exchange_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/tian_ming_divination_reward_content", g_rand_activity_tian_ming_divination_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/tian_ming_divination_rare_reward", g_rand_activity_tian_ming_divination_rare_reward, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/fanfan_reward_item_content", g_rand_activity_fanfan_reward_item_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/fanfan_exchange_word_reward_content", g_rand_activity_fanfan_exchange_word_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/continue_chongzhi_reward_content", g_rand_activity_continue_chongzhi_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/continue_consume_reward_content", g_rand_activity_continue_consume_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/army_day_win_army_reward_content", g_rand_activity_army_day_win_army_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/circulation_chongzhi_reward_content", g_rand_activity_circulation_chongzhi_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/extreme_lucky_reward_content", g_rand_activity_extreme_lucky_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/extreme_lucky_return_reward_content", g_rand_activity_extreme_lucky_reward_return_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/mijingxunbao2_zhenxi_reward_content", g_rand_activity_mijingxunbao2_zhenxi_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/money_tree2_reward_content_gold", g_rand_activity_money_tree2_reward_content_gold, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/money_tree2_reward_content_item", g_rand_activity_money_tree2_reward_content_item, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/zhenbaoge2_zhenxi_reward_content", g_rand_activity_zhenbaoge2_zhenxi_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/promoting_position_reward_item_content", g_rand_activity_promoting_position_reward_item_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/promoting_position_reward_gold_content", g_rand_activity_promoting_position_reward_gold_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/black_market_give_item_content", g_rand_activity_black_market_give_item_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/black_market_return_gold_content", g_rand_activity_black_market_return_gold_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/black_market_final_get_reward", g_rand_activity_black_market_final_get_reward, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/role_uplevel_content", g_rand_activity_role_uplevel_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/pata_content", g_rand_activity_pata_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/exp_fb_content", g_rand_activity_exp_fb_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_mount_content", g_rand_activity_upgrade_mount_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_halo_content", g_rand_activity_upgrade_halo_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_wing_content", g_rand_activity_upgrade_wing_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shengong_content", g_rand_activity_upgrade_shengong_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenyi_content", g_rand_activity_upgrade_shenyi_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/first_charge_tuan_content", g_rand_activity_first_charge_tuan_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/day_total_charge_content", g_rand_activity_day_total_charge_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_mount_total_content", g_rand_activity_upgrade_mount_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_halo_total_content", g_rand_activity_upgrade_halo_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_wing_total_content", g_rand_activity_upgrade_wing_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shengong_total_content", g_rand_activity_upgrade_shengong_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenyi_total_content", g_rand_activity_upgrade_shenyi_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/equip_strength_total_level_content", g_rand_activity_equip_strength_level_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/stone_total_level_content", g_rand_activity_stone_level_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_mount_rank_content", g_rand_activity_upgrade_mount_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_halo_rank_content", g_rand_activity_upgrade_halo_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_wing_rank_content", g_rand_activity_upgrade_wing_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shengong_rank_content", g_rand_activity_upgrade_shengong_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenyi_rank_content", g_rand_activity_upgrade_shenyi_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_qilinbi_rank_content", g_rand_activity_upgrade_qilinbi_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_toushi_rank_content", g_rand_activity_upgrade_toushi_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_yaoshi_rank_content", g_rand_activity_upgrade_yaoshi_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shizhuang_rank_content", g_rand_activity_upgrade_shizhuang_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenbing_rank_content", g_rand_activity_upgrade_shenbing_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_lingtong_rank_content", g_rand_activity_upgrade_lingtong_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_linggong_rank_content", g_rand_activity_upgrade_linggong_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_lingqi_rank_content", g_rand_activity_upgrade_lingqi_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_fabao_rank_content", g_rand_activity_upgrade_fabao_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_footprint_rank_content", g_rand_activity_upgrade_footprint_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_flypet_rank_content", g_rand_activity_upgrade_flypet_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_weiyan_rank_content", g_rand_activity_upgrade_weiyan_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/equip_strengh_level_rank_content", g_rand_activity_equip_strength_level_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/stone_total_level_rank_content", g_rand_activity_stone_total_level_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/xiannv_cap_rank_content", g_rand_activity_xiannv_cap_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/jingling_cap_rank_content", g_rand_activity_jingling_cap_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_fight_mount_rank_content", g_rand_activity_upgrade_fight_mount_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/total_cap_rank_content", g_rand_activity_total_cap_rank_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/boss_killer_content", g_rand_activity_boss_killer_total_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/marry_me", g_rand_activity_marry_me, "d");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_bipin_two", g_rand_activity_bipin_two, "ddsddsd");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_bipin", g_rand_activity_bipin, "ddsd");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_mount_rank_unclaimed_content", g_rand_activity_mount_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_wing_rank_unclaimed_content", g_rand_activity_wing_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shizhuang_rank_unclaimed_content", g_rand_activity_shizhuang_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenbing_rank_unclaimed_content", g_rand_activity_shenbing_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_fabao_rank_unclaimed_content", g_rand_activity_fabao_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_footprint_rank_unclaimed_content", g_rand_activity_footprint_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shengong_rank_unclaimed_content", g_rand_activity_shengong_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_shenyi_rank_unclaimed_content", g_rand_activity_shenyi_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_qilinbi_rank_unclaimed_content", g_rand_activity_qilinbi_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_toushi_rank_unclaimed_content", g_rand_activity_toushi_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_yaoshi_rank_unclaimed_content", g_rand_activity_yaoshi_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_lingtong_rank_unclaimed_content", g_rand_activity_lingtong_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_linggong_rank_unclaimed_content", g_rand_activity_linggong_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_lingqi_rank_unclaimed_content", g_rand_activity_lingqi_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_halo_rank_unclaimed_content", g_rand_activity_halo_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_stone_rank_unclaimed_content", g_rand_activity_stone_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_equip_rank_unclaimed_content", g_rand_activity_equip_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/xiannv_cap_rank_unclaimed_content", g_rand_activity_xiannv_cap_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/jingling_cap_rank_unclaimed_content", g_rand_activity_jingling_cap_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_fight_mount_rank_unclaimed_content", g_rand_activity_upgrade_fight_mount_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_flypet_rank_unclaimed_content", g_rand_activity_upgrade_flypet_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/upgrade_weiyan_rank_unclaimed_content", g_rand_activity_upgrade_weiyan_rank_unclaimed_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/total_cap_rank_unclaimed_content", g_rand_activity_total_cap_rank_unclaimed_content, "");

		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_summon_golden_pig", g_rand_activity_summon_golden_pig, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_hurt_golden_pig", g_rand_activity_hurt_golden_pig, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_kill_golden_pig", g_rand_activity_kill_golden_pig, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_summon_golden_pig_reward_content", g_rand_activity_summon_golden_pig_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_kill_golden_reward_content", g_rand_activity_kill_golden_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_openserver_invest", g_rand_activity_openserver_invest, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/rand_activity_openserver_invest_reward", g_rand_activity_openserver_invest_reward, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/single_chongzhi_reward_content", g_rand_activity_single_chongzhi_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/rush_buying_begin_content", g_rand_activity_rush_buying_begin, "d");

		INIT_GAME_STR("GameWorld/RandActivity/map_hunt_get_rare_city_content", g_rand_activity_map_hunt_get_rare_city_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/randactivity_check_reward", g_rand_activity_check_reward, "d");
		INIT_GAME_STR("GameWorld/RandActivity/light_tower_explore_reward_content", g_rand_activity_light_tower_explore_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/magic_shop_reward_content", g_rand_activity_magic_shop_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/chongzhi_crazy_rebate_content", g_rand_activity_chongzhi_crazy_rebate, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/perfect_lover_reward_item_content", g_rand_activity_perfect_lover_reward_item_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/jinjie_return_reward_subject", g_ra_jinjie_return_reward_subject, "");
		INIT_GAME_STR("GameWorld/RandActivity/jinjie_return_reward_content", g_ra_jinjie_return_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/time_limit_gift_reward_content", g_rand_activity_time_limit_gift_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/time_limit_gift_reward_mail", g_rand_activity_time_limit_gift_reward_mail, "");
		INIT_GAME_STR("GameWorld/RandActivity/circulation_chongzhi_not_ra_reward_content", g_circulation_chongzhi_not_ra_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/shake_money_reward_content", g_rand_activity_shake_money_reward_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/time_limit_luxury_gifts_content", g_rand_activity_time_limit_luxury_gifts_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/double_chongzhi_content", g_rand_activity_double_chongzhi_reward_content, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/expense_nice_gift_reward_mail_content", g_rand_activity_expense_nice_gift_mail_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/version_total_charge_reward_content", g_rand_activity_version_total_charge_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/version_continue_mail_content", g_rand_activity_version_continue_mail_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/huanleyaojiang_2_zhenxi_reward_content", g_rand_activity_huanleyaojiang_2_zhenxi_reward_content, "dsddd");
		INIT_GAME_STR("GameWorld/RandActivity/offline_single_charge_reward_content", g_rand_activity_offline_single_charge_reward_content, "d");
		INIT_GAME_STR("GameWorld/RandActivity/guagua_reward_content", g_rand_activity_guagua_reward_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/guagua_acc_reward_mail", g_rand_activity_guagua_acc_reward_mail, "");
		INIT_GAME_STR("GameWorld/RandActivity/uplevel_return_reward_mail", g_rand_activity_uplevel_return_reward_mail, "ddd");
		/////////////////////////////////// 奖励补发通用邮件内容 //////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/check_reward_mail_common", g_rand_activity_check_reward_common_mail_content, "d");

		/////////////////////////////////////////   秘境寻宝3   ///////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/mijingxunbao3_zhenxi_reward_content", g_rand_activity_mijingxunbao3_zhenxi_reward_content, "dsddd");

		/////////////////////////////////////////   欢乐砸蛋   ///////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/huanlezadan_zhenxi_reward_content", g_rand_activity_huanlezadan_zhenxi_reward_content, "dsddd");

		/////////////////////////////////////////   欢乐摇奖   ///////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/huanleyaojiang_zhenxi_reward_content", g_rand_activity_huanleyaojiang_zhenxi_reward_content, "dsddd");

		//////////////////////////////////////  累计充值5  /////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/total_charge5_reward_content", g_rand_activity_total_charge5_reward_content, "d");

		///////////////////////////////////////  无双  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Wushuang/pass_reward_content", g_wushuang_fb_pass_reward_content, "");

		///////////////////////////////////////  鱼池  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/FishPool/be_steal_fish_content", g_fish_pool_be_steal_content, "dddsd");

		///////////////////////////////////////  世界事件  //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/WorldEvent/event_reward_content", g_world_event_event_reward_content, "");
		INIT_GAME_STR("GameWorld/WorldEvent/set_yurenka_appearance", g_world_event_set_yurenka_appearance, "ss");
		INIT_GAME_STR("GameWorld/WorldEvent/be_set_yurenka_appearance", g_world_event_be_set_yurenka_appearance, "s");

		///////////////////////////////////////  红包  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RedPaper/fetch_succ_notice", g_red_paper_fetch_succ_notice, "dsddsdd");
		INIT_GAME_STR("GameWorld/RedPaper/fetch_over_notice", g_red_paper_fetch_over_notice, "dsd");
		INIT_GAME_STR("GameWorld/RedPaper/lucky_user_notice", g_red_paper_lucky_user_notice, "dsddsdd");
		INIT_GAME_STR("GameWorld/RedPaper/give_left_money_content", g_red_paper_give_left_money_content, "d");
		INIT_GAME_STR("GameWorld/RedPaper/create_notice", g_red_paper_create_notice, "dsdddd");
		INIT_GAME_STR("GameWorld/RedPaper/create_notice2", g_red_paper_create_notice2, "dsdddd");
		INIT_GAME_STR("GameWorld/RedPaper/singal_chat_notice", g_red_paper_singal_chat, "");
		INIT_GAME_STR("GameWorld/command_speaker/create_command", g_command_speak_create_command, "dsds");

		///////////////////////////////////////  兑换商店  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/convertshop/guild_notice", g_convershop_guild_notice, "dsddd");
		INIT_GAME_STR("GameWorld/convertshop/world_notice", g_convershop_world_notice, "dsddd");

		///////////////////////////////////////	 一站到底  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/finish_broacast", g_yizhandaodi_fininsh_brocast, "dsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/buff_broast", g_yizhandaodi_buff_broast, "dsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/broast_cur_role_num", g_yizhandaodi_broast_cur_role_num, "dd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/reward_subject", g_yizhandaodi_reward_subject, "");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/reward_rankcontent", g_yizhandaodi_reward_rank_content, "ddd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/reward_norankcontent", g_yizhandaodi_reward_norank_content, "dd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/jisha_count_notice", g_yizhandao_jisha_count_notice, "dsdd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/lucky_notice", g_yizhandaodi_lucky_notice, "s");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/lucky_reward_mail/subject", g_yizhandaodi_lucky_reward_subject, "");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/lucky_reward_mail/content", g_yizhandaodi_lucky_reward_content, "");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/dead_times_reward_content", g_yizhandaodi_dead_times_reward_content, "d");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/kickout_notice", g_yizhandaodi_kickout_notice, "dsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/first_replace_notice", g_yizhandaodi_first_replace_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/finish_rank_top1", g_yizhandaodi_finish_rank_top1, "dsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/finish_rank_top2", g_yizhandaodi_finish_rank_top2, "dsddsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/finish_rank_top3", g_yizhandaodi_finish_rank_top3, "dsddsddsd");
		INIT_GAME_STR("GameWorld/YiZhanDaoDi/kickout_mail_content", g_yizhandaodi_kickout_mail_content,"");

		///////////////////////////////////////	 塔防副本  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/TeamTowerdefend/teammate_level_limit", g_team_towerdefend_teammate_level_limit, "dsd");
		INIT_GAME_STR("GameWorld/TeamTowerdefend/teammate_can_not_enter", g_team_towerdefend_teammate_can_not_enter, "dsd");
		INIT_GAME_STR("GameWorld/TeamTowerdefend/teammate_join_times_limit", g_team_towerdefend_teammate_join_times_limit, "dsd");
		INIT_GAME_STR("GameWorld/TeamTowerdefend/teammate_attr_type_error", g_team_towerdefend_teammate_attr_type_error, "dsd");
		INIT_GAME_STR("GameWorld/TeamTowerdefend/team_pass_wave", g_team_towerdefend_pass_wave, "sd");
		INIT_GAME_STR("GameWorld/TeamTowerdefend/team_pass_reward", g_team_towerdefend_pass_reward, "");

		///////////////////////////////////////	 五行挂机  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/kill_boss", g_wuxingguaji_kill_boss_notice, "dsds");
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/boss_flush", g_wuxingguaji_boss_flush_notice, "s");
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/reward_content", g_wuxingguaji_reward_content, "");
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/lucky_notice", g_wuxingguaji_lucky_notice, "s");
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/lucky_reward_mail/subject", g_wuxingguaji_lucky_reward_subject, "");
		INIT_GAME_STR("GameWorld/ActivityWuXingGuaJi/lucky_reward_mail/content", g_wuxingguaji_lucky_reward_content, "");

		///////////////////////////////////////	 妖兽广场  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/YaoShouGuangChang/reward_notice", g_yaoshouguangchang_reward_content, "");
		INIT_GAME_STR("GameWorld/SuoYaoTa/reward_notice", g_suoyaota_reward_content, "");

		///////////////////////////////////////	 仙盟运势  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/GuildLuckyInfo/create_notice", g_guild_lucky_info_orange_notice, "dsddsddsd");

		///////////////////////////////////////	 角色转阵营  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RoleChangeCamp/change_camp_notice", g_role_change_camp_notice, "dsddd");

		///////////////////////////////////////	 卡牌  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/card/uplevel_notice", g_card_uplevel_notice, "dsdddd");

		///////////////////////////////////////	 精华护送  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/JingHuaHusong/jinghua_refresh_notice", g_jinghua_refresh_notice, "ddd");
		INIT_GAME_STR("GameWorld/JingHuaHusong/jinghua_husong_timeout_subject", g_jinghua_husong_timeout_subject, "");
		INIT_GAME_STR("GameWorld/JingHuaHusong/jinghua_husong_timeout_content", g_jinghua_husong_timeout_content, "");

		///////////////////////////////////////	 神秘商店  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Mysteriousshop/mysteriousshop_refresh", g_mysteriousrefresh_notice, "d");
		INIT_GAME_STR("GameWorld/Mysteriousshopinmall/mysteriousshop_refresh_good_item", g_mysteriousshop_refresh_good_item, "dsddd");
		INIT_GAME_STR("GameWorld/Mysteriousshopinmall/mysteriousshop_buy_good_item", g_mysteriousshop_buy_good_item, "dsddd");

		//////////////////////////////////////// 元宝转盘 ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/GoldTurntable/gold_turntable_notice", g_gold_turntable_notice,"dsd");

		///////////////////////////////////////	 土豪金  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/TuHaoJin/tuhaojin_notice", g_tuhaojin_activate_notic, "dsd");

		///////////////////////////////////////	 大表情  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/BigChatFace/bigchatface_notice", g_bigchatface_activate_notice, "dsd");

		///////////////////////////////////////	 神装  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenZhuang/downlevel", g_shenzhuang_downlevel, "dsddsddd");
		INIT_GAME_STR("GameWorld/ShenZhuang/jinjie", g_shenzhuang_jinjie, "dsdddd");

		///////////////////////////////////////	 神州六器  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/gather_content", g_shenzhou_weapon_gather_content, "dsd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/gather_refresh_content", g_shenzhou_gather_refresh_content, "d");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/shenzhou_help_box_reward", g_shenzhou_help_box_reward, "");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/gather_top_class_stone", g_shenzhou_gather_top_class_stone, "dsd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/active_shenzhou_weapon", g_shenzhou_weapon_active, "dsddd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/weapon_skill_level_up", g_shenzhou_skill_up_level, "dsdddd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/weapon_identify_level_up", g_shenzhou_identify_up_level, "dsddd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/weapon_lianhun_active", g_shenzhou_lianhun_active, "dsdd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/weapon_lianhun_level_up", g_shenzhou_lianhun_level_up, "dsdddd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/weapon_xilian", g_shenzhou_xilian, "dsdd");
		INIT_GAME_STR("GameWorld/ShenzhouWeapon/treasure", g_shenzhou_treasure, "dsdd");

		///////////////////////////////////////	 宝宝 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Baby/upgrade_mail", g_baby_upgrade_mail, "d");
		INIT_GAME_STR("GameWorld/Baby/uplevel_mail", g_baby_uplevel_mail, "d");
		INIT_GAME_STR("GameWorld/Baby/upgrade", g_baby_upgrade, "dsdd");
		INIT_GAME_STR("GameWorld/Baby/subject", g_baby_subject_mail, "");
		INIT_GAME_STR("GameWorld/Baby/qifu_succ", g_baby_qifu_succ, "dsddsd");

		///////////////////////////////////////	 魔龙系统 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/molong_rewardcontent", g_molong_reward_content, "dd");
		INIT_GAME_STR("GameWorld/molong_rewardcontent_item", g_molong_reward_content_item, "");

		///////////////////////////////////////	 宠物系统 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Pet/pet_upgrade", g_pet_upgrade, "dsdd");
		INIT_GAME_STR("GameWorld/Pet/rewardcontent", g_pet_reward, "dsdd");
		INIT_GAME_STR("GameWorld/Pet/activesuit", g_pet_active_suit, "dsddd");

		///////////////////////////////////////	 牧场精灵 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/PastureSpirit/lucky_draw_content", g_pasture_spirit_lucky_draw_content, "dsdd");

		///////////////////////////////////////	 双人坐骑 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/MultiMount/upgrade", g_multi_mount_upgrade_content, "dsddd");

		///////////////////////////////////////	 个性聊天框 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/PersonalizeWindow/bubble_rim_active_content", g_personalize_window_bubble_rim_active_content, "dsdd");

		///////////////////////////////////////	 魔卡 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/magic_card/rewardcontent", g_magic_card_reward_content, "dsdd");

		///////////////////////////////////////	 精灵 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/JingLing/jingling_halo_upgrade_notice", g_jingling_halo_upgrade_notice, "dsdd");
		INIT_GAME_STR("GameWorld/JingLing/jingling_fazhen_upgrade_notice", g_jingling_fazhen_upgrade_notice, "dsdd");
		INIT_GAME_STR("GameWorld/JingLing/jingling_wuxing_notice1", g_jingling_wuxing_notice1, "dsddd");
		INIT_GAME_STR("GameWorld/JingLing/jingling_wuxing_notice2", g_jingling_wuxing_notice2, "dsddd");
		INIT_GAME_STR("GameWorld/JingLing/jingling_card", g_jingling_card, "dsdd");
		INIT_GAME_STR("GameWorld/JingLing/jingling_refresh_purple", g_jingling_refresh_purple, "dsd");

		/////////////////////////////////////// 转生 ///////////////////////////////////////
		INIT_GAME_STR("GameWorld/zhuansheng/zhuansheng_levle_up_notice", g_zhuansheng_level_up_notice, "dsdd");
		INIT_GAME_STR("GameWorld/zhuansheng/zhuansheng_notice", g_zhuansheng_notice, "dsdd");

		///////////////////////////////////////////////////////  套装 //////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/EquipSuit/equip_suit_shishi_active_content", g_equip_suit_shishi_active_content, "dsdd");
		INIT_GAME_STR("GameWorld/EquipSuit/equip_suit_chuanshuo_active_content", g_equip_suit_chuanshuo_active_content, "dsdd");

		///////////////////////////////////////	 镜像Boss  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/CombineServerActivity/rank_reward_content", g_csa_rank_reward_content, "dd");
		INIT_GAME_STR("GameWorld/CombineServerActivity/rank_reward_notice_qianggou", g_csa_rank_reward_notice_qianggou, "sd");

		///////////////////////////////////////	好友送礼  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/FriendGift/friend_gift_shou_content", g_friend_gift_shou_content, "dsd");
		INIT_GAME_STR("GameWorld/FriendGift/friend_gift_chat_content", g_msg_gift_send_singlechat, "");
		INIT_GAME_STR("GameWorld/FriendGift/friend_song_hua_chat_content", g_msg_friend_song_hua_chat_content, "");
		INIT_GAME_STR("GameWorld/FriendGift/friend_song_hua_content", g_friend_song_hua_content, "");
		INIT_GAME_STR("GameWorld/FriendGift/friend_heli_songhua_content", g_friend_heli_songhua_content, "dsdd");
		INIT_GAME_STR("GameWorld/FriendGift/friend_heli_egg_content", g_friend_heli_egg_content, "dsdd");

		/////////////////////////////////////// 符文系统 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/runesystem/one_xunbao_notice", g_runesystem_one_xunbao_notice, "dsdd");
		INIT_GAME_STR("GameWorld/runesystem/ten_xunbao_notice", g_runesystem_ten_xunbao_notice, "dsdd");
		INIT_GAME_STR("GameWorld/runesystem/pass_layer", g_runesystem_pass_layer, "dsdd");
		INIT_GAME_STR("GameWorld/runesystem/up_level_send_to_world", g_runesystem_up_level_send_to_world, "dsddd");
		INIT_GAME_STR("GameWorld/runesystem/zhuling", g_runesystem_zhuling, "dsddd");

		/////////////////////////////////////// 日常副本 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/daily_fb/dailyfb_cant_enter_fb", g_dailyfb_cant_enter_fb, "dsd");

		/////////////////////////////////////// 组队装备副本 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/team_equip_fb/team_equip_fb_notice", g_team_equip_fb_notice, "dsd");

		/////////////////////////////////////// 爬塔副本 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/pata_fb/pata_fb_pass_layer", g_pata_fb_pass_layer, "dsdd");

		/////////////////////////////////////// 神格 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Shenge/chou_jiang_msg", g_chou_jiang_msg, "dsdd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_combine_msg", g_shenge_combine_msg, "dsdd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_total_level", g_shenge_total_level, "dsdd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_active_new_page", g_shenge_active_new_page, "dsddd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_active_combine", g_shenge_active_combine, "dsdd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_zhangkong_system_msg", g_shenge_zhangkong_system_msg, "dsddd");
		INIT_GAME_STR("GameWorld/Shenge/shenge_xilian_system_msg", g_shenge_xilian_system_msg, "dsd");

		/////////////////////////////////////// 星座 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Xingzuo/xintu_finish_combine", g_xintu_finish_combine, "dsds");
		INIT_GAME_STR("GameWorld/Xingzuo/xintu_finish_chapter", g_xintu_finish_chapter, "dsds");
		INIT_GAME_STR("GameWorld/Xingzuo/xingzuo_update_limit", g_xingzuo_update_limit, "sd");
		INIT_GAME_STR("GameWorld/Xingzuo/xingzuo_taozhuang", g_xingzuo_taozhuang, "dsddd");
		INIT_GAME_STR("GameWorld/Xingzuo/xingzuo_equip_level", g_xingzuo_equip_level, "dsddd");

		/////////////////////////////////////// 嘉年华 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/magical_precious/finish_chapter", g_finish_chapter, "dsd");

		/////////////////////////////////////// 冲级豪礼 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/chongjihaoli", g_chongjihaoli, "dsd");

		/////////////////////////////////////// 活跃奖励 ///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/active_degree", g_active_degree, "dsdd");

		/////////////////////////////////////// 被杀 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/be_killed/be_killed_by_role_in_static_scene", g_be_killed_by_role_in_static_scene, "dsddsd");
		INIT_GAME_STR("GameWorld/be_killed/be_killed_in_boss_scene", g_be_killed_in_boss_scene, "dsddsd");

		///////////////////////////////////////	水晶  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Shuijing/big_shuijing_refresh", g_big_shuijing_refresh, "d");
		INIT_GAME_STR("GameWorld/Shuijing/big_shuijing_already_refresh", g_big_shuijing_already_refresh, "");
		INIT_GAME_STR("GameWorld/Shuijing/big_shuijing_gather", g_big_shuijing_gather, "dsd");
		INIT_GAME_STR("GameWorld/Shuijing/gather_type_shuijing", g_gather_type_shuijing, "dsd");
		INIT_GAME_STR("GameWorld/Shuijing/finish_shuijing_task", g_finish_shuijing_task, "dsd");

		INIT_GAME_STR("GameWorld/CombineServerActivity/gcz_winner_reward_content", g_csa_gcz_winner_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/gcz_camp_reward_content", g_csa_gcz_camp_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/xmz_winner_reward_content", g_csa_xmz_winner_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/xmz_guild_reward_content", g_csa_xmz_guild_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/gcz_winner_notice", g_csa_gcz_winner_notice, "dsd");
		INIT_GAME_STR("GameWorld/CombineServerActivity/xmz_winner_notice", g_csa_xmz_winner_notice, "dsd");
		INIT_GAME_STR("GameWorld/CombineServerActivity/single_charge_reward_content", g_csa_single_charge_reward_content, "d");
		INIT_GAME_STR("GameWorld/CombineServerActivity/charge_reward_double_content", g_csa_charge_reward_double_content, "d");
		INIT_GAME_STR("GameWorld/CombineServerActivity/roll_reward_notice", g_csa_roll_reward_notice, "dsdd");
		INIT_GAME_STR("GameWorld/CombineServerActivity/boss_guild_rank_master_reward", g_csa_boss_guild_rank_master_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/boss_guild_rank_member_reward", g_csa_boss_guild_rank_member_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/touzijihua_reward", g_csa_touzijihua_auto_fetch_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/foundation_reward", g_csa_foundation_auto_fetch_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/login_gift_reward", g_csa_logingift_auto_fetch_reward_content, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/csa_boss_refresh_greatboss", g_csa_boss_refresh_greatboss, "");
		INIT_GAME_STR("GameWorld/CombineServerActivity/csa_boss_killall_greatboss", g_csa_boss_killall_greatboss, "");


		INIT_GAME_STR("GameWorld/welcome_mail/content", g_welcome_mail_content, "");
		INIT_GAME_STR("GameWorld/Cardzu/cardzu_active", g_cardzu_active_content, "dsdd");

		INIT_GAME_STR("GameWorld/shenshou/compose_content", g_shenshou_compose_content, "dsdd");
		INIT_GAME_STR("GameWorld/shenshou/huanling", g_shenshou_huanling_draw, "dsdd");

		INIT_GAME_STR("GameWorld/DianJi/draw_reward_content", g_dianji_draw_reward_content, "dsdd");

		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_up_layer_notice", g_cross_xiuluo_tower_up_layer_notice, "dsdd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_finish_notice", g_cross_xiuluo_tower_finish_notice, "dsdd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_reward_content", g_cross_xiuluo_tower_reward_content, "d");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_rank_reward_content", g_cross_xiuluo_tower_rank_reward_content, "d");
		INIT_GAME_STR("Cross/cross_rank/week_reward", g_cross_rank_week_reward_content, "ddd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_get_buff", g_cross_xiuluo_tower_get_buff, "ddsd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_rap_buff", g_cross_xiuluo_tower_rap_buff, "ddsdddsd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_get_reward", g_cross_xiuluo_tower_get_reward, "dsd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_reward_subject", g_cross_xiuluo_tower_reward_subject, "");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_first_reward_subject", g_cross_xiuluo_tower_first_reward_subject, "");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_kill_boss", g_cross_xiuluo_tower_kill_boss, "dsdd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_boss_refresh", g_cross_xiuluo_tower_boss_refresh, "dd");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_boss_already_refresh", g_cross_xiuluo_tower_boss_already_refresh, "d");
		INIT_GAME_STR("Cross/cross_xiuluo_tower/xiuluo_tower_gold_box_reward_mail_content", g_xiuluo_tower_gold_box_reward_mail_content, "d");

		INIT_GAME_STR("Cross/cross_1v1/dur_kill_notice", g_cross_1v1_dur_kill_notice, "dsdd");
		INIT_GAME_STR("Cross/cross_1v1/end_dur_kill_notice", g_cross_1v1_end_dur_kill_notice, "dsddsdd");
		INIT_GAME_STR("Cross/cross_1v1/xiazhu_reward_subject", g_cross_1v1_xiazhu_reward_subject, "");
		INIT_GAME_STR("Cross/cross_1v1/xiazhu_reward_content", g_cross_1v1_xiazhu_reward_content, "d");
		INIT_GAME_STR("Cross/cross_1v1/join_reward_subject", g_cross_1v1_join_reward_subject, "");
		INIT_GAME_STR("Cross/cross_1v1/join_reward_content", g_cross_1v1_join_reward_content, "");
		INIT_GAME_STR("Cross/cross_1v1/week_score_reward_subject", g_cross_1v1_week_score_reward_subject, "");
		INIT_GAME_STR("Cross/cross_1v1/week_score_reward_content", g_cross_1v1_week_score_reward_content, "dd");
		INIT_GAME_STR("Cross/cross_1v1/week_rank_reward_content", g_cross_1v1_week_rank_reward_content, "d");
		INIT_GAME_STR("Cross/cross_1v1/activity_open_notice", g_cross_1v1_activity_open_notice, "");
		INIT_GAME_STR("Cross/cross_1v1/activity_close_notice", g_cross_1v1_activity_close_notice, "s");

		INIT_GAME_STR("Cross/cross_multiuser_challenge/MultiuserChallengeScoreRankRewardSubject", g_cross_multiuser_challenge_score_rank_reward_subject, "");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/MultiuserChallengeScoreRankRewardContent", g_cross_multiuser_challenge_score_rank_reward_content, "d");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/activity_open_notice", g_cross_multiuser_challenge_open_notice, "");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/activity_close_notice", g_cross_multiuser_challenge_close_notice, "");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/day_reward_mail_content", g_cross_multiuser_challenge_day_reward_mail_content, "d");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/gongxun_reward_mail_content", g_cross_multiuser_challenge_gongxun_reward_mail_content, "");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/score_division_reward_notice", g_cross_multiuser_challenge_season_score_reward_content, "");
		INIT_GAME_STR("Cross/cross_multiuser_challenge/season_card_reward_content", g_cross_multiuser_challenge_season_card_reward_content, "");

		INIT_GAME_STR("Cross/cross_tuanzhan/occupy_pilla_content", g_cross_tuanzhan_occupy_pilla_content, "ddsd");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_winner_content", g_cross_tuanzhan_reward_winner_content, "");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_rank_winner_content", g_cross_tuanzhan_reward_rank_winner_content, "d");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_loser_content", g_cross_tuanzhan_reward_loser_content, "");
		INIT_GAME_STR("Cross/cross_tuanzhan/kill_num_content", g_cross_tuanzhan_kill_num_content, "dsdd");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_by_score_mail_content", g_cross_tuanzhan_reward_by_score_mail_content, "d");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_by_score_mail_content2", g_cross_tuanzhan_reward_by_score_mail_content2, "");
		INIT_GAME_STR("Cross/cross_tuanzhan/reward_by_score_unfetch_content", g_cross_tuanzhan_reward_by_score_unfetch_content, "");
		INIT_GAME_STR("Cross/cross_tuanzhan/broast_camp_change", g_cross_tuanzhan_broast_camp_change, "d");
		INIT_GAME_STR("Cross/cross_tuanzhan/broast_camp_switch", g_cross_tuanzhan_broast_camp_switch, "");
		INIT_GAME_STR("Cross/cross_team_fb/find_mate_notice", g_cross_team_fb_fine_mate_notice, "dsddddd");
		INIT_GAME_STR("Cross/cross_team_fb/find_mate_not_capa_notice", g_cross_team_fb_fine_mate_not_capa_notice, "dsdddd");

		INIT_GAME_STR("Cross/cross_guild_battle/own_flag_succ", g_cross_guildbattle_own_flag_succc, "dsdd");
		INIT_GAME_STR("Cross/cross_guild_battle/own_city_succ", g_cross_guildbattle_own_city_succc, "dsd");
		INIT_GAME_STR("Cross/cross_guild_battle/on_attack_warning", g_cross_guildbattle_on_attack_warning, "d");
		INIT_GAME_STR("Cross/cross_guild_battle/on_flag_hp_warning", g_cross_guildbattle_on_flag_hp_warning, "d");

		/////////////////////////////////////  跨服排行榜		/////////////////////////////////////
		INIT_GAME_STR("Cross/cross_rank/cross_add_cap_rank_content", g_cross_add_cap_rank_content, "d");
		INIT_GAME_STR("Cross/cross_rank/cross_add_charm_rank_content", g_cross_add_charm_rank_content, "d");
		INIT_GAME_STR("Cross/cross_rank/cross_add_couple_rank_qingyuan_content", g_cross_add_couple_rank_qingyuan_content, "d");

		/////////////////////////////////////////// 跨服boss //////////////////////////////////////////////////////////
		INIT_GAME_STR("Cross/cross_boss/refresh_boss", g_cross_boss_refresh_boss_content, "d");
		INIT_GAME_STR("Cross/cross_boss/refresh_cross_boss_new", g_cross_boss_refresh_cross_boss_new, "ddd");
		INIT_GAME_STR("Cross/cross_boss/refresh_boss_2", g_cross_boss_refresh_boss2_content, "d");
		INIT_GAME_STR("Cross/cross_boss/refresh_boss_3", g_cross_boss_refresh_boss3_content, "d");
		INIT_GAME_STR("Cross/cross_boss/treasure_gather_times_left", g_cross_boss_treasure_gather_times_left, "d");
		INIT_GAME_STR("Cross/cross_boss/ordinary_gather_times_left", g_cross_boss_gather_times_left, "d");
		INIT_GAME_STR("Cross/cross_boss/be_killed_in_cross_boss", g_be_killed_in_cross_boss, "dsdddsd");

		/////////////////////////////////////////// 月黑风高 //////////////////////////////////////////////////////////
		INIT_GAME_STR("Cross/cross_dark_night/first_boradcast", g_cross_dark_night_first_boradcast, "s");
		INIT_GAME_STR("Cross/cross_dark_night/kill_boss", g_cross_dark_night_kill_boss, "s");
		INIT_GAME_STR("Cross/cross_dark_night/kill_role", g_cross_dark_night_kill_role, "ss");
		INIT_GAME_STR("Cross/cross_dark_night/score_reward", g_cross_dark_night_score_reward, "sd");

		////////////////////////////////// 乱斗战场 ///////////////////////////////////////////////
		INIT_GAME_STR("Cross/cross_mess_battle/snatch_score", g_mess_battle_snatch_score, "dsddsdd");
		INIT_GAME_STR("Cross/cross_mess_battle/liangsha_notice1", g_mess_battle_liangsha_notice1, "dsdd");
		INIT_GAME_STR("Cross/cross_mess_battle/rank_top1", g_mess_battle_rank_top1, "s");
		INIT_GAME_STR("Cross/cross_mess_battle/rank_top2", g_mess_battle_rank_top2, "ss");
		INIT_GAME_STR("Cross/cross_mess_battle/rank_top3", g_mess_battle_rank_top3, "sss");



		/////////////////////////////////// 趣味钓鱼 //////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/fishing_broadcast", g_fishing_broadcast, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/be_steal_fish_content", g_be_steal_fish_content, "dsdd");
		INIT_GAME_STR("GameWorld/RandActivity/steal_fish_rank_content", g_steal_fish_rank_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/be_steal_fish_rank_content", g_be_steal_fish_rank_content, "dd");
		
		/////////////////////////////////// 植树 //////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/planting_tree_canyu_reward", g_rand_activity_planting_tree_canyu_reward, "d");
		INIT_GAME_STR("GameWorld/RandActivity/planting_tree_planting_reward", g_rand_activity_planting_tree_planting_reward, "ddd");
		INIT_GAME_STR("GameWorld/RandActivity/planting_tree_watering_reward", g_rand_activity_planting_tree_watering_reward, "ddd");

		////////////////////////////////// 达拉然水晶 /////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/steal_crystal_dalaran", g_steal_crystal_dalaran_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/crystal_dalaran_be_stealed", g_crystal_dalaran_be_stealed_reward_content, "dd");
		INIT_GAME_STR("GameWorld/RandActivity/steal_crystal_dalaran_broadcast1", g_steal_crystal_dalaran_broadcast1, "dsddsdd");
		INIT_GAME_STR("GameWorld/RandActivity/steal_crystal_dalaran_broadcast2", g_steal_crystal_dalaran_broadcast2, "dsddsdd");
		INIT_GAME_STR("GameWorld/RandActivity/steal_crystal_dalaran_broadcast3", g_steal_crystal_dalaran_broadcast3, "dsd");
		INIT_GAME_STR("GameWorld/RandActivity/be_steal_mail_content", g_be_steal_mail_content, "dsdd");

		INIT_GAME_STR("GameWorld/RandActivity/daily_love_2_reward_content", g_rand_activity_daily_love_2_reward_content, "ddd");

		////////////////////////////////// 元素之心 /////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ElementHeart/element_heart_xilian", g_element_heart_xilian, "dsdd");

		////////////////////////////////// 神印系统///////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ShenYin/shenyin_liehun", g_link_shenyin, "dsdd");

		///////////////////////////////////////	 升星助力  ////////////////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliMount", g_shengxingzhuli_mount, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliFightMount", g_shengxingzhuli_fazhen, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliWing", g_shengxingzhuli_wing, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliHalo", g_shengxingzhuli_halo, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliPifeng", g_shengxingzhuli_pifeng, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliJinglingFazhen", g_shengxingzhuli_jingling_fazhen, "dsddd");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliJinglingHalo", g_shengxingzhuli_jingling_halo, "dsddd");

		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliAutoGetRewardMailSubject", g_shengxingzhuli_mail_subject, "");
		INIT_GAME_STR("GameWorld/Shengxingzhuli/ShengxingzhuliAutoGetRewardMailContent", g_shengxingzhuli_mail_content, "");

		//////////////////////////////	 守护小鬼  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/ImpGuard/impguard_remind_mail_content", g_impguard_remind_mail_content, "dd");
		INIT_GAME_STR("GameWorld/ImpGuard/get_imp_type_1_notice", g_get_imp_type_1_notice, "dsdd");
		INIT_GAME_STR("GameWorld/ImpGuard/get_imp_type_2_notice", g_get_imp_type_2_notice, "dsdd");

		/////////////////////////////////// 每日好礼 /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/everyday_nive_gift_reward_mail", g_everyday_nice_gift_fetch_reward, "d");

		/////////////////////////////////  进阶系统奖励	/////////////////////////////////////
		INIT_GAME_STR("GameWorld/jinjiesys/reward_active", g_jinjiesys_reward_active, "dsdddd");
		INIT_GAME_STR("GameWorld/jinjiesys/reward_mail_content", g_jinjiesys_reward_mail_content, "d");

		/////////////////////////////////////// 神器特殊形象激活 ///////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Shenqi/special_image_active_content", g_shenqi_special_img_active, "dsddd");

		///////////////////////////////////////	 角色大小目标   /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/role_big_small_goal/reward_active", g_role_big_small_goal_active_reward, "dsdddd");

		/////////////////////////////////////	限时战力 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/LimitCap/time_limit_cap_img_mail_context", g_time_limit_cap_img_mail_context, "d");
		//全民比拼
		INIT_GAME_STR("GameWorld/RandActivity/mount_national_competition_content", g_mount_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/wing_national_competition_content", g_wing_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/shizhuang_national_competition_content", g_shizhuang_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/shenbing_national_competition_content", g_shenbing_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/fabao_national_competition_content", g_fabao_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/footprint_national_competition_content", g_footprint_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/halo_national_competition_content", g_halo_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/shengong_national_competition_content", g_shengong_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/qilinbi_national_competition_content", g_qilinbi_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/toushi_national_competition_content", g_toushi_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/yaoshi_national_competition_content", g_yaoshi_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/lingtong_national_competition_content", g_lingtong_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/linggong_national_competition_content", g_linggong_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/lingqi_national_competition_content", g_lingqi_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/fight_mount_national_competition_content", g_fight_mount_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/flypet_national_competition_content", g_flypet_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/weiyan_national_competition_content", g_weiyan_national_competition_content, "");
		INIT_GAME_STR("GameWorld/RandActivity/shenyi_national_competition_content", g_shenyi_national_competition_content, "");

		/////////////////////////////////////	神兵图录 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RedEquipCollect/red_equip_collect_notice_content", g_red_equip_collect_notice_content, "dsdd");
		INIT_GAME_STR("GameWorld/RedEquipCollect/red_equip_collect_act_reward_mail_content", g_red_equip_collect_act_reward_mail_content, "");
		INIT_GAME_STR("GameWorld/RedEquipCollect/red_equip_collect_fetch_title_notice_content", g_red_equip_collect_fetch_title_notice_content, "dsddd");
		INIT_GAME_STR("GameWorld/OrangeEquipCollect/orange_equip_collect_notice_content", g_orange_equip_collect_notice_content, "dsdd");

		/////////////////////////////////////	仙尊卡 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/XianZunKa/xian_zun_ka_stale_dated", g_xian_zun_ka_stale_dated, "");

		/////////////////////////////////////	boss图鉴 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/BossCard/boss_card_activity_this_type", g_boss_card_activity_this_type, "dsdd");

		/////////////////////////////////////	装备洗炼 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/EquipBaptize/baptize_four_purple", g_equip_baptize_four_purple_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/EquipBaptize/baptize_orange", g_equip_baptize_orange_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/EquipBaptize/baptize_red", g_equip_baptize_red_notice, "dsddsd");
		INIT_GAME_STR("GameWorld/EquipBaptize/baptize_orange_and_red", g_equip_baptize_orange_and_red_notice, "dsddsdd");

		//////////////////////////////////// 夫妻家园  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Spousehome/spousehome_buy_room_for_lover", g_spousehome_buy_room_for_lover, "dsdd");
		INIT_GAME_STR("GameWorld/Spousehome/spousehome_equip_furniture_for_lover", g_spousehome_equip_furniture_for_lover, "dsdd");

		/////////////////////////////////////// 名将 //////////////////////////////////////////
		INIT_GAME_STR("GameWorld/greate_soldier/draw_content", g_greate_soldier_draw_content, "dsdd");
		INIT_GAME_STR("GameWorld/greate_soldier/greate_soldier_fetch_reward_content", g_greate_fetch_reward_content, "");

		///////////////////////////////////// 天书寻主 /////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/tianshuxunzhu/fetch_reward", g_tianshuxunzhu_fretch_reward, "dsdd");

		///////////////////////////////////////	幸运云购  /////////////////////////////////////////
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_reward_mail", g_lucky_cloud_buy_reward_mail, "s");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_return_mail", g_lucky_cloud_buy_return_mail, "");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_no_reward_mail", g_lucky_cloud_buy_no_reward_mail, "");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_min_reward_mail", g_lucky_cloud_buy_min_reward_mail, "s");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_reward", g_lucky_cloud_buy_reward, "ddsd");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_noreward", g_lucky_cloud_buy_noreward, "");
		INIT_GAME_STR("GameWorld/RandActivity/lucky_cloud_buy_can_join", g_lucky_cloud_buy_can_join, "");

		/////////////////////////////////////////// 跨服随机活动 ///////////////////////////////////////////////////////////////
		INIT_GAME_STR("Cross/cross_rand_activity/chongzhi_rank_mvp_reward", g_cross_ra_chongzhi_rank_mvp_reward, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/chongzhi_rank_join_reward", g_cross_ra_chongzhi_rank_join_reward, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/chongzhi_rank_low_mvp_reward", g_cross_ra_chongzhi_rank_low_mvp_reward, "ddd");
		INIT_GAME_STR("Cross/cross_rand_activity/chongzhi_rank_low_join_reward", g_cross_ra_chongzhi_rank_low_join_reward, "ddd");

		INIT_GAME_STR("Cross/cross_rand_activity/cloud_purchase_return_gold", g_cross_ra_cloud_purchase_return_gold, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/cloud_purchase_reward_score", g_cross_ra_cloud_purchase_reward_score, "d");
		INIT_GAME_STR("Cross/cross_rand_activity/cloud_purchase_get_reward", g_cross_ra_cloud_purchase_get_reward, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/cloud_purchase_reward_notic", g_cross_ra_cloud_reward_notic, "sddd");
		INIT_GAME_STR("Cross/cross_rand_activity/cloud_purchase_act_close", g_cross_ra_cloud_purchase_act_close, "ddd");

		INIT_GAME_STR("Cross/cross_rand_activity/consume_rank_mvp_reward", g_cross_ra_consume_rank_mvp_reward, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/consume_rank_join_reward", g_cross_ra_consume_rank_join_reward, "dd");
		INIT_GAME_STR("Cross/cross_rand_activity/consume_rank_low_mvp_reward", g_cross_ra_consume_rank_low_mvp_reward, "ddd");
		INIT_GAME_STR("Cross/cross_rand_activity/consume_rank_low_join_reward", g_cross_ra_consume_rank_low_join_reward, "ddd");

		/////////////////////////////////////////// 神话系统 //////////////////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/Myth/myth_uplevel", g_myth_uplevel_broadcast, "dsddd");
		INIT_GAME_STR("GameWorld/Myth/myth_activate", g_myth_activate_broadcast, "dsdd");
		INIT_GAME_STR("GameWorld/Myth/myth_resonance", g_myth_resonance, "dsddd");
		INIT_GAME_STR("GameWorld/Myth/myth_compose", g_myth_compose_broadcast, "dsdd");

		//////////////////////////////////////////////// 周末装备 ////////////////////////////////////////////
		INIT_GAME_STR("GameWorld/tianshenhuti/combine_notice", g_tianshenhuti_combine_notice, "dsdd");
		INIT_GAME_STR("GameWorld/weekend_boss/weekend_score_boss_rank_reward", g_weekend_score_boss_rank_reward, "d");

		return true;
	}
}

