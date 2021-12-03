#ifndef __ROLE_CROSS_DEF_HPP__
#define __ROLE_CROSS_DEF_HPP__

#include "servercommon.h"
#include "servercommon/crossdef.hpp"

#pragma pack(push, 4)

// ��ɫ������߼�¼.  ���ڲ��ֳ��������������ڻ�ĳ����߼��������ü�¼��������ÿ��������õ�������
struct RoleCrossLogoutRecord
{
	// ע�����
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

	short logout_flag;				// 0��ʾ��Ч��1��ʾ����
	short cross_activity_type;		// �μӵĿ�������

	int sceneId;
	int sceneKey;
	unsigned int logout_timestamp;	// ����ʱ���

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

	long long cross_original_uuid;										// ԭ��ΨһID
	int merge_server_id;												// �Ϸ�ID
	short cross_xiuluo_tower_buy_realive_count;							// ������������򸴻����
	char cross_xiuluo_tower_add_gongji_per;								// ������������ӹ�������
	char cross_xiuluo_tower_add_hp_per;									// ���������������Ѫ����

	//////////////////////////////////////////	  ���1v1�	 ///////////////////////////////////////////////
	unsigned int cross_1v1_match_time;									// ���1v1����ƥ��ʱ��
	unsigned int cross_1v1_info_last_clear_time;						// ���1v1��һ����Ϣ����ʱ��
	int cross_1v1_score;												// ���1v1���֣�������
	int cross_1v1_day_join_count;										// ���1v1���ղ������

	short cross_1v1_today_buy_times;									// ���1v1���չ������
	short cross_1v1_fetch_score_reward_flag;							// ���1v1��ȡ���ֽ������
	short cross_1v1_join_time_reward_flag;								// ���1v1���������ȡ���
	short reserve_sh1;
	short cross_1v1_dur_win_count;										// ���1v1��ʤ��
	short cross_1v1_dur_win_max_count;									// ���1v1�����ʤ��
	unsigned int cross_1v1_curr_activity_begin_timestap;				// ���1v1�������ʼʱ���
	int cross_1v1_curr_activity_add_score;								// ���1v1��������ӵ�����
	int cross_1v1_gongxun;                                              // ���1v1��ѫ
	char cross_1v1_season_reward[CROSS_1V1_SEASON_MAX];                 // ���1v1 �����ָ

	char cross_1v1_season_reward_use_old[2];							// ���1v1 ʹ�ý�ָ(�����������ں�����չ��)
	short cross_1v1_total_join_times;                                   // ���1v1 �����ܵĲ������

	int cross_1v1_cur_join_season;                                      // ��ǰ�μ�1v1�ڼ�����
	short cross_1v1_total_win_times;                                    // ���1v1 �����ܵ�ʤ������
	//-----------------------------------------------------------------------------------------------------------
	short cross_pasture_today_get_score_times;							// ������������ջ�û��ֵĴ���

	int cross_multiuser_challenge_matching_state;						// ���3V3��ǰƥ��״̬
	int cross_multiuser_challenge_score;								// ���3V3����
	int cross_multiuser_challenge_total_match_count;					// ���3V3һ����������(��ʱû��)
	int cross_multiuser_challenge_win_match_count;						// ���3V3��ʤ��������
	int cross_multiuser_challenge_mvp_count;							// ���3V3MVP����
	

	unsigned int cross_pasture_score;									// ����������ܻ���

	int today_kill_boss_num;											// ���boss���ջ�ɱboss����
	short cross_boss_relive_times;										// ���boss�����������
	short cross_boss_enter_times_limit;
	int today_gather_ordinary_crystal_num;								// ���boss�ɼ���ͨˮ������
	int today_gather_treasure_crystal_num;								// ���boss�ɼ���ϧˮ������

	short cross_boss_scene_level;
	char cross_guildbattle_tuanzhang_prof;
	char cross_guildbattle_tuanzhang_sex;

	int cross_team_fb_pass_flag;										// �����ӱ�ͨ�ر��
	int cross_team_fb_day_count;										// �����ӱ�ÿ�մ���

	int cross_guildbattle_scene_id;										// ������ɽ��볡��id
	GameName cross_guild_tuanzhang_name;								// ��������ų�����
	int cross_guildbattle_server_reward_flag;							// �������սȫ������
	int cross_guildbattle_guild_reward_flag;							// �������ս���ɽ���
	int cross_guildbattle_recalc_time;
	int cross_guildbattle_task_reward_flag[CROSS_GUILDBATTLE_MAX_SCENE_NUM];							// �����ս������ɱ�־
	int cross_guildbattle_task_record[CROSS_GUILDBATTLE_MAX_SCENE_NUM][CROSS_GUILDBATTLE_MAX_TASK_NUM]; // �����ս�����¼

	char cross_xiuluotower_daily_drop_title_get_num;					// �������������ƺŻ����
	char cross_xiuluotower_daily_gold_box_title_get_num;				// ��������������ӳƺŻ����
	char cross_xiuluotower_daily_drop_mount_get_num;					// ���������������������
	char cross_guild_battle_daily_drop_wing_get_num;					// �������ս������������

	unsigned int cross_xiuluo_tower_gather_buff_end_timestamp;			// ����������ɼ�buff����ʱ���

	
	short cross_multiuser_challenge_dur_win_count;						// ���3V3��ʤ����
	char cross_3v3_season_reward_use_old[2];                            // ���3v3 ����ʹ�õ�����(�洢������������ʾʹ�����ĸ�����������) �����������������չ��
	char cross_3v3_season_reward[CROSS_3V3_SEASON_MAX];                 // ���3v3 �Ѿ����������(�洢���Ƕ�λ����ʾ��Ӧ�����������ĸ���λ������)

	char cross_3v3_score_reuse_flag;									// ���3v3�Ļ����ֶ��������ñ�ǣ�����֮ǰҪ���㣬��������
	char cross_3v3_join_reward_fetch_flag;								// ���3v3���뽱��ȡ��ǣ�ÿ����գ�
	unsigned short cross_multiuser_challenge_month_match_count;			// ���3V3��ǰ�����μӱ���������ÿ�������㣩

	short cross_3v3_cur_join_season;                                    // ��ǰ�μ�3v3�ڼ�����
	short cross_3v3_gongxun_reward_fetch_flag;							// ���3v3��ѫ����ȡ���
	int cross_3v3_gongxun_value;										// ���3v3��ѫֵ

	short cross_liekun_scene_zone;										// �������ش�����
	
	char cross_3v3_logout_flag;											// ���3v3���߱��
	char cross_husong_shuijing_enter_type;								// �������ˮ�����������

	unsigned int cross_boss_concern_flag[CROSS_BOSS_SCENE_MAX];			// ���boss��ע���	
	int cross_guildbattle_daily_reward_flag;							// �������սռ��ÿ�콱��

	RoleCrossLogoutRecord role_cross_logout_record;						// ��ɫ������߼�¼����

	short cross_common_boss_enter_type;									// �����ͨboss��������
	unsigned short cross_common_boss_enter_boss_id;						// �����ͨboss����bossid
	int cross_common_boss_enter_scene_id;								// �����ͨboss���볡��id

	unsigned int cross_mizang_boss_concern_flag[CROSS_MIZANG_BOSS_SCENE_MAX];	// ����ز�boss��ע���
	int cross_mizang_boss_today_kill_boss_num;							// ����ز�boss���ջ�ɱboss����
	int cross_mizang_boss_today_gather_ordinary_crystal_num;			// ���boss�ɼ���ͨˮ������
	int cross_mizang_boss_today_gather_treasure_crystal_num;			// ���boss�ɼ���ϧˮ������
	short cross_mizang_boss_scene_level;                                // ����ز�boss
	short cross_mizang_boss_relive_times;								// ���boss�����������

	unsigned int cross_youming_boss_concern_flag[CROSS_YOUMING_BOSS_SCENE_MAX];	// ����ز�boss��ע���
	int cross_youming_boss_today_kill_boss_num;							// ����ز�boss���ջ�ɱboss����
	int cross_youming_boss_today_gather_ordinary_crystal_num;			// ���boss�ɼ���ͨˮ������
	int cross_youming_boss_today_gather_treasure_crystal_num;			// ���boss�ɼ���ϧˮ������
	short cross_youming_boss_scene_level;                                // ����ز�boss
	short cross_youming_boss_relive_times;								// ���boss�����������

	char cross_1v1_season_reward_use[CROSS_ACTIVITY_1V1_RING_USE_COUNT];	// ���1v1 ʹ�ý�ָ
	char cross_3v3_season_reward_use[CROSS_ACTIVITY_3V3_RING_USE_COUNT];	// ���3v3 ����ʹ�õ�����(�洢������������ʾʹ�����ĸ�����������)

	int cross_consume_gold_num;											// �������Ԫ��
	int cross_daily_consume_gold_num;									// ���ÿ������Ԫ��
	long long cross_guildbattle_tuanzhang_uuid;							// ��������ų�uuid

	short enter_cross_move_to_pos_x;									// ������������ֱ���ƶ�������x
	short enter_cross_move_to_pos_y;									// ������������ֱ���ƶ�������y
	int cross_1v1_cur_join_season_dayid;                                // ��ǰ�μ�1v1�����Ŀ�������
	int cross_3v3_cur_join_season_dayid;                                // ��ǰ�μ�3v3�����Ŀ�������

};

typedef char RoleCrossParamHex[sizeof(RoleCrossParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCrossParamHex) < 2048);

#pragma pack(pop)

#endif // __ROLE_CROSS_DEF_HPP__