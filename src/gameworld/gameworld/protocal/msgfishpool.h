#ifndef MSGFISHPOOL_H
#define MSGFISHPOOL_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"
#include "gamedef.h"
#include "servercommon/fishpooldef.hpp"
#include "servercommon/guilddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct FishPoolCommonData
	{
		int owner_uid;
		GameName owner_name;
		int role_level;

		int bullet_buy_times;			// �����ӵ�����
		int bullet_buy_num;				// �����ӵ�������
		int bullet_consume_num;			// �����ӵ�������
		
		int today_fang_fish_times;		// �������
		int today_buy_fang_fish_tims;	// �����������
	};

	class SCFishPoolAllRaiseInfo								// ���������Ϣ 2801
	{
	public:
		SCFishPoolAllRaiseInfo();
		MessageHeader header;

		int owner_uid;
		int fish_quality;				// ���Ʒ��
		int fish_num;					// ������
		int fang_fish_time;				// �����ʱ��
	};

	class SCFishPoolCommonInfo									// ��ͨ��Ϣ 2802
	{
	public:
		SCFishPoolCommonInfo();
		MessageHeader header;

		FishPoolCommonData common_data;
	};

	class CSFishPoolBuyBulletReq								// �����ӵ����� 2851
	{
	public:
		CSFishPoolBuyBulletReq();
		MessageHeader header;
	};

	class SCUpFishQualityRet									// ������Ʒ�ʽ�� 2862
	{
	public:
		SCUpFishQualityRet();
		MessageHeader header;

		int quality;
	};

	class CSFishPoolRaiseReq									// ���������� 2850
	{
	public:
		CSFishPoolRaiseReq();
		MessageHeader header;
	};

	enum FISH_POOL_QUERY_TYPE
	{
		FISH_POOL_QUERY_TYPE_ALL_INFO,							// ������Ϣ
		FISH_POOL_QUERY_TYPE_RAISE_INFO,						// ������Ϣ
		FISH_POOL_QUERY_TYPE_WORLD_GENERAL_INFO,				// ������Ҽ�Ҫ��Ϣ
		FISH_POOL_QUERY_TYPE_STEAL_GENERAL_INFO,				// ͵���߼�Ҫ��Ϣ
		FISH_POOL_UP_FISH_QUALITY,								// ����������Ʒ��
		FISH_POOL_BUY_FANG_FISH_TIMES,							// ����������

		FISH_POOL_QUERY_MAX_TYPE,
	};

	class CSFishPoolQueryReq									// ��ѯ��Ϣ����  2852
	{
	public:
		CSFishPoolQueryReq();
		MessageHeader header;

		short query_type;
		short reserve_sh;
		int param;
	};

	class CSFishPoolStealFish									// ͵������ 2853
	{
	public:
		CSFishPoolStealFish();
		MessageHeader header;

		int target_uid;
		char is_fake_pool;
		char quality;
		char type;												// ��ͨ�㣺0  �����㣺1   û���㣺2
		char reserve;
	};

	class SCFishPoolChange										// �������仯 2863
	{
	public:
		SCFishPoolChange();
		MessageHeader header;

		int uid;
		int fish_num;
		int fish_quality;
		int is_steal_succ;										// 0͵��ʧ�ܣ�1͵��ɹ���2������Ϣ
	};

	class CSFishPoolHarvest										// �ջ����� 2854
	{
	public:
		CSFishPoolHarvest();
		MessageHeader header;

	};

	struct FishPoolGeneralInfoItem
	{
		FishPoolGeneralInfoItem() { this->Reset(); }
		void Reset()
		{
			owner_uid = 0;
			memset(owner_name, 0, sizeof(GameName));
			fish_quality = 0;
			fish_num = 0;
			fang_fish_time = 0;
			is_fake_pool = 0;
			is_fuchou = 0;
			be_steal_quality = 0;
			steal_fish_time = 0;
		}

		int owner_uid;
		GameName owner_name;
		int fish_quality;				// ���Ʒ��
		int fish_num;					// ������
		int fang_fish_time;				// �����ʱ��
		int is_fake_pool;				// �Ƿ��Ǽ����
		int is_fuchou;					// �Ƿ��Ѿ�����
		int be_steal_quality;			// ��͵�����Ʒ��
		unsigned int steal_fish_time;	// ͵��ʱ��
	};

	class SCFishPoolWorldGeneralInfo							// ���100����ҵ������Ϣ 2804
	{
	public:
		SCFishPoolWorldGeneralInfo();
		MessageHeader header;

		int info_count;
		FishPoolGeneralInfoItem info_list[FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT];
	};

	class SCFishPoolStealGeneralInfo							// ͵������Ϣ 2805
	{
	public:
		SCFishPoolStealGeneralInfo();
		MessageHeader header;

		int info_count;
		FishPoolGeneralInfoItem info_list[FISH_POOL_BE_TEAL_FISH_UID_MAX];
	};

	class SCFishPoolShouFishRewardInfo							// ���㽱����Ϣ  2810
	{
	public:
		SCFishPoolShouFishRewardInfo();
		MessageHeader header;

		int item_id;
		int item_num;
		long long exp;
		int rune_score;
		int quality;
		char is_skip;											// �Ƿ�һ������0����1��
		char reserve_ch;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif


