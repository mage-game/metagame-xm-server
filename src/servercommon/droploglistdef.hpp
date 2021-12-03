#ifndef __DROP_LOG_DEF_HPP__
#define __DROP_LOG_DEF_HPP__

#include "engineadapter.h"
#include "serverdef.h"
#include "servercommon/servercommon.h"
static const int DROP_LOG_NUM_MAX = 50;
static const int DROP_LOG_NUM_FB_MAX = 20;

struct DropLogItem
{
	DropLogItem() : uid(0), monster_id(0), item_id(0), item_num(0), timestamp(0), scene_id(0), color(0), reserve_sh(0)
	{
		memset(role_name, 0, sizeof(role_name));
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	DropLogItem(int log_uid, const GameName log_name, int log_monster_id, ItemID log_item_id, int log_item_num, int log_scene_id, short log_color, unsigned short*log_xianpin_type_list,int log_list_num) :
		uid(log_uid), monster_id(log_monster_id), item_id(log_item_id), item_num(log_item_num), scene_id(log_scene_id), color(log_color), reserve_sh(0)
	{
		F_STRNCPY(role_name, log_name, sizeof(role_name));
		int list_num = log_list_num < ItemNamespace::MAX_XIANPIN_NUM ? log_list_num : ItemNamespace::MAX_XIANPIN_NUM;
		memcpy(xianpin_type_list,log_xianpin_type_list,sizeof(unsigned short)*list_num);
		timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	void Reset()
	{
		uid = 0;
		memset(role_name, 0, sizeof(role_name));
		monster_id = 0;
		item_id = 0;
		item_num = 0;
		timestamp = 0;
		scene_id = 0;
		color = 0;
		reserve_sh = 0;
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	int uid = 0;
	GameName role_name = { 0 };
	int monster_id = 0;
	ItemID item_id = 0;
	short item_num = 0;
	unsigned int timestamp = 0;
	int scene_id = 0;
	short color = 0;
	short reserve_sh = 0;
	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM] = {0};
};

struct DropLogListParam
{
	DropLogListParam() : log_count(0)
	{
	}

	void Reset()
	{
		for (auto &item : item_list)
		{
			item.Reset();
		}
	}

	int log_count;
	DropLogItem item_list[DROP_LOG_NUM_MAX];
};

struct DropLogFbParam
{
	DropLogFbParam() : log_count(0)
	{
	}
	void Reset()
	{
		memset(item_list,0,sizeof(item_list));
	}
	int log_count;
	DropLogItem item_list[DROP_LOG_NUM_FB_MAX];
};

#endif