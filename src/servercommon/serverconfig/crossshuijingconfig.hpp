#ifndef __CROSSSHUIJING_CONFIG_HPP__
#define __CROSSSHUIJING_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct CrossShuiJingOtherCfg
{
	CrossShuiJingOtherCfg() : is_open(false), scene_id(0), relive_time(0), gather_max_times(0), gather_buff_gold(0), gather_buff_time(0), big_shuijing_id(0) {}

	bool is_open;
	int scene_id;
	int relive_time;
	int gather_max_times;
	Posi realive_pos;
	int gather_buff_gold;
	int gather_buff_time;
	int big_shuijing_id;
};

struct CrossShuijingActivityOpenTimeInfo
{
	CrossShuijingActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct CrossShuiJingGatherCfg
{
	CrossShuiJingGatherCfg() : gather_type(0), gather_id(0), bind_gold(0), mojing(0), cross_honor(0) {}

	int gather_type;
	int gather_id;
	int bind_gold;
	int mojing;
	int cross_honor;
};

class CrossShuiJingConfig
{
public:
	CrossShuiJingConfig();
	~CrossShuiJingConfig();

	static CrossShuiJingConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossShuiJingOtherCfg &GetOtherCfg() { return m_other_cfg; }

	CrossShuijingActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);// 获取活动的开启配置
	int GetOpenActivityConfigList(CrossShuijingActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]);

	const CrossShuiJingGatherCfg *GetGatherCfgByGatherId(int gather_id);

	int GetBigShuiJingId() { return m_big_shuijing_id; }

private:
	int InitOther(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossShuiJingOtherCfg m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossShuijingActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	std::map<int, CrossShuiJingGatherCfg> m_gather_cfg_map;

	int m_big_shuijing_id;
};

#endif
