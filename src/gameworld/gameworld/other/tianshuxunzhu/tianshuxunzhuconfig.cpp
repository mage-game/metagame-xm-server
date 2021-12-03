#include "stdafx.h"

#include "tianshuxunzhuconfig.hpp"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"


TianshuXZConfig::TianshuXZConfig() : m_goal_count(0)
{
}

TianshuXZConfig::~TianshuXZConfig()
{
}

bool TianshuXZConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TianshuXZConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + " xml root node error";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + " xml other node ";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode item_element = RootElement.child("goal_cfg");
		if (item_element.empty())
		{
			*err = configname + "xml not goal_cfg node";
			return false;
		}

		iRet = this->InitGoalCfg(item_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGoalCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode skill_element = RootElement.child("pass_skill");
		if (skill_element.empty())
		{
			*err = configname + "xml not pass_skill node";
			return false;
		}

		iRet = this->InitSkillCfg(skill_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSkillCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;

}

int TianshuXZConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "shixue_open_day_end", m_other_cfg.shixue_open_day_end) || m_other_cfg.shixue_open_day_end < 0)
	{
		return -1;
	}

	return 0;
}

int TianshuXZConfig::InitGoalCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int last_seq = -1;
	int last_type = -1;
	while (!data_element.empty())
	{
		int type;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0 || type >= TIANSHU_TYPE_MAX)
		{
			return -1;
		}

		int seq;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= TIANSHU_XUNZHU_SIGNLE_MAX_NUM)
		{
			return -2;
		}

		if (type != last_type && seq != 0)
		{
			return -100;
		}

		if (type != last_type)
		{
			m_goal_count++;
			last_type = type;
		}

		TianShuXZGoalConfig & goal_config = m_goal_config[type];
		goal_config.type = type;

		if (seq != 0 && seq != last_seq + 1)
		{
			return -200;
		}
		last_seq = seq;
	
		TianShuXZGoalItem & goal_item = goal_config.golal_list[seq];
		goal_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "cond_type", goal_item.cond_type) || goal_item.cond_type <= TIANSHUXZ_COND_INVILAD ||
			goal_item.cond_type >= TIANSHUXZ_COND_MAX)
		{
			return -2;
		}

		int vec_ret = this->ReadList(data_element, "cond_param", goal_item.cond_param, "|");
		if (vec_ret < 0)
		{
			return -300 + vec_ret;
		}

		if (goal_item.cond_param.size() <= 0)
		{
			return -200;
		}

		int count = ItemConfigData::ReadConfigList(data_element, "reward_item", goal_item.goal_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -300 + count;
		}

		goal_config.count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int TianshuXZConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int type;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0 || type >= TIANSHU_TYPE_MAX)
		{
			return -1;
		}

		TianshuXZSkillCfg & skill_cfg = m_skill_config[type];

		if (!PugiGetSubNodeValue(data_element, "effect_per_val", skill_cfg.effect_per_val) || skill_cfg.effect_per_val < 0)
		{
			return -2;
		}
		skill_cfg.isvalid = true;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const TianShuXZGoalConfig * TianshuXZConfig::GetTianShuXZGoalConfig(int type) const
{
	if (type < 0 || type >= TIANSHU_TYPE_MAX || type > m_goal_count)
	{
		return NULL;
	}

	return &m_goal_config[type];
}

const TianShuXZGoalItem * TianshuXZConfig::GetTianShuShuXZGoalItem(int type, int seq) const
{
	if (type < 0 || type >= TIANSHU_TYPE_MAX || type > m_goal_count)
	{
		return NULL;
	}

	const TianShuXZGoalConfig & goal_cfg = m_goal_config[type];

	if (seq < 0 || seq >= TIANSHU_XUNZHU_SIGNLE_MAX_NUM || seq > goal_cfg.count)
	{
		return NULL;
	}
	
	return &goal_cfg.golal_list[seq];
}

int TianshuXZConfig::GetTianShuCountByType(int type)const
{
	if (type < 0 || type >= TIANSHU_TYPE_MAX || type > m_goal_count)
	{
		return 0;
	}

	return m_goal_config[type].count;
}

const TianshuXZSkillCfg* TianshuXZConfig::GetSKillCfg(int type)const
{
	if (type < 0 || type >= TIANSHU_TYPE_MAX)
	{
		return NULL;
	}
	
	if (m_skill_config[type].isvalid)
	{
		return &m_skill_config[type];
	}

	return NULL;
}
