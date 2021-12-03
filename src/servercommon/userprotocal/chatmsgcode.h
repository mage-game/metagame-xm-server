#ifndef __CHATMSGCODE_H__
#define __CHATMSGCODE_H__

namespace Protocol
{
	enum
	{
		CHATCTRL_SC_BEGIN = 8000,
		CHATCTRL_CS_BEGIN = 8050,

		CHATMSG_SC_BEGIN = 8100,
		CHATMSG_CS_BEGIN = 8150,
	};

	enum _MsgTypeChat
	{
		MT_USER_LOGIN_ACK_SC = CHATCTRL_SC_BEGIN,			// 用户登录返回
		MT_USER_BE_DESTROY_SC,								// 通知用户被销毁
		MT_INTO_CHANNEL_SC,									// 用户进入频道
		MT_OUT_OF_CHANNEL_SC,								// 用户离开频道
		MT_NOTICE_ONLINE_SC,								// 上线提醒
		MT_NOTICE_OFFLINE_SC,								// 下线提醒
		MT_NOTICE_ONLINE_STATUS_SC,							// 在线状态变化提醒		

		MT_USER_LOGIN_CS = CHATCTRL_CS_BEGIN,				// 用户请求登录聊天服务器
		MT_MUTE_USER_CS,									// 屏蔽某用户
		MT_UNMUTE_USER_CS,									// 取消屏蔽
		MT_CHANGE_ONLINE_STATUS_CS,							// 用户变更在线状态

		////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_CHANNEL_CHAT_SC = 8100,							// 频道聊天返回
		MT_SINGLE_CHAT_SC = 8101,							// 私聊返回
		MT_SINGLE_CHAT_USET_NOT_EXIST_SC = 8102,			// 私聊 提示用户不在线
		MT_CHAT_LEVEL_LIMIT_SC = 8103,						// 聊天等级限制提示
		MT_FORBID_CHAT_INFO_SC = 8104,						// 禁止聊天信息
		MT_FORBID_TALK_USER_INFO_SC = 8105,					// 角色禁言信息
		MT_CHAT_BOARD_LIST_SC = 8106,						// 下发角色聊天记录列表
		MT_SINGLE_CHAT_INFO_SC = 8107,						// 下发角色私聊状态

		MT_CHANNEL_CHAT_CS = 8150,							// 请求频道聊天
		MT_SINGLE_CHAT_CS = 8151,							// 请求私聊
		MT_FORBID_CHAT_INFO_CS = 8152,						// 请求封禁列表
		MT_SINGLE_CHAT_REQ_CS = 8153,						// 角色私聊操作
	};
}

#endif // __CHATMSGCODE_H__

