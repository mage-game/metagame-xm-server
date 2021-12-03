#include "suoyaotaconfig.hpp"

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

#include "item/itempool.h"

SuoYaoTaConfig::SuoYaoTaConfig() 
	: m_scene_id(0), m_enter_pos(0, 0), m_juanzhou_itemid(0), m_room_max_user(0), m_free_join_times(0), m_buy_times_cost(0),
	m_guwu_coin_cost(0), m_guwu_gold_cost(0), m_guwu_dur_time(0), m_guwu_gongji_addpercent(0),
	m_reward_double_cost(0), m_reward_three_cost(0)
{
}

SuoYaoTaConfig::~SuoYaoTaConfig()
{

}

bool SuoYaoTaConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "SuoYaoTaConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other_cfg");
		if (other_element.empty())
		{
			*err = configname + ": no [other_cfg].";
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
		PugiXmlNode atl_element = RootElement.child("activity_time_list");
		if (atl_element.empty())
		{
			*err = configname + ": no [activity_time_list].";
			return false;
		}

		iRet = this->InitActivityTimeCfg(atl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivityTimeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ll_element = RootElement.child("lv_list");
		if (ll_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitLvIntervalCfg(ll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLvIntervalCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode monster_element = RootElement.child("monster_list");
		if (monster_element.empty())
		{
			*err = configname + ": no [monster_list].";
			return false;
		}

		iRet = this->InitMonsterCfg(monster_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode task_element = RootElement.child("task_list");
		if (task_element.empty())
		{
			*err = configname + ": no [task_list].";
			return false;
		}

		iRet = this->InitTaskCfg(task_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTaskCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int SuoYaoTaConfig::GetEnterInfo(Posi *pos)
{
	if (NULL != pos) *pos = m_enter_pos;

	return m_scene_id;
}

unsigned int SuoYaoTaConfig::GetNextStatusTime(int curr_status)
{
	int next_status = SUOYAOTA_STATUS_TYPE_INVALID;

	if (SUOYAOTA_STATUS_TYPE_STOP == curr_status)
	{
		next_status = SUOYAOTA_STATUS_TYPE_STANDBY;
	}
	else 
	{
		next_status = (curr_status + 1) % SUOYAOTA_STATUS_TYPE_MAX;
	}

	int index = -1;
	int min_interval = 60 * 60;

	for (int i = 0; i < SUOYAOTA_HOUR_OPEN_TIME; i++)
	{
		int temp = EngineAdapter::Instance().NextHourInterval(m_activity_time_list[i].minute_list[next_status], 0);
		if (temp < min_interval)	
		{
			min_interval = temp;
			index = i;
		}
	}

	if (index >= 0 && index < SUOYAOTA_HOUR_OPEN_TIME)
	{
		return static_cast<unsigned int>(EngineAdapter::Instance().Time()) + min_interval;
	}

	return 60 * 60;
}

unsigned int SuoYaoTaConfig::GetNextStandbyTime()
{
	return this->GetNextStatusTime(SUOYAOTA_STATUS_TYPE_INVALID);
}

unsigned int SuoYaoTaConfig::GetNextStopTime()
{
	return this->GetNextStatusTime(SUOYAOTA_STATUS_TYPE_START);
}

int SuoYaoTaConfig::GetLvIntervalIdxByRoleLv(int role_lv)
{
	for (int i = 0; i < SUOYAOTA_LEVEL_INTERVAL_MAX; i++)
	{
		if (role_lv >= m_logic_cfg_list[i].min_lv && role_lv <= m_logic_cfg_list[i].max_lv)
		{
			return i;
		}
	}

	return 0;
}

SuoYaoTaLogicCfg * SuoYaoTaConfig::GetSuoYaoTaLogicCfgByLv(int role_lv)
{
	int idx = this->GetLvIntervalIdxByRoleLv(role_lv);
	if (idx >= 0 && idx < SUOYAOTA_LEVEL_INTERVAL_MAX)
	{
		return &m_logic_cfg_list[idx];
	}

	return NULL;
}

void SuoYaoTaConfig::GetDailyFindReward(int role_level, int *bind_coin, long long *exp)
{
	if (NULL == bind_coin || NULL == exp) return;

	SuoYaoTaLogicCfg *item_cfg = this->GetSuoYaoTaLogicCfgByLv(role_level);
	if (NULL != item_cfg)
	{
		*bind_coin = item_cfg->reward_base_coin;
		*exp = item_cfg->reward_base_exp;
	}
}

int SuoYaoTaConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_scene_id) || m_scene_id <= 0)
	{
		return -10;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", m_enter_pos.x) || m_enter_pos.x <= 0)
	{
		return -11;
	}
	if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", m_enter_pos.y) || m_enter_pos.y <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "juanzhou_itemid", m_juanzhou_itemid) || m_juanzhou_itemid <= 0)
	{
		return -12;
	}
	if (NULL == ITEMPOOL->GetItem(m_juanzhou_itemid))
	{
		return -122;
	}

	if (!PugiGetSubNodeValue(dataElement, "room_max_user", m_room_max_user) || m_room_max_user <= 0)
	{
		return -13;
	}
	if (!PugiGetSubNodeValue(dataElement, "free_join_times", m_free_join_times) || m_free_join_times <= 0)
	{
		return -14;
	}
	if (!PugiGetSubNodeValue(dataElement, "buy_times_cost", m_buy_times_cost) || m_buy_times_cost <= 0)
	{
		return -15;
	}
	if (!PugiGetSubNodeValue(dataElement, "guwu_coin_cost", m_guwu_coin_cost) || m_guwu_coin_cost <= 0)
	{
		return -16;
	}
	if (!PugiGetSubNodeValue(dataElement, "guwu_gold_cost", m_guwu_gold_cost) || m_guwu_gold_cost <= 0)
	{
		return -17;
	}
	if (!PugiGetSubNodeValue(dataElement, "guwu_dur_time", m_guwu_dur_time) || m_guwu_dur_time <= 0)
	{
		return -18;
	}
	if (!PugiGetSubNodeValue(dataElement, "guwu_gongji_addpercent", m_guwu_gongji_addpercent) || m_guwu_gongji_addpercent <= 0)
	{
		return -19;
	}
	if (!PugiGetSubNodeValue(dataElement, "reward_double_cost", m_reward_double_cost) || m_reward_double_cost <= 0)
	{
		return -21;
	}
	if (!PugiGetSubNodeValue(dataElement, "reward_three_cost", m_reward_three_cost) || m_reward_three_cost <= 0)
	{
		return -22;
	}

	return 0;
}

int SuoYaoTaConfig::InitActivityTimeCfg(PugiXmlNode RootElement)
{
	int time_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (time_count >= SUOYAOTA_HOUR_OPEN_TIME)
		{
			return -1;
		}

		SuoYaoTaTimeCfg *time_cfg = &m_activity_time_list[time_count];

		if (!PugiGetSubNodeValue(dataElement, "standby_min", time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_STANDBY]) || time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_STANDBY] < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "start_min", time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_START]) || time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_START] < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stop_min", time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_STOP]) || time_cfg->minute_list[SUOYAOTA_STATUS_TYPE_STOP] < 0)
		{
			return -4;
		}

		++ time_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}	


int SuoYaoTaConfig::InitLvIntervalCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int lv_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "lv_idx", lv_idx) || lv_idx < 0 || lv_idx >= SUOYAOTA_LEVEL_INTERVAL_MAX)
		{
			return -16;
		}

		SuoYaoTaLogicCfg *logic_cfg = &m_logic_cfg_list[lv_idx];
		if (0 != logic_cfg->min_lv)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_lv", logic_cfg->min_lv) || logic_cfg->min_lv <= 0)
		{
			return -17;
		}
		if (!PugiGetSubNodeValue(dataElement, "max_lv", logic_cfg->max_lv) || logic_cfg->max_lv <= 0)
		{
			return -18;
		}
		if (!PugiGetSubNodeValue(dataElement, "kill_monster_score", logic_cfg->kill_monster_score) || logic_cfg->kill_monster_score <= 0)
		{
			return -19;
		}
		if (!PugiGetSubNodeValue(dataElement, "kill_boss_score", logic_cfg->kill_boss_score) || logic_cfg->kill_boss_score <= 0)
		{
			return -27;
		}
		if (!PugiGetSubNodeValue(dataElement, "gathar_score", logic_cfg->gathar_score) || logic_cfg->gathar_score <= 0)
		{
			return -27;
		}
		if (!PugiGetSubNodeValue(dataElement, "reward_base_exp", logic_cfg->reward_base_exp) || logic_cfg->reward_base_exp <= 0)
		{
			return -28;
		}
		if (!PugiGetSubNodeValue(dataElement, "reward_base_coin", logic_cfg->reward_base_coin) || logic_cfg->reward_base_coin <= 0)
		{
			return -29;
		}
		if (!PugiGetSubNodeValue(dataElement, "total_score", logic_cfg->total_score) || logic_cfg->total_score <= 0)
		{
			return -31;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int SuoYaoTaConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int lv_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "lv_idx", lv_idx) || lv_idx < 0 || lv_idx >= SUOYAOTA_LEVEL_INTERVAL_MAX)
		{
			return -16;
		}

		SuoYaoTaLogicCfg *logic_cfg = &m_logic_cfg_list[lv_idx];

		if (logic_cfg->monster_pt_count >= SUOYAOTA_GEN_MONTER_PT_MAX)
		{
			return -18;
		}

		SuoYaoTaLogicCfg::MonsterPt *pt_cfg = &logic_cfg->monster_list[logic_cfg->monster_pt_count];

		if (!PugiGetSubNodeValue(dataElement, "monster_id", pt_cfg->monster_id) || pt_cfg->monster_id <= 0)
		{
			return -19;
		}
		if (!MONSTERPOOL->IsMonsterExist(pt_cfg->monster_id))
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_pos_x", pt_cfg->flush_pos.x) || pt_cfg->flush_pos.x <= 0)
		{
			return -22;
		}
		if (!PugiGetSubNodeValue(dataElement, "flush_pos_y", pt_cfg->flush_pos.y) || pt_cfg->flush_pos.y <= 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_interval", pt_cfg->flush_interval) || pt_cfg->flush_interval <= 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_range", pt_cfg->rand_range) || pt_cfg->rand_range <= 0)
		{
			return -24;
		}

		++ logic_cfg->monster_pt_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int SuoYaoTaConfig::InitTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int lv_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "lv_idx", lv_idx) || lv_idx < 0 || lv_idx >= SUOYAOTA_LEVEL_INTERVAL_MAX)
		{
			return -16;
		}

		SuoYaoTaLogicCfg *logic_cfg = &m_logic_cfg_list[lv_idx];
		if (logic_cfg->task_count >= SUOYAOTA_TASK_MAX)
		{
			return -18;
		}
			
		SuoYaoTaTask *task_cfg = &logic_cfg->task_list[logic_cfg->task_count];

		if (!PugiGetSubNodeValue(dataElement, "task_type", task_cfg->task_type) || task_cfg->task_type <= SUOYAOTA_TASK_TYPE_INVALID || task_cfg->task_type >= SUOYAOTA_TASK_TYPE_MAX)
		{
			return -21;
		}
		if (!PugiGetSubNodeValue(dataElement, "param_id", task_cfg->param_id) || task_cfg->param_id <= 0)
		{
			return -21;
		}
		if (!PugiGetSubNodeValue(dataElement, "param_max", task_cfg->param_max) || task_cfg->param_max <= 0)
		{
			return -21;
		}

		++ logic_cfg->task_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}



















