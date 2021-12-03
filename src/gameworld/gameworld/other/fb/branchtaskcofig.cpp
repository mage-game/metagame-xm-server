#include "branchtaskcofig.hpp"
#include "monster/monsterpool.h"

BranchTaskFBConfig::BranchTaskFBConfig()
{
}

BranchTaskFBConfig::~BranchTaskFBConfig()
{
}

bool BranchTaskFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname))
	{
		sprintf(errinfo,"%s: Load FunOpenFBConfig Error,\n %s.", configname.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();

	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 支线副本配置
		TiXmlElement *level_element = RootElement->FirstChildElement("fb_cfg");
		if (NULL == level_element)
		{
			*err = configname + ": no [fb_cfg].";
			return false;
		}

		iRet = this->InitBranchFbCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBranchFbCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int BranchTaskFBConfig::InitBranchFbCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while(NULL != dataElement)
	{
		int scene_id = 0;
		if (!GetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -1;
		}

		if (m_branch_fb_cfg_map.find(scene_id) != m_branch_fb_cfg_map.end())
		{
			return -2;
		}

		BranchFBCfg cfg;
		cfg.scene_id = scene_id;

		if (!GetSubNodeValue(dataElement, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "branch_fb_type", cfg.branch_fb_type) || cfg.branch_fb_type <= BRANCH_FB_TYPE_INVALID || cfg.branch_fb_type >= BRANCH_FB_TYPE_MAX)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "monster_1", cfg.monster_1) || cfg.monster_1 < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "monster_2", cfg.monster_2) || cfg.monster_2 < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "monster_3", cfg.monster_3) || cfg.monster_3 < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "boss_monster", cfg.boss_monster) || cfg.boss_monster < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "finish_param", cfg.finish_param) || cfg.finish_param <= 0)
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "mianshang_id", cfg.mianshang_id) || cfg.mianshang_id < 0)
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "mianshang_per", cfg.mianshang_per) || cfg.mianshang_per < 0)
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "recover_id", cfg.recover_id) || cfg.recover_id < 0)
		{
			return -16;
		}

		if (!GetSubNodeValue(dataElement, "recover_per", cfg.recover_per) || cfg.recover_per < 0)
		{
			return -17;
		}

		if (!GetSubNodeValue(dataElement, "recover_interval", cfg.recover_interval) || cfg.recover_interval < 0)
		{
			return -18;
		}

		if (!GetSubNodeValue(dataElement, "mianshang_pos_x", cfg.mianshang_pos.x) || cfg.mianshang_pos.x < 0)
		{
			return -19;
		}

		if (!GetSubNodeValue(dataElement, "mianshang_pos_y", cfg.mianshang_pos.y) || cfg.mianshang_pos.y < 0)
		{
			return -20;
		}

		if (!GetSubNodeValue(dataElement, "recover_pos_x", cfg.recover_pos.x) || cfg.recover_pos.x < 0)
		{
			return -21;
		}

		if (!GetSubNodeValue(dataElement, "recover_pos_y", cfg.recover_pos.y) || cfg.recover_pos.y < 0)
		{
			return -22;
		}

		if (!GetSubNodeValue(dataElement, "recover_id_count", cfg.recover_id_count) || cfg.recover_id_count < 0)
		{
			return -23;
		}

		if (!GetSubNodeValue(dataElement, "can_move", cfg.can_move) || cfg.can_move < 0)
		{
			return -24;
		}

		if (!GetSubNodeValue(dataElement, "only_effect_boss", cfg.only_effect_boss) || cfg.only_effect_boss < 0)
		{
			return -25;
		}

		if (!GetSubNodeValue(dataElement, "recover_by_attack", cfg.recover_by_attack) || cfg.recover_by_attack < 0)
		{
			return -26;
		}

		m_branch_fb_cfg_map[scene_id] = cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const BranchFBCfg * BranchTaskFBConfig::GetBranchFBConfig(int scene_id)
{
	if (m_branch_fb_cfg_map.find(scene_id) != m_branch_fb_cfg_map.end())
	{
		return &m_branch_fb_cfg_map[scene_id];
	}

	return NULL;
}

