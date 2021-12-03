#include "zhanshendianconfig.hpp"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"

ZhanShenDianConfig::ZhanShenDianConfig() : m_zhanshen_max_level(0)
{
	memset(m_zhanshen_level_cfg_list, 0, sizeof(m_zhanshen_level_cfg_list));
}

ZhanShenDianConfig::~ZhanShenDianConfig()
{
}

bool ZhanShenDianConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ZhanShenDianConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ¹Ø¿¨ÅäÖÃ
		PugiXmlNode level_element = RootElement.child("level_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [level_cfg].";
			return false;
		}

		iRet = this->InitLevelCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ZhanShenDianConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	m_zhanshen_max_level = 0;
	m_level_cfg_map.clear();

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int pass_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", pass_level) || pass_level != m_zhanshen_max_level + 1 || pass_level > ZHANSHEN_LEVEL_MAX_COUNT)
		{
			return -1;
		}
		m_zhanshen_max_level = pass_level;

		ZhanShenDianLevelCfg &level_cfg = m_zhanshen_level_cfg_list[pass_level];
		level_cfg.level = pass_level;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", level_cfg.scene_id) || level_cfg.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(level_cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "monster_id", level_cfg.monster_id) || level_cfg.monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(level_cfg.monster_id))
		{
			return -3;
		}

		if (m_level_cfg_map.find(level_cfg.monster_id) != m_level_cfg_map.end())
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", level_cfg.monster_pos.x) || level_cfg.monster_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", level_cfg.monster_pos.y) || level_cfg.monster_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_x", level_cfg.brith_pos.x) || level_cfg.brith_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_y", level_cfg.brith_pos.y) || level_cfg.brith_pos.y <= 0)
		{
			return -8;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -9;
		}
		level_cfg.is_broadcast = (0 != is_broadcast);

		PugiXmlNode reward_item_list_element = dataElement.child("day_reward_list");
		if (reward_item_list_element.empty())
		{
			return -10;
		}

		PugiXmlNode dayreward_item_element = reward_item_list_element.child("day_reward");
		if (dayreward_item_element.empty())
		{
			return -11;
		}

		level_cfg.day_reward_count = 0;
		while (!dayreward_item_element.empty())
		{
			if (level_cfg.day_reward_count >= ZHANSHEN_DAY_REWARD_MAX_COUNT)
			{
				return -12;
			}

			if (!level_cfg.day_reward_list[level_cfg.day_reward_count].ReadConfig(dayreward_item_element))
			{
				return -13;
			}

			++ level_cfg.day_reward_count;
			dayreward_item_element = dayreward_item_element.next_sibling();
		}

		PugiXmlNode pass_reward_element = dataElement.child("pass_reward");
		if (pass_reward_element.empty())
		{
			return -14;
		}

		if (!level_cfg.pass_raward.ReadConfig(pass_reward_element))
		{
			return -15;
		}

		m_level_cfg_map[level_cfg.monster_id] = level_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ZhanShenDianLevelCfg * ZhanShenDianConfig::GetZhanShenDianLevelCfgByMonster(UInt16 monster_id)
{
	std::map<UInt16, ZhanShenDianLevelCfg>::const_iterator it = m_level_cfg_map.find(monster_id);
	if (it != m_level_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ZhanShenDianLevelCfg * ZhanShenDianConfig::GetZhanShenDianLevelCfgByLevel(int pass_level)
{
	if (pass_level > 0 && pass_level <= ZHANSHEN_LEVEL_MAX_COUNT && pass_level <= m_zhanshen_max_level)
	{
		return &m_zhanshen_level_cfg_list[pass_level];
	}

	return NULL;
}


