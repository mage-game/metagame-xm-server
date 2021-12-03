#include "storyfbconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"

#include <string>

StoryFBConfig::StoryFBConfig()
{
}

StoryFBConfig::~StoryFBConfig()
{
}

bool StoryFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "StoryFBConfig", *err);

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

	return true;
}

const StoryFBLevelCfg::ConfigItem * StoryFBConfig::GetLevelCfg(int fb_index) const
{
	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return NULL;
	}

	if (m_level_cfg.item_list[fb_index].fb_index != fb_index)
	{
		return NULL;
	}

	return &m_level_cfg.item_list[fb_index];
}

int StoryFBConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int fb_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "fb_index", fb_index) || fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
		{
			return -1;
		}

		StoryFBLevelCfg::ConfigItem &cfg = m_level_cfg.item_list[fb_index];
		cfg.fb_index = fb_index;

		if (!PugiGetSubNodeValue(dataElement, "fb_level", cfg.fb_level) || cfg.fb_level < 0)
		{
			return -3;
		}

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

		if (!PugiGetSubNodeValue(dataElement, "reward_exp", cfg.reward_exp) || cfg.reward_exp < 0)
		{
			return -10;
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
			for (int i = 1; i <= StoryFBLevelCfg::MONSTER_COUNT_MAX; ++i)
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
