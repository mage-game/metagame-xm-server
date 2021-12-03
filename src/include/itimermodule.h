
#pragma once

#ifndef ITIMERMODULE_H
#define ITIMERMODULE_H

#include "imodule.h"

static const char TIMER_MODULE[] = "TimerModule";

/*
	提供简单的定时器功能
	
	注意，定时器的时间精度由ClockModule决定，如果设置的interval_ms小于ClockModule每帧间隔时间，则通
	过在ITimerCallback::OnTimer中循环CreateTimer得到的效果是每interval_ms调一次
*/

class ITimerCallback
{
public:
	ITimerCallback(){}
	virtual ~ITimerCallback(){}

	virtual void OnTimer()=0;
	virtual void Free()=0;
};

class ITimerModule : public IModule
{
public:
	virtual ~ITimerModule(){}

	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 创建毫秒级Timer
	 @interval_ms			在interval_ms毫秒之后触发定时器
	 @callback_obj			回调对象，触发定时器后会回调callback_obj->OnTimer(); 之后立即callback_obj->Free();
	*/
	virtual void CreateTimer(unsigned int interval_ms, ITimerCallback *callback_obj)=0;

	/*
	 创建秒级Timer
	 @interval_second		在interval_second秒之后触发定时器
	 @callback_obj			回调对象，触发定时器后会回调callback_obj->OnTimer(); 之后立即callback_obj->Free();
	*/
	virtual void CreateTimerSecond(unsigned int interval_second, ITimerCallback *callback_obj)=0;
};

#endif

