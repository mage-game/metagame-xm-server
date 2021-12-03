#ifndef __MSG_FRIEND_GIFT_H__
#define __MSG_FRIEND_GIFT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/friendgiftparam.hpp"
#include "servercommon/commondataglobaldef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct FriendGiftGive
	{
		GameName name;													
		int uid;							
		int is_return;
		unsigned int shou_gift_time;	
	};

	class SCFriendGiftAllInfo				// 8350 总信息
	{
	public:
		SCFriendGiftAllInfo();
		MessageHeader header;
		
		short song_gift_count;
		short shou_gift_count;

		int shou_gift_list_count;
		FriendGiftGive shou_gift_list[FriendGiftParam::FRIEND_GIFT_LIST_COUNT];
	};
	
	class CSFriendSongGift					// 8351 送礼
	{
	public:
		CSFriendSongGift();
		MessageHeader header;
	
		int friend_uid;
		int is_all_return;
		int is_return_gift;	// 回礼是：1， 送礼是：0
	};

	class CSFriendGiftAllInfoReq			// 8352 总信息请求
	{
	public:
		CSFriendGiftAllInfoReq();
		MessageHeader header;
	};

	class SCFriendGiftShouNotice				// 8353 收到礼物的提示
	{
	public:
		SCFriendGiftShouNotice();
		MessageHeader header;
	};

	enum SC_FRIEND_HELI_REQ_YTPE
	{
		SC_FRIEND_HELI_UPLEVEL_REQ = 0,					// 升级贺礼          p1 = level
		SC_FRIEND_HELI_SKILL_BOSS_FETCH_EQUI_REQ,		// 杀boss获得好装备，p1 = bossid , p2 = 装备id
	};
	
	// 可以送贺礼给好友的通知 8354
	class SCFriendHeliNotice
	{
	public:
		SCFriendHeliNotice();
		MessageHeader header;

		int heli_type;
		
		int uid;
		int param1;
		int param2;
	};

	// 请求发贺礼 8355
	class CSFriendHeliSendReq
	{
	public:
		CSFriendHeliSendReq();
		MessageHeader header;

		int uid;
		int type;
	};

	// 发贺礼 8356
	class SCFriendHeliSend
	{
	public:
		SCFriendHeliSend();
		MessageHeader header;

		int uid;
		int type;
	};

	///////////////////////////// 物品赠送////////////////////////////////////////
	enum GIVE_ITEM_OPERA_TYPE
	{
		GIVE_ITEM_OPERA_TYPE_INFO = 0,			// 请求信息 P1: 1为赠送记录 0为收到记录
	};

	// 赠送物品操作 8357
	class CSGiveItemOpera
	{
	public:
		CSGiveItemOpera();
		MessageHeader header;

		short opera_type;			// 操作类型
		short param_1;
	};

	// 赠送物品请求 8358
	class CSGiveItemReq 
	{
	public:
		CSGiveItemReq();
		MessageHeader header;

		struct ItemInfo
		{
			int index = -1;					// 背包index
			short num = 0;					// 数量
			short reserve_sh = 0;
		};

		int target_uid;													// 赠送对方id
		int item_count;
		ItemInfo give_item_list[GIVE_ITEM_MAX_COUNT];
	};

	// 赠送物品记录 8359
	class SCGiveItemRecord
	{
	public:
		SCGiveItemRecord();
		MessageHeader header;

		char is_give;														// 是否是赠送记录
		char reserve_ch;
		short record_count;
		ItemGiftInfo record_list[FRIEND_ITEM_GIFT_RECORD_MAX_COUNT];		// 记录
	};
}

#pragma pack(pop)

#endif
