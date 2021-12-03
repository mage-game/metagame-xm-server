#include "fightingconfig.hpp"

#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"

FightingConfig::FightingConfig(): m_mining_max_quality(-1), m_mining_robot_total_weight(0), m_sailing_max_quality(-1), m_sailing_robot_total_weight(0), m_challenge_rank_reward_cfg_count(0)
{
}

FightingConfig::~FightingConfig()
{
}

bool FightingConfig::Init(std::string configname, std::string* err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// ∆‰À˚≈‰÷√
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	// Õ⁄øÛΩ±¿¯
	PUGI_XML_LOAD_CONFIG("mining_reward", InitMiningRewardCfg);
	// ∫Ω∫£Ω±¿¯
	PUGI_XML_LOAD_CONFIG("sailing_reward", InitSailingRewardCfg);
	// ÃÙ–∆≈≈√˚Ω±¿¯
	PUGI_XML_LOAD_CONFIG("challenge_rank_reward", InitChallengeRankRewardCfg);
	// …®µ¥≈‰÷√
	PUGI_XML_LOAD_CONFIG("skip_cfg", InitFightingSkipCfg);

	return true;
}

const FightingMiningRewardCfg * FightingConfig::GetMiningCfg(int quality) const
{
	if (quality < 0 || quality > m_mining_max_quality || quality > FightingMiningRewardCfg::MAX_QUALITY_TYPE)
	{
		return NULL;
	}

	return &m_mining_reward_cfg_list[quality];
}

const FightingSailingRewardCfg * FightingConfig::GetSailingCfg(int quality) const
{
	if (quality < 0 || quality > m_sailing_max_quality || quality > FightingSailingRewardCfg::MAX_QUALITY_TYPE)
	{
		return NULL;
	}

	return &m_sailing_reward_cfg_list[quality];
}

const FightingRankRewardCfg * FightingConfig::GetChallengeRankReward(int rank) const
{
	for (int i = 0; i < m_challenge_rank_reward_cfg_count && i < FightingRankRewardCfg::CFG_COUNT; ++i)
	{
		const FightingRankRewardCfg &cfg_item = m_challenge_rank_reward_cfg_list[i];
		if (rank <= cfg_item.rank)
		{
			return &cfg_item;
		}
	}

	return NULL;
}

int FightingConfig::MiningGetRandomRobotQuality() const
{
	int random_num = RandomNum(m_mining_robot_total_weight);
	for (int quality = 0; quality <= m_mining_max_quality && quality <= FightingMiningRewardCfg::MAX_QUALITY_TYPE; ++quality)
	{
		const FightingMiningRewardCfg &cfg = m_mining_reward_cfg_list[quality];
		if (random_num < cfg.robot_reflush_weight)
		{
			return quality;
		}

		random_num -= cfg.robot_reflush_weight;
	}

	return 0;
}

int FightingConfig::SailingGetRandomRobotQuality() const
{
	int random_num = RandomNum(m_sailing_robot_total_weight);
	for (int quality = 0; quality <= m_sailing_max_quality && quality <= FightingMiningRewardCfg::MAX_QUALITY_TYPE; ++quality)
	{
		const FightingSailingRewardCfg &cfg = m_sailing_reward_cfg_list[quality];
		if (random_num < cfg.robot_reflush_weight)
		{
			return quality;
		}

		random_num -= cfg.robot_reflush_weight;
	}

	return 0;
}

int FightingConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_scene_id", m_other_cfg.dm_scene_id) || m_other_cfg.dm_scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.dm_scene_id);

	if (!PugiGetSubNodeValue(data_element, "dm_sponsor_pos_x", m_other_cfg.dm_sponsor_pos.x) || m_other_cfg.dm_sponsor_pos.x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_sponsor_pos_y", m_other_cfg.dm_sponsor_pos.y) || m_other_cfg.dm_sponsor_pos.y <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_opponent_pos_x", m_other_cfg.dm_opponent_pos.x) || m_other_cfg.dm_opponent_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_opponent_pos_y", m_other_cfg.dm_opponent_pos.y) || m_other_cfg.dm_opponent_pos.y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_day_times", m_other_cfg.dm_day_times) || m_other_cfg.dm_day_times < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_buy_time_need_gold", m_other_cfg.dm_buy_time_need_gold) || m_other_cfg.dm_buy_time_need_gold <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_cost_time_m", m_other_cfg.dm_cost_time_m) || m_other_cfg.dm_cost_time_m <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_rob_times", m_other_cfg.dm_rob_times) || m_other_cfg.dm_rob_times <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_been_rob_times", m_other_cfg.dm_been_rob_times) || m_other_cfg.dm_been_rob_times < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "dm_rob_reward_rate", m_other_cfg.dm_rob_reward_rate) || m_other_cfg.dm_rob_reward_rate <= 0)
	{
		return -13;
	}


	if (!PugiGetSubNodeValue(data_element, "sl_scene_id", m_other_cfg.sl_scene_id) || m_other_cfg.sl_scene_id <= 0)
	{
		return -101;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.sl_scene_id);

	if (!PugiGetSubNodeValue(data_element, "sl_sponsor_pos_x", m_other_cfg.sl_sponsor_pos.x) || m_other_cfg.sl_sponsor_pos.x <= 0)
	{
		return -102;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_sponsor_pos_y", m_other_cfg.sl_sponsor_pos.y) || m_other_cfg.sl_sponsor_pos.y <= 0)
	{
		return -103;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_opponent_pos_x", m_other_cfg.sl_opponent_pos.x) || m_other_cfg.sl_opponent_pos.x <= 0)
	{
		return -104;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_opponent_pos_y", m_other_cfg.sl_opponent_pos.y) || m_other_cfg.sl_opponent_pos.y <= 0)
	{
		return -105;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_day_times", m_other_cfg.sl_day_times) || m_other_cfg.sl_day_times < 0)
	{
		return -106;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_buy_time_need_gold", m_other_cfg.sl_buy_time_need_gold) || m_other_cfg.sl_buy_time_need_gold <= 0)
	{
		return -107;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_cost_time_m", m_other_cfg.sl_cost_time_m) || m_other_cfg.sl_cost_time_m <= 0)
	{
		return -110;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_rob_times", m_other_cfg.sl_rob_times) || m_other_cfg.sl_rob_times <= 0)
	{
		return -111;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_been_rob_times", m_other_cfg.sl_been_rob_times) || m_other_cfg.sl_been_rob_times < 0)
	{
		return -112;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_rob_reward_rate", m_other_cfg.sl_rob_reward_rate) || m_other_cfg.sl_rob_reward_rate <= 0)
	{
		return -113;
	}

	if (!PugiGetSubNodeValue(data_element, "sl_jinghua_reward_open_level", m_other_cfg.sl_jinghua_reward_open_level) || m_other_cfg.sl_jinghua_reward_open_level < 0)
	{
		return -114;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_scene_id", m_other_cfg.cf_scene_id) || m_other_cfg.cf_scene_id <= 0)
	{
		return -201;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.cf_scene_id);

	if (!PugiGetSubNodeValue(data_element, "cf_sponsor_pos_x", m_other_cfg.cf_sponsor_pos.x) || m_other_cfg.cf_sponsor_pos.x <= 0)
	{
		return -202;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_sponsor_pos_y", m_other_cfg.cf_sponsor_pos.y) || m_other_cfg.cf_sponsor_pos.y <= 0)
	{
		return -203;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_opponent_pos_x", m_other_cfg.cf_opponent_pos.x) || m_other_cfg.cf_opponent_pos.x <= 0)
	{
		return -204;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_opponent_pos_y", m_other_cfg.cf_opponent_pos.y) || m_other_cfg.cf_opponent_pos.y <= 0)
	{
		return -205;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_default_join_times", m_other_cfg.cf_default_join_times) || m_other_cfg.cf_default_join_times <= 0)
	{
		return -206;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_buy_time_need_gold", m_other_cfg.cf_buy_time_need_gold) || m_other_cfg.cf_buy_time_need_gold <= 0)
	{
		return -207;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_restore_join_times_need_time_m", m_other_cfg.cf_restore_join_times_need_time_m) || m_other_cfg.cf_restore_join_times_need_time_m <= 0)
	{
		return -208;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_auto_reflush_interval_s", m_other_cfg.cf_auto_reflush_interval_s) || m_other_cfg.cf_auto_reflush_interval_s <= 0)
	{
		return -212;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_reflush_need_bind_gold", m_other_cfg.cf_reflush_need_bind_gold) || m_other_cfg.cf_reflush_need_bind_gold <= 0)
	{
		return -209;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_win_add_jifen", m_other_cfg.cf_win_add_jifen) || m_other_cfg.cf_win_add_jifen <= 0)
	{
		return -210;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_win_add_mojing", m_other_cfg.cf_win_add_mojing) || m_other_cfg.cf_win_add_mojing < 0)
	{
		return -211;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_win_add_exp", m_other_cfg.cf_win_add_exp) || m_other_cfg.cf_win_add_exp < 0)
	{
		return -213;
	}

	if (!PugiGetSubNodeValue(data_element, "cf_stop_level", m_other_cfg.cf_stop_level) || m_other_cfg.cf_stop_level < 0)
	{
		return -214;
	}

	{
		PugiXmlNode element = data_element.child("cf_win_item_list");
		if (element.empty())
		{
			return -300;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("cf_win_item");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -300 - i;
			}

			if (!m_other_cfg.cf_wint_item[i].ReadConfig(item_element))
			{
				if (0 == m_other_cfg.cf_wint_item[i].item_id)
				{
					break;
				}

				return -310 - i;
			}

			++i;
			item_element = item_element.next_sibling();
		}
	}

	return 0;
}

int FightingConfig::InitMiningRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode  data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_mining_max_quality = -1;
	while (!data_element.empty())
	{
		if (m_mining_max_quality >= FightingMiningRewardCfg::MAX_QUALITY_TYPE)
		{
			return -1;
		}

		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != m_mining_max_quality + 1)
		{
			return -2;
		}
		m_mining_max_quality = quality;
		FightingMiningRewardCfg &mining_cfg = m_mining_reward_cfg_list[quality];
		mining_cfg.quality = quality;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", mining_cfg.consume_gold) || mining_cfg.consume_gold <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_rate", mining_cfg.upgrade_rate) || mining_cfg.upgrade_rate <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_exp", mining_cfg.reward_exp) || mining_cfg.reward_exp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "rob_get_item_count", mining_cfg.rob_get_item_count) || mining_cfg.rob_get_item_count < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "robot_reflush_weight", mining_cfg.robot_reflush_weight) || mining_cfg.robot_reflush_weight < 0)
		{
			return -7;
		}

		m_mining_robot_total_weight += mining_cfg.robot_reflush_weight;

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (mining_cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - mining_cfg.reward_item_count;
				}

				if (!mining_cfg.reward_item_list[mining_cfg.reward_item_count].ReadConfig(item_element))
				{
					if (0 == mining_cfg.reward_item_list[mining_cfg.reward_item_count].item_id)
					{
						break;
					}
					return -100000 - mining_cfg.reward_item_list[mining_cfg.reward_item_count].item_id;
				}

				int rob_once_max_num = int(m_other_cfg.dm_been_rob_times * mining_cfg.reward_item_list[mining_cfg.reward_item_count].num);
				if (mining_cfg.rob_get_item_count > rob_once_max_num)
				{
					return -102;
				}

				++mining_cfg.reward_item_count;
				item_element = item_element.next_sibling();
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int FightingConfig::InitSailingRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode  data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_sailing_max_quality = -1;
	while (!data_element.empty())
	{
		if (m_sailing_max_quality >= FightingSailingRewardCfg::MAX_QUALITY_TYPE)
		{
			return -1;
		}

		int quality = 0;
		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != m_sailing_max_quality + 1)
		{
			return -2;
		}
		m_sailing_max_quality = quality;
		FightingSailingRewardCfg &sailing_cfg = m_sailing_reward_cfg_list[quality];
		sailing_cfg.quality = quality;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", sailing_cfg.consume_gold) || sailing_cfg.consume_gold <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_rate", sailing_cfg.upgrade_rate) || sailing_cfg.upgrade_rate <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_exp", sailing_cfg.reward_exp) || sailing_cfg.reward_exp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "shenge_jinghua", sailing_cfg.shenge_jinghua) || sailing_cfg.shenge_jinghua < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "rob_get_item_count", sailing_cfg.rob_get_item_count) || sailing_cfg.rob_get_item_count < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "robot_reflush_weight", sailing_cfg.robot_reflush_weight) || sailing_cfg.robot_reflush_weight < 0)
		{
			return -8;
		}

		m_sailing_robot_total_weight += sailing_cfg.robot_reflush_weight;

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (sailing_cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - sailing_cfg.reward_item_count;
				}

				if (!sailing_cfg.reward_item_list[sailing_cfg.reward_item_count].ReadConfig(item_element))
				{
					if (0 == sailing_cfg.reward_item_list[sailing_cfg.reward_item_count].item_id)
					{
						break;
					}
					return -100000 - sailing_cfg.reward_item_list[sailing_cfg.reward_item_count].item_id;
				}

				int rob_once_max_num = int(m_other_cfg.sl_been_rob_times * sailing_cfg.reward_item_list[sailing_cfg.reward_item_count].num);
				if (sailing_cfg.rob_get_item_count > rob_once_max_num)
				{
					return -102;
				}

				++sailing_cfg.reward_item_count;
				item_element = item_element.next_sibling();
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int FightingConfig::InitChallengeRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode  data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_challenge_rank_reward_cfg_count = 0;

	int pre_rank = 0;
	while (!data_element.empty())
	{
		int rank = 0;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank <= pre_rank)
		{
			return -1;
		}
		pre_rank = rank;

		FightingRankRewardCfg &rank_cfg = m_challenge_rank_reward_cfg_list[m_challenge_rank_reward_cfg_count];
		rank_cfg.rank = rank;

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (rank_cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - rank_cfg.reward_item_count;
				}

				if (!rank_cfg.reward_item_list[rank_cfg.reward_item_count].ReadConfig(item_element))
				{
					return -100000 - rank_cfg.reward_item_list[rank_cfg.reward_item_count].item_id;
				}

				++rank_cfg.reward_item_count;
				item_element = item_element.next_sibling();
			}
		}

		++m_challenge_rank_reward_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int FightingConfig::InitFightingSkipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode  data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_skip_type_cfg.mining_max_quality = -1;
	m_skip_type_cfg.sailing_max_quality = -1;
	while (!data_element.empty())
	{
		if (m_skip_type_cfg.mining_max_quality >= FightingSailingRewardCfg::MAX_QUALITY_TYPE || m_skip_type_cfg.sailing_max_quality >= FightingSailingRewardCfg::MAX_QUALITY_TYPE)
		{
			return -1;
		}

		int type = -1;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type < FightingSkipTypeCfg::FIGHT_SKIP_TYPE_SAIL || type > FightingSkipTypeCfg::FIGHT_SKIP_TYPE_SAIL_ROB)
		{
			return -2;
		}

		switch (type)
		{
		case FightingSkipTypeCfg::FIGHT_SKIP_TYPE_CHALLENGE:
			{
				if (!PugiGetSubNodeValue(data_element, "limit_level", m_skip_type_cfg.challenge_skip_limit_level) || m_skip_type_cfg.challenge_skip_limit_level < 0)
				{
					return -3;
				}

				if (!PugiGetSubNodeValue(data_element, "consume", m_skip_type_cfg.challenge_skip_consume) || m_skip_type_cfg.challenge_skip_consume <= 0)
				{
					return -4;
				}
			}
			break;

		case FightingSkipTypeCfg::FIGHT_SKIP_TYPE_SAIL:
			{
				int quality = 0;
				if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != m_skip_type_cfg.sailing_max_quality + 1)
				{
					return -5;
				}

				m_skip_type_cfg.sailing_max_quality = quality;
				FightingSkipCfg &sailing_cfg = m_skip_type_cfg.sailing_reward_cfg_list[quality];

				if (!PugiGetSubNodeValue(data_element, "limit_level", sailing_cfg.limit_level) || sailing_cfg.limit_level < 0)
				{
					return -6;
				}

				if (!PugiGetSubNodeValue(data_element, "consume", sailing_cfg.consume) || sailing_cfg.consume < 0)
				{
					return -7;
				}
			}
			break;

		case FightingSkipTypeCfg::FIGHT_SKIP_TYPE_MINE:
			{
				int quality = 0;
				if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality != m_skip_type_cfg.mining_max_quality + 1)
				{
					return -8;
				}

				m_skip_type_cfg.mining_max_quality = quality;
				FightingSkipCfg &mining_cfg = m_skip_type_cfg.mining_reward_cfg_list[quality];

				if (!PugiGetSubNodeValue(data_element, "limit_level", mining_cfg.limit_level) || mining_cfg.limit_level < 0)
				{
					return -9;
				}

				if (!PugiGetSubNodeValue(data_element, "consume", mining_cfg.consume) || mining_cfg.consume < 0)
				{
					return -10;
				}
			}
			break;

		case FightingSkipTypeCfg::FIGHT_SKIP_TYPE_MINE_ROB:
			{
				if (!PugiGetSubNodeValue(data_element, "limit_level", m_skip_type_cfg.mining_rob_limit_level) || m_skip_type_cfg.mining_rob_limit_level < 0)
				{
					return -11;
				}

				if (!PugiGetSubNodeValue(data_element, "consume", m_skip_type_cfg.mining_rob_consume) || m_skip_type_cfg.mining_rob_consume <= 0)
				{
					return -12;
				}
			}
			break;

		case FightingSkipTypeCfg::FIGHT_SKIP_TYPE_SAIL_ROB:
			{
				if (!PugiGetSubNodeValue(data_element, "limit_level", m_skip_type_cfg.sailing_rob_limit_level) || m_skip_type_cfg.sailing_rob_limit_level < 0)
				{
					return -13;
				}

				if (!PugiGetSubNodeValue(data_element, "consume", m_skip_type_cfg.sailing_rob_consume) || m_skip_type_cfg.sailing_rob_consume <= 0)
				{
					return -14;
				}
			}
			break;

		default:
			break;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}
