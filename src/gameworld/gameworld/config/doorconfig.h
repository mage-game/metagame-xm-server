#ifndef __DOORCONFIG_H__
#define __DOORCONFIG_H__

#include <vector>
#include <string>

#include "gamedef.h"

struct ConfigSceneDoor
{
	ConfigSceneDoor() : id(0), pos(0, 0), target_scene_id(0), target_door_id(0) {}

	int id;
	Posi pos;
	int target_scene_id;
	int target_door_id;
	bool Load(const std::string &configname, std::string &err);
};
typedef std::vector<ConfigSceneDoor> ConfigSceneDoorList;

#endif


