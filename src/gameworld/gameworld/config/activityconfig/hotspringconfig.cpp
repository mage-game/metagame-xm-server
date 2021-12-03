#include "hotspringconfig.hpp"

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

#include "scene/worldshadow/worldshadow.hpp"

#include "item/itempool.h"
#include <algorithm>

HotspringConfig::HotspringConfig()
{

}

HotspringConfig::~HotspringConfig()
{

}

bool HotspringConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "HotspringConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
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
		PugiXmlNode exp_element = RootElement.child("exp_reward_list");
		if (exp_element.empty())
		{
			*err = configname + ": no [exp_reward_list].";
			return false;
		}

		iRet = this->InitExpCfg(exp_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode present_element = RootElement.child("present_list");
		if (present_element.empty())
		{
			*err = configname + ": no [present_list].";
			return false;
		}

		iRet = this->InitPresentCfg(present_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPresentCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode present_element = RootElement.child("skill_list");
		if (present_element.empty())
		{
			*err = configname + ": no [skill_list].";
			return false;
		}

		iRet = this->InitSkillCfg(present_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode present_element = RootElement.child("gather_cfg");
		if (present_element.empty())
		{
			*err = configname + ": no [gather_cfg].";
			return false;
		}

		iRet = this->InitGatherCfg(present_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGatherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode present_element = RootElement.child("gather_pos");
		if (present_element.empty())
		{
			*err = configname + ": no [gather_pos].";
			return false;
		}

		iRet = this->InitGatherPosCfg(present_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGatherPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int HotspringConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "sceneid", m_other_cfg.sceneid) || m_other_cfg.sceneid <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_pos_x", m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_pos_y", m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_interval_s", m_other_cfg.exp_interval_s) || m_other_cfg.exp_interval_s <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "popularity_rank_interval_s", m_other_cfg.popularity_rank_interval_s) || m_other_cfg.popularity_rank_interval_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "free_give_present_times", m_other_cfg.free_give_present_times) || m_other_cfg.free_give_present_times < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "free_present_id", m_other_cfg.free_present_id) ||m_other_cfg.free_present_id <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "interaction_add", m_other_cfg.interaction_add) ||m_other_cfg.interaction_add <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "reward_item_interval", m_other_cfg.reward_item_interval) ||m_other_cfg.reward_item_interval <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_gather_interval", m_other_cfg.refresh_gather_interval) || m_other_cfg.refresh_gather_interval <= 0)
	{
		return -11;
	}

	int refresh_gather_start_time_min = 0;
	if (!PugiGetSubNodeValue(data_element, "refresh_gather_start_time_min", refresh_gather_start_time_min) || refresh_gather_start_time_min <= 0)
	{
		return -12;
	}
	m_other_cfg.refresh_gather_start_time_s = refresh_gather_start_time_min * SECOND_PER_MIN;

	if (!PugiGetSubNodeValue(data_element, "max_gather_times", m_other_cfg.max_gather_times) || m_other_cfg.max_gather_times <= 0)
	{
		return -13;
	}

	{
		PugiXmlNode element = data_element.child("interval_reward_item_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("interval_reward_item");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_other_cfg.interval_reward_item[i].ReadConfig(item_element))
			{
				if (m_other_cfg.interval_reward_item[i].item_id == 0) break;

				return - 151 - i;
			}

			m_other_cfg.item_count++;
			i++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_base", m_other_cfg.exp_factor) || m_other_cfg.exp_factor < 0)
	{
		return -14;
	}

	return 0;
}

int HotspringConfig::InitExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_min_level = 0;
	while(!data_element.empty())
	{
		int min_level = 0;
		if (!PugiGetSubNodeValue(data_element, "min_limit_level",min_level) || min_level <= 0 || min_level <= last_min_level)
		{
			return -1;
		}
		
		HotspringExpRewardCfg &exp_cfg = m_exp_reward_cfg[min_level];
		exp_cfg.min_limit_level = min_level;
		last_min_level = min_level;
		
		if (!PugiGetSubNodeValue(data_element, "max_limit_level",exp_cfg.max_limit_level) || exp_cfg.max_limit_level <= 0 || exp_cfg.min_limit_level > exp_cfg.max_limit_level)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "base_exp",exp_cfg.base_exp) || exp_cfg.base_exp < 0)
		{
			return -3;
		}
		
		data_element = data_element.next_sibling();
	}
	return 0;
}

int HotspringConfig::InitPresentCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while(!data_element.empty())
	{
		int presen_id = 0;
		if (!PugiGetSubNodeValue(data_element, "present_id",presen_id) || presen_id <= 0 || m_present_cfg.find(presen_id) != m_present_cfg.end())
		{
			return -1;
		}

		HotspringPresentCfg &present_cfg = m_present_cfg[presen_id];
		present_cfg.present_id = presen_id;

		if (!PugiGetSubNodeValue(data_element, "present_price",present_cfg.present_price) || present_cfg.present_price <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_popularity",present_cfg.add_popularity) || present_cfg.add_popularity <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HotspringConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while(!data_element.empty())
	{
		int skill_type = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_type", skill_type) || skill_type <= HOTSPRING_SKILL_INVALID || skill_type >= HOTSPRING_SKILL_MAX)
		{
			return -1;
		}

		HotspringSkillCfg &skill_cfg = m_skill_cfg[skill_type];
		skill_cfg.skill_type = skill_type;

		if (!PugiGetSubNodeValue(data_element, "cold_down_time", skill_cfg.cold_down_time) || skill_cfg.cold_down_time < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "buff_effect_time", skill_cfg.buff_effect_time) || skill_cfg.buff_effect_time < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "use_times_limit", skill_cfg.use_times_limit) || skill_cfg.use_times_limit < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_distance", skill_cfg.skill_distance) || skill_cfg.skill_distance <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "use_add_times", skill_cfg.use_add_times) || skill_cfg.use_add_times <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "be_use_add_times", skill_cfg.be_use_add_times) || skill_cfg.be_use_add_times <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_factor_use", skill_cfg.exp_factor_use) || skill_cfg.exp_factor_use < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_factor_be_use", skill_cfg.exp_factor_be_use) || skill_cfg.exp_factor_be_use < 0)
		{
			return -11;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HotspringConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int gather_id = 0;
		if (!PugiGetSubNodeValue(data_element, "gather_id", gather_id) || gather_id <= 0)
		{
			return -1;
		}

		if (m_gather_cfg_map.find(gather_id) != m_gather_cfg_map.end())
		{
			return -2;
		}

		HotspringGahterCfg &gather_cfg = m_gather_cfg_map[gather_id];
		gather_cfg.gather_id = gather_id;

		if (!PugiGetSubNodeValue(data_element, "gather_time_ms", gather_cfg.gather_time_ms) || gather_cfg.gather_time_ms <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_disappear_time", gather_cfg.gather_disappear_time) || gather_cfg.gather_disappear_time < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_count", gather_cfg.gather_count) || gather_cfg.gather_count < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "add_times", gather_cfg.add_times) || gather_cfg.add_times < 0)
		{
			return -6;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -7;
		}

		if (!gather_cfg.reward_item.ReadConfigNoCheck(item_element))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "exp_factor_gather", gather_cfg.exp_factor_gather) || gather_cfg.exp_factor_gather < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HotspringConfig::InitGatherPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Posi pos;
		if (!PugiGetSubNodeValue(data_element, "gather_pos_x", pos.x) || pos.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_pos_y", pos.y) || pos.y <= 0)
		{
			return -2;
		}

		m_gather_pos_vec.push_back(pos);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const HotspringExpRewardCfg * HotspringConfig::GetExpCfg(int level)
{
	std::map<int,HotspringExpRewardCfg>::iterator iter = m_exp_reward_cfg.begin();
	for (;iter != m_exp_reward_cfg.end();iter++)
	{
		if (iter->second.min_limit_level <= level && level <= iter->second.max_limit_level)
		{
			return &iter->second;
		}
	}

	return NULL;
}

const HotspringPresentCfg * HotspringConfig::GetPresentCfg(int index)
{
	if (index <= 0) return NULL;

	std::map<int,HotspringPresentCfg>::iterator iter = m_present_cfg.find(index);
	if (iter != m_present_cfg.end())
	{
		return &iter->second;
	}

	return	NULL;
}

const HotspringPresentCfg * HotspringConfig::GetFreePresentCfg()
{
	std::map<int,HotspringPresentCfg>::iterator iter = m_present_cfg.begin();
	if (iter != m_present_cfg.end())
	{
		return &iter->second;
	}

	return NULL;
}

const HotspringSkillCfg * HotspringConfig::GetSkillCfgBySkillType(int skill_type)
{
	if (skill_type <= HOTSPRING_SKILL_INVALID || skill_type >= HOTSPRING_SKILL_MAX)
	{
		return NULL;
	}

	std::map<int,HotspringSkillCfg>::iterator iter = m_skill_cfg.find(skill_type);
	if (iter != m_skill_cfg.end())
	{
		return &iter->second;
	}

	return NULL;
}

const HotspringGahterCfg * HotspringConfig::GetGatherRewardCfg(int gather_id)
{
	auto it = m_gather_cfg_map.find(gather_id);
	if (it != m_gather_cfg_map.end())
	{
		return &(it->second);
	}

	return nullptr;
}

const std::vector<Posi> & HotspringConfig::GetRandGatherPosVec()
{
	std::random_shuffle(m_gather_pos_vec.begin(), m_gather_pos_vec.end());
	return m_gather_pos_vec;
}
