#include "huguozhiliconfig.hpp"

bool HuguozhiliConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	// 天神护体其他
	LOAD_CONFIG("other", InitOtherCfg);
	// 天神护体激活配置
	LOAD_CONFIG("huguozhili_active", InitActiveCfg);

	return true;
}

int HuguozhiliConfig::InitOtherCfg(TiXmlElement *root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -1000;
	}

	while (nullptr != data_element)
	{
		if (!GetSubNodeValue(data_element, "injure_maxhp_per", m_other_cfg.injure_maxhp_per) || m_other_cfg.injure_maxhp_per < 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "buff_interval_s", m_other_cfg.buff_interval_s) || m_other_cfg.buff_interval_s <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "buff_cd_s", m_other_cfg.buff_cd_s) || m_other_cfg.buff_cd_s < 0)
		{
			return -3;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int HuguozhiliConfig::InitActiveCfg(TiXmlElement *root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -1000;
	}

	int last_need_death_times = 0;
	while (nullptr != data_element)
	{
		HuguozhiliActiveConfig cfg;
		if (!GetSubNodeValue(data_element, "can_active_times", cfg.can_active_times) || cfg.can_active_times <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "need_die_times", cfg.need_die_times) ||
			cfg.need_die_times <= 0 || last_need_death_times >= cfg.need_die_times)
		{
			return -2;
		}
		last_need_death_times = cfg.need_die_times;

		m_active_cfglist[cfg.can_active_times] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

const HuguozhiliActiveConfig * HuguozhiliConfig::GetActiveConfig(short exchange_times)
{
	if (exchange_times <= 0)
	{
		return nullptr;
	}

	std::map<int, HuguozhiliActiveConfig>::iterator iter = m_active_cfglist.find(exchange_times);
	if (iter != m_active_cfglist.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

int HuguozhiliConfig::GetCanActiveTimes(int die_times)
{
	int max_can_active_times = 0;
	for (std::map<int, HuguozhiliActiveConfig>::iterator iter = m_active_cfglist.begin();
		m_active_cfglist.end() != iter; ++iter)
	{
		if (die_times >= iter->second.need_die_times)
		{
			max_can_active_times = iter->second.can_active_times;
		}
	}
	return max_can_active_times;
}
