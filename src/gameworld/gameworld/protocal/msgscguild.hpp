#ifndef __MSG_SC_GUILD_HPP_
#define __MSG_SC_GUILD_HPP_

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/guilddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCGuildOperaSucc
	{
	public:
		SCGuildOperaSucc();
		MessageHeader		header;

		enum OPERA_TYPE
		{
			OPERA_TYPE_INVALID = 0, 
			OPERA_TYPE_APPLY_SET = 1,
			OPERA_TYPE_CALL_IN = 2,

			OPERA_TYPE_MAX,
		};

		short				opera_type;
		short				reserve_sh;
	};

	class SCCreateGuild
	{
	public:
		SCCreateGuild();
		MessageHeader		header;

		int					ret;			// 0成功 其他失败 

		GuildID				guild_id;
		GuildName			guild_name;
	};

	class SCDismissGuild
	{
	public:
		SCDismissGuild();
		MessageHeader		header;

		int					ret;
	};

	class SCApplyForJoinGuild
	{
	public:
		SCApplyForJoinGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					ret;
	};

	class SCApplyForJoinGuildAck
	{
	public:
		SCApplyForJoinGuildAck();
		MessageHeader		header;

		int					result;
		GuildID				guild_id;
		GuildName			guild_name;
	};

	enum GUILD_NOTIFY_TYPE
	{
		GUILD_NOTIFY_TYPE_INVALID,
		GUILD_NOTIFY_TYPE_APPLYFOR,								// 有人申请加入
		GUILD_NOTIFY_TYPE_UNION_APPLYFOR,						// 有军团申请结盟
		GUILD_NOTIFY_TYPE_UNION_JOIN,							// 加入联盟
		GUILD_NOTIFY_TYPE_UNION_QUIT,							// 退出联盟
		GUILD_NOTIFY_TYPE_UNION_REJECT,							// 拒绝联盟
		GUILD_NOTIFY_TYPE_UNION_APPLYFOR_SUCC,					// 申请军团联盟成功 
		GUILD_NOTIFY_TYPE_MEMBER_ADD,							// 成员加入
		GUILD_NOTIFY_TYPE_MEMBER_REMOVE,						// 成员退出
		GUILD_NOTIFY_TYPE_MEMBER_SOS,							// 成员求救
		GUILD_NOTIFY_TYPE_MEMBER_HUNYAN,						// 成员有婚宴
		GUILD_NOTIFY_TYPE_REP_PAPER,							// 红包相关
		GUILD_NOTIFY_TYPE_GUILD_FB,								// 仙盟副本
		GUILD_NOTIFY_TYPE_GUILD_LUCKY,							// 仙盟运势
		GUILD_NOTIFY_TYPE_BOSS,									// 仙盟boss
		GUILD_NOTIFY_TYPE_GUILD_BONFIRE,						// 仙盟篝火
		GUILD_NOTIFY_TYPE_TOTEM_UPLEVEL,						// 仙盟图腾升级
		GUILD_NOTIFY_TYPE_BOSS_CALL,							// 仙盟BOSS召唤
		GUILD_NOTIFY_TYPE_CONSUME_RELIVE_TIMES,					// 公会总复活次数
		GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_START,					// 帮派运镖开始
		GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END,					// 帮派运镖结束
		GUILD_NOTIFY_TYPE_GUILD_BOX_ASSIST,						// 挖宝协助
		GUILD_NOTIFY_TYPE_TIANCI_TONGBI_OPEN,					// 天赐铜币开始
		GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_CUR_POS,				// 帮派运镖当前坐标
		GUILD_NOTIFY_TYPE_TIANCI_TONGBI_CLOSE,					// 天赐铜币结束
		GUILD_NOTIFY_TYPE_GET_GONGZI,							// 获得仙盟工资 notify_param: 工资
		GUILD_NOTIFY_TYPE_TOTAL_GONGZI_CHNAGE,					// 仙盟总工资变更 notify_param: 工资
		GUILD_NOTIFY_TYPE_GUILD_GOAL,							// 跨服目标仙盟目标
		GUILD_NOTIFY_TYPE_ENEMY_CHANGE,							// 仙盟仇人变更
		GUILD_NOTIFY_TYPE_STORAGE_LOG,							// 仙盟仓库日志

		GUILD_NOTIFY_TYPE_MAX,
	};

	class SCNotifyGuildSuper
	{
	public:
		SCNotifyGuildSuper();					// 9803
		MessageHeader		header;

		int					notify_type;
		int					notify_param;
		int					notify_param1;
	};

	class SCQuitGuild
	{
	public:
		SCQuitGuild();
		MessageHeader		header;

		int					ret;
	};

	class SCInviteGuild
	{
	public:
		SCInviteGuild();
		MessageHeader		header;

		int					ret;
	};

	class SCInviteNotify
	{
	public:
		SCInviteNotify();
		MessageHeader		header;

		GuildID				guild_id;
		int					invite_uid;
		GameName			invite_name;
		GuildName			guild_name;
	};

	class SCKickoutGuild
	{
	public:
		SCKickoutGuild();
		MessageHeader		header;

		int					bekick_uid;
		int					ret; 
	};

	class SCKickoutGuildNotify
	{
	public:
		SCKickoutGuildNotify();
		MessageHeader		header;

		GuildID				guild_id;
		int					kick_uid;
		GameName			kick_name;
	};

	class SCAppointGuild
	{
	public:
		SCAppointGuild();
		MessageHeader		header;

		GuildID				guild_id;
		int					beappoint_uid;
		int					post;
		int					ret; 
	};

	class SCLeavePost
	{
	public:
		SCLeavePost();
		MessageHeader		header;

		GuildID				guild_id;
		int					ret;
	};

	class SCChangeNotice
	{
	public:
		SCChangeNotice();
		MessageHeader		header;

		GuildID				guild_id;
		int					ret;
	};

	class SCGuildMailAll
	{
	public:
		SCGuildMailAll();
		MessageHeader		header;

		GuildID				guild_id;
		int					ret; 
	};

	class SCAllGuildBaseInfo	// 9813
	{
	public:
		SCAllGuildBaseInfo();
		MessageHeader		header;

		struct GuildBaseInfo
		{
			GuildID				guild_id;
			GuildName			guild_name;
			int					tuanzhang_uid;
			GameName			tuanzhang_name;
			unsigned int		create_time; 
			int					guild_level;
			int					cur_member_count;
			int					max_member_count;
			char				camp;
			char				vip_type;
			short				applyfor_setup;
			GuildID				union_guild_id;
			int					applyfor_need_capability;
			int					applyfor_need_level;
			int					active_degree;
			int					total_capability;
			int					is_apply;
		};
		
		int					free_create_guild_times;
		int					is_first;
		int					count;
		GuildBaseInfo		guild_list[MAX_ONCE_GUILD_COUNT];
	};

	class SCGuildGetApplyForList
	{
	public:
		SCGuildGetApplyForList();
		MessageHeader		header;

		struct ApplyFor
		{
			int				uid;
			GameName		role_name;
			int				level;
			char			sex;
			char			prof;
			char			vip_type;
			char			vip_level;
			int				capability;
			unsigned int	applyfor_time;
		};

		int					count;
		ApplyFor			apply_for_list[MAX_APPLY_FOR_COUNT];
	};

	class SCApplyForJoinGuildList
	{
	public:
		SCApplyForJoinGuildList();
		MessageHeader		header;

		static const int MAX_APPLY_FOR_JOIN_GUILD_COUNT = 1024;

		int					count;
		GuildID				applyfor_join_guild_list[MAX_APPLY_FOR_JOIN_GUILD_COUNT];
	};

	class SCInviteGuildList
	{
	public:
		SCInviteGuildList();
		MessageHeader		header;

		static const int MAX_INVITE_GUILD_COUNT = 1024;

		int					count;
		
		struct InviteGuild
		{
			GuildID guild_id;
			GuildName guild_name;
			int guild_level;
			int member_count;

			int invite_uid;
		};

		InviteGuild			invite_guild_list[MAX_INVITE_GUILD_COUNT];
	};

	class SCGuildEventList				// 9815 军团普通事件和大事件列表
	{
	public:
		SCGuildEventList();
		MessageHeader		header;

		struct Event
		{
			short event_type;
			short event_owner_post;
			GameName event_owner;
			unsigned int event_time;

			short big_event;
			short cannot_remove;

			int param0;
			int param1;
			int param2;
			int param3;
			GuildEventStrParam sparam0;
		};

		int					count;
		Event				event_for_list[MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT];
	};

	class SCGuildMemberList
	{
	public:
		SCGuildMemberList();
		MessageHeader		header;

		struct _Member
		{
			int				uid;
			GameName		role_name;
			int				level;
			char			sex;
			char			prof;
			char			post;
			char			vip_type;
			short			vip_level;
			short			is_online;
			unsigned int	join_time;
			unsigned int    last_login_time;

			int				gongxian;
			int				total_gongxian;

			int				capability;
			long long		avatar_timestamp;

			short			guild_sign_count;
			short			avatar_window;
		};
		
		GuildID				guild_id;
		int					count;
		_Member				member_list[MAX_MEMBER_COUNT];
	};

	class SCGuildBaseInfo	// 9817
	{
	public:
		SCGuildBaseInfo();
		MessageHeader		header;

		GuildID				guild_id;
		GuildName			guild_name;

		int					guild_level;
		int					guild_exp;
		int					guild_max_exp;

		int					totem_level;
		int					totem_exp;

		int					cur_member_count;
		int					max_member_count;

		int					tuanzhang_uid;
		GameName		 	tuanzhang_name;
		unsigned int		create_time;
		char				camp;
		char				vip_level;
		short				applyfor_setup;

		GuildNotice			guild_notice;

		int					auto_kickout_setup;

		int					applyfor_need_capability;
		int					applyfor_need_level;
		int					callin_times;

		int					my_lucky_color;
		int					active_degree;

		int					total_capability;
		int					rank;

		int					totem_exp_today;

		short				is_auto_clear;
		char				avatar_changed;
		char				is_today_biaoche_start;

		unsigned long long	avatar_timestamp;

		int					guild_total_gongzi;		// 仙盟总工资
	};

	class SCGuildMemberSos
	{
	public:
		SCGuildMemberSos();
		MessageHeader		header;

		int					sos_type;
		int					member_uid;
		GameName			member_name;
		int					member_scene_id;
		short				member_pos_x;
		short				member_pos_y;

		int					enemy_uid;
		GameName			enemy_name;
		int					enemy_camp;
		GuildID				enemy_guild_id;
		GuildName			enemy_guild_name;
	};

	class SCGuildResetName
	{
	public:
		SCGuildResetName();
		MessageHeader		header;

		int					guild_id;
		GameName			old_name;
		GameName			new_name;
	};

	class SCGuildRoleGuildInfo
	{
	public:
		SCGuildRoleGuildInfo();
		MessageHeader		header;

		int guild_gongxian;
		short territorywar_reward_flag;
		short reserve;
		int daily_gongxian;
		int day_juanxian_gold;						// 今日已捐献元宝

		short skill_level_list[GUILD_SKILL_COUNT];

		short item_count;
		RoleGuildParam::ExchangeRecordItem exchange_list[GUILD_MAX_EXCHANGE_ITEM_COUNT];
	};

	class SCNoticeGuildPaperInfo						// 9828 公会红包，可发红包列表
	{
	public:
		SCNoticeGuildPaperInfo();
		MessageHeader		header;

		enum NOTICE_REASON
		{
			HAS_CAN_CREATE_RED_PAPER = 0,
			HAS_CAN_FETCH_RED_PAPER,
			HAS_FETCH_RED_PAPER,

			MAX_NOTICE_REASON,
		};

		int notice_reson;
	};

	class SCGuildRedPaperListInfo			// 4215公会红包信息列表
	{
	public:
		SCGuildRedPaperListInfo();
		MessageHeader		header;

		static const int MAX_PAPER_COUNT = 100;

		struct PaperItem
		{
			int				uid;
			GameName		role_name;
			short			red_paper_state;
			short			red_paper_seq;
			int				red_paper_id;
			unsigned int	invalid_time;
			short			red_paper_index;
			short			all_fetch;
			long long		avator_timestamp;
			char			sex;
			char			prof;
			short			reserve;
		};

		int					count;
		PaperItem			paper_list[MAX_PAPER_COUNT];
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class SCGuildCheckCanDelateAck
	{
	public:
		SCGuildCheckCanDelateAck();
		MessageHeader header;

		int can_delate;
	};

	class SCGuildMemberNum				// 9827军团当前最大成员数量
	{
	public:
		SCGuildMemberNum();
		MessageHeader		header;

		int					max_guild_member_num;
	};

	//天赐铜币----------------------------------------------------------------
	class SCGuildTianCiTongBiResult  // 4305
	{
	public:
		SCGuildTianCiTongBiResult();
		MessageHeader		header;

		int reward_mojing_num;
		int gold_bind_num;
		long long total_exp;;
		int rank_pos;
		int gather_count_list[GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX];
	};

	class SCGuildSyncTianCiTongBi		// 4306
	{
	public:
		SCGuildSyncTianCiTongBi();
		MessageHeader header;

		GuildID				guild_id;
		int					is_open;
	};

	class SCGuildTianCiTongBiUserGatherChange // 4307
	{
	public:
		SCGuildTianCiTongBiUserGatherChange();
		MessageHeader		header;

		int 				gather_type;
		int					gather_num;
		int					tianci_tongbi_tree_maturity_degree;						// 树的成熟度
		int					tianci_tongbi_max_gather_num;
		int					tianci_tongbi_tree_max_maturity_degree;
	};

	class SCGuildTianCiTongBiRankInfo // 4308
	{
	public:
		SCGuildTianCiTongBiRankInfo();
		MessageHeader		header;

		unsigned int tianci_tongbi_close_time;
		short rank_count;
		short reserver;
		GuildTianCiTongBiRankItem rank_item_list[GUILD_TIANCI_TONGBI_RANK_NUM];
	};

	class SCGuildTianCiTongBiNpcInfo // 4310
	{
	public:
		SCGuildTianCiTongBiNpcInfo();
		MessageHeader		header;

		int npc_x;
		int npc_y;
		int tianci_tongbi_readiness_time;
	};

	class SCGuildTianCiTongBiGatherAOIInfo // 4311 广播采集信息
	{
	public:
		SCGuildTianCiTongBiGatherAOIInfo();
		MessageHeader		header;

		ObjID obj_id;						// 角色obj
		short gather_type;					// 采集物类型
	};

	class SCGuildGongziRankList			// 4312 仙盟工资排行
	{
	public:
		SCGuildGongziRankList();
		MessageHeader		header;

		GuildID				guild_id;

		int					count;
		MemberGongziInfo	member_list[MAX_MEMBER_COUNT];
	};

	class SCGuildEnemyRankList			// 4313 仙盟仇人排行
	{
	public:
		SCGuildEnemyRankList();
		MessageHeader		header;

		struct GEnemyInfo
		{
			int				rank;			// 排名
			long long		enemy_uuid;		// 仇人uuid
			GameName		enemy_name;		// 仇人名字
			int				kill_score;		// 仇恨值
			unsigned int	last_kill_timestamp; // 仇人上次击杀成员的时间
		};

		int			count;
		GEnemyInfo	enemy_list[GUILD_ENEMY_RANK_MAX_COUNT];
	};

}

#pragma pack(pop)

#endif  // __MSG_SC_GUILD_HPP_

