

#pragma once

#ifndef IINTERFACEMGR_H
#define IINTERFACEMGR_H

class IModule;

class IInterfaceMgr
{
public:
	virtual ~IInterfaceMgr(){};
	/*
	 注册模块到IInterfaceMgr中
	 @name		模块名，要求是静态字符串，每个IModule的实现都必须声明一个静态字符串名，以字符串首地址来作为该模块的标识符
	 @module	被注册的模块
	 @Return	是否注册成功
	*/
	virtual bool RegisterModule(const char* name, IModule *module)=0;

	/*
	 查询模块
	 @name		模块名，静态字符串，必须使用注册时候的静态字符串
	 @Return	模块指针，查询失败则放回0
	*/
	virtual IModule* QueryModule(const char* name)=0;
};

#endif
