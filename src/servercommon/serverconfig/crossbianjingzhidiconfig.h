#ifndef __CROSS_BIANJINGZHIDI_CONFIG_HPP__
#define __CROSS_BIANJINGZHIDI_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"

#include <vector>
#include <map>


// 其他配置
struct CrossBianJingZhiDiOtherCfg
{
	CrossBianJingZhiDiOtherCfg() : 
		is_open(false), server_open_day(0), m_min_level(0), 
		m_sceneid(0), relive_pos(0, 0),
		double_reward_limit_time(0), gather_buff_gold(0), gather_buff_time(0), 
		start_reflush_time_s(0), reflush_time_s(0) {}

	bool is_open;
	int server_open_day;	// 跨服边境之地 开服第几天开启
	int m_min_level;		// 跨服边境之地 限制等级

	int m_sceneid;			// 跨服边境之地 场景ID
	Posi relive_pos;		// 角色出生点

	int double_reward_limit_time;
	int gather_buff_gold;
	int gather_buff_time;

	int start_reflush_time_s;
	int reflush_time_s;
};

// 开启时间
struct CrossBianJingZhiDiActivityOpenTimeCfg
{
	CrossBianJingZhiDiActivityOpenTimeCfg() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

// BOSS配置
struct CrossBianJingZhiDiBossCfg
{
	CrossBianJingZhiDiBossCfg() : boss_type(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int boss_type;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

// 世界等级BOSS配置
struct CrossBianJingZhiDiBossPool
{
	static const int MAX_BOSS_ID_COUNT = 20;

	CrossBianJingZhiDiBossPool() : count(0) {}

	struct BossItem
	{
		BossItem() : world_level(0), boss_id(0) { }

		int world_level;
		int boss_id;
	};

	int count;
	BossItem item_list[MAX_BOSS_ID_COUNT];
};

// 小怪配置
struct CrossBianJingZhiDiMonsterCfg
{
	CrossBianJingZhiDiMonsterCfg() : monster_id(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int monster_id;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

// 采集物配置
struct CrossBianJingZhiDiGatherCfg
{
	CrossBianJingZhiDiGatherCfg() { }

	struct GatherItem
	{
		GatherItem() : boss_type(0), gather_id(0), gather_time(0), pos_id_start(0), pos_id_len(0), disappear_s(0), item_count(0) {}

		int boss_type;
		int gather_id;
		int gather_time;
		int pos_id_start;
		int pos_id_len;
		int disappear_s;

		int item_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	};

	GatherItem gather_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
};


//////////////////////////////////////////////////////////////////////////
class CrossBianJingZhiDiConfig
{
public:
	CrossBianJingZhiDiConfig();
	~CrossBianJingZhiDiConfig();

	static CrossBianJingZhiDiConfig & Instance();

	bool Init(const std::string &path, std::string *err);
	int InitOther(PugiXmlNode RootElement);
	int InitTaskList(PugiXmlNode RootElement);
	int InitFlushPoint(PugiXmlNode RootElement);
	int InitBoss(PugiXmlNode RootElement);
	int InitMonster(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);
	int InitBoosIdByWorldLevel(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitSosCfg(PugiXmlNode RootElemen);

	bool DayTimeFormatInvalid(int day_time);

	inline bool IsCrossBianJingZhiDiScene(int scene_id) { return (0 != scene_id && scene_id == m_bianjingzhidi_other_cfg.m_sceneid); };

	const CrossBianJingZhiDiOtherCfg & GetCrossBianJingZhiDiOtherCfg() { return m_bianjingzhidi_other_cfg; }
	CrossBianJingZhiDiActivityOpenTimeCfg *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossBianJingZhiDiActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const;

	void GetCrossBianJingZhiDiTaskList(CrossBianJingZhiDiTaskInfo bianjingzhidi_task_list[CROSS_BIANJINGZHIDI_TASK_MAX]);
	const CrossBianJingZhiDiTaskInfo *GetCrossBianJingZhiDiTaskCfg(int task_id);

	const CrossBianJingZhiDiBossCfg* GetCrossBianJingZhiDiBossCfg(int boss_type);
	const int GetBossTypeByBossId(int boss_id);
	const CrossBianJingZhiDiMonsterCfg &GetCrossBianJingZhiDiMonsterCfg() { return m_bianjingzhidi_monster_cfg; }

	const CrossBianJingZhiDiGatherCfg::GatherItem *GetCrossBianJingZhiDiGatherCfg(int boss_type);
	const CrossBianJingZhiDiGatherCfg::GatherItem *GetCrossBianJingZhiDiGatherCfgByID(int gather_id);
	int GetSosCost(int times) const;

	bool GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len);
	bool GetRandomFlushPos(std::vector<Posi>& flush_pos, int count, int id_star, int id_len);//根据id取坐标

	int GetBoosIdByWorldLevel(int world_level, int boss_type);

private:

	std::map<int, CrossBianJingZhiDiTaskInfo> m_bianjingzhidi_task_map;	// 跨服边境之地 任务相关配置信息<task_id, info>

	CrossBianJingZhiDiOtherCfg m_bianjingzhidi_other_cfg;
	CrossBianJingZhiDiBossCfg m_bianjingzhidi_boss_cfg[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE]; // 跨服边境之地 BOSS信息
	CrossBianJingZhiDiMonsterCfg m_bianjingzhidi_monster_cfg;			// 跨服边境之地 怪物
	CrossBianJingZhiDiGatherCfg m_gather_cfg;							// 跨服边境之地 采集物信息
	std::map<int, Posi> m_flush_point_map;								// 刷新点

	CrossBianJingZhiDiBossPool m_boss_pool[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];	// 世界等级对应的boos id

	int m_activity_open_time_cfg_list_count;							// 活动开启时间配置数量
	CrossBianJingZhiDiActivityOpenTimeCfg m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	std::vector<int> m_sos_cost_cfg_vec;
};
#endif // __CROSS_BIANJINGZHIDI_CONFIG_HPP__

