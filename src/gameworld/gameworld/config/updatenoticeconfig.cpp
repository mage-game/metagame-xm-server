#include "updatenoticeconfig.hpp"

#include "servercommon/configcommon.h"

UpdateNoticeConfig::UpdateNoticeConfig()
{
	
}

UpdateNoticeConfig::~UpdateNoticeConfig()
{

}

bool UpdateNoticeConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "UpdateNoticeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + "xml no other node.";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitOtherCfg fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int UpdateNoticeConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode dataElement = root_element.child("data");
	if (dataElement.empty())
	{
		return -1000000;
	}

	if (!PugiGetSubNodeValue(dataElement, "version", m_other_cfg.version) || m_other_cfg.version < 0)
	{
		return -1;
	}

	PugiXmlNode reward_item_element = dataElement.child("reward_item");
	if (reward_item_element.empty())
	{
		return -2;
	}

	if (!m_other_cfg.reward_item.ReadConfig(reward_item_element))
	{
		return -3;
	}

	return 0;
}

