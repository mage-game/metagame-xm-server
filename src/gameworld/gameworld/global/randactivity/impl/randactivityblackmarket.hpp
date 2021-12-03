#ifndef __RAND_ACTIVITY_BLACK_MARKET_HPP__
#define __RAND_ACTIVITY_BLACK_MARKET_HPP__

#include "global/randactivity/randactivity.hpp"

class Role;

class RandActivityBlackMarket : public RandActivity
{
public:
	RandActivityBlackMarket(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityBlackMarket();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void GetItemInfoList(RABlackMarketItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT]);		// 获取拍卖物品列表
	
	bool OnRoleOfferItem(Role *role, int index, int price);		// 玩家竞价，返回竞价是否成功

protected:
	void CheckGiveItem();					// 发放奖励
	void RefreshItemInfoList();				// 刷新拍卖物品列表

protected:
	struct ActivityDataParam
	{
		ActivityDataParam() 
		{
			memset(item_info_list, 0, sizeof(item_info_list));
		}

		RABlackMarketItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT];		// 拍卖物品列表
	};

	ActivityDataParam m_data_param;

};

#endif