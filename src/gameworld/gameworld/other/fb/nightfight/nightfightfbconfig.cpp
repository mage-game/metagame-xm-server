
#include "nightfightfbconfig.hpp"

#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "servercommon/activitydef.hpp"

NightFightFbConfig::NightFightFbConfig() : m_reward_cfg_count(0)
{
}

NightFightFbConfig::~NightFightFbConfig()
{

}

bool NightFightFbConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;
	
	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);

	// 复活点
	PUGI_XML_LOAD_CONFIG("relive_pos", InitRelivePosCfg);

	// 奖励
	PUGI_XML_LOAD_CONFIG("reward", InitRewardCfg);

	return true;
}

const Posi NightFightFbConfig::GetRelievePos()
{
	int rand_num = RandomNum(m_relive_pos.size());

	return Posi(m_relive_pos[rand_num].relive_pos_x, m_relive_pos[rand_num].relive_pos_y);
}

const Posi NightFightFbConfig::GetEnterPosStandy() const
{
	if (m_relive_pos.size() > 0)
	{
		return Posi(m_relive_pos[0].relive_pos_x, m_relive_pos[0].relive_pos_y);
	}

	return Posi(0, 0);
}

const RewardCfg * NightFightFbConfig::GetRewardCfgByRank(const int rank)
{
	for(int i = 0; i < m_reward_cfg_count; ++i)
	{
		if (rank >= m_reward_cfg_list[i].min_rank && rank <= m_reward_cfg_list[i].max_rank)
		{
			return &m_reward_cfg_list[i];
		}
	}

	return NULL;
}

const int NightFightFbConfig::GetTitleIdByRank(const int rank)
{
	if (rank <= 0 || rank > NIGHT_FIGHT_RANK_TOP_MAX) return 0;
	if (1 == rank)
	{
		return m_other_cfg.title_first;
	}
	else if (2 == rank)
	{
		return m_other_cfg.title_second;
	}
	else
	{
		return m_other_cfg.title_third;
	}
}

int NightFightFbConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "open_cross_begin_day", m_other_cfg.open_cross_begin_day) || m_other_cfg.open_cross_begin_day <= 0)
	{
		return -101;
	}

	if (!PugiGetSubNodeValue(data_element, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "room_member_limit", m_other_cfg.room_member_limit) || m_other_cfg.room_member_limit <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "redistribute_interval_time_s", m_other_cfg.redistribute_interval_time_s) || m_other_cfg.redistribute_interval_time_s <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "interval_time_s", m_other_cfg.interval_time_s) || m_other_cfg.interval_time_s <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "interval_time_get_score", m_other_cfg.interval_time_get_score) || m_other_cfg.interval_time_get_score <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_get_score_per", m_other_cfg.kill_get_score_per) || m_other_cfg.kill_get_score_per <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "killed_reduce_score_per", m_other_cfg.killed_reduce_score_per) || m_other_cfg.killed_reduce_score_per <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "relive_interval_time_s", m_other_cfg.relive_interval_time_s) || m_other_cfg.relive_interval_time_s <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_monster_add_score", m_other_cfg.kill_monster_add_score) || m_other_cfg.kill_monster_add_score <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -10;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(data_element, "init_score", m_other_cfg.init_score) || m_other_cfg.init_score < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "min_score", m_other_cfg.min_score) || m_other_cfg.min_score < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "rank_update_interval_s", m_other_cfg.rank_update_interval_s) || m_other_cfg.rank_update_interval_s < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "assisting_area", m_other_cfg.assisting_area) || m_other_cfg.assisting_area < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "assisting_add_score", m_other_cfg.assisting_add_score) || m_other_cfg.assisting_add_score < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "delay_kick_out_time", m_other_cfg.delay_kick_out_time) || m_other_cfg.delay_kick_out_time < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "update_role_interval_s", m_other_cfg.update_role_interval_s) || m_other_cfg.update_role_interval_s < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_flush_interval_s", m_other_cfg.boss_flush_interval_s) || m_other_cfg.boss_flush_interval_s <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_id", m_other_cfg.boss_id) || m_other_cfg.boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(m_other_cfg.boss_id))
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_get_score_range", m_other_cfg.kill_get_score_range) || m_other_cfg.kill_get_score_range <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "add_score", m_other_cfg.add_score) || m_other_cfg.add_score <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(data_element, "title_first", m_other_cfg.title_first) || m_other_cfg.title_first <= 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(data_element, "title_second", m_other_cfg.title_second) || m_other_cfg.title_second <= 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(data_element, "title_third", m_other_cfg.title_third) || m_other_cfg.title_third <= 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(data_element, "activity_open_dur_s", m_other_cfg.activity_open_dur_s) || m_other_cfg.activity_open_dur_s <= 300)
	{
		return -26;
	}

	if (!m_other_cfg.boss_born_pos.ReadConfig(data_element, "boss_born_pos"))
	{
		return -100;
	}

	return 0;
}

int NightFightFbConfig::InitRelivePosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		RelivePos cfg;

		if (!PugiGetSubNodeValue(data_element, "relive_pos_x", cfg.relive_pos_x) || cfg.relive_pos_x < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "relive_pos_y", cfg.relive_pos_y) || cfg.relive_pos_y < 0)
		{
			return -2;
		}

		m_relive_pos.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int NightFightFbConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		if(m_reward_cfg_count >= REWARD_INTERVAL_MAX_NUM)
		{
			return -1;
		}

		RewardCfg &cfg = m_reward_cfg_list[m_reward_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "min_rank", cfg.min_rank) || cfg.min_rank < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank", cfg.max_rank) || cfg.max_rank < 0 || cfg.min_rank > cfg.max_rank)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "shengwang", cfg.shengwang) || cfg.shengwang < 0)
		{
			return -5;
		}

		{
			cfg.reward_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, MonsterInitParam::MAX_DROP_ITEM_COUNT);
			if (cfg.reward_count < 0)
			{
				return -200 + cfg.reward_count;
			}
		}

		m_reward_cfg_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}
