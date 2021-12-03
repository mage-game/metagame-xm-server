#ifndef __RANDACTIVITY_RUSH_BUYING_HPP__
#define __RANDACTIVITY_RUSH_BUYING_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityRushBuying : public RandActivity
{
public:
	RandActivityRushBuying(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityRushBuying();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void Update(unsigned long interval, time_t now_second);

	void GetServerItemBuyTimes(short item_buy_times_list[], int list_len);
	int GetServerItemBuyTimes(int index);

	void AddServerItemBuyTimes(int index);

protected:

private:
	struct ActivityDataParam
	{
		ActivityDataParam() 
		{
			cur_phase = 0;
			reserve_ch = 0;
			memset(server_item_buy_times, 0, sizeof(server_item_buy_times));
		}

		char cur_phase;
		char reserve_ch;
		short server_item_buy_times[RA_RUSH_BUYING_SALES_ITEMS_COUNT];
	};

	ActivityDataParam m_data_param;
};

#endif
