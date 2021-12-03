#include "closedbetaconfig.hpp"

#include "servercommon/configcommon.h"
#include <set>
#include "servercommon/vipdef.hpp"
#include "servercommon/fbdef.hpp"

ClosedBetaConfig::ClosedBetaConfig() 
{

}

ClosedBetaConfig::~ClosedBetaConfig()
{

}

bool ClosedBetaConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ClosedBetaConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÔÓÏîÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// µÇÂ¼½±ÀøÅäÖÃ
		PugiXmlNode root_element = RootElement.child("login_reward");
		if (root_element.empty())
		{
			*err = path + " xml not login_reward node ";
			return false;
		}

		iRet = this->InitLoginRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLoginRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// Éý¼¶½±ÀøÅäÖÃ
		PugiXmlNode root_element = RootElement.child("uplevel_reward");
		if (root_element.empty())
		{
			*err = path + " xml not uplevel_reward node ";
			return false;
		}

		iRet = this->InitUplevelRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitUplevelRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// VIPÅäÖÃ
		PugiXmlNode root_element = RootElement.child("vip_level_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not vip_level_cfg node ";
			return false;
		}

		iRet = this->InitViplevelCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitViplevelCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ²ÎÓë»î¶¯½±ÀøÅäÖÃ
		PugiXmlNode root_element = RootElement.child("join_activity_reward");
		if (root_element.empty())
		{
			*err = path + " xml not join_activity_reward node ";
			return false;
		}

		iRet = this->InitJoinActivityRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitJoinActivityRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ÔÚÏßÊ±³¤½±Àø
		PugiXmlNode root_element = RootElement.child("online_time_s_reward");
		if (root_element.empty())
		{
			*err = path + " xml not online_time_s_reward node ";
			return false;
		}

		iRet = this->InitOnlineTimeRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOnLineTimeRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ×°±¸±¾ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("equipfb_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not equipfb_cfg node ";
			return false;
		}

		iRet = this->InitEquipFBCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitEquipFBCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}
	return true;
}

const CloseBetaLoginRewardCfg::ConfigItem * ClosedBetaConfig::GetLoginRewardCfg(int login_day)
{
	if (login_day <= 0)
	{
		return NULL;
	}

	if (login_day >= m_login_reward_cfg.max_login_day)
	{
		return &m_login_reward_cfg.cfg_list[m_login_reward_cfg.max_login_day];
	}
	else
	{
		return &m_login_reward_cfg.cfg_list[login_day];
	}
}

const CloseBetaUplevelRewardCfg::ConfigItem * ClosedBetaConfig::GetUplevelRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_uplevel_reward_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_uplevel_reward_cfg.cfg_list[seq];
}

const CloseBetaVipLevelCfg::ConfigItem * ClosedBetaConfig::GetVipLevelCfg(int role_level)
{
	for (int i = m_vip_level_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (role_level >= m_vip_level_cfg.cfg_list[i].role_level)
		{
			return &m_vip_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const CloseBetaJoinActivityRewardCfg::ConfigItem * ClosedBetaConfig::GetJoinActivityCfgBySeq(int seq)
{
	if (seq < 0 || seq >= m_join_activity_reward_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_join_activity_reward_cfg.cfg_list[seq];
}

const CloseBetaJoinActivityRewardCfg::ConfigItem * ClosedBetaConfig::GetJoinActivityCfgByType(int activity_type)
{
	for (int i = 0; i < m_join_activity_reward_cfg.cfg_count; ++ i)
	{
		if (activity_type == m_join_activity_reward_cfg.cfg_list[i].activity_type)
		{
			return &m_join_activity_reward_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const CloseBetaOnlineTimeRewardCfg::ConfigItem * ClosedBetaConfig::GetOnlineTimeCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_online_time_reward_cfg.cfg_count && seq < CloseBetaOnlineTimeRewardCfg::ONLINE_REWARD_MAX_COUNT)
	{
		return &m_online_time_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const CloseBetaEquipFBCfg::ConfigItem * ClosedBetaConfig::GetEquipFBCfg(int seq)
{
	if (seq < 0 || seq >= m_equipfb_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_equipfb_cfg.cfg_list[seq];
}

const CloseBetaEquipFBCfg::ConfigItem * ClosedBetaConfig::GetEquipFBCfg(int chapter, int level)
{
	for (int i = 0; i < m_equipfb_cfg.cfg_count; ++i)
	{
		if (chapter == m_equipfb_cfg.cfg_list[i].need_chapter &&
			level == m_equipfb_cfg.cfg_list[i].need_level)
		{
			return &m_equipfb_cfg.cfg_list[i];
		}
	}

	return NULL;
}

int ClosedBetaConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	PugiXmlNode guildItemElement = dataElement.child("guild_item");
	if (guildItemElement.empty())
	{
		return -1;
	}

	if (!m_other_cfg.guild_reward_item.ReadConfig(guildItemElement))
	{
		return -2;
	}

	PugiXmlNode marryItemElement = dataElement.child("marry_item");
	if (marryItemElement.empty())
	{
		return -3;
	}

	if (!m_other_cfg.marry_reward_item.ReadConfig(marryItemElement))
	{
		return -4;
	}

	return 0;
}

int ClosedBetaConfig::InitLoginRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_login_reward_cfg.max_login_day = 0;

	while (!dataElement.empty())
	{
		int login_day = 0;
		if (!PugiGetSubNodeValue(dataElement, "logn_day", login_day) || login_day >= CloseBetaLoginRewardCfg::CONFIG_ITEM_MAX_COUNT || 
			login_day != m_login_reward_cfg.max_login_day + 1)
		{
			return -1;
		}
		m_login_reward_cfg.max_login_day = login_day;

		CloseBetaLoginRewardCfg::ConfigItem &cfg_item = m_login_reward_cfg.cfg_list[login_day];
		cfg_item.login_day = login_day;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ClosedBetaConfig::InitUplevelRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;

	while (!dataElement.empty())
	{
		if (m_uplevel_reward_cfg.cfg_count >= CloseBetaUplevelRewardCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		CloseBetaUplevelRewardCfg::ConfigItem &cfg_item = m_uplevel_reward_cfg.cfg_list[m_uplevel_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg_item.need_level) || cfg_item.need_level <= last_level)
		{
			return -2;
		}
		last_level = cfg_item.need_level;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold <= 0)
		{
			return -3;
		}

		++ m_uplevel_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ClosedBetaConfig::InitViplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_role_level = 0;
	int last_vip_level = 0;

	while (!dataElement.empty())
	{
		if (m_vip_level_cfg.cfg_count >= CloseBetaVipLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		CloseBetaVipLevelCfg::ConfigItem &cfg_item = m_vip_level_cfg.cfg_list[m_vip_level_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "role_level", cfg_item.role_level) || cfg_item.role_level <= last_role_level)
		{
			return -2;
		}
		last_role_level = cfg_item.role_level;

		if (!PugiGetSubNodeValue(dataElement, "vip_level", cfg_item.vip_level) || cfg_item.vip_level > MAX_VIP_LEVEL || cfg_item.vip_level != last_vip_level + 1)
		{
			return -3;
		}
		last_vip_level = cfg_item.vip_level;

		++ m_vip_level_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ClosedBetaConfig::InitEquipFBCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level_list[NEQFB_MAX_CHAPTER];
	for (int i = 0; i < NEQFB_MAX_CHAPTER; ++i)
	{
		last_level_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		if (m_equipfb_cfg.cfg_count >= CloseBetaEquipFBCfg::CONFIG_ITEM_MAX_COUNT || m_equipfb_cfg.cfg_count < 0)
		{
			return -1;
		}

		CloseBetaEquipFBCfg::ConfigItem &cfg_item = m_equipfb_cfg.cfg_list[m_equipfb_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg_item.seq) || cfg_item.seq != m_equipfb_cfg.cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_chapter", cfg_item.need_chapter) || cfg_item.need_chapter < 0 || cfg_item.need_chapter >= NEQFB_MAX_CHAPTER)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg_item.need_level) || cfg_item.need_level < 0 || cfg_item.need_level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
		{
			return -4;
		}

		if (cfg_item.need_level <= last_level_list[cfg_item.need_chapter])
		{
			return -5;
		}
		last_level_list[cfg_item.need_chapter] = cfg_item.need_level;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", cfg_item.reward_gold_bind) || cfg_item.reward_gold_bind <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_star", cfg_item.need_star) || cfg_item.need_star <= 0)
		{
			return -7;
		}

		++m_equipfb_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ClosedBetaConfig::InitJoinActivityRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	std::set<int> repeat_checker;

	while (!dataElement.empty())
	{
		if (m_join_activity_reward_cfg.cfg_count >= CloseBetaJoinActivityRewardCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		CloseBetaJoinActivityRewardCfg::ConfigItem &cfg_item = m_join_activity_reward_cfg.cfg_list[m_join_activity_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg_item.seq) || cfg_item.seq != m_join_activity_reward_cfg.cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_type", cfg_item.activity_type) || cfg_item.activity_type <= ACTIVITY_TYPE_INVALID || cfg_item.activity_type >= ACTIVITY_TYPE_MAX)
		{
			return -3;
		}

		if (repeat_checker.end() != repeat_checker.find(cfg_item.activity_type))
		{
			return -4;
		}
		repeat_checker.insert(cfg_item.activity_type);

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold <= 0)
		{
			return -5;
		}

		++ m_join_activity_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ClosedBetaConfig::InitOnlineTimeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_online_time_reward_cfg.cfg_count = 0;

	int last_reward_online_time_s = 0;

	while (!dataElement.empty())
	{
		if (m_online_time_reward_cfg.cfg_count >= CloseBetaOnlineTimeRewardCfg::ONLINE_REWARD_MAX_COUNT)
		{
			return -1;
		}

		CloseBetaOnlineTimeRewardCfg::ConfigItem &cfg_item = m_online_time_reward_cfg.cfg_list[m_online_time_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg_item.seq) || cfg_item.seq != m_online_time_reward_cfg.cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_online_time_s", cfg_item.need_online_time_s) || cfg_item.need_online_time_s <= 0)
		{
			return -3;
		}
		
		if (cfg_item.need_online_time_s <= last_reward_online_time_s)
		{
			return -4;
		}
		last_reward_online_time_s = cfg_item.need_online_time_s;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold <= 0)
		{
			return -5;
		}

		++ m_online_time_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

