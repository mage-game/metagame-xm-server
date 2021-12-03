#ifndef __MSG_ACTIVITY_HPP__
#define __MSG_ACTIVITY_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/roleactivitydef.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "global/roleboss/rolebossconfig.h"
#include "config/activityconfig/opengameactivityconfig.hpp"
#include "config/activityconfig/xingzuoyijiconfig.hpp"
#include "servercommon/magicalpreciousdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "gameworld/gameworld/config/activityconfig/randactivityconfig.hpp"
#include "other/fb/holidayguardconfig.hpp"

#pragma pack(push)
#pragma pack(4)

namespace Protocol
{
	class SCActivityStatus
	{
	public:
		SCActivityStatus();
		MessageHeader header;

		short activity_type;
		char status;
		char is_broadcast;
		unsigned int next_status_switch_time;
		unsigned int param_1;
		unsigned int param_2;
		unsigned int open_type;
	};

	class SCActivityRoomStatusAck
	{
	public:
		SCActivityRoomStatusAck();
		MessageHeader		header;

		int activity_type;

		int room_user_max;
		ActivityRoomStatus	room_status_list[ACTIVITY_ROOM_MAX];			// ���Ӧ�ķ�����Ϣ 
	}; 

	class CSActivityRoomStatusReq											// �����״̬����
	{
	public:
		CSActivityRoomStatusReq();
		MessageHeader	header;

		int	activity_type;													// �����
	};

	class CSActivityEnterReq
	{
	public:
		CSActivityEnterReq();
		MessageHeader header;

		int activity_type;
		int room_index;
	};

	class CSSendGuildSosReq
	{
	public:
		CSSendGuildSosReq();
		MessageHeader header;

		int sos_type;
	};

	class CSReplyGuildSosReq
	{
	public:
		CSReplyGuildSosReq();
		MessageHeader header;

		int sos_type;
		int pos_x;
		int pos_y;

		int scene_id;
	};

	class SCActivitySkillUseInfo
	{
	public:
		SCActivitySkillUseInfo();
		MessageHeader		header;

		int skill_id;
	};

	 /////////////////////////////////////////// ��аս����� ///////////////////////////////////////////

	 enum ZHUXIE_NOTIFY_TYPE																// ��аս��֪ͨԭ��
	 {	
		ZHUXIE_NOTIFY_TYPE_DEFAULT = 0,														// Ĭ������
		ZHUXIE_NOTIFY_TYPE_FETCH_TASKKREWARD = 1,											// ��ȡ������
		ZHUXIE_NOTIFY_TYPE_ENTER_SCENE = 2,													// ������аս��
	 };

	 class CSZhuXieFetchTaskReward															// ��ȴ��а������
	 {
	 public:
		 CSZhuXieFetchTaskReward();
		 MessageHeader header;

		 int task_id;																		// ����id
	 };

	 class SCZhuXieUserInfo
	 {
	 public:
		 SCZhuXieUserInfo();
		 MessageHeader header;

		 struct ClientTaskInfo
		 {
			 ClientTaskInfo() : task_id(0), param_value(0), max_value(0), is_fetch_reward(0) {}

			 short task_id;																	// ����ID
			 short param_value;																// ����ǰֵ
			 short max_value;																// ��������������ֵ
			 short is_fetch_reward;															// �Ƿ���ȡ����																	// ���齱��
		 };

		unsigned int boss_next_flush_time[ZHUXIE_BOSS_NUM];									// boss�´�ˢ��ʱ��
		ClientTaskInfo taskinfo_list[ZHUXIE_TASK_MAX];										// �û�������Ϣ
	 };

	 /////////////////////////////////////////// �콵�Ʊ���� ///////////////////////////////////////////

	 enum TIANJIANGCAIBAO_NOTIFY_TYPE																// �콵�Ʊ�֪ͨԭ��
	 {	
		TIANJIANGCAIBAO_NOTIFY_TYPE_DEFAULT = 0,													// Ĭ������
		TIANJIANGCAIBAO_NOTIFY_TYPE_FETCH_TASKKREWARD = 1,											// ��ȡ������
		TIANJIANGCAIBAO_NOTIFY_TYPE_ENTER_SCENE = 2,												// �����콵�Ʊ�
	 };

	 class CSTianJiangCaiBaoFetchTaskReward															// ��ȡ�콵�Ʊ�����
	 {
	 public:
		 CSTianJiangCaiBaoFetchTaskReward();
		 MessageHeader header;

		 int task_id;																		// ����id
	 };

	 class SCTianJiangCaiBaoUserInfo
	 {
	 public:
		 SCTianJiangCaiBaoUserInfo();
		 MessageHeader header;

		 unsigned int bigqianduoduo_reflush_time;											// ����Ǯ���ˢ��ʱ��
		 unsigned int qianduoduo_reflush_time;												// Ǯ����´�ˢ��ʱ��
		 int cur_qianduoduo_num;
		 int cur_bigqianduoduo_num;
		 int get_total_gold;
		 int is_finish;																		// ��Ƿ������1Ϊ�ǣ�0Ϊ����
		 int curr_task_id;
		 int curr_task_param;
		 int task_finish_num;

		 int reward_count;
		 RewardItemInfo reward_list[TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT];					// ��õĽ�����Ϣ
	 };

	 /////////////////////////////////////////// ����̽����� ///////////////////////////////////////////

	 class SCWangLingExploreUserInfo
	 {
	 public: 
		 SCWangLingExploreUserInfo();
		 MessageHeader header;

		 struct SClientTaskInfo
		 {
			 SClientTaskInfo() : task_id(0), is_finish(0), param_count(0), cur_param_value(0), is_double_reward(0){}

			 short task_id;																	// ����ID
			 short is_finish;																// �Ƿ���ȡ����

			 int param_count;																// ��������Ĳɼ������������
			 int cur_param_value;															// ��ǰֵ
			 int is_double_reward;
		 };

		 struct RewardItemInfo																// ��õ���Ʒ��Ϣ
		 {
			 RewardItemInfo() : item_id(0), num(0) {}

			 int item_id;
			 int num;
		 };

		 int boss_reflush_time;
		 int limit_task_time;
		 int boss_num;
		 int boss_owner_uid;
		 int gather_buff_time;

		 SClientTaskInfo taskinfo_list[WANGLINGEXPLORE_TASK_MAX];							// �û�������Ϣ
		 RewardItemInfo reward_list[WANGLINGEXPLORE_MAX_REWARD_ITEM_COUNT];					// ��õĽ�����Ϣ
	 };

	 class WangLingExploreBossInfo //6637
	 {
	 public:
		 WangLingExploreBossInfo();
		 MessageHeader header;

		 int monster_id;
		 long long max_hp;
		 long long cur_hp;
	 };

	 class CSWangLingExploreBuyBuff //6638
	 {
	 public:
		 CSWangLingExploreBuyBuff();
		 MessageHeader header;
	 };

	/////////////////////////////////////////////////  Ⱥ���Ҷ�  /////////////////////////////////////////////////////////
	enum QUNXIANLUANDOU_NOTIFY_REASON
	{
		QUNXIANLUANDOU_NOTIFY_REASON_DEFAULT = 0,										// Ĭ��
		QUNXIANLUANDOU_NOTIFY_REASON_WIN,												// ��ʤ
		QUNXIANLUANDOU_NOTIFY_REASON_LOSE,												// ʧ��
		QUNXIANLUANDOU_NOTIFY_REASON_DRAW,												// ƽ��
	};

	class SCQunXianLuanDouUserInfo														//4903 Ⱥ���Ҷ� �û���Ϣ
	{
	public:
		SCQunXianLuanDouUserInfo();
		MessageHeader		header;

		int notify_reason;																// ֪ͨԭ��

		short side;																		// ��ɫ��ս���е���Ӫ	
		short kills;																	// ɱ�˴���
		short lianzhan;																	// ��ɱ����
		short assists;																	// ��������
		short rob_shenshi;																// ��ʯ����
		unsigned short realive_guard_monsterid;											// �������������id;

		int kill_honor;																	// ��ɱ����
		int assist_honor;																// ��������
		int rob_shenshi_honor;															// ��ʯ����
		int free_reward_honor;															// �����������
		unsigned int last_realive_here_timestamp;										// ���һ��ԭ�ظ���ʱ��
		int extra_honor;																// ���⽱��

		GameName first_rank_num[QUNXIANLUANDOU_SIDE_MAX];
		int first_rank_uid[QUNXIANLUANDOU_SIDE_MAX];
	};

	class SCQunXianLuanDouRankInfo		// Ⱥ���Ҷ� ���а���Ϣ  4904
	{
	public:
		SCQunXianLuanDouRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : uid(0), score(0), side(0)
			{
				memset(name, 0, sizeof(name));
			}

			int uid;
			GameName name;
			int score;
			int side;
		};

		int self_score;
		int self_rank_pos;
		int count;																		// Ⱥ���Ҷ� ���а���Ϣ����
		RankItem rank_list[QUNXIANLUANDOU_RANK_NUM];									// Ⱥ���Ҷ� ���а���Ϣ�б�
	};

	class SCQunXianLuanDouSideInfo														// Ⱥ���Ҷ� ��Ӫ��Ϣ
	{
	public:
		SCQunXianLuanDouSideInfo();
		MessageHeader		header;

		int score[QUNXIANLUANDOU_SIDE_MAX];												// Ⱥ���Ҷ� ��Ӫ����
		unsigned int shenshi_next_refresh_time;											// ��ʯ�´�ˢ��ʱ�䣬0���Ѿ�����
	};

	class SCQunXianLuanDouShenShiStatus													// ��ʯ״̬
	{
	public:
		SCQunXianLuanDouShenShiStatus();												
		MessageHeader		header;

		int has_owner;																	// �Ƿ���ӵ����
		ObjID obj_id;																	// ��ʯobjid
		ObjID owner_obj_id;																// ӵ����objid
		int owner_uid;																	// ӵ����user id
	};

	class CSBattleFieldHonorInfoReq														// ����ս������ֵ												
	{
	public:
		CSBattleFieldHonorInfoReq();																										
		MessageHeader		header;
	};

	class SCBattleFieldHonorChange														// ս������ֵ�ı�
	{
	public:
		SCBattleFieldHonorChange();																										
		MessageHeader		header;

		int honor;																		// ��ǰ����ֵ
		int delta_honor;																// �����ı���ֵ
	};

	class CSQunXianLuanDouPickUpShenShi													// ������ʯ����
	{
	public:
		CSQunXianLuanDouPickUpShenShi();												
		MessageHeader		header;

		int shenshi_objid;																// ��ʯobjid
	};

	class SCQunxianluandouLianzhanChange												// ��ն���ı�
	{
	public:
		SCQunxianluandouLianzhanChange();												
		MessageHeader		header;

		ObjID obj_id;																	// ����id
		short lianzhan;																	// ��ն��
	};

	class SCQunxianluandouLuckyRewardInfo  // 4950
	{
	public:
		SCQunxianluandouLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	class CSQunxianLuandouFirstRankReq				// 4945 ����Ⱥ���Ҷ����ߵ�һ����
	{
	public:
		CSQunxianLuandouFirstRankReq();
		MessageHeader header;
	};

	class SCQunxianLuandouFirstRankInfo				// 4946 �·�Ⱥ���Ҷ����ߵ�һ����
	{
	public:
		SCQunxianLuandouFirstRankInfo();
		MessageHeader header;

		GameName first_rank_num[QUNXIANLUANDOU_SIDE_MAX];
		int first_rank_uid[QUNXIANLUANDOU_SIDE_MAX];
	};

	class CSFetchGuildBattleDailyReward				// 4975 ��������ÿ�ս�����������
	{
	public:
		CSFetchGuildBattleDailyReward();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_INFO_REQ = 0,						// ������Ϣ
			TYPE_FETCH_REQ = 1,						// ��ȡ����
		};

		int operate_type;
	};

	class SCSendGuildBattleDailyRewardFlag			// 4976 �·��������Խ�����Ϣ
	{
	public:
		SCSendGuildBattleDailyRewardFlag();
		MessageHeader header;

		short had_fetch;							//�Ƿ���ȡ��0��1��
		char rank;									//����
		char reserve_ch;
	};

	/////////////////////////////////////////////////   ������Ϣͳ��  //////////////////////////////////////////////////////
	class CSGuildInfoStatisticReq		//6690
	{
	public:
		CSGuildInfoStatisticReq();
		MessageHeader header;

		int activity_type;
	};

	class SCGuildInfoStatistic			//6691
	{
	public:
		SCGuildInfoStatistic();
		MessageHeader header;

		struct InfoItem
		{
			long long uid;
			GameName user_name;
			int guild_post;
			short kill_role;		//ɱ����
			short kill_target;		//��ɱĿ�����
			long long hurt_roles;	//���������˺�
			long long hurt_targets;	//��Ŀ������˺�
		};

		enum 
		{
			NOTIFY_ACTIVITY_CLOSE = 0, // ���������
			NOTIFY_ACTIVITY_REQ = 1,   // ���󷵻�
		};

		int notify_type;  // ֪ͨ����
		int activity_type;
		int guild_id;
		int count;
		InfoItem info_list[MAX_MEMBER_COUNT];
	};

	class SCGuildMvpInfo			//6692
	{
	public:
		SCGuildMvpInfo();
		MessageHeader header;
		
		int activity_type;
		long long uid;
		GameName user_name;
	};

	/////////////////////////////////////////////////   ����ս���  /////////////////////////////////////////////////////////

	enum GCZ_GONG_SHOU_ZIYUAN_PLACE
	{
		GCZ_GOTO_GONG_PLACE = 0,					// ����ص�
		GCZ_GOTO_SHOU_PLACE = 1,					// ����ص�
		GCZ_GOTO_ZIYUAN_PLACE = 2,					// ��Դ���ص�
	};

	class CSGCZChangePlace							// ͨ����ť�ı��ɫ��λ��
	{
	public:
		CSGCZChangePlace();
		MessageHeader header;

		int place_type;													
	};

	class SCGCZRewardInfo							// ��������
	{
	public:
		SCGCZRewardInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 4;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0) {}
			ItemID reward_item_id;
			short reward_item_num;
		};

		int honor_reward;
		int gold_reward;
		int shengwang_reward;
		int chengzhu_uid; 
		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};

	class SCGCZGlobalInfo					// ����սȫ����Ϣ   6701
	{
	public:
		SCGCZGlobalInfo();
		MessageHeader	header;

		char is_finish;
		char is_pochen;
		char is_poqiang;
		char cheng_zhu_prof;
		unsigned int current_shou_cheng_time;
		GuildID shou_guild_id;
		GuildName shou_guild_name;
		int totem_level;
		int po_cheng_times;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				shouchen_time = 0;
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;
			unsigned int shouchen_time;
			GuildName guild_name;
		};

		static const int RANK_NUM = 5;

		int self_guild_rank_pos;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGCZRoleInfo								// ��ɫ��Ϣ
	{
	public:
		SCGCZRoleInfo();
		MessageHeader		header;
		
		char is_shousite;
		char sos_times;
		short reserve_2;
		int zhangong;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				role_id = 0;
				zhangong = 0;
				memset(role_name, 0, sizeof(role_name));
			}

			int role_id;
			GameName role_name;
			int zhangong;
		};

		static const int RANK_NUM = 5;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGongChengZhanOwnerInfo					// ������Ϣ
	{
	public:
		SCGongChengZhanOwnerInfo();
		MessageHeader		header;

		int owner_id;
		GameName owner_name;
		char vip_level;
		char sex;
		char prof;
		char reserve_ch;
		int guild_id;
	};

	class SCGongChengZhanFlagInfo //6706
	{
	public:
		SCGongChengZhanFlagInfo();
		MessageHeader header;

		long long max_hp;
		long long cur_hp;
	};

	class CSGCZFetchDailyRewardReq
	{
	public:
		CSGCZFetchDailyRewardReq();
		MessageHeader		header;
	};

	class SCGczLuckyRewardInfo  // 6753
	{
	public:
		SCGczLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	class SCGCZWorshipInfo		//6754
	{
	public:
		SCGCZWorshipInfo();
		MessageHeader header;

		int worship_time;										//���Ĥ�ݴ���
		unsigned int next_worship_timestamp;					//����´�Ĥ��ʱ���
		unsigned int next_interval_addexp_timestamp;			//����´μӾ���ʱ���
	};

	class SCGCZWorshipActivityInfo				//6755
	{
	public:
		SCGCZWorshipActivityInfo();
		MessageHeader header;

		char worship_is_open;							// Ĥ�ݻ�Ƿ���
		char reserve_ch;								// ����
		short reserve_sh;								// ����
		unsigned int worship_end_timestamp;				// Ĥ�ݻ����ʱ���
	};

	class CSGCZWorshipReq		//6756
	{
	public:
		CSGCZWorshipReq();
		MessageHeader header;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCRoleWorshipInfo		// 6757 �·�����Ĥ����Ϣ
	{
	public:
		SCRoleWorshipInfo();
		MessageHeader header;

		int activity_type;							// �����
		int worship_times;							// ���Ĥ�ݴ���
		unsigned int next_worship_timestamp;		// ����´�Ĥ��ʱ���
	};

	class SCRoleWorshipStart		// 6758 �㲥��ɫ��ʼĤ�ݶ���
	{
	public:
		SCRoleWorshipStart();
		MessageHeader header;

		ObjID role_obj;
		short resever_sh;
	};

	class CSRoleWorshipReq			// 6759 Ĥ�ݵ��
	{
	public:
		CSRoleWorshipReq();
		MessageHeader header;
	};

	/////////////////////////////////////////////////   �����������  /////////////////////////////////////////////////////////
	class CSGBAddHuDun								// 6200��������  �����ͷŻ���
	{
	public:
		CSGBAddHuDun();
		MessageHeader		header;
	};

	class SCGBGlobalInfo							// 6251�������� ȫ����Ϣ���㲥��
	{
	public:
		SCGBGlobalInfo();
		MessageHeader		header;

		int self_guild_score;
		int self_guild_rank;
		char is_finish;
		char is_boss_alive;
		short reserve_2;
		unsigned husong_end_time;
		short glod_box_total_num;
		short sliver_box_total_num;
		int boss_curren_hp;
		int boss_max_hp;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				score = 0;
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;
			int score;
			GuildName guild_name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGBRoleInfo								// 6252�������� ������Ϣ
	{
	public:
		SCGBRoleInfo();
		MessageHeader		header;


		short kill_role_num;
		short husong_appearance;
		int history_get_person_credit;
		int history_get_guild_credit;
		int husong_gooods_id;
		char is_add_hudun;
		char reserve_1;
		short reserve_2;
	};

	class CSGBRoleCalcSubmitReq						// 6201�ύ������������
	{
	public:
		CSGBRoleCalcSubmitReq();
		MessageHeader		header;
	};

	class SCGBBianShenView							// 6250��������㲥
	{
	public:
		SCGBBianShenView();
		MessageHeader header;

		ObjID obj_id;
		short bianshen_image;

	};

	class CSGBWinnerInfoReq							// 6202������һ�칫�����԰�����Ϣ
	{
	public:
		CSGBWinnerInfoReq();
		MessageHeader header;
	};

	class SCGBSendWinnerInfo						// 6253������һ�������Ϣ
	{
	public:
		SCGBSendWinnerInfo();
		MessageHeader header;

		GuildID guild_id;
	};

	class SCGBLuckyRewardInfo  // 6254
	{
	public:
		SCGBLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	/////////////////////////////////////////////////   �¹�������   ///////////////////////////////////////////////////////
	class SCGBGlobalInfoNew							// 6255 �¹������� ȫ����Ϣ���㲥��
	{
	public:
		SCGBGlobalInfoNew();
		MessageHeader		header;

		struct PointItem
		{
			PointItem()
			{
				this->Reset();
			}
			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(GameName));
				blood = 0;
				max_blood = 0;
			}

			int guild_id;
			GameName guild_name;
			long long blood;
			long long max_blood;
		};

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				score = 0;
				memset(guild_name, 0, sizeof(guild_name));
				tuangzhang_uid = 0;
			}

			GuildID guild_id;
			int score;
			GuildName guild_name;
			int tuangzhang_uid;
		};

		static const int RANK_NUM = 10;
		int self_guild_score;
		int self_guild_rank;
		char is_finish;
		char reserve1;
		short reserve2;
		int rank_count;
		RankItem rank_list[RANK_NUM];
		PointItem hold_point_guild_list[GUILD_BATTLE_NEW_POINT_NUM];
	};

	class SCGBRoleInfoNew								// 6256 �¹������� ������Ϣ
	{
	public:
		SCGBRoleInfoNew();
		MessageHeader		header;

		int kill_role_num;
		int history_get_person_credit;
		char is_add_hudun;
		char sos_times;
		short reserve2;
	};

	/////////////////////////////////////////////////   ����ս���  /////////////////////////////////////////////////////////

	static const int TERRITORYWAR_SKILL_COUNT_MAX = 2;

	class SCTerritoryWarRoleInfo					// 5952 ������Ϣ		
	{
	public:
		SCTerritoryWarRoleInfo();
		MessageHeader header;

		int current_credit;					// ��ǰӵ�л���
		int history_credit;					// ��ʷ��û���
		int credit_reward_flag;				// ���ֽ�����־λ
		short kill_count;					// ��ɱ�����
		short assist_count;					// ������ɱ�����
		short death_count;					// ���������
		short side;							// 1(��)��0(��)
		char special_image_id;				// ���򹥳ǳ�����
		char reserve_1;
		short reserve_2;
		short ice_landmine_count;			// ӵ�еı����������
		short fire_landmine_count;			// ӵ�еĻ����������

		struct SkillInfo
		{
			SkillInfo():skill_index(0), last_perform_time(0){}
			
			int skill_index;
			unsigned int last_perform_time;
		};
		
		SkillInfo skill_list[TERRITORYWAR_SKILL_COUNT_MAX];
	};

	class SCTerritoryWarApperance					// 5950 ��������㲥
	{
	public:
		SCTerritoryWarApperance();
		MessageHeader header;

		ObjID obj_id;
		short special_image;

	};

	class SCTerritoryWarGlobeInfo					// 5951 ȫ����Ϣ���㲥��
	{
	public:
		SCTerritoryWarGlobeInfo();
		MessageHeader		header;

		int red_guild_credit;
		int blue_guild_credit;
		short center_relive_side;
		short reserve;
		
		long long red_fortress_max_hp;
		long long red_fortress_curr_hp;
		long long blue_fortress_max_hp;
		long long blue_fortress_curr_hp;
		long long center_relive_max_hp;
		long long center_relive_curr_hp;

		int red_building_survive_flag;		// �췽�������״̬��1��ʾ��������0��ʾ��1-12λ��ʾ
		int blue_building_survive_flag;		// �����������״̬��1��ʾ��������0��ʾ��1-12λ��ʾ

		unsigned int read_next_can_buy_tower_wudi;	// ���޵���ȥʱ��
		unsigned int blue_next_can_buy_tower_wudi;

		char winner_side;					// ʤ��һ����-1��ʾû��ʤ������1�췽ʤ����0����ʤ��
		char reserve_1;
		short reserve_2;
	};

	class CSTerritoryWarReliveShopBuy				// 5900 �����̵㹺��
	{
	public:
		CSTerritoryWarReliveShopBuy();
		MessageHeader header;

		short goods_id;
		short reserve;
	};

	class CSTerritoryWarReliveFightBuy				// 5901 ս���̵깺��
	{
	public:
		CSTerritoryWarReliveFightBuy();
		MessageHeader header;

		short type;
		short goods_id;
	};

	class CSTerritoryWarQualification				// 5902
	{
	public:
		CSTerritoryWarQualification();
		MessageHeader header;
	};

	static const int CAN_JOIN_MAX_RANK = 10;
	class SCTerritoryWarQualification				// 5953
	{
	public:
		SCTerritoryWarQualification();
		MessageHeader header;

		GuildID m_guildbattel_rank_list[CAN_JOIN_MAX_RANK];
		GuildID m_territorywar_rank_list[CAN_JOIN_MAX_RANK];
	};

	class CSTerritorySetLandMine					// 5903
	{
	public:
		CSTerritorySetLandMine();
		MessageHeader header;

		int landmine_type; // 0������� ��1�������
		Axis pos_x;						
		Axis pos_y;						
	};

	class SCTwLuckyRewardInfo  // 5954
	{
	public:
		SCTwLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	/////////////////////////////////////////////////  ��Ȫ  /////////////////////////////////////////////////////////

	class SCHotspringPlayerInfo					// 5352 ��Ȫ�������Ϣ
	{
	public:
		SCHotspringPlayerInfo();
		MessageHeader		header;

		int popularity;								// �������ֵ
		long long partner_uuid;
		short give_free_present_time;
		short swinsuit_image;
		long long self_uuid;

		ObjID partner_obj_id;							
		char question_right_count;					// ��ȷ����
		char question_wrong_count;					// ��������
		int curr_score;								// ��û���
		//int total_exp;								// ��þ���
		long long total_exp;							// ��þ���

		short skill_use_times_1;					// ��Ħ����ʹ�ô���
		short skill_use_times_2;					// ��ѩ����ʹ�ô���
		unsigned int skill_1_can_perform_time;		// ��Ħ���ܿ�ʹ��ʱ���
		unsigned int skill_2_can_perform_time;		// ��ѩ���ܿ�ʹ��ʱ���
		short gather_times;							// �ɼ�����
		short reserve_sh;
		unsigned int start_fresh_gather_time;		// �ɼ��￪ʼˢ��ʱ��

	};

	class SCHotspringRankInfo					// 5353��Ȫ���������Ϣ
	{
	public:
		SCHotspringRankInfo();
		MessageHeader header;

		
		int self_popularity;
		int self_rank;
		char is_open;
		char reserve_1;
		short reserve_2;

		struct RankList
		{
			RankList()
			{

			}

			void Reset()
			{
				uuid = 0;
				popularity = 0;
				memset(name,0,sizeof(GuildName));
			}

			long long uuid;
			int uid;
			int popularity;
			GameName name;
		};

		static const int RANK_LIST = 100;
		int rank_count;
		RankList rank_list[RANK_LIST];
	};

	class CSHotspringGivePresent				// 5300����������
	{
	public:
		CSHotspringGivePresent();
		MessageHeader	header;

		long long target_uuid;
		int present_id;
		short is_use_gold;						// 0������1����
		short is_uid;							// 0���ǣ�1��
	};

	class CSHSAddPartnerReq						// 5301��ӻ������
	{
	public:
		CSHSAddPartnerReq();
		MessageHeader		header;

		ObjID 		obj_id;
		short		is_yi_jian;
	};

	class SCHSAddPartnerReqRoute				// 5354ѯ�ʱ���ӻ��Ķ���								
	{
	public:
		SCHSAddPartnerReqRoute();
		MessageHeader		header;

		GameName			req_gamename;
		long long  			req_user_id;
		char				req_avatar;
		char				req_sex;
		char				req_prof;
		char				req_camp;
		int					req_level;
		long long			req_avatar_timestamp;
	};

	class CSHSAddPartnerRet						// 5302����ӻ�����������������
	{
	public:
		CSHSAddPartnerRet();
		MessageHeader		header;

		long long	 		req_user_id;
		GameName			req_gamename;
		char				is_accept;
		char				reserved;
		char				req_sex;
		char				req_prof;
	};

	class CSHSDeleteParter						// 5303ȡ���������							
	{
	public:
		CSHSDeleteParter();
		MessageHeader		header;
	};

	class SCHSSendPartnerInfo					// 5350���ͻ����Ϣ
	{
	public:
		SCHSSendPartnerInfo();
		MessageHeader		header;

		long long	 partner_uuid;    // ���id
		ObjID partner_obj_id;
	};

	enum AddPartnerStatus
	{
		ADDPARTNER_REJECT = 0,					// �ܾ�
		ADDPARTNER_AGREE = 1,					// ͬ��
	};

	class SCHSAddExpInfo						// 5351 ������ʷ������Ϣ								
	{
	public:
		SCHSAddExpInfo();
		MessageHeader		header;

		//int add_exp_total;
		long long add_exp_total; 
		int add_addition;  
	};

	static const int HOTSPRINT_MSG_MAX_SHUANGXIU_COUNT = 512;			// ��Ȫ˫�޶������֪ͨ�Ķ�����һ���ò�����ô�࣬�����˿�����չ��

	class SCHSShuangxiuInfo						// 5355�㲥������Ȫ˫����Ϣ
	{
	public:
		SCHSShuangxiuInfo();
		MessageHeader		header;

		ObjID role_1_obj_id;	
		ObjID role_1_partner_obj_id;
		ObjID role_2_obj_id;
		ObjID role_2_partner_obj_id;
	};

	class SCHSQARankInfo						// 5356 ��Ȫ����������Ϣ
	{
	public:
		SCHSQARankInfo();
		MessageHeader		header;

		int self_score;
		int self_rank;
		char is_finish;
		char reserve_1;
		short reserve_2;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uuid = 0;
				uid = 0;
				score = 0;
				memset(name,0,sizeof(GameName));
			}

			long long uuid;
			int uid;
			int score;
			GameName name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCHSQAQuestionBroadcast						// 5357���ʹ�������
	{
	public:
		SCHSQAQuestionBroadcast();
		MessageHeader header;

		unsigned int curr_question_begin_time;			// ���⿪ʼʱ��
		unsigned int curr_question_end_time;			// �������ʱ��
		unsigned int next_question_begin_time;			// ��һ�⿪ʼʱ��

		short broadcast_question_total;					// �ѹ㲥��Ŀ����
		short curr_question_id;							// ��ǰ����id
		int side;										// �ͻ������������һ����ȷ
		QuestionStr curr_question_str;					// ��ǰ��Ŀ�ַ���
		QuestionStr curr_answer0_desc_str;				// ��ǰ��0�ַ���
		QuestionStr curr_answer1_desc_str;				// ��ǰ��1�ַ���
	};

	class CSHSQAFirstPosReq								// 5304�������λ��
	{
	public:
		CSHSQAFirstPosReq();
		MessageHeader header;

	};

	class SCHSQASendFirstPos							// 5358���ذ���λ��
	{
	public:
		SCHSQASendFirstPos();
		MessageHeader header;

		ObjID obj_id;
		short reserve;
		int pos_x;
		int pos_y;
	};

	class CSHSQAAnswerReq								// 5305��Ҵ�������
	{
	public:
		CSHSQAAnswerReq();
		MessageHeader header;

		short is_use_item;
		short choose;
	};

	class SCHSQAnswerResult								// 5359������һش���						
	{
	public:
		SCHSQAnswerResult();
		MessageHeader header;

		int result;										// 1 �ش���ȷ��0 �ش����
		int right_result;								// ��ȷ��
	};

	class CSHSQAUseCardReq								// 5306�������ʹ�ñ���
	{
	public:
		CSHSQAUseCardReq();
		MessageHeader header;

		int target_uid;
	};

	class CSHSUseSkillReq								// 5360�������ʹ�ü���
	{
	public:
		CSHSUseSkillReq();
		MessageHeader header;

		ObjID obj_id;
		short skill_type;
	};

	class SCHSNoticeSkillInfo							// 5361���ʹ�ü��ܶ�����Ϣ
	{
	public:
		SCHSNoticeSkillInfo();
		MessageHeader header;

		ObjID use_obj_id;
		ObjID be_use_obj_id;
		short skill_type;
		short reserve;
	};

	/////////////////////////////////////  �������ս  /////////////////////////////////////
	class SCCrossGuildBattleSceneInfo			// 5373
	{
	public:
		struct FlagItem
		{
			int monster_id;
			int plat_type;
			int server_id;
			GuildName guild_name;
			Attribute cur_hp;
			Attribute max_hp;
		};

		struct RankItem
		{
			int plat_type;
			int server_id;
			GuildName guild_name;
			int score;
			int own_num;
		};

		SCCrossGuildBattleSceneInfo();
		MessageHeader		header;

		int scene_id;
		FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE];
		int guild_join_num_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
		int rank_list_count;
		RankItem rank_list[CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM];
	};

	enum SC_CROSS_GUILDBATTLE_INFO_TYPE
	{
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SCORE = 0,			// ���˻���
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_END,				// ����
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SOS,				// �ټ� param2 ����
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_ENTER,				// ���볡��
	};

	class SCCrossGuildBattleNotifyInfo					// 5374
	{
	public:
		SCCrossGuildBattleNotifyInfo();
		MessageHeader		header;

		int notify_type;
		int param_1;
		int param_2;
	};

	class SCCrossGuildBattleInfo						// 5375
	{
	public:
		SCCrossGuildBattleInfo();
		MessageHeader header;

		int server_reward_flag;
		int guild_reward_flag;
		int role_reward_flag;
		int daily_reward_flag;
		struct GuildItem
		{
			int plat_type;
			int server_id;
			int guild_id;
			int is_our_guild;		// �Ƿ񱾰���ռ��: 1->��; 0->��
			GuildName guild_name;
			GameName guild_tuanzhang_name;
			char prof;
			char sex;
			ItemID wuqi_id;              // ����ID
			char shizhuang_wuqi;          // ʱװ����
			char shizhuang_wuqi_is_special;      // ʱװ����
			char shizhuang_body;          // ʱװ����
			char shizhuang_body_is_special;      // ʱװ����
			short halo_used_imageid;        // �⻷����
			short toushi_used_imageid;        // ͷ������id
			short qilinbi_used_imageid;        // ���������id
			short mask_used_imageid;        // �������id
		};

		GuildItem guild_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	enum CROSS_GUILDBATTLE_OPERATE
	{
		CROSS_GUILDBATTLE_OPERATE_REQ_INFO,
		CROSS_GUILDBATTLE_OPERATE_FETCH_REWARD,
		CROSS_GUILDBATTLE_OPERATE_REQ_TASK_INFO,
		CROSS_GUILDBATTLE_OPERATE_BOSS_INFO,
		CROSS_GUILDBATTLE_OPERATE_SCENE_RANK_INFO,	// param1 ����id
		CROSS_GUILDBATTLE_OPERATE_GOTO_SCENE,		// param1 ��������
		CROSS_GUILDBATTLE_OPERATE_GET_DAILY_REWARD, // ��ȡռ���ÿ�콱��
	};

	class CSCrossGuildBattleOperate				// 5370
	{
	public:
		CSCrossGuildBattleOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
	};

	class CSCrossGuildBattleGetRankInfoReq		// 5371, �����ÿ������ս���а���
	{
	public:
		CSCrossGuildBattleGetRankInfoReq();
		MessageHeader header;
	};

	class SCCrossGuildBattleGetRankInfoResp		// 5372, ���ػ�ÿ������ս���а���
	{
	public:
		SCCrossGuildBattleGetRankInfoResp();
		MessageHeader header;

		enum SendType
		{
			ST_INVALID = 0,
			ST_ACT_END = 1,
			ST_ROLE_REQ = 2
		};

		int	send_type;	// 0. ��Ч; 1. �����ʱ�·�; 2. �����������
		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleTaskInfo			// 5376
	{
	public:
		SCCrossGuildBattleTaskInfo();
		MessageHeader		header;

		int task_finish_flag[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
		int task_record[CROSS_GUILDBATTLE_MAX_SCENE_NUM][CROSS_GUILDBATTLE_MAX_TASK_NUM];
	};

	class SCCrossGuildBattleBossInfo		// 5377
	{
	public:
		SCCrossGuildBattleBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		char is_refresh;
		char reserve_ch;
		short reserve_sh;

		struct BossInfo
		{
			int boss_id;
			int replace_boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		int count;
		BossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCCrossGuildBattleGetMonsterInfoResp		// 5379, ���ػ�ÿ������ս��Ӣ����Ϣ
	{
	public:
		SCCrossGuildBattleGetMonsterInfoResp();
		MessageHeader header;

		int scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleFlagInfo // 5378 �������ս������������Ϣ
	{
	public:
		SCCrossGuildBattleFlagInfo();
		MessageHeader header;

		struct FlagItem
		{
			int monster_id;
			int plat_type;
			int server_id;
			GuildName guild_name;
			Attribute cur_hp;
			Attribute max_hp;
		};

		struct SceneFlagInfo
		{
			int scene_id;
			FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE];
		};

		SceneFlagInfo flag_info[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleBossHurtInfo	//5369 �������ս��BOSS�˺���Ϣ
	{
	public:
		SCCrossGuildBattleBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//�Լ���������
		long long own_guild_hurt;//�Լ������˺�
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class SCCrossGuildBattleSceneGuilderNum    // 5368 �������ս��������������
	{
	public:
		SCCrossGuildBattleSceneGuilderNum();
		MessageHeader header;
		struct SceneInfo
		{
			SceneInfo():scene_id(0),guilder_num(0)
			{}
			int scene_id;
			int guilder_num;
		};

		long long guild_id;
		SceneInfo scene_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};
	/////////////////////////////////////////////////  ����&�������  /////////////////////////////////////////////////////////
	class SCQuestionBroadcast						// 5380���ʹ�������
	{
	public:
		SCQuestionBroadcast();
		MessageHeader header;

		short question_type;							// ��Ŀ���� 2 ���� 3����
		short curr_question_id;							// ��ǰ����id
		QuestionStr curr_question_str;					// ��ǰ��Ŀ�ַ���
		QuestionStr curr_answer0_desc_str;				// ��ǰ��0�ַ���
		QuestionStr curr_answer1_desc_str;				// ��ǰ��1�ַ���
		QuestionStr curr_answer2_desc_str;				// ��ǰ��2�ַ���
		QuestionStr curr_answer3_desc_str;				// ��ǰ��3�ַ���

		unsigned int curr_question_begin_time;			// ���⿪ʼʱ��
		unsigned int curr_question_end_time;			// �������ʱ��
		unsigned int next_question_begin_time;			// ��һ�⿪ʼʱ��
	};

	class CSQuestionAnswerReq							// 5381��Ҵ�������
	{
	public:
		CSQuestionAnswerReq();
		MessageHeader header;

		short answer_type;								// �ش����� 2���� 3����
		short choose;									// ѡ���
	};

	class SCQuestionAnswerResult						// 5382������һش���						
	{
	public:
		SCQuestionAnswerResult();
		MessageHeader header;

		short answer_type;								// ������� 2���磬 3����
		short reserve;
		short result;									// 1 �ش���ȷ��0 �ش����
		short right_result;								// ��ȷ��
	};

	class SCQuestionGuildRankInfo						// 5383�������������Ϣ
	{
	public:
		SCQuestionGuildRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uid = 0;
				right_answer_num = 0;
				memset(name, 0, sizeof(GameName));
			}

			int uid;
			int right_answer_num;
			GameName name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCQuestionRightAnswerNum						// 5384������һش���						
	{
	public:
		SCQuestionRightAnswerNum();
		MessageHeader header;

		int world_right_answer_num;								
		int guild_right_answer_num;
	};

	/////////////////////////////////////////////////  ����ս  /////////////////////////////////////////////////////////

	enum XIANMMENGZHAN_USER_INFO_NOTIFY_REASON
	{
		XIANMENGZHAN_USER_INFO_NOTIFY_REASON_DEFAULT = 0,								// Ĭ��
		XIANMENGZHAN_USER_INFO_NOTIFY_REASON_CALL_SUCCESS,								// �����ٻ��ɹ�
	};

	class SCXianMengZhanUserInfo
	{
	public:
		SCXianMengZhanUserInfo();
		MessageHeader		header;

		char                notify_reason;												// ֪ͨԭ��
		char                call_count;													// ���ٻ�����
		char                call_allow;													// �Ƿ���ٻ�
		char                reserve_ch;													// ����
		short               score;														// ����
		short               kill_count;													// ��ɱ��
		unsigned int		last_realive_here_timestamp;								// �ϴ�ԭ�ظ���ʱ��
		short				lianzhan;													// ��ն
		short				reserve_sh;													// ����
		int					assist_count;												// ������
	};

	class SCXianMengZhanGuildInfo
	{
	public:
		SCXianMengZhanGuildInfo();
		MessageHeader		header;

		int					score;														// ����
		int                 area_index;													// �ݵ�����
		unsigned int        last_call_time;												// ��һ���ٻ�ʱ��
	};

	class SCXianMengZhanDefendAreaInfo
	{
	public:
		SCXianMengZhanDefendAreaInfo();
		MessageHeader		header;

		struct DefendAreaItem
		{
			GuildName       guild_name;													// ��������
			short           guild_score;												// ���˻���
			char            area_index;													// �ݵ�����
			char            has_center_area;											// ����
			int				guild_id;													// ռ������ID
			char			camp;														// ������Ӫ
			char			reserve_ch;													// ����
			short			reserve_sh;													// ����
		};

		int					center_area_guild_id;										// ����ݵ�����ID
		GuildName			center_area_guild_name;										// ����ݵ�������
		int					center_area_guild_score;									// ���ľݵ����˻���
		char				center_area_guild_camp;										// ������Ӫ
		char				reserve_ch;													// ����
		short				reserve_sh;													// ����

		int                 area_count;													// �б��С
		DefendAreaItem      defend_area_list[XIANMENGZHAN_DEFEND_AREA_NUM];             // �ݵ��б�
	};

	class SCXianMengZhanGuildRankInfo
	{
	public:
		SCXianMengZhanGuildRankInfo();
		MessageHeader		header;

		const static int GUILD_RANK_ITEM_MAX_COUNT = 10;

		struct GuildRankItem
		{
			GuildName       guild_name;													// ��������
			int				guild_id;													// ռ������ID
			int				guild_score;												// ���˻���
			char			camp;														// ��Ӫ
			char			reserve_ch;													// ����
			short			reserve_sh;													// ����
		};

		int rank_count;
		GuildRankItem rank_item_list[GUILD_RANK_ITEM_MAX_COUNT];
	}; 

	class SCXianMengZhanDanyaoBossInfo
	{
	public:
		SCXianMengZhanDanyaoBossInfo();
		MessageHeader	   header;

		static const int   MAX_DANYAO_BOSS_NUM = 50;

		struct DanyaoBoss
		{
			unsigned short danyao_boss_obj_id;
			short		   reserved;
			int			   guild_id;
			GuildName	   guild_name;
		};

		int				   boss_count;
		DanyaoBoss		   danyao_boss_info_list[MAX_DANYAO_BOSS_NUM];
	};

	class SCXianMengZhanGuildCallNotice
	{
	public:
		SCXianMengZhanGuildCallNotice();
		MessageHeader       header;

		GameName            caller_name;												//��������
	};

	class SCXianMengZhanDefendAreaBeAttackNotice
	{
	public:
		SCXianMengZhanDefendAreaBeAttackNotice();
		MessageHeader		header;
	};

	class SCXianMengZhanResult
	{
	public:
		SCXianMengZhanResult();
		MessageHeader		header;

		int					guild_rank;
		int					guild_score;
		int					kill_score;
		int					total_score;
		int					assist_count;
	};

	class CSXianMengZhanGuildCall
	{
	public:
		CSXianMengZhanGuildCall();
		MessageHeader		header;

		int                use_gold;												//�Ƿ�ʹ��Ԫ��
	};

	class CSXianMengZhanGuildFollow
	{
	public:
		CSXianMengZhanGuildFollow();
		MessageHeader		header;
	};

	class SCXianmengzhanLianzhanChange												// ��ն���ı�
	{
	public:
		SCXianmengzhanLianzhanChange();												
		MessageHeader		header;

		ObjID obj_id;																	// ����id
		short lianzhan;																	// ��ն��
	};

	/////////////////////////////////////////////////  �����ż�  /////////////////////////////////////////////////////////

	class SCXingzuoYijiChangeBoxAndBoss													// ����������boss�����ı� 4940
	{
	public:
		SCXingzuoYijiChangeBoxAndBoss();												
		MessageHeader		header;

		short now_box_num;																// ʣ�౦������
		short now_boss_num;																// boss����												
		short gather_box_num[XingzuoYiJiConfig::MAX_GRADE_BOX];							// 4���Ѳɼ���������
		unsigned int next_box_refresh_time;												// �´�����ˢ��ʱ��
		unsigned int next_boss_refresh_time;											// �´�bossˢ��ʱ��
		int can_gather_num;																// ʣ��ɼ�����
	};

	/////////////////////////////////////////////////  ��ֵ�  /////////////////////////////////////////////////////////
	enum ZAI_CHONGZHI_STATE
	{
		ZAI_CHONGZHI_STATE_INVALID = 0,													// �ٳ�ֵ δ�ﵽҪ��
		ZAI_CHONGZHI_STATE_CAN_FETCH,													// �ٳ�ֵ ����ȡ
		ZAI_CHONGZHI_STATE_HAS_FETCH,													// �ٳ�ֵ ����ȡ

		ZAI_CHONGZHI_STATE_MAX,
	};

	class SCChongZhiInfo
	{
	public:
		SCChongZhiInfo();
		MessageHeader		header;

		long long history_chongzhi;
		int history_chongzhi_count;
		int today_chongzhi;
		int chongzhi_reward_fetch_flag;

		unsigned int special_first_chongzhi_timestamp;									// �����׳忪ʼʱ���
		char is_daily_first_chongzhi_open;												// ÿ���׳��Ƿ���
		char is_daily_first_chongzhi_fetch_reward;										// ÿ�ճ�ֵ�����Ƿ��Ѿ���ȡ
		short daily_total_chongzhi_fetch_reward_flag;									// ÿ���ۼƳ�ֵ������ȡ���
		char daily_total_chongzhi_stage;												// �ۼƳ�ֵ��ǰ�׶�
		char daily_first_chongzhi_times;												// ÿ���׳��ۼƴ���(��7�θ�����⽱������0)
		char special_first_chongzhi_fetch_reward_flag;									// �����׳影����ȡ���
		char zai_chongzhi_reward_flag;													// �ٳ�ֵ״̬��0 δ��ֵ��1 ����ȡ��2 ����ȡ��
		int daily_total_chongzhi_stage_chongzhi;										// ��ǰ�׶��ۼƳ�ֵ��
		char third_chongzhi_reward_flag;												// �����γ�ֵ״̬��0 δ��ֵ��1 ����ȡ��2 ����ȡ��
		char diff_weekday_chongzhi_is_open;												// ÿ���۳��Ƿ���	�����ڼ���أ�					
		short diff_weekday_chongzhi_stage_fetch_flag;									// ÿ���۳�׼�������ȡ�� �����ڼ���أ�
		int diff_wd_chongzhi_value;														// ÿ���۳��ȣ����ڼ���أ�
		short daily_chongzhi_value;														// ÿ�ճ�ֵ��ֵ���
		unsigned short first_chongzhi_active_reward_flag;								// �׳佱��������
		unsigned short first_chongzhi_fetch_reward_flag;								// �׳佱����ȡ���
		short daily_chongzhi_fetch_reward_flag;											// ÿ���׳佱����ȡ���
		short daily_chongzhi_complete_days;												// ÿ���׳��������
		short daily_chongzhi_times_fetch_reward_flag;									// ÿ���׳��ۼ������������
		unsigned int chongzhi_7day_reward_timestamp;									// ��ֵ-���췵�����ʱ��
		short chongzhi_7day_reward_fetch_day;											// ��ֵ-���췵����ȡ����
		short chongzhi_7day_reward_is_fetch;											// ��ֵ-���췵�������Ƿ���ȡ
		short daily_chongzhi_fetch_reward2_flag;										// ÿ���ۼƳ�ֵ����2��ȡ���
	};

	enum CHONGZHI_REWARD_TYPE
	{
		CHONGZHI_REWARD_TYPE_SPECIAL_FIRST = 0,											// �����׳�
		CHONGZHI_REWARD_TYPE_DAILY_FIRST = 1,											// �ճ��׳�
		CHONGZHI_REWARD_TYPE_DAILY_TOTAL = 2,											// �ճ��۳�
		CHONGZHI_REWARD_TYPE_DIFF_WEEKDAY_TOTAL = 3,									// ÿ���۳�
		CHONGZHI_REWARD_TYPE_FIRST = 4,													// �׳�
		CHONGZHI_REWARD_TYPE_DAILY = 5,													// ÿ�ճ�ֵ
		CHONGZHI_REWARD_TYPE_DAILY_TIMES = 6,											// ÿ�ճ�ֵ�ۼ���������
		CHONGZHI_REWARD_TYPE_DAILY2 = 7,												// ÿ�ճ�ֵ2

		CHONGZHI_REWARD_TYPE_MAX, 
	};

	class CSChongzhiFetchReward
	{
	public:
		CSChongzhiFetchReward();
		MessageHeader		header;

		int type;
		int param;
		int param2;
	};

	class CSZaiChongzhiFetchReward														// �ٳ�ֵ����ȡ����
	{
	public:
		CSZaiChongzhiFetchReward();
		MessageHeader		header;

		int is_third;	//�Ƿ�����γ�ֵ����
	};

	/////////////////////////////////////////////////  ���ֻ  /////////////////////////////////////////////////////////
	class SCCollectExchangeInfo
	{
	public:
		SCCollectExchangeInfo();
		MessageHeader		header;

		int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT];
	};

	/////////////////////////////////////////////////  ��Ҫ��Ԫ���  /////////////////////////////////////////////////////////
	class CSTotalChongzhiWantMoneyFetchInfo // 5262
	{
	public:
		CSTotalChongzhiWantMoneyFetchInfo();
		MessageHeader		header;
	};

	class SCTotalChongzhiWantMoneyFetchInfo // 5210
	{
	public:
		SCTotalChongzhiWantMoneyFetchInfo();
		MessageHeader		header;

		int reward_state;										// ��ǰҪ��Ľ׶�
		long long history_chongzhi;								// ��ǰ�ѳ�ֵ��
	};

	class CSTotalChongzhiWantMoneyFetchReward // 5263
	{
	public:
		CSTotalChongzhiWantMoneyFetchReward();
		MessageHeader		header;
	};

	class SCTotalChongzhiWantMoneyFetchReward // 5211
	{
	public:
		SCTotalChongzhiWantMoneyFetchReward();
		MessageHeader		header;

		int get_gold_bind;										// �����õ�Ԫ������
	};

	/////////////////////////////////////////////////  �۱���  /////////////////////////////////////////////////////////
	enum RA_CORNUCOPIA_OPERA_TYPE
	{
		RA_CORNUCOPIA_OPERA_TYPE_QUERY_INFO = 0,
		RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD,
		RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD_INFO,

		RA_CORNUCOPIA_OPERA_TYPE_MAX,
	};

	class SCRACornucopiaFetchInfo								// 5212�۱��裬��ȡ��ǰ״̬��Ϣ
	{
	public:
		SCRACornucopiaFetchInfo();
		MessageHeader		header;

		int reward_lun;											// ��ǰ�ǵڼ���
		long long history_chongzhi;								// ��ǰ�ѳ�ֵ��

		int record_count;										// �۱����¼����
		RACornucopiaRewardRecord record_list[RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT];
	};

	class SCRACornucopiaFetchReward		// 5213�۱��裬��ȡ��ҵ�ǰ��������Ľ���
	{
	public:
		SCRACornucopiaFetchReward();
		MessageHeader		header;

		int get_reward_gold;									// ��õĽ���
	};

	class SCRANewCornucopiaInfo			// 5226�¾۱�����Ϣ
	{
	public:
		SCRANewCornucopiaInfo();
		MessageHeader		header;

		int cornucopia_value;
		int total_cornucopia_value;
		short cornucopia_day_index;
		short cornucopia_total_reward_flag;
		unsigned char new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_MAX];
	};

	/////////////////////////////////////////////////  Ѳ��  /////////////////////////////////////////////////////////
	class CSBuyXunyouOpera										// 5270 ������Ѳ�β�������
	{
	public:
		CSBuyXunyouOpera();
		MessageHeader header;

		int type;												// ��������
	};

	class SCXunyouOperaInfo										// 5229 Ѳ�β�����Ϣ
	{
	public:
		SCXunyouOperaInfo();
		MessageHeader header;

		char yanhua_buy_times;									// �����̻��������					
		char yanhua_use_times;									// �����̻�ʹ�ô���
		char hongbao_buy_times;									// �������������
		char hongbao_use_times;									// ������ʹ�ô���
		char meigui_buy_times;									// ����õ�幺�����
		char meigui_use_times;									// ����õ��ʹ�ô���

		short reserve_sh;										// ����
	};

	/////////////////////////////////////  ȫ��boss /////////////////////////////////////
	class CSNationalBossBuyBuffReq								// ȫ��Boss ����buff
	{
	public:
		CSNationalBossBuyBuffReq();
		MessageHeader header;
	};

	class SCNationalBossUserInfo								// 4915 ȫ��Boss �û���Ϣ
	{
	public:
		SCNationalBossUserInfo();
		MessageHeader header;

		long long total_hurt;									// ���˺�
		long long total_bosshp;									// boss��Ѫ��
		char boss_flush_times;									// bossˢ�´���
		char is_finish;											// �Ƿ��Ѿ�����
		short buy_times;										// �������
		short rank_pos;											// ����
		short add_gongji_percent;								// ���ӹ����ٷֱ�
		unsigned int next_flush_timestamp;						// �´�ˢ��ʱ��
		int world_level;										// ����ȼ�
	};

	class SCNationalBossRankInfo								// 4916 ȫ��Boss ���а���Ϣ
	{
	public:
		SCNationalBossRankInfo();
		MessageHeader		header;

		const static int RANK_ITEM_MAX_COUNT = 10;

		struct RankItem
		{
			RankItem() : uid(0), total_hurt(0)
			{
				memset(name, 0, sizeof(name));
			}

			int uid;											// �û�uid
			GameName name;										// �û���
			long long total_hurt;								// ��ɵ����˺�
			char camp;											// ��Ӫ
			char reserve_ch;
			short reserve_sh;
		};

		long long total_bosshp;									// ȫ��Boss boss��Ѫ��
		int rank_count;											// ȫ��Boss ���а���Ϣ����
		RankItem rank_list[RANK_ITEM_MAX_COUNT];				// ȫ��Boss ���а���Ϣ�б�
	};

	/////////////////////////////////////////////////  �����  /////////////////////////////////////////////////////////
	class CSOpenGameActivityInfoReq								// ���󿪷����Ϣ
	{
	public:
		CSOpenGameActivityInfoReq();
		MessageHeader		header;
	};

	class CSOpenGameActivityFetchReward							// ����� �����콱
	{
	public:
		CSOpenGameActivityFetchReward();
		MessageHeader		header;

		enum REWARD_TYPE
		{
			REWARD_TYPE_PUTON_EQUIPMENT,						// װ���ռ�
			REWARD_TYPE_CAPABILITY,								// ս�������
			REWARD_TYPE_ROLELEVEL,								// ����ȼ����

			REWARD_TYPE_MAX,
		};

		short reward_type;										// ��������
		short reward_seq;										// ��������
	};

	class SCOpenGameActivityInfo								// �������Ϣ
	{
	public:
		SCOpenGameActivityInfo();
		MessageHeader		header;

		int oga_capability_reward_flag;							// ����� ս����������ȡ���
		int oga_rolelevel_reward_flag;							// ����� �ȼ�������ȡ���
		int oga_puton_equipment_reward_flag;					// ����� װ���ռ��������
		int oga_buy_equipmentgift_flag;							// ����� ����װ��������

		int total_chongzhi;										// �ۼƳ�ֵ��
		int puton_equipment_act_flag;							// װ���ռ�������

		unsigned short oga_putonequipment_fetch_times[OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ];	// ȫ���ռ�װ����ȡ��������
		unsigned short oga_capability_fetch_times[OGA_CAPABILITY_REWARD_MAX_SEQ];			// ȫ��ս�������ȡ��������
		unsigned short oga_rolelevel_fetch_times[OGA_ROLELEVEL_REWARD_MAX_SEQ];		// ȫ���ȼ������ȡ��������

		short oga_guild_battle_reward_type;						//�������Խ�������
		short oga_guild_battle_reward_flag;						//�������Խ�����־
	};

	class CSTitleOwnerInfoReq									// ����ƺ�ӵ������Ϣ
	{
	public:
		CSTitleOwnerInfoReq();
		MessageHeader		header;
	};

	class SCTitleOwnerInfo										// �ƺ�ӵ������Ϣ
	{
	public:
		SCTitleOwnerInfo();
		MessageHeader		header;

		int xianjiezhizhun_owner_uid;
		GameName xianjiezhizhun_owner_name;
		int junlintianxia_owner_uid;
		GameName junlintianxia_owner_name;
		int qingshihongyan_owner_uid;
		GameName qingshihongyan_owner_name;
		int fengliutitang_owner_uid;
		GameName fengliutitang_owner_name;
		int guosetianxiang_owner_uid;
		GameName guosetianxiang_owner_name;
		int kunlunzhanshen_owner_uid;
		GameName kunlunzhanshen_owner_name;
		int penglaizhanshen_owner_uid;
		GameName penglaizhanshen_owner_name;
		int cangqiongzhanshen_owner_uid;
		GameName cangqiongzhanshen_owner_name;
		int wangchengchengzhu_owner_uid;
		GameName wangchengchengzhu_owner_name;
		int zuiqiangxianmeng_owner_uid;
		GameName zuiqiangxianmeng_owner_name;
		int weizhencangqiong_onwer_uid;
		GameName weizhencangqiong_owner_name;
		int bosshunter_owner_uid;
		GameName bosshunter_owner_name;
		int tianxiawushuang_owner_uid;
		GameName tianxiawushuang_owner_name;
		int xiongbatianxia_owner_uid;
		GameName xiongbatianxia_owner_name;
	};

	/////////////////////////////////////////////////  1V1������  /////////////////////////////////////////////////////////
	class SCChallengeFieldStatus								// 4917 �����û���Ϣ
	{
	public:
		SCChallengeFieldStatus();
		MessageHeader		header;

		struct SideInfo
		{
			long long		role_id;
			ObjID			obj_id;
			short			level;
			GameName		name;
			char			camp;
			char			prof;
			char			avatar;
			char			sex;
			Attribute		hp;
			Attribute		max_hp;
			Attribute		mp;
			Attribute		max_mp;
			Attribute		speed;
			short			pos_x;
			short			pos_y;
			Dir				dir;
			Scalar			distance;
			int				capability;
			GuildID			guild_id;
			GuildName		guild_name;
			char			guild_post;
			char			reserve_ch;
			short			reserve_sh;
		};

		int					status;				// CHALLENGE_FIELD_STATE
		unsigned int		next_status_timestamp;
		SideInfo			side_info_list[CHALLENGE_FIELD_SIDE_MAX];
	};

	class CSChallengeFieldGetOpponentInfo						// ����������
	{
	public:
		CSChallengeFieldGetOpponentInfo();
		MessageHeader		header;

		short				is_get_top;
		short				reserve_sh;
	};

	class SCChallengeFieldOpponentInfo							//4931 ���������Ϣ 
	{
	public:
		SCChallengeFieldOpponentInfo();
		MessageHeader		header;

		struct OpponentInfo
		{
			long long user_id;
			char camp;
			char prof;
			char sex;
			char reserve_ch;
			int capability;
			int best_rank_break_level;

			GameName name;

			RoleAppearance appearance;
		};

		int opponent_count;
		OpponentInfo opponent_info_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCRoleShadowBossInfo
	{
	public:
		SCRoleShadowBossInfo();
		MessageHeader header;

		struct BossItem
		{
			int scene_id;
			int pos_x;
			int pos_y;
			ObjID obj_id;
			short reserve_sh;
			int role_id;

			SCChallengeFieldOpponentInfo::OpponentInfo role_info;
		};

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT,
			NOTIFY_REASON_REFRESH,

			NOTIFY_REASON_MAX,
		};

		int notify_reason;
		unsigned int next_refresh_timestamp;
		BossItem item_list[ROLE_BOSS_MAX_COUNT];
	};

	class CSChallengeFieldGetLuckyInfo							// ���������б�
	{
	public:
		CSChallengeFieldGetLuckyInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldFetchGuangHui							// ��ȡ���
	{
	public:
		CSChallengeFieldFetchGuangHui();
		MessageHeader		header;
	};

	class CSChallengeFieldGetRankInfo							// ����Ӣ�۰�
	{
	public:
		CSChallengeFieldGetRankInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldGetUserInfo							// ����û���Ϣ��ս��
	{
	public:
		CSChallengeFieldGetUserInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldResetOpponentList						// ˢ����ս�б�
	{
	public:
		CSChallengeFieldResetOpponentList();
		MessageHeader		header;
	};

	class CSChallengeFieldFightReq								// ��ս����
	{
	public:
		CSChallengeFieldFightReq();
		MessageHeader		header;

		short opponent_index;
		char ignore_rank_pos;
		char auto_buy_jointimes;
		int rank_pos;
	};

	class CSChallengeFieldBuyJoinTimes							// ����������
	{
	public:
		CSChallengeFieldBuyJoinTimes();
		MessageHeader		header;
	};

	class CSChallengeFieldFetchJifenRewardReq					// ��ȡ���ֽ���
	{
	public:
		CSChallengeFieldFetchJifenRewardReq();
		MessageHeader		header;

		short reward_seq;
		short reserve_sh;
	};

	class CSChallengeFieldBuyBuff								// ����buff
	{
	public:
		CSChallengeFieldBuyBuff();
		MessageHeader		header;
	};

	class SCChallengeFieldUserInfo								//4918 ��ս�б���Ϣ�͸�����Ϣ
	{
	public:
		SCChallengeFieldUserInfo();
		MessageHeader		header;

		struct  OpponentInfo
		{
			long long user_id;
			int rank_pos;
		};

		int rank_pos;
		int curr_opponent_idx;
		int join_times;
		int buy_join_times;
		int jifen;
		int jifen_reward_flag;
		int reward_guanghui;
		int reward_bind_gold;
		int liansheng;
		int buy_buff_times;
		int best_rank_pos;
		int free_day_times;

		struct RewardItem
		{
			ItemID item_id;
			short num;
		};
		RewardItem reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		OpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCChallengeFieldOpponentRankPosChange					// 4919 ��λ�仯֪ͨ
	{
	public:
		SCChallengeFieldOpponentRankPosChange();
		MessageHeader		header;

		long long user_id;
		int rank_pos;
	};

	class SCChallengeFieldReportInfo							// ս��
	{
	public:
		SCChallengeFieldReportInfo();
		MessageHeader		header;

		struct ReportItem
		{
			unsigned int		challenge_time;					// ʱ��
			long long			target_uid;						// �Է�id
			GameName			target_name;					// �Է�����
			char				is_sponsor;						// �Լ��Ƿ�Ϊ��ս��
			char				is_win;							// �Ƿ�ʤ��
			short				reserve_sh;						// ����
			unsigned short		old_rankpos;					// ����ǰ����
			unsigned short		new_rankpos;					// ����������
		};

		int report_count;
		ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
	};

	class SCChallengeFieldRankInfo								// Ӣ�۰�
	{
	public:
		SCChallengeFieldRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() :user_id(0), capability(0) 
			{
				memset(user_name, 0, sizeof(user_name));
			}

			long long user_id;
			int capability;
			GameName user_name;	
			char sex;
			char is_robot;
			char prof;
			char reserved;
			int role_level;

			RoleAppearance appearance;
		};

		RankItem rank_list[CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM];
	};

	class SCChallengeFieldLuckyPosInfo							// ��������
	{
	public:
		SCChallengeFieldLuckyPosInfo();
		MessageHeader		header;

		struct LuckyInfoItem
		{
			LuckyInfoItem() : rank_pos(0), user_id(0) 
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int rank_pos;
			int user_id;
			GameName user_name;
		};

		int lucky_rankpos_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
		LuckyInfoItem lucky_info_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	};

	class SCChallengeFieldBeDefeatNotice
	{
	public:
		SCChallengeFieldBeDefeatNotice();
		MessageHeader		header;
	};

	class SCChallengeFieldRewardGoldNotice
	{
	public:
		SCChallengeFieldRewardGoldNotice();
		MessageHeader		header;

		int reward_bind_gold;
	};

	class SCChallengeFieldWin			// 1V1������ֱ��ʤ�� 4941
	{
	public:
		SCChallengeFieldWin();
		MessageHeader		header;

		short old_rank_pos;
		short new_rank_pos;
	};

	enum CFBreakType											// ��������ʷ���ͻ������
	{
		CF_BREAK_TYPE_INFO = 0,									// 0 ������Ϣ
		CF_BREAK_TYPE_BREAK = 1,								// 1 ����ͻ��
	};

	class CSChallengeFieldBestRankBreakReq						// 1V1��������ʷ���ͻ������ 4942
	{
	public:
		CSChallengeFieldBestRankBreakReq();
		MessageHeader		header;

		int op_type;											// 0 ������Ϣ 1 ����ͻ��

	};

	class SCChallengeFieldBestRankBreakInfo						// 1V1��������ʷ���ͻ����Ϣ 4943
	{
	public:
		SCChallengeFieldBestRankBreakInfo();
		MessageHeader		header;

		int best_rank_break_level;
		int best_rank_pos;
	};

	class CSChallengeFieldReadyStartFightReq					// 1V1����������׼����ʽ��ʼս������ʱ 4944
	{
	public:
		CSChallengeFieldReadyStartFightReq();
		MessageHeader		header;
	};

	/////////////////////////////////////  ��� /////////////////////////////////////

	enum CBA_OPERA_TYPE											// ��������������
	{
		CBA_OPERA_TYPE_INVALID = 0,
		CBA_OPERA_TYPE_REQ_INFO,								// ������Ϣ
		CBA_OPERA_TYPE_FETCH_LOING_REWARD,						// ��ȡ��¼����
		CBA_OPERA_TYPE_FETCH_GUILD_REWARD,						// ��ȡ���˽���
		CBA_OPERA_TYPE_FETCH_MARRY_REWARD,						// ��ȡ��齱��
		CBA_OPERA_TYPE_FETCH_ONLINE_REWARD,						// ��ȡ���߽���
		CBA_OPERA_TYPE_FETCH_UPLEVEL_REWARD,					// ��ȡ��������
		CBA_OPERA_TYPE_FETCH_JOIN_ACTIVITY_REWARD,				// ��ȡ��������

		CBA_OPERA_TYPE_MAX,
	};

	class CSCloseBetaActivityOperaReq							// ����������
	{
	public:
		CSCloseBetaActivityOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCCloseBetaActivityInfo
	{
	public:
		SCCloseBetaActivityInfo();								// �����Ϣ
		MessageHeader header;

		char has_fetch_login_reward;							// �Ƿ��Ѿ���ȡ���յ�¼����
		char has_fetch_guild_reward;							// �Ƿ��Ѿ���ȡ���˽���
		char has_fetch_marry_reward;							// �Ƿ��Ѿ���ȡ�˽�齱��
		char fetch_online_reward_flag;							// ��ȡ����ʱ���������
		int fetch_uplevel_reward_flag;							// ����������ȡ���
		int join_activity_flag;									// �������
		int fetch_activity_reward_flag;							// ��ȡ��������
		int expfb_satisfy_reward_cond_flag;						// ���鱾���㽱���������
		int expfb_fetch_reward_flag;							// ���鱾��ȡ�������
		int equipfb_satisfy_reward_cond_flag;					// װ�������㽱���������
		int equipfb_fetch_reward_flag;							// װ������ȡ�������
		int tdfb_satisfy_reward_cond_flag;						// ���������㽱���������
		int tdfb_fetch_reward_flag;								// ��������ȡ�������
		int challengefb_satisfy_reward_cond_flag;				// ��ս�����㽱���������
		int challengefb_fetch_reeward_flag;						// ��ս����ȡ�������
		int total_login_days;									// �ܹ���¼����
		int online_time;										// ����ʱ��
	};

	enum RA_LUCKY_ROLL_OPERA_TYPE
	{
		RA_LUCKY_ROLL_OPERA_TYPE_QUERY_INFO = 0,
		RA_LUCKY_ROLL_OPERA_TYPE_ROLL,
		RA_LUCKY_ROLL_OPERA_TYPE_FETCH_EXTRA_REWARD,
		RA_LUCKY_ROLL_OPERA_TYPE_10_COUNT_ROLL,					//10����

		RA_LUCKY_ROLL_OPERA_TYPE_MAX,
	};

	class CSLuckyRollActivityOperaReq							// 4969����ת�̻
	{
	public:
		CSLuckyRollActivityOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class SCLuckyRollActivityInfo								// 4934����ת�̻��Ϣ
	{
	public:
		SCLuckyRollActivityInfo();
		MessageHeader header;

		int roll_times;
		int gold_poll;
		int record_count;
		LuckyRollRewardRecord record_list[LUCKY_ROLL_REWARD_RECORD_MAX_COUNT];
	};

	class SCLuckyRollActivityRollResult							// 4935����ת�̳齱���
	{
	public:
		SCLuckyRollActivityRollResult();
		MessageHeader header;

		short reward_index;
		short reserve_sh;
	};

	class CSUpdateNoticeFetchReward
	{
	public:
		CSUpdateNoticeFetchReward();
		MessageHeader header;
	};

	class SCUpdateNoticeInfo
	{
	public:
		SCUpdateNoticeInfo();
		MessageHeader header;

		int server_version;
		int fetch_reward_version;
	};

	class SCRAChongzhiRankInfo
	{
	public:
		SCRAChongzhiRankInfo();
		MessageHeader header;

		int chongzhi_num;
	};

	class SCRAChongzhiRank2Info		//8982
	{
	public:
		SCRAChongzhiRank2Info();
		MessageHeader header;

		int chongzhi_num;
	};

	class SCRAConsumeGoldRankInfo
	{
	public:
		SCRAConsumeGoldRankInfo();
		MessageHeader header;

		int consume_gold_num;
	};

	class SCRAConsumeGoldFanliInfo
	{
	public:
		SCRAConsumeGoldFanliInfo();
		MessageHeader header;

		int consume_gold;
	};

	class SCRAProfessRankInfo		// 2464 ������л��Ϣ
	{
	public:
		SCRAProfessRankInfo();
		MessageHeader header;

		int rank;															// ��װ�����
		int ra_profess_score;												// ������а� ��׻���
		int ra_profess_to_num;												// ������а� ��״���
		int ra_profess_from_num;											// ������а� ����״���
	};

	/////////////////////////////////////  ͨ�û /////////////////////////////////////

	class CSCommonActivityFetchReward
	{
	public:
		CSCommonActivityFetchReward();
		MessageHeader		header;

		int unit_type;
		int param;
		int	param2;
	};

	class SCCommonActivityOpenUnitInfo
	{
	public:
		SCCommonActivityOpenUnitInfo();
		MessageHeader		header;

		struct UnitInfo
		{
			int unit_type;
			int param1;
			int param2;
			long long reward_state;
		};

		int unit_num;
		UnitInfo unit_info_list[COMMON_ACTIVITY_UNIT_MAX_NUM];		
	};

	class CSRandActivityOperaReq  // 2257
	{
	public:
		CSRandActivityOperaReq();
		MessageHeader header;

		short rand_activity_type;
		short opera_type;
		int param_1;
		int param_2;
	};
	/////////////////////////////////////  ���� ȫ������ /////////////////////////////////////
	enum RA_SERVER_PANIC_BUY_OPERA_TYPE
	{
		RA_SERVER_PANIC_BUY_OPERA_TYPE_QUERY_INFO = 0,
		RA_SERVER_PANIC_BUY_OPERA_TYPE_BUY_ITEM,

		RA_SERVER_PANIC_BUY_OPERA_TYPE_MAX,
	};

	class SCRAServerPanicBuyInfo								 // ȫ���������Ϣ
	{
	public:
		SCRAServerPanicBuyInfo();
		MessageHeader header;

		int user_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
		int server_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	};

	/////////////////////////////////////  ���� ���˷��� /////////////////////////////////////
	enum RA_PERSONAL_PANIC_BUY_OPERA_TYPE
	{
		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_QUERY_INFO = 0,
		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_BUY_ITEM,

		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_MAX,
	};

	class SCRAPersonalPanicBuyInfo								// ���˷������Ϣ
	{
	public:
		SCRAPersonalPanicBuyInfo();
		MessageHeader header;

		int buy_numlist[RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT];
	};

	/////////////////////////////////////  ���� ��ֵ����� /////////////////////////////////////
	enum RA_DAY_CHONGZHI_FANLI_OPERA_TYPE
	{
		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_QUERY_INFO = 0,
		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_FETCH_REWARD,

		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_MAX,
	};

	class SCRADayChongZhiFanLiInfo								// ��ֵ�����콱���
	{
	public:
		SCRADayChongZhiFanLiInfo();
		MessageHeader header;

		int chongzhi_gold;										//��ֵ���
		int fetch_reward_flag;									//��ȡ��Ϣ
	};

	/////////////////////////////////////  ���� ÿ�����ѻ /////////////////////////////////////
	enum RA_DAY_CONSUME_GOLD_OPERA_TYPE
	{
		RA_DAY_CONSUME_GOLD_TYPE_QUERY_INFO = 0,
		RA_DAY_CONSUME_GOLD_TYPE_FETCH_REWARD,

		RA_DAY_CONSUME_GOLD_OPERA_TYPE_MAX,
	};

	class SCRADayConsumeGoldInfo								// ÿ�������콱���
	{
	public:
		SCRADayConsumeGoldInfo();
		MessageHeader header;

		int consume_gold;									    //���ѽ��
		int fetch_reward_flag;									//��ȡ��Ϣ
	};

	/////////////////////////////////////  ���� �����챦 /////////////////////////////////////
	enum RA_CHESTSHOP_OPERA_TYPE
	{
		RA_CHESTSHOP_OPERA_TYPE_QUERY_INFO = 0,
		RA_CHESTSHOP_OPERA_TYPE_FETCH_REWARD,

		RA_CHESTSHOP_OPERA_TYPE_MAX,
	};

	class SCRAChestshopInfo									
	{
	public:
		SCRAChestshopInfo();
		MessageHeader header;

		short chestshop_times;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ��ʯ���� /////////////////////////////////////
	enum RA_STONE_UPLEVEL_OPERA_TYPE
	{
		RA_STONE_UPLEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_STONE_UPLEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_STONE_UPLEVEL_OPERA_TYPE_MAX,
	};

	class SCRAStoneUplevelInfo									// ��ʯ������Ϣ
	{
	public:
		SCRAStoneUplevelInfo();
		MessageHeader header;

		int total_level;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ��Ů���� /////////////////////////////////////
	enum RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE
	{
		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_MAX,
	};

	class SCRAXiannvChanmianUplevelInfo
	{
	public:
		SCRAXiannvChanmianUplevelInfo();
		MessageHeader header;

		int chanmian_level;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ������� /////////////////////////////////////
	enum RA_MOUNT_UPGRADE_OPERA_TYPE
	{
		RA_MOUNT_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_MOUNT_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_MOUNT_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAMountUpgradeInfo
	{
	public:
		SCRAMountUpgradeInfo();
		MessageHeader header;

		int mount_grade;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  ���� �ۼ����ѻ /////////////////////////////////////
	enum RA_TOTAL_CONSUME_GOLD_OPERA_TYPE
	{
		RA_TOTAL_CONSUME_GOLD_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CONSUME_GOLD_TYPE_FETCH_REWARD,

		RA_TOTAL_CONSUME_GOLD_OPERA_TYPE_MAX,
	};

	class SCRATotalConsumeGoldInfo								// �ۼ������콱���
	{
	public:
		SCRATotalConsumeGoldInfo();
		MessageHeader header;

		int consume_gold;										//���ѽ��
		int fetch_reward_flag;									//��ȡ��Ϣ
	};

	/////////////////////////////////////  ���� ��Ծ����� /////////////////////////////////////
	enum RA_DAY_ACTIVE_DEGREE_OPERA_TYPE
	{
		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_QUERY_INFO = 0,
		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_FETCH_REWARD,

		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_TYPE_MAX,
	};

	class SCRADayActiveDegreeInfo								// ÿ�ջ�Ծ����Ϣ
	{
	public:
		SCRADayActiveDegreeInfo();
		MessageHeader header;

		int active_degree;										//��Ծ��
		int fetch_reward_flag;									//��ȡ��Ϣ
	};

	/////////////////////////////////////  ���� ������� /////////////////////////////////////
	enum RA_QIBING_UPGRADE_OPERA_TYPE
	{
		RA_QIBING_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_QIBING_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_QIBING_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAQibingUpgradeInfo
	{
	public:
		SCRAQibingUpgradeInfo();
		MessageHeader header;
		
		int qibing_grade;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ����ȫ��ȼ� /////////////////////////////////////
	enum RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE
	{
		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_MAX,
	};

	class SCRAMentalityUplevelInfo
	{
	public:
		SCRAMentalityUplevelInfo();
		MessageHeader header;
		
		int total_mentality_level;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ������� /////////////////////////////////////
	enum RA_WING_UPGRADE_OPERA_TYPE
	{
		RA_WING_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_WING_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_WING_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAWingUpgradeInfo
	{
	public:
		SCRAWingUpgradeInfo();
		MessageHeader header;
		
		int jinghua;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ��ɱboss� /////////////////////////////////////
	enum RA_KILL_BOSS_OPERA_TYPE
	{
		RA_KILL_BOSS_OPERA_TYPE_QUERY_INFO = 0,
		RA_KILL_BOSS_OPERA_TYPE_FETCH_REWARD,

		RA_KILL_BOSS_OPERA_TYPE_TYPE_MAX,
	};

	class SCRAKillBossInfo										// ��ɱboss�
	{
	public:
		SCRAKillBossInfo();
		MessageHeader header;

		int kill_count;											//��ɱ����
	};

	/////////////////////////////////////  ���� ȫ���� /////////////////////////////////////
	enum RA_QUANMIN_QIFU_OPERA_TYPE
	{
		RA_QUANMIN_QIFU_OPERA_TYPE_QUERY_INFO = 0,
		RA_QUANMIN_QIFU_OPERA_TYPE_FETCH_REWARD,

		RA_QUANMIN_QIFU_OPERA_TYPE_MAX,
	};

	class SCRAQuanminQifuInfo
	{
	public:
		SCRAQuanminQifuInfo();
		MessageHeader header;

		short qifu_times;
		short qifu_fetch_flag;
	};

	/////////////////////////////////////  ��¼���� ȫ���� /////////////////////////////////////
	enum RA_LOGIN_GIFT_OPERA_TYPE
	{
		RA_LOGIN_GIFT_OPERA_TYPE_INFO = 0,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_COMMON_REWARD,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_VIP_REWARD,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_ACCUMULATE_REWARD,

		RA_LOGIN_GIFT_OPERA_TYPE_MAX,
	};

	class SCRALoginGiftInfo
	{
	public:
		SCRALoginGiftInfo();
		MessageHeader header;

		short login_days;
		char has_login;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
	};

	class SCRALoginGiftInfo_0    // 8819
	{
	public:
		SCRALoginGiftInfo_0();
		MessageHeader header;

		short login_days;
		char has_login;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
	};

	enum EVERYDAY_NICE_GIFT_REQ
	{
		EVERYDAY_NICE_GIFT_SEND_INFO,				//������Ϣ
		EVERYDAY_NICE_GIFT_FETCH_REWARD,			//��ȡ����
	};

	struct SCRAEverydayNiceGiftInfo	// ÿ�պ��� 8820
	{
		SCRAEverydayNiceGiftInfo();
		MessageHeader header;

		short is_active;								// �Ƿ��Ѽ���
		short reserver_sh;
		int everyday_recharge;							// ���ճ�ֵ����
		int can_fetch_reward_flag;						// ������ȡ�������
		int have_fetch_reward_flag;						// �Ѿ���ȡ�������
	};

	/////////////////////////////////////  ���� �������� /////////////////////////////////////
	enum RA_SHOUYOU_YUXIANG_OPERA_TYPE
	{
		RA_SHOUYOU_YUXIANG_OPERA_TYPE_QUERY_INFO = 0,
		RA_SHOUYOU_YUXIANG_OPERA_TYPE_FETCH_REWARD,

		RA_SHOUYOU_YUXIANG_OPERA_TYPE_MAX,
	};

	class SCRAShouYouYuXiangInfo
	{
	public:
		SCRAShouYouYuXiangInfo();
		MessageHeader header;

		short shouyou_yuxiang_fetch_flag;
		short shouyou_yuxiang_give_flower_flag;
		int shouyou_yuxiang_flower_num;
	};

	/////////////////////////////////////  ���� ս����ƴ  ///////////////////////////////
	enum RA_BIPIN_CAPABILITY_OPERA_TYPE
	{
		RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO = 0,
		RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD,

		RA_BIPIN_CAPABILITY_OPERA_TYPE_MAX,
	};

	struct TopUserInfo
	{
		TopUserInfo() { this->Reset(); }

		void Reset()
		{
			uid = 0; capability = 0;
			memset(name, 0, sizeof(name));
			prof = 0;
			reserve_ch = 0;
			reserve_sh = 0;
			avatar_timestamp = 0;
		}

		int uid;
		GameName name;
		long long capability;
		char prof;
		char reserve_ch;
		short reserve_sh;
		long long avatar_timestamp;
	};

	static const int TOP_COUNT = 3;

	class SCRABipinCapabilityInfo
	{
	public:
		SCRABipinCapabilityInfo();
		MessageHeader header;

		int bipin_activity_type;
		int capability;
		int fetch_reward_flag;

		TopUserInfo top_user_info_list[TOP_COUNT];	// ������Ӫ��������һ����Ϣ
	};

	/////////////////////////////////////  ���� ���˱�ƴ /////////////////////////////////////

	class SCRAXianMengBiPinInfo
	{
	public:
		SCRAXianMengBiPinInfo();
		MessageHeader header;

		int xianmeng_bipin_kill_boss_count;
	};

	/////////////////////////////////////  ���� �������� /////////////////////////////////////

	class SCRAXianMengJueQiInfo
	{
	public:
		SCRAXianMengJueQiInfo();
		MessageHeader header;

		int xianmeng_jueqi_increase_capability;
	};

	/////////////////////////////////////  ���� ���ʳ�ֵ /////////////////////////////////////
	enum RA_DANBI_CHONGZHI_OPERA_TYPE
	{
		RA_DANBI_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,
		RA_DANBI_CHONGZHI_OPERA_TYPE_FETCH_REWARD,

		RA_DANBI_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRADanbiChongzhiInfo             // 2241
	{
	public:
		SCRADanbiChongzhiInfo();
		MessageHeader header;

		int reward_can_fetch_flag;
		int reward_fetch_flag;
	};

	/////////////////////////////////////  ���� ÿ�ճ�ֵ���� /////////////////////////////////////
	class SCRADayChongzhiRankInfo
	{
	public:
		SCRADayChongzhiRankInfo();
		MessageHeader header;

		int gold_num;
	};

	/////////////////////////////////////  ���� ÿ���������� /////////////////////////////////////
	class SCRADayConsumeRankInfo
	{
	public:
		SCRADayConsumeRankInfo();
		MessageHeader header;

		int gold_num;
	};
	
	/////////////////////////////////////////    һվ����     /////////////////////////////////////
	class SCYiZhanDaoDiRankInfo															// һվ���� ���а���Ϣ
	{
	public:
		SCYiZhanDaoDiRankInfo();														// 2231
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : uid(0), jisha_count(0), dead_count(0)
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int uid;
			GameName user_name;
			short jisha_count;
			short dead_count;
		};
	
		short self_jisha_count;
		short self_rand_pos;
		int count;																		// һվ���� ���а���Ϣ����
		RankItem rank_list[YIZHANDAODI_RANK_NUM];										// һվ���� ���а���Ϣ�б�
	};

	enum KICKOUT_BATTLE_FIELD_REASON
	{
		KICKOUT_BATTLE_FIELD_REASON_INVALID = 0,

		KICKOUT_BATTLE_FIELD_REASON_DEAD_ISOUT = 1,										// �������û��
		KICKOUT_BATTLE_FIELD_REASON_TIMEOUT = 2,										// ս��ʱ�䵽��

		KICKOUT_BATTLE_FIELD_REASON_MAX,
	};

	class SCYiZhanDaoDiKickout
	{
	public:
		SCYiZhanDaoDiKickout();															// 2227
		MessageHeader		header;	

		int reason;

		int jisha_count;
		int dead_count;
		int jisha_score;

		int lucky_count;
		int rank_pos;																	// ����21���Ժ������ͳһ��ʾΪ21�� ������� ��20���Ժ� ����
	};

	class SCYiZhanDaoDiLuckyInfo
	{
	public:
		SCYiZhanDaoDiLuckyInfo();														// 2228
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[YIZHANDAODI_LUACY_ROLE_MAX];
	};

	class SCYiZhanDaoDiTitleChange
	{
	public:
		SCYiZhanDaoDiTitleChange();														// 2229
		MessageHeader		header;

		int					uid;
		ObjID				obj_id;
		short				title_id;
		int					jisha_count;
	};

	enum YIZHANDAODI_GUWU_TYPE
	{
		YIZHANDAODI_GUWU_TYPE_INVALID = 0,

		YIZHANDAODI_GUWU_TYPE_GONGJI,
		YIZHANDAODI_GUWU_TYPE_MAXHP,

		YIZHANDAODI_GUWU_TYPE_MAX,
	};

	class CSYiZhanDaoDiGuwu
	{
	public:
		CSYiZhanDaoDiGuwu();															// 2258
		MessageHeader		header;

		int guwu_type;
	};

	class SCYiZhanDaoDiUserInfo
	{
	public:
		SCYiZhanDaoDiUserInfo();	     // 2230
		MessageHeader		header;

		struct RewardItem
		{
			RewardItem() : item_id(0), num(0), is_bind(0) {}
			int item_id;
			short num;
			short is_bind;
		};

		int jisha_count;
		int jisha_title_id;
		int dead_count;
		int jisha_score;

		int gongji_guwu_per;
		int maxhp_guwu_per;
		int kill_num_reward_flag;

		short show_jiesuan;
		short reward_count;
		const static int YZDD_REWARD_MAX_COUNT = 8;
		
		RewardItem kill_num_reward_list[YZDD_REWARD_MAX_COUNT];
	};

	class SCYiZhanDaoDiLastFirstInfo
	{
	public:
		SCYiZhanDaoDiLastFirstInfo();
		MessageHeader		header;

		int uid;
		GameName game_name;
	};

	/////////////////////////////////////  �������ֻ /////////////////////////////////////
	class SCMonsterInvadeTimeNotice
	{
	public:
		SCMonsterInvadeTimeNotice();		// 2226
		MessageHeader header;

		unsigned int next_monster_invade_time;
	};

	/////////////////////////////////////  �󸻺�� /////////////////////////////////////
	class SCMillionaireTimeNotice
	{
	public:
		SCMillionaireTimeNotice();		// 2249
		MessageHeader header;

		unsigned int next_millionaire_box_refresh_time;
		unsigned int next_millionaire_boss_refresh_time;
	};

	class CSMillionaireUseFrozenSkill
	{
	public:
		CSMillionaireUseFrozenSkill();		
		MessageHeader header;

		ObjID target_obj_id;
		short reserver;
	};
	/////////////////////////////////////  ���йһ�� /////////////////////////////////////
	class CSWuXingGuaJiGetReward
	{
	public:
		CSWuXingGuaJiGetReward();		
		MessageHeader header;

		int target_idx;
	};

	class SCWuXingGuaJiLuckyInfo
	{
	public:
		SCWuXingGuaJiLuckyInfo();														// 2228
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[WUXINGGUAJI_LUACY_ROLE_MAX];
	};

	class SCWuXingGuaJiInfo
	{
	public:
		SCWuXingGuaJiInfo();		
		MessageHeader header;

		enum WUXINGGUAJI_REASON
		{
			WUXINGGUAJI_REASON_INVALID,

			WUXINGGUAJI_REASON_ENTER,
			WUXINGGUAJI_REASON_BOSS_DEAD,
			WUXINGGUAJI_REASON_GET_ITEM,
			WUXINGGUAJI_REASON_FETCH_REWARD,
			WUXINGGUAJI_REASON_BOSS_REFRESH,

			WUXINGGUAJI_REASON_MAX,
		};

		int reason;

		struct BossInfo
		{
			int monster_id;
			unsigned int next_boss_flush_time;
		};
		BossInfo boss_list[WUXINGGUAJI_BOSS_NUM];

		int my_lucky_count;
		WuXingGuaJiTargetInfo target_list[WUXINGGUAJI_TARGET_MAX];
	};

	class SCGuajiBossBornNotice
	{
	public:
		SCGuajiBossBornNotice();														
		MessageHeader		header;

		int boss_id;
		int scene_id;
		int pos_x;
		int pos_y;
	};

	/////////////////////////////////////  ��ֵ���� /////////////////////////////////////
	enum RA_CHARGE_REPAYMENT_OPERA_TYPE
	{
		RA_CHARGE_REPAYMENT_OPERA_TYPE_QUERY_INFO = 0,
		RA_CHARGE_REPAYMENT_OPERA_TYPE_FETCH_REWARD,

		RA_CHARGE_REPAYMENT_OPERA_TYPE_MAX,
	};
	class SCChargeRewardInfo // 2236
	{
	public:
		SCChargeRewardInfo();														
		MessageHeader		header;

		int reward_active_flag;							// ��ǰ����ĳ�ֵ����
		int reward_fetch_flag;							// ��ǰ�ѻ�ȡ�ĳ�ֵ����
		int history_charge_during_act;					// ��ڼ���ۻ���ֵ��
	};

	/////////////////////////////////////  �Ϸ�� /////////////////////////////////////
	class SCCSASubActivityState
	{
	public:
		SCCSASubActivityState();
		MessageHeader		header;

		char sub_activity_state_list[CSA_SUB_TYPE_MAX];
	};

	class SCCSAActivityInfo
	{
	public:
		struct RankUser
		{
			int role_id;
			long long rank_value;
			long long rank_reserved;
			GameName role_name;
			char camp;
			char sex;
			short prof;
		};

		struct RankItem
		{
			char is_finish;
			char reserve_ch;
			short reserve_sh;
			RankUser user_list[COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM];
		};

		SCCSAActivityInfo();
		MessageHeader		header;

		int qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
		int csa_xmz_winner_roleid;
		int csa_gcz_winner_roleid;

		RankItem rank_item_list[CSA_RANK_TYPE_MAX];	

		int server_panic_buy_num_list[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	};

	class CSCSAQueryActivityInfo
	{
	public:
		CSCSAQueryActivityInfo();
		MessageHeader		header;
	};
	
	enum CSA_LOGIN_GIFT_OPERA
	{
		CSA_LOGIN_GIFT_OPERA_FETCH_COMMON_REWARD = 0,
		CSA_LOGIN_GIFT_OPERA_FETCH_VIP_REWARD,
		CSA_LOGIN_GIFT_OPERA_FETCH_ACCUMULATE_REWARD,

		CSA_LOGIN_GIFT_OPERA_MAX,
	};

	enum CSA_ROLL_OPERA
	{
		CSA_ROLL_OPERA_ROLL = 0,
		CSA_ROLL_OPERA_BROADCAST = 1,
	};

	class CSCSARoleOperaReq
	{
	public:
		CSCSARoleOperaReq();
		MessageHeader		header;

		short sub_type;
		short param_1;
		short param_2;
		short reserve_sh;
	};

	class SCCSARoleInfo
	{
	public:
		SCCSARoleInfo();
		MessageHeader		header;

		int rank_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
		int roll_chongzhi_num;
		int chongzhi_rank_chongzhi_num;
		int consume_rank_consume_gold;
		int kill_boss_kill_count;

		int personal_panic_buy_numlist[COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; 
		int server_panic_buy_numlist[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];

		short login_days;
		char reserve_ch;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
		int roll_total_chongzhi_num;
		short rank_combine_consume_num;                                // �Ϸ���������
		short rank_combine_chongzhi_num;                               // �Ϸ���ֵ����
	};

	class SCCSARollResult
	{
	public:
		SCCSARollResult();
		MessageHeader		header;

		int ret_seq;
	};

	enum CSA_BOSS_OPERA_TYPE
	{
		CSA_BOSS_OPERA_TYPE_ENTER = 0,						//���볡��
		CSA_BOSS_OPERA_TYPE_INFO_REQ = 1,					//����boss��Ϣ
		CSA_BOSS_OPERA_TYPE_RANK_REQ = 2,					//�������а���Ϣ
		CSA_BOSS_OPERA_TYPE_ROLE_INFO_REQ = 3,				//���������Ϣ
	};

	class SCCSABossInfo
	{
	public:
		SCCSABossInfo();
		MessageHeader		header;

		struct BossInfo
		{
			BossInfo() : boss_id(0), boss_type(0), next_refresh_time(0)
			{}

			unsigned short boss_id;							//boss_id
			short boss_type;								//boss����
			unsigned int next_refresh_time;					//�´�ˢ��ʱ��
		};

		BossInfo boss_info[COMBINE_SERVER_BOSS_MAX_COUNT];
		short refresh_status;								//bossˢ��״̬,0�ճ�boss,1�boss
		short gather_count;
	};

	class SCCSABossRankInfo
	{
	public:
		SCCSABossRankInfo();
		MessageHeader		header;

		struct BossRank
		{
			BossRank() : id(0), rank_value(0)
			{
				memset(name, 0, sizeof(name));
			}

			int id;										//����(����)id
			int rank_value;								//��ɱ��
			GameName name;								//����
		};

		BossRank personal_rank[COMBINE_SERVER_BOSS_RANK_NUM];
		BossRank guild_rank[COMBINE_SERVER_BOSS_RANK_NUM];
	};

	class SCCSABossRoleInfo
	{
	public:
		SCCSABossRoleInfo();
		MessageHeader		header;

		short kill_boss_num;							//���˻�ɱ��
		short guild_killl_boss_num;						//���˹����ɱ��
	};

	/////////////////////////////////////  ���� �ۼƳ�ֵ�������� /////////////////////////////////////
	enum RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE
	{
		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_FETCH_REWARD,

		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_MAX,
	};

	class SCRATotalChargeDayInfo	// 2242
	{
	public:
		SCRATotalChargeDayInfo();
		MessageHeader header;

		int charge_day_count;								// �ۼƳ�ֵ����
		int reward_has_fetch_flag;							// ����ȡ���Ľ������
	};

	/////////////////////////////////////  ���� �䱦��  /  ////////////////////////////////////
	enum RA_ZHENBAOGE_OPERA_TYPE
	{
		RA_ZHENBAOGE_OPERA_TYPE_QUERY_INFO = 0,					// ������Ϣ
		RA_ZHENBAOGE_OPERA_TYPE_BUY = 1,						// ������������
		RA_ZHENBAOGE_OPEAR_TYPE_BUY_ALL = 2,					// ȫ����������
		RA_ZHENBAOGE_OPEAR_TYPE_FLUSH = 3,						// ˢ������
		RA_ZHENBAOGE_OPEAR_TYPE_RARE_FLUSH = 4,					// ϡ��ˢ������
		RA_ZHENBAOGE_OPERA_TYPE_FETCH_SERVER_GIFT = 5,			// ��ȡȫ�����

		RA_ZHENBAOGE_OPERA_TYPE_MAX
	};

	class SCRAZhenbaogeInfo
	{
	public:
		SCRAZhenbaogeInfo();
		MessageHeader header;

		UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);
		short zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];
		short reserve_sh;
		short zhenbaoge_refresh_gift_fetch_flag;
		short cur_flush_times;
		unsigned int zhenbaoge_next_flush_timetamp;
	};

	/////////////////////////////////////���/////////////////////////////////
	class CSCrossStartReq                // 5750
	{
	public:
		CSCrossStartReq();
		MessageHeader	header;

		short			cross_activity_type;
		short			param;
		unsigned short	param_2;
		unsigned short	param_3;
		
		unsigned short	sos_pos_x;  // ���͵��������(x,y) �� ����������Ľ����ʹ�(0,0)
		unsigned short  sos_pos_y;
	};

	class SCCrossEnterServer
	{
	public:
		SCCrossEnterServer();
		MessageHeader	header;

		int				cross_activity_type;
		char			login_server_ip[64];
		int				login_server_port;
		PlatName		pname;
		unsigned int	login_time;
		LoginStr		login_str;
		char			anti_wallow;
		char			reserved1;
		short			server;
	};

	/////////////////////////////////////////���������/////////////////////////////////

	class CSCrossXiuluoTowerJoinReq
	{
	public:
		CSCrossXiuluoTowerJoinReq();
		MessageHeader	header;
	};

	class SCCrossXiuluoTowerSelfActivityInfo		// 5701
	{
	public:
		SCCrossXiuluoTowerSelfActivityInfo();
		MessageHeader	header;

		short cur_layer;
		short max_layer;
		short immediate_realive_count;
		short boss_num;
		int total_kill_count;
		int kill_role_count;
		int cur_layer_kill_count;
		int reward_cross_honor;
		int score;
		int score_reward_flag;
		unsigned int refresh_boss_time;
		unsigned int gather_buff_end_timestamp;				// �ɼ�buff����ʱ���
	};

	class SCCrossXiuluoTowerRankInfo				// 5702
	{
	public:
		SCCrossXiuluoTowerRankInfo();
		MessageHeader		header;

		struct RankInfo
		{
			GameName user_name;
			unsigned short finish_time;
			short max_layer;
			char prof;
			char camp;
			short reserve_sh;
		};

		int count;																
		RankInfo rank_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];	
	};

	class SCCrossXiuluoTowerChangeLayerNotice		// 5703
	{
	public:
		SCCrossXiuluoTowerChangeLayerNotice();
		MessageHeader		header;

		int					is_drop_layer;
	};

	class SCCrossXiuluoTowerUserResult				// 5704
	{
	public:
		SCCrossXiuluoTowerUserResult();
		MessageHeader		header;

		char				max_layer;
		char				rank_pos;
		short				kill_role_count;
		int					reward_cross_honor;
	};

	class CSCrossXiuluoTowerBuyBuff
	{
	public:
		CSCrossXiuluoTowerBuyBuff();
		MessageHeader		header;

		short				is_buy_realive_count;
		short				reserve_sh;
	};

	class SCCrossXiuluoTowerInfo
	{
	public:
		SCCrossXiuluoTowerInfo();
		MessageHeader		header;

		int					buy_realive_count;
		short				add_gongji_per;
		short				add_hp_per;
		long long			uuid;
	};

	class SCCrossHonorChange															// �������ֵ�ı�
	{
	public:
		SCCrossHonorChange();																										
		MessageHeader		header;

		int honor;																		// ��ǰ����ֵ
		int delta_honor;																// �����ı���ֵ
	};

	class SCCrossXiuluoTowerBuffInfo
	{
	public:
		SCCrossXiuluoTowerBuffInfo();																										
		MessageHeader		header;

		ObjID obj_id;
		short buff_num;
		unsigned int next_send_reward_time;
	};

	class SCCrossXiuluoTowerGatherInfo
	{
	public:
		SCCrossXiuluoTowerGatherInfo();
		MessageHeader		header;

		struct InfoItem
		{
			int gather_id;
			int gather_count;
		};
		static const int MAX_INFO_COUNT = 4;

		int count;
		InfoItem info_list[MAX_INFO_COUNT];
	};

	class CSCrossXiuluoTowerScoreRewardReq
	{
	public:
		CSCrossXiuluoTowerScoreRewardReq();
		MessageHeader		header;

		int index;
	};

	class CSCrossXiuluoTowerBuyBuffReq			// 5735 ����������buff
	{
	public:
		CSCrossXiuluoTowerBuyBuffReq();
		MessageHeader header;

	};

	class SCCossXiuluoTowerRankTitleInfo   // 5741 ���������ǰ��������10�����
	{
	public:
		SCCossXiuluoTowerRankTitleInfo();
		MessageHeader header;
		struct RankInfo
		{
			long long uuid;
			GameName user_name;
			unsigned int  finish_time;
		};
		int rank_count;
		RankInfo rank_title_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];
	};

	class SCCossXiuluoTowerBossInfo //5742
	{
	public:
		SCCossXiuluoTowerBossInfo();
		MessageHeader header;
		int monster_id;
		long long max_hp;
		long long cur_hp;
	};

	/////////////////////////////////////  ���1v1  /////////////////////////////////////

	class SCCrossActivity1V1SelfInfo		// 5707
	{
	public:
		SCCrossActivity1V1SelfInfo();
		MessageHeader	header;

		int cross_score_1v1;                                                // ���1v1����
		int cross_day_join_1v1_count;                                       // ���ղ������

		short cross_1v1_join_time_reward_flag;                              // ���뽱��
		short cross_1v1_fetch_score_reward_flag;                            // ��ѫ����

		short today_buy_times;
		short cur_season;                                                   // ��ǰ�ڼ�����

		int cross_1v1_curr_activity_add_score;								// ���1v1��������ӵĻ���

		short cross_1v1_total_join_times;                                   // �����ܵĲ������
		short cross_1v1_total_win_times;                                    // �����ܵ�ʤ������

		int cross_1v1_gongxun;                                              // ������ѫ

		short cross_1v1_dur_win_times;                                      // ���������Ӯ����
		short resver_sh;

		char cross_1v1_use_ring[CROSS_ACTIVITY_1V1_RING_USE_COUNT];         // �����ָ

		char cross_1v1_have_ring[CROSS_1V1_SEASON_MAX];                      // ���н�ָ
	};

	enum CROSS1V1_FIGHT_TIME_TYPE
	{
		CROSS1V1_FIGHT_TIME_TYPE_INVALID = 0,		// 
		CROSS1V1_FIGHT_TIME_TYPE_START,				// ��ʼ
	};

	class SCCross1v1FightStart		// 5708 ���1V1ս����ʼ
	{
	public:
		SCCross1v1FightStart();
		MessageHeader header;

		int timestamp_type;						// ʱ������
		unsigned int fight_start_timestmap;		// ��ʼʱ��
		unsigned int fight_end_timestmap;		// ����ʱ��
	};

	class CSCrossMatch1V1Req	// 5753		����ƥ��
	{
	public:
		CSCrossMatch1V1Req();
		MessageHeader	header;
	};

	class CSCross1v1FightReady	// 5754  ׼��ս��
	{
	public:
		CSCross1v1FightReady();
		MessageHeader header;
	};	

	enum CROSS_1V1_FETCH_REWARD_TYPE
	{
		CROSS_1V1_FETCH_REWARD_TYPE_JOIN_TIMES = 1,		// �μӴ���
		CROSS_1V1_FETCH_REWARD_TYPE_SCORE,				// ����

		CROSS_1V1_FETCH_REWARD_TYPE_MAX,
	};

	class CSCross1v1FetchRewardReq	// 5755		��ȡ��������
	{
	public:
		CSCross1v1FetchRewardReq();
		MessageHeader	header;

		short fetch_type;			// ����
		short seq;					// ���
	};

	class CSCross1v1WearRingReq       // 5763    ��ָ����
	{
	public:
		CSCross1v1WearRingReq();
		MessageHeader header;

		int oper_type;					// ��������
		int ring_seq;					// ��ָ���
	};

	class CSCross1v1MatchResultReq       // 5764 ƥ��������
	{
	public:
		CSCross1v1MatchResultReq();
		MessageHeader header;

		int req_type;				// 0 : ���� 1��ȡ��

	};

	class CSCross1v1BuyTimeReq		// 5762 �������
	{
	public:
		CSCross1v1BuyTimeReq();
		MessageHeader header;
	};

	/////////////////////////////////////////���3V3/////////////////////////////////

	class CSCrossMultiuserChallengeMatchgingReq				//5756 ���3V3����ƥ��
	{
	public:
		CSCrossMultiuserChallengeMatchgingReq();
		MessageHeader	header;
	};

	class CSCrossMultiuserChallengeGetBaseSelfSideInfo		//5757 ���3V3�����ʼ������Ϣ
	{
	public:
		CSCrossMultiuserChallengeGetBaseSelfSideInfo();
		MessageHeader	header;
	};

	class CSCrossMultiuserChallengeFetchDaycountReward		//5758  ���3V3��ȡÿ�մ�������
	{
	public:
		CSCrossMultiuserChallengeFetchDaycountReward();
		MessageHeader	header;

		short index;		// <0 ȫ����ȡ   >0 ��ȡһ�ν���
		short reserve_sh;
	};

	class CSCrossMultiuerChallengeCancelMatching			//5759  ���3V3ȡ��ƥ��
	{
	public:
		CSCrossMultiuerChallengeCancelMatching();
		MessageHeader	header;
	};

	class SCCrossMultiuserChallengeBaseSelfSideInfo			//5712  ���3V3��ʼ������Ϣ
	{
	public:
		SCCrossMultiuserChallengeBaseSelfSideInfo();
		MessageHeader	header;

		struct UserInfo
		{
			int				plat_type;		//ƽ̨����
			int				server_id;		//��id
			int				uid;		
			GameName		user_name;		//�û���
			char			reserved;		//����
			char			sex;			//�Ա�
			char			prof;			//ְҵ
			char			camp;			//��Ӫ
			int				level;			//�ȼ�
			int				challenge_score;	// ���3V3����
			int				win_percent;		//ʤ��
			int				mvp_count;		 // ���3V3MVP����
			long long		capability;		 // ��ս��
		};

		int user_count;
		UserInfo user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	class SCCrossMultiuserChallengeSelfInfoRefresh			//5709	���3V3������Ϣ����
	{
	public:
		SCCrossMultiuserChallengeSelfInfoRefresh();
		MessageHeader		header;

		int self_side;				//��һ��	
		int kills;					//ɱ����
		int assist;					//������
		int dead;					//������
	};

	class SCCrossMultiuserChallengeMatchInfoRefresh		//5710  ���3V3������Ϣ����
	{
	public:
		SCCrossMultiuserChallengeMatchInfoRefresh();
		MessageHeader		header;

		int strong_hold_rate_info; //�ݵ������0-100
		int side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	class SCCrossMultiuserChallengeMatchState		//5711  ���3V3״̬֪ͨ
	{
	public:
		SCCrossMultiuserChallengeMatchState();
		MessageHeader	header;

		struct MatchUserInfo
		{
			short			plat_type;		//ƽ̨����
			ObjID			obj_id;
			int				role_id;		//��ɫid
			GameName		name;			//�û���
			short			prof;			//ְҵ
			short			sex;			//�Ա�
			short			kills;			//��ɱ��
			short			assist;			//������
			short			dead;			//������
			short			occupy;			//ռ����
			int				origin_score;	//ԭ������
			int				add_score;		//��û���
			int				add_honor;		//�����ҫ
			int				add_gongxun;	//��ù�ѫ
			int				is_mvp;			//�Ƿ���mvp
		};

		short				match_state;	//����״̬ 0 ����ʱ 1 ��ս 2 ����
		short				win_side;		//ʤ��
		unsigned int		next_state_time;//��һ״̬ʱ��
		MatchUserInfo		user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	class SCCrossMultiuserChallengeSelfActicityInfo			//5713  ���3V3���˻��Ϣ
	{
	public:
		SCCrossMultiuserChallengeSelfActicityInfo();
		MessageHeader	header;

		int challenge_mvp_count;						//MVP����
		int challenge_score;							//����
		int challenge_total_match_count;				//�ܴ���
		int challenge_win_match_count;					//ʤ������
		short win_percent;								//ʤ��
		short today_match_count;						//����μӴ���
		int matching_state;								//��ǰ����״̬ 0 ƥ������� 1 ƥ�������  2 ƥ�����  3 ƥ��ȡ��

		char join_reward_fetch_flag;								// ���3v3���뽱��ȡ���
		char rev_ch;
		short rev_sh;

		char cross_3v3_season_reward_use[CROSS_ACTIVITY_3V3_RING_USE_COUNT]; // ���3v3 ����ʹ�õ�����

		char cross_3v3_season_reward[CROSS_3V3_SEASON_MAX];         // ���3v3 �Ѿ����������

		int gongxun_reward_fetch_flag;								// ���3v3��ѫ����ȡ���
		int gongxun_value;											// ���3v3��ѫֵ

		int season_count;                                           // ����
	};

	class CSMultiuserChallengeReqSideMemberPos  // 5760���3v3����ȡ����λ����Ϣ
	{
	public:
		CSMultiuserChallengeReqSideMemberPos();
		MessageHeader		header;
	};

	struct SCMultiuserChallengeTeamMemberPosItem
	{
		int				role_id;		//�û�id
		ObjID			obj_id;
		char			reserved;
		char			is_leave_scene;	//�Ƿ��뿪
		short			pos_x;			//x����
		short			pos_y;			//y����
		Dir				dir;			//����
		Scalar			distance;		//�������
		int				move_speed;		//�ƶ��ٶ�
	};

	class SCMultiuserChallengeTeamMemberPosList  // 5714���3v3����ȡ����λ����Ϣ
	{
	public:
		SCMultiuserChallengeTeamMemberPosList();
		MessageHeader		header;

		int member_count;
		SCMultiuserChallengeTeamMemberPosItem member_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	class CSMultiuserChallengeWearCardReq           // 5736 ���3v3��������
	{
	public:
		CSMultiuserChallengeWearCardReq();
		MessageHeader header;

		int oper_type;  //1 ��� 2����
		int card_seq;	//��������
	};

	class CSCrossMultiuserChallengeFetchGongxunReward // 5737 ���3v3��ȡ��ѫ����
	{
	public:
		CSCrossMultiuserChallengeFetchGongxunReward();
		MessageHeader	header;

		short seq;    //<0 ȫ����ȡ   >= 0 ��ȡ��ѫ�����е�һ��
		short reserve_sh; //����
	};

	/////////////////////////////////////  �����ս  /////////////////////////////////////
	class CSCrossTuanzhanFetchReward											// ������ȡ����
	{
	public:
		CSCrossTuanzhanFetchReward();
		MessageHeader		header;
	};

	class SCCrossTuanzhanStateNotify											// ����״̬֪ͨ
	{
	public:
		SCCrossTuanzhanStateNotify();
		MessageHeader		header;

		unsigned int fight_start_time;											// ս����ʼʱ��
		unsigned int activity_end_time;											// �����ʱ��
		unsigned int rand_side_time;											// �����Ӫʱ��
	};

	class SCCrossTuanzhanPlayerInfo												// �����Ϣ
	{
	public:
		SCCrossTuanzhanPlayerInfo();
		MessageHeader		header;

		ObjID obj_id;
		short is_broacast;
		short side;																// ������Ӫ
		short score_reward_fetch_seq;
		unsigned int score;														// ����
		unsigned int kill_num;													// ��ɱ����
		unsigned int assist_kill_num;											// ��������
		unsigned int dur_kill_num;												// ��ɱ����
	};

	class SCCrossTuanzhanRankInfo												// ������Ϣ
	{
	public:
		SCCrossTuanzhanRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : side(CROSS_TUANZHAN_SIDE_MAX), reserve_sh(0), score(0), kill(0), assist(0)
			{
				memset(name, 0, sizeof(name));
			}

			short side;
			short reserve_sh;
			unsigned int score;
			GameName name;
			short kill;
			short assist;
		};

		int rank_list_count;
		RankItem rank_list[CROSS_TUANZHAN_RANK_LIST_SIZE];
	};

	class SCCrossTuanzhanSideInfo											// ��Ӫ������Ϣ
	{
	public:
		SCCrossTuanzhanSideInfo();
		MessageHeader	header;

		unsigned int side_score_list[CROSS_TUANZHAN_SIDE_MAX];
	};

	class SCCrossTuanzhanPillaInfo											// ͨ��������Ϣ
	{
	public:
		SCCrossTuanzhanPillaInfo();
		MessageHeader	header;

		struct PillaInfo
		{
			unsigned short monster_id;												// ���ӹ���id
			ObjID obj_id;													// ���ӵĶ���id
			short owner_side;												// ռ�����ӵ���Ӫ
			short index;
			GameName owner_name;											// ռ�����������
		};

		unsigned short pilla_list_count;
		short reserve_sh;
		PillaInfo pilla_list[CROSS_TUANZHAN_PILLA_MAX_COUNT]; 
	};

	class SCCrossTuanzhanPlayerDurKillInfo									// ��ɱ��Ϣ��� 
	{
	public:
		SCCrossTuanzhanPlayerDurKillInfo();
		MessageHeader	header;

		ObjID obj_id;														// ��Ҷ���id
		short reserve_sh;
		unsigned int dur_kill_num;											// ��ɱ����
	};

	class SCCrossTuanzhanResultInfo											// �������֪ͨ
	{
	public:
		SCCrossTuanzhanResultInfo();
		MessageHeader	header;

		unsigned int personal_score;										// ���˻���
		unsigned int side_score;											// ��Ӫ����
		short result;														// ���������0 ʧ�ܣ�1 ʤ��
		short reserve_sh;
	};

	/////////////////////////////////////  �������  /////////////////////////////////////

	class SCCrossPasturePlayerInfo											// ��������������Ϣ֪ͨ  5722
	{
	public:
		SCCrossPasturePlayerInfo();																										
		MessageHeader	header;

		int score;															// ��ǰ����
		short left_get_score_times;											// ʣ���ȡ���ִ���
		short reserve;
		int x;																// ������������x
		int y;																// ������������y
		unsigned int special_monster_refresh_time;							// �������ˢ��ʱ���
	};

	class SCCrossPastureRankInfo   //������������������Ϣ 5734
	{
	public:
		SCCrossPastureRankInfo();
		MessageHeader header;

		struct RankItem
		{
			int uid;
			GameName name;
			int score;
		};

		int rank_count;
		RankItem rank_list[CROSS_PASTURE_RANK_COUNT];
	};

	enum CROSS_PSATURE_ANIMAL_NOTIC_TYPE
	{
		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_FOLLOW = 0,
		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_BE_ROBBED,

		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_MAX
	};

	class SCCPPlayerHasAttachAnimalNotic									// ��������������״̬֪ͨ
	{
	public:
		SCCPPlayerHasAttachAnimalNotic();
		MessageHeader header;

		int notic_reason;
		GameName robber_name;
	};

	struct SCCrossGuildBattleSpecialTimeNotice	// 5733
	{
		SCCrossGuildBattleSpecialTimeNotice();
		MessageHeader header;

		int activty_status = 0;					// �״̬ 0���� 2����
		unsigned int end_timestamp = 0;			// ����ʱ��
	};

	/////////////////////////////////////  ���BOSS  /////////////////////////////////////
	enum CROSS_BOSS_OPERA_TYPE
	{
		CROSS_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// ˢ����Ϣ param1 ������Ϊ0��Ϊ���в㣩
		CROSS_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// ��ɱ��¼ param1 ���� param2 boss_id
		CROSS_BOSS_OPERA_TYPE_DROP_RECORD,			// �����¼	
		CROSS_BOSS_OPERA_TYPE_CONCERN_BOSS,			// ��עboss	param1 ���� param2 boss_id
		CROSS_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// ��עboss param1 ���� param2 boss_id
		CROSS_BOSS_OPERA_TYPE_FORENOTICE,			// boss֪ͨ

		CROSS_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossBossBossInfoReq											// ������boss��Ϣ   5730
	{
	public:
		CSCrossBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossBossBossInfoAck											// ���boss��Ϣ   5731
	{
	public:
		SCCrossBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_BOSS_SCENE_MAX];
	};

	class SCCrossBossPlayerInfo												// ���BOSS�����Ϣ 5724
	{
	public:
		SCCrossBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// ʣ��Boss��
		short left_treasure_crystal_gather_times;							// ʣ����ϧˮ���ɼ�����
		int left_ordinary_crystal_gather_times;								// ʣ����ͨˮ���ɼ�����

		unsigned int concern_flag[CROSS_BOSS_SCENE_MAX];					// ��עflag
	};

	class SCCrossBossSceneInfo												// ���boss������������Ϣ 5725
	{
	public:
		SCCrossBossSceneInfo();																										
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
		CrossBossInfo boss_list[MAX_CROSS_BOSS_PER_SCENE];					// Boss��Ϣ
	};

	class SCCrossBossBossKillRecord											// 5736 ���BOSS BOSS��ɱ��ʷ
	{
	public:
		SCCrossBossBossKillRecord();
		MessageHeader header;

		struct KillerRecrod
		{
			KillerRecrod() : uuid(0), dead_timestamp(0)
			{
				memset(killer_name, 0, sizeof(GameName));
			}

			long long uuid;
			unsigned int dead_timestamp;
			GameName killer_name;
		};

		int record_count;
		KillerRecrod killer_record_list[CROSS_BOSS_MAX_HISTROY_RECROD];
	};

	class SCCrossBossDropRecord												// 5737 ���BOSS ������ʷ
	{
	public:
		SCCrossBossDropRecord();
		MessageHeader header;

		struct DropItem
		{
			long long uuid;
			GameName role_name;
			unsigned int pickup_timestamp;
			int scene_id;
			unsigned short monster_id;
			ItemID item_id;
			int item_num;
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
		};

		int record_count;
		DropItem dorp_record_list[CROSS_BOSS_MAX_DROP_RECROD];
	};

	class CSCrossBossBuyReliveTimes											//���򸴻���� 5723
	{
	public:
		CSCrossBossBuyReliveTimes();
		MessageHeader	header;

		short buy_times;
		short reserve_sh;
	};

	class SCServerShutdownNotify											// �����������ر�֪ͨ 5726
	{
	public:
		SCServerShutdownNotify();																										
		MessageHeader	header;

		int remain_second;													// ��رշ�����ʣ������
	};

	/////////////////////////////////////  ���� ���ջ  /////////////////////////////////////
	enum RA_TOMORROW_REWARD_OPERA_TYPE
	{
		RA_TOMORROW_REWARD_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOMORROW_REWARD_OPERA_TYPE_FETCH_REWARD,

		RA_TOMORROW_REWARD_OPERA_TYPE_MAX,
	};

	class SCRATomorrowRewardInfo	// 2243
	{
	public:
		SCRATomorrowRewardInfo();
		MessageHeader header;

		int reward_count;									// ��ǰӵ�г齱����
		int reward_index;									// ��ǰ�鵽����Ʒ�±�
	};

	/////////////////////////////////////  ���� �ۼƳ�ֵ�  /////////////////////////////////////
	enum RA_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRATotalChargeInfo
	{
	public:
		SCRATotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// �ۼƳ�ֵ��
		int reward_has_fetch_flag;							// ����ȡ���Ľ������
	};

	//////////////////////////////////// ���� ��ʱ���ֶһ�ϡ��װ�� //////////////////////////////////////
	enum RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE
	{
		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_QUERY = 0,
		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_EXCHANGE,

		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_MAX,
	};

	class SCRATimeLimitExchangeEquiInfo
	{
	public:
		SCRATimeLimitExchangeEquiInfo();
		MessageHeader header;

		int ra_time_limit_exchange_exchange_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		//�һ�װ������
	};

	class SCRATimeLimitExchangeJLInfo
	{
	public:
		SCRATimeLimitExchangeJLInfo();
		MessageHeader header;

		int ra_time_limit_exchange_exchange_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		//�һ��������
	};

	/////////////////////////////////////  ���� ��齱�  /////////////////////////////////////
	enum RA_LEVEL_LOTTERY_OPERA_TYPE
	{
		RA_LEVEL_LOTTERY_OPERA_TYPE_QUERY_INFO = 0,					// ������ʷ��¼��Ϣ
		RA_LEVEL_LOTTERY_OPERA_TYPE_DO_LOTTERY,						// ����齱����
		RA_LEVEL_LOTTERY_OPERA_TYPE_FETCHE_TOTAL_REWARD,			// ��ȡ�ۼƳ齱��������
		RA_LEVEL_LOTTERY_OPERA_TYPE_ACTIVITY_INFO,					// ������Ϣ

		RA_LEVEL_LOTTERY_OPERA_TYPE_MAX,
	};

	class SCRALevelLotteryInfo			// 2400
	{
	public:
		SCRALevelLotteryInfo();
		MessageHeader header;

		int lottery_cur_level;										// ��ҵ�ǰ���ڲ�
		int history_count;											// ������ҳ齱��ʷ��¼����
		RALevelLotteryHistoryItem history_list[RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT]; // ������ҳ齱��ʷ
	};

	class SCRALevelLotteryRewardList	// 2401
	{
	public:
		SCRALevelLotteryRewardList();
		MessageHeader header;

		short reward_count;											 // ����������
		short reserve;

		char lottery_reward_list[RA_LEVEL_LOTTERY_REWARD_MAX_COUNT]; // �齱�Ľ���ID�б�

		UNSTD_STATIC_CHECK(0 == (RA_LEVEL_LOTTERY_REWARD_MAX_COUNT % 4))
	};

	class SCRALevelLotteryActivityInfo	// 2453
	{
	public:
		SCRALevelLotteryActivityInfo();
		MessageHeader header;

		int ra_level_lottery_free_buy_times;							// ÿ����ѹ������
		unsigned int ra_lottery_next_free_timestamp;					// �´���ѹ���ʱ��
		int ra_lottery_buy_total_times;									// �ۼƹ������
		unsigned int ra_lottery_fetch_reward_flag;						// ��ȡ������־
	};

	/////////////////////////////////////  ���� ��ֵŤ�����ۻ  /////////////////////////////////////
	static const int SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT = 10;

	enum RA_CHONGZHI_NIU_EGG_OPERA_TYPE
	{
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_QUERY_INFO = 0,				// ������Ϣ
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_CHOU,						// �齱
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_FETCH_REWARD,				// ��ȡȫ������

		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_MAX,
	};

	class SCRANiuEggInfo											// 2402
	{
	public:
		SCRANiuEggInfo();
		MessageHeader header;

		int server_total_niu_egg_times;								// ��ڼ�ȫ����Ť������
		int cur_can_niu_egg_chongzhi_value;							// ʣ��ɳ齱�ĳ�ֵ���

		int server_reward_has_fetch_reward_flag;					// ��ǰ����ȡ�Ľ������
		int history_count;											// ������ҳ齱��ʷ��¼����

		RAChongzhiNiuEggHistoryItem history_list[RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT]; // ������ҳ齱��ʷ
	};

	class SCRANiuEggChouResultInfo									// 2403
	{
	public:
		SCRANiuEggChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// ��ȡ�Ľ�������
		short reward_req_list[SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT];	// ��ȡ�Ľ����б�

		UNSTD_STATIC_CHECK(0 == SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT % 2);
	};


	/////////////////////////////////////  ���� �ؾ�Ѱ���  ////////////////////////////////////

	enum RA_MIJINGXUNBAO_OPERA_TYPE
	{
		RA_MIJINGXUNBAO_OPERA_TYPE_QUERY_INFO = 0,				// ������Ϣ
		RA_MIJINGXUNBAO_OPERA_TYPE_TAO,							// �Ա�
		RA_MIJINGXUNBAO_OPERA_TYPE_FETCH_REWARD,				// ��ȡ�����۳齱�� param_1 = ��ȡ������������0��ʼ��

		RA_MIJINGXUNBAO_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBaoInfo
	{
	public:
		SCRAMiJingXunBaoInfo();
		MessageHeader header;

		unsigned int ra_mijingxunbao_next_free_tao_timestamp;
		
		int chou_times;
		int reward_flag;
	};

	static const int SC_MiJingXunBaoTao_MAX_TIMES = 50;

	class SCRAMiJingXunBaoTaoResultInfo
	{
	public:
		SCRAMiJingXunBaoTaoResultInfo();
		MessageHeader header;

		int count;
		short mijingxunbao_tao_seq[SC_MiJingXunBaoTao_MAX_TIMES];
	};

	///////////////////////////////////// ���� �ؾ�Ѱ��2	////////////////////////////////////

	enum RA_MIJINGXUNBAO2_OPERA_TYPE
	{
		RA_MIJINGXUNBAO2_OPERA_TYPE_QUERY_INFO = 0,				// ������Ϣ
		RA_MIJINGXUNBAO2_OPERA_TYPE_TAO,						// �Ա�

		RA_MIJINGXUNBAO2_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBao2Info			//2440
	{
	public:
		SCRAMiJingXunBao2Info();
		MessageHeader header;

		unsigned int ra_mijingxunbao_next_free_tao_timestamp;
	};

	class SCRAMiJingXunBao2TaoResultInfo		//2441
	{
	public:
		SCRAMiJingXunBao2TaoResultInfo();
		MessageHeader header;

		int count;
		short mijingxunbao_tao_seq[SC_MiJingXunBaoTao_MAX_TIMES];
	};

	/////////////////////////////////////  ���� ҡǮ���  /////////////////////////////////////
	static const int SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT = 30;

	enum RA_CHONGZHI_MONEY_TREE_OPERA_TYPE
	{
		RA_MONEY_TREE_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_MONEY_TREE_OPERA_TYPE_CHOU,						// �齱
		RA_MONEY_TREE_OPERA_TYPE_FETCH_REWARD,				// ��ȡȫ������

		RA_MONEY_TREE_OPERA_TYPE_MAX,
	};

	class SCRAMoneyTreeInfo									// 2411ҡǮ����Ϣ
	{
	public:
		SCRAMoneyTreeInfo();
		MessageHeader header;

		int money_tree_total_times;							// ��ڼ������ҡǮ����
		unsigned int money_tree_free_timestamp;				// �����ѳ齱ʱ��
		int server_total_pool_gold;							// ��ڼ佱�ؽ��
		int server_reward_has_fetch_reward_flag;			// ��ǰ����ȡ�Ľ������
	};

	class SCRAMoneyTreeChouResultInfo						//2412 ҡǮ��������Ϣ
	{
	public:
		SCRAMoneyTreeChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// ��ȡ�Ľ�������
		char reward_req_list[SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];	// ��ȡ�Ľ����б�
	};

	/////////////////////////////////////  ���� ҡǮ���2  /////////////////////////////////////
	class SCRAMoneyTree2Info	// 2442
	{
	public:
		SCRAMoneyTree2Info();
		MessageHeader header;

		int server_total_money_tree_times;						// ��ڼ�ȫ����ҡǮ����
		int server_total_pool_gold;								// ��ڼ佱�ؽ��
		int server_reward_has_fetch_reward_flag;				// ��ǰ����ȡ�Ľ������

	};

	class SCRAMoneyTree2ChouResultInfo		//2443
	{
	public:
		SCRAMoneyTree2ChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// ��ȡ�Ľ�������
		char reward_req_list[SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];	// ��ȡ�Ľ����б�
	};

	/////////////////////////////////////  ���� �����뷭�ƻ  ///////////////////////////////////
	enum RA_KING_DRAW_OPERA_TYPE
	{
		RA_KING_DRAW_OPERA_TYPE_QUERY_INFO = 0,					// ��������Ϣ
		RA_KING_DRAW_OPERA_TYPE_PLAY_ONCE,						// ֻ��һ������
		RA_KING_DRAW_OPERA_TYPE_PLAY_TIMES,						// ��������
		RA_KING_DRAW_OPERA_TYPE_REFRESH_CARD,					// ��������
		RA_KING_DRAW_OPERA_TYPE_FETCH_REWARD,					// ��ȡ�۳齱��

		RA_KING_DRAW_OPERA_TYPE_MAX
	};

	class SCRAKingDrawInfo										// ������ϢЭ�� 2420
	{
	public:
		SCRAKingDrawInfo();
		MessageHeader header;

		unsigned short draw_times[RA_KING_DRAW_LEVEL_COUNT];	// �齱�ܴ���
		short reserve_sh;
		int return_reward_flag;									// ��ȡ�۳��־
		RAKingDrawCardList card_list[RA_KING_DRAW_LEVEL_COUNT];	// ���м���ķ�����Ϣ
	};

	class SCRAKingDrawMultiReward								// ��η��Ƶ�Э�� 2421
	{
	public:
		SCRAKingDrawMultiReward();
		MessageHeader header;

		short reward_count;										// ��Ʒʵ������
		short reward_seq_list[RA_KING_DRAW_MAX_REWARD_COUNT];	// ��Ʒ��seq
		short is_use_item;										// �Ƿ�ʹ������Ʒ
	};

	/////////////////////////////////////  ���� �ۼƳ�ֵ3(������)  ///////////////////////////////////
	class SCRATotalCharge3Info  // 2422
	{
	public:
		SCRATotalCharge3Info();
		MessageHeader		header;

		int cur_total_charge;
		int cur_total_charge_has_fetch_flag;

	};

	class CSRATotalCharge3Info // 2423  �ۼƳ�ֵ3������ǰ��ҳ�ֵ��Ϣ
	{
	public:
		CSRATotalCharge3Info();
		MessageHeader		header;
	};

	/////////////////////////////////////  ���� �����������а�  /////////////////////////////////////

	class SCRASpecialAppearancePassiveInfo
	{
	public:
		SCRASpecialAppearancePassiveInfo();
		MessageHeader header;

		int role_change_times;												// ��ǰ��ҵı��������
		int rank_count;														// ���а���Ҹ���
		RASpecialAppearacePassivePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT];// ���а��б�
	};

	/////////////////////////////////////  ���� �������а�  /////////////////////////////////////

	class SCRASpecialAppearanceInfo
	{
	public:
		SCRASpecialAppearanceInfo();
		MessageHeader header;

		int role_change_times;												// ��ǰ��ҵı������
		int rank_count;														// ���а���Ҹ���
		RASpecialAppearacePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT];// ���а��б�
	};

	/////////////////////////////////////  ���� �ۼƳ�ֵ4(��������)  /////////////////////////////////////

	class CSRATotalCharge4Info // 2424  �ۼƳ�ֵ4������ǰ��ҳ�ֵ��Ϣ
	{
	public:
		CSRATotalCharge4Info();
		MessageHeader		header;
	};

	class SCRATotalCharge4Info  // 2425
	{
	public:
		SCRATotalCharge4Info();
		MessageHeader		header;

		int cur_total_charge;
		int cur_consume_reward;                              //��ǰ���ĵĽ���
		int ra_total_charge_4_cur_has_fetch_flag;           //��ǰ������ȡ��־

	};

	/////////////////////////////////////  ���� �ڿ�  ///////////////////////////////////
	enum RA_MINE_OPERA_TYPE
	{
		RA_MINE_OPERA_TYPE_QUERY_INFO = 0,					// ��������Ϣ
		RA_MINE_OPERA_REFRESH,								// ��������
		RA_MINE_OPERA_GATHER,								// �ڿ�����
		RA_MINE_OPERA_FETCH_SERVER_REWARD,					// ��ȡȫ����������
		RA_MINE_OPERA_EXCHANGE_REWARD,						// �һ�����

		RA_MINE_OPERA_TYPE_MAX,
	};

	class SCRAMineAllInfo	// 2426
	{
	public:
		SCRAMineAllInfo();
		MessageHeader header;

		int total_refresh_times;							// ȫ��ˢ�����
		int role_refresh_times;								// ���ˢ�����
		int free_gather_times;								// ʣ������ڴ���
		int next_refresh_time;								// ��һ��ϵͳ�Զ�ˢ��ʱ��
		int cur_reward_fetch_flag;							// ��ǰȫ�������ȡ���
		int gather_count_list[RA_MINE_MAX_TYPE_COUNT];		// ��ǰ�ڵ��Ŀ�ʯ��
		char mine_cur_type_list[RA_MINE_MAX_REFRESH_COUNT];	// ��ǰ�󳡵Ŀ�ʯ
		short reward_begin_index;							// �����ȡ��ʼ�±�
		short reward_end_index;								// �����ȡ�����±�
	};

	/////////////////////////////////////  ���� ���ιλ  ///////////////////////////////////
	enum RA_GUAGUA_OPERA_TYPE
	{
		RA_GUAGUA_OPERA_TYPE_QUERY_INFO = 0,					// ��������Ϣ
		RA_GUAGUA_OPERA_TYPE_PLAY_TIMES,						// �ν����
		RA_GUAGUA_OPREA_TYPE_FETCH_REWARD,						// ��ȡ�ۼƽ��� p1 = ����

		RA_GUAGUA_OPERA_TYPE_MAX
	};

	class SCRAGuaGuaInfo														// 2428
	{
	public:
		SCRAGuaGuaInfo();
		MessageHeader header;

		unsigned short guagua_acc_count;										// �ۼƳ齱����
		unsigned short guagua_acc_reward_has_fetch_flag;						// �Ѿ���ȡ���
	};

	class SCRAGuaGuaMultiReward													// 2429
	{
	public:
		SCRAGuaGuaMultiReward();
		MessageHeader header;

		int reward_count;														// �ǽ�����seq����Ŀ�����Ǿ������Ʒ��
		int is_bind;
		short reward_seq_list[RA_GUAGUA_MAX_REWARD_COUNT];					// ��Ʒ��seq��ֵ-1����ο��
	};
	/////////////////////////////////////  ���� �������Ի  ///////////////////////////////////
	enum RA_TIANMING_DIVINATION_OPERA_TYPE
	{
		RA_TIANMING_DIVINATION_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_TIANMING_DIVINATION_OPERA_TYPE_ADD_LOT_TIMES,		// ��ǩ��ע
		RA_TIANMING_DIVINATION_OPERA_TYPE_RESET_ADD_LOT_TIMES,	// ������ǩ��ע����
		RA_TIANMING_DIVINATION_OPERA_TYPE_START_CHOU,			// ��ʼ��ǩ

		RA_TIANMING_DIVINATION_OPERA_TYPE_MAX				
	};

	class SCRATianMingDivinationInfo							// ����������Ϣ 2430
	{
		struct HistoryItem
		{
			char seq;											// �����±�
			char add_lot;										// �񽱼�ע����
		};

	public:
		SCRATianMingDivinationInfo();
		MessageHeader header;

		int	total_add_lot;																// ��ǰ�ܼ�ע��
		int free_chou_times;															// ����������Ѳ��Դ���
		char add_lots_list[RA_TIANMING_LOT_COUNT];										// ��ǩ��ע
		HistoryItem reward_history_item_count_list[RA_TIANMING_REWARD_HISTORY_COUNT];	// ����ʷ��¼
		unsigned short reward_history_list_cur_index;									// ��ʷ�б�������±�
	};

	class SCTianMingDivinationActivityStartChouResult			// �������Ի񽱽��  2431
	{
	public:
		SCTianMingDivinationActivityStartChouResult();
		MessageHeader header;

		short reward_index;										// ����Ʒ�±�
		short reserve_sh;
	};

	/////////////////////////////////////  ���� ����ת�  ///////////////////////////////////
	enum RA_FANFAN_OPERA_TYPE
	{
		RA_FANFAN_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_FANFAN_OPERA_TYPE_FAN_ONCE,				// ��һ����
		RA_FANFAN_OPERA_TYPE_FAN_ALL,				// ��ȫ����
		RA_FANFAN_OPERA_TYPE_REFRESH,				// ����
		RA_FANFAN_OPERA_TYPE_WORD_EXCHANGE,			// ����һ�
		RA_FANFAN_OPERA_TYPE_LEICHOU_EXCHANGE,		// �۳�һ�

		RA_FANFAN_OPERA_TYPE_MAX,
	};

	class SCRAFanFanAllInfo							// 2432
	{
	public:
		SCRAFanFanAllInfo();
		MessageHeader header;

		unsigned int ra_fanfan_next_refresh_time;									// ��һ������ʱ��
		unsigned char ra_fanfan_card_type_list[RA_FANFAN_CARD_COUNT];				// ���������б���ƷΪseqֵ�����ؿ������鿨��ö��ֵ
		RAFanFanWordActiveInfo ra_fanfan_word_active_info_list[RA_FANFAN_MAX_WORD_COUNT];	// ���鼤����Ϣ�б�
		RAFanFanHiddenWordInfo ra_fanfan_hidden_word_info;							// ��������Ϣ
		char ra_fanfan_cur_free_times;												// ��ǰ��Ѵ���
		char reserve_ch[3];
		int ra_fanfan_leichou_times;												// ����ת�۳����
		int ra_fanfan_reward_flag;													// ����ת������ȡ���
	};

	class SCRAFanFanWordExchangeResult				// 2433
	{
	public:
		SCRAFanFanWordExchangeResult();
		MessageHeader header;

		short index;					// ����
		short active_count;				// ������
	};

	/////////////////////////////////////  ���� ������ֵ  ///////////////////////////////////
	enum RA_CONTINUE_CHONGZHI_OPERA_TYPE
	{
		RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// ��ȡ����
		RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// ��ȡ���⽱��

		RA_CONTINUE_CHONGZHI_OPERA_TYPE_MAX				
	};

	class SCRAContinueChongzhiInfo			// 2434
	{
	public:
		SCRAContinueChongzhiInfo();
		MessageHeader header;

		unsigned int today_chongzhi;						// ���ճ�ֵ��
		short can_fetch_reward_flag;						// ���������� λ0������⽱��,����λ��Ǵ�꽱����ֵ0δ���ֵ1�Ѽ���
		short has_fetch_reward_falg;						// ������ȡ��� λ0������⽱��,����λ��Ǵ�꽱����ֵ0δ��ȡ��ֵ1����ȡ
		char continue_chongzhi_days;						// ������ֵ����
		char is_activity_over;								// �Ƿ�������ֵ�ж� 0δ�ж�	1�ж�
		short reserve_sh;
	};

	/////////////////////////////////////  ���� �����ػݳ�  ///////////////////////////////////
	class SCRAContinueChongzhiInfoChu			// 2454
	{
	public:
		SCRAContinueChongzhiInfoChu();
		MessageHeader header;

		unsigned int today_chongzhi;						// ���ճ�ֵ��
		short can_fetch_reward_flag;						// ����������
		short has_fetch_reward_falg;						// ������ȡ���
		char continue_chongzhi_days;						// ������ֵ����
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  ���� �����ػݸ�  ///////////////////////////////////
	class SCRAContinueChongzhiInfoGao			// 2455
	{
	public:
		SCRAContinueChongzhiInfoGao();
		MessageHeader header;

		unsigned int today_chongzhi;						// ���ճ�ֵ��
		short can_fetch_reward_flag;						// ����������
		short has_fetch_reward_falg;						// ������ȡ���
		char continue_chongzhi_days;						// �ۼƳ�ֵ����
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  ���� ��ʱ����  ///////////////////////////////////
	class SCRAContinueChongzhiInfo2			// 2466
	{
	public:
		SCRAContinueChongzhiInfo2();
		MessageHeader header;

		unsigned int today_chongzhi;						// ���ճ�ֵ��
		short can_fetch_reward_flag;						// ����������
		short has_fetch_reward_falg;						// ������ȡ���
		char continue_chongzhi_days;						// ������ֵ����
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  ���� ��������  ///////////////////////////////////
	enum RA_CONTINUE_CONSUME_OPERA_TYPE
	{
		RA_CONTINUME_CONSUME_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_REWARD,		// ��ȡ����
		RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// ��ȡ���⽱��

		RA_CONTINUE_CONSUME_OPERA_TYPE_MAX				
	};

	class SCRAContinueConsumeInfo			// 2435
	{
	public:
		SCRAContinueConsumeInfo();
		MessageHeader header;

		unsigned int today_consume_gold_total;				// �����ۼ�����
		unsigned int cur_consume_gold;						// �ϴ��콱�����ڵ�����
		char continue_consume_total_days;					// �ܴ������
		char continue_consume_days;							// ��ǰ�����������
		char current_day_index;								// ��ǰ����ڵڼ���
		char extra_reward_num;								// ����ȡ���⽱������
	};

	/////////////////////////////////////  ���� ��������  ///////////////////////////////////

	enum RA_ARMY_DAY_OPERA_TYPE
	{
		RA_ARMY_DAY_OPERA_TYPE_INFO = 0,					// ������Ϣ
		RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_FLAG,				// �һ�����
		RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_ITEM,				// �һ���Ʒ

		RA_ARMY_DAY_OPERA_TYPE_MAX
	};

	class SCRAArmyDayInfo						// 2436
	{
	public:
		SCRAArmyDayInfo();
		MessageHeader header;
		
		char belong_army_side;								// ��������
		char exchange_flag;									// ��Ծ�ȶһ����
		short reserver_sh;
		int army_day_own_flags_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// ӵ�����о���������
		int army_day_own_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// �������о��Ż�����
		int army_day_all_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// ȫ�����о��Ż�����
	};

	/////////////////////////////////////  ���� ѭ����ֵ  ///////////////////////////////////
	enum RA_CIRCULATION_CHONGZHI_OPERA_TYPE
	{
		RA_CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// ��ȡ����
		RA_CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// ��ȡ���⽱��

		RA_CIRCULATION_CHONGZHI_OPERA_TYPE_MAX				
	};

	class SCRACirculationChongzhiInfo							// 2437
	{
	public:
		SCRACirculationChongzhiInfo();
		MessageHeader header;

		unsigned int total_chongzhi;				// �ۼƳ�ֵ
		unsigned int cur_chongzhi;					// �ϴ��콱�����ڵĳ�ֵ
	};

	/////////////////////////////////////  ���� ��������  ///////////////////////////////////
	enum RA_EXTREME_LUCKY_OPERA_TYPE
	{
		RA_EXTREME_LUCKY_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_EXTREME_LUCKY_OPERA_TYPE_GLOD_FLUSH,					// ˢ��	
		RA_EXTREME_LUCKY_OPERA_TYPE_DRAW,					// �齱
		RA_EXTREME_LUCKY_OPREA_TYPE_AUTO_FLUSH,				// �Զ�ˢ��(����9��)
		RA_EXTREME_LUCKY_OPREA_TYPE_FETCH_REWARD,			// ��ȡ��������

		RA_EXTREME_LUCKY_OPERA_TYPE_MAX
	};

	class SCRAExtremeLuckyAllInfo							// 2438
	{
	public:
		SCRAExtremeLuckyAllInfo();
		MessageHeader header;

		unsigned int next_flush_timestamp;					// �´�ˢ��ʱ��
		short free_draw_times;								// ʣ����Ѵ���
		short cur_total_draw_times;							// ��ǰ���ܳ齱����(�������)
		int gold_draw_times;								// Ԫ���齱����
		short total_draw_times;								// �ܳ齱����
		short return_reward_flag;							// ������־

		RAExtremeLuckyRewardInfo cur_item_info_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];			// ��ǰ��Ʒ��Ϣ
	};

	class SCRAExtremeLuckySingleInfo					// 2439
	{
	public:
		SCRAExtremeLuckySingleInfo();
		MessageHeader header;

		short cur_total_draw_times;
		short gold_draw_times;
		short total_draw_times;								// �ܳ齱����
		short return_reward_flag;							// ������־
		int free_draw_times;
		RAExtremeLuckyRewardInfo single_item_info;
	};

	/////////////////////////////////////  ���� ���ȹ���  ///////////////////////////////////
	enum RA_NATIONAL_DAY_OPERA_TYPE
	{
		RA_NATIONAL_DAY_OPERA_TYPE_INFO = 0,
		RA_NATIONAL_DAY_OPERA_TYPE_EXCHANGE,			// �һ���Ʒ

		RA_NATIONAL_DAY_OPERA_TYPE_MAX,
	};

	/////////////////////////////////////  ���� �䱦��2  ///////////////////////////////////
	class SCRAZhenbaoge2Info								// 2444
	{
	public:
		SCRAZhenbaoge2Info();
		MessageHeader header;

		UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);
		short zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];
		short reserve_sh;
		short zhenbaoge_server_fetch_flag;
		short cur_server_flush_times;
		unsigned int zhenbaoge_next_flush_timetamp;
	};

	/////////////////////////////////////  ���� ��������  ///////////////////////////////////
	enum RA_PROMOTING_POSITION_OPERA_TYPE
	{
		RA_PROMOTING_POSITION_OPERA_TYPE_ALL_INFO,
		RA_PROMOTING_POSITION_OPERA_TYPE_PLAY,
		RA_PROMOTING_POSITION_OPERA_FETCH_REWARD,

		RA_PROMOTING_POSITION_OPERA_TYPE_MAX,
	};

	class SCRAPromotingPositionAllInfo	// 2445
	{
	public:
		SCRAPromotingPositionAllInfo();
		MessageHeader header;

		unsigned int next_free_timestamp;
		unsigned short play_times;
		RAPromotingPositionInfo start_pos;

		int reward_flag;
		int records_count;
		RAPromotingPositionRewardRecord record_list[RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT];
	};

	class SCRAPromotingPositionRewardInfo // 2446
	{
	public:
		SCRAPromotingPositionRewardInfo();
		MessageHeader header;

		int split_position;		// ˫��λ��
		int reward_count;
		RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2];
	};

	/////////////////////////////////////  ���� ��������  ///////////////////////////////////
	enum RA_BLACK_MARKET_OPERA_TYPE
	{
		RA_BLACK_MARKET_OPERA_TYPE_ALL_INFO,
		RA_BLACK_MARKET_OPERA_TYPE_OFFER,

		RA_BLACK_MARKET_OPERA_TYPE_MAX,
	};

	struct SCRABlackMarketAllInfo				// 2447
	{
		struct ItemInfo
		{
			ItemInfo()
			{
				seq = -1;
				cur_price = 0;
				buyer_uid = 0;
				memset(buyer_name, 0, sizeof(buyer_name));
			}

			int seq;						// ��Ʒ������seq
			int cur_price;					// ��ǰ�����۸�
			int buyer_uid;					// �������UID
			GameName buyer_name;			// ���������
		};

	public:
		SCRABlackMarketAllInfo();
		MessageHeader header;

		ItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT];
	};
	
	/////////////////////////////////////  ���� �䱦�̳�  ///////////////////////////////////
	enum RA_TREASURES_MALL_OPERA_TYPE
	{
		RA_TREASURES_MALL_OPERA_TYPE_REQ_INFO = 0,	// �䱦�̳�������Ϣ
		RA_TREASURES_MALL_OPERA_TYPE_BUY,		// �䱦�̳ǹ���
		RA_TREASURES_MALL_OPERA_TYPE_EXCHANGE,	// �䱦�̳Ƕһ�

		RA_TREASURES_MALL_OPERA_TYPE_MAX,
	};

	class SCRATreasuresMallAllInfo				// �䱦�̳�������Ϣ 2448
	{
	public:
		
		SCRATreasuresMallAllInfo();
		MessageHeader header;

		int cur_score;
	};

	/////////////////////////////////////  ������  ///////////////////////////////////
	class CSCrossTeamFBOption					// �����ӱ�����
	{
	public:
		CSCrossTeamFBOption();
		MessageHeader		header;

		int option_type;
		int layer;
		int room;
		int param;
		int param2;
		int param3;
	};

	class SCCrossTeamFBSelfInfo					// �����ӱ���Ϣ
	{
	public:
		SCCrossTeamFBSelfInfo();
		MessageHeader		header;

		int cross_team_fb_pass_flag;
		int cross_team_fb_day_count;
	};

	/////////////////////////////////////  ���� �����  ///////////////////////////////////
	enum RA_OPEN_SERVER_OPERA_TYPE
	{
		RA_OPEN_SERVER_OPERA_TYPE_REQ_INFO = 0,				// ����������Ϣ
		RA_OPEN_SERVER_OPERA_TYPE_FETCH,					// ��ȡ����

		RA_OPEN_SERVER_OPERA_TYPE_REQ_BOSS_INFO,			// ��ȡboss������Ϣ
		RA_OPEN_SERVER_OPERA_TYPE_FETCH_BOSS,				// ��ȡboss���ֽ���

		RA_OPEN_SERVER_OPERA_TYPE_FETCH_BATTE_INFO,			// ��ȡ����������Ϣ
	};	

	class SCRAOpenServerInfo						// �������Ϣ
	{
	public:
		SCRAOpenServerInfo();
		MessageHeader header;

		int rand_activity_type;
		int reward_flag;
		int complete_flag;
		int today_chongzhi_role_count;				// �׳��Ź���
	};

	class SCRAOpenServerUpgradeInfo
	{
	public:
		SCRAOpenServerUpgradeInfo();
		MessageHeader header;

		int rand_activity_type;
		int total_upgrade_record_list[MAX_UPGRADE_RECORD_COUNT];
	};

	class SCRAOpenServerRankInfo					// �����������Ϣ
	{
	public:
		SCRAOpenServerRankInfo();
		MessageHeader header;

		int rand_activity_type;
		int self_rank;
		int top1_uid;
		GameName top1_name;
		char top1_sex;
		char top1_prof;
		short reserve_2;
		int top1_param;
		long long top1_avatar_timestamp;

		int top1_grade;

		int top2_uid;
		GameName top2_name;
		int top2_grade;

		int top3_uid;
		GameName top3_name;
		int top3_grade;

	};

	class SCRAOpenServerBossInfo					// �����boss������Ϣ
	{
	public:
		SCRAOpenServerBossInfo();
		MessageHeader header;

		int oga_kill_boss_reward_flag;				// ����� ɱboss�������
		long long oga_kill_boss_flag;				// ����� ɱboss���

	};

	class CSRAOpenGameGiftShopBuy					// 6627�����޹����
	{
	public:
		CSRAOpenGameGiftShopBuy();
		MessageHeader header;

		int seq;
	};

	class CSRAOpenGameGiftShopBuyInfoReq			// 6628��ȡ�޹������Ϣ
	{
	public:
		CSRAOpenGameGiftShopBuyInfoReq();
		MessageHeader header;
	};
	
	class SCRAOpenGameGiftShopBuyInfo				// 6626����޹���Ϣ
	{
	public:
		SCRAOpenGameGiftShopBuyInfo();
		MessageHeader header;

		int oga_gift_shop_flag;						// ��������־
	};

	class SCRAOpenServerBattleInfo					// ����� ����������Ϣ
	{
	public:
		SCRAOpenServerBattleInfo();
		MessageHeader header;

		int qunxian_uid;							// Ԫ��ս��
		int guildbatte_uid;							// ��������
		int gongchengzhan_uid;						// ����ս
		int territorywar_uid;						// ����ս
	};

	class SCBaiBeiFanLiInfo							// �ٱ�������Ϣ
	{
	public:
		SCBaiBeiFanLiInfo();
		MessageHeader header;

		int is_buy;
		unsigned int close_time;					// ����ʱ���
	};

	class CSBaiBeiFanLiBuy							// �ٱ�����������Ʒ
	{
	public:
		CSBaiBeiFanLiBuy();
		MessageHeader header;
	};

	class SCBaiBeiFanLi2Info							//8918 �ٱ�����2��Ϣ
	{
	public:
		SCBaiBeiFanLi2Info();
		MessageHeader header;

		int is_buy;
		unsigned int close_time;					// ����ʱ���
	};

	class CSBaiBeiFanLi2Buy							//8919 �ٱ�����2������Ʒ
	{
	public:
		CSBaiBeiFanLi2Buy();
		MessageHeader header;
	};

	/////////////////////////////////////  ���ˮ���þ�  /////////////////////////////////////
	class SCCrossShuijingPlayerInfo											// ���ˮ���������Ϣ֪ͨ 6617
	{
	public:
		SCCrossShuijingPlayerInfo();
		MessageHeader		header;

		int total_bind_gold;												// �ܹ�-��Ԫ��
		int total_mojing;													// �ܹ�-ħ��
		int total_cross_honor;												// �ܹ�-�������
		int total_relive_times;												// �ܹ�-�������
		int cur_limit_gather_times;											// ��ǰ��-ʣ��Ŀɲɼ�����
		unsigned int gather_buff_time;										// �ɼ��������buffʱ��
		int big_shuijing_num;												// ����ˮ������
	};

	class CSCrossShuijingBuyBuff											// ���ˮ���þ�������buff 6618
	{
	public:
		CSCrossShuijingBuyBuff();
		MessageHeader		header;
	};

	/////////////////////////////////////  ˮ���þ�  /////////////////////////////////////

	class SCShuijingPlayerInfo												// ˮ���������Ϣ֪ͨ 6621
	{
	public:
		SCShuijingPlayerInfo();
		MessageHeader		header;

		int total_bind_gold;												// �ܹ�-��Ԫ��
		int total_mojing;													// �ܹ�-ħ��
		int total_shengwang;												// �ܹ�-����
		int free_relive_times;												// ����Ѹ������
		int cur_gather_times;												// ��ǰ�ɼ�����
		unsigned int gather_buff_time;										// �ɼ��������buffʱ��

		int big_shuijing_num;												// ����ˮ������
		unsigned int big_shuijing_next_flush_time;							// ����ˮ���´�ˢ��ʱ��
	};

	class SCShuijingSceneInfo												// ˮ��, ������Ϣ֪ͨ 6635
	{
	public:
		SCShuijingSceneInfo();
		MessageHeader		header;

		int big_shuijing_num;												// ����ˮ������
		unsigned int big_shuijing_next_flush_time;							// ����ˮ���´�ˢ��ʱ��
	};

	class SCShuijingTaskInfo												// ˮ����������Ϣ֪ͨ 6613
	{
	public:
		SCShuijingTaskInfo();
		MessageHeader		header;

		int gather_shuijing_total_num;				// �ɼ�����ˮ������
		int gather_big_shuijing_total_num;			// �ɼ������ˮ������
		int gather_diamond_big_shuijing_num;		// �ɼ���ʯ��ˮ������
		int gather_best_shuijing_count;				// �ɼ�����ˮ������
		unsigned int fetch_task_reward_flag;
	};

	class CSShuijingFetchTaskReward					// ˮ���þ�����ȡ������ 6632
	{
	public:
		CSShuijingFetchTaskReward();
		MessageHeader		header;

		int task_id;
	};

	class CSShuijingBuyBuff							// ˮ���þ�������buff 6622
	{
	public:
		CSShuijingBuyBuff();
		MessageHeader		header;
	};


	class SCNoticeGatherBuffInfo					// 6633�㲥�ɼ����ж�buff��Ϣ
	{
	public:
		SCNoticeGatherBuffInfo();
		MessageHeader		header;

		ObjID obj_id;
		short is_gather_wudi;			// 1��buff��0 û��buff
	};

	/////////////////////////////////////  ���� ���ֻ  ///////////////////////////////////
	enum RA_ITEM_COLLECTION_OPERA_TYPE
	{
		RA_ITEM_COLLECTION_OPERA_TYPE_QUERY_INFO = 0,					// ��������Ϣ
		RA_ITEM_COLLECTION_OPERA_TYPE_EXCHANGE,							// ����һ�
		RA_ITEM_COLLECTION_OPERA_TYPE_MAX,
	};

	/////////////////////////////////////  ħ���ر�  /////////////////////////////////////
	class CSFetchMagicalPreciousRewardReq	// 6624 ����
	{
	public:
		CSFetchMagicalPreciousRewardReq();
		MessageHeader		header;

		enum OPERATE_TYPE
		{
			FETCH_REWARD = 0,				// ��ȡ���� p1:�½��������� p2:�½�
			FETCH_ALL_INFO,					// ��ȡȫ����Ϣ
			CONVERT_CHAPTER_REWARD,			// �һ��½ڽ��� p1: �½� p2:��λ���
			FETCH_CUR_CHAPTER_INFO,			// ��ȡ��ǰ�½���Ϣ p2:�½�
		};

		int operater_type;					// ��������
		int param_1;						// 
		int param_2;						// 
	};

	class SCSendMagicalPreciousAllInfo			// 6625 ȫ����Ϣ
	{
	public:
		SCSendMagicalPreciousAllInfo();
		MessageHeader		header;

		int current_chaper;										// ��ǰ�½�		-2Ϊ�½�ȫ�����
		unsigned int chapter_invalid_time;						// �½�ʧЧʱ��
		unsigned int chapter_fetch_finish_reward_flag;			// �½���ɽ�����־
		unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // ÿ���½ڵĻ����б�
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];				// �½ڻ���״̬��־��0��δ��ɣ�1��������ҿ���ȡ���֣�2������ȡ����

		struct ChapterParam
		{
			ChapterParam() : charper(0), charper_index(0), param(0)
			{}

			ChapterParam(int _charper, int _charper_index, long long _param) : charper(_charper), charper_index(_charper_index), param(_param)
			{}

			int charper;
			int charper_index;
			long long param;
		};

		int param_list_len;
		ChapterParam param_list[MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];
	};

	class SCSendMagicalPreciousCurChapterInfo			// 6634 ��ǰ�½���Ϣ
	{
	public:
		SCSendMagicalPreciousCurChapterInfo();
		MessageHeader		header;

		int fetch_chapter;										// ��ȡ�������½�
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];		// �½ڽ�����־
	};

	class SCMagicalPreciousConditionParamChange			// 6636 ������ȸı�
	{
	public:
		SCMagicalPreciousConditionParamChange();
		MessageHeader		header;

		int param_list_len;
		unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // ÿ���½ڵĻ����б�
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];				// �½ڻ���״̬��־��0��δ��ɣ�1��������ҿ���ȡ���֣�2������ȡ����
		SCSendMagicalPreciousAllInfo::ChapterParam param_list[MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];
	};

	/////////////////////////////////////  ��������  /////////////////////////////////////
	enum RA_EXP_REFINE_OPERA_TYPE
	{
		RA_EXP_REFINE_OPERA_TYPE_BUY_EXP = 0,					// ����
		RA_EXP_REFINE_OPERA_TYPE_FETCH_REWARD_GOLD,				// ��ȡ���ƺ��
		RA_EXP_REFINE_OPERA_TYPE_GET_INFO,						// ��ȡ��Ϣ
		
		RA_EXP_REFINE_OPERA_TYPE_MAX
	};

	class CSRAExpRefineReq										// 6629 ������������
	{
	public:
		CSRAExpRefineReq();
		MessageHeader header;

		int opera_type;											// ��������
	};

	class SCRAExpRefineInfo										// 6630 ����������Ϣ
	{
	public:
		SCRAExpRefineInfo();
		MessageHeader header;

		short refine_today_buy_time;							// ÿ�����ƴ���
		short reserve_ch;
		int refine_reward_gold;									// �ܽ������
	};	

/////////////////////////////////////  �������  /////////////////////////////////////
	class SCRARedEnvelopeGiftInfo		// 8410
	{
	public:
		SCRARedEnvelopeGiftInfo();
		MessageHeader header;

		int consume_gold_num_list[RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT];	// ������ʯ��(0-6��)
		int reward_flag;													// �Ƿ��Ѿ���ȡ
	};

	///////////////////////////////////  ���ǽ���  /////////////////////////////////////
	enum RA_MARRYME_OPERA_TYPE
	{
		RA_MARRYME_REQ_INFO,
	};

	class SCRAMarryMeAllInfo			// 8415
	{
	public:
		SCRAMarryMeAllInfo();
		MessageHeader header;

		struct CopleDetail
		{
			CopleDetail()
			{
				propose_id = 0;
				memset(propose_name, 0, sizeof(propose_name));

				accept_proposal_id = 0;
				memset(accept_proposal_name, 0, sizeof(accept_proposal_name));

				proposer_sex = -1;
				accept_proposal_sex = -1;
			}

			int propose_id;									//�����id
			GameName propose_name;							//���������

			int accept_proposal_id;							//�������id
			GameName accept_proposal_name;					//�����������

			char proposer_sex;								//������Ա�
			char accept_proposal_sex;						//��������Ա�
			short reserve_sh;
		};

		int cur_couple_count;
		CopleDetail counp_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX];
	};

	/////////////////////////////////////  ��ֵ-���շ���  /////////////////////////////////////
	class CSChongZhi7DayFetchReward		// 8420
	{
	public:
		CSChongZhi7DayFetchReward();
		MessageHeader header;
	};

	/////////////////////////////////////  �����ٻ�		////////////////////////////////////

	class CSGoldenPigOperateReq								//8428 �����ٻ���������
	{
	public:
		CSGoldenPigOperateReq();
		MessageHeader header;

		enum GOLDEN_PIG_OPERATE_TYPE
		{
			GOLDEN_PIG_OPERATE_TYPE_REQ_INFO = 0,			// ������Ϣ
			GOLDEN_PIG_OPERATE_TYPE_SUMMON,					// �ٻ�
		};

		enum GOLDEN_PIG_SUMMON_TYPE
		{
			GOLDEN_PIG_SUMMON_TYPE_JUNIOR = 0,				// �����ٻ�
			GOLDEN_PIG_SUMMON_TYPE_MEDIUM,					// �м��ٻ�
			GOLDEN_PIG_SUMMON_TYPE_SENIOR,					// �߼��ٻ�

			GGOLDEN_PIG_SUMMON_TYPE_MAX,
		};

		short operate_type;									// ��������
		short param;										// �ٻ�����
	};

	class SCGoldenPigSendInfo								//8429 �����ٻ�������Ϣ
	{
	public:
		SCGoldenPigSendInfo();
		MessageHeader header;

		int summon_credit;									// �ٻ�����
		int current_chongzhi;								// ��ǰ��ֵ
		unsigned int activity_end_time;						// �����ʱ��
	};

	class SCGoldenPigBossState													// 8435 �����ٻ�boss״̬
	{
	public:
		SCGoldenPigBossState();
		MessageHeader header;

		char boss_state[CSGoldenPigOperateReq::GGOLDEN_PIG_SUMMON_TYPE_MAX];	// boss״̬ 0������,1����
		char reserve_ch;
	};

	////////////////////////////////////  ����Ͷ��	////////////////////////////////////

	enum RA_OPEN_SERVER_INVEST_TYPE
	{
		RA_OPEN_SERVER_INVEST_TYPE_INFO_REQ = 0,			// ������Ϣ
		RA_OPEN_SERVER_INVEST_TYPE_INVEST,					// Ͷ��
		RA_OPEN_SERVER_INVEST_TYPE_FECTH_REWARD,			// ��ȡ����
	};

	static const int MAX_INVEST_TYPE = 3;

	class SCOpenServerInvestInfo							// 8417 ����Ͷ����Ϣ
	{
	public:
		SCOpenServerInvestInfo();
		MessageHeader header;

		int reward_flag;									// ������Ϣ

		unsigned int time_limit[MAX_INVEST_TYPE];			// ����ʱ��
		char finish_param[MAX_INVEST_TYPE];					// Ͷ�������������
		char reserve_ch;
	};

	/////////////////////////////////// �������� //////////////////////////
	enum RA_SINGLE_CHONGZHI_OPERA_TYPE
	{
		RA_SINGLE_CHONGZHI_OPERA_TYPE_INFO = 0,				// ������Ϣ
		RA_SINGLE_CHONGZHI_OPERA_TYPE_FETCH_REWARD,			// ��ȡ����
		RA_SINGLE_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRASingleChongZhiInfo		// 2456
	{
	public:
		SCRASingleChongZhiInfo();
		MessageHeader header;

		int fetch_reward_flag;
		int is_fetch_reward_flag;
	};

	/////////////////////////////////////  �ۻ���  /////////////////////////////////////
	static const int SC_RA_XIANYUAN_TREAS_MAX_COUNT_LIMIT = 10;

	enum RA_XIANYUAN_TREAS_OPERA_TYPE
	{
		RA_XIANYUAN_TREAS_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_XIANYUAN_TREAS_OPERA_TYPE_BUY,						// ������������
		RA_XIANYUAN_TREAS_OPERA_TYPE_BUY_ALL,					// ȫ����������
		RA_XIANYUAN_TREAS_OPERA_TYPE_FETCH_REWARD,				// ��ȡ����

		RA_XIANYUAN_TREAS_OPERA_TYPE_MAX,
	};

	class SCRAXianyuanTreasInfo				// 2457
	{
	public:
		SCRAXianyuanTreasInfo();
		MessageHeader header;

		int all_buy_gift_fetch_flag;
		XianyuanTreasInfo xianyuan_list[RAND_ACTIVITY_XIANYUAN_TREAS_COUNT];
	};

	/////////////////////////////////////  ��ʱ��ɱ  ///////////////////////////////////
	enum RA_RUSH_BUYING_OPERA_TYPE
	{
		RA_RUSH_BUYING_OPERA_TYPE_QUERY_ALL_INFO,			// ����������Ϣ
		RA_RUSH_BUYING_OPERA_TYPE_BUY_ITEM,					// ������Ʒ

		RA_RUSH_BUYING_OPERA_TYPE_MAX,
	};

	class SCRARushBuyingAllInfo
	{
	public:
		struct ItemBuyTimes
		{
			ItemBuyTimes() : server_buy_times(0), role_buy_times(0) {}

			short server_buy_times;
			short role_buy_times;
		};

	public:
		SCRARushBuyingAllInfo();
		MessageHeader header;

		unsigned int buy_end_timestamp;

		short reserve_sh;
		char buy_phase;
		char item_count;

		ItemBuyTimes item_buy_times_list[RA_RUSH_BUYING_SALES_ITEMS_COUNT];
	};

	/////////////////////////////////////  ���� ��ͼѰ��  ///////////////////////////////////
	enum RA_MAP_HUNT_OPERA_TYPE
	{
		RA_MAP_HUNT_OPERA_TYPE_ALL_INFO = 0,		//����������Ϣ
		RA_MAP_HUNT_OPERA_TYPE_FLUSH,				//����ˢ��
		RA_MAP_HUNT_OPERA_TYPE_AUTO_FLUSH,			//�����Զ�ˢ��
		RA_MAP_HUNT_OPERA_TYPE_HUNT,				//Ѱ��
		RA_MAP_HUNT_OPERA_TYPE_FETCH_RETURN_REWARD,	//��ȡ��������

		RA_MAP_HUNT_OPERA_TYPE_MAX
	};

	class SCRAMapHuntAllInfo						//2459
	{
	public:
		SCRAMapHuntAllInfo();
		MessageHeader header;

		RAMapHuntRouteInfo route_info;				//·����Ϣ
		int flush_times;							//ˢ�´���
		unsigned int next_flush_timestamp;			//�´�ˢ��ʱ��
		short return_reward_fetch_flag;				//������ȡ��־
		short cur_has_used_free_count;				//��ǰ��ʹ����Ѵ���
		int can_extern_reward;						//·������
	};

	/////////////////////////////////////  ���� ����Ѱ��  ///////////////////////////////////
	enum RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE
	{
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_INFO = 0,					//����������Ϣ
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_DRAW,						//�齱
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_EXTERN_REWARD,		//��ȡ����⽱��
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_SERVER_REWARD,		//��ȡȫ������

		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_MAX
	};

	class SCRALightTowerExploreInfo			// 2460 ����Ѱ��������Ϣ
	{
	public:
		SCRALightTowerExploreInfo();
		MessageHeader header;

		char light_tower_info[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];
		char extern_reward_fetch_flag;
		char server_reward_fetcht_flag;
		char cur_index;

		short cur_layer;
		short reserver_sh;
		int draw_times;
	};

	class SCRALightTowerExploreDrawInfo		// 2461 �����齱������Ϣ
	{
	public:
		SCRALightTowerExploreDrawInfo();
		MessageHeader header;

		int reward_count;
		LightTowerExploreGridInfo reward_list[MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT];
	};

	/////////////////////////////////////  ���� ���ۼƳ�ֵ�  /////////////////////////////////////
	enum RA_NEW_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRANewTotalChargeInfo	//2262
	{
	public:
		SCRANewTotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// �ۼƳ�ֵ��
		int reward_has_fetch_flag;							// ����ȡ���Ľ������
	};

	class SCRATotalChargeMultiInfo	//2286
	{
	public:
		SCRATotalChargeMultiInfo();
		MessageHeader header;

		int activity_type;
		int total_charge_value;								// �ۼƳ�ֵ��
		int reward_has_fetch_flag;							// ����ȡ���Ľ������
	};

	/////////////////////////////////////  ���� ˫��Ԫ��  /////////////////////////////////////

	class SCRADoubleGetInfo	//2287
	{
	public:
		SCRADoubleGetInfo();
		MessageHeader header;

		int double_get_reward_fetch_flag;							// �ѻ�ý������
	};

	class SCSingleChargeInfoMulti // 2288
	{
	public:
		SCSingleChargeInfoMulti();
		MessageHeader hearder;

		int activtiy_type;
		int single_charge_reward_fetch_flag; // ����ȡ���������
	};
	/////////////////////////////////////  ���� ��װ�̵�  /////////////////////////////////////
	enum RA_MAGIC_SHOP_OPERA_TYPE
	{
		RA_MAGIC_SHOP_OPERA_TYPE_ALL_INFO,				//������Ϣ
		RA_MAGIC_SHOP_OPERA_TYPE_BUY,			        //���� param1 = ���� (0 ��һ���� 1һ������) param2 = ����

		RA_MAGIC_SHOP_OPERA_TYPE_MAX,
	};

	class SCRAMagicShopAllInfo			// 2463
	{
	public:
		SCRAMagicShopAllInfo();
		MessageHeader header;

		char magic_shop_buy_flag;
		char res_ch;
		short activity_day;
	};

	/////////////////////////////////////  ���� Ȥζֲ��  ///////////////////////////////////
	enum RA_PLANTING_TREE_OPERA_TYPE
	{
		RA_PLANTING_TREE_OPERA_TYPE_RANK_INFO,				// �������а���Ϣ
		RA_PLANTING_TREE_OPERA_TYPE_TREE_INFO,				// ����һ��������Ϣ
		RA_PLANTING_TREE_OPERA_MINI_TYPE_MAP_INFO,			// ����С��ͼ������Ϣ
	};

	class SCPlantingTreeRankInfo         // 8448
	{
	public:
		SCPlantingTreeRankInfo();
		MessageHeader header;

		int rank_type;
		int opera_times;

		int rank_list_count;
		RAPlantingTreeRankInfo rank_list[RA_TREE_PLANTING_RANK_MAX_COUNT];
	};

	class SCPlantingTreeTreeInfo         // 8449
	{
	public:
		SCPlantingTreeTreeInfo();
		MessageHeader header;

		GameName owner_name;
		unsigned int vanish_time;
		int watering_times;
	};

	static const int PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT = 5;
	class SCPlantingTreeMiniMapInfo             // 8450
	{
		struct TreeInfo
		{
			int obj_id;
			short pos_x;
			short pos_y;
		};

	public:
		SCPlantingTreeMiniMapInfo();
		MessageHeader header;

		int tree_count;
		TreeInfo tree_info_list[PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT];
	};

	/////////////////////////////////////////////// ���� //////////////////////////////////////////////
	enum RA_FISHING_OPERA_TYPE
	{
		RA_FISHING_REQ,							//�������
		RA_FISHING_ACK,							//������㷵��
		RA_FISHING_STEAL,						//����͵��
		RA_FISHING_STEAL_ACK,					//͵�㷵��
		RA_FISHING_EXCHANGE,					//����һ��㽱��
		RA_FISHING_CANCEL_AUTO_FISH,			//ȡ���Զ�����
		RA_FISHING_EXIT_FISHING,				//�˳�����
		RA_FISHING_ALL_INFO,					//����������Ϣ

		RA_FISHING_MAX_TYPE,
	};

	class SCRAFishAllinfo
	{
	public:
		SCRAFishAllinfo();
		MessageHeader header;

		short fish_times;							//����Ĵ���
		short steal_fish_times;
		short be_steal_fish_times;
		short steal_fish_type;
		short fish_score;
		short steal_fish_score;
		short fish_type;							//ÿ�ε�����������
		short fish_info[RA_FISHING_FISH_TYPE_COUNT + 1];
	};

	class SCRAFishStatusInfo
	{
	public:
		SCRAFishStatusInfo();
		MessageHeader header;

		char fish_status;
		char reserve_ch;
		ObjID obj_id;
	};

	////////////////////////////////////////////////// ����Ȼˮ�� /////////////////////////////////////
	enum RA_STEAL_COIN_OPERA_TYPE
	{
		RA_STEAL_CRTSTAL_DALARAN_REQ_INFO,
		RA_STEAL_CRYSTAL_DALARAN_REQ,
	};

	class SCRAStealCrystalDalatanInfo
	{
	public:
		SCRAStealCrystalDalatanInfo();
		MessageHeader header;

		unsigned int steal_count;
		unsigned int be_steal_count;
	};

	/////////////////////////////////////////////////��Ԫ��////////////////////////////////////////////////
	class SCRaCrazyRebateChongInfo		// 8463
	{
	public:
		SCRaCrazyRebateChongInfo();
		MessageHeader header;

		int chongzhi_count;
	};

	class SCLoveDailyInfo				// 8464
	{
	public:
		SCLoveDailyInfo();
		MessageHeader header;

		int flag;
	};

	////////////////////////////////////////////////ÿ��һ��2(���ʷ���)/////////////////////////////////////////
	class SCLoveDaily2Info				// 8923
	{
	public:
		SCLoveDaily2Info();
		MessageHeader header;

		int flag;
	};

	///////////////////////////////////////// �ºڷ�� ///////////////////////////////////////////////////////
	class SCCrossDarkNightUserInfo      // 5770
	{
	public:
		SCCrossDarkNightUserInfo();
		MessageHeader header;

		struct RewardItem
		{
			RewardItem():item_id(0),num(0){}
			ItemID item_id;
			short num;
		};

		struct RewardInfo
		{
			RewardItem reward_item;
			RewardItem reward_item2;
		};

		int score;
		int box_count;
		int total_reward_box_count;

		short is_finish;
		short reward_count;
		RewardInfo info_list[CROSS_DARK_NIGHT_BOSS_MAX_COUNT];
	};

	class SCCrossDarkNightRankInfo  //5771
	{
	public:
		SCCrossDarkNightRankInfo();
		MessageHeader header;

		struct RankItem
		{
			GameName name;
			int rank_val;
		};

		int rank_count;
		RankItem rank_list[CROSS_DARK_NIGHT_RANK_COUNT];
	};

	class SCCrossDarkNightBossInfo   //5772
	{
	public:
		SCCrossDarkNightBossInfo();
		MessageHeader header;

		struct BossInfo
		{
			int monster_id;
			Posi posi;
			long long max_hp;
			long long cur_hp;
			short boss_status;
			short reserve_sh;
		};

		BossInfo boss_info[CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX];
	};

	class SCCrossDarkNightPlayerInfoBroadcast  //5773
	{
	public:
		SCCrossDarkNightPlayerInfoBroadcast();
		MessageHeader header;

		ObjID obj_id;
		short box_count;
	};

	class SCCrossDarkNightRewardTimestampInfo //5774
	{
	public:
		SCCrossDarkNightRewardTimestampInfo();
		MessageHeader header;

		unsigned int next_check_reward_timestamp;
	};

	class CSCrossDarkNightRankOpera  //5776
	{
	public:
		CSCrossDarkNightRankOpera();
		MessageHeader header;

		int opera_type;
	};

	class SCCrossDarkNightTopPlayerPosi  //5775
	{
	public:
		SCCrossDarkNightTopPlayerPosi();
		MessageHeader header;

		int obj_id;
		Posi posi;
	};

	//////////////////////////////////////////////����//////////////////////////////////////////////////

	class CSFairyBuddhaCardActivateReq                                      // ���𿨽��������� 6855
	{
	public:
		CSFairyBuddhaCardActivateReq();
		MessageHeader header;
		short card_type;
		short reserve_sh;
	};

	class SCFairyBuddhaCardActivateInfo                                      // ������Ϣ 6856
	{
	public:
		SCFairyBuddhaCardActivateInfo();
		MessageHeader header;

		unsigned int bronze_timestamp;       // ��ͭ��ʱ���
		unsigned int silver_timestamp;       // ������ʱ���
		unsigned int jewel_timestamp;        // ��ʯ��ʱ���
		short is_forever_open;               // �Ƿ����ü���
		short fairy_buddha_card_is_activate; // �����Ƿ񼤻�
		int   gold_bind_is_get_flage;        // ÿ�հ�Ԫ�Ƿ���ȡ
	};

	class CSFairyBuddhaCardGoldBindReq                                      // ������ȡÿ�հ�Ԫ 6857
	{
	public:
		CSFairyBuddhaCardGoldBindReq();
		MessageHeader header;
		short card_type;
		short reserve_sh;
	};

	/////////////////////////////////////////////////   ���ɴ���  /////////////////////////////////////////////////////////
	class CSGuildQuestionEnterReq		// 6860 ���������
	{
	public:
		CSGuildQuestionEnterReq();
		MessageHeader header;
	};

	class SCGuildQuestionPlayerInfo		// 6861 �����Ϣ
	{
	public:
		SCGuildQuestionPlayerInfo();
		MessageHeader	header;

		int uid;
		GameName name;
		long long exp;
		int guild_gongxian;
		int guild_score;
		int gather_count;			// �ɼ�����

		int true_uid;
		GameName true_name;
	};

	class SCGuildQuestionGuildRankInfo	// 6862 ����������Ϣ
	{
	public:
		SCGuildQuestionGuildRankInfo();
		MessageHeader		header;

		struct RankInfo
		{
			RankInfo() : guild_id(0), guild_score(0)
			{
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;					// ����ID
			GuildName guild_name;				// ��������
			int guild_score;					// ���ɻ���
		};

		int rank_count;
		RankInfo guild_rank_list[GUILD_QUESTION_SCORE_RANK_MAX];
	};

	class SCGuildQuestionQuestionInfo  // 6863 ��Ŀ��Ϣ
	{
	public:
		SCGuildQuestionQuestionInfo();
		MessageHeader		header;

		void Reset()
		{
			question_state = 0;
			question_state_change_timestamp = 0;
			question_index = 0;
			question_id = 0;
			question_end_timestamp = 0;
			memset(question_str, 0, sizeof(question_str));
		}

		int question_state;								// 0��׼���У�1����ʼ�ˣ�2����Ҫ����
		unsigned int question_state_change_timestamp;	// ����״̬�л�ʱ���

		int question_index;						// �ڼ���
		int question_id;						// ��ĿID
		unsigned int question_end_timestamp;	// ������Ŀ����ʱ���
		GuildQuestionStr question_str;			// ��Ŀ	
	};

	/////////////////////////////////////////////////  ����  /////////////////////////////////////////////////////////
	enum FISHING_OPERA_REQ_TYPE
	{
		FISHING_OPERA_REQ_TYPE_START_FISHING,					// ��ʼ���㣨���������棩
		FISHING_OPERA_REQ_TYPE_CASTING_RODS,					// �׸�
		FISHING_OPERA_REQ_TYPE_PULL_RODS,						// �ո�
		FISHING_OPERA_REQ_TYPE_CONFIRM_EVENT,					// ȷ�ϱ��ε����¼�
		FISHING_OPERA_REQ_TYPE_USE_GEAR,						// ʹ�÷���
		FISHING_OPERA_REQ_TYPE_BIG_FISH_HELP,					// ��æ������
		FISHING_OPERA_REQ_TYPE_STOP_FISHING,					// ֹͣ���㣨�뿪������棩
		FISHING_OPERA_REQ_TYPE_AUTO_FISHING,					// �Զ�����
		FISHING_OPERA_REQ_TYPE_RAND_USER,						// �����ɫ����
		FISHING_OPERA_REQ_TYPE_BUY_STEAL_COUNT,					// ����͵�����
		FISHING_OPERA_REQ_TYPE_RANK_INFO,					    // ����������а���Ϣ
		FISHING_OPERA_REQ_TYPE_STEAL_FISH,						// ͵������
		FISHING_OPERA_REQ_TYPE_EXCHANGE,						// �һ����� param1���һ�����±�
		FISHING_OPERA_REQ_TYPE_BUY_BAIT,						// ������� param1: �����������
		FISHING_OPERA_REQ_TYPE_SCORE_REWARD,					// ���ֽ���
	};

	class CSFishingOperaReq   // 8777 ����Э��
	{
	public:
		CSFishingOperaReq();
		MessageHeader header;

		int opera_type;				//��������
		int param1;					//����1
		int param2;					//����2
	};

	class SCFishingUserInfo  // 8778  �����Ϣ
	{
	public:
		SCFishingUserInfo();
		MessageHeader header;

		Int64 uuid;									// ���Ψһid
		char fishing_status;						// ����״̬
		unsigned char special_status_flag;			// ����״̬���Զ����㡢ʹ�÷����ȣ�
		char least_count_cfg_index;					// ˫��������͵���������������
		char is_fish_event;							// �Ƿ�������Ϲ�
		char is_consumed_auto_fishing;				// �Ƿ����ѹ��Զ�����
		char reserve_ch[3];							// ����3��char�ĳ���

		unsigned int auto_pull_timestamp;			// �Զ�����ʱ�����û�д����¼���Ϊ0
		unsigned int special_status_oil_end_timestamp;	// ����״̬���ͽ���ʱ���

		int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];	// ��������б�
		int gear_num_list[FISHING_GEAR_MAX_COUNT];		// ���������б�

		int steal_fish_count;						// ͵�����
		int be_stealed_fish_count;					// ��͵�����
		int buy_steal_count;						// ����͵�����

		int news_count;								// �¼�����
		FishingNews news_list[FISHING_NEWS_MAX];	// �¼��б�
	};

	class SCFishingCheckEventResult	//8779  �¼���Ϣ����
	{
	public:
		SCFishingCheckEventResult();
		MessageHeader header;

		short event_type;							// �¼�����
		short param1;
		short param2;
		short param3;
	};

	class SCFishingGearUseResult  //8780 ʹ�÷��������Ϣ����
	{
	public:
		SCFishingGearUseResult();
		MessageHeader header;

		short gear_type;				// ��������
		short param1;
		short param2;
		short param3;
	};

	class SCFishingEventBigFish //8781	��˵�еĴ���
	{
	public:
		SCFishingEventBigFish();
		MessageHeader header;

		int owner_uid;					// ��˵�еĴ�������id
	};

	class SCFishingTeamMemberInfo	//8782		�����Ա��Ϣ
	{
	public:
		SCFishingTeamMemberInfo();
		MessageHeader header;

		int member_count;				// ��������

		int member_uid_1;				// �������1��ɫid
		int least_count_cfg_index_1;	// ���1��˫�����������±�
		int fish_num_list_1[FISHING_FISH_TYPE_MAX_COUNT];	//��������б�

		int member_uid_2;
		int least_count_cfg_index_2;
		int fish_num_list_2[FISHING_FISH_TYPE_MAX_COUNT];

		int member_uid_3;
		int least_count_cfg_index_3;
		int fish_num_list_3[FISHING_FISH_TYPE_MAX_COUNT];
	};

	class SCFishingFishInfo		//8783						// �����Ϣ���������飩
	{
	public:
		SCFishingFishInfo();
		MessageHeader header;

		int uid;											// ���id
		int least_count_cfg_index;							// ˫�����������±�
		int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// ��������б�
	};

	class SCFishingRandUserInfo		//8784		��������Ϣ
	{
	public:
		SCFishingRandUserInfo();
		MessageHeader header;

		int user_count;												// �������
		FishingRandUserInfo user_info_list[FISHING_RAND_ROLE_NUM];	// ����б�
	};

	class SCFishingScoreInfo	//8785
	{
	public:
		SCFishingScoreInfo();
		MessageHeader header;

		int fishing_score;					// �������
	};

	class SCFishingStealResult		//8786		͵������Ϣ
	{
	public:
		SCFishingStealResult();
		MessageHeader header;

		short is_succ;						// �Ƿ�ɹ�
		short fish_type;					// �������
		int fish_num;						// �������
	};

	class SCFishingGetFishBrocast	//8787	// ����㲥��Ϣ
	{
	public:
		SCFishingGetFishBrocast();
		MessageHeader header;

		int uid;							// ���uid
		int get_fish_type;					// ����������
	};

	class SCFishingScoreRankList		// 8788		// �������������Ϣ
	{
	public:
		struct FishRankItem
		{
			GameName name;
			long long uid;							// ���id
			int total_score;						// �ܻ���
		};

		SCFishingScoreRankList();
		MessageHeader		header;

		int fish_rank_count;
		FishRankItem fish_rank_list[FISHING_SCORE_MAX_RANK];
	};

	class SCFishingScoreStageInfo	// 8789 ��ǰ���ֽ׶���Ϣ
	{
	public:
		SCFishingScoreStageInfo();
		MessageHeader header;

		int cur_score_stage;			//��ǰ�׶�
		int fishing_score;				//��ǰ�������
	};

	class SCFishingNotifyStatusChange		// 8790
	{
	public:
		SCFishingNotifyStatusChange();
		MessageHeader header;

		long long uuid;
		int obj_id;											// ����obj id
		int status;											// ״̬
		int	be_stealed_fish_count;							// ��͵������
		int	fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// ��ǰ���ϵĸ����������
	};

	struct SCFishingStealInfo		// 8791
	{
		SCFishingStealInfo();
		MessageHeader header;

		GameName thief_name;		// ��������
		GameName be_stolen_name;	// ��͵����
		short fish_type;			// ��͵������
		short fish_num;				// ��͵�������
	};

	struct SCFishingConfirmResult	// 8792
	{
		SCFishingConfirmResult();
		MessageHeader header;

		short confirm_type;
		unsigned short param_1;
		short param_2;
		short param_3;
	};

	////////////////////////////////////// ҹս���� /////////////////////////////////////////////

	class SCNightFightRoleInfo           // 8795 ҹս����������Ϣ
	{
	public:
		SCNightFightRoleInfo();
		MessageHeader header;

		int turn;
		int score;								// ����
		int total_score;						// �ܻ���
		int is_red_side;						// �Ƿ��Ǻ췽
		int rank;								// ����
		int total_rank;							// ������
		int kill_role_num;						// ��ɱ�����������
		unsigned int next_redistribute_time;	// �´η����� �� ���·���ʱ���
		unsigned int next_get_score_time;		// �´λ�ȡ����ʱ���
		unsigned int next_update_rank_time;		// �´θ�������ʱ���
		unsigned int kick_out_time;				// �ӳ��߳�ʱ��
		unsigned int next_flush_boss_time;		// �´�ˢ��bossʱ���
		int is_finish;							// ������� 0��û���� 1������
	};

	enum NIGHT_FIGHT_OPERA_TYPE
	{
		NIGHT_FIGHT_OPERA_TYPE_ENTER,			// ��������
		NIGHT_FIGHT_OPERA_TYPE_POSI_INFO,		// ��ȡ������Ϣ  param_1:rank
	};

	class CSNightFightOperaReq			  // 8796 ҹս��������
	{
	public:
		CSNightFightOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
	};

	class SCNightFightRankInfo           // 8797 ҹս����������Ϣ
	{
	public:
		SCNightFightRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 10;

		struct Rank
		{
			int score;
			GameName user_name;
			ObjID obj_id;
			short is_red_side;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCNightFightBossRankInfo           // 8802 ҹս����boss������Ϣ
	{
	public:
		SCNightFightBossRankInfo();
		MessageHeader header;

		const static int BOSS_RANK_ITEM_MAX_COUNT = 20;

		struct BossRank
		{
			long long user_key;
			char is_red_side;			// ��Ӫ
			char reserve_ch;
			short reserve_sh;
			GameName user_name;
			int hurt_per;				// ����˺���ֱ�
			long long hurt_val;			// ����˺�ֵ
		};

		int boss_left_hp_per;										// bossʣ��Ѫ����ֱ�

		int boss_rank_count;
		BossRank boss_rank_info_list[BOSS_RANK_ITEM_MAX_COUNT];
	};

	class SCNightFightReward			  // 8798 ҹս��������������Ϣ
	{
	public:
		SCNightFightReward();
		MessageHeader header;

		const static int MAX_RANK_COUNT = 16;

		int reward_list[MAX_RANK_COUNT];
	};

	class SCNightFightRedSideListInfo	  // 8799 ҹս���Ǻ췽����obj_id�б�
	{
	public:
		SCNightFightRedSideListInfo();
		MessageHeader header;

		const static int RED_SIDE_MEMBER_MAX_COUNT = 25;

		int red_side_count;
		ObjID red_side_list[RED_SIDE_MEMBER_MAX_COUNT];
	};

	class SCNightFightAllRoleScoreInfo	  // 8800 �Ҷ�ս����Ա������Ϣ
	{
	public:
		SCNightFightAllRoleScoreInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct RoleScoreInfo
		{
			ObjID obj_id;
			short score;
		};

		int role_count;
		RoleScoreInfo score_info_list[RANK_ITEM_MAX_COUNT];
	};
	
	class SCNightFightPlayerPosi  //8801 ������Ϣ
	{
	public:
		SCNightFightPlayerPosi();
		MessageHeader header;

		short rank;
		ObjID obj_id;
		Posi posi;
	};

	class SCNightFightTotalScoreRank  //8803 ҹս�����ܻ������а�
	{
	public:
		SCNightFightTotalScoreRank();
		MessageHeader header;

		const static int TOTAL_SOCRE_RANK_MAX = 10;

		struct TotalRank
		{
			int total_score;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		TotalRank total_rank_list[TOTAL_SOCRE_RANK_MAX];

	};


	////////////////////////////////////// �Ҷ�ս�� /////////////////////////////////////////////

	class SCMessBattleRoleInfo           // 8560 �Ҷ�ս��������Ϣ
	{
	public:
		SCMessBattleRoleInfo();
		MessageHeader header;

		int turn;								// �ڼ���
		int score;								// ����
		int rank;								// ����
		int total_score;						// �ܻ���
		int total_rank;							// ������
		int boss_hp_per;						// bossѪ���ٷֱ�
		unsigned int next_redistribute_time;	// �´η����� �� ���·���ʱ���
		unsigned int next_get_score_time;		// �´λ�ȡ����ʱ���
		unsigned int next_update_rank_time;		// �´θ�������ʱ���
		unsigned int kick_out_time;				// �ӳ��߳�ʱ��
		int is_finish;							// �������־ 0 ������1 δ����
		long long boss_max_hp;					// boss���Ѫ��
		long long boss_cur_hp;					// boss��ǰѪ��
	};

	class CSMessBattleEnterReq			  // 8561 �Ҷ�ս����������
	{
	public:
		CSMessBattleEnterReq();
		MessageHeader header;
	};

	class SCMessBattleRankInfo           // 8562 �Ҷ�ս������������Ϣ
	{
	public:
		SCMessBattleRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct Rank
		{
			int score;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleReward			  // 8563 �Ҷ�ս������������Ϣ
	{
	public:
		SCMessBattleReward();
		MessageHeader header;

		const static int MAX_RANK_COUNT = 16;

		int reward_list[MAX_RANK_COUNT];
	};

	class SCMessBattleHurtRankInfo	  // 8564 �Ҷ�ս���˺�������Ϣ
	{
	public:
		SCMessBattleHurtRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct Rank
		{
			int hurt_per;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleAllRoleScoreInfo	  // 8565 �Ҷ�ս����Ա������Ϣ
	{
	public:
		SCMessBattleAllRoleScoreInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct RoleScoreInfo
		{
			ObjID obj_id;
			short score;
		};

		int role_count;
		RoleScoreInfo score_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleToalScoreRank    // 8566 �Ҷ�ս���ܻ������а�
	{
	public:
		SCMessBattleToalScoreRank();
		MessageHeader header;
		const static int  TOTAL_SOCRE_RANK_MAX = 10;
		struct TotalScore
		{
			int total_score;
			GameName user_name;
			long long user_key;
		};
		int rank_count;
		TotalScore total_score_list[TOTAL_SOCRE_RANK_MAX];
	};

	////////////////////////////////////// �������ش� /////////////////////////////////////////////
	class SCCrossLieKunFBSceneInfo			//  8575 �������ش�������Ϣ
	{
	public:
		SCCrossLieKunFBSceneInfo();
		MessageHeader header;

		struct BossInfo
		{
			int boss_id;												// bossid
			ObjID boss_objid;											// boss��objid
			short reserve_sh;											// ����
		};

		short zone;														// ����
		short reserve_sh;												// ����
		BossInfo boss_info_list[LIEKUN_BOSS_TYPE_COUNT];				// boss��Ϣ
		int guild_id[LIEKUN_BOSS_TYPE_COUNT];							// ��������id
		unsigned int boss_next_flush_timestamp[LIEKUN_BOSS_TYPE_COUNT];	// boss�´�ˢ��ʱ��
	};

	class SCCrossLieKunFBLiveFlagInfo		//  8576 �������ش���boss�����Ϣ
	{
	public:
		SCCrossLieKunFBLiveFlagInfo();
		MessageHeader header;

		short zone;														// ����
		char is_main_live_flag;											// ��boss�����
		char reserve_ch;												// ����
	};

	class SCCrossLieKunFBBossHurtInfo	//8577 �������ش���BOSS�˺���Ϣ
	{
	public:
		SCCrossLieKunFBBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//�Լ���������
		long long own_guild_hurt;//�Լ������˺�
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class SCRAPerfectLoverInfo   
	{
	public:
		SCRAPerfectLoverInfo();
		MessageHeader header;

		void Reset() 
		{
			my_rank = 0;
			memset(lover_name, 0, sizeof(lover_name));

			perfect_lover_type_record_flag = 0;
			ra_perfect_lover_count = 0;
			memset(ra_perfect_lover_name_list, 0, sizeof(ra_perfect_lover_name_list));
		}

		int my_rank;
		GameName lover_name;

		short perfect_lover_type_record_flag;
		short ra_perfect_lover_count;

		GameName ra_perfect_lover_name_list[RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT * 2];
	};


	////////////////////////////////////// ȫ����� /////////////////////////////////////////////
	enum UPGRADE_TYPE
	{
		UPGRADE_TYPE_MOUNT,
		UPGRADE_TYPE_WING,
		UPGRADE_TYPE_SHIZHUANG_TYPE_BODY,
		UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI,
		UPGRADE_TYPE_FABAO,
		UPGRADE_TYPE_FOOTPRINT,
		UPGRADE_TYPE_HALO,
		UPGRADE_TYPE_QILINBI,				//�����
		UPGRADE_TYPE_SHENGONG,				//��Ů�⻷
		UPGRADE_TYPE_TOUSHI,				//ͷ��
		UPGRADE_TYPE_YAOSHI,				//����
		UPGRADE_TYPE_LINGTONG,				//��ͯ
		UPGRADE_TYPE_LINGGONG,				//�鹭
		UPGRADE_TYPE_LINGQI,				//����
		UPGRADE_TYPE_FIGHT_MOUNT,			//ս��
	};

	class SCQuanMinJinJieInfo	//8816
	{
	public:
		SCQuanMinJinJieInfo();
		MessageHeader header;

		int reward_flag;					// �������
		int grade;							// ����
	};

	////////////////////////////////////// ȫ���ܶ�Ա /////////////////////////////////////////////
	class SCUpgradeGroupeInfo	//8817
	{
	public:
		SCUpgradeGroupeInfo();
		MessageHeader header;

		int ra_upgrade_group_reward_flag;					// �������
		int count[10];										// ��������
	};

	////////////////////////////////////// �����콱 /////////////////////////////////////////////
	enum RA_CONSUM_GIFT_OPERA_TYPE 
	{
		RA_CONSUM_GIFT_OPERA_TYPE_INFO = 0,       // ������Ϣ
		RA_CONSUM_GIFT_OPERA_TYPE_FETCH,          // ��ȡ����    ����2  seq
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL,           // ��ʼҡ��
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD,    // ��ȡҡ������
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_TEN,       // ��ʼҡ��ʮ��
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD_TEN,    // ��ȡҡ������10��
	};

	class SCRAConsumGift			      //  �����콱��Ϣ   8467
	{
	public:
		SCRAConsumGift();
		MessageHeader header;

		int consum_gold;                  // ������Ŀ
		short act_theme;                  // ��������
		short left_roll_times;            // ʣ��ҡ������
		unsigned int reward_fetch_flag;   // �콱���
	};

	class SCRAConsumGiftRollReward        // �����콱ҡ������   8469
	{
	public:
		SCRAConsumGiftRollReward();
		MessageHeader header;

		char seq;                        // ��������
		char decade;                     // ʮλ��
		char units_digit;                // ��λ��
		char res_sh;                     // ����
	};

	class SCRAConsumGiftRollRewardTen        // �����콱ҡ������10��   8834
	{
	public:
		SCRAConsumGiftRollRewardTen();
		MessageHeader header;
		char seq[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                        // ��������
		char decade[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                     // ʮλ��
		char units_digit[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                // ��λ��
		char res_sh[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                     // ����
	};

	////////////////////////////////////// ÿ���޹� /////////////////////////////////////////////
	enum RA_LIMIT_BUY_OPERA_TYPE
	{
		RA_LIMIT_BUY_OPERA_TYPE_INFO = 0,  // ��Ϣ
		RA_LIMIT_BUY_OPERA_TYPE_BUY,       // ����
	};

	class SCRALimitBuyInfo             // 8470
	{
	public:
		SCRALimitBuyInfo();
		MessageHeader header;

		int act_type;                      // ���ջ����
		char had_buy_count[RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ];   // 4
	};

	////////////////////////////////////// �۱����� /////////////////////////////////////////////
	enum RA_COLLECT_TREASURE_OPERA_TYPE
	{
		RA_COLLECT_TREASURE_OPERA_TYPE_INFO = 0,  // ��Ϣ
		RA_COLLECT_TREASURE_OPERA_TYPE_ROLL,      // ��ʼҡ��
		RA_COLLECT_TREASURE_OPERA_TYPE_REWARD,    // ��ȡ����
	};

	class SCRACollectTreasureInfo          // 8471
	{
	public:
		SCRACollectTreasureInfo();
		MessageHeader header;

		short left_roll_times;
		short record_num;
		short had_join_times;
		short res_sh;
		CollectTreasureRollRecord join_record_list[RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT];   // 10
	};

	class SCRACollectTreasureResult        // 8472  ҡ������·�
	{
	public:
		SCRACollectTreasureResult();
		MessageHeader header;

		int seq;
	};

	////////////////////////////////////// �����۳� /////////////////////////////////////////////
	enum RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE
	{
		RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_INFO = 0,  // ��Ϣ
		RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_FETCH,     // �콱   seq
	};

	class SCRAHappyCumulChongzhiInfo          // 8473
	{
	public:
		SCRAHappyCumulChongzhiInfo();
		MessageHeader header;

		int chongzhi_num;        // ��ֵ����
		short act_type;          //����
		short res_sh;
		unsigned int fetch_reward_flag;  
	};

	////////////////////////////////////// ������ /////////////////////////////////////////////
	class SCRACriticalStrikeInfo			// ������   8468
	{
	public:
		SCRACriticalStrikeInfo();
		MessageHeader header;

		int act_type;                      // ���ջ����
	};

	////////////////////////////////////// ���׷��� /////////////////////////////////////////////
	enum RA_JINJIE_RETURN_OPERA_TYPE
	{
		RA_JINJIE_RETURN_OPERA_TYPE_INFO = 0,  // ��Ϣ
		RA_JINJIE_RETURN_OPERA_TYPE_FETCH,     // �콱   seq
	};

	class SCRAJinJieReturnInfo          // 8474
	{
	public:
		SCRAJinJieReturnInfo();
		MessageHeader header;

		int act_type;          //����
		unsigned int fetch_reward_flag;
	};

	/////////////////////////////////////  ���� ���ֻ�����  ///////////////////////////////////
	enum RA_CONSUME_FOR_GIFT_OPERA_TYPE
	{
		RA_CONSUME_FOR_GIFT_OPERA_TYPE_ALL_INFO,			// ����������Ϣ
		RA_CONSUME_FOR_GIFT_OPERA_TYPE_EXCHANGE_ITEM,		// �һ���Ʒ

		RA_CONSUME_FOR_GIFT_OPERA_TYPE_MAX,
	};

	class SCRAConsumeForGiftAllInfo		//5235
	{
	public:
		SCRAConsumeForGiftAllInfo();
		MessageHeader header;

		int total_consume_gold;			//������
		int cur_points;					//��ǰ����

		int item_count;					//��Ʒ����
		unsigned char item_exchange_times[RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT];	//��������,RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT = 20
	};
	///////////////////////////////////// ���� �ؾ�Ѱ��3	////////////////////////////////////

	enum RA_MIJINGXUNBAO3_OPERA_TYPE
	{
		RA_MIJINGXUNBAO3_OPERA_TYPE_QUERY_INFO = 0,				// ������Ϣ
		RA_MIJINGXUNBAO3_OPERA_TYPE_TAO,						// �Ա�
		RA_MIJINGXUNBAO3_OPERA_TYPE_FETCH_REWARD,				// ��ȡ�����۳齱�� param_1 = ��ȡ������������0��ʼ��

		RA_MIJINGXUNBAO3_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBaoInfo3  //5236  �ؾ�Ѱ��3��Ϣ
	{
	public:
		SCRAMiJingXunBaoInfo3();
		MessageHeader header;

		unsigned int ra_mijingxunbao3_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_MiJingXunBaoTao3_MAX_TIMES = 30;

	class SCRAMiJingXunBaoTaoResultInfo3  //5237 �ؾ�Ѱ��3�����Ϣ
	{
	public:
		SCRAMiJingXunBaoTaoResultInfo3();
		MessageHeader header;

		int count;
		short mijingxunbao3_tao_seq[SC_MiJingXunBaoTao3_MAX_TIMES];
	};

	///////////////////////////////////// ���� �����ҵ�	////////////////////////////////////

	enum RA_HUANLEZADAN_OPERA_TYPE
	{
		RA_HUANLEZADAN_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_HUANLEZADAN_OPERA_TYPE_TAO,						// �Ա�
		RA_HUANLEZADAN_OPERA_TYPE_FETCH_REWARD,				// ��ȡ�����۳齱�� param_1 = ��ȡ������������0��ʼ��

		RA_HUANLEZADAN_OPERA_TYPE_MAX,
	};

	class SCRAHuanLeZaDanInfo //5238
	{
	public:
		SCRAHuanLeZaDanInfo();
		MessageHeader header;

		unsigned int ra_huanlezadan_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLEZADAN_MAX_TIMES = 30;

	class SCRAHuanLeZaDanTaoResultInfo  //5239
	{
	public:
		SCRAHuanLeZaDanTaoResultInfo();
		MessageHeader header;

		int count;
		short huanlezadan_tao_seq[SC_HUANLEZADAN_MAX_TIMES];
	};

	///////////////////////////////////// ���� ����ҡ��	////////////////////////////////////

	enum RA_HUANLE_YAOJIANG_OPERA_TYPE
	{
		RA_HUANLEYAOJIANG_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_HUANLEYAOJIANG_OPERA_TYPE_TAO,						// �Ա�
		RA_HUANLEYAOJIANG_OPERA_TYPE_FETCH_REWARD,				// ��ȡ�����۳齱�� param_1 = ��ȡ������������0��ʼ��

		RA_HUANLEYAOJIANG_OPERA_TYPE_MAX,
	};

	class SCRAHuanLeYaoJiangInfo  // 5240
	{
	public:
		SCRAHuanLeYaoJiangInfo();
		MessageHeader header;

		unsigned int ra_huanleyaojiang_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLE_YAOJIANG_MAX_TIMES = 30;

	class SCRAHuanLeYaoJiangTaoResultInfo  // 5241
	{
	public:
		SCRAHuanLeYaoJiangTaoResultInfo();
		MessageHeader header;

		int count;
		short huanleyaojiang_tao_seq[SC_HUANLE_YAOJIANG_MAX_TIMES];
	};

	////////////////////////////////////////////////// ��ʱ���� /////////////////////////////////////////////////
	enum RA_LIMIT_TIME_REBATE_OPERA_TYPE
	{
		RA_LIMIT_TIME_REBATE_OPERA_TYPE_INFO,			// ��Ϣ
		RA_LIMIT_TIME_REBATE_OPERA_TYPE_FETCH_REWARD,	// ��ȡ����
	};

	class SCRALimitTimeRebateInfo			     // 8475 ��ʱ����
	{
	public:
		SCRALimitTimeRebateInfo();
		MessageHeader header;

		int cur_day_chongzhi;					// �����ֵ���	
		int chongzhi_days;						// ��ֵ����
		int reward_flag;						// �������
		int chongzhi_day_list[RA_LIMIT_TIME_REBATE_MAX_DAY];			// ��ֵ�б�
	};

	/////////////////////////////////////  ���� ��ʱ��� /////////////////////////////////////
	enum RA_TIMELIMIT_GIFT_OPERA_TYPE
	{
		RA_TIMELIMIT_GIFT_OPERA_TYPE_QUERY_INFO = 0,
		RA_TIMELIMIT_GIFT_OPERA_TYPE_FETCH_REWARD,

		RA_TIMELIMIT_GIFT_OPERA_TYPE_MAX,
	};

	enum RA_TIMELIMIT_GIFT_FETCH_TYPE
	{
		RA_TIMELIMIT_GIFT_FETCH_FIRST = 0,		// ��һ�ݽ�����ȡ����
		RA_TIMELIMIT_GIFT_FETCH_SECOND,			// �ڶ��ݽ�����ȡ����
		//RA_TIMELIMIT_GIFT_FETCH_THIRD,			// �����ݽ�����ȡ����

		RA_TIMELIMIT_GIFT_FETCH_END,
	};

	class SCRATimeLimitGiftInfo			// 8476   ��ʱ���
	{
	public:
		SCRATimeLimitGiftInfo();
		MessageHeader header;

		int reward_can_fetch_flag1;		// �ܷ���ȡ��һ�ν�����־
		int reward_fetch_flag1;			// �Ƿ���ȡ��һ�ν�����־
		int reward_can_fetch_flag2;		// �ܷ���ȡ�ڶ��ν�����־
		int reward_fetch_flag2;			// �Ƿ���ȡ�ڶ��ν�����־
		short join_vip_level;			// ����ʱvip�ȼ�
		short open_flag;				// �Ƿ������ʾ��־
		int begin_timestamp;			// ��ɫ������ʱ��
		int reward_can_fetch_flag3;		// �ܷ���ȡ�����ν�����־
		int reward_fetch_flag3;			// �Ƿ���ȡ�����ν�����־
	};

	/////////////////////////////////////  ���� ѭ����ֵ 2 ///////////////////////////////////
	enum CIRCULATION_CHONGZHI_OPERA_TYPE
	{
		CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// ��ȡ����

		CIRCULATION_CHONGZHI_OPERA_TYPE_MAX
	};

	class SCCirculationChongzhiInfo					// 8477  ѭ����ֵ2
	{
	public:
		SCCirculationChongzhiInfo();
		MessageHeader header;

		unsigned int total_chongzhi;				// �ۼƳ�ֵ
		unsigned int cur_chongzhi;					// �ϴ��콱�����ڵĳ�ֵ
	};

	/////////////////////////////////////  ���� ���ҡǮ�� /////////////////////////////////////
	enum RA_SHAKEMONEY_OPERA_TYPE
	{
		RA_SHAKEMONEY_OPERA_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_SHAKEMONEY_OPERA_TYPE_FETCH_GOLD,				// ��ȡԪ��

		RA_SHAKEMONEY_OPERA_TYPE_MAX,
	};

	class SCRAShakeMoneyInfo        // 8478   ���ҡǮ��
	{
	public:
		SCRAShakeMoneyInfo();
		MessageHeader header;

		int total_chongzhi_gold;
		int chongzhi_gold;
		int seq;                   // �·���������
	};

	/////////////////////////////////////  ���� ��ʱ���� /////////////////////////////////////
	enum RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE
	{
		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_QUERY_INFO = 0,
		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_BUY,

		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_MAX,
	};

	class SCRATimeLimitLuxuryGiftBagInfo			// 8479
	{
	public:
		SCRATimeLimitLuxuryGiftBagInfo();
		MessageHeader header;

		short is_already_buy;			      // �Ƿ��Ѿ�����
		short join_vip_level;			      // ����ʱvip�ȼ�
		int begin_timestamp;			      // ��ɫ������ʱ��
		int time_limit_luxury_gift_open_flag; //��ʼ���
	};

	enum RA_DOUBLE_CHONGZHI_OPERA_TYPE
	{
		RA_DOUBLE_CHONGZHI_OPERA_TYPE_INFO = 0,			// ������Ϣ

		RA_DOUBLE_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRAResetDoubleChongzhi      // 8480 ����ͬ��
	{
	public:
		SCRAResetDoubleChongzhi();
		MessageHeader header;

		int chongzhi_reward_flag;						// ˫����ֵ�����ı�־
		char open_flag;
		char res_ch;
		short res_sh;
	};

	///////////////////////////////// ���� - RMB�̵� //////////////////////////
	struct SCRARmbBugChestShopInfo		// 8481
	{
		SCRARmbBugChestShopInfo();
		MessageHeader header;

		char buy_count_list[RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT]; //64
	};

	///////////////////////////////// ���� - ���ѷ��� //////////////////////////
	enum RA_CONSUME_GOLD_REWARD_OPERATE_TYPE
	{
		RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_INFO = 0,		// ������Ϣ
		RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_FETCH = 1,		// ������ȡ����
	};

	class SCRAConsumeGoldRewardInfo					// 8482
	{
	public:
		SCRAConsumeGoldRewardInfo();
		MessageHeader header;

		int consume_gold;							// ���ѽ��
		char fetch_reward_flag;						// �Ƿ��Ѿ���ȡ����
		char vip_level;								// ��Ӧvip�ȼ�
		short activity_day;							// ��ڼ���
	};

	///////////////////////////	���±��		////////////////////////////
	enum RA_ITEM_COLLECTION_SECOND_OPERA_TYPE
	{
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_QUERY_INFO = 0,					// ��������Ϣ
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_EXCHANGE,							// ����һ�   param1  seq  
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_MAX,
	};

	class SCCollectSecondExchangeInfo			//8490 ���±��
	{
	public:
		SCCollectSecondExchangeInfo();
		MessageHeader		header;

		int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT]; // 5
	};

	/////////////////////////////////////  �汾�ۼƳ�ֵ  /////////////////////////////////////
	enum RA_VERSION_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRAVersionTotalChargeInfo	//8484
	{
	public:
		SCRAVersionTotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// �ۼƳ�ֵ��
		int reward_has_fetch_flag;							// ����ȡ���Ľ������
	};

	/////////////////////////////////////  �汾������ֵ  ///////////////////////////////////
	enum RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE
	{
		RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		RA_VERSION_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// ��ȡ����

		RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE_MAX
	};

	class SCRAVersionContinueChongzhiInfo			// 8485
	{
	public:
		SCRAVersionContinueChongzhiInfo();
		MessageHeader header;

		unsigned int today_chongzhi;						// ���ճ�ֵ��
		short can_fetch_reward_flag;						// ����������
		short has_fetch_reward_falg;						// ������ȡ���
		char continue_chongzhi_days;						// ������ֵ����
		char reserve1;
		short reserve2;
	};

	///////////////////////////////////// �汾� ����ҡ��2	////////////////////////////////////
	enum RA_HUANLE_YAOJIANG_2_OPERA_TYPE
	{
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_QUERY_INFO = 0,			// ������Ϣ
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_TAO,						// �Ա�
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_FETCH_REWARD,			// ��ȡ�����۳齱�� param_1 = ��ȡ������������0��ʼ��

		RA_HUANLEYAOJIANG_OPERA_2_TYPE_MAX,
	};

	class SCRAHuanLeYaoJiangTwoInfo        // 8486
	{
	public:
		SCRAHuanLeYaoJiangTwoInfo();
		MessageHeader header;

		unsigned int ra_huanleyaojiang_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLE_YAOJIANG_2_MAX_TIMES = 30;

	class SCRAHuanLeYaoJiangTwoTaoResultInfo     // 8487
	{
	public:
		SCRAHuanLeYaoJiangTwoTaoResultInfo();
		MessageHeader header;

		int count;
		short huanleyaojiang_tao_seq[SC_HUANLE_YAOJIANG_2_MAX_TIMES];
	};

	///////////////////////////////////// �汾� ���µ��ʳ�ֵ	////////////////////////////////////
	class SCRAOfflineSingleChargeInfo0			// 8488
	{
	public:
		SCRAOfflineSingleChargeInfo0();
		MessageHeader header;

		int activity_type;
		int charge_max_value;
		int reward_times[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];					// ��������
	};

	///////////////////////////////////////////////// ��һ��һ ////////////////////////////////////////////////
	enum RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE
	{
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_INFO,
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_BUY,
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_FETCH_REWARD,
	};

	class SCBuyOneGetOneFreeInfo		// 8483
	{
	public:
		SCBuyOneGetOneFreeInfo();
		MessageHeader header;

		long long buy_flag;				// ������
		long long free_reward_flag;		// ��ȡ�������
	};

	///////////////////////////////////////////////// �����ػ� ////////////////////////////////////////////////
	struct KillRankNoticeInfo
	{
		KillRankNoticeInfo() : uid(0), kill_monster_count(0), total_capablity(0), sex(0), prof(0), reserve_sh(0), pass_time(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		KillRankNoticeInfo & operator =(KillRankNoticeInfo *param)
		{
			this->uid = param->uid;
			memcpy(this->user_name, param->user_name, sizeof(this->user_name));
			this->kill_monster_count = param->kill_monster_count;
			this->total_capablity = param->total_capablity;
			this->sex = param->sex;
			this->prof = param->prof;
			this->pass_time = param->pass_time;

			return *this;
		}

		int uid;
		GameName user_name;
		int kill_monster_count;
		int total_capablity;
		char sex;
		char prof;
		short reserve_sh;
		unsigned int pass_time;
	};

	// �����ػ���ɱ������Ϣ
	struct SCRAHolidayGuardRanKInfo		// 8831
	{
		SCRAHolidayGuardRanKInfo();
		MessageHeader header;

		KillRankNoticeInfo kill_rank[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX];
	};

	// �����ػ� ���������·�
	struct CSRAHolidayGuardRankInfoReq		// 8832
	{
		CSRAHolidayGuardRankInfoReq();
		MessageHeader header;
	};

	struct NpcInfo
	{
		int npc_index;
		int scene_id;
		int npc_id;
		int npc_x;
		int npc_y;
	};

	struct SCRAExtremeChallengeNpcInfo	// 8833
	{
		SCRAExtremeChallengeNpcInfo();
		MessageHeader header;

		NpcInfo npc_info_list[HOLIDAYGUARD_NPC_CFG_MAX_COUNT];
	};

	/////////////////////////////////////  �汾� �ۼƳ�ֵ5(�汾��������)  /////////////////////////////////////

	class CSRATotalChargeFiveInfo // 8490   �ۼƳ�ֵ5������ǰ��ҳ�ֵ��Ϣ
	{
	public:
		CSRATotalChargeFiveInfo();
		MessageHeader		header;
	};

	class SCRATotalChargeFiveInfo  // 8491 
	{
	public:
		SCRATotalChargeFiveInfo();
		MessageHeader		header;

		int cur_total_charge;
		int cur_total_charge_has_fetch_flag;
	};

	///////////////////////////////////////////////// ������ս ////////////////////////////////////////////////
	enum EXTREMECHALLENGE
	{
		EXTREMECHALLENGE_INFO,						// ��Ϣ����
		EXTREMECHALLENGE_REFRESH_TASK,				// ˢ������    ����1 task_id
		EXTREMECHALLENGE_FETCH_REWARD,				// ��ȡ����    ����1 task_id
		EXTREMECHALLENGE_INIT_TASK,					// ��ʼ����
		EXTREMECHALLENGE_FETCH_ULTIMATE_REWARD,		// ��ȡ�ռ�����
	};

	struct SCRAExtremeChallengeTaskInfo		        // 8489
	{
		SCRAExtremeChallengeTaskInfo();
		MessageHeader header;

		RAExremeChallengeTaskInfo task_state[RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM];			// ����
		char is_have_fetch_ultimate_reward;

		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////////////////////// ������� ////////////////////////////////////////////////
	enum CHONGZHI_GIFT_OPER_TYPE
	{
		CHONGZHI_GIFT_OPER_TYPE_INFO = 0,               // ��Ϣ
		CHONGZHI_GIFT_OPER_TYPE_FETCH,                  // ��ȡ����    param_1 = ����
	};

	struct SCRAChongZhiGiftInfo		             // 8492
	{
		SCRAChongZhiGiftInfo();
		MessageHeader header;

		char magic_shop_fetch_reward_flag;
		char res_ch;
		short activity_day;
		unsigned int magic_shop_chongzhi_value;
	};

	///////////////////////////////////////////////// �Ϸ��--Ͷ�ʼƻ� ////////////////////////////////////////////////
	enum CSA_TOUZIJIHUA_OPERA
	{
		CSA_TOUZIJIHUA_INFO_REQ = 0,	// ������Ϣ
		CSA_TOUZIJIHUA_FETCH_REQ = 1,	// ��ȡ���� param_2 �������
	};

	class SCCSATouzijihuaInfo					//8493
	{
	public:
		SCCSATouzijihuaInfo();
		MessageHeader		header;

		char csa_touzijihua_buy_flag;										// �Ƿ���Ϸ�Ͷ�ʼƻ�
		char csa_touzijihua_reserve_ch;
		unsigned short csa_touzjihua_login_day;								// Ͷ�ʼƻ�ÿ�ս�����ȡ��־
		unsigned int csa_touzijihua_total_fetch_flag;						// Ͷ�ʼƻ���ȡ������־
	};

	///////////////////////////////////////////////// �Ϸ��--�ɳ����� ////////////////////////////////////////////////
	enum CSA_FOUNDATION_OPERA
	{
		CSA_FOUNDATION_INFO_REQ = 0,	// ������Ϣ
		CSA_FOUNDATION_FETCH_REQ = 1,	// ��ȡ������param_2�������
	};

	class SCCSAFoundationInfo					//8494
	{
	public:
		SCCSAFoundationInfo();
		MessageHeader		header;

		CSAFoundationStatus csa_foundation_status[COMBINE_SERVER_MAX_FOUNDATION_TYPE];	//����״̬ COMBINE_SERVER_MAX_FOUNDATION_TYPE = 10
	};

	///////////////////////////////////////////////// �Ϸ��--�������� ////////////////////////////////////////////////
	enum CSA_EXP_REFINE_OPERA_TYPE
	{
		CSA_EXP_REFINE_OPERA_TYPE_BUY_EXP = 0,					// ����
		CSA_EXP_REFINE_OPERA_TYPE_GET_INFO,						// ��ȡ��Ϣ

		CSA_EXP_REFINE_OPERA_TYPE_MAX
	};

	class SCCSAExpRefineInfo									// 8495
	{
	public:
		SCCSAExpRefineInfo();
		MessageHeader header;

		char had_buy;											// �Ƿ����
		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////////////////////// �Ϸ��--����ս ////////////////////////////////////////////////
	class SCCSAGongChengZhanInfo					//8496
	{
	public:
		SCCSAGongChengZhanInfo();
		MessageHeader		header;

		int win_times;							// ʤ������
	};

	////////////////////////////////////// ���׷���2 /////////////////////////////////////////////
	enum RA_JINJIE_RETURN2_OPERA_TYPE
	{
		RA_JINJIE_RETURN2_OPERA_TYPE_INFO = 0,  // ��Ϣ
		RA_JINJIE_RETURN2_OPERA_TYPE_FETCH,     // �콱   seq
	};

	class SCRAJinJieReturnInfo2        // 8498
	{
	public:
		SCRAJinJieReturnInfo2();
		MessageHeader header;

		int act_type;          //����
		unsigned int fetch_reward_flag;
	};

	////////////////////////////////////// ������2 /////////////////////////////////////////////
	class SCRACriticalStrike2Info			// ������2   8499
	{
	public:
		SCRACriticalStrike2Info();
		MessageHeader header;

		int act_type;                      // ���ջ����
	};

	////////////////////////////	��Ϲ���		//////////////////////////////
	enum RA_COMBINE_BUY_OPERA_TYPE
	{
		RA_COMBINE_BUY_OPERA_TYPE_INFO = 0,					// ������Ϣ
		RA_COMBINE_BUY_OPERA_TYPE_ADD_IN_BUCKET = 1,		// ���빺�ﳵ��param_1 = ��Ʒ����
		RA_COMBINE_BUY_OPERA_TYPE_REMOVE_BUCKET = 2,		// �Ƴ����ﳵ��param_1 = ���ﳵ����
		RA_COMBINE_BUY_OPERA_TYPE_BUY = 3,					// ����
	};

	class SCRACombineBuyBucketInfo			// 8835
	{
	public:
		SCRACombineBuyBucketInfo();
		MessageHeader header;

		int seq_list[RA_COMBINE_BUY_BUCKET_ITEM_COUNT];	// ���ﳵ��Ʒ����  = 6
	};

	class SCRACombineBuyItemInfo			//8836
	{
	public:
		SCRACombineBuyItemInfo();
		MessageHeader header;

		unsigned char buy_item_num[RA_COMBINE_BUY_MAX_ITEM_COUNT];	//������Ʒ���� RA_COMBINE_BUY_MAX_ITEM_COUNT = 96
	};

	/////////////////////////////	���췵��			////////////////////////////////
	enum DAY_CHONGZHI_REWARD_OPERA_TYPE
	{
		DAY_CHONGZHI_REWARD_OPERA_TYPE_QUERY_INFO = 0,		// ������Ϣ
		DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_REWARD,		// ������ȡ����	param_0 = ����
		DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_RARE_REWARD,	// ������ȡ��ϡ����	param_0 = ����

		DAY_CHONGZHI_REWARD_OPERA_TYPE_MAX,
	};

	class CSDayChongzhiRewardReq			// 8837
	{
	public:
		CSDayChongzhiRewardReq();
		MessageHeader header;

		short opera_type;
		short param_0;
	};

	class SCDayChongzhiRewardInfo			// 8838
	{
	public:
		SCDayChongzhiRewardInfo();
		MessageHeader header;

		int day_count;															// �ۼƳ�ֵ����
		unsigned int reward_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];		// ��ȡ������־ DAY_CHONGZHI_REWARD_FLAG_LIST_LEN = 4
		unsigned int rare_reward_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];	// ��ȡ��ϡ������־
	};

	/////////////////////////////////////  ����ز�BOSS  /////////////////////////////////////
	enum CROSS_MIZANG_BOSS_OPERA_TYPE
	{
		CROSS_MIZANG_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// ˢ����Ϣ param1 ������Ϊ0��Ϊ���в㣩
		CROSS_MIZANG_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// ��ɱ��¼ param1 ���� param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_DROP_RECORD,			// �����¼	
		CROSS_MIZANG_BOSS_OPERA_TYPE_CONCERN_BOSS,			// ��עboss	param1 ���� param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// ��עboss param1 ���� param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_FORENOTICE,			// boss֪ͨ

		CROSS_MIZANG_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossMiZangBossBossInfoReq				// �������ز�boss��Ϣ 8850
	{
	public:
		CSCrossMiZangBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossMizangBossBossInfoAck		       // ����ز�boss��Ϣ 8851
	{
	public:
		SCCrossMizangBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_MIZANG_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_MIZANG_BOSS_SCENE_MAX];
	};

	class SCCrossMizangBossSceneInfo			// ����ز�boss������������Ϣ 8852
	{
	public:
		SCCrossMizangBossSceneInfo();
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
		CrossBossInfo boss_list[MAX_CROSS_MIZANG_BOSS_PER_SCENE];					// Boss��Ϣ
	};

	class SCCrossMiZangBossPlayerInfo		// ����ز�BOSS�����Ϣ 8853
	{
	public:
		SCCrossMiZangBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// ʣ��Boss��
		short left_treasure_crystal_gather_times;							// ʣ����ϧˮ���ɼ�����
		int left_ordinary_crystal_gather_times;								// ʣ����ͨˮ���ɼ�����

		unsigned int concern_flag[CROSS_MIZANG_BOSS_SCENE_MAX];					// ��עflag
	};

	/////////////////////////////////////  �����ڤBOSS  /////////////////////////////////////
	enum CROSS_YOUMING_BOSS_OPERA_TYPE
	{
		CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// ˢ����Ϣ param1 ������Ϊ0��Ϊ���в㣩
		CROSS_YOUMING_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// ��ɱ��¼ param1 ���� param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_DROP_RECORD,			// �����¼	
		CROSS_YOUMING_BOSS_OPERA_TYPE_CONCERN_BOSS,			// ��עboss	param1 ���� param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// ��עboss param1 ���� param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_FORENOTICE,			// boss֪ͨ

		CROSS_YOUMING_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossYouMingBossBossInfoReq				// �������ز�boss��Ϣ 8856
	{
	public:
		CSCrossYouMingBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossYouMingBossBossInfoAck		       // ����ز�boss��Ϣ 8857
	{
	public:
		SCCrossYouMingBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_YOUMING_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_YOUMING_BOSS_SCENE_MAX];
	};

	class SCCrossYouMingBossSceneInfo			// ����ز�boss������������Ϣ 8858
	{
	public:
		SCCrossYouMingBossSceneInfo();
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
		CrossBossInfo boss_list[MAX_CROSS_YOUMING_BOSS_PER_SCENE];					// Boss��Ϣ
	};

	class SCCrossYouMingBossPlayerInfo		// ����ز�BOSS�����Ϣ 8859
	{
	public:
		SCCrossYouMingBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// ʣ��Boss��
		short left_treasure_crystal_gather_times;							// ʣ����ϧˮ���ɼ�����
		int left_ordinary_crystal_gather_times;								// ʣ����ͨˮ���ɼ�����

		unsigned int concern_flag[CROSS_YOUMING_BOSS_SCENE_MAX];					// ��עflag
	};

	enum LUCKY_WISH_TYPE
	{
		LUCKY_WISH_TYPE_ONLY_LUCKY_VALUE = 0,			//���߿ͻ��˸�������ֵ������RewardItem
		LUCKY_WISH_TYPE_COMMON_ITEM = 1,				//������ͨ��Ʒ
		LUCKY_WISH_TYPE_LUCKY_ITEM = 2,					//������������Ʒ
	};

	class SCLuckyWishInfo			// ����-������Ը������Ϣ 8861
	{
	public:
		SCLuckyWishInfo();
		MessageHeader header;

		struct RewardItem
		{
			RewardItem() : item_id(0), is_bind(0), num(0) {}

			unsigned short item_id;
			short is_bind;
			int num;
		};

		int lucky_value;															// ����ֵ
		char type;																	// �ο� LUCKY_WISH_TYPE
		char reserve_ch;															// ����
		short item_list_count;														// ��Ʒ�б���
		RewardItem reward_item[LUCKY_WISH_MAX_ITEM_COUNT];							// ���е���Ʒid
	};

	enum RA_LUCKY_WISH_OPERA_TYPE
	{
		RA_LUCKY_WISH_OPERA_TYPE_ALL_INFO = 0,						//����������Ϣ
		RA_LUCKY_WISH_OPERA_TYPE_WISH = 1,							//��Ը	param_1	��Ը����
	};
	class CSLuckyWishOpera			// ����-������Ը���� 8862
	{
	public:
		CSLuckyWishOpera();
		MessageHeader header;

		char type;																	// �ο� RA_LUCKY_WISH_OPERA_TYPE
		char param_1;																// ��Ը����
	};


	class SCImageCompetitionInfo           // �����ƴ�  8863
	{
	public:
		SCImageCompetitionInfo();
		MessageHeader	header;

		int opengame_day;       
	};

	/////////////////////////////////////  ���� ��ĩ�񻶻  /////////////////////////////////////
	enum RA_LOTTERY_1_OPERA_TYPE
	{
		RA_LOTTERY_1_OPERA_TYPE_INFO = 0,					// ������Ϣ
		RA_LOTTERY_1_OPERA_TYPE_DO_LOTTERY,					// ����齱���� p1�齱���� (1:һ�� 2��ʮ��)
		RA_LOTTERY_1_OPERA_TYPE_FETCH_PERSON_REWARD,		// ��ȡ���˽������� p1���

		RA_LOTTERY_1_OPERA_TYPE_MAX,
	};

	class SCRALottery1Info  // 2279 
	{
	public:
		SCRALottery1Info();
		MessageHeader header;

		unsigned int next_free_time;												// �´����ʱ��
		unsigned int person_total_chou_times;									// �����ܳ齱����
		unsigned int server_total_chou_times;									// ȫ���ܳ齱����
		unsigned short fetch_person_reward_flag;								// ��ȡ���˽������
		short item_count;					
		RALottery1ChouItem chou_item_list[RA_LOTTERY_1_TEN_CHOU_COUNT];			// ���е���Ʒ�б�
	};

	/////////////////////////////////////  �޹����2  /////////////////////////////////////
	enum GiftShopBuy2_OPERA_TYPE
	{
		GiftShopBuy2_OPERA_TYPE_INFO = 0,			//������Ϣ��opera_type = 0 
		GiftShopBuy2_OPERA_TYPE_BUY,				//������opera_type = 1��seq
	};

	class CSRAOpenGameGiftShopBuy2					// 8921 �޹����2
	{
	public:
		CSRAOpenGameGiftShopBuy2();
		MessageHeader header;

		int opera_type;
		int seq;
	};

	class SCRAOpenGameGiftShopBuy2Info				// 8920 ����޹���Ϣ
	{
	public:
		SCRAOpenGameGiftShopBuy2Info();
		MessageHeader header;

		int buy_flag;								// ��������־
	};

	/////////////////////////////////////  ���� ��Ԫ��  /////////////////////////////////////
	enum RA_ZERO_BUY_RETURN_OPERA_TYPE
	{
		RA_ZERO_BUY_RETURN_OPERA_TYPE_INFO = 0,			// ������Ϣ
		RA_ZERO_BUY_RETURN_OPERA_TYPE_BUY,				// ���� p1��������
		RA_ZERO_BUY_RETURN_OPERA_TYPE_FETCH_YUANBAO,	// ��ȡԪ�� p1��������

		RA_ZERO_BUY_RETURN_OPERA_TYPE_MAX,
	};

	class SCRAZeroBuyReturnInfo  // 2280  ��Ԫ����Ϣ 
	{
	public:
		SCRAZeroBuyReturnInfo();
		MessageHeader header;

		unsigned int zero_buy_return_buy_timestamp[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	 // ��Ԫ������ʱ���3
		unsigned int zero_buy_return_day_fetch_flag[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE]; // ��Ԫ��ÿ����ȡԪ�����3
	};

	////////////////////////////	�������		//////////////////////////////
	enum RA_KUANG_HAI_OPERA_TYPE
	{
		RA_KUANG_HAI_OPERA_TYPE_INFO = 0,			// ������Ϣ
		RA_KUANG_HAI_OPERA_TYPE_FETCH_REWARD = 1,	// �һ�������param_1 = ��������
	};

	class SCRAKuangHaiInfo	// ���������Ϣ 2281
	{
	public:
		SCRAKuangHaiInfo();
		MessageHeader header;

		struct TaskItem
		{
			short item_idx;		// ����
			short get_score;	// �ѵ��˵���
		};

		short reward_flag;			// �����Ѷһ��ı�־
		short task_item_num;		// ����
		short current_score;		// ��ǰ�˵�
		short reserve_sh;
		TaskItem task_item_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];
	};

	////////////////////////////	�����ո�		//////////////////////////////
	enum RA_GIFT_HARVEST_OPERA_TYPE
	{
		RA_GIFT_HARVEST_OPERA_TYPE_INFO = 0,	// ���������Ϣ
		RA_GIFT_HARVEST_OPERA_TYPE_RANK_INFO,	// ����������Ϣ
		RA_GIFT_HARVEST_OPERA_TYPE_ENTER_SCENE, // ������볡��
		RA_GIFT_HARVEST_OPERA_TYPE_ROUND_INFO,	// ����ǰ�ִ�״̬

		RA_GIFT_HARVEST_OPERA_TYPE_MAX,
	};

	class SCRAGiftHarvestPlayerInfo		//  �����Ϣ 2282
	{
	public:
		SCRAGiftHarvestPlayerInfo();
		MessageHeader header;

		int round;		// �����ִ�
		int get_score;	// ��õĻ���
		int kill_num;	// ��ɱ��
	};

	struct GiftHarvestRankItem
	{
		GiftHarvestRankItem() {}

		int role_id = 0;				// ��ɫid
		int cur_get_score = 0;			// �������÷���
		char sex = 0;					// �Ա�
		char prof = 0;					// ְҵ
		short reserve_sh = 0;
		long long avatar_timestamp = 0;	// ͷ��
		GameName name;					// ����
	};

	class SCRAGiftHarvestRankInfo	// 2283 ���а���Ϣ
	{
	public:
		SCRAGiftHarvestRankInfo();
		MessageHeader header;

		int rank_num;				// ����item����
		GiftHarvestRankItem rank_list[GIFT_HARVEST_PERSON_RANK_MAX];
	};

	class SCGiftHarvestSkill		// 2284 �����ո����Ϣ
	{
	public:
		SCGiftHarvestSkill();
		MessageHeader header;

		int skill_index;				// �����±�
		unsigned int next_can_perform_time;		// �´ο��ͷż���ʱ���
	};

	class SCGiftHarvestRoundNotice	// 2285
	{
	public:
		SCGiftHarvestRoundNotice();
		MessageHeader header;

		short round;						// ���ִ�
		short round_state;					// ����״̬
		unsigned int next_state_timestamp;	// ��һ״̬ʱ���
	};

	
	/////////////////////////////////////////// ����߾�֮�� ///////////////////////////////////////////

	class SCCrossBianJingZhiDiUserInfo   // 6640  ����߾�֮�ؽ�ɫ��Ϣ
	{
	public:
		SCCrossBianJingZhiDiUserInfo();
		MessageHeader header;

		struct SClientTaskInfo
		{
			SClientTaskInfo() : task_id(0), is_finish(0), param_count(0), cur_param_value(0), is_double_reward(0) {}

			short task_id;																	// ����ID
			short is_finish;																// �Ƿ���ȡ����

			int param_count;																// ��������Ĳɼ������������
			int cur_param_value;															// ��ǰֵ
			int is_double_reward;
		};

		struct RewardItemInfo																// ��õ���Ʒ��Ϣ
		{
			RewardItemInfo() : item_id(0), num(0) {}

			int item_id;
			int num;
		};

		unsigned int boss_reflush_time;
		int limit_task_time;
		int gather_buff_time;
		int sos_times;						// �ټ�����

		SClientTaskInfo taskinfo_list[CROSS_BIANJINGZHIDI_TASK_MAX];						// �û�������Ϣ
		RewardItemInfo reward_list[CROSS_BIANJINGZHIDI_MAX_REWARD_ITEM_COUNT];				// ��õĽ�����Ϣ
	};

	class SCCrossBianJingZhiDiBossInfo		// 6641 ����߾�֮��BOSS��Ϣ
	{
	public:
		SCCrossBianJingZhiDiBossInfo();
		MessageHeader header;

		struct BossInfoList // ����BOSS��Ϣ
		{
			ObjID boss_obj;			// boss objID
			short boss_live_flag;	// boss�Ƿ���

			int boss_id;			// 
			short born_pos_x;			// ����λ��x
			short born_pos_y;			// ����λ��y

			long long guild_uuid;	// ��������uuid  0��ʾ�޹���
			GuildName guild_name;	// ����������
		};

		int count;
		BossInfoList boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
	};

	class SCCrossBianJingZhiDiBossHurtInfo	// 6642 ����߾�֮�أ�BOSS�˺���Ϣ
	{
	public:
		SCCrossBianJingZhiDiBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//�Լ���������
		long long own_guild_hurt;//�Լ������˺�
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class CSCrossBianJingZhiDiBuyBuff		 // 6643 ����߾�֮����buff
	{
	public:
		CSCrossBianJingZhiDiBuyBuff();
		MessageHeader header;
	};

	/////////////////////////////////////////////////  ���������  /////////////////////////////////////////////////////////
	enum CROSS_CHALLENGEFIELD_OPERA_REQ
	{
		CROSS_CHALLENGEFIELD_OPERA_REQ_OPPONENT_INFO = 0,		 // ����������  p1:�Ƿ��ȡǰ����(1/0)
		CROSS_CHALLENGEFIELD_OPERA_REQ_RANK_INFO = 1,			 // ����Ӣ�۰�(ǰ100��)
		CROSS_CHALLENGEFIELD_OPERA_REQ_FETCH_REWARD = 2,		 // ��ȡ����
		CROSS_CHALLENGEFIELD_OPERA_REQ_SELFT_INFO = 3,			 // ����Լ���Ϣ��ս��
		CROSS_CHALLENGEFIELD_OPERA_REQ_FIGHT = 4,				 // ��ս���� p1:����index p2:�Ƿ����������仯 p3:rank_pos
		CROSS_CHALLENGEFIELD_OPERA_REQ_REFRESH = 5,				 // ˢ����ս�б� 
		CROSS_CHALLENGEFIELD_OPERA_REQ_READY = 6,				 // ����׼����ʽ��ʼս������ʱ 
	};

	class CSCrossChallengeFieldOpera						// 8940 �������������
	{
	public:
		CSCrossChallengeFieldOpera();
		MessageHeader		header;

		short				req_type;
		short				param1;
		short				param2;
		short				param3;
	};

	class SCCrossChallengeFieldStatus								// 8941 �����û���Ϣ
	{
	public:
		SCCrossChallengeFieldStatus();
		MessageHeader		header;

		struct SideInfo
		{
			long long		role_id;
			ObjID			obj_id;
			short			level;
			GameName		name;
			char			camp;
			char			prof;
			char			avatar;
			char			sex;
			Attribute		hp;
			Attribute		max_hp;
			Attribute		mp;
			Attribute		max_mp;
			Attribute		speed;
			short			pos_x;
			short			pos_y;
			Dir				dir;
			Scalar			distance;
			int				capability;
			GuildID			guild_id;
			GuildName		guild_name;
			char			guild_post;
			char			reserve_ch;
			short			reserve_sh;
		};

		int					status;				// CHALLENGE_FIELD_STATE
		unsigned int		next_status_timestamp;
		SideInfo			side_info_list[CHALLENGE_FIELD_SIDE_MAX];
	};

	class SCCrossChallengeFieldOpponentInfo							// 8943 ���ֶ�����ϸ��Ϣ 
	{
	public:
		SCCrossChallengeFieldOpponentInfo();
		MessageHeader		header;

		struct OpponentInfo
		{
			int server_id;
			long long user_id;
			long long avatar_timestamp;				
			char camp;
			char prof;
			char sex;
			char avatar;
			int capability;
			int best_rank_break_level;

			GameName name;

			RoleAppearance appearance;
		};

		int opponent_count;
		OpponentInfo opponent_info_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCCrossChallengeFieldUserInfo								//8942 ��ս�б���Ϣ�͸�����Ϣ
	{
	public:
		SCCrossChallengeFieldUserInfo();
		MessageHeader		header;

		struct  OpponentInfo
		{
			long long user_id;
			int rank_pos;
		};

		int rank_pos;
		int curr_opponent_idx;
		int join_times;
		int buy_join_times;
		int jifen;
		int jifen_reward_flag;
		int reward_guanghui;
		int reward_bind_gold;
		int liansheng;
		int buy_buff_times;
		int best_rank_pos;
		int free_day_times;

		struct RewardItem
		{
			ItemID item_id;
			short num;
		};
		RewardItem reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		OpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCCrossChallengeFieldOpponentRankPosChange					// 8948 ��λ�仯֪ͨ
	{
	public:
		SCCrossChallengeFieldOpponentRankPosChange();
		MessageHeader		header;

		long long user_id;
		int rank_pos;
	};

	class SCCrossChallengeFieldReportInfo							// 8946 ս��
	{
	public:
		SCCrossChallengeFieldReportInfo();
		MessageHeader		header;

		struct ReportItem
		{
			unsigned int		challenge_time;					// ʱ��
			long long			target_uid;						// �Է�id
			GameName			target_name;					// �Է�����
			char				is_sponsor;						// �Լ��Ƿ�Ϊ��ս��
			char				is_win;							// �Ƿ�ʤ��
			short				reserve_sh;						// ����
			unsigned short		old_rankpos;					// ����ǰ����
			unsigned short		new_rankpos;					// ����������
		};

		int report_count;
		ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
	};

	class SCCrossChallengeFieldRankInfo								// 8947 Ӣ�۰�������Ϣ
	{
	public:
		SCCrossChallengeFieldRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() :user_id(0), capability(0)
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int server_id;
			long long user_id;
			int capability;
			GameName user_name;
			char sex;
			char is_robot;
			char prof;
			char reserved;
			int role_level;

			RoleAppearance appearance;
		};

		RankItem rank_list[CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM];
	};

	class SCCrossChallengeFieldBeDefeatNotice // 8944 �����֪ͨ
	{
	public:
		SCCrossChallengeFieldBeDefeatNotice();
		MessageHeader		header;
	};

	class SCCrossChallengeFieldWin			// 8945 ������ֱ��ʤ�� 
	{
	public:
		SCCrossChallengeFieldWin();
		MessageHeader		header;

		short old_rank_pos;
		short new_rank_pos;
	};


	//////////////////////////////////////////////////////////////////////////

	class SCFirstRechargeBuffFlag	//8933
	{
	public:
		SCFirstRechargeBuffFlag();
		MessageHeader header;

		short ra_is_has_first_recharge_attr_add;	//�Ƿ�ӵ���׳����Լӳ�
		short reserve_sh;							//����
	};

	/////////////////////////////////////  ���� �����ƹ�  ///////////////////////////////////
	enum RA_LUCKY_CLOUD_BUY_OPERA_TYPE
	{
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_INFO = 0,			// ������Ϣ
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_BUY = 1,			// ����
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_OOPEN = 2,		// ����壨����1Ϊ�ϴδ�����ʱ�����
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_CLOSE = 3,		// �ر����
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_LUCKY = 4,		// ���˶���Ϣ

		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_MAX,
	};

	struct SCRALuckyCloudBuyInfo		//	�����ƹ�������Ϣ 8954
	{
	public:
		SCRALuckyCloudBuyInfo();
		MessageHeader header;

		unsigned short seq;			//����
		unsigned short buy_self;	//�Լ����������
	};

	class SCRALuckyCloudBuyBuyList	// �����ƹ������¼ 8955
	{
	public:
		SCRALuckyCloudBuyBuyList();
		MessageHeader header;

		unsigned int ret_timestamp;						// ���ص�ʱ���
		unsigned short total_buy;						//��湺�������
		unsigned short num;								// listԪ������
		GameName name_list[LUCKY_BUY_RECORD_MAX_NUM];	// ���12��
	};

	class SCRALuckyCloudBuyOpenInfo	// �����ƹ������֪ͨ 8956
	{
	public:
		SCRALuckyCloudBuyOpenInfo();
		MessageHeader header;

		unsigned int is_open;	// 0 Ŀǰ���ܲ��룬 1 �����ɲ���
	};

	///////////////////////////////////////  �񻶴��ֹ�   /////////////////////////////////////////
	enum RA_CRACYBUY_TYPE
	{
		RA_CRACYBUY_ALL_INFO,							 // �����ֵ��Ϣ
		RA_CRACYBUY_LIMIT_INFO,							 // �����޹���Ϣ
		RA_CRACYBUY_BUY,								 // �������� param1:������
	};

	class SCRACrazyBuyAllInfo	// 8959 ������������Ϣ
	{
	public:
		SCRACrazyBuyAllInfo();
		MessageHeader header;

		int chongzhi;
		int level;
	};

	class SCRACracyBuyLimitInfo	// 8960 �޹���Ϣ
	{
	public:
		SCRACracyBuyLimitInfo();
		MessageHeader header;

		struct Limit_Info
		{
			int person_limit;
			int all_limit;
		};

		Limit_Info item_limit_info[RA_MAX_CRACY_BUY_NUM_LIMIT];
	};

	///////////////////////////////////// �������ˮ�� /////////////////////////////////////

	enum CS_CROSS_HUSONG_SHUIJING_OPERA_TYPE
	{
		CS_CROSS_HUSONG_SHUIJING_OPERA_INFO,
		CS_CROSS_HUSONG_SHUIJING_OPERA_COMMIT,		
		CS_CROSS_HUSONG_SHUIJING_OPERA_GATHER_INFO,
	};

	class CSCrossHusongShuijingOpera // 8979
	{
	public:
		CSCrossHusongShuijingOpera();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class SCCrossHusongShuijingInfo	//8980
	{
	public:
		SCCrossHusongShuijingInfo();
		MessageHeader header;

		short vip_buy_times;				// �������
		short rob_count;					// ��ȡ�ɹ�����
		short gather_times;					// �ɼ�����
		unsigned short commit_count;		// �����ύ����
		short husong_type;					// ��������
		short husong_status;				// ����״̬
		UInt32 invalid_time;				// ���͵�ʧЧʱ��
	};

	enum CS_CROSS_HUSONG_SHUIJING_GATHER_INFO_TYPE
	{
		CS_CROSS_HUSONG_SHUIJING_GATHER_DEFAULT, //0 ��Ϣ�仯
		CS_CROSS_HUSONG_SHUIJING_GATHER_REFRESH, //1 ˮ��ˢ��
	};

	class SCCrossHusongShuijingGatherInfo // 8981
	{
	public:
		SCCrossHusongShuijingGatherInfo();
		MessageHeader header;

		short type;						//ˮ����Ϣ���ͣ�0 ��Ϣ�仯��1 ˮ��ˢ��
		short cur_remain_gather_time_big;//��ˮ����ǰʣ��ɲɼ�����
		UInt32 next_refresh_time_big;     //��ˮ���´�ˢ��ʱ��
	};

}

#pragma pack(pop)

#endif	// __MSG_ACTIVITY_HPP__
