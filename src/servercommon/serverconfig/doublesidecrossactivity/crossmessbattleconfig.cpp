#include "crossmessbattleconfig.hpp"

#include "servercommon/configcommon.h"

CrossMessBattleConfig::CrossMessBattleConfig() : m_activity_open_time_cfg_list_count(0)
{

}

CrossMessBattleConfig::~CrossMessBattleConfig()
{

}

CrossMessBattleConfig & CrossMessBattleConfig::Instance()
{
	static CrossMessBattleConfig _instance;
	return _instance;
}

bool CrossMessBattleConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path,"CrossMessBattleConfig",*err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 活动开启时间
		PugiXmlNode OtherElement = RootElement.child("activity_open_time");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossMessBattleConfig::InitOtherCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "is_open", m_other_cfg.is_open))
	{
		return -100;
	}

	return 0;
}

int CrossMessBattleConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
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

		CrossMessBattleActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

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

		++m_activity_open_time_cfg_list_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

CrossMessBattleActivityOpenTimeInfo *CrossMessBattleConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int CrossMessBattleConfig::GetOpenActivityConfigList(CrossMessBattleActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

bool CrossMessBattleConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}