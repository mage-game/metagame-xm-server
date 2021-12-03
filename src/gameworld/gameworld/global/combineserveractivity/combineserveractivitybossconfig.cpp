#include "combineserveractivitybossconfig.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include <algorithm>

CombineServerActivityBossConfig::CombineServerActivityBossConfig()
{

}

CombineServerActivityBossConfig::~CombineServerActivityBossConfig()
{

}

bool CombineServerActivityBossConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其它配置
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// boss配置
	PUGI_XML_LOAD_CONFIG("boss_cfg", InitBossCfg);
	// 采集物配置
	PUGI_XML_LOAD_CONFIG("gather_cfg", InitGatherCfg);
	// 采集物奖励配置
	PUGI_XML_LOAD_CONFIG("gather_reward_cfg", InitGatherRewardCfg);
	// 采集物坐标配置
	PUGI_XML_LOAD_CONFIG("gather_pos_cfg", InitGatherPosCfg);
	// 排行榜奖励配置
	PUGI_XML_LOAD_CONFIG("rank_reward", InitRankRewardCfg);
	// 普通boss定时刷新配置
	PUGI_XML_LOAD_CONFIG("common_boss_time_begin", InitCommonBossRefreshCfg);

	return true;
}

const CombineServerActivityGatherCfg * CombineServerActivityBossConfig::GetCSAGatherCfg(int gather_id)
{
	std::map<int, CombineServerActivityGatherCfg>::iterator iter = m_gather_cfg_map.find(gather_id);
	if (iter != m_gather_cfg_map.end())
	{
		return &(iter->second);
	}

	return NULL;
}

const CombineServerActivityGatherCfg * CombineServerActivityBossConfig::GetCSAGatherCfgByGatherType(int gather_type)
{
	for (std::map<int, CombineServerActivityGatherCfg>::iterator iter = m_gather_cfg_map.begin();
			iter != m_gather_cfg_map.end(); ++iter)
	{
		if (iter->second.gather_type == gather_type)
		{
			return &(iter->second);
		}
	}

	return NULL;
}

const ItemConfigData * CombineServerActivityBossConfig::GetGatherRandRewardItem(int gather_id)
{
	std::map<int, CombineServerActivityGatherRewardCfgVec>::iterator iter = m_gather_reward_cfg_map.find(gather_id);
	if (iter == m_gather_reward_cfg_map.end())
	{
		return NULL;
	}

	CombineServerActivityGatherRewardCfgVec &cfg_vec = iter->second;
	if (cfg_vec.total_weight <= 0)
	{
		return NULL;
	}

	int rand_weight = RandomNum(cfg_vec.total_weight);
	for (std::vector<CombineServerActivityGatherRewardCfg>::iterator iter = cfg_vec.reward_cfg_vec.begin();
			iter != cfg_vec.reward_cfg_vec.end(); iter++)
	{
		if (rand_weight < iter->weight)
		{
			return &iter->item;
		}
		else
		{
			rand_weight -= iter->weight;
		}
	}

	return NULL;
}

const std::vector<Posi> & CombineServerActivityBossConfig::GetCSARandomPosiCfg()
{
	std::random_shuffle(m_gather_pos_vec.begin(), m_gather_pos_vec.end());
	return m_gather_pos_vec;
}

const CombineServerActivityBossRankReward * CombineServerActivityBossConfig::GetCSABossRankRewardCfg(int day_index)
{
	if (day_index <= 0 || day_index > static_cast<int>(m_rank_reward_vec.size()))
	{
		return NULL;
	}

	return &m_rank_reward_vec[day_index - 1];
}

int CombineServerActivityBossConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	
	if (!PugiGetSubNodeValue(data_element, "enter_pos_x", m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_pos_y", m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "common_boss_begin_time", m_other_cfg.common_boss_begin_time) || m_other_cfg.common_boss_begin_time < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "common_boss_end_time", m_other_cfg.common_boss_end_time) || m_other_cfg.common_boss_end_time < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "common_boss_refresh_interval", m_other_cfg.common_boss_refresh_interval) || m_other_cfg.common_boss_refresh_interval < 0)
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "great_boss_begin_time", m_other_cfg.great_boss_begin_time) || m_other_cfg.great_boss_begin_time < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "great_boss_end_time", m_other_cfg.great_boss_end_time) || m_other_cfg.great_boss_end_time < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "common_boss_refresh_time", m_other_cfg.common_boss_refresh_time) || m_other_cfg.common_boss_refresh_time < 0)
	{
		return -8;
	}

	return 0;
}

int CombineServerActivityBossConfig::InitBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int boss_type = -1;
		if (!PugiGetSubNodeValue(data_element, "boss_type", boss_type) || (boss_type != BOSS_TYPE_COMMON && boss_type != BOSS_TYPE_GREAT))
		{
			return -1;
		}

		CombineServerActivityBossCfg boss_cfg;
		boss_cfg.boss_type = boss_type;
		if (!PugiGetSubNodeValue(data_element, "boss_id", boss_cfg.boss_id) || boss_cfg.boss_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_x", boss_cfg.pos.x) || boss_cfg.pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_y", boss_cfg.pos.y) || boss_cfg.pos.y <= 0)
		{
			return -4;
		}

		if (m_boss_cfg_map.find(boss_cfg.boss_id) != m_boss_cfg_map.end())
		{
			return -5;
		}

		m_boss_cfg_map[boss_cfg.boss_id] = boss_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityBossConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CombineServerActivityGatherCfg gather;
		if (!PugiGetSubNodeValue(data_element, "gather_id", gather.gather_id) || gather.gather_id <= 0)
		{
			return -1;
		}

		if (m_gather_cfg_map.find(gather.gather_id) != m_gather_cfg_map.end())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_type", gather.gather_type) || (gather.gather_type != GATHER_TYPE_GOLD && gather.gather_type != GATHER_TYPE_SLIVER))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_time_ms", gather.gather_time_ms) || gather.gather_time_ms <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "count", gather.count) || gather.count <= 0)
		{
			return -5;
		}
		
		PugiXmlNode item_data = data_element.child("reward");
		if (NULL == item_data)
		{
			return -6;
		}
		if (!gather.item.ReadConfig(item_data))
		{
			return -7;
		}

		m_gather_cfg_map[gather.gather_id] = gather;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityBossConfig::InitGatherPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		Posi gather_pos;
		if (!PugiGetSubNodeValue(data_element, "gather_pox_x", gather_pos.x) || gather_pos.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_pox_y", gather_pos.y) || gather_pos.y <= 0)
		{
			return -2;
		}

		m_gather_pos_vec.push_back(gather_pos);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityBossConfig::InitGatherRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int gather_id = 0;
		if (!PugiGetSubNodeValue(data_element, "gather_id", gather_id) || gather_id <= 0)
		{
			return -1;
		}

		CombineServerActivityGatherRewardCfgVec &cfg_vec = m_gather_reward_cfg_map[gather_id];
		std::vector<CombineServerActivityGatherRewardCfg> &reward_vec = cfg_vec.reward_cfg_vec;
		CombineServerActivityGatherRewardCfg reward_cfg;
		reward_cfg.gather_id = gather_id;

		if (!PugiGetSubNodeValue(data_element, "weight", reward_cfg.weight) || reward_cfg.weight <= 0)
		{
			return -2;
		}

		cfg_vec.total_weight += reward_cfg.weight;

		PugiXmlNode item_data = data_element.child("reward_item");
		if (NULL == item_data)
		{
			return -3;
		}
		if (!reward_cfg.item.ReadConfig(item_data))
		{
			return -4;
		}

		reward_vec.push_back(reward_cfg);
		data_element = data_element.next_sibling();
	}

	for (std::map<int, CombineServerActivityGatherRewardCfgVec>::iterator iter = m_gather_reward_cfg_map.begin(); 
			iter != m_gather_reward_cfg_map.end(); iter++)
	{
		if (iter->second.total_weight <= 0)
		{
			return -5;
		}
	}

	return 0;
}

int CombineServerActivityBossConfig::InitRankRewardCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_day_index = 0;
	CombineServerActivityBossRankReward cfg;
	while (!data_element.empty())
	{
		int day_index = 0;
		if (!PugiGetSubNodeValue(data_element, "day_index", day_index) || day_index <= 0)
		{
			return -1;
		}

		if (day_index != last_day_index)
		{
			last_day_index = day_index;
			m_rank_reward_vec.push_back(cfg);
		}

		if (m_rank_reward_vec.empty())
		{
			return -100;
		}

		CombineServerActivityBossRankReward &reward_cfg = m_rank_reward_vec[m_rank_reward_vec.size() - 1];
		reward_cfg.day_index = day_index;

		int rank_type = -1;
		if (!PugiGetSubNodeValue(data_element, "rank_type", rank_type) || (RANK_TYPE_PERSONAL != rank_type && RANK_TYPE_GUILD != rank_type))
		{
			return -2;
		}

		if (RANK_TYPE_PERSONAL == rank_type)
		{
			int rank = 0;
			if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank <= 0 || rank > COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM)
			{
				return -3;
			}

			PugiXmlNode item_data = data_element.child("reward_item");
			if (NULL == item_data)
			{
				return -4;
			}
			if (!reward_cfg.personal_rank_reward[rank - 1].ReadConfig(item_data))
			{
				return -5;
			}
		}
		else if (RANK_TYPE_GUILD == rank_type)
		{
			PugiXmlNode item_data = data_element.child("reward_item");
			if (NULL == item_data)
			{
				return -6;
			}
			if (!reward_cfg.member_reward.ReadConfig(item_data))
			{
				return -7;
			}
			
			item_data = data_element.child("master_reward");
			if (NULL == item_data)
			{
				return -8;
			}
			if (!reward_cfg.master_reward.ReadConfig(item_data))
			{
				return -9;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityBossConfig::InitCommonBossRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_refresh_time = 0;
	while (!data_element.empty())
	{
		int refresh_time = 0;
		if (!PugiGetSubNodeValue(data_element, "common_boss_time1", refresh_time) || refresh_time < last_refresh_time)
		{
			return -1;
		}

		m_common_boss_refresh_time_vec.push_back(refresh_time);

		last_refresh_time = refresh_time;

		data_element = data_element.next_sibling();
	}

	return 0;
}