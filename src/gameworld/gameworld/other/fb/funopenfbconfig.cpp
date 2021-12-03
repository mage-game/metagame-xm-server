#include "funopenfbconfig.hpp"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "task/taskpool.h"

FunOpenFBConfig::FunOpenFBConfig()
{
}

FunOpenFBConfig::~FunOpenFBConfig()
{
}

bool FunOpenFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FunOpenFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 副本其他配置
		PugiXmlNode level_element = RootElement.child("other_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [other_cfg].";
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

	{
		// 副本场景配置
		PugiXmlNode level_element = RootElement.child("fb_scene_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [fb_scene_cfg].";
			return false;
		}

		iRet = this->InitSceneCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSceneCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 副本怪物配置
		PugiXmlNode level_element = RootElement.child("fb_monster_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [fb_monster_cfg].";
			return false;
		}

		iRet = this->InitMonsterCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 采集物配置
		PugiXmlNode level_element = RootElement.child("fb_gather_cfg");
		if (level_element.empty())
		{
			*err = configname + ": no [fb_gather_cfg].";
			return false;
		}

		iRet = this->InitGatherCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGatherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const FunOpenSceneCfg * FunOpenFBConfig::GetFunOpenSceneCfg(int fb_type)
{
	if (fb_type <= INVALID_FB_TYPE || fb_type >= FUN_OPEN_FB_MAX)
	{
		return NULL;
	}

	std::map<int, FunOpenSceneCfg>::const_iterator iter = m_scene_cfg_map.find(fb_type);
	if (iter != m_scene_cfg_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const FunOpenGatherCfg * FunOpenFBConfig::GetFunOpenGatherCfg(int fb_type, int gather_id)
{
	if (fb_type <= INVALID_FB_TYPE || fb_type >= FUN_OPEN_FB_MAX)
	{
		return NULL;
	}
	
	std::map<int, FunOpenGatherCfg>::const_iterator iter = m_gather_cfg_map.begin();
	for(;iter != m_gather_cfg_map.end(); iter++)
	{
		if (iter->second.fb_type == fb_type && iter->second.gather_id == gather_id)
		{
			return &iter->second;
		}
	}

	return NULL;
}

int FunOpenFBConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_id", m_other_cg.shuijing_monster_id) || m_other_cg.shuijing_monster_id < 0 || !MONSTERPOOL->IsMonsterExist(m_other_cg.shuijing_monster_id))
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_num", m_other_cg.shuijing_monster_num) || m_other_cg.shuijing_monster_num < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_interval", m_other_cg.shuijing_monster_interval) || m_other_cg.shuijing_monster_interval < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_born_pos_x", m_other_cg.shuijing_monster_born_pos.x) || m_other_cg.shuijing_monster_born_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_born_pos_y", m_other_cg.shuijing_monster_born_pos.y) || m_other_cg.shuijing_monster_born_pos.y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "shuijing_monster_born_pos_rand", m_other_cg.shuijing_monster_born_pos_rand) || m_other_cg.shuijing_monster_born_pos_rand < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "mount_gather_id", m_other_cg.mount_gather_id) || m_other_cg.mount_gather_id < 0)
	{
		return -6;
	}

	return 0;
}

int FunOpenFBConfig::InitSceneCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		FunOpenSceneCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "fb_type", cfg.fb_type) || cfg.fb_type <= INVALID_FB_TYPE || cfg.fb_type >= FUN_OPEN_FB_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_id", cfg.task_id) || cfg.task_id <= 0 || NULL == TASKPOOL->GetCfgByTaskID(cfg.task_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -5;
		}

		m_scene_cfg_map[cfg.fb_type] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FunOpenFBConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		FunOpenMonsterCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "index", cfg.monster_index) || cfg.monster_index <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "fb_type", cfg.fb_type) || cfg.fb_type <= INVALID_FB_TYPE || cfg.fb_type >= FUN_OPEN_FB_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "step", cfg.step) || cfg.step <= INVALID_STEP || cfg.step >= CREATE_MONSTER_STEP_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.monster_id) || cfg.monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "bron_pos_x", cfg.bron_pos.x) || cfg.bron_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "bron_pos_y", cfg.bron_pos.y) || cfg.bron_pos.y <= 0)
		{
			return -6;
		}

		m_monster_cfg_map[cfg.monster_index] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FunOpenFBConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		FunOpenGatherCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "index", cfg.gather_index) || cfg.gather_index <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "fb_type", cfg.fb_type) || cfg.fb_type <= INVALID_FB_TYPE || cfg.fb_type >= FUN_OPEN_FB_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "step", cfg.step) || cfg.step <= INVALID_STEP || cfg.step >= CREATE_MONSTER_STEP_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", cfg.gather_id) || cfg.gather_id <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "bron_pos_x", cfg.bron_pos.x) || cfg.bron_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "bron_pos_y", cfg.bron_pos.y) || cfg.bron_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time", cfg.gather_time) || cfg.gather_time <= 0)
		{
			return -7;
		}

		m_gather_cfg_map[cfg.gather_index] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

