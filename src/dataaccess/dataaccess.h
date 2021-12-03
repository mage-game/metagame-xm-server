#pragma once
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <vector>
#include "imodule.h"
#include "utility/logagent.h"

static const char DATA_ACCESS_SERVER[] = "DataAccessServer";

class DataAccessNetworkCallback;
class DataAccessCmd;
class INetworkModule;
class IGameModule;
class IRMIModule;
class IConnFactroy;

class DataAccess : public IModule
{
	friend class DataAccessNetworkCallback;
	friend class DataAccessCmd;
public:
	DataAccess();
	~DataAccess();

	virtual int Init();
	virtual int Start();
	virtual int Update();
	virtual int Stop();
	virtual int Release();

	virtual void Free();

	void StopServer();

	bool CalcTableSizeIsTooLong();

private:
	LogAgent m_log;

	IGameModule		*m_game;
	INetworkModule	*m_network;
	IRMIModule		*m_rmi;

	DataAccessNetworkCallback	*m_network_callback;
	IConnFactroy				*m_conn_factroy;
	DataAccessCmd				*m_cmd_callback;

	unsigned short m_rmi_port;
};



#endif


