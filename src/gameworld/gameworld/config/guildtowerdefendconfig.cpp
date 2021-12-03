#include "guildtowerdefendconfig.hpp"

#include "monster/monsterpool.h"
#include "scene/scene.h"

#include <set>

GuildTowerDefendConfig::GuildTowerDefendConfig() 
	: m_activity_time(0), m_ready_time(0), m_longzhu_monster_id(0), m_longzhu_pos(0, 0), m_refresh_interval(0), m_refresh_add_percent(0), m_dead_kick_count(0),
	m_boss_hurt(0), m_boss_hurt_interval(0), m_tower_defend_direction_cfg_count(0), m_tower_defend_wave_cfg_count(0)
{
	
}

GuildTowerDefendConfig::~GuildTowerDefendConfig()
{

}

bool GuildTowerDefendConfig::Init(const std::string &configname, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "GuildTowerDefendConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + "GuildTowerDefendConfig::Init Error cannot find root node.";
		return false;
	}

	std::set<int> temp_monster_id_set;

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = configname + ": xml node error in [other_config]";
			return false;
		}

		PugiXmlNode DataElement = OtherElement.child("data");
		if (DataElement.empty())
		{
			*err = configname + ": xml node error in [other_config->data]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "activity_time", m_activity_time) || m_activity_time <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->activity_time]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "ready_time", m_ready_time) || m_ready_time < 0)
		{
			*err = configname + ": xml node error in [other_config->data->ready_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "longzhu_monster_id", m_longzhu_monster_id) || !MONSTERPOOL->IsMonsterExist(m_longzhu_monster_id))
		{
			*err = configname + ": xml node error in [other_config->data->longzhu_monster_id]";
			return false;
		}
		temp_monster_id_set.insert(m_longzhu_monster_id);


		if (!PugiGetSubNodeValue(DataElement, "longzhu_pos_x", m_longzhu_pos.x) || m_longzhu_pos.x <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->longzhu_pos_x]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "longzhu_pos_y", m_longzhu_pos.y) || m_longzhu_pos.y <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->longzhu_pos_y]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "refresh_interval", m_refresh_interval) || m_refresh_interval <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->refresh_interval]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "refresh_add_percent", m_refresh_add_percent) || m_refresh_add_percent <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->refresh_add_percent]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "dead_kick_count", m_dead_kick_count) || m_dead_kick_count <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->dead_kick_count]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "boss_hurt", m_boss_hurt) || m_boss_hurt <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->boss_hurt]";
			return false;
		}
		if (!PugiGetSubNodeValue(DataElement, "boss_hurt_interval", m_boss_hurt_interval) || m_boss_hurt_interval <= 0)
		{
			*err = configname + ": xml node error in [other_config->data->boss_hurt_interval]";
			return false;
		}
	}

	{
		PugiXmlNode DirectionElement = RootElement.child("tower_defend_direction");
		if (DirectionElement.empty())
		{
			*err = configname + ": xml node error in [tower_defend_direction].";
			return false; 
		}		

		m_tower_defend_direction_cfg_count = 0;

		PugiXmlNode DataElement = DirectionElement.child("data");
		while (!DataElement.empty())
		{
			if (m_tower_defend_direction_cfg_count >= MAX_TOWER_DEFEND_DIRECTION)
			{
				*err = configname + ": xml node error in [tower_defend_direction->data]. too many";
				return false; 
			}

			GuildTowerDefendDirectionCfg &config = m_tower_defend_direction_cfg_list[m_tower_defend_direction_cfg_count];

			if (!PugiGetSubNodeValue(DataElement, "shouhu_monster_id", config.shouhu_monster_id) || !MONSTERPOOL->IsMonsterExist(config.shouhu_monster_id))
			{
				*err = configname + ": xml node error in [tower_defend_direction->data->shouhu_monster_id].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "shouhu_monster_x", config.shouhu_monster_pos.x) || config.shouhu_monster_pos.x <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_direction->data->shouhu_monster_x].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "shouhu_monster_y", config.shouhu_monster_pos.y) || config.shouhu_monster_pos.y <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_direction->data->shouhu_monster_y].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "refresh_pos_x", config.refresh_pos.x) || config.refresh_pos.x <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_direction->data->refresh_pos_x].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "refresh_pos_y", config.refresh_pos.y) || config.refresh_pos.y <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_direction->data->refresh_pos_y].";
				return false; 
			}

			if (temp_monster_id_set.find(config.shouhu_monster_id) != temp_monster_id_set.end())
			{
				*err = configname + ": xml error monster_id repeat.";
				return false;
			}
			temp_monster_id_set.insert(config.shouhu_monster_id);

			++ m_tower_defend_direction_cfg_count;
			DataElement = DataElement.next_sibling();
		}

		if (MAX_TOWER_DEFEND_DIRECTION != m_tower_defend_direction_cfg_count)
		{
			*err = configname + ": xml node error in [4 != tower_defend_direction_cfg_count].";
			return false;
		}
	}

	{
		PugiXmlNode WaveElement = RootElement.child("tower_defend_wave");
		if (WaveElement.empty())
		{
			*err = configname + ": xml node error in [tower_defend_wave].";
			return false; 
		}		

		m_tower_defend_wave_cfg_count = 0;
		m_monster_wave_map.clear();

		PugiXmlNode DataElement = WaveElement.child("data");
		while (!DataElement.empty())
		{
			if (m_tower_defend_wave_cfg_count >= MAX_TOWER_DEFEND_WAVE)
			{
				*err = configname + ": xml node error in [tower_defend_wave->data]. too many";
				return false; 
			}

			int wave = 0;
			if (!PugiGetSubNodeValue(DataElement, "wave", wave) || wave != m_tower_defend_wave_cfg_count)
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->wave].";
				return false; 
			}

			GuildTowerDefendWaveCfg &config = m_tower_defend_wave_cfg_list[m_tower_defend_wave_cfg_count];

			if (!PugiGetSubNodeValue(DataElement, "monster_id", config.monster_id) || !MONSTERPOOL->IsMonsterExist(config.monster_id))
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->monster_id].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "count", config.count) || config.count <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->count].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "is_boss", config.is_boss))
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->is_boss].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "next_wave_time", config.next_wave_time) || config.next_wave_time <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->next_wave_time].";
				return false; 
			}
			if (!PugiGetSubNodeValue(DataElement, "reward_longhun", config.reward_longhun) || config.reward_longhun <= 0)
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->reward_longhun].";
				return false; 
			}
			/*PugiXmlNode ItemElement = DataElement.child("reward_item");
			if (ItemElement || !config.item.ReadConfig(ItemElement.empty()))
			{
				*err = configname + ": xml node error in [tower_defend_wave->data->reward_item].";
				return false;
			}*/

			if (temp_monster_id_set.find(config.monster_id) != temp_monster_id_set.end())
			{
				*err = configname + ": xml error monster_id repeat.";
				return false;
			}
			temp_monster_id_set.insert(config.monster_id);

			m_monster_wave_map[config.monster_id] = m_tower_defend_wave_cfg_count;

			++ m_tower_defend_wave_cfg_count;
			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode SkillElement = RootElement.child("skill");
		if (SkillElement.empty())
		{
			*err = configname + ": xml node error in [config->skill]";
			return false;
		}

		m_skill_cfg_map.clear();

		PugiXmlNode DataElement = SkillElement.child("data");
		while (!DataElement.empty())
		{
			int skill_id = 0;
			if (!PugiGetSubNodeValue(DataElement, "skill_id", skill_id) || m_skill_cfg_map.find(skill_id) != m_skill_cfg_map.end())
			{
				*err = configname + ": xml node error in [config->skill->data->skill_id]";
				return false;
			}

			GuildTowerDefendSkillCfg &config = m_skill_cfg_map[skill_id];
			config.cfg_skill_id = skill_id;
			config.skill_id = GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_GUILD_STATION, skill_id);

			if (!PugiGetSubNodeValue(DataElement, "distance", config.distance) || config.distance < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->distance]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "range", config.range) || config.range < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->range]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "cd", config.cd) || config.cd < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->cd]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "param_a", config.param_a) || config.param_a < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->param_a]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "param_b", config.param_b) || config.param_b < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->param_b]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "param_c", config.param_c) || config.param_c < 0)
			{
				*err = configname + ": xml node error in [config->skill->data->param_c]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}

	{
		// ²¨Êý½±Àø
		PugiXmlNode ClearWaveElement = RootElement.child("clear_reward");
		if (ClearWaveElement.empty())
		{
			*err = configname + ": xml node error in [config->clear_reward]";
			return false;
		}

		PugiXmlNode DataElement = ClearWaveElement.child("data");
		while (!DataElement.empty())
		{
			int clear_wave = 0;
			if (!PugiGetSubNodeValue(DataElement, "clear_wave_count", clear_wave) || clear_wave <= 0 || clear_wave > MAX_TOWER_DEFEND_WAVE)
			{
				*err = configname + ": xml node error in [config->clear_reward->data->clear_wave_count]";
				return false;
			}

			PugiXmlNode ItemElement = DataElement.child("reward_item");
			if (ItemElement.empty()|| !m_clear_wave_reward_list[clear_wave].ReadConfig(ItemElement))
			{
				*err = configname + ": xml node error in [clear_reward->data->reward_item].";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}

	return true;
}

GuildTowerDefendDirectionCfg * GuildTowerDefendConfig::GetGuildTowerDefendDirectionCfg(int index)
{
	if (index < 0 || index >= m_tower_defend_direction_cfg_count || index >= MAX_TOWER_DEFEND_DIRECTION)
	{
		return NULL;
	}

	return &m_tower_defend_direction_cfg_list[index];
}

GuildTowerDefendWaveCfg * GuildTowerDefendConfig::GetGuildTowerDefendWaveCfg(int wave_index)
{
	if (wave_index < 0 || wave_index >= m_tower_defend_wave_cfg_count || wave_index >= MAX_TOWER_DEFEND_WAVE)
	{
		return NULL;
	}

	return &m_tower_defend_wave_cfg_list[wave_index];
}

int GuildTowerDefendConfig::GetWaveOfMonster(int monster_id)
{
	std::map<int, int>::iterator it = m_monster_wave_map.find(monster_id);
	if (it != m_monster_wave_map.end())
	{
		return it->second;
	}

	return -1;
}

GuildTowerDefendSkillCfg * GuildTowerDefendConfig::GetGuildTowerDefendSkillCfg(int skill_id)
{
	std::map<int, GuildTowerDefendSkillCfg>::iterator it = m_skill_cfg_map.find(skill_id);
	if (it != m_skill_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const ItemConfigData * GuildTowerDefendConfig::GetClearWaveReward(int clear_wave)
{
	if (clear_wave <= 0 || clear_wave > MAX_TOWER_DEFEND_WAVE)
	{
		return NULL;
	}

	if (0 == m_clear_wave_reward_list[clear_wave].item_id)
	{
		return NULL;
	}

	return &m_clear_wave_reward_list[clear_wave];
}

