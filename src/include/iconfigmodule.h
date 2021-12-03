

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
	 获取XML路径上的值
	 @path		XML路径
	 @val		out参数，获取的值
	 @Return	是否成功取值
	*/
	virtual bool get_item(const char* path, estring* val)const=0;

	/*
	设置XML路径上的值
	@path		XML路径
	@val		值
	@Return		是否成功设置
	*/
	virtual bool set_item(const char* path, const estring& val)=0;

	/*	
	 获取XML路径上的值。如果目标路径存在，则将该值读取到result中，返回true。 
	 如果目标不存在，则创建目标，赋值为init_value，同时保存至result中，返回true。
	 如果目标无法创建则返回false，result内容设为init_value。 
	 @path			XML路径
	 @result		out参数，获取的值
	 @init_value	获取不成功时则设置该值，并将该值返回
	 @Return		操作是否成功完成
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
	将当前配置写到文件
	@Return		是否成功写到文件
	*/
	virtual bool write()=0;
};

#endif
