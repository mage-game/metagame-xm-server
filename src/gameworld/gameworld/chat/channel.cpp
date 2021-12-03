#include "channel.h"

#include "engineadapter.h"

#include "protocal/msgchatctrl.h"
#include "servercommon/servercommon.h"
#include "chat/chatmanager.h"

const int MSX_GSNETID_NUM = 1024 * 512;
GSNetID gsnetidlist[MSX_GSNETID_NUM];

Channel::Channel(ChatManager *chat_manager, ChannelID cid, unsigned int cooldown, ChannelType type, const ChannelName name)
	: m_chat_manager(chat_manager), m_channel_id(cid), m_cooldown(cooldown), m_type(type)
{
	F_STRNCPY(m_name, name, sizeof(ChannelName));
}

Channel::~Channel()
{
	for (UserListIt it = m_channel_user_list.begin(); it != m_channel_user_list.end(); ++it)
	{
		m_chat_manager->OnChannelRemoveUser(m_channel_id, it->first);
	}

	m_channel_user_list.clear();
}

bool Channel::AddUser(int uid, GSNetID netid)
{
	UserListIt it = m_channel_user_list.find(uid);
	if (it != m_channel_user_list.end())
	{
		return false;
	}

	m_channel_user_list[uid] = netid;

	if (netid != INVALID_GSNETID)
	{
		Protocol::SCIntoChannel ic;
		ic.channel_id = m_channel_id;
		ic.cooldown = m_cooldown;
		ic.channel_type = m_type;
		F_STRNCPY(ic.channel_name, m_name, sizeof(ic.channel_name));
		EngineAdapter::Instance().NetSend(netid, (const char*)&ic, sizeof(Protocol::SCIntoChannel));
	}

	m_chat_manager->OnChannelAddUser(m_channel_id, uid);

	return true;
}

bool Channel::DeleteUser(int uid)
{
	UserListIt it = m_channel_user_list.find(uid);
	if (it == m_channel_user_list.end()) 
	{
		return false;
	}

	m_channel_user_list.erase(it);

	m_chat_manager->OnChannelRemoveUser(m_channel_id, uid);

	return true;
}

void Channel::OnChannelMsg(int from_uid, const char *msg, int length)
{
	int num = 0;

	for (UserListIt it = m_channel_user_list.begin(); num < MSX_GSNETID_NUM && it != m_channel_user_list.end(); ++it)
	{
		if (it->second != INVALID_GSNETID)
		{
			gsnetidlist[num++] = it->second;
		}
	}

	if (num != 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetidlist, num, msg, length);
	}
}

void Channel::OnChannelSystemMsg(const char *msg, int length)
{
	int num = 0;

	for (UserListIt it = m_channel_user_list.begin(); num < MSX_GSNETID_NUM && it != m_channel_user_list.end(); ++it)
	{
		if (it->second != INVALID_GSNETID)
		{
			gsnetidlist[num++] = it->second;
		}
	}

	if (num != 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetidlist, num, msg, length);
	}
}

