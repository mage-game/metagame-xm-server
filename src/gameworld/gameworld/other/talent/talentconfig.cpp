#include "talentconfig.hpp"
#include "talent.hpp"
#include "item/itempool.h"

TalentConfig::TalentConfig()
{

}

TalentConfig::~TalentConfig()
{

}

bool TalentConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	TiXmlDocument document;
	if ("" == path || !document.LoadFile(path))
	{
		sprintf(err_info, "%s load TalentConfig fail !\n %s", path.c_str(), document.ErrorDesc());
		*err = err_info;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + " xml root node error";
		return false;
	}


	{
		TiXmlElement *root_element = RootElement->FirstChildElement("talent_level_max");
		if (NULL == root_element)
		{
			*err = path + " xml not talent_level_max node ";
			return false;
		}

		iRet = this->InitTalentLevelMaxCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitTalentLevelMaxCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("talent_level_cfg");
		if (NULL == root_element)
		{
			*err = path + " xml not talent_level_cfg node ";
			return false;
		}

		iRet = this->InitTalentLevelCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitTalentLevelCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("level_add_talent_point");
		if (NULL == root_element)
		{
			*err = path + " xml not level_add_talent_point node ";
			return false;
		}

		iRet = this->InitLevelAddTalentPointCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLevelAddTalentPointCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("zhuanzhi_add_talent_point");
		if (NULL == root_element)
		{
			*err = path + " xml not zhuanzhi_add_talent_point node ";
			return false;
		}

		iRet = this->InitZhuanzhiLvAddTalentPointCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiLvAddTalentPointCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ÔÓÏîÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("other");
		if (NULL == root_element)
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}


int TalentConfig::InitTalentLevelMaxCfg(TiXmlElement * RootElement)
{
	int cfg_count = 0;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		TalentLevelMaxConifg &cfg = m_talent_level_max_cfg[cfg_count];

		if (!GetSubNodeValue(dataElement, "talent_id", cfg.talent_id) || cfg.talent_id <= 0)
		{
			return -1;
		}

		if (cfg.talent_id / 100 <= TALENT_TYPE_INVALID || cfg.talent_id / 100 >= TALENT_TYPE_MAX)
		{
			return -2;
		}

		if (cfg.talent_id % 100 < 0 || cfg.talent_id % 100 >= MAX_TELENT_INDEX_COUT)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "max_level", cfg.max_level) || cfg.max_level <= 0 || cfg.max_level > ROLE_TALENT_LEVEL_MAX)
		{
			return -4;
		}

		cfg_count++;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int TalentConfig::InitTalentLevelCfg(TiXmlElement * RootElement)
{
	int last_talent_id = 0;
	int last_talent_level = 0;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int talent_id = 0;
		int talent_level = 0;

		if (!GetSubNodeValue(dataElement, "talent_id", talent_id) || talent_id <= 0)
		{
			return -1;
		}

		if (talent_id / 100 <= TALENT_TYPE_INVALID || talent_id / 100 >= TALENT_TYPE_MAX)
		{
			return -2;
		}

		if (talent_id % 100 < 0 || talent_id % 100 >= MAX_TELENT_INDEX_COUT)
		{
			return -3;
		}

		const int max_talent_level = this->GetTalentLevelMax(talent_id);
		if (max_talent_level <= 0)
		{
			return -1000;
		}

		if (!GetSubNodeValue(dataElement, "talent_level", talent_level) || talent_level <= 0 || talent_level > max_talent_level)
		{
			return -4;
		}

		if (last_talent_id == talent_id && talent_level != last_talent_level + 1)
		{
			return -5;
		}

		if (last_talent_id != talent_id && talent_level != 1)
		{
			return -6;
		}

		int talent_type = 0;
		if (!GetSubNodeValue(dataElement, "talent_type", talent_type) || talent_type != talent_id / 100)
		{
			return -7;
		}

		last_talent_id = talent_id;
		last_talent_level = talent_level;

		TalentLevelConifg cfg;

		cfg.talent_id = talent_id;
		cfg.talent_level = talent_level;
		cfg.talent_type = talent_type;

		if (!GetSubNodeValue(dataElement, "pre_talent_type", cfg.pre_talent_type) || cfg.pre_talent_type < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "pre_talent_type_level", cfg.pre_talent_type_level) || cfg.pre_talent_type_level < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "pre_talent_id", cfg.pre_talent_id) || cfg.pre_talent_id < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "pre_talent_level", cfg.pre_talent_level) || cfg.pre_talent_level < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "effect_type", cfg.effect_type) || (TALENT_EFFECT_TYPE_ATTR != cfg.effect_type && TALENT_EFFECT_TYPE_SKILL != cfg.effect_type))
		{
			return -12;
		}

		if (TALENT_EFFECT_TYPE_ATTR == cfg.effect_type)
		{
			for (int i = 1; i <= TalentLevelConifg::TALENT_MAX_ATTR_TYPE; ++i)
			{
				std::string tmp_attr_type;
				short tmp_type = 0;
				int tmp_value = 0;

				char tmp_tag[32];
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "attr_type_%d", i);
				if (!GetSubNodeValue(dataElement, tmp_tag, tmp_attr_type))
				{
					return -13;
				}

				if (!Talent::GetAttrTypeFromAttrString(tmp_attr_type, &tmp_type))
				{
					return -14;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "attr_value_%d", i);
				if (!GetSubNodeValue(dataElement, tmp_tag, tmp_value) || tmp_value < 0)
				{
					return -15;
				}

				cfg.talent_attr_type_list[i - 1] = tmp_type;
				cfg.talent_attr_value_list[i - 1] = tmp_value;
			}
		}
		else if (TALENT_EFFECT_TYPE_SKILL == cfg.effect_type)
		{
			if (!GetSubNodeValue(dataElement, "cool_down", cfg.cool_down) || cfg.cool_down < 0)
			{
				return -17;
			}

			if (!GetSubNodeValue(dataElement, "param_a", cfg.param_a) || cfg.param_a < 0)
			{
				return -19;
			}

			if (!GetSubNodeValue(dataElement, "param_b", cfg.param_b) || cfg.param_b < 0)
			{
				return -20;
			}

			if (!GetSubNodeValue(dataElement, "param_c", cfg.param_c) || cfg.param_c < 0)
			{
				return -21;
			}

			if (!GetSubNodeValue(dataElement, "param_d", cfg.param_d) || cfg.param_d < 0)
			{
				return -22;
			}
		}

		if (!GetSubNodeValue(dataElement, "add_fix_cap", cfg.add_fix_cap) || cfg.add_fix_cap < 0)
		{
			return -23;
		}

		if (!GetSubNodeValue(dataElement, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0)
		{
			return -24;
		}

		long long key = ConvertParamToLongLong(talent_id, talent_level);
		TalentLevelConfigMapIt iter = m_talent_level_cfg_map.find(key);
		if (m_talent_level_cfg_map.end() != iter)
		{
			return -1000;
		}

		m_talent_level_cfg_map[key] = cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}


int TalentConfig::InitLevelAddTalentPointCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		int level = 0, add_talent_point = 0;
		if (!GetSubNodeValue(data_element, "level", level) || level <= 0 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "add_talent_point", add_talent_point) || add_talent_point < 0)
		{
			return -2;
		}

		if (m_lv_add_talent_cfg.end() != m_lv_add_talent_cfg.find(level))
		{
			return -100;
		}

		m_lv_add_talent_cfg[level] = add_talent_point;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}


int TalentConfig::InitZhuanzhiLvAddTalentPointCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		int prof_level = 0, add_talent_point = 0;
		if (!GetSubNodeValue(data_element, "prof_level", prof_level) || prof_level <= 0 || prof_level > MAX_PROF_LEVEL)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "add_talent_point", add_talent_point) || add_talent_point < 0)
		{
			return -2;
		}

		if (m_zhuanzhi_lv_add_talent_cfg.end() != m_zhuanzhi_lv_add_talent_cfg.find(prof_level))
		{
			return -100;
		}

		m_zhuanzhi_lv_add_talent_cfg[prof_level] = add_talent_point;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int TalentConfig::InitOtherCfg(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	if (!GetSubNodeValue(data_element, "open_talent_level", m_other_cfg.open_talent_level) || m_other_cfg.open_talent_level <= 0 || m_other_cfg.open_talent_level > MAX_ROLE_LEVEL)
	{
		return -1;
	}

	if (!GetSubNodeValue(data_element, "base_talent_point", m_other_cfg.base_talent_point) || m_other_cfg.base_talent_point <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(data_element, "reset_consume_item", m_other_cfg.reset_consume_item) || m_other_cfg.reset_consume_item <= 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.reset_consume_item))
	{
		return -3;
	}

	if (!GetSubNodeValue(data_element, "proficient_talent_limit", m_other_cfg.proficient_talent_limit) || m_other_cfg.proficient_talent_limit < 0)
	{
		return -4;
	}

	if (!GetSubNodeValue(data_element, "first_point_consume_gold", m_other_cfg.first_point_consume_gold) || m_other_cfg.first_point_consume_gold < 0)
	{
		return -5;
	}

	return 0;
}

const int TalentConfig::GetTalentLevelMax(int talent_id) const
{
	for (int i = 0; i < TALENT_LEVEL_MAX_CONFIG_COUNT; i++)
	{
		if (talent_id == m_talent_level_max_cfg[i].talent_id)
		{
			return m_talent_level_max_cfg[i].max_level;
		}
	}

	return 0;
}

const TalentLevelConifg * TalentConfig::GetTalentLevelCfg(int talent_id, int talent_level) const
{
	if (talent_id / 100 <= TALENT_TYPE_INVALID || talent_id / 100 >= TALENT_TYPE_MAX || talent_id % 100 < 0 || talent_id % 100 >= MAX_TELENT_INDEX_COUT)
	{
		return NULL;
	}

	const int talent_level_max = this->GetTalentLevelMax(talent_id);
	if (talent_level <= 0 || talent_level > talent_level_max)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(talent_id, talent_level);
	TalentLevelConfigMapIt iter = m_talent_level_cfg_map.find(key);
	if (m_talent_level_cfg_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

int TalentConfig::GetLvAddTalentPoint(int lv)
{
	if (lv <= 0 || lv > MAX_ROLE_LEVEL)
	{
		return 0;
	}

	auto it = m_lv_add_talent_cfg.find(lv);
	if (it != m_lv_add_talent_cfg.end())
	{
		return it->second;
	}

	return 0;
}

int TalentConfig::GetZhuanzhiLvAddTalentPoint(int zhuanzhi_lv)
{
	if (zhuanzhi_lv <= 0 || zhuanzhi_lv > MAX_PROF_LEVEL)
	{
		return 0;
	}

	auto it = m_zhuanzhi_lv_add_talent_cfg.find(zhuanzhi_lv);
	if (it != m_zhuanzhi_lv_add_talent_cfg.end())
	{
		return it->second;
	}

	return 0;
}
