#include "randactivityserverpanicbuy.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/logicconfigmanager.hpp"

RandActivityServerPanicBuy::RandActivityServerPanicBuy(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY)
{
	memset(m_item_buy_numlist, 0, sizeof(m_item_buy_numlist));
}

RandActivityServerPanicBuy::~RandActivityServerPanicBuy()
{
 
}

void RandActivityServerPanicBuy::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_item_buy_numlist) <= sizeof(data.data));

	memcpy(m_item_buy_numlist, data.data, sizeof(m_item_buy_numlist));
}

void RandActivityServerPanicBuy::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_item_buy_numlist) <= sizeof(data->data));

	memcpy(data->data, m_item_buy_numlist, sizeof(m_item_buy_numlist));
}

void RandActivityServerPanicBuy::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		memset(m_item_buy_numlist, 0, sizeof(m_item_buy_numlist));
	}
}

void RandActivityServerPanicBuy::OnBuyItem(int item_index)
{
	if (item_index >= 0 && item_index < RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		++ m_item_buy_numlist[item_index];
	}
}

int RandActivityServerPanicBuy::GetBuyNum(int item_index)
{
	if (item_index >= 0 && item_index < RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return m_item_buy_numlist[item_index];
	}

	return 999999999;
}

void RandActivityServerPanicBuy::GetBuyItemNumList(int item_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT])
{
	memcpy(item_buy_numlist, m_item_buy_numlist, sizeof(m_item_buy_numlist));
}


