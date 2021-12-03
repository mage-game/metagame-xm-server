#include "phasefbconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"

#include <string>

PhaseFBConfig::PhaseFBConfig() 
{
}

PhaseFBConfig::~PhaseFBConfig()
{
}

bool PhaseFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "PhaseFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ¹Ø¿¨ÅäÖÃ
		PugiXmlNode lvl_element = RootElement.child("fb_list");
		if (lvl_element.empty())
		{
			*err = configname + ": no [fb_list].";
			return false;
		}

		iRet = this->InitLevelCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode lvl_element = RootElement.child("fb_buy_cfg");
		if (lvl_element.empty())
		{
			*err = configname + ": no [fb_buy_cfg].";
			return false;
		}

		iRet = this->InitPhaseResetCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPhaseResetCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const PhaseLevelCfg::ConfigItem * PhaseFBConfig::GetLevelCfg(int fb_index, int fb_level) const
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return NULL;
	}

	if (fb_level <= 0 || fb_level >= FB_PHASE_MAX_LEVEL)
	{
		return NULL;
	}

	if (m_level_cfg.item_list[fb_index][fb_level].fb_index != fb_index || m_level_cfg.item_list[fb_index][fb_level].fb_level != fb_level)
	{
		return NULL;
	}

	return &m_level_cfg.item_list[fb_index][fb_level];
}

const int PhaseFBConfig::GetPhaseResetGold(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return 0;
	}

	return m_phase_reset_cfg[fb_index];
}

int PhaseFBConfig::GetLevelLimit(int fb_type, int fb_level)
{
	if (fb_level <= 0 || fb_level >= FB_PHASE_MAX_LEVEL)
	{
		return 0;
	}

	for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
	{
		if (m_level_cfg.item_list[i][fb_level].fb_index != i)
		{
			return 0;
		}

		if (m_level_cfg.item_list[i][fb_level].fb_type == fb_type)
		{
			return m_level_cfg.item_list[i][fb_level].role_level;
		}
	}

	return 0;
}

int PhaseFBConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int fb_index = 0;
		int fb_type = 0;
		int fb_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "fb_index", fb_index) || fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "fb_type", fb_type) || fb_type < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "fb_level", fb_level) || fb_level <= 0 || fb_level >= FB_PHASE_MAX_LEVEL)
		{
			return -3;
		}

		PhaseLevelCfg::ConfigItem &cfg = m_level_cfg.item_list[fb_index][fb_level];
		cfg.fb_index = fb_index;
		cfg.fb_type = fb_type;
		cfg.fb_level = fb_level;

		if (!PugiGetSubNodeValue(dataElement, "role_level", cfg.role_level) || cfg.role_level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -5;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_times", cfg.free_times) || cfg.free_times < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "reset_gold", cfg.reset_gold) || cfg.reset_gold < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_exp", cfg.reward_exp) || cfg.reward_exp < (long long)0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_level", cfg.auto_level) || cfg.auto_level < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "open_task_id", cfg.open_task_id) || cfg.open_task_id < 0)
		{
			return -12;
		}

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

// 		{
// 			PugiXmlNode element = dataElement.child("normal_reward_list");
// 			if (element.empty())
// 			{
// 				return -200;
// 			}
// 
// 			int i = 0;
// 			PugiXmlNode item_element = element.child("normal_reward");
// 			while (!item_element.empty())
// 			{
// 				if (i >= MAX_ATTACHMENT_ITEM_NUM)
// 				{
// 					return -201 - i;
// 				}
// 
// 				if (!cfg.normal_reward_list[i].ReadConfig(item_element))
// 				{
// 					return - 251 - i;
// 				}
// 
// 				i++;
// 				item_element = item_element.next_sibling();
// 			}
// 		}

		{
			PugiXmlNode element = dataElement.child("reset_reward_list");
			if (element.empty())
			{
				return -300;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reset_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -301 - i;
				}

				if (!cfg.reset_reward_list[i].ReadConfig(item_element))
				{
					return - 351 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		{
			cfg.monster_count = 0;

			char tag_name[32] = {0};
			for (int i = 1; i <= PhaseLevelCfg::MONSTER_COUNT_MAX; ++i)
			{
				SNPRINTF(tag_name, sizeof(tag_name), "monster_id%d", i);
				if (dataElement.child(tag_name).empty())
				{
					break;
				}
				if (!PugiGetSubNodeValue(dataElement, tag_name, cfg.monster_id_list[i - 1]) || cfg.monster_id_list[i - 1] <= 0)
				{
					return -500 - i;
				}

				SNPRINTF(tag_name, sizeof(tag_name), "monster_x%d", i);
				if (dataElement.child(tag_name).empty())
				{
					break;
				}
				if (!PugiGetSubNodeValue(dataElement, tag_name, cfg.monster_pos_list[i - 1].x) || cfg.monster_pos_list[i - 1].x <= 0)
				{
					return -600 - i;
				}

				SNPRINTF(tag_name, sizeof(tag_name), "monster_y%d", i);
				if (dataElement.child(tag_name).empty())
				{
					break;
				}
				if (!PugiGetSubNodeValue(dataElement, tag_name, cfg.monster_pos_list[i - 1].y) || cfg.monster_pos_list[i - 1].y <= 0)
				{
					return -700 - i;
				}

				cfg.monster_count++;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int PhaseFBConfig::InitPhaseResetCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	int last_fb_index = -1;
	while (!data_element.empty())
	{
		int fb_index = -1;
		if (!PugiGetSubNodeValue(data_element, "fb_index", fb_index) || fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
		{
			return -1;
		}

		if (fb_index != last_fb_index + 1)
		{
			return -100;
		}
		last_fb_index = fb_index;

		int need_gold = 0;
		if (!PugiGetSubNodeValue(data_element, "need_gold", need_gold) || need_gold <= 0)
		{
			return -2;
		}

		m_phase_reset_cfg[fb_index] = need_gold;

		data_element = data_element.next_sibling();
	}
	return 0;
}
