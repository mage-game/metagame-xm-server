
#include "crossbianjingzhidiconfig.h"

#include <algorithm>
#include "servercommon/configcommon.h"


CrossBianJingZhiDiConfig::CrossBianJingZhiDiConfig()
{

}

CrossBianJingZhiDiConfig::~CrossBianJingZhiDiConfig()
{

}

CrossBianJingZhiDiConfig & CrossBianJingZhiDiConfig::Instance()
{
	static CrossBianJingZhiDiConfig _instance;
	return _instance;
}

bool CrossBianJingZhiDiConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossBianJingZhiDiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load CrossBianJingZhiDiConfig config [%s] Error.cannot find root node.", path.c_str());
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

	{
		PugiXmlNode reward_poolElem = RootElement.child("activity_open_time");
		if (reward_poolElem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->activity_open_time]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(reward_poolElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitActivityOpenTimeCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("sos_cfg");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->sos_cfg]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitSosCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSosCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossBianJingZhiDiConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElem, "is_open", is_open) || is_open < 0)
	{
		return -1;
	}
	m_bianjingzhidi_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(dataElem, "server_open_day", m_bianjingzhidi_other_cfg.server_open_day) || m_bianjingzhidi_other_cfg.server_open_day <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "min_level", m_bianjingzhidi_other_cfg.m_min_level) || m_bianjingzhidi_other_cfg.m_min_level < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_bianjingzhidi_other_cfg.m_sceneid) || m_bianjingzhidi_other_cfg.m_sceneid <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_x", m_bianjingzhidi_other_cfg.relive_pos.x) || m_bianjingzhidi_other_cfg.relive_pos.x < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElem, "relive_y", m_bianjingzhidi_other_cfg.relive_pos.y) || m_bianjingzhidi_other_cfg.relive_pos.y < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElem, "double_reward_limit_time", m_bianjingzhidi_other_cfg.double_reward_limit_time) || m_bianjingzhidi_other_cfg.double_reward_limit_time < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_gold", m_bianjingzhidi_other_cfg.gather_buff_gold) || m_bianjingzhidi_other_cfg.gather_buff_gold < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElem, "gather_buff_time", m_bianjingzhidi_other_cfg.gather_buff_time) || m_bianjingzhidi_other_cfg.gather_buff_time < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElem, "start_reflush_time_s", m_bianjingzhidi_other_cfg.start_reflush_time_s) || m_bianjingzhidi_other_cfg.start_reflush_time_s < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElem, "reflush_time_s", m_bianjingzhidi_other_cfg.reflush_time_s) || m_bianjingzhidi_other_cfg.reflush_time_s < 0)
	{
		return -11;
	}

	return 0;
}

int CrossBianJingZhiDiConfig::InitTaskList(PugiXmlNode RootElement)
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

		CrossBianJingZhiDiTaskInfo &task_info = m_bianjingzhidi_task_map[task_id];
		task_info.task_id = task_id;

		int can_limit_flag = 0;
		if (!PugiGetSubNodeValue(dataElem, "can_limit_flag", can_limit_flag) || can_limit_flag < 0)
		{
			return -2;
		}
		task_info.can_limit_flag = (can_limit_flag != 0);

		if (!PugiGetSubNodeValue(dataElem, "task_type", task_info.task_type) || task_info.task_type <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_id", task_info.param_id) || task_info.param_id <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_count", task_info.param_count) || task_info.param_count <= 0)
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

				if (!task_info.reward_item[i].ReadConfigNoCheck(item_element))
				{
					return - 151 - i;
				}

				i++;
				
				task_info.item_count++;
				item_element = item_element.next_sibling();
			}
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CrossBianJingZhiDiConfig::InitFlushPoint(PugiXmlNode RootElement)
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

const CrossBianJingZhiDiBossCfg* CrossBianJingZhiDiConfig::GetCrossBianJingZhiDiBossCfg(int boss_type)
{
	if (boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
	{
		return nullptr;
	}

	return &m_bianjingzhidi_boss_cfg[boss_type];
}

const int CrossBianJingZhiDiConfig::GetBossTypeByBossId(int boss_id)
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		for (int i = m_boss_pool[boss_type].count - 1; i >= 0; i--)
		{
			if (m_boss_pool[boss_type].item_list[i].boss_id == boss_id && boss_id > 0)
			{
				return boss_type;
			}
		}
	}

	return -1;
}

int CrossBianJingZhiDiConfig::InitBoss(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int boss_type = 0;
		if (!PugiGetSubNodeValue(dataElem, "boss_type", boss_type) || boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
		{
			return 1;
		}

		CrossBianJingZhiDiBossCfg &boss_cfg = m_bianjingzhidi_boss_cfg[boss_type];
		boss_cfg.boss_type = boss_type;

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

int CrossBianJingZhiDiConfig::InitMonster(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		CrossBianJingZhiDiMonsterCfg &monster_cfg = m_bianjingzhidi_monster_cfg;

		if (!PugiGetSubNodeValue(dataElem, "count", monster_cfg.count) || monster_cfg.count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "monster_id", monster_cfg.monster_id) /*|| !MONSTERPOOL->IsMonsterExist(monster_cfg.monster_id)*/)
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

void CrossBianJingZhiDiConfig::GetCrossBianJingZhiDiTaskList(CrossBianJingZhiDiTaskInfo bianjingzhidi_task_list[CROSS_BIANJINGZHIDI_TASK_MAX])
{
	int count = 0;
	std::map<int, CrossBianJingZhiDiTaskInfo>::iterator iter;
	for (iter = m_bianjingzhidi_task_map.begin(); m_bianjingzhidi_task_map.end() != iter; ++ iter)
	{
		if (count >= CROSS_BIANJINGZHIDI_TASK_MAX)
		{
			return;
		}

		CrossBianJingZhiDiTaskInfo &task_cfg = iter->second;

		bianjingzhidi_task_list[count] = task_cfg;
		++ count;
	}
}

const CrossBianJingZhiDiTaskInfo * CrossBianJingZhiDiConfig::GetCrossBianJingZhiDiTaskCfg(int task_id)
{
	std::map<int, CrossBianJingZhiDiTaskInfo>::const_iterator iter = m_bianjingzhidi_task_map.find(task_id);
	if (m_bianjingzhidi_task_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const CrossBianJingZhiDiGatherCfg::GatherItem *CrossBianJingZhiDiConfig::GetCrossBianJingZhiDiGatherCfg(int boss_type)
{
	if (boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE) return nullptr;

	if (m_gather_cfg.gather_list[boss_type].gather_id > 0)
	{
		return &m_gather_cfg.gather_list[boss_type];
	}

	return nullptr;
}

const CrossBianJingZhiDiGatherCfg::GatherItem *CrossBianJingZhiDiConfig::GetCrossBianJingZhiDiGatherCfgByID(int gather_id)
{
	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++ boss_type)
	{
		if (m_gather_cfg.gather_list[boss_type].gather_id == gather_id)
		{
			return &m_gather_cfg.gather_list[boss_type];
		}
	}

	return nullptr;
}

bool CrossBianJingZhiDiConfig::GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len)
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

bool CrossBianJingZhiDiConfig::GetRandomFlushPos(std::vector<Posi>& flush_pos, int count, int id_star, int id_len)
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

int CrossBianJingZhiDiConfig::InitGather(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int boss_type = -1;
		if (!PugiGetSubNodeValue(dataElem, "boss_type", boss_type) || boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
		{
			return -1;
		}

		CrossBianJingZhiDiGatherCfg::GatherItem &gather_item = m_gather_cfg.gather_list[boss_type];
		gather_item.boss_type = boss_type;

		if (!PugiGetSubNodeValue(dataElem, "gather_id", gather_item.gather_id) || gather_item.gather_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "gather_time", gather_item.gather_time) || gather_item.gather_time < 0)
		{
			return 3;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", gather_item.pos_id_start) || gather_item.pos_id_start < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", gather_item.pos_id_len) || gather_item.pos_id_len <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "disappear_s", gather_item.disappear_s) || gather_item.disappear_s < 0)
		{
			return -6;
		}

		{
			// 加载采集宝箱奖励
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

				if (!gather_item.reward_item[i].ReadConfigNoCheck(item_element))
				{
					return -151 - i;
				}

				i++;

				gather_item.item_count++;
				item_element = item_element.next_sibling();
			}
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int CrossBianJingZhiDiConfig::InitBoosIdByWorldLevel(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while(!dataElem.empty())
	{
		int boss_type = 0;
		if (!PugiGetSubNodeValue(dataElem, "boss_type", boss_type) || boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE)
		{
			return -1;
		}

		if (m_boss_pool[boss_type].count >= CrossBianJingZhiDiBossPool::MAX_BOSS_ID_COUNT) return -1;
		
		int world_level = -1;
		if (!PugiGetSubNodeValue(dataElem, "world_level", world_level) || world_level < 0)
		{
			return -2;
		}

		if (0 == m_boss_pool[boss_type].count && 0 != world_level)
		{
			return -3;
		}
		
		CrossBianJingZhiDiBossPool::BossItem &boss_item = m_boss_pool[boss_type].item_list[m_boss_pool[boss_type].count];
		boss_item.world_level = world_level;

		if (!PugiGetSubNodeValue(dataElem, "boss_id", boss_item.boss_id))
		{
			return -4;
		}
		
		m_boss_pool[boss_type].count ++;

		dataElem = dataElem.next_sibling();
	}
	return 0;
}

int CrossBianJingZhiDiConfig::GetBoosIdByWorldLevel(int world_level, int boss_type)
{
	if (world_level < 0 || world_level > MAX_ROLE_LEVEL) return -1;
	if (boss_type < 0 || boss_type >= CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE) return -1;

	for (int i = m_boss_pool[boss_type].count - 1; i >= 0; i--)
	{
		if (world_level >= m_boss_pool[boss_type].item_list[i].world_level)
		{
			return m_boss_pool[boss_type].item_list[i].boss_id;
		}
	}

	return -1;
}

CrossBianJingZhiDiActivityOpenTimeCfg *CrossBianJingZhiDiConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int CrossBianJingZhiDiConfig::GetOpenActivityConfigList(CrossBianJingZhiDiActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

bool CrossBianJingZhiDiConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

int CrossBianJingZhiDiConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!data_element.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT)
		{
			return -1;
		}

		CrossBianJingZhiDiActivityOpenTimeCfg &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(data_element, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CrossBianJingZhiDiConfig::GetSosCost(int times) const
{
	if (times < 0 || times >= static_cast<int>(m_sos_cost_cfg_vec.size()))
	{
		return -1;
	}

	return m_sos_cost_cfg_vec[times];
}

int CrossBianJingZhiDiConfig::InitSosCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int times = -1;
		if (!PugiGetSubNodeValue(dataElement, "times", times) || times < 0 || times != static_cast<int>(m_sos_cost_cfg_vec.size()))
		{
			return -1;
		}

		int cost = -1;
		if (!PugiGetSubNodeValue(dataElement, "cost", cost) || cost < 0)
		{
			return -2;
		}

		m_sos_cost_cfg_vec.push_back(cost);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
