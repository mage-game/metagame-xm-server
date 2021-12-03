
#include "activityswitchconfig.hpp"
#include "servercommon/activitydef.hpp"
#include "servercommon/configcommon.h"


ActivitySwitchConfig::ActivitySwitchConfig()
{

}

ActivitySwitchConfig::~ActivitySwitchConfig()
{

}

bool ActivitySwitchConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ActivitySwitchConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ½ûÖ¹»î¶¯¿ªÆôÅäÖÃ
		PugiXmlNode ban_element = RootElement.child("ban_activity_cfg");
		if (ban_element.empty())
		{
			*err = configname + ": no [ban_activity_cfg].";
			return false;
		}

		iRet = this->InitBanActivityCfg(ban_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBanActivityCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ActivitySwitchConfig::InitBanActivityCfg(PugiXmlNode RootElement)
{
	m_ban_activity_cfg.clear();

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int act_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "act_id", act_id))
		{
			return -1;
		}

		if (!(act_id > ACTIVITY_TYPE_INVALID && act_id < ACTIVITY_TYPE_MAX) && 
			!(act_id > SPECIAL_ACTIVITY_TYPE_INVALID && act_id < SPECIAL_ACTIVITY_TYPE_RESVERED) &&
			!(act_id >= RAND_ACTIVITY_TYPE_BEGIN && act_id < RAND_ACTIVITY_TYPE_END) &&
			!(act_id > CROSS_ACTIVITY_TYPE_INVALID && act_id < CROSS_ACTIVITY_TYPE_RESERVED)
			)
		{
			return -9999;
		}

		m_ban_activity_cfg.insert(act_id);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool ActivitySwitchConfig::IsBanActivity(int act_id)
{
	if (m_ban_activity_cfg.end() != m_ban_activity_cfg.find(act_id))
	{
		return true;
	}

	return false;
}
