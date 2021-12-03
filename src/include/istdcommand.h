
#pragma once

#ifndef ISTDCOMMAND_H
#define ISTDCOMMAND_H

#include "imodule.h"

static const char STD_COMMAND_MODULE[] = "StdCommandModule";

class ICmdCallback
{
public:
	ICmdCallback(){}
	virtual ~ICmdCallback(){}

	virtual void OnCmd(const char * cmd)=0;
};

class IStdCommandModule : public IModule
{
public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	注册命令回调对象
	@func			要注册对象
	*/
	virtual void RegisterCallback(ICmdCallback *callback) = 0;
};

#endif
