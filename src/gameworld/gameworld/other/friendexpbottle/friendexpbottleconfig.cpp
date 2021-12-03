#include "friendexpbottleconfig.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"

FriendExpBottleConfig::FriendExpBottleConfig()
{

}

FriendExpBottleConfig::~FriendExpBottleConfig()
{

}

bool FriendExpBottleConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FriendExpBottleConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load FriendExpBattleConfig config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		int ret = this->InitOtherConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitOtherConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("exp_bottle_limit");
		if (root_element.empty())
		{
			*err = configname + ": no [exp_bottle_limit].";
			return false;
		}

		int ret = this->InitExpBottleLimitConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitExpBottleLimitConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int FriendExpBottleConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "active_level", m_other_cfg.active_level) || m_other_cfg.active_level <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_minute_exp", m_other_cfg.per_minute_exp) || m_other_cfg.per_minute_exp <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "friend_extra_add_exp", m_other_cfg.friend_extra_add_exp) || m_other_cfg.friend_extra_add_exp <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "broadcast_cd", m_other_cfg.broadcast_cd) || m_other_cfg.broadcast_cd <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "get_exp_interval", m_other_cfg.get_exp_interval) || m_other_cfg.get_exp_interval <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "auto_add_friend_num_time", m_other_cfg.auto_add_friend_num_time) || m_other_cfg.auto_add_friend_num_time <= 0)
	{
		return -7;
	}

	return 0;
}

int FriendExpBottleConfig::InitExpBottleLimitConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	while (!dataElement.empty())
	{
		ExpBottleLimitCfg exp_bottle_limit_cfg;

		if (!PugiGetSubNodeValue(dataElement, "min_friend_max_count", exp_bottle_limit_cfg.min_friend_max_count) || exp_bottle_limit_cfg.min_friend_max_count < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "get_exp_times", exp_bottle_limit_cfg.get_exp_times) || exp_bottle_limit_cfg.get_exp_times < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fetch_level_limit", exp_bottle_limit_cfg.fetch_level_limit) || exp_bottle_limit_cfg.fetch_level_limit < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_limit", exp_bottle_limit_cfg.exp_limit) || exp_bottle_limit_cfg.exp_limit < 0)
		{
			return -5;
		}

		m_exp_bottle_limit_cfg_vec.push_back(exp_bottle_limit_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FriendExpBottleConfig::GetExpBottleLimit(int friend_num)
{
	//for (std::vector<ExpBottleLimitCfg>::iterator iter = m_exp_bottle_limit_cfg_vec.begin();
	//	iter != m_exp_bottle_limit_cfg_vec.end(); iter++)
	//{
	//	if (friend_num >= iter->min_friend_max_count && friend_num <= iter->max_friend_max_count)
	//	{
	//		return iter->get_exp_times;
	//	}
	//}

	return 0;
}

ExpBottleLimitCfg * FriendExpBottleConfig::GetExpBottleLimitCfg(int get_exp_count)
{
	int size = static_cast<int>(m_exp_bottle_limit_cfg_vec.size());
	if (get_exp_count >= 0 && get_exp_count < size)
	{
		return &m_exp_bottle_limit_cfg_vec[get_exp_count];
	}

	return NULL;
}