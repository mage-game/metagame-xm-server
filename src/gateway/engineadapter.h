
#ifndef ENGINEADAPTER_H
#define ENGINEADAPTER_H

#include "iclockmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "inetworkmodule.h"
#include "itimermodule.h"

class ILogModule;
class IScriptModule;

class EngineAdapter
{
public:
	static EngineAdapter &Instance()
	{
		static EngineAdapter ea;
		return ea;
	}

	void Init(ILogModule *log, IConfigModule *config, IGameModule *game, INetworkModule *network, IClockModule *clock, ITimerModule *time);

	unsigned long GetGameTime() { return m_clock->GetFrameTime(); }
	unsigned int DayID() { return m_clock->DayID(); }
	time_t Time() { return m_clock->Time(); }
	void StopGame() { m_game->StopGame(); }
	bool GetWorkPath(char* path, unsigned long size)const { return m_game->GetWorkPath(path, size); }

	template<class T>
	bool ConfigSyncValue(const char* path, T* result, const T& init_value)
	{
		return m_config->SyncValue(path, result, init_value);
	}

	bool NetListen(Port port, int backlog, NetID *netid_out=0)
	{
		return m_network->Listen(port, backlog, netid_out);
	}

	bool NetConnect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000)
	{
		return m_network->Connect(ip, port, netid_out, time_out);
	}

	bool NetSend(NetID netid, const char *data, unsigned int length)
	{
		return m_network->Send(netid, data, length);
	}

	void NetDisconnect(NetID netid)
	{
		m_network->Disconnect(netid);
	}

	void CreateTimerSecond(unsigned int interval_s, ITimerCallback *callback_obj)
	{
		m_time->CreateTimerSecond(interval_s, callback_obj);
	}

	ILogModule * GetLogModule()
	{
		return m_log;
	}

private:
	EngineAdapter(){}

	IConfigModule		*m_config;
	IGameModule			*m_game;
	INetworkModule		*m_network;
	IClockModule		*m_clock;
	ILogModule			*m_log;
	ITimerModule		*m_time;
};

#endif

