#include "dailytaskfbcofig.hpp"
#include "monster/monsterpool.h"

DailyTaskFBConfig::DailyTaskFBConfig()
{
}

DailyTaskFBConfig::~DailyTaskFBConfig()
{
}

bool DailyTaskFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "DailyTaskFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 支线副本配置
		PugiXmlNode level_element = RootElement.child("fb_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [fb_cfg].";
			return false;
		}

		iRet = this->InitDailyTaskFbCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyTaskFbCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 支线副本怪物配置
		PugiXmlNode level_element = RootElement.child("monster_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [monster_cfg].";
			return false;
		}

		iRet = this->InitDailyTaskFBMonsterCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyTaskFBMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int DailyTaskFBConfig::InitDailyTaskFbCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}
	
	int last_max_level = 0;
	int last_scene_id = 0;

	while(!dataElement.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -1;
		}

		if (last_scene_id != scene_id)
		{
			last_max_level = 0;
		}

		DailyFBCfg cfg;
		cfg.scene_id = scene_id;

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "branch_fb_type", cfg.branch_fb_type) || cfg.branch_fb_type <= DAILY_TASK_FB_TYPE_INVALID || cfg.branch_fb_type >= DAILY_TASK_FB_TYPE_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_1", cfg.monster_1) || cfg.monster_1 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_2", cfg.monster_2) || cfg.monster_2 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_3", cfg.monster_3) || cfg.monster_3 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_monster", cfg.boss_monster) || cfg.boss_monster < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "finish_param", cfg.finish_param) || cfg.finish_param <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_level", cfg.min_level) || cfg.min_level <= 0 || cfg.min_level > MAX_ROLE_LEVEL || cfg.min_level <= last_max_level)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_level", cfg.max_level) || cfg.max_level <= 0 || cfg.max_level > MAX_ROLE_LEVEL || cfg.max_level < cfg.min_level)
		{
			return -15;
		}

		last_max_level = cfg.max_level;

		DailyTaskFbKey fb_key;
		fb_key.scene_id = cfg.scene_id;
		fb_key.min_level = cfg.min_level;
		fb_key.max_level = cfg.max_level;

		if (m_daily_task_fb_cfg_map.find(fb_key) != m_daily_task_fb_cfg_map.end())
		{
			return -16;
		}

		m_daily_task_fb_cfg_map[fb_key] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DailyTaskFBConfig::InitDailyTaskFBMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -1;
		}

		std::map<int, DailyTaskFBMonsterCfg> &scene_monster_map = m_monster_cfg_map[scene_id];

		int index = 0;
		if (!PugiGetSubNodeValue(dataElement, "index", index) || index <= 0)
		{
			return -2;
		}

		if (scene_monster_map.find(index) != scene_monster_map.end())
		{
			return -3;
		}

		DailyTaskFBMonsterCfg &monster_cfg = scene_monster_map[index];

		if (!PugiGetSubNodeValue(dataElement, "min_level", monster_cfg.min_level) || monster_cfg.min_level <= 0 || monster_cfg.min_level > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_level", monster_cfg.max_level) || monster_cfg.max_level <= 0 || monster_cfg.max_level > MAX_ROLE_LEVEL ||
			monster_cfg.max_level < monster_cfg.min_level)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(monster_cfg.monster_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", monster_cfg.create_pos.x) || monster_cfg.create_pos.x <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", monster_cfg.create_pos.y) || monster_cfg.create_pos.y <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_mianshang", monster_cfg.add_mianshang) || monster_cfg.add_mianshang < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "mianshang_only_boss", monster_cfg.mianshang_only_boss) || monster_cfg.mianshang_only_boss < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "mianshang_per", monster_cfg.mianshang_per) || monster_cfg.mianshang_per < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_recover", monster_cfg.add_recover) || monster_cfg.add_recover < 0 || 
			(monster_cfg.add_recover == monster_cfg.add_mianshang && monster_cfg.add_recover != 0))
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "recover_only_boss", monster_cfg.recover_only_boss) || monster_cfg.recover_only_boss < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "recover_by_attack", monster_cfg.recover_by_attack) || monster_cfg.recover_by_attack < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "recover_per", monster_cfg.recover_per) || monster_cfg.recover_per < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "recover_interval", monster_cfg.recover_interval) || monster_cfg.recover_interval < 0)
		{
			return -15;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

DailyFBCfg * DailyTaskFBConfig::GetDailyTaskFBConfig(int scene_id, int level)
{
	std::map<DailyTaskFbKey, DailyFBCfg>::iterator iter = m_daily_task_fb_cfg_map.begin();
	for(; iter != m_daily_task_fb_cfg_map.end(); iter++)
	{
		if (scene_id == iter->second.scene_id)
		{
			if (level >= iter->second.min_level && level <= iter->second.max_level)
			{
				return &iter->second;
			}
		}
	}

	return NULL;
}

