#ifndef __NPCCONFIG_H__
#define __NPCCONFIG_H__

#include <vector>
#include <string>

#include "gamedef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct ConfigLevelSceneEntry
{
	ConfigLevelSceneEntry() : min_level(0), max_level(0), scene_id(0), scene_pos(0, 0) {}

	int min_level;
	int max_level;
	int scene_id;
	Posi scene_pos;
};

typedef std::vector< ConfigLevelSceneEntry > ConfigLevelSceneEntryList;
typedef ConfigLevelSceneEntryList::iterator ConfigLevelSceneEntryListIt;

struct ConfigSceneNpcFuntion_FB
{
	ConfigSceneNpcFuntion_FB() : fb_type(0), team_member_num(0), level(0), day_count_id(-1), require_task_id(INVALID_TASK_ID), scene_id(0), scene_x(0), scene_y(0) {}

	static const int LEVEL_SCENE_ENTRY_MAX = 16;

	int fb_type;
	unsigned int team_member_num;
	Attribute level;
	int day_count_id;
	TaskID require_task_id;
	int scene_id;
	int scene_x;
	int scene_y;
	ConfigLevelSceneEntryList level_entry_list;
};

struct ConfigSceneNpcFuntion_TransportPoint_Info
{
	ConfigSceneNpcFuntion_TransportPoint_Info() : scene_id(0), scene_x(0), scene_y(0) {}

	unsigned int scene_id;
	unsigned int scene_x;
	unsigned int scene_y;
};
typedef std::vector<ConfigSceneNpcFuntion_TransportPoint_Info> ConfigSceneNpcFuntion_TransportPoint_InfoList;

struct ConfigSceneNpcFuntion_TransportPoint_Condition
{
	ConfigSceneNpcFuntion_TransportPoint_Condition() : level(0), max_level(0), to_last_scene(false), red_name_forbid(0) {}

	unsigned int level;
	unsigned int max_level;
	bool to_last_scene;
	bool red_name_forbid;
};

struct ConfigSceneNpcFuntion_Random_TransportPoint
{
	// ConfigSceneNpcFuntion_TransportPoint_Condition trans_point_condition;
	ConfigSceneNpcFuntion_TransportPoint_InfoList trans_point_list;
};

struct ConfigSceneNpcFuntion_TransportPoint
{
	ConfigSceneNpcFuntion_TransportPoint_Condition trans_point_condition;
	ConfigSceneNpcFuntion_TransportPoint_Info trans_point_info;
};

struct ConfigSceneNpcFuntion_FB_NextTower
{
	ConfigSceneNpcFuntion_FB_NextTower():exit_sign(0), coin_bind(0), exp(0), /*vitality(0), */trans_condition(0), monster_num(0){}
	unsigned int exit_sign;
	long long coin_bind;
	int exp;
	int trans_limit; 
	int trans_condition;
	int monster_num;
	ConfigSceneNpcFuntion_TransportPoint_Info trans_point_info;
	// 满足下一级传送条件 ：0 判断剩余怪物数量
	//						1 判断boss是否死亡
	//						2 收集任务是否完成
	// 这只是策划目前提到的可能条件，暂时实现0的方式，以后扩展
};

struct ConfigSceneNpcFuntion_Shop_Item
{
	ConfigSceneNpcFuntion_Shop_Item()
		:item_id(0),is_bind(0),price(0),level_limit(0){}
	unsigned short item_id;
	short is_bind;
	unsigned int price;
	short level_limit;
};
typedef std::vector<ConfigSceneNpcFuntion_Shop_Item> ConfigSceneNpcFuntion_Shop_ItemList;

struct ConfigSceneNpcFuntion_Shop
{
	ConfigSceneNpcFuntion_Shop(){}
	ConfigSceneNpcFuntion_Shop_ItemList shopitem;
};

struct ConfigSceneNpcFuntion
{
	ConfigSceneNpcFuntion():npc_type(0), post_id(0) {}

	int npc_type;
	int post_id;
	ConfigSceneNpcFuntion_Shop shop;
	ConfigSceneNpcFuntion_FB fb;
	ConfigSceneNpcFuntion_TransportPoint transportpoint;
	ConfigSceneNpcFuntion_Random_TransportPoint random_trans_points;
	ConfigSceneNpcFuntion_FB_NextTower next_tower;
};
typedef std::vector<ConfigSceneNpcFuntion> ConfigSceneNpcFuntionList;

struct ConfigSceneNpc
{
	ConfigSceneNpc():x(0), y(0), id(0), default_dialog_id(0) {}

	int x;
	int y;
	unsigned short id;
	int default_dialog_id;
	ConfigSceneNpcFuntionList functions;
	bool Load(const std::string &configname, std::string &err);
};
typedef std::vector<ConfigSceneNpc> ConfigSceneNpcList;

#endif

