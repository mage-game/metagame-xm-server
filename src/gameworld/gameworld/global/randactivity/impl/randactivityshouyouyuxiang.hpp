#ifndef __RANDACTIVITY_SHOUYOU_YUXIANG_HPP__
#define __RANDACTIVITY_SHOUYOU_YUXIANG_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityShouYouYuXiang : public RandActivity
{
public:
	RandActivityShouYouYuXiang(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityShouYouYuXiang();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	int GetTotalFlowerNum() { return m_data_param.total_flower_num; }
	void AddFlowerNum(int add_flower_num); 

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : total_flower_num(0) {}
		
		int total_flower_num;
	};

	ActivityDataParam m_data_param;
};

#endif