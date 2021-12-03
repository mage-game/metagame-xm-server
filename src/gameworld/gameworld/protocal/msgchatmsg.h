#ifndef __MSGCHATMSG_H__
#define __MSGCHATMSG_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CHAT_CONTENT_TYPE
	{
		CHAT_CONTENT_TYPE_TEXT,//文字
		CHAT_CONTENT_TYPE_AUDIO,//免费语音
		CHAT_CONTENT_TYPE_AUDIO_2,//收费语言
		CHAT_CONTENT_TYPE_SELF,//自己文字回显

		CHAT_CONTENT_TYPE_MAX,
	};

	struct CSChannelChat		// 8150	请求频道聊天
	{
		CSChannelChat();
		MessageHeader header;

		char content_type;
		char reserve_ch;
		short channel_type;		// 0：世界，1：阵营，2：场景，3：组队，4：仙盟, 5：世界答题， 6：帮派答题
		int msg_length;
		// 后面紧跟着消息内容
	};

	struct SCChannelChat		// 8100	下发频道聊天
	{
		SCChannelChat();
		MessageHeader header;

		int	from_uid;
		UserName username;		// char [32]

		char sex;
		char camp;
		char prof;
		char authority_type;

		char content_type;
		char tuhaojin_color;		// 土豪金颜色，0 表示未激活
		char bigchatface_status;	// 聊天大表情，0 表示未激活
		char personalize_window_bubble_type;		// 个性聊天框（气泡），-1 表示未激活
		long long avatar_timestamp;
		long long uuid;

		short level;
		char vip_level;
		char channel_type;

		short guild_signin_count;					// 仙盟月签到次数
		char is_msg_record;							// 保留
		char personalize_window_avatar_type;		// 个性聊天框(头像), -1 表示未激活

		char is_answer_true;						// 答题是否正确(用于仙盟答题)
		char reserve_ch;
		short origin_type;							// 来源类型，参考namespace chatdef::enum ORIGIN_TYPE
		short has_xianzunka_flag;					// 仙尊卡拥有标志
		short reserve_sh;

		unsigned int msg_timestamp;
		
		unsigned int msg_length;
		// 后面紧跟着消息内容
	};

	struct CSSingleChat				// 8151	请求私聊
	{
		CSSingleChat();
		MessageHeader header;

		char content_type;
		char is_echo;		// 1回显标记(此消息显示在右侧。用于服务器代发私聊回显，主要实现方式:以对方名义给自己发送)
		char special_param; // 1服务器代发仇人私聊
		char reserve_sh;
		int to_uid;
		long long to_uuid;
		int msg_length;
		// 后面紧跟着消息内容
	};

	struct CSForbidChatInfo		// 8152
	{
		CSForbidChatInfo();
		MessageHeader header;
	};

	struct SCSingleChat				// 8101	下发私聊
	{
		SCSingleChat();
		MessageHeader header;

		int	from_uid;
		long long from_uuid;
		UserName username;			// char [32]
		char sex;
		char camp;
		char vip_level;
		char prof;
		char authority_type;
		char content_type;
		short level;
		char tuhaojin_color;		// 土豪金颜色，0 表示未激活
		char bigchatface_status;	// 聊天大表情，0 表示未激活
		char personalize_window_bubble_type;		// 个性聊天框（气泡），-1 表示未激活
		char personalize_window_avatar_type;		// 个性聊天框（头像），-1 表示未激活
		char is_echo;				// 1回显标记
		char special_param;			// 1服务器代发的仇人私聊
		short has_xianzunka_flag;
		long long avatar_timestamp;
		unsigned int msg_timestamp;
		unsigned int msg_length;
		// 后面紧跟着消息内容
	};

	struct SCSingleChatUserNotExist		// 8102	通知目标不在线
	{
		SCSingleChatUserNotExist();
		MessageHeader header;

		int	to_uid;
	};

	struct SCOpenLevelLimit		// 8103
	{
		SCOpenLevelLimit();
		MessageHeader header;

		struct ForbidTimeInfo
		{
			short begin_hour;					// 禁止聊天开始时间
			short end_hour;						// 禁止聊天结束时间
		};

		int ignoreLevelLimit;
		short is_forbid_audio_chat;
		short is_forbid_change_avatar;						// 是否允许更换头像
		int chat_limit_condition_type;						// 聊天限制条件类型 CHAT_LIMIT_CONDITION_TYPE_AND=0:同时满足角色等级和vip等级， CHAT_LIMIT_CONDITION_TYPE_OR=1:满足其中一个条件
		int open_level_list[OPENLEVEL_LIMIT_TYPE_MAX];
		short vip_level_list[OPENLEVEL_LIMIT_TYPE_MAX];

		ForbidTimeInfo forbid_time_info_list[OPENLEVEL_LIMIT_TYPE_MAX];
		short is_forbid_cross_speaker;
		short reserve_sh;
	};

	struct SCForbidChatInfo		//8104 封禁信息
	{
		SCForbidChatInfo();
		MessageHeader header;

		static const int MAX_FORBID_UID_COUNT = 500;

		int forbid_uid_count;
		int forbid_uid_list[MAX_FORBID_UID_COUNT];
	};

	struct SCForbidUserInfo		// 8105	个人禁言信息
	{
		SCForbidUserInfo();

		MessageHeader header;

		unsigned int forbid_talk_end_timestamp;
	};

	struct SCChatBoardListInfo    // 8106  下发聊天记录内容列表
	{
		SCChatBoardListInfo();
		MessageHeader header;

		short channel_type;		// 聊天频道类型
		short list_count;		// 聊天记录个数
		
		// 后面紧跟聊天内容char buff数组 [len_1 + buff_1 ... len_n + buff_n]
		// len_ ：是int类型，表示每一个聊天消息包的长度
		// buff_ ：是每一个聊天消息包
	};

	// 私聊在线状态通知----------------------------
	enum SINGLE_CHAT_REQ
	{
		SINGLE_CHAT_REQ_ADD = 0,				// 添加监听对象在线状态 p1:plat_type p2:target_id
		SINGLE_CHAT_REQ_DELETE = 1,				// 删除单个监听对象在线状态 p1:plat_type p2:target_id
		SINGLE_CHAT_REQ_DELETE_ALL = 2,			// 删除全部监听对象在线状态 
	};

	struct CSSingleChatReq		// 8153 私聊请求
	{
		CSSingleChatReq();
		MessageHeader header;

		short req_type;
		short reserve_sh;

		int param_1;
		int param_2;
	};

	struct SCSingleChatOnlineStatus		// 8107	返回玩家在线信息
	{
		SCSingleChatOnlineStatus();
		MessageHeader header;

		long long uuid = 0;					// uuid
		char online_type = 0;				// 在线类型
		char reserve_ch = 0;
		short reserve_sh = 0;
	};
}

#pragma pack(pop)

#endif // __MSGCHATMSG_H__

