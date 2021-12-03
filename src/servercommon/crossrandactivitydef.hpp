#ifndef __CROSS_RAND_ACTIVITY_DEF_HPP__
#define __CROSS_RAND_ACTIVITY_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/crossdef.hpp"

//////////////////////////////////////////////// ������� ////////////////////////////////////////////////
enum CROSS_RAND_ACTIVITY_TYPE
{
	CROSS_RAND_ACTIVITY_TYPE_BEGIN = 4000,

	CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK = CROSS_RAND_ACTIVITY_TYPE_BEGIN,		// �����ֵ����
	CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE,										// ���һԪ�ƹ�
	CROSS_RAND_ACTIVITY_TYPE_END1,

	CROSS_RAND_ACTIVITY_TYPE_BEGIN2 = CROSS_RAND_ACTIVITY_TYPE_END1,
	CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK = CROSS_RAND_ACTIVITY_TYPE_BEGIN2,			// �����������

	CROSS_RAND_ACTIVITY_TYPE_END,

};

static const int CROSS_RAND_ACTIVITY_TYPE_PART_ONE_COUNT = CROSS_RAND_ACTIVITY_TYPE_END1 - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
static const int CROSS_RAND_ACTIVITY_TYPE_PART_TWO_COUNT = CROSS_RAND_ACTIVITY_TYPE_END - CROSS_RAND_ACTIVITY_TYPE_BEGIN2;

static const int CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT = CROSS_RAND_ACTIVITY_TYPE_END - CROSS_RAND_ACTIVITY_TYPE_BEGIN;

static const int CROSS_RA_CHONGZHI_RANK_MAX_JOIN_USER_COUNT = 100;			// ��ֵ����-�������Ĳ����������
static const int CROSS_RA_CONSUME_RANK_MAX_JOIN_USER_COUNT = 100;			// ��������-�������Ĳ����������

static const int CLOUD_PURCHASE_ITEM_COUNT_MAX = 32;						// һԪ�ƹ���Ʒ�����
static const int CLOUD_PURCHASE_TICKET_TYPE_COUNT_MAX = 10;					// һԪ����ȯ���������
static const int CLOUD_PURCHASE_MAX_JOIN_USER_COUNT = 50;					// һԪ�ƹ���󱣴�����û�����
static const int CLOUD_PURCHASE_RECORD_MAX_COUNT = 30;						// һԪ�ƹ��н���¼������


/////////////////////////////////////////////////////////////////// ���һԪ�ƹ� ////////////////////////////////////////////////////////////////////

static const int MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT = 32;
static const int MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT = 32;

struct CloudPurchaseCovertRecord
{
	CloudPurchaseCovertRecord()
	{
		this->Reset();
	}

	void Reset()
	{
		item_id = 0;
		convert_count = 0;
	}

	unsigned short item_id;
	short convert_count;
};

struct CloudPurchaseBuyRecord
{
	CloudPurchaseBuyRecord()
	{
		this->Reset();
	}

	void Reset()
	{
		item_id = 0;
		buy_count = 0;
		buy_timestamp = 0;
	}

	ItemID item_id;
	short buy_count;
	unsigned int buy_timestamp;
};

struct ServerPurchaseInfo
{
	ServerPurchaseInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		total_buy_times = 0;				// �ܵĹ������
		give_reward_timestamp = 0;			// ����ʱ��
	}

	int total_buy_times;
	unsigned int give_reward_timestamp;
};

struct ServerPurchaseRecord
{
	ServerPurchaseRecord()
	{
		this->Reset();
	}

	void Reset()
	{
		reward_server_id = 0;
		memset(user_name, 0, sizeof(user_name));

		reward_item_id = 0;
		reserve_sh = 0;
	}

	int reward_server_id;
	GameName user_name;

	ItemID reward_item_id;
	short reserve_sh;
};

struct CloudPurchaseItem
{
	CloudPurchaseItem()
	{
		this->Reset();
	}

	void Reset()
	{
		uid = 0;
		times = 0;
		reserve_sh = 0;
	}

	int uid;
	short times;
	short reserve_sh;
};

struct CloudPurchaseTypeParam
{
	CloudPurchaseTypeParam()
	{
		this->Reset();
	}

	void Reset()
	{
		can_buy_timestamp = 0;
		server_info.Reset();

		for (int i = 0; i < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; i++)
		{
			item_list[i].Reset();
		}
	}

	bool HasUser(int uid)
	{
		if (0 == uid)
		{
			return false;
		}

		for (int i = 0; i < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; i++)
		{
			if (uid == item_list[i].uid)
			{
				return true;
			}
		}

		return false;
	}

	unsigned int can_buy_timestamp;
	ServerPurchaseInfo server_info;
	CloudPurchaseItem item_list[CLOUD_PURCHASE_MAX_JOIN_USER_COUNT];
};

struct CrossCloudPurchaseParam
{
	CrossCloudPurchaseParam()
	{
		this->Reset();
	}

	void Reset()
	{
		record_count = 0;
		record_cover_index = 0;

		for (int i = 0; i < CLOUD_PURCHASE_RECORD_MAX_COUNT; i++)
		{
			ra_cloud_reward_record_list[i].Reset();
		}

		for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i++)
		{
			ra_cloud_purchase_list[i].Reset();
		}
	}

	short record_count;
	short record_cover_index;
	ServerPurchaseRecord ra_cloud_reward_record_list[CLOUD_PURCHASE_RECORD_MAX_COUNT];
	CloudPurchaseTypeParam ra_cloud_purchase_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];
};

struct CrossRandActivityData
{
	static const int DATA_LENGTH = 128;

	CrossRandActivityData()
	{
		memset(data, 0, sizeof(data));
	}

	char data[DATA_LENGTH];
};

struct CrossRandActivityParam // �����������ͨ���ݣ�һ�㶼������
{
	CrossRandActivityParam() { this->Reset(); }

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	CrossRandActivityData data_list[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CrossRandActivityBigParam // ����������Ƚϴ�����ݷ������Ҫ�ż�
{
	CrossRandActivityBigParam() { this->Reset(); }

	void Reset()
	{
		ra_chongzhi_rank_user_count = 0;
		memset(ra_chongzhi_rank_all_uid_list, 0, sizeof(ra_chongzhi_rank_all_uid_list));
	}

	int ra_chongzhi_rank_user_count;
	int ra_chongzhi_rank_all_uid_list[CROSS_RA_CHONGZHI_RANK_MAX_JOIN_USER_COUNT];
};

struct CrossRandActivityBigParam2 // ����������Ƚϴ�����ݷ������Ҫ�ż�
{
	CrossRandActivityBigParam2() { this->Reset(); }

	void Reset()
	{
		ra_consume_rank_user_count = 0;
		memset(ra_consume_rank_all_uid_list, 0, sizeof(ra_consume_rank_all_uid_list));
	}

	int ra_consume_rank_user_count;
	int ra_consume_rank_all_uid_list[CROSS_RA_CONSUME_RANK_MAX_JOIN_USER_COUNT];
};

#endif // __CROSS_RAND_ACTIVITY_DEF_HPP__



