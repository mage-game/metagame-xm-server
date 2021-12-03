#ifndef __CROSSSERVERMODULE_H__
#define __CROSSSERVERMODULE_H__

#include <vector>
#include <set>

#include "imodule.h"
#include "utility/logagent.h"
#include "inetworkmodule.h"

#include "crossserver/crossserver.h"

static const char CROSSSERVER_MODULE[] = "CrossServerModule";

class IConfigModule;
class IGameModule;
class ITimerModule;
class IClockModule;
class ServerInternalNetCallback;

class CrossServerModule : public IModule
{	
	friend class ServerInternalNetCallback;

public:
	CrossServerModule();
	~CrossServerModule();

	virtual int Init();
	virtual int Start();
	virtual int Update();
	virtual int Stop();
	virtual int Release();
	virtual void Free();

	void StopServer();

	bool ListenForGlobal();

private:
	bool InitCrossServerConfig(const std::string &config_dir);
	bool ConnectToDB();

	IClockModule				*m_clock;
	IGameModule					*m_game;
	ITimerModule				*m_timer;
	INetworkModule				*m_internal_network;

	ServerInternalNetCallback	*m_internal_network_callback;

	LogAgent					m_log;

	unsigned long				m_current_frame;
	CrossServer					m_cross_server;
};

#endif	// __CROSSSERVERMODULE_H__
