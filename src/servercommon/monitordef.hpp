#ifndef __MONITORDEF_HPP__
#define __MONITORDEF_HPP__

#include "servercommon/servercommon.h"

enum MONITOR_TYPE
{
	MONITOR_TYPE_DAY_GOLD_GET,								// 每日该玩家元宝进账 (非充值手段)
	MONITOR_TYPE_DAY_GOLD_CONSUME,							// 每日该玩家元宝消耗
	MONITOR_TYPE_DAY_NOBIND_ITEM_NUM,						// 每日该玩家非绑定物品进账数目

	MONITOR_TYPE_P2PTRADE_NUM,								// 每日该玩家交易次数
	MONITOR_TYPE_PUBLICSALE_NUM,							// 每日该玩家拍卖次数
	MONITOR_TYPE_SENDMAIL_NUM,								// 每日该玩家发信件次数
	MONITOR_TYPE_FETCH_ATTACHMENT_NUM,						// 每日该玩家收附件
	MONITOR_TYPE_GUILD_STORE_OPER_NUM,						// 每日该玩家操作军团仓库次数
	MONITOR_TYPE_SHOP_BUY_NUM,								// 每日该玩家商城购买次数
	MONITOR_TYPE_CHEST_SHOP_BUY_NUM,						// 每日该玩家祈福次数

	MONITOR_TYPE_DAY_NOBIND_COIN_GET,						// 每日该玩家非绑铜钱进账
	MONITOR_TYPE_DAY_NOBIND_COIN_CONSUME,					// 每日该玩家非绑铜钱消耗

	MONITOR_TYPE_DAY_MYSTERYSHOP_FLUSH_NUM,					// 每日该玩家神秘商店刷新次数
	MONITOR_TYPE_DAY_COMPOSE_NUM,							// 每日该玩家合成物品数量

	MONITOR_TYPE_MAX = 20,
};

static const int MONITOR_LOG_INTERVAL = 1800;				// 监控日志间隔

#pragma pack(push, 4)

struct MonitorParam
{
	MonitorParam() { this->Reset(); }

	void Reset()
	{
		next_monitor_time = 0;
		memset(monitor_num_list, 0, sizeof(monitor_num_list));
	}

	unsigned int next_monitor_time;
	int monitor_num_list[MONITOR_TYPE_MAX];
};

typedef char MonitorParamHex[sizeof(MonitorParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MonitorParamHex) < 256);

#pragma pack(pop)

#endif	// __MONITORDEF_HPP__
