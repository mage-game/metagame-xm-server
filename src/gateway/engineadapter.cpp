
#include "engineadapter.h"

void EngineAdapter::Init(ILogModule *log, IConfigModule *config, IGameModule *game, INetworkModule *network, IClockModule *clock, ITimerModule *time)
{
	m_log = log;
	m_config = config;
	m_game = game;
	m_network = network;
	m_clock = clock;
	m_time = time;
}

