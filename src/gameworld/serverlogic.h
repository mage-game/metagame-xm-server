#ifndef __SERVERLOGICMODULE_H__
#define __SERVERLOGICMODULE_H__

#include "imodule.h"
#include "utility/logagent.h"

static const char SERVER_MODULE[] = "ServerModule";

class ServerNetworkCallback;
class ServerInternalNetCallback;
class IClockModule;
class INetworkModule;
class IGameModule;
class IRMIModule;
class ITimerModule;
class World;

class ServerLogic : public IModule
{
	friend class ServerNetworkCallback;
	friend class ServerInternalNetCallback;

public:
	static ServerLogic * GetInstServerLogic();

	ServerLogic(bool check_res_mode, int* check_ret, int total_server, int server_index);
	~ServerLogic();

	virtual int Init();
	virtual int Start();
	virtual int Update();
	virtual int Stop();
	virtual int Release();

	virtual void Free();

	void StopServer();

	int GetFrameElapse() { return m_frame_elapse_time; }
	unsigned long GetPiTime() { return m_pi_time; }
	unsigned long long GetLocalTimeMs() { return m_system_time_ms;}

private:
	LogAgent		m_log;
	
	bool			m_check_res_mode;
	int*			m_check_ret;

	int				m_total_server;
	int				m_server_index;

	IClockModule	*m_clock;
	IGameModule		*m_game;
	INetworkModule	*m_network;
	INetworkModule	*m_internal_network;
	IRMIModule		*m_rmi;
	ITimerModule	*m_timer;

	unsigned long	m_current_frame;									// ��ǰ֡
	unsigned long	m_frame_start_time;									// ��ǰ֡��ʼʱ��
	int				m_frame_elapse_time;								// ��ǰ֡����ʱ��
	unsigned long	m_pi_time;											// ��ȷʱ��
	unsigned long long m_system_time_ms;								// ϵͳ��ǰʱ�䣨���룩

	ServerNetworkCallback *m_network_callback;
	ServerInternalNetCallback *m_internal_network_callback;
	World *m_world;
};

#endif
