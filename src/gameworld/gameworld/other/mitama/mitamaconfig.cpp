#include "mitamaconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

MitamaConfig::MitamaConfig() : m_max_mitama_count(0), m_max_task_count(0), m_max_exchange_item_count(0)
{
}

MitamaConfig::~MitamaConfig()
{
}

bool MitamaConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MitamaConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		/*PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}*/
	}

	{
		// 等级配置信息
		PugiXmlNode root_element = RootElement.child("level");
		if (root_element.empty())
		{
			*err = configname + ": no [level].";
			return false;
		}

		iRet = this->InitLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 任务配置-信息
		PugiXmlNode root_element = RootElement.child("task_info");
		if (root_element.empty())
		{
			*err = configname + ": no [task_info].";
			return false;
		}

		iRet = this->InitTaskInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTaskInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 任务配置-奖励
		PugiXmlNode root_element = RootElement.child("task_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [task_reward].";
			return false;
		}

		iRet = this->InitTaskRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTaskRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 兑换配置
		PugiXmlNode root_element = RootElement.child("exchange");
		if (root_element.empty())
		{
			*err = configname + ": no [exchange].";
			return false;
		}

		iRet = this->InitExchangeItemCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExchangeItemCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MitamaConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	return 0;
}

int MitamaConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int last_seq = 0;

	while (!data_element.empty())
	{
	    int seq = 0;
	    if (!PugiGetSubNodeValue(data_element, "seq", seq) || (seq != last_seq && seq != last_seq + 1))
	    {
	        return -1;
	    }

	    if (seq >= MITAMA_MAX_MITAMA_COUNT)
	    {
	        return -1000;
	    }

	    m_level_cfg_list[seq].seq = seq;
	    last_seq = seq;

	    if (m_level_cfg_list[seq].max_level >= MITAMA_MAX_LEVEL)
	    {
	        return -2000;
	    }
	   
	    int level = 0;
	    if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_level_cfg_list[seq].max_level + 1)
	    {
	        return -2;
	    }
	    
		MitamaLevelConfig::LevelConfig &level_cfg = m_level_cfg_list[seq].level_cfg_list[level];
	    level_cfg.level = level;
	    m_level_cfg_list[seq].max_level = level;
	    
		{
			PugiXmlNode item_list_element = data_element.child("consume_item_list");
			if (item_list_element.empty())
			{
				return -100;
			}
			level_cfg.consume_item_count = 0;

			PugiXmlNode item_element = item_list_element.child("consume_item");
			if (item_element.empty())
			{
				return -101;
			}

			while (!item_element.empty())
			{
				if (level_cfg.consume_item_count >= MitamaLevelConfig::MAX_CONSUME_ITEM_COUNT)
				{
					return -102;
				}

				if (!level_cfg.consume_item_list[level_cfg.consume_item_count].ReadConfig(item_element))
				{
					return -103;
				}

				++ level_cfg.consume_item_count;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

	    data_element = data_element.next_sibling();
	}

	m_max_mitama_count = last_seq + 1;

	for (int seq = 0; seq < m_max_mitama_count && seq < MITAMA_MAX_MITAMA_COUNT; ++ seq)
	{
		if (MITAMA_MAX_LEVEL != m_level_cfg_list[seq].max_level)
		{
			return -3000;
		}
	}
	
	return 0;
}

int MitamaConfig::InitTaskInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_task_count = 0;

	while (!data_element.empty())
	{
		if (m_max_task_count >= MITAMA_MAX_TASK_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_max_task_count)
		{
			return -1;
		}

		MitamaTaskInfoConfig &task_info_cfg = m_task_info_cfg_list[seq];
		task_info_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "level", task_info_cfg.level) || task_info_cfg.level <= 0 || task_info_cfg.level > MITAMA_MAX_LEVEL)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(data_element, "need_time", task_info_cfg.need_time) || task_info_cfg.need_time <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_item_count", task_info_cfg.reward_item_count) || task_info_cfg.reward_item_count <= 0)
		{
			return -4;
		}

		++ m_max_task_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int MitamaConfig::InitTaskRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int last_seq = 0;

	while (!data_element.empty())
	{
	    int seq = 0;
	    if (!PugiGetSubNodeValue(data_element, "seq", seq) || (seq != last_seq && seq != last_seq + 1))
	    {
	        return -1;
	    }

	    if (seq >= MITAMA_MAX_TASK_COUNT)
	    {
	        return -1000;
	    }

	    m_task_reward_cfg_list[seq].seq = seq;
	    last_seq = seq;

		if (m_task_reward_cfg_list[seq].reward_count >= MitamaTaskRewardConfig::MAX_REWARD_ITEM_COUNT)
	    {
	        return -2000;
	    }

	    int index = 0;
	    if (!PugiGetSubNodeValue(data_element, "index", index) || index != m_task_reward_cfg_list[seq].reward_count)
	    {
	        return -2;
	    }
	    
		MitamaTaskRewardConfig::RewardInfo &reward_cfg = m_task_reward_cfg_list[seq].reward_info_list[index];
	    reward_cfg.index = index;
	    
		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}
			if (!reward_cfg.reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "weight", reward_cfg.weight) || reward_cfg.weight <= 0)
		{
			return -3;
		}
	    
	    ++ m_task_reward_cfg_list[seq].reward_count;
	    data_element = data_element.next_sibling();
	}
	
	if (m_max_task_count != last_seq + 1)
	{
		return -3000;
	}
	
	return 0;
}

int MitamaConfig::InitExchangeItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_exchange_item_count = 0;

	while (!data_element.empty())
	{
		if (m_max_exchange_item_count >= MITAMA_MAX_EXCHANGE_ITEM_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_max_exchange_item_count)
		{
			return -1;
		}

		MitamaExchangeItemCfg &item_cfg = m_exchange_item_cfg_list[seq];
		item_cfg.seq = seq;
		
		{
			PugiXmlNode item_element = data_element.child("exchange_item");
			if (item_element.empty())
			{
				return -100;
			}
			if (!item_cfg.exchange_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "need_score", item_cfg.need_score) || item_cfg.need_score <= 0)
		{
			return -2;
		}

		++ m_max_exchange_item_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const MitamaLevelConfig::LevelConfig * MitamaConfig::GetLevelCfg(int seq, int level)
{
	if (seq < 0 || seq >= m_max_mitama_count || seq >= MITAMA_MAX_MITAMA_COUNT)
	{
		return NULL;
	}

	if (level < 0 || level > m_level_cfg_list[seq].max_level || level > MITAMA_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_level_cfg_list[seq].level_cfg_list[level];
}

const MitamaTaskInfoConfig * MitamaConfig::GetTaskInfoCfg(int seq)
{
	if (seq < 0 || seq >= m_max_task_count || seq >= MITAMA_MAX_TASK_COUNT)
	{
		return NULL;
	}

	return &m_task_info_cfg_list[seq];
}

const MitamaTaskRewardConfig::RewardInfo * MitamaConfig::GetRandomTaskRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_max_task_count || seq >= MITAMA_MAX_TASK_COUNT)
	{
		return NULL;
	}

	const MitamaTaskRewardConfig &reward_cfg = m_task_reward_cfg_list[seq];

	// 总权重
	int total_weight = 0;
	for (int index = 0; index < reward_cfg.reward_count && index < MitamaTaskRewardConfig::MAX_REWARD_ITEM_COUNT; ++ index)
	{
		total_weight += reward_cfg.reward_info_list[index].weight;
	}

	if (total_weight <= 0)
	{
		return NULL;
	}

	// 计算
	int rand_val = RandomNum(total_weight);
	for (int index = 0; index < reward_cfg.reward_count && index < MitamaTaskRewardConfig::MAX_REWARD_ITEM_COUNT; ++ index)
	{
		if (rand_val < reward_cfg.reward_info_list[index].weight)
		{
			return &reward_cfg.reward_info_list[index];
		}

		rand_val -= reward_cfg.reward_info_list[index].weight;
	}

	return NULL;
}

const MitamaExchangeItemCfg * MitamaConfig::GetExchangeItemCfg(int seq)
{
	if (seq < 0 || seq >= m_max_exchange_item_count || seq >= MITAMA_MAX_EXCHANGE_ITEM_COUNT)
	{
		return NULL;
	}

	return &m_exchange_item_cfg_list[seq];
}
