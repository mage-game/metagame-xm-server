
#pragma once

#ifndef IMODULE_H
#define IMODULE_H

/*
	Game的流程将保证，	最先成功完成Init的Module，最后调用Release。
						最先成功完成Start的Module，最后调用Stop。
*/
#include "iinterfacemgr.h"	//每个模块都会使用


class IModule
{
	friend class Game;
public:
	enum
	{
		Succeed = 0,
		Fail,
		Pending,
	};
	enum
	{
		ST_Initing,
		ST_Inited,
		ST_Starting,
		ST_Started,
		ST_Updating,
		ST_Updated,
		ST_Stopping,
		ST_Stopped,
		ST_Releasing,
		ST_Released,
		ST_Fail,
	};
	IModule()
		:m_interface_mgr(0), m_module_state(ST_Released)
	{}
	
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;

	/* 此函数释放自己所占用的空间，调用此函数后，Game将不再使用此模块的指针 */
	virtual void Free()=0;

	int GetState()const{ return m_module_state;}
protected:
	virtual ~IModule(){}
	IInterfaceMgr* Interface(){ return m_interface_mgr; }
private:
	IInterfaceMgr* m_interface_mgr;
	int m_module_state;
};


/*
	此宏用于简化模块依赖定义的代码
	EXPECT_ON		如果模块已被注册，则等待其到达指定状态，或者Fail，这两种情况均视为成功。否则返回Pending
	DEPEND_ON		指定模块必须被注册，并且达到指定状态。如果指定模块未注册，或Fail，则返回Fail，否则返回Pending。

	注意：用户需要自己避免循环依赖的出现
*/
#define _EXPECT_ON(module_name, ws)\
{\
	int state = GetState();\
	int wait_state = -1;\
	switch(state)\
	{\
	case ST_Initing:\
	case ST_Released:\
		wait_state = ST_Inited;\
		break;\
	case ST_Inited:\
	case ST_Starting:\
		wait_state = ST_Started;\
		break;\
	case ST_Started:\
	case ST_Updating:\
		wait_state = ST_Updated;\
		break;\
	case ST_Updated:\
	case ST_Stopping:\
		wait_state = ST_Stopped;\
		break;\
	case ST_Stopped:\
	case ST_Releasing:\
		wait_state = ST_Released;\
		break;\
	default:\
		break;\
	}\
	if ( wait_state  == ws)\
	{\
		IModule* module = Interface()->QueryModule(module_name);\
		if ( module != 0 && ( module->GetState() != wait_state && module->GetState() != ST_Fail ) )\
		{\
			return IModule::Pending;\
		}\
	}\
}

#define _DEPEND_ON(module_name, ws)\
{\
	int state = GetState();\
	int wait_state = -1;\
	switch(state)\
	{\
	case ST_Initing:\
	case ST_Released:\
		wait_state = ST_Inited;\
		break;\
	case ST_Inited:\
	case ST_Starting:\
		wait_state = ST_Started;\
		break;\
	case ST_Started:\
	case ST_Updating:\
		wait_state = ST_Updated;\
		break;\
	case ST_Updated:\
	case ST_Stopping:\
		wait_state = ST_Stopped;\
		break;\
	case ST_Stopped:\
	case ST_Releasing:\
		wait_state = ST_Released;\
		break;\
	default:\
		break;\
	}\
	if ( wait_state == ws )\
	{\
		IModule* module = Interface()->QueryModule(module_name);\
		if ( module != 0 )\
		{\
			if ( module->GetState() == ST_Fail )\
			{\
				return IModule::Fail;\
			}\
			else if ( module->GetState() != wait_state )\
			{\
				return IModule::Pending;\
			}\
		}\
		else\
		{\
			return IModule::Fail;\
		}\
	}\
}

#define DEPEND_ON_INIT(module_name) _DEPEND_ON(module_name, ST_Inited)
#define DEPEND_ON_START(module_name) _DEPEND_ON(module_name, ST_Started)
#define EXPECT_ON_INIT(module_name) _EXPECT_ON(module_name, ST_Inited)
#define EXPECT_ON_START(module_name) _EXPECT_ON(module_name, ST_Started)

#define MAX_MODULE_NAME_LEN		32

#endif
