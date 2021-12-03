
#ifndef ENGINEADAPTER_H
#define ENGINEADAPTER_H

#include "iclockmodule.h"
#include "itimermodule.h"

class EngineAdapter
{
public:
	static EngineAdapter &Instance()
	{
		static EngineAdapter ea;
		return ea;
	}

	void Init(IClockModule *clock, ITimerModule *timer);

	unsigned long GetGameTime() { return m_clock->GetFrameTime(); }
	unsigned int DayID() { return m_clock->DayID(); }
	time_t Time() { return m_clock->Time(); }

	void CreateTimerSecond(unsigned int interval_s, ITimerCallback *callback_obj)
	{
		m_timer->CreateTimerSecond(interval_s, callback_obj);
	}
private:
	EngineAdapter(){}

	IClockModule		*m_clock;
	ITimerModule		*m_timer;
};

#endif

