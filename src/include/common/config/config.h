#ifndef BASECONFIG_H
#define BASECONFIG_H

/*

	通用 Config 基类，采用类似文件系统的路径访问方式，允许派生类自由扩展实际的访问操作。
	本Config仅仅做路径分析，并按照分析的顺序调用访问函数。
*/

#include <string>
#include <sstream>
#include <vector>

class IConfig
{
public:
	virtual ~IConfig(){}

	/*
		path:	/abc/aaa/acd@attribute
				../aaa/acd@attribute
	*/
	bool get_item(const char* path, std::string* val)const;
	bool set_item(const char* path, const std::string& val);

	template<class T>
	bool get_item(const char* path, T* val)const
	{
		std::string str;
		if ( get_item( path, &str) )
		{
			std::istringstream stream;
			stream.str(str);
			if (stream>>*val)
			{
				return true;
			}
		}

		return false;
	}

	template<class T>
	T get_value(const char* path, const T& default_value)const
	{
		T val;
		if (get_item(path, &val))
		{
			return val;
		}
		else
		{
			return default_value;
		}
	}

	template<class T>
	bool set_item(const char* path, const T& val)
	{
		std::ostringstream stream;
		if ( stream << val )
		{
			return set_item(path, stream.str());
		}
		else
		{
			return false;
		}
	}

	bool list_item(const char* path, std::vector<std::string>* element_list, std::vector<std::string>* attribute_list=NULL);


protected:

	bool find_item(const char* path, bool read_only);

	//在每次set和get之前调用。本函数应该清除可能残余的中间状态
	virtual void clean()=0;
	//在每次路径分析遇到 ".."符号时调用
	virtual bool up()=0;
	//在每次路径分析遇到'/'符号时调用
	virtual bool down()=0;
	//在每次路径分析遇到'@'符号时调用
	virtual bool attribute()=0;
	//在每次路径分析遇到一般的路径字符串时调用，若 read_only 为真，则说明此次的路径访问是 get 操作，不应改变原有的结构。
	virtual bool value(const std::string& v, bool read_only)=0;

	virtual bool list(std::vector<std::string>* element_list, std::vector<std::string>* attribute_list)=0;
	
	//根据当前状态，获得值。
	virtual bool get(std::string* str)const=0;

	//根据当期状态，设置值
	virtual bool set(const std::string& str)=0;
};


#endif // config_h__

