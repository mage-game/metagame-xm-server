
#ifndef INTERNALCOMM_H
#define INTERNALCOMM_H

#include "inetworkmodule.h"
#include <string>

class InternalComm
{
public:
	static InternalComm &Instance()
	{
		static InternalComm ic;
		return ic;
	}

	void Init(INetworkModule *internal_network);

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
protected:
	InternalComm(){};
public:
	INetworkModule		*m_internal_network;
};

#endif



