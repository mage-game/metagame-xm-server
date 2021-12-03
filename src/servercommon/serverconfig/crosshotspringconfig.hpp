#ifndef __CROSS_HOT_SPRING_CONFIG_HPP__
#define __CROSS_HOT_SPRING_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossHotSpringActivityTimeCfg
{
	CrossHotSpringActivityTimeCfg() : is_open(false), open_day_secon_count(0), standby_dur_s(0), open_dur_s(0) 
	{
		memset(open_day_list, 0, sizeof(open_day_list));
		memset(open_day_second, 0, sizeof(open_day_second));
	}

	bool is_open;
	bool open_day_list[7];
	int open_day_secon_count;
	int open_day_second[MAX_ACTIVIYT_TIME_CFG_NUM];	// 活动开放时间 距0点的秒数
	int standby_dur_s;				// 准备阶段持续时间 秒数
	int open_dur_s;					// 活动开启持续时间 秒数
};

class CrossHotSpringConfig
{
public:
	CrossHotSpringConfig();
	~CrossHotSpringConfig();

	static CrossHotSpringConfig & Instance();

	bool Init(const std::string &path, std::string *err);

	CrossHotSpringActivityTimeCfg * GetActivityTimeCfg() { return &m_activity_time_cfg; }
	int GetActivityStandbyTime() { return m_activity_time_cfg.standby_dur_s; }
	int GetActivityOpenTime() { return m_activity_time_cfg.open_dur_s; }
	unsigned int GetActivityRefrshInterval(){return m_refresh_rank_interval_s;}

	Posi GetEnterPos() const { return m_enter_pos; }
	int GetSceneID() const { return m_scene_id; }

	unsigned int GetCrossInfoClearTime() { return m_cross_info_clear_time; }

private:

	CrossHotSpringActivityTimeCfg m_activity_time_cfg;

	Posi m_enter_pos;
	int m_scene_id;
	unsigned int m_refresh_rank_interval_s;

	unsigned int m_cross_info_clear_time;
};

#endif // __CROSS_HOT_SPRING_CONFIG_HPP__

