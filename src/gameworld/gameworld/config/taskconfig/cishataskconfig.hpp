#ifndef __CISHA_TASK_CONFIG_HPP__
#define __CISHA_TASK_CONFIG_HPP__

#include <set>
#include <string>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/campdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

struct CishaTaskRewardConfig
{
	CishaTaskRewardConfig() : need_level(0), reward_exp(0) {}

	int need_level;
	int reward_exp;
};

class CishaTaskConfig : public ConfigBase
{
public:
	CishaTaskConfig();
	~CishaTaskConfig();

	bool Init(const std::string &path, std::string *err);

	int CanCommitTimes() { return m_can_commit_times; }
	const ItemConfigData & GetRewardItem() { return m_reward_item; }

	const CishaTaskRewardConfig * GetRewardCfg(int level);
	TaskID RandOneTask(int camp_type);
	bool Acceptable(int camp, TaskID task_id);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitTaskCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);

	int m_can_commit_times;
	ItemConfigData m_reward_item;
	
	typedef std::set<TaskID>  TaskList;
	typedef std::set<TaskID>::iterator TaskListIt;

	TaskList m_acceptable_task_list[CAMP_TYPE_MAX];

	const static int REWARD_CFG_MAX_COUNT = 100;

	int m_reward_cfg_count;
	CishaTaskRewardConfig m_reward_cfg_list[REWARD_CFG_MAX_COUNT];
};

#endif