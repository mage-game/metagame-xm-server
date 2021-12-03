#ifndef __CONFIG_BASE_H__
#define __CONFIG_BASE_H__
#include <vector>
#include <string.h>
#include "pugixml/pugixml_adapter.hpp"
#include "servercommon.h"

// 读配置基类
class ConfigBase
{
public:
	ConfigBase() {}
	virtual ~ConfigBase() {}

	/*
	** 改函数用于读取列表,只读取,不负责检查
	** 类型名T一般是int, unsigned int, short, unsigned short, long long中的一个
	** 参数element，		xml节点
	** 参数element_name，	字段名称
	** 参数vec，			容器，类型T一般是数字类型，取值范围是long long以下
	** 参数split,			分隔符
	*/
	template<typename T>
	int ReadList(const PugiXmlNode & element, const std::string & element_name, std::vector<T> &vec, const std::string & split)
	{
		std::string str_list;
		if (element.empty())
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(element, element_name, str_list) || str_list.size() == 0)
		{
			return -2;
		}
		std::vector<std::string> vec_str;
		F_STRSPLIT(vec_str, str_list, split);
		for (size_t j = 0; j < vec_str.size(); ++j)
		{
			vec.push_back(static_cast<T>(atoll(vec_str[j].c_str())));
		}
		if (vec.size() == 0)
		{
			return -3;
		}
		return 0;
	}

	/*
	** 改函数用于读取列表中的列表,只读取,不负责检查
	** 类型名T一般是int, unsigned int, short, unsigned short, long long中的一个
	** 参数element，		xml节点
	** 参数element_name，	字段名称
	** 参数vec，			容器，类型T一般是数字类型，取值范围是long long以下
	** 参数split_in,		内分隔符
	** 参数split_out,		外分隔符
	*/
	template<typename T>
	int ReadListInList(const PugiXmlNode & element, const std::string & element_name, std::vector<std::vector<T> > &vec_in_vec, const std::string & split_in, const std::string & split_out)
	{
		std::string str_list;
		if (element.empty())
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(element, element_name, str_list) || str_list.size() == 0)
		{
			return -2;
		}
		std::vector<std::string> range_vec_str;
		F_STRSPLIT(range_vec_str, str_list, split_out);
		for (size_t k = 0; k < range_vec_str.size(); ++k)
		{
			std::vector<std::string> vec_str;
			std::vector<T> vec_t;
			F_STRSPLIT(vec_str, range_vec_str[k], split_in);
			for (size_t j = 0; j < vec_str.size(); ++j)
			{
				vec_t.push_back(static_cast<T>(atoll(vec_str[j].c_str())));
			}
			if (vec_t.size() == 0)
			{
				return -3;
			}
			vec_in_vec.push_back(vec_t);
		}
		if (vec_in_vec.size() == 0)
		{
			return -4;
		}
		return 0;
	}

	/*
	** 改函数用于读取列数组,只读取,不负责检查
	** 参数element，		xml节点
	** 参数element_name，	字段名称,函数内会循环在字段名后面附加个数字，从0一直往上加，直到读不到为止
	** 参数vec，			容器
	** 参数max_column，		最大列数（循环次数）
	*/
	template<typename T>
	int ReadColumnList(const PugiXmlNode & element, const std::string & element_name, std::vector<T> &vec, const size_t max_column)
	{
		if (element.empty())
		{
			return -1;
		}
		for (size_t i = 0; i < max_column; ++i)
		{
			T temp_t;
			if (!PugiGetSubNodeValue(element, element_name + std::to_string(i), temp_t))
			{
				break;
			}
			vec.push_back(temp_t);
		}
		if (vec.size() == 0)
		{
			return -2;
		}
		return 0;
	}
};

#endif