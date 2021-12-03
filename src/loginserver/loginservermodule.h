#pragma once

#ifndef SERVERLOGICMODULE_H
#define SERVERLOGICMODULE_H

#include <vector>

#include "imodule.h"
#include "utility/logagent.h"
#include "irmimodule.h"
#include "inetworkmodule.h"

static const char LOGIN_MODULE[] = "LoginServerModule";

class ServerNetworkCallback;
class ServerInternalNetCallback;
class IClockModule;
class IGameModule;

class LoginServerModule : public IModule
{
	friend class ServerNetworkCallback;
	friend class ServerInternalNetCallback;

public:
	LoginServerModule();
	~LoginServerModule();

	virtual int Init();
	virtual int Start();
	virtual int Update();
	virtual int Stop();
	virtual int Release();

	virtual void Free();

	void StopServer();

private:
	bool ConnectToDB();
	bool ListenForGameServer();
	bool ListenForGateWay();
	bool ListenForUser();

	LogAgent		m_log;

	IClockModule	*m_clock;
	IGameModule		*m_game;
	INetworkModule	*m_network;
	INetworkModule	*m_internal_network;
	IRMIModule		*m_rmi;

	unsigned long	m_current_time;

	Port			m_gs_listen_port;
	Port			m_gateway_listen_port;

	ServerNetworkCallback *m_network_callback;
	ServerInternalNetCallback *m_internal_network_callback;

	std::vector<rmi::Session>	m_db_rmi_session_list;

protected:

	struct ConnectUser 
	{
		ConnectUser() : netid(-1), accept_time(0), last_active_time(0), ip(0) {}

		void Reset()
		{
			netid = -1;
			accept_time = 0;
			last_active_time = 0;
			ip = 0;
		}

		NetID			netid;
		unsigned long	accept_time;
		unsigned long	last_active_time;
		IP				ip;
	};

	typedef std::vector<ConnectUser>	ConnectUserList;
	ConnectUserList		m_connect_list;

	void EraseTimeOut();

	bool			m_check_timeout;
	unsigned long	m_last_check_timeout;
	unsigned long	m_check_timeout_interval;
	unsigned long	m_heart_time;
	unsigned long	m_max_server_time;
};

#endif
