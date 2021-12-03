#include "runetowerconfig.hpp"
#include "servercommon/servercommon.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "item/itempool.h"

RuneTowerConfig::RuneTowerConfig()
{
}

RuneTowerConfig::~RuneTowerConfig()
{
}

bool RuneTowerConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// ∆‰À˚≈‰÷√
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);

	// ≤„≈‰÷√
	PUGI_XML_LOAD_CONFIG("layer", InitLayerCfg);

	// Ω±¿¯ÀÊª˙«¯º‰≈‰÷√
	PUGI_XML_LOAD_CONFIG("rand_reward_weight_cfg", InitRandWeightCfg);

	//Ω±¿¯≥ÿ≈‰÷√
	PUGI_XML_LOAD_CONFIG("pass_reward_pool", InitRandRewardCfg);

	return true;
}

int RuneTowerConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "offline_time_max", m_other_cfg.offline_time_max) || m_other_cfg.offline_time_max <= 0)
	{
		return -1001;
	}

	if (!PugiGetSubNodeValue(data_element, "offline_time_free", m_other_cfg.offline_time_free) || m_other_cfg.offline_time_free <= 0)
	{
		return -1002;
	}

	if (!PugiGetSubNodeValue(data_element, "offline_time_free_count", m_other_cfg.offline_time_free_count) || m_other_cfg.offline_time_free_count < 0)
	{
		return -1003;
	}

	if (!PugiGetSubNodeValue(data_element, "add_hp_per", m_other_cfg.add_hp_per) || m_other_cfg.add_hp_per < 0)
	{
		return -1004;
	}

	if (!PugiGetSubNodeValue(data_element, "add_gongji_per", m_other_cfg.add_gongji_per) || m_other_cfg.add_gongji_per < 0)
	{
		return -1005;
	}

	if (!PugiGetSubNodeValue(data_element, "pass_reward_min", m_other_cfg.pass_reward_min) || m_other_cfg.pass_reward_min <= 0)
	{
		return -1006;
	}

	if (!PugiGetSubNodeValue(data_element, "pass_reward_max", m_other_cfg.pass_reward_max) || m_other_cfg.pass_reward_max <= 0 || m_other_cfg.pass_reward_max < m_other_cfg.pass_reward_min)
	{
		return -1007;
	}

	if (!PugiGetSubNodeValue(data_element, "add_role_gongji_per", m_other_cfg.add_role_gongji_per) || m_other_cfg.add_role_gongji_per < 0)
	{
		return -1008;
	}

	return 0;
}

int RuneTowerConfig::InitLayerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -2000;
	}

	while (!data_element.empty())
	{
		int fb_layer = 0;
		if (!PugiGetSubNodeValue(data_element, "fb_layer", fb_layer) || fb_layer <= 0 || fb_layer > RUNE_TOWER_FB_LAYER_MAX)
		{
			return -2001;
		}

		RuneTowerLayerCfg &cfg = m_layer_list[fb_layer];
		cfg.fb_layer = fb_layer;
		
		if (!PugiGetSubNodeValue(data_element, "level_limit", cfg.level_limit) || cfg.level_limit < 0)
		{
			return -2002;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2003;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(data_element, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -2004;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -2005;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id", cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -2006;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_num", cfg.monster_num) || cfg.monster_num <= 0)
		{
			return -2007;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_pos_x", cfg.monster_pos.x) || cfg.monster_pos.x <= 0)
		{
			return -2008;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_pos_y", cfg.monster_pos.y) || cfg.monster_pos.y <= 0)
		{
			return -2009;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_range", cfg.monster_range) || cfg.monster_range <= 0)
		{
			return -2010;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_rune_exp_rate", cfg.monster_rune_exp_rate) || cfg.monster_rune_exp_rate < 0)
		{
			return -2011;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_rune_exp_min", cfg.monster_rune_exp_min) || cfg.monster_rune_exp_min < 0)
		{
			return -2012;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_rune_exp_max", cfg.monster_rune_exp_max) || cfg.monster_rune_exp_max < cfg.monster_rune_exp_min)
		{
			return -2013;
		}

		if (!PugiGetSubNodeValue(data_element, "first_reward_rune_exp", cfg.first_reward_rune_exp) || cfg.first_reward_rune_exp < 0)
		{
			return -2014;
		}

		{
			int item_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("first_reward_item"), cfg.first_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -2100 + item_count;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "normal_reward_rune_exp", cfg.normal_reward_rune_exp) || cfg.normal_reward_rune_exp < 0)
		{
			return -2015;
		}

		{
			int item_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("normal_reward_item"), cfg.normal_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -2200 + item_count;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "capability", cfg.capability) || cfg.capability <= 0)
		{
			return -2016;
		}

		if (!PugiGetSubNodeValue(data_element, "kill_monster_speed", cfg.kill_monster_speed) || cfg.kill_monster_speed <= 0.0f)
		{
			return -2017;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneTowerConfig::InitRandRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -3000;
	}

	int last_index = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "index", last_index) || last_index < 0 || last_index >= RUNE_TOWER_RAND_REWARD_CFG_MAX)
		{
			return -3001;
		}

		RuneTowerRandRewardCfg &cfg = m_rand_reward_cfg[last_index];
		cfg.index = last_index;

		if (!PugiGetSubNodeValue(data_element, "rune_id", cfg.rune_id) || cfg.rune_id <= 0 || NULL == ITEMPOOL->GetItem(cfg.rune_id))
		{
			return -3002;
		}

		if (!PugiGetSubNodeValue(data_element, "weight0", cfg.weight[0]) || cfg.weight[0] < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight1", cfg.weight[1]) || cfg.weight[1] < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "weight2", cfg.weight[2]) || cfg.weight[2] < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "weight3", cfg.weight[3]) || cfg.weight[3] < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight4", cfg.weight[4]) || cfg.weight[4] < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "weight5", cfg.weight[5]) || cfg.weight[5] < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "weight6", cfg.weight[6]) || cfg.weight[6] < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "weight7", cfg.weight[7]) || cfg.weight[7] < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "weight8", cfg.weight[8]) || cfg.weight[8] < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "weight9", cfg.weight[9]) || cfg.weight[9] < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "weight10", cfg.weight[10]) || cfg.weight[10] < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "weight11", cfg.weight[11]) || cfg.weight[11] < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "weight12", cfg.weight[12]) || cfg.weight[12] < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "weight13", cfg.weight[13]) || cfg.weight[13] < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "weight14", cfg.weight[14]) || cfg.weight[14] < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "weight15", cfg.weight[15]) || cfg.weight[15] < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "weight16", cfg.weight[16]) || cfg.weight[16] < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "weight17", cfg.weight[17]) || cfg.weight[17] < 0)
		{
			return -18;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RuneTowerConfig::InitRandWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -4000;
	}

	int weight_index = 0;
	int last_weight_index = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "weight_index", weight_index) || weight_index < 0 || weight_index >= RUNE_TOWER_RAND_WEGHT_INDEX_MAX)
		{
			return -1;
		}

		if (weight_index != last_weight_index + 1)
		{
			return -2;
		}

		last_weight_index = weight_index;

		RuneTowerRandWeightCfg &cfg = m_rand_weight_cfg[weight_index];
		cfg.weight_index = weight_index;

		if (!PugiGetSubNodeValue(data_element, "begin_layer", cfg.begin_layer) || cfg.begin_layer <= 0 || cfg.begin_layer > RUNE_TOWER_FB_LAYER_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "end_layer", cfg.end_layer) || cfg.end_layer <= 0 || cfg.end_layer > RUNE_TOWER_FB_LAYER_MAX || cfg.end_layer < cfg.begin_layer)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RuneTowerLayerCfg * RuneTowerConfig::GetRuneTowerLayerCfg(int layer) const
{
	if (layer <= 0 || layer > RUNE_TOWER_FB_LAYER_MAX || 0 == m_layer_list[layer].fb_layer)
	{
		return NULL;
	}

	return &m_layer_list[layer];
}

void RuneTowerConfig::GetRandRewardList(int layer, int reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT], int &count)
{
	int rand_count = this->GetRandNum();
	int rune_id = 0;
	for (int i = 0; i < rand_count; ++i)
	{
		rune_id = this->GetRandRewardByLayer(layer);
		if (rune_id > 0 && count < RUNE_TOWER_RAND_REWARD_MAX_COUNT)
		{
			reward_list[count] = rune_id;
			++count;
		}
	}
}

const int RuneTowerConfig::GetRandNum() const
{
	return RandomNum(m_other_cfg.pass_reward_min, m_other_cfg.pass_reward_max);
}

const int RuneTowerConfig::GetRandWeightIndexByLayer(int layer)const
{
	for (int i = 0; i < RUNE_TOWER_RAND_WEGHT_INDEX_MAX; ++i)
	{
		if (layer >= m_rand_weight_cfg[i].begin_layer && layer <= m_rand_weight_cfg[i].end_layer)
		{
			return m_rand_weight_cfg[i].weight_index;
		}
	}

	return 0;
}

const int RuneTowerConfig::GetRandRewardByLayer(int layer)const
{
	int weight_index = this->GetRandWeightIndexByLayer(layer);
	if (weight_index < 0 || weight_index >= RUNE_TOWER_RAND_WEGHT_INDEX_MAX)
	{
		return 0;
	}

	int total_weight = 0;
	for (int i = 0; i < RUNE_TOWER_RAND_REWARD_CFG_MAX; ++i)
	{
		total_weight += m_rand_reward_cfg[i].weight[weight_index];
	}

	if (total_weight <= 0) return 0;

	int rand_num = RandomNum(0, total_weight);
	for (int i = 0; i < RUNE_TOWER_RAND_REWARD_CFG_MAX; ++i)
	{
		if (rand_num <= m_rand_reward_cfg[i].weight[weight_index])
		{
			return m_rand_reward_cfg[i].rune_id;
		}
		else
		{
			rand_num -= m_rand_reward_cfg[i].weight[weight_index];
		}
	}

	return 0;
}
