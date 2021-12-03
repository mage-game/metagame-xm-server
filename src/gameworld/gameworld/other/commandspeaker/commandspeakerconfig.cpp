#include "commandspeakerconfig.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"

CommandSpeakerConfig::CommandSpeakerConfig()
{
}

CommandSpeakerConfig::~CommandSpeakerConfig()
{
}

bool CommandSpeakerConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "CommandSpeakerConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo, "Load CommandSpeaker config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		int ret = this->InitOtherConfig(root_element);
		if (ret)
		{
			sprintf(errinfo, "%s: InitOtherConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CommandSpeakerConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "consume", m_other_cfg.consume) || m_other_cfg.consume <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "reward_role_limit", m_other_cfg.reward_role_limit) || m_other_cfg.reward_role_limit <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "bind_gold_num", m_other_cfg.bind_gold_num) || m_other_cfg.bind_gold_num <= 0)
	{
		return -4;
	}

	return 0;
}