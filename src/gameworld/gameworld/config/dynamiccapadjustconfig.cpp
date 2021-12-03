#include "dynamiccapadjustconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/crossroleglobaldef.hpp"

DynamicCapAdjustConfig::DynamicCapAdjustConfig()
{
	
}

DynamicCapAdjustConfig::~DynamicCapAdjustConfig()
{
	
}

bool DynamicCapAdjustConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("cap_adjust", InitCapAdjustCfg);

	return true;
}

int DynamicCapAdjustConfig::GetSubIndex(const CapAdjustKeyObj &key)
{
	auto it = m_cap_adjust_map.find(key);
	if (it != m_cap_adjust_map.end())
	{
		return it->second;
	}

	return -1;
}

int DynamicCapAdjustConfig::InitCapAdjustCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_sub_index = -1;
	int last_cap_type = -1;
	while (!dataElement.empty())
	{
		CapAdjustKeyObj key;
		if (!PugiGetSubNodeValue(dataElement, "cap_type", key.cap_type) || key.cap_type < ADJUST_CAP_TYPE_TITLE || key.cap_type >= ADJUST_CAP_TYPE_COUNT)
		{
			return -1;
		}

		if (last_cap_type != key.cap_type)
		{
			last_sub_index = -1;
		}
		last_cap_type = key.cap_type;

		if (!PugiGetSubNodeValue(dataElement, "param_0", key.param_0) || key.param_0 < 0)
		{
			return -2;
		}

		int param_1 = 0;
		if (PugiGetSubNodeValue(dataElement, "param_1", param_1))
		{
			key.param_1 = param_1;
		}
		else
		{
			key.param_1 = 0;
		}

		int sub_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "sub_index", sub_index) || sub_index < 0 || sub_index != last_sub_index + 1)	//保证从0开始递增
		{
			return -3;
		}
		last_sub_index = sub_index;

		if (!m_cap_adjust_map.emplace(std::make_pair(key, sub_index)).second)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}