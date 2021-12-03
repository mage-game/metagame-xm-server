#include "multibattlefieldskillconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "scene/scene.h"

#include <string>


MultiBattleFieldSkillConfig::MultiBattleFieldSkillConfig()
{

}

MultiBattleFieldSkillConfig::~MultiBattleFieldSkillConfig()
{

}

bool MultiBattleFieldSkillConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MultiBattleFieldSkillConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 群仙乱斗技能
		PugiXmlNode qx_skill_element = RootElement.child("qunxianskill");
		if (qx_skill_element.empty())
		{
			*err = configname + ": no [qunxianskill].";
			return false;
		}

		iRet = this->InitQunxianSkillCfg(qx_skill_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitQunxianSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙魔争霸技能
		PugiXmlNode xm_skill_element = RootElement.child("xianmoskill");
		if (xm_skill_element.empty())
		{
			*err = configname + ": no [xianmoskill].";
			return false;
		}

		iRet = this->InitXianmoSkillCfg(xm_skill_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianmoSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MultiBattleFieldSkillConfig::InitQunxianSkillCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		MultiBattleFieldSkillCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "skill_index", cfg.skill_id) || cfg.skill_id < 0 || cfg.skill_id >= MAX_MULTI_BATTLEFILED_SKILL_COUNT)
		{
			return -1;
		}

		if (!m_qunxian_skill_list[cfg.skill_id].Invalid())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_s", cfg.cd_s) || cfg.cd_s < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "distance", cfg.distance) || cfg.distance <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "zhanyi_cost", cfg.zhanyi_cost) || cfg.zhanyi_cost < 0)
		{
			return -10;
		}

		m_qunxian_skill_list[cfg.skill_id] = cfg;

		++ count;

		dataElement = dataElement.next_sibling();
	}

	if (count != MAX_MULTI_BATTLEFILED_SKILL_COUNT)
	{
		return -50;
	}

	return 0;
}
int MultiBattleFieldSkillConfig::InitXianmoSkillCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		MultiBattleFieldSkillCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "skill_index", cfg.skill_id) || cfg.skill_id < 0 || cfg.skill_id >= MAX_MULTI_BATTLEFILED_SKILL_COUNT)
		{
			return -1;
		}

		if (!m_xianmo_skill_list[cfg.skill_id].Invalid())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_s", cfg.cd_s) || cfg.cd_s < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "distance", cfg.distance) || cfg.distance <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "zhanyi_cost", cfg.zhanyi_cost) || cfg.zhanyi_cost < 0)
		{
			return -10;
		}

		m_xianmo_skill_list[cfg.skill_id] = cfg;

		++ count;

		dataElement = dataElement.next_sibling();
	}

	if (count != MAX_MULTI_BATTLEFILED_SKILL_COUNT)
	{
		return -50;
	}

	return 0;
}

const MultiBattleFieldSkillCfg * MultiBattleFieldSkillConfig::GetQunxianSkillCfg(int skill_index) const
{
	if (skill_index >= 0 && skill_index < MAX_MULTI_BATTLEFILED_SKILL_COUNT)
	{
		return &m_qunxian_skill_list[skill_index];
	}

	return NULL;
}

const MultiBattleFieldSkillCfg * MultiBattleFieldSkillConfig::GetXianmoSkillCfg(int skill_index) const
{
	if (skill_index >= 0 && skill_index < MAX_MULTI_BATTLEFILED_SKILL_COUNT)
	{
		return &m_xianmo_skill_list[skill_index];
	}

	return NULL;
}
