#include "cishataskconfig.hpp"
#include "servercommon/configcommon.h"
#include "task/taskpool.h"

CishaTaskConfig::CishaTaskConfig() : m_can_commit_times(0), m_reward_cfg_count(0)
{

}

CishaTaskConfig::~CishaTaskConfig()
{

}

bool CishaTaskConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CishaTaskConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 杂项配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 任务配置
		PugiXmlNode root_element = RootElement.child("task_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not task_cfg node ";
			return false;
		}

		iRet = this->InitTaskCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitTaskCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 奖励配置
		PugiXmlNode root_element = RootElement.child("reward");
		if (root_element.empty())
		{
			*err = path + " xml not reward node ";
			return false;
		}

		iRet = this->InitRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const CishaTaskRewardConfig * CishaTaskConfig::GetRewardCfg(int level)
{
	for (int i = m_reward_cfg_count - 1; i >= 0; -- i)
	{
		if (level >= m_reward_cfg_list[i].need_level)
		{
			return &m_reward_cfg_list[i];
		}
	}

	return NULL;
}

TaskID CishaTaskConfig::RandOneTask(int camp_type)
{
	TaskID task_id = INVALID_TASK_ID;

	if (camp_type > CAMP_TYPE_INVALID && camp_type < CAMP_TYPE_MAX)
	{
		TaskList &task_list = m_acceptable_task_list[camp_type];
		int task_count = static_cast<int>(task_list.size());

		if (task_count > 0)
		{
			int rand_value = RandomNum(task_count);

			TaskListIt it = task_list.begin();
			advance(it, rand_value);
			task_id = *it;
		}
	}

	return task_id;
}

bool CishaTaskConfig::Acceptable(int camp, TaskID task_id)
{
	if (camp > CAMP_TYPE_INVALID && camp < CAMP_TYPE_MAX)
	{
		TaskListIt it = m_acceptable_task_list[camp].find(task_id);
		if (m_acceptable_task_list[camp].end() != it)
		{
			return true;
		}
	}

	return false;
}

int CishaTaskConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "can_commit_times", m_can_commit_times) || m_can_commit_times <= 0)
		{
			return -1;
		}
	}

	PugiXmlNode reward_item_element = dataElement.child("reward_item");
	if (reward_item_element.empty())
	{
		return -2;
	}

	if (!m_reward_item.ReadConfig(reward_item_element))
	{
		return -3;
	}

	return 0;
}

int CishaTaskConfig::InitTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	TaskList task_repeat_checker;

	while (!dataElement.empty())
	{
		TaskID task_id = INVALID_TASK_ID;

		if (!PugiGetSubNodeValue(dataElement, "task_id", task_id))
		{
			return -1;
		}

		if (task_repeat_checker.end() != task_repeat_checker.find(task_id))
		{
			return -2;
		}
		task_repeat_checker.insert(task_id);

		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(static_cast<int>(task_id));
		if (NULL == task_cfg)
		{
			return -3;
		}

		if (0 != task_cfg->camp) // 不能有阵营限制
		{
			return -4;
		}

		if (task_cfg->min_level > 1 || task_cfg->max_level < MAX_ROLE_LEVEL) // 不能有等级限制
		{
			return -5;
		}

		if (0 != task_cfg->pretaskid) // 不能有前置任务
		{
			return -6;
		}

		if (TASK_TYPE_CISHA != task_cfg->task_type)
		{ 
			return -8;
		}

		int camp_can_accpet = 0;

		if (!PugiGetSubNodeValue(dataElement, "camp_1_can_accept", camp_can_accpet))
		{
			return -9;
		}
		if (0 != camp_can_accpet)
		{
			m_acceptable_task_list[CAMP_TYPE_FEIXING].insert(task_id);
		}

		if (!PugiGetSubNodeValue(dataElement, "camp_2_can_accept", camp_can_accpet))
		{
			return -10;
		}
		if (0 != camp_can_accpet)
		{
			m_acceptable_task_list[CAMP_TYPE_ZHURI].insert(task_id);
		}

		if (!PugiGetSubNodeValue(dataElement, "camp_3_can_accept", camp_can_accpet))
		{
			return -11;
		}
		if (0 != camp_can_accpet)
		{
			m_acceptable_task_list[CAMP_TYPE_ZHUIYUE].insert(task_id);
		}

		dataElement = dataElement.next_sibling();
	}

	for (int i = CAMP_TYPE_INVALID + 1; i < CAMP_TYPE_MAX; ++ i)
	{
		if (m_acceptable_task_list[i].empty())
		{
			return -1000;
		}
	}
	
	return 0;
}

int CishaTaskConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;

	while (!dataElement.empty())
	{
		if (m_reward_cfg_count >= REWARD_CFG_MAX_COUNT)
		{
			return -1;
		}

		CishaTaskRewardConfig &reward_cfg = m_reward_cfg_list[m_reward_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "level", reward_cfg.need_level) || reward_cfg.need_level <= last_level)
		{
			return -2;
		}
		last_level = reward_cfg.need_level;

		if (!PugiGetSubNodeValue(dataElement, "exp", reward_cfg.reward_exp) ||reward_cfg.reward_exp < 0) 
		{
			return -3;
		}

		++ m_reward_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (1 != m_reward_cfg_list[0].need_level)
	{
		return -1000;
	}

	return 0;
}


