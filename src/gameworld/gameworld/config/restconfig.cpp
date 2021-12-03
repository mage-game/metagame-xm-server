#include "restconfig.hpp"

#include "servercommon/configcommon.h"

RestConfig::RestConfig()
{
}

RestConfig::~RestConfig()
{
}

bool RestConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RestConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	PugiXmlNode RestReward_Element = RootElement.child("rest_reward");
	if (RestReward_Element.empty())
	{
		sprintf(errinfo, "%s: xml node error in [config->rest_reward]", path.c_str());
		*err = errinfo;
		return false;
	}

	PugiXmlNode RewardItem_Element = RestReward_Element.child("data");
	if (RewardItem_Element.empty())
	{
		sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item]", path.c_str());
		*err = errinfo;
		return false;
	}

	while (!RewardItem_Element.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(RewardItem_Element, "level", level) || level <= 0 || level > MAX_ROLE_LEVEL)
		{
			sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->level: %d]", path.c_str(), level);
			*err = errinfo;
			return false;
		}

		if (!m_reward[level].Invalid())
		{
			sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->level: %d] repeated", path.c_str(), level);
			*err = errinfo;
			return false;
		}
		
		{
			int exp = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "exp", exp) || exp < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->exp: %d]", path.c_str(), exp);
				*err = errinfo;
				return false;
			}
			m_reward[level].exp = exp;
		}

		{
			int xianhun = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "xianhun", xianhun) || xianhun < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->xianhun: %d]", path.c_str(), xianhun);
				*err = errinfo;
				return false;
			}
			m_reward[level].xianhun = xianhun;
		}

		{
			int yuanli = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "xianhun", yuanli) || yuanli < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->yuanli: %d]", path.c_str(), yuanli);
				*err = errinfo;
				return false;
			}
			m_reward[level].yuanli = yuanli;
		}

		{
			int mojing = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "mojing", mojing) || mojing < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->mojing: %d]", path.c_str(), mojing);
				*err = errinfo;
				return false;
			}
			m_reward[level].mojing = mojing;
		}

		{
			int monster_id = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "monster_id", monster_id) || monster_id < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->monster_id: %d]", path.c_str(), monster_id);
				*err = errinfo;
				return false;
			}
			m_reward[level].monster_id = monster_id;
		}

		{
			int minute_drop_count = 0;
			if (!PugiGetSubNodeValue(RewardItem_Element, "minute_drop_count", minute_drop_count) || minute_drop_count < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->rest_reward->reward_item->minute_drop_count: %d]", path.c_str(), minute_drop_count);
				*err = errinfo;
				return false;
			}
			m_reward[level].minute_drop_count = minute_drop_count;
		}

		RewardItem_Element = RewardItem_Element.next_sibling();
	}

	return true;
}

bool RestConfig::GetRestReward(int level, long long *exp, int *xianhun, int *yuanli, int *mojing)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL) return false;

	if (NULL != exp) *exp = m_reward[level].exp;
	if (NULL != xianhun) *xianhun = m_reward[level].xianhun;
	if (NULL != yuanli) *yuanli = m_reward[level].yuanli;
	if (NULL != mojing) *mojing = m_reward[level].mojing;
	return true;
}

const RestConfig::RewardItem* RestConfig::GetRestConfig(int level) const
{
	if (level <= 0 || level > MAX_ROLE_LEVEL)
		return nullptr;

	return &m_reward[level];
}