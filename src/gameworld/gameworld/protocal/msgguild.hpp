/******************************************************************************************
FileName: msgguild.hpp
Author: shell
Description: 军团GameWorld与客户端通信协议 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/11/ 16:32:08     1.0           创建
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
		int					knapsack_index;		// 物品在背包中的索引
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
		int					is_auto_join;		// 一键加入 是:否 1:0
	};

	class CSApplyForJoinGuildAck
	{
	public:
		CSApplyForJoinGuildAck();
		MessageHeader		header;

		GuildID				guild_id;
		int					result;			// 0为同意  其他不同意
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
		int					knapsack_index;		// 弹劾物品在背包中的索引
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

		ADD_GUILD_EXP_TYPE_GOLD,							// 元宝捐献
		ADD_GUILD_EXP_TYPE_ITEM,							// 物品捐献

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

	// 仙盟仓库 begin --------------------------------------------------------------------------------
	enum GUILD_STORGE_OPERATE
	{
		GUILD_STORGE_OPERATE_PUTON_ITEM = 1,	// 捐献/放入 param1背包索引 param2数量
		GUILD_STORGE_OPERATE_TAKE_ITEM,			// 兑换/取出 param1仓库格子索引 param2数量 param3物品ID
		GUILD_STORGE_OPERATE_REQ_INFO,			// 请求信息
		GUILD_STORGE_OPERATE_DISCARD_ITEM,		// 销毁 param1仓库格子索引 param2物品ID
	};

	class CSGuildStorgeOperate				// 4289 仙盟仓库操作请求
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
		GUILD_STORGE_OPERATE_PUTON_ITEM_ONE_KEY = 1,	// 捐献
		GUILD_STORGE_OPERATE_DISCARD_ITEM_ONE_KEY,		// 销毁 
	};

	class CSGuildStorgeOneKeyOperate			// 4295 仙盟仓库一键操作请求
	{
	public:
		CSGuildStorgeOneKeyOperate();
		MessageHeader		header;

		struct ItemInfo
		{
			ItemInfo():item_index(0), param_1(0){}

			short item_index;			// 捐献传背包索引，销毁传仓库格子索引
			unsigned short param_1;		// 捐献传num，销毁传item_id
		};

		int operate_type;		// GUILD_STORGE_ONE_KEY_OPERATE
		int item_count;
		ItemInfo item_list[GUILD_STORAGE_MAX_GRID_NUM];
	};
	// 仙盟仓库 end --------------------------------------------------------------------------------

	class CSGuildSetAutoClearReq		// 4304 设置自动清理公会成员
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

		short up_type; //0 普通升级 1 全部升级
		short skill_index;
	};

	class CSGuildUpTotemLevel						// 4291仙盟图腾升级
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
		GBOT_INVITE_ASSIST,         // 邀请协助 p1:对方uid
		GBOT_THANK_ASSIST,         // 感谢协助  p1:宝箱index  p2:对方uid 
	};

	class CSGuildBoxOperate							// 4292仙盟宝箱操作
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
		GUILD_COMMON_REQ_TYPE_FETCH_REWARD = 0,   // 领取每日奖励
		GUILD_COMMON_REQ_TYPE_GIVE_GONGZI,	      // 发工资
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

		int today_guild_pao_saizi_times;					// 每天公会抛骰子次数
		long long today_last_guild_pao_saizi_time;			// 最后一次抛骰子时间
		int today_guild_saizi_score;						// 每天骰子积分
		int pao_saizi_num;									// 抛到什么分数

		GuildSaiziRankItem guild_saizi_rank_list[MAX_MEMBER_COUNT]; // 排行信息
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

	class SCGuildFbStatus				// 4205仙盟密境状态信息
	{
	public:
		SCGuildFbStatus();
		MessageHeader header;

		short open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
	};

	class CSGuildFbStartReq				// 4275仙盟秘境开启请求
	{
	public:
		CSGuildFbStartReq();
		MessageHeader header;
	};

	class SCGuildBonfireStatus			// 4208仙盟篝火状态信息
	{
	public:
		SCGuildBonfireStatus();
		MessageHeader header;

		short open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
	};

	class CSGuildBonfireStartReq		// 4286仙盟篝火开启请求 
	{
	public:
		CSGuildBonfireStartReq();
		MessageHeader header;
	};

	class CSGuildBonfireGotoReq			// 4287仙盟篝火前往请求
	{
	public:
		CSGuildBonfireGotoReq();
		MessageHeader header;
	};
	
	class CSGuildBonfireAddMucaiReq		// 4288加木材请求
	{
	public:
		CSGuildBonfireAddMucaiReq();
		MessageHeader header;

	};

	class CSGuildFbEnterReq				// 4276仙盟密境进入请求
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

		int invite_uid;				// 0是一键祝福
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

	class SCGuildActiveDegreeInfo // 仙盟凶兽活动信息  4723
	{
	public:
		SCGuildActiveDegreeInfo();
		MessageHeader header;

		short today_open_times;
		short reserve_sh;
		unsigned int finish_timestamp;
		int active_degree;
	};

	class CSGuildCallBeastReq // 仙盟凶兽，召唤BOSS请求  4280
	{
	public:
		CSGuildCallBeastReq();
		MessageHeader header;
	};

	class CSGuildActiveDegreeInfoReq // 仙盟凶兽活动信息  4281
	{
	public:
		CSGuildActiveDegreeInfoReq();
		MessageHeader header;
	};

	class CSGuildExtendMemberReq	// 仙盟扩展成员请求 4296
	{
	public:
		CSGuildExtendMemberReq();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			EXTEND_MEMBER = 0,		// 扩展成员
			MEMBER_MAX_COUNT_INFO,	// 请求最大成员信息
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
		NOTIFY_INFO_TYPE_SCENE = 0,					// 场景信息
		NOTIFY_INFO_TYPE_RANK = 1,					// 排行信息通知
		NOTIFY_INFO_TYPE_ROLE_INFO_CHANGE = 2,		// 个人信息变化通知
		NOTIFY_INFO_TYPE_STAR_LEVEL_CHANGE = 3,		// 星级变化通知

	};

	class SCGuildBossActivityInfo			// 4213 仙盟boss活动信息
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
		
		short notify_type;						// 消息通知类型
		short reserve_sh;					
		int boss_id;
		int boss_level;
		ObjID boss_objid;
		short is_super_boss;
		long long totem_totem_exp;
		short cur_star_level;					// 当前星级
		short gather_num;						// 已采集数量
		unsigned int next_change_star_time;		// 距离下次改变星级时间戳

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT]; // 排名信息
	};

	// 仙盟仓库 beign -------------------------------------------------------------------------------------------------------
	class SCGuildStorgeInfo					// 4209 仙盟仓库信息
	{
	public:
		SCGuildStorgeInfo();
		MessageHeader header;

		int open_grid_count;	// 仓库开启的格子数量
		int storage_score;		// 积分

		int count;
		ItemDataWrapper storge_item_list[GUILD_STORAGE_MAX_GRID_NUM];  // 仓库格子信息 102个
	};

	class SCGuildStorgeChange				// 4210 仙盟仓库改变
	{
	public:
		SCGuildStorgeChange();
		MessageHeader header;

		int index;
		ItemDataWrapper item_datawrapper;
	};

	class SCGuildStorageLogList				// 4216 仙盟仓库日志事件列表
	{
	public:
		SCGuildStorageLogList();
		MessageHeader		header;

		struct StorageLog
		{
			GameName log_owner_name;	// 名字
			unsigned int log_time;		// 时间

			int opt_type;				// 操作类型 GUILD_STORE_OPTYPE
			int item_id;				// 物品ID
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 仙品属性
		};

		int					count;
		StorageLog			log_list[MAX_STORAGE_EVENT_COUNT];
	};
	// 仙盟仓库 end -------------------------------------------------------------------------------------------------------

	class SCGuildBoxInfo							// 4211仙盟宝箱信息
	{
	public:
		SCGuildBoxInfo();
		MessageHeader header;

		struct InfoData
		{
			short box_level;
			short is_reward;
			int assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT];
			char is_thank_assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT]; // 是否已感谢
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

	class SCGuildBoxNeedAssistInfo					// 4212仙盟宝箱需要协助信息
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

	class CSGuildRedPaperListInfoReq				// 4214请求公会红包信息列表
	{
	public:
		CSGuildRedPaperListInfoReq();
		MessageHeader header;
	};

	class CSCreateGuildRedPaperReq					// 9829请求创建公会红包
	{
	public:
		CSCreateGuildRedPaperReq();
		MessageHeader header;

		int paper_seq;
		int fetech_time;
		int red_paper_index;
	};

	class CSSingleChatRedPaperRole					// 9830私聊有红包未派发的玩家
	{
	public:
		CSSingleChatRedPaperRole();
		MessageHeader header;

		int red_paper_index;
	};

	enum GUILD_MAZE_OPERATE_TYPE
	{
		GUILD_MAZE_OPERATE_TYPE_GET_INFO,			// 请求信息
		GUILD_MAZE_OPERATE_TYPE_SELECT,				// 选门
	};

	class CSGuildMazeOperate						// 9831公会迷宫操作请求
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

	class SCGuildMemberMazeInfo						// 9832公会成员迷宫信息
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

	class SCGuildMazeRankInfo						// 9833公会迷宫排行信息
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
		GUILD_SINGIN_REQ_TYPE_SIGNIN,								// 签到
		GUILD_SINGIN_REQ_TYPE_FETCH_REWARD,							// 拿奖励 p1 index
		GUILD_SINGIN_REQ_ALL_INFO,									// 请求所有信息
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

		int is_signin_today;										// 今天是否已签到
		char signin_count_month;									// 月签到次数
		char guild_signin_fetch_reward_flag;						// 工会总签到领取标志
		short guild_signin_count_today;								// 工会总签到次数
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
		BIAOCHE_OPERA_TYPE_START = 0,		// 开始护送 param_1:guild_id
		BIAOCHE_OPERA_TYPE_TRANS,			// 传送到镖车 
		BIAOCHE_OPERA_TYPE_BIAOCHE_POS,		// 镖车坐标

	};

	class CSBiaoCheOpera				// 9838 请求操作
	{
	public:
		CSBiaoCheOpera();
		MessageHeader header;

		int opera_type;
		int param_1;
	};

	//天赐铜币----------------------------------------------------------------
	enum GUILD_TIANCITONGBI_REQ_TYPE
	{
		GUILD_TIANCITONGBI_REQ_TYPE_OPEN = 0,		 // 开启 p1:guild_id p2:role_id
		GUILD_TIANCITONGBI_REQ_TYPE_USE_GATHER,		 // 提交采集物
		GUILD_TIANCITONGBI_REQ_TYPE_RANK_INFO,		 // 请求排名信息
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

	class CSGuildEnemyRankList			// 4314 获取仙盟仇人排行
	{
	public:
		CSGuildEnemyRankList();
		MessageHeader		header;
	};
}

#pragma pack(pop)

#endif


