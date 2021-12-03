#ifndef __REQ_EQUIP_COLLECT_REQ_HANDLER_HPP__
#define __REQ_EQUIP_COLLECT_REQ_HANDLER_HPP__

#include "reqhandler/reqhandlerbase.hpp"
#include "gameworld/item/itemgriddata.h"
#include "config/shenbingtuluconfig.h"

// 红装收集
namespace reqhandler_red_equip_collect
{
	// 穿上
	class TakeOn : public ReqHandlerBase
	{
	protected:
		virtual bool ParamCheck(void *cs);
		virtual bool PreHandleCheck();
		virtual bool DoHandle();

		int m_seq = 0;
		int m_slot = 0;
		ItemDataWrapper *m_item_wrap = 0;	// 红装槽物品
		int m_knapsack_index = 0;
		ItemGridData m_item_data;			// 装备槽物品
	};

	// 领取开服七天奖励
	class FetchActReward : public ReqHandlerBase
	{
	protected:
		virtual bool ParamCheck(void *cs);
		virtual bool PreHandleCheck();
		virtual bool DoHandle();

		int m_seq = 0;
		const RedEquipCollectActConfig::CfgItem *m_reward_cfg = nullptr;
	};

	class RedEquipCollectFetchRewardHandler : public ReqHandlerBase
	{
	protected:
		virtual bool ParamCheck(void *cs);
		virtual bool PreHandleCheck();
		virtual bool DoHandle();

		int m_seq;
	};
}

#endif // __REQ_EQUIP_COLLECT_REQ_HANDLER_HPP__