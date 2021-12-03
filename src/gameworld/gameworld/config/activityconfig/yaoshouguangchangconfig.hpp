#ifndef _YAOSHOUGUANGCHANG_CONFIG_HPP_
#define _YAOSHOUGUANGCHANG_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

enum YAOSHOUGUANGCHANG_STATUS_TYPE
{
	YAOSHOUGUANGCHANG_STATUS_TYPE_INVALID = 0,

	YAOSHOUGUANGCHANG_STATUS_TYPE_STANDBY,
	YAOSHOUGUANGCHANG_STATUS_TYPE_START,
	YAOSHOUGUANGCHANG_STATUS_TYPE_STOP,

	YAOSHOUGUANGCHANG_STATUS_TYPE_MAX,
};

static const int YAOSHOUGUANGCHANG_HOUR_OPEN_TIME = 2;						// 每个小时刷两场

struct YaoShouGuangChangTimeCfg
{
	YaoShouGuangChangTimeCfg()
	{
		memset(minute_list, 0, sizeof(minute_list));
	}

	int minute_list[YAOSHOUGUANGCHANG_STATUS_TYPE_MAX];
};

struct YaoShouGuangChangLogicCfg
{
	YaoShouGuangChangLogicCfg() : min_lv(0), max_lv(0), kill_monster_score(0), kill_boss_score(0),
		reward_base_exp(0), reward_base_coin(0), total_score(0), wave_count(0)
	{

	}

	int min_lv;
	int max_lv;

	int kill_monster_score;
	int kill_boss_score;

	long long reward_base_exp;
	int reward_base_coin;
	int total_score;
	
	struct WavePointCfg
	{
		WavePointCfg() : monster_id(0), monster_num(0), flush_pos(0, 0), flush_range(0) {}

		int monster_id;
		int monster_num;
		Posi flush_pos;
		int flush_range;
	};

	static const int YAOSHOUGUANGCHANG_WAVE_POINT_MAX = 16;					// 每波的点数

	struct WaveCfg
	{
		WaveCfg() : pt_count(0) {}

		int pt_count;
		WavePointCfg pt_list[YAOSHOUGUANGCHANG_WAVE_POINT_MAX];
	};

	static const int YAOSHOUGUANGCHANG_WAVE_MAX = 10;

	int wave_count;
	WaveCfg wave_list[YAOSHOUGUANGCHANG_WAVE_MAX];
};

static const int YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX = 10;

class YaoShouGuangChangConfig : public ConfigBase
{
public:
	YaoShouGuangChangConfig();
	~YaoShouGuangChangConfig();

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
	YaoShouGuangChangLogicCfg * GetYaoShouGuangChangLogicCfgByLv(int role_lv);

	void GetDailyFindReward(int role_level, int *bind_coin, long long *exp);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityTimeCfg(PugiXmlNode RootElement);
	int InitLvIntervalCfg(PugiXmlNode RootElement);
	int InitWaveCfg(PugiXmlNode RootElement);

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

	YaoShouGuangChangTimeCfg m_activity_time_list[YAOSHOUGUANGCHANG_HOUR_OPEN_TIME];
	YaoShouGuangChangLogicCfg m_logic_cfg_list[YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX];
};

#endif // _YAOSHOUGUANGCHANG_CONFIG_HPP_



