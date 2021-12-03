#ifndef __CROSS_MESS_BATTLE_CONFIG_HPP__
#define __CROSS_MESS_BATTLE_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct MessBattleOtherConfig
{
	MessBattleOtherConfig() : is_open(false) {}

	bool is_open;
};

struct CrossMessBattleActivityOpenTimeInfo
{
	CrossMessBattleActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

class CrossMessBattleConfig:public ConfigBase
{
public:
	CrossMessBattleConfig();
	~CrossMessBattleConfig();

	static CrossMessBattleConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const MessBattleOtherConfig & GetOtherCfg() { return m_other_cfg; }
	CrossMessBattleActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossMessBattleActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT]);

	bool DayTimeFormatInvalid(int day_time);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);

	MessBattleOtherConfig m_other_cfg;
	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossMessBattleActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间
};

#endif // __CROSS_MESS_BATTLE_CONFIG_HPP__

