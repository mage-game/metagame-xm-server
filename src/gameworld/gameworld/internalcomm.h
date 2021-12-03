#ifndef INTERNALCOMM_H
#define INTERNALCOMM_H

#include "inetworkmodule.h"
#include <string>

#include "servercommon/dbhandler.h"
#include "servercommon/serverdef.h"
#include "servercommon/serverconfig/crossconfig.hpp"

class InternalComm
{
public:
	static InternalComm &Instance()
	{
		static InternalComm ic;
		return ic;
	}

	void Init(INetworkModule *internal_network);

	bool NetListen(Port port, int backlog, NetID *netid_out = 0)
	{
		return m_internal_network->Listen(port, backlog, netid_out);
	}

	bool NetConnect(const char *ip, Port port, NetID *netid_out, unsigned long time_out = 3000)
	{
		return m_internal_network->Connect(ip, port, netid_out, time_out);
	}
	
	bool NetConnectAsyn(const char *ip, Port port, int *handle, unsigned long time_out=3000)
	{
		return m_internal_network->ConnectAsyn(ip, port, handle, time_out);
	}

	bool NetSend(NetID netid, const char *data, unsigned int length)
	{
		return m_internal_network->Send(netid, data, length);
	}

	void NetDisconnect(NetID netid)
	{
		m_internal_network->Disconnect(netid);
	}

	void SetSesstionKeyBase(const std::string &session_key_base);
	void SetDBSession(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
		const rmi::Session &systm_session, const rmi::Session &name_session);

	const DBHandler &GetDBHandler() { return m_db_handler; }

	bool SendToCross(const char *data, int length, int cross_func_type = CROSS_FUNCTION_TYPE_COMMON);   // 只做转接用，真正实现接口转移到CrossManager
	bool SendToCrossTransferToGame(int transfer_type, long long param, const char *data, int length);
	bool UserMsgTransmitToCross(const UserID &user_id, int msg_type, const char *data, int length);
	bool UserProtocalTransferToGame(long long uuid, const char *data, int length);	// uuid为0的时候，把协议发给所有服务器的玩家

protected:
	InternalComm();
	~InternalComm();

	DBHandler			m_db_handler;

public:
	INetworkModule		*m_internal_network;
	std::string			m_session_key_base;

	NetID				m_login_server_id;
	NetID				m_golbal_server_id;

	int					m_reconnet_login_handle;
};

#endif

