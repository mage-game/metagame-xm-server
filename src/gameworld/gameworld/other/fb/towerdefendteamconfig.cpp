#include "towerdefendteamconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "scene/scene.h"
#include <set>

#include <string>
#include "item/itempool.h"

TeamTowerDefendConfig::TeamTowerDefendConfig() : m_wave_refresh_start_delay(0), m_wave_refresh_interval(0),
	m_life_tower_monster_id(0), m_team_life_tower_monster_id(0), m_warning_interval_s(0), m_sync_interval_s(0)
{
	UNSTD_STATIC_CHECK(sizeof(m_warning_percent_list) / sizeof(m_warning_percent_list[0]) >= 2);

	m_warning_percent_list[0] = TEAM_TOWERDEFEND_WARNING_PERCENT_50;
	m_warning_percent_list[1] = TEAM_TOWERDEFEND_WARNING_PERCENT_10;

	memset(m_skill_up_cost_list, 0, sizeof(m_skill_up_cost_list));
}

TeamTowerDefendConfig::~TeamTowerDefendConfig()
{

}

bool TeamTowerDefendConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TeamTowerDefendConfig", *err);


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

// 	{
// 		// 关卡配置
// 		PugiXmlNode level_element = RootElement.child("level_scene_cfg");
// 		if (level_element.empty())
// 		{
// 			*err = configname + ": no [level_scene_cfg].";
// 			return false;
// 		}
// 
// 		iRet = this->InitLevelCfg(level_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	{
		// 组队塔防波次
		PugiXmlNode wave_element = RootElement.child("team_wave_list");
		if (wave_element.empty())
		{
			*err = configname + ": no [team_wave_list].";
			return false;
		}

		iRet = this->InitTeamWaveCfg(wave_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTeamWaveCfg failed %d", configname.c_str(), iRet);
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
		PugiXmlNode buy_join_times_element = RootElement.child("buy_cost");
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

	return true;
}

int TeamTowerDefendConfig::InitTeamWaveCfg(PugiXmlNode RootElement)
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

	int last_wave = -1;
	std::set<int> monsterid_repeat_checker;

	while ( !dataElement.empty())
	{
		int wave = 0;
		if (!PugiGetSubNodeValue(dataElement, "wave", wave) || wave != last_wave + 1 || wave >= TEAM_TOWERDEFEND_WAVE_COUNT)
		{
			return -2;
		}
		last_wave = wave;

		TeamTowerDefendWaveCfg &wave_cfg = m_other_cfg.team_wave_list[wave] ;
		wave_cfg.wave = wave;

		if (!PugiGetSubNodeValue(dataElement, "next_wave_s", wave_cfg.next_wave_s) || wave_cfg.next_wave_s <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "kill_score", wave_cfg.kill_score) || wave_cfg.kill_score <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "sweep_calue", wave_cfg.sweep_calue) || wave_cfg.sweep_calue < 0)
		{
			return -5;
		}

		{
			char tmp_tag[32];
			for (int i = TEAM_TOWERDEFEND_PATH_LEFT; i < TEAM_TOWERDEFEND_PATH_MAX; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_id_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_cfg.monster_id_list[i]) || wave_cfg.monster_id_list[i] < 0)
				{
					return -20;
				}

				if (wave_cfg.monster_id_list[i] > 0)
				{
					if (!MONSTERPOOL->IsMonsterExist(wave_cfg.monster_id_list[i]))
					{
						return -21;
					}

					if (monsterid_repeat_checker.end() != monsterid_repeat_checker.find(wave_cfg.monster_id_list[i]))
					{
						return -22;
					}
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
				if (wave_cfg.monster_id_list[i] == 0 && wave_cfg.monster_count_list[i] > 0)
				{
					return -27;
				}

				if (wave_cfg.monster_count_list[i] * m_wave_refresh_interval >= wave_cfg.next_wave_s)
				{
					return -28;
				}

				wave_cfg.total_monster_count += wave_cfg.monster_count_list[i];

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "angry_value%d", i);
				int angry_value = 0;
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, angry_value) || angry_value < 0)
				{
					return -29;
				}

				if (angry_value > 0 && wave_cfg.monster_id_list[i] <= 0)
				{
					return -30;
				}

				if (angry_value > 0)
				{
					std::map<int, int>::iterator itr = m_monster_angry_value.find(wave_cfg.monster_id_list[i]);
					if (m_monster_angry_value.end() != itr)
					{
						if (itr->second != angry_value)
						{
							return -31;
						}
					}
					else
					{
						m_monster_angry_value.insert(std::make_pair(wave_cfg.monster_id_list[i], angry_value));
					}
				}
			}
		}

		//掉落包
		{
			// 读取跳层的掉落包配置
			PugiXmlNode dropidListElement = dataElement.child("dropid_list");
			if (dropidListElement.empty())
			{
				return -300000;
			}

			PugiXmlNode dropidElement = dropidListElement.child("dropid");
			if (dropidElement.empty())
			{
				return -300001;
			}

			while (!dropidElement.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropidElement, dropid))
				{
					return -102;
				}

				if (0 != dropid)
				{
					if (NULL == DROPPOOL->GetDropConfig(dropid))
					{
						return -103;
					}

					wave_cfg.dropid_list.push_back(dropid);
					if ((int)wave_cfg.dropid_list.size() >= MonsterInitParam::DROP_LIST_MAX_COUNT)
					{
						return -104;
					}
				}

				dropidElement = dropidElement.next_sibling();
			}
		}

		for (int i = TEAM_TOWERDEFEND_PATH_LEFT; i < TEAM_TOWERDEFEND_PATH_MAX; ++i)
		{
			monsterid_repeat_checker.insert(wave_cfg.monster_id_list[i]);
		}

		++ m_other_cfg.team_wave_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int TeamTowerDefendConfig::InitLevelCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level != last_level + 1 || level >= TeamTowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}
		last_level = level;

		TeamTowerDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
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

		++ m_level_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TeamTowerDefendConfig::InitPathCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int path = 0;
		if (!PugiGetSubNodeValue(dataElement, "path_idx", path) || path <= TEAM_TOWERDEFEND_PATH_INVALID || path >= TEAM_TOWERDEFEND_PATH_MAX)
		{
			return -2;
		}

		{
			char tmp_tag[32];
			for (int i = 0; i < TEAM_TOWERDEFEND_PATH_POINT_COUNT; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_x_%d", i + 1);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_pathway_list[path][i].x) || wave_pathway_list[path][i].x < 0)
				{
					return -10;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_y_%d", i + 1);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, wave_pathway_list[path][i].y) || wave_pathway_list[path][i].y < 0)
				{
					return -11;
				}
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TeamTowerDefendConfig::InitOtherCfg(PugiXmlNode RootElement)
{
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

			if (!PugiGetSubNodeValue(dataElement, "team_life_tower_monster_id", m_team_life_tower_monster_id) || m_team_life_tower_monster_id <= 0)
			{
				return -6;
			}
			if (!MONSTERPOOL->IsMonsterExist(m_team_life_tower_monster_id))
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

		if (!PugiGetSubNodeValue(dataElement, "team_enter_sceneid", m_other_cfg.team_enter_sceneid) || m_other_cfg.team_enter_sceneid <= 0)
		{
			return -17;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.team_enter_sceneid);

		if (!PugiGetSubNodeValue(dataElement, "team_birth_pos_x", m_other_cfg.team_enter_pos.x) || m_other_cfg.team_enter_pos.x <= 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "team_birth_pos_y", m_other_cfg.team_enter_pos.y) || m_other_cfg.team_enter_pos.y <= 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_double_reward_gold", m_other_cfg.buy_double_reward_need_gold) || m_other_cfg.buy_double_reward_need_gold < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_triple_reward_gold", m_other_cfg.buy_triple_reward_need_gold) || m_other_cfg.buy_triple_reward_need_gold < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(dataElement, "double_reward_need_vip_level", m_other_cfg.double_reward_need_vip_level) || m_other_cfg.double_reward_need_vip_level < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "triple_reward_need_vip_level", m_other_cfg.triple_reward_need_vip_level) || m_other_cfg.triple_reward_need_vip_level < 0)
		{
			return -23;
		}

		//额外掉落
		{
			PugiXmlNode dropidListElement = dataElement.child("dropid_list");
			if (dropidListElement.empty())
			{
				return -300000;
			}

			PugiXmlNode dropidElement = dropidListElement.child("dropid");
			if (dropidElement.empty())
			{
				return -300001;
			}

			while (!dropidElement.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropidElement, dropid))
				{
					return -102;
				}

				if (0 != dropid)
				{
					if (NULL == DROPPOOL->GetDropConfig(dropid))
					{
						return -103;
					}

					m_other_cfg.dropid_list.push_back(dropid);
					if ((int)m_other_cfg.dropid_list.size() >= MonsterInitParam::DROP_LIST_MAX_COUNT)
					{
						return -104;
					}
				}

				dropidElement = dropidElement.next_sibling();
			}
		}
	}

	return 0;
}

int TeamTowerDefendConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	std::set<int> skillid_repeat_checker;
	std::set<int> skill_seq_repeat_checker[TEAM_TOWERDEFEND_ATTRTYPE_MAX];

	while (!dataElement.empty())
	{
		TeamTowerDefendSkillCfg::ConfigItem cfg;

		if (!PugiGetSubNodeValue(dataElement, "skill_index", cfg.skill_index) || cfg.skill_index < 0 || cfg.skill_index >= TeamTowerDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
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

		if (!PugiGetSubNodeValue(dataElement, "param_d", cfg.param_d))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost", cfg.energy_cost) || cfg.energy_cost < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "hurt_percent", cfg.hurt_percent) || cfg.hurt_percent < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "fix_hurt", cfg.fix_hurt) || cfg.fix_hurt < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_factor", cfg.cd_factor) || cfg.cd_factor < 0)
		{
			return -14;
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "attr_type", cfg.attr_type) || cfg.attr_type < TEAM_TOWERDEFEND_ATTRTYPE_INVALID || cfg.attr_type >= TEAM_TOWERDEFEND_ATTRTYPE_MAX)
			{
				return -100;
			}

			if (!PugiGetSubNodeValue(dataElement, "skill_seq", cfg.skill_seq) || cfg.skill_seq < 0 || cfg.skill_seq >= TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT)
			{
				return -101;
			}

			if (!PugiGetSubNodeValue(dataElement, "skill_id", cfg.skill_id) || cfg.skill_id < 0 || cfg.skill_id > TEAM_TOWERDEFEND_MAX_SKILL_COUNT)
			{
				return -102;
			}

			if (TEAM_TOWERDEFEND_ATTRTYPE_INVALID == cfg.attr_type)
			{
				if (0 != cfg.skill_id)
				{
					return -103;
				}
			}
			if (cfg.attr_type > TEAM_TOWERDEFEND_ATTRTYPE_INVALID && cfg.attr_type < TEAM_TOWERDEFEND_ATTRTYPE_MAX)
			{
				if (skill_seq_repeat_checker[cfg.attr_type].end() != skill_seq_repeat_checker[cfg.attr_type].find(cfg.skill_seq))
				{
					return -104;
				}
				skill_seq_repeat_checker[cfg.attr_type].insert(cfg.skill_seq);

				if (skillid_repeat_checker.end() != skillid_repeat_checker.find(cfg.skill_id))
				{
					return -105;
				}
				skillid_repeat_checker.insert(cfg.skill_id);

				{
					UNSTD_STATIC_CHECK(sizeof(m_team_skill_base_list) / sizeof(m_team_skill_base_list[0]) == TEAM_TOWERDEFEND_MAX_SKILL_COUNT+1);
					UNSTD_STATIC_CHECK(TEAM_TOWERDEFEND_MAX_SKILL_LEVEL <= MAX_SKILL_LEVEL);

					TeamTowerDefendSkill &skill_base = m_team_skill_base_list[cfg.skill_id];
					UInt16 scene_skill_id = static_cast<UInt16>(GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_TEAM_TOWERDEFEND, cfg.skill_id));
					skill_base.SetSkillID(scene_skill_id);
					skill_base.SetSkillCfgID(cfg.skill_id);
					skill_base.SetAttackDis(cfg.distance);

					for (int i = 1; i <= TEAM_TOWERDEFEND_MAX_SKILL_LEVEL; ++i)
					{
						skill_base.SetParamA(cfg.param_a, i);
						skill_base.SetParamB(cfg.param_b, i);
						skill_base.SetParamC(cfg.param_c, i);
						skill_base.SetParamD(cfg.param_d, i);
						skill_base.SetCD(cfg.cd_s - i * cfg.cd_factor, i);
						skill_base.SetHurtPercent(cfg.hurt_percent, i);
						skill_base.SetFixHurt(cfg.fix_hurt, i);
					}
				}
			}
		}

		m_skill_cfg.cfg_list[cfg.skill_index] = cfg;

		++ count;

		dataElement = dataElement.next_sibling();
	}

	if (count != TeamTowerDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
	{
		return -50;
	}

	for (int i = TEAM_TOWERDEFEND_ATTRTYPE_INVALID + 1; i < TEAM_TOWERDEFEND_ATTRTYPE_MAX; ++ i)
	{
		if (TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT != (int)skill_seq_repeat_checker[i].size())
		{
			return -60;
		}
	}

	return 0;
}

int TeamTowerDefendConfig::InitBuyJoinTimesCfg(PugiXmlNode RootElement)
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
		if (m_buy_join_times_cfg.cfg_count >= TeamTowerDefendBuyJoinTimesCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		TeamTowerDefendBuyJoinTimesCfg::ConfigItem &cfg_item = m_buy_join_times_cfg.cfg_list[m_buy_join_times_cfg.cfg_count];

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

int TeamTowerDefendConfig::InitSkillLearnCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int skill_id = 0;
		int skill_level = 0;
		int score_cost = 0;

		if (!PugiGetSubNodeValue(dataElement, "skill_index", skill_id) || skill_id <= 0 || skill_id >= TEAM_TOWERDEFEND_MAX_SKILL_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level <= 0 || skill_level > TEAM_TOWERDEFEND_MAX_SKILL_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "score_cost", score_cost) || score_cost < 0)
		{
			return -3;
		}

		m_skill_up_cost_list[skill_id][skill_level] = score_cost;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TeamTowerDefendConfig::GetWaveCountBySceneID(int scene_id) const
{
	const TeamTowerDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return 0;
	}

	return cfg_item->wave_count;
}

const TeamTowerDefendWaveCfg * TeamTowerDefendConfig::GetWaveCfg(int scene_id, int wave) const
{
	if (scene_id <= 0 || wave < 0 || wave >= TEAM_TOWERDEFEND_WAVE_COUNT)
	{
		return NULL;
	}

	const TeamTowerDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL != cfg_item)
	{
		if (wave < cfg_item->wave_count)
		{
			return &cfg_item->wave_list[wave];
		}
	}

	return NULL;
}

const TeamTowerDefendWaveCfg * TeamTowerDefendConfig::GetWaveCfgByMonsterID(int scene_id, int monster_id) const
{
	if (monster_id <= 0 || scene_id <= 0)
	{
		return NULL;
	}

	const TeamTowerDefendLevelCfg::ConfigItem *cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_item->wave_count; ++ i)
	{
		const TeamTowerDefendWaveCfg *wave_cfg = &cfg_item->wave_list[i];

		for (int j = 0; j < TEAM_TOWERDEFEND_PATH_MAX; ++ j)
		{
			if (wave_cfg->monster_id_list[j] == monster_id)
			{
				return wave_cfg;
			}
		}
	}

	return NULL;
}

const TeamTowerDefendWaveCfg * TeamTowerDefendConfig::GetTeamWaveCfg(int wave) const
{
	if (wave >= 0 && wave < m_other_cfg.team_wave_count)
	{
		return &m_other_cfg.team_wave_list[wave];
	}

	return NULL;
}

const TeamTowerDefendWaveCfg * TeamTowerDefendConfig::GetTeamWaveCfgByMonsterID(int monster_id) const
{
	for (int wave = 0; wave < m_other_cfg.team_wave_count; ++ wave)
	{
		const TeamTowerDefendWaveCfg *wave_cfg = &m_other_cfg.team_wave_list[wave];

		for (int path_idx = 0; path_idx < TEAM_TOWERDEFEND_PATH_MAX; ++ path_idx)
		{
			if (wave_cfg->monster_id_list[path_idx] == monster_id)
			{
				return wave_cfg;
			}
		}
	}

	return NULL;
}

int TeamTowerDefendConfig::GetMonsterAngryValue(int monster_id)
{
	std::map<int, int>::iterator itr = m_monster_angry_value.find(monster_id);
	if (itr != m_monster_angry_value.end())
	{
		return itr->second;
	}
	return 0;
}
Posi TeamTowerDefendConfig::GetBirthPos(int path) const
{
	if (path <= TEAM_TOWERDEFEND_PATH_INVALID || path >= TEAM_TOWERDEFEND_PATH_MAX) 
	{
		return wave_pathway_list[TEAM_TOWERDEFEND_PATH_MID][0];
	}

	return wave_pathway_list[path][0];
}

bool TeamTowerDefendConfig::GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos)
{
	UNSTD_STATIC_CHECK(TEAM_TOWERDEFEND_PATH_MAX > 0);

	birth_path = birth_path % TEAM_TOWERDEFEND_PATH_MAX;

	Posi last_posi = wave_pathway_list[birth_path][TEAM_TOWERDEFEND_PATH_POINT_COUNT - 1];

	for (int count_i = 0; count_i < TEAM_TOWERDEFEND_PATH_MAX; count_i++)
	{
		int i = (count_i + birth_path) % TEAM_TOWERDEFEND_PATH_MAX;

		bool is_find = false;
		for (int j = 0; j < TEAM_TOWERDEFEND_PATH_POINT_COUNT - 1; j++)
		{
			Posi pos1 = wave_pathway_list[i][j], pos2 = wave_pathway_list[i][j+1];
			Posi deltaPos = pos2 - pos1;

			float dir = atan2((float)deltaPos.y, (float)deltaPos.x);
			dir = -dir;
			float distance = sqrt((float)(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y));

			Posi offofset_pos = curr_pos - pos1;

			float x = offofset_pos.x * cos(dir) - offofset_pos.y * sin(dir);
			float y = offofset_pos.x * sin(dir) + offofset_pos.y * cos(dir);
			if (x >= -2.0f && x <= distance + 2.0f && y >= -20.0f && y <= 20.0f)
			{
				is_find = true;
				if (NULL != pos) *pos = pos2;
			}
		}
		if (is_find) return true;
	}

	if (NULL != pos) *pos = last_posi;

	return false;
}

bool TeamTowerDefendConfig::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < TEAM_TOWERDEFEND_WARNING_PERCENT_COUNT; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (NULL != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

const TeamTowerDefendLevelCfg::ConfigItem * TeamTowerDefendConfig::GetLevelCfgBySceneID(int scene_id) const
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < TeamTowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (m_level_cfg.cfg_list[i].scene_id == scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const TeamTowerDefendLevelCfg::ConfigItem * TeamTowerDefendConfig::GetLevelCfg(int level) const
{
	if (level < 0 || level >= TeamTowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[level];
}

const TeamTowerDefendSkillCfg::ConfigItem * TeamTowerDefendConfig::GetSkillCfg(int skill_index)
{
	if (skill_index < 0 || skill_index >= TeamTowerDefendSkillCfg::CONFIG_ITEM_MAX_COUNT)
	{
		return NULL;
	}

	return &m_skill_cfg.cfg_list[skill_index];
}

int TeamTowerDefendConfig::GetBuyJoinTimesCfg(int buy_times)
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

const TeamTowerDefendSkillCfg::ConfigItem * TeamTowerDefendConfig::GetTeamSkillCfg(int skill_id)
{
	for (int i = 0; i < TeamTowerDefendSkillCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (m_skill_cfg.cfg_list[i].attr_type > TEAM_TOWERDEFEND_ATTRTYPE_INVALID && m_skill_cfg.cfg_list[i].attr_type < TEAM_TOWERDEFEND_ATTRTYPE_MAX)
		{
			if (m_skill_cfg.cfg_list[i].skill_id == skill_id)
			{
				return &m_skill_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}

void TeamTowerDefendConfig::GetTeamSkillListByAttr(int attr_type, int skill_id_list[TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT]) const
{
	for (int i = 0; i < TeamTowerDefendSkillCfg::CONFIG_ITEM_MAX_COUNT; ++i)
	{
		if (attr_type == m_skill_cfg.cfg_list[i].attr_type)
		{
			if (m_skill_cfg.cfg_list[i].skill_seq >= 0 && m_skill_cfg.cfg_list[i].skill_seq < TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT)
			{
				skill_id_list[m_skill_cfg.cfg_list[i].skill_seq] = m_skill_cfg.cfg_list[i].skill_id;
			}
		}
	}
}

int TeamTowerDefendConfig::GetUpTeamSkillCost(int skill_id, int skill_level) const
{
	if (skill_id > 0 && skill_id < TEAM_TOWERDEFEND_MAX_SKILL_COUNT) 
	{
		if (skill_level >= 1 && skill_level <= TEAM_TOWERDEFEND_MAX_SKILL_LEVEL)
		{
			return m_skill_up_cost_list[skill_id][skill_level];
		}
	}

	return 9999999;
}

const TeamTowerDefendSkill * TeamTowerDefendConfig::GetTeamSkillBase(int skill_id) const
{
	if (skill_id <= 0 || skill_id > TEAM_TOWERDEFEND_MAX_SKILL_COUNT) return NULL;

	return &m_team_skill_base_list[skill_id];
}

bool TeamTowerDefendConfig::CheckLevelMonsterID(const TeamTowerDefendLevelCfg::ConfigItem &cfg_item)
{
	std::set<int> monster_id_list;

	for (int j = 0; j < cfg_item.wave_count; ++ j)
	{
		const TeamTowerDefendWaveCfg &wave_cfg = cfg_item.wave_list[j];
		std::set<int> cur_wave_monster_id_list;

		for (int k = 0; k < TEAM_TOWERDEFEND_PATH_MAX; ++k)
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

