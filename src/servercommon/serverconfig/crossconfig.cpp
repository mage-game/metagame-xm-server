#include "crossconfig.hpp"

#include <servercommon/configcommon.h>

CrossConfig::CrossConfig() : m_is_open_cross(false), m_is_hidden_server(false)
{

}

CrossConfig::~CrossConfig()
{

}

CrossConfig & CrossConfig::Instance()
{
	static CrossConfig cc;
	return cc;
}


bool CrossConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	PugiXmlNode AddrElement = RootElement.child("CrossServerAddr");
	if (AddrElement.empty())
	{
		*err = path + ": xml node error in [config->CrossServerAddr]";
		return false;
	}

	m_cross_config_map.clear();
	m_cross_function_type_map.clear();

	std::set<std::string> cross_addr_check; // 检测重复IP端口
	char temp_addr_key[30];

	PugiXmlNode addr_item_element = AddrElement.child("AddrItem");
	while (!addr_item_element.empty())
	{
		int cross_type = 0;
		if (!PugiGetSubNodeValue(addr_item_element, "CrossType", cross_type) || cross_type <= 0)
		{
			*err = path + ": xml node error in [config->CrossServerAddr->CrossType]";
			return false;
		}

		NetAddress net_addr;
		if (!PugiGetSubNodeValue(addr_item_element, "Ip", net_addr.ipstr) || net_addr.ipstr.empty())
		{
			*err = path + ": xml node error in [config->CrossServerAddr->Ip]";
			return false;
		}
		if (!PugiGetSubNodeValue(addr_item_element, "Port", net_addr.port) || 0 == net_addr.port)
		{
			*err = path + ": xml node error in [config->CrossServerAddr->Port]";
			return false;
		}

		// 跨服类型不可重复
		if (m_cross_config_map.find(cross_type) != m_cross_config_map.end())
		{
			*err = path + ": CrossType repeat ";
			return false;
		}

		SNPRINTF(temp_addr_key, sizeof(temp_addr_key), "%s:%u", net_addr.ipstr.c_str(), net_addr.port);
		if (cross_addr_check.find(temp_addr_key) != cross_addr_check.end())
		{
			*err = path + ": CrossServerAddr repeat " + temp_addr_key;
			return false;
		}

		m_cross_config_map[cross_type] = net_addr;
		addr_item_element = addr_item_element.next_sibling();
	}

	PugiXmlNode cross_function_list_element = RootElement.child("cross_function_list");
	if (cross_function_list_element.empty())
	{
		*err = path + ": xml node error in [config->cross_function_list]";
		return false;
	}

	std::set<int> func_type_set; // 检查有无重复func type
	PugiXmlNode function_item_element = cross_function_list_element.child("function_item");
	while (!function_item_element.empty())
	{
		int func_type = 0;
		if (!GetNodeAttributeValue(function_item_element, "func_type", func_type) || func_type < CROSS_FUNCTION_TYPE_MIN)
		{
			*err = path + ": xml node error in [config->cross_function_list->func_type]";
			return false;
		}

		if (func_type_set.find(func_type) != func_type_set.end())
		{
			*err = path + ": xml node error in [config->cross_function_list->func_type] repeat";
			return false;
		}

		func_type_set.insert(func_type);

		CrossTypeInfo cross_type_info;
		if (!GetNodeAttributeValue(function_item_element, "cross_type", cross_type_info.cross_type) || cross_type_info.cross_type <= 0)
		{
			*err = path + ": xml node error in [config->cross_function_list->cross_type]";
			return false;
		}

		// 通用跨服类型要有对应的跨服配置
		if (m_cross_config_map.find(cross_type_info.cross_type) == m_cross_config_map.end())
		{
			*err = path + ": xml node error in [config->CrossServerAddr] cross_type not exist";
			return false;
		}

		if (!GetNodeAttributeValue(function_item_element, "opengame_day", cross_type_info.opengame_day) || cross_type_info.opengame_day < 0)
		{
			*err = path + ": xml node error in [config->cross_function_list->opengame_day]";
			return false;
		}

		m_cross_function_type_map[func_type] = cross_type_info;
		function_item_element = function_item_element.next_sibling();
	}

	if (!PugiGetSubNodeValue(RootElement, "open_cross", m_is_open_cross))
	{
		*err = path + ": xml node error in [config->open_cross]";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "is_hidden_server", m_is_hidden_server))
	{
		*err = path + ": xml node error in [config->is_hidden_server]";
		return false;
	}

	if (m_is_hidden_server && !m_is_open_cross)
	{
		*err = path + ": xml node error in [config->is_hidden_server] and [config->open_cross], hidden server must open cross!";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "hidden_server_cross_type", m_hidden_server_cross_type) || m_hidden_server_cross_type <= 0)
	{
		*err = path + ": xml node error in [config->hidden_server_cross_type]";
		return false;
	}

	if (m_is_hidden_server && m_cross_config_map.find(m_hidden_server_cross_type) == m_cross_config_map.end())
	{
		*err = path + ": xml node error in [config->hidden_server_cross_type] cross_type not exist";
		return false;
	}

	if (m_cross_config_map.size() <= 0)
	{
		*err = path + ": xml node error in [config->CrossServerAddr] m_cross_config_map.size() <= 0";
		return false;
	}

	// 至少要为通用跨服功能指定连接的跨服类型
	std::map<int, CrossTypeInfo>::iterator common_func_it = m_cross_function_type_map.find(CROSS_FUNCTION_TYPE_COMMON);
	if (common_func_it == m_cross_function_type_map.end())
	{
		*err = path + ": xml node error in [config->cross_function_list] no CROSS_FUNCTION_TYPE_COMMON";
		return false;
	}
	m_common_cross_type = common_func_it->second.cross_type;

	return true;
}

bool CrossConfig::Reload(const std::string &path, std::string *err, bool *is_cross_addr_change, bool *is_open_cross_change)
{
	std::map<int, NetAddress> old_cross_config_map = m_cross_config_map;
	bool old_open_cross = m_is_open_cross;

	bool ret = this->Init(path, err);
	if (ret)
	{
		if (NULL != is_cross_addr_change)
		{
			*is_cross_addr_change = (old_cross_config_map != m_cross_config_map);
		}

		if (NULL != is_open_cross_change)
		{
			*is_open_cross_change = (old_open_cross != m_is_open_cross);
		}
	}

	return ret;
}

int CrossConfig::GetCrossTypeWithCrossFuncType(int func_type)
{
	if (m_cross_function_type_map.find(func_type) != m_cross_function_type_map.end())
	{
		return m_cross_function_type_map[func_type].cross_type;
	}

	return m_common_cross_type; // 找不到匹配的，则返回默认cross类型
}

bool CrossConfig::CheckCrossServerHasCrossActivity(int cross_activity_type, int cross_type)
{
	if (-1 == cross_type)
	{
		cross_type = m_hidden_server_cross_type;
	}

	int act_cross_type = this->GetCrossTypeWithCrossFuncType(cross_activity_type);
	return (cross_type == act_cross_type);
}

bool CrossConfig::CheckCrossServerHasCrossRandActivity(int cross_rand_activity_type, int cross_type)
{
	if (-1 == cross_type)
	{
		cross_type = m_hidden_server_cross_type;
	}

	int act_cross_type = this->GetCrossTypeWithCrossFuncType(cross_rand_activity_type);
	return (cross_type == act_cross_type);
}

int CrossConfig::GetCrossOpenDay(int activity_type)
{
	std::map<int, CrossTypeInfo>::const_iterator it = m_cross_function_type_map.find(activity_type);
	if (it != m_cross_function_type_map.end())
	{
		return it->second.opengame_day;
	}

	return INT_MAX;
}


