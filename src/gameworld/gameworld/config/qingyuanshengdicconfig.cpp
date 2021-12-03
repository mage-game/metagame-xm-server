#include "qingyuanshengdicconfig.h"

#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"

QingYuanShengDiConfig::QingYuanShengDiConfig()
{

}

QingYuanShengDiConfig::~QingYuanShengDiConfig()
{

}

bool QingYuanShengDiConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("layer", InitLayerCfg);
	PUGI_XML_LOAD_CONFIG("monster", InitMonsterCfg);
	PUGI_XML_LOAD_CONFIG("gather", InitGatherCfg);
	PUGI_XML_LOAD_CONFIG("pos", InitPosCfg);
	PUGI_XML_LOAD_CONFIG("task", InitTaskCfg);

	return true;
}

int QingYuanShengDiConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	{
		m_other_cfg.male_item_count = ItemConfigData::ReadConfigList(data_element, std::string("male_reward_item"),
			m_other_cfg.male_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (m_other_cfg.male_item_count < 0)
		{
			return -100 + m_other_cfg.male_item_count;
		}
	}

	{
		m_other_cfg.female_item_count = ItemConfigData::ReadConfigList(data_element, std::string("female_reward_item"),
			m_other_cfg.female_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (m_other_cfg.female_item_count < 0)
		{
			return -200 + m_other_cfg.female_item_count;
		}
	}

	if (!PugiGetSubNodeValue(data_element, "skip_task_consume", m_other_cfg.skip_task_consume) || m_other_cfg.skip_task_consume <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "skip_task_limit_level", m_other_cfg.skip_task_limit_level) || m_other_cfg.skip_task_limit_level <= 0)
	{
		return -2;
	}

	return 0;
}

int QingYuanShengDiConfig::InitLayerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int layer = 0;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0)
		{
			return -1;
		}

		QYSDLayerCfg &cfg = m_layer_cfg_map[layer];
		cfg.layer = layer;

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(data_element, "birth_pos_x", cfg.birth_pos.x) || cfg.birth_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "birth_pos_y", cfg.birth_pos.y) || cfg.birth_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_level", cfg.enter_level) || cfg.enter_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "begin_time", cfg.begin_time) || cfg.begin_time < 0 || cfg.begin_time > 2400)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "end_time", cfg.end_time) || cfg.end_time < cfg.begin_time || cfg.end_time > 2400)
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingYuanShengDiConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int layer = 0;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || m_layer_cfg_map.find(layer) == m_layer_cfg_map.end())
		{
			return -1;
		}

		QYSDLayerCfg &cfg = m_layer_cfg_map[layer];

		QYSDLayerCfg::MonsterItem item;

		if (!PugiGetSubNodeValue(data_element, "monster_id", item.monster_id) || !MONSTERPOOL->IsMonsterExist(item.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_count", item.monster_count) || item.monster_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "refresh_time", item.refresh_time) || item.refresh_time < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_type", item.pos_type) || item.pos_type < 0)
		{
			return -5;
		}

		cfg.monster_vec.push_back(item);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingYuanShengDiConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int layer = 0;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || m_layer_cfg_map.find(layer) == m_layer_cfg_map.end())
		{
			return -1;
		}

		QYSDLayerCfg &cfg = m_layer_cfg_map[layer];

		QYSDLayerCfg::GatherItem item;

		if (!PugiGetSubNodeValue(data_element, "gather_id", item.gather_id) || item.gather_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_count", item.gather_count) || item.gather_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_time", item.gather_time) || item.gather_time <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "refresh_time", item.refresh_time) || item.refresh_time < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_type", item.pos_type) || item.pos_type < 0)
		{
			return -6;
		}

		cfg.gather_vec.push_back(item);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingYuanShengDiConfig::InitPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int pos_type = 0;
		if (!PugiGetSubNodeValue(data_element, "pos_type", pos_type) || pos_type < 0)
		{
			return -1;
		}

		std::vector<Posi> &pos_vec = m_pos_map[pos_type];
		Posi pos(0, 0);

		if (!PugiGetSubNodeValue(data_element, "pos_x", pos.x) || pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", pos.y) || pos.y <= 0)
		{
			return -3;
		}

		pos_vec.push_back(pos);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingYuanShengDiConfig::InitTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int level_min = -1, level_max = -1;
	while (!data_element.empty())
	{
		int task_id = 0;
		if (!PugiGetSubNodeValue(data_element, "task_id", task_id) || task_id < 0)
		{
			return -1;
		}

		QYSDTaskCfg &task = m_task_cfg_map[task_id];
		task.task_id = task_id;

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < level_max)
		{
			return -2;
		}

		if (level != level_max)
		{
			level_min = level_max + 1;
			level_max = level;
		}
		task.level_min = level_min;
		task.level_max = level_max;

		if (!PugiGetSubNodeValue(data_element, "task_type", task.task_type) || task.task_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", task.param1) || task.param1 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", task.param2) || task.param2 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_exp", task.reward_exp) || task.reward_exp < 0)
		{
			return -6;
		}

		{
			task.male_item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				task.male_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (task.male_item_count < 0)
			{
				return -100 + task.male_item_count;
			}
		}

		{
			task.female_item_count = ItemConfigData::ReadConfigList(data_element, std::string("female_reward_item"),
				task.female_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (task.female_item_count < 0)
			{
				return -200 + task.female_item_count;
			}
		}

		data_element = data_element.next_sibling();
	}
	return 0;
}

const QYSDLayerCfg * QingYuanShengDiConfig::GetQYSDLayerCfg(int layer)
{
	std::map<int, QYSDLayerCfg>::iterator it = m_layer_cfg_map.find(layer);
	if (it != m_layer_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int QingYuanShengDiConfig::GetCanEnterMaxLayer(int marry_level)
{
	int max_layer = 0;
	for (std::map<int, QYSDLayerCfg>::iterator it = m_layer_cfg_map.begin(); it != m_layer_cfg_map.end(); it++)
	{
		if (marry_level >= it->second.enter_level && it->first > max_layer)
		{
			max_layer = it->first;
		}
	}

	return max_layer;
}

const std::vector<Posi> * QingYuanShengDiConfig::GetQYSDPosCfg(int pos_type)
{
	std::map<int, std::vector<Posi> >::iterator it = m_pos_map.find(pos_type);
	if (it != m_pos_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const QYSDTaskCfg * QingYuanShengDiConfig::GetQYSDTaskCfg(int task_id)
{
	std::map<int, QYSDTaskCfg>::iterator it = m_task_cfg_map.find(task_id);
	if (it != m_task_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int QingYuanShengDiConfig::GetQYSDTaskList(int marry_level, int *task_id_list, int task_id_count)
{
	if (NULL == task_id_list || task_id_count <= 0)
	{
		return 0;
	}

	int count = 0;
	for (std::map<int, QYSDTaskCfg>::iterator it = m_task_cfg_map.begin(); it != m_task_cfg_map.end(); it++)
	{
		QYSDTaskCfg &info = it->second;
		if (marry_level >= info.level_min && marry_level <= info.level_max)
		{
			task_id_list[count++] = info.task_id;
			if (count >= task_id_count)
			{
				break;
			}
		}
	}

	return count;
}
