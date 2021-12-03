#include "nationalbossconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

NationalBossConfig::NationalBossConfig() : m_scene_id(0), m_refresh_interval_s(0), m_buff_gold(0), m_buy_gongji_percent(0),
	m_max_buy_times(0), m_refresh_count(0), m_reward_count(0)
{
}

NationalBossConfig::~NationalBossConfig()
{

}

bool NationalBossConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "NationalBossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 
		PugiXmlNode pos_element = RootElement.child("poslist");
		if (pos_element.empty())
		{
			*err = configname + ": no [poslist].";
			return false;
		}

		iRet = this->InitPosCfg(pos_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode rwd_element = RootElement.child("rankreward");
		if (rwd_element.empty())
		{
			*err = configname + ": no [rankreward].";
			return false;
		}

		iRet = this->InitRankRewardCfg(rwd_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRankRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
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
		// ÈÎÎñ½±Àø
		PugiXmlNode task_reward_element = RootElement.child("task_reward");
		if (task_reward_element.empty())
		{
			*err = configname + ": no [task_reward].";
			return false;
		}

		iRet = this->InitTaskRewardCfg(task_reward_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTaskRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int NationalBossConfig::InitPosCfg(PugiXmlNode RootElement)
{
	m_refresh_count =  0;

	int last_world_level = -1;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_refresh_count >= NATIONALBOSS_FRESH_CONFIG_COUNT)
		{
			return -1;
		}

		NBRefreshItem &cfg_item = m_refresh_list[m_refresh_count];

		if (!PugiGetSubNodeValue(dataElement, "world_level", cfg_item.world_level) || cfg_item.world_level < last_world_level)
		{
			return -2;
		}
		last_world_level = cfg_item.world_level;

		UNSTD_STATIC_CHECK(NATIONALBOSS_FRESH_BOSS_COUNT >= 4);

		{
			int tmp_boss_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "boss_id_1", tmp_boss_id) || !MONSTERPOOL->IsMonsterExist(tmp_boss_id))
			{
				return -13;
			}
			cfg_item.boss_id_list[0] = tmp_boss_id;

			Posi tmp_pos;
			if (!PugiGetSubNodeValue(dataElement, "pos_x_1", tmp_pos.x) || tmp_pos.x <= 0)
			{
				return -14;
			}
			if (!PugiGetSubNodeValue(dataElement, "pos_y_1", tmp_pos.y) || tmp_pos.y <= 0)
			{
				return -15;
			}
			cfg_item.born_pos_list[0] = tmp_pos;
		}

		{
			int tmp_boss_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "boss_id_2", tmp_boss_id) || !MONSTERPOOL->IsMonsterExist(tmp_boss_id))
			{
				return -23;
			}
			cfg_item.boss_id_list[1] = tmp_boss_id;

			Posi tmp_pos;
			if (!PugiGetSubNodeValue(dataElement, "pos_x_2", tmp_pos.x) || tmp_pos.x <= 0)
			{
				return -24;
			}
			if (!PugiGetSubNodeValue(dataElement, "pos_y_2", tmp_pos.y) || tmp_pos.y <= 0)
			{
				return -25;
			}
			cfg_item.born_pos_list[1] = tmp_pos;
		}

		{
			int tmp_boss_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "boss_id_3", tmp_boss_id) || !MONSTERPOOL->IsMonsterExist(tmp_boss_id))
			{
				return -33;
			}
			cfg_item.boss_id_list[2] = tmp_boss_id;

			Posi tmp_pos;
			if (!PugiGetSubNodeValue(dataElement, "pos_x_3", tmp_pos.x) || tmp_pos.x <= 0)
			{
				return -34;
			}
			if (!PugiGetSubNodeValue(dataElement, "pos_y_3", tmp_pos.y) || tmp_pos.y <= 0)
			{
				return -35;
			}
			cfg_item.born_pos_list[2] = tmp_pos;
		}

		{
			int tmp_boss_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "boss_id_4", tmp_boss_id) || !MONSTERPOOL->IsMonsterExist(tmp_boss_id))
			{
				return -33;
			}
			cfg_item.boss_id_list[3] = tmp_boss_id;

			Posi tmp_pos;
			if (!PugiGetSubNodeValue(dataElement, "pos_x_4", tmp_pos.x) || tmp_pos.x <= 0)
			{
				return -34;
			}
			if (!PugiGetSubNodeValue(dataElement, "pos_y_4", tmp_pos.y) || tmp_pos.y <= 0)
			{
				return -35;
			}
			cfg_item.born_pos_list[3] = tmp_pos;
		}

		++ m_refresh_count;
		dataElement = dataElement.next_sibling();
	}

	if (0 != m_refresh_list[0].world_level)
	{
		return -100;
	}

	return 0;
}

int NationalBossConfig::InitRankRewardCfg(PugiXmlNode RootElement)
{
	m_reward_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_reward_count >= MAX_REWARD_NUM)
		{
			break;
		}

		NBRankReward cfg;

		if (!PugiGetSubNodeValue(dataElement, "min_rank", cfg.min_rank))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_rank", cfg.max_rank))
		{
			return -2;
		}

		PugiXmlNode RandItemElement = dataElement.child("reward_item");
		if (RandItemElement.empty())
		{
			return -10;
		}

		if (!cfg.item_cfg.ReadConfig(RandItemElement))
		{
			return -11;
		}

		m_reward_list[m_reward_count++] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int NationalBossConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "refresh_interval_s", m_refresh_interval_s) )
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", m_scene_id) )
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "buff_gold", m_buff_gold) )
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_gongji_percent", m_buy_gongji_percent) )
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_buy_times", m_max_buy_times) )
		{
			return -5;
		}
	}

	return 0;
}

int NationalBossConfig::InitTaskRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -20000;
	}

	int last_world_level = -1; int last_hurt_percent = -1;

	while (!dataElement.empty())
	{
		if (m_task_reward_cfg.cfg_count >= NBTaskRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		NBTaskRewardConfig::ConfigItem &cfg_item = m_task_reward_cfg.cfg_list[m_task_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "world_level", cfg_item.world_level) || cfg_item.world_level < last_world_level)
		{
			return -11;
		}
		if (last_world_level != cfg_item.world_level)
		{
			last_world_level = cfg_item.world_level;
			last_hurt_percent = -1;			
		}

		if (!PugiGetSubNodeValue(dataElement, "hurt_percent", cfg_item.hurt_percent) || cfg_item.hurt_percent <= last_hurt_percent)
		{
			return -1;
		}
		last_hurt_percent = cfg_item.hurt_percent;

		if (!PugiGetSubNodeValue(dataElement, "reward_exp", cfg_item.reward_exp) || cfg_item.reward_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_coin", cfg_item.reward_coin) || cfg_item.reward_coin <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_level", cfg_item.reward_level) || cfg_item.reward_level <= 0)
		{
			return -4;
		}

		++ m_task_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_task_reward_cfg.cfg_count <= 0)
	{
		return -40000;
	}

	if (0 != m_task_reward_cfg.cfg_list[0].world_level)
	{
		return -50000;
	}

	return 0;
}

const NBRefreshItem * NationalBossConfig::GetRefreshList(int world_level)
{
	for (int i = m_refresh_count - 1; i >= 0; -- i)
	{
		if (world_level >= m_refresh_list[i].world_level)
		{
			return &m_refresh_list[i];
		}
	}

	return NULL;
}

ItemConfigData * NationalBossConfig::GetRankReward(int rank)
{
	if (rank <= 10)
	{
		for (int i = 0; i < m_reward_count; ++i)
		{
			if (rank >= m_reward_list[i].min_rank && rank <= m_reward_list[i].max_rank)
			{
				return &m_reward_list[i].item_cfg;
			}
		}
	}
	else
	{
		for (int i = m_reward_count - 1; i >= 0; --i)
		{
			if (rank >= m_reward_list[i].min_rank && rank <= m_reward_list[i].max_rank)
			{
				return &m_reward_list[i].item_cfg;
			}
		}
	}

	return NULL;
}

const NBTaskRewardConfig::ConfigItem * NationalBossConfig::GetTaskReward(int world_level, int hurt_percent)
{
	int suitable_world_level = 0;
	for (int i = m_task_reward_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (world_level >= m_task_reward_cfg.cfg_list[i].world_level)
		{
			suitable_world_level = m_task_reward_cfg.cfg_list[i].world_level;
			break;
		}
	}

	for (int i = m_task_reward_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (suitable_world_level == m_task_reward_cfg.cfg_list[i].world_level)
		{
			if (hurt_percent >= m_task_reward_cfg.cfg_list[i].hurt_percent)
			{
				return &m_task_reward_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}


