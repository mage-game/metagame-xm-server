#ifndef CONFIGCOMMON_H
#define CONFIGCOMMON_H

#include <cstring>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>

#include "servercommon/pugixml/pugixml.hpp"
#include "servercommon/servercommon.h"
#include "gameworld/gameworld/gamedef.h"

// 将字符串转成基础数据类型,非基础类型需显示定义
template<class T>
inline bool StringToBaseType(const char * str, T& val)
{
	std::istringstream stream;
	stream.str(str);
	if (stream >> val)
	{
		return true;
	}
	return false;
}

template<class T>
inline bool StringToBaseType(const std::string& str, T& val)
{
	std::istringstream stream;
	stream.str(str);
	if (stream >> val)
	{
		return true;
	}
	return false;
}

#define TEMPLATE_SPECIALIZATION_STRING(VAL_TYPE, FUNCTION) template<>\
inline bool StringToBaseType(const std::string &str, VAL_TYPE& val)\
{\
	if (str.size() > 0) {\
          val = static_cast<VAL_TYPE>(FUNCTION(str.c_str()));       \
		return true;\
	}\
	return false;\
}\

#define TEMPLATE_SPECIALIZATION_CHAR(VAL_TYPE, FUNCTION) template<>\
inline bool StringToBaseType(const char * str, VAL_TYPE& val)\
{\
	if (strlen(str) > 0) {\
		val = static_cast<VAL_TYPE>(FUNCTION(str));\
		return true;\
	}\
	return false;\
}\


template<>
inline bool StringToBaseType(const char * str, bool& val)
{
	if (strlen(str) > 0) {
		val = (atoi(str) > 0);
		return true;
	}
	return false;
}

template<>
inline bool StringToBaseType(const char * str, std::string& val)
{
	if (strlen(str) > 0) {
		val = str;
		return true;
	}
	return false;
}

// bool 的特化，强转会有警告，所以直接这样写
template<>
inline bool StringToBaseType(const std::string& str, bool& val)
{
	if (str.size() > 0) {
          val = (atoi(str.c_str()) > 0);
		return true;
	}
	return false;
}
template<>
inline bool StringToBaseType(const std::string& str, std::string& val)
{
	if (str.size() > 0) {
		val = str;
		return true;
	}
	return false;
}

//TEMPLATE_SPECIALIZATION_STRING(bool, stoi)
TEMPLATE_SPECIALIZATION_STRING(unsigned int, atoi)
TEMPLATE_SPECIALIZATION_STRING(int, atoi)
TEMPLATE_SPECIALIZATION_STRING(unsigned short, atoi)
TEMPLATE_SPECIALIZATION_STRING(short, atoi)
TEMPLATE_SPECIALIZATION_STRING(unsigned char, atoi)
TEMPLATE_SPECIALIZATION_STRING(char, atoi)
TEMPLATE_SPECIALIZATION_STRING(unsigned long, atol)
TEMPLATE_SPECIALIZATION_STRING(long, atol)
TEMPLATE_SPECIALIZATION_STRING(unsigned long long, atoll)
TEMPLATE_SPECIALIZATION_STRING(long long, atoll)
TEMPLATE_SPECIALIZATION_STRING(double, atof)
TEMPLATE_SPECIALIZATION_STRING(float, atof)

//TEMPLATE_SPECIALIZATION_CHAR(bool, atoi)
TEMPLATE_SPECIALIZATION_CHAR(unsigned int, atoi)
TEMPLATE_SPECIALIZATION_CHAR(int, atoi)
TEMPLATE_SPECIALIZATION_CHAR(unsigned short, atoi)
TEMPLATE_SPECIALIZATION_CHAR(short, atoi)
TEMPLATE_SPECIALIZATION_CHAR(unsigned char, atoi)
TEMPLATE_SPECIALIZATION_CHAR(char, atoi)
TEMPLATE_SPECIALIZATION_CHAR(unsigned long, atol)
TEMPLATE_SPECIALIZATION_CHAR(long, atol)
TEMPLATE_SPECIALIZATION_CHAR(unsigned long long, atoll)
TEMPLATE_SPECIALIZATION_CHAR(long long, atoll)
TEMPLATE_SPECIALIZATION_CHAR(double, atof)
TEMPLATE_SPECIALIZATION_CHAR(float, atof)

// 将基础数据类型转成字符串,非基础类型需显示定义
template<class T>
bool BaseTypeToString(const T& src, std::string& tar)
{
	std::ostringstream stream;
	if (stream<<src)
	{
		tar = stream.str();
		return true;
	}
	return false;
}

#include "common/tinyxml/tinyxml.h"

// 读写element的子节点name的值，不适用多个同名子节点
// <element>
//		……
//		<name>s_val</name>
//		……
// </element>
template<class T>
static bool GetSubNodeValue(TiXmlElement *element, const char *name, T& val)
{
	if (!element) return false;
	TiXmlElement *TmpElement = element->FirstChildElement(name);
	return GetNodeValue(TmpElement, val);
}
template<class T>
static bool GetSubNodeValue(TiXmlElement *element, const std::string& name, T& val)
{	
	return GetSubNodeValue(element, name.c_str(), val);
}

template<class T>
static bool SetSubNodeValue(TiXmlElement *element, const std::string& name, T& val)
{
	if (!element) return false;
	TiXmlElement *TmpElement = element->FirstChildElement(name.c_str());
	return SetNodeValue(TmpElement, val);
}

// 读写如下节点element的值,不支持形如<element/>的空值
// <element>s_val</element>
template<class T>
static bool GetNodeValue(TiXmlElement *element, T& val)
{
	if (!element || !element->FirstChild() || !StringToBaseType(element->FirstChild()->ValueStr(), val))
	{
		return false;
	}
	return true;
}
template<class T>
static bool SetNodeValue(TiXmlElement *element, T& val)
{
	std::string value;
	if (!element || !element->FirstChild() || !BaseTypeToString(val,value))
	{
		return false;
	}
	element->FirstChild()->SetValue(value);
	return true;
}


/*
* 获取节点attribute值
*/
template<class T>
static bool GetNodeAttributeValue(TiXmlElement *element, const std::string &name, T& val)
{
	if (!element || !StringToBaseType(*element->Attribute(name), val))
	{
		return false;
	}

	return true;
}

template<class T>
static bool GetNodeAttributeValue(pugi::xml_node element, const std::string &name, T& val)
{
	if (element.empty() || !StringToBaseType(element.attribute(name.c_str()).value(), val))
	{
		return false;
	}

	return true;
}


// 对应至少一个子节点的xml结构
// <Element>
//		<node_0>value</node_0>
//		<node_1>value</node_1>
//		<node_2>value</node_2>
//		……
//		<Element>……</Element>
// </Element>
class ConfigElementBase
{
public:
	ConfigElementBase():m_element(0) {}
	virtual ~ConfigElementBase() {}

	void SetElement(TiXmlElement *element) { m_element = element; }

	// 供子类列表中获取偏移地址
	virtual ConfigElementBase* offset() = 0;
	// 将子节点读到相应成员数据中
	virtual bool read() = 0;
	// 将成员数据写入文件
	virtual bool write() = 0;

protected:
	// 获取子节点的值,不适用有多个同名子节点的情况
	template<class T>
	bool GetValue(const char *name, T* value)
	{
		if (m_element == 0) return false;
		return GetSubNodeValue(m_element, name, *value);
	}
	bool GetValue(const char *name, ConfigElementBase* value)
	{
		if (m_element == 0) return false;
		value->SetElement(m_element->FirstChildElement(name));
		return value->read();
	}

	// 获取子节点的值,适用有多个同名子节点的情况,与数组对应
	template<class T>
	bool GetListValue(const char *name, T* list_value, int& list_len, int list_max_len)
	{
		list_len = 0;
		if (m_element == 0)
		{
			return false;
		}
		TiXmlElement *tmp_element = m_element->FirstChildElement(name);
		for ( ;list_len < list_max_len && tmp_element != 0; ++list_len, ++list_value, tmp_element = tmp_element->NextSiblingElement(name))
		{
			if (!GetNodeValue(tmp_element, *list_value)) return false;
		}
		return true;
	}
	bool GetListValue(const char *name, ConfigElementBase* list_value, int& list_len, int list_max_len)
	{
		list_len = 0;
		if (m_element == 0)
		{
			return false;
		}
		TiXmlElement *tmp_element = m_element->FirstChildElement(name);
		for ( ;list_len < list_max_len && tmp_element != 0; ++list_len)
		{
			list_value->SetElement(tmp_element);
			if (!list_value->read())return false;
			list_value = list_value->offset();
			tmp_element = tmp_element->NextSiblingElement(name);
		}
		return true;
	}

	// 设置子节点的值,不适用有多个同名子节点的情况
	template<class T>
	bool SetValue(const char *name, T* value)
	{
		if (m_element == 0) return false;

		TiXmlElement * tmp_element = m_element->FirstChildElement(name);
		if (!tmp_element)
		{
			TiXmlElement insert_node(name);
			tmp_element = (TiXmlElement *)m_element->InsertEndChild(insert_node);
		}
		if (!tmp_element->FirstChild())
		{
			std::string tmp_value;
			BaseTypeToString(*value, tmp_value);
			TiXmlText sub_node(tmp_value);
			tmp_element->InsertEndChild(sub_node);
			return true;
		}
		return SetSubNodeValue(m_element, name, *value);
	}
	bool SetValue(const char *name, ConfigElementBase* value)
	{
		if (m_element == 0) return false;
		TiXmlElement * tmp_element = m_element->FirstChildElement(name);
		if (!tmp_element)
		{
			TiXmlElement insert_node(name);
			m_element->InsertEndChild(insert_node);
			tmp_element = m_element->FirstChildElement(name);
		}
		value->SetElement(tmp_element);
		return value->write();
	}

	// 设置子节点的值,适用有多个同名子节点的情况,与数组对应
	template<class T>
	bool SetListValue(const char *name, T* list_value, int list_len)
	{
		if (m_element == 0 || list_len < 0)
		{
			return false;
		}
		TiXmlElement *tmp_element = m_element->FirstChildElement(name);
		for(int i=0;i<list_len;++i,++list_value)
		{
			if (!tmp_element)
			{
				TiXmlElement insert_node(name);
				tmp_element = (TiXmlElement *)m_element->InsertEndChild(insert_node);
			}
			if (!tmp_element->FirstChild())
			{
				std::string tmp_value;
				BaseTypeToString(*list_value, tmp_value);
				TiXmlText sub_node(tmp_value);
				tmp_element->InsertEndChild(sub_node);
			}
			else
			{
				if (!SetNodeValue(tmp_element, *list_value)) return false;
			}
			tmp_element = tmp_element->NextSiblingElement(name);
		}
		while(tmp_element)
		{
			TiXmlElement *to_remove = tmp_element;
			tmp_element = tmp_element->NextSiblingElement(name);
			m_element->RemoveChild(to_remove);
		};
		return true;
	}
	bool SetListValue(const char *name, ConfigElementBase* list_value, int list_len)
	{
		if (m_element == 0 || list_len < 0)
		{
			return false;
		}
		TiXmlElement *tmp_element = m_element->FirstChildElement(name);
		for(int i=0;i<list_len;++i)
		{
			if (!tmp_element)
			{
				TiXmlElement insert_node(name);
				tmp_element = (TiXmlElement *)m_element->InsertEndChild(insert_node);
			}

			list_value->SetElement(tmp_element);
			if (!list_value->write()) return false;

			list_value= list_value->offset();
			tmp_element = tmp_element->NextSiblingElement(name);
		}
		while(tmp_element)
		{
			TiXmlElement *to_remove = tmp_element;
			tmp_element = tmp_element->NextSiblingElement(name);
			m_element->RemoveChild(to_remove);
		};
		return true;
	}


	// 至少有一个节点的xml结构
	TiXmlElement *m_element;

private:
};

namespace pugi {
	class xml_node;
}

struct AttrConfig
{
	enum ATTR_TYPE
	{
		ATTR_TYPE_INVALID = -1,
		ATTR_TYPE_MAXHP,
		ATTR_TYPE_GONGJI,
		ATTR_TYPE_FANGYU,
		ATTR_TYPE_MINGZHONG,
		ATTR_TYPE_SHANBI,
		ATTR_TYPE_BAOJI,
		ATTR_TYPE_JIANREN,

		ATTR_TYPE_MAX_COUNT
	};

	AttrConfig() : maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	int ReadConfig(TiXmlElement *data_element);
	int ReadConfig(pugi::xml_node &data_element);

	void Add(const AttrConfig &add_attr, double factor)
	{
		this->maxhp += static_cast<long long>(add_attr.maxhp * factor);
		this->gongji += static_cast<long long>(add_attr.gongji * factor);
		this->fangyu += static_cast<long long>(add_attr.fangyu * factor);
		this->mingzhong += static_cast<long long>(add_attr.mingzhong * factor);
		this->shanbi += static_cast<long long>(add_attr.shanbi * factor);
		this->baoji += static_cast<long long>(add_attr.baoji * factor);
		this->jianren += static_cast<long long>(add_attr.jianren * factor);
	}

	void Reset()
	{
		this->maxhp = 0;
		this->gongji = 0;
		this->fangyu = 0;
		this->mingzhong = 0;
		this->shanbi = 0;
		this->baoji = 0;
		this->jianren = 0;
	}

	void AddTranstAttr(int &type, int &value)
	{
		switch (static_cast<ATTR_TYPE>(type))
		{
		case ATTR_TYPE_MAXHP:
			{
				maxhp += value;
			}
			break;

		case ATTR_TYPE_GONGJI:
			{
				gongji += value;
			}
			break;

		case ATTR_TYPE_FANGYU:
			{
				fangyu += value;
			}
			break;

		case ATTR_TYPE_MINGZHONG:
			{
				mingzhong += value;
			}
			break;

		case ATTR_TYPE_SHANBI:
			{
				shanbi += value;
			}
			break;

		case ATTR_TYPE_BAOJI:
			{
				baoji += value;
			}
			break;

		case ATTR_TYPE_JIANREN:
			{
				jianren += value;
			}
			break;

		default: 
			break;
		}
	}
};

namespace globalconfig
{

#define CONFIG_INIT(config_struct, config_name) \
	CurrElement = RootElement->FirstChildElement(config_name);\
	if (!config_struct.InitConfig(configname, err, CurrElement, config_name)) return false;

#define START_CHECK_NODE \
	char errinfo[512] = {0};\
	char nodeinfo[64] = {0};\
	F_STRNCPY(nodeinfo, config_name, sizeof(nodeinfo));\
	if (NULL == node)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s].\ncannot find node.", file_name.c_str(), config_name);\
		*err = errinfo;\
		return false;\
	}\
	TiXmlElement *sub_node = node->FirstChildElement("data");\
	if (NULL == sub_node)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s] cannot find node.", file_name.c_str(), config_name);\
		*err = errinfo;\
		return false;\
	}


#define GET_NODE_DATA(mem, str, can_be_zero)\
	if (!GetSubNodeValue(sub_node, str, mem) || mem < 0)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s].%s", file_name.c_str(), nodeinfo, str);\
		*err = errinfo;\
		return false;\
	}\
	if (!can_be_zero && 0 == mem)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s].%s", file_name.c_str(), nodeinfo, str);\
		*err = errinfo;\
		return false;\
	}

#define GET_UNSIGNED_NODE_DATA(mem, str, can_be_zero)\
	if (!GetSubNodeValue(sub_node, str, mem))\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s].%s", file_name.c_str(), nodeinfo, str);\
		*err = errinfo;\
		return false;\
	}\
	if (!can_be_zero && 0 == mem)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s].%s", file_name.c_str(), nodeinfo, str);\
		*err = errinfo;\
		return false;\
	}

#define CHECK_SENTENCE(sentence, str)\
	if (sentence)\
	{\
		sprintf(errinfo, "%s: xml node error in [config->%s] %s", file_name.c_str(), nodeinfo, str);\
		*err = errinfo;\
		return false;\
	}\


#define GET_NEXT_NODE \
	sub_node = sub_node->NextSiblingElement("data");\
	if (NULL == sub_node) break;

#define PRE_LOAD_CONFIG \
	char errinfo[1024] = {0};\
	TiXmlDocument document;\
	if (configname == "" || !document.LoadFile(configname))\
	{\
	sprintf(errinfo,"Load config [%s] Error.%s", configname.c_str(), document.ErrorDesc());\
	*err = errinfo;\
	return false;\
}\
	TiXmlElement *RootElement = document.RootElement();\
	if (NULL == RootElement)\
	{\
	sprintf(errinfo,"Load config [%s] Error.cannot find root node.", configname.c_str());\
	*err = errinfo;\
	return false;\
}\
	int iRet = 0;

#define LOAD_CONFIG(element_name, init_func)\
	{\
		TiXmlElement *root_element = RootElement->FirstChildElement(element_name);\
		if (NULL == root_element)\
		{\
		*err = configname + ": no [" element_name "].";\
		return false;\
		}\
		iRet = this->init_func(root_element);\
		if (iRet)\
		{\
		sprintf(errinfo,"%s: %s failed %d", configname.c_str(), #init_func, iRet);\
		*err = errinfo;\
		return false;\
		}\
	}

#define LOAD_CHILD_ELEMENT_CONFIG(PARENT_ELEMENT, CHILD_NAME, FUNC)\
	{\
		TiXmlElement *CHILD_NAME_##element = PARENT_ELEMENT->FirstChildElement(#CHILD_NAME); \
		if (nullptr == CHILD_NAME_##element)\
		{\
			*err = path + " xml no [" #CHILD_NAME "] node";\
			return false;\
		}\
		iRet  = FUNC(CHILD_NAME_##element);\
		if (iRet)\
		{\
			sprintf(err_info, "%s %s failed: %d.\n", path.c_str(), #FUNC, iRet);\
			*err = err_info;\
			return false;\
		}\
	}

}

// 按分隔符分解内容，存到result里，返回分解的个数
template<class T>
inline int ParseParam(const char *src, std::vector<T> &result, const char *delim)
{
	static char temp[1024 * 8] = {};

	STRNCPY(temp, src, sizeof(temp));
	result.clear();

	char *token = strtok(temp, delim);

	while (NULL != token)
	{
		static T v;
		StringToBaseType(token, v);
		result.push_back(v);
		token = strtok(NULL, delim);
	}

	return (int)result.size();
}

// 列表配置，用"|"分隔
template<class T>
class ListConfig
{
public:
	bool ReadConfig(TiXmlElement * data_element, const char *node);
	bool ReadConfig(pugi::xml_node data_element, const char *node);

	bool IsValidIndex(size_t i) const { return i >= 0 && i < value_list.size(); }
	const T& operator[](size_t i) const { return value_list[i]; }

	int Count() const { return (int)value_list.size(); }

	// 返回下标为i的元素
	const T* Value(size_t i) const
	{
		if (i < 0 || i >= value_list.size())
		{
			return nullptr;
		}
		return &value_list[i];
	}

	// 返回v的下标，没有返回-1
	int ValueIdx(T v) const
	{
		for (typename std::vector<T>::const_iterator it = value_list.begin(); it != value_list.end(); ++it)
		{
			if (v == *it) return (int)(it - value_list.begin());
		}
		return -1;
	}

	// 是否存在v
	bool HasValue(T v) const
	{
		for (typename std::vector<T>::const_iterator it = value_list.begin(); it != value_list.end(); ++it)
		{
			if (v == *it) return true;
		}
		return false;
	}

private:
	std::vector<T> value_list;
	static std::string error_desc; // 仅保留最后一次的错误描述
};

template<class T>
std::string ListConfig<T>::error_desc = "No error";

template<class T>
bool ListConfig<T>::ReadConfig(TiXmlElement * data_element, const char *node)
{
	std::string tmp_str;
	if (!GetSubNodeValue(data_element, node, tmp_str))
	{
		error_desc = "no " + std::string(node);
		return false;
	}

	ParseParam(tmp_str.c_str(), value_list, "|");

	return true;
}

template<class T>
bool ListConfig<T>::ReadConfig(pugi::xml_node data_element, const char *node)
{
	std::string tmp_str;
	//if (!PugiGetSubNodeValue(data_element, node, tmp_str))
	//{
	//	error_desc = "no " + std::string(node);
	//	return false;
	//}
	if (data_element.empty())
	{
		error_desc = "no " + std::string(node);
		return false;
	}

	pugi::xml_node tmp_element = data_element.child(node);
	if (tmp_element.empty() || !StringToBaseType(tmp_element.child_value(), tmp_str))
	{
		error_desc = "no " + std::string(node) + " child";
		return false;
	}

	ParseParam(tmp_str.c_str(), value_list, "|");

	return true;
}

// 祝福值暴击配置，如： 暴击值：1|5|10 暴击概率：50|30|20
struct LuckyConfig
{
	struct LuckyItem
	{
		LuckyItem(int _val, int _weight) : val(_val), weight(_weight) {}
		int val = 0;
		int weight = 0;
	};

	template<class T>
	bool ReadConfig(T &data_element, const char *lucky_vals_name, const char *lucky_rates_name)
	{
		ListConfig<int> lucky_vals_cfg;
		if (!lucky_vals_cfg.ReadConfig(data_element, lucky_vals_name)) return false;

		ListConfig<int> lucky_rates_cfg;
		if (!lucky_rates_cfg.ReadConfig(data_element, lucky_rates_name)) return false;

		if (lucky_vals_cfg.Count() != lucky_rates_cfg.Count()) return false;

		luck_item_list.reserve(lucky_vals_cfg.Count());
		for (int i = 0; i < lucky_vals_cfg.Count(); ++i)
		{
			luck_item_list.push_back(LuckyItem(*lucky_vals_cfg.Value(i), *lucky_rates_cfg.Value(i)));
		}

		return true;
	}

	// 随机一个暴击值
	int Random() const;

	// 是否合法
	bool IsValid() const
	{
		return luck_item_list.size() > 0;
	}

private:
	std::vector<LuckyItem> luck_item_list;
};

struct NumListConfig
{
	bool ReadConfig(pugi::xml_node data_element, const pugi::char_t* node);

	enum CHECK_POLICY
	{
		CHECK_POLICY_NONE,				// 无
		CHECK_POLICY_GRANTER_0,			// 大于0
		CHECK_POLICY_GRANTER_EQUAL_0,	// 大于等于0
		CHECK_POLICY_RANGE,				// 取值范围，左闭右开
		//CHECK_POLICY_MONSTER,			// 检查monster
		CHECK_POLICY_UP_ORDER,          // 检查升序
	};
	bool Check(int policy, int param1 = 0, int param2 = 0);

	const char* ErrorDesc() { return error_desc.c_str(); }

	int Count() const
	{
		return (int)num_list.size();
	}

	const int* Value(int i) const
	{
		if (i < 0 || i >= (int)num_list.size())
		{
			return NULL;
		}
		return &num_list[i];
	}

	// 转换成C数组，返回实际转换的个数
	int ToCArray(int *arr, int len) const;

private:
	std::vector<int> num_list;
	std::string error_desc;
};

// 例：50,50
struct PointConfig
{
	PointConfig() : pos(0, 0) {}

	static bool ParsePos(const char pos_str[], Posi &pos)
	{
		char tmp_buff[1024];
		STRNCPY(tmp_buff, pos_str, sizeof(tmp_buff));

		char *token = strtok(tmp_buff, ",");
		if (NULL == token)
		{
			return false;
		}
		if (!StringToBaseType(token, pos.x))
		{
			return false;
		}

		token = strtok(NULL, ",");
		if (NULL == token)
		{
			return false;
		}

		if (!StringToBaseType(token, pos.y))
		{
			return false;
		}

		return true;
	}

	bool ReadConfig(TiXmlElement * data_element, const char *node);

	bool ReadConfig(pugi::xml_node data_element, const pugi::char_t* name);

	const Posi &GetPos() const { return pos; }

private:
	Posi pos;
};

// 例：50,50|20,20
struct PointListConfig
{
	PointListConfig() {}

	bool ReadConfig(TiXmlElement * data_element, const char *node);

	bool ReadConfig(pugi::xml_node data_element, const pugi::char_t* name);

	int Count()
	{
		return (int)point_list.size();
	}

	const Posi* Value(int i) const
	{
		if (i < 0 || i >= (int)point_list.size())
		{
			return NULL;
		}
		return &point_list[i];
	}

private:
	std::vector<Posi> point_list;
};

#endif

