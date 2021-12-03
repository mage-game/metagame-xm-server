#include "spaceconfig.hpp"

#include "item/itempool.h"
#include <set>
#include "item/itembase.h"

SpaceConfig::SpaceConfig()	
	: m_cai_addrenqi(0), m_cai_addhuoli(0), m_cai_dayhuoli_limit(0), m_gift_addrenqi(0), m_gift_addhuoli(0), m_gift_cost(0)
{

}

SpaceConfig::~SpaceConfig()
{

}

bool SpaceConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "SpaceConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("other_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int SpaceConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "cai_addrenqi", m_cai_addrenqi) || m_cai_addrenqi <= 0)
	{
		return -11;
	}
	if (!PugiGetSubNodeValue(dataElement, "cai_addhuoli", m_cai_addhuoli) || m_cai_addhuoli <= 0)
	{
		return -12;
	}
	if (!PugiGetSubNodeValue(dataElement, "cai_dayhuoli_limit", m_cai_dayhuoli_limit) || m_cai_dayhuoli_limit <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "gift_addrenqi", m_gift_addrenqi) || m_gift_addrenqi <= 0)
	{
		return -14;
	}
	if (!PugiGetSubNodeValue(dataElement, "gift_addhuolizhi", m_gift_addhuoli) || m_gift_addhuoli <= 0)
	{
		return -15;
	}
	if (!PugiGetSubNodeValue(dataElement, "gift_cost", m_gift_cost) || m_gift_cost <= 0)
	{
		return -16;
	}

	return 0;
}




