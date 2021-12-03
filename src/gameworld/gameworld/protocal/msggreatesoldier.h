// 系统协议
#ifndef __MSG_GREATE_SOLDIER_HPP__
#define __MSG_GREATE_SOLDIER_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/greatesoldierdef.hpp"

#pragma pack(push, 4)

namespace Protocol
{
	enum GREATE_SOLDIER_REQ_TYPE
	{
		GREATE_SOLDIER_REQ_TYPE_INFO = 0,					// 请求所有信息
		GREATE_SOLDIER_REQ_TYPE_LEVEL_UP = 1,				// 升级请求，param1是seq
		GREATE_SOLDIER_REQ_TYPE_BIANSHEN = 2,				// 变身请求
		GREATE_SOLDIER_REQ_TYPE_WASH_POTENTIAL = 3,			// 洗练潜能请求，param1是seq
		GREATE_SOLDIER_REQ_TYPE_PUTON = 4,					// 装上将位请求，param1是名将seq，param2是将位槽seq
		GREATE_SOLDIER_REQ_TYPE_PUTOFF = 5,					// 卸下将位请求，param1是将位槽seq
		GREATE_SOLDIER_REQ_TYPE_SLOT_LEVEL_UP = 6,			// 升级将位请求，param1是将位槽seq
		GREATE_SOLDIER_REQ_TYPE_DRAW = 7,					// 抽奖请求，param1是抽奖类型
		GRAETE_SOLDIER_REQ_TYPE_CONFIRM_WASH_POTENTIAL = 8,	// 确认洗练潜能结果，param1是seq
		GRAETE_SOLDIER_REQ_TYPE_WASH_ATTR = 9,				// 洗练属性请求，param1是seq
		GRAETE_SOLDIER_REQ_TYPE_BIANSHEN_TRIAL = 10,		// 变身体验请求，param1是seq
		GRAETE_SOLDIER_REQ_TYPE_FETCH_REWARD = 11,			// 名将领奖， param1是奖励seq
		GRAETE_SOLDIER_REQ_TYPE_FETCH_GOAL_REWARD = 12,		// 领取目标奖励，param1是目标类型
		GRAETE_SOLDIER_REQ_TYPE_BUY_GOAL_REWARD = 13,		// 购买目标奖励，param1是目标类型
		GREATE_SOLDIER_REQ_TYPE_USE_HUANHUA_ID = 14,		// 使用幻化形象，param1是形象ID（传0表示取消）
		GREATE_SOLDIER_REQ_TYPE_EXCHANGE = 15,				// 使用幻化形象，param1是名将seq
		GREATE_SOLDIER_REQ_TYPE_PUTOFF_EQUIPMENT = 16,		// 卸下装备，param1是名将seq，param2是装备槽seq

		GREATE_SOLDIER_REQ_TYPE_MAX,
	};

	// 名将操作请求，8595
	class CSGreateSoldierOpera
	{
	public:
		CSGreateSoldierOpera();
		MessageHeader header;

		UInt16 req_type;
		UInt16 param_1;
		UInt16 param_2;
		UInt16 param_3;
	};

	// 吞噬强化装备	8596
	class CSGreateSoldierReqStrength
	{
	public:
		CSGreateSoldierReqStrength();
		MessageHeader header;

		short seq;																// 名将seq
		short equip_index;														// 要强化的装备下标

		int destroy_num;														// 消耗物品个数
		short destroy_backpack_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];// 消耗的物品所在背白格子下标列表
	};

	// 名将信息，8600
	class SCGreateSoldierItemInfo
	{
	public:
		SCGreateSoldierItemInfo();
		MessageHeader header;
		
		int seq;
		GreateSoldierItemInfo item_info;
	};

	// 名将其他信息，8601
	class SCGreateSoldierOtherInfo
	{
	public:
		SCGreateSoldierOtherInfo();
		MessageHeader header;

		GreateSoldierOtherParam other_param;

		int bianshen_cd_reduce_s;												// 变身CD缩短时间
		int is_bianshen_cd_end;													// 是否变身CD结束通知 （1是 ，0否）

		short seq_exchange_counts[GREATE_SOLDIER_MAX_COUNT];					// 兑换信息
	};

	// 名将将位信息，8602
	class SCGreateSoldierSlotInfo
	{
	public:
		SCGreateSoldierSlotInfo();
		MessageHeader header;

		GreateSoldierSlotParam slot_param;
	};

	// 名将领取阶段奖励，8603
	class SCGreateSoldierFetchReward
	{
	public:
		SCGreateSoldierFetchReward();
		MessageHeader header;

		int draw_times;						//抽奖总次数
		unsigned int fetch_flag;			//抽奖保底奖励，0位开始
	};

	// 名将-目标信息 8604
	class SCGreateSoldierGoalInfo
	{
	public:
		SCGreateSoldierGoalInfo();
		MessageHeader header;

		GreateSoldierGoalData goal_data;
	};

}

#pragma pack(pop)

#endif // __MSG_GREATE_SOLDIER_HPP__