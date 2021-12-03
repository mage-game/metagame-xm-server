#ifndef __OTHERGLOBALCONFIG_H__
#define __OTHERGLOBALCONFIG_H__

#include <string>
#include <vector>

#include "gamedef.h"

#include "servercommon/servercommon.h"
#include "servercommon/frienddef.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/guilddef.hpp"

#include "other/chestshop/chestshop.hpp"

#include "obj/character/attribute.hpp"

#include "item/itemextern.hpp"
#include "servercommon/campdef.hpp"

namespace globalconfig
{
	/*
	ȫ������
	itemdrop_interval_owner:�Լ�������Ʒ��ʰȡʱ�䣬��
	itemdrop_interval_delete:������Ʒ��ʧ��ʱ�䣬��
	itemdrop_length:������Ʒʰȡ����

	monster_dead_delay�����������ӳ�ɾ��ʱ��,����
	monster_follow_interval: ���������������

	bag_sortinorder_interval����������Сʱ�������ƣ���

	fight_status_leave_interval:����ս��״̬��ʱ�䣬��

	check_move_valid:���ڼ��ͻ��˷����ƶ�Э���Ƿ�Ϸ�
	is_check_valid:�Ƿ���з��Ҳ���
	max_move_speed:��Ϸ�п��ܵ�����ƶ��ٶ�
	move_check_length:�ƶ�������Ƿ����룬��λ������

	max_transport_length�����͵���������Զ���룬��λ������

	max_gather_distance����ɫ��ɼ��ص������룬��λ����
	max_npc_touch_distance����ɫ��npc������룬��λ����
	max_friend_num: ��������

	server_max_role_num ���������ɵ���������

	friend_bless_reward_max_count ����ף����������
	friend_bless_reward_tili ����ף����������
	first_change_avatar_reward_gold ��һ�θı�ͷ��Ԫ������
	*/
	class OtherGlobalConfig
	{
	public:
		OtherGlobalConfig();
		~OtherGlobalConfig() {}

		bool Init(const std::string& path, std::string* err);

		unsigned int itemdrop_interval_owner;
		unsigned int itemdrop_interval_delete;
		unsigned int itemdrop_interval_delete_no_static_scene;
		unsigned int itemdrop_length;

		unsigned int monster_dead_delay;
		unsigned int monster_follow_interval;
		unsigned int bag_sortinorder_interval;
		unsigned int fight_status_leave_interval_s;
		bool is_check_valid;
		unsigned int max_move_speed;
		unsigned int move_check_length;
		unsigned int max_transport_length;

		unsigned int max_gather_distance;
		unsigned int max_npc_touch_distance;

		int gm_switch;			

		int random_camp_itemid;
		int random_camp_gold_bind;
		int ignore_level_limit_chongzhi;		// ��ֵXԪ������������ȼ�

		const static int KNAPSACK_EXTEND_COST_CFG_MAX_COUNT = 32;
		struct KnapsackExtendCostCfgItem
		{
			KnapsackExtendCostCfgItem() : min_extend_index(0), need_item_count(0),
			min_extend_index_storage(0), need_item_count_storage(0){}

			int min_extend_index;
			int need_item_count;
			int min_extend_index_storage;
			int need_item_count_storage;
		};

		int knapsack_extend_cost_cfgcount;
		KnapsackExtendCostCfgItem knapsack_extend_cost_cfglist[KNAPSACK_EXTEND_COST_CFG_MAX_COUNT];

		std::vector<unsigned int> knapsack_grid_auto_add_time_vec;		// �Զ���չ��������

		ItemID knapsack_extend_bag_item_id;
		ItemID knapsack_extend_storage_item_id;

		int speaker_level;							// ���ȿ����ȼ�
		int speaker_item_id;						// ��������������Ʒid
		int speaker_need_gold;						// ����������ҪԪ��
		int cross_speaker_item_id;					// �������������Ʒid
		int cross_speaker_need_gold;				// ���������ҪԪ��

		int clear_mentality_cd_interval_per_gold;
		int gengu_baohufu_item_id;

		int rest_limit_level;
		int rest_recovery_interval;
		int rest_recovery_hp_per;
		int rest_reward_interval;

		int shuangxiu_add_per;
		int shuangxiu_intimacy_interval;
		int shuangxiu_intimacy_add;

		int namecolor_reduce_exp_percent[NAME_COLOR_MAX];
		int namecolor_reduce_hp_per_percent[NAME_COLOR_MAX];
		int namecolor_reduce_gongji_per_percent[NAME_COLOR_MAX];
		int namecolor_evil_list[NAME_COLOR_MAX];
		int namecolor_max_evil;
		int namecolor_update_interval_s;
		int namecolor_update_dec_evil;

		ItemID realive_item_id;
		int realive_recover_common_scene_percent;
		int realive_recover_fb_percent;

		TaskID role_birth_task_id;

		int team_today_intimacy_limit;
		int team_intimacy_reward;
		int intimacy_reward_monster_level;

		struct FriendNumForLevel
		{
			FriendNumForLevel() : level(0), number(0) {}

			int level;
			int number;
		};
		FriendNumForLevel friendnum_for_level[FRIENDNUM_FOR_LEVEL_COUNT];

		int honour_init_value;
		int kill_other_camp_get_honour_per;
		int kill_other_camp_max_level_diff;
		int double_honour_kill_count;
		
		int sceneid_of_camp1;
		int sceneid_of_camp2;
		int sceneid_of_camp3;

		static const int MAX_CTRL_EFFECT_REDUCE_COUNT = 10;			// ����Ч��˥����������

		int ctrl_effect_reduce_circle_time;							// ����Ч��˥������
		int ctrl_effect_reduce_percent_list[MAX_CTRL_EFFECT_REDUCE_COUNT + 1];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		static const int MAX_FRIEND_ATTR_ITEM_NUM = 15;				// ����buff��������
		static const int MAX_FRIEND_ATTR_TYPE_NUM = 3;				// ����buff�ı���������
		UNSTD_STATIC_CHECK(MAX_FRIEND_ATTR_TYPE_NUM <= 3);			// buff�������3������

		struct FriendAttrItem
		{
			FriendAttrItem() : intimacy(0), count(0) {}

			int intimacy;
			int count;
			CharIntAttrs::AttrPair attr_list[MAX_FRIEND_ATTR_TYPE_NUM];
		};

		int friend_attr_count;
		FriendAttrItem friend_attr_list[MAX_FRIEND_ATTR_ITEM_NUM];

		///////////////////////////////////////////////////////ɱ�־������//////////////////////////////////////////////////////////
		static const int MAX_LEVEL_FACTOR_COUNT = 4;

		struct MonsterDeltaLevel
		{
			MonsterDeltaLevel() : delta_level(0), factor(0) {}

			int delta_level;
			double factor;
		};

		struct LevelFactor
		{
			LevelFactor() : role_level(0) {}

			int role_level;
			std::vector<MonsterDeltaLevel> monster_delta_level_vec;
		};

		std::vector<LevelFactor> m_low_level_factor_vec;
		std::vector<LevelFactor> m_high_level_factor_vec;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int max_nuqi;													// ŭ������
		int nuqi_atk_other;												// 
		int nuqi_atked;													// 
		int max_energy;													// ��������
		int give_energy_interval_s;
		int give_energy_per_time;
		int buy_max_energy;												// �ɹ������������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int world_level_open_level;										// ����ȼ��ӳɿ��ŵȼ�
		double world_level_exp_add_percent_perlevel;						// ÿ��һ���ӳɰٷֱ�
		int world_level_max_exp_add_percent;							// ����ȼ��������ӳɱ���

		int welfare_daily_find_days;									// �ճ���������һ�����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int jump_limit_level;
		int jump_limit_times;
		int jump_recover_time;
		int reset_jump_pos_min_level;

		//////////////////////////////////////////////////////////////////////////
		int shenjiang_list[CAMP_TYPE_MAX];

		//////////////////////////////////////////////////////////////////////////
		int reset_name_item_id;

		//////////////////////////////////////////////////////////////////////////
		int speedup_hello_count_max;			// ���ټ����Ϣ�������ֵ
		int speedup_hello_timeout_s;			// ���ټ����Ϣ�����
		int speedup_hello_count_forbit_time_s;	// ���ټ�� ��Ϣ�������������ʱ��
		int speedup_hello_timeout_forbit_time_s;// ���ټ�� ��Ϣ���ͼ����ʱ����ʱ��
		int speedup_hello_all_pack_percent_limit;// ���ټ�� ��Ϣ����������̰ٷֱ�
		bool speedup_hello_open_check;			// �Ƿ������ټ�⣨��բ��

		//////////////////////////////////////////////////////////////////////////
		int server_max_role_num;				// ���������ɵ��������� 

		//////////////////////////////////////////////////////////////////////////
		int friend_bless_reward_max_count;		// ����ף����������
		int friend_bless_reward_tili;			// ����ף����������
		int friend_bless_max_count;				// ף�����ѵ�������

		int first_change_avatar_reward_gold;	// ��һ�θı�ͷ����

		//////////////////////////////////////////////////////////////////////////
		int world_talk_is_limit;				// ���������Ƿ���
		int world_talk_effect_msg_min_len;		// ������������Ϣ��С����
		int world_talk_torelate_max_count;		// �����������������ͬ��Ϣ��
		int world_talk_accumulate_count_reset_interval; // ���������ۼ�������ʱ����

		///////////////////////////////////////////////////////////////////////////��������
		int fb_guwu_cost_gold;
		int fb_guwu_cost_coin;					
		int fb_guwu_gongji_per;
		int fb_guwu_time;

		///////////////////////////////////////////////////////////////////////////���׷���
		int trade_logintime_limit;
		unsigned int publicsale_solt_out_time;
		int publicsale_max_price;
		int is_allow_gm_public_sale_buy;						// �Ƿ����������г�����

		static const int MAX_PUBLIC_SALE_TAX_NUM = 10;
		struct PublicsaleTaxCfg
		{	
			PublicsaleTaxCfg() : gold(0), tax(0) {}

			int gold;
			int tax;
		};

		PublicsaleTaxCfg publicsale_once_tax_list[MAX_PUBLIC_SALE_TAX_NUM];

		private:
			int InitCommonCfg(TiXmlElement *RootElement);
			int InitSpeedUpCfg(TiXmlElement *RootElement);
			int InitTalkCfg(TiXmlElement *RootElement);
			int InitBagCfg(TiXmlElement *RootElement);
			int InitKnapsackGridAutoAddCfg(TiXmlElement *RootElement);
			int InitRedNameCfg(TiXmlElement *RootElement);
			int InitFriendCountCfg(TiXmlElement *RootElement);
			int InitBuffDecCfg(TiXmlElement *RootElement);
			int InitFriendBuffCfg(TiXmlElement *RootElement);
			int InitPublicSaleTaxCfg(TiXmlElement *RootElement);
	};
};

#endif

