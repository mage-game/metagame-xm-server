#include "vipfbconfig.hpp"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"

VipFbConfig::VipFbConfig()
{
}

VipFbConfig::~VipFbConfig()
{
}

bool VipFbConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "VipFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ¹Ø¿¨ÅäÖÃ
		PugiXmlNode level_element = RootElement.child("levelcfg");
		if (level_element.empty())
		{
			*err = configname + ": no [levelcfg].";
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

int VipFbConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	int m_max_level = 0;
	m_level_cfg_map.clear();

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_boss_id = 0;
	while (!dataElement.empty())
	{
		int pass_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", pass_level) || pass_level != m_max_level + 1)
		{
			return -1;
		}
		m_max_level = pass_level;

		VipLevelConfig &cfg = m_level_cfg_map[pass_level];
		cfg.level = pass_level;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg.birth_pos.x) || cfg.birth_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg.birth_pos.y) || cfg.birth_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", cfg.boss_id) || cfg.boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id) || last_boss_id == cfg.boss_id)
		{
			return -5;
		}
		last_boss_id = cfg.boss_id;

		if (!PugiGetSubNodeValue(dataElement, "boss_x", cfg.boss_pos.x) || cfg.boss_pos.x <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_y", cfg.boss_pos.y) || cfg.boss_pos.y <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_times", cfg.free_times) || cfg.free_times <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_level", cfg.enter_level) || cfg.enter_level < 0)
		{
			return -9;
		}

		{
			PugiXmlNode element = dataElement.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const VipLevelConfig * VipFbConfig::GetLevelCfgByMonsterID(UInt16 monster_id)
{
	std::map<int, VipLevelConfig>::const_iterator iter;
	for (iter = m_level_cfg_map.begin(); m_level_cfg_map.end() != iter; ++ iter)
	{
		if (monster_id == iter->second.boss_id)
		{
			return &iter->second;
		}
	}
	return NULL;
}

const VipLevelConfig * VipFbConfig::GetLevelCfgByLevel(int pass_level)
{
	std::map<int, VipLevelConfig>::const_iterator it = m_level_cfg_map.find(pass_level);
	if (it == m_level_cfg_map.end())
	{
		return NULL;
	}

	return &it->second;
}

const VipLevelConfig * VipFbConfig::GetLevelCfgBySceneID(int scene_id)
{
	std::map<int, VipLevelConfig>::const_iterator iter;
	for (iter = m_level_cfg_map.begin(); m_level_cfg_map.end() != iter; ++ iter)
	{
		if (scene_id == iter->second.scene_id)
		{
			return &iter->second;
		}
	}
	return NULL;
}
