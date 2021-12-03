#include "worldbossbaseconfig.hpp"
#include <memory.h>
#include <stdlib.h>
#include "engineadapter.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/scene.h"
#include <set>
#include <string>
#include <time.h>


WorldBossBaseConfig::WorldBossBaseConfig():m_max_worldboss_count(0) 
{
}

WorldBossBaseConfig::~WorldBossBaseConfig()
{
}

bool WorldBossBaseConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "WorldBossBaseConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode worldboss_element = RootElement.child("other");
		if (worldboss_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitWorldBossOtherCfg(worldboss_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode worldboss_element = RootElement.child("worldboss_list");
		if (worldboss_element.empty())
		{
			*err = configname + ": no [worldboss_list].";
			return false;
		}

		iRet = this->InitWorldBossCfg(worldboss_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode weekreward_element = RootElement.child("week_rank_reward");
		if (weekreward_element.empty())
		{
			*err = configname + ": no [week_rank_reward].";
			return false;
		}

		iRet = this->InitWorldBossWeekRewardCfg(weekreward_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossWeekRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode hurt_reward_element = RootElement.child("cur_boss_hurt_reward");
		if (hurt_reward_element.empty())
		{
			*err = configname + ": no [cur_boss_hurt_reward].";
			return false;
		}

		iRet = this->InitWorldBossPersonHurtRewardCfg(hurt_reward_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossPersonHurtRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode hurt_guid_element = RootElement.child("cur_boss_hurt_guid_reward");
		if (hurt_guid_element.empty())
		{
			*err = configname + ": no [cur_boss_hurt_guid_reward].";
			return false;
		}

		iRet = this->InitWorldBossGuildHurtRewardCfg(hurt_guid_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossGuildHurtRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode hurt_guid_element = RootElement.child("relive_cfg");
		if (hurt_guid_element.empty())
		{
			*err = configname + ": no [relive_cfg].";
			return false;
		}

		iRet = this->InitWorldBossReliveCfg(hurt_guid_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWorldBossReliveCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const WorldBossInfoCfg * WorldBossBaseConfig::GetWorldBossByBossId(int boss_id) const
{
	std::map<int, WorldBossInfoCfg>::const_iterator it = m_boss_info_map.find(boss_id);
	if(it != m_boss_info_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int WorldBossBaseConfig::GetWorldBossIdBySceneId(int scene_id)
{
	if (scene_id <= 0)
	{
		return 0;
	}

	for (std::map<int, WorldBossInfoCfg>::iterator it = m_boss_info_map.begin(), end = m_boss_info_map.end(); it != end; ++ it)
	{
		if (it->second.scene_id == scene_id)
		{
			return it->second.bossID;
		}
	}

	return 0;
}

const WorldBossWeekRankRewardCfg* WorldBossBaseConfig::GetWorldBossWeekRankReward(int rank_value) const
{
	std::map<int, WorldBossWeekRankRewardCfg>::const_iterator iter = m_guild_week_rank_reward_cfg.find(rank_value);
	if (m_guild_week_rank_reward_cfg.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

const WorldBossPersonHurtRankRewardCfg* WorldBossBaseConfig::GetWorldBossPersonHurtReward(int rank_value, int boss_id) const
{
	if (rank_value <= 0 || boss_id <= 0)
	{
		return NULL;
	}

	long long reward_index = ((long long)(rank_value) << 32) + boss_id;

	std::map<long long, WorldBossPersonHurtRankRewardCfg>::const_iterator iter = m_person_hurt_rank_reward.find(reward_index);
	if (m_person_hurt_rank_reward.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

const WorldBossGuildHurtRankRewardCfg* WorldBossBaseConfig::GetWorldBossGuildHurtReward(int rank_value, int boss_id) const
{
	if (rank_value <= 0 || boss_id <= 0)
	{
		return NULL;
	}

	long long reward_index = ((long long)(rank_value) << 32) + boss_id;

	std::map<long long, WorldBossGuildHurtRankRewardCfg>::const_iterator iter = m_guild_hurt_rank_reward.find(reward_index);
	if (m_guild_hurt_rank_reward.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

const WorldBossInfoCfg * WorldBossBaseConfig::GetWorldBossInfo(int boss_id) const
{
	if (boss_id < 0) return NULL;

	std::map<int, WorldBossInfoCfg>::const_iterator iter = m_boss_info_map.find(boss_id);
	if (m_boss_info_map.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

const WorldBossReliveCfg * WorldBossBaseConfig::GetWorldBossReliveInfo(int scene_id) const
{
	if (!Scene::IsWorldBossScene(scene_id))
	{
		return NULL;
	}
	
	std::map<int, WorldBossReliveCfg>::const_iterator iter = m_worldboss_relive_cfg_map.find(scene_id);
	if (m_worldboss_relive_cfg_map.end() != iter)
	{
		return &iter->second;
	}

	return NULL;
}

int WorldBossBaseConfig::InitWorldBossOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "refresh_time_one", m_other_cfg.refresh_time_one) || m_other_cfg.refresh_time_one <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "refresh_time_two", m_other_cfg.refresh_time_two) || m_other_cfg.refresh_time_two <= 0 || m_other_cfg.refresh_time_one >= m_other_cfg.refresh_time_two)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "person_reward_rank_max", m_other_cfg.person_reward_rank_max) || m_other_cfg.person_reward_rank_max < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_reward_rank_max", m_other_cfg.guild_reward_rank_max) || m_other_cfg.guild_reward_rank_max < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "week_reward_rank_max", m_other_cfg.week_reward_rank_max) || m_other_cfg.week_reward_rank_max < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "refresh_hurt_rank", m_other_cfg.refresh_hurt_rank) || m_other_cfg.refresh_hurt_rank < 1)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "roll_effective_time", m_other_cfg.roll_effective_time) || m_other_cfg.roll_effective_time < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "refresh_time_three", m_other_cfg.refresh_time_three) || m_other_cfg.refresh_time_three <= 0 
		/*|| m_other_cfg.refresh_time_two >= m_other_cfg.refresh_time_three*/)
	{
		return -2;
	}

	return 0;
}

int WorldBossBaseConfig::InitWorldBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	while (!dataElement.empty())
	{
		WorldBossInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "bossID", cfg.bossID) || cfg.bossID <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.bossID))
		{
			return -1;
		}

		std::map<int, WorldBossInfoCfg>::iterator iter = m_boss_info_map.find(cfg.bossID);	// 没有重复的BOSSID
		if (m_boss_info_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsWorldBossScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);
		SceneRegister::Instance().AddWorldBossScene(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_bind_gold", cfg.per_bind_gold) || cfg.per_bind_gold < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shield_continue_time", cfg.shield_continue_time) || cfg.shield_continue_time < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "shield_effective_times", cfg.shield_effective_times) || cfg.shield_effective_times < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shield_fix_hurt", cfg.shield_fix_hurt))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_trigger_hudun", cfg.first_trigger_hudun) || cfg.first_trigger_hudun < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "second_trigger_hudun", cfg.second_trigger_hudun) || cfg.second_trigger_hudun < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "roll_max_num", cfg.roll_max_num) || cfg.roll_max_num <= 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg.min_lv) || cfg.min_lv <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg.max_lv) || cfg.max_lv <= 0 || cfg.max_lv < cfg.min_lv)
		{
			return -14;
		}

		PugiXmlNode Itemelement = dataElement.child("gift_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg.gift_ierm.ReadConfig(Itemelement))
			{
				return -13;
			}
		}

		PugiXmlNode Itemelement2 = dataElement.child("stuff_item");
		ItemID itemid2 = 0;
		if (nullptr != Itemelement2 && PugiGetSubNodeValue(Itemelement2, "item_id", itemid2) && itemid2 > 0)
		{
			if (!cfg.stuff_item.ReadConfig(Itemelement2))
			{
				return -14;
			}
		}

		m_boss_info_map[cfg.bossID] = cfg;

		m_max_worldboss_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int WorldBossBaseConfig::InitWorldBossWeekRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");

	int last_rank_value = 0;
	while (!dataElem.empty())
	{

		int rank_value = 0;
		if (!PugiGetSubNodeValue(dataElem, "rank_value", rank_value) || rank_value <= 0 || rank_value <= last_rank_value)
		{
			return -1;
		}

		WorldBossWeekRankRewardCfg week_reward_cfg;
		week_reward_cfg.rank_value = rank_value;
		last_rank_value = rank_value;

		PugiXmlNode Itemelement = dataElem.child("stuff_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!week_reward_cfg.stuff_item.ReadConfig(Itemelement))
			{
				return -2;
			}
		}

		Itemelement = dataElem.child("gift_item");
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!week_reward_cfg.gift_item.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		m_guild_week_rank_reward_cfg[rank_value] = week_reward_cfg;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WorldBossBaseConfig::InitWorldBossPersonHurtRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");

	while (!dataElem.empty())
	{
		int boss_id = 0;
		int rank_value = 0;

		if (!PugiGetSubNodeValue(dataElem, "bossID", boss_id) || boss_id <= 0 || NULL == MONSTERPOOL->GetMonsterParam(boss_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "rank_value", rank_value) || rank_value <= 0 )
		{
			return -2;
		}

		WorldBossPersonHurtRankRewardCfg cfg;
		cfg.bossID = boss_id;
		cfg.rank_value =rank_value;

		PugiXmlNode Itemelement = dataElem.child("gift_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg.gift_item.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		Itemelement = dataElem.child("stuff_item");
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg.stuff_item.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		long long person_hurt_reward_index = ((long long)(cfg.bossID) << 32) + cfg.rank_value;

		std::map<long long, WorldBossPersonHurtRankRewardCfg>::iterator iter = m_person_hurt_rank_reward.find(person_hurt_reward_index);
		if (m_person_hurt_rank_reward.end() != iter)
		{
			return -5;
		}

		m_person_hurt_rank_reward[person_hurt_reward_index] = cfg;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WorldBossBaseConfig::InitWorldBossGuildHurtRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");

	while (!dataElem.empty())
	{
		int boss_id = 0;
		int rank_value = 0;

		if (!PugiGetSubNodeValue(dataElem, "bossID", boss_id) || boss_id <= 0 || NULL == MONSTERPOOL->GetMonsterParam(boss_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "rank_value", rank_value) || rank_value <= 0 )
		{
			return -2;
		}

		WorldBossGuildHurtRankRewardCfg cfg;
		cfg.bossID = boss_id;
		cfg.rank_value =rank_value;

		PugiXmlNode Itemelement = dataElem.child("gift_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg.gift_item.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		long long guild_hurt_reward_index = ((long long)(cfg.bossID) << 32) + cfg.rank_value;

		std::map<long long, WorldBossGuildHurtRankRewardCfg>::iterator iter = m_guild_hurt_rank_reward.find(guild_hurt_reward_index);
		if (m_guild_hurt_rank_reward.end() != iter)
		{
			return -4;
		}

		m_guild_hurt_rank_reward[guild_hurt_reward_index] = cfg;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WorldBossBaseConfig::InitWorldBossReliveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");

	while (!dataElem.empty())
	{
		int scene_id = 0;

		if (!PugiGetSubNodeValue(dataElem, "boss_scene_id", scene_id) || scene_id <= 0 || !Scene::IsWorldBossScene(scene_id) || m_worldboss_relive_cfg_map.find(scene_id) != m_worldboss_relive_cfg_map.end())
		{
			return -1;
		}
		
		WorldBossReliveCfg cfg;
		cfg.boss_scene_id = scene_id;

		if (!PugiGetSubNodeValue(dataElem, "relive_scene_id", cfg.relive_scene_id) || cfg.relive_scene_id <= 0 || !Scene::IsWorldBossReliveScene(cfg.relive_scene_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "born_x", cfg.relive_pos.x) || cfg.relive_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "born_y", cfg.relive_pos.y) || cfg.relive_pos.y <= 0)
		{
			return -4;
		}

		m_worldboss_relive_cfg_map[scene_id] = cfg;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

bool WorldBossBaseConfig::GetNextRefreshTimeOfBoss(unsigned int *next_refresh_time) const
{
	if (NULL == next_refresh_time) return false;

	int temp1 = EngineAdapter::Instance().NextDayInterval(m_other_cfg.refresh_time_one / 100, m_other_cfg.refresh_time_one % 100, 0);
	int temp2 = EngineAdapter::Instance().NextDayInterval(m_other_cfg.refresh_time_two / 100, m_other_cfg.refresh_time_two % 100, 0);
	int temp3 = EngineAdapter::Instance().NextDayInterval(m_other_cfg.refresh_time_three / 100, m_other_cfg.refresh_time_three % 100, 0);

	// 一天刷新三次，取最近的时间返回
	if (temp1 < temp2)
	{
		if (temp1 < temp3)
		{
			*next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + temp1;
		}
		else 
		{
			*next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + temp3;
		}
		
	}
	else if (temp1 > temp2)
	{
		if (temp2 < temp3)
		{
			*next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + temp2;
		}
		else
		{
			*next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + temp3;
		}
	}
	else
	{
		return false;
	}

	return true;
}








