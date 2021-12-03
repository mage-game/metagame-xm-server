
#include "engineadapter.h"

void EngineAdapter::Init(IClockModule *clock, ITimerModule *timer)
{
	m_clock = clock;
	m_timer = timer;
}

