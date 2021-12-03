#ifndef __TASKPOOL_HPP__
#define __TASKPOOL_HPP__

#include <string>
#include <vector>
#include <limits.h>
#include "gamedef.h"
#include "servercommon/taskdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include <map>
#include <set>

static const int COMPLETE_ALL_DAILY_REWARD_ITEM_MAX_COUNT = 3;							// 完成所有屠魔任务 物品奖励最大数量

struct TaskCfg
{
	static const int MAX_REWARD_ITEM_COUNT = 4;

	TaskCfg() : task_id(0), task_type(0), ver(0), min_level(0), max_level(0), pretaskid(0), camp(0),
		accept_npc(0), commit_npc(0), auto_commit_time(0), auto_commit_openday(0), condition(0), c_param1(0), c_param2(0), c_param3(0),
		accept_op(0), a_param1(0), a_param2(0), a_param3(0),
		coin_bind(0), exp(0), nv_wa_shi(0), yuanli(0), 
		normal_item_count(0), prof_item_count_1(0), prof_item_count_2(0), prof_item_count_3(0), prof_item_count_4(0) 
	{}

	int task_id;
	int task_type;
	int ver;

	int min_level;
	int max_level;
	int pretaskid;
	int camp;

	int accept_npc;
	int commit_npc;
	int auto_commit_time;			// 自动提交时间
	int auto_commit_openday;		// 大于开服天数则自动提交,开服第一天为1

	int condition;
	int c_param1;
	int c_param2;
	int c_param3;

	int accept_op;
	int a_param1;
	int a_param2;
	int a_param3;

	int coin_bind;
	long long exp;
	int nv_wa_shi;
	int yuanli;

	int normal_item_count;
	ItemConfigData normal_item_list[MAX_REWARD_ITEM_COUNT];

	int prof_item_count_1;
	ItemConfigData prof_item_list_1[MAX_REWARD_ITEM_COUNT];

	int prof_item_count_2;
	ItemConfigData prof_item_list_2[MAX_REWARD_ITEM_COUNT];

	int prof_item_count_3;
	ItemConfigData prof_item_list_3[MAX_REWARD_ITEM_COUNT];

	int prof_item_count_4;
	ItemConfigData prof_item_list_4[MAX_REWARD_ITEM_COUNT];
};

/////////////////////////////////////////////////////////// 日常任务 ///////////////////////////////////////
struct DailyLevelTaskInfo
{
	DailyLevelTaskInfo() : min_level(0), max_level(0) {}

	bool Invalid() { return max_level < 0 || min_level >= max_level; }
	bool LevelInRange(int level) { return level >= min_level && level <= max_level; }

	int min_level;																	// 最小等级
	int max_level;																	// 最大等级
	std::set<TaskID> task_list;														// 任务id
};

struct DailyTaskOtherCfg
{
	DailyTaskOtherCfg() : daily_pretask(0), fix_first_daily_task(0), fix_daily_task_level(0), double_gold(0), free_double_vip_level(0), daily_treble_gold(0), exp_ball_limit(0), valid_time_h(0), double_maxstar_gold(0), example_task_id(0), max_star_cost_coin(0), 
		exp_factor_dailytask(0), exp_factor_guildtask(0), exp_factor_huantask(0){}

	int daily_pretask;
	int fix_first_daily_task;
	int fix_daily_task_level;
	int double_gold;
	int free_double_vip_level;
	int daily_treble_gold;
	long long exp_ball_limit;
	int valid_time_h;
	int double_maxstar_gold;
	int example_task_id;
	int max_star_cost_coin;
	double exp_factor_dailytask;
	double exp_factor_guildtask;
	double exp_factor_huantask;
};

struct DailyTaskReward
{
	DailyTaskReward() : min_level(0), max_level(0), exp(0), bind_coin(0), complete_all_daily_reward_coin(0), complete_all_daily_reward_itemcount(0) {}

	int min_level;
	int max_level;
	long long exp;
	int bind_coin;

	int complete_all_daily_reward_coin;
	int complete_all_daily_reward_itemcount;
	ItemConfigData complete_all_daily_reward_itemlist[COMPLETE_ALL_DAILY_REWARD_ITEM_MAX_COUNT];
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int PAOHUAN_LV_FIX_COUNT = 10;

struct PaohuanTaskLvFixCfg
{
	PaohuanTaskLvFixCfg() : min_level(0), max_level(0), nvwashi_addpercent(0), exp_addpercent(0){}

	int min_level;
	int max_level;
	int nvwashi_addpercent;
	int exp_addpercent;
};

struct PaohuanTaskRewardCfg
{
	PaohuanTaskRewardCfg() : paohuan_task_count(0)
	{
		memset(nvwashi_list, 0, sizeof(nvwashi_list));
		memset(exp_list, 0, sizeof(exp_list));
	}

	int paohuan_task_count;

	int nvwashi_list[PAOHUAN_ONEDAY_COMMIT_MAX];
	long long exp_list[PAOHUAN_ONEDAY_COMMIT_MAX];

	ItemConfigData reward_item[PAOHUAN_ONEDAY_COMMIT_MAX];
};

static const int PAOHUAN_ROLLPOOL_TOTAL_COUNT = 8;

struct PaoHuanRollItem
{
	PaoHuanRollItem() : rolllist_id(0), weight(0) {}

	ItemConfigData item_cfg;
	int rolllist_id;
	int weight;
};

struct PaoHuanRollPool
{
	PaoHuanRollPool() {}
	
	PaoHuanRollItem item_list[PAOHUAN_ROLLPOOL_TOTAL_COUNT];
};

struct PaoHuanTaskOtherCfg
{
	PaoHuanTaskOtherCfg() : one_task_need_gold(0), one_key_commit_task_count(0), skip_paohuan_task_limit_level(0){}

	int one_task_need_gold;
	int one_key_commit_task_count;

	int skip_paohuan_task_limit_level;
};

struct GuildTaskConfig
{
	static const int CONFIG_ITEM_MAX_COUNT = 30;
	static const int GUILD_TASK_SERIES_LIMIT_COUNT = 10;
	static const int GUILD_TASK_SERIES_SPECIAL_LIMIT_COUNT = 5;

	struct ConfigItem
	{
		ConfigItem() : first_task(0), end_task(0) {}

		TaskID first_task;
		TaskID end_task;
	};

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct GuildTaskRewardCfg
{
	static const int GUILD_TASK_REWARD_ITEM_MAX = 4;

	GuildTaskRewardCfg() : level(0), exp(0), gongxian(0), item_count(0) {}

	int level;
	long long exp;
	int gongxian;

	int item_count;
	ItemConfigData item_list[GUILD_TASK_REWARD_ITEM_MAX];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
static const int MAX_TASK_RANDOM_CONFIG = 200;

struct PerRandomReward
{
	void reset()
	{
		task_type = 0;
		weight = 0;
		index = 0;
	};
	int task_type;
	int weight;
	int index;
	ItemConfigData item_data;
};

struct TaskRandomReward
{
	TaskRandomReward()
	{
		count = 0;
		weight_sum = 0;
		for (int i = 0; i < MAX_TASK_RANDOM_CONFIG; i++)
		{
			per_reward_list[i].reset();
		}
	}

	PerRandomReward per_reward_list[MAX_TASK_RANDOM_CONFIG];
	int count;
	int weight_sum;
};

struct ZhuanzhiTaskConfig
{
	static const int CONFIG_ITEM_MAX_COUNT = 10;

	struct ConfigItem
	{
		ConfigItem() : first_task(0), end_task(0) {}

		TaskID first_task;
		TaskID end_task;
	};

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class TiXmlElement;

#define TASKPOOL TaskPool::Instance()

class TaskPool
{
public:
	static TaskPool * Instance();

	bool Init(const std::string &dir, const std::string &filename, std::string *err);
	static bool Reload(const std::string &dir, const std::string &filename, std::string *err);

	const TaskCfg * GetCfgByTaskID(int task_id) const;

	void GetNextTask(int task_id, short &task_count, int task_id_list[MAX_NEXT_TASK_COUNT]) const;
	void GetTaskAtLevel(int role_level, short &task_count, int task_id_list[MAX_NEXT_TASK_COUNT]) const;

	bool RandDailyTaskByRoleLevel(int role_level, int *out_taskid, int except_param);
	int GetDailyPreTask() { return m_daily_other_cfg.daily_pretask; }
	int GetFixFirstDailyTask() { return m_daily_other_cfg.fix_first_daily_task; }
	int GetFixDailyTaskLevel() { return m_daily_other_cfg.fix_daily_task_level; }
	int GetDailyFinishDoubleGold() { return m_daily_other_cfg.double_gold; }
	int GetDailyFinishDoubleMaxStarGold() { return m_daily_other_cfg.double_maxstar_gold;}
	const DailyTaskReward * GetDailyReward(int level);
	int GetDailyExampleTaskID() { return m_daily_other_cfg.example_task_id; }
	int GetDailyMaxStarCostCoin() { return m_daily_other_cfg.max_star_cost_coin; }

	const DailyTaskOtherCfg &GetDailyTaskOtherCfg() { return m_daily_other_cfg; }
	
	bool GetPaohuanTaskReward(int role_lv, int complete_tasknum, int *nvwashi, long long &exp, ItemConfigData &item);
	bool RandPaohuanTaskByRoleLevel(int role_level, int *out_taskid);
	int GetPaoHuanOneTaskFinishGold() { return m_paohuan_other_cfg.one_task_need_gold; }
	int GetPaoHuanOneKeyTaskCount() { return m_paohuan_other_cfg.one_key_commit_task_count; }
	int GetPaoHuanSkipTaskLimitLevel() { return m_paohuan_other_cfg.skip_paohuan_task_limit_level; }
	bool GetPaoHuanRollPool(PaoHuanRollItem roll_list[PAOHUAN_ROLLPOOL_TOTAL_COUNT], int rolllist_id);

	const GuildTaskRewardCfg * GetGuildReward(int level);
	int GetGuildDoubleRewardCost() { return m_guild_other_cfg.guild_double_reward_cost; }
	const ItemConfigData & GetGuildTaskAllCompleteReward() { return m_guild_other_cfg.guild_task_complete_all_reward_item; }
	int GetGuildTaskOneKeyCostPerTask() { return m_guild_other_cfg.guild_task_gold; }
	int GetGuildTaskSpecialCountLimitLevel(){ return m_guild_other_cfg.guild_task_special_count_limit_level;}
	
	TaskID GetEndTaskByFirstTask(TaskID first_task);
	bool IsGuildFirstTaskID(TaskID task_id);
	bool IsInGuildTaskList(TaskID first_task, TaskID task_id);
	const std::set<int> & GetGuildTaskList() { return m_guild_taskid_set; }

	int GetGuildExampleTaskID() { return m_guild_task_list.cfg_list[0].end_task; }

	int GetRandomTaskStar();
	int GetTaskStarRewardPer(int level);
	int GetMaxTaskStarLevel() { return m_task_star_max_level; }
	int GetPaoHuanTaskCount() { return m_paohuan_reward_cfg.paohuan_task_count; }

	PerRandomReward* GetRandomReward(int task_type);
	int GetTaskRollFinishTime() { return m_task_roll_finish_time; };

	const std::set<int> & GetZhuanzhiTaskList() { return m_zhuanzhi_taskid_set; }
	bool IsInZhuanzhiTaskList(TaskID first_task, TaskID task_id);
	bool IsZhuanzhiFirstTaskID(TaskID task_id);
	TaskID GetZhuanzhiEndTaskByFirstTask(TaskID first_task);
	const ZhuanzhiTaskConfig::ConfigItem * GetZhuanzhiTaskConfigByProfLevel(int prof_level);
private:
	TaskPool();
	~TaskPool();

	int InitTaskCfg(TiXmlElement *RootElement);
	int InitOther(TiXmlElement *RootElement);
	int InitDailyTaskReward(TiXmlElement *RootElement);
	int InitGuildTaskReward(TiXmlElement *RootElement);
	int InitGuildTaskList(TiXmlElement *RootElement);
	int InitTaskStar(TiXmlElement *RootElement);
	
	int InitPaohuanTaskRewardCfg(TiXmlElement *RootElement);
	int InitPaohuanTaskLvFixCfg(TiXmlElement *RootElement);
	int InitPaoHuanRollCfg(TiXmlElement *RootElement);
	int InitTaskRandomCfg(TiXmlElement *RootElement);
	int InitZhuanzhiTaskList(TiXmlElement *RootElement);

	void PutNextTask(int pretaskid, int nexttaskid);
	void PutLevelTask(int taskid, int level);
	int PutDailyTask(TaskCfg *task_cfg);
	int PutPaohuanTask(TaskCfg *task_cfg);
	void PutGuildTask(int task_id);
	void PutZhuanzhiTask(int task_id);

	bool CheckGuildTask(TaskID first_task, TaskID end_task);
	bool CheckGuildTaskByLevel();
	
	// 全部任务
	typedef std::vector<int> TaskList;

	std::set<int> m_task_chk_set;

	std::map<int, TaskCfg> m_task_map;
	std::map<int, TaskList> m_next_map;
	std::set<int> m_guild_taskid_set;
	TaskList m_level_task_list[MAX_ROLE_LEVEL + 1];
	std::set<int> m_zhuanzhi_taskid_set;

	/////////////////////////////////////////////////////  日常任务  /////////////////////////////////////////////////////  

	int m_daily_level_interval_count;															
	DailyLevelTaskInfo m_daily_level_taskinfo[DAILY_LEVEL_INTERVAL_MAX];						

	DailyTaskOtherCfg m_daily_other_cfg;
	std::vector<DailyTaskReward> m_daily_task_reward_list;

	/////////////////////////////////////////////////////  跑环任务  /////////////////////////////////////////////////////  
	struct PaohuanLevelTaskInfo															
	{
		PaohuanLevelTaskInfo() : min_level(0), max_level(0) {}

		bool Invalid() { return max_level < 0 || min_level >= max_level; }
		bool LevelInRange(int level) { return level >= min_level && level <= max_level; }

		int min_level;																	// 最小等级
		int max_level;																	// 最大等级
		std::set<TaskID> task_list;														// 任务id
	};

	int m_paohuan_level_interval_count;		

	PaohuanLevelTaskInfo m_paohuan_level_taskinfo[PAOHUAN_LEVEL_INTERVAL_MAX];			// 跑环任务信息 按等级分配	
	PaohuanTaskRewardCfg m_paohuan_reward_cfg;											// 跑环任务奖励
	PaohuanTaskLvFixCfg m_paohuan_lvfix_cfg[PAOHUAN_LV_FIX_COUNT];						// 等级修正奖励配置
	PaoHuanTaskOtherCfg m_paohuan_other_cfg;											// 其他配置
	
	std::map<int, PaoHuanRollPool> m_paohuan_roll_map;

	//////////////////////////////////////////////////	仙盟任务	//////////////////////////////////////////////////
	GuildTaskConfig m_guild_task_list;

	struct GuildTaskOtherCfg
	{
		GuildTaskOtherCfg() :  guild_double_reward_cost(0), guild_task_gold(0), guild_task_special_count_limit_level(0) {}
		
		int guild_double_reward_cost;
		ItemConfigData guild_task_complete_all_reward_item;
		int guild_task_gold;
		int guild_task_special_count_limit_level;
	};

	GuildTaskOtherCfg m_guild_other_cfg;
	GuildTaskRewardCfg m_guild_task_reward_list[MAX_ROLE_LEVEL + 1];

	//////////////////////////////////////////////////	星级	//////////////////////////////////////////////////
	struct TaskStarCfg
	{
		TaskStarCfg() : star_level(0), weight(0), reward_rate(0) {}

		int star_level;
		int	weight;
		int reward_rate;
	};
	
	int m_task_star_max_level;
	int m_task_star_total_weight;
	std::map<int, TaskStarCfg> m_task_star_map;

	/////////////////////////////////////////////////////任务奖池//////////////////////////////////////////////
	std::map<int, TaskRandomReward> task_random_config_map;	
	int m_task_roll_finish_time;

	//////////////////////////////////////////////////	转职任务 /////////////////////////////////////////////////////
	ZhuanzhiTaskConfig m_zhuanzhi_task_list;
};

#endif // __TASKPOOL_HPP__


