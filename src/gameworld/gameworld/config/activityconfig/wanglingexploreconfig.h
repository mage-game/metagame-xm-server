#ifndef __WANGLINGEXPLORE_CONFIG_HPP__
#define __WANGLINGEXPLORE_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <map>

struct  WangLingExploreExtraRewardCfg
{
	WangLingExploreExtraRewardCfg() {}

	ItemConfigData item1;
	ItemConfigData item2;
	ItemConfigData item3;
	ItemConfigData item4;
};

struct WangLingExploreBossCfg
{
	WangLingExploreBossCfg() : boss_id(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int boss_id;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

struct WangLingExploreMonsterCfg
{
	WangLingExploreMonsterCfg() : monster_id(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int monster_id;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

struct WangLingExploreGatherCfg
{
	static const int MAX_GATHER_CFG_COUNT = 2;

	WangLingExploreGatherCfg() { }

	struct GatherItem
	{
		GatherItem() : gather_index(0), gather_id(0), gather_time(0), count(0), pos_id_start(0), pos_id_len(0), disappear_s(0) {}

		int gather_index;
		int gather_id;
		int gather_time;
		int count;
		int pos_id_start;
		int pos_id_len;
		int disappear_s;
	};

	GatherItem gather_list[MAX_GATHER_CFG_COUNT];
};

struct RewardPool
{
	static const int MAX_REWARDPOOL_CFG_COUNT = 30;

	RewardPool() : cfg_count(0) {}

	struct RewardItem
	{
		RewardItem() : seq(0), reward_bind_cost(0)
		{

		}

		int seq;
		int reward_bind_cost;

		ItemConfigData item;
	};

	int cfg_count;
	RewardItem item_list[MAX_REWARDPOOL_CFG_COUNT];
};

struct WangLingExploreOtherCfg
{
	WangLingExploreOtherCfg() : m_sceneid(0), m_min_level(0), item_count(0), reward_gold_box_bind_diamone(0), double_reward_limit_time(0), relive_pos(0, 0),
	gather_buff_gold(0),gather_buff_time(0){}

	int m_sceneid;			// 王陵探险 场景ID
	int m_min_level;		// 王陵探险 限制等级
	int item_count;
	ItemConfigData reward_gold_box_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int reward_gold_box_bind_diamone;
	int double_reward_limit_time;
	Posi relive_pos;		// 角色出生点
	int gather_buff_gold;
	int gather_buff_time;
};

struct BoosPool
{
	static const int MAX_BOOS_ID_COUNT = 20;

	BoosPool() : count(0) {}

	struct BoosItem
	{
		BoosItem() : world_level(0), boos_id(0)
		{

		}

		int world_level;
		int boos_id;
	};

	int count;
	BoosItem item_list[MAX_BOOS_ID_COUNT];
};

class WangLingExploreConfig : public ConfigBase
{
public:
	WangLingExploreConfig();
	~WangLingExploreConfig();

	bool Init(const std::string &path, std::string *err);
	int InitOther(PugiXmlNode RootElement);
	int InitTaskList(PugiXmlNode RootElement);
	int InitTaskExtraReward(PugiXmlNode RootElement);
	int InitFlushPoint(PugiXmlNode RootElement);
	int InitBoss(PugiXmlNode RootElement);
	int InitMonster(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);
	int InitRewardPool(PugiXmlNode RootElement);
	int InitTaskExpCfg(PugiXmlNode RootElement);
	int InitBoosIdByWorldLevel(PugiXmlNode RootElement);

	const WangLingExploreOtherCfg & GetWangLingExploreOtherCfg() { return m_wang_ling_explore_other_cfg; }

	const int GetTaskRewardExp(int level);

	void GetWangLingExploreTaskList(WangLingExploreTaskInfo wanglingexplore_task_list[WANGLINGEXPLORE_TASK_MAX]);
	const WangLingExploreTaskInfo *GetWangLingExploreTaskCfg(int task_id);

	const WangLingExploreExtraRewardCfg &GetTaskExtraReward() { return m_reward_item_cfg; }

	const WangLingExploreBossCfg &GetWangLingExploreBossCfg() { return m_wanglingexplore_boss_cfg; }
	const WangLingExploreMonsterCfg &GetWangLingExploreMonsterCfg() { return m_wanglingexplore_monster_cfg; }

	const WangLingExploreGatherCfg &GetWangLingExploreGatherCfg() { return m_gather_cfg; }

	bool GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len);
	bool GetRandomFlushPos(std::vector<Posi>& flush_pos, int count, int id_star, int id_len);//根据id取坐标

	const RewardPool::RewardItem * GetRandomRewardItem();

	int GetBoosIdByWorldLevel(int world_level);

private:

	std::map<int, WangLingExploreTaskInfo> m_wanglingexplore_task_map;	// 王陵探险 相关配置信息

	std::map<int, Posi> m_flush_point_map;

	WangLingExploreBossCfg m_wanglingexplore_boss_cfg;					// 王陵探险 BOSS信息
	WangLingExploreMonsterCfg m_wanglingexplore_monster_cfg;			// 王陵探险 怪物
	WangLingExploreExtraRewardCfg m_reward_item_cfg;					// 王陵探险 额外奖励
	WangLingExploreGatherCfg m_gather_cfg;								// 王陵探险 采集物信息
	RewardPool m_reward_pool_cfg;										// 王陵探险 奖池配置
	WangLingExploreOtherCfg m_wang_ling_explore_other_cfg;

	int m_task_exp_reward[MAX_ROLE_LEVEL + 1];
	BoosPool m_boos_pool;												// 世界等级对应的boos id
};
#endif // __WANGLINGEXPLORE_CONFIG_HPP__

