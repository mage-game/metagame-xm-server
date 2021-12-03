#ifndef __MSGCHATCTRL_H__
#define __MSGCHATCTRL_H__

#include "servercommon/chatserverdef.h"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct CSUserLogin
	{
		CSUserLogin();
		MessageHeader header;

		UID uid;
		UserKey	key;
	};

	enum _LoginAck
	{
		LA_SUC = 0,								// 登录成功
		LA_LOGINED = -1,						// 已登录
		LA_KEY_ERROR = -2,						// KEY认证失败
		LA_USER_EXIST = -3,						// 用户已存在
	};

	struct SCUserLoginAck
	{
		SCUserLoginAck();
		MessageHeader header;

		int	ack;
	};

	struct SCUserBeDestroy
	{
		SCUserBeDestroy();
		MessageHeader header;
	};

	struct SCIntoChannel
	{
		SCIntoChannel();
		MessageHeader header;

		ChannelID channel_id;
		unsigned int cooldown;
		ChannelType channel_type;
		ChannelName channel_name;
	};

	struct SCOutOfChannel
	{
		SCOutOfChannel();
		MessageHeader header;

		ChannelID channel_id;
	};

	#define MAX_NOTICE_ONLINE_NUM	1024
	struct SCNoticeOnline
	{
		SCNoticeOnline();
		MessageHeader header;

		int	count;
		UID online_uid_list[MAX_NOTICE_ONLINE_NUM];
	};

	struct SCNoticeOffline
	{
		SCNoticeOffline();
		MessageHeader header;

		UID	offline_uid;
	};

	struct CSChangeOnlineStatus
	{
		CSChangeOnlineStatus();
		MessageHeader header;

		int status;
	};

	struct SCNoticeOnlineStatus
	{
		SCNoticeOnlineStatus();
		MessageHeader header;

		UID uid;
		int	status;
	};

	struct CSMuteUser
	{
		CSMuteUser();
		MessageHeader header;

		UID	mute_uid;
	};

	struct CSUnMuteUser 
	{
		CSUnMuteUser();
		MessageHeader header;

		UID	unmute_uid;
	};
}

#pragma pack(pop)

#endif // __MSGCHATCTRL_H__

