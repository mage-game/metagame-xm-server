

#pragma once

#ifndef INETWORKMODULE_H
#define INETWORKMODULE_H

#include "imodule.h"

static const char NETWORK_MODULE[] = "NetworkModule";

typedef	unsigned int		NetID;
typedef unsigned int		IP;
typedef unsigned short		Port;

class IEngineNetCallback
{
public:
	virtual ~IEngineNetCallback(){}

	/*
	 ��������acceptʱ��ص��ú���
	 @netid			�����ӵ�netid
	 @ip			Զ�˵�ַ��������
	 @port			Զ�˶˿ں�
	*/
	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)=0;

	/*
	 ���յ�������Ϣʱ�ص��ú���
	 @netid			������Ϣ��Դ��netid
	 @data			��������
	 @length		���ݳ���
	*/
	virtual void OnRecv(NetID netid, const char *data, int length)=0;

	/*
	 ��������Ͽ���ʱ��ص��ú���
	 @netid			�Ͽ����������ӵ�netid
	*/
	virtual void OnDisconnect(NetID netid)=0;

	/*
	 �������첽ConnectAsyn������غ�ص�
	 @result		���ӽ���Ƿ�ɹ�
	 @handle		ConnectAsynʱ���������handle
	 @netid			���ӳɹ�ʱ��netidʱ�������ӵ������id
	 @ip			����ip
	 @port			����port
	*/
	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)=0;
};

class INetworkModule : public IModule
{

public:
	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 ע��������Ϣ�ص�
	 @callback		����ص�
	*/
	virtual void RegisterCallback(IEngineNetCallback * callback)=0;

	/*
	 �����������
	 @port			Ҫ�����Ķ˿ں�
	 @backlog		������İ뿪���ӵ�������
	 @netid_out		out�����������ɹ���ò���Ϊ���������netid��ע�⣬���ټ������ʱʹ��Disconnect������������netid��Ϊ�������
	 @ip_bind		listen ʱ�İ�socket �� ip�����Ϊ0��󶨵�INADDR_ANY
	 @Return		�Ƿ�����ɹ�
	*/
	virtual bool Listen(Port port, int backlog, NetID *netid_out=0, const char *ip_bind=0)=0;

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	virtual bool Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000)=0;

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	virtual bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000)=0;

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	virtual bool ConnectAsyn(const char *ip, Port port, int *handle, unsigned long time_out=3000) = 0;

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	virtual bool ConnectAsyn(IP ip, Port port, int *handle, unsigned long time_out=3000) = 0;

	/*
	 ������Ϣ����ע�⣬�ú����������������Ѿ��ɹ����ͣ����ǽ���Ϣ�ŵ������Ͷ������첽����
	 ���ĳ�����ӶϿ��ص�disconnectʱ���п��ܵ�����Ϣ����ʧ
	 �ú�������Ϣ������˳�����б�֤��
	 @neiid			����Ŀ�������id
	 @data			���͵���Ϣ��
	 @length		��Ϣ������
	 @Return		�Ƿ�ɹ��ŵ������Ͷ�����
	*/
	virtual bool Send(NetID netid, const char *data, unsigned int length)=0;

	/*
	 �Ͽ�ĳ���ӣ����ԶϿ����������׽������ڵ���������
	 ע�⣬�ú�������ʱ��������������Ѿ��Ͽ������ǽ������ӱ��Ϊ���Ͽ�
	 �����Ͽ�����Ϊ����IEngineNetCallback::OnDisconnect��֤��
	 @netid			Ҫ�Ͽ�������id
	*/
	virtual void Disconnect(NetID netid)=0;
};

#endif
