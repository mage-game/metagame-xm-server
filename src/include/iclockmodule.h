

#pragma once

#ifndef ICLOCKMODULE_H
#define ICLOCKMODULE_H

#include "imodule.h"
#include <time.h>

static const char CLOCK_MODULE[] = "ClockModule";

class IClockModule : public IModule
{
public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 ��ȡ��ǰ֡
	 @Return	��ǰ֡��
	*/
	virtual unsigned long GetCurrentFrame()const=0;

	/*
	��ȡ��ǰ֡�Ŀ�ʼʱ��
	@Return		��ǰ֡��ʼʱ��
	*/
	virtual unsigned long GetFrameTime()const=0;

	/*
	��ȡ��ǰ֡��
	@Return		��ǰ֡��
	*/
	virtual unsigned long GetFPS()const=0;

	/*
	���õ�ǰ֡��
	@fps		��ǰ֡��
	*/
	virtual void SetFPS(unsigned long fps)=0;

	/*
	��ȡ��1970�굽���ڵ�����
	*/
	virtual time_t Time() const=0;

	/*
	��ȡÿ�첻ͬ��������ʾ
	*/
	virtual unsigned int DayID() const=0;

	/*
	��ȡ��ǰʱ��ṹ
	*/
	virtual const tm *LocalTime() const=0;

	/*
	��ȡ��ǰʱ���ַ���(ģ��������CLOCK_MODULE�󣬿���ֻ��ȡһ�Σ�IClockModule��֤���ص�const char *��ַ����)
	*/
	virtual const char *AscTime() const=0;

	/*
	������һ����(Сʱ���졢���ڡ���)��ָ��ʱ��(��)���뵱ǰʱ�������
	@sceond		�� [0,59]
	@minute		�� [0,59]
	@hour		ʱ [0,23]
	@wday		���ڼ� ���������� [0,6]
	@date		���ڣ��� [1,31]

	@Return		�������
	*/
	virtual int NextMinuteInterval(int sceond) const=0;
	virtual int NextHourInterval(int minute, int second) const=0;
	virtual int NextDayInterval(int hour, int minute, int second) const=0;
	virtual int NextWeekInterval(int wday, int hour, int minute, int second) const=0;
	virtual int NextMouthInterval(int date, int hour, int minute, int second) const=0;

	/*
	����t����ʱ��
	@t			Ҫ�����ʱ���

	@Return		�������������tΪδ��ʱ���򷵻�ֵ����0������ǹ�ȥʱ����С��0
	*/
	virtual int LocalTimeInterval(tm *t) const=0;
};

#endif
