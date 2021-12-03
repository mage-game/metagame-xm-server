#include "upgradeskillconfig.hpp"
#include "servercommon/configcommon.h"

UpGradeSkillConfig::UpGradeSkillConfig()
{
	
}

UpGradeSkillConfig::~UpGradeSkillConfig()
{

}

bool UpGradeSkillConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "UpGradeSkillConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("gauge_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not gauge_cfg node ";
			return false;
		}

		iRet = this->InitGaugeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGaugeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("skill_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not skill_cfg node ";
			return false;
		}

		iRet = this->InitSkillLevelCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSkillLevelCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int UpGradeSkillConfig::InitGaugeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int skill_count;
		if (!PugiGetSubNodeValue(data_element, "skill_count", skill_count) || skill_count <= 0 || skill_count > UpGradeSkillConfig::SKILL_TYPE_MAX)
		{
			return -1;
		}

		if (m_gauge_map.find(skill_count) != m_gauge_map.end())
		{
			return -2;
		}

		int gauge = 0;
		if (!PugiGetSubNodeValue(data_element, "gauge", gauge) || gauge <= 0)
		{
			return -3;
		}

		m_gauge_map[skill_count] = gauge;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int UpGradeSkillConfig::InitSkillLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int skill_type = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_type", skill_type) || skill_type < UpGradeSkillConfig::SKILL_TYPE_MOUNT || skill_type >= UpGradeSkillConfig::SKILL_TYPE_MAX)
		{
			return -1;
		}

		std::map<int, UpGradeSkillLevelCfg> &skill_cfg = m_upgrade_skill_map[skill_type];
		UpGradeSkillLevelCfg level_cfg;
		level_cfg.skill_type = skill_type;

		if (!PugiGetSubNodeValue(data_element, "skill_level", level_cfg.skill_level) || level_cfg.skill_level <= 0)
		{
			return -2;
		}

		if (skill_cfg.find(level_cfg.skill_level) != skill_cfg.end())
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "injure_per", level_cfg.injure_per) || level_cfg.injure_per <= 0)
		{
			return -4;
		}

		skill_cfg[level_cfg.skill_level] = level_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int UpGradeSkillConfig::GetGaugeBySkillCount(int skill_count)
{
	std::map<int, int>::iterator iter = m_gauge_map.find(skill_count);
	if (iter != m_gauge_map.end())
	{
		return iter->second;
	}

	return 0;
}

UpGradeSkillLevelCfg * UpGradeSkillConfig::GetSkillLevelCfg(int skill_type, int skill_level)
{
	if (skill_type < UpGradeSkillConfig::SKILL_TYPE_MOUNT || skill_type >= UpGradeSkillConfig::SKILL_TYPE_MAX || skill_level <= 0) return NULL;

	std::map<int, UpGradeSkillLevelCfg> &skill_cfg = m_upgrade_skill_map[skill_type];

	std::map<int, UpGradeSkillLevelCfg>::iterator iter = skill_cfg.find(skill_level);
	if (iter != skill_cfg.end() && iter->second.skill_type == skill_type && iter->second.skill_level == skill_level)
	{
		return &iter->second;
	}

	return NULL;
}
