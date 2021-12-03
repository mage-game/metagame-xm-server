

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
	 ע����־����
	 @cata			��־������
	 @output		���Ŀ�꣬0ΪĬ�ϣ�"1"Ϊ��׼�����"0"Ϊnul�����������Ϊ�ļ����
	 @new_file_interval ����0��ʾ����������뿪���ļ�(���ļ�����ʱ���׺)��0�򲻿����ļ��������NEW_FILE_INTERVAL_STRATEGY��ֵ�򰴲����¿��ļ�
	 @Return		����ID������printf,vprintf�Ⱥ���
	*/
	virtual int RegisterCatagory(const char *cata, const char *output=0, int new_file_interval=0)=0;

	/*
	 �����־
	 @log_level		��־�ȼ�������ΪLL_XXX�е�����һ��
	 @log_catagory	��־����ID
	 @fmt			�����ʽ���÷��ο���׼c����printf
	 @...			�ɱ��������fmt�еĸ�ʽ��Ӧ
	*/
	virtual void printf(int log_level, int log_catagory, const char *fmt, ...)=0;

	/*
	 �����־
	 @log_level		��־�ȼ�������ΪLL_XXX�е�����һ��
	 @log_catagory	��־����ID
	 @fmt			�����ʽ���÷��ο�printf
	 @argp			�����б��׵�ַ��va_list���ͣ��÷��ο���׼c����vprintf
	*/
	virtual void vprintf(int log_level, int log_catagory, const char *fmt, va_list argp)=0;

	/*
	 �����־
	 @log_level		��־�ȼ�������ΪLL_XXX�е�����һ��
	 @log_catagory	��־����ID
	 @str			��־����
	*/
	virtual void print(int log_level, int log_catagory, const char *str)=0;

	/*
	 ���������־�ȼ���ֻ����־�ȼ���������ȼ�����־�ᱻ���
	 @level			���õĵȼ�
	*/
	virtual void SetLogLevel(int level)=0;

	/*
	 ��ȡ��־����ȼ�
	 @Return		��־����ȼ�
	*/
	virtual int GetLogLevel()const=0;
	

};

#endif
