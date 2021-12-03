#ifndef __MONSTERREPLACE_CONFIG_HPP__
#define __MONSTERREPLACE_CONFIG_HPP__

#include <map>
#include <string.h>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

enum REMONSTER_ENV
{
	REMONSTER_ENV_PERSONAL_TOWERDEFEND = 0,  // µ¥ÈËËþ·À
	REMONSTER_ENV_LOVERS_FB = 1,

	REMONSTER_ENV_MAX,
};

struct ReMonsterWorldLevelCfg
{
	ReMonsterWorldLevelCfg() : monster_id(0), world_min_level(0), world_max_level(0), remonster_id(0) {}

	int monster_id;
	int world_min_level;
	int world_max_level;
	int remonster_id;
};

struct ReMonsterRoleLevelCfg
{
	ReMonsterRoleLevelCfg() : monster_id(0), role_min_level(0), role_max_level(0), remonster_id(0) {}
	
	int monster_id;
	int role_min_level;
	int role_max_level;
	int remonster_id;
};

struct ReMonsterCrossWorldLevelCfg
{
	ReMonsterCrossWorldLevelCfg() : monster_id(0), cross_world_min_level(0), cross_world_max_level(0), remonster_id(0) {}

	int monster_id;
	int cross_world_min_level;
	int cross_world_max_level;
	int remonster_id;
};

class MonsterReplaceConfig : public ConfigBase
{
public:
	MonsterReplaceConfig();
	~MonsterReplaceConfig();

	bool Init(const std::string &path, std::string *err);

	int GetReMonsterIdByWorldLevel(int monsterid);
	int GetOrginalWorldLevMonsterId(int remonsterid);
	int GetReMonsterIdByCrossBossLevel(int monsterid, int boss_level = 0);

	int GetReMonsterIdByRoleLevel(int remonster_env, int role_level, int monsterid);
	int GetOriginalRoleLevMonsterId(int remonster_env, int remonsterid);

private:
	int InitReplaceWorldLevelConfig(PugiXmlNode RootElement);
	int InitReplaceRoleLevelConfig(PugiXmlNode RootElement);
	int InitReplaceCrossWorldLevelConfig(PugiXmlNode RootElement);

	typedef std::vector<ReMonsterWorldLevelCfg> MonsterWorldLevelVec;
	typedef std::map<int, MonsterWorldLevelVec> MonsterWorldLevelMap;

	typedef std::vector<ReMonsterRoleLevelCfg> MonsterRoleLevelVec;
	typedef std::map<int, MonsterRoleLevelVec> MonsterRoleLevelMap;
	typedef std::map<int, MonsterRoleLevelMap> MonsterRoleLevelEnvMap;

	typedef std::vector<ReMonsterCrossWorldLevelCfg> MonsterCrossWorldLevelVec;
	typedef std::map<int, MonsterCrossWorldLevelVec> MonsterCrossWorldLevelMap;

	MonsterWorldLevelMap m_monster_worldlevel_map;
	MonsterRoleLevelEnvMap m_monster_rolelevel_map;
	MonsterCrossWorldLevelMap m_monster_cross_worldlevel_map;
};

#endif