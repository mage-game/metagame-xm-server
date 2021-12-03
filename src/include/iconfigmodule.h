

#pragma once

#ifndef ICONFIGMODULE_H
#define ICONFIGMODULE_H

#include "imodule.h"
#include "common/estring.h"
#include <sstream>

static const char CONFIG_MODULE[] = "ConfigModule";

class IConfigModule : public IModule
{
public:

	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;
	virtual void Free()=0;

	/*
	 ��ȡXML·���ϵ�ֵ
	 @path		XML·��
	 @val		out��������ȡ��ֵ
	 @Return	�Ƿ�ɹ�ȡֵ
	*/
	virtual bool get_item(const char* path, estring* val)const=0;

	/*
	����XML·���ϵ�ֵ
	@path		XML·��
	@val		ֵ
	@Return		�Ƿ�ɹ�����
	*/
	virtual bool set_item(const char* path, const estring& val)=0;

	/*	
	 ��ȡXML·���ϵ�ֵ�����Ŀ��·�����ڣ��򽫸�ֵ��ȡ��result�У�����true�� 
	 ���Ŀ�겻���ڣ��򴴽�Ŀ�꣬��ֵΪinit_value��ͬʱ������result�У�����true��
	 ���Ŀ���޷������򷵻�false��result������Ϊinit_value�� 
	 @path			XML·��
	 @result		out��������ȡ��ֵ
	 @init_value	��ȡ���ɹ�ʱ�����ø�ֵ��������ֵ����
	 @Return		�����Ƿ�ɹ����
	*/
	template<class T>
	bool SyncValue(const char* path, T* result, const T& init_value)
	{
		std::string result_str;
		result_str.reserve(1024);
		if (get_item(path, &result_str))
		{
			std::istringstream stream;
			stream.str(result_str);
			if (stream>>*result)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			*result = init_value;
			std::ostringstream stream;
			if (stream << init_value)
			{
				return set_item(path, stream.str());
			}
			else
			{
				return false;
			}
		}
	}

	/*
	����ǰ����д���ļ�
	@Return		�Ƿ�ɹ�д���ļ�
	*/
	virtual bool write()=0;
};

#endif
