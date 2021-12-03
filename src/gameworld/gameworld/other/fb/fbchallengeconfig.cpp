#include "fbchallengeconfig.hpp"
#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"
#include "servercommon/servercommon.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include <set>

ChallengeFBConfig::ChallengeFBConfig():m_buy_cost_count(0)
{

}

ChallengeFBConfig::~ChallengeFBConfig()
{

}

bool ChallengeFBConfig::Init(const std::string &path, std::string * err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ChallengeFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
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
		PugiXmlNode root_element = RootElement.child("chaptercfg");
		if (root_element.empty())
		{
			*err = path + " xml not chaptercfg node ";
			return false;
		}

		iRet = this->InitLevelRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLevelRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("levelcfg");
		if (root_element.empty())
		{
			*err = path + " xml not levelcfg node ";
			return false;
		}

		iRet = this->InitLayerCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLayerCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("buy_cost");
		if (root_element.empty())
		{
			*err = path + " xml not buy_cost node ";
			return false;
		}

		iRet = this->InitBuyCostCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBuyCostCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const ChallengeFBLevelCfg::ConfigItem * ChallengeFBConfig::GetLevelCfg(int level)
{
	if (level < 0 || level >= ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[level];
}

const ChallengeFBLevelCfg::ConfigItem * ChallengeFBConfig::GetLevelCfgBySceneID(int scene_id, int *belong_layer)
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		const ChallengeFBLevelCfg::ConfigItem &lvl_cfg = m_level_cfg.cfg_list[i];

		for (int j = 0; j < ChallengeFBLevelCfg::LAYER_PER_LEVEL; ++ j)
		{
			if (lvl_cfg.layer_list[j].scene_id == scene_id)
			{
				if (NULL != belong_layer) *belong_layer = j;

				return &m_level_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}

const ChallengeFBLevelCfg::ConfigItem * ChallengeFBConfig::GetLevelCfgByStartSceneID(int scene_id)
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (scene_id == m_level_cfg.cfg_list[i].start_scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const ChallengeFBLevelCfg::ConfigItem * ChallengeFBConfig::GetLevelCfgByEndSceneID(int scene_id) 
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (scene_id == m_level_cfg.cfg_list[i].end_scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const ChallengeFBLevelCfg::Layer * ChallengeFBConfig::GetLayerCfgBySceneID(int scene_id)
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		const ChallengeFBLevelCfg::ConfigItem &lvl_cfg = m_level_cfg.cfg_list[i];

		for (int j = 0; j < ChallengeFBLevelCfg::LAYER_PER_LEVEL; ++ j)
		{
			if (lvl_cfg.layer_list[j].scene_id == scene_id)
			{
				return &m_level_cfg.cfg_list[i].layer_list[j];
			}
		}
	}

	return NULL;
}

const ChallengeFBLevelCfg::Layer * ChallengeFBConfig::GetLayerCfg(int level, int layer) 
{
	if (level < 0 || level >= ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return NULL;
	}

	if (layer < 0 || layer >= ChallengeFBLevelCfg::LAYER_PER_LEVEL)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[level].layer_list[layer];
}

ItemConfigData * ChallengeFBConfig::GetRewardItemList(int level, unsigned int pass_time, int &reward_flag)
{
	if (0 == reward_flag)
	{
		if (level >= 0 && level < m_level_cfg.cfg_count)
		{
			for (int i = 0; i < ChallengeFBLevelCfg::REWARD_DEGREE; i++)
			{
				if (pass_time >= m_level_cfg.layer_reward_list[level][i].min_time && pass_time <= m_level_cfg.layer_reward_list[level][i].max_time)
				{
					reward_flag = i + 1;
					return m_level_cfg.layer_reward_list[level][i].reward_item_list;
				}
			}
		}
	}
	else if (reward_flag >= 1 && reward_flag <= 3)
	{
		if (level >= 0 && level < m_level_cfg.cfg_count)
		{
			return m_level_cfg.layer_reward_list[level][reward_flag - 1].reward_item_list;
		}
	}

	return NULL;
}

ItemConfigData * ChallengeFBConfig::GetRewardItemList(int level, int reward_flag)
{
	if (reward_flag >= 1 && reward_flag <= 3)
	{
		if (level >= 0 && level < m_level_cfg.cfg_count)
		{
			return m_level_cfg.layer_reward_list[level][reward_flag - 1].reward_item_list;
		}
	}
	return NULL;
}

const ChallengeFBBuyCostCfg* ChallengeFBConfig::GetBuyCostCfg(int buy_times)
{
	if (buy_times < 0) return NULL;

	for (int i = 0; i < m_buy_cost_count && i < CHALLENGE_FB_BUY_COST_MAX_COUNT; i++)
	{
		ChallengeFBBuyCostCfg &cfg = m_buy_cost_cfg[i];
		if (buy_times >= cfg.buy_times)
		{
			return &cfg;
		}
	}

	return NULL;
}

int ChallengeFBConfig::InitLayerCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2000000;
	}

	std::set<int> sceneid_list;
	int last_layer_list[ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT];

	for (int i = 0; i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		last_layer_list[i] = -1;
	}

	while (!data_element.empty())
	{
		int level = 0; 
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
		{
			return -1;
		}

		int layer = 0;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || layer >= ChallengeFBLevelCfg::LAYER_PER_LEVEL || layer != last_layer_list[level] + 1)
		{
			return -2;
		}
		last_layer_list[level] = layer;

		ChallengeFBLevelCfg::Layer &layer_cfg = m_level_cfg.cfg_list[level].layer_list[layer];
		layer_cfg.level = level;
		layer_cfg.layer = layer;

		if (!PugiGetSubNodeValue(data_element, "scene_id", layer_cfg.scene_id) || layer_cfg.scene_id < 0)
		{
			return -3;
		}

		if (layer > 0)
		{
			if (layer_cfg.scene_id != m_level_cfg.cfg_list[level].layer_list[layer - 1].scene_id + 1)
			{
				return -4;
			}
		}

		if (sceneid_list.end() != sceneid_list.find(layer_cfg.scene_id))
		{
			return -5;
		}
		sceneid_list.insert(layer_cfg.scene_id);
		CheckResourceCenter::Instance().GetSceneCheck()->Add(layer_cfg.scene_id);

		if (!PugiGetSubNodeValue(data_element, "boss_id", layer_cfg.boss_id) || layer_cfg.boss_id < 0)
		{
			return -9;
		}
		if (layer_cfg.boss_id > 0 && !MONSTERPOOL->IsMonsterExist(layer_cfg.boss_id))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_x", layer_cfg.boss_pos.x) || layer_cfg.boss_pos.x < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_y", layer_cfg.boss_pos.y) || layer_cfg.boss_pos.y < 0)
		{
			return -11;
		}

		if((layer_cfg.boss_id <= 0 || layer_cfg.boss_pos.x <= 0 || layer_cfg.boss_pos.y <= 0))
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "zhanli", layer_cfg.zhanli) || layer_cfg.zhanli < 0)
		{
			return -13;
		}

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < m_level_cfg.cfg_count && i < ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (ChallengeFBLevelCfg::LAYER_PER_LEVEL != last_layer_list[i] + 1)
		{
			return -4000000;
		}

		ChallengeFBLevelCfg::ConfigItem &lvl_cfg = m_level_cfg.cfg_list[i];

		lvl_cfg.start_scene_id = lvl_cfg.layer_list[0].scene_id;
		lvl_cfg.end_scene_id = lvl_cfg.layer_list[ChallengeFBLevelCfg::LAYER_PER_LEVEL - 1].scene_id;
	}

	return 0;
}

int ChallengeFBConfig::InitLevelRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2000000;
	}

	m_level_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_level_cfg.cfg_count >= ChallengeFBLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFBLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[m_level_cfg.cfg_count];
		cfg_item.level = m_level_cfg.cfg_count;

		if (!PugiGetSubNodeValue(data_element, "pos_x", cfg_item.birth_pos.x) || cfg_item.birth_pos.x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", cfg_item.birth_pos.y) || cfg_item.birth_pos.y < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "role_level", cfg_item.role_level_limit) || cfg_item.role_level_limit < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "star_min_time_3", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][2].min_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][2].min_time < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "star_max_time_3", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][2].max_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][2].max_time < 0)
		{
			return -6;
		}

// 		int item_count = ItemConfigData::ReadConfigList(data_element, std::string("star_reward_item_3"), m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][2].reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
// 		if (item_count < 0)
// 		{
// 			return -100 + item_count;
// 		}

		if (!PugiGetSubNodeValue(data_element, "star_min_time_2", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][1].min_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][1].min_time < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "star_max_time_2", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][1].max_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][1].max_time < 0)
		{
			return -9;
		}

// 		item_count = ItemConfigData::ReadConfigList(data_element, std::string("star_reward_item_2"), 
// 			m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][1].reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
// 		if (item_count < 0)
// 		{
// 			return -1000 + item_count;
// 		}

		if (!PugiGetSubNodeValue(data_element, "star_min_time_1", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][0].min_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][0].min_time < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "star_max_time_1", m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][0].max_time) || m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][0].max_time < 0)
		{
			return -12;
		}

// 		item_count = ItemConfigData::ReadConfigList(data_element, std::string("star_reward_item_1"), 
// 			m_level_cfg.layer_reward_list[m_level_cfg.cfg_count][0].reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
// 		if (item_count < 0)
// 		{
// 			return -10000 + item_count;
// 		}

		int count = ItemConfigData::ReadConfigList(data_element, "find_reward", cfg_item.find_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -100 + count;
		}

		++ m_level_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChallengeFBConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2000000;
	}

	if (!PugiGetSubNodeValue(data_element, "day_free_times", m_other_cfg.day_free_times) || m_other_cfg.day_free_times < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "auto_need_star", m_other_cfg.auto_need_star) || m_other_cfg.auto_need_star < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_max_times", m_other_cfg.buy_max_times) || m_other_cfg.buy_max_times < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "max_layer", m_other_cfg.max_layer) || m_other_cfg.max_layer < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "add_role_gongji_per", m_other_cfg.add_role_gongji_per) || m_other_cfg.add_role_gongji_per < 0)
	{
		return -5;
	}

	return 0;
}

int ChallengeFBConfig::InitBuyCostCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2000000;
	}

	m_buy_cost_count = 0;
	int last_buy_times = 0;
	while (!data_element.empty())
	{
		if (m_buy_cost_count < 0 || m_buy_cost_count >= CHALLENGE_FB_BUY_COST_MAX_COUNT)
		{
			return -1;
		}

		ChallengeFBBuyCostCfg &cfg = m_buy_cost_cfg[m_buy_cost_count];

		if (!PugiGetSubNodeValue(data_element, "buy_times", cfg.buy_times) || cfg.buy_times < 0)
		{
			return -2;
		}

		if (0 != last_buy_times)
		{
			if (cfg.buy_times >= last_buy_times)
			{
				return -3;
			}
		}

		last_buy_times = cfg.buy_times;

		if (!PugiGetSubNodeValue(data_element, "gold_cost", cfg.gold_cost) || cfg.gold_cost < 0)
		{
			return -4;
		}

		++m_buy_cost_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}