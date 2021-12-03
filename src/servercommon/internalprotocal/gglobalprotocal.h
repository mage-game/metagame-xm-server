#ifndef __GGLOBALPROTOCAL_H__
#define __GGLOBALPROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"

#include "servercommon/maildef.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/tradedef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/roleactivitydef.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/marrydef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace gglobalprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		///////////////////////////////////////////   基本协议   /////////////////////////////////////////
		MT_GGLOBAL_BEGIN = 21000,
		MT_USER_MSG,										// 用户在服务器内部消息 （ggprotocal协议中的内容）	

		MT_MSG_TO_CROSS_SERVER,								// 给cross发消息 (crossgameprotocal协议)
		MT_CROSS_GAME_MSG,									// cross发给gameworld的消息 (crossgameprotocal协议)	
		MT_MSG_TO_USER,										// 给用户发消息 （Protocal用户协议内容）


		///////////////////////////////////////////   拍卖   /////////////////////////////////////////
		MT_ADD_PUBLICSALE_ITEM,								// 增加拍卖物品
		MT_REMOVE_PUBLICSALE_ITEM,							// 删除拍卖物品
		MT_BUY_PUBLICSALE_ITEM,								// 购买物品
		MT_PUBLICSALE_GET_ITEM_LIST,						// 拉取拍卖物品列表
		MT_PUBLICSALE_SEND_ITEM_INFO_TO_WORLD,				// 发送拍卖物品信息到世界聊天频道


		///////////////////////////////////////////   其他   /////////////////////////////////////////
		MT_SYNC_PERSON_RANK_INFO,							// 同步排行榜信息
		MT_SYSTEMCMD_RELOAD,								// 通知gameworld重读配置表的命令
		MT_SYSTEMCMD_TO_WORLD_COUNT,						// 设置全服角色功能次数（偷猪次数等）
		MT_SYSTEMCMD_TO_WORLD_FB_COUNT,						// 设置全服角色副本次数
		MT_GAMEWORLD_SEND_MAIL_TO_USER,						// gamewaorld给用户发邮件
		MT_SYNC_ROLE_JOIN_CAMP,								// 有用户加入阵营
		MT_ACTIVITY_STATUS,									// 活动状态广播
		MT_CHECK_SHOPLIMIT_INFO,							// 检查商城购买限购物品
		MT_ACTIVITY_FORCE_TO_NEXT_STATE,					// 切换活动到下一状态
		MT_EVALUATE_ROLE,									// 评价玩家
		MT_SYNC_BOSS_FLUSH_TIME,							// 同步BOSS刷新时间
		MT_ROLE_RESET_NAME,									// 角色改名
		MT_SYNC_TEAM_RANK_INFO,								// 同步组队排行榜信息
		MT_ADD_CHEST_SHOP_NEWS_ITEM,						// 增加宝箱消息
		MT_FORBID_SUBLOCK,									// 全局禁止二级锁
		MT_FORCE_SET_MICRO_PC_FLAG,							// 全局设置登陆器是否开启标记
		MT_FORCE_SET_AUTO_FORBID,							// 是否自动封号
		MT_LINGMAI_INFO_CHANGE,								// 灵脉信息改变
		MT_ADD_MYSTERIOUS_SHOP_NEWS_ITEM,					// 增加神秘商店消息
		MT_KILL_OTHER_CAMP,									// 击杀其他阵营玩家
		MT_MINGRENTANG_EVENT,								// 完成名人堂事件
		MT_ADD_CARD_LUCKY_NEWS_ITEM,						// 增加卡牌抽奖幸运公告
		MT_ADD_FB_XHT_PASS_NEWS_ITEM,						// 增加仙魂塔副本通关记录
		MT_SYNC_WORLD_LEVEL_INFO,							// 同步服务器等级信息
		MT_SYNC_LAST_BATTLEFILED_RANK_INFO,					// gameworld->global 同步战场排行
		MT_SYNC_ROLE_SET_PROF,								// 同步角色选择职业
		MT_GM_WEEK_CHANGE,									// gameworld->global WeekChange
		MT_GM_EVALUATE_ROLE,								// GM评价玩家
		MT_ADD_1V1_NEWSITEM,								// 1v1本周战绩
		MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_REQ,			// gameworld->global 取角色战力排行榜数据
		MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_ACK,			// global->gameworld 取角色战力排行榜数据返回
		MT_ADD_RANDACTIVITY_LUCKY_ROLL_NEWS_ITEM,			// 增加随机活动幸运抽奖幸运公告
		MT_DATING_INVITE_REQ,								// 约会邀请
		MT_DATING_INVITE_CHECK_TARGET_RET,					// 约会邀请检查被邀请人返回
		MT_DATING_INVITE_ACK,								// 约会邀请回复
		MT_DATING_GM_FORCE_INVITE,							// GM约会强制邀请
		MT_DATING_GIVE_PRESENT_ACK,							// 约会赠送礼物返回
		MT_DATING_DAYCOUNT_CHECK_ACK,						// 约会扣每日次数成功返回
		MT_ADD_COMBINE_QINGDIAN_ROLL_NEWS_ITEM,				// 合服庆典转盘幸运公告

		///////////////////////////////////////////   P2P交易   /////////////////////////////////////////
		MT_TRADE_AGREE,										// 双方同意 通知global交易开始
		MT_TRADE_ITEM_LIST,									// 交易物品上传 双方锁定后的操作
		MT_TRADE_AFFIRM,									// 通知global交易正式进行 双方确认后的操作
		MT_TRADE_CONSUME_RET,								// 扣除物品返回
		MT_TRADE_OVER,										// 交易终止通知global
		MT_TRADE_SUCC_RET,									// 单方交易成功

		///////////////////////////////////////////   军团   /////////////////////////////////////////
		MT_CREATE_GUILD,									// 创建军团
		MT_DISMISS_GUILD,									// 解散军团
		MT_APPLY_FOR_JOIN_GUILD,							// 申请加入
		MT_APPLY_FOR_JOIN_GUILD_ACK,						// 申请加入回复  审批 
		MT_QUIT_GUILD,										// 退出军团
		MT_INVITE_GUILD,									// 邀请加入
		MT_INVITE_GUILD_ACK,								// 邀请加入回复
		MT_KICKOUT_GUILD,									// 踢人
		MT_APPOINT_GUILD,									// 任命
		MT_LEAVE_POST,										// 卸任
		MT_GUILD_MAIL_ALL,									// 群发邮件
		MT_GUILD_CHANGE_NOTICE,								// 修改公告
		MT_ALL_GUILD_BASE_INFO,								// 获取所有军团列表
		MT_GET_APPLY_FOR_LIST,								// 获取申请列表
		MT_GET_MEMBER_LIST,									// 获取成员列表
		MT_GET_GUILD_BASE_INFO,								// 获取军团详细信息 
		MT_GET_GUILD_EVENT_LIST,							// 获取日志列表
		MT_GET_APPLY_FOR_JOIN_GUILD_LIST,					// 获取已申请加入的军团列表
		MT_GUILD_SET_STORAGESHOP_USE,						// 设置商店使用权限
		MT_GUILD_DELATE,									// 弹劾
		MT_GUILD_ADD_EXP,									// 增加军团经验值
		MT_GUILD_JIANZHU_ADD_EXP_RET,						// 增加建筑经验返回
		MT_GUILD_MEMBER_SOS,								// 军团成员求救
		MT_GET_GUILD_INVITE_LIST,							// 获取军团邀请列表
		MT_GET_GUILD_JIANZHU_INFO,							// 获取军团建筑信息
		MT_GUILD_RESET_NAME,								// 军团改名
		MT_GUILD_APPLYFOR_SETUP,							// 军团申请设置
		MT_GUILD_BOSS_GM_ADD_EXP,							// GM增加军团神兽经验
		MT_SYNC_GUILD_TASK_INFO_TO_GLOBAL,					// 同步仙盟任务信息 gameworld->global
		MT_GUILD_SET_AUTO_KICKOUT_SETUP,					// 军团设置是否自动踢人
		MT_GUILD_YUNBIAO_START,								// 运镖活动开始啦 global->start
		MT_GUILD_YUNBIAO_STATUS,							// 帮派运镖状态

		///////////////////////////////////////////   世界BOSS   /////////////////////////////////////////
		MT_WORLDBOSS_REPORT,								// gameworld上报到global

		///////////////////////////////////////////   活动   /////////////////////////////////////////
		MT_SYNC_GUILD_RANK_INFO,							// 仙盟排行信息同步
		MT_SYNC_USER_GIVE_FLOWER_NUM,						// gameworld-->global 同步玩家送花数量
		MT_SYNC_USER_CHEST_SHOP_BUY_COUNT,					// gameworld-->global 同步玩家寻宝次数
		MT_SYNC_RAND_ACTIVITY_ALLLIMITBUY_BUY_SUCC,			// gameworld-->global 随机活动全服限购购买成功
		MT_GM_ACTIVITY_FIND_REQ,							// gameworld-->global GM活动找回请求
		MT_COMBINE_SERVER_ACTIVITY_SYNC_STATE,				// global通知gameworld--合服活动子活动状态
		MT_COMBINE_SERVER_ACTIVITY_SYNC_VALUE,				// gameworld上报到global--合服活动排名信息

		///////////////////////////////////////////   结婚   /////////////////////////////////////////
		MT_MARRY_RESERVE_ACK,								// 预约结婚返回
		MT_GM_MARRY_RESERVE_REQ,							// Gm命令预约结婚
		MT_MARRY_INFO,										// 同步当前婚礼信息
		MT_MARRYOBJ_SYNC_STATUS_TO_GLOBAL,					// 同步结婚对象状态到global
		MT_MARRY_HUNYAN_INVITE,								// 婚宴发送请柬
		MT_DIVORCE_REQ,										// 离婚请求
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int	msg_type;
	};

	static const int MAX_REGISTER_SCENE_LIST_NUM = 1024;

	struct GGlobalUserMsg
	{
		GGlobalUserMsg() : header(MT_USER_MSG) {}

		MessageHeader header;
		int db_index;
		int role_id;
		// 后面紧跟着ggprotocal协议中的内容
	};

	struct GGlobalMsgToUser
	{
		GGlobalMsgToUser() : header(MT_MSG_TO_USER) {}

		MessageHeader header;
		int db_index;
		int role_id;
		// 后面紧跟着Protocal用户协议内容
	};


	struct GGlobalMsgToCrossServer
	{
		GGlobalMsgToCrossServer() : header(MT_MSG_TO_CROSS_SERVER) {}
		MessageHeader header;

		int cross_activity_type;
		// 后面紧跟着crossgameprotocal协议内容
	};

	struct GGlobalCrossGameMsg
	{
		GGlobalCrossGameMsg() : header(MT_CROSS_GAME_MSG) {}
		MessageHeader header;

		// 后面紧跟着crossgameprotocal协议内容
	};

	//////////////////////////////////////////////    拍卖    //////////////////////////////////////////////////
	struct AddPublicSaleItem
	{
		AddPublicSaleItem() : header(MT_ADD_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		GameName role_name;
		short is_to_world;
		short camp;
		SaleItemParam item_param;
	};

	struct RemovePublicSaleItem
	{
		RemovePublicSaleItem() : header(MT_REMOVE_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		int	sale_index;
	};

	struct BuyPublicSaleItem
	{
		BuyPublicSaleItem() : header(MT_BUY_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		GameName role_name;

		int	item_id;
		int	item_num;

		int	sale_value;
		int	price;

		short sale_item_type;
		short price_type;

		int	seller_db_index;
		int	seller_role_id;
		int	sale_index;
	};

	struct PublicSaleGetItemList
	{
		PublicSaleGetItemList() : header(MT_PUBLICSALE_GET_ITEM_LIST) {}
		MessageHeader header;

		int db_index;
		int	role_id;
	};

	struct PublicSaleSendItemInfoToWorld
	{
		PublicSaleSendItemInfoToWorld() : header(MT_PUBLICSALE_SEND_ITEM_INFO_TO_WORLD) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		short camp;
		short sale_index;
		GameName role_name;
	};

	//////////////////////////////////////////////    其他    //////////////////////////////////////////////////

	struct GameworldSendMailToUser
	{
		GameworldSendMailToUser() : header(MT_GAMEWORLD_SEND_MAIL_TO_USER) {}

		MessageHeader		header;

		int reason;		
		MailContentParam contentparam;

		struct UserIDInfo
		{
			int db_index;
			int role_id; 
		};

		int user_num;
		UserIDInfo user_list[MAX_GAMEWORLD_SEND_MAIL_USER];
	};

	struct GSystemCmdReload
	{
		GSystemCmdReload() : header(MT_SYSTEMCMD_RELOAD) {}
		MessageHeader		header;

		int					config_type;
		int					param1;
		int					param2;
	};

	struct SystemCmdToWorldCount
	{
		SystemCmdToWorldCount() : header(MT_SYSTEMCMD_TO_WORLD_COUNT) {}
		MessageHeader header;

		int count_type;
		int	count;
	};

	struct SystemCmdToWorldFbCount
	{
		SystemCmdToWorldFbCount() : header(MT_SYSTEMCMD_TO_WORLD_FB_COUNT) {}
		MessageHeader header;

		int day_count_id;
		int	count;
	};

	struct SyncRoleJoinCamp
	{
		SyncRoleJoinCamp() : header(MT_SYNC_ROLE_JOIN_CAMP) {}
		MessageHeader header;

		int camp_type;
	};

	struct SyncRoleSetProf
	{
		SyncRoleSetProf() : header(MT_SYNC_ROLE_SET_PROF) {}
		MessageHeader header;

		int prof_type;
	};

	struct GmWeekChange
	{
		GmWeekChange() : header(MT_GM_WEEK_CHANGE) {}
		MessageHeader header;
	};

	struct ActivityForceToNextState
	{
		ActivityForceToNextState() : header(MT_ACTIVITY_FORCE_TO_NEXT_STATE) {}
		MessageHeader header;

		int activity_type;
	};

	struct EvaluateRole
	{
		EvaluateRole() : header(MT_EVALUATE_ROLE) {}
		MessageHeader header;

		int				db_index;
		int				role_id;

		int				target_uid;
	};

	struct GMEvaluateRole
	{
		GMEvaluateRole() : header(MT_GM_EVALUATE_ROLE) {}
		MessageHeader header;

		int				db_index;
		int				role_id;

		int				target_uid;
		short			rank_type;
		short			is_admire;
		int				times;
	};

	struct SyncBossFlushTime
	{
		SyncBossFlushTime() : header(MT_SYNC_BOSS_FLUSH_TIME) {}
		MessageHeader header;

		int				monster_id;
		unsigned int	flush_time;
		int				is_together_broadcast;
	};

	struct SyncTeamRankInfo
	{
		SyncTeamRankInfo() : header(MT_SYNC_TEAM_RANK_INFO) {}

		MessageHeader		header;

		TeamRankMemberInfo member_list[MAX_TEAM_MEMBER_NUM];
		int rank_type;
		long long rank_value;
		int flexible_int;
		long long flexible_ll;
	};

	struct AddChestShopNewsItem
	{
		AddChestShopNewsItem() : header(MT_ADD_CHEST_SHOP_NEWS_ITEM) {}
		MessageHeader		header;

		int					chest_shop_type;
		ChestShopNewsItem	news_item;
	};

	struct AddMysteriousShopNewsItem
	{
		AddMysteriousShopNewsItem() : header(MT_ADD_MYSTERIOUS_SHOP_NEWS_ITEM) {}
		MessageHeader		header;

		MysteriousShopNewsItem news_item;
	};

	struct AddRandActivityLuckyRollNewsItem
	{
		AddRandActivityLuckyRollNewsItem() : header(MT_ADD_RANDACTIVITY_LUCKY_ROLL_NEWS_ITEM) {}
		MessageHeader		header;

		RandActivityLuckyRollNewsItem news_item;
	};

	struct AddCombineQingdianRollNewsItem
	{
		AddCombineQingdianRollNewsItem() : header(MT_ADD_COMBINE_QINGDIAN_ROLL_NEWS_ITEM) {}
		MessageHeader		header;
	};

	struct MingrentangAchieve
	{
		MingrentangAchieve() : header(MT_MINGRENTANG_EVENT) {}
		MessageHeader header;

		int mingrentang_id;
		UserID user_id;
	};

	struct AddFBXianhuntaPassNewsItem
	{
		AddFBXianhuntaPassNewsItem() : header(MT_ADD_FB_XHT_PASS_NEWS_ITEM) {}
		MessageHeader		header;

		short fb_type;									// 副本类型
		short pass_level;								// 关卡
		XianhuntaFBPassNewsItem news_item;
	};

	struct SyncWorldLevelInfo
	{
		SyncWorldLevelInfo() : header(MT_SYNC_WORLD_LEVEL_INFO) {}
		MessageHeader		header;

		int					world_level;				// 世界等级
		int					top_user_level;				// 最高角色等级
		int					capability_rank_mincap;		// 战力榜最低战力
	};

	struct SyncLastBattleFieldRankItem
	{
		UserID user_id;
		char vip;
		char sex;
		char prof;
		char camp;
		int level;
		GameName user_name;
		GameName flexible_name;
		long long rank_value;
		long long flexible_ll;
		int flexible_int;
		long long reserved;
	};

	struct SyncLastBattleFieldRankInfo
	{
		SyncLastBattleFieldRankInfo() : header(MT_SYNC_LAST_BATTLEFILED_RANK_INFO) {}

		MessageHeader		header;

		int rank_type;
		int rank_count;
		SyncLastBattleFieldRankItem rank_list[PERSON_RANK_MAX_SIZE];
	};

	//////////////////////////////////////////////    P2P交易    //////////////////////////////////////////////////
	struct TradeItemList
	{
		TradeItemList() : header(MT_TRADE_ITEM_LIST) {}
		MessageHeader		header;

		int					uid;
		int					empty_grid_num;

		int					item_count;
		ItemDataWrapper		item_list[P2P_TRADE_MAX_ITEM_COUNT];

		int					other_uid;			// 冗余检查
	};

	struct TradeAgree
	{
		TradeAgree() : header(MT_TRADE_AGREE) {}
		MessageHeader		header;

		int					req_uid;
		int					recv_uid;
		GameName			req_name;
		GameName			recv_name;
	};

	struct TradeAffirm
	{
		TradeAffirm() : header(MT_TRADE_AFFIRM) {}
		MessageHeader		header;

		int					from_uid;
		GameName			from_name;
		int					other_uid;			// 冗余检查
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeConsumeRet
	{
		TradeConsumeRet() : header(MT_TRADE_CONSUME_RET) {}
		MessageHeader		header;

		int					ret;				// 0为成功 其他失败
		int					from_uid;
		GameName			from_name;
		int					other_uid;			// 冗余检查
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeOver
	{
		TradeOver() : header(MT_TRADE_OVER) {}
		MessageHeader		header;

		int					uid;	
		GameName			user_name;
		int					other_uid;			// 冗余检查
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeSuccRet
	{
		TradeSuccRet() : header(MT_TRADE_SUCC_RET) {}
		MessageHeader		header;

		int					uid;
		GameName			user_name;
		int					other_uid;
		GameName			other_user_name;
		long long			trade_id;
	};

	//////////////////////////////////////////////    军团    //////////////////////////////////////////////////
	struct CreateGuild
	{
		CreateGuild() : header(MT_CREATE_GUILD) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GameName			role_name;
		PlatName			plat_name;
		GuildName			guild_name;
		GuildNotice			guild_notice;

		int					level;
		char				sex;
		char				prof;
		char				camp;
		int					vip_level;
		int					capability;

		int					create_guild_type;

		ItemDataWrapper		item_wrapper;
	};

	struct DismissGuild
	{
		DismissGuild() : header(MT_DISMISS_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
	};

	struct ApplyForJoinGuild
	{
		ApplyForJoinGuild() : header(MT_APPLY_FOR_JOIN_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GameName			role_name;
		PlatName			plat_name;

		int					level;
		char				sex;
		char				prof;
		char				camp;
		char				is_auto_join;
		int					capability;
		int					vip_level;
	};

	struct ApplyForJoinGuildAck
	{
		ApplyForJoinGuildAck() : header(MT_APPLY_FOR_JOIN_GUILD_ACK) {}
		MessageHeader		header;

		struct ApplyForAck
		{
			int				apply_for_db_index;
			int				apply_for_role_id;
		};

		GuildID				guild_id;
		int					result;
		int					db_index;
		int					role_id;
		int					count;
		ApplyForAck			apply_for[MAX_APPLY_FOR_COUNT];
	};

	struct QuitGuild
	{
		QuitGuild() : header(MT_QUIT_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct InviteGuild
	{
		InviteGuild() : header(MT_INVITE_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					beinvite_db_index;
		int					beinvite_role_id;
	};

	struct InviteGuildAck
	{
		InviteGuildAck() : header(MT_INVITE_GUILD_ACK) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GameName			role_name;
		PlatName			plat_name;

		int					level;
		char				prof;
		char				camp;
		char				sex;
		char				reserve;
		int					vip_level;
		int					capability;

		int					invite_db_index;
		int					invite_role_id;

		int					result;
	};

	struct KickoutGuild
	{
		KickoutGuild() : header(MT_KICKOUT_GUILD) {}
		MessageHeader		header;

		struct KickoutGuildRoleInfo
		{
			int					bekicker_db_index;
			int					bekicker_role_id;
		};

		GuildID				guild_id;

		int					kicker_db_index;
		int					kicker_role_id;

		int					 bekicker_count;
		KickoutGuildRoleInfo bekicker_role_info_list[MAX_MEMBER_COUNT];
	};

	struct AppointGuild
	{	
		AppointGuild() : header(MT_APPOINT_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					beappoint_db_index;
		int					beappoint_role_id;

		int					post;
	};

	struct LeavePostGuild
	{	
		LeavePostGuild() : header(MT_LEAVE_POST) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;
	};

	struct GuildMailAll
	{
		GuildMailAll() : header(MT_GUILD_MAIL_ALL) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GuildID				guild_id;

		MailSubject			subject;

		int					contenttxt_len;
		MailContentTxt		contenttxt;
	};

	struct GuildChangeNotice
	{
		GuildChangeNotice() : header(MT_GUILD_CHANGE_NOTICE) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GuildID				guild_id;
		GuildNotice         notice;
	};

	struct AllGuildBaseInfo
	{
		AllGuildBaseInfo() : header(MT_ALL_GUILD_BASE_INFO) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;
	};

	struct GetApplyForList
	{
		GetApplyForList() : header(MT_GET_APPLY_FOR_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetApplyForJoinGuildList
	{
		GetApplyForJoinGuildList() : header(MT_GET_APPLY_FOR_JOIN_GUILD_LIST) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;
	};

	struct GetMemberList
	{
		GetMemberList() : header(MT_GET_MEMBER_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildBaselInfo
	{
		GetGuildBaselInfo() : header(MT_GET_GUILD_BASE_INFO) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildEventList
	{
		GetGuildEventList() : header(MT_GET_GUILD_EVENT_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildInviteList
	{
		GetGuildInviteList() : header(MT_GET_GUILD_INVITE_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildJianzhuInfo
	{
		GetGuildJianzhuInfo() : header(MT_GET_GUILD_JIANZHU_INFO) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GuildSetStorageShopUse
	{
		GuildSetStorageShopUse() : header(MT_GUILD_SET_STORAGESHOP_USE) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					storage_use_limit;
		int					shop_use_limit;
	};

	struct GuildDelate
	{
		GuildDelate() : header(MT_GUILD_DELATE) {}
		MessageHeader header;

		GuildID				guild_id;
		int					knapsack_index;
	};

	struct GuildSetAutoKickoutSetup
	{
		GuildSetAutoKickoutSetup() : header(MT_GUILD_SET_AUTO_KICKOUT_SETUP) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					auto_kickout_setup;
	};

	struct GuildAddExp
	{
		GuildAddExp() : header(MT_GUILD_ADD_EXP) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					use_type;
		int					value;

		int					add_exp;
	};

	struct GuildJianZhuAddExpRet
	{
		GuildJianZhuAddExpRet() : header(MT_GUILD_JIANZHU_ADD_EXP_RET) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					jianzhu_type;
		int					use_gold;
		int					value;
		int					add_gongxian;
		int					add_caifu;
		int					add_exp;
	};

	enum SYNC_GUILD_INFO_OPERATE_TYPE
	{
		SYNC_GUILD_INFO_OPERATE_TYPE_INIT = 0,	// 初始化
		SYNC_GUILD_INFO_OPERATE_TYPE_CREATE,	// 建立军团
		SYNC_GUILD_INFO_OPERATE_TYPE_REMOVE,	// 解散军团
		SYNC_GUILD_INFO_OPERATE_TYPE_UPDATE,	// 更新军团
	};

	struct GmAddGuildBossExp
	{
		GmAddGuildBossExp() : header(MT_GUILD_BOSS_GM_ADD_EXP) {}
		MessageHeader header;

		int guild_id;
		int uid;
		int add_exp;
	};

	struct SyncGuildTaskInfoToGlobal
	{
		SyncGuildTaskInfoToGlobal() : header(MT_SYNC_GUILD_TASK_INFO_TO_GLOBAL) {}
		MessageHeader header;

		int role_id;
		int accepted_task_count;
		int add_score;
		int refresh_color;
	};

	struct ForbidSublock
	{
		ForbidSublock() : header(MT_FORBID_SUBLOCK) {}
		MessageHeader header;
	};

	struct ForceSetMicroPcFlag
	{
		ForceSetMicroPcFlag() : header(MT_FORCE_SET_MICRO_PC_FLAG) {}
		MessageHeader header;

		int is_micro_pc_open;
	};

	struct ForceSetAutoForbid
	{
		ForceSetAutoForbid() : header(MT_FORCE_SET_AUTO_FORBID) {}
		MessageHeader header;

		int is_auto_forbid;
	};

	struct GGlobalRoleResetName
	{
		GGlobalRoleResetName():header(MT_ROLE_RESET_NAME){}
		MessageHeader	header;

		UserID			user_id;
		GameName		name;
	};

	struct GuildMemberSos
	{
		GuildMemberSos() : header(MT_GUILD_MEMBER_SOS) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					sos_type;
		int					scene_id;
		short				pos_x;
		short				pos_y;
		int					enemy_uid;
	};

	struct GuildResetName
	{
		GuildResetName() : header(MT_GUILD_RESET_NAME) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GuildName			new_name;
	};

	struct GuildApplyforSetup
	{
		GuildApplyforSetup() : header(MT_GUILD_APPLYFOR_SETUP) {}
		MessageHeader header;

		GuildID				guild_id;
		int					applyfor_setup;
		int					need_capability;
		int					need_level;
	};

	struct KillOtherCamp
	{
		KillOtherCamp() : header(MT_KILL_OTHER_CAMP) {}
		MessageHeader header;

		UserID killer_user_id;
		UserID bekiller_user_id;
		int delta_level;
	};

	struct GuildYunBiaoStart
	{
		GuildYunBiaoStart() : header(MT_GUILD_YUNBIAO_START) {}
		MessageHeader header;

		GuildID guild_id;
		int guild_level;
	};

	enum YUNBIAO_STATUS
	{
		YUNBIAO_STATUS_INVALID = 0,
		YUNBIAO_STATUS_START,						// 开始运镖
		YUNBIAO_STATUS_COMPLETE,					// 成功结束运镖
		YUNBIAO_STATUS_FAIL,						// 运镖失败
		YUNBIAO_STATUS_UPDATE,						// 更新镖车信息
		YUNBIAO_STATUS_BE_HURT,						// 镖车被攻击
		YUNBIAO_STATUS_ACTIVITY_END_FAIL,			// 活动结束运镖失败
		YUNBIAO_STATUS_MAX,
	};

	struct GuildYunBiaoStatus
	{
		GuildYunBiaoStatus() : header(MT_GUILD_YUNBIAO_STATUS) {}
		MessageHeader header;

		int status;
		GuildID guild_id;
		int guild_lv;

		GuildID rob_guild_id;
		int biaoche_pos_x;
		int biaoche_pos_y;
	};

	//////////////////////////////////////////////    世界BOSS    //////////////////////////////////////////////////
	struct ReportWorldBossToGlobal
	{
		ReportWorldBossToGlobal() : header(MT_WORLDBOSS_REPORT) {}
		MessageHeader header;

		enum ENEVENTTYPE
		{
			ENEVENTTYPE_BORN = 1,						// boss出生
			ENEVENTTYPE_DIED,							// boss死亡
		};

		int event_type;
		int boss_id;

		int killer_uid;
		GameName killer_name;
	};

	//////////////////////////////////////////////    活动    //////////////////////////////////////////////////
	struct SyncGuildRankInfo
	{
	public:
		SyncGuildRankInfo() : header(MT_SYNC_GUILD_RANK_INFO) {}

		MessageHeader header;

		static const int MAX_GUILD_RANK_NUM = 100;

		struct GuildRank
		{
			GuildID guild_id;
			long long rank_value;
		};

		int guild_rank_type;
		int guild_rank_count;
		GuildRank guild_rank_list[MAX_GUILD_RANK_NUM];
	};

	struct SyncUserGiveFlowerNum
	{
	public:
		SyncUserGiveFlowerNum() : header(MT_SYNC_USER_GIVE_FLOWER_NUM) {}
		MessageHeader header;

		int give_flower_num;
	};

	struct SyncUserChestShopBuyCount
	{
	public:
		SyncUserChestShopBuyCount() : header(MT_SYNC_USER_CHEST_SHOP_BUY_COUNT) {}
		MessageHeader header;

		int chest_shop_buy_count;
	};

	struct SyncRandActivityAllLimitBuyBuySucc
	{
	public:
		SyncRandActivityAllLimitBuyBuySucc() : header(MT_SYNC_RAND_ACTIVITY_ALLLIMITBUY_BUY_SUCC) {}
		MessageHeader header;

		short activity_type;
		short reserve;
	};

	struct MarryReserveAck
	{
		MarryReserveAck() : header(MT_MARRY_RESERVE_ACK) {}
		MessageHeader header;

		int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];
		GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];
		char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];
		short reserve_sh;
		short marry_level;
		short marry_reserve_index;
	};

	struct GmMarryReserveReq
	{
		GmMarryReserveReq() : header(MT_GM_MARRY_RESERVE_REQ) {}
		MessageHeader header;

		int uid;
		short reserve_index;
		short marry_level;
		short is_clear;
		short is_force_start;
		int is_force_next_status;
	};

	struct MarryInfo
	{
		MarryInfo() : header(MT_MARRY_INFO) {}
		MessageHeader header;

		SyncMarryInfo marry_info;
	};

	struct MarryobjSyncPosToGlobal
	{
		MarryobjSyncPosToGlobal() : header(MT_MARRYOBJ_SYNC_STATUS_TO_GLOBAL) {}
		MessageHeader		header;	

		short				pos_x;
		short				pos_y;
	};

	struct MarryHunyanInvite
	{
		MarryHunyanInvite() : header(MT_MARRY_HUNYAN_INVITE) {}
		MessageHeader		header;

		int					invite_uid;
		int					be_invite_uid;
		int					is_free_invite;
	};

	struct DivorceReq
	{
		DivorceReq() : header(MT_DIVORCE_REQ) {}
		MessageHeader		header;

		int					other_uid;
		GameName			other_name;
		int					other_sex;

		unsigned int		divorce_time;
		int					clear_intimacy;
	};

	struct DatingInviteReq
	{
		DatingInviteReq() : header(MT_DATING_INVITE_REQ) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_specific_invite;
	};

	struct DatingInviteCheckTargetRet
	{
		DatingInviteCheckTargetRet() : header(MT_DATING_INVITE_CHECK_TARGET_RET) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_specific_invite;
		int					check_result;
	};

	struct DatingInviteAck
	{
		DatingInviteAck() : header(MT_DATING_INVITE_ACK) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_agree;
	};

	struct DatingGivePresentAck
	{
		DatingGivePresentAck() : header(MT_DATING_GIVE_PRESENT_ACK) {}
		MessageHeader		header;

		int					req_uid;
		int					present_type;
	};

	struct GMDatingForceInvite
	{
		GMDatingForceInvite() : header(MT_DATING_GM_FORCE_INVITE) {} 
		MessageHeader		header;

		int					self_uid;
		int					target_uid;
	};

	struct DatingDayconutCheckAck
	{
		DatingDayconutCheckAck() : header(MT_DATING_DAYCOUNT_CHECK_ACK) {}
		MessageHeader		header;

		int					uid;
		int					can_consume_daycount;
	};

	struct CommonActivityGetCapabilityRankReq
	{
		CommonActivityGetCapabilityRankReq() : header(MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_REQ) {}
		MessageHeader		header;

		int req_count;
	};

	struct CommonActivityCapabilityRankItem
	{
		CommonActivityCapabilityRankItem() : role_id(0), camp(0), reserved(0), reserved1(0), capability(0) 
		{
			memset(name, 0, sizeof(name));
		}

		int role_id;
		GameName name;
		char camp;
		char reserved;
		short reserved1;
		int capability;
	};

	struct CommonActivityGetCapabilityRankAck
	{
		CommonActivityGetCapabilityRankAck() : header(MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_ACK) {}
		MessageHeader		header;

		int count;
		CommonActivityCapabilityRankItem item_list[PERSON_RANK_MAX_SIZE];
	};

	struct GmActivityFindReq
	{
		GmActivityFindReq() : header(MT_GM_ACTIVITY_FIND_REQ) {}
		MessageHeader header;

		short is_find_reward;						// 0：请求找回信息 1：请求找回活动奖励
		short activity_find_type;
		int self_uid;
	};

	struct CombineServerActivitySyncState
	{
	public:
		CombineServerActivitySyncState() : header(MT_COMBINE_SERVER_ACTIVITY_SYNC_STATE) {}
		MessageHeader header;

		char sub_activity_state_list[CSA_SUB_TYPE_MAX];
	};

	struct CombineServerActivitySyncValue
	{
	public:
		CombineServerActivitySyncValue() : header(MT_COMBINE_SERVER_ACTIVITY_SYNC_VALUE) {}
		MessageHeader header;

		int rank_type;
		int role_id;
		long long value;
	};
}


struct SyncPersonRankInfoItem
{
	int 			rank_type;
	long long		rank_value;
	int				record_index;

	GameName		flexible_name;
	int				flexible_int;
	long long		flexible_ll;
};

static const int SYNC_PERSON_RANK_INFO_SIZE = 1024;

struct SyncPersonRankInfo
{
	SyncPersonRankInfo() : header(gglobalprotocal::MT_SYNC_PERSON_RANK_INFO) {}

	gglobalprotocal::MessageHeader		header;

	int					db_index;
	int					role_id;
	long long           exp;
	int                 level;
	GameName			user_name;
	GuildName			guild_name;
	char				reserve_ch;
	char				sex;
	char				prof;
	char				camp;
	char				vip_level;
	char				reserved2;
	short				jingjie_level;

	void AddItem(int rank_type, long long rank_value, int record_index, GameName flexible_name, int flexible_int, long long flexible_ll)
	{
		if (item_count < 0 || item_count >= SYNC_PERSON_RANK_INFO_SIZE)
		{
			return;
		}

		SyncPersonRankInfoItem *item = &items[item_count];
		item->rank_type = rank_type;
		item->rank_value = rank_value;
		item->record_index = record_index;
		memcpy(item->flexible_name, flexible_name, sizeof(item->flexible_name));
		item->flexible_int = flexible_int;
		item->flexible_ll = flexible_ll;

		++ item_count;
	}

	void ResetItems()
	{
		item_count = 0;
		memset(items, 0, sizeof(items));
	}

	int item_count;
	SyncPersonRankInfoItem items[SYNC_PERSON_RANK_INFO_SIZE];
};

////////////////////////////////////////////////////////////////// 夫妻排行 //////////////////////////////////////////////////////////
struct SyncCoupleRankItem
{
	int rank_type;
	int male_rank_value;
	int female_rank_value;
	int flexible_int;
	long long flexible_ll;
};

struct SyncCoupleRankInfo
{
	int male_uid;
	int female_uid;
	GameName male_name;
	GameName female_name;
	long long male_avatar;
	long long female_avatar;
	char male_prof;
	char female_prof;
	short reserve_sh;

	int item_count;
	SyncCoupleRankItem rank_item_list[COUPLE_RANK_TYPE_MAX];
};

#pragma pack(pop)

#endif  // __GGLOBALPROTOCAL_H__

