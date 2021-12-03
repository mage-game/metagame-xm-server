#include "crosshotspringconfig.hpp"

#include <servercommon/configcommon.h>

CrossHotSpringConfig::CrossHotSpringConfig(): m_enter_pos(0, 0),m_scene_id(0),m_refresh_rank_interval_s(0),m_cross_info_clear_time(0)
{

}

CrossHotSpringConfig::~CrossHotSpringConfig()
{

}

CrossHotSpringConfig & CrossHotSpringConfig::Instance()
{
	static CrossHotSpringConfig cvcc;
	return cvcc;
}

bool CrossHotSpringConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossHotSpringConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode ActivityTimeCfgElement = RootElement.child("activity_time_cfg");
		if (ActivityTimeCfgElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg]";
			return false;
		}

		if (!PugiGetSubNodeValue(ActivityTimeCfgElement, "is_open", m_activity_time_cfg.is_open))
		{
			*err = path + ": xml node error in [config->activity_time_cfg->is_open]";
			return false;
		}

		PugiXmlNode OpenDayListElement = ActivityTimeCfgElement.child("open_day_list");
		if (OpenDayListElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list]";
			return false;
		}

		PugiXmlNode OpenDayElement = OpenDayListElement.child("open_day");
		if (OpenDayElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
			return false;
		}

		while (!OpenDayElement.empty())
		{
			int open_day = 0;
			if (!PugiGetNodeValue(OpenDayElement, open_day) || open_day <= 0 || open_day > CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			open_day = open_day % CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS;
			if (open_day < 0 || open_day >= CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			m_activity_time_cfg.open_day_list[open_day] = true;

			OpenDayElement = OpenDayElement.next_sibling();
		}

		PugiXmlNode OpenTimeListElement = ActivityTimeCfgElement.child("day_open_time_list");
		if (OpenTimeListElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->day_open_time_list]";
			return false;
		}

		PugiXmlNode OpenTimestampElement = OpenTimeListElement.child("open_timestamp");
		if (OpenTimestampElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_timestamp]";
			return false;
		}

		m_activity_time_cfg.open_day_secon_count = 0;

		while (!OpenTimestampElement.empty())
		{
			if (m_activity_time_cfg.open_day_secon_count < 0 || m_activity_time_cfg.open_day_secon_count >= MAX_ACTIVIYT_TIME_CFG_NUM)
			{
				*err = path + ": m_activity_time_cfg.open_day_secon_count more than MAX_ACTIVIYT_TIME_CFG_NUM";
				return false;
			}

			int hour = 0, minute = 0, second = 0;
			if (!PugiGetSubNodeValue(OpenTimestampElement, "hour", hour) || hour < 0 || hour >= 24)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->time_interval->hour]";
				return false;
			}
			if (!PugiGetSubNodeValue(OpenTimestampElement, "minute", minute) || minute < 0 || minute >= 60)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->time_interval->minute]";
				return false;
			}
			if (!PugiGetSubNodeValue(OpenTimestampElement, "second", second) || second < 0 || second >= 60)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->time_interval->second]";
				return false;
			}

			m_activity_time_cfg.open_day_second[m_activity_time_cfg.open_day_secon_count] = hour * 3600 + minute * 60 + second;
			m_activity_time_cfg.open_day_secon_count++;

			OpenTimestampElement = OpenTimestampElement.next_sibling();
		}

		if (m_activity_time_cfg.open_day_secon_count <= 0)
		{
			*err = path + ": m_activity_time_cfg.open_day_secon_count less than 1";
			return false;
		}

		PugiXmlNode IntervalElement = ActivityTimeCfgElement.child("time_interval");
		if (IntervalElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval]";
			return false;
		}
		
		int standby_dur_s = 0, open_dur_s = 0;
		if (!PugiGetSubNodeValue(IntervalElement, "standby_dur_s", standby_dur_s) || standby_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->standby_dur_s]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "open_dur_s", open_dur_s) || open_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->open_dur_s]";
			return false;
		}

		m_activity_time_cfg.standby_dur_s = standby_dur_s;
		m_activity_time_cfg.open_dur_s = open_dur_s;
	}
	{
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		if (!PugiGetSubNodeValue(OtherElement, "enter_pos_x", m_enter_pos.x) || m_enter_pos.x <= 0)
		{
			*err = path + ": xml node error in [config->other->pos_1_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(OtherElement, "enter_pos_y", m_enter_pos.y) || m_enter_pos.y <= 0)
		{
			*err = path + ": xml node error in [config->other->pos_1_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(OtherElement, "scene_id", m_scene_id) || m_scene_id <= 0)
		{
			*err = path + ": xml node error in [config->other->scene_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(OtherElement, "refresh_rank_interval_s", m_refresh_rank_interval_s) || m_refresh_rank_interval_s <= 0)
		{
			*err = path + ": xml node error in [config->other->refresh_rank_interval_s]";
			return false;
		}

		if (!PugiGetSubNodeValue(OtherElement, "cross_info_clear_time", m_cross_info_clear_time) || m_cross_info_clear_time <= 0)
		{
			*err = path + ": xml node error in [config->other->cross_info_clear_time]";
			return false;
		}
	}

	return true;
}


