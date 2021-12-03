#ifndef __MSG_CROSS_H__
#define __MSG_CROSS_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/crossmsgcode.h"
#include "servercommon/userprotocal/msgcrosscommon.h"

#include "servercommon/crossdef.hpp"
#include "servercommon/rankdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct CSCrossGetPersonRankList			// 14010 ��ȡ������а��б�
	{
	public:
		CSCrossGetPersonRankList() : header(MT_CROSS_GET_PERSON_RANK_LIST_CS) {}

		MessageHeader header;

		int rank_type;
	};

	struct SCGetCrossPersonRankListAck		// 14001 �·�������а��б�
	{
	public:
		SCGetCrossPersonRankListAck() : header(MT_CROSS_GET_PERSON_RANK_LIST_ACK_SC) {}
		MessageHeader header;

		struct CrossPersonRankListItem
		{
			int plat_type;
			int role_id;
			GameName name;
			int level;
			char prof;
			char sex;
			char camp;
			char reserved;
			long long exp;
			long long rank_value;
			long long flexible_ll;
			GameName flexible_name;
			int server_id;
			int vip_level;
			int flexible_int;
		};

		int rank_type;
		int count;
		CrossPersonRankListItem rank_list[CROSS_PERSON_RANK_MAX_SIZE];
	};

	struct SCGetCrossCoupleRankListAck			// 14002 �·�������°��б�
	{
	public:
		SCGetCrossCoupleRankListAck() : header(MT_CROSS_GET_COUPLE_RANK_LIST_ACK_SC) {}
		MessageHeader header;

		struct CrossCoupleRankListItem
		{
			int plat_type;			// ƽ̨����
			int server_id;			// ������id
			int male_uid;			// ��uid
			int female_uid;			// Ůuid
			GameName male_name;		// ������
			GameName female_name;	// Ů����
			char male_prof;			// ��ְҵ
			char female_prof;		// Ůְҵ
			short reserve_sh;		// ����
			int male_rank_value;	// ������ֵ
			int female_rank_value;	// Ů����ֵ
		};

		int rank_type;
		int count;
		CrossCoupleRankListItem rank_list[CROSS_COUPLE_RANK_MAX_SIZE];
	};

	///////////////////////////////////////////���1v1///////////////////////////////////////////////
	struct CSCross1v1MatchQuery	// 14115 ƥ��״̬��ѯ
	{
	public:
		CSCross1v1MatchQuery() : header(MT_CROSS_1V1_MATCH_QUERY_CS) {}
		MessageHeader header;

		int req_type;
	};

	struct SCCross1v1MatchAck	// 14100 ƥ��״̬
	{
	public:
		SCCross1v1MatchAck() : header(MT_CROSS_1V1_MATCH_ACK_SC) {}
		MessageHeader header;

		short result;					// 0 : ûƥ�䣬1��ƥ����
		short reserve;
		unsigned int match_end_left_time;	// Ԥ��ƥ��ɹ���ʱ���
	};

	struct SCCross1v1MatchResult	// 14103 ƥ����
	{
	public:
		SCCross1v1MatchResult() : header(MT_CROSS_1V1_MATCH_RESULT_SC) {}
		MessageHeader header;

		short result;
		short side;
		int oppo_plat_type;
		int oppo_server_id;
		int oppo_role_id;
		GameName oppo_name;
		unsigned int fight_start_left_time;
		char oppo_prof;
		char oppo_sex;
		char oppo_camp;
		char reserved;
		int oppo_level;
		unsigned int fight_end_left_time;
		long long oppo_capability;				// ս��
	};

	struct SCCross1v1FightResult               // 14104 ս�����
	{
	public:
		SCCross1v1FightResult() : header(MT_CROSS_1V1_FIGHT_RESULT_SC) {}
		MessageHeader		header;

		int result;						// 1:ʤ����0:ʧ��
		int award_score;				// ��������
	};

	struct CSCross1v1WeekRecordQuery		// 14116 ���˼�¼��ѯ
	{
	public:
		CSCross1v1WeekRecordQuery() : header(MT_CROSS_1V1_WEEK_RECORD_QUERY_CS) {}
		MessageHeader header;
	};

	struct SCCross1v1WeekRecord			// 14101 ���˼�¼����
	{
	public:
		SCCross1v1WeekRecord() : header(MT_CROSS_1V1_WEEK_RECORD_SC) {}
		MessageHeader header;

		struct RecordInfo
		{
			short result;				// 1��ʤ����0��ʧ��
			short reserve;
			int oppo_plat_type;			// ����ƽ̨����
			int oppo_server_id;			// �������ڷ�ID
			int oppo_role_uid;			// ����uid
			long long oppo_capability;	// ����ս��
			GameName oppo_name;			// ��������
			int add_score;			// ���λ�û���
		};

		short win_this_week;			// ����ʤ������
		short lose_this_week;			// ����ʧ�ܴ���

		int record_count;
		RecordInfo record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT];
	};

	struct CSGetCross1V1RankList	// 14117 ��ȡ���а�
	{
		CSGetCross1V1RankList() : header(MT_CROSS_1V1_GET_RANK_LIST_CS) {}
		MessageHeader header;
	};

	struct SCCross1V1RankList	// 14102 �������а�
	{
		SCCross1V1RankList() : header(MT_CROSS_1V1_RANK_LIST_SC) {}
		MessageHeader header;

		int count;
		CrossActivity1V1RankItem rank_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX];
	};

	//////////////////////////////////////////////////////////////////////////////////////////

	struct SCCrossMultiuserChallengeMatchingState  //14130 ���3V3ƥ��״̬֪ͨ
	{
	public:
		SCCrossMultiuserChallengeMatchingState() : header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_SC) {}
		MessageHeader	header;

		struct MatchingUserInfo
		{
			int				plat_type;
			int				server_id;
			int				role_id;
			GameName		role_name;
			char			reserved;
			char			sex;
			char			prof;
			char			camp;
			int				level;
			int				challenge_score;
			int				win_percent;
			long long		capability;
			int				mvp_count;
		};

		int matching_state;   //��ǰ����״̬ 0 ƥ������� 1 ƥ�������  2 ƥ�����  3 ƥ��ȡ��
		int user_count;
		MatchingUserInfo user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	struct CSGetMultiuserChallengeRankList    //14145 ��ȡ���3V3���а��б�
	{
	public:
		CSGetMultiuserChallengeRankList() : header(MT_GET_MULTIUSER_CHALLENGE_RANK_LIST_CS) {}
		MessageHeader	header;

		int rank_type;
	};

	struct CSCheckMultiuserChallengeHasMatch  //14146 ���3V3�������Ƿ��б���
	{
	public:
		CSCheckMultiuserChallengeHasMatch() : header(MT_MULTIUSER_CHALLENGE_CHECK_USER_HAS_MATCH_CS) {}
		MessageHeader	header;
	};

	struct SCMultiuserChallengeRankList  //14131 ���3V3���а��б�
	{
	public:
		SCMultiuserChallengeRankList() : header(MT_CROSS_MULTIUSER_CHALLENGE_RANKLIST_SC) {}
		MessageHeader	header;

		short rank_type;
		short count;
		CrossMultiuserChallengeUserRankItem rank_list[CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX];
	};

	struct SCMultiuserChallengeHasMatchNotice //14132 ���3V3�Ƿ��б���֪ͨ
	{
	public:
		SCMultiuserChallengeHasMatchNotice() : header(MT_CROSS_MULTIUSER_CHALLENGE_HAS_MATCH_NOTICE_SC) {}
		MessageHeader	header;

		int has_match;
	};
	
	struct SCNoticeCanEnterCross		// 14003 ֪ͨ��ҽ�����
	{
	public:
		SCNoticeCanEnterCross() : header(MT_NOTICE_CAN_ENTER_CROSS_SC) {}
		MessageHeader	header;

		int activity_type;
	};

	struct SCNoticeCrossTeamFBRoomInfoChange    // 14250 �����ӱ���������Ϣ�ı�
	{
	public:
		SCNoticeCrossTeamFBRoomInfoChange() : header(MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_CHANGE_SC) {}
		MessageHeader	header;

		int layer;
		int room;
		long long opera_uuid;
		int opera_type;
	};

	struct SCCrossTeamFBRoomListInfo    // 14251 �����ӱ��������б���Ϣ
	{
	public:
		SCCrossTeamFBRoomListInfo() : header(MT_NOTICE_CROSS_TEAM_FB_ROOM_LIST_INFO_SC) {}
		MessageHeader	header;

		struct RoomInfo
		{
			RoomInfo() : need_capability(0), password(0), is_auto_start(0), leader_prof(0), leader_sex(0), fb_state(0), user_count(0)
			{
				memset(leader_name, 0, sizeof(leader_name));
			}

			int need_capability;
			int password;
			int is_auto_start;
			GameName leader_name;
			char leader_prof;
			char leader_sex;
			char fb_state;
			char user_count;
			int room;
			int layer;
		};

		int room_count;
		RoomInfo room_info[CROSS_TEAM_FB_ROOM_MAX];
	};

	struct SCCrossTeamFBRoomInfo    // 14252 �����ӱ���������Ϣ
	{
	public:
		SCCrossTeamFBRoomInfo() : header(MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_SC) {}
		MessageHeader	header;

		struct UserInfo
		{
			UserInfo() : uuid(0), prof(0), sex(0), camp(0), user_state(0), capability(0)
			{
				memset(name, 0, sizeof(name));
			}

			long long uuid;
			char prof;
			char sex;
			char camp;
			char user_state;
			GameName name;
			int capability;
			int index;
		};

		int layer;
		int room;
		short is_auto_start;
		short fb_state;
		int user_count;
		UserInfo User_info[CROSS_TEAM_FB_ROLE_MAX];
	};
	//////////////////////////////////////// ��������� /////////////////////////////////////////
	class CSCrossXiuluoTowerDropLog			// 14190 ���������������־��Ϣ����
	{
	public:
		CSCrossXiuluoTowerDropLog() : header(MT_CROSS_XIULUO_TOWER_GET_DROP_LOG_CS) {};
		MessageHeader header;
	};

	class SCCrossXiuluoTowerDropLog		// 14205 ���������������־��Ϣ�·�
	{
	public:
		SCCrossXiuluoTowerDropLog() : header(MT_CROSS_XIULUO_TOWER_DROP_LOG_SC) {};
		MessageHeader header;

		int log_count;									// ��־����
		CrossXiuluoTowerDropLog log_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX];	// ��־�б�
	};
	//////////////////////////////////////// �������ս��־ /////////////////////////////////////////
	class CSCrossGuildBattleDropLog					// 14220 �������ս������־��Ϣ����
	{
	public:
		CSCrossGuildBattleDropLog() : header(MT_CROSS_GUILD_BATTLE_GET_DROP_LOG_CS) {};
		MessageHeader header;
	};

	class CSCrossGuildBattleGetMonsterInfoReq		// 14221, �����ÿ������ս��Ӣ����Ϣ
	{
	public:
		CSCrossGuildBattleGetMonsterInfoReq() : header(MT_CROSS_GUILD_BATTLE_GET_MONSTER_INFO_CS) {}
		MessageHeader header;
	};

	class SCCrossGuildBattleDropLog					// 14235  �������ս������־��Ϣ�·�
	{
	public:
		SCCrossGuildBattleDropLog() : header(MT_CROSS_GUILD_BATTLE_DROP_LOG_SC) {};
		MessageHeader header;

		int log_count;									// ��־����
		CrossGuildBattleDropLog log_list[CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX];	// ��־�б� CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX = 100
	};

	//////////////////////////////////////// ����ش� /////////////////////////////////////////
	enum CROSS_LIEKUNFB_REQ_TYPE
	{
		LIEKUNFB_TYPE_GET_PLAYER_INFO = 0,	// ��ȡ�����Ϣ param1:0 param2:����id

		LIEKUNFB_TYPE_MAX,
	};

	struct CSCrossLieKunFBReq		// 14160  �������ش���������Ϣ
	{
	public:
		CSCrossLieKunFBReq() : header(MT_CROSS_LIEKUN_FB_REQ_CS) {}

		MessageHeader	header;

		short type;
		short param1;
		int param2;
	};

	struct SCCrossLieKunFBPlayerInfo	// 14175 �������ش��������Ϣ
	{
	public:
		SCCrossLieKunFBPlayerInfo() : header(MT_CROSS_LIEKUN_FB_PLAYER_INFO_SC) {}

		MessageHeader	header;

		char is_enter_main_zone;				// �Ƿ���Խ���������
		char reserve_ch;						// ����
		short reserve_sh;						// ����
		int role_num[LIEKUN_ZONE_TYPE_COUNT];	// �����������
	};

	//////////////////////////////////////// ����߾�֮�� /////////////////////////////////////////
	struct CSCrossServerBianJingZhiDiBossInfoReq		// 14285  ����߾�֮�أ�ԭ������BOSS��Ϣ
	{
	public:
		CSCrossServerBianJingZhiDiBossInfoReq() : header(MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_CS) {}

		MessageHeader	header;

	};

	struct SCCrossServerBianJingZhiDiBossInfo	// 14280 ����߾�֮�أ�����·�BOSS��Ϣ
	{
	public:
		SCCrossServerBianJingZhiDiBossInfo() : header(MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_SC) {}

		MessageHeader	header;

		short boss_num;				// ����BOSS����
		short boss_total_num;		// BOSS����
		int left_boss_refresh_time; // ʣ���bossˢ��ʱ�䣨-1��ʾû�У�Ӧ���ǻ�û��ʼ���
	};

	//////////////////////////////////////// ������� /////////////////////////////////////////
	struct CSCrossRAChongzhiRankGetRank			// 14901
	{
	public:
		CSCrossRAChongzhiRankGetRank() : header(MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_CS) {}
		MessageHeader	header;
	};

	struct SCCrossRAChongzhiRankGetRankACK		// 14907
	{
	public:
		SCCrossRAChongzhiRankGetRankACK() : header(MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_ACK_SC) {}
		MessageHeader	header;

		struct RankItem
		{
			long long total_chongzhi;
			GameName mvp_name;
			int mvp_server_id;
			int mvp_plat_type;
		};

		int rank_count;
		RankItem rank_list[CROSS_RA_CHONGZHI_RANK_MAX_NUM];
	};

	//////////////////////////////////////// ������� /////////////////////////////////////////
	struct CSCrossRAConsumeRankGetRank			// 14908
	{
	public:
		CSCrossRAConsumeRankGetRank() : header(MT_CROSS_RA_CONSUME_RANK_GET_RANK_CS) {}
		MessageHeader	header;
		int modify_id;
	};

	struct SCCrossRAConsumeRankGetRankACK		// 14909
	{
	public:
		SCCrossRAConsumeRankGetRankACK() : header(MT_CROSS_RA_CONSUME_RANK_GET_RANK_ACK_SC) {}
		MessageHeader	header;

		struct RankItem
		{
			long long total_consume;
			GameName mvp_name;
			int mvp_server_id;
			int mvp_plat_type;
		};

		int modify_id;
		int rank_count;
		RankItem rank_list[CROSS_RA_CONSUME_RANK_MAX_NUM];
	};
}

#pragma pack(pop)

#endif	// __MSG_CROSS_H__

