

#pragma once

#ifndef IGAMEMODULE_H
#define IGAMEMODULE_H

#include "imodule.h"

static const char GAME_MODULE[] = "GameModule";

class IGameModule : public IModule
{
public:
	enum
	{
		GAME_MAX_PATH = 1024
	};
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 停止整个游戏服务
	*/
	virtual void StopGame()=0;

	/*
	 获取当前工作目录
	 @path		out参数，获取成功则path为当前工作目录
	 @size		path的buffer长度
	*/
	virtual bool GetWorkPath(char* path, unsigned long size)const=0;
};


#endif
