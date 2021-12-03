#include "crossvipbossconfig.hpp"
#include "servercommon/configcommon.h"
#include <set>

CrossVipBossConfig::CrossVipBossConfig()
{

}

CrossVipBossConfig::~CrossVipBossConfig()
{

}

CrossVipBossConfig & CrossVipBossConfig::Instance()
{
	static CrossVipBossConfig _instance;
	return _instance;
}

bool CrossVipBossConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path))
	{
		*err = path + "£ºLoad CrossVipBossConfig Error,\n" + document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		TiXmlElement *other_element = RootElement->FirstChildElement("other");
		if (NULL == other_element)
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossVipBossConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	int is_open = 0;
	if (!GetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);

	return 0;
}