#ifndef __CROSS_GAME_RA_MSG_H__
#define __CROSS_GAME_RA_MSG_H__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/crossrandactivitydef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace crossgameramsg
{
	// C - cross, G - gameworld
	enum _MsgType
	{
		MSG_INVALID = 0,

		MSG_COMMON_BEGIN = 1000,											// ����ʹ�õ�ͨѶ��

		MSG_COMMON_ACTIVITY_STATUS_CG,										// ͬ���״̬
		MSG_COMMON_FORCE_TO_NEXT_STATUS_GC,									// ǿ���л�����һ���״̬
		MSG_COMMON_FORCE_SET_ACTIVITY_TIME_GC,								// ǿ�Ƹ��Ļʱ��

		MSG_COMMON_END = 2000,

		MSG_ACT_BEGIN = 2001,												// ��Զ���ͨѶ��

		MSG_CHONGZHI_RANK_SYNC_DATA_GC,										// ��ֵ����-ͬ����������ֵ����
		MSG_CHONGZHI_RANK_CHECK_REWARD_CG,									// ��ֵ����-֪ͨԭ������

		MSG_CLOUDPURCHASE_SYNC_DATA_GC,										// һԪ�ᱦͬ����������������
		MSG_CLOUDPURCHASE_SYNC_DATA_CG,										// һԪ�ᱦͬ���ܴ�����ȫ��gameworld
		MSG_CLOUDPURCHASE_GIVE_REWARD_CG,									// һԪ�ᱦ����
		MSG_CLOUDPURCHASE_SYCINFO_NOTICE_CG,								// һԪ�ᱦ֪ͨ����ͬ�����ݣ���ֹ��ڼ�����������
		MSG_CLOUDPURCHASE_SYCINFO_GC,										// һԪ�ᱦͬ��ȫ�����ݵ����

		MSG_CONSUME_RANK_SYNC_DATA_GC,										// ��������-ͬ����������������
		MSG_CONSUME_RANK_CHECK_REWARD_CG,									// ��������-֪ͨԭ������

		MSG_ACT_END = 10000
 	};

	struct MessageHeader
	{
		MessageHeader(): msg_type(MSG_INVALID) {}
		MessageHeader(int _msg_type): msg_type(_msg_type) {}
		int	msg_type;
	};

	struct CGSyncActivityStatus
	{
		CGSyncActivityStatus() : header(MSG_COMMON_ACTIVITY_STATUS_CG) {}
		MessageHeader	header;

		short is_status_change;
		short activity_status;
		unsigned int begin_time;
		unsigned int end_time;
		unsigned int cross_cur_time;
	};

	struct GCForceToNextStatus
	{
		GCForceToNextStatus() : header(MSG_COMMON_FORCE_TO_NEXT_STATUS_GC) {}
		MessageHeader	header;
	};

	struct GCForceSetActivityTime
	{
		GCForceSetActivityTime() : header(MSG_COMMON_FORCE_SET_ACTIVITY_TIME_GC) {}
		MessageHeader	header;

		unsigned int begin_time;
		unsigned int end_time;
		unsigned int gameworld_cur_time;
	};


	// ������ÿ����Զ���ͨѶЭ�� =================================================================
	
	// ================================= �����ֵ�� =================================================
	struct GCChongzhiRankSyncData
	{
		GCChongzhiRankSyncData() : header(MSG_CHONGZHI_RANK_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// ƽ̨����
		int merge_server_id;												// ������id

		GameName mvp_name;													// �÷���ֵ��ߵ��������
		int mvp_server_id;													// ���ԭ������id
		long long total_chongzhi;											// ȫ���ܳ�ֵ
	};

	struct CGChongzhiRankCheckReward
	{
		CGChongzhiRankCheckReward() : header(MSG_CHONGZHI_RANK_CHECK_REWARD_CG) {}
		MessageHeader	header;

		int rank;															// ��������������
	};

	// ================================= һԪ�ƹ� =================================================
	struct GCCloudPurchaseSyncData
	{
		GCCloudPurchaseSyncData() : header(MSG_CLOUDPURCHASE_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// ƽ̨����
		int merge_server_id;												// ������id

		GameName name;
		int uid;
		int server_id;
		int purchase_type;
		int buy_times;
	};

	struct CGCloudPurchaseTimes
	{
		CGCloudPurchaseTimes() : header(MSG_CLOUDPURCHASE_SYNC_DATA_CG) {}
		MessageHeader	header;

		int purchase_type;
		int times;
		unsigned int givereward_timestamp;
	};

	struct CGCloudPurchaseGiveReward
	{
		CGCloudPurchaseGiveReward() : header(MSG_CLOUDPURCHASE_GIVE_REWARD_CG) {}
		MessageHeader header;

		int type;
		int uid;

		int server_id;
		GameName name;
	};

	struct CGCloudPurchaseSycinfoNotice
	{
		CGCloudPurchaseSycinfoNotice() : header(MSG_CLOUDPURCHASE_SYCINFO_NOTICE_CG) {}
		MessageHeader header;
	};

	struct GCCloudPurchaseSycAllInfo
	{
		GCCloudPurchaseSycAllInfo() : header(MSG_CLOUDPURCHASE_SYCINFO_GC) {}
		MessageHeader header;

		struct CloudPurchaseUserItem
		{
			int uid;
			int times;
			GameName user_name;
			int server_id;
		};

		struct CloudPurchaseTypeInfo
		{
			CloudPurchaseUserItem info_list[CLOUD_PURCHASE_MAX_JOIN_USER_COUNT];
		};

		CloudPurchaseTypeInfo type_info_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];
	};

	// ================================= ������Ѱ� =================================================
	struct GCConsumeRankSyncData
	{
		GCConsumeRankSyncData() : header(MSG_CONSUME_RANK_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// ƽ̨����
		int merge_server_id;												// ������id

		GameName mvp_name;													// �÷���ֵ��ߵ��������
		int mvp_server_id;													// ���ԭ������id
		long long total_consume;											// ȫ��������
	};

	struct CGConsumeRankCheckReward
	{
		CGConsumeRankCheckReward() : header(MSG_CONSUME_RANK_CHECK_REWARD_CG) {}
		MessageHeader	header;

		int rank;															// ��������������
	};

}

#pragma pack(pop)

#endif	// __CROSS_GAME_RA_MSG_H__

