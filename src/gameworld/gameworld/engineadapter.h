
#ifndef ENGINEADAPTER_H
#define ENGINEADAPTER_H

#include "iclockmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "irmimodule.h"
#include "itimermodule.h"
#include "ifilewritemodule.h"

#include "gamedef.h"

class ILogModule;
class IScriptModule;
class INetworkModule;

class EngineAdapter
{
public:
	static EngineAdapter &Instance()
	{
		static EngineAdapter ea;
		return ea;
	}

	void Init(ILogModule *log, 
		IConfigModule *config, 
		IGameModule *game, 
		INetworkModule *network, 
		IClockModule *clock, 
		IRMIModule *rmi_module,
		ITimerModule *timer,
		IFileWriteModule *filewrite);

	unsigned long GetGameTime() { return m_clock->GetFrameTime(); }
	unsigned int DayID() { return m_clock->DayID(); }
	time_t Time() { return m_clock->Time(); }
	const tm *LocalTime() { return m_clock->LocalTime(); }
	const char *AscTime() { return m_clock->AscTime(); }
 
	int NextMinuteInterval(int second) { return m_clock->NextMinuteInterval(second); }
	int NextHourInterval(int minute, int second) { return m_clock->NextHourInterval(minute, second); }
	int NextDayInterval(int hour, int minute, int second) { return m_clock->NextDayInterval(hour, minute, second); }
	int NextWeekInterval(int wday, int hour, int minute, int second) { return m_clock->NextWeekInterval(wday, hour, minute, second); }
	int NextMouthInterval(int date, int hour, int minute, int second) { return m_clock->NextMouthInterval(date, hour, minute, second); }

	void StopGame() { m_game->StopGame(); }
	bool GetWorkPath(char* path, unsigned long size)const { return m_game->GetWorkPath(path, size); }

	template<class T>
	bool ConfigSyncValue(const char* path, T* result, const T& init_value)
	{
		return m_config->SyncValue(path, result, init_value);
	}

	bool NetSend(const GSNetID &netid, const char *data, unsigned int length);
	bool NetMulSend(GSNetID *netidlist, int num, const char *data, unsigned int length);
	bool NetSendAll(NetID gateway_netid, const char *data, unsigned int length);
	void NetDisconnect(const GSNetID &netid, UserID user_id, const GameName role_name, int scene_id, const char *reason);
	void NetHasCheck(const GSNetID &gsnetid);

	bool RMIObjRegister(rmi::RMIObject *obj)
	{
		return m_rmi->Register(obj);
	}

	rmi::Session RMICreateSession(const char *ip, Port port)
	{
		return m_rmi->CreateSession(ip, port);
	}

	void RMICloseSession(const rmi::Session &session)
	{
		m_rmi->CloseSession(session);
	}

	void RMIStartAsyncThread(int threadnum)
	{
		m_rmi->StartAsyncThread(threadnum);
	}

	void CreateTimer(unsigned int interval_ms, ITimerCallback *callback_obj)
	{
		m_time->CreateTimer(interval_ms, callback_obj);
	}

	void CreateTimerSecond(unsigned int interval_s, ITimerCallback *callback_obj)
	{
		m_time->CreateTimerSecond(interval_s, callback_obj);
	}

	bool AsynFileWrite(const char *path, const char *data, int length, bool is_append)
	{
		return m_filewrite->Write(path, data, length, is_append);
	}


public:
	int					m_server_index;
	int					m_game_thread;
private:
	EngineAdapter();
	~EngineAdapter();

	IConfigModule		*m_config;
	IGameModule			*m_game;
	INetworkModule		*m_network;
	IClockModule		*m_clock;
	ILogModule			*m_log;
	IRMIModule			*m_rmi;
	ITimerModule		*m_time;
	IFileWriteModule	*m_filewrite;

	char				*m_buffer;	// 用于NetSend是组包
};

#endif

