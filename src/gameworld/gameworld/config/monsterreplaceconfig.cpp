#include "monsterreplaceconfig.hpp"
#include "item/itempool.h"
#include <set>
#include "item/itembase.h"
#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
MonsterReplaceConfig::MonsterReplaceConfig()
{

}

MonsterReplaceConfig::~MonsterReplaceConfig()
{

}

bool MonsterReplaceConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MonsterReplaceConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("replace_monster");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitReplaceWorldLevelConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitReplaceConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("re_monster_by_role_level");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitReplaceRoleLevelConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitReplaceRoleLevelConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("re_cross_world_monster");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitReplaceCrossWorldLevelConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitReplaceCrossWorldLevelConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int MonsterReplaceConfig::InitReplaceWorldLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_monster_id = 0;
	int last_world_max_level = 0;

	while (!dataElement.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}
		
		ReMonsterWorldLevelCfg item_cfg;
		item_cfg.monster_id = monster_id;

		if (!PugiGetSubNodeValue(dataElement, "world_min_level", item_cfg.world_min_level) || item_cfg.world_min_level < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "world_max_level", item_cfg.world_max_level) || item_cfg.world_max_level < 0 || item_cfg.world_max_level < item_cfg.world_min_level)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "remonster_id", item_cfg.remonster_id) || !MONSTERPOOL->IsMonsterExist(item_cfg.remonster_id))
		{
			return -4;
		}
		
		MonsterWorldLevelVec &monster_vec = m_monster_worldlevel_map[monster_id];
		monster_vec.push_back(item_cfg);

		if (last_monster_id == monster_id)
		{
			if (1 != item_cfg.world_min_level - last_world_max_level)
			{
				return -100;
			}
		}

		last_monster_id = monster_id;
		last_world_max_level = item_cfg.world_max_level;

		dataElement = dataElement.next_sibling();

	}

	return 0;
}

int MonsterReplaceConfig::InitReplaceRoleLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_role_max_level = 0;
	int last_replace_env = 0;

	while (!dataElement.empty())
	{
		ReMonsterRoleLevelCfg item_cfg;
		int replace_env = 0;

		if (!PugiGetSubNodeValue(dataElement, "replace_env", replace_env) || replace_env < 0 || replace_env >= REMONSTER_ENV_MAX)
		{
			return -1;
		}

		if (last_replace_env != replace_env)
		{
			last_replace_env = 0;
			last_role_max_level = 0;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", item_cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(item_cfg.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_min_level", item_cfg.role_min_level) || item_cfg.role_min_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_max_level", item_cfg.role_max_level) || item_cfg.role_max_level <= item_cfg.role_min_level)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "remonster_id", item_cfg.remonster_id) || !MONSTERPOOL->IsMonsterExist(item_cfg.remonster_id))
		{
			return -5;
		}

		MonsterRoleLevelMap &monster_map = m_monster_rolelevel_map[replace_env];
		MonsterRoleLevelVec &vec = monster_map[item_cfg.monster_id];
		vec.push_back(item_cfg);

		if (last_replace_env == replace_env && last_role_max_level != 0 && last_role_max_level != item_cfg.role_max_level)
		{
			if (1 != item_cfg.role_min_level - last_role_max_level)
			{ 
				return -100;
			}
		}

		last_replace_env = replace_env;
		last_role_max_level = item_cfg.role_max_level;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MonsterReplaceConfig::InitReplaceCrossWorldLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_monster_id = 0;
	int last_cross_world_max_level = 0;

	while (!dataElement.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}

		ReMonsterCrossWorldLevelCfg item_cfg;
		item_cfg.monster_id = monster_id;

		if (!PugiGetSubNodeValue(dataElement, "cross_world_min_level", item_cfg.cross_world_min_level) || item_cfg.cross_world_min_level < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "cross_world_max_level", item_cfg.cross_world_max_level) || item_cfg.cross_world_max_level < 0 || item_cfg.cross_world_max_level < item_cfg.cross_world_min_level)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "remonster_id", item_cfg.remonster_id) || !MONSTERPOOL->IsMonsterExist(item_cfg.remonster_id))
		{
			return -4;
		}

		MonsterCrossWorldLevelVec &monster_vec = m_monster_cross_worldlevel_map[monster_id];
		monster_vec.push_back(item_cfg);

		if (last_monster_id == monster_id)
		{
			if (1 != item_cfg.cross_world_min_level - last_cross_world_max_level)
			{
				return -100;
			}
		}

		last_monster_id = monster_id;
		last_cross_world_max_level = item_cfg.cross_world_max_level;

		dataElement = dataElement.next_sibling();

	}

	return 0;
}

int MonsterReplaceConfig::GetReMonsterIdByWorldLevel(int monsterid)
{
	MonsterWorldLevelMap::iterator iter = m_monster_worldlevel_map.find(monsterid);
	if (m_monster_worldlevel_map.end() == iter)
	{
		return monsterid;
	}

	MonsterWorldLevelVec &monster_vec = iter->second;
	int world_level = WorldShadow::Instance().GetWorldLevel();
	int size = static_cast<int>(monster_vec.size());

	for (int i = 0; i < size; ++ i)
	{
		if (world_level >= monster_vec[i].world_min_level && world_level <= monster_vec[i].world_max_level)
		{
			return monster_vec[i].remonster_id;
		}
	}

	return monsterid;
}

int MonsterReplaceConfig::GetOrginalWorldLevMonsterId(int remonsterid)
{
	for (MonsterWorldLevelMap::const_iterator iter = m_monster_worldlevel_map.begin(); m_monster_worldlevel_map.end() != iter; ++ iter)
	{
		const MonsterWorldLevelVec &vec = iter->second;
		for (MonsterWorldLevelVec::const_iterator vec_iter = vec.begin(); vec.end() != vec_iter; ++ vec_iter)
		{
			if (vec_iter->remonster_id == remonsterid)
			{
				return vec_iter->monster_id;
			}
		}
	}

	return remonsterid;
}

int MonsterReplaceConfig::GetReMonsterIdByCrossBossLevel(int monsterid, int boss_level)
{
	MonsterCrossWorldLevelMap::iterator iter = m_monster_cross_worldlevel_map.find(monsterid);
	if (m_monster_cross_worldlevel_map.end() == iter)
	{
		return monsterid;
	}

	MonsterCrossWorldLevelVec &monster_vec = iter->second;
	int size = static_cast<int>(monster_vec.size());

	int world_level = boss_level;
	if (world_level <= 0)
	{
		world_level = WorldShadow::Instance().GetWorldLevel();
		if (CrossConfig::Instance().IsHiddenServer())
		{
			world_level = WorldShadow::Instance().GetHiddenWorldLevel();
		}
	}

	for (int i = 0; i < size; ++i)
	{
		if (world_level >= monster_vec[i].cross_world_min_level && world_level <= monster_vec[i].cross_world_max_level)
		{
			return monster_vec[i].remonster_id;
		}
	}

	return monsterid;
}

int MonsterReplaceConfig::GetReMonsterIdByRoleLevel(int remonster_env, int role_level, int monsterid)
{
	MonsterRoleLevelEnvMap::iterator env_iter = m_monster_rolelevel_map.find(remonster_env);
	if (m_monster_rolelevel_map.end() == env_iter)
	{
		return monsterid;
	}
	
	MonsterRoleLevelMap &monster_map = env_iter->second;
 	MonsterRoleLevelMap::iterator monster_iter = monster_map.find(monsterid);
	if (monster_map.end() == monster_iter)
	{
		return monsterid;
	}

	MonsterRoleLevelVec &vec = monster_iter->second;
	for (MonsterRoleLevelVec::const_iterator iter = vec.begin(); vec.end() != iter; ++ iter)
	{
		if (role_level >= iter->role_min_level && role_level <= iter->role_max_level)
		{
			return iter->remonster_id;
		}
	}

	return monsterid;
}

int MonsterReplaceConfig::GetOriginalRoleLevMonsterId(int remonster_env, int remonsterid)
{
	MonsterRoleLevelEnvMap::iterator env_iter = m_monster_rolelevel_map.find(remonster_env);
	if (m_monster_rolelevel_map.end() == env_iter)
	{
		return remonsterid;
	}

	MonsterRoleLevelMap &monster_map = env_iter->second;

	for (MonsterRoleLevelMap::const_iterator m_iter = monster_map.begin(); monster_map.end() != m_iter; ++ m_iter)
	{
		const MonsterRoleLevelVec &vec = m_iter->second;
		for (MonsterRoleLevelVec::const_iterator v_iter = vec.begin(); vec.end() != v_iter; ++ v_iter)
		{
			if (v_iter->remonster_id == remonsterid)
			{
				return v_iter->monster_id;
			}
		}
	}

	return remonsterid;
}