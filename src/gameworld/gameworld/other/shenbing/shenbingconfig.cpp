#include "shenbingconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

ShenBingConfig::ShenBingConfig()
{

}

ShenBingConfig::~ShenBingConfig()
{

}

bool ShenBingConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ShenBingConfig", *err);

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
		// 升级物品
		PugiXmlNode element = RootElement.child("up_level_stuff");
		if (element.empty())
		{
			*err = path + " xml not up_level_stuff node ";
			return false;
		}

		iRet = this->InitUpLevelStuffCfg(element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitUpLevelStuffCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}


int ShenBingConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > ShenBingConfig::MAX_SHENBING_LEVEL)
		{
			return -1;
		}

		ShenBingLevelCfg &cfg = m_level_cfg_list[level];
		if (0 != cfg.level)
		{
			return -2;
		}

		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_exp", cfg.uplevel_exp) || cfg.uplevel_exp < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_limit", cfg.shuxingdan_limit) || cfg.shuxingdan_limit < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_limit", cfg.chengzhangdan_limit) || cfg.chengzhangdan_limit < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast)) return -120;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShenBingConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= MAX_SHENBING_SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level <= 0 || skill_level > MAX_SHENBING_SKILL_LEVEL)
		{
			return -2;
		}

		ShenBingSkillCfg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		if (0 != cfg.skill_level)
		{
			return -3;
		}

		cfg.skill_idx = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "shenbing_level", cfg.shenbing_level) || cfg.shenbing_level < 0) 
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
			return -7;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShenBingConfig::InitUpLevelStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_up_level_item_index = -1;
	while (!dataElement.empty())
	{
		int up_level_item_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "up_level_item_index", up_level_item_index) || up_level_item_index < 0 || up_level_item_index > ShenBingConfig::MAX_UP_LEVEL_ITEM_COUNT || up_level_item_index <= last_up_level_item_index)
		{
			return -1;
		}

		ShenBingUpLevelStuffCfg &cfg = m_uplevel_item_list[up_level_item_index];
		cfg.up_level_item_index = up_level_item_index;
		last_up_level_item_index = up_level_item_index;

		if (!PugiGetSubNodeValue(dataElement, "up_level_item_id", cfg.up_level_item_id) || cfg.up_level_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg.up_level_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "level_exp", cfg.level_exp) || cfg.level_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_per", cfg.baoji_per) || cfg.baoji_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul <= 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ShenBingLevelCfg * ShenBingConfig::GetShenBingLevelCfg(int level)
{
	if (level < 0 || level > MAX_SHENBING_LEVEL || m_level_cfg_list[level].level != level)
	{
		return NULL;
	}

	return &m_level_cfg_list[level];
}

const ShenBingSkillCfg * ShenBingConfig::GetShenBingSkillCfg(int skill_idx, int skill_level)
{
	if (skill_idx < 0 || skill_idx >= MAX_SHENBING_SKILL_COUNT || skill_level <= 0 || skill_level > MAX_SHENBING_SKILL_LEVEL)
	{
		return NULL;
	}

	if (m_skill_cfg_list[skill_idx][skill_level].skill_level <= 0)
	{
		return NULL;
	}

	return &m_skill_cfg_list[skill_idx][skill_level];
}

const ShenBingUpLevelStuffCfg * ShenBingConfig::GetUpLevelStuffCfg(int stuff_index)
{
	if (stuff_index < 0 || stuff_index >= ShenBingConfig::MAX_UP_LEVEL_ITEM_COUNT)
		return NULL;

	return &m_uplevel_item_list[stuff_index];
}