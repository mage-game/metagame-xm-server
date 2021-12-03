#include "tianjiangcaibaoconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/worldshadow/worldshadow.hpp"

TianJiangCaiBaoConfig::TianJiangCaiBaoConfig() : m_sceneid(0), m_attack_get_gold(0), m_min_level(0),m_enter_pos(0,0)
{
	memset(m_task_exp_reward, 0, sizeof(m_task_exp_reward));
}

TianJiangCaiBaoConfig::~TianJiangCaiBaoConfig()
{

}

bool TianJiangCaiBaoConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "TianJiangCaiBaoConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load TianJiangCaiBaoConfig config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	int iRet = 0;

	{																					// ¶ÁÈ¡other
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

	{																					// ¶ÁÈ¡task_list																					
		PugiXmlNode taskListElem = RootElement.child("task_list");
		if (taskListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_list]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTaskList(taskListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskList fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{																					// ¶ÁÈ¡task_reward																					
		PugiXmlNode task_reward = RootElement.child("task_reward");
		if (NULL == task_reward)
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTaskReward(task_reward);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskReward fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode FlushPointListElem = RootElement.child("flush_point");
		if (FlushPointListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->flush_point]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitFlushPoint(FlushPointListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitFlushPoint fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode BigQianCfgListElem = RootElement.child("big_qianduoduo");
		if (BigQianCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->big_qianduoduo]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitBigqian(BigQianCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitBigqian fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode QianduoduoCfgListElem = RootElement.child("qianduoduo");
		if (QianduoduoCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->qianduoduo]", path.c_str());
			*err =  errinfo;
			return false;
		}

		this->InitQianduoduo(QianduoduoCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitQianduoduo fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode QianduoduoCfgListElem = RootElement.child("task_exp_reward");
		if (QianduoduoCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_exp_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		this->InitTaskExpCfg(QianduoduoCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskExpCfg fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	return true;
}

int TianJiangCaiBaoConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_sceneid) || m_sceneid <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_sceneid);		

	if (!PugiGetSubNodeValue(dataElem, "attack_get_diamonds", m_attack_get_gold) || m_attack_get_gold < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "min_level", m_min_level) || m_min_level < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElem, "enter_pos_x", m_enter_pos.x) || m_enter_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElem, "enter_pos_y", m_enter_pos.y) || m_enter_pos.y <= 0)
	{
		return -5;
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitTaskList(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int task_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "task_id", task_id) || task_id < 0)
		{
			return -1;
		}

		TianJiangCaiBaoTaskInfo &task_info = m_tianjiangcaibao_task_map[task_id];
		task_info.task_id = task_id;

		if (!PugiGetSubNodeValue(dataElem, "task_type", task_info.task_type) || task_info.task_type <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_id", task_info.param_id) || task_info.param_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_count", task_info.param_count) || task_info.param_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "reward_exp", task_info.reward_exp) || task_info.reward_exp < 0)
		{
			return -5;
		}

		{
			PugiXmlNode element = dataElem.child("reward_list");
			if (element.empty())
			{
				return -200;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -201 - i;
				}

				if (!task_info.reward_list[i].ReadConfig(item_element))
				{
					return - 251 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(dataElem, "exp_factor_task", task_info.exp_factor_task) || task_info.exp_factor_task < 0)
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitTaskReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElem, "seq", seq) || seq < 0)
		{
			return -1;
		}

		TianJiangCaiBaoRewardCfg &reward_info = m_reward_list[seq];
		reward_info.seq = seq;

		if (!PugiGetSubNodeValue(dataElem, "complete_task_num", reward_info.complete_task_num) || reward_info.complete_task_num <= 0)
		{
			return -2;
		}

		PugiXmlNode Itemelement = dataElem.child("reward_item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item1.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		Itemelement = dataElem.child("reward_item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item2.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		Itemelement = dataElem.child("reward_item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item3.ReadConfig(Itemelement))
			{
				return -5;
			}
		}

		Itemelement = dataElem.child("reward_item4");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item4.ReadConfig(Itemelement))
			{
				return -6;
			}
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitFlushPoint(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		Posi positem;

		if (!PugiGetSubNodeValue(dataElem, "pos_x", positem.x) || positem.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_y", positem.y) || positem.y <= 0)
		{
			return -3;
		}

		m_flush_point_vec.push_back(positem);

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitBigqian(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		TianJiangCaiBaoBigQianCfg &bigqian_cfg = m_tianjiangcaibao_bigqian_cfg;

		if (!PugiGetSubNodeValue(dataElem, "bigqian_id", bigqian_cfg.bigqian_id) || !MONSTERPOOL->IsMonsterExist(bigqian_cfg.bigqian_id))
		{
			return 1;
		}

		if (!PugiGetSubNodeValue(dataElem, "kill_gold", bigqian_cfg.kill_gold) || bigqian_cfg.kill_gold < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "reflush_time_m", bigqian_cfg.reflush_time_m) || bigqian_cfg.reflush_time_m <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "bigqian_x", bigqian_cfg.born_pos.x) || bigqian_cfg.born_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "bigqian_y", bigqian_cfg.born_pos.y) || bigqian_cfg.born_pos.y <= 0)
		{
			return -5;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitQianduoduo(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		TianJiangCaiBaoQianduoduoCfg &qingduoduo_cfg = m_tianjiangcaibao_qianduoduo_cfg;

		if (!PugiGetSubNodeValue(dataElem, "count", qingduoduo_cfg.count) || qingduoduo_cfg.count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "qingduoduo_id", qingduoduo_cfg.qianduoduo_id) || !MONSTERPOOL->IsMonsterExist(qingduoduo_cfg.qianduoduo_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "min_diamonds", qingduoduo_cfg.kill_min_gold) || qingduoduo_cfg.kill_min_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "max_diamonds", qingduoduo_cfg.kill_max_gold) || qingduoduo_cfg.kill_max_gold < qingduoduo_cfg.kill_min_gold)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "reflush_time_m", qingduoduo_cfg.reflush_time_m) || qingduoduo_cfg.reflush_time_m < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "last_reflush_time_m", qingduoduo_cfg.last_reflush_time_m) || qingduoduo_cfg.last_reflush_time_m < 0)
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::InitTaskExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int level = 0;
		int exp = 0;

		if (!PugiGetSubNodeValue(dataElem, "level", level) || level < 0 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "exp_reward", exp) || exp < 0)
		{
			return -2;
		}

		m_task_exp_reward[level] = exp;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int TianJiangCaiBaoConfig::GetEnterInfo(Posi *pos)
{
	if (NULL == pos) return 0;
	*pos = m_enter_pos;
	return m_sceneid;
}

const TianJiangCaiBaoTaskInfo * TianJiangCaiBaoConfig::GetTianJiangCaiBaoTaskCfg(int task_id)
{
	std::map<int, TianJiangCaiBaoTaskInfo>::const_iterator iter = m_tianjiangcaibao_task_map.find(task_id);
	if (m_tianjiangcaibao_task_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const Posi* TianJiangCaiBaoConfig::GetRandomFlushPos()
{
	int count = static_cast<int>(m_flush_point_vec.size());
	if (count > 0)
	{
		int rand_pos = RandomNum(count);
		return &m_flush_point_vec[rand_pos];
	}

	return NULL;
}

const int TianJiangCaiBaoConfig::GetTaskRewardExp(int level)
{
	if (level < 0 || level > MAX_ROLE_LEVEL)
	{
		return 0;
	}

	return m_task_exp_reward[level];
}
