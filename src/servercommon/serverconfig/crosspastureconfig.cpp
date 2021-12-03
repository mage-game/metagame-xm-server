#include "crosspastureconfig.hpp"

#include <servercommon/configcommon.h>

#include <set>
#include "servercommon/servercommon.h"

CrossPastureConfig::CrossPastureConfig()
	: m_activity_open_time_cfg_list_count(0), m_monster_create_pos_list_count(0), m_monster_info_cfg_count(0), m_fence_cfg_count(0), spical_monster_id(0)
{

}

CrossPastureConfig::~CrossPastureConfig()
{

}

CrossPastureConfig & CrossPastureConfig::Instance()
{
	static CrossPastureConfig _instance;
	return _instance;
}

bool CrossPastureConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossPastureConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 活动开启时间配置
		PugiXmlNode OpenTimeElement = RootElement.child("activity_open_time");
		if (OpenTimeElement.empty())
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OpenTimeElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物出生点配置
		PugiXmlNode ReliveElement = RootElement.child("create_pos");
		if (ReliveElement.empty())
		{
			*err = path + ": xml node error in [config->create_pos]";
			return false;
		}

		iRet = this->InitReliveCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitReliveCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物配置
		PugiXmlNode ReliveElement = RootElement.child("monster_info");
		if (ReliveElement.empty())
		{
			*err = path + ": xml node error in [config->monster_info]";
			return false;
		}

		iRet = this->InitMonsterInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 栅栏配置
		PugiXmlNode ReliveElement = RootElement.child("fence");
		if (ReliveElement.empty())
		{
			*err = path + ": xml node error in [config->fence]";
			return false;
		}

		iRet = this->InitFenceInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFenceInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能配置
		PugiXmlNode ReliveElement = RootElement.child("skill");
		if (ReliveElement.empty())
		{
			*err = path + ": xml node error in [config->skill]";
			return false;
		}

		iRet = this->InitSkillInfoCfg(ReliveElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSkillInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能配置
		PugiXmlNode rankElement = RootElement.child("point_rank_award");
		if (rankElement.empty())
		{
			*err = path + ": xml node error in [config->point_rank_award]";
			return false;
		}

		iRet = this->InitRankReward(rankElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRankReward failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossPastureConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);
	
	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "role_xpos", m_other_cfg.role_pos.x) || m_other_cfg.role_pos.x <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "role_ypos", m_other_cfg.role_pos.y) || m_other_cfg.role_pos.y <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "day_max_get_score_times", m_other_cfg.day_max_get_score_times) || m_other_cfg.day_max_get_score_times < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "monster_create_interval_s", m_other_cfg.monster_create_interval_s) || m_other_cfg.monster_create_interval_s < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "player_relive_time_s", m_other_cfg.player_relive_time_s) || m_other_cfg.player_relive_time_s < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "dec_speed_percent", m_other_cfg.dec_speed_percent) || m_other_cfg.dec_speed_percent <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "rank_title_id", m_other_cfg.rank_title_id) || m_other_cfg.rank_title_id <= 0)
	{
		return -14;
	}
	
	return 0;
}

int CrossPastureConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
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

		CrossPastureActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

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

int CrossPastureConfig::InitReliveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_monster_create_pos_list_count >= CROSS_PASTURE_RELIVE_POS_MAX_COUNT)
		{
			return -1;
		}

		Posi &player_pos = m_monster_create_pos_list[m_monster_create_pos_list_count];

		if (!PugiGetSubNodeValue(dataElement, "x_pos", player_pos.x) || player_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", player_pos.y) || player_pos.y <= 0)
		{
			return -4;
		}

		++ m_monster_create_pos_list_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_monster_create_pos_list_count <= 0)
	{
		return -5;
	}

	return 0;
}

int CrossPastureConfig::InitMonsterInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	std::set<int> monster_id_set;

	while (!dataElement.empty())
	{
		if (m_monster_info_cfg_count >= CROSS_PASTURE_MONSTER_CFG_MAX_COUNT)
		{
			return -1;
		}

		CrossPastureMonsterInfoConfig &mon_cfg = m_monster_info_cfg_list[m_monster_info_cfg_count];
		if (!PugiGetSubNodeValue(dataElement, "monster_id", mon_cfg.monster_id) || mon_cfg.monster_id <= 0)
		{
			return -2;
		}

		if (monster_id_set.find(mon_cfg.monster_id) != monster_id_set.end())
		{
			return -3;
		}

		monster_id_set.insert(mon_cfg.monster_id);

		if (!PugiGetSubNodeValue(dataElement, "score", mon_cfg.score) || mon_cfg.score <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_num", mon_cfg.max_num) || mon_cfg.max_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_posi_index_begin", mon_cfg.rand_pos_index_begin) || mon_cfg.rand_pos_index_begin < 0 ||
			mon_cfg.rand_pos_index_begin >= m_monster_create_pos_list_count)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_posi_index_end", mon_cfg.rand_pos_index_end) || 
			mon_cfg.rand_pos_index_end < mon_cfg.rand_pos_index_begin || mon_cfg.rand_pos_index_end >= m_monster_create_pos_list_count)
		{
			return -7;
		}

		PugiXmlNode reward_item = dataElement.child("reward_item");
		if (reward_item.empty())
		{
			return -8;
		}

		if (!mon_cfg.reward_item.ReadConfigNoCheck(reward_item))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", mon_cfg.weight) || mon_cfg.weight < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_count", mon_cfg.is_count) || mon_cfg.is_count < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_control_refresh", mon_cfg.is_control_refresh) || mon_cfg.is_control_refresh < 0)
		{
			return -12;
		}

		if (mon_cfg.is_control_refresh)
		{
			spical_monster_id = mon_cfg.monster_id;
		}
		
		++ m_monster_info_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossPastureConfig::InitFenceInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_fence_cfg_count >= CROSS_PASTURE_FENCE_CFG_MAX_COUNT)
		{
			return -1;
		}

		CrossPastureFenceConfig &fence_cfg = m_fence_cfg_list[m_fence_cfg_count];
		if (!PugiGetSubNodeValue(dataElement, "fence_id", fence_cfg.fence_id) || fence_cfg.fence_id != m_fence_cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "x1", fence_cfg.x1) || fence_cfg.x1 <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "x2", fence_cfg.x2) || fence_cfg.x2 <= fence_cfg.x1)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "y1", fence_cfg.y1) || fence_cfg.y1 <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "y2", fence_cfg.y2) || fence_cfg.y2 <= fence_cfg.y1)
		{
			return -7;
		}

		++ m_fence_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossPastureConfig::InitSkillInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int cfg_count = 0;
	while (!dataElement.empty())
	{
		if (cfg_count >= CROSS_PASTURE_SKILL_CFG_MAX_COUNT)
		{
			return -1;
		}

		CrossPastureSkillConfig &skill_cfg = m_skill_cfg_list[cfg_count];
		if (!PugiGetSubNodeValue(dataElement, "index", skill_cfg.index) || skill_cfg.index != cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_id", skill_cfg.skill_id) || skill_cfg.skill_id <= 0)
		{
			return -3;
		}

		++ cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (cfg_count != CROSS_PASTURE_SKILL_CFG_MAX_COUNT)
	{
		return -100;
	}

	return 0;
}

int CrossPastureConfig::InitRankReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	
	int last_rank = 0;
	while (!dataElement.empty())
	{
		CrossPastureRankReward rank_item;
		if (!PugiGetSubNodeValue(dataElement, "rank", rank_item.max_rank) || rank_item.max_rank <= 0)
		{
			return -1;
		}
		if (m_rank_reward.size() == 0)
		{
			rank_item.min_rank = 1;
			last_rank = rank_item.max_rank;
		}
		else
		{
			rank_item.min_rank = last_rank + 1;
			if (rank_item.min_rank > rank_item.max_rank)
			{
				return -100;
			}
			last_rank = rank_item.max_rank;
		}
		
		int ret = ItemConfigData::ReadConfigListNoCheck(dataElement, "reward_item", rank_item.reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (ret < 0)
		{
			return -200 + ret;
		}

		m_rank_reward.push_back(rank_item);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

bool CrossPastureConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

CrossPastureActivityOpenTimeInfo *CrossPastureConfig::GetCrossPastureActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

Posi CrossPastureConfig::GetRandomMonsterCreatePosByMonsterId(int monster_id)
{
	if (m_monster_create_pos_list_count <= 0)
	{
		return Posi(0, 0);
	}

	const CrossPastureMonsterInfoConfig *monster_cfg = this->GetMonsterInfoCfgWithMonsterID(monster_id);
	if (NULL != monster_cfg)
	{
		int rand_index = RandomNum(monster_cfg->rand_pos_index_begin, monster_cfg->rand_pos_index_end + 1);

		if (rand_index >= 0 && rand_index < m_monster_create_pos_list_count)
		{
			return m_monster_create_pos_list[rand_index];
		}
	}

	return Posi(0, 0);
}

int CrossPastureConfig::GetOpenActivityConfig(CrossPastureActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

const CrossPastureMonsterInfoConfig *CrossPastureConfig::GetMonsterInfoCfgWithIndex(int index)
{
	if (index < 0 || index >= m_monster_info_cfg_count)
	{
		return NULL;
	}

	return &m_monster_info_cfg_list[index];
}

const CrossPastureMonsterInfoConfig *CrossPastureConfig::GetMonsterInfoCfgWithMonsterID(int monster_id)
{
	for (int index = 0; index < m_monster_info_cfg_count; ++ index)
	{
		if (m_monster_info_cfg_list[index].monster_id == monster_id)
		{
			return &m_monster_info_cfg_list[index];
		}
	}

	return NULL;
}

const CrossPastureFenceConfig *CrossPastureConfig::GetFenceCfg(int index)
{
	if (index < 0 || index >= m_fence_cfg_count)
	{
		return NULL;
	}

	return &m_fence_cfg_list[index];
}

const CrossPastureMonsterInfoConfig *CrossPastureConfig::GetRandRewardWithMonsterID(int monster_id)
{
		int total_weight = 10000;
// 		if (m_monster_info_cfg_count > 0)
// 		{
// 			for (int index = 0; index < m_monster_info_cfg_count; ++index)
// 			{
// 				if (m_monster_info_cfg_list[index].weight > 0)
// 				{
// 					total_weight += m_monster_info_cfg_list[index].weight;
// 				}
// 			}
// 		}

		int rand_value = RandomNum(total_weight);

		for (int index = 0; index < m_monster_info_cfg_count; ++index)
		{
			if (m_monster_info_cfg_list[index].monster_id == monster_id )
			{
				if (rand_value < m_monster_info_cfg_list[index].weight)
				{
					return &m_monster_info_cfg_list[index];
				}
			}
		}

	return NULL;
}

const CrossPastureRankReward * CrossPastureConfig::GetRankRewardByRank(int rank) const
{
	if (rank <= 0)
	{
		return NULL;
	}
	for (unsigned int i = 0; i < m_rank_reward.size(); ++i)
	{
		if (rank >= m_rank_reward[i].min_rank && rank <= m_rank_reward[i].max_rank)
		{
			return &m_rank_reward[i];
		}
	}

	return NULL;
}