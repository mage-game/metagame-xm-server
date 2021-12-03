#ifndef __RANDACTIVITYCRACYBUY_HPP__
#define __RANDACTIVITYCRACYBUY_HPP__

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"

class RandActivityCracyBuyToAll : public RandActivity
{
public:

	RandActivityCracyBuyToAll(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityCracyBuyToAll();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetLimit(int seq) { ++m_data_param.m_to_all_buy_limit[seq]; }
	int GetLimit(int seq) { return m_data_param.m_to_all_buy_limit[seq]; }

private:
	struct ActivityDataParam
	{
		ActivityDataParam()
		{
			memset(m_to_all_buy_limit, 0, sizeof(m_to_all_buy_limit));
		}

		int m_to_all_buy_limit[RA_MAX_CRACY_BUY_NUM_LIMIT];
	};

	ActivityDataParam m_data_param;
};

#endif