#ifndef __ENGINEADAPTER_H__
#define __ENGINEADAPTER_H__

#include "iclockmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "inetworkmodule.h"
#include "irmimodule.h"
#include "itimermodule.h"

#include "servercommon/serverdef.h"

class ILogModule;
class INetworkModule;

class EngineAdapter
{
public:
	static EngineAdapter & Instance()
	{
		static EngineAdapter ea;
		return ea;
	}

	void Init(ILogModule *log, IConfigModule *config, IGameModule *game, IClockModule *clock, IRMIModule *rmi_module, ITimerModule *timer);

	unsigned long GetGameTime() { return m_clock->GetFrameTime(); }
	unsigned int DayID() { return m_clock->DayID(); }
	time_t Time() { return m_clock->Time(); }
	const tm * LocalTime() { return m_clock->LocalTime(); }
	const char *AscTime() { return m_clock->AscTime(); }

	int NextMinuteInterval(int second) { return m_clock->NextMinuteInterval(second); }
	int NextHourInterval(int minute, int second) { return m_clock->NextHourInterval(minute, second); }
	int NextDayInterval(int hour, int minute, int second) { return m_clock->NextDayInterval(hour, minute, second); }
	int NextWeekInterval(int wday, int hour, int minute, int second) { return m_clock->NextWeekInterval(wday, hour, minute, second); }
	int NextMouthInterval(int date, int hour, int minute, int second) { return m_clock->NextMouthInterval(date, hour, minute, second); }

	void StopGame() { m_game->StopGame(); }
	bool GetWorkPath(char* path, unsigned long size) const { return m_game->GetWorkPath(path, size); }

	template<class T>
	bool ConfigSyncValue(const char* path, T* result, const T& init_value)
	{
		return m_config->SyncValue(path, result, init_value);
	}

	ILogModule * GetLogModule()
	{
		return m_log;
	}

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
		m_timer->CreateTimer(interval_ms, callback_obj);
	}

	void CreateTimerSecond(unsigned int interval_s, ITimerCallback *callback_obj)
	{
		m_timer->CreateTimerSecond(interval_s, callback_obj);
	}	

private:
	EngineAdapter();
	~EngineAdapter();

	IConfigModule		*m_config;
	IGameModule			*m_game;
	IClockModule		*m_clock;
	ILogModule			*m_log;
	IRMIModule			*m_rmi;
	ITimerModule		*m_timer;
};

#endif	// __ENGINEADAPTER_H__

