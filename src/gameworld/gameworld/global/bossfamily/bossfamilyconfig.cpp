#include "bossfamilyconfig.hpp"
#include <memory.h>
#include <stdlib.h>
#include "engineadapter.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "scene/worldshadow/worldshadow.hpp"

#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "servercommon/errornum.h"
#include "item/itempool.h"
#include <set>
#include <string>
#include <time.h>
#include <algorithm>


BossFamilyConfig::BossFamilyConfig():m_dabao_buy_cost_count(0)
{
}

BossFamilyConfig::~BossFamilyConfig()
{
}

bool BossFamilyConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "BossFamilyConfig", *err);

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

		iRet = this->InitBossFamilyOtherCfg(worldboss_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBossFamilyOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	//疲劳值
	{
		PugiXmlNode weary_element = RootElement.child("weary");
		if (weary_element.empty())
		{
			*err = configname + ": no [weary].";
			return false;
		}

		iRet = this->InitWearyCfg(weary_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitWearyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	//boss疲劳价格
	{
		PugiXmlNode weary_cost_element = RootElement.child("weary_cost");
		if (weary_cost_element.empty())
		{
			*err = configname + ": no [weary_cost].";
			return false;
		}

		iRet = this->InitWearyCostCfg(weary_cost_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitWearyCostCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// boss之家
	{
		PugiXmlNode bossfamily_element = RootElement.child("boss_family");
		if (bossfamily_element.empty())
		{
			*err = configname + ": no [boss_family].";
			return false;
		}

		iRet = this->InitBossFamilyBossCfg(bossfamily_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBossFamilyBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘窟boss
	{
		PugiXmlNode miku_element = RootElement.child("miku_boss");
		if (miku_element.empty())
		{
			*err = configname + ": no [miku_boss].";
			return false;
		}

		iRet = this->InitBossFamilyMikuBossCfg(miku_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBossFamilyMikuBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 进入条件
	{
		PugiXmlNode enter_condition_element = RootElement.child("enter_condition");
		if (enter_condition_element.empty())
		{
			*err = configname + ": no [enter_condition].";
			return false;
		}

		iRet = this->InitBossFamilyEnterCfg(enter_condition_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBossFamilyEnterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 打宝购买配置
	{
		PugiXmlNode enter_dabao_element = RootElement.child("dabao_cost");
		if (enter_dabao_element.empty())
		{
			*err = configname + ": no [dabao_cost].";
			return false;
		}

		iRet = this->InitDabaoBuyCostCfg(enter_dabao_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDabaoBuyCostCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 打宝boss
	{
		PugiXmlNode dabao_element = RootElement.child("dabao_boss");
		if (dabao_element.empty())
		{
			*err = configname + ": no [dabao_boss].";
			return false;
		}

		iRet = this->InitDabaoBossCfg(dabao_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDabaoBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 活跃boss
	{
		PugiXmlNode dabao_element = RootElement.child("active_boss");
		if (dabao_element.empty())
		{
			*err = configname + ": no [active_boss].";
			return false;
		}

		iRet = this->InitActiveBossCfg(dabao_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActiveBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘窟小怪
	{
		PugiXmlNode mikumonster_element = RootElement.child("miku_monster");
		if (mikumonster_element.empty())
		{
			*err = configname + ": no [miku_monster].";
			return false;
		}

		iRet = this->InitMikuMonsterCfg(mikumonster_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMikuMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// boss特殊刷新
	{
		PugiXmlNode mikumonster_element = RootElement.child("boss_special_refresh");
		if (mikumonster_element.empty())
		{
			*err = configname + ": no [boss_special_refresh].";
			return false;
		}

		iRet = this->InitBossSpecialRefreshCfg(mikumonster_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBossSpecialRefreshCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘藏boss其他信息
	{
		PugiXmlNode mikumonster_element = RootElement.child("precious_boss_other");
		if (mikumonster_element.empty())
		{
			*err = configname + ": no [precious_boss_other].";
			return false;
		}

		iRet = this->InitPreciousBossOtherCfg(mikumonster_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPreciousBossOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘藏boss信息
	{
		PugiXmlNode element = RootElement.child("precious_boss_monster");
		if (element.empty())
		{
			*err = configname + ": no [precious_boss_other].";
			return false;
		}

		iRet = this->InitPreciousMonsterCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPreciousMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘藏boss任务
	{
		PugiXmlNode element = RootElement.child("precious_boss_task");
		if (element.empty())
		{
			*err = configname + ": no [precious_boss_task].";
			return false;
		}

		iRet = this->InitPreciousBossTaskCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPreciousBossTaskCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘藏boss任务
	{
		PugiXmlNode element = RootElement.child("precious_boss_reward");
		if (element.empty())
		{
			*err = configname + ": no [precious_boss_reward].";
			return false;
		}

		iRet = this->InitPreciousBossRewardCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPreciousBossRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 秘藏boss任务
	{
		PugiXmlNode element = RootElement.child("precious_boss_pos");
		if (element.empty())
		{
			*err = configname + ": no [precious_boss_pos].";
			return false;
		}

		iRet = this->InitPreciousBossPosCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPreciousBossPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 活跃boss排行奖励
	{
		PugiXmlNode element = RootElement.child("active_boss_rank_reward");
		if (element.empty())
		{
			*err = configname + ": no [active_boss_rank_reward].";
			return false;
		}

		iRet = this->InitBossHurtRankCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBossHurtRankCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 云游boss配置
	{
		PugiXmlNode element = RootElement.child("yunyou_boss");
		if (element.empty())
		{
			*err = configname + ": no [yunyou_boss].";
			return false;
		}

		iRet = this->InitYunyouBossCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitYunyouBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 云游boss刷新配置
	{
		PugiXmlNode element = RootElement.child("yunyou_boss_refresh_time");
		if (element.empty())
		{
			*err = configname + ": no [yunyou_boss_refresh_time].";
			return false;
		}

		iRet = this->InitYunyouBossRefreshCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitYunyouBossRefreshCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const std::vector<PreciousMonsterCfg> & BossFamilyConfig::GetPreciousMonsterWorldLevelCfg()
{
	static std::vector<PreciousMonsterCfg> empty_vec;
	int world_level = WorldShadow::Instance().GetWorldLevel();
	std::vector<PreciousMonsterWorldLevelCfg>::iterator iter = m_precious_monster_world_level_cfg_vec.begin();
	for (; iter != m_precious_monster_world_level_cfg_vec.end(); ++iter)
	{
		if (world_level > iter->world_level)
		{
			continue;
		}

		return iter->world_level_cfg_vec;
	}

	return empty_vec;
}

const std::map<int, PreciousBossRewardCfg> & BossFamilyConfig::GetPreciousBossRewardCfg(int level)
{
	static std::map<int, PreciousBossRewardCfg> empty_map;
	std::vector<PreciousBossRewardLevelCfg>::iterator iter = m_precious_boss_reward_level_cfg_vec.begin();
	for (; iter != m_precious_boss_reward_level_cfg_vec.end(); ++iter)
	{
		if (level > iter->level)
		{
			continue;
		}

		return iter->reward_cfg_map;
	}

	return empty_map;
}

const BossFamilyBossInfoCfg * BossFamilyConfig::GetBossFamilyBossInfo(int boss_id)
{
	std::map<int, BossFamilyBossInfoCfg>::const_iterator it = m_bossfamily_boss_map.find(boss_id);
	if(it != m_bossfamily_boss_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const BossFamilyMikuBossInfoCfg * BossFamilyConfig::GetMikuBossInfo(int boss_id)
{
	std::map<int, BossFamilyMikuBossInfoCfg>::const_iterator it = m_miku_boss_map.find(boss_id);
	if(it != m_miku_boss_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const BossFamilyEnterConditionCfg * BossFamilyConfig::GetEnterConditionInfo(Role *role, int scene_id)
{
	if (NULL == role) return NULL;

	std::map<int, BossFamilyEnterConditionCfg>::const_iterator it = m_enter_conditon_map.find(scene_id);
	if(it != m_enter_conditon_map.end())
	{
		if (role->GetLevel() < it->second.min_lv)
		{
			role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
			return NULL;
		}

		return &it->second;
	}

	return NULL;
}

int BossFamilyConfig::GetDabaoBossBuyCountCost(int today_buy_count)
{
	for (int i = m_dabao_buy_cost_count - 1; i >= 0 && i < MAX_DABAO_BUY_COST_COUNT; i--)
	{
		if (today_buy_count >= m_dabao_buy_cost_list[i].times)
		{
			return m_dabao_buy_cost_list[i].cost_gold;
		}
	}

	return 0;
}

const DabaoBossInfoCfg * BossFamilyConfig::GetDabaoBossInfo(int boss_id)
{
	std::map<int, DabaoBossInfoCfg>::const_iterator it = m_dabao_boss_map.find(boss_id);
	if(it != m_dabao_boss_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const DabaoBuyCostCfg * BossFamilyConfig::GetDabaoBuyCostCfg(int times)
{
	if (times < 0 || times >= m_dabao_buy_cost_count || times >= MAX_DABAO_BUY_COST_COUNT)
	{
		return nullptr;
	}
	return &m_dabao_buy_cost_list[times];
}

const ActiveBossInfoCfg * BossFamilyConfig::GetActiveBossInfo(int boss_id)
{
	std::map<int, ActiveBossInfoCfg>::const_iterator it = m_active_boss_map.find(boss_id);
	if(it != m_active_boss_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const MikuMonsterInfoCfg * BossFamilyConfig::GetMikuMonsterInfo(int monster_id)
{
	std::map<int, MikuMonsterInfoCfg>::const_iterator it = m_miku_monster_map.find(monster_id);
	if (it != m_miku_monster_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const PreciousBossTaskCfg * BossFamilyConfig::GetPreciousBossTaskCfg(int task_param)
{
	std::map<int, PreciousBossTaskCfg>::iterator iter = m_precious_boss_task_cfg_map.find(task_param);
	if (iter != m_precious_boss_task_cfg_map.end())
	{
		return &(iter->second);
	}

	return NULL;
}

int BossFamilyConfig::InitBossFamilyOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "refresh_interval", m_other_cfg.refresh_interval) || m_other_cfg.refresh_interval < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "weary_upper_limit", m_other_cfg.weary_upper_limit) || m_other_cfg.weary_upper_limit < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_add_weary", m_other_cfg.kill_add_weary) || m_other_cfg.kill_add_weary < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "dabao_free_times", m_other_cfg.dabao_free_times) || m_other_cfg.dabao_free_times < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "max_value", m_other_cfg.max_value) || m_other_cfg.max_value < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_boss_value", m_other_cfg.kill_boss_value) || m_other_cfg.kill_boss_value < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_monster_value", m_other_cfg.kill_monster_value) || m_other_cfg.kill_monster_value < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "leave_scene_id", m_other_cfg.leave_scene_id) || m_other_cfg.leave_scene_id <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "leave_pos_x", m_other_cfg.leave_pos.x) || m_other_cfg.leave_pos.x < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "leave_pos_y", m_other_cfg.leave_pos.y) || m_other_cfg.leave_pos.y < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "dabao_kick_delay_time", m_other_cfg.dabao_kick_delay_time) || m_other_cfg.dabao_kick_delay_time < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "active_kick_delay_time", m_other_cfg.active_kick_delay_time) || m_other_cfg.active_kick_delay_time < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "active_max_weary", m_other_cfg.active_max_weary) || m_other_cfg.active_max_weary < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "dabao_next_add_angry_interval_s", m_other_cfg.dabao_add_angry_interval_s) || m_other_cfg.dabao_add_angry_interval_s <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "dabao_angry_value", m_other_cfg.dabao_add_angry_value) || m_other_cfg.dabao_add_angry_value < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "dabao_angry_value_die", m_other_cfg.dabao_angry_value_die) || m_other_cfg.dabao_angry_value_die < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "comeback_timeout_min", m_other_cfg.comeback_timeout_min) || m_other_cfg.comeback_timeout_min < 0)
	{
		return -18;
	}

	return 0;
}

int BossFamilyConfig::InitBossFamilyBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_family_boss_scene_id_set.clear();
	while (!dataElement.empty())
	{
		BossFamilyBossInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "bossID", cfg.bossID) || cfg.bossID <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.bossID))
		{
			return -1;
		}

		std::map<int, BossFamilyBossInfoCfg>::iterator iter = m_bossfamily_boss_map.find(cfg.bossID);	// 没有重复的BOSSID
		if (m_bossfamily_boss_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsBossFamilyScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);
		m_family_boss_scene_id_set.insert(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", cfg.refresh_interval) || cfg.refresh_interval < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg.min_lv) || cfg.min_lv < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg.max_lv) || cfg.max_lv < 0 || cfg.max_lv < cfg.min_lv)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_cross", cfg.is_cross) || (cfg.is_cross != 0 && cfg.is_cross != 1))
		{
			return -9;
		}

		m_bossfamily_boss_map[cfg.bossID] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitBossFamilyMikuBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_miku_boss_scene_id_set.clear();
	while (!dataElement.empty())
	{
		BossFamilyMikuBossInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "bossID", cfg.bossID) || cfg.bossID <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.bossID))
		{
			return -1;
		}

		std::map<int, BossFamilyMikuBossInfoCfg>::iterator iter = m_miku_boss_map.find(cfg.bossID);	// 没有重复的BOSSID
		if (m_miku_boss_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsBossDongKuScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);
		m_miku_boss_scene_id_set.insert(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", cfg.refresh_interval) || cfg.refresh_interval < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg.min_lv) || cfg.min_lv < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg.max_lv) || cfg.max_lv < 0 || cfg.max_lv < cfg.min_lv)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_peace", cfg.is_peace) || cfg.is_peace < 0)
		{
			return -9;
		}

		m_miku_boss_map[cfg.bossID] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitBossFamilyEnterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		BossFamilyEnterConditionCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -1;
		}

		std::map<int, BossFamilyEnterConditionCfg>::iterator iter = m_enter_conditon_map.find(cfg.scene_id);	
		if (m_enter_conditon_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_vip_level", cfg.free_vip_level) || cfg.free_vip_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_x", cfg.enter_pos.x) || cfg.enter_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_y", cfg.enter_pos.y) || cfg.enter_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_item_id", cfg.need_item_id) || cfg.need_item_id < 0)
		{
			return -6;
		}

		if (cfg.need_item_id != 0 && NULL == ITEMPOOL->GetItem(cfg.need_item_id))
		{
			return -100001;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_item_num", cfg.need_item_num) || cfg.need_item_num < 0 )
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_gold", cfg.cost_gold) || cfg.cost_gold < 0 )
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg.min_lv) || cfg.min_lv < 0 )
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg.max_lv) || cfg.max_lv < 0 )
		{
			return -10;
		}

		m_enter_conditon_map[cfg.scene_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitDabaoBuyCostCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_dabao_buy_cost_count = 0;

	while (!dataElement.empty())
	{
		if (m_dabao_buy_cost_count >= MAX_DABAO_BUY_COST_COUNT)
		{
			return -100;
		}

		DabaoBuyCostCfg &cfg = m_dabao_buy_cost_list[m_dabao_buy_cost_count];

		if (!PugiGetSubNodeValue(dataElement, "times", cfg.times) || cfg.times < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_gold", cfg.cost_gold) || cfg.cost_gold < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume_item_id", cfg.consume_item_id) || nullptr == ITEMPOOL->GetItem(cfg.consume_item_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume_item_num", cfg.consume_item_num) || cfg.consume_item_num < 0)
		{
			return -4;
		}

		m_dabao_buy_cost_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitDabaoBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_dabao_boss_scene_id_set.clear();
	while (!dataElement.empty())
	{
		DabaoBossInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "bossID", cfg.bossID) || cfg.bossID <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.bossID))
		{
			return -1;
		}

		std::map<int, DabaoBossInfoCfg>::iterator iter = m_dabao_boss_map.find(cfg.bossID);	// 没有重复的BOSSID
		if (m_dabao_boss_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsDabaoBossScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);
		m_dabao_boss_scene_id_set.insert(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "start_refresh_time", cfg.start_refresh_time) || cfg.start_refresh_time < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "end_refresh_time", cfg.end_refresh_time) || cfg.end_refresh_time < 0 || cfg.end_refresh_time <= cfg.start_refresh_time)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", cfg.refresh_interval) || cfg.refresh_interval <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "kill_boss_value", cfg.kill_boss_value) || cfg.kill_boss_value < 0)
		{
			return -9;
		}

		m_dabao_boss_map[cfg.bossID] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitActiveBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_active_boss_scene_id_set.clear();
	while (!dataElement.empty())
	{
		ActiveBossInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "bossID", cfg.bossID) || cfg.bossID <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.bossID))
		{
			return -1;
		}

		std::map<int, ActiveBossInfoCfg>::iterator iter = m_active_boss_map.find(cfg.bossID);	// 没有重复的BOSSID
		if (m_active_boss_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsActiveBossScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);
		m_active_boss_scene_id_set.insert(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_weary", cfg.add_weary) || cfg.add_weary < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg.min_lv) || cfg.min_lv < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg.max_lv) || cfg.max_lv < 0 || cfg.max_lv < cfg.min_lv)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval",cfg.refresh_interval) || cfg.refresh_interval < 0)
		{
			return -9;
		}
		m_active_boss_map[cfg.bossID] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitMikuMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		MikuMonsterInfoCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.monster_id) || cfg.monster_id <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.monster_id))
		{
			return -1;
		}

		std::map<int, MikuMonsterInfoCfg>::iterator iter = m_miku_monster_map.find(cfg.monster_id);	// 没有重复的Monster
		if (m_miku_monster_map.end() != iter)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0 || !Scene::IsBossDongKuScene(cfg.scene_id))
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "start_refresh_time", cfg.start_refresh_time) || cfg.start_refresh_time < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "end_refresh_time", cfg.end_refresh_time) || cfg.end_refresh_time < 0 || cfg.end_refresh_time <= cfg.start_refresh_time)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", cfg.refresh_interval) || cfg.refresh_interval <= 0)
		{
			return -8;
		}

		m_miku_monster_map[cfg.monster_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const BossSpecialRefreshCfg* BossFamilyConfig::GetBossSpecialRefreshCfgByBossId(int boss_id)
{
	const std::map<int, BossSpecialRefreshCfg>::const_iterator it = m_boss_special_refresh_cfg_map.find(boss_id);
	if (it != m_boss_special_refresh_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int BossFamilyConfig::InitBossSpecialRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int boss_id;
		if (!PugiGetSubNodeValue(dataElement, "boss_id", boss_id) || boss_id <= 0)
		{
			return -1;
		}

		BossSpecialRefreshCfg &cfg = m_boss_special_refresh_cfg_map[boss_id];
		cfg.boss_id = boss_id;

		if (!PugiGetSubNodeValue(dataElement, "min_role_level", cfg.min_role_level) || cfg.min_role_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_role_level", cfg.max_role_level) || cfg.max_role_level < cfg.min_role_level)
		{
			return -3;
		}

		if (cfg.item_count >= BossSpecialRefreshCfg::ITEM_COUNT_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_num", cfg.item_list[cfg.item_count].role_num) || cfg.item_list[cfg.item_count].role_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_time_s", cfg.item_list[cfg.item_count].flush_time_s) || cfg.item_list[cfg.item_count].flush_time_s <= 0)
		{
			return -6;
		}

		cfg.item_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitPreciousBossOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "precious_boss_limit_level", m_precious_boss_other_cfg.precious_boss_limit_level) || m_precious_boss_other_cfg.precious_boss_limit_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "precious_boss_scene_id", m_precious_boss_other_cfg.precious_boss_scene_id) || m_precious_boss_other_cfg.precious_boss_scene_id <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "team_member_reward_precent", m_precious_boss_other_cfg.team_member_reward_precent) || m_precious_boss_other_cfg.team_member_reward_precent < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "boss_type_count", m_precious_boss_other_cfg.boss_type_count) || m_precious_boss_other_cfg.boss_type_count <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "monster_type_count", m_precious_boss_other_cfg.monster_type_count) || m_precious_boss_other_cfg.monster_type_count <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_refresh_interval", m_precious_boss_other_cfg.gather_refresh_interval) || m_precious_boss_other_cfg.gather_refresh_interval <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_id", m_precious_boss_other_cfg.gather_id) || m_precious_boss_other_cfg.gather_id <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_cost_time", m_precious_boss_other_cfg.gather_cost_time) || m_precious_boss_other_cfg.gather_cost_time <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_pos_start_seq", m_precious_boss_other_cfg.gather_pos_start_seq) || m_precious_boss_other_cfg.gather_pos_start_seq < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_pos_end_seq", m_precious_boss_other_cfg.gather_pos_end_seq) || m_precious_boss_other_cfg.gather_pos_end_seq < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "monster_pos_start_seq", m_precious_boss_other_cfg.monster_pos_start_seq) || m_precious_boss_other_cfg.monster_pos_start_seq < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "monster_pos_end_seq", m_precious_boss_other_cfg.monster_pos_end_seq) || m_precious_boss_other_cfg.monster_pos_end_seq < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "boss_pos_start_seq", m_precious_boss_other_cfg.boss_pos_start_seq) || m_precious_boss_other_cfg.boss_pos_start_seq < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "boss_pos_end_seq", m_precious_boss_other_cfg.boss_pos_end_seq) || m_precious_boss_other_cfg.boss_pos_end_seq < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "forbid_pk_pos_x", m_precious_boss_other_cfg.forbid_pk_pos.x) || m_precious_boss_other_cfg.forbid_pk_pos.x <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "forbid_pk_pos_y", m_precious_boss_other_cfg.forbid_pk_pos.y) || m_precious_boss_other_cfg.forbid_pk_pos.y <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "forbid_pk_radius", m_precious_boss_other_cfg.forbid_pk_radius) || m_precious_boss_other_cfg.forbid_pk_radius <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_num", m_precious_boss_other_cfg.gather_num) || m_precious_boss_other_cfg.gather_num <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "skip_task_consume", m_precious_boss_other_cfg.skip_task_consume) || m_precious_boss_other_cfg.skip_task_consume <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "skip_task_limit_level", m_precious_boss_other_cfg.skip_task_limit_level) || m_precious_boss_other_cfg.skip_task_limit_level <= 0)
	{
		return -20;
	}

	return 0;
}

int BossFamilyConfig::InitPreciousMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_world_level = 0;
	int stage = 0;

	while (!dataElement.empty())
	{
		PreciousMonsterWorldLevelCfg level_cfg;
		PreciousMonsterCfg monster_cfg;

		if (!PugiGetSubNodeValue(dataElement, "world_level", level_cfg.world_level) || level_cfg.world_level <= 0)
		{
			return -1;
		}

		if (last_world_level != level_cfg.world_level)
		{
			last_world_level = level_cfg.world_level;
			++stage;
		}

		if (last_world_level > level_cfg.world_level)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_type", monster_cfg.monster_type) || monster_cfg.monster_type < PreciousMonsterCfg::PRECIOUS_MONSTER_TYPE_BOSS
			|| monster_cfg.monster_type >= PreciousMonsterCfg::PRECIOUS_MONSTER_TYPE_MAX_TYPE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_cfg.monster_id) || monster_cfg.monster_id <= 0 || NULL == MONSTERPOOL->GetMonsterParam(monster_cfg.monster_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", monster_cfg.seq) || monster_cfg.seq < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", monster_cfg.refresh_interval) || monster_cfg.refresh_interval <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_max_count", monster_cfg.refresh_max_count) || monster_cfg.refresh_max_count <= 0)
		{
			return -6;
		}
		
		int size = static_cast<int>(m_precious_monster_world_level_cfg_vec.size());
		if (stage > size)
		{
			m_precious_monster_world_level_cfg_vec.push_back(level_cfg);
		}

		m_precious_monster_world_level_cfg_vec[m_precious_monster_world_level_cfg_vec.size() - 1].world_level_cfg_vec.push_back(monster_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitPreciousBossTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		PreciousBossTaskCfg task_cfg;

		if (!PugiGetSubNodeValue(dataElement, "task_id", task_cfg.task_id) || task_cfg.task_id < 0)
		{
			return -1000;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "task_type", task_cfg.task_type) || task_cfg.task_type < PreciousBossTaskCfg::PRECIOUS_BOSS_TASK_TYPE_BOSS
			|| task_cfg.task_type >= PreciousBossTaskCfg::PRECIOUS_BOSS_TASK_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_condition", task_cfg.task_condition) || task_cfg.task_condition <= 0)
		{
			return -2;
		}
		
		if (m_precious_boss_task_cfg_map.find(task_cfg.task_type) == m_precious_boss_task_cfg_map.end())
		{
			m_precious_boss_task_cfg_map[task_cfg.task_type] = task_cfg;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitPreciousBossRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int stage = 0;

	while (!dataElement.empty())
	{
		PreciousBossRewardLevelCfg level_cfg;
		PreciousBossRewardCfg reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "level", level_cfg.level) || level_cfg.level <= 0)
		{
			return -1;
		}

		if (last_level != level_cfg.level)
		{
			last_level = level_cfg.level;
			++stage;
		}

		if (last_level > level_cfg.level)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_type", reward_cfg.reward_type) || reward_cfg.reward_type < PreciousBossRewardCfg::PRECIOUS_BOSS_REWARD_TYPE_TASK
			|| reward_cfg.reward_type >= PreciousBossRewardCfg::PRECIOUS_BOSS_REWARD_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_param", reward_cfg.reward_param) || reward_cfg.reward_param < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_score", reward_cfg.reward_score) || reward_cfg.reward_score <= 0)
		{
			return -4;
		}

		int size = static_cast<int>(m_precious_boss_reward_level_cfg_vec.size());
		if (stage > size)
		{
			m_precious_boss_reward_level_cfg_vec.push_back(level_cfg);
		}

		m_precious_boss_reward_level_cfg_vec[m_precious_boss_reward_level_cfg_vec.size() - 1].reward_cfg_map[reward_cfg.reward_param] = reward_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitPreciousBossPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		PreciousBossRefreshPosCfg pos_cfg;

		if (!PugiGetSubNodeValue(dataElement, "pos_type", pos_cfg.pos_type) || pos_cfg.pos_type < PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_BOSS
			|| pos_cfg.pos_type >= PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_cfg.pos.x) || pos_cfg.pos.x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_cfg.pos.y) || pos_cfg.pos.y <= 0)
		{
			return -3;
		}
		
		std::vector<Posi> &pos_vec = m_precious_boss_pos_cfg[pos_cfg.pos_type];
		pos_vec.push_back(pos_cfg.pos);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitWearyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_weary_map.clear();
	
	while (!dataElement.empty())
	{
		WearyCfg weary_cfg;
		
		if (!PugiGetSubNodeValue(dataElement, "weary_tpye", weary_cfg.weary_tpye) || weary_cfg.kill_add_weary < 0 || m_weary_map.find(weary_cfg.weary_tpye) != m_weary_map.end())
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "weary_limit", weary_cfg.weary_limit) || weary_cfg.weary_limit < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "kill_add_weary", weary_cfg.kill_add_weary) || weary_cfg.kill_add_weary < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_give_weary", weary_cfg.buy_give_weary) || weary_cfg.buy_give_weary < 0)
		{
			return -4;
		}

		m_weary_map[weary_cfg.weary_tpye] = weary_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitWearyCostCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_buy_weary_cost_map.clear();
	while (!dataElement.empty())
	{
		
		WearyCostCfg weary_cost_cfg;
		if (!PugiGetSubNodeValue(dataElement, "buy_times", weary_cost_cfg.buy_times) || weary_cost_cfg.buy_times < 0 || m_buy_weary_cost_map.find(weary_cost_cfg.buy_times) != m_buy_weary_cost_map.end())
		{
			return -1;
		}

		
		if (!PugiGetSubNodeValue(dataElement, "cost", weary_cost_cfg.cost) || weary_cost_cfg.cost < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_tpye", weary_cost_cfg.cost_tpye) || weary_cost_cfg.cost_tpye < 0)
		{
			return -3;
		}

		m_buy_weary_cost_map[weary_cost_cfg.buy_times] = weary_cost_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitBossHurtRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int monster_id = 0;
	int last_rank = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "bossid", monster_id) || monster_id <= 0)
		{
			return -1;
		}

		BossFamilyBossHurtRankCfg &hurt_rank_cfg = m_boss_hurt_rank_cfg_map[monster_id];

		if (hurt_rank_cfg.monster_id != monster_id)
		{
			last_rank = 0;
		}
		hurt_rank_cfg.monster_id = monster_id;

		if (hurt_rank_cfg.rank_cfg_count >= MAX_BOSS_FAMILY_BOSS_RANK_CFG_COUNT)
		{
			return -100;
		}

		BossFamilyBossHurtRankCfg::RankItem &rank_item = hurt_rank_cfg.rank_item_list[hurt_rank_cfg.rank_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rank", rank_item.rank) || rank_item.rank <= 0 || rank_item.rank <= last_rank)
		{
			return -2;
		}
		last_rank = rank_item.rank;

		char temp_str[64] = { 0 };

		for (int i = 1; i < PROF_TYPE_PROF_NO_LIMIT; ++i)
		{
			sprintf(temp_str, "reward_item_%d", i);
			rank_item.item_count_list[i] = ItemConfigData::ReadConfigList(data_element, temp_str, rank_item.item_list[i], MAX_ATTACHMENT_ITEM_NUM);
			if (rank_item.item_count_list[i] <= 0)
			{
				return -3;
			}
		}

		++hurt_rank_cfg.rank_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitYunyouBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		YunyouBossBornCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -1;
		}

		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || cfg.boss_id <= 0 || NULL == MONSTERPOOL->GetMonsterParam(cfg.boss_id))
		{
			return -2;
		}

		if (!cfg.born_pos.ReadConfig(data_element, "born_x"))
		{
			return -3;
		}

		m_yunyou_boss_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BossFamilyConfig::InitYunyouBossRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		int flush_time = 0;
		if (!PugiGetSubNodeValue(data_element, "flush_time", flush_time) || flush_time <= 0 || flush_time >= 2400)
		{
			return -1;
		}

		m_yunyou_refresh_time_list.push_back(flush_time);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const WearyCfg * BossFamilyConfig::GetWearyCfg(int weary_tpye)
{
	std::map<int, WearyCfg>::const_iterator it = m_weary_map.find(weary_tpye);
	if (it != m_weary_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const WearyCostCfg * BossFamilyConfig::GetBuyWearyCostCfg(int buy_times)
{
	std::map<int, WearyCostCfg>::const_iterator it = m_buy_weary_cost_map.find(buy_times);
	if (it != m_buy_weary_cost_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const BossFamilyBossHurtRankCfg::RankItem * BossFamilyConfig::GetBossHurtRankCfg(int monster_id, int rank)
{
	BossFamilyBossHurtRankCfgMapIt it = m_boss_hurt_rank_cfg_map.find(monster_id);
	if (it != m_boss_hurt_rank_cfg_map.end())
	{
		BossFamilyBossHurtRankCfg &hurt_rank_cfg = it->second;

		for (int j = 0; j < hurt_rank_cfg.rank_cfg_count && j < MAX_BOSS_FAMILY_BOSS_RANK_CFG_COUNT; ++j)
		{
			if (rank <= hurt_rank_cfg.rank_item_list[j].rank)
			{
				return &hurt_rank_cfg.rank_item_list[j];
			}
		}
	}

	return nullptr;
}

bool BossFamilyConfig::IsCrossVipBossScene(int scene_id)
{
	for (std::map<int, BossFamilyBossInfoCfg>::iterator iter = m_bossfamily_boss_map.begin(); iter != m_bossfamily_boss_map.end(); ++iter)
	{
		if (iter->second.scene_id == scene_id && iter->second.is_cross > 0)
		{
			return true;
		}
	}

	return false;
}

bool BossFamilyConfig::GetNextRefreshTimeOfYunyouBoss(unsigned int *next_refresh_time) const
{
	if (NULL == next_refresh_time) return false;

	int next_refresh_interval = SECOND_PER_DAY;
	int tmp_interval = 0;
	for (auto &it : m_yunyou_refresh_time_list)
	{
		tmp_interval = EngineAdapter::Instance().NextDayInterval(it / 100, it % 100, 0);
		if (tmp_interval < next_refresh_interval)
		{
			next_refresh_interval = tmp_interval;
		}
	}

	*next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + next_refresh_interval;

	return true;
}

bool BossFamilyConfig::IsYunyouBoss(int boss_id)
{
	if (boss_id <= 0)
	{
		return false;
	}

	for (auto &it : m_yunyou_boss_vec)
	{
		if (it.boss_id == boss_id)
		{
			return true;
		}
	}

	return false;
}
