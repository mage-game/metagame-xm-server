#include "husongshuijingconfig.hpp"
#include <string>

HusongShuijingConfig::HusongShuijingConfig()
{
}

HusongShuijingConfig::~HusongShuijingConfig()
{

}

HusongShuijingConfig & HusongShuijingConfig::Instance()
{
	static HusongShuijingConfig ins;
	return ins;
}

bool HusongShuijingConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document,configname,"HusongShuijingConfig",*err)

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
		PugiXmlNode gather_element = RootElement.child("gather_cfg");
		if (gather_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitGatherCfg(gather_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGatherCfg failed %d", configname.c_str(), iRet);
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

	{
		PugiXmlNode open_element = RootElement.child("activity_open_time");
		if (open_element.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->activity_open_time]", configname.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(open_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitActivityOpenTimeCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int HusongShuijingConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement,"is_open",m_other_cfg.is_open))
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "pos_x", m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "pos_y", m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_day_count", m_other_cfg.gather_day_count) || m_other_cfg.gather_day_count < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "commit_count", m_other_cfg.commit_count) || m_other_cfg.commit_count < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "dec_speed_per", m_other_cfg.dec_speed_per) || m_other_cfg.dec_speed_per < 0)
	{
		return -7;
	}

	return 0;
}

int HusongShuijingConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	std::set<int> gather_id_set;
	while (!dataElement.empty())
	{
		HusongSJGatherCfg gather_cfg;

		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather_cfg.gather_id) || gather_cfg.gather_id <= 0)
		{
			return -3;
		}
	
		if (!PugiGetSubNodeValue(dataElement, "gather_pos_x", gather_cfg.gather_pos.x) || gather_cfg.gather_pos.x <= 0)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_pos_y", gather_cfg.gather_pos.y) || gather_cfg.gather_pos.y <= 0)
		{
			return -41;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time", gather_cfg.gather_time) || gather_cfg.gather_time <= 0)
		{
			return -42;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_change_times", gather_cfg.gather_change_times) || gather_cfg.gather_change_times < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_min_times", gather_cfg.gather_min_times) || gather_cfg.gather_min_times <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_max_times", gather_cfg.gather_max_times) || gather_cfg.gather_max_times < gather_cfg.gather_min_times)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "commit_npcid", gather_cfg.commit_npcid) || gather_cfg.commit_npcid <= 0)
		{
			return -8;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "gather_day_count", gather_cfg.gather_day_count) || gather_cfg.gather_day_count <= 0)
// 		{
// 			return -9;
// 		}

		if (!PugiGetSubNodeValue(dataElement, "husong_timeout", gather_cfg.husong_timeout) || gather_cfg.husong_timeout <= 0 || gather_cfg.husong_timeout > 3600)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_cost", gather_cfg.buy_cost) || gather_cfg.buy_cost <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval_s", gather_cfg.refresh_interval_s) || gather_cfg.refresh_interval_s <= 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "type", gather_cfg.gather_type) || gather_cfg.gather_type != static_cast<int>(m_gather_cfg_vec.size()))
		{
			return -13;
		}

		m_gather_cfg_vec.push_back(gather_cfg);

		if (!gather_id_set.insert(gather_cfg.gather_id).second)
		{
			return -14;
		}

		dataElement = dataElement.next_sibling();
	}

	if (static_cast<int>(m_gather_cfg_vec.size()) != HUSONG_SHUIJING_TYPE_MAX)
	{
		return -17;
	}
	
	return 0;
}

int HusongShuijingConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	m_reward_vec.resize(m_gather_cfg_vec.size());

	int last_commit_times = 0;
	int last_gather_type = -1;
	while(!dataElement.empty())
	{
		int gather_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "type", gather_type) || gather_type < 0 || gather_type >= static_cast<int>(m_gather_cfg_vec.size()))
		{
			return -1000;
		}

		HusongSJRewardVec &reward_vec = m_reward_vec[gather_type];
		if (last_gather_type != gather_type)
		{
			last_gather_type = gather_type;
			last_commit_times = 0;
		}

		HusongSJRewardCfg item_cfg;
		item_cfg.gather_type = gather_type;

		if (!PugiGetSubNodeValue(dataElement, "commit_times", item_cfg.commit_times) || item_cfg.commit_times < last_commit_times)
		{
			return -1;
		}
		last_commit_times = item_cfg.commit_times;

		int item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "total_reward_item", item_cfg.total_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 + item_count;
		}
		item_cfg.total_reward_item_count = item_count;

		item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "be_robbed_reward_item", item_cfg.be_robbed_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -200 + item_count;
		}
		item_cfg.be_robbed_reward_item_count = item_count;

		reward_vec.push_back(item_cfg);

		dataElement = dataElement.next_sibling();
	}

	return  0;
}

int HusongShuijingConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!data_element.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
		{
			return -1;
		}

		HusongSJActivityOpenTimeCfg &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(data_element, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool HusongShuijingConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

const HusongSJGatherCfg * HusongShuijingConfig::GetGatherCfg(int gather_type) const
{
	if (gather_type < 0 || gather_type >= static_cast<int>(m_gather_cfg_vec.size()))
	{
		return nullptr;
	}

	return &m_gather_cfg_vec[gather_type];
}

Posi HusongShuijingConfig::GetEnterPos(int gather_type)
{
	if (gather_type < 0 || gather_type >= static_cast<int>(m_gather_cfg_vec.size()))
	{
		return Posi(0,0);
	}

	return m_gather_cfg_vec[gather_type].gather_pos;
}

const HusongSJGatherCfg * HusongShuijingConfig::GetGatherCfgByGatherID(int gather_id) const
{
	for (const auto &cfg : m_gather_cfg_vec)
	{
		if (cfg.gather_id == gather_id)
		{
			return &cfg;
		}
	}

	return nullptr;
}

const HusongSJRewardCfg * HusongShuijingConfig::GetJingHuaRewardCfg(int gather_type, int commit_times) const
{
	if (gather_type < 0 || gather_type >= static_cast<int>(m_reward_vec.size()))
	{
		return nullptr;
	}

	if (commit_times < 0) return nullptr;

	const HusongSJRewardVec &reward_vec = m_reward_vec[gather_type];

	for (HusongSJRewardVec::const_iterator iter = reward_vec.cbegin(); reward_vec.cend() != iter; ++ iter)
	{
		if (commit_times <= iter->commit_times)
		{
			return &(*iter);
		}
	}

	return nullptr;
}

HusongSJActivityOpenTimeCfg *HusongShuijingConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int HusongShuijingConfig::GetOpenActivityConfigList(HusongSJActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}