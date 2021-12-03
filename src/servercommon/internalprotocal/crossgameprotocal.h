#ifndef __CROSS_GAME_PROTOCAL_H__
#define __CROSS_GAME_PROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/struct/roleallparam.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace crossgameprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_CROSS_GAME_BEGIN = 29000,
		MT_CROSS_GAME_SYNC_CROSS_USER,					// 同步已跨服角色名单到原服
		MT_GAME_CROSS_START_CROSS_REQ,					// 开始跨服请求
		MT_GAME_CROSS_REGISTER_GAME_SERVER,				// 注册gameserver
		MT_CROSS_GAME_SAVE_CROSS_ROLE_INFO,				// 保存跨服角色信息
		MT_GAME_CROSS_SAVE_CROSS_ROLE_RET,				// 保存跨服角色信息返回
		MT_CROSS_GAME_START_CROSS_ACK,					// 开始跨服回应
		MT_CROSS_GAME_USER_MSG,							// cross给用户发消息 Protocol
		MT_CROSS_GAME_ALL_USER_MSG,						// cross给所有用户发消息 Protocol
		MT_GAME_CROSS_USER_MSG,							// 用户发消息到cross Protocol
		MT_GAME_CROSS_USER_LOGIN,						// 通知cross角色已登录到隐藏服
		MT_GAME_CROSS_USER_LOGOUT,						// 通知cross角色退出隐藏服
		MT_GAME_CROSS_KICK_CROSS_USER,					// game请求cross踢掉跨服角色
		MT_GAME_CROSS_TRANSFER_TO_GAME,					// game通过cross转发给其它game

		MT_CROSS_GAME_SYNC_CROSS_ACTIVITY_STATUS,		// 同步跨服活动状态
		MT_CROSS_GAME_XIULUOTA_GOTO_SCENE,				// 通知进入修罗塔场景
		MT_GAME_CROSS_ACTIVITY_FORCE_NEXTSTATE,			// 强制活动进入下一状态
		MT_GAME_CROSS_SYNC_XIULUOTOWER_RESULT,			// 同步修罗塔结果
		MT_CROSS_GAME_SYNC_XIULUOTOWER_USER_RESULT,		// 同步修罗塔结果
		MT_GAME_CROSS_SYNC_XIULUOTOWER_INFO,			// 同步修罗塔信息
		MT_CROSS_GAME_CROSS_SYNC_XIULUOTOWER_INFO,		// 同步修罗塔信息
		MT_GAME_CROSS_SYNC_DROP_INFO,					// 同步修罗塔掉落信息
		MT_GAME_CROSS_SYNC_XIULUOTOWER_RANK_TITLE,		// 同步修罗塔排行发称号
		MT_CROSS_GAME_SYNC_XIULUOTOWER_RANK_TITLE,		// 同步修罗塔排行发称号

		MT_CROSS_GAME_KILL_USER,						// cross请求game踢人
		MT_CROSS_GAME_HELLO,							// cross到game的心跳包
		MT_GAME_CROSS_SYNC_CROSS_PERSON_RANK_INFO,		// 同步个人排行榜信息到跨服
		MT_GAME_CROSS_QUERY_ROLE_INFO_REQUEST,			// 查询跨服角色信息请求 use locate game->cross
		MT_CROSS_GAME_GET_ROLE_INFO_REQUEST,			// 查询跨服角色信息请求 cross->target locate gameworld
		MT_GAME_CROSS_USER_PROTOCAL_TRANSFER_TO_GAME,	// 用户协议内容从game到cross到game中转请求
		MT_CROSS_GAME_SYNC_CROSS_PERSON_RANK_TITLE_INFO, // 同步跨服排行榜称号信息
		MT_CROSS_GAME_UPDATE_CROSS_RECORD,				// 更新玩家跨服记录
		MT_CROSS_GAME_SYNC_ROLE_DATA,					// 同步保存玩家数据回原服
		MT_CROSS_GAME_CHANNEL_CHAT,						// 跨服玩家聊天同步回原服
		MT_CROSS_GAME_RANK_INFO_REQ,					// 跨服玩家查看原服排行榜
		MT_GAME_CROSS_RANK_INFO_ACK,					// 跨服玩家查看原服排行榜返回
		MT_GAME_CROSS_SEND_MAIL_TO_USER,				// 跨服发邮件回原服给玩家
		MT_CROSS_GAME_SEND_MAIL_TO_USER,				// 跨服发邮件回原服给玩家
		MT_GAME_CROSS_GM_ADD_WEEK,						// gm命令跨周
		MT_GAME_CROSS_GM_OPERA,							// gm操作
		MT_GAME_CROSS_SYNC_ROLE_NUM_TO_GAME,			// game同步本服各等级登录角色人数到game
		MT_CROSS_GAME_TIME_INFO_TO_HIDDEN,				// CROSS发送game时间信息给隐藏服

		MT_GAME_CROSS_WORLD_COMMON_DATA_CHANGE,			// 全局数据变化 (隐藏服->跨服)
		MT_CROSS_GAME_WORLD_COMMON_DATA_CHANGE,			// 全局数据变化 (跨服->原服)

		MT_GAME_CROSS_FORBID_TALK,						// 禁言(origin->cross)
		MT_CROSS_GAME_FORBID_TALK,						// 禁言（cross->hidden）
		/////////////////////////////////////////////  跨服1v1 /////////////////////////////////////////////
		MT_CROSS_GAME_1V1_GOTO_SCENE,					// 跨服1V1进入场景
		MT_CROSS_GAME_1V1_INFO,							// 同步跨服1V1相关信息
		MT_CROSS_GAME_1V1_SYNC_MATCH_INFO,				// 同步跨服1v1信息到隐藏游戏服

		MT_GAME_CROSS_1V1_MATCH_REQ,					// 游戏服跨服1v1请求匹配
		MT_GAME_CROSS_1V1_FIGHT_RESULT,					// 同步跨服1v1比赛结果到跨服			
		MT_GAME_CROSS_1V1_ROLE_RANK_BASE_INFO,			// 更新跨服1v1排行榜个人信息
		MT_GAME_CROSS_1V1_RANK_LIST,					// 同步跨服1v1排行榜
		MT_CROSS_GAME_CROSS_1V1_SCORE_RANK_REWARD,		// 跨服1v1积分排行结算
		MT_GAME_CROSS_1V1_MATCH_RESULT_REQ,				// 游戏服跨服1v1请求匹配状态

		/////////////////////////////////////////////  跨服3v3 /////////////////////////////////////////////
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ,				// 跨服3V3请求匹配
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_CANCEL,			// 取消跨服3V3匹配
		MT_CROSS_MULTIUSER_CHALLENGE_START_MATCH,				// 跨服3V3开始一场比赛
		MT_GAMECROSS_CROSS_MULTIUSER_CHALLENGE_MATCH_RESULT,	// 跨服3V3比赛结果
		MT_GAME_CROSS_MULTIUSER_CHALLENGE_RANK_LIST,			// 跨服3V3同步排行榜信息
		MT_CROSS_GAME_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD,	// 跨服3V3全服积分榜奖励

		MT_SYNC_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE,		// 跨服3V3同步匹配状态
		MT_CROSS_GAME_MULTIUSER_CHALLENGE_GOTO_SCENE,			// 通知进入跨服3V3比赛场景

		MT_CROSS_GAME_RANK_WEEK_REWARD_COMMAND,					// 跨服排行每周奖励
		MT_GAME_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE,		// 游戏服查询角色跨服3V3匹配状态

		/////////////////////////////////////////////  跨服团战 /////////////////////////////////////////////
		MT_CROSS_GAME_TUANZHAN_GOTO_SCENE,				// 进入场景
		MT_GAME_CROSS_TUANZHAN_SYNC_FIGHT_RESULT,		// 团战结果信息传到cross

		//////////////////////////////////////////// 跨服牧场 ///////////////////////////////////////////////
		MT_CROSS_GAME_CROSS_ACTIVITY_GOTO_SCENE,        // 跨服进入场景(统一进入跨服场景协议)
		MT_GAME_CROSS_PASTURE_INFO_SYNC,				// 跨服农场玩家信息同步回原服

		/////////////////////////////////////////////  跨服BOSS /////////////////////////////////////////////
		MT_GAME_CROSS_BOSS_SYNC_PLAYER_INFO,			// 跨服boss个人信息传到cross（传回原服）
		MT_GAME_GAME_BOSS_CREATE_BOSS_BROADCAST,		// 跨服boss通知原服有boss生成（隐藏服->原服）
		MT_GAME_GAME_BOSS_INFO_REQ,						// 跨服boss信息查询（原服->隐藏服）

		/////////////////////////////////////////////  跨服通用信息 /////////////////////////////////////////////
		MT_GAME_CROSS_COMMON_INFO_SYNC,					// 跨服元宝信息同步
		MT_GAME_CROSS_COMMON_OPEAR_REQ_CS,				// 跨服统一请求协议

		/////////////////////////////////////////////  跨服活动参与信息 /////////////////////////////////////////////
		MT_GAME_CROSS_COMMON_ATTEND_INFO_SYNC,					//  跨服活动参与信息传回原服

		/////////////////////////////////////////////  跨服组队 ////////////////////////////////////////////////
		MT_SYNC_CROSS_TEAM_FB_USER_INFO,				// 同步跨服组队用户信息（跨服->隐藏服）
		MT_CROSS_TEAM_FB_USER_OPTION_INFO,				// 跨服组队用户操作（原服->跨服）
		MT_CROSS_TEAM_FB_ROOM_OPTION_INFO,				// 跨服组队房间操作（原服->跨服）
		
		/////////////////////////////////////////////  跨服帮派战 ////////////////////////////////////////////////												/////////////////////////////////////////////  跨服仙盟战 /////////////////////////////////////////////
		MT_GAME_CROSS_GUILD_BATTLE_INFO_SYNC,			// 同步跨服帮派战信息。 隐藏服到跨服
		MT_CROSS_GAME_GUILD_BATTLE_INFO_SYNC,			// 同步跨服帮派战信息。 隐藏服到原服
		MT_GAME_CROSS_GUILD_BATTLE_RANK_LOG_SYNC,		// 同步跨服帮派战排名结果到跨服保存（隐藏服 -> 跨服）
		MT_GAME_CROSS_GET_GUILD_BATTLE_RANK_LOG_REQ,	// 请求获取跨服帮派站排名结果的数据（游戏服 -> 跨服）
		MT_CROSS_GAME_GET_GUILD_BATTLE_RANK_LOG_RESP,	// 请求获取跨服帮派站排名结果的数据（跨服 -> 游戏服）
		MT_CROSS_GAME_GET_GUILD_BATTLE_BOSS_INFO_REQ,	// 请求获取跨服帮派站boss信息（跨服 -> 游戏服）
		MT_CROSS_GAME_GET_GUILD_BATTLE_MONSTER_INFO_REQ,// 请求获取跨服帮派站精英怪信息（跨服 -> 游戏服）
		MT_CROSS_GAME_GUILD_BATTLE_SPECIAL_TIME_NOTICE,	// 三倍积分时间提醒(跨服->游戏服)
		MT_GAME_CROSS_GUILD_BATTLE_SYNC_DROP_INFO,		// 同步跨服帮派战掉落信息
		MT_CROSS_GAME_GUILD_BATTLE_USER_SCORE_RANK,		// 同步用户积分排名(跨服->游戏服)
		MT_CROSS_GAME_GUILD_BATTLE_REGISTER_SYNC,		// 连到跨服信息同步(跨服->隐藏服)
		MT_GAME_GAME_GUILD_BATTLE_REGISTER_SYNC,		// 连到跨服信息同步(隐藏服->原服)

		/////////////////////////////////////////////  跨服钓鱼 /////////////////////////////////////////////
		MT_GAME_CROSS_FISHING_INFO_SYNC,				// 同步跨服钓鱼信息（隐藏服 -> 跨服）
		MT_CROSS_GAME_FISHING_INFO_SYNC,				// 同步跨服钓鱼信息（跨服 -> 本服）

		/////////////////////////////////////////////  跨服世界等级  /////////////////////////////////////////////
		MT_CROSS_GAME_CROSS_WORLD_LEVEL_INFO,					// 跨服世界等级 跨服->原服

		/////////////////////////////////////////////  跨服猎鲲地带 ////////////////////////////////////////////////
		MT_GAME_CROSS_LIEKUN_FB_INFO_SYNC,				// 跨服猎鲲地带副本信息（原服->跨服)
		MT_GAME_CROSS_LIEKUN_FB_KILL_ZONE_BOSS_SYNC,	// 跨服猎鲲地带击杀区域boss信息（隐藏服->跨服）
		MT_CROSS_GAME_LIEKUN_FB_KILL_ZONE_BOSS_SYNC,	// 跨服猎鲲地带击杀区域boss信息（跨服->原服）
		MT_CROSS_GAME_LIEKUN_FB_TOP_REWARD_SYNC,		// 跨服猎鲲地带发放奖励信息（跨服->原服）
		MT_GAME_CROSS_LIEKUN_FB_MSG_INFO_SYNC,			// 跨服猎鲲地带传闻信息（隐藏服->跨服）
		MT_CROSS_GAME_LIEKUN_FB_MSG_INFO_SYNC,			// 跨服猎鲲地带传闻信息（跨服->原服）

		/////////////////////////////////////////////  跨服乱斗战场 /////////////////////////////////////////////
		MT_GAME_CROSS_MESS_BATTLE_INFO_SYNC,				// 同步跨服钓鱼信息（隐藏服 -> 跨服）
		MT_CROSS_GAME_MESS_BATTLE_INFO_SYNC,				// 同步跨服钓鱼信息（跨服 -> 本服）

		/////////////////////////////////////////////  跨服乱斗战场 /////////////////////////////////////////////
		MT_GAME_CROSS_NIGHT_FIGHT_INFO_SYNC,				// 同步跨服钓鱼信息（隐藏服 -> 跨服）
		MT_CROSS_GAME_NIGHT_FIGHT_INFO_SYNC,				// 同步跨服钓鱼信息（跨服 -> 本服）

		/////////////////////////////////////////////  以下暂时未用到  ///////////////////////////////////////////////////////////
		MT_GAMECROSS_CROSS_CHALLENGE_MATCH_RESULT,		// 跨服争霸一场比赛结果上传到cross
		MT_CROSSGAME_CROSS_CHALLENGE_START_MATCH,		// 跨服争霸开始一场比赛
		MT_GAMECROSS_GM_CMD,							// 游戏服发给cross的GM命令
		MT_GAMECROSS_SYNC_CHALLENGE_PARAM,				// 游戏服同步战斗数据到cross
		MT_CROSS_TEAMRACE_MATCHING_REQ,					// 跨服组队竞速请求匹配
		MT_CROSS_TEAMRACE_MATCHING_CANCEL,				// 取消跨服组队竞速匹配
		MT_CROSS_TEAMRACE_START_MATCH,					// 跨服组队竞速开始一场比赛
		MT_CROSS_TEAMRACE_MATCH_RESULT,					// 跨服组队竞速比赛结果

		/////////////////////////////////////////////	跨服排行榜	////////////////////////////////////////////
		MT_CROSS_GAME_SYNC_RANK_FIRST,							// 跨服个人榜（跨服 -> 本服）
		MT_CROSS_GAME_SYNC_COUPLE_RANK_FIRST,					// 跨服情侣榜（跨服 -> 本服）
		MT_GAME_CROSS_SYNC_COUPLE_RANK_DIVORCE,					// 跨服情侣榜离婚（本服 -> 跨服）
		MT_GAME_CROSS_SYNC_CROSS_COUPLE_RANK_INFO, 		        // 同步夫妻排行榜信息到跨服
		MT_CROSS_GAME_SYNC_CROSS_COUPLE_FORCE_SNAP,		        // 强制同步夫妻排行榜信息


		/////////////////////////////////////////////  跨服普通boss ////////////////////////////////////////////////	
		MT_CROSS_GAME_CROSS_COMMON_BOSS_INFO_CHANGE,	        // 跨服普通boss信息变更
		MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO_REQ,	        // 跨服普通boss信息同步请求
		MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO,		        // 跨服普通boss信息同步
		MT_CROSS_GAME_CROSS_COMMON_BOSS_DROP_INFO,              // 跨服普通boss掉落

		/////////////////////////////////////////////  其他  ///////////////////////////////////////////////////////////
		MT_USE_GOLD_IN_HIDDEN_GS,								// 在隐藏服使用了元宝，通知原服

		MT_CROSS_GAME_SINGLE_CHAT,								// 跨服玩家私聊同步回原服
		MT_CROSS_GAME_LOCAL_SPEAKER,							// 跨服玩家发本服喇叭回原服
		MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN,						// 隐藏服同步系统消息回原服(隐藏服到跨服使用了转发协议，这个定义只是从跨服回原服)
		MT_CROSS_GAME_GUILD_CHAT,								// 隐藏服中请求发送本服仙盟聊天

		MT_GAME_GAME_CROSS_GUILD_MEMBER_BE_KILL,				// 发送仙盟成员被击杀（隐藏服->原服）
		MT_CROSS_GAME_GET_GUILD_ENEMY_LIST,						// 跨服玩家获取仙盟仇人列表（隐藏服->原服）

		/////////////////////////////////////////// 跨服随机 ////////////////////////////////////////////////////////////////////
		MT_CROSS_GAME_SYNC_CROSS_RAND_ACTIVITY_DATA,			// 同步跨服随机活动数据（跨服->原服）
		MT_GAME_CROSS_SYNC_CROSS_RAND_ACTIVITY_DATA,			// 同步跨服随机活动数据（原服->跨服）

		/////////////////////////////////////////// 六界召集 ////////////////////////////////////////////////////////////////////
		MT_GAME_GAME_CROSS_GUILD_BATTLE_SOS,					// 同步召集信息（隐藏服->原服）
																				
		/////////////////////////////////////////// 击杀神域boss ////////////////////////////////////////////////////////////////////
		MT_GAME_GAME_KILL_CROSS_BOSS,							// 同步击杀神域boss（原服->跨服）

		/////////////////////////////////////////////  在隐藏服捡取仙盟珍稀物品  ///////////////////////////////////////////////////////////
		MT_PICK_GUILD_RARE_ITEM_IN_HIDDEN_GS,					// 在隐藏服捡取仙盟珍稀物品，通知原服
		MT_BROADCAST_GUILD_RARE_INFO,							// 广播仙盟珍稀

		/////////////////////////////////////////////  跨服排行榜信息到原服  ///////////////////////////////////////////////////////////
		MT_SYNC_CROSS_RANK_INFO,								// 同步跨服榜前三到原服（用于添加仙盟频道传闻）

		/////////////////////////////////////////// 跨服边境之地 ////////////////////////////////////////////////////////////////////
		MT_HIDDEN_CROSS_BIANJINGZHIDI_BOSS_INFO,				// 同步跨服边境之地boss（隐藏服->到CROSS）

		/////////////////////////////////////////// 跨服竞技场 ////////////////////////////////////////////////////////////////////
		MT_SYNC_CROSS_CHALLENGEFIELD_REQ,						// 同步跨服竞技场数据（跨服-原服）
		MT_SYNC_CROSS_CHALLENGEFIELD_DATA_INFO,					// 同步跨服竞技场数据返回
		
		/////////////////////////////////////////// 跨服护送水晶 ////////////////////////////////////////////////////////////////////
		MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_REQ,				// 跨服护送水晶(原服->隐藏服)
 	};

	struct MessageHeader
	{
		MessageHeader(): msg_type(MT_INVALID) {}
		MessageHeader(int _msg_type): msg_type(_msg_type) {}
		int	msg_type;
	};

	struct CrossGameSyncCrossUser
	{
		CrossGameSyncCrossUser() : header(MT_CROSS_GAME_SYNC_CROSS_USER) {}
		MessageHeader	header;

		enum 
		{
			SYNC_CROSS_USER_INIT = 0,
			SYNC_CROSS_USER_ADD,
			SYNC_CROSS_USER_REMOVE,
		};

		static const int MAX_IN_CROSS_USER_NUM = 2048;

		struct CrossUserItem
		{
			int			uid;
			int			cross_activity_type;
		};

		int				sync_type;
		int				cross_type;  // crossserver的战区类型
		int				count;
		CrossUserItem	in_cross_user_list[MAX_IN_CROSS_USER_NUM];
	};

	struct GameCrossStartCrossReq
	{
		GameCrossStartCrossReq() : header(MT_GAME_CROSS_START_CROSS_REQ) {}
		MessageHeader	header;

		static const int MAX_ROLE_CROSS_PARAM_DATA_LEN = 4 * 1024 * 1024;

		int				is_force;
		int				cross_activity_type;
		int				plat_type;
		PlatName		pname;
		int				uid;
		int				role_cross_param_data_len;
		char			role_cross_param_data[MAX_ROLE_CROSS_PARAM_DATA_LEN];
	};

	struct GameCrossRegisterGameServer
	{
		GameCrossRegisterGameServer() : header(MT_GAME_CROSS_REGISTER_GAME_SERVER) {}
		MessageHeader		header;

		static const int MAX_SERVER_ID_NUM = 512;

		int					cross_protocal_ver;
		int					plat_type;
		unsigned int		server_start_time;
		unsigned int		server_combine_time;
		UInt32				role_all_param_size;
		UInt32				server_version;

		int					count;
		int					server_id_list[MAX_SERVER_ID_NUM];
	};

	struct CrossGameSaveCrossRoleInfo
	{
		CrossGameSaveCrossRoleInfo() : header(MT_CROSS_GAME_SAVE_CROSS_ROLE_INFO) {}
		MessageHeader		header;

		static const int MAX_ROLE_CROSS_PARAM_DATA_LEN = 4 * 1024 * 1024;

		int				cross_activity_type;
		int				original_plat_type;
		int				original_uid;
		GameName		original_username;
		PlatName		pname;
		int				role_cross_param_data_len;
		char			role_cross_param_data[MAX_ROLE_CROSS_PARAM_DATA_LEN];
	};

	struct GameCrossSaveCrossRoleRet
	{
		GameCrossSaveCrossRoleRet() : header(MT_GAME_CROSS_SAVE_CROSS_ROLE_RET) {}
		MessageHeader	header;

		int				result;

		int				cross_activity_type;
		int				original_plat_type;
		int				original_uid;
		GameName		original_username;

		PlatName		pname;
		int				uid;

		char			login_server_ip[64];
		int				login_server_port;
		unsigned int	login_time;
		LoginStr		login_str;
		char			anti_wallow;
		char			reserved1;
		short			server;
	};

	struct CrossGameStartCrossAck
	{
		CrossGameStartCrossAck() : header(MT_CROSS_GAME_START_CROSS_ACK) {}
		MessageHeader	header;

		int				result;
		int				original_uid;
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

	struct CrossGameUserMsg
	{
		CrossGameUserMsg() : header(MT_CROSS_GAME_USER_MSG) {}
		MessageHeader		header;

		int					uid;
		// 后面紧跟 Protocol 协议内容
	};

	struct CrossGameAllUserMsg
	{
		CrossGameAllUserMsg() : header(MT_CROSS_GAME_ALL_USER_MSG) {}
		MessageHeader		header;

		int					cross_activity_type;
		// 后面紧跟 Protocol 协议内容
	};

	struct GameCrossUserMsg
	{
		GameCrossUserMsg() : header(MT_GAME_CROSS_USER_MSG) {}
		MessageHeader		header;

		long long			uuid;
		int					merge_server_id;
		// 后面紧跟 Protocol 协议内容
	};

	struct GameCrossUserLogin
	{
		GameCrossUserLogin() : header(MT_GAME_CROSS_USER_LOGIN) {}
		MessageHeader	header;

		int				uid;

		int				original_plat_type;
		int				original_uid;
		GameName		original_user_name;
	};

	struct GameCrossUserLogout
	{
		GameCrossUserLogout() : header(MT_GAME_CROSS_USER_LOGOUT) {}
		MessageHeader	header;

		int				uid;
	};

	struct GameCrossKickCrossUser
	{
		GameCrossKickCrossUser() : header(MT_GAME_CROSS_KICK_CROSS_USER) {}
		MessageHeader	header;

		long long		bekick_original_uuid;
	};

	enum CROSS_TRANSFER_TYPE
	{
		CROSS_TRANSFER_TYPE_HIDDEN,			// 转到隐藏服
		CROSS_TRANSFER_TYPE_COMMON,			// 转到普通服
		CROSS_TRANSFER_TYPE_ALL,			// 转到所有服
		CROSS_TRANSFER_TYPE_OTHER,			// 转到其它服
		CROSS_TRANSFER_TYPE_BY_UUID,		// 转到指定服
		CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID,// 转到指定服--global_netid
	};
	struct GameCrossRransferToGame
	{
		GameCrossRransferToGame() : header(MT_GAME_CROSS_TRANSFER_TO_GAME) {}
		MessageHeader	header;

		int				length;
		int				transfer_type;
		long long		param;
	};
	
	struct CrossGameSyncActivityStatus
	{
		CrossGameSyncActivityStatus() : header(MT_CROSS_GAME_SYNC_CROSS_ACTIVITY_STATUS) {}
		MessageHeader	header;

		int				cross_activity_type;
		int				activity_status;
		unsigned int	status_begin_time;
		unsigned int	status_end_time;
		int				activity_special_param;
		unsigned int	cross_now_scecond;
	};

	//全局数据回传
	enum WORLD_COMMONDATA_CHANGE_TYPE
	{
		WORLD_COMMONDATA_CHANGE_TYPE_INVALID,

		WORLD_COMMONDATA_CHANGE_TYPE_1V1,					// 跨服1v1数据
		WORLD_COMMONDATA_CHANGE_TYPE_3V3,					// 跨服3v3数据

		WORLD_COMMONDATA_CHANGE_TYPE_CHENGZHU_CHANGE,		// 城主改变(原服发到隐藏服)
		WORLD_COMMONDATA_CHANGE_TYPE_COMBINE_KILL_BOSS,		// 合服BOSS抢夺
		WORLD_COMMONDATA_CHANGE_TYPE_GUILD_GONGZI,			// 仙盟工资
		WORLD_COMMONDATA_CHANGE_TYPE_SINGLE_CHAT_LISTEN,	// 私聊监听
		WORLD_COMMONDATA_CHANGE_TYPE_GUILD_KILL_BOSS,		// 跨服仙盟击杀榜
		WORLD_COMMONDATA_CHANGE_TYPE_CROSS_CHALLENGEFIELD,	// 跨服竞技场排行
	};

	enum WORLD_COMMONDATA_SUB_TYPE
	{
		WORLD_COMMONDATA_SUB_TYPE_INVALID,

		WORLD_COMMONDATA_SUB_TYPE_RANK,               
		WORLD_COMMONDATA_SUB_TYPE_SEANSON,				// 新开赛季
		WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE,		// 排名奖励最低分， 最后一名玩家对应的积分，用于原服玩家月结判断是否给段位奖励（榜上玩家不用给段位奖励）
		WORLD_COMMONDATA_SUB_TYPE_INIT,					// 连接跨服成功初始化
	};

	struct GameCrossWorldCommonDataChange     // 隐藏服->跨服
	{
	public:
		GameCrossWorldCommonDataChange() : header(MT_GAME_CROSS_WORLD_COMMON_DATA_CHANGE) {}

		MessageHeader	header;

		long long uuid;

		short change_type;
		short sub_type;
		int param_2;
		int param_3;
		int param_4;
	};

	struct CrossGameWorldCommonDataChange       // 跨服->原服(全局数据变更)
	{
	public:
		CrossGameWorldCommonDataChange() : header(MT_CROSS_GAME_WORLD_COMMON_DATA_CHANGE) {}

		MessageHeader	header;

		void Reset()
		{
			change_type = 0;
			sub_type = 0;
			param_2 = 0;
			param_3 = 0;
			param_4 = 0;
		}

		long long uuid;

		short change_type;
		short sub_type;
		int param_2;
		int param_3;
		int param_4;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	struct CrossGameXiuluotaGotoScene
	{
		CrossGameXiuluotaGotoScene() : header(MT_CROSS_GAME_XIULUOTA_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_uid;
	};

	struct GameCrossActivityForceNextState
	{
		GameCrossActivityForceNextState() : header(MT_GAME_CROSS_ACTIVITY_FORCE_NEXTSTATE) {}
		MessageHeader	header;

		int activity_type;
	};

	static const int XIULUO_TOWER_USER_RESULT_MAX = 1024;
	struct GameCrossSyncXiuluoTowerResult
	{
		GameCrossSyncXiuluoTowerResult() : header(MT_GAME_CROSS_SYNC_XIULUOTOWER_RESULT) {}
		MessageHeader	header;

		struct UserResult
		{
			long long uuid;
			char max_layer;
			char rank_pos;
			short kill_role_count;
			short kill_boss_count;
			short user_level;
			int reward_cross_honor;
		};

		int user_result_count;
		UserResult user_result_list[XIULUO_TOWER_USER_RESULT_MAX];
	};

	struct CrossGameSyncXiuluoTowerUserResult
	{
		CrossGameSyncXiuluoTowerUserResult() : header(MT_CROSS_GAME_SYNC_XIULUOTOWER_USER_RESULT) {}
		MessageHeader	header;

		long long		uuid;
		char			max_layer;
		char			rank_pos;
		short			kill_role_count;
		short			kill_boss_count;
		short			user_level;
		int				reward_cross_honor;
	};

	enum XIULUOTOWER_INFO_TYPE
	{
		XIULUOTOWER_INFO_TYPE_BUFF = 0,					// BUFF奖励邮件
		XIULUOTOWER_INFO_TYPE_FIRST,					// 首通邮件
	};

	struct GameCrossSyncXiuluoTowerInfo
	{
		GameCrossSyncXiuluoTowerInfo() : header(MT_GAME_CROSS_SYNC_XIULUOTOWER_INFO) {}
		MessageHeader	header;

		long long	uuid;
		int			type;
		int			param;
	};

	struct CrossGameCrossSyncXiuluoTowerInfo
	{
		CrossGameCrossSyncXiuluoTowerInfo() : header(MT_CROSS_GAME_CROSS_SYNC_XIULUOTOWER_INFO) {}
		MessageHeader	header;

		long long	uuid;
		int			type;
		int			param;
	};

	struct GameCrossSyncXiuluoTowerDropInfo
	{
		GameCrossSyncXiuluoTowerDropInfo() : header(MT_GAME_CROSS_SYNC_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		int log_type;
		GameName name;
		unsigned int timestamp;
		unsigned short item_id;
		short item_num;
	};

	struct GameCrossSyncXiuluoTowerRankTitle
	{
		GameCrossSyncXiuluoTowerRankTitle() :header(MT_GAME_CROSS_SYNC_XIULUOTOWER_RANK_TITLE) {}
		MessageHeader header;

		long long uuid;
		int rank;
	};

	struct CrossGameSyncXiuluoTowerRankTitle
	{
		CrossGameSyncXiuluoTowerRankTitle():header(MT_CROSS_GAME_SYNC_XIULUOTOWER_RANK_TITLE){}
		MessageHeader header;

		int uid;
		int rank;
	};

	struct GameCrossSyncGuildBattleDropInfo
	{
		GameCrossSyncGuildBattleDropInfo() : header(MT_GAME_CROSS_GUILD_BATTLE_SYNC_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		int log_type;
		GameName name;
		unsigned int timestamp;
		unsigned short item_id;
		short item_num;
	};

	struct CrossGameKickUser
	{
		CrossGameKickUser() : header(MT_CROSS_GAME_KILL_USER) {}
		MessageHeader	header;

		int				bekick_uid;
		long long		bekick_uuid;
	};

	static const int SYNC_CROSS_PERSON_RANK_INFO_SIZE = 1024;

	struct GameCrossSyncCrossPersonRankInfo
	{
		GameCrossSyncCrossPersonRankInfo() : header(MT_GAME_CROSS_SYNC_CROSS_PERSON_RANK_INFO) {}
		MessageHeader	header;

		int plat_type;
		int role_id;
		GameName name;
		int level;
		char prof;
		char sex;
		char camp;
		char vip_level;
		long long exp;
		// RoleAppearance appearance;

		struct SyncCrossPersonRankInfoItem
		{
			int rank_type;
			long long rank_value;
			int record_index;
			long long flexible_ll;
			int flexible_int;
			GameName flexible_name;
		};

		void ResetItems()
		{
			item_count = 0;
			memset(items, 0, sizeof(items));
		}

		int item_count;
		SyncCrossPersonRankInfoItem items[SYNC_CROSS_PERSON_RANK_INFO_SIZE];
	};

	struct GameCrossQueryRoleInfoRequest
	{
		enum QueryRoleInfoType
		{
			QUERY_ROLE_INFO_TYPE_ALL,
			QUERY_ROLE_INFO_TYPE_UUID_NAME,
		};
		GameCrossQueryRoleInfoRequest() : header(MT_GAME_CROSS_QUERY_ROLE_INFO_REQUEST) {}
		MessageHeader	header;

		int target_plat_type;
		int target_uid;

		int request_plat_type;
		int request_uid;

		int request_type;
		int param;
	};

	struct CrossGameGetRoleInfoRequest
	{
		CrossGameGetRoleInfoRequest() : header(MT_CROSS_GAME_GET_ROLE_INFO_REQUEST) {}
		MessageHeader	header;

		int target_uid;
		int request_plat_type;
		int request_uid;
		int request_type;
		int param;
	};

	struct GameCrossUserProtocalTransferToGame
	{
		GameCrossUserProtocalTransferToGame() : header(MT_GAME_CROSS_USER_PROTOCAL_TRANSFER_TO_GAME) {}
		MessageHeader		header;

		long long			uuid;
		int					protocal_len;

		// 后面紧跟 Protocol 协议内容
	};

	struct CrossGameHello
	{
		CrossGameHello() : header(MT_CROSS_GAME_HELLO) {}
		MessageHeader	header;
	};

	struct CrossGameSyncCrossPersonRankTitleInfo
	{
		CrossGameSyncCrossPersonRankTitleInfo() : header(MT_CROSS_GAME_SYNC_CROSS_PERSON_RANK_TITLE_INFO) {}
		MessageHeader header;

		int rank_type;
		int uuid_count;
		long long rank_title_user_uuid_list[CROSS_PERSON_RANK_MAX_SIZE];
	};

	struct CrossGameUpdateCrossRecord
	{
		CrossGameUpdateCrossRecord() : header(MT_CROSS_GAME_UPDATE_CROSS_RECORD) {}
		MessageHeader	header;

		enum
		{
			UPDATE_CROSS_RECORD_INVALID = 0,
			UPDATE_CROSS_RECORD_CROSS_1V1,
			UPDATE_CROSS_RECORD_CROSS_MULTIUSER_CHALLENGE,
			UPDATE_CROSS_RECORD_CROSS_TUANZHAN,
			UPDATE_CROSS_RECORD_CROSS_PASTURE,
			UPDATE_CROSS_RECORD_CROSS_BOSS,
			UPDATE_CROSS_RECORD_CROSS_COMMON,
			UNDATE_CROSS_RECORD_CROSS_ATTEND,
			UPDATE_CROSS_RECORD_CROSS_MIZANG_BOSS,
			UPDATE_CROSS_RECORD_CROSS_YOUMING_BOSS,

			UPDATE_CROSS_RECORD_MAX
		};

		static const int CROSS_RECORD_PARAM_MAX = 16;

		int				uid;
		int				update_cross_record_type;
		int				record_param_list[CROSS_RECORD_PARAM_MAX];
		RoleAppearance	appearance;
		
	};

	struct CrossGameSyncRoleData
	{
		CrossGameSyncRoleData() : header(MT_CROSS_GAME_SYNC_ROLE_DATA) {}
		MessageHeader header;

		int				is_logout;
		long long		uuid;
		
		static const int MAX_DATA_LEN = 1024 * 1024 * 4;
		int				data_len;
		char			role_all_data[MAX_DATA_LEN];
	};

	struct CrossGameChannelChat
	{
		CrossGameChannelChat() : header(MT_CROSS_GAME_CHANNEL_CHAT) {}
		MessageHeader header;

		long long		uuid;
		short			channel_type;
		short			role_camp;
		int				guild_id;
		int				protocal_len;

		// 后面紧跟 Protocol 协议内容
	};

	// 跨服中私聊同步回原服
	struct CrossGameSingleChat
	{
		CrossGameSingleChat() : header(MT_CROSS_GAME_SINGLE_CHAT) {}
		MessageHeader header;

		long long to_uuid;
	};

	struct CrossGameSpeaker
	{
		CrossGameSpeaker() : header(MT_CROSS_GAME_LOCAL_SPEAKER) {}
		MessageHeader header;
	};

	struct CrossGameRankInfoReq
	{
		CrossGameRankInfoReq() : header(MT_CROSS_GAME_RANK_INFO_REQ) {}
		MessageHeader header;

		long long		uuid;
		short			is_personal_rank;
		short			is_personal_rank_top_user;
		int				rank_type;
	};

	struct GameCrossRankInfoAck
	{
		GameCrossRankInfoAck() : header(MT_GAME_CROSS_RANK_INFO_ACK) {}
		MessageHeader header;

		long long		uuid;

		// 后面紧跟 Protocol 协议内容
	};

	////////////////////////////////////跨服邮件//////////////////////////////////////
	struct GameCrossSendMailToUser
	{
	public:
		GameCrossSendMailToUser() : header(MT_GAME_CROSS_SEND_MAIL_TO_USER) {}
		MessageHeader header;

		long long unique_user_id;
		MailContentParam contentparam;
	};

	struct CrossGameSendMailToUser
	{
	public:
		CrossGameSendMailToUser() : header(MT_CROSS_GAME_SEND_MAIL_TO_USER) {}
		MessageHeader header;

		int user_id;
		MailContentParam contentparam;
	};

	////////////////////////////////////gm//////////////////////////////////////
	struct GameCrossGmAddWeek
	{
	public:
		GameCrossGmAddWeek() : header(MT_GAME_CROSS_GM_ADD_WEEK) {}
		MessageHeader header;
	};

	struct GameCrossGmOpera
	{
	public:
		GameCrossGmOpera() : header(MT_GAME_CROSS_GM_OPERA) {}
		MessageHeader header;

		int opera_type;
		int param_1;
		int param_2;
	};

	///////////////////////////////////// 同步角色 /////////////////////////////////////
	struct GameCrossSyncRoleNumToGame
	{
	public:
		GameCrossSyncRoleNumToGame() : header(MT_GAME_CROSS_SYNC_ROLE_NUM_TO_GAME) {}
		MessageHeader header;

		short role_num_list[MAX_ROLE_LEVEL + 1];
		short reserve_sh;
		long long unique_serverId;
	};

	////////////////////////////////////// 同步时间 ////////////////////////////////////
	struct CrossGameTimeInfoToHidden
	{
		CrossGameTimeInfoToHidden() : header(MT_CROSS_GAME_TIME_INFO_TO_HIDDEN) {}
		MessageHeader		header;

		struct UniqueGameSerInfo
		{
			UniqueGameSerInfo() : server_start_time(0), server_combine_time(0), server_start_day_index(0) {}
			
			UniqueServerID gs_unique_id;
			unsigned int server_start_time;
			unsigned int server_combine_time;
			int server_start_day_index;
		};

		int					count;
		UniqueGameSerInfo	server_info_list[GameCrossRegisterGameServer::MAX_SERVER_ID_NUM];
	};

	/////////////////////////////////////////////  跨服1v1 /////////////////////////////////////////////
	struct CrossGame1V1GotoScene	// 29043 进入场景
	{
		CrossGame1V1GotoScene() : header(MT_CROSS_GAME_1V1_GOTO_SCENE) {}
		MessageHeader header;

		long long uuid;
		UserID cross_uid;
	};

	struct CrossGame1V1Info			// 29044 信息
	{
		CrossGame1V1Info() : header(MT_CROSS_GAME_1V1_INFO) {}
		MessageHeader header;

		int cross_uid;
		int cross_score_1v1;
		int cross_day_join_1v1_count;
		short reserve_sh;
		char cross_1v1_xiazhu_seq;
		char cross_1v1_fetch_xiazhu_reward_flag;
		short cross_1v1_xiazhu_gold;
	};

	struct GameCrossActivity1V1MatchReq	// 29046请求匹配
	{
		GameCrossActivity1V1MatchReq() : header(MT_GAME_CROSS_1V1_MATCH_REQ) {}
		MessageHeader	header;

		long long		uuid;
		char			prof;
		char			sex;
		char			camp;
		char			reserve_ch;
		short			level;
		short			jingjie;			// 境界等级
		int				score;
		int				day_join_count;		// 每日参加次数
		long long		capability;
		GameName		name;
		RoleAppearance	appearance;
		int				dur_win_count;		// 连胜次数
		short           total_join_times;	// 总参加次数
		short           total_win_times;	// 总胜利次数
		int             cross_1v1_gongxun;	// 功勋
	};

	struct GameCrossActivity1V1RoleRankBaseInfo		// 29048 更新排行榜个人信息
	{
		GameCrossActivity1V1RoleRankBaseInfo() : header(MT_GAME_CROSS_1V1_ROLE_RANK_BASE_INFO) {}
		MessageHeader	header;

		long long		uuid;
		int				level;
		long long		capability;
	};

	struct CrossGameSyncCross1v1MatchInfo		// 29045
	{
	public:
		CrossGameSyncCross1v1MatchInfo() : header(MT_CROSS_GAME_1V1_SYNC_MATCH_INFO) {}
		MessageHeader header;

		static const int MAX_FIGHT_UNIT_COUNT = 1024;

		unsigned int m_next_match_left_time;
		int unit_count;
		CrossActivity1v1FightUnit fight_unit_list[MAX_FIGHT_UNIT_COUNT];
	};

	struct GameCrossCross1V1FightResult			// 29047
	{
	public:
		GameCrossCross1V1FightResult() : header(MT_GAME_CROSS_1V1_FIGHT_RESULT) {}
		MessageHeader header;

		long long uuid1;
		long long uuid2;
		short user1_is_win;
		short user2_is_win;
	};

	static const int MAX_CROSS_1V1_RANK_COUNT = 1024;

	struct GameCrossCross1V1RankList
	{
		GameCrossCross1V1RankList() : header(MT_GAME_CROSS_1V1_RANK_LIST) {}
		MessageHeader	header;

		struct RankInfo
		{
			int plat_type;
			int role_id;
			GameName name;
			short level;
			char prof;
			char sex;
			int score;
			int dur_win_count;
			short win_percent;
			char jingjie;
			char reserve_ch;
			long long capability;
			RoleAppearance appearance;
		};

		int count;
		RankInfo rank_list[MAX_CROSS_1V1_RANK_COUNT];
	};

	struct CrossGameCross1V1ScoreRankReward
	{
		CrossGameCross1V1ScoreRankReward() : header(MT_CROSS_GAME_CROSS_1V1_SCORE_RANK_REWARD) {}
		MessageHeader	header;

		int				uid;
		int				rank_pos;
		int				score;
	};

	enum CROSS1V1_MATCH_RESULT_REQ_TYPE
	{
		CROSS1V1_MATCH_RESULT_REQ_TYPE_QUERY = 0,
		CROSS1V1_MATCH_RESULT_REQ_TYPE_CANCEL,
	};
	struct CrossGameCross1V1MatchResult		
	{
		CrossGameCross1V1MatchResult() : header(MT_GAME_CROSS_1V1_MATCH_RESULT_REQ) {}
		MessageHeader	header;

		long long		uuid;
		int req_type;
	};

	//////////////////////////////////////// 跨服3v3 //////////////////////////////////////////////////

	struct GameCrossCrossMuliuserChallengeMatchingReq
	{
		GameCrossCrossMuliuserChallengeMatchingReq() : header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ) {}
		MessageHeader	header;

		struct MatchingUserInfo
		{
			long long	uuid;
			GameName	user_name;
			char		camp;
			char		sex;
			char		prof;
			char		reserved;
			short		level;
			int			capability;
			int			challenge_score;
			int			challenge_exp;
			int			win_percent;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			dur_win_count;
			int			mvp_count;
		};

		long long		req_uuid;
		int				count;
		MatchingUserInfo	user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	struct GameCrossCrossMuliuserChallengeMatchingCancel
	{
		GameCrossCrossMuliuserChallengeMatchingCancel() : header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_CANCEL) {}
		MessageHeader	header;

		long long		uuid;
	};

	struct CrossGameCrossMultiuserChallengeStartMatch
	{
		CrossGameCrossMultiuserChallengeStartMatch() : header(MT_CROSS_MULTIUSER_CHALLENGE_START_MATCH) {}
		MessageHeader	header;

		struct MatchingUserInfo
		{
			long long	uuid;
			GameName	user_name;
			char		camp;
			char		sex;
			char		prof;
			char		select_skill_id;
			short		level;
			long long	capability;
			int			challenge_score;
			int			challenge_exp;
			int			win_percent;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			mvp_count;
		};

		int				match_key;
		MatchingUserInfo	user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	struct GameCrossCrossMultiuserChallengeMatchResult
	{
		GameCrossCrossMultiuserChallengeMatchResult() : header(MT_GAMECROSS_CROSS_MULTIUSER_CHALLENGE_MATCH_RESULT) {}
		MessageHeader	header;

		struct SyncUserInfo
		{
			long long	uuid;
			GameName	user_name;
			short		level;
			char		prof;
			char		sex;
			int			challenge_score;
			int			challenge_exp;
			int			challenge_level;
			int			match_total_count;
			int			match_win_count;
			int			dur_win_count;
			int			mvp_count;
			int			add_challenge_score;
			int			add_challenge_exp;
			short		is_mvp;
			char		jingjie;
			char		reserve_ch;
			int			add_honor;
			int			add_gongxun;
			long long	reward_add_exp;
		};

		int				match_key;
		int				win_side;
		SyncUserInfo 	sync_user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	static const int MAX_MULTIUSER_CHALLENGE_RANK_COUNT = 1024;

	struct GlobalCrossMultiuserChallengeRankList
	{
		GlobalCrossMultiuserChallengeRankList() : header(MT_GAME_CROSS_MULTIUSER_CHALLENGE_RANK_LIST) {}
		MessageHeader	header;

		struct RankInfo
		{
			int plat_type;
			int role_id;
			GameName name;
			short level;
			char sex;
			char prof;
			short match_total_count;
			char win_percent;
			char jingjie;
			int challenge_score;
			short challenge_level;
			short mvp_count;
			int capability;
		};

		int count;
		RankInfo rank_list[MAX_MULTIUSER_CHALLENGE_RANK_COUNT];
	};

	struct CrossGameMultiuserChallengeScoreRankReward
	{
		CrossGameMultiuserChallengeScoreRankReward() : header(MT_CROSS_GAME_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD) {}
		MessageHeader	header;

		int				uid;
		int				rank_pos;
		int				socre;
	};

	struct CrossGameMultiuserChallengeMatchingStateNotify  // 跨服3V3同步匹配状态
	{
		CrossGameMultiuserChallengeMatchingStateNotify() : header(MT_SYNC_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE) {};
		MessageHeader		header;

		int					cross_uid;
		int					matching_state;
		short				auto_start_cross;	// 1，0 能否主动拉进3v3场景。战斗中掉线重连后上线需要自动拉进3v3场景
		short				notice_match;		// 0,1,2,3 提示匹配冷却。主动放弃战斗的玩家，在本次战斗结束前不能再次匹配
	};

	struct CrossGameMultiuserChallengeGotoScene
	{
		CrossGameMultiuserChallengeGotoScene() : header(MT_CROSS_GAME_MULTIUSER_CHALLENGE_GOTO_SCENE) {}
		MessageHeader		header;

		int					cross_uid;
		int					fb_key;
		int					side;
	};

	struct CrossGameRankWeekRewardCommand
	{
		CrossGameRankWeekRewardCommand() : header(MT_CROSS_GAME_RANK_WEEK_REWARD_COMMAND) {}
		MessageHeader header;

		struct Item
		{
			int plat_type;
			int uid;
			long long  rank_value;
		};

		int rank_type;
		int item_count;
		Item item_list[CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT];
	};

	struct GameCrossCrossMuliuserChallengeGetMatchingState		// 游戏服查询角色跨服3V3匹配状态
	{
		GameCrossCrossMuliuserChallengeGetMatchingState() : header(MT_GAME_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE) {}
		MessageHeader	header;

		long long		uuid;
	};

	//=========================================== 跨服团战  ===========================================
	struct CrossGameTuanzhanGotoScene		// 29042
	{
		CrossGameTuanzhanGotoScene() : header(MT_CROSS_GAME_TUANZHAN_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_uid;
	};

	struct GameCrossTuanzhanSyncFightResult		// 29043
	{
		GameCrossTuanzhanSyncFightResult() : header(MT_GAME_CROSS_TUANZHAN_SYNC_FIGHT_RESULT) {}
		MessageHeader	header;

		long long uuid;														// 全平台唯一ID
		unsigned int score;													// 获得积分
		short user_is_win;													// 是否胜利，0 失败， 1 胜利
		short side_rank;													// 所在阵营内排名
	};

	struct GameCrossCommonInfoSync
	{
	public:
		GameCrossCommonInfoSync() : header(MT_GAME_CROSS_COMMON_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;

		int has_use_gold;												// 当前次跨服消耗的元宝（一般用于复活）
	};

	struct GameCrossCommonAttendInfoSync
	{
	public:
		GameCrossCommonAttendInfoSync() : header(MT_GAME_CROSS_COMMON_ATTEND_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;

		int attend_activity_num;										// 参与该活动的活动号
	};

	enum CROSS_COMMON_OPERA_REQ
	{
		CROSS_COMMON_OPERA_REQ_INVALID = 0,

		CROSS_COMMON_OPERA_REQ_CROSS_GUILDBATTLE_BOSS_INFO,					// 跨服组帮派战Boss信息

		CROSS_COMMON_OPERA_REQ_MAX
	};

	struct CSCrossCommonOperaReq
	{
	public:
		CSCrossCommonOperaReq() : header(MT_GAME_CROSS_COMMON_OPEAR_REQ_CS) {}
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
		long long param_3;
	};


	//============================================= 跨服牧场 =============================================

	struct CrossGameCrossActivityGotoScene // 跨服活动通知玩家进入指定活动对应的场景，之后统一用这个，不用每次都新加一模一样的协议
	{
		CrossGameCrossActivityGotoScene() : header(MT_CROSS_GAME_CROSS_ACTIVITY_GOTO_SCENE) {}
		MessageHeader	header;

		int cross_activity_type;
		int cross_uid;
	};

	struct GameCrossPastureInfoSync
	{
	public:
		GameCrossPastureInfoSync() : header(MT_GAME_CROSS_PASTURE_INFO_SYNC) {}
		MessageHeader header;

		long long uuid;
		int get_score;													// 当前次跨服获得积分
		int get_score_times;											// 当前次跨服获取积分的次数
		int rank;
	};

	//============================================= 跨服BOSS =============================================
	enum CROSS_BOSS_TYPE
	{
		CROSS_BOSS_TYPE_CROSS = 0,
		CROSS_BOSS_TYPE_MIZANG,
		CROSS_BOSS_TYPE_YOUMING,
	};

	struct GameCrossBossSyncPlayerInfo
	{
		GameCrossBossSyncPlayerInfo() : header(MT_GAME_CROSS_BOSS_SYNC_PLAYER_INFO) {}
		MessageHeader	header;

		int cross_boss_type;

		long long uuid;														// 全平台唯一ID
		int today_gather_ordinary_crystal_times;							// 今日已采集普通水晶次数
		int today_gather_treasure_crystal_times;							// 今日已采集珍惜水晶次数
		int tire_value;														// 疲劳值
		unsigned int concern_flag[CROSS_BOSS_SCENE_MAX];

		UNSTD_STATIC_CHECK(CROSS_MIZANG_BOSS_SCENE_MAX <= CROSS_BOSS_SCENE_MAX)       // Getconcernflag的地方用了同一个协议,有memcpy
		UNSTD_STATIC_CHECK(CROSS_YOUMING_BOSS_SCENE_MAX <= CROSS_BOSS_SCENE_MAX)
	};

	struct GameGameBossCreateBossBroadcast                    // boss刷新广播
	{
		GameGameBossCreateBossBroadcast() : header(MT_GAME_GAME_BOSS_CREATE_BOSS_BROADCAST) {}
		MessageHeader	header;

		short boss_id;
		short scene_id;
		unsigned int next_refresh_timestamp;
		long long uuid;
	};

	struct GameGameBossInfoReq
	{
		GameGameBossInfoReq() : header(MT_GAME_GAME_BOSS_INFO_REQ) {}
		MessageHeader	header;

		long long uuid;
		int cross_boss_type;
		int req_type;
		int param_1;
		int param_2;
	};

	/////////////////////////////////////////////  跨服组队 /////////////////////////////////////////////
	struct SyncCrossTeamFBUserInfo
	{
		SyncCrossTeamFBUserInfo() : header(MT_SYNC_CROSS_TEAM_FB_USER_INFO) {}
		MessageHeader	header;

		CrossTeamFBUserInfo user_info;
	};

	struct CrossTeamFBOptionReq
	{
		CrossTeamFBOptionReq() : header(MT_CROSS_TEAM_FB_USER_OPTION_INFO) {}
		MessageHeader	header;

		int option_type;
		int param;
		int param2;
		int param3;
		CrossTeamFBUserInfo user_info;
	};

	struct CrossTeamFBOption2Req
	{
		CrossTeamFBOption2Req() : header(MT_CROSS_TEAM_FB_ROOM_OPTION_INFO) {}
		MessageHeader	header;

		int option2_type;
		int layer;
		int room;
	};

	/////////////////////////////////////////////  跨服帮派战 /////////////////////////////////////////////
	struct GameCrossGuildBattleInfoSync
	{
	public:
		GameCrossGuildBattleInfoSync() : header(MT_GAME_CROSS_GUILD_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		CrossGuildBattleSceneInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct CrossGameGuildBattleInfoSync
	{
	public:
		CrossGameGuildBattleInfoSync() : header(MT_CROSS_GAME_GUILD_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		CrossGuildBattleRankItem owner_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct GameCrossGuildBattleRankLogSync
	{
	public:
		GameCrossGuildBattleRankLogSync() : header(MT_GAME_CROSS_GUILD_BATTLE_RANK_LOG_SYNC) {}
		MessageHeader header;

		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	struct GameCrossGetGuildBattleRankLogReq
	{
	public:
		GameCrossGetGuildBattleRankLogReq() : header(MT_GAME_CROSS_GET_GUILD_BATTLE_RANK_LOG_REQ) {}
		MessageHeader header;

		UniqueUserID uni_user_id;
	};

	struct CrossGameGetGuildBattleRankLogResp
	{
	public:
		CrossGameGetGuildBattleRankLogResp() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_RANK_LOG_RESP) {}
		MessageHeader header;

		UniqueUserID uni_user_id;
		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	struct CrossGameGetGuildBattleBossInfoReq
	{
	public:
		CrossGameGetGuildBattleBossInfoReq() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_BOSS_INFO_REQ) {}
		MessageHeader header;

		long long uuid;
		int scene_id;
	};

	struct CrossGameGetGuildBattleMonsterInfoReq
	{
	public:
		CrossGameGetGuildBattleMonsterInfoReq() : header(MT_CROSS_GAME_GET_GUILD_BATTLE_MONSTER_INFO_REQ) {}
		MessageHeader header;

		long long uuid;
	};

	struct CrossGameGuildBattleSpecialTimeNotice
	{
		CrossGameGuildBattleSpecialTimeNotice() : header(MT_CROSS_GAME_GUILD_BATTLE_SPECIAL_TIME_NOTICE) {}
		MessageHeader header;
	};

	struct CrossGameGuildBattleUserScoreRank
	{
		CrossGameGuildBattleUserScoreRank() : header(MT_CROSS_GAME_GUILD_BATTLE_USER_SCORE_RANK) {}
		MessageHeader header;

		int count;
		CrossGuildBattleUserScoreRankInfo user_core_rank_list[CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM];
	};

	struct CrossGameGuildBattleRegisterSync
	{
		CrossGameGuildBattleRegisterSync() :header(MT_CROSS_GAME_GUILD_BATTLE_REGISTER_SYNC) {}
		MessageHeader header;

		NetID gs_netid;
	};

	struct GameGameGuildBattleRegisterSync
	{
	public:
		GameGameGuildBattleRegisterSync() : header(MT_GAME_GAME_GUILD_BATTLE_REGISTER_SYNC) {}
		MessageHeader header;

		CrossGuildBattleRankItem owner_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	/////////////////////////////////////////////  跨服钓鱼 /////////////////////////////////////////////

	struct GameCrossFishingInfoSync
	{
		GameCrossFishingInfoSync() : header(MT_GAME_CROSS_FISHING_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameFishingInfoSync
	{
		CrossGameFishingInfoSync() : header(MT_CROSS_GAME_FISHING_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};

	/////////////////////////////////////////////  跨服世界等级 /////////////////////////////////////////////
	struct CrossGameCrossWorldLevelInfo
	{
	public:
		CrossGameCrossWorldLevelInfo() : header(MT_CROSS_GAME_CROSS_WORLD_LEVEL_INFO) {}
		MessageHeader header;

		int world_level;
	};

	/////////////////////////////////////////////  跨服猎鲲地带 /////////////////////////////////////////////
	struct GameCrossLieKunFBInfoSync
	{
	public:
		GameCrossLieKunFBInfoSync() : header(MT_GAME_CROSS_LIEKUN_FB_INFO_SYNC) {}
		MessageHeader header;

		CrossLieKunFBSceneInfo scene_list[LIEKUN_ZONE_TYPE_COUNT];
	};

	struct GameCrossLieKunFBKillZoneBossSync
	{
	public:
		GameCrossLieKunFBKillZoneBossSync() : header(MT_GAME_CROSS_LIEKUN_FB_KILL_ZONE_BOSS_SYNC) {}
		MessageHeader header;

		long long uuid;

		int zone;
		GuildID kill_zone_boss_guild_id;
	};

	struct CrossGameLieKunFBKillZoneBossSync
	{
	public:
		CrossGameLieKunFBKillZoneBossSync() : header(MT_CROSS_GAME_LIEKUN_FB_KILL_ZONE_BOSS_SYNC) {}
		MessageHeader header;

		long long uuid;

		int zone;
		GuildID kill_zone_boss_guild_id;
	};

	struct CrossGameLieKunFBTopRewardSync
	{
	public:
		CrossGameLieKunFBTopRewardSync() : header(MT_CROSS_GAME_LIEKUN_FB_TOP_REWARD_SYNC) {}
		MessageHeader header;
	};

 /////////////////////////////////////////////  跨服乱斗 /////////////////////////////////////////////
	struct GameCrossMessBattleInfoSync
	{
		GameCrossMessBattleInfoSync() : header(MT_GAME_CROSS_MESS_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameMessBattleInfoSync
	{
		CrossGameMessBattleInfoSync() : header(MT_CROSS_GAME_MESS_BATTLE_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};
	/////////////////////////////////////////////  跨服夜战王城 ///////////////////////////////////////////
	struct GameCrossnNightFightInfoSync
	{
		GameCrossnNightFightInfoSync() : header(MT_GAME_CROSS_NIGHT_FIGHT_INFO_SYNC) {}
		MessageHeader header;

		Int64 unique_user_id;
		int rank;
	};

	struct CrossGameNightFightInfoSync
	{
		CrossGameNightFightInfoSync() : header(MT_CROSS_GAME_NIGHT_FIGHT_INFO_SYNC) {}
		MessageHeader header;

		Int32 uid;
		int rank;
	};

	////////////////////////////////////	跨服排行榜		//////////////////////////////////

	struct CrossGameSyncRankFirst
	{
		CrossGameSyncRankFirst() : header(MT_CROSS_GAME_SYNC_RANK_FIRST) {}
		MessageHeader	header;

		int rank_type;
		int uid_list[CROSS_RANK_REWARD_COUNT];
	};

	struct CrossGameSyncCoupleRankFirst
	{
		CrossGameSyncCoupleRankFirst() : header(MT_CROSS_GAME_SYNC_COUPLE_RANK_FIRST) {}
		MessageHeader	header;

		int rank_type;
		long long male_uuid_list[CROSS_RANK_REWARD_COUNT];
		long long female_uuid_list[CROSS_RANK_REWARD_COUNT];
	};

	struct GameCrossSyncCrossCoupleRankInfo
	{
		GameCrossSyncCrossCoupleRankInfo() : header(MT_GAME_CROSS_SYNC_CROSS_COUPLE_RANK_INFO) {}
		MessageHeader	header;

		struct RankItem
		{
			bool operator< (const RankItem &rhs) const
			{
				return male_rank_value + female_rank_value < rhs.male_rank_value + rhs.female_rank_value;
			}

			int server_id = 0;				// 服务器id
			int male_uid = 0;				// 男uid
			int female_uid = 0;				// 女uid
			GameName male_name = { 0 };		// 男名字
			GameName female_name = { 0 };	// 女名字
			char male_prof = 0;				// 男职业
			char female_prof = 0;			// 女职业
			short reserve_sh = 0;			// 保留
			int male_rank_value = 0;		// 男排行值
			int female_rank_value = 0;		// 女排行值
		};

		int plat_type;

		int rank_type;
		int item_count;
		RankItem rank_item_list[COUPLE_RANK_MAX_SIZE];
	};

	struct GameCrossCoupleRankDivorce
	{
		GameCrossCoupleRankDivorce() : header(MT_GAME_CROSS_SYNC_COUPLE_RANK_DIVORCE) {}
		MessageHeader	header;

		int plat_type;
		int uid;
	};

	struct CrossGameSyncCrossCoupleForceSnap
	{
		CrossGameSyncCrossCoupleForceSnap() : header(MT_CROSS_GAME_SYNC_CROSS_COUPLE_FORCE_SNAP) {}
		MessageHeader	header;
	};

	//=================================================== 跨服普通boss活动 ===========================================
	enum CROSS_COMMON_BOSS_SYNC_REASON_TYPE
	{
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_BORN = 0,		// 怪物出生
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DIE,				// 怪物死亡
		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DROP,            // 掉落

		CROSS_COMMON_BOSS_SYNC_REASON_TYPE_MAX
	};

	struct CrossGameCrossCommonBossInfoChange
	{
		struct KillerInfo
		{
			int uid;
			unsigned int kill_timestamp;
			GameName name;
		};

		CrossGameCrossCommonBossInfoChange() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_INFO_CHANGE) {}
		MessageHeader header;

		short boss_type;
		short reason_type;
		unsigned short scene_id;
		unsigned short boss_id;
		unsigned int next_refresh_timestamp;
		KillerInfo killer;
	};

	struct CrossGameSyncCrossCommonBossInfoReq
	{
		CrossGameSyncCrossCommonBossInfoReq() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO_REQ) {}
		MessageHeader header;

		unsigned int global_netid;
	};

	struct CrossGameSyncCrossCommonBossInfo
	{
		static const int BOSS_MAX_COUNT = 50;
		static const int KILLER_INFO_MAX_COUNT = 5;
		struct KillerInfo
		{
			int killer_uid;
			int kill_timestamp;
			GameName killer_name;
		};

		struct BossInfo
		{
			unsigned short scene_id;
			unsigned short boss_id;
			char boss_status;
			char reserve_ch_list[3];
			unsigned int next_refresh_timestamp;

			KillerInfo killer_info_list[KILLER_INFO_MAX_COUNT];
		};

		CrossGameSyncCrossCommonBossInfo() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO) {}
		MessageHeader header;

		int boss_type;
		int boss_count;
		BossInfo boss_info_list[BOSS_MAX_COUNT];
	};

	struct CrossGameSyncCrossCommonBossDropInfo
	{
		CrossGameSyncCrossCommonBossDropInfo() : header(MT_CROSS_GAME_CROSS_COMMON_BOSS_DROP_INFO) {}
		MessageHeader header;

		long long uuid;
		GameName role_name;
		int scene_id;
		unsigned short monster_id;
		ItemID item_id;
		int item_num;
		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
	};

	////////////////////////////////////	跨服边境之地		//////////////////////////////////
	struct GameCrossSyncCrossBianJingZhiBossInfo
	{
		GameCrossSyncCrossBianJingZhiBossInfo() : header(MT_HIDDEN_CROSS_BIANJINGZHIDI_BOSS_INFO) {}
		MessageHeader header;

		struct BossInfo
		{
			int boss_id;
			int live_flag;
		};

		unsigned int next_boss_refresh_time;
		BossInfo boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
	};

	///////////////////////////////////////////////////////////////////////////////////////////

	struct UseGoldInHiddenGs
	{
		UseGoldInHiddenGs() : header(MT_USE_GOLD_IN_HIDDEN_GS) {}
		MessageHeader header;

		int is_use_bind;
		int is_consume_sys;
		int uid;
		GameName user_name;
		PlatName plat_name;
		long long use_gold;
		long long cur_gold;
		char type[128];
	};

	//=================================================== 跨服随机活动 ===========================================
	struct CrossGameSyncCrossRandActivityData
	{
		CrossGameSyncCrossRandActivityData() : header(MT_CROSS_GAME_SYNC_CROSS_RAND_ACTIVITY_DATA) {}
		MessageHeader	header;

		int			activity_type;			// 随机活动类型

											// 后面接活动数据
	};

	struct GameCrossSyncCrossRandActivityData
	{
		GameCrossSyncCrossRandActivityData() : header(MT_GAME_CROSS_SYNC_CROSS_RAND_ACTIVITY_DATA) {}
		MessageHeader	header;

		int			activity_type;			// 随机活动类型

											// 后面接活动数据
	};
	
	struct GameGameCrossGuildBattleSos
	{
		GameGameCrossGuildBattleSos():header(MT_GAME_GAME_CROSS_GUILD_BATTLE_SOS){}
		MessageHeader header;

		static const int MEMBER_SCENE_NAME_LENGTH = 32;

		GuildID				guild_id;
		int					member_uid;
		GameName			member_name;
		int					member_camp;
		int					member_scene_id;
		int					member_scene_key;
		short				member_pos_x;
		short				member_pos_y;
		char				member_scene_name[MEMBER_SCENE_NAME_LENGTH];
		short				sos_type;
		short				sos_sysmsg;			// 1SOS仙盟聊天窗口
	};

	struct GameGameKillCrossBoss
	{
		GameGameKillCrossBoss():header(MT_GAME_GAME_KILL_CROSS_BOSS){}
		MessageHeader header;

		GuildID guild_id;
		UserID  uid;
		short scene_type;
		short reserve_sh;
	};

	struct PickGuildRareItemInHiddenGs
	{
		PickGuildRareItemInHiddenGs() : header(MT_PICK_GUILD_RARE_ITEM_IN_HIDDEN_GS) {}
		MessageHeader header;

		struct GuildRareLogItem
		{
			GuildRareLogItem() :uuid(0), item_id(0), item_num(0), timestamp(0), scene_id(0), monster_id(0)
			{
				memset(role_name, 0, sizeof(GameName));
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			long long uuid;
			GameName role_name;
			ItemID item_id;
			short item_num;
			short is_from_gift;
			ItemID gift_item_id;
			unsigned int timestamp;
			int	scene_id;
			int monster_id;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// 仙品属性类型
		};

		GuildID guild_id;
		GuildRareLogItem item;
	};

	struct BroadcastGuildRareInfo
	{
		BroadcastGuildRareInfo() : header(MT_BROADCAST_GUILD_RARE_INFO) {}
		MessageHeader header;

		struct GuildRareLogItem
		{
			GuildRareLogItem() :uuid(0), item_id(0), item_num(0), timestamp(0), scene_id(0), monster_id(0)
			{
				memset(role_name, 0, sizeof(GameName));
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			long long uuid;
			GameName role_name;
			ItemID item_id;
			short item_num;
			short is_from_gift;
			ItemID gift_item_id;
			unsigned int timestamp;
			int	scene_id;
			int monster_id;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// 仙品属性类型
		};

		int plat_type;
		GuildID guild_id;
		long long target_uuid;

		GuildRareLogItem item;
	};

	struct SyncCrossRankInfoToOriginGs
	{
		SyncCrossRankInfoToOriginGs() : header(MT_SYNC_CROSS_RANK_INFO) {}
		MessageHeader header;

		int cross_rank_type;
		int param1;
		int param2;
		long long rank_value;
		int rank_pos;
	};

	struct SyncCrossSystemMsgToOriginServer  // 隐藏服同步系统消息回原服(隐藏服到跨服使用了转发协议，这个定义只是从跨服回原服)
	{
		SyncCrossSystemMsgToOriginServer() : header(MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN) {}
		MessageHeader header;

		long long uuid; // 唯一标识符，可能是公会uuid或者玩家uuid
		int msg_type;   // enum SYS_MSG_TYPE
		int msg_length;
		// ... 紧跟消息内容 Protocol::SCSystemMsg
	};

	struct CrossGameRequestGuildChat			// 隐藏服中请求本服仙盟聊天
	{
		CrossGameRequestGuildChat() : header(MT_CROSS_GAME_GUILD_CHAT) {}
		MessageHeader header;

		long long			guild_id;	// 公会ID

		int					msg_origin_type;
		int					msg_length;
		char				msg_buff[2048];  // 仙盟聊天内容
	};

	struct GameGameCrossGuildMemberBeKill				// 发送仙盟成员被击杀回原服
	{
		GameGameCrossGuildMemberBeKill() :header(MT_GAME_GAME_CROSS_GUILD_MEMBER_BE_KILL) {}
		MessageHeader header;

		long long			killer_uuid;
		GameName			killer_name;
		long long			be_killer_uuid;
		GuildID				be_killer_guild_id;
	};
	
	struct CrossGameGetGuildEnemyList				// 从原服查询获取仙盟仇人列表
	{
		CrossGameGetGuildEnemyList() :header(MT_CROSS_GAME_GET_GUILD_ENEMY_LIST) {}
		MessageHeader header;

		long long			request_uuid;
		GuildID				guild_id;
	};

	//////////////////////////////////// 跨服竞技场 ////////////////////////////////

	struct CrossGameSyncCrossChallengefieldReq   // 跨服竞技场请求
	{
		CrossGameSyncCrossChallengefieldReq() : header(MT_SYNC_CROSS_CHALLENGEFIELD_REQ) {}
		MessageHeader header;

		unsigned int global_netid;
	};

	enum SYNC_CROSS_CHALLENGEFILED_INFO_TYPE
	{
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_INVALID = 0,
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME,			// 初始化原服数据
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_INSERT_TO_HIDDEN,		// 插入新玩家到隐藏服
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_HIDDEN,		// 同步更新到隐藏服
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_RANK_CHANGE_TO_HIDDEN,	// 排名变更更新到隐藏服
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_GAME,			// 同步更新到游戏服
	};

	enum SYNC_CROSS_CHALLENGEFILED_INFO_INIT_SUB_TYPE
	{
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_BEGIN,			// 开始初始化原服数据
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_INFO,			// 信息初始化原服数据
		SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_END,			// 结束初始化原服数据
	};
	struct SyncCrossChallengefieldDataInfo		// 同步竞技场数据
	{
		SyncCrossChallengefieldDataInfo() : header(MT_SYNC_CROSS_CHALLENGEFIELD_DATA_INFO) {}
		MessageHeader header;

		short sync_type;
		unsigned short sub_type;

		int count;
		ChallengeUserListParam::ChallengeUserAttr user_list[CHALLENGE_FIELD_MAX_USER_NUM];
	};

	struct CrossHusongShuijingGatherInfoReq
	{
		CrossHusongShuijingGatherInfoReq():header(MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_REQ){}
		MessageHeader header;
		long long uuid;
	};

	struct GameCrossForbidTalk
	{
		GameCrossForbidTalk() : header(MT_GAME_CROSS_FORBID_TALK) {}
		MessageHeader header;

		long long uuid;
		int forbid_time;
	};

	struct CrossGameForbidTalk
	{
		CrossGameForbidTalk() : header(MT_CROSS_GAME_FORBID_TALK) {}
		MessageHeader header;

		int cross_role_id;
		int forbid_time;
	};
}

#pragma pack(pop)

#endif	// __CROSS_GAME_PROTOCAL_H__

