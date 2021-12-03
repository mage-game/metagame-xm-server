#include "mojieskillconfig.hpp"
#include "servercommon/configcommon.h"

MojieSkillConfig::MojieSkillConfig()
{

}

MojieSkillConfig::~MojieSkillConfig()
{

}

bool MojieSkillConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("active_cfg", InitActiveCfg);
	PUGI_XML_LOAD_CONFIG("skill_cfg", InitSkillCfg);

	return true;
}

void MojieSkillConfig::SetActiveSkillFlag(unsigned long long &flag, int pata_layer)
{
	for (const auto &skill : m_active_cfg_list)
	{
		if (pata_layer >= skill.pata_layer)
		{
			flag |= (LL_ONE_BIT << skill.skill_id);
		}
	}
}

const MojieSkillCfg * MojieSkillConfig::GetSkillCfg(int skill_id)
{
	if (skill_id < 0 || skill_id >= MOJIE_SKILL_COUNT)
	{
		return nullptr;
	}

	return &m_skill_cfg_list[skill_id];
}

const int MojieSkillConfig::GetSkillTypeByLayer(int layer)
{
	int skill_id = -1;
	for (const auto &skill : m_active_cfg_list)
	{
		if (layer >= skill.pata_layer)
		{
			skill_id = skill.skill_id;
		}
	}

	if (skill_id < 0 || skill_id >= MOJIE_SKILL_COUNT)
	{
		return -1;
	}

	return m_skill_cfg_list[skill_id].skill_type;
}

int MojieSkillConfig::InitActiveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_skill_id = -1;
	while (!dataElement.empty())
	{
		int skill_id = -1;
		if (!PugiGetSubNodeValue(dataElement, "skill_id", skill_id) || skill_id < 0 || skill_id >= MOJIE_SKILL_COUNT || last_skill_id + 1 != skill_id)
		{
			return -1;
		}

		MojieSkillActiveCfg &active_cfg = m_active_cfg_list[skill_id];
		active_cfg.skill_id = skill_id;

		last_skill_id = active_cfg.skill_id;

		if (!PugiGetSubNodeValue(dataElement, "pata_layer", active_cfg.pata_layer) || active_cfg.pata_layer <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MojieSkillConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_skill_id = -1;
	while (!dataElement.empty())
	{
		int skill_id = -1;
		if (!PugiGetSubNodeValue(dataElement, "skill_id", skill_id) || skill_id < 0 || skill_id >= MOJIE_SKILL_COUNT || last_skill_id + 1 != skill_id)
		{
			return -1;
		}

		MojieSkillCfg &skill_cfg = m_skill_cfg_list[skill_id];
		skill_cfg.skill_id = skill_id;
		last_skill_id = skill_cfg.skill_id;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", skill_cfg.skill_type) || skill_cfg.skill_type < 0 || skill_cfg.skill_type >= SKILL_TYPE_MAX)
		{
			return -2;
		}

		for (int i = 0; i < MojieSkillCfg::MAX_PARAM_COUNT; ++i)
		{
			char cfg_string[32] = { 0 };

			SNPRINTF(cfg_string, sizeof(cfg_string), "param_%d", i);

			if (!PugiGetSubNodeValue(dataElement, cfg_string, skill_cfg.param_list[i]) ||
				(skill_cfg.param_list[i] < 0))
			{
				return -100 - i;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	// 这个技能类型2和技能类型8都是减伤效果，如果大于100%，暴击时会出现给人加血的情况，直接在配置处屏蔽
	//if (!this->CheckSkillParam())
	//{
	//	return -1000;
	//}

	return 0;
}

bool MojieSkillConfig::CheckSkillParam()
{
// 	for (int id_1 = 0; id_1 < MOJIE_SKILL_COUNT; ++id_1)
// 	{
// 		if (m_skill_cfg_list[id_1].skill_type == SKILL_TYPE_2)
// 		{
// 			for (int id_2 = 0; id_2 < MOJIE_SKILL_COUNT; ++id_2)
// 			{
// 				if (m_skill_cfg_list[id_2].skill_type != SKILL_TYPE_8) continue;
// 				if (m_skill_cfg_list[id_1].param_list[MojieSkillCfg::SKILL_PARAM_0] + m_skill_cfg_list[id_2].param_list[MojieSkillCfg::SKILL_PARAM_0] > 100 ||
// 					m_skill_cfg_list[id_1].param_list[MojieSkillCfg::SKILL_PARAM_1] + m_skill_cfg_list[id_2].param_list[MojieSkillCfg::SKILL_PARAM_0] > 100)
// 					return false;
// 			}
// 		}
// 	}
	return true;
}