#include "huashenconfig.hpp"
#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"

HuaShenConfig::HuaShenConfig() : m_huashen_max_id(0), m_huashen_spirit_max_id(0)
{
}
HuaShenConfig::~HuaShenConfig()
{
}

bool HuaShenConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "HuaShenConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 化神信息配置
		PugiXmlNode huashen_info_element = RootElement.child("huashen_info");
		if (huashen_info_element.empty())
		{
			*err = configname + ": no [huashen_info].";
			return false;
		}

		iRet = this->InitHuaShenInfoCfg(huashen_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHuaShenInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{	
		// 化神等级配置
		PugiXmlNode huashen_level_element = RootElement.child("huashen_level");
		if (huashen_level_element.empty())
		{
			*err = configname + ": no [huashen_level].";
			return false;
		}

		iRet = this->InitHuaShenLevelCfg(huashen_level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHuaShenLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{	
		// 化神精灵配置
		PugiXmlNode root_element = RootElement.child("huashen_spirit");
		if (root_element.empty())
		{
			*err = configname + ": no [huashen_spirit].";
			return false;
		}

		iRet = this->InitHuaShenSpiritCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHuaShenSpiritCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{	
		// 化神进阶配置
		PugiXmlNode root_element = RootElement.child("huashen_grade");
		if (root_element.empty())
		{
			*err = configname + ": no [huashen_grade].";
			return false;
		}

		iRet = this->InitHuaShenGradeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitHuaShenGradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int HuaShenConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "level_once_val", m_other_cfg.level_once_val) || m_other_cfg.level_once_val <= 0)
	{
		return -1;
	}

	return 0;
}

int HuaShenConfig::InitHuaShenInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	m_huashen_max_id = -1;
	while (!data_element.empty())
	{
		if (m_huashen_max_id >= HUASHEN_MAX_ID_LIMIT)
		{
			return -20;
		}

		int id = 0;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id != m_huashen_max_id + 1)
		{
			return -1;
		}

		HuaShenInfoConfig &info_cfg = m_huashen_info_list[id];
		info_cfg.id = id;
		m_huashen_max_id = id;

		if (!PugiGetSubNodeValue(data_element, "monster_id", info_cfg.monster_id) || NULL == MONSTERPOOL->GetMonsterParam(info_cfg.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", info_cfg.maxhp) || info_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", info_cfg.gongji) || info_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", info_cfg.fangyu) || info_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", info_cfg.mingzhong) || info_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", info_cfg.shanbi) || info_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HuaShenConfig::InitHuaShenLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id > m_huashen_max_id || id > HUASHEN_MAX_ID_LIMIT)
		{
			return -1;
		}

		HuaShenInfoConfig &huashen_cfg = m_huashen_info_list[id];
		if (huashen_cfg.max_level >= HUASHEN_MAX_LEVEL_LIMIT)
		{
			return -2;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level <= 0 || level != huashen_cfg.max_level + 1 || level > HUASHEN_MAX_LEVEL_LIMIT)
		{
			return -3;
		}

		HuaShenLevelConfig &level_cfg = huashen_cfg.level_list[level];
		level_cfg.level = level;
		huashen_cfg.max_level = level;

		if (!PugiGetSubNodeValue(data_element, "jinhua_val", level_cfg.jinhua_val) || level_cfg.jinhua_val < m_other_cfg.level_once_val)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_id", level_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", level_cfg.stuff_num) || level_cfg.stuff_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int HuaShenConfig::InitHuaShenSpiritCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int last_id = 0;

	while (!data_element.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(data_element, "id", id) || (id != last_id && id != last_id + 1))
		{
			return -1;
		}

		if (id > HUASHEN_SPIRIT_MAX_ID_LIMIT)
		{
			return -1000;
		}

		m_huashen_spirit_cfg_list[id].id = id;
		last_id = id;
		m_huashen_spirit_max_id = id;

		if (m_huashen_spirit_cfg_list[id].max_level >= HUASHEN_SPIRIT_MAX_LEVEL)
		{
			return -2000;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_huashen_spirit_cfg_list[id].max_level + 1)
		{
			return -2;
		}

		HuaShenSpiritConfig::SpiritLevelConfig &level_cfg = m_huashen_spirit_cfg_list[id].level_cfg_list[level];
		level_cfg.level = level;
		m_huashen_spirit_cfg_list[id].max_level = level;

		if (!PugiGetSubNodeValue(data_element, "consume_item_id", level_cfg.consume_item_id) || NULL == ITEMPOOL->GetItem(level_cfg.consume_item_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_item_count", level_cfg.consume_item_count) || level_cfg.consume_item_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "add_exp_val", level_cfg.add_exp_val) || level_cfg.add_exp_val <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "need_exp_val", level_cfg.need_exp_val) || level_cfg.need_exp_val <= 0 || (0 != level_cfg.need_exp_val % level_cfg.add_exp_val))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HuaShenConfig::InitHuaShenGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id > m_huashen_max_id || id > HUASHEN_MAX_ID_LIMIT)
		{
			return -1;
		}

		HuaShenInfoConfig &huashen_cfg = m_huashen_info_list[id];
		if (huashen_cfg.max_grade >= HUASHEN_MAX_GRADE_LIMIT)
		{
			return -2;
		}

		int grade = 0;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade <= 0 || grade != huashen_cfg.max_grade + 1 || grade > HUASHEN_MAX_GRADE_LIMIT)
		{
			return -3;
		}

		HuaShenGradeConfig &grade_cfg = huashen_cfg.grade_list[grade];
		grade_cfg.grade = grade;
		huashen_cfg.max_grade = grade;

		if (!PugiGetSubNodeValue(data_element, "stuff_id", grade_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(grade_cfg.stuff_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", grade_cfg.stuff_num) || grade_cfg.stuff_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", grade_cfg.maxhp) || grade_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", grade_cfg.gongji) || grade_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", grade_cfg.fangyu) || grade_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", grade_cfg.mingzhong) || grade_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", grade_cfg.shanbi) || grade_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const HuaShenInfoConfig* HuaShenConfig::GetHuaShenInfoCfg(int id)
{
	if (id < 0 || id > m_huashen_max_id || id > HUASHEN_MAX_ID_LIMIT)
	{
		return NULL;
	}

	return &m_huashen_info_list[id];
}

const HuaShenLevelConfig* HuaShenConfig::GetHuaShenLevelCfg(int id, int level)
{
	if (id < 0 || id > m_huashen_max_id || id > HUASHEN_MAX_ID_LIMIT)
	{
		return NULL;
	}

	HuaShenInfoConfig &huashen_cfg = m_huashen_info_list[id];

	if (level < 1 || level > huashen_cfg.max_level || level > HUASHEN_MAX_LEVEL_LIMIT)
	{
		return NULL;
	}

	return &huashen_cfg.level_list[level];
}

int HuaShenConfig::GetHuaShenSpiritMaxLevel(int spirit_id)
{
	if (spirit_id < 0 || spirit_id > HUASHEN_SPIRIT_MAX_ID_LIMIT || spirit_id > m_huashen_spirit_max_id)
	{
		return 0;
	}

	return m_huashen_spirit_cfg_list[spirit_id].max_level;
}

const HuaShenGradeConfig * HuaShenConfig::GetHuaShenGradeCfg(int id, int grade)
{
	if (id < 0 || id > m_huashen_max_id || id > HUASHEN_MAX_ID_LIMIT)
	{
		return NULL;
	}

	HuaShenInfoConfig &huashen_cfg = m_huashen_info_list[id];

	if (grade < 1 || grade > huashen_cfg.max_grade || grade > HUASHEN_MAX_GRADE_LIMIT)
	{
		return NULL;
	}

	return &huashen_cfg.grade_list[grade];
}

const HuaShenSpiritConfig::SpiritLevelConfig * HuaShenConfig::GetHuaShenSpiritLevelCfg(int spirit_id, int level)
{
	if (spirit_id < 0 || spirit_id > m_huashen_spirit_max_id || spirit_id > HUASHEN_SPIRIT_MAX_ID_LIMIT)
	{
		return NULL;
	}

	if (level < 0 || level > m_huashen_spirit_cfg_list[spirit_id].max_level || level > HUASHEN_SPIRIT_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_huashen_spirit_cfg_list[spirit_id].level_cfg_list[level];
}


