#include "goldturntableconfig.hpp"

GoldTurntableConfig::GoldTurntableConfig() : m_total_weight(0), m_reward_pool_count(0)
{
}

GoldTurntableConfig::~GoldTurntableConfig()
{
}

bool GoldTurntableConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GoldTurntableConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = InitOtherConfig(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherConfig failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode pool_element = RootElement.child("reward_pool");
		if (pool_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = InitRewardPoolConfig(pool_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardPoolConfig failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
		return true;
}

int GoldTurntableConfig::InitOtherConfig(PugiXmlNode element)
{
	PugiXmlNode dataElement = element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_transform", m_other_cfg.per_transform) || m_other_cfg.per_transform <= 0 || m_other_cfg.per_transform > 100)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "sigle_cj_zhuanshi", m_other_cfg.sigle_cj_gold) || m_other_cfg.sigle_cj_gold < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "baodi", m_other_cfg.min_pool) || m_other_cfg.min_pool <= 0)
	{
		return -3;
	}

	return 0;
}

int GoldTurntableConfig::InitRewardPoolConfig(PugiXmlNode element)
{
	PugiXmlNode dataElement = element.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_reward_pool_count = 0;
	while (!dataElement.empty())
	{
		if (m_reward_pool_count < 0 || m_reward_pool_count >= GOLD_TURNTABLE_POOL_COUNT)
		{
			return -1000;
		}

		int index = -1;
		if (!PugiGetSubNodeValue(dataElement, "index", index) || index != m_reward_pool_count)
		{
			return -1;
		}

		GoldTurntableRewardPoolCfg &cfg = m_reward_pool_cfg_list[m_reward_pool_count];
		cfg.index = index;

		PugiXmlNode item_element = dataElement.child("item");
		if (item_element.empty() || (!cfg.item.ReadConfig(item_element) && 0 != cfg.item.item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "zhuanshi", cfg.rewrad_gold) || cfg.rewrad_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianchi_per", cfg.jianchi_per) || cfg.jianchi_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -5;
		}
		m_total_weight += cfg.weight;

		if (!PugiGetSubNodeValue(dataElement, "broadcast", cfg.broadcast) || cfg.broadcast < 0)
		{
			return -6;
		}

		if (0 == cfg.rewrad_gold && 0 == cfg.jianchi_per && 0 == cfg.item.item_id)
		{
			return -7;
		}

		m_reward_pool_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

GoldTurntableRewardPoolCfg *GoldTurntableConfig::RandRewardCfg()
{
	if (m_total_weight <= 0)
	{
		return NULL;
	}

	int rand_num = RandomNum(m_total_weight);

	for (int i = 0; i < m_reward_pool_count && i < GOLD_TURNTABLE_POOL_COUNT; ++i)
	{
		if (rand_num < m_reward_pool_cfg_list[i].weight)
		{
			return &m_reward_pool_cfg_list[i];
		}

		rand_num -= m_reward_pool_cfg_list[i].weight;
	}

	return NULL;
}
