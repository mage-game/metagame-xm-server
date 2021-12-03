#ifndef _INTERNALCOMM_H__
#define _INTERNALCOMM_H__

#include "inetworkmodule.h"
#include "irmimodule.h"
#include <string>

#include "servercommon/serverdef.h"
#include "servercommon/dbhandler.h"

class InternalComm
{
public:
	static InternalComm & Instance()
	{
		static InternalComm ic;
		return ic;
	}

	void Init(INetworkModule *internal_network)
	{
		m_internal_network = internal_network;
	}

	bool NetListen(Port port, int backlog, NetID *netid_out = NULL)
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

	void SetDBSession(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
		const rmi::Session &system_session, const rmi::Session &name_session, const rmi::Session &cross_session);

	const DBHandler &GetDBHandler() { return m_db_handler; }

protected:
	InternalComm() : m_internal_network (NULL) {}
	~InternalComm() {}

	INetworkModule		*m_internal_network;
	DBHandler			m_db_handler;
};

#endif	// _INTERNALCOMM_H__

