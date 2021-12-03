#include "armordefendconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "scene/scene.h"
#include "task/taskpool.h"
#include <set>

#include <string>

ArmorDefendConfig::ArmorDefendConfig() : m_wave_refresh_start_delay(0), m_wave_refresh_interval(0),
	m_life_tower_monster_id(0), m_team_life_tower_monster_id(0), m_warning_interval_s(0), m_sync_interval_s(0)
{
	UNSTD_STATIC_CHECK(sizeof(m_warning_percent_list) / sizeof(m_warning_percent_list[0]) >= 2);

	m_warning_percent_list[0] = ARMORDEFEND_WARNING_PERCENT_50;
	m_warning_percent_list[1] = ARMORDEFEND_WARNING_PERCENT_10;

}

ArmorDefendConfig::~ArmorDefendConfig()
{

}

bool ArmorDefendConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ArmorDefendConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 关卡配置
		PugiXmlNode level_element = RootElement.child("level_scene_cfg");
		if (NULL == level_element)
		{
			*err = configname + ": no [level_scene_cfg].";
			return false;
		}

		iRet = this->InitLevelCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 新手波次
		PugiXmlNode wave_element = RootElement.child("new_player_wave_list");
		if (wave_element.empty())
		{
			*err = configname + ": no [new_player_wave_list].";
			return false;
		}

		iRet = this->InitNewPlayerWaveCfg(wave_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitNewPlayerWaveCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 波次
		PugiXmlNode wave_element = RootElement.child("wave_list");
		if (wave_element.empty())
		{
			*err = configname + ": no [wave_list].";
			return false;
		}

		iRet = this->InitWaveCfg(wave_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWaveCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 路线
		PugiXmlNode path_element = RootElement.child("path_list");
		if (path_element.empty())
		{
			*err = configname + ": no [path_list].";
			return false;
		}

		iRet = this->InitPathCfg(path_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPathCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能
		PugiXmlNode skill_element = RootElement.child("skill_cfg");
		if (skill_element.empty())
		{
			*err = configname + ": no [skill_cfg].";
			return false;
		}

		iRet = this->InitSkillCfg(skill_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 购买参与次数
		PugiXmlNode  buy_join_times_element = RootElement.child("buy_cost");
		if (buy_join_times_element.empty())
		{
			*err = configname + ": no [buy_cost].";
			return false;
		}

		iRet = this->InitBuyJoinTimesCfg(buy_join_times_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBuyJoinTimesCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	{
		//杀怪获取能量
		PugiXmlNode energy_element = RootElement.child("get_energy");
		if (energy_element.empty())
		{
			*err = configname + ": no [get_energy].";
			return false;
		}

		iRet = this->InitGetEnergyCfg(energy_element);
		if(iRet)
		{
			sprintf(errinfo, "%s: InitGetEnergyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ArmorDefendConfig::InitNewPlayerWaveCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int last_wave_list[ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT];
	for (int i = 0; i < ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++i)
	{
		last_wave_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
		{
			return -1;
		}

		ArmorDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		int &last_wave = last_wave_list[level];

		int wave = 0;
		if (!PugiGetSubNodeValue(dataElement, "wave", wave) || wave != last_wave + 1 || wave >= PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL)
		{
			return -2;
		}
		last_wave = wave;

		ArmorDefendWaveCfg &wave_cfg = cfg_item.new_player_wave_list[wave];
		wave_cfg.wave = wave;

		if (!PugiGetSubNodeValue(dataElement, "next_wave_s", wave_cfg.next_wave_s) || wave_cfg.next_wave_s <= 0)
		{
			return -3;
		}

		{
			char tmp_tag[32];
			for (int i = ARMORDEFEND_PATH_LEFT; i < ARMORDEFEND_PATH_MAX; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_id_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_cfg.monster_id_list[i]) || wave_cfg.monster_id_list[i] < 0)
				{
					return -20;
				}

				if (wave_cfg.monster_id_list[i] > 0 && !MONSTERPOOL->IsMonsterExist(wave_cfg.monster_id_list[i]))
				{
					return -21;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_count_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_cfg.monster_count_list[i]) || wave_cfg.monster_count_list[i] < 0)
				{
					return -25;
				}

				if (wave_cfg.monster_id_list[i] > 0 && 0 == wave_cfg.monster_count_list[i])
				{
					return -26;
				}

				if (wave_cfg.monster_count_list[i] * m_wave_refresh_interval >= wave_cfg.next_wave_s)
				{
					return -27;
				}

				wave_cfg.total_monster_count += wave_cfg.monster_count_list[i];
			}
		}

		++cfg_item.new_player_wave_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ArmorDefendConfig::InitWaveCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int last_wave_list[ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT];
	for (int i = 0; i < ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		last_wave_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
		{
			return -1;
		}

		ArmorDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		int &last_wave = last_wave_list[level];

		int wave = 0;
		if (!PugiGetSubNodeValue(dataElement, "wave", wave) || wave != last_wave + 1 || wave >= PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL)
		{
			return -2;
		}
		last_wave = wave;

		ArmorDefendWaveCfg &wave_cfg = cfg_item.wave_list[wave];
		wave_cfg.wave = wave;

		if (!PugiGetSubNodeValue(dataElement, "next_wave_s", wave_cfg.next_wave_s) || wave_cfg.next_wave_s <= 0)
		{
			return -3;
		}

		{
			char tmp_tag[32];
			for (int i = ARMORDEFEND_PATH_LEFT; i < ARMORDEFEND_PATH_MAX; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_id_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_cfg.monster_id_list[i]) || wave_cfg.monster_id_list[i] < 0)
				{
					return -20;
				}

				if (wave_cfg.monster_id_list[i] > 0 && !MONSTERPOOL->IsMonsterExist(wave_cfg.monster_id_list[i]))
				{
					return -21;
				}

				if (wave_cfg.monster_id_list[i] > 0 && NULL == MONSTERPOOL->GetMonsterParam(wave_cfg.monster_id_list[i]))
				{
					return -22;
				}

				if (wave_cfg.monster_id_list[i] > 0 && MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING != MONSTERPOOL->GetMonsterParam(wave_cfg.monster_id_list[i])->ai_type)
				{
					return -23;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_count_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_cfg.monster_count_list[i]) || wave_cfg.monster_count_list[i] < 0)
				{
					return -25;
				}

				if (wave_cfg.monster_id_list[i] > 0 && 0 == wave_cfg.monster_count_list[i])
				{
					return -26;
				}

				if (wave_cfg.monster_count_list[i] * m_wave_refresh_interval >= wave_cfg.next_wave_s)
				{
					return -27;
				}

				wave_cfg.total_monster_count += wave_cfg.monster_count_list[i];
			}
		}

		++ cfg_item.wave_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ArmorDefendConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	m_level_cfg.cfg_count = 0;

	int last_level = -1;
	std::set<int> sceneid_set;

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level != last_level + 1 || level >= ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}
		last_level = level;

		ArmorDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		cfg_item.level = level;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -3;
		}
		if (sceneid_set.end() != sceneid_set.find(cfg_item.scene_id))
		{
			return -4;
		}
		sceneid_set.insert(cfg_item.scene_id);
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_x", cfg_item.birth_pos.x) || cfg_item.birth_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_y", cfg_item.birth_pos.y) || cfg_item.birth_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg_item.role_level) || cfg_item.role_level <= 0)
		{
			return -7;
		}

		int count = ItemConfigData::ReadConfigList(dataElement, "find_reward", cfg_item.find_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -100 + count;
		}

		++ m_level_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ArmorDefendConfig::InitPathCfg(PugiXmlNode RootElement)
{

	PugiXmlNode dataElement = RootElement.child("data");

	int path = 0;
	int pos_index = 0;
	int last_pos_index = -1;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "path_idx", path) || path <= ARMORDEFEND_PATH_INVALID || path >= ARMORDEFEND_PATH_MAX)
		{
			return -1;
		}

		ArmorDefendMonsterPath & amdpath = wave_pathway_list[path];
		
		if (!PugiGetSubNodeValue(dataElement, "pos_index", pos_index) || pos_index < 0 || pos_index >= ARMORDEFEND_PATH_POINT_MAX)
		{
			return -2;
		}

		if (pos_index != 0 &&pos_index != last_pos_index + 1)
		{
			return -3;
		}
		if (pos_index != amdpath.cfg_count)
		{
			return -4;
		}

		last_pos_index = pos_index;

		if (!PugiGetSubNodeValue(dataElement, "x", amdpath.pos_list[pos_index].x) || amdpath.pos_list[pos_index].x < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "y", amdpath.pos_list[pos_index].y) || amdpath.pos_list[pos_index].y < 0)
		{
			return -6;
		}

		amdpath.cfg_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ArmorDefendConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -1000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "first_wave_s", m_wave_refresh_start_delay) || m_wave_refresh_start_delay <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", m_wave_refresh_interval) || m_wave_refresh_interval <= 0)
		{
			return -2;
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "life_tower_monster_id", m_life_tower_monster_id) || m_life_tower_monster_id <= 0)
			{
				return -6;
			}
			if (!MONSTERPOOL->IsMonsterExist(m_life_tower_monster_id))
			{
				return -7;
			}

		}

		if (!PugiGetSubNodeValue(dataElement, "warning_interval_s", m_warning_interval_s) || m_warning_interval_s <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "sync_interval_s", m_sync_interval_s) || m_sync_interval_s <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_join_times", m_other_cfg.free_join_times) || m_other_cfg.free_join_times < 0 || m_other_cfg.free_join_times > 16)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_fb_empty_grid", m_other_cfg.auto_fb_empty_grid) || m_other_cfg.auto_fb_empty_grid <= 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_item_id", m_other_cfg.auto_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.auto_item_id))
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_item_count", m_other_cfg.auto_item_count) || m_other_cfg.auto_item_count <= 0)
		{
			return -16;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "escape_num_to_failure", m_other_cfg.escape_num_to_failure) || m_other_cfg.escape_num_to_failure < 0)
		{
			return -17;
		}
		if (!PugiGetSubNodeValue(dataElement, "escape_num_to_warn", m_other_cfg.escape_num_to_warn) || m_other_cfg.escape_num_to_warn <= 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "guid_task_id", m_other_cfg.guid_task_id) || m_other_cfg.guid_task_id <= 0 || NULL == TASKPOOL->GetCfgByTaskID(m_other_cfg.guid_task_id))
		{
			return -19;
		}
		if (!PugiGetSubNodeValue(dataElement, "boss_num_to_failure", m_other_cfg.boss_num_to_failure) || m_other_cfg.boss_num_to_failure < 0)
		{
			return -20;
		}

		m_other_cfg.reward_count = ItemConfigData::ReadConfigList(dataElement, "guid_reward_item", m_other_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (m_other_cfg.reward_count < 0)
		{
			return -100 + m_other_cfg.reward_count;
		}
	}

	return 0;
}

int ArmorDefendConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		ArmorDefendSkillCfg::ConfigItem cfg;

		if (!PugiGetSubNodeValue(dataElement, "skill_index", cfg.skill_index) || cfg.skill_index < 0 || cfg.skill_index >= ArmorDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		if (!m_skill_cfg.cfg_list[cfg.skill_index].Invalid())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_s", cfg.cd_s) || cfg.cd_s < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "distance", cfg.distance) || cfg.distance <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "energy_cost", cfg.energy_cost) || cfg.energy_cost < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "hurt_percent", cfg.hurt_percent) || cfg.hurt_percent < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "fix_hurt", cfg.fix_hurt) || cfg.fix_hurt < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_factor", cfg.cd_factor) || cfg.cd_factor < 0)
		{
			return -13;
		}

		m_skill_cfg.cfg_list[cfg.skill_index] = cfg;

		++ count;

		dataElement = dataElement.next_sibling();
	}

	if (count != ArmorDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
	{
		return -50;
	}

	return 0;
}

int ArmorDefendConfig::InitBuyJoinTimesCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_buy_join_times_cfg.cfg_count = 0;

	int last_buy_times = 0;

	while (!data_element.empty())
	{
		if (m_buy_join_times_cfg.cfg_count >= ArmorDefendBuyJoinTimesCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		ArmorDefendBuyJoinTimesCfg::ConfigItem &cfg_item = m_buy_join_times_cfg.cfg_list[m_buy_join_times_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "buy_times", cfg_item.buy_times) || cfg_item.buy_times <= last_buy_times)
		{
			return -1;
		}
		last_buy_times = cfg_item.buy_times;

		if (!PugiGetSubNodeValue(data_element, "gold_cost", cfg_item.gold_cost) || cfg_item.gold_cost <= 0)
		{
			return -2;
		}

		++ m_buy_join_times_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	if (1 != m_buy_join_times_cfg.cfg_list[0].buy_times)
	{
		return -40000;
	}

	return 0;
}

int ArmorDefendConfig::InitGetEnergyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	m_get_energy.clear();
	while (!data_element.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(data_element, "monster_id", monster_id) || monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}
		int energy = 0;

		if (!PugiGetSubNodeValue(data_element, "energy", energy) || energy < 0)
		{
			return -2;
		}
		
		std::map<int, int>::iterator itr= m_get_energy.find(monster_id);
		if (itr != m_get_energy.end())
		{
			return -3;
		}
		m_get_energy.insert(std::make_pair(monster_id, energy));
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ArmorDefendConfig::GetWaveCountBySceneID(int scene_id, bool is_new_player) const
{
	const ArmorDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return 0;
	}

	int wave_count = 0;
	if (is_new_player)
	{
		wave_count = cfg_item->new_player_wave_count;
	}
	else
	{
		wave_count = cfg_item->wave_count;
	}

	return wave_count;
}

const ArmorDefendWaveCfg * ArmorDefendConfig::GetWaveCfg(int scene_id, int wave, bool is_new_player) const
{
	if (scene_id <= 0 || wave < 0 || wave >= PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL)
	{
		return NULL;
	}

	const ArmorDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL != cfg_item)
	{
		if (wave < cfg_item->wave_count)
		{
			if (is_new_player)
			{
				return &cfg_item->new_player_wave_list[wave];
			}
			else
			{
				return &cfg_item->wave_list[wave];
			}
		}
	}

	return NULL;
}

const ArmorDefendWaveCfg * ArmorDefendConfig::GetWaveCfgByMonsterID(int scene_id, int monster_id, bool is_new_player) const
{
	if (monster_id <= 0 || scene_id <= 0)
	{
		return NULL;
	}

	const ArmorDefendLevelCfg::ConfigItem *cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_item->wave_count; ++ i)
	{
		const ArmorDefendWaveCfg *wave_cfg = nullptr;
		if (is_new_player)
		{
			wave_cfg = &cfg_item->new_player_wave_list[i];
		}
		else
		{
			wave_cfg = &cfg_item->wave_list[i];
		}

		for (int j = 0; j < ARMORDEFEND_PATH_MAX; ++ j)
		{
			if (wave_cfg->monster_id_list[j] == monster_id)
			{
				return wave_cfg;
			}
		}
	}

	return NULL;
}

const ArmorDefendMonsterPath* ArmorDefendConfig::GetArmorDefendPathByMonsterID(int scene_id, int monster_id, bool is_new_player) const
{
	if (monster_id <= 0 || scene_id <= 0)
	{
		return NULL;
	}

	const ArmorDefendLevelCfg::ConfigItem *cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_item->wave_count; ++i)
	{
		const ArmorDefendWaveCfg *wave_cfg = nullptr;
		if (is_new_player)
		{
			wave_cfg = &cfg_item->new_player_wave_list[i];
		}
		else
		{
			wave_cfg = &cfg_item->wave_list[i];
		}

		for (int j = 0; j < ARMORDEFEND_PATH_MAX; ++j)
		{
			if (wave_cfg->monster_id_list[j] == monster_id)
			{
				return &wave_pathway_list[j];
			}
		}
	}
	return NULL;
}
Posi ArmorDefendConfig::GetBirthPos(int path) const
{
	if (path <= ARMORDEFEND_PATH_INVALID || path >= ARMORDEFEND_PATH_MAX) 
	{
		return wave_pathway_list[ARMORDEFEND_PATH_MID].pos_list[0];
	}

	return wave_pathway_list[path].pos_list[0];
}

int ArmorDefendConfig::GetArmorDefendPathPosCount(int path) const
{
	if (path <= ARMORDEFEND_PATH_INVALID || path >= ARMORDEFEND_PATH_MAX)
	{
		return wave_pathway_list[ARMORDEFEND_PATH_MID].cfg_count;
	}

	return wave_pathway_list[path].cfg_count;
}
Posi ArmorDefendConfig::GetArmorDefendPathPos(int path, int pos_idx) 
{
	if (path <= ARMORDEFEND_PATH_INVALID || path >= ARMORDEFEND_PATH_MAX ||pos_idx <0 || pos_idx > wave_pathway_list[path].cfg_count)
	{
		return Posi(0,0);
	}

	return wave_pathway_list[path].pos_list[pos_idx];
}


bool ArmorDefendConfig::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < ARMORDEFEND_WARNING_PERCENT_COUNT; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (NULL != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

const ArmorDefendLevelCfg::ConfigItem * ArmorDefendConfig::GetLevelCfgBySceneID(int scene_id) const
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (m_level_cfg.cfg_list[i].scene_id == scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const ArmorDefendLevelCfg::ConfigItem * ArmorDefendConfig::GetLevelCfg(int level) const
{
	if (level < 0 || level >= ArmorDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[level];
}

const ArmorDefendSkillCfg::ConfigItem * ArmorDefendConfig::GetSkillCfg(int skill_index)
{
	if (skill_index < 0 || skill_index >= ArmorDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
	{
		return NULL;
	}

	return &m_skill_cfg.cfg_list[skill_index];
}

int ArmorDefendConfig::GetBuyJoinTimesCfg(int buy_times)
{
	for (int i = m_buy_join_times_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (buy_times >= m_buy_join_times_cfg.cfg_list[i].buy_times)
		{
			return m_buy_join_times_cfg.cfg_list[i].gold_cost;
		}
	}

	return 0;
}

bool ArmorDefendConfig::CheckLevelMonsterID(const ArmorDefendLevelCfg::ConfigItem &cfg_item)
{
	std::set<int> monster_id_list;

	for (int j = 0; j < cfg_item.wave_count; ++ j)
	{
		const ArmorDefendWaveCfg &wave_cfg = cfg_item.wave_list[j];
		std::set<int> cur_wave_monster_id_list;

		for (int k = 0; k < ARMORDEFEND_PATH_MAX; ++k)
		{
			if (wave_cfg.monster_id_list[k] > 0)
			{
				cur_wave_monster_id_list.insert(wave_cfg.monster_id_list[k]);
			}
		}

		if (cur_wave_monster_id_list.size() <= 0)
		{
			return false;
		}

		for (std::set<int>::iterator it = cur_wave_monster_id_list.begin(); cur_wave_monster_id_list.end() != it; ++ it)
		{
			if (monster_id_list.end() != monster_id_list.find(*it))
			{
				return false;
			}

			monster_id_list.insert(*it);
		}
	}

	return true;
}

int ArmorDefendConfig::GetEnergyByMonsterId(int monster_id)
{
	std::map<int, int>::iterator itr = m_get_energy.find(monster_id);
	if (itr != m_get_energy.end())
	{
		return itr->second;
	}
	else
	{
		return 0;
	}
}