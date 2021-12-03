#ifndef MSG_FB_H
#define MSG_FB_H

#include "servercommon/fbdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/crossdef.hpp"
#include "other/fb/towerdefendconfig.hpp"
#include "other/fb/buildtowerfbconfig.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAutoFBRewardDetail
	{
	public:
		SCAutoFBRewardDetail();
		MessageHeader header;

		const static int REWARD_ITEM_MAX_COUNT = 32;

		struct RewardItem
		{
			ItemID item_id;
			short num;
			char is_bind;
			char reserve_ch;
			short reserve_sh;
		};

		short fb_type;
		short reserve_sh;

		int reward_coin;
		long long reward_exp;
		int reward_xianhun;
		int reward_yuanli;		
		int item_count;
		RewardItem item_list[REWARD_ITEM_MAX_COUNT];
	};

	///////////////////////////////////////////////// װ������ /////////////////////////////////////////////////
	class CSEquipFBGetInfo                // 5457 ����װ��������Ϣ
	{
	public:
		CSEquipFBGetInfo();
		MessageHeader header;
	};

	class SCEquipFBInfo		              // 5414 ������Ϣ
	{
	public:
		SCEquipFBInfo();
		MessageHeader header;

		short max_layer_today_entered;											// ��������������
		short flag;																// ��λ��EFB_FLAG
		char mysterylayer_list[FB_EQUIP_MAX_GOODS_SEQ];							// ���ز���Ʒ�����¼
	};

	class SCEquipFBResult                 // 5409 ͨ�ؽ��
	{
	public:
		SCEquipFBResult();
		MessageHeader header;

		struct FallItemInfo
		{
			ItemID item_id;
			short item_num;
			short item_color;
			short res_sh;
		};

		char is_finish;                  // ��ǰ�����
		char is_passed;					 // 1��ͨ�أ�0��δͨ�� ����ǰ��ͨ�أ�
		char can_jump;					 // 1�������㣬0����������
		char is_all_over;				 // 1����� 0��δ��� (ȫ��ͨ��)
		int  use_time;					 //  ������ʼ����ǰ����ʱ
		short have_pass_reward;			 // 0 �ޣ����˴� 1 �У�����Ʊ
		char is_leave;                   // �Ƿ��뿪����
		char item_count;
		//FallItemInfo fall_item_list[NEQFB_PICK_FALL_ITEM_RECORD_COUNT];
		FallItemInfo fall_item_list[NEQFB_MAX_DROP_COUNT];
	};

	class SCEquipFBTotalPassExp           // 5412 ����
	{
	public:
		SCEquipFBTotalPassExp();
		MessageHeader header;

		long long total_pass_exp;				 // �ۻ�ͨ�ؾ���
	};

	/////////////////////////////////////////////////// �������� /////////////////////////////////////////////////////
	class SCTowerDefendRoleInfo						// 5404
	{
	public:
		SCTowerDefendRoleInfo();
		MessageHeader header;

		char personal_join_times;
		char personal_buy_join_times;
		char personal_max_pass_level;
		char personal_auto_fb_free_times;
		short personal_item_buy_join_times;
		short personal_last_level_star;
	};

	class CSTowerDefendRoleInfo
	{
	public:
		CSTowerDefendRoleInfo();
		MessageHeader header;
	};

	class CSTowerDefendBuyJoinTimes			// 5454 �������
	{
	public:
		CSTowerDefendBuyJoinTimes();
		MessageHeader header;
	};

	class SCTowerDefendInfo					// 5413
	{
	public:
		SCTowerDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_FINISHED = 3,
		};

		short reason;
		short reserve;

		unsigned int time_out_stamp;
		short is_finish;
		short is_pass;
		int pass_time_s;

		long long life_tower_left_hp;					// ������HP
		long long life_tower_left_maxhp;				// ���������HP
		short curr_wave;								// ��ǰ��
		short curr_level;								// ��ǰ�ǵڼ���
		int next_wave_refresh_time;						// ��һ������ʱ��
		short clear_wave_count;							// ������ﲨ��
		short death_count;								// ��������

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		SCTowerDefendInfo::DropItem drop_items[FB_RECORD_DROP_ITEM_MAX_NUM];
	};

	class SCFBDropInfo							// 5420�����������ͳ��
	{
	public:
		SCFBDropInfo();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCTowerDefendResult	// 5433
	{
	public:
		SCTowerDefendResult();
		MessageHeader header;

		char is_passed;
		char clear_wave_count;
		short use_time;
		short have_pass_reward; // 0 �ޣ����˴�1 �У�����Ʊ
		short reserve_sh;
	};

	class SCTowerDefendWarning						// 5431
	{
	public:
		SCTowerDefendWarning();
		MessageHeader header;

		enum WARNING_TYPE
		{
			WARNING_TYPE_NORMAL = 1,
			WARNING_TYPE_LOW_PERCENT,
		};

		short warning_type;								// 1����ͨ���򣨺��԰ٷֱȣ���2��HP���ڰٷֱ�
		short percent;
	};

	class CSTowerDefendNextWave
	{
	public:
		CSTowerDefendNextWave();
		MessageHeader header;
	};

	//////////////////////////////////////////////////// ͨ�ø���ͨ��Э�� /////////////////////////////////////////////////////
	class SCFBFinish					// 5410����ͨ��ͨ��֪ͨ
	{
	public:
		SCFBFinish();
		MessageHeader header;
	};

	class SCFBBossHPPercentInfo
	{
	public:
		SCFBBossHPPercentInfo();
		MessageHeader header;

		unsigned short monster_id;
		short hp_percent;
	};

	/////////////////////////////////////////////////// ��װ���� /////////////////////////////////////////////////////
	class SCNeqFBInfo									// 8501��װ������Ϣ
	{
	public:
		SCNeqFBInfo();
		MessageHeader header;
		enum INFO_TYPE
		{
			NEQ_FB_INFO_DEFAULT= 0,  //ȫ��ˢ��
			NEQ_FB_INFO_VIP_BUY_TIME = 1, //vip�������
			NEQ_FB_INFO_ITEM_BUY_TIME =2, //��ͨ�������
			NEQ_FB_INFO_REWARD = 3,// ��ȡ����

		};
		struct OneLevel
		{
			char join_times;
			char max_star;								// ���ؿ��������
			short reserve_sh;
		};

		struct OneChapter
		{
			char reward_flag;							// �������  flag & reward_seq != 0 ��ʾ����˽���
			char reserve_ch;
			short reserve_sh;
			OneLevel level_list[NEQFB_MAX_LEVEL_PER_CHAPTER];
		};

		short info_type;
		short reverse_sh;
		int neqfb_score;								// ����
		short toal_buy_times;							// �����ܴ���
		short today_fight_all_times;					// ����ս������
		short today_vip_buy_times;						// vip�������
		short today_item_buy_times;						// ��Ʒ�������
		OneChapter chapter_list[NEQFB_MAX_CHAPTER];		// ���б�
	};

	class SCNeqPass										// 8502��װ����ͨ����Ϣ
	{
	public:
		SCNeqPass();
		MessageHeader header;

		short pass_result;								// 1��ͨ�أ�0��ʧ��
		short reserve;
		int pass_sec;									// ͨ��ʱ����
		short pass_star;								// ͨ������
		short reward_score;								// ��������
	};

	class SCNeqRollPool		// 8503
	{
	public:
		SCNeqRollPool();
		MessageHeader header;

		struct RollItem
		{
			unsigned short item_id;
			char is_bind;
			char num;
		};
		RollItem roll_item_list[NEQFB_ROLLPOOL_TOTAL_COUNT];
	};

	class SCNeqRollInfo		//8504
	{
	public:
		SCNeqRollInfo();
		MessageHeader header;

		enum RIREASON
		{
			RIREASON_START = 0,							// ��ʼ����
			RIREASON_ROLL = 1,							// ���ƽ��
			RIREASON_END = 2,							// ���ƽ���
		};
		short reason;
		short reserve;

		char max_free_roll_times;
		char free_roll_times;
		char gold_roll_times;
		char hit_seq;									// ������Ʒ��ţ��������±�
	};

	class CSNeqFBStarRewardReq							// 8511��װ���������콱
	{
	public:
		CSNeqFBStarRewardReq();
		MessageHeader header;

		short chapter;									// ��
		short seq;										// �������
	};

	class CSNeqFBExchangeReq							// 8512 ��װ��������һ�
	{
	public:
		CSNeqFBExchangeReq();
		MessageHeader header;

		int item_id;
	};

	class CSNeqFBBuyTimesReq							// 8513 ��װ�������������
	{
	public:
		CSNeqFBBuyTimesReq();
		MessageHeader header;
	};

	class CSNeqFBAutoReq								// 8514 ��װ��������ɨ��
	{
	public:
		CSNeqFBAutoReq();
		MessageHeader header;

		short chapter;									// ��
		short level;									// �ؿ�
	};

	class CSNeqInfoReq									// 8515 ��ѯ��װ�����ؿ���Ϣ
	{
	public:
		CSNeqInfoReq();
		MessageHeader header;
	};

	class CSNeqRollReq		// 8516
	{
	public:
		CSNeqRollReq();
		MessageHeader header;

		short end_roll;
		short reserve_sh;
	};

	/////////////////////////////////////////////////// ���߲��ϸ��� /////////////////////////////////////////////////////
	enum CS_ARMOR_DEFEND_REQTYPE
	{
		CS_ARMOR_DEFEND_ROLE_INFO_REQ = 0,
		CS_ARMOR_DEFEND_BUY_JOIN_TIMES = 1,
		CS_ARMOE_DEFEND_NEXT_WAVE_REQ = 2,
		CS_ARMOR_DEFEND_AUTO_REFRESH = 3,//0 ���Զ�ˢ��1 �����Զ�ˢ��
	};

	class CSArmorDefendRoleReq     //8535
	{
	public:
		CSArmorDefendRoleReq();
		MessageHeader header;

		short req_type;
		short parm1;
	};

	class SCArmorDefendRoleInfo		// 8536
	{
	public:
		SCArmorDefendRoleInfo();
		MessageHeader header;

		char personal_join_times;
		char personal_buy_join_times;
		char personal_max_pass_level;
		char personal_auto_fb_free_times;
		short personal_item_buy_join_times;
		short reserve_sh;
	};

	class SCArmorDefendResult	// 8537
	{
	public:
		SCArmorDefendResult();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;
		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};
		char is_passed;
		char clear_wave_count;
		short use_time;  //��������ʱ

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCArmorDefendInfo  // 8538
	{
	public:
		SCArmorDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_FINISHED,
		};

		short reason;
		short reserve_ch;
		int escape_monster_count;						// ������
		short curr_wave;								// ��ǰ��
		short energy;									// ����
		int next_wave_refresh_time;						// ��һ������ʱ��
		short clear_wave_count;							// ������ﲨ��
		short refresh_when_clear;						// ���������ˢ��
	};

	class SCArmorDefendWarning  //8539
	{
	public:
		SCArmorDefendWarning();
		MessageHeader header;
							
		short escape_num;
		short reserve_sh;
	};

	class SCArmorDefendPerformSkill//8540
	{
	public:
		SCArmorDefendPerformSkill();
		MessageHeader header;
		int skill_index;
		unsigned int next_perform_time[ARMORDEFEND_SKILL_MAX_COUNT];
	};

	//////////////////////////////////////////////////// ������Э�� /////////////////////////////////////////////////////

	enum BUILD_TOWER_OPERA_TYPE     
	{
		BUILD_TOWER_OPERA_TYPE_BUILD = 0,       // ������
		BUILD_TOWER_OPERA_TYPE_UPGRADE = 1,		// ������
		BUILD_TOWER_OPERA_TYPE_REMOVE = 2,		// �Ƴ���
		BUILD_TOWER_OPERA_TYPE_FLUSH = 3,		// ����ˢ��
		BUILD_TOWER_OPERA_TYPE_CALL = 4,		// �ٻ������
	};

	class CSBuildTowerReq		 //8570
	{
	public:
		CSBuildTowerReq();
		MessageHeader header;

		int operate_type;
		short param1;
		short param2;
	};

	class SCBuildTowerFBSceneLogicInfo  //8571
	{
	public:
		SCBuildTowerFBSceneLogicInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_MONSTER_WAVE = 1,
			NOTIFY_BOSS_FLUSH = 2,
			NOTIFY_EXTRA_BOSS = 3,
			NOTIFY_FB_END = 4,
			NOTIFY_WAVE_FLUSH_END = 5, //��ˢ��
			NOTIFY_PREPARE_TIME = 10,//׼������
		};

		void reset()
		{
			notify_reason = NOTIFY_REASON_DEFAULT;
			is_finish = 0;
			is_pass = 0;
			can_call_extra_monster = 0;

			time_out_timestamp = 0;
			exp = 0;
			douhun = 0;
			cur_wave = 0;
			escape_monster_count = 0;
			next_wave_timestamp = 0;
			item_count = 0;
			special_monster_num = 0;
			remain_buyable_monster_num = 0;
			memset(tower_info_list, 0, sizeof(tower_info_list));
			memset(item_list, 0, sizeof(item_list));
		}

		struct DropItem
		{
			ItemID item_id;
			short num;
		};

		struct TowerInfo
		{
			short tower_type;
			short tower_level;
		};

		char notify_reason;
		char is_finish;
		char is_pass;
		char can_call_extra_monster;

		unsigned int time_out_timestamp;
		long long exp;
		int douhun;
		short cur_wave;
		short escape_monster_count;
		unsigned int next_wave_timestamp;
		short notify_next_wave_timestamp;
		short item_count;

		int special_monster_num;				//�����������
		int remain_buyable_monster_num;

		TowerInfo tower_info_list[BUILD_TOWER_MAX_TOWER_POS_INDEX + 1];
		DropItem  item_list[BUILD_TOWER_MAX_DROP_ID_NUM];
	};

	class CSBuildTowerBuyTimes		 //8572
	{
	public:
		CSBuildTowerBuyTimes();
		MessageHeader header;
	};

	/////////////////////////////////////////////////// ����ػ����� /////////////////////////////////////////////////////
	class SCTeamTowerDefendInfo		// 8580
	{
	public:
		SCTeamTowerDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_SKILL = 1,
			NOTIFY_REASON_INIT = 2,
			NOTIFY_REASON_NEXT_WAVE = 3,
		};

		struct SkillInfo
		{
			SkillInfo() : skill_id(0), skill_level(0), next_can_perform_time(0) {}

			UInt16 skill_id;
			short skill_level;
			unsigned int next_can_perform_time;
		};

		static const int MAX_SKILL_COUNT = 4;

		int reason;
		long long life_tower_left_hp;					// ������HP
		long long life_tower_left_maxhp;				// ���������HP
		int gongji_uid;									// �����ӳ���
		int fangyu_uid;									// �����ӳ���
		int assist_uid;									// �����ӳ���
		int curr_wave;									// ��ǰ��
		int next_wave_refresh_time;						// ��һ������ʱ��
		int score;										// ��û���
		int exp;										// ��þ���
		int clear_wave;									// ������
		SkillInfo skill_list[MAX_SKILL_COUNT];			// �����б�
	};

	class SCTeamTowerDefendAttrType		//8581
	{
	public:
		SCTeamTowerDefendAttrType();
		MessageHeader header;

		struct AttrType
		{
			AttrType():uid(0),attr_type(0){}
			int uid;					// ���id
			int attr_type;				// ��������ӳ���������
		};

		AttrType attr_list[MAX_TEAM_MEMBER_NUM];
	};

	class SCTeamTowerDefendSkill		// 8582
	{
	public:
		SCTeamTowerDefendSkill();
		MessageHeader header;

		UInt16 skill_index;				// �����±�
		short skill_level;				// ���ܵȼ�
		unsigned int next_can_perform_time;		// �´ο��ͷż���ʱ���
	};

	enum TEAM_TOWER_DEFEND_OPREAT_REQ_TYPE
	{
		TEAM_TOWER_DEFEND_SET_ATTR_TYPE = 0,
		TEAM_TOWER_DEFEND_NEXT_WAVE_REQ = 1,
	};

	class CSTeamTowerDefendOpreatReq		//8583 ����������������ӳ���������
	{
	public:
		CSTeamTowerDefendOpreatReq();
		MessageHeader header;

		int req_type;
		int param1;						// ���id
		int param2;						// ���Լӳ�����
	};

	class SCTeamTowerDefendAllRole  // 8584 ���������Ϣ�·�
	{
	public:
		SCTeamTowerDefendAllRole();
		MessageHeader header;
		static const int BUFF_COUNT_MAX = 6;
		struct roleInfo
		{
			int uid;
			GameName user_name;
			long long hp;
			long long max_hp;
			char attr_type;
			char buff_count;
			char buff[BUFF_COUNT_MAX];
		};
		int role_count;
		roleInfo role_list[MAX_TEAM_MEMBER_NUM];
	};

	class SCTeamTowerDefendResult	//8585 ����ػ� ������Ϣ
	{
	public:
		SCTeamTowerDefendResult();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;
		struct DropItem
		{
			DropItem() : item_id(0), num(0) {}

			ItemID item_id;
			short num;
		};

		char is_passed;
		char clear_wave_count;
		short use_time;  //��������ʱ

		int xiannv_shengwu;//����ֵ
		int item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};


	///////////////////////////////////////////////////���鸱�� /////////////////////////////////////////////////////
	class CSStoryFBGetInfo
	{
	public:
		CSStoryFBGetInfo();
		MessageHeader header;
	};

	class SCStoryFBInfo
	{
	public:
		SCStoryFBInfo();
		MessageHeader header;

		struct InfoItem
		{
			short is_pass;
			short today_times;
		};
		
		InfoItem info_list[FB_STORY_MAX_COUNT];
	};

	class SCFbPickItemInfo
	{
	public:
		SCFbPickItemInfo();
		MessageHeader header;

		static const int PICK_ITEM_COUNT_MAX = 20;

		int item_count;
		ItemDataWrapper item_list[PICK_ITEM_COUNT_MAX];
	};

	class CSFbGuwu
	{
	public:
		CSFbGuwu();
		MessageHeader header;

		int is_gold;
	};

	class SCFBSceneLogicInfo				// 5499
	{
	public:
		SCFBSceneLogicInfo();
		MessageHeader header;

		void reset()
		{
			time_out_stamp = 0;
			flush_timestamp = 0;
			kick_timestamp = 0;
			scene_type = 0;
			is_finish = 0;
			is_pass = 0;
			is_active_leave_fb = 0;

			total_boss_num = 0;
			total_allmonster_num = 0;
			kill_boss_num = 0;
			kill_allmonster_num = 0;

			pass_time_s = 0;
			coin = 0;
			exp = 0;

			param1 = 0;
			param2 = 0;
			param3 = 0;
		}
		unsigned int time_out_stamp;									// ������ʱ����ʱ����������ڵ���ʱ��
		unsigned int flush_timestamp;                                   // ����ˢ�������������ʱ�����ˢ�£�
		unsigned int kick_timestamp;                                    // ��ɫ�߳�����ʱ����������ڵ���ʱ��

		char scene_type;												// ��������
		char is_finish;													// �Ƿ����
		char is_pass;													// �Ƿ�ͨ��
		char is_active_leave_fb;										// �Ƿ������˳�
		
		short total_boss_num;											// boss������
		short total_allmonster_num;										// ������������������ͨ�ֺ�boss)
		short kill_boss_num;											// �ѻ�ɱboss����
		short kill_allmonster_num;										// �ѻ�ɱ������������������ͨ�ֺ�boss)

		int pass_time_s;												// ���븱����Ŀǰ������ʱ�䣨�٣�
		int coin;														// ͭ��
		long long exp;														// ����

		long long param1;
		int param2;
		int param3;
	};
	
	// ���鸱����Ϣ 5555
	class SCExpFbInfo
	{
	public:
		SCExpFbInfo();
		MessageHeader header;

		void reset()
		{
			time_out_stamp = 0;
			scene_type = 0;
			is_finish = 0;
			guwu_times = 0;
			team_member_num = 0;
			exp = 0;
			record_max_exp = 0;
			exp_percent = 0;
			wave = 0;
			kill_allmonster_num = 0;
			start_time = 0;
		}
		time_t time_out_stamp;											// ������ʱ����ʱ���
		char scene_type;												// ��������
		char is_finish;													// �Ƿ����
		char guwu_times;												// �������
		char team_member_num;											// �������
		long long exp;													// ����
		long long record_max_exp;									    // ��ʷ��߾���
		int exp_percent;
		short wave;														// ����
		unsigned short kill_allmonster_num;								// ɱ����
		time_t start_time;												// ��ʼʱ��
	};

	// ���鸱���������  5556
	class CSExpFbPayBuff
	{
	public:
		CSExpFbPayBuff();
		MessageHeader header;

		short is_auto;													// �Ƿ��Զ�����
		short reverse_sh;
	};

	/////////////////////////////////////////////////// �������� ////////////////////////////////////////////////////////
	class CSFbGuideFinish												// 8400 ���
	{
	public:
		CSFbGuideFinish();
		MessageHeader header;

		int monster_x;
		int monster_y;								
		int monster_id;	
	};

	class CSFbGuideCreateGather											// 8401
	{
	public:
		CSFbGuideCreateGather();
		MessageHeader header;

		int gather_x;
		int gather_y;
		int gather_id;
		int gather_time;	// ��λ��
	};

	///////////////////////////////////////////////////���ܿ������� /////////////////////////////////////////////////////
	class SCFunOpenMountInfo									// 8300 ���︱����Ϣ
	{
	public:
		SCFunOpenMountInfo();
		MessageHeader header;

		char is_finish;											// �Ƿ����
		char reserve_1;								
		short reserve_2;									

		int cur_step_monster_total_num;							// ��ǰ�������ˢ��������
		int cur_step_monster_kill_num;							// ��ǰ��������ɱ����

		int cur_step_gather_total_num;							// ��ǰ���ɼ���ˢ��������
		int cur_step_gather_num;								// ��ǰ����ɼ���ɼ�����
	};

	class SCFunOpenWingInfo							// 8301 ��������Ϣ
	{
	public:
		SCFunOpenWingInfo();
		MessageHeader header;

		char is_finish;											// �Ƿ����
		char reserve_1;								
		short reserve_2;											
		
		int cur_step_monster_total_num;							// ��ǰ�������ˢ��������
		int cur_step_monster_kill_num;;							// ��ǰ��������ɱ����
	};

	class SCFunOpenXiannvInfo						// 8302Ů�񸱱���Ϣ
	{
	public:
		SCFunOpenXiannvInfo();
		MessageHeader header;
		
		char is_finish;											// �Ƿ����
		char reserve_1;								
		short reserve_2;											

		int cur_step_monster_total_num;							// ��ǰ�������ˢ��������
		int cur_step_monster_kill_num;;							// ��ǰ��������ɱ����
	};

	class CSFunOpenStoryStep						// 8325
	{
	public:
		CSFunOpenStoryStep();
		MessageHeader header;

		int create_monster_step;
	};

	class CSFunOpenSetObjToPos						// 8326
	{
	public:
		CSFunOpenSetObjToPos();
		MessageHeader header;
		
		ObjID obj_id;
		short reserve;
		
		int pos_x;
		int pos_y;
	};

	class SCFunOpenStoryStepEnd						// 8327
	{
	public:
		SCFunOpenStoryStepEnd();
		MessageHeader header;

		int cur_step;								// ��ǰ����
	};

	///////////////////////////////////////////////////ħ�丱�� /////////////////////////////////////////////////////
	class SCMoJieFBAllInfo							// ������Ϣ 
	{
	public:
		SCMoJieFBAllInfo();
		MessageHeader header;

		int mojie_human_pian_task_mark;						// ħ������ƪ��ɱ��
		int mojie_airen_pian_task_mark;						// ħ�䰫��ƪ��ɱ��
		int mojie_jingling_pian_task_mark;					// ħ�侫��ƪ��ɱ��
	};

	///////////////////////////////////////////////////�ճ����� /////////////////////////////////////////////////////
	class CSDailyFBGetRoleInfo						// �����ճ�����������Ϣ 5410
	{
	public:
		CSDailyFBGetRoleInfo();
		MessageHeader header;
	};

	class CSExpFBRetchFirstRewardReq				// ���鱾��ȡ��ͨ���� 5554
	{
	public:
		CSExpFBRetchFirstRewardReq();
		MessageHeader header;

		int fetch_reward_wave;
	};

	class SCDailyFBRoleInfo							// ������Ϣ 5411
	{
	public:
		SCDailyFBRoleInfo();
		MessageHeader header;

		short expfb_today_pay_times;	// ���칺�����
		short expfb_today_enter_times;	// ����������
		unsigned int last_quit_fb_time; // ���һ���˳�ʱ��
		long long max_exp;				// �����
		short max_wave;					// �����
		unsigned short expfb_history_enter_times; // ��ʷ�������
	};

	///////////////////////////////////////////////////�������� /////////////////////////////////////////////////////
	class CSPataFbAllInfo
	{
	public:
		CSPataFbAllInfo();
		MessageHeader header;
	};

	class SCPataFbAllInfo
	{
	public:
		SCPataFbAllInfo();
		MessageHeader header;

		short pass_level;
		short today_level;
	};

	///////////////////////////////////////////////////VIP����////////////////////////////////////////////////
	class CSVipFbAllInfoReq
	{
	public:
		CSVipFbAllInfoReq();
		MessageHeader header;
	};

	class SCVipFbAllInfo
	{
	public:
		SCVipFbAllInfo();
		MessageHeader header;
		
		int is_pass_flag;
		char today_times_list[FB_VIP_NUM_MAX];
	};

	///////////////////////////////////////////////////��Ӹ������� ////////////////////////////////////////////////
	class CSTeamFbRoomOperate
	{
	public:
		CSTeamFbRoomOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
		int param3;
		int param4;
		int param5;
	};

	class SCTeamFbRoomList
	{
	public:
		SCTeamFbRoomList();
		MessageHeader header;

		int team_type;
		int count;

		struct TeamFbRoomItem
		{
			TeamFbRoomItem() { this->Reset(); }

			void Reset()
			{
				team_index = 0;
				memset(leader_name, 0, sizeof(GameName));
				leader_capability = 0;
				limit_capability = 0;
				avatar_key_big = 0;
				avatar_key_small = 0;
				menber_num = 0;
				mode = 0;
				leader_sex = 0;
				leader_prof = 0;
				layer = 0;
				assign_mode = 0;
				is_must_check = 0;
				reserve_2 = 0;
			}

			int team_index;
			GameName leader_name;
			int leader_capability;
			int limit_capability;
			unsigned int avatar_key_big;
			unsigned int avatar_key_small;
			char menber_num;
			char mode;
			char leader_sex;
			char leader_prof;
			int leader_uid;
			char layer;
			char assign_mode;
			char is_must_check;
			char reserve_2;
		};

		TeamFbRoomItem room_list[FB_TEAM_ROOM_MAX_NUM];
	};

	class SCTeamFbRoomEnterAffirm
	{
	public:
		SCTeamFbRoomEnterAffirm();
		MessageHeader header;

		int team_type;
		int mode;
		int layer;
	};

	//////////////////////////////////�������޼�̳����///////////////////////////////////////////////////////////////////
	static const int TEAMFB_MAX_ATTR_NUM = 4;
	class SCYsjtTeamFbSceneLogicInfo
	{
	public:
		SCYsjtTeamFbSceneLogicInfo();
		MessageHeader header;
		
		unsigned int time_out_stamp;									// ������ʱ����ʱ����������ڵ���ʱ��
		char scene_type;												// ��������
		char is_finish;													// �Ƿ����
		char is_pass;													// �Ƿ�ͨ��
		char is_active_leave_fb;										// �Ƿ������˳�
		
		int kill_boss_num;
		int pass_time_s;
		int mode;
		int boss_attr_type;
		YaoShouJiTanTeamFbRoleAttrInfo role_attrs[TEAMFB_MAX_ATTR_NUM];
	};

	//////////////////////////////////�Թ��ɸ�����///////////////////////////////////////////////////////////////////
	class CSMgxfTeamFbTouchDoor
	{
	public:
		CSMgxfTeamFbTouchDoor();
		MessageHeader header;

		short layer;
		short door_index;
	};

	class SCMgxfTeamFbSceneLogicInfo
	{
	public:
		SCMgxfTeamFbSceneLogicInfo();
		MessageHeader header;

		unsigned int time_out_stamp;									// ������ʱ����ʱ����������ڵ���ʱ��
		short is_finish;													// �Ƿ����
		short is_pass;													// �Ƿ�ͨ��

		int pass_time_s;
		int mode;
		short layer;													// ����������ڲ���
		char kill_hide_boos_num;										// ɱ��������boss����
		char kill_end_boss_num;											// ɱ��������boss����
		
		struct LayerItem
		{
			int door_status_list[MIGONGXIANFU_DOOR_NUM];
		};
		
		LayerItem layer_list[MIGONGXIANFU_LAYER_NUM];
	};

	/////////////////////////////////////////////////// ��ս���� /////////////////////////////////////////////////////
	enum CHALLENGE_FB_OPERATE_TYPE
	{
		CHALLENGE_FB_OPERATE_TYPE_AUTO_FB = 0,							//ɨ��
		CHALLENGE_FB_OPERATE_TYPE_RESET_FB,								//����
		CHALLENGE_FB_OPERATE_TYPE_SEND_INFO_REQ,						//������Э��
		CHALLENGE_FB_OPERATE_TYPE_BUY_TIMES,							//�������
	};

	class CSChallengeFBOP								//5467
	{
	public:
		CSChallengeFBOP();
		MessageHeader header;

		short type;										//0ɨ����1���ùؿ���2������Э��
		short param;									//0,1�ؿ�����  2, 3��0
	};

	class SCChallengeFBInfo								//5407
	{
	public:
		SCChallengeFBInfo();
		MessageHeader header;

		struct Level
		{
			char is_pass;
			char fight_layer;
			short state;
			short history_max_reward;
			short is_continue;
			char use_count;
			char res_ch;
			short res_sh;
		};

		int enter_count;
		int buy_count;
		short is_auto;
		short reserve_sh;
		Level level_list[ChallengeFBParam::LEVEL_MAX_COUNT];
	};

	class SCChallengePassLevel							//5408
	{
	public:
		SCChallengePassLevel();
		MessageHeader header;

		short level;
		char is_pass;
		char fight_layer;
		char is_active_leave_fb;
		char reward_flag;
		short reserve_sh;
		unsigned int pass_time;
	};

	class SCChallengeLayerInfo						//5419
	{
	public:
		SCChallengeLayerInfo();
		MessageHeader header;

		char is_pass;
		char is_finish;
		short reserve_sh;
		unsigned int time_out_stamp;
	};

	/////////////////////////////////////////////////// �������� /////////////////////////////////////////////////////
	class CSQueryGuildMonster
	{
	public:
		CSQueryGuildMonster();
		MessageHeader header;
	};

	class SCGuildMonsterInfo
	{
	public:
		SCGuildMonsterInfo();
		MessageHeader header;

		short my_finish_seq;
		short guild_finish_seq;
		GameName guild_finish_username;
		char prof;
		char reserve_ch;
		short reserve_sh;
	};

	class SCGuildMonsterFBResult
	{
	public:
		SCGuildMonsterFBResult();
		MessageHeader header;

		short is_pass;
		short reserve;
	};

	//////////////////////////////////�׶θ���//////////////////////////////
	class SCPhaseFBInfo			// 5418 �׶θ���������Ϣ
	{
	public:
		SCPhaseFBInfo();
		MessageHeader header;

		struct InfoItem
		{
			char is_pass;			// �Ѿ�ͨ���˵ڼ���
			char today_buy_times;	// ���չ���ĸ�������
			short today_times;		// ���ո�������
		};

		InfoItem info_list[FB_PHASE_MAX_COUNT];
	};

	enum PHASE_FB_OPERATE_TYPE
	{
		PHASE_FB_OPERATE_TYPE_INFO = 0,			// ��ȡ��Ϣ
		PHASE_FB_OPERATE_TYPE_BUY_TIMES = 1,	// �������
	};

	class CSPhaseFBInfoReq		// 5465 �׶θ���������Ϣ����
	{
	public:
		CSPhaseFBInfoReq();
		MessageHeader header;

		short operate_type;		// ��������enum PHASE_FB_OPERATE_TYPE
		short fb_index;			// ��������
	};

	//////////////////////////////////���˸���//////////////////////////////
	class SCGuildFBInfo					// 5478
	{
	public:
		SCGuildFBInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_ENTER,				// ���볡��
			NOTIFY_REASON_WAIT,					// �ȴ�
			NOTIFY_REASON_UPDATE,				// ��������
			NOTIFY_REASON_FINISH,				// �������

			NOTIFY_REASON_MAX,
		};

		const static int RANK_ITEM_MAX_COUNT = 20;

		struct Rank
		{
			int user_id;
			GameName user_name;
			long long hurt_val;
		};

		short notify_reason;
		short curr_wave;
		unsigned int next_wave_time;
		short wave_enemy_count;
		short wave_enemy_max;
		short is_pass;
		short is_finish;
		long long hp;
		long long maxhp;
		unsigned int kick_role_time;

		int self_rank_pos;
		long long self_hurt_val;
		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT]; // boss�˺�������Ϣ
	};

	enum REWARD_RECORDE_INFO_TYPE
	{
		REWARD_RECORDE_INFO_TYPE_CUR_WAVE = 0,			// ��ǰ��������¼
		REWARD_RECORDE_INFO_TYPE_HISTORY,				// ��ʷ������¼
	};

	class SCGuildFBRewardRecordInfo				//	2701 ���˸���������Ϣ
	{
	public:
		SCGuildFBRewardRecordInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 20;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0), item_color(0), reserve_sh(0){}
			ItemID reward_item_id;
			short reward_item_num;
			short item_color;
			short reserve_sh;
		};

		short info_type;
		short resserve_sh;

		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};


	//////////////////////////////////boss֮��//////////////////////////////

	enum BOSS_ENTER_TYPE
	{
		BOSS_ENTER_TYPE_IVNALID = 0,

		BOSS_ENTER_TYPE_BOSS_FAMILY,	   // vip boss	
		BOSS_ENTER_TYPE_BOSS_MIKU,         // ��Ӣboss
		BOSS_ENTER_TYPE_BOSS_DABAO,        // ��װboss
		BOSS_ENTER_LEAVE_BOSS_SCENE,       // �뿪
		BOSS_ENTER_TYPE_BOSS_ACTIVE,       // ��Ծboss
		BOSS_ENTER_TYPE_BOSS_PRECIOUS,
		BOSS_ENTER_TYPE_BOSS_SHANGGU,      // �Ϲ�boss
		BOSS_ENTER_TYPE_BOSS_BABY,         // ����boss
		BOSS_ENTER_TYPE_MAX,               // ������ԭ����Boss�����������������õ� 

		BOSS_TYPE_CROSS_BOSS = 10,         // ���Զ��boss
		BOSS_TYPE_CROSS_MIZANG_BOSS,       // ����(�ܲ�)BOSS
		BOSS_TYPE_CROSS_YOUMING_BOSS,
		
		BOSS_ENTER_TYPE_GODMAGIC_BOSS ,		  // ��ħboss

		BOSS_ENTER_TYPE_ALL_MAX,           // boss�������ֵ(���������ԭ��)
	};

	class CSBossFamilyOperate				//  5506 boss֮�Ҳ���
	{
	public:
		CSBossFamilyOperate();
		MessageHeader header;

		enum BOSS_FAMILY_OPERATE_TYPE
		{
			BOSS_FAMILY_BUY_MIKU_WEARY = 0, // �����ؿ�BOSSƣ��ֵ
			BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY = 1, // �����ԾBOSSƣ��ֵ
			BOSS_DABAO_BUY_ENTER_COUNT = 2,        // �����boss�������
		};

		int operate_type;
	};

	class CSEnterBossFamily			// 5477 ����boss֮������
	{
	public:
		CSEnterBossFamily();
		MessageHeader header;

		int enter_type;
		int scene_id;

		char is_buy_dabao_times;
		char reserve1;
		unsigned short boss_id;
	};

	class CSGetBossInfoReq    // 5463 ����boss��Ϣ
	{
	public:
		CSGetBossInfoReq();
		MessageHeader header;

		int enter_type;
	};

	class CSBossKillerInfoReq		// 5466 Boss��ɱ�б���Ϣ����
	{
	public:
		CSBossKillerInfoReq();
		MessageHeader header;

		int boss_type;
		int boss_id;
		int scene_id;
	};

	class SCDabaoBossInfo		// 6551 ��boss��Ϣ
	{
	public:
		SCDabaoBossInfo();
		MessageHeader header;

		short dabao_angry_value;
		short enter_count;

		unsigned int kick_time;

	};

	class SCFamilyBossInfo		// 6552 boss֮��boss��Ϣ
	{
	public:
		SCFamilyBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct FamilyBossInfo
		{
			int boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		FamilyBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCMikuBossInfo		// 6553 �ؿ�boss��Ϣ
	{
	public:
		SCMikuBossInfo();
		MessageHeader header;

		short miku_boss_weary;
		short reserve;

		static const int MAX_BOSS_COUNT = 100;
		int count;
		int scene_id;
		int miku_monster_num;

		struct MikuBossInfo
		{
			int boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		MikuBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCMikuMonsterInfo		//  6561�ؿ߾�Ӣ������Ϣ
	{
	public:
		SCMikuMonsterInfo();
		MessageHeader header;

		int miku_monster_num;
	};

	class SCBossRoleInfo		// 6554 boss��ظ�����Ϣ
	{
	public:
		SCBossRoleInfo();
		MessageHeader header;

		short miku_boss_weary;
		short reserve;
	};

	class SCDabaoBossNextFlushInfo		// 6555 ��bossˢ��ʱ����Ϣ
	{
	public:
		SCDabaoBossNextFlushInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct DabaoBossInfo
		{
			int boss_id;
			unsigned int next_refresh_time;
		};

		DabaoBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCActiveBossNextFlushInfo		// 6559 ��Ծbossˢ��ʱ����Ϣ
	{
	public:
		SCActiveBossNextFlushInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct ActiveBossInfo
		{
			int boss_id;
			unsigned int next_refresh_time;
		};

		ActiveBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCActiveBossInfo			// 6560 ��Ծboss��Ϣ
	{
	public:
		SCActiveBossInfo();
		MessageHeader header;

		short enter_count;
		short active_boss_weary;
	};

	class SCActiveBossHurtRankInfo			// 6566 ��Ծboss�˺�������Ϣ
	{
	public:
		SCActiveBossHurtRankInfo();
		MessageHeader header;

		static const int MAX_RANK_COUNT = 5;

		struct RankInfo
		{
			GameName name;			// �������
			Attribute hurt;			// �˺�
		};

		int monster_id;
		long long my_hurt;
		short my_rank;
		short rank_count;			// ��������
		RankInfo rank_info_list[MAX_RANK_COUNT];
	};

	class SCActiveBossLeaveInfo			  // 6567 �뿪��Ծboss�˺���Ϣ����
	{
	public:
		SCActiveBossLeaveInfo();
		MessageHeader header;
	};

	class SCMikuBossHurtRankInfo			// 6568 ��Ӣ(����)boss�˺�������Ϣ
	{
	public:
		SCMikuBossHurtRankInfo();
		MessageHeader header;

		static const int MAX_RANK_COUNT = 5;

		struct RankInfo
		{
			GameName name;			// �������
			Attribute hurt;			// �˺�
		};

		int monster_id;
		long long my_hurt;
		short my_rank;
		short rank_count;			// ��������
		RankInfo rank_info_list[MAX_RANK_COUNT];
	};
	class SCMikuBossLeaveInfo			  // 6569 �뿪��Ծboss�˺���Ϣ����
	{
	public:
		SCMikuBossLeaveInfo();
		MessageHeader header;
	};
	//////////////////////////////////////////////////////////////////////////
	enum NOTIFY_REASON_BOSS
	{
		NOTIFY_REASON_BOSS_ADVANCE = 0,					// ˢ��ǰ���ѣ�60s��
		NOTIFY_REASON_BOSS_REFRESH = 1,					// bossˢ��
		NOTIFY_REASON_BOSS_DIE = 2,						// BOSS����
	};

	class SCBossInfoToAll			// 6556 boss�㲥��Ϣ
	{
	public:
		SCBossInfoToAll();
		MessageHeader header;

		int notify_reason;					// ֪ͨ����  enum NOTIFY_REASON_BOSS
		int boss_type;						// BOSSö������ enum BOSS_ENTER_TYPE
		int boss_id;
		int scene_id;
		int status;
		unsigned int next_refresh_time;
		int killer_uid;
	};

	class SCBossKillerList			// 6557 boss��ɱ�б���Ϣ
	{
	public:
		SCBossKillerList();
		MessageHeader header;

		struct KillerInfo
		{
			KillerInfo():killer_uid(0), killier_time(0)
			{
				memset(killer_name, 0, sizeof(killer_name));
			}

			int killer_uid;
			unsigned int killier_time;
			GameName killer_name;
		};
		static const int KILLER_LIST_MAX_COUNT = 5;

		KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	};

	class SCFollowBossInfo		// 6558 boss��ע�б���Ϣ
	{
	public:
		SCFollowBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int count;

		struct BossBaseInfo
		{
			short boss_id;
			char boss_type;
			char reserve_ch;
			int scene_id;
		};

		BossBaseInfo follow_boss_list[MAX_BOSS_COUNT];
	};

	class CSFollowBossReq		//  6570 boss��ע����
	{
	public:
		CSFollowBossReq();
		MessageHeader header;

		enum FOLLOW_BOSS_OPE_TYPE
		{
			FOLLOW_BOSS = 0,		// ��עboss
			UNFOLLOW_BOSS,			// ȡ����ע
			GET_FOLLOW_LIST,		// ��ȡ��ע�б�
		};

		int opera_type;
		int boss_type;
		int boss_id;
		int scene_id;
	};

	class SCPreciousBossTaskInfo					// 6562 �ز�boss������Ϣ
	{
	public:
		SCPreciousBossTaskInfo();
		MessageHeader header;

		static const int MAX_TASK_COUNT = 10;

		struct PreciousBossTaskInfo
		{
			PreciousBossTaskInfo() : task_condition(0), task_type(0), is_finish(0)
			{}

			short task_condition;					// ������������
			short task_type;						// ��������
			int is_finish;							// �Ƿ����
		};

		int count;									// ��������
		PreciousBossTaskInfo task_list[MAX_TASK_COUNT];
	};

	class SCPreciousBossInfo						// 6563 �ز�boss��Ϣ
	{
	public:
		SCPreciousBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 10;

		struct PreciousBossInfo
		{
			PreciousBossInfo() : boss_id(0), next_refresh_time(0)
			{}

			int boss_id;							// boss��id
			unsigned int next_refresh_time;			// �´�ˢ��ʱ��,Ϊ0��ʾboss�����򲻴��
		};

		int count;									// boss����
		PreciousBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class CSPreciousOperaReq							// 6564 �ز�����λ����Ϣ
	{
	public:
		CSPreciousOperaReq();
		MessageHeader header;

		enum PRECIOUS_OPERA_TYPE
		{
			PRECIOUS_OPERA_TYPE_POS = 0,
			PRECIOUS_OPERA_TYPE_TASK,
		};

		int type;									// 0����λ�ã�1��������Э��
		int param;									// λ������
		int param_2;								// boss_id
	};

	class SCPreciousPosInfo							// 6565 �ز�λ����Ϣ
	{
	public:
		SCPreciousPosInfo();
		MessageHeader header;

		int pos_x;									// ������
		int pos_y;									// ������
		int param;									// Ŀ��id
	};

	//////////////////////////////////���󳡾�����ˢ����Ϣ//////////////////////////////

	static const int SCENE_MONSTER_GENERATER_MAX_NUM = 64;
	static const int SCENE_GATHER_GENERATER_MAX_NUM = 64;

	class CSReqMonsterGeneraterList // ����������ɵ��б���Ϣ  5479
	{
	public:
		CSReqMonsterGeneraterList();
		MessageHeader		header;

		int get_scene_id;
	};

	struct SCMonsterGeneraterInfo
	{
		int boss_id;
		unsigned int next_refresh_time;
	};

	class SCMonsterGeneraterList			// �·���ǰ�����������ɵ��б���Ϣ5480
	{
	public:
		SCMonsterGeneraterList();
		MessageHeader		header;

		int boss_count;
		SCMonsterGeneraterInfo boss_list[SCENE_MONSTER_GENERATER_MAX_NUM];
	};

	class CSReqGatherGeneraterList			// ����ɼ������ɵ��б���Ϣ 5525
	{
	public:
		CSReqGatherGeneraterList();
		MessageHeader		header;

		int get_scene_id;
		int scene_key;
	};

	struct SCGatherGeneraterInfo
	{
		int gather_id;
		int pos_x;
		int pos_y;
		unsigned int next_refresh_time;
	};

	class SCGatherGeneraterList				// �·���ǰ�����ɼ������ɵ��б���Ϣ  5526
	{
	public:
		SCGatherGeneraterList();
		MessageHeader		header;

		int gather_count;
		SCGatherGeneraterInfo gather_list[SCENE_GATHER_GENERATER_MAX_NUM];
	};

	//////////////////////////////////���޹㳡//////////////////////////////
	
	class CSEnterYaoShouGuangChang
	{
	public:
		CSEnterYaoShouGuangChang();
		MessageHeader		header;

		int is_buy_times;
	};

	static const int YAOSHOUGUANGCHANG_ROOM_USER_MAX = 10;

	class SCYaoShouGuangChangFBInfo	
	{
	public:
		SCYaoShouGuangChangFBInfo();
		MessageHeader		header;

		enum  
		{
			YAOSHOUGUANGCHANG_NOTIFY_INVALID = 0,
			YAOSHOUGUANGCHANG_NOTIFY_SCORE_CHANGE,
			YAOSHOUGUANGCHANG_NOTIFY_ENTER_SCENE,
			YAOSHOUGUANGCHANG_NOTIFY_WAVE_FINISH,
			YAOSHOUGUANGCHANG_NOTIFY_MAX,
		};

		void Reset()
		{
			reason = 0;

			wave_index = 0;
			fb_lv = 0;
			role_num = 0;
			monster_num = 0;

			user_count = 0;
			memset(user_list, 0, sizeof(user_list));
		}

		int reason;

		int wave_index;
		int fb_lv;
		int role_num;
		int monster_num;

		struct YSGCUserInfo
		{
			int uid;
			int score;
		};
		
		int user_count;
		YSGCUserInfo user_list[YAOSHOUGUANGCHANG_ROOM_USER_MAX];
	};

	class SCYaoShouGuangChangState
	{
	public:
		SCYaoShouGuangChangState();
		MessageHeader		header;

		int status;
		unsigned int next_status_time;
		unsigned int next_standy_time;
		unsigned int next_stop_time;

		int datais_valid;
		int ysgc_max_score;
		int quanfu_topscore;
		int	quanfu_topscore_uid;
		GameName quanfu_topscore_name;
	};

	class CSGetYaoShouGuangChangState
	{
	public:
		CSGetYaoShouGuangChangState();
		MessageHeader		header;
	};

	class CSGetYaoShouGuangChangReward
	{
	public:
		CSGetYaoShouGuangChangReward();
		MessageHeader		header;

		int times;
	};

	class SCNotifyYaoShouGuangChangReward
	{
	public:
		SCNotifyYaoShouGuangChangReward();
		MessageHeader		header;

		int score;
		int exp;
		int bind_coin;
	};

	//////////////////////////////////������//////////////////////////////

	class CSEnterSuoYaoTa
	{
	public:
		CSEnterSuoYaoTa();
		MessageHeader		header;

		int is_buy_times;
	};

	static const int SUOYAOTA_ROOM_USER_MAX = 10;
	static const int SUOYAOTA_TASK_MAX = 4;

	class SCSuoYaoTaFBInfo	
	{
	public:
		SCSuoYaoTaFBInfo();
		MessageHeader		header;

		void Reset()
		{
			reason = 0;

			fb_lv = 0;

			user_count = 0;
			memset(user_list, 0, sizeof(user_list));

			memset(task_list, 0, sizeof(task_list));
		}

		enum  
		{
			SUOYAOTA_NOTIFY_INVALID = 0,
			SUOYAOTA_NOTIFY_SCORE_CHANGE,
			SUOYAOTA_NOTIFY_ENTER_SCENE,
			SUOYAOTA_NOTIFY_WAVE_FINISH,
			SUOYAOTA_NOTIFY_MAX,
		};

		int reason;

		int fb_lv;

		struct SYTTask
		{
			int task_type;
			int param_id;
			int param_num;
			int param_max;
		};
		SYTTask task_list[SUOYAOTA_TASK_MAX];

		struct SYTUserInfo
		{
			int uid;
			int score;
		};

		int user_count;
		SYTUserInfo user_list[SUOYAOTA_ROOM_USER_MAX];
	};

	class SCSuoYaoTaState
	{
	public:
		SCSuoYaoTaState();
		MessageHeader		header;

		int status;
		unsigned int next_status_time;
		unsigned int next_standby_time;
		unsigned int next_stop_time;
		
		int datais_valid;
		int syt_max_score;
		int quanfu_topscore;
		int	quanfu_topscore_uid;
		GameName quanfu_topscore_name;
	};

	class CSGetSuoYaoTaState
	{
	public:
		CSGetSuoYaoTaState();
		MessageHeader		header;
	};

	class CSGetSuoYaoTaReward
	{
	public:
		CSGetSuoYaoTaReward();
		MessageHeader		header;

		int times;
	};

	class SCNotifySuoYaoTaReward
	{
	public:
		SCNotifySuoYaoTaReward();
		MessageHeader		header;

		int score;
		int exp;
		int bind_coin;
	};

	////////////////////////////////////////////////////////////////////////////////////
	class CSGetGuildFBGuardPos				// 5496
	{
	public:
		CSGetGuildFBGuardPos();
		MessageHeader		header;
	};

	class SCGuildFBGuardPos					// 5497
	{
	public:
		SCGuildFBGuardPos();
		MessageHeader		header;

		int scene_id;
		int pos_x;
		int pos_y;
	};

	////////////////////////////////////////////////////////////////////////////////////
	class SCWorldBossDead
	{
	public:
		SCWorldBossDead();
		MessageHeader		header;

		int boss_id;
	};

	enum ZHUAGUI_NOTIFY_REASON
	{
		ZHUAGUI_NOTIFY_REASON_ENTER,
		ZHUAGUI_NOTIFY_REASON_MONSTER_DEAD,
		ZHUAGUI_NOTIFY_REASON_ENTER_FINISH,
	};

	class SCZhuaGuiFbInfo
	{
	public:
		SCZhuaGuiFbInfo();
		MessageHeader		header;

		static const int NEW_FB_REWARD_ITEM_SC_MAX = 30;						// ֪ͨ�ͻ��������Ʒ��
		void Reset()
		{
			monster_count = 0;
			ishave_boss = 0;
			boss_isdead = 0;
			enter_role_num = 0;
			item_count = 0;
			kick_time = 0;

			memset(zhuagui_info_list, 0, sizeof(zhuagui_info_list));
			memset(item_list, 0, sizeof(item_list));
		}

		int reason;

		int monster_count;
		short ishave_boss;
		short boss_isdead;
		unsigned int kick_time;

		struct TeamZhuaGuiInfo
		{
			TeamZhuaGuiInfo():uid(0), get_hunli(0), get_mojing(0), kill_boss_count(0){}

			int uid;
			int get_hunli;
			int get_mojing;
			int kill_boss_count;
		};

		void AddHunli(int uid, int huoli, int mojing, int boss_monster_count)
		{
			for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
			{
				if (zhuagui_info_list[i].uid == uid || 0 == zhuagui_info_list[i].uid)
				{
					zhuagui_info_list[i].uid = uid;
					zhuagui_info_list[i].get_hunli += huoli;
					zhuagui_info_list[i].get_mojing += mojing;
					zhuagui_info_list[i].kill_boss_count += boss_monster_count;
					break;
				}
			}
		}

		TeamZhuaGuiInfo zhuagui_info_list[MAX_TEAM_MEMBER_NUM];
		short enter_role_num;
		short item_count;
		FBDropItemInfo item_list[NEW_FB_REWARD_ITEM_SC_MAX];
	};

	class SCZhuaguiAddPerInfo
	{
	public:
		SCZhuaguiAddPerInfo();
		MessageHeader	header;

		short couple_hunli_add_per;  
		short couple_boss_add_per;
		short team_hunli_add_per;
		short team_boss_add_per;
	};

	class SCCrossTeamFbInfo					// 5498
	{
	public:
		SCCrossTeamFbInfo();
		MessageHeader		header;

		struct UserInfo
		{
			GameName user_name;
			long long dps;
		};

		int user_count;
		UserInfo user_info[CROSS_TEAM_FB_ROLE_MAX];
	};

	class SCTeamEquipFbInfo						// 5570
	{
	public:
		SCTeamEquipFbInfo();
		MessageHeader		header;

		struct UserInfo
		{
			GameName user_name;
			long long dps;
		};

		int user_count;
		UserInfo user_info[TEAM_EQUIP_FB_ROLE_MAX];
	};

	class CSTeamEquipFbBuyDropCount							// 5572���װ����������������
	{
	public:
		CSTeamEquipFbBuyDropCount();
		MessageHeader		header;
	};


	class SCTeamEquipFbDropCountInfo						// 5571���װ���������������Ϣ
	{
	public:
		SCTeamEquipFbDropCountInfo();
		MessageHeader		header;

		int team_equip_fb_pass_flag;						// ���װ����ͨ�ر��
		int team_equip_fb_day_count;						// ���װ����ÿ�մ���
		int team_equip_fb_day_buy_count;					// ���װ����ÿ�չ������
	};

	/////////////////////////////////////////// ��ͼ���� /////////////////////////////////////////////

	enum TUITU_FB_OPERA_REQ_TYPE
	{
		TUITU_FB_OPERA_REQ_TYPE_ALL_INFO = 0,				//������Ϣ	
		TUITU_FB_OPERA_REQ_TYPE_BUY_TIMES,					//������븱������ param_1 ���򸱱����� param_2, �������
		TUITU_FB_OPERA_REQ_TYPE_FETCH_STAR_REWARD,			//��ȡ�Ǽ����� param_1:��������  param_2:���ñ�seq
		TUITU_FB_OPERA_REQ_TYPE_SAODANG,					//ɨ�� param_1:�������� param_2:�½� param_3:�ؿ�

		TUITU_FB_OPERA_REQ_TYPE_MAX
	};

	class CSTuituFbOperaReq			// 8430
	{
	public:
		CSTuituFbOperaReq();
		MessageHeader header;

		int opera_type;					
		int param_1;
		int param_2;
		int param_3;
	};

	class SCTuituFbInfo				//8431
	{
	public:
		SCTuituFbInfo();
		MessageHeader header;

		TuituFbParam::FbInfo fb_info_list[TUTUI_FB_TYPE_NUM_MAX];
	};

	class SCTuituFbResultInfo		//8432
	{
		struct ItemInfo
		{
			ItemID item_id;
			short num;
			char is_bind;
			char reserve_ch;
			short reserve_sh;
		};

	public:
		static const int MAX_REWARD_ITEM_NUM = 64;

		SCTuituFbResultInfo();
		MessageHeader header;

		char star;			//ͨ���Ǽ� star > 0��ɹ� ����ʧ��
		char reserve_ch;	//�����ֶ�
		short item_count;	//��Ʒ����

		ItemInfo reward_item_list[MAX_REWARD_ITEM_NUM];
	};

	class SCTuituFbSingleInfo		//8433
	{
	public:
		SCTuituFbSingleInfo();
		MessageHeader header;
	
		short fb_type;						//��������
		char chapter;						//�����½�
		char level;							//�����ؿ��ȼ�
		short cur_chapter;					//��ǰ�����½�
		short cur_level;					//��ǰ���йؿ��ȼ�
		short today_join_times;				//���ս��븱������
		short buy_join_times;				//�������
		short chapter_total_star;			//�½�������
		short chapter_star_reward_flag;		//�½������������
		TuituFbParam::LayerInfo layer_info;	//�ؿ���Ϣ
	};

	class SCTuituFbFetchResultInfo
	{
	public:
		SCTuituFbFetchResultInfo();
		MessageHeader header;

		short is_success;			//�Ƿ�ɹ���ȡ 1�ɹ� 0ʧ��
		short fb_type;				//fb���ͣ�Ŀǰֻ����ͨ���н�����
		short chapter;				//�½�
		short seq;					//��ȡ��seq���ͻ��˸������seqȡ������ʾ��
	};
	/////////////////////////////////////////// ����boss /////////////////////////////////////////////
	class CSBabyBossOperate						//8614
	{
	public:
		CSBabyBossOperate();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_BOSS_INFO_REQ = 0,				// ����boss��Ϣ
			TYPE_ROLE_INFO_REQ = 1,				// �������������Ϣ
			TYPE_ENTER_REQ = 2,					// ������뱦��boss
			TYPE_LEAVE_REQ = 3,					// �����뿪����boss
		};

		int operate_type;
		int param_0;
		unsigned short param_1;
		short reserve_sh;
	};

	class SCBabyBossRoleInfo				//8615
	{
	public:
		SCBabyBossRoleInfo();
		MessageHeader header;

		short enter_times;					//�������
		short angry_value;					//��ŭֵ
		unsigned int kick_time;				//���ʱ��
	};

	struct BabyBossInfo
	{
		BabyBossInfo() : scene_id(0), boss_id(0), next_refresh_time(0)
		{}

		short scene_id;						//����id
		unsigned short boss_id;				//boss_id
		unsigned int next_refresh_time;		//�´�ˢ��ʱ��

		struct KillerInfo
		{
			KillerInfo() :killer_uid(0), killier_time(0)
			{
				memset(killer_name, 0, sizeof(killer_name));
			}

			int killer_uid;
			unsigned int killier_time;		//��ɱʱ��
			GameName killer_name;			//��ɱ����
		};

		static const int KILLER_LIST_MAX_COUNT = 5;

		KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	};

	class SCAllBabyBossInfo					//8616
	{
	public:
		SCAllBabyBossInfo();
		MessageHeader header;

		static const int MAX_BABY_BOSS_COUNT = 100;

		int boss_count;
		BabyBossInfo boss_info_list[MAX_BABY_BOSS_COUNT];
	};

	class SCSingleBabyBossInfo				//8617
	{
	public:
		SCSingleBabyBossInfo();
		MessageHeader header;

		BabyBossInfo boss_info;
	};

	class SCRuneTowerPassRewardInfo         // 8818 ������������Ϣ    �ͻ�����ʾ�������
	{
	public:
		SCRuneTowerPassRewardInfo();
		MessageHeader header;

		struct RewardItem
		{
			ItemID item_id;
			char is_bind;
			char num;
		};
		
		int reward_count;
		RewardItem reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT];     // 7
	};

	/////////////////////////////////////////////////// �����ػ����� /////////////////////////////////////////////////////
	enum CS_HOLIDAI_GUARD_REQ
	{
		HOLIDAY_GUARD_NEXT_WAVE,
	};

	class CSHolidayGuardRoleReq		// 8825 �������
	{
	public:
		CSHolidayGuardRoleReq();
		MessageHeader header;

		int req_type;
	};

	class SCHolidayGuardRoleInfo		// 8826 ������Ϣ
	{
	public:
		SCHolidayGuardRoleInfo();
		MessageHeader header;

		int personal_join_times;
		short personal_kill_monster_count;
		short reserve_ch;
	};

	class SCHolidayGuardInfo			// 8827 ������Ϣ
	{
	public:
		SCHolidayGuardInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_KILL_MONSTER = 3,
		};

		short reason;
		short reserve;

		unsigned int time_out_stamp;
		short is_finish;
		short is_pass;
		int pass_time_s;

		long long life_tower_left_hp;							// ������HP
		long long life_tower_left_maxhp;						// ���������HP
		short curr_wave;								// ��ǰ��
		short reserve_1;
		int next_wave_refresh_time;						// ��һ������ʱ��
		short clear_wave_count;							// ������ﲨ��
		short total_kill_monster_count;					// ��ɱ��������

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		SCHolidayGuardInfo::DropItem drop_items[FB_RECORD_DROP_ITEM_MAX_NUM];
	};

	class SCHolidayGuardFBDropInfo		// 8828 �����������ͳ��
	{
	public:
		SCHolidayGuardFBDropInfo();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCHolidayGuardResult		// 8829 ͨ�ؽ��	
	{
	public:
		SCHolidayGuardResult();
		MessageHeader header;

		char is_passed;
		char clear_wave_count;
		short resertotal_kill_monster_countve_sh;
	};

	class SCHolidayGuardWarning		// 8830 ������������Ԥ��
	{
	public:
		SCHolidayGuardWarning();
		MessageHeader header;

		enum WARNING_TYPE
		{
			WARNING_TYPE_NORMAL = 1,
			WARNING_TYPE_LOW_PERCENT,
		};

		short warning_type;			// 1����ͨ���򣨺��԰ٷֱȣ���2��HP���ڰٷֱ�
		short percent;
	};

	class SCNoticeBossDead                                  // boss����֪ͨͬ�������������� 8823
	{
	public:
		SCNoticeBossDead();
		MessageHeader header;

		int boss_id;
		int killer_uid;
		long long killer_avatar_timestamp;					// ��ɱ��ͷ��
	};

	class CSFetchDoubleRewardReq							// 8873 ����˫����ȡ
	{
	public:
		CSFetchDoubleRewardReq();
		MessageHeader header;

		enum OPERA_TYPE
		{
			OPERA_TYPE_INFO_REQ = 0,
			OPERA_TYPE_BUY_DOUBLE_REWARD,
			OPERA_TYPE_BUY_TRIPLE_REWARD,
		};

		int fb_type;										//��������
		int opera_type;											
	};

	class SCFetchDoubleRewardResult							// 8874 ˫����ȡ���
	{
	public:
		SCFetchDoubleRewardResult();
		MessageHeader header;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int fb_type;										//��������
		char today_buy_times;								//�����ѹ������
		char reserve_sh;
		short item_count;
		int xiannv_shengwu;									//����ֵ����ػ���
		DropItem item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	};

	////////////////////////////����boss//////////////////////////////////
	class CSPersonBossInfoReq					// ����boss��Ϣ����   8876
	{
	public:
		CSPersonBossInfoReq();
		MessageHeader header;
	};

	class SCPersonBossInfo						// ����boss��Ϣ    8877
	{
	public:
		SCPersonBossInfo();
		MessageHeader header;

		struct Info
		{
			Info() : layer(0), times(0) {}
			short layer;
			short times;
		};

		Info info_list[MAX_PERSON_BOSS_LAYER];     // 40
	};

	/////////////////////////////////////  ��ħBOSS  /////////////////////////////////////
	enum GODMAGIC_BOSS_OPERA_TYPE
	{
		GODMAGIC_BOSS_OPERA_TYPE_ENTER =0,				// ���� param1 scene_id pararm2 boss_id
		GODMAGIC_BOSS_OPERA_TYPE_PLAYER_INFO,			// �����Ϣ 
		GODMAGIC_BOSS_OPERA_TYPE_GET_FLUSH_INFO,		// ˢ����Ϣ param1 ������Ϊ0��Ϊ���в㣩

		GODMAGIC_BOSS_OPERA_TYPE_MAX,
	};

	class CSGodmagicBossInfoReq											// ������ħboss��Ϣ   8910
	{
	public:
		CSGodmagicBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCGodmagicBossInfoAck											//  ��ħboss��Ϣ   8911
	{
	public:
		SCGodmagicBossInfoAck();
		MessageHeader	header;

		struct BossInfo
		{
			int boss_id;
			unsigned int next_flush_time;
		};

		struct SceneInfo
		{
			short layer;
			short left_treasure_crystal_count;
			short left_monster_count;
			short boss_count;
			BossInfo boss_list[MAX_GODMAGIC_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[GODMAGIC_BOSS_SCENE_MAX];
	};

	class SCGodmagicBossPlayerInfo												// ��ħBOSS�����Ϣ 8912
	{
	public:
		SCGodmagicBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// ʣ��Boss��
		short left_treasure_crystal_gather_times;							// ʣ����ϧˮ���ɼ�����
		int left_ordinary_crystal_gather_times;								// ʣ����ͨˮ���ɼ�����

	};

	class SCGodmagicBossSceneInfo												// ��ħboss������������Ϣ 8913
	{
	public:
		SCGodmagicBossSceneInfo();
		MessageHeader	header;

		struct CrossBossInfo
		{
			CrossBossInfo() : boss_id(0), is_exist(0), next_flush_time(0) {}

			int boss_id;
			int is_exist;
			unsigned int next_flush_time;
		};

		short left_monster_count;											// ʣ��С������
		short left_treasure_crystal_num;									// ʣ����ϧˮ������
		short layer;
		short treasure_crystal_gather_id;									// ��ϧˮ���ɼ���id
		unsigned int monster_next_flush_timestamp;							// С���´�ˢ��ʱ��
		unsigned int treasure_crystal_next_flush_timestamp;					// ��ϧˮ���´�ˢ��ʱ��
		CrossBossInfo boss_list[MAX_GODMAGIC_BOSS_PER_SCENE];					// Boss��Ϣ
	};
	
	/////////////////////////////////////////// ����boss ////////////////////////////////////
	class CSYunyouBossOperate						//8906
	{
	public:
		CSYunyouBossOperate();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_BOSS_INFO_REQ = 0,				// ����boss��Ϣ p0����id
			TYPE_BOSS_COUNT_ALL_SCENE = 1,		// ����boss���г���������Ϣ 
		};

		int operate_type;
		int param_0;
	};

	class SCYouyouBossInfo				//8907 ��������������Ϣ
	{
	public:
		SCYouyouBossInfo();
		MessageHeader header;

		struct BossInfo
		{
			int scene_id = 0;
			int boss_id = 0;
			int born_pos_x = 0;
			int born_pos_y = 0;
		};

		unsigned int nex_refresh_time;			// ��һ��ˢ��ʱ��
		int scene_id;

		int boss_count;
		BossInfo boss_info_list[YUNYOU_BOSS_MAX_NUM_PER_SCENE]; // boss��Ϣ
	};

	class SCYouyouSceneInfo				// 8908 ����bossȫ������������Ϣ
	{
	public:
		SCYouyouSceneInfo();
		MessageHeader header;

		struct SceneInfo
		{
			int scene_id = 0;
			int boss_count = 0;
		};

		int scene_count;
		SceneInfo scene_info_list[YUNYOU_BOSS_MAX_SCENE_NUM]; // boss��Ϣ
	};
}

#pragma pack(pop)

#endif


