#include "giftharvestconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include <set>

GiftHarvestConfig::GiftHarvestConfig()
	: m_activity_open_time_cfg_list_count(0), m_monster_create_pos_list_count(0), m_monster_info_cfg_count(0), m_fence_cfg_count(0)
{

}

GiftHarvestConfig::~GiftHarvestConfig()
{

}

bool GiftHarvestConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };

	TiXmlDocument document;
	if (path == "" || !document.LoadFile(path))
	{
		*err = path + "：Load GiftHarvestConfig Error,\n" + document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		TiXmlElement *OtherElement = RootElement->FirstChildElement("other");
		if (NULL == OtherElement)
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
		// 活动开启时间配置
		TiXmlElement *OpenTimeElement = RootElement->FirstChildElement("round_open_time");
		if (NULL == OpenTimeElement)
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OpenTimeElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物出生点配置
		TiXmlElement *ReliveElement = RootElement->FirstChildElement("create_pos");
		if (NULL == ReliveElement)
		{
			*err = path + ": xml node error in [config->create_pos]";
			return false;
		}

		iRet = this->InitReliveCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitReliveCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物配置
		TiXmlElement *ReliveElement = RootElement->FirstChildElement("monster_info");
		if (NULL == ReliveElement)
		{
			*err = path + ": xml node error in [config->monster_info]";
			return false;
		}

		iRet = this->InitMonsterInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMonsterInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 栅栏配置
		TiXmlElement *ReliveElement = RootElement->FirstChildElement("fence");
		if (NULL == ReliveElement)
		{
			*err = path + ": xml node error in [config->fence]";
			return false;
		}

		iRet = this->InitFenceInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFenceInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能配置
		TiXmlElement *ReliveElement = RootElement->FirstChildElement("skill");
		if (NULL == ReliveElement)
		{
			*err = path + ": xml node error in [config->skill]";
			return false;
		}

		iRet = this->InitSkillInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSkillInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GiftHarvestConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	int is_open = 0;
	if (!GetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);

	if (!GetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(dataElement, "role_xpos", m_other_cfg.role_pos.x) || m_other_cfg.role_pos.x <= 0)
	{
		return -3;
	}

	if (!GetSubNodeValue(dataElement, "role_ypos", m_other_cfg.role_pos.y) || m_other_cfg.role_pos.y <= 0)
	{
		return -4;
	}

	if (!GetSubNodeValue(dataElement, "day_max_get_score_times", m_other_cfg.day_max_get_score_times) || m_other_cfg.day_max_get_score_times < 0)
	{
		return -10;
	}

	if (!GetSubNodeValue(dataElement, "monster_create_interval_s", m_other_cfg.monster_create_interval_s) || m_other_cfg.monster_create_interval_s < 0)
	{
		return -11;
	}

	//TiXmlElement *item_element = dataElement->FirstChildElement("reward_item");
	//if (nullptr == item_element)
	//{
	//	return -100;
	//}
	//if (!m_other_cfg.reward_item.ReadConfig(item_element))
	//{
	//	return -101;
	//}

	{// 礼物收割 轮次活动参与奖
		TiXmlElement *reward_item_list = dataElement->FirstChildElement("reward_item_list");
		if (nullptr == reward_item_list)
		{
			return -153;
		}

		TiXmlElement *reward_item = reward_item_list->FirstChildElement("reward_item");
		if (nullptr == reward_item)
		{
			return -154;
		}

		int i = 0;

		while (nullptr != reward_item)
		{
			if (!m_other_cfg.reward_item_list[i].ReadConfig(reward_item) || i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -155;
			}
			++i;
			reward_item = reward_item->NextSiblingElement();
		}
	}

	return 0;
}

int GiftHarvestConfig::InitActivityOpenTimeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	//int pre_activity_week_day = 0;
	//int pre_activity_end_time = 0;
	int round_check = 0;

	while (NULL != dataElement)
	{
		if (m_activity_open_time_cfg_list_count >= GIFT_HARVEST_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		GiftHarvestRoundOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!GetSubNodeValue(dataElement, "round", open_cfg.round) || open_cfg.round < 0)
		{
			return -2;
		}

		if (open_cfg.round - round_check != 1)
		{
			return -200;// 轮次必须按1递增
		}
		round_check = open_cfg.round;

		if (!GetSubNodeValue(dataElement, "round_ready_time", open_cfg.round_ready_time) || open_cfg.round_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.round_ready_time))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "round_start_time", open_cfg.round_start_time) || open_cfg.round_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.round_start_time))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "round_end_time", open_cfg.round_end_time) || open_cfg.round_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.round_end_time))
		{
			return -5;
		}

		int ready_time_sec = int(open_cfg.round_ready_time / 100) * 3600 + (open_cfg.round_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.round_start_time / 100) * 3600 + (open_cfg.round_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.round_end_time / 100) * 3600 + (open_cfg.round_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int GiftHarvestConfig::InitReliveCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		if (m_monster_create_pos_list_count >= GIFT_HARVEST_RELIVE_POS_MAX_COUNT)
		{
			return -1;
		}

		Posi &player_pos = m_monster_create_pos_list[m_monster_create_pos_list_count];

		if (!GetSubNodeValue(dataElement, "x_pos", player_pos.x) || player_pos.x <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "y_pos", player_pos.y) || player_pos.y <= 0)
		{
			return -4;
		}

		++m_monster_create_pos_list_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int GiftHarvestConfig::InitMonsterInfoCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	std::set<int> monster_id_set;

	while (NULL != dataElement)
	{
		if (m_monster_info_cfg_count >= GIFT_HARVEST_MONSTER_CFG_MAX_COUNT)
		{
			return -1;
		}

		GiftHarvestMonsterInfoConfig &mon_cfg = m_monster_info_cfg_list[m_monster_info_cfg_count];
		if (!GetSubNodeValue(dataElement, "monster_id", mon_cfg.monster_id) || mon_cfg.monster_id <= 0)
		{
			return -2;
		}

		if (monster_id_set.find(mon_cfg.monster_id) != monster_id_set.end())
		{
			return -3;
		}

		monster_id_set.insert(mon_cfg.monster_id);

		if (!GetSubNodeValue(dataElement, "score", mon_cfg.score) || mon_cfg.score <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "max_num", mon_cfg.max_num) || mon_cfg.max_num <= 0)
		{
			return -5;
		}

		++m_monster_info_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int GiftHarvestConfig::InitFenceInfoCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		if (m_fence_cfg_count >= GIFT_HARVEST_FENCE_CFG_MAX_COUNT)
		{
			return -1;
		}

		GiftHarvestFenceConfig &fence_cfg = m_fence_cfg_list[m_fence_cfg_count];
		if (!GetSubNodeValue(dataElement, "fence_id", fence_cfg.fence_id) || fence_cfg.fence_id != m_fence_cfg_count)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "x1", fence_cfg.x1) || fence_cfg.x1 <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "x2", fence_cfg.x2) || fence_cfg.x2 <= fence_cfg.x1)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "y1", fence_cfg.y1) || fence_cfg.y1 <= 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "y2", fence_cfg.y2) || fence_cfg.y2 <= fence_cfg.y1)
		{
			return -7;
		}

		++m_fence_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int GiftHarvestConfig::InitSkillInfoCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	int cfg_count = 0;
	while (NULL != dataElement)
	{
		if (cfg_count >= GIFT_HARVEST_SKILL_CFG_MAX_COUNT)
		{
			return -1;
		}

		GiftHarvestSkillConfig &skill_cfg = m_skill_cfg_list[cfg_count];
		if (!GetSubNodeValue(dataElement, "index", skill_cfg.index) || skill_cfg.index != cfg_count)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "skill_id", skill_cfg.skill_id) || skill_cfg.skill_id <= 0)
		{
			return -3;
		}

		++cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	if (cfg_count != GIFT_HARVEST_SKILL_CFG_MAX_COUNT)
	{
		return -100;
	}

	return 0;
}

bool GiftHarvestConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

const GiftHarvestRoundOpenTimeInfo *GiftHarvestConfig::GetGiftHarvestActivityOpenTimeInfo(int round)
{
	if (round > 0 && round <= m_activity_open_time_cfg_list_count && round <= GIFT_HARVEST_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[round - 1];
	}

	return nullptr;
}

Posi GiftHarvestConfig::GetRandomMonsterCreatePos()
{
	if (m_monster_create_pos_list_count == 0)
	{
		return Posi(0,0);
	}
	int rand_index = rand() % m_monster_create_pos_list_count;
	return m_monster_create_pos_list[rand_index];
}


const GiftHarvestMonsterInfoConfig *GiftHarvestConfig::GetMonsterInfoCfgWithIndex(int index)
{
	if (index < 0 || index >= m_monster_info_cfg_count)
	{
		return NULL;
	}

	return &m_monster_info_cfg_list[index];
}

const GiftHarvestMonsterInfoConfig *GiftHarvestConfig::GetMonsterInfoCfgWithMonsterID(int monster_id)
{
	for (int index = 0; index < m_monster_info_cfg_count; ++index)
	{
		if (m_monster_info_cfg_list[index].monster_id == monster_id)
		{
			return &m_monster_info_cfg_list[index];
		}
	}

	return NULL;
}

const GiftHarvestFenceConfig *GiftHarvestConfig::GetFenceCfg(int index)
{
	if (index < 0 || index >= m_fence_cfg_count)
	{
		return NULL;
	}

	return &m_fence_cfg_list[index];
}