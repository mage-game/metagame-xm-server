

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
	 ֹͣ������Ϸ����
	*/
	virtual void StopGame()=0;

	/*
	 ��ȡ��ǰ����Ŀ¼
	 @path		out��������ȡ�ɹ���pathΪ��ǰ����Ŀ¼
	 @size		path��buffer����
	*/
	virtual bool GetWorkPath(char* path, unsigned long size)const=0;
};


#endif
