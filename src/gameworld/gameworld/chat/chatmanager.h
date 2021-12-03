#ifndef __CHANNELMANAGER_HPP__
#define __CHANNALMANAGER_HPP__

#include <unordered_map>
#include <map>
#include <list>

#include "chat/msgboarditem.h"
#include "servercommon/chatserverdef.h"

class Role;
class Channel;
class ChatUser;

class ChatManager
{
public:
	static ChatManager & Instance();

	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);
	void OnChannelAddUser(int channel_id, int uid);
	void OnChannelRemoveUser(int channel_id, int uid);
	ChatUser * GetChatUser(int uid);

	// �����ҡ����Դ���
	void OnChatForbidRole(int forbid_uid);
	void OnRequestForbidInfo(Role *role);

	ChatUser * CreateChatUser(int uid, GSNetID netid);
	Channel * CreateChatChannel(ChannelID cid, unsigned int cooldown, ChannelType type, const ChannelName name);
	bool DestroyChatChannel(ChannelID cid);
	Channel * GetChatChannel(ChannelID cid);

	bool AddToChannel(ChannelID cid, int uid);
	void RemoveFromChannel(ChannelID cid, int uid);

	void OnChannelChat(Role *role, void* msg, int length, short origin_type);
	void OnSingleChat(Role *role, void* msg, int length, bool is_sys);

	void OnCrossChannelChat(short channel_type, short camp, int guild_id, const char* buff, int total_len);
	void OnCrossSingleChat(long long to_uuid, const char *buff, int total_len);

	void OnGuildChatInCross(UniqueUserID guild_id, int msg_origin_type, const char* buff, int length);  // ���������ԭ�����˷�������ϵͳ����
	
	void CheckSingleChatBoard(Role *role);
	void CheckGuildChatBoard(Role *role);

	void SyncChatOpenLevelToClient(Role *role = nullptr);
	void AddGuildChatBoard(int guild_id, const char *msg, int msg_len);

	void SetChannelChatForbid(int channel_type, unsigned int start_forbid_timestamp, unsigned int end_forbid_timestamp);
	bool IsInForbidChannelChatTime(int channel_type);

	void CheckWorldChatBoard(Role *role);
	void AddWorldChatBoard(const char *msg, int msg_len, bool is_speak = false);

	// ˽��-------------------------------------------
	void AddCanAckSingleChat(long long role_id, long long target_id);						// ����ܷ�������
	bool IsCanAckSingleChat(long long role_id, long long target_id);						// �ܷ�������
	void AddListenRoleInfo(Role *role, long long target_uuid);					// ��Ӽ����������״̬
	void OnCrossAddListenRoleInfo(long long uuid, long long target_uuid);		// �����ط���Ӽ����������״̬
	void DeleteListenRoleInfo(Role *role, long long target_uuid);				// ɾ�����������������״̬
	void DeleteListenAllRoleInfo(Role *role);									// ɾ������ȫ���������״̬
	void CheckChangeRoleOnline(long long uuid);									// �������״̬���
	void SendOnlineInfo(long long send_to_uuid, long long uuid, int online_type);

	bool GetFromUUIDByCrossSingleChatBuff(const char *buff, size_t len, long long *from_uuid);	//�ڿ��˽��Э�����buff��from_uuid
	bool GetFromUUIDBySingleChatBuff(const char *buff, size_t len, long long *from_uuid);		//��˽��Э�����buff��from_uuid

protected:
	void OnWorldChat(Role *role, const char* buff, int total_len);
	void OnSceneChat(Role *role, const char* buff, int total_len);
	void OnTeamChat(Role *role, const char* buff, int total_len);
	void OnGuildChat(Role *role, const char* buff, int total_len);
	void OnCampChat(Role *role, const char* buff, int total_len);
	void OnWorldQuestionChat(Role *role, const char* buff, int total_len);
	void OnGuildQuestionChat(Role *role, const char* buff, int total_len);

	void CheckCrossChannelChat(Role *role, short channel_type, const char* buff, int total_len);
	void CheckCrossSingleChat(Role *role, long long to_uuid, const char *buff, int total_len);

	bool AddSingleChatBoard(int uid, const char *msg, int msg_len);

	ChatManager();
	~ChatManager();

	typedef std::map<ChannelID, Channel*> ChannelList;
	typedef std::map<ChannelID, Channel*>::iterator ChannelListIt; 

	ChannelList	m_channel_list;

	typedef std::map<int, ChatUser*> ChatUserList; 
	typedef std::map<int, ChatUser*>::iterator ChatUserListIt; 

	ChatUserList m_chat_user_list;

	typedef std::list<MsgBoardItem> MsgBoardItemQueue;
	typedef std::list<MsgBoardItem>::iterator MsgBoardItemQueueIt;
	typedef std::map<int, MsgBoardItemQueue> MsgBoardMap;
	typedef std::map<int, MsgBoardItemQueue>::iterator MsgBoardMapIt;

	MsgBoardMap m_single_chat_board_map;
	MsgBoardMap m_guild_chat_board_map;
	MsgBoardItemQueue m_world_board_list;


	struct ForbidChannelChatInfo
	{
		ForbidChannelChatInfo() : start_forbid_timestamp(0), end_forbid_timestamp(0) {}

		unsigned int start_forbid_timestamp;
		unsigned int end_forbid_timestamp;
	};

	ForbidChannelChatInfo m_channel_chat_forbid_info_list[OPENLEVEL_LIMIT_TYPE_MAX];	// ��ֹ����ʱ����б�

	std::set<int> m_forbid_uid_set;

	// ˽��-----------------------------------------
	std::unordered_map<long long, std::set<long long> >   m_recent_single_chat_role_map;					// uid - uid_set  ��¼���˽�Ĺ�uid������б�uid_set
	std::unordered_map<long long, std::set<long long> >   m_single_chat_listen_role_map;		// uuid - uuid_set  ��¼uuid��������������б�uuid_set
	std::unordered_map<long long, std::set<long long> >   m_single_chat_be_listen_role_map;		// uuid - uuid_set  ��¼uuid����������������б�uuid_set
};

#endif

