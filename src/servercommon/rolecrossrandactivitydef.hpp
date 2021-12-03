#ifndef __ROLE_CROSS_RAND_ACTIVITY_DEF_HPP__
#define __ROLE_CROSS_RAND_ACTIVITY_DEF_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/crossrandactivitydef.hpp"
#include "servercommon/servercommon.h"
#include "serverdef.h"

#pragma pack(push, 4)

struct RoleCrossRandActivityParam
{
	RoleCrossRandActivityParam() { this->Reset(); }

	void Reset()
	{
		chongzhi_rank_join_timestamp = 0;
		chongzhi_rank_chongzhi = 0;

		cloud_purchase_join_timestamp = 0;
		cloud_purchase_score = 0;

		cloud_purchase_record_buy_count = 0;
		cloud_purchase_record_convert_count = 0;

		for (int i = 0; i < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT; ++i)
		{
			cloud_purchase_buy_record_list[i].Reset();
		}

		for (int i = 0; i < MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT; ++i)
		{
			convert_record_list[i].Reset();
		}

		cloud_purchase_ticket_num = 0;

		consume_rank_join_timestamp = 0;
		consume_rank_consume = 0;
	}

	unsigned int chongzhi_rank_join_timestamp;								// ��ֵ���л - �ϴβ����ʱ���
	int chongzhi_rank_chongzhi;												// ��ֵ���л - ��ڼ��ܳ�ֵ���

	unsigned int cloud_purchase_join_timestamp;								// һԪ�ᱦ - �ϴβ����ʱ���
	int cloud_purchase_score;												// һԪ�ᱦ����

	short cloud_purchase_record_buy_count;									// һԪ�ᱦ�����¼��
	short cloud_purchase_record_convert_count;								// һԪ�ᱦ�һ���¼��

	CloudPurchaseBuyRecord cloud_purchase_buy_record_list[MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT];		// һԪ�ᱦ�����¼�б�
	CloudPurchaseCovertRecord convert_record_list[MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT];			// һԪ�ᱦ�һ���¼�б�

	int cloud_purchase_ticket_num;											// һԪ�ᱦȯ����

	unsigned int consume_rank_join_timestamp;								// �������л - �ϴβ����ʱ���
	int consume_rank_consume;												// �������л - ��ڼ������Ѷ��
}; 

typedef char RoleCrossRandActivityParamHex[sizeof(RoleCrossRandActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCrossRandActivityParamHex) < 1024);

#pragma pack(pop)

#endif // __ROLE_CROSS_RAND_ACTIVITY_DEF_HPP__

