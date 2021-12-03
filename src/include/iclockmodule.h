

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
	 获取当前帧
	 @Return	当前帧数
	*/
	virtual unsigned long GetCurrentFrame()const=0;

	/*
	获取当前帧的开始时间
	@Return		当前帧开始时间
	*/
	virtual unsigned long GetFrameTime()const=0;

	/*
	获取当前帧率
	@Return		当前帧率
	*/
	virtual unsigned long GetFPS()const=0;

	/*
	设置当前帧率
	@fps		当前帧率
	*/
	virtual void SetFPS(unsigned long fps)=0;

	/*
	获取从1970年到现在的秒数
	*/
	virtual time_t Time() const=0;

	/*
	获取每天不同的天数标示
	*/
	virtual unsigned int DayID() const=0;

	/*
	获取当前时间结构
	*/
	virtual const tm *LocalTime() const=0;

	/*
	获取当前时间字符串(模块依赖了CLOCK_MODULE后，可以只获取一次，IClockModule保证返回的const char *地址不变)
	*/
	virtual const char *AscTime() const=0;

	/*
	计算下一分钟(小时、天、星期、月)内指定时刻(秒)距离当前时间的秒数
	@sceond		秒 [0,59]
	@minute		分 [0,59]
	@hour		时 [0,23]
	@wday		星期几 从星期天起 [0,6]
	@date		日期，号 [1,31]

	@Return		距今秒数
	*/
	virtual int NextMinuteInterval(int sceond) const=0;
	virtual int NextHourInterval(int minute, int second) const=0;
	virtual int NextDayInterval(int hour, int minute, int second) const=0;
	virtual int NextWeekInterval(int wday, int hour, int minute, int second) const=0;
	virtual int NextMouthInterval(int date, int hour, int minute, int second) const=0;

	/*
	计算t距今的时间
	@t			要计算的时间点

	@Return		距今的秒数，如果t为未来时间则返回值大于0，如果是过去时间则小于0
	*/
	virtual int LocalTimeInterval(tm *t) const=0;
};

#endif
