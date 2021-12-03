#include "crossyoumingbossconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/serverconfig/configstruct.h"
#include <limits>
#include <set>

CrossYoumingBossConfig::CrossYoumingBossConfig() : m_cross_boss_scene_count(0), m_cross_boss_ordinary_crystal_num(0)
{

}

CrossYoumingBossConfig::~CrossYoumingBossConfig()
{

}

CrossYoumingBossConfig & CrossYoumingBossConfig::Instance()
{
	static CrossYoumingBossConfig _instance;
	return _instance;
}

bool CrossYoumingBossConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossYoumingBossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode crossboss_element = RootElement.child("other");
		if (crossboss_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 经验奖励配置
		PugiXmlNode crossboss_element = RootElement.child("reward_exp");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->reward_exp]";
			return false;
		}

		iRet = this->InitRewardExpCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitRewardExpCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 大怪配置
		PugiXmlNode crossboss_element = RootElement.child("boss_cfg");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->boss_cfg]";
			return false;
		}

		iRet = this->InitBossCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitBossCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 小怪配置
		PugiXmlNode crossboss_element = RootElement.child("monster_cfg");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->monster_cfg]";
			return false;
		}

		iRet = this->InitMonsterCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitMonsterCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 层数配置
		PugiXmlNode crossboss_element = RootElement.child("layer_cfg");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->layer_cfg]";
			return false;
		}

		iRet = this->InitLayerCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitLayerCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 普通水晶配置
		PugiXmlNode crossboss_element = RootElement.child("ordinary_pos_cfg");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->ordinary_pos_cfg]";
			return false;
		}

		iRet = this->InitOrdinaryCrystalPosCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitOrdinaryCrystalPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 珍惜水晶配置
		PugiXmlNode crossboss_element = RootElement.child("treasure_pos_cfg");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->treasure_pos_cfg]";
			return false;
		}

		iRet = this->InitTreasureCrystalPosCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitTreasureCrystalPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// boss刷新配置
		PugiXmlNode crossboss_element = RootElement.child("cross_boss_flush");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->cross_boss_flush]";
			return false;
		}

		iRet = this->InitCrossBossFlushCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitCrossBossFlushCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// boss进入时间限制
		PugiXmlNode crossboss_element = RootElement.child("cross_boss_enter_time");
		if (crossboss_element.empty())
		{
			*err = path + ": xml node error in [config->cross_boss_enter_time]";
			return false;
		}

		iRet = this->InitCrossBossEnterTimeCfg(crossboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: CrossYoumingBossConfig::InitCrossBossEnterTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossYoumingBossConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (is_open != 0);


	if (!PugiGetSubNodeValue(dataElement, "gather_time_s", m_other_cfg.gather_time_s) || m_other_cfg.gather_time_s <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_time_s1", m_other_cfg.gather_time_s1) || m_other_cfg.gather_time_s1 <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_relive_times_need_gold", m_other_cfg.buy_relive_times_need_gold) || m_other_cfg.buy_relive_times_need_gold <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "relive_time", m_other_cfg.relive_time) || m_other_cfg.relive_time <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "monster_flush_time_s", m_other_cfg.monster_flush_time_s) || m_other_cfg.monster_flush_time_s <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "treasure_crystal_gather_times", m_other_cfg.treasure_crystal_gather_times) || m_other_cfg.treasure_crystal_gather_times <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "treasure_crystal_flush_time_s", m_other_cfg.treasure_crystal_flush_time_s) || m_other_cfg.treasure_crystal_flush_time_s <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "treasure_crystal_flush_num", m_other_cfg.treasure_crystal_flush_num) || m_other_cfg.treasure_crystal_flush_num < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "ordinary_crystal_gather_times", m_other_cfg.ordinary_crystal_gather_times) || m_other_cfg.ordinary_crystal_gather_times <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "ordinary_crystal_flush_time_s", m_other_cfg.ordinary_crystal_flush_time_s) || m_other_cfg.ordinary_crystal_flush_time_s <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "ordinary_crystal_gather_id", m_other_cfg.ordinary_crystal_gather_id) || m_other_cfg.ordinary_crystal_gather_id <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "daily_boss_num", m_other_cfg.daily_boss_num) || m_other_cfg.daily_boss_num <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "reward_exp_cd_s", m_other_cfg.reward_exp_cd_s) || m_other_cfg.reward_exp_cd_s <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_reduce_hp_precent", m_other_cfg.gather_reduce_hp_precent) || m_other_cfg.gather_reduce_hp_precent <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_reduce_hp_cd_s", m_other_cfg.gather_reduce_hp_cd_s) || m_other_cfg.gather_reduce_hp_cd_s <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "relive_tire_max_level", m_other_cfg.relive_tire_max_level) || m_other_cfg.relive_tire_max_level <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "relive_tire_time_s", m_other_cfg.relive_tire_time_s) || m_other_cfg.relive_tire_time_s <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "relive_tire_clear_time_s", m_other_cfg.relive_tire_clear_time_s) || m_other_cfg.relive_tire_clear_time_s <= 0)
	{
		return -18;
	}

	PugiXmlNode Itemelement = dataElement.child("ordinary_crystal_reward_item");
	int item_id = 0;
	if (!Itemelement.empty() && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && item_id > 0)
	{
		if (!m_other_cfg.ordinary_crystal_reward_item.ReadConfigNoCheck(Itemelement))
		{
			return -16;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::InitRewardExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_reward_exp_map.clear();

	int role_level = 0;
	int last_role_level = 0;
	long long every_time_exp = 0;
	long long last_every_time_exp = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "role_level", role_level) || role_level <= last_role_level)
		{
			return -1;
		}

		last_role_level = role_level;

		if (!PugiGetSubNodeValue(data_element, "every_time_exp", every_time_exp) || every_time_exp < last_every_time_exp)
		{
			return -2;
		}

		last_every_time_exp = every_time_exp;

		if (m_reward_exp_map.find(role_level) != m_reward_exp_map.end())
		{
			return -3;
		}

		m_reward_exp_map[role_level] = every_time_exp;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	std::set<long long> union_check_set;
	union_check_set.clear();

	int layer = 0;
	int last_layer = 0;
	int monster_index = 0;
	int last_monster_index = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || (layer != last_layer + 1 && layer != last_layer))
		{
			return -1;
		}

		if (layer != last_layer)
		{
			last_layer = layer;
			last_monster_index = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_index", monster_index) || monster_index <= 0 || monster_index > CROSS_YOUMING_BOSS_MAX_MONSTER_NUM || monster_index != last_monster_index + 1)
		{
			return -2;
		}

		CrossYoumingBossMonsterCfg::MonsterCfg &monster_cfg = m_cross_boss_monster_list[layer - 1].monster_list[monster_index - 1];
		last_monster_index = monster_index;
		monster_cfg.monster_index = monster_index;

		if (!PugiGetSubNodeValue(data_element, "pos_x", monster_cfg.pos_x) || monster_cfg.pos_x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", monster_cfg.pos_y) || monster_cfg.pos_y <= 0)
		{
			return -4;
		}

		long long key = ConvertParamToLongLong(monster_cfg.pos_x, monster_cfg.pos_y);
		if (union_check_set.find(key) != union_check_set.end()) // 重复的点
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id", monster_cfg.monster_id) || monster_cfg.monster_id <= 0)
		{
			return -5;
		}

		++m_cross_boss_monster_list[layer - 1].monster_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	std::set<long long> union_check_set;
	union_check_set.clear();

	int layer = 0;
	int last_layer = 0;
	int boss_index = 0;
	int last_boss_index = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || (layer != last_layer + 1 && layer != last_layer))
		{
			return -1;
		}

		if (layer != last_layer)
		{
			last_layer = layer;
			last_boss_index = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_index", boss_index) || boss_index <= 0 || boss_index > MAX_CROSS_YOUMING_BOSS_PER_SCENE || boss_index != last_boss_index + 1)
		{
			return -2;
		}

		CrossYoumingBossBossCfg::BossCfg &boss_cfg = m_cross_boss_boss_list[layer - 1].boss_list[boss_index - 1];
		last_boss_index = boss_index;
		boss_cfg.boss_index = boss_index;

		if (!PugiGetSubNodeValue(data_element, "flush_pos_x", boss_cfg.flush_pos_x) || boss_cfg.flush_pos_x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "flush_pos_y", boss_cfg.flush_pos_y) || boss_cfg.flush_pos_y <= 0)
		{
			return -4;
		}

		long long key = ConvertParamToLongLong(boss_cfg.flush_pos_x, boss_cfg.flush_pos_y);
		if (union_check_set.find(key) != union_check_set.end()) // 重复的点
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "flush_interval", boss_cfg.flush_interval) || boss_cfg.flush_interval <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", boss_cfg.boss_id) || boss_cfg.boss_id <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "max_delta_level", boss_cfg.max_delta_level) || boss_cfg.max_delta_level < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "max_delta_level", boss_cfg.max_delta_level) || boss_cfg.max_delta_level < 0)
		{
			return -7;
		}

		++m_cross_boss_boss_list[layer - 1].boss_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitLayerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int layer = 0;
	int last_layer = 0;
	int level_limit = 0;
	int last_level_limit = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "layer_index", layer) || layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || layer != last_layer + 1)
		{
			return -1;
		}

		m_cross_boss_layer_list[layer - 1].layer_index = layer;

		if (!PugiGetSubNodeValue(data_element, "level_limit", level_limit) || level_limit <= 0 || level_limit <= last_level_limit)
		{
			return -2;
		}

		m_cross_boss_layer_list[layer - 1].level_limit = level_limit;

		if (!PugiGetSubNodeValue(data_element, "scene_id", m_cross_boss_layer_list[layer - 1].scene_id) || m_cross_boss_layer_list[layer - 1].scene_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "treasure_crystal_gather_id", m_cross_boss_layer_list[layer - 1].treasure_crystal_gather_id) || m_cross_boss_layer_list[layer - 1].treasure_crystal_gather_id <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "entry_x", m_cross_boss_layer_list[layer - 1].enter_pos.x) || m_cross_boss_layer_list[layer - 1].enter_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "entry_y", m_cross_boss_layer_list[layer - 1].enter_pos.y) || m_cross_boss_layer_list[layer - 1].enter_pos.y <= 0)
		{
			return -6;
		}

		PugiXmlNode Itemelement = data_element.child("treasure_crystal_reward_item");
		int item_id = 0;
		if (!Itemelement.empty() && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && item_id > 0)
		{
			if (!m_cross_boss_layer_list[layer - 1].treasure_crystal_reward_item.ReadConfigNoCheck(Itemelement))
			{
				return -16;
			}
		}

		last_layer = layer;
		last_level_limit = level_limit;

		m_cross_boss_scene_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitOrdinaryCrystalPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	std::set<long long> union_check_set;

	int index = 0;
	int last_index = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "index", index) || index <= 0 || index > CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL || index != last_index + 1)
		{
			return -1;
		}

		last_index = index;
		m_ordinary_crystal_pos_list[index - 1].index = index;

		Posi pos;
		if (!PugiGetSubNodeValue(dataElement, "ordinary_pos_x", pos.x) || pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "ordinary_pos_y", pos.y) || pos.y <= 0)
		{
			return -3;
		}

		long long key = ConvertParamToLongLong(pos.x, pos.y);
		if (union_check_set.find(key) != union_check_set.end()) // 重复的点
		{
			return -4;
		}

		union_check_set.insert(key);
		m_ordinary_crystal_pos_list[index - 1].pos = pos;
		++m_cross_boss_ordinary_crystal_num;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitTreasureCrystalPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	std::set<long long> union_check_set;
	union_check_set.clear();

	int layer = 0;
	int last_layer = 0;
	int pos_num = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || (layer != last_layer + 1 && layer != last_layer))
		{
			return -1;
		}

		if (layer != last_layer)
		{
			union_check_set.clear();
			last_layer = layer;
			pos_num = 0;
		}

		Posi pos;
		if (!PugiGetSubNodeValue(data_element, "treasure_pos_x", pos.x) || pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "treasure_pos_y", pos.y) || pos.y <= 0)
		{
			return -3;
		}

		long long key = ConvertParamToLongLong(pos.x, pos.y);
		if (union_check_set.find(key) != union_check_set.end()) // 重复的点
		{
			return -4;
		}

		union_check_set.insert(key);
		m_treasure_crystal_pos_list[layer - 1].pos_list[pos_num] = pos;
		++pos_num;
		if (pos_num >= CROSS_YOUMING_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM)
		{
			return -5;
		}

		m_treasure_crystal_pos_list[layer - 1].pos_num = pos_num;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitCrossBossFlushCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		CrossYoumingBossFlushConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		int temp = 0;
		if (!PugiGetSubNodeValue(dataElement, "time", temp) || temp / 100 < 0 || temp / 100 > 23 || temp % 100 < 0 || temp % 100 > 59)
		{
			return -3;
		}

		cfg.time = (temp / 100) * SECOND_PER_HOUR + (temp % 100) * SECOND_PER_MIN;

		m_cross_boss_flush_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::InitCrossBossEnterTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		CrossYoumingBossEnterTimeConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		int temp = 0;
		if (!PugiGetSubNodeValue(dataElement, "begin_time", temp) || temp / 100 < 0 || temp / 100 > 23 || temp % 100 < 0 || temp % 100 > 59)
		{
			return -2;
		}

		cfg.begin_time = (temp / 100) * SECOND_PER_HOUR + (temp % 100) * SECOND_PER_MIN;

		if (!PugiGetSubNodeValue(dataElement, "end_time", temp) || temp / 100 < 0 || temp / 100 > 23 || temp % 100 < 0 || temp % 100 > 59)
		{
			return -3;
		}

		cfg.end_time = (temp / 100) * SECOND_PER_HOUR + (temp % 100) * SECOND_PER_MIN;

		if (temp % 100 == 59)
		{
			cfg.end_time += SECOND_PER_MIN;
		}

		m_cross_boss_enter_time_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossYoumingBossConfig::GetSceneIdList(int scene_id_list[CROSS_YOUMING_BOSS_SCENE_MAX])
{
	int count = 0;

	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		if (m_cross_boss_layer_list[i].scene_id > 0)
		{
			scene_id_list[count++] = m_cross_boss_layer_list[i].scene_id;
		}
	}

	return count;
}

int CrossYoumingBossConfig::GetSceneIdFirst()
{
	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		if (m_cross_boss_layer_list[i].scene_id > 0)
		{
			return m_cross_boss_layer_list[i].scene_id;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::GetLayerIndexBySceneId(int scene_id) const
{
	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		if (m_cross_boss_layer_list[i].scene_id == scene_id)
		{
			return m_cross_boss_layer_list[i].layer_index;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::GetLevelLimitBySceneLevel(int scene_level) const
{
	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		if (m_cross_boss_layer_list[i].layer_index == scene_level)
		{
			return m_cross_boss_layer_list[i].level_limit;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::GetCrossBossMonsterCount(int layer) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return 0;
	}

	return m_cross_boss_monster_list[layer - 1].monster_count;
}

int CrossYoumingBossConfig::GetCrossBossBossCount(int layer) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return 0;
	}

	return m_cross_boss_boss_list[layer - 1].boss_count;
}

long long CrossYoumingBossConfig::GetCrossBossRewardExp(int level)
{
	if (int(m_reward_exp_map.size()) <= 0) return 0;
	std::map<int, long long>::reverse_iterator rit;
	for (rit = m_reward_exp_map.rbegin(); rit != m_reward_exp_map.rend(); rit++)
	{
		if (level >= rit->first)
		{
			return rit->second;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::GetTreasureGatherIdBySceneId(int scene_id) const
{
	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		if (m_cross_boss_layer_list[i].scene_id == scene_id)
		{
			return m_cross_boss_layer_list[i].treasure_crystal_gather_id;
		}
	}

	return 0;
}

int CrossYoumingBossConfig::GetIndexByBossId(int layer, int boss_id) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return -1;
	}

	for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
	{
		if (boss_id == m_cross_boss_boss_list[layer - 1].boss_list[i].boss_id)
		{
			return i;
		}
	}

	return -1;
}

const CrossYoumingBossTreasureCrystalPosCfg * CrossYoumingBossConfig::GetTreasureCrystalPointListByLayer(int layer) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return NULL;
	}

	return &m_treasure_crystal_pos_list[layer - 1];
}

const CrossYoumingBossMonsterCfg::MonsterCfg *CrossYoumingBossConfig::GetCrossBossMonsterCfgByLayer(int layer, int index) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || index < 0 || index >= CROSS_YOUMING_BOSS_MAX_MONSTER_NUM)
	{
		return NULL;
	}

	return &m_cross_boss_monster_list[layer - 1].monster_list[index];
}

const CrossYoumingBossBossCfg::BossCfg * CrossYoumingBossConfig::GetCrossBossBossCfgByLayer(int layer, int index) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX || index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
	{
		return NULL;
	}

	return &m_cross_boss_boss_list[layer - 1].boss_list[index];
}

const CrossYoumingBossBossCfg::BossCfg * CrossYoumingBossConfig::GetCrossBossBossCfgByMonsterId(int scene_id, unsigned short monster_id) const
{
	int layer = this->GetLayerIndexBySceneId(scene_id);
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return NULL;
	}

	for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; ++i)
	{
		if (m_cross_boss_boss_list[layer - 1].boss_list[i].boss_id == monster_id)
		{
			return &m_cross_boss_boss_list[layer - 1].boss_list[i];
		}
	}

	return NULL;
}

const CrossYoumingBossOrdinaryCrystalPosCfg * CrossYoumingBossConfig::GetCrossBossOrdinaryCrystalPosCfg(int index) const
{
	if (index <= 0 || index > CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL)
	{
		return NULL;
	}

	return &m_ordinary_crystal_pos_list[index - 1];
}

const CrossYoumingBossLayerCfg * CrossYoumingBossConfig::GetCrossBossLayerCfg(int layer) const
{
	if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return NULL;
	}

	return &m_cross_boss_layer_list[layer - 1];
}

int CrossYoumingBossConfig::GetCrossBossNextFlushTimeInterval(unsigned int now_second)
{
	int day_second = now_second - static_cast<unsigned int>(GetZeroTime(now_second));

	for (int i = 0; i < (int)m_cross_boss_flush_cfg_vec.size(); ++i)
	{
		if (m_cross_boss_flush_cfg_vec[i].time > day_second)
		{
			return m_cross_boss_flush_cfg_vec[i].time - day_second;
		}
	}

	return static_cast<int>(GetZeroTime(now_second + SECOND_PER_DAY) - now_second + m_cross_boss_flush_cfg_vec[0].time);
}

bool CrossYoumingBossConfig::CanEnterCrossBoss(unsigned int now_second)
{
	int day_second = now_second - static_cast<unsigned int>(GetZeroTime(now_second));
	for (int i = 0; i < (int)m_cross_boss_enter_time_vec.size(); ++i)
	{
		if (day_second >= m_cross_boss_enter_time_vec[i].begin_time && day_second <= m_cross_boss_enter_time_vec[i].end_time)
		{
			return true;
		}
	}

	return false;
}