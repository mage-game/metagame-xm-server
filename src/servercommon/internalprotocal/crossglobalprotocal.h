#ifndef __CROSS_GLOBAL_PROTOCAL_H__
#define __CROSS_GLOBAL_PROTOCAL_H__

#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

#pragma pack(push) 
#pragma pack(4)

namespace crossglobalprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		MT_CROSSGLOBAL_BEGIN = 28000,
		MT_GLOBALCROSS_USER_MSG,					// 用户发消息到cross Protocol
		MT_CROSSGLOBAL_USER_MSG,					// cross给用户发消息 Protocol
		MT_CROSSGLOBAL_ALL_USER_MSG,				// cross给所有用户发消息 Protocol
		MT_CROSSGLOBAL_GS_MSG,						// 游戏服消息 crossgameprotocal
		MT_TRANSFER_TO_GLOBAL,						// global请求cross中转消息
		MT_TRANSFER_TO_ALL_GLOBAL,					// global请求cross中转消息
		MT_TRANSFER_TO_SCENE_GAMEWORLD,				// cross请求global中转消息
		MT_TRANSFER_TO_USER_GAMEWORLD,				// cross请求global中转消息
		MT_SAVE_CROSS_ROLE_INFO,					// 保存跨服角色信息
		MT_SAVE_CROSS_ROLE_RET,						// 保存跨服角色信息返回
		MT_CROSS_USER_LOGIN,						// 通知cross角色已登录到隐藏服
		MT_CROSS_USER_LOGOUT,						// 通知cross角色退出隐藏服
		MT_GLOBALCROSS_KICK_CROSS_USER,				// global请求cross踢掉跨服角色
		MT_CROSSGLOBAL_KICK_USER,					// cross请求global踢人
		MT_CROSSGLOBAL_KICK_ALL_CROSS_USER,			// 通知隐藏服踢出所有跨服玩家
		MT_CROSSGLOBAL_SYNC_CROSS_ACTIVITY_STATUS,	// 同步跨用活动状态
		MT_CROSSGLOBAL_QUERY_USER_INFO,				// 查询玩家信息
		MT_CROSSGLOBAL_FIXPLATTYPE_CHECK_USER_REQ,	// FIX平台类型错误 cross请求global检查玩家 
		MT_GLOBALCROSS_FIXPLATTYPE_CHECK_USER_ACK,	// FIX平台类型错误 global回应cross检查结果
		MT_CROSS_USER_FIRST_ENTER_SCENE,			// cross角色首次进入场景
	};

	struct MessageHeader
	{
		MessageHeader(): msg_type(MT_INVALID) {}
		MessageHeader(int _msg_type): msg_type(_msg_type) {}
		int	msg_type;
	};

	struct CrossGlobalGSMsg
	{
		CrossGlobalGSMsg() : header(MT_CROSSGLOBAL_GS_MSG) {}
		MessageHeader		header;
		// 后面紧跟 crossgameprotocal 协议内容
	};

	struct GlobalCrossTransferToGlobal
	{
		GlobalCrossTransferToGlobal() : header(MT_TRANSFER_TO_GLOBAL) {}
		MessageHeader		header;
		
		int					target_plat_type;
		int					target_server_id;
		// 后面紧跟 crossglobalprotocal 协议内容
	};

	struct GlobalCrossTransferToAllGlobal
	{
		GlobalCrossTransferToAllGlobal() : header(MT_TRANSFER_TO_ALL_GLOBAL) {}
		MessageHeader		header;

		int					is_send_back;
		// 后面紧跟 crossglobalprotocal 协议内容
	};

	struct CrossGlobalTransferToSceneGameWorld
	{
		CrossGlobalTransferToSceneGameWorld() : header(MT_TRANSFER_TO_SCENE_GAMEWORLD) {}
		MessageHeader		header;

		int					scene_id;
		// 后面紧跟 crossgameprotocal 协议内容
	};

	struct CrossGlobalTransferToUserGameWorld
	{
		CrossGlobalTransferToUserGameWorld() : header(MT_TRANSFER_TO_USER_GAMEWORLD) {}
		MessageHeader		header;

		int					receive_uid;
		// 后面紧跟 crossgameprotocal 协议内容
	};

	struct CrossGlobalSaveCrossRoleInfo
	{
		CrossGlobalSaveCrossRoleInfo() : header(MT_SAVE_CROSS_ROLE_INFO) {}
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

	struct GlobalCrossSaveCrossRoleRet
	{
		GlobalCrossSaveCrossRoleRet() : header(MT_SAVE_CROSS_ROLE_RET) {}
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
	
	struct CrossGlobalKickUser
	{
		CrossGlobalKickUser() : header(MT_CROSSGLOBAL_KICK_USER) {}
		MessageHeader	header;
		
		int				bekick_uid;
	};

	struct CrossGlobalKickAllCrossUser
	{
		CrossGlobalKickAllCrossUser() : header(MT_CROSSGLOBAL_KICK_ALL_CROSS_USER) {}
		MessageHeader	header;

		int				cross_activity_type;
	};

	struct CrossGlobalQueryUserInfo
	{
		CrossGlobalQueryUserInfo() : header(MT_CROSSGLOBAL_QUERY_USER_INFO) {}
		MessageHeader	header;

		enum
		{
			USER_INFO_TYPE_INVALID = 0,
			USER_INFO_TYPE_CROSS_RECORD,			// 跨服个人记录

			USER_INFO_TYPE_MAX
		};

		int				user_info_type;
		int				target_uid;
		long long		req_uuid;
	};

	struct CrossGlobalFixPlatTypeCheckUserReq
	{
		CrossGlobalFixPlatTypeCheckUserReq() : header(MT_CROSSGLOBAL_FIXPLATTYPE_CHECK_USER_REQ) {}
		MessageHeader	header;

		enum 
		{
			FIX_TYPE_INVALID = 0, 
			FIX_TYPE_CROSS_CHALLENGE_USER,				// 修正CrossChallengeUser中的平台类型
			FIX_TYPE_CROSS_CHALLENGE_SESSION_RECORD,	// 修正CrossChallengeSessionRecord中的平台类型

			FIX_TYPE_MAX
		};

		int				fix_type;
		int				fix_param;
		long long		tofix_uuid;
		GameName		tofix_role_name;
	};

	struct GlobalCrossFixPlatTypeCheckUserAck
	{
		GlobalCrossFixPlatTypeCheckUserAck() : header(MT_GLOBALCROSS_FIXPLATTYPE_CHECK_USER_ACK) {}
		MessageHeader	header;

		int				fix_type;
		int				fix_param;
		long long		tofix_uuid;	
		long long		real_uuid;
	};

	struct GlobalCrossCrossUserFirstEnterScene
	{
		GlobalCrossCrossUserFirstEnterScene() : header(MT_CROSS_USER_FIRST_ENTER_SCENE) {}
		MessageHeader	header;

		int				uid;
	};

}

#pragma pack(pop)

#endif

