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

	void AddChannel(ChannelID change_id); // ��channel adduser��deleteuserʱ����
	bool DeleteChannel(ChannelID cid);

	bool IgnoreWorldTalk();
	bool IsForbidChannel(Role *role);
	bool ChannelCanChat(Role *role, int channel_type); // ���
	int GetChannelOpenLevel(int channel_type);		   // ��ȡƵ�����쿪���ȼ�
	int GetChannelOpenVipLevel(int channel_type);	   // ��ȡƵ�����쿪��vip�ȼ�
	int GetSingleChatOpenVipLevel();				   // ��ȡ˽�Ŀ���vip�ȼ�

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

	// ˽�� 1���Ӻͳ���10��˽��
	static const int MAX_SINGLECHAT_MONITOR_COUNT = 10;
	SingleChatMonitor m_single_chat_monitor[MAX_SINGLECHAT_MONITOR_COUNT];

	int m_world_msg_last_length;
	int m_world_msg_same_length_accumulate_count;
	time_t m_world_msg_first_same_length_timestamp;
	bool m_ignore_world_talk;
};

#endif
