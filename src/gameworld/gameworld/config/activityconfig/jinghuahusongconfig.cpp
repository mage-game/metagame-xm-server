#include "jinghuahusongconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

#include "item/itempool.h"

JingHuaHusongConfig::JingHuaHusongConfig()
{
}

JingHuaHusongConfig::~JingHuaHusongConfig()
{

}

bool JingHuaHusongConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "JingHuaHusongConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode other_element = RootElement.child("reward");
		if (other_element.empty())
		{
			*err = configname + ": no [reward].";
			return false;
		}

		iRet = this->InitRewardCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int JingHuaHusongConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

 	if (!PugiGetSubNodeValue(dataElement, "open_hour", m_other_cfg.open_hour) || m_other_cfg.open_hour < 0 || m_other_cfg.open_hour >= 24)
 	{
 		return -1;
 	}

	if (!PugiGetSubNodeValue(dataElement, "end_hour", m_other_cfg.end_hour) || m_other_cfg.end_hour <= 0 || m_other_cfg.end_hour <= m_other_cfg.open_hour || m_other_cfg.end_hour > 24)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_id", m_other_cfg.gather_id) || m_other_cfg.gather_id <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_scene_id", m_other_cfg.gather_scene_id) || m_other_cfg.gather_scene_id <= 0)
	{
		return -4;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.gather_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "gather_pos_x", m_other_cfg.gather_pos.x) || m_other_cfg.gather_pos.x <= 0)
	{
		return -40;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_pos_y", m_other_cfg.gather_pos.y) || m_other_cfg.gather_pos.y <= 0)
	{
		return -41;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_time", m_other_cfg.gather_time) || m_other_cfg.gather_time <= 0)
	{
		return -42;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_change_times", m_other_cfg.gather_change_times) || m_other_cfg.gather_change_times <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_min_times", m_other_cfg.gather_min_times) || m_other_cfg.gather_min_times <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_max_times", m_other_cfg.gather_max_times) || m_other_cfg.gather_max_times < m_other_cfg.gather_min_times)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "commit_npcid", m_other_cfg.commit_npcid) || m_other_cfg.commit_npcid <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_day_count", m_other_cfg.gather_day_count) || m_other_cfg.gather_day_count <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_get_jinghua", m_other_cfg.gather_get_jinghua) || m_other_cfg.gather_get_jinghua <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "lost_jinghua_per", m_other_cfg.lost_jinghua_per) || m_other_cfg.lost_jinghua_per <= 0 || m_other_cfg.lost_jinghua_per >= 100)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "husong_timeout", m_other_cfg.husong_timeout) || m_other_cfg.husong_timeout <= 0 || m_other_cfg.husong_timeout > 3600)
	{
		return -12;
	}

	return 0;
}

int JingHuaHusongConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_commit_times = 0;

	while(!dataElement.empty())
	{
		JHHusongRewardCfg item_cfg;

		if (!PugiGetSubNodeValue(dataElement, "commit_times", item_cfg.commit_times) || item_cfg.commit_times < last_commit_times)
		{
			return -1;
		}
		last_commit_times = item_cfg.commit_times;

		if (!PugiGetSubNodeValue(dataElement, "reward_per", item_cfg.reward_per) || item_cfg.reward_per > 100 || item_cfg.reward_per <= 0)
		{
			return -2;
		}

		m_reward_vec.push_back(item_cfg);

		dataElement = dataElement.next_sibling();
	}

	return  0;
}

int JingHuaHusongConfig::GetJingHuaRewardPer(int commit_times)
{
	if (commit_times < 0) return 0;

	for (JhhusongRewardVec::const_iterator iter = m_reward_vec.begin(); m_reward_vec.end() != iter; ++ iter)
	{
		if (commit_times <= iter->commit_times)
		{
			return iter->reward_per;
		}
	}
	return 0;
}