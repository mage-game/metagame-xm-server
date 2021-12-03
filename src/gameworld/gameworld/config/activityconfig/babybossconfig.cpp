#include "babybossconfig.hpp"
#include "guildfbconfig.hpp"
#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/scene.h"

BabyBossConfig::BabyBossConfig()
{

}

BabyBossConfig::~BabyBossConfig()
{

}

bool BabyBossConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG

	LOAD_CONFIG("other", InitOtherCfg)
	LOAD_CONFIG("kill_angry_value", InitKillAngryValueCfg)
	LOAD_CONFIG("enter_cost", InitEnterCostCfg)
	LOAD_CONFIG("scene_cfg", InitSceneCfg)
	LOAD_CONFIG("boss_flush_time", InitBabyBossFlushCfg)
	
	return true;
}

int BabyBossConfig::GetKillAngryValue(unsigned short monster_id)
{
	auto iter = m_kill_angry_value_cfg_map.find(monster_id);
	if (iter != m_kill_angry_value_cfg_map.end())
	{
		return iter->second;
	}

	return 0;
}

const BabyBossEnterCostCfg * BabyBossConfig::GetEnterCostCfg(int enter_times)
{
	int vec_size = static_cast<int>(m_enter_cost_cfg_vec.size());
	if (enter_times < 0 || enter_times >= vec_size)
	{
		return nullptr;
	}

	return &m_enter_cost_cfg_vec[enter_times];
}

bool BabyBossConfig::NeedLog(ItemID item_id)
{
	return m_need_log_item_set.find(item_id) != m_need_log_item_set.end();
}

const BabyBossCfg * BabyBossConfig::GetBabyBossCfgBySceneId(int scene_id)
{
	std::map<int, BabyBossCfg>::iterator it = m_baby_boss_cfg.begin();
	for (; it != m_baby_boss_cfg.end(); ++it)
	{
		if (it->second.scene_id == scene_id)
		{
			return &it->second;
		}
	}
	return nullptr;
}

const BabyBossCfg * BabyBossConfig::GetBabyBossCfgByBossId(int boss_id)
{
	std::map<int, BabyBossCfg>::iterator it = m_baby_boss_cfg.find(boss_id);
	if (it != m_baby_boss_cfg.end())
	{
		return &it->second;
	}
	return nullptr;
}

bool BabyBossConfig::IsBabyBoss(int scene_id, int scene_type, unsigned short monster_id)
{
	if (scene_type != Scene::SCENE_TYPE_BABY_BOSS)
	{
		return false;
	}

	std::map<int, BabyBossCfg>::iterator it = m_baby_boss_cfg.find(monster_id);
	if (it == m_baby_boss_cfg.end())
	{
		return false;
	}

	return it->second.scene_id == scene_id;
}

bool BabyBossConfig::IsBabyBoss(unsigned short monster_id)
{
	std::map<int, BabyBossCfg>::iterator it = m_baby_boss_cfg.find(monster_id);
	if (it != m_baby_boss_cfg.end())
	{
		return it->second.is_boss > 0;
	}
	return false;
}

int BabyBossConfig::GetBabyBossNextFlushTimeInterval(unsigned int now_second)
{
	int day_second = now_second - static_cast<unsigned int>(GetZeroTime(now_second));

	for (int i = 0; i < (int)m_baby_boss_flush_cfg_vec.size(); ++i)
	{
		if (m_baby_boss_flush_cfg_vec[i].time > day_second)
		{
			return m_baby_boss_flush_cfg_vec[i].time - day_second;
		}
	}

	return static_cast<int>(GetZeroTime(now_second + SECOND_PER_DAY) - now_second + m_baby_boss_flush_cfg_vec[0].time);
}

int BabyBossConfig::InitOtherCfg(TiXmlElement *root_element)
{
	TiXmlElement *dataElement = root_element->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -1000000;
	}

	if (!GetSubNodeValue(dataElement, "angry_value_add_per_minute", m_other_cfg.angry_value_add_per_minute) || m_other_cfg.angry_value_add_per_minute < 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(dataElement, "angry_value_limit", m_other_cfg.angry_value_limit) || m_other_cfg.angry_value_limit < 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(dataElement, "delay_kick_out_time", m_other_cfg.delay_kick_out_time) || m_other_cfg.delay_kick_out_time < 0)
	{
		return -3;
	}

	if (!GetSubNodeValue(dataElement, "logout_kick_out_time", m_other_cfg.logout_kick_out_time) || m_other_cfg.logout_kick_out_time < 0)
	{
		return -4;
	}

	if (!GetSubNodeValue(dataElement, "need_item_id", m_other_cfg.need_item_id) || m_other_cfg.need_item_id < 0 || nullptr == ITEMPOOL->GetItem(m_other_cfg.need_item_id))
	{
		return -5;
	}

	if (!GetSubNodeValue(dataElement, "angry_value_die", m_other_cfg.angry_value_die) || m_other_cfg.angry_value_die < 0)
	{
		return -5;
	}

	return 0;
}

int BabyBossConfig::InitKillAngryValueCfg(TiXmlElement *root_element)
{
	TiXmlElement *dataElement = root_element->FirstChildElement("data");

	while (NULL != dataElement)
	{
		unsigned short monster_id = 0;
		if (!GetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}

		int angry_value = -1;
		if (!GetSubNodeValue(dataElement, "angry_value", angry_value) || angry_value < 0)
		{
			return -2;
		}

		if (!m_kill_angry_value_cfg_map.insert(std::make_pair(monster_id, angry_value)).second)
		{
			return -3;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int BabyBossConfig::InitEnterCostCfg(TiXmlElement *root_element)
{
	TiXmlElement *dataElement = root_element->FirstChildElement("data");

	while (NULL != dataElement)
	{
		int enter_times = -1;
		if (!GetSubNodeValue(dataElement, "enter_times", enter_times) || enter_times != static_cast<int>(m_enter_cost_cfg_vec.size()))
		{
			return -1;
		}

		BabyBossEnterCostCfg cfg;
		if (!GetSubNodeValue(dataElement, "need_item_num", cfg.need_item_num) || cfg.need_item_num < 0)
		{
			return -2;
		}

		m_enter_cost_cfg_vec.push_back(cfg);

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int BabyBossConfig::InitNeedLogItemConfig(TiXmlElement *root_element)
{
	TiXmlElement *dataElement = root_element->FirstChildElement("data");

	while (NULL != dataElement)
	{
		ItemID item_id = 0;
		if (!GetSubNodeValue(dataElement, "item_id", item_id) || nullptr == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		if (!m_need_log_item_set.insert(item_id).second)
		{
			return false;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int BabyBossConfig::InitSceneCfg(TiXmlElement *root_element)
{
	TiXmlElement *dataElement = root_element->FirstChildElement("data");

	while (NULL != dataElement)
	{
		int boss_id = -1;
		if (!GetSubNodeValue(dataElement, "monster_id", boss_id) || NULL == MONSTERPOOL->GetMonsterParam(boss_id))
		{
			return -1;
		}

		if (m_baby_boss_cfg.find(boss_id) != m_baby_boss_cfg.end())
		{
			return -2;
		}

		BabyBossCfg &boss_cfg = m_baby_boss_cfg[boss_id];
		boss_cfg.boss_id = boss_id;

		if (!GetSubNodeValue(dataElement, "scene_id", boss_cfg.scene_id) || !SceneRegister::Instance().CheckSceneIsExist(boss_cfg.scene_id))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "is_boss", boss_cfg.is_boss) || boss_cfg.is_boss < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "enter_pos_x", boss_cfg.enter_pos.x) || boss_cfg.enter_pos.x < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "enter_pos_y", boss_cfg.enter_pos.y) || boss_cfg.enter_pos.y < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "born_pos_x", boss_cfg.born_pos.x) || boss_cfg.born_pos.x < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "born_pos_y", boss_cfg.born_pos.y) || boss_cfg.born_pos.y < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "refresh_interval", boss_cfg.refresh_interval) || boss_cfg.refresh_interval < 0)
		{
			return -9;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int BabyBossConfig::InitBabyBossFlushCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	if (NULL == dataElement)
	{
		return -1000;
	}

	while (NULL != dataElement)
	{
		BabyBossFlushConfig cfg;

		if (!GetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		int temp = 0;
		if (!GetSubNodeValue(dataElement, "time", temp) || temp / 100 < 0 || temp / 100 > 23 || temp % 100 < 0 || temp % 100 > 59)
		{
			return -3;
		}

		cfg.time = (temp / 100) * SECOND_PER_HOUR + (temp % 100) * SECOND_PER_MIN;

		m_baby_boss_flush_cfg_vec.push_back(cfg);

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}
