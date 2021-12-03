#ifndef __FRIEND_GIFT_PARAM_HPP__
#define __FRIEND_GIFT_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

struct FriendGiftParam
{
	static const int FRIEND_GIFT_MAX_GIFT_COUNT = 50;					// ÿ������������
	static const int FRIEND_GIFT_LIST_COUNT = 30;		                // ���������б���Ϣ����
	static const int FRIEND_GIFT_MAX_SONG_COUNT	 = 1;		            // ��ÿ������ÿ����������

	struct ShouGiftInfo
	{
		ShouGiftInfo():uid(0), shou_gift_time(0), is_return(0){}
		
		int uid;
		unsigned int shou_gift_time;	
		int is_return;					// 0:û�л���1:�Ѿ�����
	};

	FriendGiftParam()
	{
		this->Reset();
	}
	
	void Reset()
	{
		shou_gift_count = 0;
		song_gift_count = 0;
		memset(shou_gift_info_list, 0, sizeof(shou_gift_info_list));
	}

	short shou_gift_count;				// ÿ��������
	short song_gift_count;				// ÿ��������

	ShouGiftInfo shou_gift_info_list[FRIEND_GIFT_LIST_COUNT];
};

#pragma pack(pop)

typedef char FriendGiftParamHex[sizeof(FriendGiftParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FriendGiftParamHex) < 2048);

#endif
