#ifndef __TIANJIANGCAIBAO_CONFIG_HPP__
#define __TIANJIANGCAIBAO_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

enum TIAN_JIANG_CAI_BAO_TASK_TYPE
{
	TIAN_JIANG_CAI_BAO_TASK_TYPE_NONE = 0,
	TIAN_JIANG_CAI_BAO_TASK_TYPE_GATHER,
	TIAN_JIANG_CAI_BAO_TASK_TYPE_MONSTER,
	TIAN_JIANG_CAI_BAO_TASK_TYPE_MAX,
};

struct TianJiangCaiBaoBigQianCfg
{
	TianJiangCaiBaoBigQianCfg() : bigqian_id(0), kill_gold(0), reflush_time_m(0), born_pos(0,0) {}

	UInt16 bigqian_id;
	int kill_gold;
	int reflush_time_m;
	Posi born_pos;
};

struct TianJiangCaiBaoQianduoduoCfg
{
	TianJiangCaiBaoQianduoduoCfg() : qianduoduo_id(0), count(0), kill_min_gold(0), kill_max_gold(0), reflush_time_m(0), last_reflush_time_m(0) {}

	UInt16 qianduoduo_id;
	int count;
	int kill_min_gold;
	int kill_max_gold;
	int reflush_time_m;
	int last_reflush_time_m;
};

struct TianJiangCaiBaoRewardCfg
{
	TianJiangCaiBaoRewardCfg() : seq(0), complete_task_num(0) {}

	int seq;
	int complete_task_num;
	
	ItemConfigData item1;
	ItemConfigData item2;
	ItemConfigData item3;
	ItemConfigData item4;
};

class TianJiangCaiBaoConfig : public ConfigBase
{
public:
	TianJiangCaiBaoConfig();
	~TianJiangCaiBaoConfig();

	bool Init(const std::string &path, std::string *err);

	int GetEnterInfo(Posi *pos);

	int GetTianJiangCaiBaoEnterInfo() { return m_sceneid; }
 	int GetAttackRewardGold() { return m_attack_get_gold; }
	int GetLimitLevel() { return m_min_level; }

	std::map<int, TianJiangCaiBaoTaskInfo> & GetTianJiangCaiBaoTaskCfgMap() { return m_tianjiangcaibao_task_map; }
	const TianJiangCaiBaoTaskInfo *GetTianJiangCaiBaoTaskCfg(int task_id);

	const TianJiangCaiBaoRewardCfg * GetTaskReward() { return m_reward_list; }

	const TianJiangCaiBaoBigQianCfg & GetTianJiangCaiBaoBigQianCfg() { return m_tianjiangcaibao_bigqian_cfg; }
	const TianJiangCaiBaoQianduoduoCfg & GetTianJiangCaiBaoQianduoduoCfg() { return m_tianjiangcaibao_qianduoduo_cfg; }

	const Posi* GetRandomFlushPos();

	const int GetTaskRewardExp(int level);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitTaskList(PugiXmlNode RootElement);
	int InitTaskReward(PugiXmlNode RootElement);
	int InitFlushPoint(PugiXmlNode RootElement);
	int InitBigqian(PugiXmlNode RootElement);
	int InitQianduoduo(PugiXmlNode RootElement);
	int InitTaskExpCfg(PugiXmlNode RootElement);

	int m_sceneid;														// 天降财宝 场景ID
	int m_attack_get_gold;
	int m_min_level;													// 天降财宝 最小等级
	Posi m_enter_pos;

	std::map<int, TianJiangCaiBaoTaskInfo> m_tianjiangcaibao_task_map;	// 天降财宝 任务信息

	std::vector<Posi> m_flush_point_vec;

	TianJiangCaiBaoBigQianCfg m_tianjiangcaibao_bigqian_cfg;			// 天降财宝 巨型钱多多
	TianJiangCaiBaoQianduoduoCfg m_tianjiangcaibao_qianduoduo_cfg;		// 天降财宝 钱多多
	TianJiangCaiBaoRewardCfg m_reward_list[MAX_REWARD_CFG_COUNT];		// 天降财宝 奖励列表

	int m_task_exp_reward[MAX_ROLE_LEVEL + 1];
};

#endif // __ZHU_XIE_CONFIG_HPP__

