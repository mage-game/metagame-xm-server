#ifndef __SCENECONFIG_H__
#define __SCENECONFIG_H__

#include <vector>
#include <string>
#include "gamedef.h"
#include "npcconfig.h"
#include "doorconfig.h"

#include "servercommon/triggerdef.h"

enum MAPTYPE
{
	COMMON = 0,	// 一般地图
	FB,			// 副本
};


struct ConfigSceneTriggerArea
{
	ConfigSceneTriggerArea():x(0),y(0),range(0) {}
	int x;
	int y;
	int range;
};
typedef std::vector<ConfigSceneTriggerArea> ConfigSceneTriggerAreaList;

struct ConfigSceneTownPoint
{
	ConfigSceneTownPoint():sceneid(0),scenex(0),sceney(0){}
	int sceneid;
	int scenex;
	int sceney;
};

struct ConfigSceneMonsterPoint
{
	ConfigSceneMonsterPoint():x(0),y(0),interval(0),num(0), histroytotalnum(0),monsterid(0) {}
	int x;
	int y;
	int interval;
	int num;
	int histroytotalnum;
	int monsterid;
};
typedef std::vector<ConfigSceneMonsterPoint> ConfigSceneMonsterPointList;


struct ConfigSceneGather
{
	ConfigSceneGather():id(0),x(0),y(0),create_interval(0),gather_time(0), evil_add(0), disappear_after_gather(true), gather_index(0) {};
	UInt16 id;
	int x;
	int y;
	unsigned int create_interval;
	unsigned int gather_time;
	int evil_add;
	bool disappear_after_gather;
	int gather_index;
};
typedef std::vector<ConfigSceneGather> ConfigSceneGatherList;

struct ConfigSceneTrigger
{
	ConfigSceneTrigger() : id(0), x(0), y(0), range(0), trigger_cond_type(0), trigger_action_type(0), trigger_max_count(0)
	{
		memset(trigger_cond_param_list, 0, sizeof(trigger_cond_param_list));
		memset(trigger_action_param_list, 0, sizeof(trigger_action_param_list));
	}

	UInt16 id;
	int x;
	int y;
	int range;
	int trigger_cond_type;
	int trigger_cond_param_list[TRIGGER_COND_PARAM_MAX];
	int trigger_action_type;
	int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX];
	int trigger_max_count;
};
typedef std::vector<ConfigSceneTrigger> ConfigSceneTriggerList;

struct ConfigScene
{
	ConfigScene():id(0), mapid(0), levellimit(0), istax(false), speciallogic_type(0), scene_timeout(0), is_timeout_kick_user(false), is_forbid_pk(false){}
	int id;
	std::string name;
	int mapid;
	int levellimit;
	bool istax;
	int speciallogic_type;
	unsigned int scene_timeout;
	bool is_timeout_kick_user;
	bool is_forbid_pk;
	ConfigSceneNpcList npcs;
	ConfigSceneDoorList doors;
	ConfigSceneTriggerAreaList triggerareas;
	ConfigSceneTownPoint townpoint;
	ConfigSceneMonsterPointList monsterpoints;
	ConfigSceneGatherList gathers;
	ConfigSceneTriggerList triggers;

	std::string lasterr;
	bool Load(const std::string &dir,const std::string &filename);
	const std::string& GetError(){ return lasterr; }
};

#endif


