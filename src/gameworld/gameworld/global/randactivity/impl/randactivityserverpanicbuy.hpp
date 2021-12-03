#ifndef __RANDACTIVITY_SERVER_PANIC_BUY_HPP__
#define __RANDACTIVITY_SERVER_PANIC_BUY_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityServerPanicBuy : public RandActivity
{
public:
	RandActivityServerPanicBuy(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityServerPanicBuy();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void OnBuyItem(int item_index);
	int GetBuyNum(int item_index);

	void GetBuyItemNumList(int item_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT]);

private:	
	int m_item_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
};

#endif
