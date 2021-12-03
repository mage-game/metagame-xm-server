#ifndef __MSG_SHENSHOU_H__
#define __MSG_SHENSHOU_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenshoudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SHENSHOU_REQ_TYPE
	{
		SHENSHOU_REQ_TYPE_ALL_INFO = 0,											// 请求所有信息,服务端发送2562,2563,2564
		SHENSHOU_REQ_TYPE_PUT_ON,												// 装备， param1 神兽ID, param2 背包格子index，param3 装备槽格子index
		SHENSHOU_REQ_TYPE_TAKE_OFF,												// 卸下， param1 神兽ID, param2 装备槽index
		SHENSHOU_REQ_TYPE_ZHUZHAN,												// 助战， param1 神兽ID，
		SHENSHOU_REQ_TYPE_ADD_ZHUZHAN,											// 扩展神兽助战位
		SHENSHOU_REQ_TYPE_COMPOSE,												// 合成， param_1 物品id ，param_2 背包格子index1 ，param_3 背包格子index2，param_4 背包格子index3
		
		SHENSHOU_REQ_TYPE_HUANLING_INFO,										// 请求唤灵信息,服务器发送2565
		SHENSHOU_REQ_TYPE_HUANLING_REFRESH,										// 唤灵刷新
		SHENSHOU_REQ_TYPE_HUANLING_DRAW,										// 唤灵抽奖
	};

	class CSShenshouOperaReq													// 神兽操作请求  2560
	{
	public:
		CSShenshouOperaReq();
		MessageHeader header;

		short opera_type;														// 详见 SHENSHOU_REQ_TYPE
		short param_1;
		short param_2;
		short param_3;
		int param_4;
	};

	class CSSHenshouReqStrength													// 强化装备	2561
	{
	public:
		CSSHenshouReqStrength();
		MessageHeader header;

		short shenshou_id;														// 神兽id
		short equip_index;														// 要强化的装备下标

		short is_use_double_shuliandu;											// 是否激活双倍熟练度
		short destroy_num;														// 消耗物品个数
		short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT];			// 消耗的物品下标列表
	};

	class SCShenshouBackpackInfo												// 背包信息  2562
	{
	public:
		struct GridItem
		{
			short reserve_sh;
			short index;
			ShenshouBackpackItem iteminfo;
		};

		SCShenshouBackpackInfo();
		MessageHeader header;

		char is_full_backpack;													// 是否全量信息下发
		char reserve;

		short grid_num;															// 格子数量
		GridItem grid_list[SHENSHOU_MAX_BACKPACK_COUNT];						// 格子信息列表
	};

	class SCShenshouListInfo													// 神兽信息  2563
	{
	public:
		struct ShenshouListItem
		{
			short reserve_sh;
			short shou_id;
			ShenshouInfo shou_info;
		};

		SCShenshouListInfo();
		MessageHeader header;

		char is_all_shenshou;													// 是否全量信息下发
		char reserve;

		short shenshou_num;														// 有装备信息的神兽列表
		ShenshouListItem shenshou_list[SHENSHOU_MAX_ID + 1];
	};

	class SCShenshouBaseInfo													// 神兽基本信息  2564 
	{
	public:
		SCShenshouBaseInfo();
		MessageHeader header;

		short extra_zhuzhan_count;												// 额外的助战次数
		short reserve_sh;
	};

	class SCShenshouHuanlingListInfo											// 神兽唤灵刷新列表 2565
	{
	public:
		SCShenshouHuanlingListInfo();
		MessageHeader header;

		int score;																// 抽奖积分
		int cur_draw_times;														// 当前抽奖次数
		HuanlingListInfo huanling_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT];
		UNSTD_STATIC_CHECK(0 == SHENSHOU_MAX_RERFESH_ITEM_COUNT % 2);
	};

	class SCShenshouHuanlingDrawInfo											// 神兽唤灵抽奖结果 2566
	{
	public:
		SCShenshouHuanlingDrawInfo();
		MessageHeader header;

		int score;																// 抽奖积分
		short seq;																// 抽奖结果
		short cur_draw_times;													// 当前抽奖次数
	};
}

#pragma pack(pop)

#endif

