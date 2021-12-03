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

	unsigned int chongzhi_rank_join_timestamp;								// 充值排行活动 - 上次参与的时间戳
	int chongzhi_rank_chongzhi;												// 充值排行活动 - 活动期间总充值额度

	unsigned int cloud_purchase_join_timestamp;								// 一元夺宝 - 上次参与的时间戳
	int cloud_purchase_score;												// 一元夺宝积分

	short cloud_purchase_record_buy_count;									// 一元夺宝购买记录数
	short cloud_purchase_record_convert_count;								// 一元夺宝兑换记录数

	CloudPurchaseBuyRecord cloud_purchase_buy_record_list[MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT];		// 一元夺宝购买记录列表
	CloudPurchaseCovertRecord convert_record_list[MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT];			// 一元夺宝兑换记录列表

	int cloud_purchase_ticket_num;											// 一元夺宝券数量

	unsigned int consume_rank_join_timestamp;								// 消费排行活动 - 上次参与的时间戳
	int consume_rank_consume;												// 消费排行活动 - 活动期间总消费额度
}; 

typedef char RoleCrossRandActivityParamHex[sizeof(RoleCrossRandActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleCrossRandActivityParamHex) < 1024);

#pragma pack(pop)

#endif // __ROLE_CROSS_RAND_ACTIVITY_DEF_HPP__

