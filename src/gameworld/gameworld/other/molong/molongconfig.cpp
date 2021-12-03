#include "molongconfig.h"

MolongConfig::MolongConfig() : m_max_loop(0), m_day_max_move_step(0)
{

};

MolongConfig::~MolongConfig() 
{

}


bool MolongConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	TiXmlDocument document;
	if ("" == path || !document.LoadFile(path))
	{
		sprintf(err_info, "%s load MolongConfig fail !\n %s", path.c_str(), document.ErrorDesc());
		*err = err_info;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÒÆ¶¯ÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("move");
		if (NULL == root_element)
		{
			*err = path + " xml not move node ";
			return false;
		}

		iRet = this->InitMoveCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitMoveCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ½±ÀøÅäÖÃ
		TiXmlElement *root_element = RootElement->FirstChildElement("reward");
		if (NULL == root_element)
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

	{
		// Í·ÏÎµÈ¼¶ÅäÖÃ

		/*TiXmlElement *root_element = RootElement->FirstChildElement("rank");
		if (NULL == root_element)
		{
			*err = path + " xml not rank node ";
			return false;
		}

		iRet = this->InitRankCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRankCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}*/
	}
	
	return true;
}

const MolongRewardConfig * MolongConfig::GetRewardCfg(int loop, int total_move_step)
{
	if (loop < 0 || loop > m_max_loop || total_move_step <= 0 || total_move_step > MOLONG_MAX_MOVE_STEP)
	{
		return NULL;
	}

	int locate_idx = total_move_step;
	if (MOLONG_MAX_MOVE_STEP == total_move_step)
	{
		locate_idx += loop;
	}

	return &m_reward_cfglist[locate_idx];
}

const MolongMoveConfig * MolongConfig::GetMoveCfg(int move_step)
{
	if (move_step <= 0 || move_step > m_day_max_move_step)
	{
		return NULL;
	}

	return &m_move_cfglist[move_step];
}

int MolongConfig::InitMoveCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	m_day_max_move_step = 0;
	int last_consume_gold = 0;
	while (NULL != dataElement)
	{
		int step = 0;
		if (!GetSubNodeValue(dataElement, "step", step) ||  step > MOLONG_DAY_MAX_MOVE_STEP) 
		{
			return -1;
		}
		m_day_max_move_step = step;

		MolongMoveConfig &move_cfg = m_move_cfglist[step];
		move_cfg.step = step;

		if (!GetSubNodeValue(dataElement, "consume_gold", move_cfg.consume_gold) || move_cfg.consume_gold <= last_consume_gold)
		{
			return -2;
		}
		last_consume_gold = move_cfg.consume_gold;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int MolongConfig::InitRewardCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	int last_grid = 0;
	while (NULL != dataElement)
	{
		int grid = 0;
		if (!GetSubNodeValue(dataElement, "grid", grid) || grid != last_grid + 1 || grid >= MOLONG_REWARD_CONFIG_MAX_COUNT) 
		{
			return -1;
		}
		last_grid = grid;

		MolongRewardConfig &reward_cfg = m_reward_cfglist[grid];

		if (!GetSubNodeValue(dataElement, "fanli_rate", reward_cfg.fanli_rate) || reward_cfg.fanli_rate < 0) 
		{
			return -2;
		}

		TiXmlElement *reward_item_element = dataElement->FirstChildElement("reward_item");
		if (NULL != reward_item_element)
		{
			if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				reward_cfg.reward_item.item_id = 0;
			}
		}

		if (!GetSubNodeValue(dataElement, "virtual_type", reward_cfg.virtual_type) || reward_cfg.virtual_type <= MAIL_VIRTUAL_ITEM_INVALID || reward_cfg.virtual_type >= MAIL_VIRTUAL_ITEM_MAX)
		{
			return -3;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	m_max_loop = last_grid - MOLONG_MAX_MOVE_STEP;

	return 0;
}

int MolongConfig::InitRankCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	int grade = 0;

	while (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "grade", grade) || grade<0 || grade > MOLONG_MAX_RANK_STEP)
		{
			return -1;
		}
		
		MoLongRankConfig m_rank;

		if (!GetSubNodeValue(dataElement, "value_percent", m_rank.value_percent) || m_rank.value_percent<0 )
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "war_value", m_rank.war_value) || m_rank.war_value < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "cumulate_gold", m_rank.cumulate_gold) || m_rank.cumulate_gold < 0)
		{
			return -4;
		}

		m_rank_cfg[grade] = m_rank;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

MoLongRankConfig * MolongConfig::GetRankGradeParamCfg(int m_grade)
{
	std::map<int, MoLongRankConfig>::iterator iter = m_rank_cfg.find(m_grade);

	if (iter != m_rank_cfg.end())
	{
		return &iter->second;
	}

	return NULL;
}