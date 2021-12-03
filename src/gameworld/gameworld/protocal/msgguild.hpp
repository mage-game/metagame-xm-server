/******************************************************************************************
FileName: msgguild.hpp
Author: shell
Description: ����GameWorld��ͻ���ͨ��Э�� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/11/ 16:32:08     1.0           ����
*****************************************************************************************/

#ifndef __MSGGUILD_HPP__
#define __MSGGUILD_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/maildef.hpp"
#include "config/guildconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	// C -> S

	class CSCreateGuild
	{
	public:
		CSCreateGuild();
		MessageHeader		header;

		GuildName			guild_name;
		int					create_guild_type;	// GUILD_CREATE_TYPE
		int					knapsack_index;		// ��Ʒ�ڱ����е�����
		GuildNotice			guild_notice;
	};

	class CSDismissGuild
	{
	public:
		CSDismissGuild();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSApplyForJoinGuild
	{
	public:
		CSApplyForJoinGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					is_auto_join;		// һ������ ��:�� 1:0
	};

	class CSApplyForJoinGuildAck
	{
	public:
		CSApplyForJoinGuildAck();
		MessageHeader		header;

		GuildID				guild_id;
		int					result;			// 0Ϊͬ��  ������ͬ��
		int					count;
		int					applyfor_uid_list[MAX_APPLY_FOR_COUNT];
	};

	class CSQuitGuild
	{
	public:
		CSQuitGuild();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSInviteGuild
	{
	public:
		CSInviteGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					beinvite_uid;
	};

	class CSInviteGuildAck
	{
	public:
		CSInviteGuildAck();
		MessageHeader		header;

		GuildID				guild_id;
		int					invite_uid;
		int					result;
	};

	class CSKickoutGuild
	{
	public:
		CSKickoutGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					bekicker_count;
		int					bekicker_uid_list[MAX_MEMBER_COUNT];
	};

	class CSAppointGuild
	{
	public:
		CSAppointGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					beappoint_uid;
		int					post;
	};

	class CSLeaveGuildPost
	{
	public:
		CSLeaveGuildPost();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSGuildChangeNotice
	{
	public:
		CSGuildChangeNotice();
		MessageHeader		header;

		GuildID				guild_id;
		GuildNotice 		notice;
	};

	class CSGuildMailAll
	{
	public:
		CSGuildMailAll();
		MessageHeader		header;

		void ResetMailContentTxt()
		{
			memset(contenttxt, 0, sizeof(contenttxt));
		}

		GuildID				guild_id;

		MailSubject			subject;

		int					contenttxt_len;
		MailContentTxt		contenttxt;
	};

	class CSGetGuildInfo	// 4262 
	{
	public:
		CSGetGuildInfo();
		MessageHeader		header;

		int					guild_info_type;
		GuildID				guild_id;
	};


	class CSGuildDelate
	{
	public:
		CSGuildDelate();
		MessageHeader		header;

		GuildID				guild_id;
		int					knapsack_index;		// ������Ʒ�ڱ����е�����
	};

	class CSGuildBackToStation
	{
	public:
		CSGuildBackToStation();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSGuildResetName
	{
	public:
		CSGuildResetName();
		MessageHeader		header;

		GuildID				guild_id;
		GuildName			new_name;
	};

	class CSApplyforSetup
	{
	public:
		CSApplyforSetup();
		MessageHeader		header;

		GuildID				guild_id;
		int					applyfor_setup;
		int					need_capability;
		int					need_level;
	};

	enum ADD_GUILD_EXP_TYPE
	{
		ADD_GUILD_EXP_TYPE_INVALID = 0,

		ADD_GUILD_EXP_TYPE_GOLD,							// Ԫ������
		ADD_GUILD_EXP_TYPE_ITEM,							// ��Ʒ����

		ADD_GUILD_EXP_TYPE_MAX,
	};

	struct JxItem
	{
		JxItem() : item_id(0), resreve(0), item_num(0) { }
		ItemID item_id;
		short resreve;
		int item_num;
	};

	class CSAddGuildExp
	{
	public:
		CSAddGuildExp();
		MessageHeader		header;

		short type;
		short times;
		int value;

		JxItem item_list[GUILD_MAX_UPLEVEL_STUFF_COUNT];
	};

	class CSGuildUpLevel
	{
	public:
		CSGuildUpLevel();
		MessageHeader		header;

		int hall_type;
	};

	class CSGuildGetBuff
	{
	public:
		CSGuildGetBuff();
		MessageHeader		header;

		int buff_type;
	};

	class CSGuildExchange
	{
	public:
		CSGuildExchange();
		MessageHeader		header;

		ItemID item_id;
		short num;
	};

	class CSGuildSetAutoKickoutSetup
	{
	public:
		CSGuildSetAutoKickoutSetup();
		MessageHeader		header;

		int guild_id;
		int auto_kickout_setup;
	};

	class CSGuildCallIn
	{
	public:
		CSGuildCallIn();
		MessageHeader		header;

		int guild_id;
	};

	// ���˲ֿ� begin --------------------------------------------------------------------------------
	enum GUILD_STORGE_OPERATE
	{
		GUILD_STORGE_OPERATE_PUTON_ITEM = 1,	// ����/���� param1�������� param2����
		GUILD_STORGE_OPERATE_TAKE_ITEM,			// �һ�/ȡ�� param1�ֿ�������� param2���� param3��ƷID
		GUILD_STORGE_OPERATE_REQ_INFO,			// ������Ϣ
		GUILD_STORGE_OPERATE_DISCARD_ITEM,		// ���� param1�ֿ�������� param2��ƷID
	};

	class CSGuildStorgeOperate				// 4289 ���˲ֿ��������
	{
	public:
		CSGuildStorgeOperate();
		MessageHeader		header;

		int operate_type;  // GUILD_STORGE_OPERATE
		int param1;
		int param2;
		int param3;
	};

	enum GUILD_STORGE_ONE_KEY_OPERATE
	{
		GUILD_STORGE_OPERATE_PUTON_ITEM_ONE_KEY = 1,	// ����
		GUILD_STORGE_OPERATE_DISCARD_ITEM_ONE_KEY,		// ���� 
	};

	class CSGuildStorgeOneKeyOperate			// 4295 ���˲ֿ�һ����������
	{
	public:
		CSGuildStorgeOneKeyOperate();
		MessageHeader		header;

		struct ItemInfo
		{
			ItemInfo():item_index(0), param_1(0){}

			short item_index;			// ���״��������������ٴ��ֿ��������
			unsigned short param_1;		// ���״�num�����ٴ�item_id
		};

		int operate_type;		// GUILD_STORGE_ONE_KEY_OPERATE
		int item_count;
		ItemInfo item_list[GUILD_STORAGE_MAX_GRID_NUM];
	};
	// ���˲ֿ� end --------------------------------------------------------------------------------

	class CSGuildSetAutoClearReq		// 4304 �����Զ��������Ա
	{
	public:
		CSGuildSetAutoClearReq();
		MessageHeader		header;

		short is_auto_clear;
		short reserve;
	};

	enum GUILD_SKILL_UP_LEVEL_TYPE
	{
		GUILD_SKILL_UP_LEVEL = 0,
		GUILD_SKILL_UP_LEVEL_ALL,
	};
	class CSGuildSkillUplevel
	{
	public:
		CSGuildSkillUplevel();
		MessageHeader		header;

		short up_type; //0 ��ͨ���� 1 ȫ������
		short skill_index;
	};

	class CSGuildUpTotemLevel						// 4291����ͼ������
	{
	public:
		CSGuildUpTotemLevel();
		MessageHeader header;
	};

	enum GUILD_BOX_OPERATE_TYPE
	{
		GBOT_QUERY_SELF,
		GBOT_UPLEVEL,
		GBOT_OPEN,
		GBOT_FETCH,
		GBOT_QUERY_NEED_ASSIST,
		GBOT_ASSIST,
		GBOT_CLEAN_ASSIST_CD,
		GBOT_INVITE_ASSIST,         // ����Э�� p1:�Է�uid
		GBOT_THANK_ASSIST,         // ��лЭ��  p1:����index  p2:�Է�uid 
	};

	class CSGuildBoxOperate							// 4292���˱������
	{
	public:
		CSGuildBoxOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
	};

	enum GUILD_COMMON_REQ_TYPE
	{
		GUILD_COMMON_REQ_TYPE_FETCH_REWARD = 0,   // ��ȡÿ�ս���
		GUILD_COMMON_REQ_TYPE_GIVE_GONGZI,	      // ������
	};

	class CSGuildCommonReq						// 4293 
	{
	public:
		CSGuildCommonReq();
		MessageHeader header;

		short req_type;
		short reserve_sh;
	};

	enum GUID_TERRITORY_WELF_OPERATE_TYPE
	{
		GTW_FETCH_REWARD,
		GTW_FETCH_EXTRA_REWARD,
	};

	enum GUID_TERRITORY_WELF_PHASE_REWARD
	{
		INVALID_PHASE = -1,

		ONE_PHASE = 0,
		TWO_PHASE,
		THREE_PHASE,
		FOUR_PHASE,

		MAX_PAHSE,

		PRESIDENT_EXTRA_REWARD = 8,
	};

	class CSGuildTerritoryWelfOperate  // 4294
	{
	public:
		CSGuildTerritoryWelfOperate();
		MessageHeader header;

		int operate_type;
		int param1;
	};

	class CSFetchGuildBossRedbag	// 4297
	{
	public:
		CSFetchGuildBossRedbag();
		MessageHeader header;

		int index;
	};

	class SCGulidReliveTimes		// 4298
	{
	public:
		SCGulidReliveTimes();
		MessageHeader header;

		int	daily_guild_all_relive_times;
		int	daily_guild_all_kill_boss_times;
	};

	class SCGulidBossRedbagInfo		// 4299
	{
	public:
		SCGulidBossRedbagInfo();
		MessageHeader header;

		int	daily_use_guild_relive_times;
		short daily_boss_redbag_reward_fetch_flag;
		short reverse;
	};

	struct GuildSaiziRankItem
	{
		GuildSaiziRankItem()
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			score = 0;
			memset(name, 0, sizeof(name));
		}

		int uid;
		int score;
		GuildName name;
	};

	class SCGulidSaiziInfo			// 4301
	{
	public:
		SCGulidSaiziInfo();
		MessageHeader header;

		int today_guild_pao_saizi_times;					// ÿ�칫�������Ӵ���
		long long today_last_guild_pao_saizi_time;			// ���һ��������ʱ��
		int today_guild_saizi_score;						// ÿ�����ӻ���
		int pao_saizi_num;									// �׵�ʲô����

		GuildSaiziRankItem guild_saizi_rank_list[MAX_MEMBER_COUNT]; // ������Ϣ
	};

	class CSGulidPaoSaizi			// 4302
	{
	public:
		CSGulidPaoSaizi();
		MessageHeader header;
	};

	class CSReqGulidSaiziInfo			// 4303
	{
	public:
		CSReqGulidSaiziInfo();
		MessageHeader header;
	};

	struct FetchUserInfo
	{
		int uid;
		int gold_num;
		GameName name;
	};

	const static int GUILD_BOSS_REDBAG_MAX_CAN_FETCH_TIMES = 300;
	class SCGuildBossRedBagInfo // 4300
	{
	public:
		SCGuildBossRedBagInfo();
		MessageHeader header;

		int total_gold_num;
		
		int leader_uid;
		GameName leader_name;
		long long creater_avator_timestamp;
		char creater_sex;
		char creater_prof;
		short reserve;

		int guild_id;
		GameName guild_name;

		int fetch_user_count;
		FetchUserInfo fetch_user_info_list[GUILD_BOSS_REDBAG_MAX_CAN_FETCH_TIMES];
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// S -> C
	
	class SCGuildBackToStation
	{
	public:
		SCGuildBackToStation();
		MessageHeader		header;

		int					result;
		int					cost_zhangong;
	};

	class SCAddGuildExpSucc
	{
	public:
		SCAddGuildExpSucc();
		MessageHeader		header;

		int add_gongxian;
		int add_caifu;
	};

	//////////////////////////////////////////////////////////////////////////

	class CSGuildXianshuInfoReq
	{
	public:
		CSGuildXianshuInfoReq();
		MessageHeader		header;
	};

	class CSGuildXianshuUplevelReq
	{
	public:
		CSGuildXianshuUplevelReq();
		MessageHeader		header;
	};

	class SCGuildXianshuInfo
	{
	public:
		SCGuildXianshuInfo();
		MessageHeader header;

		short xianshu_level;
		short reserve;
	};

	class CSGuildCheckCanDelate
	{
	public:
		CSGuildCheckCanDelate();
		MessageHeader header;

		int guild_id; 
	};

	class SCGuildFbStatus				// 4205�����ܾ�״̬��Ϣ
	{
	public:
		SCGuildFbStatus();
		MessageHeader header;

		short open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
	};

	class CSGuildFbStartReq				// 4275�����ؾ���������
	{
	public:
		CSGuildFbStartReq();
		MessageHeader header;
	};

	class SCGuildBonfireStatus			// 4208��������״̬��Ϣ
	{
	public:
		SCGuildBonfireStatus();
		MessageHeader header;

		short open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
	};

	class CSGuildBonfireStartReq		// 4286�������������� 
	{
	public:
		CSGuildBonfireStartReq();
		MessageHeader header;
	};

	class CSGuildBonfireGotoReq			// 4287��������ǰ������
	{
	public:
		CSGuildBonfireGotoReq();
		MessageHeader header;
	};
	
	class CSGuildBonfireAddMucaiReq		// 4288��ľ������
	{
	public:
		CSGuildBonfireAddMucaiReq();
		MessageHeader header;

	};

	class CSGuildFbEnterReq				// 4276�����ܾ���������
	{
	public:
		CSGuildFbEnterReq();
		MessageHeader header;
	};

	class CSGuildZhuLuckyReq
	{
	public:
		CSGuildZhuLuckyReq();
		MessageHeader header;

		int be_zhufu_uid;
	};

	class CSInviteLuckyZhufu
	{
	public:
		CSInviteLuckyZhufu();
		MessageHeader header;

		int invite_uid;				// 0��һ��ף��
	};

	class SCInviteLuckyZhufu
	{
	public:
		SCInviteLuckyZhufu();
		MessageHeader header;

		int req_invite_uid;					
		GameName req_invite_name;
	};

	class CSGetAllGuildMemberLuckyInfo
	{
	public:
		CSGetAllGuildMemberLuckyInfo();
		MessageHeader header;
	};

	class SCGuildLuckyInfo
	{
	public:
		SCGuildLuckyInfo();
		MessageHeader header;
	
		enum NOTIFY_REASON
		{
			NOTIFY_REASON_GET,
			NOTIFY_REASON_CHANGE,
		};

		struct MemberLuckyInfo
		{
			MemberLuckyInfo() : uid(0), lucky_color(0) 
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int uid;
			int lucky_color;
			GameName user_name;
		};
		
		int reason;

		int member_count;
		MemberLuckyInfo member_luckyinfo_list[MAX_MEMBER_COUNT];
	};

	class SCGuildLuckyChangeNotice
	{
	public:
		SCGuildLuckyChangeNotice();
		MessageHeader header;

		int bless_uid;
		GameName bless_name;
		int to_color;
	};

	class SCGuildActiveDegreeInfo // �������޻��Ϣ  4723
	{
	public:
		SCGuildActiveDegreeInfo();
		MessageHeader header;

		short today_open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
		int active_degree;
	};

	class CSGuildCallBeastReq // �������ޣ��ٻ�BOSS����  4280
	{
	public:
		CSGuildCallBeastReq();
		MessageHeader header;
	};

	class CSGuildActiveDegreeInfoReq // �������޻��Ϣ  4281
	{
	public:
		CSGuildActiveDegreeInfoReq();
		MessageHeader header;
	};

	class CSGuildExtendMemberReq	// ������չ��Ա���� 4296
	{
	public:
		CSGuildExtendMemberReq();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			EXTEND_MEMBER = 0,		// ��չ��Ա
			MEMBER_MAX_COUNT_INFO,	// ��������Ա��Ϣ
		};

		char operate_type;
		char can_use_gold;
		short num;
	};

	enum GUILD_OBSS_OPER_TYPE
	{
		GUILD_BOSS_UPLEVEL = 0,
		GUILD_BOSS_CALL = 1,
		GUILD_BOSS_INFO_REQ = 2,
	};
	
	class CSGuildBossOperate
	{
	public:
		CSGuildBossOperate();
		MessageHeader header;

		short oper_type;
		short param;
	};

	class SCGuildBossInfo
	{
	public:
		SCGuildBossInfo();
		MessageHeader header;

		char boss_normal_call_count;
		char boss_super_call_count;
		short boss_level;
		int boss_exp;
		int boss_super_call_uid;
		GameName boss_super_call_name;
	};

	enum NOTIFY_INFO_TYPE
	{
		NOTIFY_INFO_TYPE_SCENE = 0,					// ������Ϣ
		NOTIFY_INFO_TYPE_RANK = 1,					// ������Ϣ֪ͨ
		NOTIFY_INFO_TYPE_ROLE_INFO_CHANGE = 2,		// ������Ϣ�仯֪ͨ
		NOTIFY_INFO_TYPE_STAR_LEVEL_CHANGE = 3,		// �Ǽ��仯֪ͨ

	};

	class SCGuildBossActivityInfo			// 4213 ����boss���Ϣ
	{
	public:
		SCGuildBossActivityInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 20;

		struct Rank
		{
			int user_id;
			GameName user_name;
			long long hurt_val;
		};
		
		short notify_type;						// ��Ϣ֪ͨ����
		short reserve_sh;					
		int boss_id;
		int boss_level;
		ObjID boss_objid;
		short is_super_boss;
		long long totem_totem_exp;
		short cur_star_level;					// ��ǰ�Ǽ�
		short gather_num;						// �Ѳɼ�����
		unsigned int next_change_star_time;		// �����´θı��Ǽ�ʱ���

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT]; // ������Ϣ
	};

	// ���˲ֿ� beign -------------------------------------------------------------------------------------------------------
	class SCGuildStorgeInfo					// 4209 ���˲ֿ���Ϣ
	{
	public:
		SCGuildStorgeInfo();
		MessageHeader header;

		int open_grid_count;	// �ֿ⿪���ĸ�������
		int storage_score;		// ����

		int count;
		ItemDataWrapper storge_item_list[GUILD_STORAGE_MAX_GRID_NUM];  // �ֿ������Ϣ 102��
	};

	class SCGuildStorgeChange				// 4210 ���˲ֿ�ı�
	{
	public:
		SCGuildStorgeChange();
		MessageHeader header;

		int index;
		ItemDataWrapper item_datawrapper;
	};

	class SCGuildStorageLogList				// 4216 ���˲ֿ���־�¼��б�
	{
	public:
		SCGuildStorageLogList();
		MessageHeader		header;

		struct StorageLog
		{
			GameName log_owner_name;	// ����
			unsigned int log_time;		// ʱ��

			int opt_type;				// �������� GUILD_STORE_OPTYPE
			int item_id;				// ��ƷID
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��Ʒ����
		};

		int					count;
		StorageLog			log_list[MAX_STORAGE_EVENT_COUNT];
	};
	// ���˲ֿ� end -------------------------------------------------------------------------------------------------------

	class SCGuildBoxInfo							// 4211���˱�����Ϣ
	{
	public:
		SCGuildBoxInfo();
		MessageHeader header;

		struct InfoData
		{
			short box_level;
			short is_reward;
			int assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT];
			char is_thank_assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT]; // �Ƿ��Ѹ�л
			unsigned int open_time;
			GameName assist_name[MAX_GUILD_BOX_BE_ASSIST_COUNT];
		};

		short uplevel_count;
		short assist_count;
		short be_assist_count;
		short reserve_sh;
		unsigned int assist_cd_end_time;
		InfoData info_list[MAX_GUILD_BOX_COUNT];
	};

	class SCGuildBoxNeedAssistInfo					// 4212���˱�����ҪЭ����Ϣ
	{
	public:
		SCGuildBoxNeedAssistInfo();
		MessageHeader header;

		static const int MAX_BOX_ASSIST_COUNT = 200;
		struct InfoData
		{
			int uid;
			short box_index;
			short box_level;
			unsigned int open_time;
			GameName user_name;
		};

		int box_count;
		InfoData info_list[MAX_BOX_ASSIST_COUNT];
	};

	class CSGuildRedPaperListInfoReq				// 4214���󹫻�����Ϣ�б�
	{
	public:
		CSGuildRedPaperListInfoReq();
		MessageHeader header;
	};

	class CSCreateGuildRedPaperReq					// 9829���󴴽�������
	{
	public:
		CSCreateGuildRedPaperReq();
		MessageHeader header;

		int paper_seq;
		int fetech_time;
		int red_paper_index;
	};

	class CSSingleChatRedPaperRole					// 9830˽���к��δ�ɷ������
	{
	public:
		CSSingleChatRedPaperRole();
		MessageHeader header;

		int red_paper_index;
	};

	enum GUILD_MAZE_OPERATE_TYPE
	{
		GUILD_MAZE_OPERATE_TYPE_GET_INFO,			// ������Ϣ
		GUILD_MAZE_OPERATE_TYPE_SELECT,				// ѡ��
	};

	class CSGuildMazeOperate						// 9831�����Թ���������
	{
	public:
		CSGuildMazeOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
	};

	enum GUILD_MAZE_INFO_REASON
	{
		GUILD_MAZE_INFO_REASON_DEF = 0,
		GUILD_MAZE_INFO_REASON_FIRST_SUCC,
		GUILD_MAZE_INFO_REASON_SUCC,
		GUILD_MAZE_INFO_REASON_FAIL,
	};

	class SCGuildMemberMazeInfo						// 9832�����Ա�Թ���Ϣ
	{
	public:
		SCGuildMemberMazeInfo();
		MessageHeader header;

		int reason;
		int layer;
		unsigned int complete_time;
		unsigned int cd_end_time;
		int max_layer;
	};

	class SCGuildMazeRankInfo						// 9833�����Թ�������Ϣ
	{
	public:
		SCGuildMazeRankInfo();
		MessageHeader header;

		struct MazeRankItem
		{
			int uid;
			GameName user_name;
			unsigned int complete_time;
		};

		int rank_count;
		MazeRankItem rank_list[MAX_MEMBER_COUNT];
	};

	enum GUILD_SINGIN_REQ_TYPE
	{
		GUILD_SINGIN_REQ_TYPE_SIGNIN,								// ǩ��
		GUILD_SINGIN_REQ_TYPE_FETCH_REWARD,							// �ý��� p1 index
		GUILD_SINGIN_REQ_ALL_INFO,									// ����������Ϣ
	};

	class CSGuildSinginReq											//9834
	{
	public:
		CSGuildSinginReq();
		MessageHeader header;

		short req_type;
		short param1;
	};

	class SCGuildSinginAllInfo										//9835
	{
	public:
		SCGuildSinginAllInfo();
		MessageHeader header;

		int is_signin_today;										// �����Ƿ���ǩ��
		char signin_count_month;									// ��ǩ������
		char guild_signin_fetch_reward_flag;						// ������ǩ����ȡ��־
		short guild_signin_count_today;								// ������ǩ������
	};

	class CSGuildChangeAvatar										//9836
	{
	public:
		CSGuildChangeAvatar();
		MessageHeader  header;

		unsigned long long avatar_timestamp;
	};

	class SCGuildSendAppearance										//9837
	{
	public:
		SCGuildSendAppearance();
		MessageHeader  header;

		unsigned short obj_id;
		short reserve_sh;
		GuildName guild_name;
		unsigned long long avatar_timestamp;
	};

	enum BIAOCHE_OPERA_TYPE
	{
		BIAOCHE_OPERA_TYPE_START = 0,		// ��ʼ���� param_1:guild_id
		BIAOCHE_OPERA_TYPE_TRANS,			// ���͵��ڳ� 
		BIAOCHE_OPERA_TYPE_BIAOCHE_POS,		// �ڳ�����

	};

	class CSBiaoCheOpera				// 9838 �������
	{
	public:
		CSBiaoCheOpera();
		MessageHeader header;

		int opera_type;
		int param_1;
	};

	//���ͭ��----------------------------------------------------------------
	enum GUILD_TIANCITONGBI_REQ_TYPE
	{
		GUILD_TIANCITONGBI_REQ_TYPE_OPEN = 0,		 // ���� p1:guild_id p2:role_id
		GUILD_TIANCITONGBI_REQ_TYPE_USE_GATHER,		 // �ύ�ɼ���
		GUILD_TIANCITONGBI_REQ_TYPE_RANK_INFO,		 // ����������Ϣ
	};

	class CSGuildTianCiTongBiReq  // 4309
	{
	public:
		CSGuildTianCiTongBiReq();
		MessageHeader		header;

		int						opera_type;
		int						param_1;
		int						param_2;
	};

	class CSGuildEnemyRankList			// 4314 ��ȡ���˳�������
	{
	public:
		CSGuildEnemyRankList();
		MessageHeader		header;
	};
}

#pragma pack(pop)

#endif


