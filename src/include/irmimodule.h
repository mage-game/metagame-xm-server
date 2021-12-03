

#pragma once

#ifndef IRMIMODULE_H
#define IRMIMODULE_H

#include "common/tlvprotocol.h"
#include "imodule.h"

static const char RMI_MODULE[] = "RMIModule";

class IRMIModule;

typedef	unsigned int		NetID;

namespace rmi
{
	class RMIObject;
	class RMIBackObject;

	struct Session 
	{
		Session():handle(0), rmi_module(0), netid(-1){}
		void *handle;
		IRMIModule *rmi_module;
		NetID netid;
	};
}
typedef unsigned short Port;



class IRMIModule : public IModule
{
public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	����RMI����
	@listen_port RMI��������˿�
	@Return		�Ƿ�ɹ�
	*/
	virtual bool StartServer(Port listen_port)=0;

	/*
	ע��RMI����
	@obj		RMI����ע�⣬ע���ö�����RMIģ�����Free�ͷ�
	@Return		ע��RMI�����Ƿ�ɹ���RMIObject::__get_obj_id�ظ��ǲ��ɹ���Ψһԭ��
	*/
	virtual bool Register(rmi::RMIObject *obj) = 0;

	/*
	����RMI�Ự
	@ip			Զ��IP
	@port		Զ��Port
	@Return		�����ɹ�������ָ����Ϊ������أ����򷵻�0��ע�⣬�ⲿ��Ӧ��ʹ���κθ�
				ָ����г���GetXX֮���һ�в���
	*/
	virtual rmi::Session CreateSession(const char *ip, Port port) = 0;
	/*
	����RMI�Ự
	ע�⣺CreateSession��CloseSession���뱣֤������RMIģ��Update�����в������ã���ֻ����Start��
	��Session����Stop����
	@session	������CreateSession���صľ��
	*/
	virtual void CloseSession(const rmi::Session &session) = 0;

	/*
	RMI����
	@session	�Ը�session����RMI
	@module		Զ��ģ����
	@method		Զ�˷�����
	@in_stream	Զ�˵��õ��������
	@backobj	����ʱ�ĵ��ö���,ע�⣺�ö�����RMIģ���ڷ��ص��ú����Free����
	@remotec_syn	Զ���Ƿ���������
	@return		�Ƿ�ɹ��ύcall����
	*/
	virtual bool Call(const rmi::Session &session, const char *module, const char *method, const TLVSerializer &in_stream, 
					rmi::RMIBackObject *backobj, bool remotec_syn=true, unsigned long timeout=10000)=0;

	/*
	RMIģ������������첽���ã������ڷ�ʱ������RMIʹ��
	@threadnum	�����첽���õ��߳�������Ϊ0��ֹͣ�첽���ù���
	*/
	virtual void StartAsyncThread(int threadnum)=0;
};

#endif
