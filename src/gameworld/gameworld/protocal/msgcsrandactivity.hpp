#ifndef __MSG_CROSS_RAND_ACTIVITY_HPP__
#define __MSG_CROSS_RAND_ACTIVITY_HPP__

#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push)
#pragma pack(4)

namespace Protocol
{
	class SCCrossRandActivityStatus		// 8896
	{
	public:
		SCCrossRandActivityStatus();
		MessageHeader header;

		short activity_type;
		short status;
		unsigned int begin_time;
		unsigned int end_time;
	};

	class CSCrossRandActivityRequest										// �������ͳһ����  8895
	{
	public:
		CSCrossRandActivityRequest();
		MessageHeader header;

		short activity_type;
		short opera_type;
		int param_1;
		int param_2;
		int param_3;
	};

	// =========================  ������ÿ����Զ���Э��  ==========================================================

	// =========================  �����ֵ���л  ===================================================================
	enum
	{
		CS_CROSS_RA_CHONGZHI_RANK_REQ_TYPE_INFO = 0,						// �����ڼ���ҳ�ֵ��Ϣ
	};

	class SCCrossRAChongzhiRankChongzhiInfo									// ��ڼ���ҳ�ֵ��Ϣ  8897
	{
	public:
		SCCrossRAChongzhiRankChongzhiInfo();
		MessageHeader header;

		unsigned int total_chongzhi;
	};

	///////////////////////////////////// һԪ�ᱦ //////////////////////////////////////////
	enum RA_CLOUDPURCHASE_OPERA_TYPE
	{
		RA_CLOUDPURCHASE_OPERA_TYPE_INFO = 0,
		RA_CLOUDPURCHASE_OPERA_TYPE_BUY,
		RA_CLOUDPURCHASE_OPERA_TYPE_BUY_RECORD,
		RA_CLOUDPURCHASE_OPERA_TYPE_CONVERT,
		RA_CLOUDPURCHASE_OPERA_TYPE_CONVERT_INFO,
		RA_CLOUDPURCHASE_OPERA_TYPE_SERVER_RECORD_INFO,

		RA_CLOUDPURCHASE_OPERA_TYPE_MAX,
	};

	class SCCloudPurchaseInfo						// 8900
	{
	public:
		SCCloudPurchaseInfo();
		MessageHeader header;

		unsigned int can_buy_timestamp_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];
		ServerPurchaseInfo item_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];		// ��Ʒ�����б���Ϣ
	};

	class SCCloudPurchaseConvertInfo				// 8901		���˶һ���Ϣ
	{
	public:
		SCCloudPurchaseConvertInfo();
		MessageHeader header;

		int score;
		int record_count;
		CloudPurchaseCovertRecord convert_record_list[MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT];
	};

	class SCCloudPurchaseBuyRecordInfo				// 8902
	{
	public:
		SCCloudPurchaseBuyRecordInfo();
		MessageHeader header;

		int record_count;
		CloudPurchaseBuyRecord buy_record_list[MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT];
	};

	class SCCloudPurchaseServerRecord				// 8903
	{
	public:
		SCCloudPurchaseServerRecord();
		MessageHeader header;

		int count;
		ServerPurchaseRecord cloud_reward_record_list[CLOUD_PURCHASE_RECORD_MAX_COUNT];
	};

	class SCCloudPurchaseUserInfo				// 8904 ������Ϣ
	{
	public:
		SCCloudPurchaseUserInfo();
		MessageHeader header;

		int score;							// ӵ�жһ�����
		int ticket_num;						// ӵ��ȯ�� 
	};

	// =========================  ����������л  ===================================================================
	enum
	{
		CS_CROSS_RA_CONSUME_RANK_REQ_TYPE_INFO = 0,							// �����ڼ����������Ϣ
	};

	class SCCrossRAConsumeRankConsumeInfo									// ��ڼ����������Ϣ  8905
	{
	public:
		SCCrossRAConsumeRankConsumeInfo();
		MessageHeader header;

		unsigned int total_consume;
	};

}

#pragma pack(pop)

#endif	// __MSG_CROSS_RAND_ACTIVITY_HPP__



