#include "loginusertrace.hpp"

#include "servercommon/configcommon.h"

LoginUserTrace::LoginUserTrace() : m_is_open(false)
{

}

LoginUserTrace::~LoginUserTrace()
{

}

LoginUserTrace & LoginUserTrace::Instance()
{
	static LoginUserTrace obj;
	return obj;
}

bool LoginUserTrace::Init(const std::string &path, const std::string &configname, std::string *err)
{
	TiXmlDocument m_document;
	if (path == "" || !m_document.LoadFile(path+configname))
	{
		*err = path + configname + ": LoginUserTrace::Init Error: " + m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (NULL == RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	if (!GetSubNodeValue(RootElement, "is_open", m_is_open))
	{
		*err = configname + ": xml node error in is_open";
		return false;
	}

	TiXmlElement *pname_list_element = RootElement->FirstChildElement("plat_name_list");
	if (NULL == pname_list_element)
	{
		*err = configname + ": xml node error in [config->plat_name_list]";
		return false;
	}

	TiXmlElement *pname_element = pname_list_element->FirstChildElement("plat_name");

	std::string temp_pname;
	while (NULL != pname_element)
	{
		if (!GetNodeValue(pname_element, temp_pname))
		{
			*err = configname + ": xml node error in [config->plat_name_list->plat_name]";
			return false;
		}

		m_pname_list.insert(temp_pname);

		pname_element = pname_list_element->NextSiblingElement("plat_name");
	}

	return true;
}

void LoginUserTrace::SetOpenFlag(bool is_open)
{
	m_is_open = is_open;
}

void LoginUserTrace::AddPName(PlatName pname)
{
	if (NULL == pname) return;

	pname[sizeof(PlatName) - 1] = 0;
	
	m_pname_list.insert(pname);
}

void LoginUserTrace::ClearPNameList()
{
	m_pname_list.clear();
}

bool LoginUserTrace::IsTrace(PlatName pname)
{
	if (!m_is_open || NULL == pname) return false;

	pname[sizeof(PlatName) - 1] = 0;

	return m_pname_list.find(pname) != m_pname_list.end();
}

