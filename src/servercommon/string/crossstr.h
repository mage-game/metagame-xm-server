#ifndef __CROSSSTR_H__
#define __CROSSSTR_H__

namespace gamestring
{
	extern char GAMESTRING_BUF[2048];

	/////////////////////////////////////// cross 1v1  /////////////////////////////////////////
	extern char *g_cross_1v1_dur_kill_notice;
	extern char *g_cross_1v1_end_dur_kill_notice;
	extern char *g_cross_1v1_join_reward_subject;
	extern char *g_cross_1v1_join_reward_content;
	extern char *g_cross_1v1_week_score_reward_subject;
	extern char *g_cross_1v1_week_score_reward_content;
	extern char *g_cross_1v1_week_rank_reward_content;
	extern char *g_cross_1v1_activity_open_notice;
	extern char *g_cross_1v1_activity_close_notice;

	extern char *g_cross_multiuser_challenge_score_rank_reward_subject;
	extern char *g_cross_multiuser_challenge_score_rank_reward_content;
	extern char *g_cross_multiuser_challenge_open_notice;
	extern char *g_cross_multiuser_challenge_close_notice;
	extern char *g_cross_multiuser_challenge_day_reward_mail_content;
	extern char *g_cross_multiuser_challenge_finish_reward_mail_content;
	extern char *g_cross_multiuser_challenge_gongxun_reward_mail_content;
	extern char *g_cross_multiuser_challenge_season_score_reward_content;
	extern char *g_cross_multiuser_challenge_season_card_reward_content;

	extern char *g_cross_team_fb_fine_mate_notice;
	extern char *g_cross_team_fb_fine_mate_not_capa_notice;

	///////////////////////////////////////	 跨服团战   /////////////////////////////////////////
	extern char *g_cross_tuanzhan_occupy_pilla_content;
	extern char *g_cross_tuanzhan_reward_winner_content;
	extern char *g_cross_tuanzhan_reward_rank_winner_content;
	extern char *g_cross_tuanzhan_reward_loser_content;
	extern char *g_cross_tuanzhan_kill_num_content;
	extern char *g_cross_tuanzhan_reward_by_score_mail_content;
	extern char *g_cross_tuanzhan_reward_by_score_mail_content2;
	extern char *g_cross_tuanzhan_reward_by_score_unfetch_content;
	extern char *g_cross_tuanzhan_broast_camp_change;
	extern char *g_cross_tuanzhan_broast_camp_switch;

	//////////////////////////////////////// 跨服帮派战 ////////////////////////////////////////
	extern char *g_cross_guildbattle_own_flag_succc;
	extern char *g_cross_guildbattle_own_city_succc;
	extern char *g_cross_guildbattle_on_attack_warning;
	extern char *g_cross_guildbattle_on_flag_hp_warning;

	///////////////////////////////////////	 跨服BOSS   /////////////////////////////////////////
	extern char *g_cross_boss_refresh_boss_content;
	extern char *g_cross_boss_refresh_cross_boss_new;
	extern char *g_cross_boss_refresh_boss2_content;
	extern char *g_cross_boss_refresh_boss3_content;
	extern char *g_cross_boss_treasure_gather_times_left;
	extern char *g_cross_boss_gather_times_left;
	extern char *g_be_killed_in_cross_boss;

	////////////////////////////////////// 月黑风高 /////////////////////////////////////////
	extern char *g_cross_dark_night_first_boradcast;
	extern char *g_cross_dark_night_kill_boss;
	extern char *g_cross_dark_night_kill_role;
	extern char *g_cross_dark_night_score_reward;

	//////////////////////////////////////// 乱斗战场 ////////////////////////////////////////
	extern char *g_mess_battle_snatch_score;
	extern char *g_mess_battle_liangsha_notice1;
	extern char *g_mess_battle_rank_top1;
	extern char *g_mess_battle_rank_top2;
	extern char *g_mess_battle_rank_top3;

	/////////////////////////////////////  跨服排行榜		/////////////////////////////////////
	extern char *g_cross_add_cap_rank_content;
	extern char *g_cross_add_charm_rank_content;
	extern char *g_cross_add_couple_rank_qingyuan_content;

	extern char  *g_cross_ra_cloud_purchase_return_gold;
	extern char  *g_cross_ra_cloud_purchase_reward_score;
	extern char  *g_cross_ra_cloud_purchase_get_reward;
	extern char  *g_cross_ra_cloud_reward_notic;
	extern char	 *g_cross_ra_cloud_purchase_act_close;

	bool StringInit(const char *filename, std::string *error);
}

#endif // __CROSSSTR_H__

