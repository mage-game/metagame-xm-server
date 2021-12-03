#ifndef __CHATUSER_HPP__
#define __CHATUSER_HPP__

#include <stdlib.h>

#include "servercommon/chatserverdef.h"
#include "engineadapter.h"
#include "protocal/msgchatmsg.h"
#include "servercommon/servercommon.h"

#include <vector>

class Role;
class ChatManager;

class ChatUser
{
public:
	struct UserChannelRef
	{
		ChannelID channel_id;
		unsigned long next_active_time;
	};

	ChatUser(ChatManager *chat_manager, int uid, GSNetID netid);
	~ChatUser();

	const GSNetID & GetNetId() { return m_netid; }
	int GetUID() { return m_uid; }

	void AddChannel(ChannelID change_id); // 被channel adduser和deleteuser时调用
	bool DeleteChannel(ChannelID cid);

	bool IgnoreWorldTalk();
	bool IsForbidChannel(Role *role);
	bool ChannelCanChat(Role *role, int channel_type); // 检查
	int GetChannelOpenLevel(int channel_type);		   // 获取频道聊天开启等级
	int GetChannelOpenVipLevel(int channel_type);	   // 获取频道聊天开启vip等级
	int GetSingleChatOpenVipLevel();				   // 获取私聊开启vip等级

	void AddNotice(UID *uid_list, int num);
	void DeleteNotice(UID uid);
	
	void ResetName(UserName name);

	int FindSingleChatMonitorMinTimeIndex();
	void OnSingleChat(Protocol::CSSingleChat *sc_req);

	void OnGuildChat(Protocol::CSChannelChat *cc_req);
	void OnWorldChat(Protocol::CSChannelChat *cc_req);

private:
	int GetCooldown(int channal_type);

	ChatManager *m_chat_manager;
	int m_uid;
	GSNetID m_netid;

	unsigned int next_active_list[chatdef::CHANNEL_TYPE_MAX];

	std::vector<UserChannelRef> m_channel_vec;

	struct SingleChatMonitor
	{
		SingleChatMonitor() : to_uid(0), last_single_chat_time(0) {}

		UID to_uid;
		long long last_single_chat_time;
	};

	// 私聊 1分钟和超过10人私聊
	static const int MAX_SINGLECHAT_MONITOR_COUNT = 10;
	SingleChatMonitor m_single_chat_monitor[MAX_SINGLECHAT_MONITOR_COUNT];

	int m_world_msg_last_length;
	int m_world_msg_same_length_accumulate_count;
	time_t m_world_msg_first_same_length_timestamp;
	bool m_ignore_world_talk;
};

#endif
