#include "patafbconfig.hpp"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"

PataFbConfig::PataFbConfig()
{
}

PataFbConfig::~PataFbConfig()
{
}

bool PataFbConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "PataFbConfig", *err);

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

	{
		// otherÅäÖÃ
		PugiXmlNode level_element = RootElement.child("other");
		if (level_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int PataFbConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "add_hp_per", m_other_cfg.add_hp_per) || m_other_cfg.add_hp_per < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "add_gongji_per", m_other_cfg.add_gongji_per) || m_other_cfg.add_gongji_per < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "add_role_hurt_buff_percent", m_other_cfg.add_role_hurt_buff_percent) || m_other_cfg.add_role_hurt_buff_percent < 0)
	{
		return -4;
	}

	return 0;
}

int PataFbConfig::InitLevelCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "level", pass_level) || pass_level > ROLE_PATA_FB_MAX_LEVEL || pass_level != m_max_level + 1)
		{
			return -1;
		}
		m_max_level = pass_level;

		PataLevelConfig &cfg = m_level_cfg_map[pass_level];
		cfg.level = pass_level;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);


		if (!PugiGetSubNodeValue(dataElement, "boss_id", cfg.boss_id) || cfg.boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id) || last_boss_id == cfg.boss_id)
		{
			return -3;
		}
		last_boss_id = cfg.boss_id;

		if (!PugiGetSubNodeValue(dataElement, "boss_x", cfg.boss_pos.x) || cfg.boss_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_y", cfg.boss_pos.y) || cfg.boss_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg.birth_pos.x) || cfg.birth_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg.birth_pos.y) || cfg.birth_pos.y <= 0)
		{
			return -8;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -9;
		}
		cfg.is_broadcast = (0 != is_broadcast);

		{
			PugiXmlNode element = dataElement.child("first_reward_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("first_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.first_reward_list[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_exp", cfg.reward_exp) || cfg.reward_exp < 0)
		{
			return -10;
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "has_normal_reward", cfg.has_normal_reward))
			{
				return -11;
			}

			if (cfg.has_normal_reward)
			{
				PugiXmlNode element = dataElement.child("normal_reward_list");
				if (element.empty())
				{
					return -200;
				}

				int i = 0;
				PugiXmlNode item_element = element.child("normal_reward");
				while (!item_element.empty())
				{
					if (i >= MAX_ATTACHMENT_ITEM_NUM)
					{
						return -201 - i;
					}

					if (!cfg.normal_reward_list[i].ReadConfig(item_element))
					{
						return - 251 - i;
					}

					i++;
					item_element = item_element.next_sibling();
				}
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability <= 0)
		{
			return -12;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const PataLevelConfig * PataFbConfig::GetLevelCfgByMonsterID(UInt16 monster_id)
{
	std::map<int, PataLevelConfig>::const_iterator iter;
	for (iter = m_level_cfg_map.begin(); m_level_cfg_map.end() != iter; ++ iter)
	{
		if (monster_id == iter->second.boss_id)
		{
			return &iter->second;
		}
	}
	return NULL;
}

const PataLevelConfig * PataFbConfig::GetLevelCfgByLevel(int pass_level)
{
	std::map<int, PataLevelConfig>::const_iterator it = m_level_cfg_map.find(pass_level);
	if (it == m_level_cfg_map.end())
	{
		return NULL;
	}

	return &it->second;
}


