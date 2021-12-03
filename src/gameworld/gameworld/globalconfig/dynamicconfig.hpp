#ifndef __DYNAMIC_CONFIG_HPP__
#define __DYNAMIC_CONFIG_HPP__

#include <string>

#include "servercommon/activitydef.hpp"

// 开服第几天强开
static const int QUXIANLUANDOU_SERVER_OPEN_DAY_INDEX = 0;			// 开服第1天强开群仙乱斗
static const int GUILDBATTLE_SERVER_OPEN_DAY_INDEX = 1;				// 开服第2天强开仙魔战场
static const int GONGCHENGZHAN_SERVER_OPEN_DAY_INDEX = 2;			// 开服第3天强开攻城战
static const int WORSHIP_SERVER_OPEN_DAY_INDEX = 3;					// 开服第4天强开膜拜城主

// ★★★ 群仙乱斗,仙魔战场,攻城战 开服3天内只能在以上天数内开  ★★★
static const int SPECIAL_BAN_ACTIVITY_SERVER_OPEN_DAY = 3;


class DynamicConfig
{
public:
	DynamicConfig();
	~DynamicConfig();
	
	bool Init(const std::string &path, std::string *err);

	int GetActivtyNextStatus(int activity_type, int curr_status, int next_open_wday, StatusTimeCfgList *statuestime_list_cfg);
	unsigned int GetActivityNextSwitchTime(int activity_type, int curr_status, int next_open_wday, StatusTimeCfgList *statuestime_list_cfg, bool is_find);
	bool GetActivityNextCfg(int activity_type, int *next_open_wday, StatusTimeCfgList *statuestime_list_cfg, bool *is_find = NULL);
	bool IsActivityOpenDay(int activity_type, int weekday);

	bool GetSpecialActivityTime(int special_activity_type, unsigned int *begin_time, unsigned int *end_time);

	const RandActivityCfg * GetRandActivityCfg(int rand_activity_type);

	int GetActivitySpecialOpenDay(int activity_type);

	// 是否在开服第open_game_day天禁止开启该活动(开服第一天为open_game_day == 0)
	bool IsActivitySpecialCloseDay(int activity_type, int open_game_day);

private:
	ActivityCfg m_activitycfg_list[ACTIVITY_TYPE_MAX];

	SpecialActivityCfg m_special_activitycfg_list[SPECIAL_ACTIVITY_TYPE_MAX];

	RandActivityCfg m_rand_activitycfg_list[RAND_ACTIVITY_TYPE_MAX];
};

#endif // __DYNAMIC_CONFIG_HPP__

