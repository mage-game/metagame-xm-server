#include "upgraderushconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

UpgradeRushConfig::UpgradeRushConfig()
{
	
}

UpgradeRushConfig::~UpgradeRushConfig()
{
	
}

bool UpgradeRushConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "UpgradeRushConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode element = RootElement.child("upgrade_rush_return");
		if (element.empty())
		{
			*err = configname + ": no [upgrade_rush_return].";
			return false;
		}

		iRet = InitRushReturnCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRushRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int UpgradeRushConfig::InitRushReturnCfg(PugiXmlNode RootElement)
{
	int last_upgrade_type = 0;
	int last_grade = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int upgrade_type = 0;
		int grade = 0;

		if (!PugiGetSubNodeValue(dataElement, "upgrade_type", upgrade_type)|| upgrade_type <= UPGRADE_RUSH_TYPE_INVALID || upgrade_type >= UPGRADE_RUSH_TYPE_MAX)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "grade", grade)|| grade <= 0)
		{
			return -2;
		}

		if (last_upgrade_type == upgrade_type && grade != last_grade + 1)
		{
			return 101;
		}
		last_upgrade_type = upgrade_type;
		last_grade = grade;

		UpgradeRushReturnCfg cfg;
		cfg.upgrade_type = upgrade_type;
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "limit_time", cfg.limit_time) || cfg.limit_time < 0)
		{
			return -3;
		}
		
		PugiXmlNode reward_item_element = dataElement.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (cfg.reward_item.ReadConfig(reward_item_element))
		{
			cfg.has_reward = true;
		}

		long long key = ConvertParamToLongLong(upgrade_type, grade);
		std::map<long long, UpgradeRushReturnCfg>::const_iterator iter = m_upgrade_rush_reward_map.find(key);
		if (m_upgrade_rush_reward_map.end() != iter) 
		{
			return -100;
		}

		m_upgrade_rush_reward_map[key] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const UpgradeRushReturnCfg * UpgradeRushConfig::GetUpgradeRushReturnCfg(int upgrade_type, int grade) const
{
	if (upgrade_type <= UPGRADE_RUSH_TYPE_INVALID || upgrade_type >= UPGRADE_RUSH_TYPE_MAX)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(upgrade_type, grade);
	std::map<long long, UpgradeRushReturnCfg>::const_iterator iter = m_upgrade_rush_reward_map.find(key);
	if (iter != m_upgrade_rush_reward_map.end())
	{
		return &iter->second;
	}

	return NULL;
}
