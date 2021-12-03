#include "worshipconfig.hpp"
#include "checkresourcecenter.hpp"

bool WorshipConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "WorshipConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	int iRet = 0;

	{
		PugiXmlNode elem = RootElement.child("other");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->other]", configname.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitOtherCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOtherCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("worship_reward");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->worship_reward]", configname.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitRewardCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitRewardCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int WorshipConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(data_element, "activityde_type", activity_type) || activity_type < 0)
		{
			return -1;
		}

		if (!(activity_type > ACTIVITY_TYPE_INVALID && activity_type < ACTIVITY_TYPE_MAX) &&
			!(activity_type > CROSS_ACTIVITY_TYPE_INVALID && activity_type < CROSS_ACTIVITY_TYPE_RESERVED)
			)
		{
			return -9999;
		}

		WorshipOtherConfig &cfg = m_other_cfg[activity_type];
		cfg.activityde_type = activity_type;

		if (!PugiGetSubNodeValue(data_element, "worship_time", cfg.worship_time) || cfg.worship_time <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_scene_id", cfg.worship_scene_id) || cfg.worship_scene_id <= 0)
		{
			return -3;
		}

		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.worship_scene_id);

		if (!PugiGetSubNodeValue(data_element, "worship_pos_x", cfg.worship_pos.x) || cfg.worship_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_pos_y", cfg.worship_pos.y) || cfg.worship_pos.y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_range", cfg.worship_range) || cfg.worship_range <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_reward_interval", cfg.worship_reward_interval) || cfg.worship_reward_interval <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_click_times", cfg.worship_click_times) || cfg.worship_click_times <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_click_cd", cfg.worship_click_cd) || cfg.worship_click_cd <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "worship_gather_time", cfg.worship_gather_time) || cfg.worship_gather_time <= 0)
		{
			return -10;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int WorshipConfig::InitRewardCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int total_weight = 0;

	while (!data_element.empty())
	{
		int weight = 0;
		int reward_gold_bind = 0;
		if (!PugiGetSubNodeValue(data_element, "weight", weight) || weight < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold_bind", reward_gold_bind) || reward_gold_bind <= 0)
		{
			return -2;
		}

		total_weight += weight;

		m_reward_cfg[weight] = reward_gold_bind;

		data_element = data_element.next_sibling();
	}

	if (10000 != total_weight)
	{
		return -9999;
	}

	return 0;
}

const WorshipOtherConfig * WorshipConfig::GetOtherConfig(int activity_type)
{
	auto it = m_other_cfg.find(activity_type);
	if (m_other_cfg.end() != it)
	{
		return &it->second;
	}

	return nullptr;
}

int WorshipConfig::GetReardGoldBind()
{
	int rand_num = RandomNum(10000);

	for (auto it = m_reward_cfg.begin(); it != m_reward_cfg.end(); ++it)
	{
		if (rand_num < it->first)
		{
			return it->second;
		}
		else
		{
			rand_num -= it->first;
		}
	}

	return 0;
}
