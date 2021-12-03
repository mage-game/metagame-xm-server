#ifndef __MSGSYSTEM_H__
#define __MSGSYSTEM_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		系统相关
	*/

	#define PROTOCAL_MAX_SYSTEM_MSG_LENGTH 2048

	class SCSystemMsg
	{
	public:
		SCSystemMsg();
		MessageHeader header;

		unsigned int send_time;
		short msg_type;
		short msg_length;
		int	display_pos;
		int color;
		char msg[PROTOCAL_MAX_SYSTEM_MSG_LENGTH];
	};

	class SCNoticeNum
	{
	public:
		SCNoticeNum();
		MessageHeader header;

		int notice_num;
	};

	class SCZeroHour
	{
	public:
		SCZeroHour();
		MessageHeader header;
	};

	class SCSpeaker		//703
	{
	public:
		SCSpeaker();	
		MessageHeader header;

		UID	from_uid;
		long long unique_uid;
		UserName username;
		char sex;
		char camp;
		char content_type;
		char camp_post;
		char guild_post;
		char prof;
		char authourity_type;
		char vip_level;
		long long avatar_timestamp;
		PlatName plat_name;
		int server_id;
		char speaker_type;
		char tuhaojin_color;
		short bigchatface_status;
		char personalize_window_type; // 喇叭框皮肤 0 表示未激活
		char personalize_window_bubble_type;	// 气泡框皮肤 -1 表示未激活
		char is_record_msg;						// 是否是消息记录
		char personalize_window_avatar_type;	// 头像框皮肤 -1 表示未激活
		unsigned int send_time_stamp;			// 发送喇叭的时间戳

		unsigned int speaker_msg_length;
		char speaker_msg[MAX_SPEAKER_MSG_LENGTH];
	};
}

#pragma pack(pop)

#endif

