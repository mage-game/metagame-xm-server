#ifndef __SHUIJING_CONFIG_HPP__
#define __SHUIJING_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

const static int MAX_SHUIJING_TASK_COUNT = 4;
const static int MAX_SHUIJING_TASK_EXP_PER_COUNT = 20; 

struct ShuiJingOtherCfg
{
	ShuiJingOtherCfg() : scene_id(0), gather_max_times(0), relive_time(0), free_relive_count(0), 
		gather_buff_gold(0), gather_buff_time(0), big_shuijing_id(0), reflush_notice_time(0){}

	int scene_id;
	int gather_max_times;
	int relive_time;
	int free_relive_count;
	Posi realive_pos;
	int gather_buff_gold;
	int gather_buff_time;
	int big_shuijing_id;
	int reflush_notice_time;
};

enum SHUIJING_GATHER_TYPE
{
	SHUIJING_TYPE_INVALID = 0,

	SHUIJING_TYPE_MIN,				// 小水晶
	SHUIJING_TYPE_MED,				// 中水晶
	SHUIJING_TYPE_BIG,				// 大水晶
	SHUIJING_TYPE_BEST,				// 至尊水晶

	SHUIJING_TYPE_MAX,
};

enum SHUIJING_GATHER_SUB_TYPE
{
	SHUIJING_SUB_TYPE_INVALID = 0,

	SHUIJING_SUB_TYPE_OTHER,			// 其他
	SHUIJING_SUB_TYPE_MOJING,			// 魔晶水晶
	SHUIJING_SUB_TYPE_SHENGWANG,		// 声望水晶
	SHUIJING_SUB_TYPE_DIAMOND,			// 钻石水晶

	SHUIJING_SUB_TYPE_MAX,
};

struct ShuiJingGatherCfg
{
	ShuiJingGatherCfg() : gather_type(0), gather_sub_type(0), gather_id(0), bind_gold(0), mojing(0), shengwang(0), is_broadcast(false) {}

	int gather_type;
	int gather_sub_type;
	int gather_id;
	int bind_gold;
	int mojing;
	int shengwang;
	bool is_broadcast;
};

enum SHUIJING_TASK_TYPE
{
	SHUIJING_TASK_TYPE_ONE = 0,
	SHUIJING_TASK_TYPE_TWO,
	SHUIJING_TASK_TYPE_THREE,
	SHUIJING_TASK_TYPE_FOUR,
};

enum SHUIJING_TYPE
{
	SHUIJING_TOTAL = 0,

	SHUIJING_BIG,				// 大水晶
	SHUIJING_BIG_DIAMOND,		// 钻石大水晶
	SHUIJING_BEST,				// 至尊水晶

	SHUIJING_MAX,
};


struct ShuiJingTaskCfg
{
	ShuiJingTaskCfg() : task_id(-1),shuijing(0),need_gather_count(0), reward_exp(0), is_notice(0), exp_factor_task(0){}

	int task_id;
	int shuijing;
	int need_gather_count;
	int reward_exp;
	int is_notice;
	double exp_factor_task;
};

struct ShuiJingTaskExpPerCfg
{
	ShuiJingTaskExpPerCfg() : min_level(0), max_level(0), exp_per(0){}

	int min_level;
	int max_level;
	int exp_per;
};

class ShuiJingConfig : public ConfigBase
{
public:
	ShuiJingConfig();
	~ShuiJingConfig();

	bool Init(const std::string &path, std::string *err);

	int GetShuiJingEnterInfo(Posi *enter_pos);
	const ShuiJingOtherCfg &GetOtherCfg() { return m_other_cfg; }
	const int GetShuijingScenId() { return m_other_cfg.scene_id; }
	const ShuiJingGatherCfg *GetGatherCfgByGatherId(int gather_id);
	bool IsBestShuiJing(int gather_id);
	const ShuiJingTaskCfg * GetTaskCfgByTaskID(int task_id);
	int GetTaskExpPerByLevle(int level);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);
	int InitTask(PugiXmlNode RootElement);
	int InitTaskExpPer(PugiXmlNode RootElement);

	ShuiJingOtherCfg m_other_cfg;
	std::map<int, ShuiJingGatherCfg> m_gather_cfg_map;

	ShuiJingTaskCfg m_task_cfg[MAX_SHUIJING_TASK_COUNT];

	int m_task_exp_per_cfg_count;
	ShuiJingTaskExpPerCfg m_task_exp_per_cfg[MAX_SHUIJING_TASK_EXP_PER_COUNT];
};

#endif
