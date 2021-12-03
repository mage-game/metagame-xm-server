#ifndef _HUSONG_SHUIJING_CONFIG_HPP_
#define _HUSONG_SHUIJING_CONFIG_HPP_

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/crossdef.hpp"

#include <vector>


struct HusongSJOtherCfg
{
	HusongSJOtherCfg():is_open(0),scene_id(0),enter_pos(Posi(0,0)),gather_day_count(0), commit_count(0), dec_speed_per(0){}
	bool is_open;
	int scene_id;
	Posi enter_pos;
	int gather_day_count;
	int commit_count;
	int dec_speed_per;
};

// 开启时间
struct HusongSJActivityOpenTimeCfg
{
	HusongSJActivityOpenTimeCfg() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

enum HUSONG_SHUIJING_TYPE
{
	HUSONG_SHUIJING_TYPE_VALID = 0,

	HUSONG_SHUIJING_TYPE_BIG = HUSONG_SHUIJING_TYPE_VALID,
	HUSONG_SHUIJING_TYPE_SMALL = 1,

	HUSONG_SHUIJING_TYPE_MAX,
};

struct HusongSJGatherCfg
{
	HusongSJGatherCfg() : gather_id(0), gather_pos(0,0),gather_time(0), gather_change_times(0),
					gather_min_times(0), gather_max_times(0), commit_npcid(0), husong_timeout(0), 
					buy_cost(0), refresh_interval_s(0), gather_type(0)
					{}
	int gather_id;
	Posi gather_pos;
	int gather_time;
	int gather_change_times;
	int gather_min_times;
	int gather_max_times;
	int commit_npcid;
	//int gather_day_count;
	int husong_timeout;
	int buy_cost;
	int refresh_interval_s;
	int gather_type;
};

struct HusongSJRewardCfg
{
	HusongSJRewardCfg() : gather_type(0), commit_times(0),total_reward_item_count(0), be_robbed_reward_item_count(0) {}

	int gather_type;
	int commit_times;
	
	int total_reward_item_count;
	ItemConfigData total_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int be_robbed_reward_item_count;
	ItemConfigData be_robbed_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class HusongShuijingConfig
{
public:
	HusongShuijingConfig();
	~HusongShuijingConfig();

	static HusongShuijingConfig & Instance();



	bool Init(const std::string &configname, std::string *err);

	const HusongSJOtherCfg & GetOtherCfg(){ return m_other_cfg; }
	int GetGatherTypeCount() { return m_gather_cfg_vec.size(); }
	Posi GetEnterPos(int gather_type);

	const HusongSJGatherCfg * GetGatherCfg(int gather_type) const;
	const HusongSJGatherCfg * GetGatherCfgByGatherID(int gather_id) const;

	const HusongSJRewardCfg * GetJingHuaRewardCfg(int gather_type, int commit_times) const;

	HusongSJActivityOpenTimeCfg *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(HusongSJActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	
	bool DayTimeFormatInvalid(int day_time);

	HusongSJOtherCfg m_other_cfg;
	std::vector<HusongSJGatherCfg> m_gather_cfg_vec;

	typedef std::vector<HusongSJRewardCfg> HusongSJRewardVec;
	std::vector<HusongSJRewardVec> m_reward_vec;

	int m_activity_open_time_cfg_list_count;							// 活动开启时间配置数量
	HusongSJActivityOpenTimeCfg m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间
};

#endif	// _JINGHUASONG_CONFIG_HPP_



