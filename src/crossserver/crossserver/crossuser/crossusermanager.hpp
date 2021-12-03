#ifndef __CROSS_USER_MANAGER_HPP__
#define __CROSS_USER_MANAGER_HPP__

#include <map>

#include "servercommon/chatserverdef.h"
#include "servercommon/crossdef.hpp"

#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossServer;
class CrossUser;

class CrossUserManager
{
public:
	CrossUserManager();
	~CrossUserManager();

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart() {}
	void OnServerStop() {}

	void OnInternalGSDisconnnect(const UniqueServerID &unique_server_id);

	void Update(unsigned long interval, time_t now_second);

	CrossUser * GetCrossUser(const UniqueUserID &original_unique_user_id);
	CrossUser * GetCrossUser(const UserID &cross_user_id);

	void OnCrossUserLogin(crossgameprotocal::GameCrossUserLogin*gcul);
	void OnCrossUserLogout(const UserID &cross_user_id, bool is_hidden_global_disconnect = false);
	void OnHiddenGlobalDisconnect();
	
	void OnRegisterGameServer(NetID global_netid, crossgameprotocal::GameCrossRegisterGameServer *gcrgs);
	void OnKickCrossUser(const UniqueUserID &original_unique_user_id);

private:
	void Release();

	CrossServer *m_cross_server;

	typedef std::map<UniqueUserID, CrossUser *>	OrignalUUIDCrossUserMap;
	typedef std::map<UniqueUserID, CrossUser *>::iterator OriginalUUIDCrossUserMapIt;
	OrignalUUIDCrossUserMap m_uniqe_user_id_user_map;									// 原服全局角色ID与CrossUser对应表

	typedef std::map<UserID, CrossUser *> CrossUserIDCrossUserMap;
	typedef std::map<UserID, CrossUser *>::iterator CrossUserIDCrossUserMapIt;
	CrossUserIDCrossUserMap m_cross_user_id_user_map;									// 隐藏服UserID与CrossUser对应表
};

#endif //__CROSS_USER_MANAGER_HPP__

