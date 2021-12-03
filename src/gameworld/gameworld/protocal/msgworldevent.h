#ifndef MSGWORLDEVENT_H
#define MSGWORLDEVENT_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"
#include "gamedef.h"
#include "servercommon/fishpooldef.hpp"
#include "servercommon/guilddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		世界事件
	*/

	class SCWorldEventCommonInfo									// 世界时间通用信息
	{
	public:
		SCWorldEventCommonInfo();
		MessageHeader header;

		short curr_event_type;										// 事件类型
		short logic_state;											// 逻辑状态
		unsigned int next_logic_state_timestamp;					// 下个逻辑状态时间戳
		int event_score;											// 事件积分
	};

	class CSWorldEventQueryCommonInfo								// 世界事件通用信息请求
	{
	public:
		CSWorldEventQueryCommonInfo();
		MessageHeader header;					
	};

	class SCWorldEventRoleInfo										// 世界事件通用信息
	{
	public:
		SCWorldEventRoleInfo();
		MessageHeader header;

		char light_word_flag;
		char curr_find_npc_id;
		short reserve_sh;
		int fetch_reward_flag;
		int find_npc_flag;
		int event_score_list[WORLD_EVENT_TYPE_MAX];
	};

	class CSWorldEventGetNpcLocateInfo
	{
	public:
		CSWorldEventGetNpcLocateInfo();
		MessageHeader header;

		int npc_id;
	};

	class SCWorldEventNpcLocateInfo
	{
	public:
		SCWorldEventNpcLocateInfo();
		MessageHeader header;

		int find_npc_id;

		int locate_scene_id_count;
		int locate_scend_idlist[WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE];
	};

	class CSWorldEventQueryRoleInfo
	{
	public:
		CSWorldEventQueryRoleInfo();
		MessageHeader header;
	};

	class CSWorldEventFetchReward
	{
	public:
		CSWorldEventFetchReward();
		MessageHeader header;
	};

	class CSWorldEventObjTouch
	{
	public:
		CSWorldEventObjTouch();
		MessageHeader		header;

		ObjID obj_id;
		short reserve_sh;
	};

	class CSWorldEventLightWord
	{
	public:
		CSWorldEventLightWord();
		MessageHeader		header;
	};

	class SCWorldEventKillFindNpcMonster
	{
	public:
		SCWorldEventKillFindNpcMonster();
		MessageHeader		header;
	};
}

#pragma pack(pop)

#endif


