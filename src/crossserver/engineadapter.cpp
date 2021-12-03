#include "engineadapter.h"

#include "ilogmodule.h"
#include "inetworkmodule.h"

#include "servercommon/servercommon.h"

EngineAdapter::EngineAdapter()
	: m_config(NULL), m_game(NULL), m_clock(NULL), m_log(NULL), m_timer(NULL)
{

}

EngineAdapter::~EngineAdapter()
{
	
}

void EngineAdapter::Init(ILogModule *log, IConfigModule *config, IGameModule *game, IClockModule *clock, IRMIModule *rmi_module, ITimerModule *timer)
{
	m_log = log;
	m_config = config;
	m_game = game;
	m_clock = clock;
	m_rmi = rmi_module;
	m_timer = timer;
}

