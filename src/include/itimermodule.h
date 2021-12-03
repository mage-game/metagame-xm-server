
#pragma once

#ifndef ITIMERMODULE_H
#define ITIMERMODULE_H

#include "imodule.h"

static const char TIMER_MODULE[] = "TimerModule";

/*
	�ṩ�򵥵Ķ�ʱ������
	
	ע�⣬��ʱ����ʱ�侫����ClockModule������������õ�interval_msС��ClockModuleÿ֡���ʱ�䣬��ͨ
	����ITimerCallback::OnTimer��ѭ��CreateTimer�õ���Ч����ÿinterval_ms��һ��
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
	 �������뼶Timer
	 @interval_ms			��interval_ms����֮�󴥷���ʱ��
	 @callback_obj			�ص����󣬴�����ʱ�����ص�callback_obj->OnTimer(); ֮������callback_obj->Free();
	*/
	virtual void CreateTimer(unsigned int interval_ms, ITimerCallback *callback_obj)=0;

	/*
	 �����뼶Timer
	 @interval_second		��interval_second��֮�󴥷���ʱ��
	 @callback_obj			�ص����󣬴�����ʱ�����ص�callback_obj->OnTimer(); ֮������callback_obj->Free();
	*/
	virtual void CreateTimerSecond(unsigned int interval_second, ITimerCallback *callback_obj)=0;
};

#endif

