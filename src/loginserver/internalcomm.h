
#ifndef INTERNALCOMM_H
#define INTERNALCOMM_H

#include "inetworkmodule.h"
#include "irmimodule.h"
#include <string>

#include "servercommon/dbhandler.h"

class InternalComm
{
public:
	static InternalComm &Instance()
	{
		static InternalComm ic;
		return ic;
	}

	void Init(INetworkModule *internal_network, const std::string &session_key_base);

	bool NetListen(Port port, int backlog, NetID *netid_out=0)
	{
		return m_internal_network->Listen(port, backlog, netid_out);
	}

	bool NetConnect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000)
	{
		return m_internal_network->Connect(ip, port, netid_out, time_out);
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
		const rmi::Session &systm_session, const rmi::Session &name_session);

	const DBHandler &GetDBHandler() { return m_db_handler; }

protected:
	InternalComm(){};

	DBHandler m_db_handler;

public:
	INetworkModule		*m_internal_network;
	std::string			m_session_key_base;
};

#endif



