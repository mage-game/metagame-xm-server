

#pragma once

#ifndef IINTERFACEMGR_H
#define IINTERFACEMGR_H

class IModule;

class IInterfaceMgr
{
public:
	virtual ~IInterfaceMgr(){};
	/*
	 ע��ģ�鵽IInterfaceMgr��
	 @name		ģ������Ҫ���Ǿ�̬�ַ�����ÿ��IModule��ʵ�ֶ���������һ����̬�ַ����������ַ����׵�ַ����Ϊ��ģ��ı�ʶ��
	 @module	��ע���ģ��
	 @Return	�Ƿ�ע��ɹ�
	*/
	virtual bool RegisterModule(const char* name, IModule *module)=0;

	/*
	 ��ѯģ��
	 @name		ģ��������̬�ַ���������ʹ��ע��ʱ��ľ�̬�ַ���
	 @Return	ģ��ָ�룬��ѯʧ����Ż�0
	*/
	virtual IModule* QueryModule(const char* name)=0;
};

#endif
