
#pragma once

#ifndef IFILEWRITEMODULE_H
#define IFILEWRITEMODULE_H

#include "imodule.h"

static const char FILE_WRITE_MODULE[] = "FileWriteModule";

/*
	�ṩ�򵥵��첽д�ļ��Ĺ���
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
	 �첽д�ļ�
	 @path			·��( ��'\0'�������ַ��� )
	 @data			����
	 @length		���ݳ���
	 @is_append		�Ƿ�׷�����ݣ�Ϊtrue�����path������data׷�ӵ�ԭ�ļ�ĩβ��false���ļ������򳤶�����
	 @Return		�Ƿ�ɹ��ύд����
	*/
	virtual bool Write(const char *path, const char *data, int length, bool is_append)=0;
};

#endif

