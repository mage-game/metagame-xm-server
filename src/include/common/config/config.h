#ifndef BASECONFIG_H
#define BASECONFIG_H

/*

	ͨ�� Config ���࣬���������ļ�ϵͳ��·�����ʷ�ʽ������������������չʵ�ʵķ��ʲ�����
	��Config������·�������������շ�����˳����÷��ʺ�����
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

	//��ÿ��set��get֮ǰ���á�������Ӧ��������ܲ�����м�״̬
	virtual void clean()=0;
	//��ÿ��·���������� ".."����ʱ����
	virtual bool up()=0;
	//��ÿ��·����������'/'����ʱ����
	virtual bool down()=0;
	//��ÿ��·����������'@'����ʱ����
	virtual bool attribute()=0;
	//��ÿ��·����������һ���·���ַ���ʱ���ã��� read_only Ϊ�棬��˵���˴ε�·�������� get ��������Ӧ�ı�ԭ�еĽṹ��
	virtual bool value(const std::string& v, bool read_only)=0;

	virtual bool list(std::vector<std::string>* element_list, std::vector<std::string>* attribute_list)=0;
	
	//���ݵ�ǰ״̬�����ֵ��
	virtual bool get(std::string* str)const=0;

	//���ݵ���״̬������ֵ
	virtual bool set(const std::string& str)=0;
};


#endif // config_h__

