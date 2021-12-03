
#pragma once

#ifndef IMODULE_H
#define IMODULE_H

/*
	Game�����̽���֤��	���ȳɹ����Init��Module��������Release��
						���ȳɹ����Start��Module��������Stop��
*/
#include "iinterfacemgr.h"	//ÿ��ģ�鶼��ʹ��


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

	/* �˺����ͷ��Լ���ռ�õĿռ䣬���ô˺�����Game������ʹ�ô�ģ���ָ�� */
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
	�˺����ڼ�ģ����������Ĵ���
	EXPECT_ON		���ģ���ѱ�ע�ᣬ��ȴ��䵽��ָ��״̬������Fail���������������Ϊ�ɹ������򷵻�Pending
	DEPEND_ON		ָ��ģ����뱻ע�ᣬ���Ҵﵽָ��״̬�����ָ��ģ��δע�ᣬ��Fail���򷵻�Fail�����򷵻�Pending��

	ע�⣺�û���Ҫ�Լ�����ѭ�������ĳ���
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
