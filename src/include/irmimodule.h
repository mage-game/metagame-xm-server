

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
	启动RMI服务
	@listen_port RMI服务监听端口
	@Return		是否成功
	*/
	virtual bool StartServer(Port listen_port)=0;

	/*
	注册RMI对象
	@obj		RMI对象，注意，注册后该对象由RMI模块调用Free释放
	@Return		注册RMI对象是否成功，RMIObject::__get_obj_id重复是不成功的唯一原因
	*/
	virtual bool Register(rmi::RMIObject *obj) = 0;

	/*
	创建RMI会话
	@ip			远端IP
	@port		远端Port
	@Return		创建成功，则将其指针作为句柄返回，否则返回0，注意，外部不应该使用任何该
				指针进行除了GetXX之外的一切操作
	*/
	virtual rmi::Session CreateSession(const char *ip, Port port) = 0;
	/*
	销毁RMI会话
	注意：CreateSession和CloseSession必须保证在整个RMI模块Update过程中不被调用，即只能在Start创
	建Session，由Stop销毁
	@session	传入由CreateSession返回的句柄
	*/
	virtual void CloseSession(const rmi::Session &session) = 0;

	/*
	RMI调用
	@session	对该session调用RMI
	@module		远端模块名
	@method		远端方法名
	@in_stream	远端调用的输入参数
	@backobj	返回时的调用对象,注意：该对象由RMI模块在返回调用后调用Free销毁
	@remotec_syn	远端是否阻塞调用
	@return		是否成功提交call请求
	*/
	virtual bool Call(const rmi::Session &session, const char *module, const char *method, const TLVSerializer &in_stream, 
					rmi::RMIBackObject *backobj, bool remotec_syn=true, unsigned long timeout=10000)=0;

	/*
	RMI模块启动服务端异步调用，供对于费时操作的RMI使用
	@threadnum	用于异步调用的线程数，设为0则停止异步调用功能
	*/
	virtual void StartAsyncThread(int threadnum)=0;
};

#endif
