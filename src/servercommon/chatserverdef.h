/******************************************************************************************
FileName: chatserverdef.hpp
Author: dreamwang
Description: 聊天服务器相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __CHATDEF_H__
#define __CHATDEF_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

typedef unsigned int	NetID;
typedef Int32			UID;
typedef UInt32			UserKey;					// 由用户创建用户时赋予用户 用户同时传输一个UserKey 两者必须相等
typedef Int32			ChannelID;
typedef Int32			ChannelType;


typedef char UserName[32];
typedef char ChannelName[32];

static const int WORLD_CHANNEL_ID = 0;
static const int MAX_CHAT_LENGTH = 2048;
static const int MAX_SPEAKER_MSG_LENGTH = 512;

static const int CHANNEL_OPEN_LEVEL_DEFAULT = 10;

// static const int SINGLECHAT_OPEN_LEVEL = 140;

// 聊天监控
static const int MAX_SINGLE_CHAT_MONITOR_LENGTH = 512;
static const int MAX_GUILD_CHAT_MONITOR_LENGTH = 128;
static const int MAX_WORLD_CHAT_MONITOR_LENGTH = 128;
static const int MAX_MAIL_MONITOR_SUBJECT_LENGTH = 128;

enum OPENLEVEL_LIMIT_TYPE
{
	OPENLEVEL_LIMIT_TYPE_WORLD = 0,			// 世界聊天
	OPENLEVEL_LIMIT_TYPE_CAMP,				// 阵营聊天
	OPENLEVEL_LIMIT_TYPE_SCENE,				// 场景聊天
	OPENLEVEL_LIMIT_TYPE_TEAM,				// 组队聊天
	OPENLEVEL_LIMIT_TYPE_GUILD,				// 仙盟聊天
	OPENLEVEL_LIMIT_TYPE_SINGLE,			// 私聊
	OPENLEVEL_LIMIT_TYPE_SEND_MAIL,			// 邮件
	OPENLEVEL_LIMIT_TYPE_SPEAKER,			// 喇叭

	OPENLEVEL_LIMIT_TYPE_MAX,
};

enum CHAT_LIMIT_CONDITION_TYPE
{
	CHAT_LIMIT_CONDITION_TYPE_AND = 0,		// 聊天限制条件与
	CHAT_LIMIT_CONDITION_TYPE_OR,			// 聊天限制条件或

	CHAT_LIMIT_CONDITION_TYPE_MAX
};

#endif // __CHATDEF_H__


