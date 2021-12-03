#include "openserverconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "engineadapter.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <time.h>

OpenServerConfig::OpenServerConfig()
	: m_server_real_start_time(0), m_server_real_combine_time(0), m_server_version(0), m_opengame_activity_end_time(0), m_opengame_chongci_end_time(0), m_combine_server_activity_end_time(0), m_newhand_guide_end_time(0), 
	m_kill_monster_double_exp_end_time(0), m_combine_qingdian_activity_end_time(0), m_pingbi_worldexp_add(0), m_channel_open_level_world(0), m_channel_open_level_scene(0), m_channel_open_level_camp(0), m_channel_open_level_team(0),
	m_channel_open_level_guild(0), m_single_chat_level(0), m_send_mail_level(0), m_line_num(0), m_is_ios_examine(false)
{

}

OpenServerConfig::~OpenServerConfig()
{

}

bool OpenServerConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "OpenServerConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml node error in root";
		return false;
	}

	{
		m_server_real_start_time = 0;

		PugiXmlNode StartTimeElement = RootElement.child("server_real_start_time");
		if (StartTimeElement.empty())
		{
			*err = path + ": xml node error in [config->server_real_start_time]";
			return false;
		}

		if (StartTimeElement.first_child().empty() || !ConvertTimeStringToUnixTime(StartTimeElement.first_child().value(), &m_server_real_start_time))
		{
			*err = path + ": xml node error in [config->server_real_start_time], time format error";
			return false;
		}
	}

	{
		m_server_real_combine_time = 0;

		PugiXmlNode CombineTimeElement = RootElement.child("server_real_combine_time");
		if (CombineTimeElement.empty())
		{
			*err = path + ": xml node error in [config->server_real_combine_time]";
			return false;
		}

		if (CombineTimeElement.first_child().empty() || !ConvertTimeStringToUnixTime(CombineTimeElement.first_child().value(), &m_server_real_combine_time))
		{
			//*err = path + ": xml node error in [config->server_real_combine_time], time format error";
			//return false;
		}
	}

	if (!PugiGetSubNodeValue(RootElement, "server_version", m_server_version))
	{
		*err = path + ": xml node error in [config->server_version]";
		return false;
	}

	{
		int opengame_activity_day = 0;
		int opengame_chongci_day = 0;
		int combine_server_activity_day = 0;
		int newhand_guide_day = 0;
		int kill_monster_double_exp_day = 0;
		int combine_qingdian_activity_day = 0;

		if (!PugiGetSubNodeValue(RootElement, "opengame_activity_day", opengame_activity_day) || opengame_activity_day < 0)
		{
			*err = path + ": xml node error in [config->opengame_activity_day]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "opengame_chongci_day", opengame_chongci_day) || opengame_chongci_day < 0)
		{
			*err = path + ": xml node error in [config->opengame_chongci_day]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "combine_server_activity_day", combine_server_activity_day) || combine_server_activity_day < 0)
		{
			*err = path + ": xml node error in [config->combine_server_activity_day]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "newhand_guide_day", newhand_guide_day) || newhand_guide_day < 0)
		{
			*err = path + ": xml node error in [config->newhand_guide_day]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "kill_monster_double_exp_day", kill_monster_double_exp_day) || kill_monster_double_exp_day < 0)
		{
			*err = path + ": xml node error in [config->kill_monster_double_exp_day]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "combine_qingdian_activity_day", combine_qingdian_activity_day) || combine_qingdian_activity_day < 0)
		{
			*err = path + ": xml node error in [config->combine_qingdian_activity_day]";
			return false;
		}

		struct tm *server_start_local_begin_time = ::localtime(&m_server_real_start_time);
		if (NULL == server_start_local_begin_time)
		{
			*err = path + ": xml error server_start_localtime";
			return false;
		}

		time_t zero_time = m_server_real_start_time - (server_start_local_begin_time->tm_hour * 3600 + server_start_local_begin_time->tm_min * 60 + server_start_local_begin_time->tm_sec);

		m_opengame_activity_end_time = zero_time + (opengame_activity_day * 24 * 3600) + 5;
		m_opengame_chongci_end_time = zero_time + (opengame_chongci_day * 24 * 3600);
		m_newhand_guide_end_time = zero_time + (newhand_guide_day * 24 * 3600);
		m_kill_monster_double_exp_end_time = zero_time + (kill_monster_double_exp_day * 24 * 3600);

		struct tm *server_combine_local_begin_time = ::localtime(&m_server_real_combine_time);
		if (NULL == server_combine_local_begin_time)
		{
			*err = path + ": xml error server_combine_localtime";
			return false;
		}

		m_combine_server_activity_end_time = m_server_real_combine_time - (server_combine_local_begin_time->tm_hour * 3600 + server_combine_local_begin_time->tm_min * 60 + server_combine_local_begin_time->tm_sec) + (combine_server_activity_day * 24 * 3600);
		m_combine_qingdian_activity_end_time = m_server_real_combine_time - (server_combine_local_begin_time->tm_hour * 3600 + server_combine_local_begin_time->tm_min * 60 + server_combine_local_begin_time->tm_sec) + (combine_qingdian_activity_day * 24 * 3600) + 5;
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "channel_open_level_world", m_channel_open_level_world) || m_channel_open_level_world < 0)
		{
			*err = path + ": xml node error in [config->channel_open_level_world]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "channel_open_level_scene", m_channel_open_level_scene) || m_channel_open_level_scene < 0)
		{
			*err = path + ": xml node error in [config->channel_open_level_scene]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "channel_open_level_camp", m_channel_open_level_camp) || m_channel_open_level_camp < 0)
		{
			*err = path + ": xml node error in [config->channel_open_level_camp]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "channel_open_level_team", m_channel_open_level_team) || m_channel_open_level_team < 0)
		{
			*err = path + ": xml node error in [config->channel_open_level_team]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "channel_open_level_guild", m_channel_open_level_guild) || m_channel_open_level_guild < 0)
		{
			*err = path + ": xml node error in [config->channel_open_level_guild]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "single_chat_level", m_single_chat_level) || m_single_chat_level < 0)
		{
			*err = path + ": xml node error in [config->single_chat_level]";
			return false;
		}
		if (!PugiGetSubNodeValue(RootElement, "send_mail_level", m_send_mail_level) || m_send_mail_level < 0)
		{
			*err = path + ": xml node error in [config->send_mail_level]";
			return false;
		}
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "line_num", m_line_num) || m_line_num <= 0)
		{
			*err = path + ": xml node error in [config->line_num]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "is_ios_examine", m_is_ios_examine))
		{
			*err = path + ": xml node error in [config->is_ios_examine]";
			return false;
		}
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "pingbi_worldexp_add", m_pingbi_worldexp_add))
		{
			m_pingbi_worldexp_add = 0;
		}
	}

	return true;
}

time_t OpenServerConfig::GetOpenServerZeroTime() const
{
	time_t now_second = EngineAdapter::Instance().Time();
	time_t today_zero_timestamp = GetZeroTime(now_second);
	int open_day = GetDayIndex(m_server_real_start_time, now_second);
	time_t open_server_zero_timestamp = today_zero_timestamp - SECOND_PER_DAY * open_day;

	return open_server_zero_timestamp;
}

int OpenServerConfig::GetOpenServerDayIndex()
{
	return GetDayIndex(m_server_real_start_time, EngineAdapter::Instance().Time());
}

void OpenServerConfig::GetOpenGameActivityTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (NULL != begin_time) *begin_time = static_cast<unsigned int>(m_server_real_start_time);
	if (NULL != end_time) *end_time = static_cast<unsigned int>(m_opengame_activity_end_time);
}

void OpenServerConfig::GetOpenGameChongCiTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (NULL != begin_time) *begin_time = static_cast<unsigned int>(m_server_real_start_time);
	if (NULL != end_time) *end_time = static_cast<unsigned int>(m_opengame_chongci_end_time);
}

void OpenServerConfig::GetCombineServerActivityTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (NULL != begin_time) *begin_time = static_cast<unsigned int>(m_server_real_combine_time);
	if (NULL != end_time) *end_time = static_cast<unsigned int>(m_combine_server_activity_end_time);
}

void OpenServerConfig::GetKillMonsterDoubleExpTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (NULL != begin_time) *begin_time = static_cast<unsigned int>(m_server_real_start_time);
	if (NULL != end_time) *end_time = static_cast<unsigned int>(m_kill_monster_double_exp_end_time);
}

void OpenServerConfig::GetCombineQingdianActivityTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (NULL != begin_time) *begin_time = static_cast<unsigned int>(m_server_real_combine_time);
	if (NULL != end_time) *end_time = static_cast<unsigned int>(m_combine_qingdian_activity_end_time);
}

