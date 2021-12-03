#ifndef __OTHER_MODULE_DATA_HPP__
#define __OTHER_MODULE_DATA_HPP__

#include "gameworld/gameworld/other/turntable/turntableconfig.h"
#include "gameworld/gameworld/other/upgraderush/upgraderushconfig.hpp"
#include "gameworld/gameworld/config/touzijihuaconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

struct OtherModuleDataParam								// һЩСģ�����������
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

		zhuanzhi_tianming = 0;						// תְ�����ȼ�
		zhuanzhi_canglong = 0;						// תְ�����ȼ�
		zhuanzhi_shengxiao = 0;					// תְ��Ф�ȼ�
		zhuanzhi_yuanqi = 0;						// תְԪ���ȼ�
		zhuanzhi_shenqi = 0;						// תְ�����ȼ�
		zhuanzhi_xingxiu = 0;                      // תְ���޵ȼ�
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

		memset(upgrade_return_end_time_list, 0, sizeof(upgrade_return_end_time_list));	// ���׽�������ʱ���б�
		memset(foundation_next_reward_index, 0, sizeof(foundation_next_reward_index));	// �ɳ�������ȡ���
	}

	unsigned int monthcard_active_timestamp;			// �¿�����ʱ��
	int monthcard_last_day;								// �¿�����ʱ��
	int monthcard_next_reward_day_idx;					// �¿��´ν�����
	int monthcard_buy_times;							// �¿��������

	int phase_fb_pass_flag;								// �׶θ���ͨ�ر��(����)

	short turntable_lucky_value[TURNTABLE_TYPE_MAX_COUNT];// ת������ֵ

	char wabao_complete_times;							// �ڱ���ɴ���
	char wabao_buy_join_times;							// �����ڱ�����
	char jilian_once_join;								// �Ƿ������������
	char roll_money_already_roll_gold_num;				// �Ѿ�ҡ��Ԫ������
	int wabao_scene_id;									// �ڱ�����id
	int wabao_pos_x;									// �ڱ�����x
	int wabao_pos_y;									// �ڱ�����y

	short team_fb_open_times;							// ��Ӹ�����������
	short team_fb_assist_times;							// ��Ӹ���Э������

	unsigned int last_reset_role_module_data_dayi;		// ������ý�ɫģ������dayid
	unsigned int last_reset_role_module_data_timestamp;	// ���һ�����ý�ɫģ�����ݵ�ʱ��

	char jilian_type;									// ��������
	char jilian_is_working;								// �Ƿ����ڼ���
	short jilian_progress;								// ��������

	short reseve_short;
	char reserve_char;
	char jilian_qte_succ_times;							// ����QTE�ɹ�����

	short zhanshendian_pass_level;						// ս������ͨ��
	char touzijihua_active_plan_0;						// Ͷ�ʼƻ�����ƻ�0
	char touzijihua_active_plan_1;						// Ͷ�ʼƻ�����ƻ�1

	short touzijihua_plan_0_reward_flag;				// Ͷ�ʼƻ�0�������
	short touzijihua_plan_1_reward_flag;				// Ͷ�ʼƻ�1�������

	short roll_money_gold_fetch_reward_times;			// ҡǮ��ȡԪ����������
	short roll_money_coin_fetch_reward_times;			// ҡǮ��ȡͭ�ҽ�������

	char roll_money_gold_num_0;							// ҡǮԪ������0
	char roll_money_gold_num_1;							// ҡǮԪ������1
	char roll_money_gold_num_2;							// ҡǮԪ������2
	char roll_money_gold_roll_times;					// ҡǮrollԪ�����ִ���

	char roll_money_coin_num_0;							// ҡǮͭ������0
	char roll_money_coin_num_1;							// ҡǮͭ������1
	char roll_money_coin_num_2;							// ҡǮͭ������2
	char roll_money_coin_num_3;							// ҡǮͭ������3

	char roll_money_coin_num_4;							// ҡǮͭ������4
	char roll_money_coin_num_5;							// ҡǮͭ������5
	char roll_money_coin_num_6;							// ҡǮͭ������6
	char roll_money_coin_roll_times;					// ҡǮrollͭ�����ִ���

	short zhanshendian_yesterday_pass_level;			// ����ս������ͨ��
	char touzhijihua_active_highest_plan;
	char reserve_sh;									// ����
	unsigned int m_zhanshendian_fetch_day_reward_timestamp;	// ս���ÿ�ս�����ȡʱ���

	int	world_event_fetch_reward_flag;					// �����¼� �콱���
	short world_event_find_npc_flag;					// �����¼� Ѱ��npc���
	char world_event_curr_find_npc_id;					// �����¼�	��ǰѰ��npc id
	char world_event_light_word_flag;					// �����¼� �����ֱ��
	int world_event_find_npc_score;						// �����¼�	Ѱ��npc����
	int world_event_kill_rand_boss_score;				// �����¼� ���boss����
	int world_event_shenmozhixi_score;					// �����¼� ��ħ֮϶����
	int world_event_xukongzhimen_score;					// �����¼� ���֮�Ż���
	int world_event_quanmintafang_score;				// �����¼� ȫ����������
	int world_event_jiqingpk_score;						// �����¼� ����pk����
	int world_event_tiannvsanhua_score;					// �����¼� ��Ůɢ������

	short patafb_pass_level;							// ����ͨ�ز���
	short patafb_today_level;							// ��������ͨ�ز���
	int week_add_charm;									// ÿ����������

	unsigned int new_touzijihua_buy_time;				// ��Ͷ�ʼƻ�����ʱ��
	int new_touzijihua_reward_flag;						// ��Ͷ�ʼƻ��������
	int new_touzijihua_vip_reward_flag;					// ��Ͷ�ʼƻ�vip�������
	int new_touzijihua_gold_bind_flag;					// ��Ͷ�ʼƻ����꽱�����

	char touzijihua_active_plan_2;						// Ͷ�ʼƻ�����ƻ�2
	char new_touzijihua_first_reward_flag;				// ��Ͷ�ʼƻ�������־
	short touzijihua_plan_2_reward_flag;				// Ͷ�ʼƻ�2�������

	int wabao_baotu_count;								// ��ͼ����
	int wabao_reward_type;								// �ڱ�������������
	unsigned int baotu_fetch_flag;						// ��ͼ��ȡ��־
	unsigned int shouhuzhe_time;						// ��ɱ�ػ���bossʱ��

	int day_add_charm;									// ÿ����������

	unsigned int upgrade_return_end_time_list[UPGRADE_RUSH_MAX_TYPE_COUNT]; // ���׽�������ʱ���б�

	char zhuanzhi_tianming;						// תְ�����ȼ�
	char zhuanzhi_canglong;						// תְ�����ȼ�
	char zhuanzhi_shengxiao;					// תְ��Ф�ȼ�
	char zhuanzhi_yuanqi;						// תְԪ���ȼ�
	char zhuanzhi_shenqi;						// תְ�����ȼ�
	char zhuanzhi_xingxiu;                      // תְ���޵ȼ�
	short touzijihua_kill_boss_num;				//Ͷ�ʼƻ���ɱ��boss��
	
	short touzijihua_plan_3_reward_flag;				// Ͷ�ʼƻ�3�������
	char touzijihua_active_plan_3;						// Ͷ�ʼƻ�����ƻ�3
	char res_ch;

	int molong_curr_loop;								// ħ����ǰ��
	int molong_accumulate_consume_gold;					// ħ����ǰ�ۼ�����
	int molong_today_consume_gold;						// ħ����������
	short molong_total_move_step;						// ħ���ܹ��ƶ�����
	short molong_today_move_step;						// ħ�������ƶ�����
	int molong_rank_grade;								// ħ��ͷ�εȼ�
	int molong_rank_cumulate_gold;						// ħ��ͷ���ۼ�����
	short daily_task_can_fetch_flag;					// �ճ��������ȡ���
	short daily_task_fetch_flag;						// �ճ���������ȡ���
	unsigned int daily_task_exp_ball_valid_time;		// �ճ���������ʧЧʱ��
	long long daily_task_exp_ball_exp;					// �ճ�����������

	char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];       // �ɳ�������ȡ���

	int	knapsack_grid_auto_add_count;					// ��������������

	short touzijihua_active_fb_plan;					//Ͷ�ʼƻ���������
	short touzijihua_fb_plan_reward_flag;				//Ͷ�ʼƻ�������ȡ

	short touzijihua_active_boss_plan;					//Ͷ�ʼƻ�boss����
	short touzijihua_boss_plan_reward_flag;				//Ͷ�ʼƻ�boss��ȡ

	short touzijihua_kill_shenyun_boss_num;						//Ͷ�ʼƻ���ɱ������boss��
	short touzijihua_active_shenyun_boss_plan;					//Ͷ�ʼƻ�����boss����
	short touzijihua_shenyun_boss_plan_reward_flag;				//Ͷ�ʼƻ�����boss��ȡ
	short reserve_sh1;
				
};

#pragma pack(pop)

typedef char OtherModuleDataParamHex[sizeof(OtherModuleDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(OtherModuleDataParamHex) < 1024);

#endif  // __OTHER_MODULE_DATA_HPP__


