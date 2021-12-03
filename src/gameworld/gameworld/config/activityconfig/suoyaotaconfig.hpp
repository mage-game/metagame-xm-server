#ifndef _SUOYAOTA_CONFIG_HPP_
#define _SUOYAOTA_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

enum SUOYAOTA_STATUS_TYPE
{
	SUOYAOTA_STATUS_TYPE_INVALID = 0,

	SUOYAOTA_STATUS_TYPE_STANDBY,
	SUOYAOTA_STATUS_TYPE_START,
	SUOYAOTA_STATUS_TYPE_STOP,

	SUOYAOTA_STATUS_TYPE_MAX,
};

static const int SUOYAOTA_HOUR_OPEN_TIME = 2;						// 每个小时刷两场

struct SuoYaoTaTimeCfg
{
	SuoYaoTaTimeCfg()
	{
		memset(minute_list, 0, sizeof(minute_list));
	}

	int minute_list[SUOYAOTA_STATUS_TYPE_MAX];
};

enum SUOYAOTA_TASK_TYPE
{
	SUOYAOTA_TASK_TYPE_INVALID = 0,

	SUOYAOTA_TASK_TYPE_KILL_MONSTER,
	SUOYAOTA_TASK_TYPE_GATHAR,

	SUOYAOTA_TASK_TYPE_MAX,
};

struct SuoYaoTaTask
{
	SuoYaoTaTask() : task_type(SUOYAOTA_TASK_TYPE_INVALID), param_id(0), param_max(0) {}

	int task_type;
	int param_id;
	int param_max;
};

static const int SUOYAOTA_TASK_MAX = 4;

static const int SUOYAOTA_GEN_MONTER_PT_MAX = 64;

struct SuoYaoTaLogicCfg
{
	SuoYaoTaLogicCfg() : min_lv(0), max_lv(0), kill_monster_score(0), kill_boss_score(0), gathar_score(0),
		reward_base_exp(0), reward_base_coin(0), total_score(0), monster_pt_count(0), task_count(0)
	{

	}

	int min_lv;
	int max_lv;

	int kill_monster_score;
	int kill_boss_score;
	int gathar_score;

	long long reward_base_exp;
	int reward_base_coin;
	int total_score;

	struct MonsterPt
	{
		MonsterPt() : monster_id(0), flush_pos(0, 0), flush_interval(0), rand_range(0) {}

		int monster_id;
		Posi flush_pos;
		int flush_interval;
		int rand_range;
	};

	int monster_pt_count;
	MonsterPt monster_list[SUOYAOTA_GEN_MONTER_PT_MAX];

	int task_count;
	SuoYaoTaTask task_list[SUOYAOTA_TASK_MAX];
};

static const int SUOYAOTA_LEVEL_INTERVAL_MAX = 10;

class SuoYaoTaConfig : public ConfigBase
{
public:
	SuoYaoTaConfig();
	~SuoYaoTaConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetEnterInfo(Posi *pos);
	int GetJuanZhouItemID() { return m_juanzhou_itemid; }
	int GetRoomMaxUser() { return m_room_max_user; }

	int GetFreeJoinTimes() { return m_free_join_times; }
	int GetBuyTimesCost() { return m_buy_times_cost; }

	int GetGuwuCoinCost() { return m_guwu_coin_cost; }
	int GetGuwuGoldCost() { return m_guwu_gold_cost; }
	int GetGuwuDurTime() { return m_guwu_dur_time; }
	int GetGuwuGongJiAddPercent() { return m_guwu_gongji_addpercent; }

	int GetRewardDoubleCost() { return m_reward_double_cost; }
	int GetRewardThreeCost() { return m_reward_three_cost; }

	unsigned int GetNextStatusTime(int curr_status);
	unsigned int GetNextStandbyTime();
	unsigned int GetNextStopTime();

	int GetLvIntervalIdxByRoleLv(int role_lv);
	SuoYaoTaLogicCfg * GetSuoYaoTaLogicCfgByLv(int role_lv);

	void GetDailyFindReward(int role_level, int *bind_coin, long long *exp);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityTimeCfg(PugiXmlNode RootElement);
	int InitLvIntervalCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitTaskCfg(PugiXmlNode RootElement);

	int m_scene_id;
	Posi m_enter_pos;
	
	int m_juanzhou_itemid;

	int m_room_max_user;

	int m_free_join_times;
	int m_buy_times_cost;

	int m_guwu_coin_cost;
	int m_guwu_gold_cost;
	int m_guwu_dur_time;
	int m_guwu_gongji_addpercent;

	int m_reward_double_cost;
	int m_reward_three_cost;

	SuoYaoTaTimeCfg m_activity_time_list[SUOYAOTA_HOUR_OPEN_TIME];
	SuoYaoTaLogicCfg m_logic_cfg_list[SUOYAOTA_LEVEL_INTERVAL_MAX];
};

#endif // _SUOYAOTA_CONFIG_HPP_


