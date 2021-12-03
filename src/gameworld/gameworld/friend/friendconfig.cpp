#include "friendconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

FriendConfig::FriendConfig()
{

}

FriendConfig::~FriendConfig()
{

}

bool FriendConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// ÷˙ ÷
	PUGI_XML_LOAD_CONFIG("favorable_impression_other", InitOther);

	return true;
}

int FriendConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "max_favorable_impression", m_other_cfg.max_favorable_impression) || m_other_cfg.max_favorable_impression < 0)
	{
		return -1;
	}
	if (!PugiGetSubNodeValue(data_element, "max_day_favorable_impression", m_other_cfg.max_day_favorable_impression) || m_other_cfg.max_day_favorable_impression < 0)
	{
		return -2;
	}
	if (!PugiGetSubNodeValue(data_element, "add_favorable_impression", m_other_cfg.add_favorable_impression) || m_other_cfg.add_favorable_impression < 0)
	{
		return -3;
	}

	return 0;
}

