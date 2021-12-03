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

		MSG_COMMON_BEGIN = 1000,											// 基类使用的通讯号

		MSG_COMMON_ACTIVITY_STATUS_CG,										// 同步活动状态
		MSG_COMMON_FORCE_TO_NEXT_STATUS_GC,									// 强制切换到下一个活动状态
		MSG_COMMON_FORCE_SET_ACTIVITY_TIME_GC,								// 强制更改活动时间

		MSG_COMMON_END = 2000,

		MSG_ACT_BEGIN = 2001,												// 活动自定义通讯号

		MSG_CHONGZHI_RANK_SYNC_DATA_GC,										// 充值排行-同步服务器充值数据
		MSG_CHONGZHI_RANK_CHECK_REWARD_CG,									// 充值排行-通知原服结算

		MSG_CLOUDPURCHASE_SYNC_DATA_GC,										// 一元夺宝同步服务器购买数据
		MSG_CLOUDPURCHASE_SYNC_DATA_CG,										// 一元夺宝同步总次数到全部gameworld
		MSG_CLOUDPURCHASE_GIVE_REWARD_CG,									// 一元夺宝开奖
		MSG_CLOUDPURCHASE_SYCINFO_NOTICE_CG,								// 一元夺宝通知各服同步数据（防止活动期间跨服被重启）
		MSG_CLOUDPURCHASE_SYCINFO_GC,										// 一元夺宝同步全部数据到跨服

		MSG_CONSUME_RANK_SYNC_DATA_GC,										// 消费排行-同步服务器消费数据
		MSG_CONSUME_RANK_CHECK_REWARD_CG,									// 消费排行-通知原服结算

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


	// 以下是每个活动自定义通讯协议 =================================================================
	
	// ================================= 跨服充值榜 =================================================
	struct GCChongzhiRankSyncData
	{
		GCChongzhiRankSyncData() : header(MSG_CHONGZHI_RANK_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// 平台类型
		int merge_server_id;												// 服务器id

		GameName mvp_name;													// 该服充值最高的玩家名字
		int mvp_server_id;													// 玩家原服务器id
		long long total_chongzhi;											// 全服总充值
	};

	struct CGChongzhiRankCheckReward
	{
		CGChongzhiRankCheckReward() : header(MSG_CHONGZHI_RANK_CHECK_REWARD_CG) {}
		MessageHeader	header;

		int rank;															// 服务器所在排名
	};

	// ================================= 一元云购 =================================================
	struct GCCloudPurchaseSyncData
	{
		GCCloudPurchaseSyncData() : header(MSG_CLOUDPURCHASE_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// 平台类型
		int merge_server_id;												// 服务器id

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

	// ================================= 跨服消费榜 =================================================
	struct GCConsumeRankSyncData
	{
		GCConsumeRankSyncData() : header(MSG_CONSUME_RANK_SYNC_DATA_GC) {}
		MessageHeader	header;

		int plat_type;														// 平台类型
		int merge_server_id;												// 服务器id

		GameName mvp_name;													// 该服充值最高的玩家名字
		int mvp_server_id;													// 玩家原服务器id
		long long total_consume;											// 全服总消费
	};

	struct CGConsumeRankCheckReward
	{
		CGConsumeRankCheckReward() : header(MSG_CONSUME_RANK_CHECK_REWARD_CG) {}
		MessageHeader	header;

		int rank;															// 服务器所在排名
	};

}

#pragma pack(pop)

#endif	// __CROSS_GAME_RA_MSG_H__

