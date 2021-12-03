#include <memory.h>
#include <stdlib.h>

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "mojieconfig.hpp"
#include "item/itempool.h"
#include "skill/skilldef.hpp"
#include "skill/skillpool.hpp"

MojieConfig::MojieConfig()
{
	memset(m_mojie_max_level, 0 , sizeof(m_mojie_max_level));
}

MojieConfig::~MojieConfig()
{

}

bool MojieConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ActiveDegreeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

// 	{
// 		// 其他
// 		PugiXmlNode element = RootElement.child("other");
// 		if (element.empty())
// 		{
// 			*err = path + " xml not other node ";
// 			return false;
// 		}
// 
// 		iRet = this->InitOtherCfg(element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}

	{
		// 等级属性
		PugiXmlNode element = RootElement.child("level");
		if (element.empty())
		{
			*err = path + " xml not level node ";
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

	return true;
}

int MojieConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	return 0;
}


int MojieConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int mojie_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "mojie_type", mojie_type) || mojie_type <= INVALID_MOJIE_TYPE || mojie_type >= MOJIE_TYPE_MAX)
		{
			return -1;
		}

		int mojie_level = -1;
		
		if (!PugiGetSubNodeValue(dataElement, "mojie_level", mojie_level) || mojie_level < 0 || mojie_level > MojieParam::MOJIE_MAX_LEVEL)
		{
			return -2;
		}

		if (0 < mojie_level)
		{
			MojieUplevelCfg &last_level_cfg = m_mojie_level_cfg[mojie_type][mojie_level - 1];

			if ((mojie_level - 1) != last_level_cfg.mojie_level)
			{
				return -3;
			}
		}

		MojieUplevelCfg &cfg = m_mojie_level_cfg[mojie_type][mojie_level];
		cfg.mojie_type = mojie_type;
		cfg.mojie_level = mojie_level;
		
		if (!PugiGetSubNodeValue(dataElement, "has_skill", cfg.has_skill) || cfg.has_skill < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_id", cfg.skill_id) || cfg.skill_id < 0 || cfg.skill_id >= SkillPool::MAX_SKILL_ID)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_level", cfg.skill_level) || cfg.skill_level < 0 || cfg.skill_level > MAX_SKILL_LEVEL) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "use_global_cold_time", cfg.use_global_cold_time) || cfg.use_global_cold_time < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_level_stuff_id", cfg.up_level_stuff_id) || cfg.up_level_stuff_id <= 0 || NULL == ITEMPOOL->GetItem(cfg.up_level_stuff_id)) 
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_level_stuff_num", cfg.up_level_stuff_num) || cfg.up_level_stuff_num <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_level_limit", cfg.up_level_limit) || cfg.up_level_limit <= 0) 
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) 
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) 
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_pofang", cfg.per_pofang) || cfg.per_pofang < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_baoji_hurt", cfg.per_baoji_hurt) || cfg.per_baoji_hurt < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_mianshang", cfg.per_mianshang) || cfg.per_mianshang < 0)
		{
			return -22;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0) 
		{
			return -23;
		}

		if (0 < cfg.mojie_level)
		{
			++m_mojie_max_level[cfg.mojie_type];
		}
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const MojieUplevelCfg * MojieConfig::GetMojieLevelCfg(int mojie_type, int mojie_level) const
{
	if (mojie_type <= INVALID_MOJIE_TYPE || mojie_type >= MOJIE_TYPE_MAX)
	{
		return NULL;
	}

	if (mojie_level < 0 || mojie_level > MojieParam::MOJIE_MAX_LEVEL || mojie_level > m_mojie_max_level[mojie_type])
	{
		return NULL;
	}

	return &m_mojie_level_cfg[mojie_type][mojie_level];
}
