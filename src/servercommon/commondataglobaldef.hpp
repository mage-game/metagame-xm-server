
#ifndef __COMMON_DATA_GOLBAL_HPP__
#define __COMMON_DATA_GOLBAL_HPP__

#include "servercommon/struct/itemlistparam.h"

#pragma pack(push) 
#pragma pack(4)

//��Ʒ����----------------------------------
static const int FRIEND_ITEM_GIFT_RECORD_MAX_COUNT = 20;		    // ������Ʒ��¼����
static const int GIVE_ITEM_MAX_COUNT = 7;							// ����������Ʒ��

struct ItemGiftInfo
{
	ItemGiftInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		uid = 0;
		memset(role_name, 0, sizeof(role_name));
		item_id = 0;
		item_num = 0;
		timestamp = 0;
	}

	int uid;
	GameName role_name;
	ItemID item_id;
	unsigned short item_num;
	unsigned int timestamp;
};

struct ItemGiftRecord
{
	ItemGiftRecord()
	{
		this->Reset();
	}

	void Reset()
	{
		give_gift.Reset();
		recieve_gift.Reset();
	}

	ItemGiftInfo give_gift;
	ItemGiftInfo recieve_gift;
};

struct CommonDataGlobalParam
{

	CommonDataGlobalParam()
	{
		this->Reset();
	}

	void Reset()
	{
		sys_consume_gold_in_cross = 0;
		is_chengzhu = 0;
		reserve_ch = 0;
		combine_kill_boss_count = 0;

		memset(gift_record_list, 0, sizeof(gift_record_list));

		cross_challengefield_rank_last_mount_id = 0;
		cross_challengefield_rank_mount_id = 0;
		cross_challengefield_rank_title_id = 0;
		cross_challengefield_rank_last_title_id = 0;
		nex_clear_id_time = 0;
		cross_challengefield_rank = 0;

		reserve_sh = 0;
	}

	int sys_consume_gold_in_cross;					// ��һ���ڿ����ϵͳ���ĵ�Ԫ��
	char is_chengzhu;								// �Ƿ��ǳ���
	char reserve_ch;								// 
	short combine_kill_boss_count;					// �����ɱ�Ϸ�boss��

	ItemGiftRecord gift_record_list[FRIEND_ITEM_GIFT_RECORD_MAX_COUNT];						// ��Ʒ����/�յ���¼

	short cross_challengefield_rank_last_mount_id;		// ������������н�����һ������
	short cross_challengefield_rank_mount_id;			// ������������н�������
	short cross_challengefield_rank_title_id;			// ������������н����ƺ�
	short cross_challengefield_rank_last_title_id;		// ������������н�����һ�γƺ�
	unsigned int nex_clear_id_time;						// �´�������н���ʱ��
	short cross_challengefield_rank;					// �������������
	short reserve_sh;						

};

#pragma pack(pop)

typedef char CommonDataGlobalParamHex[sizeof(CommonDataGlobalParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CommonDataGlobalParamHex) < 4096);

#endif  // __COMMON_DATA_GOLBAL_HPP__