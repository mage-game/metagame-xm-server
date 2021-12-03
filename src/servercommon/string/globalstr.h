#ifndef __GLOBALSTR_H__
#define __GLOBALSTR_H__

namespace gamestring
{
	extern char GAMESTRING_BUF[2048];

	bool StringInit(const char *filename, std::string *error);

	///////////////////////////////////////  拍卖  /////////////////////////////////////////
	extern char *g_publicsale_item_full_subject;
	extern char *g_publicsale_item_full_content;
	extern char *g_publicsale_item_cancel_subject;
	extern char *g_publicsale_item_cancel_content;
	extern char *g_publicsale_item_expire_subject;
	extern char *g_publicsale_item_expire_content;
	extern char *g_publicsale_buy_succ_subject;
	extern char *g_publicsale_buy_succ_content;
	extern char *g_publicsale_buy_fail_subject;
	extern char *g_publicsale_buy_fail_content;
	extern char *g_publicsale_sell_succ_subject;
	extern char *g_publicsale_sell_succ_content;
	extern char *g_publicsale_sell_to_world;

	extern char *g_create_guild_mail_subject;
	extern char *g_create_guild_mail_content;
	extern char *g_dimiss_guild_mail_subject;
	extern char *g_dimiss_guild_mail_content;
	extern char *g_auto_dimiss_guild_mail_subject;
	extern char *g_auto_dimiss_guild_mail_content;
	extern char *g_gm_dimiss_guild_mail_subject;
	extern char *g_gm_dimiss_guild_mail_content;
	extern char *g_guild_downgrade_subject;
	extern char *g_guild_downgrade_content;
	extern char *g_guild_warning_dismiss_subject;
	extern char *g_guild_warning_dismiss_content;
	extern char *g_guild_warning_dismiss_byexp_subject;
	extern char *g_guild_warning_dismiss_byexp_content;
	extern char *g_guild_upgrade_level_subject;
	extern char *g_guild_upgrade_level_content;
	extern char *g_join_guild_mail_subject;
	extern char *g_join_guild_mail_content;
	extern char *g_bekicked_guild_mail_subject;
	extern char *g_bekicked_guild_mail_content;
	extern char *g_guild_tuanzhang_appointed_subject;
	extern char *g_guild_tuanzhang_appointed_content;

	extern char *g_guild_tuanzhang_delated_subject;
	extern char *g_guild_tuanzhang_delated_content;
	extern char *g_guild_tuanzhang_autodelated_subject;
	extern char *g_guild_tuanzhang_autodelated_content;

	///////////////////////////////////////  军团  /////////////////////////////////////////
	extern char *g_create_guild_succ_public_notice;
	extern char *g_create_lv2_guild_succ_public_notice;
	extern char *g_msg_join_guild;
	extern char *g_msg_leave_guild;
	extern char *g_msg_appointed_subject;
	extern char *g_msg_futuanzhang_be_appointed;
	extern char *g_msg_zhanglao_be_appointed;
	extern char *g_msg_jingying_be_appointed;
	extern char *g_msg_hufa_be_appointed;
	extern char *g_msg_guild_notice_changed;
	extern char *g_msg_guild_leave_post;
	extern char *g_guild_boss_called_notice;
	extern char *g_guild_boss_called_chatmsg;
	extern char *g_guild_rename_notice;
	extern char *g_guild_call_beast_content;
	extern char *g_guild_kill_beast_content;
	extern char *g_guild_rename_subject;
	extern char *g_guild_rename_content;
	extern char *g_msg_guild_reset_name;
	extern char *g_msg_member_reset_name;
	extern char *g_guild_tianci_tongbi_notice;
	extern char *g_guild_xianfanguplevel;
	extern char *g_guild_shenluuplevel;
	extern char *g_guild_shopuplevel;
	extern char *g_guild_cangkuuplevel;
	extern char *g_guild_yaojianguplevel;
	extern char *g_guild_bossuplevel;
	extern char *g_guild_bossfeed1;
	extern char *g_guild_bossfeed2;
	extern char *g_guild_bossfeed3;
	extern char *g_guild_taskcolor;
	extern char *g_guild_taskluck;
	extern char *g_guild_guilduplevel;
	extern char *g_guild_taskscore_notice;
	extern char *g_msg_guild_capabilipy_notice;
	extern char *g_msg_guild_invite_blessing_notice;
	extern char *g_guild_tower_defend_notice;

	extern char *g_msg_guild_juanxian_bygold;
	extern char *g_msg_guild_juanxian_byitem;
	extern char *g_msg_guild_juanxian_bycoin;
	extern char *g_msg_guild_boss_feed;
	extern char *g_guild_change_notice_cd_error;
	extern char *g_guild_mail_all_cd_error;

	extern char *g_guild_open_bonfire;
	extern char *g_guild_box_assist_subject;
	extern char *g_guild_box_assist_content;
	extern char *g_guild_boss_call_notice;
	extern char *g_guild_boss_call_content;
	extern char *g_guild_boss_reward_content;

	///////////////////////////////////////  邮件  /////////////////////////////////////////
	extern char *g_mail_receiver_full_subject;
	extern char *g_mail_receiver_full_content;

	///////////////////////////////////////	 世界BOSS  /////////////////////////////////////////
	extern char *g_boss_refresh;
	extern char *g_refresh_three_boss;
	extern char *g_refresh_one_boss;

	///////////////////////////////////////	 排行榜  /////////////////////////////////////////
	extern char *g_toprank_new_first_broadcast;
	extern char *g_toprank_change_first_broadcast;
	extern char *g_honour_rank_reward_subject;
	extern char *g_honour_rank_reward_content;

	extern char *g_toprank_capability_top_user_changed_notice;

	extern char *g_toprank_capability_user_login;
	extern char *g_toprank_ten_capability_user_login;
	extern char *g_meilirank_capability_user_login;

	///////////////////////////////////////	 合服活动  /////////////////////////////////////////
	extern char *g_csa_reward_subject;
	extern char *g_csa_chongzhi_content;
	extern char *g_csa_stone_score_content;
	extern char *g_csa_gcz_owner_content;
	extern char *g_csa_pet_capability_content;
	extern char *g_csa_mount_capability_content;
	extern char *g_csa_strengthen_score_content;
	extern char *g_csa_card_capability_content;
	extern char *g_csa_prof_capability_content;
	extern char *g_csa_chongzhi_none_notice;
	extern char *g_csa_stone_score_none_notice;
	extern char *g_csa_gcz_owner_none_notice;
	extern char *g_csa_pet_capability_none_notice;
	extern char *g_csa_mount_capability_none_notice;
	extern char *g_csa_strengthen_score_none_notice;
	extern char *g_csa_card_capability_none_notice;
	extern char *g_csa_prof_capability_none_notice;
	extern char *g_csa_chongzhi_notice;
	extern char *g_csa_stone_score_notice;
	extern char *g_csa_gcz_owner_notice;
	extern char *g_csa_pet_capability_notice;
	extern char *g_csa_mount_capability_notice;
	extern char *g_csa_strengthen_score_notice;
	extern char *g_csa_card_capability_notice;
	extern char *g_csa_prof_capability_notice;
	extern char *g_csa_chongzhi_end_notice;
	extern char *g_csa_stone_end_score_notice;
	extern char *g_csa_gcz_owner_end_notice;
	extern char *g_csa_pet_capability_end_notice;
	extern char *g_csa_mount_capability_end_notice;
	extern char *g_csa_strengthen_score_end_notice;
	extern char *g_csa_card_capability_end_notice;
	extern char *g_csa_prof_capability_end_notice;
	extern char *g_csa_prof2_capability_end_notice;
	extern char *g_csa_prof3_capability_end_notice;
	extern char *g_csa_prof4_capability_end_notice;
	extern char *g_csa_chongzhi_change_notice;
	extern char *g_csa_stone_score_change_notice;
	extern char *g_csa_gcz_owner_change_notice;
	extern char *g_csa_pet_capability_change_notice;
	extern char *g_csa_mount_capability_change_notice;
	extern char *g_csa_strengthen_score_change_notice;
	extern char *g_csa_card_capability_change_notice;
	extern char *g_csa_prof_capability_change_notice;

	///////////////////////////////////////	 合服庆典  /////////////////////////////////////////
	extern char *g_csqd_consume_rank_reward_subject;
	extern char *g_csqd_consume_rank_reward_content;
	extern char *g_csqd_chongzhi_reward_subject;
	extern char *g_csqd_chongzhi_reward_content;
	extern char *g_csqd_chongzhi_rank_reward_subject;
	extern char *g_csqd_chongzhi_rank_reward_content;

	///////////////////////////////////////	 开服活动  /////////////////////////////////////////
	extern char *g_open_game_guild_rank_reward_subject;
	extern char *g_open_game_guild_rank_reward_content;
	extern char *g_open_game_consume_rank_reward_subject;
	extern char *g_open_game_consume_rank_reward_content;

	///////////////////////////////////////	 商城  /////////////////////////////////////////
	extern char *g_buy_open_game_shop_limit_item;

	///////////////////////////////////////	 刺杀城主  /////////////////////////////////////////
	extern char *g_cscz_open_notice;

	///////////////////////////////////////	 评价  /////////////////////////////////////////
	extern char *g_evalute_admire_reward_notice;
	extern char *g_evalute_admire_reward_subject;
	extern char *g_evalute_admire_reward_content;
	extern char *g_evalute_contempt_reward_notice;
	extern char *g_evalute_contempt_reward_subject;
	extern char *g_evalute_contempt_reward_content;

	///////////////////////////////////////	 通用活动  /////////////////////////////////////////
	extern char *g_common_activity_daily_give_flower_rank_reward_subject;
	extern char *g_common_activity_daily_give_flower_rank_reward_content;

	extern char *g_common_activity_female_day_charm_rank_reward_subject;
	extern char *g_common_activity_female_day_charm_rank_reward_content;

	extern char *g_common_activity_online_rand_reward_subject;
	extern char *g_common_activity_online_rand_reward_content;
	extern char *g_common_activity_online_rand_reward_notice;

	extern char *g_common_activity_guild_capability_increase_rank_reward_subject;
	extern char *g_common_activity_guild_capability_increase_rank_reward_content;
	///////////////////////////////////////	 结婚  /////////////////////////////////////////
	extern char *g_marry_reserve_subject;
	extern char *g_marry_reserve_content;
	extern char *g_marry_zhunbei_notice;
	extern char *g_marry_zhunbei_subject;
	extern char *g_marry_zhunbei_content;
	extern char *g_marry_baitiandi_finish;
	extern char *g_marry_hunyan_invite_subject;
	extern char *g_marry_hunyan_invite_content;
	extern char *g_marry_hunyan_zhunbei_notice;
	extern char *g_marry_hunyan_bless_subject;
	extern char *g_marry_baitiandi_start;
}

#endif // __GLOBALSTR_H__

