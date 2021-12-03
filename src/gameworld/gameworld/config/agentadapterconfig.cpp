#include "agentadapterconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "gameworld/globalconfig/globalconfig.h"

AgentAdapterConfig::AgentAdapterConfig() : m_single_chat_must_be_friend_flag(0), m_single_chat_must_be_vip_flag(0), m_chat_limit_condition_type_flag(0),
	m_is_forbid_gm_account_offline_too_long(false), m_is_create_role_mail_notice(false), m_is_forbid_change_avatar(false), m_is_forbid_cross_speaker(false)
{

}

AgentAdapterConfig::~AgentAdapterConfig()
{

}

bool AgentAdapterConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };
	TiXmlDocument document;

	if (path == "" || !document.LoadFile(path))
	{
		*err = path + "：Load AgentAdapter Error,\n" + document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		LOAD_CHILD_ELEMENT_CONFIG(RootElement, other, InitOtherCfg);
	}

	{
		// 渠道配置
		TiXmlElement *root_element = RootElement->FirstChildElement("single_chat_friend_limit");
		if (NULL == root_element)
		{
			*err = path + ": xml no [single_chat_friend_limit] node";
			return false;
		}

		iRet = this->InitSingleChatFriendLimitCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitSingleChatFriendLimitCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		LOAD_CHILD_ELEMENT_CONFIG(RootElement, channel_chat_limit, InitChannelChatLimitCfg);
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("single_chat_vip_limit");
		if (NULL == root_element)
		{
			*err = path + ": xml no [single_chat_vip_limit] node";
			return false;
		}

		iRet = this->InitSingleChatVipLimitCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitSingleChatVipLimitCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	LOAD_CHILD_ELEMENT_CONFIG(RootElement, gm_account_forbid, InitGmAccountForbidCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, create_role_mail_notice, InitCreateRoleMailNoticeCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, forbid_change_avatar, InitForbidChangeAvatarCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, cross_chat, InitCrossChatCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, single_chat, InitSingleChatCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, num_first_group, InitOpenFirstChargeFakeCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, auto_clear_guild_member, InitAutoClearGuildMemberCfg);
	LOAD_CHILD_ELEMENT_CONFIG(RootElement, cross_speaker, InitForbidCrossSpeaker);

	return true;
}

bool AgentAdapterConfig::IsSingleChatCanOffline()
{
	int plat_type = LocalConfig::Instance().GetPlatType();
	auto it = m_platform_single_chat_map.find(plat_type);
	if (it != m_platform_single_chat_map.end())
	{
		return (it->second.can_offline_chat != 0) ? true : false;
	}

	return false;
}

bool AgentAdapterConfig::IsSingleChatCanAckChat()
{
	int plat_type = LocalConfig::Instance().GetPlatType();
	auto it = m_platform_single_chat_map.find(plat_type);
	if (it != m_platform_single_chat_map.end())
	{
		return (it->second.can_ack_chat != 0) ? true : false;
	}

	return false;
}

const ChannelChatLimitCfg * AgentAdapterConfig::GetChannelChatLimitCfg(int channel_type)
{
	if (channel_type < OPENLEVEL_LIMIT_TYPE_WORLD || channel_type >= OPENLEVEL_LIMIT_TYPE_MAX)
	{
		return nullptr;
	}

	return &m_channel_chat_limit_cfg_list[channel_type];
}

void AgentAdapterConfig::CmdSetChannelChatLevel(int channel_type, int level)
{
	if (channel_type >= OPENLEVEL_LIMIT_TYPE_WORLD && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
	{
		m_channel_chat_limit_cfg_list[channel_type].level_limit = level;
	}
	else
	{
		for (int i = OPENLEVEL_LIMIT_TYPE_WORLD; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
		{
			m_channel_chat_limit_cfg_list[i].level_limit = level;
		}
	}
}

bool AgentAdapterConfig::IsForbidCrossChat(int platform_type)
{
	auto it = m_platform_cross_chat_map.find(platform_type);
	if (it != m_platform_cross_chat_map.end())
	{
		return (it->second.is_forbid_cross_chat != 0) ? true : false;
	}
	
	return false;
}

bool AgentAdapterConfig::IsCanbeViewOtherServer(int platform_type)
{
	auto it = m_platform_cross_chat_map.find(platform_type);
	if (it != m_platform_cross_chat_map.end())
	{
		return (it->second.can_be_view_other_server != 0) ? true : false;
	}

	return true;
}

int AgentAdapterConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	if (dataElement != NULL)
	{
		if (!GetSubNodeValue(dataElement, "gm_account_forbid_offline_interval", m_other_cfg.gm_account_forbid_offline_interval) || m_other_cfg.gm_account_forbid_offline_interval <= 0)
		{
			return -1;
		}
	}

	return 0;
}

int AgentAdapterConfig::InitSingleChatFriendLimitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_single_chat_must_be_friend_flag = 1;
	}

	return 0;
}

int AgentAdapterConfig::InitChannelChatLimitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -1000;
	}
	
	int plat_type = LocalConfig::Instance().GetPlatType();
	int channel_chat_type = 0;
	PlatUnion plat_union;
	plat_union.id = 0;
	bool has_cfg = false;

	int limit_condition_type = 0;

	while (nullptr != data_element)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string plat_string;
		if (!GetSubNodeValue(data_element, "spid", plat_string) || plat_string.empty() || plat_string.length() >= sizeof(PlatName))
		{
			return -1;
		}

		SNPRINTF(plat_name, sizeof(plat_name), "%s", plat_string.c_str());

		for (int i = 0; i < 3; i++)
		{
			if ('\0' != plat_name[i])
			{
				plat_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
			}
		}

		// 默认或者配置相对应的渠道
		if (plat_union.id == plat_type)
		{
			has_cfg = true;

			if (!GetSubNodeValue(data_element, "channel_chat_type", channel_chat_type) || channel_chat_type < OPENLEVEL_LIMIT_TYPE_WORLD || channel_chat_type > OPENLEVEL_LIMIT_TYPE_MAX)
			{
				return -2;
			}

			ChannelChatLimitCfg &limit_cfg = m_channel_chat_limit_cfg_list[channel_chat_type];

			if (!GetSubNodeValue(data_element, "level_limit", limit_cfg.level_limit) || limit_cfg.level_limit < 0)
			{
				return -3;
			}

			if (!GetSubNodeValue(data_element, "vip_level_limit", limit_cfg.vip_level_limit) || limit_cfg.vip_level_limit < 0)
			{
				return -4;
			}

			if (!GetSubNodeValue(data_element, "limit_condition_type", limit_condition_type) || limit_condition_type < 0)
			{
				return -5;
			}

			if (!GetSubNodeValue(data_element, "forbid_chat_begin_time", limit_cfg.forbid_chat_begin_hour) || limit_cfg.forbid_chat_begin_hour >= 24)
			{
				return -6;
			}

			if (!GetSubNodeValue(data_element, "forbid_chat_end_time", limit_cfg.forbid_chat_end_hour) || limit_cfg.forbid_chat_end_hour < limit_cfg.forbid_chat_begin_hour)
			{
				return -7;
			}

			if (!GetSubNodeValue(data_element, "not_save_chat", limit_cfg.not_save_chat) || limit_cfg.not_save_chat < 0)
			{
				return -8;
			}

			if (limit_condition_type > 0)
			{
				m_chat_limit_condition_type_flag |= (1 << channel_chat_type);
			}
		}

		data_element = data_element->NextSiblingElement();
	}

	if (!has_cfg)
	{
		int level_limit = 0;

		for (int i = OPENLEVEL_LIMIT_TYPE_WORLD; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
		{
			switch (i)
			{
			case OPENLEVEL_LIMIT_TYPE_WORLD:
				{
					level_limit = SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelWorld();
				}
				break;

			case OPENLEVEL_LIMIT_TYPE_CAMP:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelCamp();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_SCENE:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelScene();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_TEAM:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelTeam();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_GUILD:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelGuild();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_SINGLE:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetSingleChatLevel();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_SEND_MAIL:
			{
				level_limit = SHAREDCONFIG->GetOpenServerConfig().GetSendMailLevel();
			}
			break;

			case OPENLEVEL_LIMIT_TYPE_SPEAKER:
			{
				level_limit = GLOBALCONFIG->GetSpeakerLevel();
			}
			break;

			}
			
			m_channel_chat_limit_cfg_list[i].level_limit = level_limit;
		}
	}

	return 0;
}

int AgentAdapterConfig::InitSingleChatVipLimitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_single_chat_must_be_vip_flag = 1;
	}

	return 0;
}

int AgentAdapterConfig::InitGmAccountForbidCfg(TiXmlElement *RootElement)
{
	m_is_forbid_gm_account_offline_too_long = false;

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_is_forbid_gm_account_offline_too_long = true;
	}

	return 0;
}

int AgentAdapterConfig::InitCreateRoleMailNoticeCfg(TiXmlElement *RootElement)
{
	m_is_create_role_mail_notice = false;

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_is_create_role_mail_notice = true;
	}

	return 0;
}

int AgentAdapterConfig::InitForbidChangeAvatarCfg(TiXmlElement *RootElement)
{
	m_is_forbid_change_avatar = false;

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_is_forbid_change_avatar = true;
	}

	return 0;
}

int AgentAdapterConfig::InitCrossChatCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}
		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		for (int i = 0; i < 3; ++i)
		{
			plat_type_union.spid[i + 1] = plat_name[i];
		}

		if (m_platform_cross_chat_map.find(plat_type_union.id) != m_platform_cross_chat_map.end())
		{
			return -2;
		}

		CrossChatLimitCfg cfg;
		cfg.platform_type = plat_type_union.id;

		if (!GetSubNodeValue(dataElement, "is_forbid_cross_chat", cfg.is_forbid_cross_chat) || cfg.is_forbid_cross_chat < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "can_be_view_other_server", cfg.can_be_view_other_server) || cfg.can_be_view_other_server < 0)
		{
			return -4;
		}

		m_platform_cross_chat_map.emplace(std::make_pair(plat_type_union.id, cfg));

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int AgentAdapterConfig::InitSingleChatCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}
		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		for (int i = 0; i < 3; ++i)
		{
			plat_type_union.spid[i + 1] = plat_name[i];
		}

		if (m_platform_single_chat_map.find(plat_type_union.id) != m_platform_single_chat_map.end())
		{
			return -2;
		}

		SingleChatLimitCfg cfg;
		cfg.platform_type = plat_type_union.id;

		if (!GetSubNodeValue(dataElement, "can_offline_chat", cfg.can_offline_chat) || cfg.can_offline_chat < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "can_ack_chat", cfg.can_ack_chat) || cfg.can_ack_chat < 0)
		{
			return -4;
		}

		m_platform_single_chat_map.emplace(std::make_pair(plat_type_union.id, cfg));

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int AgentAdapterConfig::InitOpenFirstChargeFakeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		std::string plat_name;
		if (!GetSubNodeValue(dataElement, "plat_name", plat_name) || plat_name.empty())
		{
			return -1;
		}

		if (m_plat_open_first_charge_fake_count_map.find(plat_name) != m_plat_open_first_charge_fake_count_map.end())
		{
			return -2;
		}

		FirstChargeFakeCfg cfg;

		if (!GetSubNodeValue(dataElement, "is_open", cfg.is_open))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "num_limit_down", cfg.min_percent))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "num_limit_up", cfg.max_percent))
		{
			return -5;
		}

		m_plat_open_first_charge_fake_count_map[plat_name] = cfg;

		if (m_plat_open_first_charge_fake_count_map.find("default") == m_plat_open_first_charge_fake_count_map.end())
		{
			return -20;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

bool AgentAdapterConfig::GetOpenFirstChargeFakeCfgByPlatName(std::string plat_name)
{
	auto iter = m_plat_open_first_charge_fake_count_map.find(plat_name);
	if (iter != m_plat_open_first_charge_fake_count_map.end())
	{
		return iter->second.is_open;
	}
	else
	{
		iter = m_plat_open_first_charge_fake_count_map.find("default");
		if (iter != m_plat_open_first_charge_fake_count_map.end())
		{
			return iter->second.is_open;
		}
	}

	return false;
}

const FirstChargeFakeCfg * AgentAdapterConfig::GetOpenFirstChargeFakeCfg(std::string plat_name)
{
	auto iter = m_plat_open_first_charge_fake_count_map.find(plat_name);
	if (iter != m_plat_open_first_charge_fake_count_map.end())
	{
		return &(iter->second);
	}
	else
	{
		iter = m_plat_open_first_charge_fake_count_map.find("default");
		if (iter != m_plat_open_first_charge_fake_count_map.end())
		{
			return &(iter->second);
		}
	}

	return NULL;
}

const AutoClearGuildMemberCfg * AgentAdapterConfig::GetAutoClearGuildMemberCfg(const std::string &plat_name)
{
	if (m_plat_auto_clear_guild_member_map.find(plat_name) == m_plat_auto_clear_guild_member_map.end())
	{
		if (m_plat_auto_clear_guild_member_map.find("default") == m_plat_auto_clear_guild_member_map.end())
		{
			return nullptr;
		}
		return &m_plat_auto_clear_guild_member_map.at("default");
	}
	return &m_plat_auto_clear_guild_member_map.at(plat_name);
}

int AgentAdapterConfig::InitAutoClearGuildMemberCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		std::string plat_name;
		if (!GetSubNodeValue(dataElement, "plat_name", plat_name) || plat_name.empty())
		{
			return -1;
		}

		if (m_plat_auto_clear_guild_member_map.find(plat_name) != m_plat_auto_clear_guild_member_map.end())
		{
			return -2;
		}

		AutoClearGuildMemberCfg cfg;

		if (!GetSubNodeValue(dataElement, "is_auto_clear", cfg.is_auto_clear))
		{
			return -3;
		}

		m_plat_auto_clear_guild_member_map[plat_name] = cfg;

		if (m_plat_auto_clear_guild_member_map.find("default") == m_plat_auto_clear_guild_member_map.end())
		{
			return -20;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int AgentAdapterConfig::InitForbidCrossSpeaker(TiXmlElement *RootElement)
{
	m_is_forbid_cross_speaker = false;

	int plat_type = LocalConfig::Instance().GetPlatType();
	std::set<int> agent_adapter_set;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		PlatName plat_name; memset(plat_name, 0, sizeof(plat_name));
		std::string spid;
		if (!GetSubNodeValue(dataElement, "spid", spid) || spid.length() <= 0 || spid.length() >= sizeof(plat_name))
		{
			return -1;
		}

		PlatUnion plat_type_union;
		plat_type_union.id = 0;

		SNPRINTF(plat_name, sizeof(plat_name), "%s", spid.c_str());

		if (!spid.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if ('\0' != plat_name[i])
				{
					plat_type_union.spid[i + 1] = plat_name[i];	// 这样做是为了确保id大于0 字节序的关系
				}
			}
		}

		agent_adapter_set.insert(plat_type_union.id);

		dataElement = dataElement->NextSiblingElement();
	}

	if (agent_adapter_set.find(plat_type) != agent_adapter_set.end())
	{
		m_is_forbid_cross_speaker = true;
	}

	return 0;
}