#ifndef __GCPROTOCAL_H__
#define __GCPROTOCAL_H__

#pragma pack(push) 
#pragma pack(4)

/*
	ChatServer 聊天服务器

	提供功能：提供创建频道 创建用户 增加用户到频道中 从频道删除用户 销毁用户，销毁频道等功能
	控制服务器创建用户 用户连接ChatServer(两者顺序不严格要求)后 用户信息将在频道中广播 

	控制服务器通过频道ID和用户ID来控制用户 之后由ChatServer通知用户进入(退出)某个频道

	注意：频道ID和用户ID均由控制服务器指定， 唯一性由控制服务器保证
*/

#include "servercommon/chatserverdef.h"

namespace gcprotocal
{
	// MAX_GCPROTOCAL_LENGTH 1024 定义在servercommon.h中
	enum _MsgType
	{
		MT_INVALID,

		MT_GC_BEGIN = 20000,
		MT_CREATE_USER,							// 创建一个聊天用户
		MT_DESTROY_USER,						// 销毁一个聊天用户
		MT_CHANGE_USER_INFO,					// 用户信息改变同步
		MT_CREATE_CHANNEL,						// 创建一个聊天频道
		MT_DESTROY_CHANNEL,						// 销毁一个聊天频道
		MT_ADD_USER_TO_CHANNEL,					// 用户加入聊天频道
		MT_DELETE_USER_FROM_CHANNEL,			// 用户离开聊天频道
		MT_MUTE_USER,							// 用户对某个用户禁言
		MT_UNMUTE_USER,							// 用户对某个用户解除禁言
		MT_ADD_ONLINE_NOTICE,					// 关注在线提示
		MT_DELETE_ONLINE_NOTICE,				// 取消在线提示
		MT_CHANNEL_FORBID,						// 禁言
		MT_GUILD_SYSTEM_MSG,					// 军团系统消息
		MT_RESET_USER_NAME,						// 角色改名
		MT_RELOAD_CONFIG,						// 重读配置
		MT_PRINT_STATUS,						// 调试输出
		MT_SINGLE_CHAT_MONITOR,					// 私聊监控消息
		MT_WORLD_CHAT_MONITOR,					// 世界频道聊天监控消息
		MT_GUILD_CHAT_MONITOR,					// 仙盟频道聊天监控消息
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int	msg_type;
	};

	struct CreateUser
	{
		CreateUser() : header(MT_CREATE_USER) {}
		MessageHeader header;

		UID	uid;
		UserKey key;
		UserName name;
	};

	struct DestroyUser
	{
		DestroyUser() : header(MT_DESTROY_USER) {}
		MessageHeader header;

		UID	uid;
	};

	struct ChangeUserInfo
	{
		ChangeUserInfo() : header(MT_CHANGE_USER_INFO) {}
		MessageHeader	header;

		UID uid;
		char avatar;
		char sex;
		char camp;
		char vip_type;
		char camp_post;
		char guild_post;
		char prof;
		char authority_type;
		short level;
		short vip_level;
	};

	struct CreateChannel
	{
		CreateChannel() : header(MT_CREATE_CHANNEL) {}
		MessageHeader header;

		ChannelID cid;
		unsigned int cooldown;
		ChannelType	type;
		ChannelName name;
	};

	struct DestroyChannel
	{
		DestroyChannel() : header(MT_DESTROY_CHANNEL) {}
		MessageHeader header;

		ChannelID cid;
	};

	struct AddUserToChannel
	{
		AddUserToChannel() : header(MT_ADD_USER_TO_CHANNEL) {}
		MessageHeader header;

		UID	uid;
		ChannelID cid;
	};

	struct DeleteUserFromChannel
	{
		DeleteUserFromChannel() : header(MT_DELETE_USER_FROM_CHANNEL) {}
		MessageHeader header;

		UID	uid;
		ChannelID cid;
	};

	struct MuteUser
	{
		MuteUser() : header(MT_MUTE_USER) {}
		MessageHeader header;

		UID uid;
		UID mute_uid;
	};

	struct UnMuteUser
	{
		UnMuteUser() : header(MT_UNMUTE_USER) {}
		MessageHeader header;

		UID uid;
		UID	unmute_uid;
	};

#define MAX_NOTICE_NUM	1024

	struct AddOnlineNotice
	{
		AddOnlineNotice() : header(MT_ADD_ONLINE_NOTICE) {}
		MessageHeader header;

		UID uid;
		int	count;
		UID	notice_list[MAX_NOTICE_NUM];
	};

	struct DeleteOnlineNotice
	{
		DeleteOnlineNotice() : header(MT_DELETE_ONLINE_NOTICE) {}
		MessageHeader header;

		UID uid;
		UID	d_notice_uid;
	};

	struct ChannelForbid
	{
		ChannelForbid() : header(MT_CHANNEL_FORBID) {}
		MessageHeader header;

		UID	uid;
		unsigned int forbid_time;
	};

	struct PrintStatus
	{
		PrintStatus() : header(MT_PRINT_STATUS) {}
		MessageHeader header;
	};

	struct GuildSystemMsg 
	{
		GuildSystemMsg() : header(MT_GUILD_SYSTEM_MSG) {}
		MessageHeader header;

		ChannelID channel_id;
		int msg_length;
		char msg[MAX_CHAT_LENGTH];
	};

	struct ResetUserName
	{
		ResetUserName():header(MT_RESET_USER_NAME){}
		MessageHeader	header;

		UID				uid;
		UserName		name;
	};

	struct ChatReloadConfig
	{
		ChatReloadConfig():header(MT_RELOAD_CONFIG){}
		MessageHeader	header;
	};

	struct SingleChatMonitor
	{
		SingleChatMonitor():header(MT_SINGLE_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};

	struct WorldChatMonitor
	{
		WorldChatMonitor():header(MT_WORLD_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};

	struct GuildChatMonitor
	{
		GuildChatMonitor():header(MT_GUILD_CHAT_MONITOR){}
		MessageHeader	header;

		UID				uid;
		unsigned int	msg_time;

		int				msg_length;
		char			msg[MAX_CHAT_LENGTH];	
	};
}

#pragma pack(pop)

#endif


