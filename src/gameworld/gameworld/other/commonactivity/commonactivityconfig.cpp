#include "commonactivityconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/activitydef.hpp"

CommonActivityConfig::CommonActivityConfig()
{

}

CommonActivityConfig::~CommonActivityConfig()
{

}

bool CommonActivityConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CommonActivityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ¿ªÆôµ¥ÔªÅäÖÃ
		PugiXmlNode root_element = RootElement.child("open_unit");
		if (root_element.empty())
		{
			*err = path + " xml not open_unit node ";
			return false;
		}

		iRet = this->InitOpenUnitCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOpenUnitCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

bool CommonActivityConfig::IsUnitOpen(int unit_type)
{
	std::set<int>::iterator it = m_open_unit.find(unit_type);
	if (m_open_unit.end() != it)
	{
		return true;
	}

	return false;
}

int CommonActivityConfig::InitOpenUnitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int open_unit = 0;
		if (!PugiGetSubNodeValue(dataElement, "open_unit", open_unit) || open_unit <= COMMON_ACTIVITY_UNIT_TYPE_INVALID || open_unit >= COMMON_ACTIVITY_UNIT_TYPE_MAX)
		{
			return -1;
		}

		if (m_open_unit.end() != m_open_unit.find(open_unit))
		{
			return -2;
		}

		m_open_unit.insert(open_unit);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

