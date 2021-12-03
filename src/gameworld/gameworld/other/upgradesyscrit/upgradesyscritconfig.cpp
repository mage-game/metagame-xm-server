#include "upgradesyscritconfig.hpp"




UpgradeSysCritConfig::UpgradeSysCritConfig()
{

}

UpgradeSysCritConfig::~UpgradeSysCritConfig()
{

}

bool UpgradeSysCritConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;
	PUGI_XML_LOAD_CONFIG("upgrade_sys_crit", InitUpgradeSysCritConfig);
	return true;
}

int UpgradeSysCritConfig::InitUpgradeSysCritConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}
	int i = 0;
	while (!dataElement.empty())
	{
		if (i >= MAX_UPGRADE_CRIT_TYPE_COUNT)
		{
			return -1;
		}
		/*if (!PugiGetSubNodeValue(dataElement, "upgrade_exp", upgrade_sys_crit[i].upgrade_exp) || upgrade_sys_crit[i].upgrade_exp < 0)
		{
			return -2;
		}*/
		if (!PugiGetSubNodeValue(dataElement, "crit_rate", upgrade_sys_crit[i].crit_rate) || upgrade_sys_crit[i].crit_rate < 0)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "crit_value", upgrade_sys_crit[i].crit_value) || upgrade_sys_crit[i].crit_value < 0)
		{
			return -4;
		}
		++i;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const UpgradeSysCritStruct* UpgradeSysCritConfig::GetUpgradeSysCritConfigByType(int type)
{
	if (type <= UPGRADE_SYS_CRIT_TYPE_INVALID || type >= UPGRADE_SYS_CRIT_TYPE_END)
	{
		return nullptr;
	}
	return &upgrade_sys_crit[type];
}

const int UpgradeSysCritConfig::GetUpgradeSysCritRate(int type)
{
	if (type <= UPGRADE_SYS_CRIT_TYPE_INVALID || type >= UPGRADE_SYS_CRIT_TYPE_END)
	{
		return 0;
	}
	return upgrade_sys_crit[type].crit_rate;
}

const int UpgradeSysCritConfig::GetUpgradeSysCritValue(int type)
{
	if (type <= UPGRADE_SYS_CRIT_TYPE_INVALID || type >= UPGRADE_SYS_CRIT_TYPE_END)
	{
		return 0;
	}
	return upgrade_sys_crit[type].crit_value;
}