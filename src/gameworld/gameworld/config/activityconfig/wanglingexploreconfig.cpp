#include "wanglingexploreconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include <algorithm>

WangLingExploreConfig::WangLingExploreConfig()
{
	memset(m_task_exp_reward, 0, sizeof(m_task_exp_reward));
}

WangLingExploreConfig::~WangLingExploreConfig()
{

}

bool WangLingExploreConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "WangLingExploreConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load WangLingExploreConfig config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	int iRet = 0;

	{																					// 读取other
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

	{																					// 读取task_list																					
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

	{																					// 读取task_extra_reward																					
		PugiXmlNode task_extra_reward = RootElement.child("task_extra_reward");
		if (NULL == task_extra_reward)
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_extra_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTaskExtraReward(task_extra_reward);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskExtraReward fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode FlushPointListElem = RootElement.child("flush_point");	// 读取刷新点列表
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
		PugiXmlNode bossElem = RootElement.child("boss");	// 读取BOSS信息
		if (bossElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->boss]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitBoss(bossElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitBoss fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode monsterElem = RootElement.child("monster");	// 读取怪野信息
		if (monsterElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->monster]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitMonster(monsterElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitMonster fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode gatherElem = RootElement.child("gather");	// 读取采集物
		if (gatherElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->gather]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitGather(gatherElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitGather fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode reward_poolElem = RootElement.child("reward_pool");	// 读取奖池
		if (reward_poolElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->reward_pool]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitRewardPool(reward_poolElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitRewardPool fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode reward_poolElem = RootElement.child("task_exp_reward");	
		if (reward_poolElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_exp_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitTaskExpCfg(reward_poolElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitTaskExpCfg fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode reward_poolElem = RootElement.child("boss_id");	
		if (reward_poolElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->boss_id]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitBoosIdByWorldLevel(reward_poolElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitWorldLevelToBoosId fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	return true;
}

int WangLingExploreConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_wang_ling_explore_other_cfg.m_sceneid) || m_wang_ling_explore_other_cfg.m_sceneid <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_wang_ling_explore_other_cfg.m_sceneid);		

	if (!PugiGetSubNodeValue(dataElem, "min_level", m_wang_ling_explore_other_cfg.m_min_level) || m_wang_ling_explore_other_cfg.m_min_level < 0)
	{
		return -2;
	}

	{
		PugiXmlNode element = dataElem.child("reward_gold_box_item_list");
		if (element.empty())
		{
			return -3;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("reward_gold_box_item");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -4;
			}

			ItemID itemid = 0;
			if (NULL != item_element && PugiGetSubNodeValue(item_element, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
			{
				if (!m_wang_ling_explore_other_cfg.reward_gold_box_item_list[i].ReadConfig(item_element))
				{
					return -5;
				}

			}else
			{
				return -6;
			}

			m_wang_ling_explore_other_cfg.item_count++;
			i++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(dataElem, "reward_gold_box_bind_diamone", m_wang_ling_explore_other_cfg.reward_gold_box_bind_diamone) || m_wang_ling_explore_other_cfg.reward_gold_box_bind_diamone < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElem, "double_reward_limit_time", m_wang_ling_explore_other_cfg.double_reward_limit_time) || m_wang_ling_explore_other_cfg.double_reward_limit_time < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_x", m_wang_ling_explore_other_cfg.relive_pos.x) || m_wang_ling_explore_other_cfg.relive_pos.x < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_y", m_wang_ling_explore_other_cfg.relive_pos.y) || m_wang_ling_explore_other_cfg.relive_pos.y < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_gold", m_wang_ling_explore_other_cfg.gather_buff_gold) || m_wang_ling_explore_other_cfg.gather_buff_gold < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_time", m_wang_ling_explore_other_cfg.gather_buff_time) || m_wang_ling_explore_other_cfg.gather_buff_time < 0)
	{
		return -12;
	}

	return 0;
}

int WangLingExploreConfig::InitTaskList(PugiXmlNode RootElement)
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

		WangLingExploreTaskInfo &task_info = m_wanglingexplore_task_map[task_id];
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
			PugiXmlNode element = dataElem.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!task_info.reward_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				
				task_info.item_count++;
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

int WangLingExploreConfig::InitTaskExtraReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		WangLingExploreExtraRewardCfg &reward_info = m_reward_item_cfg;

		PugiXmlNode Itemelement = dataElem.child("reward_item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item1.ReadConfig(Itemelement))
			{
				return -1;
			}
		}

		Itemelement = dataElem.child("reward_item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item2.ReadConfig(Itemelement))
			{
				return -2;
			}
		}

		Itemelement = dataElem.child("reward_item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item3.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		Itemelement = dataElem.child("reward_item4");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_info.item4.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WangLingExploreConfig::InitFlushPoint(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int id = 0;
		if (!PugiGetSubNodeValue(dataElem, "id", id) || id < 0)
		{
			return -1;
		}
		Posi& pos = m_flush_point_map[id];

		if (!PugiGetSubNodeValue(dataElem, "pos_x", pos.x) || pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_y", pos.y) || pos.y <= 0)
		{
			return -3;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WangLingExploreConfig::InitBoss(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		WangLingExploreBossCfg &boss_cfg = m_wanglingexplore_boss_cfg;

		if (!PugiGetSubNodeValue(dataElem, "boss_id", boss_cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(boss_cfg.boss_id))
		{
			return 1;
		}

		if (!PugiGetSubNodeValue(dataElem, "count", boss_cfg.count) || boss_cfg.count <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "start_reflush_time_s", boss_cfg.start_reflush_time_s) || boss_cfg.start_reflush_time_s < 0)
		{
			return 3;
		}

		if (!PugiGetSubNodeValue(dataElem, "reflush_time_s", boss_cfg.reflush_time_s) || boss_cfg.reflush_time_s < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", boss_cfg.pos_id_start) || boss_cfg.pos_id_start < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", boss_cfg.pos_id_len) || boss_cfg.pos_id_len <= 0)
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WangLingExploreConfig::InitMonster(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		WangLingExploreMonsterCfg &monster_cfg = m_wanglingexplore_monster_cfg;

		if (!PugiGetSubNodeValue(dataElem, "count", monster_cfg.count) || monster_cfg.count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "monster_id", monster_cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(monster_cfg.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "start_reflush_time_s", monster_cfg.start_reflush_time_s) || monster_cfg.start_reflush_time_s < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "reflush_time_s", monster_cfg.reflush_time_s) || monster_cfg.reflush_time_s < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", monster_cfg.pos_id_start) || monster_cfg.pos_id_start < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", monster_cfg.pos_id_len) || monster_cfg.pos_id_len <= 0)
		{
			return -6;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

void WangLingExploreConfig::GetWangLingExploreTaskList( WangLingExploreTaskInfo wanglingexplore_task_list[WANGLINGEXPLORE_TASK_MAX])
{
	int count = 0;
	std::map<int, WangLingExploreTaskInfo>::iterator iter;
	for (iter = m_wanglingexplore_task_map.begin(); m_wanglingexplore_task_map.end() != iter; ++ iter)
	{
		if (count >= WANGLINGEXPLORE_TASK_MAX)
		{
			return;
		}

		WangLingExploreTaskInfo &task_cfg = iter->second;

		wanglingexplore_task_list[count] = task_cfg;
		++ count;
	}
}

const WangLingExploreTaskInfo * WangLingExploreConfig::GetWangLingExploreTaskCfg(int task_id)
{
	std::map<int, WangLingExploreTaskInfo>::const_iterator iter = m_wanglingexplore_task_map.find(task_id);
	if (m_wanglingexplore_task_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

bool WangLingExploreConfig::GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len)
{
	if (id_star < 0 || id_len <= 0)
	{
		return false;
	}

	if (m_flush_point_map.end() == m_flush_point_map.find(id_star))
	{
		return false;
	}

	int rand_len = RandomNum(id_len);
	if (m_flush_point_map.end() != m_flush_point_map.find(id_star + rand_len))
	{
		flush_pos = m_flush_point_map[id_star + rand_len];
	}
	else
	{
		flush_pos = m_flush_point_map[id_star];
	}

	return true;
}

bool WangLingExploreConfig::GetRandomFlushPos(std::vector<Posi>& flush_pos, int count, int id_star, int id_len)
{
	if (count <= 0 ||count > id_len)
	{
		return false;
	}

	if (m_flush_point_map.end() == m_flush_point_map.find(id_star +id_len))
	{
		return false;
	}

	//掉落点不重复
	std::vector<int> len_vec;
	for (int j = 0; j <id_len; j++)
	{
		len_vec.push_back(j);
	}
	std::random_shuffle(len_vec.begin(), len_vec.end());
	for (int i = 0; i < count; i++)
	{
		std::map<int, Posi>::iterator itr = m_flush_point_map.find(id_star + len_vec[i]);
		if (m_flush_point_map.end() != itr )
		{
			flush_pos.push_back(itr->second);
		}
		else
		{
			return false;
		}
	}
	
	return true;
}


const RewardPool::RewardItem * WangLingExploreConfig::GetRandomRewardItem()
{
	int seq = RandomNum(m_reward_pool_cfg.cfg_count);
	if (seq < 0 || seq >= RewardPool::MAX_REWARDPOOL_CFG_COUNT)
	{
		return NULL;
	}

	return &m_reward_pool_cfg.item_list[seq];
}

int WangLingExploreConfig::InitGather(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int gather_index = -1;
		if (!PugiGetSubNodeValue(dataElem, "gather_index", gather_index) || gather_index < 0 || gather_index >= WangLingExploreGatherCfg::MAX_GATHER_CFG_COUNT)
		{
			return -1;
		}

		WangLingExploreGatherCfg::GatherItem &gather_item = m_gather_cfg.gather_list[gather_index];

		if (!PugiGetSubNodeValue(dataElem, "gather_id", gather_item.gather_id) || gather_item.gather_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "gather_time", gather_item.gather_time) || gather_item.gather_time < 0)
		{
			return 3;
		}

		if (!PugiGetSubNodeValue(dataElem, "count", gather_item.count) || gather_item.count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", gather_item.pos_id_start) || gather_item.pos_id_start < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", gather_item.pos_id_len) || gather_item.pos_id_len <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "disappear_s", gather_item.disappear_s) || gather_item.disappear_s < 0)
		{
			return -7;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WangLingExploreConfig::InitRewardPool(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(dataElem, "seq", seq) || seq < 0 || seq >= RewardPool::MAX_REWARDPOOL_CFG_COUNT)
		{
			return -1;
		}

		RewardPool::RewardItem &reward_item = m_reward_pool_cfg.item_list[seq];
		reward_item.seq = seq;

		if (!PugiGetSubNodeValue(dataElem, "reward_bind_cost", reward_item.reward_bind_cost) || reward_item.reward_bind_cost < 0)
		{
			return -2;
		}

		PugiXmlNode Itemelement = dataElem.child("reward_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!reward_item.item.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		++ m_reward_pool_cfg.cfg_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int WangLingExploreConfig::InitTaskExpCfg(PugiXmlNode RootElement)
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

const int WangLingExploreConfig::GetTaskRewardExp(int level)
{
	if (level < 0 || level > MAX_ROLE_LEVEL)
	{
		return 0;
	}

	return m_task_exp_reward[level];
}

int WangLingExploreConfig::InitBoosIdByWorldLevel(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while(!dataElem.empty())
	{
		if (m_boos_pool.count >= BoosPool::MAX_BOOS_ID_COUNT) return -1;
		
		int last_world_level = -1;
		int world_level = -1;
		if (!PugiGetSubNodeValue(dataElem, "world_level", world_level) || world_level < 0 || world_level <= last_world_level )
		{
			return -2;
		}

		if (0 == m_boos_pool.count && 0 != world_level)
		{
			return -3;
		}
		
		last_world_level = world_level;
		BoosPool::BoosItem &boos_item = m_boos_pool.item_list[m_boos_pool.count];
		boos_item.world_level = world_level;

		if (!PugiGetSubNodeValue(dataElem, "boos_id", boos_item.boos_id) || !MONSTERPOOL->IsMonsterExist(boos_item.boos_id))
		{
			return -4;
		}
		
		m_boos_pool.count ++;

		dataElem = dataElem.next_sibling();
	}
	return 0;
}

int WangLingExploreConfig::GetBoosIdByWorldLevel(int world_level)
{
	if (world_level < 0 || world_level > MAX_ROLE_LEVEL) return -1;

	for (int i = m_boos_pool.count - 1; i >= 0; i--)
	{
		if (world_level >= m_boos_pool.item_list[i].world_level)
		{
			return m_boos_pool.item_list[i].boos_id;
		}
	}

	return -1;
}
