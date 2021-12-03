#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <map>

#include "servercommon/chatserverdef.h"
#include "servercommon/servercommon.h"

class ChatManager;

class Channel
{
public:
	Channel(ChatManager *chat_manager, ChannelID cid, unsigned int cooldown, ChannelType type, const ChannelName name);
	~Channel();

	ChannelID GetId() { return m_channel_id; }
	unsigned int GetCoolDown() { return m_cooldown; }
	ChannelType	GetType() { return m_type; }

	void GetName(ChannelName name) { F_STRNCPY(name, m_name, sizeof(ChannelName)); }

	bool AddUser(int uid, GSNetID netid);
	bool DeleteUser(int uid);

	void OnChannelMsg(int from_uid, const char *msg, int length);
	void OnChannelSystemMsg(const char *msg, int length);

	void * operator new(size_t c);
	void operator delete(void *m);

private:
	ChatManager *m_chat_manager;
	ChannelID m_channel_id;
	unsigned int m_cooldown;
	ChannelType	m_type;							// 用于给逻辑服务器标记channel类型 对chatserver无用
	ChannelName	m_name;
	
	typedef std::map<int, GSNetID> UserList;	// 这里只做聚合，不做管理
	typedef std::map<int, GSNetID>::iterator UserListIt; 

	UserList m_channel_user_list;
};

#endif // __CHANNEL_HPP__

