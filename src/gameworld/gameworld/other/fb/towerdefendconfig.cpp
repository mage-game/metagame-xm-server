#include "towerdefendconfig.hpp"
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

TowerDefendConfig::TowerDefendConfig()
{
	UNSTD_STATIC_CHECK(sizeof(m_warning_percent_list) / sizeof(m_warning_percent_list[0]) >= 2);

	m_warning_percent_list[0] = TOWERDEFEND_WARNING_PERCENT_50;
	m_warning_percent_list[1] = TOWERDEFEND_WARNING_PERCENT_10;

}

TowerDefendConfig::~TowerDefendConfig()
{

}

bool TowerDefendConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TowerDefendConfig", *err);
	
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
			sprintf(errinfo, "%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 关卡配置
		PugiXmlNode level_element = RootElement.child("level_scene_cfg");
		if (level_element.empty())
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

	{
		// 星级配置
		PugiXmlNode buy_join_times_element = RootElement.child("star_level");
		if (buy_join_times_element.empty())
		{
			*err = configname + ": no [star_level].";
			return false;
		}

		iRet = this->InitStarLevelCfg(buy_join_times_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitStarLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// buff掉落配置
		PugiXmlNode buff_drop_weight_element = RootElement.child("buff_drop_weight");
		if (buff_drop_weight_element.empty())
		{
			*err = configname + ": no [buff_drop_weight].";
			return false;
		}

		iRet = this->InitBuffDropWeightCfg(buff_drop_weight_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBuffDropWeightCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// buff配置
		PugiXmlNode buff_element = RootElement.child("buff_config");
		if (buff_element.empty())
		{
			*err = configname + ": no [buff_config].";
			return false;
		}

		iRet = this->InitBuffCfg(buff_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBuffCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 波次
		PugiXmlNode wave_element = RootElement.child("wave_list_test");
		if (wave_element.empty())
		{
			*err = configname + ": no [wave_list_test].";
			return false;
		}

		iRet = this->InitWaveTestCfg(wave_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitWaveTestCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 路线
		PugiXmlNode path_element = RootElement.child("path_list_test");
		if (path_element.empty())
		{
			*err = configname + ": no [path_list_test].";
			return false;
		}

		iRet = this->InitPathTestCfg(path_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPathTestCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int TowerDefendConfig::InitWaveCfg(PugiXmlNode RootElement)
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

	int last_wave_list[TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT];
	for (int i = 0; i < TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++i)
	{
		last_wave_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
		{
			return -1;
		}

		TowerDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		int &last_wave = last_wave_list[level];

		int wave = 0;
		if (!PugiGetSubNodeValue(dataElement, "wave", wave) || wave != last_wave + 1 || wave >= PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL)
		{
			return -2;
		}
		last_wave = wave;

		TowerDefendWaveCfg &wave_cfg = cfg_item.wave_list[wave];
		wave_cfg.wave = wave;

		if (!PugiGetSubNodeValue(dataElement, "next_wave_s", wave_cfg.next_wave_s) || wave_cfg.next_wave_s <= 0)
		{
			return -3;
		}

		{
			char tmp_tag[32];
			for (int i = TOWERDEFEND_PATH_LEFT; i < TOWERDEFEND_PATH_MAX; ++i)
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

				if (wave_cfg.monster_id_list[i] > 0 && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK != MONSTERPOOL->GetMonsterParam(wave_cfg.monster_id_list[i])->ai_type)
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

				wave_cfg.total_monster_count += wave_cfg.monster_count_list[i];
			}
		}

		++ cfg_item.wave_count;

		dataElement = dataElement.next_sibling();
	}

	for (int i = 0; i < m_level_cfg.cfg_count; ++ i)
	{
		if (!this->CheckLevelMonsterID(m_level_cfg.cfg_list[i]))
		{
			return -300000;
		}
	}

	return 0;
}

int TowerDefendConfig::InitLevelCfg(PugiXmlNode RootElement)
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

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level != last_level + 1 || level >= TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}
		last_level = level;

		TowerDefendLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		cfg_item.level = level;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_x", cfg_item.birth_pos.x) || cfg_item.birth_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "birth_pos_y", cfg_item.birth_pos.y) || cfg_item.birth_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg_item.capability) || cfg_item.capability <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg_item.role_level) || cfg_item.role_level <= 0)
		{
			return -8;
		}

		int count = ItemConfigData::ReadConfigList(dataElement, "find_reward", cfg_item.find_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -100 + count;
		}
		
		++m_level_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TowerDefendConfig::InitPathCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int path = 0;
		if (!PugiGetSubNodeValue(dataElement, "path_idx", path) || path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX)
		{
			return -1;
		}

		{
			char tmp_tag[32];
			for (int i = 0; i < TOWERDEFEND_PATH_POINT_COUNT; ++i)
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

int TowerDefendConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "free_join_times", m_other_cfg.free_join_times) || m_other_cfg.free_join_times < 0 || m_other_cfg.free_join_times > 16)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_fb_empty_grid", m_other_cfg.auto_fb_empty_grid) || m_other_cfg.auto_fb_empty_grid <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_cost", m_other_cfg.auto_cost) || m_other_cfg.auto_cost <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_wave_s", m_other_cfg.first_wave_refresh_delay) || m_other_cfg.first_wave_refresh_delay < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", m_other_cfg.wave_refresh_interval) || m_other_cfg.wave_refresh_interval < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "life_tower_monster_id", m_other_cfg.life_tower_monster_id) || m_other_cfg.life_tower_monster_id < 0 || 
			!MONSTERPOOL->IsMonsterExist(m_other_cfg.life_tower_monster_id))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "warning_interval_s", m_other_cfg.warning_interval_s) || m_other_cfg.warning_interval_s < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "sync_interval_s", m_other_cfg.sync_interval_s) || m_other_cfg.sync_interval_s < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "buff_exit_time", m_other_cfg.buff_exit_time) || m_other_cfg.buff_exit_time <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_hp_per", m_other_cfg.add_hp_per) || m_other_cfg.add_hp_per < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_gongji_per", m_other_cfg.add_gongji_per) || m_other_cfg.add_gongji_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_role_gongji_per", m_other_cfg.add_role_gongji_per) || m_other_cfg.add_role_gongji_per < 0)
		{
			return -13;
		}
		if (!PugiGetSubNodeValue(dataElement, "relive_back_home_interval", m_other_cfg.relive_back_home_interval) || m_other_cfg.relive_back_home_interval < 0)
		{
			return -14;
		}
		if (!PugiGetSubNodeValue(dataElement, "relive_gold_interval", m_other_cfg.relive_gold_interval) || m_other_cfg.relive_gold_interval < 0)
		{
			return -15;
		}

		int ret = ItemConfigData::ReadConfigList(dataElement, "first_reward", m_other_cfg.first_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (ret < 0)
		{
			return -100 + ret;
		}

		if (!PugiGetSubNodeValue(dataElement, "life_tower_monster_id_test", m_other_cfg.life_tower_monster_id_test) || m_other_cfg.life_tower_monster_id_test < 0)
		{
			return	-16;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval_test", m_other_cfg.refresh_interval_test) || m_other_cfg.refresh_interval_test < 0)
		{
			return -6;
		}
	}

	return 0;
}

int TowerDefendConfig::InitBuyJoinTimesCfg(PugiXmlNode RootElement)
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
		if (m_buy_join_times_cfg.cfg_count >= TowerDefendBuyJoinTimesCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		TowerDefendBuyJoinTimesCfg::ConfigItem &cfg_item = m_buy_join_times_cfg.cfg_list[m_buy_join_times_cfg.cfg_count];

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

int TowerDefendConfig::InitStarLevelCfg(PugiXmlNode RootElement)
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

	memset(m_pass_star_cfg, 0, sizeof(m_pass_star_cfg));

	int star_level_count = 0;
	int last_death_count = -1;
	while (!dataElement.empty())
	{
		if (star_level_count < 0 ||star_level_count >= TOWERDEFEND_STAR_LEVEL_COUNT)
		{
			return -1;
		}

		TowerDefendStarLevelCfg &cfg = m_pass_star_cfg[star_level_count];

		if (!PugiGetSubNodeValue(dataElement, "death_count", cfg.death_count) || cfg.death_count < 0 || cfg.death_count <= last_death_count)
		{
			return -2;
		}

		last_death_count = cfg.death_count;
		
		if (!PugiGetSubNodeValue(dataElement, "pass_star", cfg.star_level) || cfg.star_level <= 0)
		{
			return -3;
		}

		++star_level_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TowerDefendConfig::InitBuffDropWeightCfg(PugiXmlNode RootElement)
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

	while (!dataElement.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id < 0 || NULL == MONSTERPOOL->GetMonsterParam(monster_id))
		{
			return -1;
		}

		if (m_buff_drop_weight_map.find(monster_id) != m_buff_drop_weight_map.end())
		{
			return -2;
		}

		TowerDefendBuffDropWeightCfg &cfg = m_buff_drop_weight_map[monster_id];
		cfg.monster_id = monster_id;

		char temp_str[32] = { 0 };
		for (int i = 0; i < TOWERDEFEND_BUFF_DROP_WEIGHT_COUNT; i++)
		{
			sprintf(temp_str, "weight_%d", i + 1);
			if (!PugiGetSubNodeValue(dataElement, temp_str, cfg.weight[i]) || cfg.weight[i] < 0)
			{
				return -3;
			}

			cfg.total_weight += cfg.weight[i];
		}

		if (0 >= cfg.total_weight)
		{
			return -4;
		}
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TowerDefendConfig::InitBuffCfg(PugiXmlNode RootElement)
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

	while (!dataElement.empty())
	{
		int buff_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "buff_type", buff_type) || buff_type <= DEFEND_BUff_TYPE_INVALID || buff_type >= DEFEND_BUff_TYPE_MAX)
		{
			return -1;
		}

		if (m_buff_cfg_map.find(buff_type) != m_buff_cfg_map.end())
		{
			return -2;
		}

		TowerDefendBuffCfg &cfg = m_buff_cfg_map[buff_type];
		cfg.buff_type = buff_type;

		char temp_str[32] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			sprintf(temp_str, "param_%d", i);
			if (!PugiGetSubNodeValue(dataElement, temp_str, cfg.param[i]) || cfg.param[i] < 0)
			{
				return -3;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TowerDefendConfig::InitWaveTestCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	while (!dataElement.empty())
	{
		TowerDefendWaveCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "wave", cfg.wave) || cfg.wave < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "next_wave_s", cfg.next_wave_s) || cfg.next_wave_s <= 0)
		{
			return -3;
		}

		{
			char tmp_tag[32];
			for (int i = TOWERDEFEND_PATH_LEFT; i < TOWERDEFEND_PATH_MAX; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_id_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, cfg.monster_id_list[i]) || cfg.monster_id_list[i] < 0)
				{
					return -20;
				}

				if (cfg.monster_id_list[i] > 0 && !MONSTERPOOL->IsMonsterExist(cfg.monster_id_list[i]))
				{
					return -21;
				}

				if (cfg.monster_id_list[i] > 0 && NULL == MONSTERPOOL->GetMonsterParam(cfg.monster_id_list[i]))
				{
					return -22;
				}

				if (cfg.monster_id_list[i] > 0 && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK != MONSTERPOOL->GetMonsterParam(cfg.monster_id_list[i])->ai_type)
				{
					return -23;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_count_%d", i);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, cfg.monster_count_list[i]) || cfg.monster_count_list[i] < 0)
				{
					return -25;
				}

				if (cfg.monster_id_list[i] > 0 && 0 == cfg.monster_count_list[i])
				{
					return -26;
				}

				cfg.total_monster_count += cfg.monster_count_list[i];
			}
		}
		
		m_wave_cfg_test.emplace_back(cfg);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int TowerDefendConfig::InitPathTestCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int path = 0;
		if (!PugiGetSubNodeValue(dataElement, "path_idx", path) || path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX)
		{
			return -1;
		}

		{
			char tmp_tag[32];
			for (int i = 0; i < TOWERDEFEND_PATH_POINT_COUNT; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_x_%d", i + 1);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, m_wave_pathway_list_test[path][i].x) || m_wave_pathway_list_test[path][i].x < 0)
				{
					return -10;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_y_%d", i + 1);
				if (!PugiGetSubNodeValue(dataElement, tmp_tag, m_wave_pathway_list_test[path][i].y) || m_wave_pathway_list_test[path][i].y < 0)
				{
					return -11;
				}
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

void TowerDefendConfig::GetFirstReward(ItemConfigData* first_reward, int num)const
{
	int get_num = num < MAX_ATTACHMENT_ITEM_NUM ? num : MAX_ATTACHMENT_ITEM_NUM;
	memcpy(first_reward, m_other_cfg.first_reward, sizeof(ItemConfigData)*get_num);

	return;
}

int TowerDefendConfig::GetWaveCountBySceneID(int scene_id) const
{
	const TowerDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return 0;
	}

	return cfg_item->wave_count;
}

int TowerDefendConfig::GetLevelBySceneID(int scene_id) const
{
	const TowerDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return -1;
	}

	return cfg_item->level;
}
const TowerDefendWaveCfg * TowerDefendConfig::GetWaveCfg(int scene_id, int wave) const
{
	if (scene_id <= 0 || wave < 0 || wave >= PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL)
	{
		return NULL;
	}

	const TowerDefendLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL != cfg_item)
	{
		if (wave < cfg_item->wave_count)
		{
			return &cfg_item->wave_list[wave];
		}
	}

	return NULL;
}

const TowerDefendWaveCfg * TowerDefendConfig::GetWaveTestCfgRand() const
{
	if (m_wave_cfg_test.size() <= 0)
	{
		return NULL;
	}
	int index = RandomNum(m_wave_cfg_test.size());

	return &m_wave_cfg_test[index];
}

const TowerDefendWaveCfg *TowerDefendConfig::GetWaveTestCfgByWave(int wave) const
{
	for (int i = 0; i < (int)m_wave_cfg_test.size(); ++i)
	{
		if (m_wave_cfg_test[i].wave == wave)
		{
			return &m_wave_cfg_test[i];
		}
	}

	return NULL;
}

const TowerDefendWaveCfg * TowerDefendConfig::GetWaveCfgByMonsterID(int scene_id, int monster_id) const
{
	if (monster_id <= 0 || scene_id <= 0)
	{
		return NULL;
	}

	const TowerDefendLevelCfg::ConfigItem *cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (NULL == cfg_item)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_item->wave_count; ++ i)
	{
		const TowerDefendWaveCfg *wave_cfg = &cfg_item->wave_list[i];

		for (int j = 0; j < TOWERDEFEND_PATH_MAX; ++ j)
		{
			if (wave_cfg->monster_id_list[j] == monster_id)
			{
				return wave_cfg;
			}
		}
	}

	return NULL;
}

Posi TowerDefendConfig::GetBirthPos(int path) const
{
	if (path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX) 
	{
		return wave_pathway_list[TOWERDEFEND_PATH_MID][0];
	}

	return wave_pathway_list[path][0];
}

Posi TowerDefendConfig::GetBirthPosTest(int path) const
{
	if (path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX)
	{
		return m_wave_pathway_list_test[TOWERDEFEND_PATH_MID][0];
	}

	return m_wave_pathway_list_test[path][0];
}

bool TowerDefendConfig::GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos)
{
	UNSTD_STATIC_CHECK(TOWERDEFEND_PATH_MAX > 0);

	birth_path = birth_path % TOWERDEFEND_PATH_MAX;

	Posi last_posi = wave_pathway_list[birth_path][TOWERDEFEND_PATH_POINT_COUNT - 1];

	for (int count_i = 0; count_i < TOWERDEFEND_PATH_MAX; count_i++)
	{
		int i = (count_i + birth_path) % TOWERDEFEND_PATH_MAX;

		bool is_find = false;
		for (int j = 0; j < TOWERDEFEND_PATH_POINT_COUNT - 1; j++)
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

bool TowerDefendConfig::GetNextPosiTest(int birth_path, const Posi &curr_pos, Posi *pos)
{
	UNSTD_STATIC_CHECK(TOWERDEFEND_PATH_MAX > 0);

	birth_path = birth_path % TOWERDEFEND_PATH_MAX;

	Posi last_posi = m_wave_pathway_list_test[birth_path][TOWERDEFEND_PATH_POINT_COUNT - 1];

	for (int count_i = 0; count_i < TOWERDEFEND_PATH_MAX; count_i++)
	{
		int i = (count_i + birth_path) % TOWERDEFEND_PATH_MAX;

		bool is_find = false;
		for (int j = 0; j < TOWERDEFEND_PATH_POINT_COUNT - 1; j++)
		{
			Posi pos1 = m_wave_pathway_list_test[i][j], pos2 = m_wave_pathway_list_test[i][j + 1];
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

void TowerDefendConfig::GetPathWayList(int path, std::vector<Posi> & walk_pos)
{
	if (path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX)
	{
		path = TOWERDEFEND_PATH_MID;
	}
	for (int i = 0; i < TOWERDEFEND_PATH_POINT_COUNT; i++)
	{
		walk_pos.push_back(wave_pathway_list[path][i]);
	}
}

void TowerDefendConfig::GetPathWayListTest(int path, std::vector<Posi> & walk_pos)
{
	if (path <= TOWERDEFEND_PATH_INVALID || path >= TOWERDEFEND_PATH_MAX)
	{
		path = TOWERDEFEND_PATH_MID;
	}
	for (int i = 0; i < TOWERDEFEND_PATH_POINT_COUNT; i++)
	{
		walk_pos.push_back(m_wave_pathway_list_test[path][i]);
	}
}

bool TowerDefendConfig::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < TOWERDEFEND_WARNING_PERCENT_COUNT; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (NULL != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

const TowerDefendLevelCfg::ConfigItem * TowerDefendConfig::GetLevelCfgBySceneID(int scene_id) const
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		if (m_level_cfg.cfg_list[i].scene_id == scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const TowerDefendLevelCfg::ConfigItem * TowerDefendConfig::GetLevelCfg(int level) const
{
	if (level < 0 || level >= TowerDefendLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return NULL;
	}

	return &m_level_cfg.cfg_list[level];
}

int TowerDefendConfig::GetBuyJoinTimesCfg(int buy_times)
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

short TowerDefendConfig::GetStarByDeathCount(short death_count)
{
	for (int i = TOWERDEFEND_STAR_LEVEL_COUNT - 1; i >= 0; i--)
	{
		if (death_count >= m_pass_star_cfg[i].death_count)
		{
			return m_pass_star_cfg[i].star_level;
		}
	}

	return 0;
}

TowerDefendBuffDropWeightCfg * TowerDefendConfig::GetBuffDropCfg(int monster_id)
{
	std::map<int, TowerDefendBuffDropWeightCfg>::iterator iter = m_buff_drop_weight_map.find(monster_id);
	if (iter != m_buff_drop_weight_map.end() && iter->second.monster_id == monster_id)
	{
		return &iter->second;
	}

	return NULL;
}

TowerDefendBuffCfg * TowerDefendConfig::GetBufffCfg(int buff_type)
{
	std::map<int, TowerDefendBuffCfg>::iterator iter = m_buff_cfg_map.find(buff_type);
	if (iter != m_buff_cfg_map.end() && iter->second.buff_type == buff_type)
	{
		return &iter->second;
	}

	return NULL;
}

bool TowerDefendConfig::CheckLevelMonsterID(const TowerDefendLevelCfg::ConfigItem &cfg_item)
{
	std::set<int> monster_id_list;

	for (int j = 0; j < cfg_item.wave_count; ++ j)
	{
		const TowerDefendWaveCfg &wave_cfg = cfg_item.wave_list[j];
		std::set<int> cur_wave_monster_id_list;

		for (int k = 0; k < TOWERDEFEND_PATH_MAX; ++k)
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

