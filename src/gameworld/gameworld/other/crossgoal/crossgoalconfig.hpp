#ifndef __CROSS_GOAL_CONFIG_HPP__
#define __CROSS_GOAL_CONFIG_HPP__

#include <vector>
#include "servercommon/crossgoaldef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int CROSS_GOAL_ITEM_CFG_MAX = CROSS_GOAL_TASK_COUNT_MAX;

enum CROSS_GOAL_COND_TYPE
{
	CROSS_GOAL_COND_INVALID_TYPE = 0,
	CROSS_GOAL_COND_KILL_CROSS_BOSS = 1,  //击杀神域boss数量
	CROSS_GOAL_COND_CROSS_BOSS_ROLE_KILLER = 2,//神域boss场景杀人数
	CROSS_GOAL_COND_FINISH_BAIZHAN_TASK= 3,	//完成百战任务
	CROSS_GOAL_COND_KILL_BAIZHAN_BOSS = 4,	// 击杀百战boss
	CROSS_GOAL_COND_GUILD_KILL_CROSS_BOSS = 5,	// 公会击杀神域boss
	CROSS_GOAL_COND_GUILD_KILL_BAIZHAN_BOSS = 6,// 公会击杀百战boss

	CROSS_GOAL_COND_FINISH_ALL_BEFORE = 100,	//终极目标
	CROSS_GOAL_COND_MAX_TYPE,
};

struct CrossGoalOtherCfg
{
	CrossGoalOtherCfg():open_day_beg(0),open_day_end(0), guild_notify_sec(0){}
	int open_day_beg;
	int open_day_end;
	unsigned int guild_notify_sec;
};

struct CrossGoalItem
{
	CrossGoalItem():index(0),cond_type(0){}
	int index;
	int cond_type;	
	std::vector<int> cond_param;	
	ItemConfigData goal_reward[MAX_ATTACHMENT_ITEM_NUM];//奖励
};


class CrossGoalConfig : public ConfigBase
{
public:
	CrossGoalConfig();
	~CrossGoalConfig();

	bool Init(std::string path, std::string *err);
	const CrossGoalOtherCfg& GetOtherCfg() { return m_other_cfg; }
	const CrossGoalItem * GetCrossGoalItemByIndex(int index) const;
	const CrossGoalItem * GetGuildGoalItemByIndex(int index) const;
	bool HaveGuildGoalFinish(int cond_type,int param,int *flag);
	bool CheckGuildFinalGoal(int *flag);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGoalItemCfg(PugiXmlNode RootElement);
	int InitGuildItemCfg(PugiXmlNode RootElement);
	
	CrossGoalOtherCfg m_other_cfg;
	int m_cross_goal_item_count;
	CrossGoalItem m_cross_goal_item_list[CROSS_GOAL_ITEM_CFG_MAX];

	int m_guild_goal_item_count;
	CrossGoalItem m_guild_goal_item_list[CROSS_GOAL_ITEM_CFG_MAX];
};

#endif

