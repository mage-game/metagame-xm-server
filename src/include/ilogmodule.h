

#pragma once

#ifndef ILOGMODULE_H
#define ILOGMODULE_H

#include "imodule.h"
#include <stdarg.h>

static const char LOG_MODULE[] = "LogModule";

#define  MAX_LOG_LENGTH			15360

enum
{
	LL_CRITICAL=0,
	LL_ERROR,
	LL_MAINTANCE,
	LL_WARNING,
	LL_INFO,
	LL_DEBUG,
	LL_SPECIAL,
	LL_COUNT,
};

class ILogModule : public IModule
{
public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	enum NEW_FILE_INTERVAL_STRATEGY
	{
		NFIS_HALF_HOUR=-1,
		NFIS_HOUR=-2,
		NFIS_HALF_DAY=-3,
		NFIS_DAY=-4,
	};
	/*
	 注册日志分类
	 @cata			日志分类名
	 @output		输出目标，0为默认，"1"为标准输出，"0"为nul输出，其他则为文件输出
	 @new_file_interval 大于0表示，间隔多少秒开新文件(旧文件加上时间后缀)，0则不开新文件，如果是NEW_FILE_INTERVAL_STRATEGY的值则按策略新开文件
	 @Return		分类ID，用于printf,vprintf等函数
	*/
	virtual int RegisterCatagory(const char *cata, const char *output=0, int new_file_interval=0)=0;

	/*
	 输出日志
	 @log_level		日志等级，可以为LL_XXX中的其中一个
	 @log_catagory	日志分类ID
	 @fmt			输出格式，用法参考标准c函数printf
	 @...			可变参数，与fmt中的格式对应
	*/
	virtual void printf(int log_level, int log_catagory, const char *fmt, ...)=0;

	/*
	 输出日志
	 @log_level		日志等级，可以为LL_XXX中的其中一个
	 @log_catagory	日志分类ID
	 @fmt			输出格式，用法参考printf
	 @argp			参数列表首地址，va_list类型，用法参考标准c函数vprintf
	*/
	virtual void vprintf(int log_level, int log_catagory, const char *fmt, va_list argp)=0;

	/*
	 输出日志
	 @log_level		日志等级，可以为LL_XXX中的其中一个
	 @log_catagory	日志分类ID
	 @str			日志内容
	*/
	virtual void print(int log_level, int log_catagory, const char *str)=0;

	/*
	 设置输出日志等级，只有日志等级高于输出等级的日志会被输出
	 @level			设置的等级
	*/
	virtual void SetLogLevel(int level)=0;

	/*
	 获取日志输出等级
	 @Return		日志输出等级
	*/
	virtual int GetLogLevel()const=0;
	

};

#endif
