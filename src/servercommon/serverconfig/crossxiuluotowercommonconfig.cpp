#include "crossxiuluotowercommonconfig.hpp"
#include "servercommon/configcommon.h"

CrossXiuluoTowerCommonConfig::CrossXiuluoTowerCommonConfig()
	: m_layer_reward_cfg_count(0), m_rank_reward_cfg_count(0), m_kill_boss_reward_item_id(0), m_kill_role_reward_honor(0),
	m_kill_role_score(0), m_kill_monster_score(0), m_pass_layer_score(0), m_boss_refresh_time(0),
	m_dur_dead_buff_count(0), m_dur_kill_buff_count(0)
{
	memset(m_xiuluotower_rank_title, 0, sizeof(m_xiuluotower_rank_title));
}

CrossXiuluoTowerCommonConfig::~CrossXiuluoTowerCommonConfig()
{

}

CrossXiuluoTowerCommonConfig & CrossXiuluoTowerCommonConfig::Instance()
{
	static CrossXiuluoTowerCommonConfig cxtcc;
	return cxtcc;
}

bool CrossXiuluoTowerCommonConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossXiuluoTowerCommonConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode ActivityTimeCfgElement = RootElement.child("activity_time_cfg");
		if (ActivityTimeCfgElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg]";
			return false;
		}

		if (!PugiGetSubNodeValue(ActivityTimeCfgElement, "is_open", m_activity_time_cfg.is_open))
		{
			*err = path + ": xml node error in [config->activity_time_cfg->is_open]";
			return false;
		}

		PugiXmlNode OpenDayListElement = ActivityTimeCfgElement.child("open_day_list");
		if (OpenDayListElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list]";
			return false;
		}

		PugiXmlNode OpenDayElement = OpenDayListElement.child("open_day");
		if (OpenDayElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
			return false;
		}

		while (!OpenDayElement.empty())
		{
			int open_day = 0;
			if (!PugiGetNodeValue(OpenDayElement, open_day) || open_day <= 0 || open_day > CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			open_day = open_day % CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS;
			if (open_day < 0 || open_day >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS)
			{
				*err = path + ": xml node error in [config->activity_time_cfg->open_day_list->open_day]";
				return false;
			}

			m_activity_time_cfg.open_day_list[open_day] = true;

			OpenDayElement = OpenDayElement.next_sibling();
		}

		if (!PugiGetSubNodeValue(ActivityTimeCfgElement, "is_open", m_activity_time_cfg.is_open))
		{
			*err = path + ": xml node error in [config->activity_time_cfg->is_open]";
			return false;
		}

		PugiXmlNode IntervalElement = ActivityTimeCfgElement.child("time_interval");
		if (IntervalElement.empty())
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval]";
			return false;
		}

		int hour = 0, minute = 0, second = 0, standby_dur_s = 0, open_dur_s = 0;
		if (!PugiGetSubNodeValue(IntervalElement, "hour", hour) || hour < 0 || hour >= 24)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->hour]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "minute", minute) || minute < 0 || minute >= 60)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->minute]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "second", second) || second < 0 || second >= 60)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->second]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "standby_dur_s", standby_dur_s) || standby_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->standby_dur_s]";
			return false;
		}
		if (!PugiGetSubNodeValue(IntervalElement, "open_dur_s", open_dur_s) || open_dur_s <= 0)
		{
			*err = path + ": xml node error in [config->activity_time_cfg->time_interval->open_dur_s]";
			return false;
		}

		m_activity_time_cfg.open_day_second = hour * 3600 + minute * 60 + second;
		m_activity_time_cfg.standby_dur_s = standby_dur_s;
		m_activity_time_cfg.open_dur_s = open_dur_s;
	}

	{
		PugiXmlNode LayerListElement = RootElement.child("layer_list");
		if (LayerListElement.empty())
		{
			*err = path + ": xml node error in [config->layer_list]";
			return false;
		}

		PugiXmlNode LayerElement = LayerListElement.child("layer_cfg");
		if (LayerElement.empty())
		{
			*err = path + ": xml node error in [config->layer_list->layer_cfg]";
			return false;
		}

		int layer_count = 0;
		while (!LayerElement.empty())
		{
			if (layer_count >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
			{
				*err = path + ": xml node error in [config->layer_list->layer_cfg] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(LayerElement, "scene_id", m_layer_list[layer_count].scene_id) || m_layer_list[layer_count].scene_id <= 0)
			{
				*err = path + ": xml node error in [config->layer_list->layer_cfg->scene_id]";
				return false;
			}

			if (!PugiGetSubNodeValue(LayerElement, "up_layer_need_kills", m_layer_list[layer_count].up_layer_need_kills) || m_layer_list[layer_count].up_layer_need_kills <= 0)
			{
				*err = path + ": xml node error in [config->layer_list->layer_cfg->up_layer_need_kills]";
				return false;
			}

			PugiXmlNode RealivePointListElement = LayerElement.child("realive_point_list");
			if (RealivePointListElement.empty())
			{
				*err = path + ": xml node error in [config->layer_list->layer_cfg->realive_point_list]";
				return false;
			}

			PugiXmlNode RealivePointElement = RealivePointListElement.child("realive_point");
			if (RealivePointElement.empty())
			{
				*err = path + ": xml node error in [config->layer_list->layer_cfg->realive_point_list->realive_point]";
				return false;
			}

			int point_count = 0;
			while (!RealivePointElement.empty())
			{
				if (point_count >= CROSS_ACTIVITY_XIULUO_TOWER_REALIVE_POINT_MAX)
				{
					*err = path + ": xml node error in [config->layer_list->layer_cfg->realive_point_list->realive_point] more than limit";
					return false;
				}

				if (!PugiGetSubNodeValue(RealivePointElement, "realive_pos_x", m_layer_list[layer_count].realive_point_list[point_count].x) || m_layer_list[layer_count].realive_point_list[point_count].x <= 0)
				{
					*err = path + ": xml node error in [config->layer_list->layer_cfg->realive_pos_x]";
					return false;
				}

				if (!PugiGetSubNodeValue(RealivePointElement, "realive_pos_y", m_layer_list[layer_count].realive_point_list[point_count].y) || m_layer_list[layer_count].realive_point_list[point_count].y <= 0)
				{
					*err = path + ": xml node error in [config->layer_list->layer_cfg->realive_pos_y]";
					return false;
				}

				++ point_count;
				RealivePointElement = RealivePointElement.next_sibling();
			}

			m_layer_list[layer_count].realive_point_count = point_count;

			++layer_count;
			LayerElement = LayerElement.next_sibling();
		}
	}

	{
		PugiXmlNode RewardListElement = RootElement.child("layer_reward_list");
		if (RewardListElement.empty())
		{
			*err = path + ": xml node error in [config->layer_reward_list]";
			return false;
		}

		PugiXmlNode RewardElement = RewardListElement.child("reward");
		if (RewardElement.empty())
		{
			*err = path + ": xml node error in [config->layer_reward_list->reward]";
			return false;
		}

		int reward_count = 0;
		while (!RewardElement.empty())
		{
			if (reward_count >= CROSS_ACTIVITY_XIULUO_TOWER_REWARD_CFG_MAX)
			{
				*err = path + ": xml node error in [config->layer_reward_list->reward] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardElement, "reward_layer", m_layer_reward_cfg_list[reward_count].reward_layer) || m_layer_reward_cfg_list[reward_count].reward_layer <= 0 || m_layer_reward_cfg_list[reward_count].reward_layer > CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
			{
				*err = path + ": xml node error in [config->layer_reward_list->reward->reward_layer]";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardElement, "reward_cross_honor", m_layer_reward_cfg_list[reward_count].reward_cross_honor) || m_layer_reward_cfg_list[reward_count].reward_cross_honor <= 0)
			{
				*err = path + ": xml node error in [config->layer_reward_list->reward->reward_cross_honor]";
				return false;
			}

			PugiXmlNode RewardItemElement = RewardElement.child("reward_item");
			if (RewardItemElement.empty())
			{
				*err = path + ": xml node error in [config->layer_reward_list->reward->reward_item_list->reward_item]";
				return false;
			}

			if (!m_layer_reward_cfg_list[reward_count].reward_item.ReadConfigNoCheck(RewardItemElement))
			{
				*err = path + ": xml node error in [config->layer_reward_list->reward->reward_item_list->reward_item]";
				return false;
			}

			++ reward_count;
			RewardElement = RewardElement.next_sibling();
		}
		m_layer_reward_cfg_count = reward_count;
	}

	{
		PugiXmlNode RewardListElement = RootElement.child("rank_reward_list");
		if (RewardListElement.empty())
		{
			*err = path + ": xml node error in [config->rank_reward_list]";
			return false;
		}

		PugiXmlNode RewardElement = RewardListElement.child("reward");
		if (RewardElement.empty())
		{
			*err = path + ": xml node error in [config->rank_reward_list->reward]";
			return false;
		}

		int rank_reward_count = 0;
		while (!RewardElement.empty())
		{
			if (rank_reward_count >= CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_CFG_MAX)
			{
				*err = path + ": xml node error in [config->rank_reward_list->reward] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(RewardElement, "min_pos", m_rank_reward_cfg_list[rank_reward_count].min_pos) || m_rank_reward_cfg_list[rank_reward_count].min_pos <= 0)
			{
				*err = path + ": xml node error in [config->rank_reward_list->reward->min_pos]";
				return false;
			}

			if (rank_reward_count > 0)
			{
				if (m_rank_reward_cfg_list[rank_reward_count].min_pos <= m_rank_reward_cfg_list[rank_reward_count - 1].max_pos)
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->min_pos]";
					return false;
				}
			}

			if (!PugiGetSubNodeValue(RewardElement, "max_pos", m_rank_reward_cfg_list[rank_reward_count].max_pos) || m_rank_reward_cfg_list[rank_reward_count].max_pos < m_rank_reward_cfg_list[rank_reward_count].min_pos)
			{
				*err = path + ": xml node error in [config->rank_reward_list->reward->max_pos]";
				return false;
			}

			PugiXmlNode LevelRewardElement = RewardElement.child("level_reward");
			if (LevelRewardElement.empty())
			{
				*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward]";
				return false;
			}

			int level_reward_count = 0;
			while (!LevelRewardElement.empty())
			{
				if (level_reward_count >= CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_LEVEL_CFG_MAX)
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward] more than limit";
					return false;
				}

				if (!PugiGetSubNodeValue(LevelRewardElement, "min_level", m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].min_level) || m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].min_level <= 0)
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->min_level]";
					return false;
				}

				if (level_reward_count > 0)
				{
					if (m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].min_level <= m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count - 1].max_level)
					{
						*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->min_level]";
						return false;
					}
				}

				if (!PugiGetSubNodeValue(LevelRewardElement, "max_level", m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].max_level) || m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].max_level < m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].min_level)
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->max_level]";
					return false;
				}

				if (!PugiGetSubNodeValue(LevelRewardElement, "reward_honor", m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].reward_honor) || m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].reward_honor < 0)
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->reward_honor]";
					return false;
				}

				PugiXmlNode RewardItemElement = LevelRewardElement.child("reward_item");
				if (RewardItemElement.empty())
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->reward_item]";
					return false;
				}

				if (!m_rank_reward_cfg_list[rank_reward_count].level_reward_list[level_reward_count].reward_item.ReadConfigNoCheck(RewardItemElement))
				{
					*err = path + ": xml node error in [config->rank_reward_list->reward->level_reward->reward_item]";
					return false;
				}

				++ level_reward_count;
				LevelRewardElement = LevelRewardElement.next_sibling();
			}

			m_rank_reward_cfg_list[rank_reward_count].level_reward_count = level_reward_count;
			++ rank_reward_count;
			RewardElement = RewardElement.next_sibling();
		}
		m_rank_reward_cfg_count = rank_reward_count;
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "kill_boss_reward_item_id", m_kill_boss_reward_item_id) || m_kill_boss_reward_item_id < 0)
		{
			*err = path + ": xml node error in [config->kill_boss_reward_item_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "kill_role_reward_honor", m_kill_role_reward_honor) || m_kill_role_reward_honor < 0)
		{
			*err = path + ": xml node error in [config->kill_role_reward_honor]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "kill_role_score", m_kill_role_score) || m_kill_role_score <= 0)
		{
			*err = path + ": xml node error in [config->kill_role_score]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "kill_monster_score", m_kill_monster_score) || m_kill_monster_score <= 0)
		{
			*err = path + ": xml node error in [config->kill_monster_score]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "pass_layer_score", m_pass_layer_score) || m_pass_layer_score < 0)
		{
			*err = path + ": xml node error in [config->pass_layer_score]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "boss_refresh_time", m_boss_refresh_time) || m_boss_refresh_time <= 0)
		{
			*err = path + ": xml node error in [config->boss_refresh_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "boss_refresh_notice_time", m_boss_refresh_notice_time) || m_boss_refresh_notice_time <= 0)
		{
			*err = path + ": xml node error in [config->boss_refresh_notice_time]";
			return false;
		}
	}

	{
		PugiXmlNode DurKillBuffListElement = RootElement.child("dur_kill_buff_list");
		if (DurKillBuffListElement.empty())
		{
			*err = path + ": xml node error in [config->dur_kill_buff_list]";
			return false;
		}

		PugiXmlNode DataElement = DurKillBuffListElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->dur_kill_buff_list->data]";
			return false;
		}

		m_dur_kill_buff_count = 0;
		while (!DataElement.empty())
		{
			if (m_dur_kill_buff_count >= CROSS_ACTIVITY_XIULUO_TOWER_DUR_KILL_BUFF_MAX)
			{
				*err = path + ": xml node error in [config->dur_kill_buff_list->data] more than limit";
				return false;
			}

			CrossXiuluoTowerDurKillBuff &cfg = m_dur_kill_buff_list[m_dur_kill_buff_count];

			if (!PugiGetSubNodeValue(DataElement, "dur_kill_count", cfg.dur_kill_count) || cfg.dur_kill_count <= 0)
			{
				*err = path + ": xml node error in [config->dur_kill_buff_list->data->dur_kill_count]";
				return false;
			}

			if (m_dur_kill_buff_count > 0 && m_dur_kill_buff_list[m_dur_kill_buff_count - 1].dur_kill_count >= cfg.dur_kill_count)
			{
				*err = path + ": xml node error in [config->dur_kill_buff_list->data->dur_kill_count]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "add_max_hp_per", cfg.add_max_hp_per) || cfg.add_max_hp_per <= 0)
			{
				*err = path + ": xml node error in [config->dur_kill_buff_list->data->add_max_hp_per]";
				return false;
			}


			if (!PugiGetSubNodeValue(DataElement, "add_gongji_per", cfg.add_gongji_per) || cfg.add_gongji_per <= 0)
			{
				*err = path + ": xml node error in [config->dur_kill_buff_list->data->add_gongji_per]";
				return false;
			}


			++ m_dur_kill_buff_count;
			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode DurDeadBuffListElement = RootElement.child("dur_dead_buff_list");
		if (DurDeadBuffListElement.empty())
		{
			*err = path + ": xml node error in [config->dur_dead_buff_list]";
			return false;
		}

		PugiXmlNode DataElement = DurDeadBuffListElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->dur_dead_buff_list->data]";
			return false;
		}

		m_dur_dead_buff_count = 0;
		while (!DataElement.empty())
		{
			if (m_dur_dead_buff_count >= CROSS_ACTIVITY_XIULUO_TOWER_DUR_DEAD_BUFF_MAX)
			{
				*err = path + ": xml node error in [config->dur_dead_buff_list->data] more than limit";
				return false;
			}

			CrossXiuluoTowerDurDeadBuff &cfg = m_dur_dead_buff_list[m_dur_dead_buff_count];

			if (!PugiGetSubNodeValue(DataElement, "dur_dead_count", cfg.dur_dead_count) || cfg.dur_dead_count <= 0)
			{
				*err = path + ": xml node error in [config->dur_dead_buff_list->data->dur_dead_count]";
				return false;
			}

			if (m_dur_dead_buff_count > 0 && m_dur_dead_buff_list[m_dur_dead_buff_count - 1].dur_dead_count >= cfg.dur_dead_count)
			{
				*err = path + ": xml node error in [config->dur_dead_buff_list->data->dur_dead_count]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "add_max_hp_per", cfg.add_max_hp_per) || cfg.add_max_hp_per <= 0)
			{
				*err = path + ": xml node error in [config->dur_dead_buff_list->data->add_max_hp_per]";
				return false;
			}


			if (!PugiGetSubNodeValue(DataElement, "add_gongji_per", cfg.add_gongji_per) || cfg.add_gongji_per <= 0)
			{
				*err = path + ": xml node error in [config->dur_dead_buff_list->data->add_gongji_per]";
				return false;
			}


			++ m_dur_dead_buff_count;
			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode buff_reward_element = RootElement.child("buff_reward");
		if (buff_reward_element.empty())
		{
			*err = path + ": xml node error in [config->buff_reward] is NULL";
			return false;
		}

		if (!m_reward_cfg.buff_item.ReadConfigNoCheck(buff_reward_element))
		{
			*err = path + ": xml node error in [config->buff_reward]";
			return false;
		}
	}

	{
		PugiXmlNode first_reward_element = RootElement.child("first_reward");
		if (first_reward_element.empty())
		{
			*err = path + ": xml node error in [config->first_reward] is NULL";
			return false;
		}

		PugiXmlNode data_element = first_reward_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->first_reward->data]";
			return false;
		}

		while (!data_element.empty())
		{
			int layer = 0;
			if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER <= layer)
			{
				*err = path + ": xml node error in [config->first_reward->data->layer]";
				return false;
			}

			CrossXiuluoTowerFirstRewardCfg *first_cfg = &m_reward_cfg.first_reward[layer];
			if (!PugiGetSubNodeValue(data_element, "honour", first_cfg->honour) || first_cfg->honour < 0)
			{
				*err = path + ": xml node error in [config->first_reward->data->honour]";
				return false;
			}

			PugiXmlNode reward_element = data_element.child("reward_item");
			if (!reward_element.empty())
			{
				if (!first_cfg->item.ReadConfigNoCheck(reward_element))
				{
					*err = path + ": xml node error in [config->first_reward->data->reward_item]";
					return false;
				}
			}

			data_element = data_element.next_sibling();
		}
	}

	{
		PugiXmlNode boss_cfg_element = RootElement.child("boss_cfg");
		if (boss_cfg_element.empty())
		{
			*err = path + ": xml node error in [config->boss_cfg] is NULL";
			return false;
		}

		CrossXiuluoTowerBossCfg *boss_cfg = &m_reward_cfg.boss_cfg;
		if (!PugiGetSubNodeValue(boss_cfg_element, "boss_id", boss_cfg->monster_id) || boss_cfg->monster_id <= 0)
		{
			*err = path + ": xml node error in [config->boss_cfg->boss_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(boss_cfg_element, "boss_num", boss_cfg->monster_num) || boss_cfg->monster_num <= 0)
		{
			*err = path + ": xml node error in [config->boss_cfg->boss_num]";
			return false;
		}

		if (!PugiGetSubNodeValue(boss_cfg_element, "pos_x", boss_cfg->pos.x) || boss_cfg->pos.x < 0)
		{
			*err = path + ": xml node error in [config->boss_cfg->pos_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(boss_cfg_element, "pos_y", boss_cfg->pos.y) || boss_cfg->pos.y < 0)
		{
			*err = path + ": xml node error in [config->boss_cfg->pos_y]";
			return false;
		}
	}

	{
		PugiXmlNode score_reward_element = RootElement.child("score_reward");
		if (score_reward_element.empty())
		{
			*err = path + ": xml node error in [config->score_reward] is NULL";
			return false;
		}

		PugiXmlNode data_element = score_reward_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->score_reward->data]";
			return false;
		}

		int last_score = 0;
		int index = 0;
		while (!data_element.empty())
		{
			if (index < 0 || index >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_SCORE_REWARD)
			{
				*err = path + ": xml node error in [config->score_reward->data] too max";
				return false;
			}

			CrossXiuluoTowerScoreRewardCfg *cfg = &m_reward_cfg.score_reward[index];
			if (!PugiGetSubNodeValue(data_element, "score", cfg->score) || cfg->score <= last_score)
			{
				*err = path + ": xml node error in [config->score_reward->data->score]";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "reward_honor", cfg->reward_honor) || cfg->reward_honor < 0)
			{
				*err = path + ": xml node error in [config->score_reward->data->reward_honor]";
				return false;
			}

			cfg->item_count = 0;

			PugiXmlNode reward_element = data_element.child("reward_item");
			while (!reward_element.empty())
			{
				if (cfg->item_count < 0 || cfg->item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					*err = path + ": xml node error in [config->score_reward->data->reward_item] too max";
					return false;
				}

				if (!cfg->item_list[cfg->item_count].ReadConfigNoCheck(reward_element))
				{
					*err = path + ": xml node error in [config->first_reward->data->reward_item]";
					return false;
				}

				cfg->item_count++;
				reward_element = reward_element.next_sibling("reward_item");
			}

			index++;
			data_element = data_element.next_sibling();
		}
	}

	{
		PugiXmlNode gather_reward_element = RootElement.child("gather_reward");
		if (gather_reward_element.empty())
		{
			*err = path + ": xml node error in [config->gather_reward] is NULL";
			return false;
		}

		PugiXmlNode data_element = gather_reward_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->gather_reward->data]";
			return false;
		}

		while (!data_element.empty())
		{
			int gather_id = 0;
			if (!PugiGetSubNodeValue(data_element, "gather_id", gather_id) || gather_id <= 0)
			{
				*err = path + ": xml node error in [config->gather_reward->data->gather_id]";
				return false;
			}

			CrossXiuluoTowerGatherRewardCfg &cfg = m_reward_cfg.gather_reward_map[gather_id];
			if (0 != cfg.gather_id)
			{
				*err = path + ": xml node error in [config->gather_reward->data->gather_id] exist";
				return false;
			}

			int is_golden = 0;
			cfg.is_golden = false;
			if (!PugiGetSubNodeValue(data_element, "is_golden", is_golden) || is_golden < 0)
			{
				*err = path + ": xml node error in [config->gather_reward->data->gather_id]";
				return false;
			}

			if (0 < is_golden)
			{
				cfg.is_golden = true;
			}

			if (!PugiGetSubNodeValue(data_element, "item_id", cfg.reward_item.item_id))
			{	
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "is_bind", cfg.reward_item.is_bind))
			{	
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "num", cfg.reward_item.num) || cfg.reward_item.num <= 0)
			{	
				return false;
			}

			data_element = data_element.next_sibling();
		}
	}


	{
		PugiXmlNode gather_reward_element = RootElement.child("drop_weight");
		if (gather_reward_element.empty())
		{
			*err = path + ": xml node error in [config->drop_weight] is NULL";
			return false;
		}

		PugiXmlNode data_element = gather_reward_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->drop_weight->data]";
			return false;
		}

		while (!data_element.empty())
		{
			int layer_id = 0;
			if (!PugiGetSubNodeValue(data_element, "layer_id", layer_id) || layer_id < 0 || CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER <= layer_id)
			{
				*err = path + ": xml node error in [config->drop_weight->data->layer_id]";
				return false;
			}

			int drop_weight = 0;
			if (!PugiGetSubNodeValue(data_element, "drop_weight", drop_weight) || drop_weight < 0 || drop_weight > 100)
			{
				*err = path + ": xml node error in [config->drop_weight->data->drop_weight]";
				return false;
			}
			m_drop_weight_list[layer_id] = drop_weight;

			data_element = data_element.next_sibling();
		}
	}

	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode other_element = RootElement.child("other_cfg");
		if (other_element.empty())
		{
			*err = path + "xml node no [other_cfg] node.\n";
			return false;
		}
		
		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitOtherCfg failed: %d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	// ³ÆºÅµôÂäÏÞÖÆÅäÖÃ
	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode other_element = RootElement.child("title_drop_limit");
		if (other_element.empty())
		{
			*err = path + "xml node no [title_drop_limit] node.\n";
			return false;
		}

		iRet = this->InitServerTitleDropLimitCfg(other_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitServerTitleDropLimitCfg failed: %d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	// ×øÆïµôÂäÏÞÖÆÅäÖÃ
	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode other_element = RootElement.child("mount_drop_limit");
		if (other_element.empty())
		{
			*err = path + "xml node no [other_cfg] node.\n";
			return false;
		}

		iRet = this->InitServerMountDropLimitCfg(other_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitServerMountDropLimitCfg failed: %d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	// ½ðÏä×ÓµôÂäÏÞÖÆÅäÖÃ
	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode other_element = RootElement.child("goldbox_drop_limit");
		if (other_element.empty())
		{
			*err = path + "xml node no [other_cfg] node.\n";
			return false;
		}

		iRet = this->InitServerGoldBoxDropLimitCfg(other_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitServerGoldBoxDropLimitCfg failed: %d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode title_element = RootElement.child("rank_title_id");
		if (title_element.empty())
		{
			*err = path + "xml node no [rank_title_id] node.\n";
			return false;
		}

		iRet = this->InitXiuluoTowerRankTitle(title_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitXiuluoTowerRankTitle failed: %d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	//
	{
		int iRet = 0;
		char error_info[512] = { 0 };

		PugiXmlNode gather_element = RootElement.child("gather");
		if(gather_element.empty())
		{
			*err = path + "xml node no [gather] node \n";
			return false;
		}

		iRet = this->InitXiuluoTowerGatherCfg(gather_element);
		if (iRet)
		{
			sprintf(error_info, "%s InitXiuluoTowerGatherCfg:%d\n", path.c_str(), iRet);
			*err = error_info;
			return false;
		}
	}

	return true;
}

int CrossXiuluoTowerCommonConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (NULL == RootElement)
	{
		return -1000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1001;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_buff_gold", m_other_cfg.gather_buff_gold) || m_other_cfg.gather_buff_gold <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "gather_buff_duration", m_other_cfg.gather_buff_duration) || m_other_cfg.gather_buff_duration <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "title_drop_id", m_other_cfg.title_drop_id) || m_other_cfg.title_drop_id <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "title_drop_rate", m_other_cfg.title_drop_rate) || m_other_cfg.title_drop_rate < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "server_title_drop_total_num_limit", m_other_cfg.server_title_drop_total_num_limit) || m_other_cfg.server_title_drop_total_num_limit <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "mount_drop_id", m_other_cfg.mount_drop_id) || m_other_cfg.mount_drop_id <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "mount_drop_rate", m_other_cfg.mount_drop_rate) || m_other_cfg.mount_drop_rate < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "server_mount_drop_total_num_limit", m_other_cfg.server_mount_drop_total_num_limit) || m_other_cfg.server_mount_drop_total_num_limit <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_box_title_item_id", m_other_cfg.gold_box_item_id) || m_other_cfg.gold_box_item_id <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_box_title_item_rate", m_other_cfg.gold_box_item_rate) || m_other_cfg.gold_box_item_rate < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "server_gold_box_total_num_limit", m_other_cfg.server_gold_box_total_num_limit) || m_other_cfg.server_gold_box_total_num_limit <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "role_title_drop_daily_num_limit", m_other_cfg.role_title_drop_daily_num_limit) || m_other_cfg.role_title_drop_daily_num_limit <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "role_mount_drop_daily_num_limit", m_other_cfg.role_mount_drop_daily_num_limit) || m_other_cfg.role_mount_drop_daily_num_limit <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "role_gold_box_item_daily_num_limit", m_other_cfg.role_gold_box_item_daily_num_limit) || m_other_cfg.role_gold_box_item_daily_num_limit <= 0)
	{
		return -14;
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::InitServerTitleDropLimitCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1001;
	}

	m_server_title_drop_total_weight = 0;

	int i = 0;

	while (!data_element.empty())
	{
		if (i >= CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX)
		{
			return -100;
		}

		CrossXiuluoTowerDropLimitConfig &drop_limit_cfg = m_server_title_drop_limit_cfg_list[i];

		if (!PugiGetSubNodeValue(data_element, "drop_num_limit", drop_limit_cfg.drop_num_limit) || drop_limit_cfg.drop_num_limit <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", drop_limit_cfg.weight) || drop_limit_cfg.weight <= 0)
		{
			return -2;
		}

		m_server_title_drop_total_weight += drop_limit_cfg.weight;
		++i;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::InitServerMountDropLimitCfg( PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1001;
	}

	m_server_title_drop_total_weight = 0;

	int i = 0;

	while (!data_element.empty())
	{
		if (i >= CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX)
		{
			return -100;
		}

		CrossXiuluoTowerDropLimitConfig &drop_limit_cfg = m_server_mount_drop_limit_cfg_list[i];

		if (!PugiGetSubNodeValue(data_element, "drop_num_limit", drop_limit_cfg.drop_num_limit) || drop_limit_cfg.drop_num_limit <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", drop_limit_cfg.weight) || drop_limit_cfg.weight <= 0)
		{
			return -2;
		}

		m_server_mount_drop_total_weight += drop_limit_cfg.weight;
		++i;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::InitServerGoldBoxDropLimitCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1001;
	}

	m_server_title_drop_total_weight = 0;

	int i = 0;

	while (!data_element.empty())
	{
		if (i >= CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX)
		{
			return -100;
		}

		CrossXiuluoTowerDropLimitConfig &drop_limit_cfg = m_server_goldbox_drop_limit_cfg_list[i];

		if (!PugiGetSubNodeValue(data_element, "drop_num_limit", drop_limit_cfg.drop_num_limit) || drop_limit_cfg.drop_num_limit <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", drop_limit_cfg.weight) || drop_limit_cfg.weight <= 0)
		{
			return -2;
		}

		m_server_glodbox_drop_total_weight += drop_limit_cfg.weight;
		++i;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::InitXiuluoTowerRankTitle(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1001;
	}
	
	while (!data_element.empty())
	{
		int rank = -1;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank < 0 || rank > CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM)
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(data_element, "title_id", m_xiuluotower_rank_title[rank - 1]) || m_xiuluotower_rank_title[rank - 1] <= 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::InitXiuluoTowerGatherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1001;
	}

	while (!dataElement.empty())
	{
		if (m_xiuluo_tower_gather_cfg.gather_count <0 || m_xiuluo_tower_gather_cfg.gather_count >= CROSS_XIULUO_TOWER_GATER_ITEM_NUM_MAX)
		{
			return -1;
		}

		CrossXiuluoTowerGatherCfg::GatherItem &gather_item = m_xiuluo_tower_gather_cfg.gather_list[m_xiuluo_tower_gather_cfg.gather_count];
		if (!PugiGetSubNodeValue(dataElement, "gather_index", gather_item.gather_index) || gather_item.gather_index < 0 || gather_item.gather_index != m_xiuluo_tower_gather_cfg.gather_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather_item.gather_id) || gather_item.gather_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time", gather_item.gather_time) || gather_item.gather_time <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", gather_item.pos_x) || gather_item.pos_x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", gather_item.pos_y) || gather_item.pos_y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "disappear_s", gather_item.disappear_s) || gather_item.disappear_s < 0)
		{
			return -7;
		}
		m_xiuluo_tower_gather_cfg.gather_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossXiuluoTowerCommonConfig::GetLayerSceneId(int layer, Posi *enter_pos)
{
	if (layer < 0 || layer >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER) return 0;

	if (NULL != enter_pos) *enter_pos = this->GetRealivePos(layer);

	return m_layer_list[layer].scene_id;
}

int CrossXiuluoTowerCommonConfig::GetSceneIdStandy(Posi *enter_pos)
{
	 *enter_pos = m_layer_list[0].realive_point_list[0];

	return m_layer_list[0].scene_id;
}

int CrossXiuluoTowerCommonConfig::GetUpLayerNeedKills(int layer)
{
	if (layer < 0 || layer >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
	{
		return 999999;
	}

	return m_layer_list[layer].up_layer_need_kills;
}

Posi CrossXiuluoTowerCommonConfig::GetRealivePos(int layer)
{
	if (layer < 0 || layer >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
	{
		return Posi(0, 0);
	}

	if (0 < m_layer_list[layer].realive_point_count && m_layer_list[layer].realive_point_count < CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
	{
		int rand_index = RandomNum(m_layer_list[layer].realive_point_count);
		return m_layer_list[layer].realive_point_list[rand_index];
	}

	return Posi(0, 0);
}

CrossXiuluoTowerRankRewardCfg::LevelRewardCfg * CrossXiuluoTowerCommonConfig::GetRankRewardItem(int rank_pos, int level)
{
	for (int i = 0; i < m_rank_reward_cfg_count && i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_CFG_MAX; i++)
	{
		if (m_rank_reward_cfg_list[i].min_pos <= rank_pos && rank_pos <= m_rank_reward_cfg_list[i].max_pos)
		{
			for (int j = 0; j < m_rank_reward_cfg_list[i].level_reward_count && j < CROSS_ACTIVITY_XIULUO_TOWER_RANK_REWARD_LEVEL_CFG_MAX; j++)
			{
				if (m_rank_reward_cfg_list[i].level_reward_list[j].min_level <= level && level <= m_rank_reward_cfg_list[i].level_reward_list[j].max_level)
				{
					return &m_rank_reward_cfg_list[i].level_reward_list[j];
				}
			}
		}
	}

	return NULL;
}

bool CrossXiuluoTowerCommonConfig::GetLayerReward(int layer, ItemConfigData *reward_item, int *reward_cross_honor)
{
	if (NULL == reward_item || NULL == reward_cross_honor)
	{
		return false;
	}

	for (int i = m_layer_reward_cfg_count - 1; i >= 0; i--)
	{
		if (m_layer_reward_cfg_list[i].reward_layer <= layer)
		{
			*reward_item = m_layer_reward_cfg_list[i].reward_item;
			*reward_cross_honor = m_layer_reward_cfg_list[i].reward_cross_honor;

			return true;
		}
	}

	return false;
}

bool CrossXiuluoTowerCommonConfig::GetDurDeadBuff(int dur_dead_count, int *add_max_hp_per, int *add_gongji_per)
{
	if (NULL == add_max_hp_per || NULL == add_gongji_per) return false;

	for (int i = m_dur_dead_buff_count - 1; i >= 0; i--)
	{
		if (dur_dead_count >= m_dur_dead_buff_list[i].dur_dead_count)
		{
			*add_max_hp_per = m_dur_dead_buff_list[i].add_max_hp_per;
			*add_gongji_per = m_dur_dead_buff_list[i].add_gongji_per;

			return true;
		}
	}

	return false;
}

bool CrossXiuluoTowerCommonConfig::GetDurKillBuff(int dur_kill_count, int *add_max_hp_per, int *add_gongji_per)
{
	if (NULL == add_max_hp_per || NULL == add_gongji_per) return false;

	for (int i = m_dur_kill_buff_count - 1; i >= 0; i--)
	{
		if (dur_kill_count >= m_dur_kill_buff_list[i].dur_kill_count)
		{
			*add_max_hp_per = m_dur_kill_buff_list[i].add_max_hp_per;
			*add_gongji_per = m_dur_kill_buff_list[i].add_gongji_per;

			return true;
		}
	}

	return false;
}

const ItemConfigData * CrossXiuluoTowerCommonConfig::GetBuffReward()
{
	if (m_reward_cfg.buff_item.num <= 0) return NULL;

	return &m_reward_cfg.buff_item;
}

const CrossXiuluoTowerBossCfg * CrossXiuluoTowerCommonConfig::GetCrossXiuluoTowerBossCfg()
{
	if (m_reward_cfg.boss_cfg.monster_num <= 0) return NULL;

	return &m_reward_cfg.boss_cfg;
}

const CrossXiuluoTowerFirstRewardCfg * CrossXiuluoTowerCommonConfig::GetCrossXiuluoTowerFirstRewardCfg(int layer)
{
	if (layer < 0 || CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER <= layer) return NULL;

	if (m_reward_cfg.first_reward[layer].honour <= 0 &&
		m_reward_cfg.first_reward[layer].item.num <= 0) return NULL;

	return &m_reward_cfg.first_reward[layer];
}

const CrossXiuluoTowerScoreRewardCfg * CrossXiuluoTowerCommonConfig::GetCrossXiuluoTowerScoreRewardCfg(int index)
{
	if (index < 0 || CROSS_ACTIVITY_XIULUO_TOWER_MAX_SCORE_REWARD <= index) return NULL;

	if (m_reward_cfg.score_reward[index].score <= 0) return NULL;

	return &m_reward_cfg.score_reward[index];
}

const CrossXiuluoTowerGatherRewardCfg * CrossXiuluoTowerCommonConfig::GetCrossXiuluoTowerGatherRewardCfg(int gather_id)
{
	std::map<int, CrossXiuluoTowerGatherRewardCfg>::iterator it  = m_reward_cfg.gather_reward_map.find(gather_id);
	if (it == m_reward_cfg.gather_reward_map.end())
	{
		return NULL;
	}

	return &it->second;
}

int CrossXiuluoTowerCommonConfig::GetDropWeightByLayerID(int layer_id)
{
	if (layer_id < 0 || layer_id >= CROSS_ACTIVITY_XIULUO_TOWER_MAX_LAYER)
	{
		return 100;
	}

	return m_drop_weight_list[layer_id];
}

const int CrossXiuluoTowerCommonConfig::GetCrossXiuluoServerTitleDropNumLimit()
{
	if (m_server_title_drop_total_weight <= 0)
	{
		return 1;
	}

	int rand_val = RandomNum(m_server_title_drop_total_weight);

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX; ++i)
	{
		if (rand_val < m_server_title_drop_limit_cfg_list[i].weight)
		{
			return  m_server_title_drop_limit_cfg_list[i].drop_num_limit;
		}

		rand_val -= m_server_title_drop_limit_cfg_list[i].weight;
	}

	return 1;
}

const int CrossXiuluoTowerCommonConfig::GetCrossXiuluoServerMountDropNumLimit()
{
	if (m_server_mount_drop_total_weight <= 0)
	{
		return 1;
	}

	int rand_val = RandomNum(m_server_mount_drop_total_weight);

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX; ++i)
	{
		if (rand_val < m_server_mount_drop_limit_cfg_list[i].weight)
		{
			return  m_server_mount_drop_limit_cfg_list[i].drop_num_limit;
		}

		rand_val -= m_server_mount_drop_limit_cfg_list[i].weight;
	}

	return 1;
}

const int CrossXiuluoTowerCommonConfig::GetCrossXiuluoServerGoldBoxDropNumLimit()
{
	if (m_server_glodbox_drop_total_weight <= 0)
	{
		return 1;
	}

	int rand_val = RandomNum(m_server_glodbox_drop_total_weight);

	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LIMIT_CFG_COUNT_MAX; ++i)
	{
		if (rand_val < m_server_goldbox_drop_limit_cfg_list[i].weight)
		{
			return  m_server_goldbox_drop_limit_cfg_list[i].drop_num_limit;
		}

		rand_val -= m_server_goldbox_drop_limit_cfg_list[i].weight;
	}

	return 1;
}

const int CrossXiuluoTowerCommonConfig::GetXiuluoTowerRankTitleId(int rank)
{
	if (rank <= 0 || rank > CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM)
	{
		return 0;
	}
	return m_xiuluotower_rank_title[rank - 1];
}