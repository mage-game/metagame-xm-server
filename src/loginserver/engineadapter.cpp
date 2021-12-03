
#include "engineadapter.h"

void EngineAdapter::Init(ILogModule *log, IConfigModule *config, IGameModule *game, INetworkModule *network, IClockModule *clock, IRMIModule *rmi_module, ITimerModule *timer)
{
	m_log = log;
	m_config = config;
	m_game = game;
	m_network = network;
	m_clock = clock;
	m_rmi = rmi_module;
	m_timer = timer;
}

