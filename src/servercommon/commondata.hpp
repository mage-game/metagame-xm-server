
#ifndef __COMMON_DATA_HPP__
#define __COMMON_DATA_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"

#pragma pack(push) 
#pragma pack(4)

struct CommonDataParam
{
	enum INITFLAG
	{
		INITFLAG_NUQI = 0,
		INITFLAG_ENERGY,
		INITFLAG_MAX,
	};

	static const int SHANGGU_BOSS_MAX_LAYER = 6;						// �Ϲ�BOSS - ��������ͬ�ϣ�
	static const int EQUIP_IMPGUARD_MAX_NUM = 2;						// ����װ��С������
	static const int NOTICE_DAY_MAX_NUM = 16;							// ����Ԥ������ֽ���
	static const int NOTICE_DAY_MAX_ID = NOTICE_DAY_MAX_NUM * 8;		// ����Ԥ�����id [1-127]
	UNSTD_STATIC_CHECK(INITFLAG_MAX <= 30)
	static const int BOSS_FAMILY_INFO_MAX_COUNT = 128;		

	CommonDataParam() 
	{
		this->Reset();
	}

	void Reset()
	{
		init_flag = 0;
		nuqi = 0;
		energy = 0;
		last_give_energy = 0;
		shengwang = 0;

		lover_uid = 0;
		memset(lover_name, 0, sizeof(lover_name));

		last_marry_time = 0;
		last_divorce_time = 0;

		week_kill_boss_count = 0;

		red_paper_consume_gold_num = 0;
		red_paper_lucky_value = 0;

		next_fake_private_chat_timestamp = 0;
		fake_private_chat_times = 0;

		last_logout_timestamp = 0;

		day_hunyan_gather_bind_coin = 0;
		day_hunyan_add_nvwashi = 0;

		plat_spid = 0;

		has_send_welcome_mail = 0;
		is_fetch_worldboss_week_reward = 0;
		cross_equip_itemid = 0;

		cross_honor = 0;

		cross_equip_gain_timestamp = 0;

		m_day_revival_times = 0;
		daily_task_star_level = 0;
		recored_use_norex_num = 0;
		memset(use_norex_day_times, 0, sizeof(use_norex_day_times));

		yaoshouguangchang_score = 0;
		yaoshouguangchang_lv = 0;

		suoyaota_score = 0;
		suoyaota_lv = 0;

		yaoshouguangchang_max_score = 0;
		suoyaota_max_score = 0;

		expbottle_use_count = 0;
		expbottle_transform_count = 0;

		space_renqi = 0;
		space_getgift_count = 0;
		space_day_cai_addhuoli = 0;
		space_huoli = 0;
		memset(space_xinqing, 0, sizeof(space_xinqing));

		dabaoboss_validtimestamp = 0;

		space_last_seek_gift_timestamp = 0;
		space_last_seek_msg_timestamp = 0;

		zhuagui_day_gethunli = 0;
		zhuagui_day_catch_count = 0;

		next_extra_drop_timestamp = 0;

		paohuan_complete_count = 0;
		paohuan_getphase_reward_idx = 0;

		jinghua_husong_commit_count = 0;
		jinghua_husong_status = 0;
		jinghua_husong_invalid_time = 0;	
		
		day_guild_red_paper_consume_gold_num = 0;
		day_hunyan_fa_blessgift_count = 0;
		m_next_canchangecamp_timestamp = 0;

		cur_use_bubble_type = -1;

		memset(reserve_list, 0, sizeof(reserve_list));

		day_gongxun = 0;

		equip_xunbao_times = 0;
		jl_xunbao_times = 0;
		hchz_kill_count = 0;
		gcz_kill_count = 0;

		rare_chestshop_times = 0;

		jump_remain_times = 0;
		jump_last_recover_time = 0;

		move_chess_free_times = 0;				
		move_chess_reset_times = 0;	
		move_chess_cur_step = 0;
		move_chess_next_free_time = 0;
		millionaire_valid_perform_skill_time = 0;
		millionaire_gather_count = 0;
		is_turn_millionaire = 0;
		is_turn = 0;

		miku_boss_weary = 0;						
		dabao_angry_value = 0;						

		world_chat_count1 = 0;
		world_chat_count2 = 0;
		world_chat_record_time1 = 0;
		world_chat_record_time2 = 0;

		zhuagui_get_drop_time = 0;
		zhuagui_score = 0;				

		hunjie_item_id = 0;								
		hunjie_star_level = 1;
		hunyan_buy_fb_join_times = 0;
		last_buy_hunyan_fb_times_dayid = 0;

		millionaire_last_join_time = 0;
		millionaire_reward_index = -1;
		accept_task_change_appearn = 0;

		water_self_love_tree_time = 0;
		water_other_love_tree_time = 0;

		zhuagui_get_couple_drop_time = 0;

		next_recover_hp_time = 0;

		zhuagui_kill_boss_count = 0;

		dabao_kick_time = 0;

		mikuboss_relive_weary = 0;
		miku_relive_weary_last_die_time = 0;

		auto_join_guild_last_time = 0;

		rev_sh = 0;

		daily_guild_redbag_reward_fetch_flag = 0;

		team_equip_fb_pass_flag = 0;
		team_equip_fb_day_count = 0;
		team_equip_fb_day_buy_count = 0;
		daily_use_guild_relive_times = 0;
		daily_use_free_song_hua_times = 0;
		today_use_free_relive_num = 0;

		upgrade_skill_next_skill_index = -1;	
		upgrade_skill_skill_active_flag = 0;	
		upgrade_skill_attack_count = 0;	

		millionaire_use_skill_times = 0;
		red_paper_be_singal_chat_count = 0;

		advance_notice_last_fectch_id = 0;
		today_putong_hunyan_times = 0;	

		world_question_last_answer_time = 0;
		world_question_right_num = 0;
		world_question_right_num_change_time = 0;

		guild_question_last_answer_time = 0;
		guild_question_right_num = 0;
		red_paper_be_singal_last_be_chat_time = 0;	
		last_send_heli_notice_time = 0;	
		is_auto_dec_offline_friend = 1;

		covertshop_guild_last_notice_time = 0;

		collect_drop_num = 0;
		jingling_advantage_gather_blue = 0;

		last_signin_timestamp = 0;
		guild_signin_fetch_reward_flag = 0;
		famous_man_flag = 0;
		guild_battle_fetch_daily_reward = 0;

		accept_task_change_appearn_param = 0;
		challenge_field_best_rank_break_level = 0;
		jingjie_level = 0;
		
		baby_boss_kick_time = 0;
		baby_boss_angry_value = 0;
		baby_boss_enter_times = 0;

		active_boss_weary = 0;
		is_pass_build_tower_fb = 0;
		appearacne_body_use_type = 0;

		res_sh_1 = 0;
		memset(shanggu_boss_concern_flag_list, 0, sizeof(shanggu_boss_concern_flag_list));

		imp_guard_mail_flag = 0;                            // С��

		qingyuan_capablity = 0;
		baby_capablity = 0;
		little_pet_capablity = 0;

		dabao_next_add_angry_timestamp = 0;

		appearacne_wuqi_use_type = 0;
		avatar_changed = 0;								// �Ƿ���ȡ�����˻�ͷ����
		cross_boss_relive_tire = 0;						// ���boss����ƣ��

		cross_boss_relive_tire_reset_time = 0;		// ����ƣ������ʱ��
		cross_boss_can_realive_time = 0;

		cur_use_avatar_type = -1;

		memset(imp_guard_type, 0, sizeof(imp_guard_type));
		memset(imp_guard_end_timestamp, 0, sizeof(imp_guard_end_timestamp));

		jinjiesys_reward_flag = 0;
		jinjiesys_can_reward_flag = 0;
		jinjiesys_reward_flag_1 = 0;
		jinjiesys_can_reward_flag_1 = 0;
		
		shenbing_use_image_id = 0;
		baojia_use_image_id = 0;
		shizhuang_wuqi = 0;
		shizhuang_body = 0;
		mojie_skill_active_flag = 0;

		new_max_cap = 0;
		add_cap_for_rank = 0;
		next_clear_add_cap_time = 0;
		had_reset_new_max_cap = 0;

		res_ch = 0;
		res_sh_2 = 0;
		add_charm_for_rank = 0;
		next_clear_add_charm_time = 0;

		miku_relive_weary_last_relive_time = 0;

		res_sh = 0;
		cross_mizang_boss_relive_tire = 0;
		cross_mizang_boss_relive_tire_reset_time = 0;
		cross_mizang_boss_can_realive_time = 0;

		res_sh_3 = 0;
		cross_youming_boss_relive_tire = 0;
		cross_youming_boss_relive_tire_reset_time = 0;
		cross_youming_boss_can_realive_time = 0;

		reserve_int = 0;
		memset(imp_guard_itemid, 0, sizeof(imp_guard_itemid));
		kill_active_boss_count = 0;
		kill_miku_boss_count = 0;

		memset(notice_day_fectch_flag_list, 0, sizeof(notice_day_fectch_flag_list));

		godmagic_today_kill_boss_num =0 ;
		today_gather_ordinary_crystal_num = 0;
		today_gather_treasure_crystal_num = 0;

		trigger_continue_kill_invalid_timestamp = 0;		// ��նʱ�䴥��ʧЧʱ��
		last_add_continue_kill_buff_timestamp = 0;			// ������նbuffʱ��

		continue_kill_count = 0;							// ������ɱ����

		last_send_continue_kill_timestamp = 0;				// �������ն��Ϣʱ��

		last_accept_task_change_appearn = 0;
		last_accept_task_change_appearn_param = 0;

		uuid= 0;
		uu_last_login_serverid = 0;
		memset(tianshu_xunzhu_active_flag_list, 0, sizeof(tianshu_xunzhu_active_flag_list));
		memset(tianshu_xunzhu_goal_fetch_flag_list, 0, sizeof(tianshu_xunzhu_goal_fetch_flag_list));
		tianshu_xunzhu_zhuanzhi_equip_fangyu = 0;
		memset(today_boss_family_kill_info, 0, sizeof(today_boss_family_kill_info));
		has_xianzunka_flag = 0;
		reserve_sh = 0;
	}

	int init_flag;								// ĳЩ�ֶγ�ʼֵ����0�����������Ƴ�ʼ��
	short nuqi;
	short energy;
	unsigned int last_give_energy;
	int shengwang;								// ����

	int lover_uid;								// ����UID
	GameName lover_name;						// ��������
	unsigned int last_marry_time;				// ���ʱ��
	unsigned int last_divorce_time;				// ���ʱ��

	int week_kill_boss_count;					// ÿ�ܻ�ɱ������boss������ÿ����0
	int red_paper_consume_gold_num;				// ��������ĵ�Ԫ������
	int red_paper_lucky_value;					// �������ָ��

	unsigned int next_fake_private_chat_timestamp; // �´�α��˽��ʱ��� 
	int	fake_private_chat_times;				// α��˽�Ĵ���

	unsigned int last_logout_timestamp;			// �ϴ�����ʱ��

	int day_hunyan_gather_bind_coin;			// ÿ���ڻ����л���ɻ�����

	int plat_spid;								// ƽ̨����id

	char has_send_welcome_mail;					// �Ѿ����ͻ�ӭ�ʼ�
	char is_fetch_worldboss_week_reward;		// �����Ƿ���ȡ����BOSS
	ItemID cross_equip_itemid;					// ���װ��ID

	int cross_honor;							// �������

	unsigned int cross_equip_gain_timestamp;	// ���װ�����ʱ��

	unsigned short m_day_revival_times;			// ÿ�ո������
	char daily_task_star_level;					// �ճ��������ǵȼ�(����)
	unsigned char recored_use_norex_num;		// ��¼��������������Ʒ����
	NorextItemUseDayTimes use_norex_day_times[ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM];	//ʹ��������������Ʒÿ�մ�������

	int yaoshouguangchang_score;				// ���޹㳡����
	int yaoshouguangchang_lv;					// ���޹㳡�ȼ�

	int suoyaota_score;							// ����������
	int suoyaota_lv;							// �������ȼ�

	int yaoshouguangchang_max_score;			// ���޹㳡������
	int suoyaota_max_score;						// ������������

	int expbottle_use_count;					// ����ƿʹ�ô���
	int expbottle_transform_count;				// ����ƿת������

	int space_renqi;							// �ռ�����
	int space_getgift_count;					// �ռ����������
	int space_day_cai_addhuoli;					// �ռ�ÿ�ղȻ�õĻ���
	int space_huoli;							// �ռ����
	SpaceMessage space_xinqing;					// �ռ�����

	int day_hunyan_add_nvwashi;					// ÿ���ڻ����л��Ů�ʯ

	unsigned int dabaoboss_validtimestamp;		// ��boss��Чʱ��� 

	unsigned int space_last_seek_gift_timestamp;// �ռ�-���һ�β鿴�������ݵ�ʱ��
	unsigned int space_last_seek_msg_timestamp;	// �ռ�-���һ�β鿴��Ϣ��ʱ��

	int zhuagui_day_gethunli;					// ÿ��ץ���û����ۼ�
	int zhuagui_day_catch_count;				// ÿ��ץ�����
	unsigned int next_extra_drop_timestamp;		// �´ζ�������ʱ���

	short paohuan_complete_count;				// �ܻ�������ɴ���
	short paohuan_getphase_reward_idx;			// �ܻ����� ��ȡ��������

	unsigned short jinghua_husong_commit_count;	// �������͵����ύ����
	short jinghua_husong_status;				// ��������״̬
	UInt32 jinghua_husong_invalid_time;			// �������͵�ʧЧʱ��

	short day_guild_red_paper_consume_gold_num;			//ÿ�����˺������Ԫ����,ÿ������
	short day_hunyan_fa_blessgift_count;				//ÿ���ڻ��緢�������,ÿ������	

	unsigned int m_next_canchangecamp_timestamp;		// ��һ�ο���ת��Ӫ��ʱ��
	short cur_use_bubble_type;							// ��ǰʹ�õ������
	short reserve_list[7];								// Ԥ��
	
	int day_gongxun;									// ��ѫֵ

	unsigned short equip_xunbao_times;					// װ��Ѱ������
	unsigned short jl_xunbao_times;						// ����Ѱ������
	unsigned short hchz_kill_count;						// �ʳǻ�սɱ����
	unsigned short gcz_kill_count;						// ����սɱ����

	int rare_chestshop_times;							// �����챦Ѱ������

	int jump_remain_times;								// ��Ծʣ�����
	unsigned int jump_last_recover_time;				// ���ָ���Ծʱ��

	short move_chess_free_times;						// ������ÿ����Ѵ���
	short move_chess_reset_times;						// ������ÿ�����ô���
	int move_chess_cur_step;							// ��ǰ���ڸ���
	unsigned int move_chess_next_free_time;				// ��һ�����������ʱ���

	unsigned int millionaire_valid_perform_skill_time;	// �󸻺�����������ȴʱ��						
	short millionaire_gather_count;						// �󸻺�����ɼ�����
	char is_turn_millionaire;							// ת���󸻺�Ϊ1 ������Ϊ0
	char is_turn;										// �Ƿ�ת��ת��

	short miku_boss_weary;								// �ؿ�bossƣ��ֵ
	unsigned short dabao_angry_value;					// �򱦷�ŭֵ

	int world_chat_count1;								// ������������
	int world_chat_count2;								// ������������
	unsigned int world_chat_record_time1;
	unsigned int world_chat_record_time2;

	int zhuagui_get_drop_time;							// ץ���ȡ���������						
	int zhuagui_score;									// ץ�����

	ItemID hunjie_item_id;								// ��Ե�����Ʒid
	char accept_task_change_appearn;					// ���������������		
	char hunyan_buy_fb_join_times;						// ��Ե�����������
	unsigned int last_buy_hunyan_fb_times_dayid;		// ���һ�ι�����縱��������dayid

	unsigned int millionaire_last_join_time;			// �󸻺��������ʱ��
	short millionaire_reward_index;						// �󸻺���������
	short hunjie_star_level;							// ��Ե����ǵȼ�

	short water_self_love_tree_time;					// ���Լ���˼����ˮ����
	short water_other_love_tree_time;					// ��������˼����ˮ����

	int zhuagui_get_couple_drop_time;					// ץ���ȡ���޵��������

	unsigned int next_recover_hp_time;					// ��һ�λָ���Ѫʱ��

	int zhuagui_kill_boss_count;						// ץ���ɱboss����

	unsigned int dabao_kick_time;						// �򱦷�ŭֵ�����߳�����ʱ��

	int mikuboss_relive_weary;							// �ؿ�boss����ƣ��ֵ
	unsigned int miku_relive_weary_last_die_time;		// �ؿ�boss�������ʱ��

	unsigned int auto_join_guild_last_time;				// ���ټ��빫���������ʱ��
	short rev_sh;							            // 
	short daily_guild_redbag_reward_fetch_flag;			// ���칫���boss�����ȡ���

	int team_equip_fb_pass_flag;						// ���װ����ͨ�ر��
	int team_equip_fb_day_count;						// ���װ����ÿ�մ���
	int team_equip_fb_day_buy_count;					// ���װ����ÿ�չ������

	int	daily_use_guild_relive_times;					// ����ʹ�ù��Ḵ�����
	int daily_use_free_song_hua_times;					// �����Ѿ�ʹ���˶��ٴ�����ͻ�

	int today_use_free_relive_num;						// ÿ����Ѹ������

	short upgrade_skill_next_skill_index;				// ��һ�ͷŽ���ϵͳװ������index
	unsigned short upgrade_skill_skill_active_flag;		// ����ϵͳ���ܼ����־
	int upgrade_skill_attack_count;						// ����ϵͳװ�����ܴ�������

	short millionaire_use_skill_times;					// �󸻺�ʹ�ñ������ܴ���
	short red_paper_be_singal_chat_count;				// ��������˽�Ĵ���

	int advance_notice_last_fectch_id;					// ����Ԥ�潱����־					
	int today_putong_hunyan_times;						// ÿ����ͨ�������

	unsigned int world_question_last_answer_time;		// ���ش�������Ŀʱ��
	int world_question_right_num;						// ������������
	unsigned int world_question_right_num_change_time;  // ���������������ı�ʱ���

	unsigned int guild_question_last_answer_time;		// ���ش𹫻���Ŀʱ��
	int guild_question_right_num;						// ������������
	unsigned int red_paper_be_singal_last_be_chat_time;	// ���������˽��ʱ��	
	int last_send_heli_notice_time;						// ���һ������ѷ���ף�ص�����ʱ��
	int is_auto_dec_offline_friend;						// �Ƿ��Զ�ɾ������x��ĺ���

	unsigned int covertshop_guild_last_notice_time;		// �һ����ṫ�����ʱ��

	int collect_drop_num;								// ÿ�ռ��ֵ������

	int jingling_advantage_gather_blue;					// ÿ�վ��������ɼ�����ɫ��������

	unsigned int last_signin_timestamp;					// �ϴι���ǩ��ʱ��
	char guild_signin_fetch_reward_flag;				// ������ǩ����ȡ��־
	char famous_man_flag;								// �Լ��Ƿ�ȡ�������óƺŽ��������ڴ浽��ɫ���ϣ���ֹ�Ϸ��󹫹����ݲ������ˣ�
	short guild_battle_fetch_daily_reward;				// ��ȡ��������ÿ�ս���

	int accept_task_change_appearn_param;				// ������������������
	int challenge_field_best_rank_break_level;			// �������������ͻ�Ƶȼ�

	int jingjie_level;									// �������ϵͳ�ȼ�
	unsigned int baby_boss_kick_time;					// ����boss��ŭֵ�����߳�����ʱ��
	short baby_boss_angry_value;						// ����boss��ŭֵ
	short baby_boss_enter_times;						// ����boss�������

	short active_boss_weary;							// ��Ծbossƣ��ֵ
	char is_pass_build_tower_fb;						// �Ƿ�ͨ�ؽ�������
	char appearacne_wuqi_use_type;						// �������ʹ������

	short imp_guard_mail_flag;                          // С��  ���������ʼ����ѣ�����12��Сʱ�ĵ�һ�����߽������ѣ�֮�����߻��ڶ������ѣ�
	short res_sh_1;                      //

	unsigned int shanggu_boss_concern_flag_list[SHANGGU_BOSS_MAX_LAYER];	// �Ϲ�BOSS����״̬

	int qingyuan_capablity;                             // ��Ես��
	int baby_capablity;                                 // ����ս��
	int little_pet_capablity;                           // С����ս��

	unsigned int dabao_next_add_angry_timestamp;        // �򱦳���ÿ��n������m���ŭֵʱ���

	char appearacne_body_use_type;						// �·����ʹ������
	char avatar_changed;								// �Ƿ���ȡ�����˻�ͷ����
	short cross_boss_relive_tire;						// ���boss����ƣ��

	unsigned int cross_boss_relive_tire_reset_time;		// ����ƣ������ʱ��
	unsigned int cross_boss_can_realive_time;			// ����ƣ�ͽ���ʱ
	
	short cur_use_avatar_type;							// ��ǰʹ�õ�ͷ���
	char imp_guard_type[EQUIP_IMPGUARD_MAX_NUM];        // ����С������
	unsigned int imp_guard_end_timestamp[EQUIP_IMPGUARD_MAX_NUM]; // ����С�������ʱ��

	unsigned int jinjiesys_reward_flag;					// ���׽�����ȡ(����)���
	unsigned int jinjiesys_can_reward_flag;				// ���׽����ɷ���ȡ���

	unsigned int jinjiesys_reward_flag_1;				// ���׽�����ȡ(����)���
	unsigned int jinjiesys_can_reward_flag_1;			// ���׽����ɷ���ȡ���

	char shenbing_use_image_id;							// ���ʹ��id
	char baojia_use_image_id;							// ����ʹ��id
	char shizhuang_wuqi;								// ʱװ����
	char shizhuang_body;								// ʱװ����
	
	unsigned long long mojie_skill_active_flag;				// ħ�似�ܼ����־

	unsigned int trigger_continue_kill_invalid_timestamp;	// ��նʱ�䴥��ʧЧʱ��
	unsigned int last_add_continue_kill_buff_timestamp;	// ������նbuffʱ��
	int continue_kill_count;							// ������ɱ����
	unsigned int last_send_continue_kill_timestamp;		// �������ն��Ϣʱ��

	int new_max_cap;									// ��ʷ���ս��(��������ս�����а�)
	int add_cap_for_rank;								// ����ս��
	unsigned int next_clear_add_cap_time;				// �´��������ս����ʱ��
	char had_reset_new_max_cap;                         // ����
	char res_ch;
	short res_sh_2;
	int add_charm_for_rank;								// ��������
	unsigned int next_clear_add_charm_time;				// �´��������������ʱ��

	unsigned int miku_relive_weary_last_relive_time;    // �ؿ�boss����ƣ��ֵ�����ʱ��

	short res_sh;
	short cross_mizang_boss_relive_tire;						// ���boss����ƣ��
	unsigned int cross_mizang_boss_relive_tire_reset_time;		// ����ƣ������ʱ��
	unsigned int cross_mizang_boss_can_realive_time;			// ����ƣ�ͽ���ʱ

	short res_sh_3;
	short cross_youming_boss_relive_tire;						// ���boss����ƣ��
	unsigned int cross_youming_boss_relive_tire_reset_time;		// ����ƣ������ʱ��
	unsigned int cross_youming_boss_can_realive_time;			// ����ƣ�ͽ���ʱ

	int reserve_int;											// ����
	unsigned short imp_guard_itemid[EQUIP_IMPGUARD_MAX_NUM];    // ����С��id
	unsigned short kill_active_boss_count;						// �ѻ�ɱ��Ծboss����
	unsigned short kill_miku_boss_count;						// �ѻ�ɱ�ؿ�bossboss����
	unsigned char notice_day_fectch_flag_list[NOTICE_DAY_MAX_NUM];	// ����Ԥ�潱����־

	int godmagic_today_kill_boss_num;											// ��ħboss���ջ�ɱboss����
	int today_gather_ordinary_crystal_num;								// ��ħboss�ɼ���ͨˮ������
	int today_gather_treasure_crystal_num;								// ��ħboss�ɼ���ϧˮ������

	int last_accept_task_change_appearn;						// �ϴν�������������������н�����ʱ����֮ǰ�������״̬��
	int last_accept_task_change_appearn_param;					// �ϴν�����������������

	long long uuid;												// ͬ��RoleCross�е�UUID
	long long uu_last_login_serverid;							// ���һ�ε�½�ı��ط�ID,�����жϵ�ǰ�ϵ��ĸ���������
	
	UInt32 tianshu_xunzhu_active_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];		// ����Ѱ����������
	UInt32 tianshu_xunzhu_goal_fetch_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];	// ����Ѱ����Ŀ����ȡ����б�
	long long tianshu_xunzhu_zhuanzhi_equip_fangyu;								// ����Ѱ����תְװ����
	struct TodayBossFamilyKillInfo
	{
		TodayBossFamilyKillInfo() : monster_id(0), kill_count(0) {}

		unsigned short monster_id;
		unsigned short kill_count;
	};
	TodayBossFamilyKillInfo today_boss_family_kill_info[BOSS_FAMILY_INFO_MAX_COUNT];
	short has_xianzunka_flag;									// ӵ�����𿨵ı�ǣ���ʱ�����ö��㣩
	short reserve_sh;
};

#pragma pack(pop)

typedef char CommonDataParamHex[sizeof(CommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CommonDataParamHex) < 4096);

#endif  // __COMMON_DATA_HPP__
