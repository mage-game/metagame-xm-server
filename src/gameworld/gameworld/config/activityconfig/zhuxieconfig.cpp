#include "zhuxieconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/worldshadow/worldshadow.hpp"

ZhuXieConfig::ZhuXieConfig()
	: m_zhuxie_sceneid(0), m_kill_role_get_shengwang(0), m_kill_role_get_max_shengwang(0),
	m_zhuexie_realive_pos_count(0)
{
	memset(m_zhuxie_realive_pos_list, 0, sizeof(m_zhuxie_realive_pos_list));
}

ZhuXieConfig::~ZhuXieConfig()
{

}

bool ZhuXieConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ZhuXieConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load ZhuXieConfig config [%s] Error.cannot find root node.", path.c_str());
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

	{
		PugiXmlNode RealivePointListElem = RootElement.child("realive_point_list");
		if (RealivePointListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->realive_point_list]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitRealivePoint(RealivePointListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitRealivePoint fail %d ", path.c_str(), iRet);
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
		PugiXmlNode BossCfgListElem = RootElement.child("boss");
		if (BossCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->boss]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitBoss(BossCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitBoss fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode MonsterCfgListElem = RootElement.child("monster");
		if (MonsterCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->monster]", path.c_str());
			*err =  errinfo;
			return false;
		}

		this->InitMonster(MonsterCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitMonster fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	{
		PugiXmlNode GatherCfgListElem = RootElement.child("gather");
		if (GatherCfgListElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->gather]", path.c_str());
			*err =  errinfo;
			return false;
		}

		this->InitGather(GatherCfgListElem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitGather fail %d ", path.c_str(), iRet);
			*err =  errinfo;
			return false;
		}
	}

	return true;
}

int ZhuXieConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "scene_id", m_zhuxie_sceneid) || m_zhuxie_sceneid <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_zhuxie_sceneid);		

	if (!PugiGetSubNodeValue(dataElem, "kill_role_get_shengwang", m_kill_role_get_shengwang) || m_kill_role_get_shengwang < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElem, "kill_role_get_max_shengwang", m_kill_role_get_max_shengwang) || m_kill_role_get_max_shengwang < 0 || m_kill_role_get_max_shengwang <= m_kill_role_get_shengwang)
	{
		return -3;
	}

	return 0;
}

int ZhuXieConfig::InitTaskList(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_world_min_level = 0;
	int last_world_max_level = 0;
	int task_count = 0;

	while (!dataElem.empty())
	{
		int task_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "task_id", task_id) || task_id <= 0)
		{
			return -1;
		}

		ZhuXieTaskInfo &task_info = m_zhuxie_task_map[task_id];
		task_info.task_id = task_id;

		if (!PugiGetSubNodeValue(dataElem, "world_min_level", task_info.task_world_min_level) || task_info.task_world_min_level < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "world_max_level", task_info.task_world_max_level) || task_info.task_world_max_level < 0 || task_info.task_world_max_level < task_info.task_world_min_level)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "task_type", task_info.task_type) || task_info.task_type <= ZHUXIE_TASK_TYPE_INVALID || 
			task_info.task_type >= ZHUXIE_TASK_TYPE_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_id", task_info.param_id) || task_info.param_id < 0)
		{

			return -5;
		}

		if ((ZHUXIE_TASK_TYPE_KILL_BOSS == task_info.task_type || ZHUXIE_TASK_TYPE_KILL_MONSTER == task_info.task_type) && 
			(task_info.param_id != 0 && !MONSTERPOOL->IsMonsterExist(task_info.param_id)))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "param_max_value", task_info.max_value) || task_info.max_value <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElem, "shengwang", task_info.shengwang) || task_info.shengwang <= 0)
		{
			return -8;
		}

		PugiXmlNode Itemelement = dataElem.child("reward_item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!task_info.reward_item1.ReadConfig(Itemelement))
			{
				return -20;
			}
		}

		Itemelement = dataElem.child("reward_item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!task_info.reward_item2.ReadConfig(Itemelement))
			{
				return -21;
			}
		}

		Itemelement = dataElem.child("reward_item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!task_info.reward_item3.ReadConfig(Itemelement))
			{
				return -22;
			}
		}

		// ¼ì²é
		{
			if (task_info.task_world_min_level != last_world_min_level)
			{
				if (task_count != ZHUXIE_TASK_MAX)
				{
					return -100;
				}
				
				if (task_info.task_world_min_level - last_world_max_level != 1)
				{
					return -101;
				}
				
				task_count = 0;
				last_world_min_level = task_info.task_world_min_level;
			}

			++ task_count;

			last_world_max_level = task_info.task_world_max_level;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ZhuXieConfig::InitRealivePoint(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!dataElem.empty())
	{
		if (count >= ZHUXIE_REALIVE_POS_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_x", m_zhuxie_realive_pos_list[count].x) || m_zhuxie_realive_pos_list[count].x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_y", m_zhuxie_realive_pos_list[count].y) || m_zhuxie_realive_pos_list[count].y <= 0)
		{
			return -3;
		}

		++ count;
		dataElem = dataElem.next_sibling();
	}

	m_zhuexie_realive_pos_count = count;

	return 0;
}

int ZhuXieConfig::InitFlushPoint(PugiXmlNode RootElement)
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

int ZhuXieConfig::InitBoss(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int boss_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "boss_index", boss_index) || boss_index < 0 || boss_index >= ZHUXIE_BOSS_NUM)
		{
			return -1;
		}

		ZhuXieBossCfg &boss_cfg = m_zhuxie_boss_cfg_list[boss_index];

		if (boss_cfg.replace_boss_count >= ZHUXIE_MAX_RELACE_BOSS_NUM)
		{
			return -2;
		}

		ZhuXieReplaceBossCfg &replace_boss_cfg = boss_cfg.replace_boss_list[boss_cfg.replace_boss_count];

		if (!PugiGetSubNodeValue(dataElem, "boss_id", replace_boss_cfg.replace_boss_id) || !MONSTERPOOL->IsMonsterExist(replace_boss_cfg.replace_boss_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "limitLev", replace_boss_cfg.replace_need_world_level) || replace_boss_cfg.replace_need_world_level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "flush_time_on_die", replace_boss_cfg.flush_time_on_die) || replace_boss_cfg.flush_time_on_die <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", replace_boss_cfg.pos_id_start) || replace_boss_cfg.pos_id_start < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", replace_boss_cfg.pos_id_len) || replace_boss_cfg.pos_id_len <= 0)
		{
			return -7;
		}

		++ boss_cfg.replace_boss_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ZhuXieConfig::InitMonster(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int monster_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "monster_index", monster_index) || monster_index < 0 || monster_index >= ZHUXIE_MONSTER_NUM)
		{
			return -1;
		}

		ZhuXieMonsterCfg &monster_cfg = m_zhuxie_monster_cfg_list[monster_index];

		if (monster_cfg.replace_monster_count >= ZHUXIE_MAX_RELACE_MONSTER_NUM)
		{
			return -2;
		}

		ZhuXieReplaceMonsterCfg &replace_monster_cfg = monster_cfg.replace_monster_list[monster_cfg.replace_monster_count];

		if (!PugiGetSubNodeValue(dataElem, "monster_id", replace_monster_cfg.replace_monster_id) || !MONSTERPOOL->IsMonsterExist(replace_monster_cfg.replace_monster_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "limitLev", replace_monster_cfg.replace_need_world_level) || replace_monster_cfg.replace_need_world_level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "count", replace_monster_cfg.count) || replace_monster_cfg.count <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", replace_monster_cfg.pos_id_start) || replace_monster_cfg.pos_id_start < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", replace_monster_cfg.pos_id_len) || replace_monster_cfg.pos_id_len <= 0)
		{
			return -7;
		}

		//½±Àø
		{
			if (m_monster_reward_map.end() != m_monster_reward_map.find(replace_monster_cfg.replace_monster_id))
			{
				return -8;
			}
			ZhuXieReward &reward = m_monster_reward_map[replace_monster_cfg.replace_monster_id];

			if (!PugiGetSubNodeValue(dataElem, "shengwang", reward.shengwang) || reward.shengwang <= 0)
			{
				return -8;
			}

			PugiXmlNode Itemelement = dataElem.child("reward_item");
			ItemID itemid = 0;
			if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
			{
				if (!reward.item.ReadConfig(Itemelement))
				{
					return -20;
				}
			}
		}

		++ monster_cfg.replace_monster_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ZhuXieConfig::InitGather(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int gather_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "gather_index", gather_index) || gather_index < 0 || gather_index >= ZHUXIE_GATHER_NUM)
		{
			return -1;
		}

		ZhuXieGatherCfg &gather_cfg = m_zhuxie_gather_cfg_list[gather_index];

		if (gather_cfg.replace_gather_count >= ZHUXIE_MAX_RELACE_GATHER_NUM)
		{
			return -2;
		}

		ZhuXieReplaceGatherCfg &replace_gather_cfg = gather_cfg.replace_gather_list[gather_cfg.replace_gather_count];

		if (!PugiGetSubNodeValue(dataElem, "gather_id", replace_gather_cfg.replace_gather_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "limitLev", replace_gather_cfg.replace_need_world_level) || replace_gather_cfg.replace_need_world_level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "gather_time", replace_gather_cfg.gather_time) || replace_gather_cfg.gather_time <= 0)
		{
			return -44;
		}

		if (!PugiGetSubNodeValue(dataElem, "count", replace_gather_cfg.count) || replace_gather_cfg.count <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_start", replace_gather_cfg.pos_id_start) || replace_gather_cfg.pos_id_start < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "pos_id_len", replace_gather_cfg.pos_id_len) || replace_gather_cfg.pos_id_len <= 0)
		{
			return -7;
		}
		
		//½±Àø
		{
			if (m_gather_reward_map.end() != m_gather_reward_map.find(replace_gather_cfg.replace_gather_id))
			{
				return -8;
			}
			ZhuXieReward &reward = m_gather_reward_map[replace_gather_cfg.replace_gather_id];
			
			if (!PugiGetSubNodeValue(dataElem, "shengwang", reward.shengwang) || reward.shengwang <= 0)
			{
				return -8;
			}

			PugiXmlNode Itemelement = dataElem.child("reward_item");
			ItemID itemid = 0;
			if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
			{
				if (!reward.item.ReadConfig(Itemelement))
				{
					return -20;
				}
			}
		}

		++ gather_cfg.replace_gather_count;
		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ZhuXieConfig::GetZhuXieFBEnterInfo(Posi *enterpos)
{
	if (0 < m_zhuexie_realive_pos_count && m_zhuexie_realive_pos_count <= ZHUXIE_REALIVE_POS_MAX)
	{
		if (NULL != enterpos) *enterpos = m_zhuxie_realive_pos_list[RandomNum(m_zhuexie_realive_pos_count)];
	}

	return m_zhuxie_sceneid;
}

void ZhuXieConfig::GetZhuXieTaskList(ZhuXieTaskInfo zhuxie_task_list[ZHUXIE_TASK_MAX], int world_level)
{
	int count = 0;
	std::map<int, ZhuXieTaskInfo>::iterator iter;
	for (iter = m_zhuxie_task_map.begin(); m_zhuxie_task_map.end() != iter; ++ iter)
	{
		if (count >= ZHUXIE_TASK_MAX)
		{
			return;
		}

		ZhuXieTaskInfo &task_cfg = iter->second;
		if (world_level >= task_cfg.task_world_min_level && world_level <= task_cfg.task_world_max_level)
		{
			zhuxie_task_list[count] = task_cfg;
			++ count;
		}
	}
}

const ZhuXieTaskInfo *ZhuXieConfig::GetZhuXieTaskCfg(int task_id)
{
	std::map<int, ZhuXieTaskInfo>::const_iterator iter = m_zhuxie_task_map.find(task_id);
	if (m_zhuxie_task_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const ZhuXieReplaceBossCfg *ZhuXieConfig::GetReplaceBossCfg(int boss_index, int world_level)
{
	if (boss_index < 0 || boss_index >= ZHUXIE_BOSS_NUM || world_level < 0)
	{
		return NULL;
	}

	ZhuXieBossCfg *boss_cfg = &m_zhuxie_boss_cfg_list[boss_index];
	for (int i = boss_cfg->replace_boss_count - 1; i >= 0 && i < ZHUXIE_MAX_RELACE_BOSS_NUM; i --)
	{
		ZhuXieReplaceBossCfg *replace_boss_cfg = &boss_cfg->replace_boss_list[i];
		if (replace_boss_cfg->replace_need_world_level <= world_level)
		{
			return replace_boss_cfg;
		}
	}

	return NULL;
}

const ZhuXieReplaceBossCfg *ZhuXieConfig::GetZhuXieReplacBossCfgById(int boss_id)
{
	for (int i = 0; i < ZHUXIE_BOSS_NUM; ++ i)
	{
		ZhuXieBossCfg *boss_cfg = &m_zhuxie_boss_cfg_list[i];
		for (int k = 0; k < ZHUXIE_MAX_RELACE_BOSS_NUM; ++ k)
		{
			ZhuXieReplaceBossCfg *replace_boss_cfg = &boss_cfg->replace_boss_list[k];
			if (replace_boss_cfg->replace_boss_id == boss_id)
			{
				return replace_boss_cfg;
			}
		}
	}

	return NULL;
}

const ZhuXieReplaceMonsterCfg *ZhuXieConfig::GetReplaceMonsterCfg(int monster_index, int world_level)
{
	if (monster_index < 0 || monster_index >= ZHUXIE_MONSTER_NUM || world_level < 0)
	{
		return NULL;
	}

	ZhuXieMonsterCfg *monster_cfg = &m_zhuxie_monster_cfg_list[monster_index];
	for (int i = monster_cfg->replace_monster_count - 1; i >= 0 && i < ZHUXIE_MAX_RELACE_MONSTER_NUM; -- i)
	{
		ZhuXieReplaceMonsterCfg *replace_monster_cfg = &monster_cfg->replace_monster_list[i];
		if (replace_monster_cfg->replace_need_world_level <= world_level)
		{
			return replace_monster_cfg;
		}
	}

	return NULL;
}

const ZhuXieReplaceGatherCfg *ZhuXieConfig::GetReplaceGatherCfg(int gather_index, int world_level)
{
	if (gather_index < 0 || gather_index >= ZHUXIE_GATHER_NUM || world_level < 0)
	{
		return NULL;
	}

	ZhuXieGatherCfg *gather_cfg = &m_zhuxie_gather_cfg_list[gather_index];
	for (int i = gather_cfg->replace_gather_count - 1; i >= 0 && i < ZHUXIE_MAX_RELACE_GATHER_NUM; -- i)
	{
		ZhuXieReplaceGatherCfg *replace_gather_cfg = &gather_cfg->replace_gather_list[i];
		if (replace_gather_cfg->replace_need_world_level <= world_level)
		{
			return replace_gather_cfg;
		}
	}

	return NULL;
}

bool ZhuXieConfig::GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len)
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

const ZhuXieReward *ZhuXieConfig::GetKillMonsterReward(int monster_id)
{
	std::map<int, ZhuXieReward>::const_iterator iter = m_monster_reward_map.find(monster_id);

	if (m_monster_reward_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const ZhuXieReward *ZhuXieConfig::GetGatherReward(int gather_id)
{
	std::map<int, ZhuXieReward>::const_iterator iter = m_gather_reward_map.find(gather_id);

	if (m_gather_reward_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}