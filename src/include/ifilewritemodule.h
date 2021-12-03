
#pragma once

#ifndef IFILEWRITEMODULE_H
#define IFILEWRITEMODULE_H

#include "imodule.h"

static const char FILE_WRITE_MODULE[] = "FileWriteModule";

/*
	提供简单的异步写文件的功能
*/


class IFileWriteModule : public IModule
{
public:
	virtual ~IFileWriteModule(){}

	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 异步写文件
	 @path			路径( 以'\0'结束的字符串 )
	 @data			数据
	 @length		数据长度
	 @is_append		是否追加数据，为true则如果path存在则将data追加到原文件末尾，false则文件存在则长度清零
	 @Return		是否成功提交写请求
	*/
	virtual bool Write(const char *path, const char *data, int length, bool is_append)=0;
};

#endif

