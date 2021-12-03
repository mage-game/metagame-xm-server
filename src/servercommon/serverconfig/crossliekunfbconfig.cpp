#include "crossliekunfbconfig.hpp"
#include "servercommon/configcommon.h"


CrossLieKunFbConfig::CrossLieKunFbConfig() : m_activity_open_time_cfg_list_count(0), m_zone_cfg_list_count(0)
{
}

CrossLieKunFbConfig::~CrossLieKunFbConfig()
{
}

CrossLieKunFbConfig & CrossLieKunFbConfig::Instance()
{
	static CrossLieKunFbConfig _instance;
	return _instance;
}

bool CrossLieKunFbConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossLieKunFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¿ªÆôÊ±¼äÅäÖÃ
		PugiXmlNode activity_open_time_element = RootElement.child("activity_open_time");
		if (activity_open_time_element.empty())
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(activity_open_time_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¹Ø¿¨ÅäÖÃ
		PugiXmlNode zone_element = RootElement.child("zone");
		if (zone_element.empty())
		{
			*err = path + ": xml node error in [config->zone]";
			return false;
		}

		iRet = this->InitZoneCfg(zone_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitZoneCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ½±Àø×ø±ê
		PugiXmlNode reward_pos_element = RootElement.child("reward_pos");
		if (reward_pos_element.empty())
		{
			*err = path + ": xml node error in [config->reward_pos]";
			return false;
		}

		iRet = this->InitRewardPosCfg(reward_pos_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRewardPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ½±ÀøÅäÖÃ
		PugiXmlNode reward_element = RootElement.child("reward");
		if (reward_element.empty())
		{
			*err = path + ": xml node error in [config->reward]";
			return false;
		}

		iRet = this->InitRewardCfg(reward_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}
bool CrossLieKunFbConfig::ReadPosConfig(const PugiXmlNode &element, const char *node, Posi & pos)
{
	std::string tmp_str;
	if (!PugiGetSubNodeValue(element, node , tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	char *token = strtok(buffer, ",");
	if (NULL == token)
	{
		return false;
	}
	if (!StringToBaseType(token, pos.x))
	{
		return false;
	}

	token = strtok(NULL, ",");
	if (NULL == token)
	{
		return false;
	}

	if (!StringToBaseType(token, pos.y))
	{
		return false;
	}

	return true;
}
int CrossLieKunFbConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(data_element, "is_open", is_open))
	{
		return -1;
	}

	m_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(data_element, "open_level", m_other_cfg.open_level))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_time_limit_s", m_other_cfg.enter_time_limit_s) || m_other_cfg.enter_time_limit_s <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_zone_boss_s", m_other_cfg.refresh_zone_boss_s) || m_other_cfg.refresh_zone_boss_s <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_minor_zone_boss_s", m_other_cfg.refresh_minor_zone_boss_s) || m_other_cfg.refresh_minor_zone_boss_s <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_main_zone_boss_s", m_other_cfg.refresh_main_zone_boss_s) || m_other_cfg.refresh_main_zone_boss_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "relive_need_gold", m_other_cfg.relive_need_gold) || m_other_cfg.relive_need_gold <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_time_s", m_other_cfg.gather_time_s) || m_other_cfg.gather_time_s <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_times", m_other_cfg.gather_times) || m_other_cfg.gather_times <= 0)
	{
		return -9;
	}

	PugiXmlNode title_item_element = data_element.child("title_item");
	if (title_item_element.empty())
	{
		return -10;
	}

	if (!m_other_cfg.title_item.ReadConfigNoCheck(title_item_element))
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "portal_id", m_other_cfg.portal_id) || m_other_cfg.portal_id <= 0)
	{
		return -12;
	}

 	if (!ReadPosConfig(data_element, "portal_flush_pos", m_other_cfg.portal_flush_pos))
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "portal_item_offset", m_other_cfg.portal_item_offset) || m_other_cfg.portal_item_offset <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "portal_time_s", m_other_cfg.portal_time_s) || m_other_cfg.portal_time_s < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "effective_range", m_other_cfg.effective_range) || m_other_cfg.effective_range <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "title_id", m_other_cfg.title_id) || m_other_cfg.title_id <= 0)
	{
		return -17;
	}
	
	return 0;
}

bool CrossLieKunFbConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

int CrossLieKunFbConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
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

		CrossLieKunFbActivityOpenTimeCfg &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

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

int CrossLieKunFbConfig::InitZoneCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int zone = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "zone", zone) || zone < 0 || zone >= LIEKUN_ZONE_TYPE_COUNT)
		{
			return -1;
		}

		CrossLieKunFbZoneCfg &cfg = m_zone_cfg_list[zone];
		cfg.zone = zone;

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2;
		}

 		if (!ReadPosConfig(data_element, "enter_pos", cfg.enter_pos))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id_0", cfg.boss_id_arr[0]) || cfg.boss_id_arr[0] <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id_1", cfg.boss_id_arr[1]) || cfg.boss_id_arr[1] <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id_2", cfg.boss_id_arr[2]) || cfg.boss_id_arr[2] <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id_3", cfg.boss_id_arr[3]) || cfg.boss_id_arr[3] <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id_4", cfg.boss_id_arr[4]) || cfg.boss_id_arr[4] <= 0)
		{
			return -8;
		}

		if (!ReadPosConfig(data_element, "boss_pos_0", cfg.boss_pos_arr[0]))
		{
			return -9;
		}

		if (!ReadPosConfig(data_element, "boss_pos_1", cfg.boss_pos_arr[1]))
		{
			return -10;
		}

		if (!ReadPosConfig(data_element, "boss_pos_2", cfg.boss_pos_arr[2]))
		{
			return -11;
		}

		if (!ReadPosConfig(data_element, "boss_pos_3", cfg.boss_pos_arr[3]))
		{
			return -12;
		}

		if (!ReadPosConfig(data_element, "boss_pos_4", cfg.boss_pos_arr[4]))
		{
			return -13;
		}

		++m_zone_cfg_list_count;
		if (m_zone_cfg_list_count > LIEKUN_ZONE_TYPE_COUNT)
		{
			return -20000;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossLieKunFbConfig::InitRewardPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CrossLieKunRewardPosCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "zone", cfg.zone) || cfg.zone < 0 || cfg.zone >= LIEKUN_ZONE_TYPE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || cfg.boss_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_id", cfg.gather_id) || cfg.gather_id <= 0)
		{
			return -3;
		}

 		if (!ReadPosConfig(data_element, "gather_flush_pos",cfg.gather_flush_pos))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_item_offset", cfg.gather_item_offset) || cfg.gather_item_offset <= 0)
		{
			return -5;
		}

		m_reward_pos_cfg_list.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossLieKunFbConfig::InitRewardCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CrossLieKunRewardCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "gather_id", cfg.gather_id) || cfg.gather_id <= 0)
		{
			return -1;
		}

		PugiXmlNode gather_item_list_element = data_element.child("gather_item_list");
		if (gather_item_list_element.empty())
		{
			return -2;
		}

		PugiXmlNode gather_item_element = gather_item_list_element.child("gather_item");
		if (gather_item_element.empty())
		{
			return -3;
		}

		while (gather_item_element)
		{
			if (cfg.gather_count >= LIEKUN_MAX_GATHER_ITEM_COUNT)
			{
				return -4;
			}

			if (!cfg.gather_item_list[cfg.gather_count].ReadConfigNoCheck(gather_item_element))
			{
				return -5;
			}

			++cfg.gather_count;
			gather_item_element = gather_item_element.next_sibling();
		}

		PugiXmlNode suipian_item_element = data_element.child("suipian_item");
		if (suipian_item_element.empty())
		{
			return -6;
		}

		if (!cfg.suipian_item.ReadConfigNoCheck(suipian_item_element))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "suipian_drop_rate", cfg.suipian_drop_rate) || cfg.suipian_drop_rate < 0)
		{
			return -8;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -9;
		}

		if (!cfg.reward_item.ReadConfigNoCheck(reward_item_element))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_drop_rate", cfg.reward_drop_rate) || cfg.reward_drop_rate < 0)
		{
			return -11;
		}

		CrossLieKunRewardCfgMapIt it = m_reward_cfg_map.find(cfg.gather_id);
		if (it != m_reward_cfg_map.end())
		{
			return -12;
		}

		m_reward_cfg_map[cfg.gather_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

CrossLieKunFbActivityOpenTimeCfg *CrossLieKunFbConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int CrossLieKunFbConfig::GetOpenActivityConfigList(CrossLieKunFbActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

int CrossLieKunFbConfig::GetZoneIndexBySceneId(int scene_id)
{
	for (int i = 0; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
	{
		if (m_zone_cfg_list[i].scene_id == scene_id)
		{
			return m_zone_cfg_list[i].zone;
		}
	}

	return -1;
}

void CrossLieKunFbConfig::GetBossIdList(int scene_id, int boss_id_list[LIEKUN_BOSS_TYPE_COUNT])
{
	bool find = false;
	int i = 0;
	for (; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
	{
		if (m_zone_cfg_list[i].scene_id == scene_id)
		{
			find = true;
			break;
		}
	}

	if (find)
	{
		for (int num = 0; num < LIEKUN_BOSS_TYPE_COUNT; ++num)
		{
			boss_id_list[num] = m_zone_cfg_list[i].boss_id_arr[num];
		}
	}
}

const CrossLieKunFbZoneCfg *CrossLieKunFbConfig::GetCrossLieKunFbZoneCfg(int zone) const
{
	if (zone < 0 || zone >= m_zone_cfg_list_count)
	{
		return NULL;
	}
	return &m_zone_cfg_list[zone];
}

const CrossLieKunRewardPosCfg *CrossLieKunFbConfig::GetCrossLieKunRewardPosCfgByBossId(int zone, int boss_id) const
{
	if (zone < 0 || zone >= m_zone_cfg_list_count || boss_id <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < (int)m_reward_pos_cfg_list.size(); ++i)
	{
		if (m_reward_pos_cfg_list[i].zone == zone && m_reward_pos_cfg_list[i].boss_id == boss_id)
		{
			return &m_reward_pos_cfg_list[i];
		}
	}

	return NULL;
}

const CrossLieKunRewardPosCfg *CrossLieKunFbConfig::GetCrossLieKunRewardPosCfgByGatherId(int zone, int gather_id) const
{
	if (zone < 0 || zone >= m_zone_cfg_list_count || gather_id <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < (int)m_reward_pos_cfg_list.size(); ++i)
	{
		if (m_reward_pos_cfg_list[i].zone == zone && m_reward_pos_cfg_list[i].gather_id == gather_id)
		{
			return &m_reward_pos_cfg_list[i];
		}
	}

	return NULL;
}

bool CrossLieKunFbConfig::GetCrossLieKunRewardItemList(int gather_id, ItemConfigData gather_item_list[LIEKUN_MAX_ITEM_COUNT], int &item_count, int &suipian_id)
{
	bool is_drop_suipian = false;
	int rand_num = RandomNum(1,10001);

	CrossLieKunRewardCfgMapIt it = m_reward_cfg_map.find(gather_id);
	if (it != m_reward_cfg_map.end())
	{
		item_count = it->second.gather_count;
		for (int i = 0; i < item_count; ++i)
		{
			gather_item_list[i] = it->second.gather_item_list[i];
		}

		if (rand_num <= it->second.suipian_drop_rate)
		{
			is_drop_suipian = true;
			suipian_id = it->second.suipian_item.item_id;
			gather_item_list[item_count++] = it->second.suipian_item;
		}
		if (rand_num <= it->second.reward_drop_rate)
		{
			gather_item_list[item_count++] = it->second.reward_item;
		}
	}

	return is_drop_suipian;
}
