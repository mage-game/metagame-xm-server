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
	struct CSCrossGetPersonRankList			// 14010 获取跨服排行榜列表
	{
	public:
		CSCrossGetPersonRankList() : header(MT_CROSS_GET_PERSON_RANK_LIST_CS) {}

		MessageHeader header;

		int rank_type;
	};

	struct SCGetCrossPersonRankListAck		// 14001 下发跨服排行榜列表
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

	struct SCGetCrossCoupleRankListAck			// 14002 下发跨服情侣榜列表
	{
	public:
		SCGetCrossCoupleRankListAck() : header(MT_CROSS_GET_COUPLE_RANK_LIST_ACK_SC) {}
		MessageHeader header;

		struct CrossCoupleRankListItem
		{
			int plat_type;			// 平台类型
			int server_id;			// 服务器id
			int male_uid;			// 男uid
			int female_uid;			// 女uid
			GameName male_name;		// 男名字
			GameName female_name;	// 女名字
			char male_prof;			// 男职业
			char female_prof;		// 女职业
			short reserve_sh;		// 保留
			int male_rank_value;	// 男排行值
			int female_rank_value;	// 女排行值
		};

		int rank_type;
		int count;
		CrossCoupleRankListItem rank_list[CROSS_COUPLE_RANK_MAX_SIZE];
	};

	///////////////////////////////////////////跨服1v1///////////////////////////////////////////////
	struct CSCross1v1MatchQuery	// 14115 匹配状态查询
	{
	public:
		CSCross1v1MatchQuery() : header(MT_CROSS_1V1_MATCH_QUERY_CS) {}
		MessageHeader header;

		int req_type;
	};

	struct SCCross1v1MatchAck	// 14100 匹配状态
	{
	public:
		SCCross1v1MatchAck() : header(MT_CROSS_1V1_MATCH_ACK_SC) {}
		MessageHeader header;

		short result;					// 0 : 没匹配，1：匹配中
		short reserve;
		unsigned int match_end_left_time;	// 预期匹配成功的时间戳
	};

	struct SCCross1v1MatchResult	// 14103 匹配结果
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
		long long oppo_capability;				// 战力
	};

	struct SCCross1v1FightResult               // 14104 战斗结果
	{
	public:
		SCCross1v1FightResult() : header(MT_CROSS_1V1_FIGHT_RESULT_SC) {}
		MessageHeader		header;

		int result;						// 1:胜利，0:失败
		int award_score;				// 奖励积分
	};

	struct CSCross1v1WeekRecordQuery		// 14116 个人记录查询
	{
	public:
		CSCross1v1WeekRecordQuery() : header(MT_CROSS_1V1_WEEK_RECORD_QUERY_CS) {}
		MessageHeader header;
	};

	struct SCCross1v1WeekRecord			// 14101 个人记录返回
	{
	public:
		SCCross1v1WeekRecord() : header(MT_CROSS_1V1_WEEK_RECORD_SC) {}
		MessageHeader header;

		struct RecordInfo
		{
			short result;				// 1：胜利，0：失败
			short reserve;
			int oppo_plat_type;			// 对手平台类型
			int oppo_server_id;			// 对手所在服ID
			int oppo_role_uid;			// 对手uid
			long long oppo_capability;	// 对手战力
			GameName oppo_name;			// 对手名字
			int add_score;			// 本次获得积分
		};

		short win_this_week;			// 本周胜利次数
		short lose_this_week;			// 本周失败次数

		int record_count;
		RecordInfo record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT];
	};

	struct CSGetCross1V1RankList	// 14117 获取排行榜
	{
		CSGetCross1V1RankList() : header(MT_CROSS_1V1_GET_RANK_LIST_CS) {}
		MessageHeader header;
	};

	struct SCCross1V1RankList	// 14102 返回排行榜
	{
		SCCross1V1RankList() : header(MT_CROSS_1V1_RANK_LIST_SC) {}
		MessageHeader header;

		int count;
		CrossActivity1V1RankItem rank_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX];
	};

	//////////////////////////////////////////////////////////////////////////////////////////

	struct SCCrossMultiuserChallengeMatchingState  //14130 跨服3V3匹配状态通知
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

		int matching_state;   //当前参赛状态 0 匹配队友中 1 匹配对手中  2 匹配完成  3 匹配取消
		int user_count;
		MatchingUserInfo user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	struct CSGetMultiuserChallengeRankList    //14145 获取跨服3V3排行榜列表
	{
	public:
		CSGetMultiuserChallengeRankList() : header(MT_GET_MULTIUSER_CHALLENGE_RANK_LIST_CS) {}
		MessageHeader	header;

		int rank_type;
	};

	struct CSCheckMultiuserChallengeHasMatch  //14146 跨服3V3检查玩家是否有比赛
	{
	public:
		CSCheckMultiuserChallengeHasMatch() : header(MT_MULTIUSER_CHALLENGE_CHECK_USER_HAS_MATCH_CS) {}
		MessageHeader	header;
	};

	struct SCMultiuserChallengeRankList  //14131 跨服3V3排行榜列表
	{
	public:
		SCMultiuserChallengeRankList() : header(MT_CROSS_MULTIUSER_CHALLENGE_RANKLIST_SC) {}
		MessageHeader	header;

		short rank_type;
		short count;
		CrossMultiuserChallengeUserRankItem rank_list[CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX];
	};

	struct SCMultiuserChallengeHasMatchNotice //14132 跨服3V3是否有比赛通知
	{
	public:
		SCMultiuserChallengeHasMatchNotice() : header(MT_CROSS_MULTIUSER_CHALLENGE_HAS_MATCH_NOTICE_SC) {}
		MessageHeader	header;

		int has_match;
	};
	
	struct SCNoticeCanEnterCross		// 14003 通知玩家进入跨服
	{
	public:
		SCNoticeCanEnterCross() : header(MT_NOTICE_CAN_ENTER_CROSS_SC) {}
		MessageHeader	header;

		int activity_type;
	};

	struct SCNoticeCrossTeamFBRoomInfoChange    // 14250 跨服组队本，房间信息改变
	{
	public:
		SCNoticeCrossTeamFBRoomInfoChange() : header(MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_CHANGE_SC) {}
		MessageHeader	header;

		int layer;
		int room;
		long long opera_uuid;
		int opera_type;
	};

	struct SCCrossTeamFBRoomListInfo    // 14251 跨服组队本，房间列表信息
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

	struct SCCrossTeamFBRoomInfo    // 14252 跨服组队本，房间信息
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
	//////////////////////////////////////// 跨服修罗塔 /////////////////////////////////////////
	class CSCrossXiuluoTowerDropLog			// 14190 跨服修罗塔掉落日志信息请求
	{
	public:
		CSCrossXiuluoTowerDropLog() : header(MT_CROSS_XIULUO_TOWER_GET_DROP_LOG_CS) {};
		MessageHeader header;
	};

	class SCCrossXiuluoTowerDropLog		// 14205 跨服修罗塔掉落日志信息下发
	{
	public:
		SCCrossXiuluoTowerDropLog() : header(MT_CROSS_XIULUO_TOWER_DROP_LOG_SC) {};
		MessageHeader header;

		int log_count;									// 日志条数
		CrossXiuluoTowerDropLog log_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX];	// 日志列表
	};
	//////////////////////////////////////// 跨服帮派战日志 /////////////////////////////////////////
	class CSCrossGuildBattleDropLog					// 14220 跨服帮派战掉落日志信息请求
	{
	public:
		CSCrossGuildBattleDropLog() : header(MT_CROSS_GUILD_BATTLE_GET_DROP_LOG_CS) {};
		MessageHeader header;
	};

	class CSCrossGuildBattleGetMonsterInfoReq		// 14221, 请求获得跨服仙盟战精英怪信息
	{
	public:
		CSCrossGuildBattleGetMonsterInfoReq() : header(MT_CROSS_GUILD_BATTLE_GET_MONSTER_INFO_CS) {}
		MessageHeader header;
	};

	class SCCrossGuildBattleDropLog					// 14235  跨服帮派战掉落日志信息下发
	{
	public:
		SCCrossGuildBattleDropLog() : header(MT_CROSS_GUILD_BATTLE_DROP_LOG_SC) {};
		MessageHeader header;

		int log_count;									// 日志条数
		CrossGuildBattleDropLog log_list[CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX];	// 日志列表 CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX = 100
	};

	//////////////////////////////////////// 猎鲲地带 /////////////////////////////////////////
	enum CROSS_LIEKUNFB_REQ_TYPE
	{
		LIEKUNFB_TYPE_GET_PLAYER_INFO = 0,	// 获取玩家信息 param1:0 param2:帮派id

		LIEKUNFB_TYPE_MAX,
	};

	struct CSCrossLieKunFBReq		// 14160  跨服猎鲲地带，请求信息
	{
	public:
		CSCrossLieKunFBReq() : header(MT_CROSS_LIEKUN_FB_REQ_CS) {}

		MessageHeader	header;

		short type;
		short param1;
		int param2;
	};

	struct SCCrossLieKunFBPlayerInfo	// 14175 跨服猎鲲地带，玩家信息
	{
	public:
		SCCrossLieKunFBPlayerInfo() : header(MT_CROSS_LIEKUN_FB_PLAYER_INFO_SC) {}

		MessageHeader	header;

		char is_enter_main_zone;				// 是否可以进入主区域
		char reserve_ch;						// 保留
		short reserve_sh;						// 保留
		int role_num[LIEKUN_ZONE_TYPE_COUNT];	// 区域玩家人数
	};

	//////////////////////////////////////// 跨服边境之地 /////////////////////////////////////////
	struct CSCrossServerBianJingZhiDiBossInfoReq		// 14285  跨服边境之地，原服请求BOSS信息
	{
	public:
		CSCrossServerBianJingZhiDiBossInfoReq() : header(MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_CS) {}

		MessageHeader	header;

	};

	struct SCCrossServerBianJingZhiDiBossInfo	// 14280 跨服边境之地，跨服下发BOSS信息
	{
	public:
		SCCrossServerBianJingZhiDiBossInfo() : header(MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_SC) {}

		MessageHeader	header;

		short boss_num;				// 存活的BOSS数量
		short boss_total_num;		// BOSS总数
		int left_boss_refresh_time; // 剩余的boss刷新时间（-1表示没有，应该是还没开始活动）
	};

	//////////////////////////////////////// 跨服随机活动 /////////////////////////////////////////
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

	//////////////////////////////////////// 跨服随机活动 /////////////////////////////////////////
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

