#include "jinglingadvantageconfig.hpp"

#include "servercommon/configcommon.h"

#include <algorithm>

#include "config/logicconfigmanager.hpp"
#include "config/monsterreplaceconfig.hpp"

JinglingAdvantageConfig::JinglingAdvantageConfig() : m_blue_gather_total_weight(0), m_purple_gather_total_weight(0)
{

}

JinglingAdvantageConfig::~JinglingAdvantageConfig()
{

}

bool JinglingAdvantageConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "JinglingAdvantageConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode element = RootElement.child("other");
		if (element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitJinglingAdvantageOtherCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitJinglingAdvantageOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 采集物
		PugiXmlNode element = RootElement.child("gather_info");
		if (element.empty())
		{
			*err = configname + ": no [gather_info].";
			return false;
		}

		iRet = this->InitJinglingAdvantageGatherCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitJinglingAdvantageGatherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他
		PugiXmlNode element = RootElement.child("gather_pos");
		if (element.empty())
		{
			*err = configname + ": no [gather_pos].";
			return false;
		}

		iRet = this->InitJinglingAdvantageGatherPosCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitJinglingAdvantageGatherPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// boss配置
		PugiXmlNode element = RootElement.child("boss_cfg");
		if (NULL == element)
		{
			*err = configname + ": no [boss_cfg].";
			return false;
		}

		iRet = this->InitJinglingAdvantageBossCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitJinglingAdvantageBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const JinglingAdvantageGatherCfg * JinglingAdvantageConfig::GetJinglingAdvantageRandGatherCfg(int type)
{
	int rand_weight = 0;
	switch (type)
	{
	case JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE:
		{
			if (m_blue_gather_total_weight <= 0)
			{
				return NULL;
			}
			rand_weight = RandomNum(m_blue_gather_total_weight);
		}
		break;

	case JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_PUEPLE:
		{
			if (m_purple_gather_total_weight <= 0)
			{
				return NULL;
			}
			rand_weight = RandomNum(m_purple_gather_total_weight);
		}
		break;

	default:
		return NULL;
	}

	for (std::map<int, JinglingAdvantageGatherCfg>::iterator it = m_jingling_gather_cfg_map.begin();
	it != m_jingling_gather_cfg_map.end(); ++it)
	{
		if (it->second.gather_type != type)
		{
			continue;
		}

		if (rand_weight < it->second.weight)
		{
			return &(it->second);
		}
		else
		{
			rand_weight -= it->second.weight;
		}
	}

	return NULL;
}

const JinglingAdvantageGatherCfg * JinglingAdvantageConfig::GetJinglingAdvantageGatherCfg(int gather_id)
{
	std::map<int, JinglingAdvantageGatherCfg>::iterator it = m_jingling_gather_cfg_map.find(gather_id);
	if (it != m_jingling_gather_cfg_map.end())
	{
		return &(it->second);
	}

	return NULL;
}

std::vector<JinglingAdvantageGatherPosCfg> & JinglingAdvantageConfig::GetJinglingAdvantageGatherRandPosCfg()
{
	return m_jingling_gather_pos_cfg_vec;
}

const JinglingAdvantageBossCfg * JinglingAdvantageConfig::GetJinglingAdvantageBossCfg(int vip_level)
{
	for (const auto &cfg : m_boss_cfg_vec)
	{
		if (vip_level >= cfg.vip_level_min && vip_level <= cfg.vip_level_max)
		{
			return &cfg;
		}
	}

	return nullptr;
}

bool JinglingAdvantageConfig::IsJingLingAdvantageBoss(unsigned short boss_id)
{
	unsigned short origin_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetOrginalWorldLevMonsterId(boss_id);
	for (const auto &cfg : m_boss_cfg_vec)
	{
		if (cfg.monster_id == origin_boss_id)
		{
			return true;
		}
	}

	return false;
}

int JinglingAdvantageConfig::InitJinglingAdvantageOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "times", m_other_cfg.times) || m_other_cfg.times <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_num", m_other_cfg.max_num) || m_other_cfg.max_num <= 0 || m_other_cfg.max_num > JINGLING_ADVANTAGE_MAX_EGG_PER_SCENE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_num", m_other_cfg.min_num) || m_other_cfg.min_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "check_time_interval", m_other_cfg.check_time_interval) || m_other_cfg.check_time_interval <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", m_other_cfg.rate) || m_other_cfg.rate <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time", m_other_cfg.gather_time) || m_other_cfg.gather_time <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_level_limit", m_other_cfg.gather_level_limit) || m_other_cfg.gather_level_limit <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "skip_gather_consume", m_other_cfg.skip_gather_consume) || m_other_cfg.skip_gather_consume <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "skip_limit_level", m_other_cfg.skip_level_limit) || m_other_cfg.skip_level_limit <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_max_drop_times", m_other_cfg.boss_max_drop_times) || m_other_cfg.boss_max_drop_times <= 0)
		{
			return -10;
		}
	}

	return 0;
}

int JinglingAdvantageConfig::InitJinglingAdvantageGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		JinglingAdvantageGatherCfg gather_cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", gather_cfg.seq) || gather_cfg.seq < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_type", gather_cfg.gather_type))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather_cfg.gather_id) || gather_cfg.gather_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", gather_cfg.weight) || gather_cfg.weight <= 0)
		{
			return -4;
		}

		PugiXmlNode item_element = dataElement.child("reward_item");
		if (NULL != item_element)
		{
			if (!gather_cfg.reward_item.ReadConfig(item_element))
			{
				return -5;
			}
		}

		switch (gather_cfg.gather_type)
		{
		case JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE:
			{
				m_blue_gather_total_weight += gather_cfg.weight;
			}
			break;
		case JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_PUEPLE:
			{
				m_purple_gather_total_weight += gather_cfg.weight;
			}
			break;

		default:
			return -6;
		}

		m_jingling_gather_cfg_map[gather_cfg.gather_id] = gather_cfg;
		dataElement = dataElement.next_sibling();
	}

	if (m_blue_gather_total_weight <= 0)
	{
		return -7;
	}

	if (m_purple_gather_total_weight <= 0)
	{
		return -8;
	}

	return 0;
}

int JinglingAdvantageConfig::InitJinglingAdvantageGatherPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		JinglingAdvantageGatherPosCfg pos_cfg;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", pos_cfg.scene_id) || pos_cfg.scene_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_cfg.pos.x) || pos_cfg.pos.x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_cfg.pos.y) || pos_cfg.pos.y < 0)
		{
			return -3;
		}

		m_jingling_gather_pos_cfg_vec.push_back(pos_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JinglingAdvantageConfig::InitJinglingAdvantageBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		JinglingAdvantageBossCfg boss_cfg;

		if (!PugiGetSubNodeValue(dataElement, "vip_level_min", boss_cfg.vip_level_min) || boss_cfg.vip_level_min < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "vip_level_max", boss_cfg.vip_level_max) || boss_cfg.vip_level_max < boss_cfg.vip_level_min)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", boss_cfg.monster_id) || boss_cfg.monster_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", boss_cfg.rate) || boss_cfg.rate <= 0)
		{
			return -4;
		}

		m_boss_cfg_vec.push_back(boss_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}