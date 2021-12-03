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

	class SCFriendGiftAllInfo				// 8350 ����Ϣ
	{
	public:
		SCFriendGiftAllInfo();
		MessageHeader header;
		
		short song_gift_count;
		short shou_gift_count;

		int shou_gift_list_count;
		FriendGiftGive shou_gift_list[FriendGiftParam::FRIEND_GIFT_LIST_COUNT];
	};
	
	class CSFriendSongGift					// 8351 ����
	{
	public:
		CSFriendSongGift();
		MessageHeader header;
	
		int friend_uid;
		int is_all_return;
		int is_return_gift;	// �����ǣ�1�� �����ǣ�0
	};

	class CSFriendGiftAllInfoReq			// 8352 ����Ϣ����
	{
	public:
		CSFriendGiftAllInfoReq();
		MessageHeader header;
	};

	class SCFriendGiftShouNotice				// 8353 �յ��������ʾ
	{
	public:
		SCFriendGiftShouNotice();
		MessageHeader header;
	};

	enum SC_FRIEND_HELI_REQ_YTPE
	{
		SC_FRIEND_HELI_UPLEVEL_REQ = 0,					// ��������          p1 = level
		SC_FRIEND_HELI_SKILL_BOSS_FETCH_EQUI_REQ,		// ɱboss��ú�װ����p1 = bossid , p2 = װ��id
	};
	
	// �����ͺ�������ѵ�֪ͨ 8354
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

	// ���󷢺��� 8355
	class CSFriendHeliSendReq
	{
	public:
		CSFriendHeliSendReq();
		MessageHeader header;

		int uid;
		int type;
	};

	// ������ 8356
	class SCFriendHeliSend
	{
	public:
		SCFriendHeliSend();
		MessageHeader header;

		int uid;
		int type;
	};

	///////////////////////////// ��Ʒ����////////////////////////////////////////
	enum GIVE_ITEM_OPERA_TYPE
	{
		GIVE_ITEM_OPERA_TYPE_INFO = 0,			// ������Ϣ P1: 1Ϊ���ͼ�¼ 0Ϊ�յ���¼
	};

	// ������Ʒ���� 8357
	class CSGiveItemOpera
	{
	public:
		CSGiveItemOpera();
		MessageHeader header;

		short opera_type;			// ��������
		short param_1;
	};

	// ������Ʒ���� 8358
	class CSGiveItemReq 
	{
	public:
		CSGiveItemReq();
		MessageHeader header;

		struct ItemInfo
		{
			int index = -1;					// ����index
			short num = 0;					// ����
			short reserve_sh = 0;
		};

		int target_uid;													// ���ͶԷ�id
		int item_count;
		ItemInfo give_item_list[GIVE_ITEM_MAX_COUNT];
	};

	// ������Ʒ��¼ 8359
	class SCGiveItemRecord
	{
	public:
		SCGiveItemRecord();
		MessageHeader header;

		char is_give;														// �Ƿ������ͼ�¼
		char reserve_ch;
		short record_count;
		ItemGiftInfo record_list[FRIEND_ITEM_GIFT_RECORD_MAX_COUNT];		// ��¼
	};
}

#pragma pack(pop)

#endif
