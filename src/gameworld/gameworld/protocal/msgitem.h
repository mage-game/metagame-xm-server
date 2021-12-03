#ifndef MSGITEM_H
#define MSGITEM_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/itemcolddowndef.hpp"
#include "item/gift/giftitem.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	
	/*
		物品相关操作，包括使用物品，装备等
	*/

	class CSKnapsackInfoReq
	{
	public:
		CSKnapsackInfoReq();		// 1550
		MessageHeader		header;
	};

	class SCKnapsackInfoAck
	{
	public:
		SCKnapsackInfoAck();		// 1500
		MessageHeader		header;

		struct InfoType 
		{
			ItemID			item_id;
			short			index;
			short			num;
			char			is_bind;
			char			has_param;		// 有为1，无为0
			UInt32		    invalid_time;
		};

		short				max_knapsack_valid_num;
		short				max_storage_valid_num;
		int					item_count;

		InfoType			item_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	};

	class SCKnapsackItemChange
	{
	public:
		SCKnapsackItemChange();			//1501。1502、1503因为有参数所以都写在最下面
		MessageHeader		header;

		short				change_type;
		short				reason_type;
		short				is_bind;
		short				index;
		ItemID				item_id;
		short				num;
		UInt32				invalid_time;
	};

	class SCUseItemSuc
	{
	public:
		SCUseItemSuc();
		MessageHeader		header;

		UInt16				item_id;
		short				reserve;
	};

	class SCKnapsackMaxGridNum
	{
	public:
		SCKnapsackMaxGridNum();
		MessageHeader		header;

		int					max_grid_num;
		char				is_auto_extend;
		char				reserve_ch;
	};

	class SCStorageMaxGridNum
	{
	public:
		SCStorageMaxGridNum();
		MessageHeader		header;

		int					max_grid_num;
	};

	class SCKnapsackGridExtendAuto		//	1513
	{
	public:
		SCKnapsackGridExtendAuto();
		MessageHeader		header;

		unsigned int online_time;		//	在线时间
		int auto_extend_times;			//	自动扩展数量
	};

	class CSKnapsackGridExtendAuto      // 1514
	{
	public:
		CSKnapsackGridExtendAuto();
		MessageHeader		header;
	};

	class SCLackItem
	{
	public:
		SCLackItem();
		MessageHeader		header;

		int					item_id;
		int					item_count;
	};

	class CSUseItem
	{
	public:
		CSUseItem();				//1551
		MessageHeader		header;

		short				index;
		short				num;
		short				equip_index;
		short				reserve1;
	};

	class CSUseItemMaxNum
	{
	public:
		CSUseItemMaxNum();				//1564
		MessageHeader		header;

		ItemID				item_id;
		short				num;
	};

	class CSMoveItem
	{
	public:
		CSMoveItem();				//1552
		MessageHeader		header;

		short				from_index;
		short				to_index;
	};

	class CSDiscardItem  // 1553
	{
	public:
		CSDiscardItem();
		MessageHeader		header;

		short					index;
		ItemID					item_id_in_client;
		int						item_num_in_client;

		short					reserve_sh;
		short					discard_num;

		int						discard_medthod;
	};
	
	struct DiscardItem
	{
		short					index;
		ItemID					item_id_in_client;
		int						item_num_in_client;
		
		short					reserve_sh;
		short					discard_num;
	
		int						discard_medthod;
	};

	class CSBatchDiscardItem		// 1565
	{
		static const int MAX_BATCH_DISCARD_NUM = 200;
	public:
		CSBatchDiscardItem();
		MessageHeader		header;

		int index_count;

		DiscardItem index_list[MAX_BATCH_DISCARD_NUM];
	};

	class CSKnapsackStoragePutInOrder
	{
	public:
		CSKnapsackStoragePutInOrder();
		MessageHeader		header;

		short				is_storage;		// 整理的是哪个，1为仓库，0为背包
		short				ignore_bind;	// 是否忽略绑定，1为是，0为否
	};


	class CSSplitItem
	{
	public:
		CSSplitItem();
		MessageHeader		header;

		short				index;
		short				num;
	};

	// 物品拾取
	class CSPickItem
	{
	public:
		CSPickItem();
		MessageHeader		header;

		enum
		{
			MAX_PICK_ITEM = 256
		};

		int					count;
		ObjID				obj_id_list[MAX_PICK_ITEM];
	};

	// 物品拾取回应
	class SCPickItemAck
	{
	public:
		SCPickItemAck();
		MessageHeader header;

		enum
		{
			MAX_PICK_TIME_ACK = 256
		};

		int count;
		ObjID obj_id_list[MAX_PICK_TIME_ACK];
	};

	class SCTeamMemberPickItem
	{
	public:
		SCTeamMemberPickItem();
		MessageHeader		header;

		int					role_id;
		GameName			role_name;
		ItemID				item_id;
		short				role_camp;
	};

	class CSItemCompose		
	{
	public:
		enum COMPOSE_TYPE
		{
			COMPOSE_TYPE_COMPOSE = 0,			// 合成
			COMPOSE_TYPE_EXCHANGE,				// 兑换
		};

		CSItemCompose();
		MessageHeader		header;

		int					product_seq;
		short				num;
		short				compose_type;
	};

	class CSBluePrintCompose		
	{
	public:
		CSBluePrintCompose();
		MessageHeader		header;

		UInt16				blueprint_id;
		short				num;
		int					index_count;
		short				stuff_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	};

	class CSTakeOutAllItem
	{
	public:
		CSTakeOutAllItem();
		MessageHeader		header;
	};

	class CSGemstoneBatchCompose	
	{
	public:
		CSGemstoneBatchCompose();
		MessageHeader		header;

		int					end_level;
	};

	
	// 扩展背包/仓库请求
	class CSKnapsackStorageExtendGridNum
	{
	public:
		CSKnapsackStorageExtendGridNum();
		MessageHeader		header;

		short				type;		// 1为仓库，0为背包
		short				extend_num;
		short				can_use_gold;
		short				reserve_sh;
	};

	class SCItemMultiInvalid
	{
	public:
		const static int LIST_MAX_NUM = 16;

		SCItemMultiInvalid();
		MessageHeader		header;

		short				index_type;
		short				count;
		short				index_list[LIST_MAX_NUM];
	};

	class SCRewardListInfo								// 1563奖励列表信息
	{
	public:
		SCRewardListInfo();
		MessageHeader header;

		const static int REWARD_LIST_MAX_NUM = 400;

		enum NOTICE_REWARD_TYPE
		{
			NOTICE_TYPE_INVAILD = 0,

			NOTICE_TYPE_SHENZHOU_WEAPON,
			NOTICE_TYPE_SHENZHOU_WEAPON_OPEN_BOX,
			NOTICE_TYPE_MOVE_CHESS,

			NOTICE_TYPE_MAX,
		};

		struct RewardItem
		{
			RewardItem(): item_id(0), num(0), is_bind(0){}

			int item_id;		
			int num;
			int is_bind;
		};

		int notice_reward_type;
		int mojing;
		int reward_num;
		RewardItem reward_list[REWARD_LIST_MAX_NUM];
	};

	//////////////////和装备参数有关的物品协议///////////////////////////////////////////////////////

	class SCKnapsackItemChangeParam
	{
	public:
		SCKnapsackItemChangeParam();		//1503
		MessageHeader		header;

		short				change_type;
		short				reason_type;
		short				index;
		short				reserve;

		ItemDataWrapper		item_wrapper;
	};


	class SCKnapsackInfoParam
	{
	public:
		SCKnapsackInfoParam();			//1502
		MessageHeader		header;

		struct InfoType 
		{
			short	index;
			short	reserve;

			ItemParamDataStruct	param;
		};

		int count;
		InfoType data[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	};

	class SCItemColddownInfo
	{
	public:
		SCItemColddownInfo();
		MessageHeader		header;

		int					count;
		ItemColddownInfo	cd_info_list[MAX_COLDDOWN_NUM];
	};

	class SCRandGiftItemInfo
	{
	public:
		SCRandGiftItemInfo();
		MessageHeader		header;

		enum GIFT_BAG_TYPE
		{
			GIFT_BAG_TYPE_DEF = 1,   // 固定礼包
			GIFT_BAG_TYPE_RAND = 2,  // 随机礼包
		};

		struct RewardItem
		{
			RewardItem() : item_id(0), num(0), is_bind(0) {}

			int item_id;
			int num;
			int is_bind;
		};

		int gift_type;
		int count;
		RewardItem	item_list[GiftItem::GIFT_ITEM_MAX_NUM];
	};
}

#pragma pack(pop)

#endif





