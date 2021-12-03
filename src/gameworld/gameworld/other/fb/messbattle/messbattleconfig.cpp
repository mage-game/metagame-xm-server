#include "messbattleconfig.hpp"

#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "monster/monsterpool.h"


MessBattleFbConfig::MessBattleFbConfig() : m_kill_boss_get_score_cfg_count(0), m_reward_cfg_count(0)
{
	memset(m_title_reward, 0, sizeof(m_title_reward));
}

MessBattleFbConfig::~MessBattleFbConfig()
{

}

bool MessBattleFbConfig::Init(const std::string &configname, std::string *err)
{
	YY_XML_CONFIG_PRE_LOAD;

	// 其他配置
	YY_XML_LOAD_CONFIG("other", InitOtherCfg);
	// 复活点
	YY_XML_LOAD_CONFIG("relive_pos", InitRelivePosCfg);
	// 击杀boss获取积分
	YY_XML_LOAD_CONFIG("kill_boss_get_score", InitKillBossGetScoreCfg);
	// 奖励
	YY_XML_LOAD_CONFIG("reward", InitRewardCfg);

	YY_XML_LOAD_CONFIG("rank_title", InitRewardTitle);

	return true;
}

const Posi MessBattleFbConfig::GetRelievePos()
{
	if (m_relive_pos.size() <= 0)
	{
		return Posi(0, 0);
	}
	int rand_num = RandomNum(m_relive_pos.size());

	return Posi(m_relive_pos[rand_num].relive_pos_x, m_relive_pos[rand_num].relive_pos_y);
}

const Posi MessBattleFbConfig::GetRelievePosByKey(int key)
{
		if (m_relive_pos.size() <= 0)
	{
		return Posi(0, 0);
	}
	if(key >= static_cast<int>(m_relive_pos.size()))
	{
		int rand_num = RandomNum(m_relive_pos.size());

		return Posi(m_relive_pos[rand_num].relive_pos_x, m_relive_pos[rand_num].relive_pos_y);
	}

	return Posi(m_relive_pos[key].relive_pos_x, m_relive_pos[key].relive_pos_y);
}

const MessBattleRewardCfg * MessBattleFbConfig::GetRewardCfgByRank(const int rank)
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

int MessBattleFbConfig::GetKillBossGetScoreCfgByRank(const int rank) const
{
	for (int i = 0; i < m_kill_boss_get_score_cfg_count; ++i)
	{
		if (rank >= m_kill_boss_get_score_cfg_list[i].min_rank && rank <= m_kill_boss_get_score_cfg_list[i].max_rank)
		{
			return m_kill_boss_get_score_cfg_list[i].get_score;
		}
	}

	return 0;
}

int MessBattleFbConfig::GetTitleIdByRank(const int rank)const
{
	if (rank < 1 || rank > MESS_BATTLE_RANK_TOP_TITLE_MAX) return 0;
	
	return m_title_reward[rank - 1];
}

int MessBattleFbConfig::InitOtherCfg(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level <= 0)
	{
		return -1;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "room_member_limit", m_other_cfg.room_member_limit) || m_other_cfg.room_member_limit <= 0)
	{
		return -2;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "kill_item_get_score", m_other_cfg.kill_item_get_score) || m_other_cfg.kill_item_get_score <= 0)
	{
		return -3;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "snatch_score_per", m_other_cfg.snatch_score_per) || m_other_cfg.snatch_score_per <= 0)
	{
		return -4;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "min_score", m_other_cfg.min_score) || m_other_cfg.min_score <= 0)
	{
		return -5;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_score", m_other_cfg.max_score) || m_other_cfg.max_score <= 0)
	{
		return -6;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -7;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "one_score_to_honor", m_other_cfg.one_score_to_honor) || m_other_cfg.one_score_to_honor < 0)
	{
		return -8;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "init_score", m_other_cfg.init_score) || m_other_cfg.init_score < 0)
	{
		return -9;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "rank_update_interval_s", m_other_cfg.rank_update_interval_s) || m_other_cfg.rank_update_interval_s < 0)
	{
		return -10;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "redistribute_interval_time_s", m_other_cfg.redistribute_interval_time_s) || m_other_cfg.redistribute_interval_time_s < 0)
	{
		return -11;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "boss_id", m_other_cfg.boss_id) || m_other_cfg.boss_id < 0 || NULL == MONSTERPOOL->GetMonsterParam(m_other_cfg.boss_id))
	{
		return -12;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "delay_kick_out_time", m_other_cfg.delay_kick_out_time) || m_other_cfg.delay_kick_out_time < 0 )
	{
		return -13;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "boss_position_x", m_other_cfg.boss_posi.x) || m_other_cfg.boss_posi.x <= 0)
	{
		return -14;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "boss_position_y", m_other_cfg.boss_posi.y) || m_other_cfg.boss_posi.y <= 0)
	{
		return -15;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "update_role_interval_s", m_other_cfg.update_role_interval_s) || m_other_cfg.update_role_interval_s < 0)
	{
		return -16;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "submit_report_limit_score", m_other_cfg.submit_report_limit_score) || m_other_cfg.submit_report_limit_score < 0)
	{
		return -17;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "wudi_time_s", m_other_cfg.wudi_time_s) || m_other_cfg.wudi_time_s < 0)
	{
		return -17;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "open_cross_begin_day", m_other_cfg.open_cross_begin_day) || m_other_cfg.open_cross_begin_day <= 0)
	{
		return -18;
	}

	if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "activity_open_dur_s", m_other_cfg.activity_open_dur_s) || m_other_cfg.activity_open_dur_s < 0)
	{
		return -19;
	}

	return 0;
}

int MessBattleFbConfig::InitRelivePosCfg(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		MessBattleRelivePos cfg;

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "relive_pos_x", cfg.relive_pos_x) || cfg.relive_pos_x < 0)
		{
			return -1;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "relive_pos_y", cfg.relive_pos_y) || cfg.relive_pos_y < 0)
		{
			return -2;
		}

		m_relive_pos.push_back(cfg);

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int MessBattleFbConfig::InitKillBossGetScoreCfg(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		if (m_kill_boss_get_score_cfg_count >= MESS_BATTLE_REWARD_INTERVAL_MAX_NUM)
		{
			return -1;
		}

		MessBattleKillBossGetScoreCfg &cfg = m_kill_boss_get_score_cfg_list[m_kill_boss_get_score_cfg_count];

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "min_rank", cfg.min_rank) || cfg.min_rank < 0)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_rank", cfg.max_rank) || cfg.max_rank < 0 || cfg.min_rank > cfg.max_rank)
		{
			return -3;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "get_score", cfg.get_score) || cfg.get_score < 0)
		{
			return -4;
		}

		m_kill_boss_get_score_cfg_count++;
		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int MessBattleFbConfig::InitRewardCfg(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		if(m_reward_cfg_count >= MESS_BATTLE_REWARD_INTERVAL_MAX_NUM)
		{
			return -1;
		}

		MessBattleRewardCfg &cfg = m_reward_cfg_list[m_reward_cfg_count];

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "min_rank", cfg.min_rank) || cfg.min_rank < 0)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "max_rank", cfg.max_rank) || cfg.max_rank < 0 || cfg.min_rank > cfg.max_rank)
		{
			return -3;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0 )
		{
			return -4;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "shengwang", cfg.shengwang) || cfg.shengwang < 0 )
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
		data_element = YY_XML_NEXT_SIBLING(data_element);
	}

	return 0;
}

int MessBattleFbConfig::InitRewardTitle(YY_XML_NODE RootElement)
{
	YY_XML_NODE data_element = YY_XML_FIRST_CHILD_ELEMENT(RootElement, "data");
	if (YY_XML_NODE_IS_EMPTY(data_element))
	{
		return -1000;
	}

	while (!YY_XML_NODE_IS_EMPTY(data_element))
	{
		int rank = 0;
		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "rank", rank) || rank < 0 || rank >= MESS_BATTLE_RANK_TOP_TITLE_MAX)
		{
			return -2;
		}

		if (!YY_XML_GET_SUB_NODE_VALUE(data_element, "title_id", m_title_reward[rank]) || m_title_reward[rank] <=0 )
		{
			return -3;
		}

		data_element = YY_XML_NEXT_SIBLING(data_element);
	}
	return 0;
}
