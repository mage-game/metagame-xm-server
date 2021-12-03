#include "zhibaoconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

ZhiBaoConfig::ZhiBaoConfig()
{

}

ZhiBaoConfig::~ZhiBaoConfig()
{

}

bool ZhiBaoConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ZhiBaoConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 等级属性
		PugiXmlNode element = RootElement.child("level_attr");
		if (element.empty())
		{
			*err = path + " xml not level_attr node ";
			return false;
		}

		iRet = this->InitLevelCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLevelCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 技能
		PugiXmlNode element = RootElement.child("skill");
		if (element.empty())
		{
			*err = path + " xml not skill node ";
			return false;
		}

		iRet = this->InitSkillCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSkillCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 幻化激活
		PugiXmlNode element = RootElement.child("activity_huanhua");
		if (element.empty())
		{
			*err = path + " xml not activity_huanhua node ";
			return false;
		}

		iRet = this->InitAcitvityHuanHuaCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitAcitvityHuanHuaCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 幻化进阶
		PugiXmlNode element = RootElement.child("huanhua");
		if (element.empty())
		{
			*err = path + " xml not huanhua node ";
			return false;
		}

		iRet = this->InitHuanhuaGradeCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitHuanhuaGradeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}


int ZhiBaoConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_ZHIBAO_LEVEL)
		{
			return -1;
		}

		ZhiBaoLevelCfg &cfg = m_level_cfg_list[level];
		if (0 != cfg.level)
		{
			return -2;
		}

		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_exp", cfg.upleve_exp) || cfg.upleve_exp < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "mount_attr_add", cfg.mount_attr_add) || cfg.mount_attr_add < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "wing_attr_add", cfg.wing_attr_add) || cfg.wing_attr_add < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "image_id", cfg.image_id) || cfg.image_id < 0) return -10;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhiBaoConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= MAX_ZHIBAO_SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level <= 0 || skill_level > MAX_ZHIBAO_SKILL_LEVEL)
		{
			return -2;
		}

		ZhiBaoSkillCfg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		if (0 != cfg.skill_level)
		{
			return -3;
		}

		cfg.skill_idx = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "zhibao_level", cfg.zhibao_level) || cfg.zhibao_level < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a) || cfg.param_a < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b) || cfg.param_b < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c) || cfg.param_c < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_d", cfg.param_d) || cfg.param_d < 0) 
		{
			return -8;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhiBaoConfig::InitAcitvityHuanHuaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_huanhua_cfg.huanhua_count = 0;

	while (!dataElement.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElement, "id", id) || id < 0 || id >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT)
		{
			return -1;
		}

		if (id != m_huanhua_cfg.huanhua_count)
		{
			return -2;
		}

		ZhiBaoHuanhuaCfg::ConfigItem &cfg = m_huanhua_cfg.cfg_list[id];
		cfg.id = id;

		if (!PugiGetSubNodeValue(dataElement, "active_item", cfg.active_item_id) || NULL == ITEMPOOL->GetItem(cfg.active_item_id)) 
		{
			return -10;
		}

		m_huanhua_cfg.huanhua_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhiBaoConfig::InitHuanhuaGradeCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000001;
	}

	while (!dataElement.empty())
	{
		int huanhua_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_type", huanhua_type) || huanhua_type < 0 || huanhua_type >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT)
		{
			return -1;
		}

		int grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= MAX_ZHIBAO_HUANHUA_GRADE_COUNT)
		{
			return -2;
		}

		ZhiBaoHuanhuaGradeCfg::GradeCfg &grade_cfg = m_huanhua_grade_list.m_grade_list[huanhua_type][grade];

		grade_cfg.huanhua_type = huanhua_type;
		grade_cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", grade_cfg.stuff_id) || grade_cfg.stuff_id <= 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_count", grade_cfg.stuff_count) || grade_cfg.stuff_count <= 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", grade_cfg.maxhp) || grade_cfg.maxhp < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", grade_cfg.gongji) || grade_cfg.gongji < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", grade_cfg.fangyu) || grade_cfg.fangyu < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", grade_cfg.mingzhong) || grade_cfg.mingzhong < 0) 
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", grade_cfg.shanbi) || grade_cfg.shanbi < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", grade_cfg.baoji) || grade_cfg.baoji < 0) 
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", grade_cfg.jianren) || grade_cfg.jianren < 0) 
		{
			return -11;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

ZhiBaoLevelCfg * ZhiBaoConfig::GetZhiBaoLevelCfg(int level)
{
	if (level < 0 || level > MAX_ZHIBAO_LEVEL || m_level_cfg_list[level].level != level)
	{
		return NULL;
	}

	return &m_level_cfg_list[level];
}

ZhiBaoSkillCfg * ZhiBaoConfig::GetZhiBaoSkillCfg(int skill_idx, int skill_level)
{
	if (skill_idx < 0 || skill_idx >= MAX_ZHIBAO_SKILL_COUNT || skill_level <= 0 || skill_level > MAX_ZHIBAO_SKILL_LEVEL)
	{
		return NULL;
	}

	if (m_skill_cfg_list[skill_idx][skill_level].skill_level <= 0)
	{
		return NULL;
	}

	return &m_skill_cfg_list[skill_idx][skill_level];
}

ZhiBaoHuanhuaGradeCfg::GradeCfg * ZhiBaoConfig::GetHuanhuaGradeCfg(int huanhua_type, int grade)
{
	if (huanhua_type < 0 || huanhua_type >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT || grade < 0 || grade >= MAX_ZHIBAO_HUANHUA_GRADE_COUNT)
	{
		return NULL;
	}

	if (m_huanhua_grade_list.m_grade_list[huanhua_type][grade].grade >= 0)
	{
		return &m_huanhua_grade_list.m_grade_list[huanhua_type][grade];
	}

	return NULL;
}

ZhiBaoHuanhuaCfg::ConfigItem * ZhiBaoConfig::GetHuanHuaCfg(int huanhua_id)
{
	if (huanhua_id < 0 || huanhua_id >= m_huanhua_cfg.huanhua_count || huanhua_id >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT)
	{
		return NULL;
	}

	return &m_huanhua_cfg.cfg_list[huanhua_id];
}
