#include "shuijingconfig.hpp"
#include "servercommon/configcommon.h"

ShuiJingConfig::ShuiJingConfig(): m_task_exp_per_cfg_count(0)
{	
}

ShuiJingConfig::~ShuiJingConfig()
{
}

bool ShuiJingConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ActiveDegreeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load ShuiJingConfig config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	int iRet = 0;

	{
		PugiXmlNode otherElem = RootElement.child("other");
		if (otherElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitOther(otherElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOther fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode GatherListElem = RootElement.child("gather");
		if (GatherListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->gather]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitGather(GatherListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitGather fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode GatherListElem = RootElement.child("task");
		if (GatherListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTask(GatherListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTask fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode GatherListElem = RootElement.child("task_exp_xishu");
		if (GatherListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_exp_xishu]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTaskExpPer(GatherListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskExpPer fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}


	return true;
}

int ShuiJingConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_max_times", m_other_cfg.gather_max_times) || m_other_cfg.gather_max_times < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_time", m_other_cfg.relive_time) || m_other_cfg.relive_time < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElem, "free_relive_count", m_other_cfg.free_relive_count) || m_other_cfg.free_relive_count < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_pos_x", m_other_cfg.realive_pos.x) || m_other_cfg.realive_pos.x <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_pos_y", m_other_cfg.realive_pos.y) || m_other_cfg.realive_pos.y <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_gold", m_other_cfg.gather_buff_gold) || m_other_cfg.gather_buff_gold <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_time", m_other_cfg.gather_buff_time) || m_other_cfg.gather_buff_time <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElem, "reflush_notice_time", m_other_cfg.reflush_notice_time) || m_other_cfg.reflush_notice_time <= 0)
	{
		return -9;
	}

	return 0;
}

int ShuiJingConfig::InitGather(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int gather_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "gather_id", gather_id) || gather_id <= 0 || m_gather_cfg_map.find(gather_id) != m_gather_cfg_map.end())
		{
			return -1;
		}

		ShuiJingGatherCfg &cfg = m_gather_cfg_map[gather_id];
		if (0 != cfg.gather_id)
		{
			return -100;
		}

		cfg.gather_id = gather_id;

		if (!PugiGetSubNodeValue(dataElem, "gather_type", cfg.gather_type) || cfg.gather_type <= SHUIJING_TYPE_INVALID || cfg.gather_type >= SHUIJING_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "gather_sub_type", cfg.gather_sub_type) || cfg.gather_sub_type <= SHUIJING_SUB_TYPE_INVALID || cfg.gather_sub_type >= SHUIJING_SUB_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "bind_gold", cfg.bind_gold) || cfg.bind_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "mojing", cfg.mojing) || cfg.mojing < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "shengwang", cfg.shengwang) || cfg.shengwang < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "broadcast", cfg.is_broadcast))
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ShuiJingConfig::InitTask(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int task_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "task_id", task_id) || task_id < 0 || task_id >= MAX_SHUIJING_TASK_COUNT)
		{
			return -1;
		}

		ShuiJingTaskCfg &cfg = m_task_cfg[task_id];
		cfg.task_id = task_id;

		if (!PugiGetSubNodeValue(dataElem, "gather_count", cfg.need_gather_count) || cfg.need_gather_count < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "reward_exp", cfg.reward_exp) || cfg.reward_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "is_notice", cfg.is_notice) || cfg.is_notice < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "exp_factor_task", cfg.exp_factor_task) || cfg.exp_factor_task < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "shuijing", cfg.shuijing) || cfg.shuijing < SHUIJING_TOTAL || cfg.shuijing >= SHUIJING_MAX)
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ShuiJingConfig::InitTaskExpPer(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_level = 0;
	m_task_exp_per_cfg_count = 0;

	while (!dataElem.empty())
	{
		if (m_task_exp_per_cfg_count < 0 || m_task_exp_per_cfg_count >= MAX_SHUIJING_TASK_EXP_PER_COUNT)
		{
			return -1;
		}

		ShuiJingTaskExpPerCfg &cfg = m_task_exp_per_cfg[m_task_exp_per_cfg_count];

		if (!PugiGetSubNodeValue(dataElem, "min_level", cfg.min_level) || cfg.min_level < 0 || cfg.min_level > MAX_ROLE_LEVEL || cfg.min_level != last_level + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "max_level", cfg.max_level) || cfg.max_level < 0 || cfg.max_level < cfg.min_level || cfg.max_level > MAX_ROLE_LEVEL)
		{
			return -3;
		}
		last_level = cfg.max_level;

		if (!PugiGetSubNodeValue(dataElem, "exp_per", cfg.exp_per) || cfg.exp_per < 0)
		{
			return -4;
		}

		m_task_exp_per_cfg_count++;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ShuiJingConfig::GetShuiJingEnterInfo(Posi *enter_pos)
{
	if (NULL != enter_pos) *enter_pos = m_other_cfg.realive_pos;
	return m_other_cfg.scene_id;
}

const ShuiJingGatherCfg * ShuiJingConfig::GetGatherCfgByGatherId(int gather_id)
{
	std::map<int, ShuiJingGatherCfg>::iterator it = m_gather_cfg_map.find(gather_id);
	if (it == m_gather_cfg_map.end())
	{
		return NULL;
	}

	return &(it->second);
}

bool ShuiJingConfig::IsBestShuiJing(int gather_id)
{
	std::map<int, ShuiJingGatherCfg>::iterator it = m_gather_cfg_map.find(gather_id);
	if (it == m_gather_cfg_map.end())
	{
		return false;
	}

	return it->second.gather_type == SHUIJING_TYPE_BEST;
}

const ShuiJingTaskCfg * ShuiJingConfig::GetTaskCfgByTaskID(int task_id)
{
	if (task_id <0 || task_id >= MAX_SHUIJING_TASK_COUNT)
	{
		return NULL;
	}

	ShuiJingTaskCfg &task_cfg = m_task_cfg[task_id];
	if (task_cfg.task_id != task_id)
	{
		return NULL;
	}

	return &task_cfg;
}

int ShuiJingConfig::GetTaskExpPerByLevle(int level)
{
	for(int i = 0; i < m_task_exp_per_cfg_count && i < MAX_SHUIJING_TASK_EXP_PER_COUNT; i++)
	{
		ShuiJingTaskExpPerCfg &exp_per_cfg = m_task_exp_per_cfg[i];
		if (level >= exp_per_cfg.min_level && level <= exp_per_cfg.max_level)
		{
			return exp_per_cfg.exp_per;
		}
	}

	return 0;
}
