#include "servercommon/configcommon.h"

#include "iplimit.hpp"

IPLimit & IPLimit::Instance()
{
	static IPLimit ip_limist;
	return ip_limist;
}

IPLimit::IPLimit()
	: m_limit_switch(false)
{

}

IPLimit::~IPLimit()
{

}

bool IPLimit::Init(const std::string& path, const std::string& configname, std::string* err)
{
	TiXmlDocument m_document;

	if (path == "" || !m_document.LoadFile(path+configname))
	{
		*err = path + configname + ": Load registe config file Error: " + m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	std::string configpath;

	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	int limit_switch = 0;

	if (!GetSubNodeValue(RootElement, "limit_switch", limit_switch))
	{
		*err = configname + ": xml node error in [config->limit_switch]";
		return false;
	}
	
	m_limit_switch = (1 == limit_switch);

	TiXmlElement *ip_list_element = RootElement->FirstChildElement("ip_list");
	if (!ip_list_element)
	{
		*err = configname + ": xml node error in [config->ip_list]";
		return false;
	}

	TiXmlElement *ip_element = ip_list_element->FirstChildElement("ip");
	if (!ip_element)
	{
		*err = configname + ": xml node error in [config->ip_list->ip]";
		return false;
	}
	
	while (ip_element)
	{	
		std::string ip; 

		if (!GetNodeValue(ip_element, ip))
		{
			*err = configname + ": xml node error in [config->ip_list->ip]";
			return false;
		}

		m_auth_ip_list.insert(ip);

		ip_element = ip_element->NextSiblingElement();
	}

	return true;
}

bool IPLimit::CanPassIPLimit(const std::string & ip)
{
	AuthIPListIt it = m_auth_ip_list.find(ip);
	if (it == m_auth_ip_list.end()) return false;
	return true;
}

