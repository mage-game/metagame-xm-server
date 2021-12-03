#include "crossshuijingconfig.hpp"
#include "servercommon/configcommon.h"

CrossShuiJingConfig::CrossShuiJingConfig() : m_activity_open_time_cfg_list_count(0)
{
	
}

CrossShuiJingConfig::~CrossShuiJingConfig()
{

}

CrossShuiJingConfig & CrossShuiJingConfig::Instance()
{
	static CrossShuiJingConfig instance;
	return instance;
}

bool CrossShuiJingConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossShuiJingConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load CrossShuiJingConfig config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}
	
	int iRet = 0;

	{																					// ¶ÁÈ¡other
		PugiXmlNode otherElem = RootElement.child("other");
		if (otherElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitOther(otherElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOther fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OpentimeListElem = RootElement.child("activity_open_time");
		if (OpentimeListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->activity_open_time]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OpentimeListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitActivityOpenTimeCfg fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode GatherListElem = RootElement.child("gather");
		if (GatherListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->gather]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitGather(GatherListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitGather fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}
	
	return true;
}

CrossShuijingActivityOpenTimeInfo *CrossShuiJingConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int CrossShuiJingConfig::GetOpenActivityConfigList(CrossShuijingActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

int CrossShuiJingConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElem, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_time", m_other_cfg.relive_time) || m_other_cfg.relive_time <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_max_times", m_other_cfg.gather_max_times) || m_other_cfg.gather_max_times <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_pos_x", m_other_cfg.realive_pos.x) || m_other_cfg.realive_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_pos_y", m_other_cfg.realive_pos.y) || m_other_cfg.realive_pos.y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_gold", m_other_cfg.gather_buff_gold) || m_other_cfg.gather_buff_gold <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_time", m_other_cfg.gather_buff_time) || m_other_cfg.gather_buff_time <= 0)
	{
		return -7;
	}

	return 0;
}

int CrossShuiJingConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!dataElement.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		CrossShuijingActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(dataElement, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
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

		++ m_activity_open_time_cfg_list_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossShuiJingConfig::InitGather(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int max_gahter_type = -1;

	while (!dataElem.empty())
	{
		int gather_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "gather_id", gather_id) || gather_id <= 0)
		{
			return -1;
		}

		CrossShuiJingGatherCfg &cfg = m_gather_cfg_map[gather_id];
		if (0 != cfg.gather_id)
		{
			return -100;
		}

		cfg.gather_id = gather_id;

		if (!PugiGetSubNodeValue(dataElem, "gather_type", cfg.gather_type))
		{
			return -2;
		}

		if (cfg.gather_type > max_gahter_type)
		{
			max_gahter_type = cfg.gather_type;
			m_big_shuijing_id = gather_id;
		}

		if (!PugiGetSubNodeValue(dataElem, "bind_gold", cfg.bind_gold) || cfg.bind_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "mojing", cfg.mojing) || cfg.mojing < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)
		{
			return -5;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

bool CrossShuiJingConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

const CrossShuiJingGatherCfg *CrossShuiJingConfig::GetGatherCfgByGatherId(int gather_id)
{
	std::map<int, CrossShuiJingGatherCfg>::iterator it = m_gather_cfg_map.find(gather_id);
	if (it == m_gather_cfg_map.end())
	{
		return NULL;
	}

	return &(it->second);
}
